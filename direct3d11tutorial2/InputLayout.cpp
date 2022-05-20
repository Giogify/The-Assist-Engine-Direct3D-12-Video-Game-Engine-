#include "InputLayout.h"

InputLayout::InputLayout(GraphicsOutput& gfx,
	const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout,
	Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderByteCode) {

	getDevice(gfx)->CreateInputLayout(
		layout.data(),
		(UINT)layout.size(),
		pVertexShaderByteCode->GetBufferPointer(),
		pVertexShaderByteCode->GetBufferSize(),
		&m_pInputLayout
	);
}

void InputLayout::bind(GraphicsOutput& gfx) noexcept {
	getDeviceContext(gfx)->IASetInputLayout(m_pInputLayout.Get());
}