//#include "Timer.h"
//
//Timer::Timer() {
//	previous = std::chrono::steady_clock::now();
//}
//double Timer::mark() {
//	const auto before = previous;
//	previous = std::chrono::steady_clock::now();
//	const std::chrono::duration<double> frameDuration = previous - before;
//	return mSpeedMult * frameDuration.count();
//}
//double Timer::peek() const {
//	return mSpeedMult * std::chrono::duration<double>(std::chrono::steady_clock::now() - previous).count();
//}
//void Timer::incSpeedMult() noexcept {
//	mSpeedMult += 1.0f;
//}
//void Timer::decSpeedMult() noexcept {
//	mSpeedMult -= 1.0f;
//}