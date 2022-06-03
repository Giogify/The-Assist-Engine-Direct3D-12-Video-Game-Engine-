#pragma once
#include "Bindable.h"

class Topology : public Bindable {

protected:

	D3D12_PRIMITIVE_TOPOLOGY m_type{};

public:

	Topology() = default;
	Topology(GraphicsOutput& gfx, D3D12_PRIMITIVE_TOPOLOGY type);
	
	void bind(GraphicsOutput& gfx) noexcept override;

};