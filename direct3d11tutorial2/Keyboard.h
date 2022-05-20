/*
/  ----------------------------------------------
/  Keyboard.h
/  
/  A data system that handles the keyboard 
/  KeyDown and KeyUp events as well as storing 
/  these events in a queue for interpreting 
/  multiple keys / keys at a later time.
/  
/  Also has functionality for reading and storing 
/  input characters from WM_CHAR events.
/  ----------------------------------------------
*/
#pragma once
#include <queue>
#include <bitset>

class Keyboard {
	
	friend class Window;

public:

	// Event Class
	class Event {

	public:

		// Type of Event
		enum class Type {

			Press,
			Release,
			Invalid

		};

	private:

		// Fields
		Type			type	= {};
		unsigned char	code	= {};

	public:

		// Constructors
		Event() noexcept :
			type(Type::Invalid),
			code(0u) {}

		Event(Type type, unsigned char code) noexcept :
			type(type),
			code(code) {}

		// Getters/Setters/Is
		unsigned char getCode() const noexcept { return code; }
		bool isPress() const noexcept { return type == Type::Press; }
		bool isRelease() const noexcept { return type == Type::Release; }
		bool isValid() const noexcept { return type != Type::Invalid; }

	};

private:

	// Fields
	static constexpr unsigned int	m_nKeys					= 256u;
	static constexpr unsigned int	m_bufferSize			= 16u;
	bool							m_autoRepeatEnabled		= { false };
	std::bitset<m_nKeys>			m_keyStates				= {};
	std::queue<Event>				m_keyBuffer				= {};
	std::queue<char>				m_charBuffer			= {};

	// Part of the Window
	void OnKeyPressed(unsigned char keycode) noexcept;
	void OnKeyReleased(unsigned char keycode) noexcept;
	void OnChar(char character) noexcept;
	void ClearState() noexcept;
	template<typename T>
	static void TrimBuffer(std::queue<T>& buffer) noexcept;

public:

	// Constructors
	Keyboard() = default;
	Keyboard(const Keyboard&) = delete;
	
	// Key Event
	bool KeyIsPressed(unsigned char keycode) const noexcept;
	Event ReadKey() noexcept;
	bool KeyIsEmpty() const noexcept;
	void ClearKey() noexcept;

	// Char Event
	char ReadChar() noexcept;
	bool CharIsEmpty() const noexcept;
	void ClearChar() noexcept;
	
	// Key and Char Event
	void Clear() noexcept;

	// Auto-Repeat Controls
	void EnableAutoRepeat() noexcept;
	void DisableAutoRepeat() noexcept;
	bool IsAutoRepeatEnabled() const noexcept;

	// Operator Overloads
	Keyboard& operator=(const Keyboard&) = delete;

};