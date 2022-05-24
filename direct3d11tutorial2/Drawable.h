#pragma once
#include "VertexBuffer.h"
#include "Bindable.h"
#include <DirectXMath.h>
#include <memory>


class Drawable {

	template<class T>
	friend class DrawableBase;

protected:

	std::unique_ptr<VertexBuffer> m_pVertexBuffer{};
	std::vector<std::unique_ptr<Bindable>> m_binds{};

private: // Private Fields

	virtual std::vector<std::unique_ptr<Bindable>>& getStaticBinds() noexcept = 0;

protected: // Protected Methods

	void addBind(std::unique_ptr<Bindable> bind) noexcept;
	void addVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer) noexcept;

public: // Public Methods

	Drawable() = default;

	virtual DirectX::XMMATRIX getTransformXM() const noexcept = 0;
	
	void draw(GraphicsOutput& gfx) noexcept;
	virtual void update(float dt) noexcept = 0;

};