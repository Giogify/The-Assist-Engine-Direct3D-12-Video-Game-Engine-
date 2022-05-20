#pragma once
class Script;
#include "Actor.h"

class Script {

public:

	virtual void init(Actor& actor) noexcept = 0;
	virtual void run(Actor& actor) noexcept = 0;

};