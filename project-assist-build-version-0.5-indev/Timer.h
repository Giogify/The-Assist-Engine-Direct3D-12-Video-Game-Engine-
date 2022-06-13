#pragma once
#include <chrono>

class Timer {

private:

	std::chrono::steady_clock::time_point previous;
	static inline float mSpeedMult{ 1.0f };

public: // Public Methods

	// Constructor
	Timer();
	
	float mark();
	float peek() const;
	static void incSpeedMult() noexcept;
	static void decSpeedMult() noexcept;

};