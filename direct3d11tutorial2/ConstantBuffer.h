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

	ConstantBuffer(GraphicsOutput& gfx, const C* constants, unsigned int size) {
		
		const UINT MAX_ELEMENTS = D3D11_REQ_CONSTANT_BUFFER_ELEMENT_COUNT;
		UINT amountLeftToProcess = size;
		UINT factor = size / MAX_ELEMENTS;
		
		auto csd = std::make_unique<D3D11_SUBRESOURCE_DATA[]>((static_cast<size_t>(factor)) + 1);

		for (int i = 0; amountLeftToProcess > MAX_ELEMENTS; i++) {
			csd[i].pSysMem = constants + (i * (/*sizeof(C) * */MAX_ELEMENTS));
			amountLeftToProcess -= MAX_ELEMENTS;
		}
		csd[factor].pSysMem = constants + (factor * (/*sizeof(C) * */MAX_ELEMENTS));

		// Create and Configure Constant Buffer Descriptor
		D3D11_BUFFER_DESC cbd = {};
		cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd.Usage = D3D11_USAGE_DYNAMIC;
		cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd.MiscFlags = 0u;
		cbd.ByteWidth = sizeof(C) * MAX_ELEMENTS;
		cbd.StructureByteStride = sizeof(C);

		amountLeftToProcess = size;

		for (int i = 0; amountLeftToProcess > MAX_ELEMENTS; i++) {
			getDevice(gfx)->CreateBuffer(&cbd, &csd[i], &buffers[cbufferCount]);
			cbufferCount++;
			amountLeftToProcess -= MAX_ELEMENTS;
		}
		
		// Create and Configure Constant Buffer Descriptor
		D3D11_BUFFER_DESC cbd2 = {};
		cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cbd2.Usage = D3D11_USAGE_DYNAMIC;
		cbd2.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		cbd2.MiscFlags = 0u;
		cbd2.ByteWidth = sizeof(C) * amountLeftToProcess;
		cbd2.StructureByteStride = sizeof(C);
		
		getDevice(gfx)->CreateBuffer(&cbd2, &csd[factor], &buffers[cbufferCount]);
		cbufferCount++;
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