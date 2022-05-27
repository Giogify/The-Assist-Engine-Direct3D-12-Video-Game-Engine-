#pragma once
#include "Inputtable.h"
#include "Script_Factory.h"
#include "Actor.h"
#include <iostream>
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

	std::vector<Actor> mActors{};

public:

	Scene(GraphicsOutput& gfx, std::string& file) {

		// The current line
		std::string nextline{};

		// The scene file to read from
		std::ifstream sceneFile{};
		
		// Append file extension, open file
		sceneFile.open(file + ".txt");

		// Check if the file is readable
		if (sceneFile.is_open()) {

			// Create string for storing file name
			std::string tempObjectFileName{};

			// Create unsigned int for storing object count
			unsigned int objectCount{};

			// Create vector for storing scripts
			std::vector<std::string> tempScripts{};

			// Check if the file has a nextline
			while (sceneFile) {

				// Store the next line
				std::getline(sceneFile, nextline);

				if (nextline.starts_with("file ")) {
					nextline.erase(0u, 5u);
					tempObjectFileName = nextline;
				}
				else if (nextline.starts_with("script ")) {
					nextline.erase(0u, 7u);
					tempScripts.push_back(nextline);
				}
				else if (nextline.starts_with("count ")) {
					nextline.erase(0u, 6u);
					objectCount = std::stoi(nextline);
				}
				else if (nextline.starts_with("end")) {

					nextline.erase(0u, 3u);

					if (objectCount == 0u) objectCount = 1u;
					for (unsigned int i = 0u; i < objectCount; i++) {

						Actor::ACTOR_CREATION_DESC actorDesc{};
						actorDesc.objFileName = tempObjectFileName;
						for (auto& s : tempScripts) actorDesc.scripts.push_back(Script_Factory::parseActorScript(s));
						std::cout << i << '\n';
						mActors.push_back(*std::make_unique<Actor>(gfx, actorDesc));
					}

					// Reset temporary data
					tempObjectFileName.clear();
					objectCount = 0u;
					tempScripts.clear();

				}
			}
		}
	}
	
	void input(const std::vector<char>& keys, const std::vector<Mouse::Event>& mouse) noexcept override {
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