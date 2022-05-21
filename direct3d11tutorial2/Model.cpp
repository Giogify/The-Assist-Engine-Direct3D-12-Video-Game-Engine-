#include "Model.h"
#include "CustomGeo.h"
#include "Collections.h"
#include "MaterialParser.h"

Model::Model(GraphicsOutput& gfx, std::string& objPath) {
	namespace dx = DirectX;

	for (auto& itl : ITL_Collection::getCollection()) if (itl.first == objPath) m_ModelData = std::make_unique<IndexedTriangleList>(itl.second);

	if (m_ModelData == nullptr) {
		m_ModelData = std::make_unique<IndexedTriangleList>(CustomGeo::make(objPath, *std::make_unique<std::vector<Material>>(MaterialParser::make(objPath))));
		ITL_Collection::add(objPath, m_ModelData.get());
	}

	//m_ModelData = std::make_shared<IndexedTriangleList>(CustomGeo::make(objPath));

	for (int i = 0; i < m_ModelData->m_objects.size(); i++) m_objects.push_back(std::make_unique<Object>(gfx, m_ModelData->m_objects.at(i)));
}

void Model::update() noexcept {

}

void Model::draw(GraphicsOutput& gfx) const noexcept { for (auto& o : m_objects) o->draw(gfx); }