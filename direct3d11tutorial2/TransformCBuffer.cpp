#include "TransformCBuffer.h"
#include <iostream>

TransformCBuffer::TransformCBuffer(GraphicsOutput& gfx) {

	if (!mptrVCBuffer) {
		mptrVCBuffer = std::make_unique<VertexConstantBuffer<VertexCBuffer>>(gfx);
	}
	if (!mptrPCBuffer) {
		mptrPCBuffer = std::make_unique<PixelConstantBuffer<PixelCBuffer>>(gfx);
	}
}

void TransformCBuffer::bind(GraphicsOutput& gfx, std::unique_ptr<Scene>& scene) noexcept {
	
	auto camera = std::make_unique<Camera>(scene->getCamera());

	Light::LightData light = {};
	light.pos = { 0.0f, 0.0f, 0.0f, 1.0f };
	light.color = { 1.0f, 1.0f, 1.0f, 1.0f };
	light.constAtten = 0.0f;
	light.linAtten = 0.1f;
	light.quadAtten = 0.0f;
	light.isEnabled = true;
	light.type = Light::POINT_LIGHT;

	for (auto& a : scene->getActors()) {
		//a.getModel().draw(gfx);
		
		for (auto& o : a.getModel().getObjects()) {
			VertexCBuffer VCB = {};
			VCB.TransformXM = o.getTransformXM();
			VCB.cameraMatrix = camera->getMatrix();
			VCB.projMatrix = gfx.getProjection();
			auto InverseDeterminant = DirectX::XMMatrixDeterminant(o.getTransformXM());
			auto InverseMatrix = DirectX::XMMatrixTranspose(o.getTransformXM());
			VCB.InverseTranspose = DirectX::XMMatrixInverse(&InverseDeterminant, InverseMatrix);
			
			PixelCBuffer PCB = {};
			PCB.materialData = o.getMaterialData();
			PCB.eyePos = { camera->mEye.x, camera->mEye.y, camera->mEye.z, 1.0f};
			PCB.globalAmbient = { 0.0f, 0.0f, 0.0f, 1.0f };
			PCB.lights[0].light = light;

			mptrVCBuffer->update(gfx, VCB);
			mptrPCBuffer->update(gfx, PCB);
			mptrVCBuffer->bind(gfx);
			mptrPCBuffer->bind(gfx);

		}
	}
}

std::unique_ptr<VertexConstantBuffer<TransformCBuffer::VertexCBuffer>> TransformCBuffer::mptrVCBuffer;
std::unique_ptr<PixelConstantBuffer<TransformCBuffer::PixelCBuffer>> TransformCBuffer::mptrPCBuffer;