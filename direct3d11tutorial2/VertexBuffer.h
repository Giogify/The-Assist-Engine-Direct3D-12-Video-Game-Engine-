#pragma once
#include "DataStructures.h"
#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;
namespace DSU = DataStructsUtil;

class VertexBuffer {

private:

	ComPtr<ID3D12Resource2>		mpDestRes{};
	ComPtr<ID3D12Resource2>		mpIntermedRes{};
	D3D12_VERTEX_BUFFER_VIEW	mVBView{};

public:

	VertexBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const DSU::VertexData* vertices, const UINT& size)  {
		createDestinationResource(pDevice, size);
		createIntermediateResource(pDevice, size);
		createVertexBufferView(pCommandList, vertices, size);
	}

	void createDestinationResource(ComPtr<ID3D12Device9>& pDevice, const UINT& size) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof(DSU::VertexData)) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
	}
	void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice, const UINT& size) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof(DSU::VertexData)) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
	}
	void createVertexBufferView(ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const DSU::VertexData* vertices, const UINT& size) noexcept {
		D3D12_SUBRESOURCE_DATA srd{}; {
			srd.pData = vertices;
			srd.RowPitch = size * sizeof(DSU::VertexData);
			srd.SlicePitch = srd.RowPitch;
		}
		UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
		mVBView.BufferLocation = mpDestRes->GetGPUVirtualAddress();
		mVBView.SizeInBytes = sizeof(DSU::VertexData) * size;
		mVBView.StrideInBytes = sizeof(DSU::VertexData);
	}

	ComPtr<ID3D12Resource2>& getDestinationResource() noexcept { return mpDestRes; }
	ComPtr<ID3D12Resource2>& getIntermediateResource() noexcept { return mpIntermedRes; }
	D3D12_VERTEX_BUFFER_VIEW& getView() noexcept { return mVBView; }
	UINT getCount() const noexcept {
		return mVBView.SizeInBytes / sizeof(DSU::VertexData);
	}
};