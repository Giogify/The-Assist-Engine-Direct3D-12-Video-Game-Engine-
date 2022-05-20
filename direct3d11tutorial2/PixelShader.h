#pragma once
#include "Bindable.h"

class PixelShader : public Bindable {

protected:

	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader;

public:

	PixelShader(GraphicsOutput& gfx, const std::wstring& path);

	void bind(GraphicsOutput& gfx) noexcept override;

};