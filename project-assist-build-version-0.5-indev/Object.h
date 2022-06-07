#pragma once
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "IndexedTriangleList.h"
#include "PipelineStateObject.h"
#include "DataStructures.h"

class Object {

	friend class Model;

private:

	DSU::Position mPos{};
	DSU::Speed mSpeed{};

	VertexBuffer mVertexBuffer{};

	ComPtr<ID3D12PipelineState> mpPipelineState{};

	PipelineStateObject mPSO{};

	// Object Data
	IndexedTriangleList::Object mObject{};
	DSU::MaterialData mMaterialData{};

public:

	Object() = default;

	Object(const Object& obj) : mPos(obj.mPos), mSpeed(obj.mSpeed), mObject(obj.mObject),
	mMaterialData(obj.mMaterialData), /*mVB(obj.mVB), */mpPipelineState(obj.mpPipelineState),
	mPSO(obj.mPSO) {}

	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);

	void update(float dt) noexcept;

	DirectX::XMMATRIX getTransformXM() const noexcept;

	DSU::Position& getPos() noexcept { return mPos; }

	DSU::Speed& getSpeed() noexcept { return mSpeed; }

	DSU::MaterialData& getMaterialData() noexcept { return mMaterialData; }

	int onCommand(GraphicsOutput& gfx) noexcept {
		
		return 0;
	}
};