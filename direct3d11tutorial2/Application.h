#pragma once
#include "Window.h"
#include "Timer.h"
#include "Actor.h"
#include "TransformCBuffer.h"

class Application {
	
private: // Private Fields

	Window wnd;
	Timer startTimer;
	Timer timer;
	Timer renderTimer;
	bool m_isFrameAdvance = false;
	std::vector<std::unique_ptr<Actor>> actors;

	TransformCBuffer tcb{ wnd.getGraphicsOutput() };

	float maxFPS = 0.0f;
	float FPS = 120.000000f;

private: // Private Methods

	// Application Logic
	void applicationUpdate();

public: // Public Methods

	// Constructor
	Application();
	~Application();

	// Application Startup Procedures
	int applicationStart();

};