#pragma once
#include "Bindable.h"

class InputLayout : public Bindable {

protected:

	Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;

public:

	InputLayout() = default;
	InputLayout(GraphicsOutput& gfx, const std::vector<D3D11_INPUT_ELEMENT_DESC>& layout, Microsoft::WRL::ComPtr<ID3DBlob> pVertexShaderByteCode);

	void bind(GraphicsOutput& gfx) noexcept override;

};