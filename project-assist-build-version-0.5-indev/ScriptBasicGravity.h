#pragma once
#include "Actor.h"

namespace Scripts {
	void doBasicGravity(Actor& actor) {
		if (actor.getGroundState() == Actor::ActorGroundState::AIR) {
			if (actor.getModel().getObjects().at(0).getSpeed().dy > -53.6448f / 1000.f) {
				for (auto& o : actor.getModel().getObjects()) {
					o.getSpeed().dy += -9.81f * 0.001f * 0.001f;
				}
			}
		}
	}
}