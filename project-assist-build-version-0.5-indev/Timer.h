#pragma once
#include <chrono>

class Timer {

private:

	std::chrono::high_resolution_clock::time_point previous;
	static inline double mSpeedMult{ 1.0f };

public: // Public Methods

	// Constructor
	Timer();
	
	double mark();
	double peek() const;
	static void incSpeedMult() noexcept;
	static void decSpeedMult() noexcept;

};