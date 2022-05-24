#pragma once
#include "GraphicsOutput.h"

class Bindable {

protected:

	static Microsoft::WRL::ComPtr<ID3D11DeviceContext> getDeviceContext(GraphicsOutput& gfx) noexcept;
	static Microsoft::WRL::ComPtr<ID3D11Device> getDevice(GraphicsOutput& gfx) noexcept;

public:

	virtual void bind(GraphicsOutput& gfx) noexcept = 0;

};