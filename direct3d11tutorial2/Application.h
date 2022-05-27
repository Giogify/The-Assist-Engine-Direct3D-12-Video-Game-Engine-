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
	
	TransformCBuffer tcb{ mWnd.getGraphicsOutput() };

	Timer oneSecondTimer{};

	Timer renderTimer{};
	unsigned int inputcount{};
	std::vector<float> inputTimes{};

	unsigned int updatecount{};
	std::vector<float> updateTimes{};

	unsigned int rendercount{};
	std::vector<float> renderTimes{};

	float mMaxFPS{ 0.0f };
	float mFPSCap{ 240.0f };

private: // Private Methods

	// Application Logic
	int applicationUpdate();

public: // Public Methods

	// Constructor
	Application();
	~Application();

	// Application Startup Procedures
	int applicationStart();

	int doPriorityInput(std::vector<char> keys, std::vector<Mouse::Event> mouse) noexcept;
	
	// Game Loop
	int doInput() noexcept;
	int doUpdate() noexcept;
	int doRender() noexcept;

};