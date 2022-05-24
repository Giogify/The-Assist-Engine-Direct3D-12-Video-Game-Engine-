#include "Drawable.h"
#include <cassert>
#include <typeinfo>

void Drawable::draw(GraphicsOutput& gfx) noexcept {
	for (auto& b : m_binds) {
		b->bind(gfx);
	}
	for (auto& b : getStaticBinds()) {
		b->bind(gfx);
	}
	gfx.Draw(m_pVertexBuffer->getCount());
}

void Drawable::addBind(std::unique_ptr<Bindable> bind) noexcept {
	m_binds.push_back(std::move(bind));
}

void Drawable::addVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer) noexcept {
	//m_pVertexBuffer = std::make_unique<VertexBuffer>(vertexBuffer);
	//m_binds.push_back(std::make_unique<VertexBuffer>(vertexBuffer));
}