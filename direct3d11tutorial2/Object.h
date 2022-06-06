#pragma once
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexedTriangleList.h"
#include "PipelineStateObject.h"

class Object {

	friend class Model;

public:

	// Material Data
	struct MaterialData {
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorEmissive;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorAmbient;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorDiffuse;
		struct {
			float r{};
			float g{};
			float b{};
			float a{};
		} colorSpecular;

		float specularPower{};
		int isTextured{ false };
		int padding0{};
		int padding1{};
	};

	// Positional Fields
	struct Position {
		float r{};
		float roll{};
		float pitch{};
		float yaw{};
		float theta{};
		float phi{};
		float chi{};
	};

	// Speed Fields
	struct Speed {
		float droll{};
		float dpitch{};
		float dyaw{};
		float dtheta{};
		float dphi{};
		float dchi{};
	};

private:

	Position mPos{};
	Speed mSpeed{};

	//VertexBuffer mVB{};

	ComPtr<ID3D12PipelineState> mpPipelineState{};

	PipelineStateObject mPSO{};

	// Object Data
	IndexedTriangleList::Object mObject{};
	MaterialData mMaterialData{};

public:

	Object() = default;

	Object(const Object& obj) : mPos(obj.mPos), mSpeed(obj.mSpeed), mObject(obj.mObject),
	mMaterialData(obj.mMaterialData), /*mVB(obj.mVB), */mpPipelineState(obj.mpPipelineState),
	mPSO(obj.mPSO) {}

	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);

	void update(float dt) noexcept;

	DirectX::XMMATRIX getTransformXM() const noexcept;

	Position& getPos() noexcept { return mPos; }

	Speed& getSpeed() noexcept { return mSpeed; }

	MaterialData& getMaterialData() noexcept { return mMaterialData; }

	int onCommand(GraphicsOutput& gfx) noexcept {
		
		return 0;
	}
};