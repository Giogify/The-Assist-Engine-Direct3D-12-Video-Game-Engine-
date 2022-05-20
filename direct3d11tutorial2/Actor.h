#pragma once

class Actor;

#include "Timer.h"
#include "Model.h"
#include "Scriptable.h"
#include <memory>
#include <DirectXMath.h>

class Actor : public Scriptable {

private:

	// Model
	std::shared_ptr<Model> m_model{};

	// Timers
	Timer startTimer{};
	Timer updateTimer{};

public:

	Actor(GraphicsOutput& gfx, std::string& objPath);

	Model* getModel() const noexcept { return m_model.get(); }

	void update() noexcept;

	Timer& getStartTimer() noexcept { return startTimer; }
};