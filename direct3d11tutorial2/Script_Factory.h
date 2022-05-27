#pragma once
#include "Script_Actor.h"
#include "Script_Actor_RandomTransform.h"

#include "Script_Scene.h"


Script_Actor* parseActorScript(const std::string& scriptName) noexcept {
	if (scriptName == "Script_Actor_RandomTransform") {
		return new Script_Actor_RandomTransform;
	}
}

Script_Scene* parseSceneScript(const std::string& scriptName) noexcept {

}