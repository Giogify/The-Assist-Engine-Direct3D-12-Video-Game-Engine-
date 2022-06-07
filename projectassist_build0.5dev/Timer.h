#pragma once
#include <chrono>

class Timer {

private:

	std::chrono::steady_clock::time_point previous;

public: // Public Methods

	// Constructor
	Timer();
	
	float mark();
	float peek() const;

};