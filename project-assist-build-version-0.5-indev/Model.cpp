#include "Model.h"
#include "CustomGeo.h"
#include "Collections.h"
#include "MaterialParser.h"

Model::Model(GraphicsOutput& gfx, std::string& objPath) {
	namespace dx = DirectX;

	bool found{ false };
	for (auto& itl : ITL_Collection::getCollection()) {
		if (itl.first == objPath) mITLData = *std::make_unique<IndexedTriangleList>(itl.second);
		found = true;
	}
	if (!found) {
		mITLData = *std::make_unique<IndexedTriangleList>(CustomGeo::make(objPath, *std::make_unique<std::vector<Material>>(MaterialParser::make(objPath))));
		ITL_Collection::add(objPath, mITLData);
	}
	for (int i = 0; i < mITLData.m_objects.size(); i++) {
		Object object = { gfx, mITLData.m_objects.at(i) };
		mObjects.push_back(object);
	}
}

void Model::update() noexcept {

}

int Model::onCommand(GraphicsOutput& gfx) noexcept {
	for (auto& o : mObjects) o.draw(gfx);
	return 0;
}