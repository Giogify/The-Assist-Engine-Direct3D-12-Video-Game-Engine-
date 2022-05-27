#pragma once
class Scene;
#include "Scene.h"

class Script_Scene {

public:

	virtual void init(Scene& scene) noexcept = 0;
	virtual void run(Scene& scene) noexcept = 0;

};