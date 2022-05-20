#include "PixelShader.h"

PixelShader::PixelShader(GraphicsOutput& gfx, const std::wstring& path) {
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(path.c_str(), &pBlob);
	getDevice(gfx)->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &m_pPixelShader);
}

void PixelShader::bind(GraphicsOutput& gfx) noexcept {
	getDeviceContext(gfx)->PSSetShader(m_pPixelShader.Get(), nullptr, 0u);
}