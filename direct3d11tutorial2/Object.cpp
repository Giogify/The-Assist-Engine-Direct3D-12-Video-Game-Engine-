#include "Object.h"
#include "IndexedTriangleList.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "InputLayout.h"
#include "Topology.h"
#include "TransformCBuffer.h"
#include <DirectXMath.h>
#include <memory>

Object::Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data) {
	namespace dx = DirectX;

	m_object = std::make_unique<IndexedTriangleList::Object>(itl_data);

	std::vector<VertexData> ObjectData{};
	ObjectData.resize(m_object->pos.size());
	for (int i = 0; i < m_object->pos.size(); i++) {
		ObjectData.at(i).pos.x = m_object->pos.at(i).x;
		ObjectData.at(i).pos.y = m_object->pos.at(i).y;
		ObjectData.at(i).pos.z = m_object->pos.at(i).z;
		ObjectData.at(i).texcoord.x = m_object->tex.at(i).x;
		ObjectData.at(i).texcoord.y = m_object->tex.at(i).y;
		ObjectData.at(i).norm.x = m_object->norm.at(i).x;
		ObjectData.at(i).norm.y = m_object->norm.at(i).y;
		ObjectData.at(i).norm.z = m_object->norm.at(i).z;
		ObjectData.at(i).color.r = (m_object->norm.at(i).x + 1.0f) / (2.0f);
		ObjectData.at(i).color.g = (m_object->norm.at(i).y + 1.0f) / (2.0f);
		ObjectData.at(i).color.b = (m_object->norm.at(i).z + 1.0f) / (2.0f);
		ObjectData.at(i).color.a = 0.5f;
	}

	addVertexBuffer(std::make_unique<VertexBuffer>(gfx, ObjectData.data(), ObjectData.size()));

	auto pvs = std::make_unique<VertexShader>(gfx, L"VertexShader.cso");
	auto pvsbc = pvs->getBytecode();
	addBind(std::move(pvs));

	addBind(std::make_unique<PixelShader>(gfx, L"PixelShader.cso"));

	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	addBind(std::make_unique<InputLayout>(gfx, ied, pvsbc));
	addBind(std::make_unique<Topology>(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	addBind(std::make_unique<TransformCBuffer>(gfx, *this));
}

void Object::update(float dt) noexcept {

}

DirectX::XMMATRIX Object::getTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(pos.m_pitch, 0.0f, pos.m_roll)
		* DirectX::XMMatrixTranslation(pos.m_r, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(pos.m_theta, pos.m_phi, pos.m_chi)
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}