#pragma once
#include "Model.h"
#include "Timer.h"

struct Script_Info_Actor {

	Model model{};
	Timer startTimer{};
	Timer updateTimer{};

};