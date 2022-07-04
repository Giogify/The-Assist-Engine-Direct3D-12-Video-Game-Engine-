#include "Application.h"
#include <memory>
#include <iostream>
#include <iomanip>
#include <array>
#include <fstream>

// Top-Level Application Logic
int Application::applicationUpdate() {

	float dt = mTickTimer.mark();
	
	GID::DSU::Timer BMTimer{};
	//std::cout << "Processing " << ticks << " ticks\n";
	if (doInput() != 0) return 1;
	//std::cout << BMTimer.mark() * 1.e6 << " us" << '\n';
	//if (doUpdate(dt) != 0) return 1;
	
	if (doRender() != 0) return 1;
	
	// For benchmarking the render function
	/*{
		timerBenchmark.mark();
		if (doRender() != 0) return 1;
		sum += timerBenchmark.mark() * 1000.f;
		runInstances++;
		if (mTimerStart.peek() >= 30.f) {
			std::ofstream file("benchmark.txt", std::ios::out | std::ios::app);
			file << "[Render Time] " << sum / runInstances << " ms\n";
			file.close();
			return 1;
		}
	}*/
	bool wndTitleDebug{ true };
	#if defined(_DEBUG)
	wndTitleDebug = true;
	#endif
	// Create Debug Information
	if (wndTitleDebug) {
		if (mMaxFPS < 1000.0f / (mTimerRender.peek() * 1000.0000000f)) mMaxFPS = 1000.0f / (mTimerRender.peek() * 1000.0000000f);
		std::wostringstream oss = {};
		oss.setf(std::ios::fixed);
		oss << std::setprecision(2);

		//Camera camera = GID::Render::gfx.getCamera();

		const auto t = mTimerStart.peek();
		oss << "[Elasped Time] " << std::fixed << t;
		//oss << " [Mouse Position] (" << mWnd.mouse.getX() << "," << mWnd.mouse.getY() << ")";
		oss << " Highest FPS: " << mMaxFPS;
		//oss << " [Frames: " << this->mFPSCap << "] " << 1000 / (mTimerRender.mark() * 1000) << '\n';
		oss << " [FPS: " << 1000 / (mTimerRender.mark() * 1000) << "] " <<  '\n';
		//DSU::Position temppos = mScene.getActors().at(0).getModel().getObjects().at(0).getPos();
		//DSU::Speed temp = mScene.getActors().at(0).getModel().getObjects().at(0).getSpeed();
		/*oss << " [x] " << temppos.translation.m128_f32[0] << " [y] " << temppos.translation.m128_f32[1] << " [z] " << temppos.translation.m128_f32[2];
		oss << " [dX] " << temp.deltaTranslation.m128_f32[0] << " [dY] " << temp.deltaTranslation.m128_f32[1] << " [dZ] " << temp.deltaTranslation.m128_f32[2];
		oss << ' '
			<< camera.mRotation.m128_f32[Camera::Rotation::Pitch] << ' '
			<< camera.mRotation.m128_f32[Camera::Rotation::Yaw] << ' '
			<< camera.mRotation.m128_f32[Camera::Rotation::Roll] << '\n';*/

		/*oss << " [Actors] " << mScene.getActors().size();
		oss << " [X: " << camera.mEye.x << "]"
			<< " [Y: " << camera.mEye.y << "]"
			<< " [Z: " << camera.mEye.z << "]"
			<< " [Pitch: " << camera.mPitch << "]"
			<< " [Yaw: " << camera.mYaw << "]"
			<< " [Roll: " << camera.mRoll << "]";*/

		/*for (auto& key : keys) if (key == '1') {
			mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
			mActors.shrink_to_fit();
		}
		for (auto& key : keys) if (key == '!') {
			for (int i = 0; i < 100; i++) mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
			mActors.shrink_to_fit();
		}
		for (auto& key : keys) if (key == '2') {
			mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
			mActors.shrink_to_fit();
		}
		for (auto& key : keys) if (key == '@') {
			for (int i = 0; i < 10; i++) mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
			mActors.shrink_to_fit();
		}
		for (auto& key : keys) if (key == '`') {
			auto iter = mActors.size();
			for (int i = 0; i < iter; i++) {
				mActors.pop_back();
			}
			mActors.shrink_to_fit();
		}*/

		// Print Debug Information to Window Title
		GID::GSO::WindowNS::gWnd.at((uint8_t)GID::DSU::WindowType::MAINWINDOW).get()->setTitle(oss.str());
	}
	return 0;
}

Application::~Application() {}
Application::Application() {
	using namespace GID;
	using namespace GSO;
	using namespace DSU::AssistMath;
	GSO::Util::initQuickStart();
	FAMMATRIX projectionTemp{ 
		FAMMatrixPerspectiveFovLH(
			AMConvertToRadians(GID::GSO::General::gCfgGen.gCameraAngle), 16.0f / 9.0f, 0.25f, 5000.0f
		) 
	};

	Render::setGFXProjection(GID::DSU::WindowType::MAINWINDOW, projectionTemp);

	GID::DSU::LightData light0 = {}; {
		light0.pos = { 20.0f, -35.0f, 20.0f, 1.0f };
		light0.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		light0.constAtten = 0.0f;
		light0.linAtten = 0.1f;
		light0.quadAtten = 0.0f;
		light0.isEnabled = true;
		light0.type = (int32_t)GID::DSU::LightConst::POINT_LIGHT;
	}
	GID::GSO::Scene::addLight(light0);
	GID::DSU::LightData light1 = {}; {
		light1.pos = { -20.0f, -35.0f, -20.0f, 1.0f };
		light1.color = { 1.0f, 1.0f, 1.0f, 1.0f };
		light1.constAtten = 0.0f;
		light1.linAtten = 0.1f;
		light1.quadAtten = 0.0f;
		light1.isEnabled = true;
		light1.type = (int32_t)GID::DSU::LightConst::POINT_LIGHT;
	}
	GID::GSO::Scene::addLight(light1);

	for (int i = 0; i < 1; i++)
		Scene::addActor({ "testCube" });

	int bk = 0;

	//GID::Render::gfx.mProjection = { FAMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f) };
	//GID::Scene::gActors.push_back(Actor(GID::Render::gfx, *std::make_unique<std::string>("testCube")));
	//GID::Scene::gActors.push_back(Actor(GID::Render::gfx, *std::make_unique<std::string>("plane2")));
	//mWnd.getGraphicsOutput().getCamera().mbFollow = Camera::FOLLOW;
	//mScene = { mWnd.getGraphicsOutput() };
	//mWnd.getGraphicsOutput().mCamera = { actor.getModel().getObjects().at(0).getPos().center };
	//std::string file{ "scene00" };
	//mScene = std::make_unique<Scene>(mWnd.getGraphicsOutput(), file);
	//mScene->getCamera().translate(0.0f, 0.0f, 30.0f);
}
int Application::applicationStart() {
	while (true) {
		int hr{};
		for (auto& w : GID::GSO::WindowNS::gWnd)
			if (const auto msgCode = w.get()->handleMessages()) {
				for (auto& gfx : GID::GSO::Render::gGFX) {
					gfx.gfx.flushGPU();
					CloseHandle(gfx.gfx.mhFenceEvent);
				}
				return *msgCode;
			}
		if (hr = applicationUpdate() != 0) {
			for (auto& gfx : GID::GSO::Render::gGFX) {
				gfx.gfx.flushGPU();
				CloseHandle(gfx.gfx.mhFenceEvent);
			}
			return hr;
		}
	}
}

//int Application::doPriorityInput() noexcept {
//
//	if (Input::gInput.gKB.KeyIsPressed(VK_ESCAPE)) return 1;
//	
//	//for (auto& key : gInput.gKeysChar) if (key == 'v') gfx.mVSync = !gfx.mVSync;
//	//for (auto& key : keysChar) if (key == '+') Timer::incSpeedMult();
//	//for (auto& key : keysChar) if (key == '-') Timer::decSpeedMult();
//	
//	//if (gInput.gKB.KeyIsPressed(VK_RETURN) && gInput.gKB.KeyIsPressed(VK_MENU)) gfx.setFullscreen();
//
//	for (auto& key : Input::gInput.gKeysChar) if (key == '[') mFPSCap -= 1.0f; // VK_OEM_4 is '['
//	for (auto& key : Input::gInput.gKeysChar) if (key == '{') mFPSCap -= 10.0f; // VK_OEM_4 is '['
//	for (auto& key : Input::gInput.gKeysChar) if (key == ']') mFPSCap += 1.0f; // VK_OEM_6 is ']'
//	for (auto& key : Input::gInput.gKeysChar) if (key == '}') mFPSCap += 10.0f; // VK_OEM_6 is ']'
//	for (auto& e : Input::gInput.gMouseEvents) {
//		if (e.getType() == Mouse::Event::Type::WheelUp) {
//			viewingAngle -= 10.0f;
//			//gfx.setProjection(FAMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f));
//		}
//		if (e.getType() == Mouse::Event::Type::WheelDown) {
//			viewingAngle += 10.0f;
//			//gfx.setProjection(FAMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f));
//		}
//	}
//
//	return 0;
//}

int Application::doInput() noexcept {
	bool inputDebug{ false };

	GID::GSO::Input::processInput();

	//while (!gWnd.at(GID::Window::WindowType::MAINWINDOW).kb.KeyIsEmpty()) {
	//	keys.push_back(gWnd.at(GID::Window::WindowType::MAINWINDOW).kb.ReadKey());
	//	if (inputDebug) std::cout << "[Key Pressed (Code)] (" << keys.back().getCode() << ")" << '\n';
	//}
	//while (!gWnd.at(GID::Window::WindowType::MAINWINDOW).kb.CharIsEmpty()) {
	//	keysChar.push_back(gWnd.at(GID::Window::WindowType::MAINWINDOW).kb.ReadChar());
	//	if (inputDebug) std::cout << "[Character Inputted] (" << keysChar.back() << ")" << '\n';
	//}
	//while (!gWnd.at(GID::Window::WindowType::MAINWINDOW).mouse.isEmpty()) {
	//	mouse.push_back(gWnd.at(GID::Window::WindowType::MAINWINDOW).mouse.readEvent());
	//	if (inputDebug) {
	//		std::cout << "[";
	//		switch (mouse.back().getType()) {
	//		case Mouse::Event::Type::Enter:
	//			std::cout << "Enter]";
	//			break;
	//		case Mouse::Event::Type::Exit:
	//			std::cout << "Exit]";
	//			break;
	//		case Mouse::Event::Type::LMBPress:
	//			std::cout << "LMBPress]";
	//			break;
	//		case Mouse::Event::Type::LMBRelease:
	//			std::cout << "LMBRelease]";
	//			break;
	//		case Mouse::Event::Type::Move:
	//			std::cout << "Move] (" << mouse.back().getX() << ", " << mouse.back().getY() << ")";
	//			break;
	//		case Mouse::Event::Type::RMBPress:
	//			std::cout << "RMBPress]";
	//			break;
	//		case Mouse::Event::Type::RMBRelease:
	//			std::cout << "RMBRelease]";
	//			break;
	//		case Mouse::Event::Type::WheelDown:
	//			std::cout << "WheelDown]";
	//			break;
	//		case Mouse::Event::Type::WheelUp:
	//			std::cout << "WheelUp]";
	//			break;
	//		}
	//		std::cout << '\n';
	//	}
	//}

	////gInput.gKB = gWnd.at(GID::Window::WindowType::MAINWINDOW).kb;
	//Input::gInput.gKeys = keys;
	//Input::gInput.gKeysChar = keysChar;
	////gInput.gMouse = gWnd.at(GID::Window::WindowType::MAINWINDOW).mouse;
	//Input::gInput.gMouseEvents = mouse;

	/*for (auto& c : keysChar) if (c == '1') {
		mScene.getActors().push_back(Actor(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
		mScene.getActors().back().addScript(Scripts::ScriptName::BasicGravity);
		mScene.getActors().back().addScript(Scripts::ScriptName::BasicCollision);
	}*/

	//gfx.getCamera().input(gInput.gKB, gInput.gKeys, gInput.gKeysChar, gInput.gMouse, gInput.gMouseEvents, gWnd.at(GID::Window::WindowType::MAINWINDOW).getWindowInfo().rcClient);
	//Scripts::processInputScripts(mScene, mWnd.kb, keys, keysChar, mWnd.mouse, mouse);
	//mScene.input(mWnd.kb, keys, keysChar, mWnd.mouse, mouse);
	//Scripts::doMove(mVecActors.at(0), mWnd.kb);

	return 0;
}
int Application::doUpdate(float dt) noexcept {
	//GID::Render::gfx.getCamera().update(dt);
	//mScene.update();
	//Scripts::processUpdateScripts(mScene);
	//hr = mScene->update();
	//if (hr == 2) // load new scene
	//Scripts::doFollow(mWnd.getGraphicsOutput().getCamera(), mScene.getActors().at(0));
	//Scripts::doBasicGravity(mVecActors);
	//Scripts::doBasicCollision(mVecActors);
	return 0;
}
int Application::doRender() noexcept {
	GID::GSO::Render::mainGFX().startFrame();
	uint16_t drawCt{};
	double total{};
	GID::DSU::Timer testTimer{};
	for (auto& a : GID::GSO::Scene::gActors) {
		testTimer.mark();
		a.draw();
		total += testTimer.mark() * 1000.f;
		drawCt++;
	}
	std::cout << total / drawCt << '\n';
	//mScene.draw(mWnd.getGraphicsOutput());
	//mWnd.getGraphicsOutput().doFrame();
	GID::GSO::Render::mainGFX().endFrame();
	//while ((mTimerRender.peek() * 1000.f) < (1000.f / mFPSCap));

	return 0;
}