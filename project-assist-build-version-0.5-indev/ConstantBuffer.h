#pragma once
#include "DataStructures.h"
#include "d3dx12.h"
#include <d3d12.h>
#include <wrl.h>
#include <memory>

using namespace Microsoft::WRL;
namespace DSU = DataStructsUtil;
namespace DX = DirectX;

class VertexConstantBuffer {

private:

	ComPtr<ID3D12Resource2>			mpDestRes{};
	ComPtr<ID3D12Resource2>			mpIntermedRes{};
	ComPtr<ID3D12DescriptorHeap>	mpCBVHeap{};
	UINT							mpCBVHeapSize{};

public:

	VertexConstantBuffer() = default;
	VertexConstantBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const DSU::VertexConstantBuffer& data) {
		createDestinationResource(pDevice);
		createIntermediateResource(pDevice);
		createDescriptorHeap(pDevice);
		createConstantBufferView(pDevice, pCommandList, data);
	}
	
	void createDestinationResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
	}
	void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
	}
	void createDescriptorHeap(ComPtr<ID3D12Device9>& pDevice) noexcept {
		D3D12_DESCRIPTOR_HEAP_DESC cbvhd{};
		cbvhd.NumDescriptors = 1u;
		cbvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		pDevice->CreateDescriptorHeap(&cbvhd, IID_PPV_ARGS(&mpCBVHeap));
		mpCBVHeapSize = pDevice->GetDescriptorHandleIncrementSize(cbvhd.Type);
	}
	void createConstantBufferView(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const DSU::VertexConstantBuffer& data) noexcept {
		D3D12_SUBRESOURCE_DATA srd{}; {
			srd.pData = &data;
			srd.RowPitch = alignBytes();
			srd.SlicePitch = srd.RowPitch;
		}
		UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{}; {
			cbvd.BufferLocation = mpDestRes->GetGPUVirtualAddress();
			cbvd.SizeInBytes = alignBytes();
		}
		pDevice->CreateConstantBufferView(&cbvd, mpCBVHeap->GetCPUDescriptorHandleForHeapStart());
	}
	
	ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
	ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
	UINT alignBytes() const noexcept {
		return (UINT)(sizeof(DSU::VertexConstantBuffer) % 256 == 0 
			? sizeof(DSU::VertexConstantBuffer) 
			: 256 - sizeof(DSU::VertexConstantBuffer) % 256 + sizeof(DSU::VertexConstantBuffer));
	}
	void transitionToRead(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		pCommandList->ResourceBarrier(1u, &barrier);
	}
	void transitionToWrite(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			D3D12_RESOURCE_STATE_COPY_DEST);
		pCommandList->ResourceBarrier(1u, &barrier);
	}
};
class PixelConstantBuffer {

private:

	ComPtr<ID3D12Resource2>			mpDestRes{};
	ComPtr<ID3D12Resource2>			mpIntermedRes{};
	ComPtr<ID3D12DescriptorHeap>	mpCBVHeap{};
	UINT							mpCBVHeapSize{};

public:

	PixelConstantBuffer() = default;
	PixelConstantBuffer(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const DSU::PixelConstantBuffer& data) {
		createDestinationResource(pDevice);
		createIntermediateResource(pDevice);
		createDescriptorHeap(pDevice);
		createConstantBufferView(pDevice, pCommandList, data);
	}

	void createDestinationResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_DEFAULT);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_COPY_DEST, nullptr, IID_PPV_ARGS(&mpDestRes));
	}
	void createIntermediateResource(ComPtr<ID3D12Device9>& pDevice) noexcept {
		CD3DX12_HEAP_PROPERTIES hp(D3D12_HEAP_TYPE_UPLOAD);
		CD3DX12_RESOURCE_DESC rd{ CD3DX12_RESOURCE_DESC::Buffer(alignBytes()) };
		pDevice->CreateCommittedResource(&hp, D3D12_HEAP_FLAG_NONE, &rd, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&mpIntermedRes));
	}
	void createDescriptorHeap(ComPtr<ID3D12Device9>& pDevice) noexcept {
		D3D12_DESCRIPTOR_HEAP_DESC cbvhd{};
		cbvhd.NumDescriptors = 1u;
		cbvhd.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
		cbvhd.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
		pDevice->CreateDescriptorHeap(&cbvhd, IID_PPV_ARGS(&mpCBVHeap));
		mpCBVHeapSize = pDevice->GetDescriptorHandleIncrementSize(cbvhd.Type);
	}
	void createConstantBufferView(ComPtr<ID3D12Device9>& pDevice, ComPtr<ID3D12GraphicsCommandList6>& pCommandList, const DSU::PixelConstantBuffer& data) noexcept {
		D3D12_SUBRESOURCE_DATA srd{}; {
			srd.pData = &data;
			srd.RowPitch = alignBytes();
			srd.SlicePitch = srd.RowPitch;
		}
		UpdateSubresources(pCommandList.Get(), mpDestRes.Get(), mpIntermedRes.Get(), 0u, 0u, 1u, &srd);
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbvd{}; {
			cbvd.BufferLocation = mpDestRes->GetGPUVirtualAddress();
			cbvd.SizeInBytes = alignBytes();
		}
		pDevice->CreateConstantBufferView(&cbvd, mpCBVHeap->GetCPUDescriptorHandleForHeapStart());
	}

	ComPtr<ID3D12Resource2>& getDestRes() noexcept { return mpDestRes; }
	ComPtr<ID3D12Resource2>& getIntermedRes() noexcept { return mpIntermedRes; }
	UINT alignBytes() const noexcept {
		return (UINT)(sizeof(DSU::PixelConstantBuffer) % 256 == 0
			? sizeof(DSU::PixelConstantBuffer)
			: 256 - sizeof(DSU::PixelConstantBuffer) % 256 + sizeof(DSU::PixelConstantBuffer));
	}
	void transitionToRead(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
		pCommandList->ResourceBarrier(1u, &barrier);
	}
	void transitionToWrite(ComPtr<ID3D12GraphicsCommandList6>& pCommandList) noexcept {
		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(mpDestRes.Get(), D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER,
			D3D12_RESOURCE_STATE_COPY_DEST);
		pCommandList->ResourceBarrier(1u, &barrier);
	}
};