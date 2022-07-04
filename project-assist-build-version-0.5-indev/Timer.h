#pragma once
//#include <chrono>
//
//struct Timer {
//	std::chrono::high_resolution_clock::time_point previous;
//	Timer() { previous = std::chrono::steady_clock::now(); }
//	float mark() {
//		const auto before = previous;
//		previous = std::chrono::steady_clock::now();
//		const std::chrono::duration<float> frameDuration = previous - before;
//		return frameDuration.count();
//	}
//	float peek() const { return std::chrono::duration<float>(std::chrono::steady_clock::now() - previous).count(); }
//};