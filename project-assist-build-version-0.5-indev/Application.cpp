#include "Application.h"
#include "Collections.h"
#include "Scene.h"
#include "ScriptFactory.h"
#include "AssistMath.h"
#include "ScriptCameraFollow.h"
#include <memory>
#include <iostream>
#include <iomanip>
#include <array>
#include <fstream>

using namespace AssistMath;

// Top-Level Application Logic
int Application::applicationUpdate() {

	auto dt = mTickTimer.mark() * (double)1000 + rem;
	int ticks = std::trunc(dt);
	rem = dt - ticks;
	std::cout << "Processing " << ticks << " ticks\n";
	for (int i = 0; i < ticks; i++) {
		if (doInput() != 0) return 1;
		if (doUpdate() != 0) return 1;
	}
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
	bool wndTitleDebug{ false };
#if defined(_DEBUG)
	wndTitleDebug = true;
#endif
	// Create Debug Information
	if (wndTitleDebug) {
		if (mMaxFPS < 1000.0f / (mTimerRender.peek() * 1000.0000000f)) mMaxFPS = 1000.0f / (mTimerRender.peek() * 1000.0000000f);
		std::wostringstream oss = {};
		oss.setf(std::ios::fixed);
		oss << std::setprecision(2);

		Camera camera = mWnd.getGraphicsOutput().getCamera();

		const auto t = mTimerStart.peek();
		oss << "[Elasped Time] " << std::fixed << t;
		//oss << " [Mouse Position] (" << mWnd.mouse.getX() << "," << mWnd.mouse.getY() << ")";
		oss << " Highest FPS: " << mMaxFPS;
		oss << " [Frames: " << this->mFPSCap << "] " << 1000 / (mTimerRender.mark() * 1000) << '\n';
		DSU::Position temppos = mScene.getActors().at(0).getModel().getObjects().at(0).getPos();
		DSU::Speed temp = mScene.getActors().at(0).getModel().getObjects().at(0).getSpeed();
		oss << " [x] " << temppos.x << " [y] " << temppos.y << " [z] " << temppos.z;
		oss << " [dX] " << temp.dx << " [dY] " << temp.dy << " [dZ] " << temp.dz;
		oss << ' '
			<< camera.mPitch << ' '
			<< camera.mYaw << ' '
			<< camera.mRoll << '\n';

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
		mWnd.setTitle(oss.str());
	}
	
	return 0;
}

Application::~Application() {}
Application::Application() : mWnd(1280, 720, L"Window") {
	
	mWnd.getGraphicsOutput().mProjection = { AMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f) };

	//mWnd.getGraphicsOutput().getCamera().mbFollow = Camera::FOLLOW;

	mScene = { mWnd.getGraphicsOutput() };

	//mWnd.getGraphicsOutput().mCamera = { actor.getModel().getObjects().at(0).getPos().center };

	//std::string file{ "scene00" };
	//mScene = std::make_unique<Scene>(mWnd.getGraphicsOutput(), file);
	//mScene->getCamera().translate(0.0f, 0.0f, 30.0f);
}
int Application::applicationStart() {
	while (true) {
		if (const auto msgCode = Window::handleMessages()) return *msgCode;
		if (applicationUpdate() != 0) {
			mWnd.getGraphicsOutput().flushGPU();
			CloseHandle(mWnd.getGraphicsOutput().mhFenceEvent);
			return 0;
		}
	}
}

int Application::doPriorityInput(const Keyboard& kb, const std::vector<Keyboard::Event>& keys, const std::vector<unsigned char>& keysChar, const Mouse& mouse,
	const std::vector<Mouse::Event>& mouseEvents) noexcept {
	if (kb.KeyIsPressed(VK_ESCAPE)) return 1;
	
	for (auto& key : keysChar) if (key == 'v') mWnd.getGraphicsOutput().mVSync = !mWnd.getGraphicsOutput().mVSync;
	//for (auto& key : keysChar) if (key == '+') Timer::incSpeedMult();
	//for (auto& key : keysChar) if (key == '-') Timer::decSpeedMult();
	
	if (kb.KeyIsPressed(VK_RETURN) && kb.KeyIsPressed(VK_MENU)) mWnd.getGraphicsOutput().setFullscreen();

	for (auto& key : keysChar) if (key == '[') mFPSCap -= 1.0f; // VK_OEM_4 is '['
	for (auto& key : keysChar) if (key == '{') mFPSCap -= 10.0f; // VK_OEM_4 is '['
	for (auto& key : keysChar) if (key == ']') mFPSCap += 1.0f; // VK_OEM_6 is ']'
	for (auto& key : keysChar) if (key == '}') mFPSCap += 10.0f; // VK_OEM_6 is ']'
	for (auto& e : mouseEvents) {
		if (e.getType() == Mouse::Event::Type::WheelUp) {
			viewingAngle -= 10.0f;
			mWnd.getGraphicsOutput().setProjection(AMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f));
		}
		if (e.getType() == Mouse::Event::Type::WheelDown) {
			viewingAngle += 10.0f;
			mWnd.getGraphicsOutput().setProjection(AMMatrixPerspectiveFovLH(AMConvertToRadians(viewingAngle), 16.0f / 9.0f, 0.25f, 5000.0f));
		}
	}

	return 0;
}

int Application::doInput() noexcept {
	bool inputDebug{ false };

	std::vector<Keyboard::Event> keys{};
	std::vector<unsigned char> keysChar{};
	std::vector<Mouse::Event> mouse{};

	while (!mWnd.kb.KeyIsEmpty()) {
		keys.push_back(mWnd.kb.ReadKey());
		if (inputDebug) std::cout << "[Key Pressed (Code)] (" << keys.back().getCode() << ")" << '\n';
	}
	while (!mWnd.kb.CharIsEmpty()) {
		keysChar.push_back(mWnd.kb.ReadChar());
		if (inputDebug) std::cout << "[Character Inputted] (" << keysChar.back() << ")" << '\n';
	}
	while (!mWnd.mouse.isEmpty()) {
		mouse.push_back(mWnd.mouse.readEvent());
		if (inputDebug) {
			std::cout << "[";
			switch (mouse.back().getType()) {
			case Mouse::Event::Type::Enter:
				std::cout << "Enter]";
				break;
			case Mouse::Event::Type::Exit:
				std::cout << "Exit]";
				break;
			case Mouse::Event::Type::LMBPress:
				std::cout << "LMBPress]";
				break;
			case Mouse::Event::Type::LMBRelease:
				std::cout << "LMBRelease]";
				break;
			case Mouse::Event::Type::Move:
				std::cout << "Move] (" << mouse.back().getX() << ", " << mouse.back().getY() << ")";
				break;
			case Mouse::Event::Type::RMBPress:
				std::cout << "RMBPress]";
				break;
			case Mouse::Event::Type::RMBRelease:
				std::cout << "RMBRelease]";
				break;
			case Mouse::Event::Type::WheelDown:
				std::cout << "WheelDown]";
				break;
			case Mouse::Event::Type::WheelUp:
				std::cout << "WheelUp]";
				break;
			}
			std::cout << '\n';
		}
	}

	for (auto& c : keysChar) if (c == '1') {
		mScene.getActors().push_back(Actor(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
		mScene.getActors().back().addScript(Scripts::ScriptName::BasicGravity);
		mScene.getActors().back().addScript(Scripts::ScriptName::BasicCollision);
	}

	if (doPriorityInput(mWnd.kb, keys, keysChar, mWnd.mouse, mouse) != 0) return 1;
	mWnd.getGraphicsOutput().getCamera().input(mWnd.kb, keys, keysChar, mWnd.mouse, mouse, mWnd.getWindowInfo().rcClient);
	Scripts::processInputScripts(mScene, mWnd.kb, keys, keysChar, mWnd.mouse, mouse);
	
	//mScene.input(mWnd.kb, keys, keysChar, mWnd.mouse, mouse);

	//Scripts::doMove(mVecActors.at(0), mWnd.kb);

	return 0;
}
int Application::doUpdate() noexcept {
	mWnd.getGraphicsOutput().getCamera().update();
	mScene.update();
	Scripts::processUpdateScripts(mScene);
	//hr = mScene->update();
	//if (hr == 2) // load new scene
	Scripts::doFollow(mWnd.getGraphicsOutput().getCamera(), mScene.getActors().at(0));
	//Scripts::doBasicGravity(mVecActors);
	//Scripts::doBasicCollision(mVecActors);
	return 0;
}
int Application::doRender() noexcept {
	mWnd.getGraphicsOutput().startFrame();
	mScene.draw(mWnd.getGraphicsOutput());
	//mWnd.getGraphicsOutput().doFrame();
	mWnd.getGraphicsOutput().endFrame();
	//while ((mTimerRender.peek() * 1000.f) < (1000.f / mFPSCap));

	return 0;
}