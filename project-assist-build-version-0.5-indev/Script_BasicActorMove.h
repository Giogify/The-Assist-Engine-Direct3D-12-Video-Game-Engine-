#pragma once
#include "Actor.h"
#include "Keyboard.h"

namespace Scripts {
	void doMove(Actor& actor, const Keyboard& kb) {
		for (auto& o : actor.getModel().getObjects())
			if (kb.KeyIsPressed('W')) 
				o.getSpeed().dz += 0.01f;
	}
}