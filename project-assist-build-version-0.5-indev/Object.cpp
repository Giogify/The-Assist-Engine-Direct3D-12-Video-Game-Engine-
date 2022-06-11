#include "Object.h"
#include <DirectXMath.h>
#include <memory>

namespace DSU = DataStructsUtil;

Object::Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data) {
	namespace dx = DirectX;

	// Process data
	mObject = *std::make_unique<IndexedTriangleList::Object>(itl_data);
	mObjectData.resize(mObject.pos.size());
	for (int i = 0; i < mObject.pos.size(); i++) {
		mObjectData.at(i).pos.x = mObject.pos.at(i).x;
		mObjectData.at(i).pos.y = mObject.pos.at(i).y;
		mObjectData.at(i).pos.z = mObject.pos.at(i).z;
		mObjectData.at(i).texcoord.x = mObject.tex.at(i).x;
		mObjectData.at(i).texcoord.y = mObject.tex.at(i).y;
		mObjectData.at(i).norm.x = mObject.norm.at(i).x;
		mObjectData.at(i).norm.y = mObject.norm.at(i).y;
		mObjectData.at(i).norm.z = mObject.norm.at(i).z;
	}
	
	// Emissive Color
	{
		mMaterialData.colorEmissive.r = 0.0f;
		mMaterialData.colorEmissive.g = 0.0f;
		mMaterialData.colorEmissive.b = 0.0f;
		mMaterialData.colorEmissive.a = 1.0f;
	}
	// Ambient Color
	{
		mMaterialData.colorAmbient.r = itl_data.mtl.KA_RGB.at(0);
		mMaterialData.colorAmbient.g = itl_data.mtl.KA_RGB.at(1);
		mMaterialData.colorAmbient.b = itl_data.mtl.KA_RGB.at(2);
		mMaterialData.colorAmbient.a = 1.0f;
	}
	// Diffuse Color
	{
		mMaterialData.colorDiffuse.r = itl_data.mtl.KD_RGB.at(0);
		mMaterialData.colorDiffuse.g = itl_data.mtl.KD_RGB.at(1);
		mMaterialData.colorDiffuse.b = itl_data.mtl.KD_RGB.at(2);
		mMaterialData.colorDiffuse.a = 1.0f;
	}
	// Specular Color
	{
		mMaterialData.colorSpecular.r = itl_data.mtl.KS_RGB.at(0);
		mMaterialData.colorSpecular.g = itl_data.mtl.KS_RGB.at(1);
		mMaterialData.colorSpecular.b = itl_data.mtl.KS_RGB.at(2);
		mMaterialData.colorSpecular.a = 1.0f;
	}
	// Misc
	{
		mMaterialData.specularPower = itl_data.mtl.NS_exponent;
		mMaterialData.isTextured = false;
	}

	mVertexBuffer = { gfx.getDevice(), gfx.getCommandList(), mObjectData.data(), mObjectData.size() };

}

void Object::update(float dt) noexcept {
	
}
void Object::draw(GraphicsOutput& gfx) noexcept {

	DX::XMMATRIX transformM{ 
		DirectX::XMMatrixRotationRollPitchYaw(mPos.pitch, mPos.yaw, mPos.roll)
		* DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.y)
		* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f) 
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f) };
	auto pDeterminant = std::make_unique<DirectX::XMVECTOR>(DirectX::XMMatrixDeterminant(transformM));
	DSU::VertexConstantBuffer matrices{ 
		transformM,
		gfx.getCamera().getMatrix(),
		gfx.getProjection(),
		DirectX::XMMatrixInverse(pDeterminant.get(),
		DirectX::XMMatrixTranspose(transformM)) };

	/*DSU::PixelConstantBuffer pcbData{};
	pcbData.mtl = mMaterialData;
	pcbData.eyePos = DirectX::XMFLOAT4(gfx.getCamera().mEye.x, gfx.getCamera().mEye.y, gfx.getCamera().mEye.z, 1.0f);
	pcbData.globalAmbient = { 0.f, 0.f, 0.f, 1.f };
	Light::LightData light = {}; {
		light.pos = { 0.0f, 0.0f, 0.0f, 1.0f };
		light.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		light.constAtten = 0.0f;
		light.linAtten = 0.1f;
		light.quadAtten = 0.0f;
		light.isEnabled = true;
		light.type = Light::POINT_LIGHT;
	}
	pcbData.lights[0] = light;*/

	mVCB = { gfx.getDevice(), gfx.getCommandList(), matrices };
	//mPCB = { gfx.getDevice(), gfx.getCommandList(), pcbData };

	mVertexBuffer.transitionToRead(gfx.getCommandList());
	mVCB.transitionToRead(gfx.getCommandList());
	//mPCB.transitionToRead(gfx.getCommandList());
	gfx.getCommandList()->SetGraphicsRootConstantBufferView(0u, mVCB.getDestRes()->GetGPUVirtualAddress());
	gfx.getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx.getCommandList()->IASetVertexBuffers(0u, 1u, &mVertexBuffer.getView());
	gfx.getCommandList()->DrawInstanced(mVertexBuffer.getCount(), 1u, 0u, 0u);

	mVertexBuffer.transitionToWrite(gfx.getCommandList());
	mVCB.transitionToWrite(gfx.getCommandList());
}

DirectX::XMMATRIX Object::getTransformXM() const noexcept {
	return DirectX::XMMatrixRotationRollPitchYaw(mPos.pitch, mPos.yaw, mPos.roll)
		* DirectX::XMMatrixTranslation(mPos.x, mPos.y, mPos.z)
		* DirectX::XMMatrixRotationRollPitchYaw(0.0f, 0.0f, 0.0f)
		* DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);
}