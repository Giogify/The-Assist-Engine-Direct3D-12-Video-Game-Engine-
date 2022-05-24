#include "TransformCBuffer.h"

TransformCBuffer::TransformCBuffer(GraphicsOutput& gfx) {

	if (!mptrVCBuffer) {
		mptrVCBuffer = std::make_unique<VertexConstantBuffer<VertexCBuffer>>(gfx);
	}
	if (!mptrPCBuffer) {
		mptrPCBuffer = std::make_unique<PixelConstantBuffer<PixelCBuffer>>(gfx);
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
	light.pos = { -4.0f, 12.0f, 5.0f, 1.0f };
	light.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.constAtten = 1.0f;
	light.linAtten = 0.08f;
	light.quadAtten = 0.0f;
	light.isEnabled = true;
	light.type = Light::POINT_LIGHT;

	PCB.eyePos = { gfx.getCamera().mEye.x, gfx.getCamera().mEye.y, gfx.getCamera().mEye.z, 1.0f};
	PCB.globalAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };

	PCB.lights[0].light = light;

	auto test = sizeof(VCB);
	auto test2 = sizeof(PCB);

	mptrVCBuffer->update(gfx, VCB);
	mptrPCBuffer->update(gfx, PCB);
	mptrVCBuffer->bind(gfx);
	mptrPCBuffer->bind(gfx);
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::VertexCBuffer>> TransformCBuffer::mptrVCBuffer;
std::unique_ptr<PixelConstantBuffer<TransformCBuffer::PixelCBuffer>> TransformCBuffer::mptrPCBuffer;