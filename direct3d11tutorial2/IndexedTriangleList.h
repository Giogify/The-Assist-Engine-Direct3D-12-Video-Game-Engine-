#pragma once
#include <vector>
#include <memory>
#include <string>
#include <DirectXMath.h>

class IndexedTriangleList {

public:

	struct Object {
		std::string name{};
		std::vector<DirectX::XMFLOAT3> pos{};
		std::vector<DirectX::XMFLOAT2> tex{};
		std::vector<DirectX::XMFLOAT3> norm{};
	};

public: // Public Fields

	std::vector<std::string> m_names{};
	std::vector<DirectX::XMFLOAT3> m_pos{};
	std::vector<DirectX::XMFLOAT2> m_tex{};
	std::vector<DirectX::XMFLOAT3> m_norm{};
	std::vector<DirectX::XMFLOAT3X3> m_indices{};
	std::vector<unsigned int> m_objectIndexTable{};

	std::vector<Object> m_objects{};

public: // Public Methods

	// Constructors
	IndexedTriangleList() = default;
	IndexedTriangleList(const IndexedTriangleList& itl) :
	m_names(itl.m_names),
	m_pos(itl.m_pos),
	m_tex(itl.m_tex),
	m_norm(itl.m_norm),
	m_indices(itl.m_indices),
	m_objectIndexTable(itl.m_objectIndexTable),
	m_objects(itl.m_objects) {}
	
	IndexedTriangleList(IndexedTriangleList* pitl) :
		m_names(pitl->m_names),
		m_pos(pitl->m_pos),
		m_tex(pitl->m_tex),
		m_norm(pitl->m_norm),
		m_indices(pitl->m_indices),
		m_objectIndexTable(pitl->m_objectIndexTable),
		m_objects(pitl->m_objects) {}
	
	IndexedTriangleList(std::vector<std::string> names,
		std::vector<DirectX::XMFLOAT3> pos,
		std::vector<DirectX::XMFLOAT2> tex,
		std::vector<DirectX::XMFLOAT3> norm,
		std::vector<DirectX::XMFLOAT3X3> indices,
		std::vector<unsigned int> objectIndexTable) :
		m_names(std::move(names)),
		m_pos(std::move(pos)),
		m_tex(std::move(tex)),
		m_norm(std::move(norm)),
		m_indices(std::move(indices)),
		m_objectIndexTable(std::move(objectIndexTable)) {
		//for (int i = 0; i < m_objects.size(); i++) {
		//	for (int j = 0; j < 3; j++) {
		//		//auto it = indices.insert(indices.begin(), 3);
		//		indices.push_back(*std::make_unique<unsigned short>(faces.at(i).m[j][0]));
		//	}
		//}

		for (int i = 0; i < m_objectIndexTable.size() - 1; i++) {
			
			Object objectTemp{};
			objectTemp.name = m_names.at(i);
			
			for (int j = m_objectIndexTable.at(i); j < m_objectIndexTable.at(i + 1); j++) {
				for (int k = 0; k < 3; k++) {
					
					if (!(m_indices.at(j).m[k][0] < 0.0f)) {
						objectTemp.pos.push_back(std::move(m_pos.at(m_indices.at(j).m[k][0])));
					}
					else {
						objectTemp.pos.push_back(*std::make_unique<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f));
					}
					
					if (!(m_indices.at(j).m[k][1] < 0.0f)) {
						objectTemp.tex.push_back(std::move(m_tex.at(m_indices.at(j).m[k][1])));
					}
					else {
						objectTemp.tex.push_back(*std::make_unique<DirectX::XMFLOAT2>(0.0f, 0.0f));
					}

					if (!(m_indices.at(j).m[k][2] < 0.0f)) {
						objectTemp.norm.push_back(std::move(m_norm.at(m_indices.at(j).m[k][2])));
					}
					else {
						objectTemp.norm.push_back(*std::make_unique<DirectX::XMFLOAT3>(0.0f, 0.0f, 0.0f));
					}
				}
			}
			m_objects.push_back(std::move(objectTemp));
		}

		/*for (int i = 0; i < m_objects.size(); i++) {
			Object objectTemp{};
			objectTemp.name = m_objects.at(i).name;
			objectTemp.indices = m_objects.at(i).indices;
			for (int j = 0; j < m_objects.at(i).indices.size(); j++) {
				for (int k = 0; k < 3; k++) {
					objectTemp.pos.push_back(*std::make_unique<DirectX::XMFLOAT3>(m_objects.at(i).pos.at(m_objects.at(i).indices.at(j).m[k][0])));
					objectTemp.tex.push_back(*std::make_unique<DirectX::XMFLOAT2>(m_objects.at(i).tex.at(m_objects.at(i).indices.at(j).m[k][1])));
					objectTemp.norm.push_back(*std::make_unique<DirectX::XMFLOAT3>(m_objects.at(i).norm.at(m_objects.at(i).indices.at(j).m[k][2])));
				}
			}
			m_objects.at(i).pos.resize(objectTemp.pos.size());
			m_objects.at(i).tex.resize(objectTemp.tex.size());
			m_objects.at(i).norm.resize(objectTemp.norm.size());
			m_objects.at(i) = objectTemp;
		}*/
	}

	void transform(DirectX::FXMMATRIX matrix) {
		for (int i = 0; i < m_objects.size(); i++) {
			for (auto& v : m_objects.at(i).pos) {
				const DirectX::XMVECTOR pos = DirectX::XMLoadFloat3(&v);
				DirectX::XMStoreFloat3(&v, DirectX::XMVector3Transform(pos, matrix));
			}
		}
	}

};