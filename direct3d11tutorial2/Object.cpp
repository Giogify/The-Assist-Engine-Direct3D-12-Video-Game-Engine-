#include "Object.h"
#include "DataStructures.h"
#include <DirectXMath.h>
#include <memory>

namespace DSU = DataStructsUtil;

Object::Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data) {
	namespace dx = DirectX;

	// Process data
	mObject = *std::make_unique<IndexedTriangleList::Object>(itl_data);
	std::vector<DSU::VertexData> ObjectData{};
	ObjectData.resize(mObject.pos.size());
	for (int i = 0; i < mObject.pos.size(); i++) {
		ObjectData.at(i).pos.x = mObject.pos.at(i).x;
		ObjectData.at(i).pos.y = mObject.pos.at(i).y;
		//ObjectData.at(i).pos.z = mObject.pos.at(i).z;
		/*ObjectData.at(i).texcoord.x = mObject.tex.at(i).x;
		ObjectData.at(i).texcoord.y = mObject.tex.at(i).y;
		ObjectData.at(i).norm.x = mObject.norm.at(i).x;
		ObjectData.at(i).norm.y = mObject.norm.at(i).y;
		ObjectData.at(i).norm.z = mObject.norm.at(i).z;*/
	}

	mMaterialData.colorEmissive.r = 0.0f;
	mMaterialData.colorEmissive.g = 0.0f;
	mMaterialData.colorEmissive.b = 0.0f;
	mMaterialData.colorEmissive.a = 1.0f;

	mMaterialData.colorAmbient.r = itl_data.mtl.KA_RGB.at(0);
	mMaterialData.colorAmbient.g = itl_data.mtl.KA_RGB.at(1);
	mMaterialData.colorAmbient.b = itl_data.mtl.KA_RGB.at(2);
	mMaterialData.colorAmbient.a = 1.0f;

	mMaterialData.colorDiffuse.r = itl_data.mtl.KD_RGB.at(0);
	mMaterialData.colorDiffuse.g = itl_data.mtl.KD_RGB.at(1);
	mMaterialData.colorDiffuse.b = itl_data.mtl.KD_RGB.at(2);
	mMaterialData.colorDiffuse.a = 1.0f;

	mMaterialData.colorSpecular.r = itl_data.mtl.KS_RGB.at(0);
	mMaterialData.colorSpecular.g = itl_data.mtl.KS_RGB.at(1);
	mMaterialData.colorSpecular.b = itl_data.mtl.KS_RGB.at(2);
	mMaterialData.colorSpecular.a = 1.0f;

	mMaterialData.specularPower = itl_data.mtl.NS_exponent;
	mMaterialData.isTextured = false;

	// Bind object

}

void Object::update(float dt) noexcept {

}

DirectX::XMMATRIX Object::getTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(mPos.pitch, mPos.yaw, mPos.roll)
		* DirectX::XMMatrixTranslation(mPos.r, 0.0f, 0.0f)
		* DirectX::XMMatrixRotationRollPitchYaw(mPos.theta, mPos.phi, mPos.chi)
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}