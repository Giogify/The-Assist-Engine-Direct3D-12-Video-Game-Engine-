#pragma once
#include "Inputtable.h"
#include "Script_Actor_RandomTransform.h"
#include "Actor.h"
#include <memory>
#include <fstream>
#include <map>

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

	std::vector<Actor> mActors{};

public:

	Scene(GraphicsOutput& gfx) {

		std::string file{ "testCube" };
		//mActors.push_back(*new Actor(gfx, file));

		std::vector<Script_Actor*> tempScripts{};
		tempScripts.push_back(new Script_Actor_RandomTransform());

		Actor::ACTOR_CREATION_DESC actor = {};
		actor.objFileName = *std::make_unique<std::string>("testCube");
		actor.scripts = tempScripts;

		mActors.push_back(*std::make_unique<Actor>(gfx, actor));
	};
	Scene(GraphicsOutput& gfx, std::string& file) {
		
		// The current line
		std::string nextline{};

		// The scene file to read from
		std::ifstream sceneFile{};
		
		// Append file extension, open file
		sceneFile.open(file + ".txt");

		// Check if the file is readable
		if (sceneFile.is_open()) {

			// Check if the file has a nextline
			while (sceneFile) {

				// Create string for storing file name
				std::string tempObjectFileName{};

				// Create unsigned int for storing object count
				unsigned int objectCount{};

				// Create vector for storing scripts
				std::vector<Script_Actor*> tempScripts{};

				// Store the next line
				std::getline(sceneFile, nextline);

				// If the current line starts with "actor "
				if (nextline.starts_with("actor ")) {

					// Instantiate a new actor
					mActors.push_back(*std::make_unique<Actor>());
				}
			}
		}
	}
	
	void input(std::vector<char>& keys, std::vector<Mouse::Event>& mouse) noexcept override {
		for (auto& a : mActors) a.input(keys, mouse);
	}

	int update() noexcept {
		for (auto& a : mActors) {
			a.update();
		}
		return 0;
	}

	void draw(GraphicsOutput& gfx) noexcept {
		for (auto& a : mActors) a.getModel().draw(gfx);
	}

	std::vector<Actor>& getActors() noexcept {
		return mActors;
	}
};