//#include "Object.h"
//#include <memory>
//
//using namespace GID;
//
//Object::Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data) {
//	using namespace AssistMath;
//	// Process data
//	mObject = *std::make_unique<IndexedTriangleList::Object>(itl_data);
//	mObjectData.resize(mObject.pos.size());
//	float highX{ mObject.pos.at(0).x };
//	float lowX{ mObject.pos.at(0).x };
//	float highY{ mObject.pos.at(0).y };
//	float lowY{ mObject.pos.at(0).y };
//	float highZ{ mObject.pos.at(0).z };
//	float lowZ{ mObject.pos.at(0).z };
//	float furthestPoint{ 0.0f };
//	for (int i = 0; i < mObject.pos.size(); i++) {
//		mObjectData.at(i).pos.x = mObject.pos.at(i).x;
//		mObjectData.at(i).pos.y = mObject.pos.at(i).y;
//		mObjectData.at(i).pos.z = mObject.pos.at(i).z;
//		mObjectData.at(i).texcoord.x = mObject.tex.at(i).x;
//		mObjectData.at(i).texcoord.y = mObject.tex.at(i).y;
//		mObjectData.at(i).norm.x = mObject.norm.at(i).x;
//		mObjectData.at(i).norm.y = mObject.norm.at(i).y;
//		mObjectData.at(i).norm.z = mObject.norm.at(i).z;
//
//		if (mObjectData.at(i).pos.x > highX) highX = mObjectData.at(i).pos.x;
//		if (mObjectData.at(i).pos.x < lowX) lowX = mObjectData.at(i).pos.x;
//		if (mObjectData.at(i).pos.x > highY) highY = mObjectData.at(i).pos.y;
//		if (mObjectData.at(i).pos.x < lowY) lowY = mObjectData.at(i).pos.y;
//		if (mObjectData.at(i).pos.x > highZ) highZ = mObjectData.at(i).pos.z;
//		if (mObjectData.at(i).pos.x < lowZ) lowZ = mObjectData.at(i).pos.z;
//
//	}
//
//	mPos.center = { AMLoadFloat3({ (highX + lowX) / 2, (highY + lowY) / 2, (highZ + lowZ) / 2 }) };
//
//	for (int i = 0; i < mObject.pos.size(); i++) {
//
//		FAMVECTOR vert{ AMLoadFloat3(mObject.pos.at(i)) };
//
//		FAMVECTOR res{ _mm_sub_ps(AMLoadFloat3(mObject.pos.at(i)), mPos.center) };
//		res = { _mm_mul_ps(res, res) };
//		float delta = std::sqrt(res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2]);
//		if (delta > furthestPoint) furthestPoint = delta;
//
//	}
//	
//	// Collision
//	//mCollision.center = mPos.center;
//	//mCollision.radius = furthestPoint + 0.1f;
//
//	// Color
//	{
//		// Emissive Color
//		{
//			mMaterialData.colorEmissive.r = 0.0f;
//			mMaterialData.colorEmissive.g = 0.0f;
//			mMaterialData.colorEmissive.b = 0.0f;
//			mMaterialData.colorEmissive.a = 1.0f;
//		}
//		// Ambient Color
//		{
//			mMaterialData.colorAmbient.r = itl_data.mtl.KA_RGB.at(0);
//			mMaterialData.colorAmbient.g = itl_data.mtl.KA_RGB.at(1);
//			mMaterialData.colorAmbient.b = itl_data.mtl.KA_RGB.at(2);
//			mMaterialData.colorAmbient.a = 1.0f;
//		}
//		// Diffuse Color
//		{
//			mMaterialData.colorDiffuse.r = itl_data.mtl.KD_RGB.at(0);
//			mMaterialData.colorDiffuse.g = itl_data.mtl.KD_RGB.at(1);
//			mMaterialData.colorDiffuse.b = itl_data.mtl.KD_RGB.at(2);
//			mMaterialData.colorDiffuse.a = 1.0f;
//		}
//		// Specular Color
//		{
//			mMaterialData.colorSpecular.r = itl_data.mtl.KS_RGB.at(0);
//			mMaterialData.colorSpecular.g = itl_data.mtl.KS_RGB.at(1);
//			mMaterialData.colorSpecular.b = itl_data.mtl.KS_RGB.at(2);
//			mMaterialData.colorSpecular.a = 1.0f;
//		}
//		// Misc
//		{
//			mMaterialData.specularPower = itl_data.mtl.NS_exponent;
//			mMaterialData.isTextured = false;
//		}
//	}
//
//	mVertexBuffer = { gfx.getDevice(), gfx.getCommandList(), mObjectData.data(), mObjectData.size() };
//
//}
//
//void Object::update() noexcept {
//	mPos.translation = _mm_add_ps(mPos.translation, mSpeed.deltaTranslation);
//	mPos.rotation = _mm_add_ps(mPos.rotation, mSpeed.deltaRotation);
//	mPos.center = _mm_add_ps(mPos.center, mSpeed.deltaTranslation);
//}
//void Object::draw(GraphicsOutput& gfx) noexcept {
//
//	FAMMATRIX transformM{ getTransformMx() };
//	FAMVECTOR det{ FAMMatrixDeterminant(transformM) };
//	FAMMATRIX tpose{ FAMMatrixTranspose(transformM) };
//	DSU::VertexConstantBuffer matrices{
//		transformM,
//		gfx.getCamera().getMatrix(),
//		gfx.getProjection(),
//		FAMMatrixInverse(det, tpose) 
//	};
//
//	DSU::PixelConstantBuffer pcbData{};
//	pcbData.mtl = mMaterialData;
//	pcbData.eyePos = { gfx.getCamera().mEye.m128_f32[0], gfx.getCamera().mEye.m128_f32[1], gfx.getCamera().mEye.m128_f32[2], 1.0f};
//	pcbData.globalAmbient = { 0.f, 0.f, 0.f, 1.f };
//	Light::LightData light0 = {}; {
//		light0.pos = { 20.0f, -35.0f, 20.0f, 1.0f };
//		light0.color = { 1.0f, 1.0f, 1.0f, 1.0f };
//		light0.constAtten = 0.0f;
//		light0.linAtten = 0.1f;
//		light0.quadAtten = 0.0f;
//		light0.isEnabled = true;
//		light0.type = Light::POINT_LIGHT;
//	}
//	Light::LightData light1 = {}; {
//		light1.pos = { -20.0f, -35.0f, -20.0f, 1.0f };
//		light1.color = { 1.0f, 1.0f, 1.0f, 1.0f };
//		light1.constAtten = 0.0f;
//		light1.linAtten = 0.1f;
//		light1.quadAtten = 0.0f;
//		light1.isEnabled = true;
//		light1.type = Light::POINT_LIGHT;
//	}
//	pcbData.lights[0] = light0;
//	pcbData.lights[1] = light1;
//
//	mVCB = { gfx.getDevice(), gfx.getCommandList(), matrices };
//	mPCB = { gfx.getDevice(), gfx.getCommandList(), pcbData };
//
//	mVertexBuffer.transitionToRead(gfx.getCommandList());
//	mVCB.transitionToRead(gfx.getCommandList());
//	mPCB.transitionToRead(gfx.getCommandList());
//	gfx.getCommandList()->SetGraphicsRootConstantBufferView(0u, mVCB.getDestRes()->GetGPUVirtualAddress());
//	gfx.getCommandList()->SetGraphicsRootConstantBufferView(1u, mPCB.getDestRes()->GetGPUVirtualAddress());
//	gfx.getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//	gfx.getCommandList()->IASetVertexBuffers(0u, 1u, &mVertexBuffer.getView());
//	gfx.getCommandList()->DrawInstanced(mVertexBuffer.getCount(), 1u, 0u, 0u);
//
//	mVertexBuffer.transitionToWrite(gfx.getCommandList());
//	mVCB.transitionToWrite(gfx.getCommandList());
//	mPCB.transitionToWrite(gfx.getCommandList());
//}
//
//FAMMATRIX Object::getTransformMx() noexcept {
//	return {
//		FAMMatrixRotationRollPitchYaw(mPos.rotation)
//		* FAMMatrixTranslation(mPos.translation)
//	};
//}