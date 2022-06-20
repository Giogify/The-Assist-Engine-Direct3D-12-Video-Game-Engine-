#pragma once
#include "Window.h"
#include "Timer.h"
#include "Actor.h"
#include "Scene.h"

class Application {
	
private: // Private Fields

	Scene mScene{};

	Window mWnd;

	Timer mTimerStart{};
	Timer mTimer{};
	Timer mTimerRender{};
	Timer mTickTimer{};

	bool mIsCameraModeEnabled{ false };
	bool mDebugEnoughTimeElapsed{ false };

	std::vector<Actor> mVecActors{};

	double mMaxFPS{ 0.0 };
	double mFPSCap{ 60.0 };
	double viewingAngle{ 70.0 };

	double rem{};

	// Benchmarking
	double sum{};
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