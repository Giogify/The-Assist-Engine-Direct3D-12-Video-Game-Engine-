#pragma once
#include "Camera.h"
#include "Actor.h"
#include <DirectXMath.h>

namespace Scripts {
	void doFollow(Camera& camera, Actor& actor) noexcept {
		if (camera.mbFollow) {
			DirectX::XMFLOAT3 actorPos{ actor.getModel().getObjects().at(0).getPos().center };
			camera.mFocus = actorPos;
			camera.mEye = { actorPos.x, actorPos.y + 5.0f, actorPos.z - 5.0f };
		}
	}
}