#pragma once
#include "Camera.h"
#include "Actor.h"
#include "AssistMath.h"

namespace Scripts {

	void doFollow(Camera& camera, Actor& actor) noexcept {
		using namespace AssistMath;
		if (camera.mbFollow) {
			FAMVECTOR actorPos{ actor.getModel().getObjects().at(0).getPos().center };
			camera.mFocus = actorPos;
			camera.mEye = { actorPos.m128_f32[0], actorPos.m128_f32[1] + 5.0f, actorPos.m128_f32[2] - 5.0f};
		}
	}
}