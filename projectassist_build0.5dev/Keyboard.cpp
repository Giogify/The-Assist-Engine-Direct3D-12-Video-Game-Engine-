#include "Keyboard.h"

// Keyboard Class Private Methods
void Keyboard::OnKeyPressed(unsigned char keycode) noexcept {
	m_keyStates[keycode] = true;
	m_keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Press, keycode));
	TrimBuffer(m_keyBuffer);
}
void Keyboard::OnKeyReleased(unsigned char keycode) noexcept {
	m_keyStates[keycode] = false;
	m_keyBuffer.push(Keyboard::Event(Keyboard::Event::Type::Release, keycode));
	TrimBuffer(m_keyBuffer);
}
void Keyboard::OnChar(char character) noexcept {
	m_charBuffer.push(character);
	TrimBuffer(m_charBuffer);
}
void Keyboard::ClearState() noexcept { m_keyStates.reset(); }

template<typename T>
void Keyboard::TrimBuffer(std::queue<T>& buffer) noexcept { while (buffer.size() > m_bufferSize) { buffer.pop(); } }

// Keyboard Public Methods

	// Key Event
bool Keyboard::KeyIsPressed(unsigned char keycode) const noexcept { return m_keyStates[keycode]; }
Keyboard::Event Keyboard::ReadKey() noexcept {
	if (m_keyBuffer.size() > 0u) {
		Keyboard::Event e = m_keyBuffer.front();
		m_keyBuffer.pop();
		return e;
	}
	else { return Keyboard::Event(); }
}
bool Keyboard::KeyIsEmpty() const noexcept { return m_keyBuffer.empty(); }
void Keyboard::ClearKey() noexcept { m_keyBuffer = std::queue<Event>(); }

	// Char Event
char Keyboard::ReadChar() noexcept {
	if (m_charBuffer.size() > 0u) {
		unsigned char charCode = m_charBuffer.front();
		m_charBuffer.pop();
		return charCode;
	}
	else { return 0; }
}
bool Keyboard::CharIsEmpty() const noexcept { return m_charBuffer.empty(); }
void Keyboard::ClearChar() noexcept { m_charBuffer = std::queue<char>(); }

	// Key and Char Event
void Keyboard::Clear() noexcept {
	ClearKey();
	ClearChar();
}

	// Auto-Repeat Controls
void Keyboard::EnableAutoRepeat() noexcept { m_autoRepeatEnabled = true; }
void Keyboard::DisableAutoRepeat() noexcept { m_autoRepeatEnabled = false; }
bool Keyboard::IsAutoRepeatEnabled() const noexcept { return m_autoRepeatEnabled; }