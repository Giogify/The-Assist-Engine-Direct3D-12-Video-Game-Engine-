#pragma once
#include "Timer.h"
#include "Model.h"
#include "Inputtable.h"
#include "Script_Info_Actor.h"
#include "Script_Actor.h"
#include <memory>
#include <DirectXMath.h>

class Actor : public Inputtable {

public:

	typedef struct ACTOR_CREATION_DESC {
		std::string objFileName{};
		std::vector<std::shared_ptr<Script_Actor>> scripts{};
	} ACTOR_CREATION_DESC;

private:

	// model, startTimer, updateTimer
	Script_Info_Actor mActorInfo{};

	// Scripts
	std::vector<std::shared_ptr<Script_Actor>> mScripts{};

public:

	Actor(GraphicsOutput& gfx, ACTOR_CREATION_DESC& desc);

	Model& getModel() noexcept { return mActorInfo.model; }

	void input(const std::vector<char>& keys, const std::vector<Mouse::Event>& mouse) noexcept override;
	void update() noexcept;

	void addScript(std::shared_ptr<Script_Actor>& script) noexcept {
		mScripts.push_back(script);
		mScripts.back()->init(mActorInfo);
	}
};