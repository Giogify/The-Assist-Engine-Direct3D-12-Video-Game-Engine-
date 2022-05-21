#pragma once
#include "ConstantBuffer.h"
#include "Object.h"
#include <DirectXMath.h>

class TransformCBuffer : public Bindable {

public:

	struct TotalCBuffer {
		Object::MaterialData materialData{};
		DirectX::XMMATRIX TransformXM{};
		DirectX::XMFLOAT3X3 InverseTranspose{};
		DirectX::XMMATRIX cameraMatrix{};
		DirectX::XMMATRIX projMatrix{};
	};

private:

	static std::unique_ptr<VertexConstantBuffer<TotalCBuffer>> m_pVCBuffer;
	static std::unique_ptr<PixelConstantBuffer<TotalCBuffer>> m_pPCBuffer;
	Object& m_parent;

public:

	TransformCBuffer(GraphicsOutput& gfx, Object& parent);
	void bind(GraphicsOutput& gfx) noexcept override;

};