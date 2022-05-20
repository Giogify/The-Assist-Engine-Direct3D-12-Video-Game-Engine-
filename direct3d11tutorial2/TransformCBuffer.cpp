#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(GraphicsOutput& gfx, const Drawable& parent) :
	m_parent(parent) {

	if (!m_pVCBuffer) {
		m_pVCBuffer = std::make_unique<VertexConstantBuffer<DirectX::XMMATRIX>>(gfx);
	}
}

void TransformCBuffer::bind(GraphicsOutput& gfx) noexcept {

	m_pVCBuffer->update(gfx, m_parent.getTransformXM() * gfx.getCamera().getMatrix() * gfx.getProjection());
	m_pVCBuffer->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> TransformCBuffer::m_pVCBuffer;