#pragma once
#include "Window.h"
#include "Timer.h"
#include "Actor.h"
#include "TransformCBuffer.h"
#include "Scene.h"

class Application {
	
private: // Private Fields

	std::unique_ptr<Scene> mScene{};

	Window mWnd;

	Timer mTimerStart{};
	Timer mTimer{};
	Timer mTimerRender{};

	bool mIsCameraModeEnabled{ false };
	bool mDebugEnoughTimeElapsed{ false };
	
	TransformCBuffer tcb{ mWnd.getGraphicsOutput() };

	float mMaxFPS{ 0.f };
	float mFPSCap{ 1000.f };

private: // Private Methods

	// Application Logic
	int applicationUpdate();

public: // Public Methods

	// Constructor
	Application();
	~Application();

	// Application Startup Procedures
	int applicationStart();

	int doPriorityInput(const Keyboard& kb,
		const std::vector<Keyboard::Event>& keys,
		const std::vector<unsigned char>& keysChar,
		const Mouse& mouse,
		const std::vector<Mouse::Event>& mouseEvents) noexcept;
	
	// Game Loop
	int doInput() noexcept;
	int doUpdate() noexcept;
	int doRender() noexcept;

};