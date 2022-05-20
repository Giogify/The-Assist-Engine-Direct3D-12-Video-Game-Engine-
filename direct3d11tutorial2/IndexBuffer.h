#pragma once
#include "Bindable.h"

class IndexBuffer : public Bindable {

protected:

	UINT m_count;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer;

public:

	IndexBuffer(GraphicsOutput& gfx, const std::vector<unsigned short>& indices);
	void bind(GraphicsOutput& gfx) noexcept override;
	UINT getCount() const noexcept;

};