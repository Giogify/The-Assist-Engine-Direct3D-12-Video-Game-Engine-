#pragma once
#include "Timer.h"
#include "Model.h"
#include "Inputtable.h"
#include "GraphicsOutput.h"
#include "Scriptable.h"
#include "Collections.h"
#include "AssistMath.h"
#include <memory>


class Actor : public Inputtable, public Scriptable {

public:

	enum ActorGroundState {
		GROUND,
		AIR,
		WATER,
		INVALID
	};

private:

	Model mModel{};
	ActorGroundState mGroundState{ AIR };

public:

	Timer mInitTimer{};
	Timer mCurrentStateTimer{};
	std::pair<std::string, UINT> mActorID{};

public:

	Actor() = default;
	Actor(GraphicsOutput& gfx, std::string& objPath) {
		mActorID.first = objPath;
		mActorID.second = ActorID_Collection::addCollection(objPath);
		mModel = { gfx, objPath };
		mCurrentStateTimer.mark();
		mInitTimer.mark();
	}

	Model& getModel() noexcept { 
		return mModel;
	}

	int input(const Keyboard& kb, const std::vector<Keyboard::Event>& keys, const std::vector<unsigned char>& keysChar, const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept override {
		return 0;
	}
	void update() noexcept {
		mModel.update();
	}
	void draw(GraphicsOutput& gfx) noexcept {
		mModel.draw(gfx);
	}
	auto& getGroundState() noexcept {
		return mGroundState;
	}
};