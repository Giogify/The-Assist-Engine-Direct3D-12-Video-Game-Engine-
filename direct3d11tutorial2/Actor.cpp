#include "Actor.h"
#include "Collections.h"
//test
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

void Actor::input(std::vector<char>& keys, std::vector<Mouse::Event>& mouse) noexcept {

}

void Actor::update() noexcept {
	for (auto& s : mScripts) s->run(mActorInfo);
}