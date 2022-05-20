#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {

protected:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob;
	std::wstring path = {};

public:

	VertexShader(GraphicsOutput& gfx, const std::wstring& path);

	void bind(GraphicsOutput& gfx) noexcept override;
	Microsoft::WRL::ComPtr<ID3DBlob> getBytecode() const noexcept;

};