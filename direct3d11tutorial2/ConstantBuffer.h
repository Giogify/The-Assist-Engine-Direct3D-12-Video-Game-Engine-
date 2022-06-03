#pragma once
#include "GraphicsOutput.h"

template<typename C>
class ConstantBuffer {

protected:

	ComPtr<ID3D12Resource2> mpConstantBuffer{};

public:

	ConstantBuffer(GraphicsOutput& gfx, const C& constants) {

		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);

		auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(C));

		gfx.getDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &heapDesc,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr, 
			IID_PPV_ARGS(&mpConstantBuffer));

		std::unique_ptr<UINT8> pConstantBufferStart{};

		CD3DX12_RANGE readRange(0u, 0u);

		mpConstantBuffer->Map(0u, &readRange, (void**)pConstantBufferStart.get());
		memcpy(pConstantBufferStart.get(), constants, sizeof(C));
		mpConstantBuffer->Unmap(0u, nullptr);

	}

	ConstantBuffer(GraphicsOutput& gfx) {

		CD3DX12_HEAP_PROPERTIES heapProperties(D3D12_HEAP_TYPE_DEFAULT);

		auto heapDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(C));

		gfx.getDevice()->CreateCommittedResource(&heapProperties, D3D12_HEAP_FLAG_NONE, &heapDesc,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, nullptr,
			IID_PPV_ARGS(&mpConstantBuffer));

		std::unique_ptr<UINT8> pConstantBufferStart{};

		CD3DX12_RANGE readRange(0u, 0u);

		//mpConstantBuffer->Map(0u, &readRange, (void**)pConstantBufferStart.get());
		//memcpy(pConstantBufferStart.get(), nullptr, sizeof(C));
		//mpConstantBuffer->Unmap(0u, nullptr);
	
	}

	void update(GraphicsOutput& gfx, const C& constants) {
		std::unique_ptr<UINT8> pConstantBufferStart{};
		CD3DX12_RANGE readRange(0u, 0u);
		mpConstantBuffer->Map(0u, &readRange, (void**)pConstantBufferStart.get());
		memcpy(pConstantBufferStart.get(), &constants, sizeof(constants));
		mpConstantBuffer->Unmap(0u, nullptr);
	}

};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {

	using ConstantBuffer<C>::mpConstantBuffer;

public:

	using ConstantBuffer<C>::ConstantBuffer;
	void bind(GraphicsOutput& gfx) noexcept {
		//getDeviceContext(gfx)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}

};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {

	using ConstantBuffer<C>::mpConstantBuffer;

public:

	using ConstantBuffer<C>::ConstantBuffer;
	void bind(GraphicsOutput& gfx) noexcept {
		//getDeviceContext(gfx)->PSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}

};