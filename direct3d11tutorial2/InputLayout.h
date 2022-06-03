//#pragma once
//#include "Bindable.h"
//
//class InputLayout : public Bindable {
//
//protected:
//
//	ComPtr<ID3D12InputLayout> m_pInputLayout = nullptr;
//
//public:
//
//	InputLayout() = default;
//	InputLayout(GraphicsOutput& gfx, 
//		const std::vector<D3D12_INPUT_ELEMENT_DESC>& layout, 
//		ComPtr<ID3DBlob> pVertexShaderByteCode);
//
//	void bind(GraphicsOutput& gfx) noexcept override;
//
//};