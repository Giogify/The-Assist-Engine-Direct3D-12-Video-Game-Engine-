#include "Actor.h"
#include "Collections.h"
#include <iostream>

Actor::Actor(GraphicsOutput& gfx, ACTOR_CREATION_DESC& desc) {

	// Start the Starting Timer (never mark this timer)
	mActorInfo.startTimer.mark();

	// Assign model
	mActorInfo.model = { gfx, desc.objFileName };

	for (auto& s : desc.scripts) {
		mScripts.push_back(s);
		mScripts.back()->init(mActorInfo);
	}
}

int Actor::input(const Keyboard& kb,
	const std::vector<Keyboard::Event>& keys,
	const std::vector<unsigned char>& keysChar,
	const Mouse& mouse,
	const std::vector<Mouse::Event>& mouseEvents) noexcept {

	return 0;
}

void Actor::update() noexcept {
	for (auto& s : mScripts) s->run(mActorInfo);
}