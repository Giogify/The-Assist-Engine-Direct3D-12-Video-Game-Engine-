#pragma once
#include "Script_Actor.h"
#include "Script_Actor_RandomTransform.h"

#include "Script_Scene.h"

class Script_Factory {
	
public:
	
	static inline std::shared_ptr<Script_Actor> parseActorScript(const std::string& scriptName) noexcept {
		if (scriptName == "RandomTransform") return std::make_shared<Script_Actor_RandomTransform>();
	}
};


//Script_Scene* parseSceneScript(const std::string& scriptName) noexcept {
//
//}