#include "Bindable.h"

Microsoft::WRL::ComPtr<ID3D11DeviceContext> Bindable::getDeviceContext(GraphicsOutput& gfx) noexcept {
	return gfx.getDeviceContext();
}

Microsoft::WRL::ComPtr<ID3D11Device> Bindable::getDevice(GraphicsOutput& gfx) noexcept {
	return gfx.getDevice();
}