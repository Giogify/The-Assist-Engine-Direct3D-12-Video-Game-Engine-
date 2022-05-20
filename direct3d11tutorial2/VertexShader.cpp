#include "VertexShader.h"

VertexShader::VertexShader(GraphicsOutput& gfx, const std::wstring& path) :
	path(path) {
	D3DReadFileToBlob(path.c_str(), m_pBytecodeBlob.GetAddressOf());
	//getDevice(gfx)->CreateVertexShader(m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(), nullptr, &m_pVertexShader);
	getDevice(gfx)->CreateVertexShader(m_pBytecodeBlob->GetBufferPointer(), m_pBytecodeBlob->GetBufferSize(), nullptr, &m_pVertexShader);
}

void VertexShader::bind(GraphicsOutput& gfx) noexcept {
	getDeviceContext(gfx)->VSSetShader(m_pVertexShader.Get(), nullptr, 0u);
}

Microsoft::WRL::ComPtr<ID3DBlob> VertexShader::getBytecode() const noexcept {
	return m_pBytecodeBlob.Get();
}