//#pragma once
//#include "GlobalStateObject.h"
//#include "GraphicsOutput.h"
//#include "IndexedTriangleList.h"
//#include "VertexBuffer.h"
//#include "ConstantBuffer.h"
//#include "DataStructures.h"
//#include "Timer.h"
//#include "AssistMath.h"
//#include "ModelFileParsing.h"
//
//// Forward decs
//namespace GID {
//	namespace GSO {
//		namespace Render {
//			GraphicsOutput& mainGFX();
//		}
//	}
//
//	namespace DSU {
//		struct Object {
//
//			//friend class Model;
//
//			Position mPos{};
//			Speed mSpeed{};
//			SphereCollisionCheckData mCollision{};
//
//			VertexBuffer mVertexBuffer{};
//			VertexConstantBuffer mVCB{};
//			PixelConstantBuffer mPCB{};
//
//			// Object Data
//			IndexedObjectFileData mObjectFileData{};
//			MaterialData mMaterialData{};
//			std::vector<VertexData> mObjectData{};
//
//			Timer timer{};
//
//			Object() = default;
//			Object(IndexedObjectFileData& data) {
//				using namespace AssistMath;
//				// Process data
//				mObjectFileData = data;
//				mObjectData.resize(mObjectFileData.pos.size());
//				float highX{ mObjectFileData.pos.at(0).x };
//				float lowX{ mObjectFileData.pos.at(0).x };
//				float highY{ mObjectFileData.pos.at(0).y };
//				float lowY{ mObjectFileData.pos.at(0).y };
//				float highZ{ mObjectFileData.pos.at(0).z };
//				float lowZ{ mObjectFileData.pos.at(0).z };
//				float furthestPoint{ 0.0f };
//				for (int i = 0; i < mObjectFileData.pos.size(); i++) {
//					mObjectData.at(i).pos.x = mObjectFileData.pos.at(i).x;
//					mObjectData.at(i).pos.y = mObjectFileData.pos.at(i).y;
//					mObjectData.at(i).pos.z = mObjectFileData.pos.at(i).z;
//					mObjectData.at(i).texcoord.x = mObjectFileData.tex.at(i).x;
//					mObjectData.at(i).texcoord.y = mObjectFileData.tex.at(i).y;
//					mObjectData.at(i).norm.x = mObjectFileData.norm.at(i).x;
//					mObjectData.at(i).norm.y = mObjectFileData.norm.at(i).y;
//					mObjectData.at(i).norm.z = mObjectFileData.norm.at(i).z;
//
//					if (mObjectData.at(i).pos.x > highX) highX = mObjectData.at(i).pos.x;
//					if (mObjectData.at(i).pos.x < lowX) lowX = mObjectData.at(i).pos.x;
//					if (mObjectData.at(i).pos.x > highY) highY = mObjectData.at(i).pos.y;
//					if (mObjectData.at(i).pos.x < lowY) lowY = mObjectData.at(i).pos.y;
//					if (mObjectData.at(i).pos.x > highZ) highZ = mObjectData.at(i).pos.z;
//					if (mObjectData.at(i).pos.x < lowZ) lowZ = mObjectData.at(i).pos.z;
//
//				}
//
//				mPos.center = { AMLoadFloat3({ (highX + lowX) / 2, (highY + lowY) / 2, (highZ + lowZ) / 2 }) };
//
//				for (int i = 0; i < mObjectFileData.pos.size(); i++) {
//
//					FAMVECTOR vert{ AMLoadFloat3(mObjectFileData.pos.at(i)) };
//
//					FAMVECTOR res{ _mm_sub_ps(AMLoadFloat3(mObjectFileData.pos.at(i)), mPos.center) };
//					res = { _mm_mul_ps(res, res) };
//					float delta = std::sqrt(res.m128_f32[0] + res.m128_f32[1] + res.m128_f32[2]);
//					if (delta > furthestPoint) furthestPoint = delta;
//
//				}
//
//				// Collision
//				//mCollision.center = mPos.center;
//				//mCollision.radius = furthestPoint + 0.1f;
//
//				// Color
//				{
//					// Emissive Color
//					{
//						mMaterialData.colorEmissive.r = 0.0f;
//						mMaterialData.colorEmissive.g = 0.0f;
//						mMaterialData.colorEmissive.b = 0.0f;
//						mMaterialData.colorEmissive.a = 1.0f;
//					}
//					// Ambient Color
//					{
//						mMaterialData.colorAmbient.r = data.mtl.KA.RGB.x;
//						mMaterialData.colorAmbient.g = data.mtl.KA.RGB.y;
//						mMaterialData.colorAmbient.b = data.mtl.KA.RGB.z;
//						mMaterialData.colorAmbient.a = 1.0f;
//					}
//					// Diffuse Color
//					{
//						mMaterialData.colorDiffuse.r = data.mtl.KD.RGB.x;
//						mMaterialData.colorDiffuse.g = data.mtl.KD.RGB.y;
//						mMaterialData.colorDiffuse.b = data.mtl.KD.RGB.z;
//						mMaterialData.colorDiffuse.a = 1.0f;
//					}
//					// Specular Color
//					{
//						mMaterialData.colorSpecular.r = data.mtl.KS.RGB.x;
//						mMaterialData.colorSpecular.g = data.mtl.KS.RGB.y;
//						mMaterialData.colorSpecular.b = data.mtl.KS.RGB.z;
//						mMaterialData.colorSpecular.a = 1.0f;
//					}
//					// Misc
//					{
//						mMaterialData.specularPower = data.mtl.NS;
//						mMaterialData.isTextured = false;
//					}
//				}
//
//				mVertexBuffer = {
//					GSO::Render::mainGFX().getDevice(),
//					GSO::Render::mainGFX().getCommandList(),
//					mObjectData.data(), mObjectData.size() };
//
//			}
//
//			Position& getPos() noexcept { return mPos; }
//			Speed& getSpeed() noexcept { return mSpeed; }
//			MaterialData& getMaterialData() noexcept { return mMaterialData; }
//
//			void update() noexcept {
//				mPos.translation = _mm_add_ps(mPos.translation, mSpeed.deltaTranslation);
//				mPos.rotation = _mm_add_ps(mPos.rotation, mSpeed.deltaRotation);
//				mPos.center = _mm_add_ps(mPos.center, mSpeed.deltaTranslation);
//			}
//			void draw() noexcept {
//				using namespace GSO::Render;
//				FAMMATRIX transformM{ getTransformMx() };
//				FAMVECTOR det{ FAMMatrixDeterminant(transformM) };
//				FAMMATRIX tpose{ FAMMatrixTranspose(transformM) };
//				VertexConstantBufferData matrices{
//					transformM,
//					mainGFX().getCamera().getMatrix(),
//					mainGFX().getProjection(),
//					FAMMatrixInverse(det, tpose)
//				};
//
//				PixelConstantBufferData pcbData{};
//				pcbData.mtl = mMaterialData;
//				pcbData.eyePos = { mainGFX().getCamera().mEye.m128_f32[0], mainGFX().getCamera().mEye.m128_f32[1], mainGFX().getCamera().mEye.m128_f32[2], 1.0f };
//				pcbData.globalAmbient = { 0.f, 0.f, 0.f, 1.f };
//				Light::LightData light0 = {}; {
//					light0.pos = { 20.0f, -35.0f, 20.0f, 1.0f };
//					light0.color = { 1.0f, 1.0f, 1.0f, 1.0f };
//					light0.constAtten = 0.0f;
//					light0.linAtten = 0.1f;
//					light0.quadAtten = 0.0f;
//					light0.isEnabled = true;
//					light0.type = Light::POINT_LIGHT;
//				}
//				Light::LightData light1 = {}; {
//					light1.pos = { -20.0f, -35.0f, -20.0f, 1.0f };
//					light1.color = { 1.0f, 1.0f, 1.0f, 1.0f };
//					light1.constAtten = 0.0f;
//					light1.linAtten = 0.1f;
//					light1.quadAtten = 0.0f;
//					light1.isEnabled = true;
//					light1.type = Light::POINT_LIGHT;
//				}
//				pcbData.lights[0] = light0;
//				pcbData.lights[1] = light1;
//
//				mVCB = { GSO::Render::mainGFX().getDevice(), mainGFX().getCommandList(), matrices };
//				mPCB = { GSO::Render::mainGFX().getDevice(), mainGFX().getCommandList(), pcbData };
//
//				mVertexBuffer.transitionToRead(mainGFX().getCommandList());
//				mVCB.transitionToRead(mainGFX().getCommandList());
//				mPCB.transitionToRead(mainGFX().getCommandList());
//				mainGFX().getCommandList()->SetGraphicsRootConstantBufferView(0u, mVCB.getDestRes()->GetGPUVirtualAddress());
//				mainGFX().getCommandList()->SetGraphicsRootConstantBufferView(1u, mPCB.getDestRes()->GetGPUVirtualAddress());
//				mainGFX().getCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//				mainGFX().getCommandList()->IASetVertexBuffers(0u, 1u, &mVertexBuffer.getView());
//				mainGFX().getCommandList()->DrawInstanced(mVertexBuffer.getCount(), 1u, 0u, 0u);
//
//				mVertexBuffer.transitionToWrite(mainGFX().getCommandList());
//				mVCB.transitionToWrite(mainGFX().getCommandList());
//				mPCB.transitionToWrite(mainGFX().getCommandList());
//			}
//
//			FAMMATRIX getTransformMx() noexcept {
//				return {
//				FAMMatrixRotationRollPitchYaw(mPos.rotation)
//				* FAMMatrixTranslation(mPos.translation)
//				};
//			}
//		};
//	}
//}