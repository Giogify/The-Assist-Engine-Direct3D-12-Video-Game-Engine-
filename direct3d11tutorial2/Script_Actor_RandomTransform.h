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
		std::uniform_real_distribution<float> odist(0.0f, 3.1415f * 0.1f);
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

		std::cout << r << '\n'
			<< r << '\n'
			<< droll << '\n'
			<< dpitch << '\n'
			<< dyaw << '\n'
			<< dphi << '\n'
			<< dtheta << '\n'
			<< dchi << '\n'
			<< chi << '\n'
			<< theta << '\n'
			<< phi << '\n';

		for (auto& o : actorInfo.model.getObjects()) {
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

	void run(Script_Info_Actor& actorInfo) noexcept override {

		for (auto& o : actorInfo.model.getObjects()) {
			std::cout << "Start" << '\n';
			o.getPos().roll = o.getSpeed().droll * actorInfo.startTimer.peek();
			o.getPos().pitch = o.getSpeed().dpitch * actorInfo.startTimer.peek();
			std::cout << o.getPos().pitch << '\n';
			o.getPos().yaw = o.getSpeed().dyaw * actorInfo.startTimer.peek();
			o.getPos().phi = o.getSpeed().dphi * actorInfo.startTimer.peek();
			o.getPos().theta = o.getSpeed().dtheta * actorInfo.startTimer.peek();
			o.getPos().chi = o.getSpeed().dchi * actorInfo.startTimer.peek();
		}
	}

};