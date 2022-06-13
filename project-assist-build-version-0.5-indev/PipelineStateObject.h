//#pragma once
//#include "GraphicsOutput.h"
//#include "RootSignature.h"
//#include "VertexShader.h"
//#include "PixelShader.h"
//
//class PipelineStateObject {
//
//public:
//
//	struct Binds {
//		RootSignature rs{};
//		VertexShader vs{};
//		PixelShader ps{};
//		std::vector<D3D12_INPUT_ELEMENT_DESC> ied{};
//
//	};
//
//private:
//
//	Binds mBinds{};
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC psod{};
//	ComPtr<ID3D12PipelineState> mpPipelineStateObject{};
//
//public:
//
//	PipelineStateObject() = default;
//	PipelineStateObject(GraphicsOutput& gfx, Binds& binds) : mBinds(binds) {
//		psod.InputLayout = { mBinds.ied.data(), (UINT)mBinds.ied.size() };
//		psod.pRootSignature = mBinds.rs.getRootSignature().Get();
//		psod.VS = { (UINT8*)mBinds.vs.getBytecode()->GetBufferPointer(), mBinds.vs.getBytecode()->GetBufferSize() };
//		psod.PS = { (UINT8*)mBinds.ps.getBytecode()->GetBufferPointer(), mBinds.ps.getBytecode()->GetBufferSize() };
//		psod.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
//		psod.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
//		psod.DepthStencilState.DepthEnable = FALSE;
//		psod.DepthStencilState.StencilEnable = FALSE;
//		psod.SampleMask = UINT_MAX;
//		psod.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//		psod.NumRenderTargets = 1;
//		psod.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
//		psod.SampleDesc.Count = 1;
//
//		gfx.getDevice()->CreateGraphicsPipelineState(&psod, IID_PPV_ARGS(&mpPipelineStateObject));
//	}
//
//	ComPtr<ID3D12PipelineState>& getPSO() noexcept { return mpPipelineStateObject; }
//};