#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(GraphicsOutput& gfx, Object& parent) :
	m_parent(parent) {

	if (!m_pVCBuffer) {
		m_pVCBuffer = std::make_unique<VertexConstantBuffer<TotalCBuffer>>(gfx);
	}
	if (!m_pPCBuffer) {
		m_pPCBuffer = std::make_unique<PixelConstantBuffer<TotalCBuffer>>(gfx);
	}
}

void TransformCBuffer::bind(GraphicsOutput& gfx) noexcept {

	DirectX::XMFLOAT4X4 proj{};
	DirectX::XMStoreFloat4x4(&proj, gfx.getProjection());

	float inv = 1 / proj._34;

	TotalCBuffer TCB;
	TCB.materialData = m_parent.getMaterialData();
	TCB.TransformXM = DirectX::XMMatrixTranspose(m_parent.getTransformXM());
	TCB.InverseTranspose = { inv, inv, inv, inv, inv, inv, inv, inv, inv };
	TCB.cameraMatrix = DirectX::XMMatrixTranspose(gfx.getCamera().getMatrix());
	TCB.projMatrix = DirectX::XMMatrixTranspose(gfx.getProjection());

	unsigned short test2 = sizeof(TCB);

	m_pVCBuffer->update(gfx, TCB);
	m_pPCBuffer->update(gfx, TCB);
	m_pVCBuffer->bind(gfx);
	m_pPCBuffer->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::TotalCBuffer>> TransformCBuffer::m_pVCBuffer;
std::unique_ptr<PixelConstantBuffer<TransformCBuffer::TotalCBuffer>> TransformCBuffer::m_pPCBuffer;