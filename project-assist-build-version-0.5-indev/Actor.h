#pragma once
#include "Timer.h"
#include "Model.h"
#include "Inputtable.h"
#include "GraphicsOutput.h"
#include <memory>
#include <DirectXMath.h>


class Actor : public Inputtable {

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

public:

	Actor() = default;
	Actor(GraphicsOutput& gfx, std::string& objPath) {
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
	ActorGroundState& getGroundState() noexcept {
		return mGroundState;
	}
};