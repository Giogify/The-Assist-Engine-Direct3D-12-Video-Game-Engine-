#pragma once
#include "ConstantBuffer.h"
#include "Drawable.h"
#include <DirectXMath.h>

class TransformCBuffer : public Bindable {

private:

	static std::unique_ptr<VertexConstantBuffer<DirectX::XMMATRIX>> m_pVCBuffer;
	const Drawable& m_parent;

public:

	TransformCBuffer(GraphicsOutput& gfx, const Drawable& parent);
	void bind(GraphicsOutput& gfx) noexcept override;

};