#include "Actor.h"
#include "Collections.h"
#include "Script_RandomTransform.h"

Actor::Actor(GraphicsOutput& gfx, std::string& objPath) {

	// Start the Starting Timer (never mark this timer)
	startTimer.mark();

	// Assign model

	m_model = std::make_unique<Model>(gfx, objPath);
	
	if (objPath != "ground" && objPath != "frs3") {
		scripts_add(new Script_RandomTransform());
		scripts_init(*this);
	}
}

void Actor::update() noexcept {
	scripts_run(*this);
}