#pragma once
#include "VertexBuffer.h"
#include <DirectXMath.h>
#include <memory>

class Bindable;

class Drawable {

	template<class T>
	friend class DrawableBase;

private:

	const class VertexBuffer* m_pVertexBuffer = nullptr;
	std::vector<std::unique_ptr<Bindable>> m_binds;

private: // Private Fields

	virtual const std::vector<std::unique_ptr<Bindable>>& getStaticBinds() const noexcept = 0;

protected: // Protected Methods

	void addBind(std::unique_ptr<Bindable> bind) noexcept;
	void addVertexBuffer(std::unique_ptr<class VertexBuffer> vertexBuffer) noexcept;

public: // Public Methods

	Drawable() = default;
	Drawable(const Drawable&) = delete;
	virtual ~Drawable() = default;

	virtual DirectX::XMMATRIX getTransformXM() const noexcept = 0;
	
	void draw(GraphicsOutput& gfx) const noexcept;
	virtual void update(float dt) noexcept = 0;

};