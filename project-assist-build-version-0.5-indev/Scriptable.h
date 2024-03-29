#pragma once
#include "ScriptBase.h"
#include <vector>

namespace GID {
	namespace Scripts {
		struct Scriptable {

			std::vector<Scripts::ScriptName> mScripts{};

			void addScript(Scripts::ScriptName name) noexcept {
				std::vector<Scripts::ScriptName>::iterator newend;
				newend = std::remove(mScripts.begin(), mScripts.end(), name);
				mScripts.push_back(name);
			}

			std::vector<Scripts::ScriptName>& getScripts() noexcept {
				return mScripts;
			}

			void removeScript(Scripts::ScriptName name) noexcept {
				std::vector<Scripts::ScriptName>::iterator newend;
				newend = std::remove(mScripts.begin(), mScripts.end(), name);
			}

			void clearScripts() noexcept {
				mScripts.clear();
			}

		};
	}
}