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
	Model m_model{};

	// Timers
	Timer startTimer{};
	Timer updateTimer{};

public:

	Actor(GraphicsOutput& gfx, std::string& objPath);

	Model& getModel() noexcept { return m_model; }

	void update() noexcept;

	Timer& getStartTimer() noexcept { return startTimer; }
};