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
	DSU::VertexConstantBuffer mMatrices{};

	VertexBuffer mVertexBuffer{};
	//IndexBuffer mIndexBuffer{};
	VertexConstantBuffer mVCB{};
	PixelConstantBuffer mPCB{};

	DSU::PipelineStateStream mPSS{};
	ComPtr<ID3D12PipelineState> mpPipelineState{};
	
	ComPtr<ID3DBlob> mpVSBytecode{};
	ComPtr<ID3DBlob> mpPSBytecode{};

	ComPtr<ID3D12RootSignature> mpRootSignature{};
	ComPtr<ID3DBlob> mpRootSignatureBytecode{};
	ComPtr<ID3DBlob> mpRootSignatureErrorBytecode{};

	// Object Data
	IndexedTriangleList::Object mObject{};
	std::vector<DSU::VertexData> mObjectData{};
	DSU::MaterialData mMaterialData{};

public:

	Object() = default;
	Object(GraphicsOutput& gfx, IndexedTriangleList::Object& itl_data);

	void update(float dt) noexcept;

	DirectX::XMMATRIX getTransformXM() const noexcept;

	DSU::Position& getPos() noexcept { return mPos; }

	DSU::Speed& getSpeed() noexcept { return mSpeed; }

	DSU::MaterialData& getMaterialData() noexcept { return mMaterialData; }

	void draw(GraphicsOutput& gfx) noexcept;
};