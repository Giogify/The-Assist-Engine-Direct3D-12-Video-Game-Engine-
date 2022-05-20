#include "Drawable.h"
#include <cassert>
#include <typeinfo>

void Drawable::draw(GraphicsOutput& gfx) const noexcept {
	for (auto& b : m_binds) {
		b->bind(gfx);
	}
	for (auto& b : getStaticBinds()) {
		b->bind(gfx);
	}
	gfx.Draw(m_pVertexBuffer->getCount());
}

void Drawable::addBind(std::unique_ptr<Bindable> bind) noexcept {
	assert("*Must* use addVertexBuffer to bind index buffer" && typeid(*bind) != typeid(VertexBuffer));
	m_binds.push_back(std::move(bind));
}

void Drawable::addVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer) noexcept {
	assert("Attempting to add Vertex buffer a second time" && m_pVertexBuffer == nullptr);
	m_pVertexBuffer = vertexBuffer.get();
	m_binds.push_back(std::move(vertexBuffer));
}