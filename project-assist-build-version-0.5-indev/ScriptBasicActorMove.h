#pragma once
#include "Actor.h"
#include "Keyboard.h"
#include "ScriptBase.h"

namespace Scripts {

	void doMove(Actor& actor, const Keyboard& kb) {
		for (auto& o : actor.getModel().getObjects()) {
			if (kb.KeyIsPressed('W'))
				o.getSpeed().deltaTranslation.m128_f32[0] += 0.0001f;
			else o.getSpeed().deltaTranslation.m128_f32[0] /= 1.01f;

			/*if (kb.KeyIsPressed('S'))
				o.getSpeed().dz -= 0.0001f;
			else o.getSpeed().dz /= 1.01f;

			if (kb.KeyIsPressed('A'))
				o.getSpeed().dx -= 0.0001f;
			else o.getSpeed().dx /= 1.01f;

			if (kb.KeyIsPressed('D'))
				o.getSpeed().dx += 0.0001f;
			else o.getSpeed().dx /= 1.01f;

			if (kb.KeyIsPressed(VK_SPACE))
				o.getSpeed().dy += 0.0001f;*/
		}
	}

}