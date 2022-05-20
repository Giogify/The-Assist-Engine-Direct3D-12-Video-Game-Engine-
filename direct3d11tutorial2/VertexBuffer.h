#pragma once
#include "Bindable.h"

class VertexBuffer : public Bindable {

protected:

	UINT m_stride = {};
	UINT m_offset = { 0u };
	UINT m_count = { 0u };
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer;

public:
	
	VertexBuffer() = default;

	template<class V>
	VertexBuffer(GraphicsOutput& gfx, const V* vertices, unsigned int size) :
		m_stride(sizeof(V)),
		m_count(size) {

		// Configure Vertex Buffer Description
		D3D11_BUFFER_DESC vertexBD = {};
		vertexBD.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		vertexBD.Usage = D3D11_USAGE_DEFAULT;
		vertexBD.CPUAccessFlags = 0u;
		vertexBD.MiscFlags = 0u;
		vertexBD.ByteWidth = UINT(sizeof(V) * m_count);
		vertexBD.StructureByteStride = sizeof(V);

		// Vertex Buffer Subresource
		D3D11_SUBRESOURCE_DATA vertexSD = {};
		vertexSD.pSysMem = vertices;

		// Fill Vertex Buffer with Vertex Buffer Desc and Subresource
		getDevice(gfx)->CreateBuffer(&vertexBD, &vertexSD, &m_pVertexBuffer);
	}

	void bind(GraphicsOutput& gfx) noexcept override;

	UINT getCount() const noexcept;
};