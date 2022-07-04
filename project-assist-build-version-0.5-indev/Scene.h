#pragma once
#include "Inputtable.h"
#include "Actor.h"
#include "Camera.h"
#include "ScriptBase.h"
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <map>

class Scene : Inputtable {

private:

	std::vector<Actor> mActors{};

public:

	Scene() = default;

	Scene(GraphicsOutput& gfx) {
		
		mActors.push_back(Actor(gfx, *std::make_unique<std::string>("testCube")));
		mActors.push_back(Actor(gfx, *std::make_unique<std::string>("plane2")));

		mActors.at(0).getModel().getObjects().at(0).getSpeed().deltaTranslation.m128_f32[0] += 0.1f;

		//mActors.at(0).addScript(Scripts::ScriptName::BasicActorMove);
		mActors.at(0).addScript(Scripts::ScriptName::BasicGravity);
		mActors.at(0).addScript(Scripts::ScriptName::BasicCollision);

		//gfx.getCamera().addScript(Scripts::ScriptName::BasicCameraFollow);

	}
	
	/*int input(const Keyboard& kb,
		const std::vector<Keyboard::Event>& keys,
		const std::vector<unsigned char>& keysChar,
		const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept override {
		for (auto& actorcontainer : mActors) {
			for (auto& inputscript : actorcontainer.inputScripts) {
				Scripts::processScript(actorcontainer.actor, actorcontainer.inputScripts);
			}
		}

		return 0;
	}

	int update() noexcept {
		mCamera.update();
		for (auto& a : mActors) {
			a.update();
		}
		return 0;
	}*/

	int input(const Keyboard& kb,
		const std::vector<Keyboard::Event>& keys,
		const std::vector<unsigned char>& keysChar,
		const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept override {

		return 0;
	}

	void update() noexcept {
		for (auto& a : mActors) a.update();
	}

	void draw(GraphicsOutput& gfx) noexcept {
		for (auto& a : mActors) a.draw(gfx);
	}

	std::vector<Actor>& getActors() noexcept {
		return mActors;
	}
	//Camera& getCamera() noexcept { return mCamera; }
};