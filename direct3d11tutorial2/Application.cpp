#include "Application.h"
#include "Actor.h"
#include <random>
#include <memory>
#include <iostream>
#include <iomanip>
#include "Collections.h"

// Top-Level Application Logic
void Application::applicationUpdate() {

	// Create Debug Information
	std::ostringstream oss = {};
	oss.setf(std::ios::fixed);
	oss << std::setprecision(2);

	const float t = startTimer.peek();
	oss << "[Elasped Time] " << std::fixed << t;
	oss << " [Mouse Position] (" << wnd.mouse.getX() << "," << wnd.mouse.getY() << ")";

	// UPDATE CODE HERE

	wnd.getGraphicsOutput().flushBackBufferColor(0.5294f, 0.8078f, 0.9216f);
	//wnd.getGraphicsOutput().flushBackBufferColor(0.0f, 0.0f, 0.0f);
	for (auto& a : actors) {
		a->update();
		a->getModel()->draw(wnd.getGraphicsOutput());
	}

	auto key = wnd.kb.ReadChar();
	auto mouse = wnd.mouse.readEvent();

	if (key == '+') wnd.getGraphicsOutput().changeDepth(0.05);

	if (key == '-') wnd.getGraphicsOutput().changeDepth(-0.05);

	oss << " [Depth] " << wnd.getGraphicsOutput().getDepth();

	//if (key == 'w') {
	//    wnd.getGraphicsOutput().get
	//}

	// UPDATE CODE HERE

	// Frame Advance Mode
	if (key == '/') {
		m_isFrameAdvance = true;
	}

	while (m_isFrameAdvance) {
		Window::handleMessages();
		while (!wnd.kb.CharIsEmpty()) {
			auto key = wnd.kb.ReadChar();
			if (key == '.') {
				goto goto1;
			}
			else if (key == '/') {
				m_isFrameAdvance = false;
			}
		}
	}

	goto1:

	wnd.getGraphicsOutput().endFrame();

	while ((renderTimer.peek() * 1000.0000000f) < (1000.0000000f / FPS));

	if (key == '[') {
		FPS -= 1.0f;
	}
	if (key == ']') {
		FPS += 1.0f;
	}
	if (key == '{') {
		FPS -= 10.0f;
	}
	if (key == '}') {
		FPS += 10.0f;
	}

	if (maxFPS < 1000.0f / (renderTimer.peek() * 1000.0000000f)) {
		maxFPS = 1000.0f / (renderTimer.peek() * 1000.0000000f);
	}

	oss << " Highest FPS: " << maxFPS;

	oss << " [Frames: " << this->FPS << "] " << 1000 / (renderTimer.mark() * 1000) << '\n';

	if (key == '1') {
		actors.push_back(std::make_unique<Actor>(wnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
		actors.shrink_to_fit();
	}
	if (key == '!') {
		for (int i = 0; i < 100; i++) actors.push_back(std::make_unique<Actor>(wnd.getGraphicsOutput(), *std::make_unique<std::string>("testCube")));
		actors.shrink_to_fit();
	}
	if (key == '2') {
		actors.push_back(std::make_unique<Actor>(wnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
		actors.shrink_to_fit();
	}
	if (key == '@') {
		for (int i = 0; i < 10; i++) actors.push_back(std::make_unique<Actor>(wnd.getGraphicsOutput(), *std::make_unique<std::string>("nanosuit")));
		actors.shrink_to_fit();
	}
	if (key == '`') {
		auto iter = actors.size();
		for (int i = 0; i < iter; i++) {
			actors.pop_back();
		}
		actors.shrink_to_fit();
	}

	oss << " [Actors] " << actors.size() << " " << actors.capacity();

	// Camera Manipulation

	// Up
	if (key == 'w') wnd.getGraphicsOutput().getCamera().translate(0.0, -0.5, 0.0);
	
	// Down
	if (key == 's') wnd.getGraphicsOutput().getCamera().translate(0.0, 0.5, 0.0);
	
	// Left
	if (key == 'a') wnd.getGraphicsOutput().getCamera().translate(0.5, 0.0, 0.0);
	
	// Right
	if (key == 'd') wnd.getGraphicsOutput().getCamera().translate(-0.5, 0.0, 0.0);
	
	// Rotate Left
	if (key == 'q') wnd.getGraphicsOutput().getCamera().addHeading(-2);

	// Rotate Right
	if (key == 'e') wnd.getGraphicsOutput().getCamera().addHeading(2);

	// Rotate Up
	if (key == 'r') wnd.getGraphicsOutput().getCamera().addPitch(2);

	// Rotate Down
	if (key == 'f') wnd.getGraphicsOutput().getCamera().addPitch(-2);

	// Forward
	if (mouse.getType() == Mouse::Event::Type::WheelUp) wnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, -0.5);
	
	// Backward
	if (mouse.getType() == Mouse::Event::Type::WheelDown) wnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, 0.5);

	DirectX::XMFLOAT4X4 proj = {};
	DirectX::XMStoreFloat4x4(&proj, wnd.getGraphicsOutput().getCamera().getMatrix());
	/*oss << " [Camera Matrix] "
		<< proj._11 << " " << proj._12 << " " << proj._13 << " " << proj._14 << " "
		<< proj._21 << " " << proj._22 << " " << proj._23 << " " << proj._24 << " "
		<< proj._31 << " " << proj._32 << " " << proj._33 << " " << proj._34 << " "
		<< proj._41 << " " << proj._42 << " " << proj._43 << " " << proj._44;*/

	// Print Debug Information to Window Title
	wnd.setTitle(oss.str());
}

Application::~Application() {}

Application::Application() : wnd(1280, 720, L"Window") {

}

int Application::applicationStart() {

	wnd.getGraphicsOutput().setProjection(DirectX::XMMatrixPerspectiveLH(1.0f, 9.0f / 16.0f, 0.25f, 5000.0f));
	wnd.getGraphicsOutput().getCamera().translate(0.0, 0.0, 50.0);

	while (true) {
		if (const auto msgCode = Window::handleMessages()) return *msgCode;
		applicationUpdate();
	}
}