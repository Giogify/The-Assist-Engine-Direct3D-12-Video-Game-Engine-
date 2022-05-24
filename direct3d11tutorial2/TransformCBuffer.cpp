#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(GraphicsOutput& gfx) {

	if (!m_pVCBuffer) {
		m_pVCBuffer = std::make_unique<VertexConstantBuffer<VertexCBuffer>>(gfx);
	}
	if (!m_pPCBuffer) {
		m_pPCBuffer = std::make_unique<PixelConstantBuffer<PixelCBuffer>>(gfx);
	}
}

void TransformCBuffer::bind(GraphicsOutput& gfx, Object& object) noexcept {

	DirectX::XMFLOAT4X4 proj{};
	DirectX::XMStoreFloat4x4(&proj, gfx.getProjection());

	VertexCBuffer VCB = {};
	VCB.TransformXM = object.getTransformXM();
	VCB.cameraMatrix = gfx.getCamera().getMatrix();
	VCB.projMatrix = gfx.getProjection();

	auto InverseDeterminant = DirectX::XMMatrixDeterminant(object.getTransformXM());
	auto InverseMatrix = DirectX::XMMatrixTranspose(object.getTransformXM());

	VCB.InverseTranspose = DirectX::XMMatrixInverse(&InverseDeterminant, InverseMatrix);

	PixelCBuffer PCB = {};
	PCB.materialData = object.getMaterialData();

	Light::LightData light = {};
	light.pos = { 0.0f, 0.0f, 0.0f, 1.0f };
	light.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.constAtten = 5.0f;
	light.linAtten = 0.0f;
	light.quadAtten = 0.0f;
	light.isEnabled = true;
	light.type = Light::POINT_LIGHT;

	PCB.eyePos = { gfx.getCamera().m_x, gfx.getCamera().m_y, gfx.getCamera().m_z, 1.0f};
	PCB.globalAmbient = { 0.1f, 0.1f, 0.1f, 1.0f };

	PCB.lights[0].light = light;

	auto test = sizeof(VCB);
	auto test2 = sizeof(PCB);

	m_pVCBuffer->update(gfx, VCB);
	m_pPCBuffer->update(gfx, PCB);
	m_pVCBuffer->bind(gfx);
	m_pPCBuffer->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::VertexCBuffer>> TransformCBuffer::m_pVCBuffer;
std::unique_ptr<PixelConstantBuffer<TransformCBuffer::PixelCBuffer>> TransformCBuffer::m_pPCBuffer;