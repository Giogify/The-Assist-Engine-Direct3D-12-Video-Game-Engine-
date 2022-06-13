#pragma once
#include "Inputtable.h"
#include "Actor.h"
#include "Camera.h"
#include <iostream>
#include <string_view>
#include <memory>
#include <fstream>

class Scene : public Inputtable {

public:

	enum GAMEOBJ_TYPE {
		SCENE,
		INPUT,
		ACTOR,
		INVALID
	};

	typedef struct GAMEOBJ_GEN_DESC {
		std::string name{};
		GAMEOBJ_TYPE type{};
	} GAMEOBJ_GEN_DESC;

private:

	Camera mCamera{};
	std::vector<Actor> mActors{};

public:

	Scene(GraphicsOutput& gfx, std::string& file) {
		
	}
	
	int input(const Keyboard& kb,
		const std::vector<Keyboard::Event>& keys,
		const std::vector<unsigned char>& keysChar,
		const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept override {
		for (auto& a : mActors) a.input(kb, keys, keysChar, mouse, mouseEvents);

		return 0;
	}

	int update() noexcept {
		for (auto& a : mActors) {
			a.update();
		}
		return 0;
	}

	void draw(GraphicsOutput& gfx) noexcept {
		//for (auto& a : mActors) a.getModel().draw(gfx);
	}

	std::vector<Actor>& getActors() noexcept {
		return mActors;
	}
	Camera& getCamera() noexcept { return mCamera; }
};