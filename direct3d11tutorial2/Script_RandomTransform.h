#pragma once
#include "Script.h"
#include <random>

class Script_RandomTransform : public Script {

private:

public:

	virtual void init(Actor& actor) noexcept override {
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
			o.getPos().m_r = r;
			o.getPos().m_chi = chi;
			o.getPos().m_theta = theta;
			o.getPos().m_phi = phi;
			o.getSpeed().m_droll = droll;
			o.getSpeed().m_dpitch = dpitch;
			o.getSpeed().m_dyaw = dyaw;
			o.getSpeed().m_dphi = dphi;
			o.getSpeed().m_dtheta = dtheta;
			o.getSpeed().m_dchi = dchi;
		}
	}

	virtual void run(Actor& actor) noexcept override {

		for (auto& o : actor.getModel().getObjects()) {
			o.getPos().m_roll = o.getSpeed().m_droll * actor.getStartTimer().peek();
			o.getPos().m_pitch = o.getSpeed().m_dpitch * actor.getStartTimer().peek();
			o.getPos().m_yaw = o.getSpeed().m_dyaw * actor.getStartTimer().peek();
			o.getPos().m_phi = o.getSpeed().m_dphi * actor.getStartTimer().peek();
			o.getPos().m_theta = o.getSpeed().m_dtheta * actor.getStartTimer().peek();
			o.getPos().m_chi = o.getSpeed().m_dchi * actor.getStartTimer().peek();
		}
	}

};