#pragma once
#include "Bindable.h"

class VertexShader : public Bindable {

protected:

	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> m_pBytecodeBlob = nullptr;
	std::wstring path{};

public:

	VertexShader() = default;

	VertexShader(GraphicsOutput& gfx, const std::wstring& path);

	void bind(GraphicsOutput& gfx) noexcept override;
	Microsoft::WRL::ComPtr<ID3DBlob> getBytecode() const noexcept;

};