#pragma once
#include "Drawable.h"
#include "VertexBuffer.h"

template<class T>
class DrawableBase : public Drawable {

private: // Private Fields

	static std::vector<std::unique_ptr<Bindable>> staticBinds;

private:

	const std::vector<std::unique_ptr<Bindable>>& getStaticBinds() const noexcept {
		return staticBinds;
	}

protected:

	bool isStaticInitialized() const noexcept {
		return !staticBinds.empty();
	}
	static void addStaticBind(std::unique_ptr<Bindable> bind) noexcept {
		assert("*Must* use addStaticVertexBuffer to bind index buffer" && typeid(*bind) != typeid(VertexBuffer));
		staticBinds.push_back(std::move(bind));
	}
	void addStaticVertexBuffer(std::unique_ptr<VertexBuffer> vertexBuffer) noexcept {
		assert("Attempting to add Index Buffer a second time..." && m_pVertexBuffer == nullptr);
		m_pVertexBuffer = vertexBuffer.get();
		staticBinds.push_back(std::move(vertexBuffer));
	}
	void setIndexFromStatic() noexcept {
		for (const auto& b : staticBinds) {
			if (const auto p = dynamic_cast<VertexBuffer*>(b.get())) {
				m_pVertexBuffer = p;
				return;
			}
		}
		assert("Failed to find Index Buffer among vector [staticBinds]." && m_pVertexBuffer != nullptr);
	}

};

template<class T>
std::vector<std::unique_ptr<Bindable>> DrawableBase<T>::staticBinds;