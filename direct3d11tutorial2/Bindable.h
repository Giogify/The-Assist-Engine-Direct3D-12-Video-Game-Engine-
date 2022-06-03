#pragma once
#include "GraphicsOutput.h"

class Bindable {

protected:

	static Microsoft::WRL::ComPtr<ID3D12CommandList> getCommandList(GraphicsOutput& gfx) noexcept;
	static Microsoft::WRL::ComPtr<ID3D12Device9> getDevice(GraphicsOutput& gfx) noexcept;

public:

	virtual void bind(GraphicsOutput& gfx) noexcept = 0;

};