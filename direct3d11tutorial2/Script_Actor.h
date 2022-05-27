#pragma once
#include "Script_Info_Actor.h"

class Script_Actor {

public:

	virtual void init(Script_Info_Actor& actorInfo) noexcept = 0;
	virtual void run(Script_Info_Actor& actorInfo) noexcept = 0;

};