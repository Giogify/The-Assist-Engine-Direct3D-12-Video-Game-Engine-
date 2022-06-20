#pragma once
#include "Camera.h"
#include "Actor.h"
#include "AssistMath.h"

namespace Scripts {

	void doFollow(Camera& camera, Actor& actor) noexcept {
		using namespace AssistMath;
		if (camera.mbFollow) {
			AMDOUBLE3 actorPos{ actor.getModel().getObjects().at(0).getPos().center };
			camera.mFocus = actorPos;
			camera.mEye = { actorPos.x, actorPos.y + 5.0f, actorPos.z - 5.0f };
		}
	}
}