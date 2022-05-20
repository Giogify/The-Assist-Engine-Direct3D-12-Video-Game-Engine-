#pragma once
class Scriptable;
#include "Script.h"

class Scriptable {

protected:

	std::vector<Script*> scripts{};

public:

	void scripts_init(Actor& actor) noexcept {
		for (auto& s : scripts) {
			s->init(actor);
		}
	}

	void scripts_run(Actor& actor) noexcept {
		for (auto& s : scripts) {
			s->run(actor);
		}
	}

	void scripts_add(Script* script) noexcept {
		scripts.push_back(script);
	}

};