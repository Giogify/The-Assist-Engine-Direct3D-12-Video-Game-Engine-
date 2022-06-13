#pragma once
#include "Window.h"
#include "Timer.h"
#include "Actor.h"
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

	std::vector<Actor> mVecActors{};

	float mMaxFPS{ 0.0f };
	float mFPSCap{ 60.0f };
	float viewingAngle{ 70.0f };

	// Benchmarking
	float sum{};
	UINT runInstances{ 0u };
	Timer timerBenchmark{};

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