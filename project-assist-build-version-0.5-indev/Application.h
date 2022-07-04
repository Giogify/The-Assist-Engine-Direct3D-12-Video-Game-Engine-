#pragma once
#include "GlobalInformationDatabase.h"

class Application {
	
private: // Private Fields

	GID::DSU::Timer mTimerStart{};
	GID::DSU::Timer mTimer{};
	GID::DSU::Timer mTimerRender{};
	GID::DSU::Timer mTickTimer{};

	bool mIsCameraModeEnabled{ false };
	bool mDebugEnoughTimeElapsed{ false };

	double mMaxFPS{ 0.0 };
	double mFPSCap{ 60.0 };
	double viewingAngle{ 90.0 };

	double rem{};

	// Benchmarking
	double sum{};
	uint16_t runInstances{ 0u };
	GID::DSU::Timer timerBenchmark{};

private: // Private Methods

	// Application Logic
	int applicationUpdate();

public: // Public Methods

	// Constructor
	Application();
	~Application();

	// Application Startup Procedures
	int applicationStart();
	
	// Game Loop
	int doInput() noexcept;
	int doUpdate(float) noexcept;
	int doRender() noexcept;

};