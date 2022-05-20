#include "Model.h"
#include "CustomGeo.h"
#include "Collections.h"

Model::Model(GraphicsOutput& gfx, std::string& objPath) {
	namespace dx = DirectX;

	bool found = false;
	for (auto& itl : ITL_Collection::getCollection()) {
		if (itl.first == objPath) {
			m_ModelData = itl.second;
			found = true;
		}
	}

	if (!found) {
		m_ModelData = std::make_shared<IndexedTriangleList>(CustomGeo::make(objPath));
		ITL_Collection::add(objPath, m_ModelData.get());
	}

	//m_ModelData = std::make_shared<IndexedTriangleList>(CustomGeo::make(objPath));

	for (int i = 0; i < m_ModelData->m_objects.size(); i++) {
		m_objects.push_back(std::make_unique<Object>(gfx, m_ModelData->m_objects.at(i)));
	}
}

void Model::update() noexcept {

}

void Model::draw(GraphicsOutput& gfx) const noexcept {
	for (auto& o : m_objects) {
		o->draw(gfx);
	}
}