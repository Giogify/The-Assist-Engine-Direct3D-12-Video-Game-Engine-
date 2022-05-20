#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(GraphicsOutput& gfx, const std::vector<unsigned short>& indices) :
	
	m_count((UINT)indices.size()) {

	// Configure Index Buffer Description
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = UINT(m_count * sizeof(unsigned short));
	ibd.StructureByteStride = sizeof(unsigned short);

	// Index Buffer Subresource
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices.data();

	// Fill Index Buffer
	getDevice(gfx)->CreateBuffer(&ibd, &isd, &m_pIndexBuffer);
}

void IndexBuffer::bind(GraphicsOutput& gfx) noexcept {
	getDeviceContext(gfx)->IASetIndexBuffer(m_pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
}

UINT IndexBuffer::getCount() const noexcept {
	return m_count;
}