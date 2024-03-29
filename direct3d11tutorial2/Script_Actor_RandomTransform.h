#pragma once
#include "Script_Actor.h"
#include <iostream>
#include <random>

class Script_Actor_RandomTransform : public Script_Actor {

public:

	void init(Script_Info_Actor& actorInfo) noexcept override {
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
		std::uniform_real_distribution<float> rdist(10.0f, 20.0f);
		std::uniform_real_distribution<float> bdist(6.0f, 20.0f);

		float x = rdist(rng);
		float y = rdist(rng);
		float z = rdist(rng);
		float droll = ddist(rng);
		float dpitch = ddist(rng);
		float dyaw = ddist(rng);
		float dphi = odist(rng);
		float dtheta = odist(rng);
		float dchi = odist(rng);
		float chi = adist(rng);
		float theta = adist(rng);
		float phi = adist(rng);

		for (auto& o : actorInfo.model.getObjects()) {
			o.getPos().x = x;
			o.getPos().y = y;
			o.getPos().z = z;
			o.getSpeed().droll = droll;
			o.getSpeed().dpitch = dpitch;
			o.getSpeed().dyaw = dyaw;
			o.getSpeed().dphi = dphi;
			o.getSpeed().dtheta = dtheta;
			o.getSpeed().dchi = dchi;
		}
	}

	void run(Script_Info_Actor& actorInfo) noexcept override {

		for (auto& o : actorInfo.model.getObjects()) {
			o.getPos().roll = o.getSpeed().droll * actorInfo.startTimer.peek();
			o.getPos().pitch = o.getSpeed().dpitch * actorInfo.startTimer.peek();
			o.getPos().yaw = o.getSpeed().dyaw * actorInfo.startTimer.peek();
		}
	}

};