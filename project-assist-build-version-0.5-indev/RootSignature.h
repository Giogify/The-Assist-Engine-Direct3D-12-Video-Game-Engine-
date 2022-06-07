#pragma once
#include "GraphicsOutput.h"

class RootSignature {

private:

	ComPtr<ID3D12RootSignature> mpRootSignature{};
	ComPtr<ID3DBlob> mpSignature{};
	ComPtr<ID3DBlob> mpError{};

public:

	RootSignature() = default;
	RootSignature(GraphicsOutput& gfx) {
		CD3DX12_ROOT_SIGNATURE_DESC rsd;
		rsd.Init(0u, nullptr, 0u, nullptr, 
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

		D3D12SerializeRootSignature(&rsd, D3D_ROOT_SIGNATURE_VERSION_1,
			&mpSignature, &mpError);
		
		gfx.getDevice()->CreateRootSignature(0u, 
			mpSignature->GetBufferPointer(), mpSignature->GetBufferSize(), 
			IID_PPV_ARGS(&mpRootSignature));
	}

	ComPtr<ID3D12RootSignature>& getRootSignature() noexcept { return mpRootSignature; }
};