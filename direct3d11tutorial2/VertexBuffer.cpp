#include "VertexBuffer.h"

void VertexBuffer::bind(GraphicsOutput& gfx) noexcept {
	getDeviceContext(gfx)->IASetVertexBuffers(0u, 1u, m_pVertexBuffer.GetAddressOf(), &m_stride, &m_offset);
}

UINT VertexBuffer::getCount() const noexcept {
	return m_count;
}