//#pragma once
//#include "Scene.h"
//#include "ScriptBase.h"
//#include "ScriptBasicActorMove.h"
//#include "ScriptBasicGravity.h"
//#include "ScriptBasicCollision.h"
//#include <map>
//#include <vector>
//#include <string>
//
//namespace Scripts {
//
//	void processInputScripts(Scene& scene, const Keyboard& kb,
//		const std::vector<Keyboard::Event>& keys,
//		const std::vector<unsigned char>& keysChar,
//		const Mouse& mouse,
//		const std::vector<Mouse::Event>& mouseEvents) noexcept {
//		for (auto& a : scene.getActors()) {
//			for (auto& s : a.getScripts()) {
//				switch (s) {
//				case BasicActorMove:
//					doMove(a, kb);
//					break;
//				case BasicGravity:
//					doBasicGravity(a);
//					break;
//				case BasicCollision:
//					Physics::doBasicCollision(a, scene.getActors());
//					break;
//				}
//			}
//		}
//	}
//
//	void processUpdateScripts(Scene& scene) noexcept {
//		for (int i = 0; i < scene.getActors().size(); i++) {
//
//
//
//		}
//	}
//
//	void processCameraScripts(Camera& camera) noexcept {
//		for (auto& s : camera.getScripts()) {
//
//		}
//	}
//
//}