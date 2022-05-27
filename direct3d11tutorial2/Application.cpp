#include "Application.h"
#include "Collections.h"
#include "Scene.h"
#include <random>
#include <memory>
#include <iostream>
#include <iomanip>


// Top-Level Application Logic
int Application::applicationUpdate() {

	if (doInput() != 0) return 1;
	if (doUpdate() != 0) return 1;
	if (doRender() != 0) return 1;

	/*renderTimer.mark();

	if (doInput() != 0) return 1;
	inputTimes.push_back(renderTimer.mark());
	inputcount++;
	if (doUpdate() != 0) return 1;
	updateTimes.push_back(renderTimer.mark());
	updatecount++;
	if (doRender() != 0) return 1;
	renderTimes.push_back(renderTimer.mark());
	rendercount++;
	if (oneSecondTimer.peek() >= 5.0f) {

		float first{};
		float second{};
		float third{};

		for (int i = 0; i < inputTimes.size(); i++) {
			first += inputTimes.at(i);
		}
		for (int i = 0; i < updateTimes.size(); i++) {
			second += updateTimes.at(i);
		}
		for (int i = 0; i < renderTimes.size(); i++) {
			third += renderTimes.at(i);
		}

		std::cout << "Input Time: " << first / inputTimes.size() << " ms" << '\n';
		std::cout << "Update Time: " << second / updateTimes.size() << " ms" << '\n';
		std::cout << "Render Time: " << third / renderTimes.size() << " ms" << '\n';

		inputTimes.clear();
		updateTimes.clear();
		renderTimes.clear();
		inputcount = 0;
		updatecount = 0;
		rendercount = 0;

		oneSecondTimer.mark();
	}*/

	if (mMaxFPS < 1000.0f / (mTimerRender.peek() * 1000.0000000f)) {
		mMaxFPS = 1000.0f / (mTimerRender.peek() * 1000.0000000f);
	}

	// Create Debug Information
	std::wostringstream oss = {};
	oss.setf(std::ios::fixed);
	oss << std::setprecision(2);

	const auto t = mTimerStart.peek();
	oss << "[Elasped Time] " << std::fixed << t;
	oss << " [Mouse Position] (" << mWnd.mouse.getX() << "," << mWnd.mouse.getY() << ")";
	oss << " Highest FPS: " << mMaxFPS;
	oss << " [Frames: " << this->mFPSCap << "] " << 1000 / (mTimerRender.mark() * 1000) << '\n';
	oss << " [Actors] " << mScene->getActors().size();

	oss << " [X: " << mWnd.getGraphicsOutput().getCamera().mEye.x << "]"
		<< " [Y: " << mWnd.getGraphicsOutput().getCamera().mEye.y << "]"
		<< " [Z: " << mWnd.getGraphicsOutput().getCamera().mEye.z << "]"
		<< " [Pitch: " << mWnd.getGraphicsOutput().getCamera().mPitch << "]"
		<< " [Yaw: " << mWnd.getGraphicsOutput().getCamera().mYaw << "]"
		<< " [Roll: " << mWnd.getGraphicsOutput().getCamera().mRoll << "]";

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

	return 0;
}

Application::~Application() {}

Application::Application() : mWnd(1280, 720, L"Window") {
	

	mWnd.getGraphicsOutput().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.25f, 5000.0f));
	mWnd.getGraphicsOutput().getCamera().translate(0.0f, 0.0f, 30.0f);

	std::string file{ "scene00" };
	mScene = std::make_unique<Scene>(mWnd.getGraphicsOutput(), file);
}

int Application::applicationStart() {
	while (true) {
		if (const auto msgCode = Window::handleMessages()) return *msgCode;
		if (applicationUpdate() != 0) return 0;
	}
}

int Application::doPriorityInput(std::vector<char> keys, std::vector<Mouse::Event> mouse) noexcept {
	for (auto& key : keys) {
		if (key == VK_ESCAPE) return 1;
		if (key == '[') mFPSCap -= 1.0f;
		if (key == ']') mFPSCap += 1.0f;
		if (key == '{') mFPSCap -= 10.0f;
		if (key == '}') mFPSCap += 10.0f;
	}
	return 0;
}

int Application::doInput() noexcept {
	std::vector<char> keys{};
	std::vector<Mouse::Event> mouse{};

	while (!mWnd.kb.CharIsEmpty()) {
		keys.push_back(mWnd.kb.ReadChar());
		//std::cout << "[Key Pressed] (" << keys.back() << ")" << '\n';
	}
	while (!mWnd.mouse.isEmpty()) {
		mouse.push_back(mWnd.mouse.readEvent());
		/*std::cout << "[";
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
		std::cout << '\n';*/

	}
	
	if (doPriorityInput(keys, mouse) != 0) return 1;
	mWnd.getGraphicsOutput().getCamera().input(keys, mouse, mWnd.getWindowInfo().rcClient);
	mScene->input(keys, mouse);

	return 0;
}

int Application::doUpdate() noexcept {
	unsigned int hr = mScene->update();
	if (hr == 1) return 1;
	if (hr == 2) {
		// load new scene
	}

	return 0;
}

int Application::doRender() noexcept {

	mWnd.getGraphicsOutput().flushBackBufferColor(0.5294f, 0.8078f, 0.9216f); //sky blue
	//mWnd.getGraphicsOutput().flushBackBufferColor(0.0f, 0.0f, 0.0f); //black

	mScene->draw(mWnd.getGraphicsOutput());

	for (auto& a : mScene->getActors()) {
		for (auto& o : a.getModel().getObjects()) {
			tcb.bind(mWnd.getGraphicsOutput(), o);
		}
	}

	mWnd.getGraphicsOutput().endFrame();

	while ((mTimerRender.peek() * 1000.f) < (1000.f / mFPSCap));

	return 0;
}