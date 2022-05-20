#include "Timer.h"

Timer::Timer() {
	previous = std::chrono::steady_clock::now();
}
float Timer::mark() {
	const auto before = previous;
	previous = std::chrono::steady_clock::now();
	const std::chrono::duration<float> frameDuration = previous - before;
	return frameDuration.count();
}
float Timer::peek() const {
	return std::chrono::duration<float>(std::chrono::steady_clock::now() - previous).count();
}