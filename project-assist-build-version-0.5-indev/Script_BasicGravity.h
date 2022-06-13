#pragma once
#include "Actor.h"
#include <vector>

namespace Scripts {
	void doBasicGravity(std::vector<Actor>& actors) {
		for (auto& a : actors) {
			if (a.getGroundState() == Actor::ActorGroundState::AIR) {
				if (a.getModel().getObjects().at(0).getSpeed().dy > -300.0f) {
					for (auto& o : a.getModel().getObjects()) {
						const float dt = a.mCurrentStateTimer.peek();
						o.getSpeed().dy = -9.81f * dt * dt;
					}
				}
			}
		}
	}
}