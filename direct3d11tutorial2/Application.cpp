#include "Actor.h"
#include "Application.h"
#include "Collections.h"
#include <random>
#include <memory>
#include <iostream>
#include <iomanip>


// Top-Level Application Logic
int Application::applicationUpdate() {

	// Create Debug Information
	std::wostringstream oss = {};
	oss.setf(std::ios::fixed);
	oss << std::setprecision(2);

	const float t = mTimerStart.peek();
	oss << "[Elasped Time] " << std::fixed << t;
	oss << " [Mouse Position] (" << mWnd.mouse.getX() << "," << mWnd.mouse.getY() << ")";

	// UPDATE CODE HERE

	//wnd.getGraphicsOutput().flushBackBufferColor(0.5294f, 0.8078f, 0.9216f);
	mWnd.getGraphicsOutput().flushBackBufferColor(0.1f, 0.1f, 0.1f);
	for (auto& a : mActors) {
		a->update();
		a->getModel().draw(mWnd.getGraphicsOutput());

		for (auto& o : a->getModel().getObjects()) {
			tcb.bind(mWnd.getGraphicsOutput(), o);
		}
	}

	std::vector<char> keys{};
	std::vector<Mouse::Event> mouse{};

	while (!mWnd.kb.CharIsEmpty()) {
		keys.push_back(mWnd.kb.ReadChar());
		//std::cout << "[Key Pressed] (" << keys.back() << ")" << '\n';
	}
	while (!mWnd.mouse.isEmpty()) {
		mouse.push_back(mWnd.mouse.readEvent());
		//std::cout << "[";
		/*switch (mouse.back().getType()) {
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
		}*/
		//std::cout << '\n';
	
	}

	//if (key == 'w') {
	//    wnd.getGraphicsOutput().get
	//}

	// UPDATE CODE HERE

	// Frame Advance Mode
	for (auto& key : keys) if (key == '/') {
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

	for (auto& key : keys) if (key == '[') mFPSCap -= 1.0f;
	for (auto& key : keys) if (key == ']') mFPSCap += 1.0f;
	for (auto& key : keys) if (key == '{') mFPSCap -= 10.0f;
	for (auto& key : keys) if (key == '}') mFPSCap += 10.0f;

	if (mMaxFPS < 1000.0f / (mTimerRender.peek() * 1000.0000000f)) {
		mMaxFPS = 1000.0f / (mTimerRender.peek() * 1000.0000000f);
	}

	oss << " Highest FPS: " << mMaxFPS;

	oss << " [Frames: " << this->mFPSCap << "] " << 1000 / (mTimerRender.mark() * 1000) << '\n';

	for (auto& key : keys) if (key == '1') {
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
	}

	auto dt = mTimer.mark();
	//mWnd.getGraphicsOutput().getCamera().addYaw(20*dt);

	oss << " [Actors] " << mActors.size();

	// Camera Manipulation

	// Up
	for (auto& key : keys) if (key == 'w') mWnd.getGraphicsOutput().getCamera().translate(0.0, 0.5, 0.0);
	
	// Down
	for (auto& key : keys) if (key == 's') mWnd.getGraphicsOutput().getCamera().translate(0.0, -0.5, 0.0);
	
	// Left
	for (auto& key : keys) if (key == 'a') mWnd.getGraphicsOutput().getCamera().translate(0.5, 0.0, 0.0);
	
	// Right
	for (auto& key : keys) if (key == 'd') mWnd.getGraphicsOutput().getCamera().translate(-0.5, 0.0, 0.0);
	

	if (mIsCameraModeEnabled) {
		for (auto& e : mouse) {
			if (e.getType() == Mouse::Event::Type::Move) {
				RECT rc = mWnd.getWindowInfo().rcClient;
				auto centerX = (rc.right - rc.left) / 2;
				auto centerY = (rc.bottom - rc.top) / 2;
				if (e.getX() != centerX || e.getY() != centerY) {
					SetCursorPos(centerX + rc.left, centerY + rc.top);
				}
				auto dx = centerX - e.getX();
				auto dy = centerY - e.getY();
				mWnd.getGraphicsOutput().getCamera().addYaw(0.25f * dx);
				mWnd.getGraphicsOutput().getCamera().addPitch(0.25f * dy);
			}
		}
	}
	
	for (auto& e : mouse) if (e.getType() == Mouse::Event::Type::LMBPress) {
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
	for (auto& e : mouse) if (e.getType() == Mouse::Event::Type::WheelUp) mWnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, -0.5);
	
	// Backward
	for (auto& e : mouse) if (e.getType() == Mouse::Event::Type::WheelDown) mWnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, 0.5);

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

	// Print Debug Information to Window Title
	mWnd.setTitle(oss.str());

	for (auto& key : keys) if (key == VK_ESCAPE) return 1;

	return 0;
}

Application::~Application() {}

Application::Application() : mWnd(1280, 720, L"Window") {
	for (int i = 0; i < 1; i++) {
		mActors.push_back(std::make_unique<Actor>(mWnd.getGraphicsOutput(), *std::make_unique<std::string>("testCubeSuperDetailed")));
	}
}

int Application::applicationStart() {

	mWnd.getGraphicsOutput().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.25f, 5000.0f));
	mWnd.getGraphicsOutput().getCamera().translate(0.0f, 0.0f, 1.0f);

	while (true) {
		if (const auto msgCode = Window::handleMessages()) return *msgCode;
		if (applicationUpdate() != 0) return 0;
	}
}