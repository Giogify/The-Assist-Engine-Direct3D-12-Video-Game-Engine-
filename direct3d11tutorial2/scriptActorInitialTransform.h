#pragma once
#include "Script.h"

class ScriptActorInitialTransform : public Script {

private:

public:

	void init(Actor& actor) noexcept override {
		for (auto& o : actor.getModel().getObjects()) {
			o.getPos();
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