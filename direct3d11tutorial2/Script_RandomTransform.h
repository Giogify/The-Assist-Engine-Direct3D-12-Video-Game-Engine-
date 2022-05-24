#pragma once
#include "Script.h"
#include <random>

class Script_RandomTransform : public Script {

private:

public:

	void init(Actor& actor) noexcept override {
		std::mt19937 rng(std::random_device{}());
		std::uniform_real_distribution<float> adist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> ddist(0.0f, 3.1415f * 2.0f);
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.3f);
		std::uniform_real_distribution<float> rdist(10.0f, 20.0f);
		std::uniform_real_distribution<float> bdist(6.0f, 20.0f);

		float r = rdist(rng);
		float droll = ddist(rng);
		float dpitch = ddist(rng);
		float dyaw = ddist(rng);
		float dphi = odist(rng);
		float dtheta = odist(rng);
		float dchi = odist(rng);
		float chi = adist(rng);
		float theta = adist(rng);
		float phi = adist(rng);

		for (auto& o : actor.getModel().getObjects()) {
			o.getPos().r = r;
			o.getPos().chi = chi;
			o.getPos().theta = theta;
			o.getPos().phi = phi;
			o.getSpeed().droll = droll;
			o.getSpeed().dpitch = dpitch;
			o.getSpeed().dyaw = dyaw;
			o.getSpeed().dphi = dphi;
			o.getSpeed().dtheta = dtheta;
			o.getSpeed().dchi = dchi;
		}
	}

	void run(Actor& actor) noexcept override {

		for (auto& o : actor.getModel().getObjects()) {
			o.getPos().roll = o.getSpeed().droll * actor.getStartTimer().peek();
			o.getPos().pitch = o.getSpeed().dpitch * actor.getStartTimer().peek();
			o.getPos().yaw = o.getSpeed().dyaw * actor.getStartTimer().peek();
			o.getPos().phi = o.getSpeed().dphi * actor.getStartTimer().peek();
			o.getPos().theta = o.getSpeed().dtheta * actor.getStartTimer().peek();
			o.getPos().chi = o.getSpeed().dchi * actor.getStartTimer().peek();
		}
	}

};