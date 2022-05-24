#include "Actor.h"
#include "Application.h"
#include "Collections.h"
#include <random>
#include <memory>
#include <iostream>
#include <iomanip>


// Top-Level Application Logic
void Application::applicationUpdate() {

	// Create Debug Information
	std::ostringstream oss = {};
	oss.setf(std::ios::fixed);
	oss << std::setprecision(2);

	const float t = mTimerStart.peek();
	oss << "[Elasped Time] " << std::fixed << t;
	oss << " [Mouse Position] (" << mWnd.mouse.getX() << "," << mWnd.mouse.getY() << ")";

	// UPDATE CODE HERE

	//wnd.getGraphicsOutput().flushBackBufferColor(0.5294f, 0.8078f, 0.9216f);
	mWnd.getGraphicsOutput().flushBackBufferColor(0.0f, 0.0f, 0.0f);
	for (auto& a : mActors) {
		a->update();
		a->getModel().draw(mWnd.getGraphicsOutput());

		for (auto& o : a->getModel().getObjects()) {
			tcb.bind(mWnd.getGraphicsOutput(), o);
		}
	}

	auto key = mWnd.kb.ReadChar();
	auto mouse = mWnd.mouse.readEvent();

	if (key == '+') mWnd.getGraphicsOutput().changeDepth(0.05);

	if (key == '-') mWnd.getGraphicsOutput().changeDepth(-0.05);

	oss << " [Depth] " << mWnd.getGraphicsOutput().getDepth();

	//if (key == 'w') {
	//    wnd.getGraphicsOutput().get
	//}

	// UPDATE CODE HERE

	// Frame Advance Mode
	if (key == '/') {
		mIsFrameAdvanceEnabled = true;
	}
	while (mIsFrameAdvanceEnabled) {
		Window::handleMessages();
		while (!mWnd.kb.CharIsEmpty()) {
			auto key = mWnd.kb.ReadChar();
			if (key == '.') {
				goto goto1;
			}
			else if (key == '/') {
				mIsFrameAdvanceEnabled = false;
			}
		}
	}
	goto1:

	mWnd.getGraphicsOutput().endFrame();

	while ((mTimerRender.peek() * 1000.0000000f) < (1000.0000000f / mFPSCap));

	if (key == '[') mFPSCap -= 1.0f;
	if (key == ']') mFPSCap += 1.0f;
	if (key == '{') mFPSCap -= 10.0f;
	if (key == '}') mFPSCap += 10.0f;

	if (mMaxFPS < 1000.0f / (mTimerRender.peek() * 1000.0000000f)) {
		mMaxFPS = 1000.0f / (mTimerRender.peek() * 1000.0000000f);
	}

	oss << " Highest FPS: " << mMaxFPS;

	oss << " [Frames: " << this->mFPSCap << "] " << 1000 / (mTimerRender.mark() * 1000) << '\n';

	if (key == '1') {
		mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
		mActors.shrink_to_fit();
	}
	if (key == '!') {
		for (int i = 0; i < 100; i++) mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
		mActors.shrink_to_fit();
	}
	if (key == '2') {
		mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
		mActors.shrink_to_fit();
	}
	if (key == '@') {
		for (int i = 0; i < 10; i++) mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
		mActors.shrink_to_fit();
	}
	if (key == '`') {
		auto iter = mActors.size();
		for (int i = 0; i < iter; i++) {
			mActors.pop_back();
		}
		mActors.shrink_to_fit();
	}

	auto dt = mTimer.mark();
	//mWnd.getGraphicsOutput().getCamera().addYaw(20*dt);

	oss << " [Actors] " << mActors.size();

	// Camera Manipulation

	// Up
	if (key == 'w') mWnd.getGraphicsOutput().getCamera().translate(0.0, 0.5, 0.0);
	
	// Down
	if (key == 's') mWnd.getGraphicsOutput().getCamera().translate(0.0, -0.5, 0.0);
	
	// Left
	if (key == 'a') mWnd.getGraphicsOutput().getCamera().translate(0.5, 0.0, 0.0);
	
	// Right
	if (key == 'd') mWnd.getGraphicsOutput().getCamera().translate(-0.5, 0.0, 0.0);
	

	if (mIsCameraModeEnabled) {
		if (mouse.getType() == Mouse::Event::Type::Move) {
			std::vector<Mouse::Event> events;
			events.push_back(mouse);
			while (!mWnd.mouse.isEmpty()) {
				events.push_back(mWnd.mouse.readEvent());
			}
			while (!events.empty()) {
				auto event = events.back();
				RECT rc = mWnd.getWindowInfo().rcClient;
				auto centerX = (rc.right - rc.left) / 2;
				auto centerY = (rc.bottom - rc.top) / 2;
				SetCursorPos(centerX + rc.left, centerY + rc.top);
				auto dx = centerX - event.getX();
				auto dy = centerY - event.getY();
				mWnd.getGraphicsOutput().getCamera().addYaw(0.25f * dx);
				mWnd.getGraphicsOutput().getCamera().addPitch(0.25f * dy);
				events.pop_back();
			}
		}
	}
	
	if (mouse.getType() == Mouse::Event::Type::LMBPress) {
		mIsCameraModeEnabled = !mIsCameraModeEnabled;
		ShowCursor(!mIsCameraModeEnabled);
		RECT rc{ mWnd.getWindowInfo().rcClient };
		if (mIsCameraModeEnabled) ClipCursor(&rc);
		else { ClipCursor(NULL); }
		auto centerX = (rc.right - rc.left) / 2;
		auto centerY = (rc.bottom - rc.top) / 2;
		SetCursorPos(centerX + rc.left, centerY + rc.top);
	}

	// Forward
	if (mouse.getType() == Mouse::Event::Type::WheelUp) mWnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, -0.5);
	
	// Backward
	if (mouse.getType() == Mouse::Event::Type::WheelDown) mWnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, 0.5);

	DirectX::XMFLOAT4X4 proj = {};
	DirectX::XMStoreFloat4x4(&proj, mWnd.getGraphicsOutput().getCamera().getMatrix());
	/*oss << " [Camera Matrix] "
		<< proj._11 << " " << proj._12 << " " << proj._13 << " " << proj._14 << " "
		<< proj._21 << " " << proj._22 << " " << proj._23 << " " << proj._24 << " "
		<< proj._31 << " " << proj._32 << " " << proj._33 << " " << proj._34 << " "
		<< proj._41 << " " << proj._42 << " " << proj._43 << " " << proj._44;*/

	oss << " [X: " << mWnd.getGraphicsOutput().getCamera().mEye.x << "]"
		<< " [Y: " << mWnd.getGraphicsOutput().getCamera().mEye.y << "]"
		<< " [Z: " << mWnd.getGraphicsOutput().getCamera().mEye.z << "]"
		<< " [Pitch: " << mWnd.getGraphicsOutput().getCamera().mPitch << "]"
		<< " [Yaw: " << mWnd.getGraphicsOutput().getCamera().mYaw << "]"
		<< " [Roll: " << mWnd.getGraphicsOutput().getCamera().mRoll << "]";

	if (mIsCameraModeEnabled) {
		oss << " [Camera Mouse Control Enabled]";
	}

	// Print Debug Information to Window Title
	mWnd.setTitle(oss.str());
}

Application::~Application() {}

Application::Application() : mWnd(1280, 720, L"Window") {
	for (int i = 0; i < 1; i++) {
		mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
	}
}

int Application::applicationStart() {

	mWnd.getGraphicsOutput().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.25f, 5000.0f));
	mWnd.getGraphicsOutput().getCamera().translate(0.0, 11.0, 8.5);

	while (true) {
		if (const auto msgCode = Window::handleMessages()) return *msgCode;
		applicationUpdate();
	}
}