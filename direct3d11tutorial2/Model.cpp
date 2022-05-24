#include "Model.h"
#include "CustomGeo.h"
#include "Collections.h"
#include "MaterialParser.h"

Model::Model(GraphicsOutput& gfx, std::string& objPath) {
	namespace dx = DirectX;

	bool found{ false };
	for (auto& itl : ITL_Collection::getCollection()) {
		if (itl.first == objPath) m_ModelData = *std::make_unique<IndexedTriangleList>(itl.second);
		found = true;
	}
	if (!found) {
		m_ModelData = *std::make_unique<IndexedTriangleList>(CustomGeo::make(objPath, *std::make_unique<std::vector<Material>>(MaterialParser::make(objPath))));
		ITL_Collection::add(objPath, m_ModelData);
	}
	for (int i = 0; i < m_ModelData.m_objects.size(); i++) {
		Object object = { gfx, m_ModelData.m_objects.at(i) };
		m_objects.push_back(object);
	}
}

void Model::update() noexcept {

}

void Model::draw(GraphicsOutput& gfx) noexcept { 
	for (auto& o : m_objects) {
		auto test = 1;
		auto test2 = 2;
		o.draw(gfx);
	}
}