#pragma once
#include "Bindable.h"

template<typename C>
class ConstantBuffer : public Bindable {

protected:

	UINT cbufferCount = 0u;
	ID3D11Buffer* buffers[5u];
	//static inline auto m_pConstantBuffers = std::make_unique<Microsoft::WRL::ComPtr<ID3D11Buffer>[]>(5u);
	//std::unique_ptr<Microsoft::WRL::ComPtr<ID3D11Buffer>[], std::default_delete<Microsoft::WRL::ComPtr<ID3D11Buffer>[]>> m_pConstantBuffers = std::make_unique<Microsoft::WRL::ComPtr<ID3D11Buffer>[]>(4);
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer{};

public:

	ConstantBuffer(GraphicsOutput& gfx, const C& constants) {

		// Create and Configure Constant Buffer Descriptor
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;
			
		D3D11_SUBRESOURCE_DATA csd = {};
		csd.pSysMem = &constants;

		getDevice(gfx)->CreateBuffer(&cbd, &csd, &m_pConstantBuffer);
	}

	ConstantBuffer(GraphicsOutput& gfx) {

		// Create and Configure Constant Buffer Descriptor
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C);
		cbd.StructureByteStride = 0u;

		getDevice(gfx)->CreateBuffer(&cbd, nullptr, &m_pConstantBuffer);
	}

	void update(GraphicsOutput& gfx, const C& constants) {
		D3D11_MAPPED_SUBRESOURCE subResourceMapped;
		getDeviceContext(gfx)->Map(m_pConstantBuffer.Get(), 0u, D3D11_MAP_WRITE_DISCARD, 0u, &subResourceMapped);
		memcpy(subResourceMapped.pData, &constants, sizeof(constants));
		getDeviceContext(gfx)->Unmap(m_pConstantBuffer.Get(), 0u);
	}

};

template<typename C>
class VertexConstantBuffer : public ConstantBuffer<C> {

	using ConstantBuffer<C>::m_pConstantBuffer;
	using Bindable::getDeviceContext;

public:

	using ConstantBuffer<C>::ConstantBuffer;
	void bind(GraphicsOutput& gfx) noexcept override {
		getDeviceContext(gfx)->VSSetConstantBuffers(0u, 1u, m_pConstantBuffer.GetAddressOf());
	}

};

template<typename C>
class PixelConstantBuffer : public ConstantBuffer<C> {

	using ConstantBuffer<C>::m_pConstantBuffer;
	using ConstantBuffer<C>::buffers;
	using Bindable::getDeviceContext;

public:

	using ConstantBuffer<C>::ConstantBuffer;
	void bind(GraphicsOutput& gfx) noexcept override {
		getDeviceContext(gfx)->PSSetConstantBuffers(0u, 5u, &buffers[0]);
	}
	//void bind(GraphicsOutput& gfx, std::vector<Microsoft::WRL::ComPtr<ID3D11Buffer>> pConstantBuffers) noexcept {
	//	getDeviceContext(gfx)->PSSetConstantBuffers(0u, pConstantBuffers.size(), pConstantBuffers.at(0).GetAddressOf());
	//}

};