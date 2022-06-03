#pragma once
#include "GraphicsOutput.h"

class VertexBuffer {

private:

	//UINT mStride = { 0u };
	//UINT mOffset = { 0u };
	//UINT mCount = { 0u };
	ComPtr<ID3D12Resource2> mpVertexBuffer{};
	D3D12_VERTEX_BUFFER_VIEW mVertexBufferView{};

public:
	
	VertexBuffer() = default;
	VertexBuffer(const VertexBuffer& vb) : 
		mpVertexBuffer(vb.mpVertexBuffer),
		mVertexBufferView(vb.mVertexBufferView) {}
	VertexBuffer(const std::unique_ptr<VertexBuffer> vb) :
		mpVertexBuffer(vb.get()->mpVertexBuffer),
		mVertexBufferView(vb.get()->mVertexBufferView) {}

	VertexBuffer(GraphicsOutput& gfx, const GraphicsOutput::VertexData* vertices, unsigned int size) /*:
		mStride(sizeof(Object::VertexData)), mCount(size)*/ {

		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);

		auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(size);

		gfx.getDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &heapDesc,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr, 
			IID_PPV_ARGS(&mpVertexBuffer));

		std::unique_ptr<UINT8> pVertexDataStart{};

		CD3DX12_RANGE readRange(0u, 0u);

		mpVertexBuffer->Map(0u, &readRange, (void**)pVertexDataStart.get());
		memcpy(pVertexDataStart.get(), vertices, size);
		mpVertexBuffer->Unmap(0u, nullptr);

		mVertexBufferView.BufferLocation = mpVertexBuffer->GetGPUVirtualAddress();
		mVertexBufferView.StrideInBytes = sizeof(GraphicsOutput::VertexData);
		mVertexBufferView.SizeInBytes = sizeof(GraphicsOutput::VertexData) * size;

	}

	UINT getCount() const noexcept {
		return mVertexBufferView.SizeInBytes / sizeof(GraphicsOutput::VertexData);
	}
};