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

	std::array<float, 1000u> FPSsum{};
	uint64_t FPScount{};

	// Benchmarking
	double sum{};
	uint16_t runInstances{ 0u };
	GID::DSU::Timer timerBenchmark{};

private: // Private Methods

	// Application Logic
	uint8_t applicationUpdate();

public: // Public Methods

	// Constructor
	Application();
	~Application();

	// Application Startup Procedures
	uint8_t applicationStart();
	
	// Game Loop
	uint8_t doInput() noexcept;
	uint8_t doUpdate() noexcept;
	uint8_t doRender() noexcept;

};