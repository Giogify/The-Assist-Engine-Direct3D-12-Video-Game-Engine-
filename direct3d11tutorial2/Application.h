#pragma once
#include "Window.h"
#include "Timer.h"
#include "Actor.h"
#include "TransformCBuffer.h"

class Application {
	
private: // Private Fields

	std::vector<std::unique_ptr<Actor>> mActors{};

	Window mWnd;

	Timer mTimerStart{};
	Timer mTimer{};
	Timer mTimerRender{};

	bool mIsFrameAdvanceEnabled{ false };
	bool mIsCameraModeEnabled{ false };
	
	TransformCBuffer tcb{ mWnd.getGraphicsOutput() };

	float mMaxFPS{ 0.0f };
	float mFPSCap{ 120.0f };

private: // Private Methods

	// Application Logic
	int applicationUpdate();

public: // Public Methods

	// Constructor
	Application();
	~Application();

	// Application Startup Procedures
	int applicationStart();

};