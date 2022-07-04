//#include "Mouse.h"
//#include <iostream>
//#include "PCWindows.h"
//
//void Mouse::OnMouseMove(int x, int y) noexcept {
//	
//	m_xPos = x;
//	m_yPos = y;
//
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::Move, *this));
//	TrimBuffer();
//}
//void Mouse::OnMouseEnter() noexcept {
//	m_isInsideWindow = true;
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::Enter, *this));
//	TrimBuffer();
//}
//void Mouse::OnMouseExit() noexcept {
//	m_isInsideWindow = false;
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::Exit, *this));
//	TrimBuffer();
//}
//void Mouse::OnLMBPressed(int x, int y) noexcept {
//	m_isLMBPressed = true;
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::LMBPress, *this));
//	TrimBuffer();
//}
//void Mouse::OnLMBReleased(int x, int y) noexcept {
//	m_isLMBPressed = false;
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::LMBRelease, *this));
//	TrimBuffer();
//}
//void Mouse::OnRMBPressed(int x, int y) noexcept {
//	m_isRMBPressed = true;
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::RMBPress, *this));
//	TrimBuffer();
//}
//void Mouse::OnRMBReleased(int x, int y) noexcept {
//	m_isRMBPressed = false;
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::RMBRelease, *this));
//	TrimBuffer();
//}
//void Mouse::OnWheelUp(int x, int y) noexcept {
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::WheelUp, *this));
//	TrimBuffer();
//}
//void Mouse::OnWheelDown(int x, int y) noexcept {
//	m_buffer.push(Mouse::Event(Mouse::Event::Type::WheelDown, *this));
//	TrimBuffer();
//}
//void Mouse::OnWheelDelta(int x, int y, int delta) noexcept {
//	m_wheelDelta += delta;
//	while (m_wheelDelta >= WHEEL_DELTA) {
//		m_wheelDelta -= WHEEL_DELTA;
//		OnWheelUp(x, y);
//	}
//	while (m_wheelDelta <= -(WHEEL_DELTA)) {
//		m_wheelDelta += WHEEL_DELTA;
//		OnWheelDown(x, y);
//	}
//}
//void Mouse::TrimBuffer() noexcept {
//	while (m_buffer.size() > M_BUFFERSIZE) {
//		m_buffer.pop();
//	}
//}
//
//int Mouse::getX() const noexcept { return m_xPos; }
//int Mouse::getY() const noexcept { return m_yPos; }
//int Mouse::getWheelDelta() const noexcept { return m_wheelDelta; }
//bool Mouse::isLMBPressed() const noexcept { return m_isLMBPressed; }
//bool Mouse::isRMBPressed() const noexcept { return m_isRMBPressed; }
//std::pair<int, int> Mouse::getPos() const noexcept { return { m_xPos,m_yPos }; }
//
//Mouse::Event Mouse::readEvent() noexcept {
//	if (m_buffer.size() > 0u) {
//		Mouse::Event e = m_buffer.front();
//		m_buffer.pop();
//		return e;
//	}
//	else { return Mouse::Event(); }
//}
//void Mouse::Clear() noexcept { m_buffer = std::queue<Event>(); }
//bool Mouse::isInsideWindow() const noexcept { return m_isInsideWindow; }