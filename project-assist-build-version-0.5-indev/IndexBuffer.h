#pragma once
#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;

class IndexBuffer {

private:

	ComPtr<ID3D12Resource2>		mpDestRes{};
	ComPtr<ID3D12Resource2>		mpIntermedRes{};
	D3D12_INDEX_BUFFER_VIEW		mIBView{};

public:

	IndexBuffer() = default;
	IndexBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const WORD* indices, const UINT& size) {
		createDestinationResource(pDevice, size);
		createIntermediateResource(pDevice, size);
		createIndexBufferView(pCommandList, indices, size);
	}

	void createDestinationResource(ComPtr<ID3D12Device9>& pDevice, const UINT& size) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof(WORD)) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
		mpDestRes->SetName(L"[ID3D12Resource] [Destination Resource - IndexBuffer]");
	}
	void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice, const UINT& size) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(size * sizeof(WORD)) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
		mpIntermedRes->SetName(L"[ID3D12Resource] [Intermediate Resource - IndexBuffer]");
	}
	void createIndexBufferView(ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const WORD* indices, const UINT& size) noexcept {
		D3D12_SUBRESOURCE_DATA srd{};
		srd.pData = indices;
		srd.RowPitch = size * sizeof(WORD);
		srd.SlicePitch = srd.RowPitch;
		UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
		mIBView.BufferLocation = mpDestRes->GetGPUVirtualAddress();
		mIBView.SizeInBytes = sizeof(WORD) * size;
		mIBView.Format = DXGI_FORMAT_R16_UINT;
	}

	ComPtr<ID3D12Resource2>& getDestinationResource() noexcept { return mpDestRes; }
	ComPtr<ID3D12Resource2>& getIntermediateResource() noexcept { return mpIntermedRes; }
	D3D12_INDEX_BUFFER_VIEW& getView() noexcept { return mIBView; }
	UINT getCount() const noexcept {
		return mIBView.SizeInBytes / sizeof(WORD);
	}
	void transitionToRead(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_INDEX_BUFFER);
		pCommandList->ResourceBarrier(1u, &barrier);
	}
	void transitionToWrite(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_INDEX_BUFFER, D3D12_RESOURCE_STATE_COPY_DEST);
		pCommandList->ResourceBarrier(1u, &barrier);
	}
};