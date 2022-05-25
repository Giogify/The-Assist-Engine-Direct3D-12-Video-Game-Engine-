/*
/  ----------------------------------------------
/  Mouse.h
/
/  A data system that handles the Mouse 
/  LeftMouseButton presses and releases, 
/  RightMouseButton presses and releases, mouse 
/  wheel events, mouse cursor movements, and 
/  additional mouse events.
/  
/  Currently no system for queuing mouse events 
/  for later interpretation.
/  ----------------------------------------------
*/
#pragma once
#include <queue>

class Mouse {

	friend class Window;

public: // Public Data Structures

	// Event Class
	class Event {

	public: //Enum for Type of Mouse Event

		enum class Type {
			
			LMBPress,
			LMBRelease,
			RMBPress,
			RMBRelease,
			WheelUp,
			WheelDown,
			Move,
			Enter,
			Exit,
			Invalid

		};

	private: // Private Fields

		Type	m_type			= {};
		int		m_xPos			= {};
		int		m_yPos			= {};
		bool	m_isLMBPressed	= {};
		bool	m_isRMBPressed	= {};

	public: // Public Methods

		// Empty Constructor
		Event() noexcept :
			m_type			(Type::Invalid),
			m_isLMBPressed	(false),
			m_isRMBPressed	(false),
			m_xPos			(0),
			m_yPos			(0) {}

		// Expected Constructor
		Event(Type type, const Mouse& mouse) noexcept :
			m_type			(type),
			m_xPos			(mouse.m_xPos),
			m_yPos			(mouse.m_yPos),
			m_isLMBPressed	(mouse.m_isLMBPressed),
			m_isRMBPressed	(mouse.m_isRMBPressed) {}

		// Getters/Setters/Is
		Type getType() const noexcept				{ return m_type; }
		bool isLMBPressed() const noexcept			{ return m_isLMBPressed; }
		bool isRMBPressed() const noexcept			{ return m_isRMBPressed; }
		int getX() const noexcept					{ return m_xPos; }
		int getY() const noexcept					{ return m_yPos; }
		bool isValid() const noexcept				{ return m_type != Type::Invalid; }
		std::pair<int, int> getPos() const noexcept	{ return { m_xPos, m_yPos }; }

	};

private: // Private Fields

	static constexpr unsigned int	M_BUFFERSIZE		= 256u;
	int								m_xPos				= {};
	int								m_yPos				= {};
	int								m_wheelDelta		= { 0 };
	bool							m_isLMBPressed		= false;
	bool							m_isRMBPressed		= false;
	bool							m_isInsideWindow	= false;
	std::queue<Event>				m_buffer			= {};

private: // Private Methods

	void OnMouseMove(int x, int y) noexcept;
	void OnMouseEnter() noexcept;
	void OnMouseExit() noexcept;
	void OnLMBPressed(int x, int y) noexcept;
	void OnLMBReleased(int x, int y) noexcept;
	void OnRMBPressed(int x, int y) noexcept;
	void OnRMBReleased(int x, int y) noexcept;
	void OnWheelUp(int x, int y) noexcept;
	void OnWheelDown(int x, int y) noexcept;
	void OnWheelDelta(int x, int y, int delta) noexcept;
	void TrimBuffer() noexcept;

public: // Public Methods

	// Default Constructor
	Mouse() = default;
	Mouse(const Mouse&) = delete;

	// Getters/Setters/Is
	int					getX()			const noexcept;
	int					getY()			const noexcept;
	int					getWheelDelta() const noexcept;
	bool				isLMBPressed()	const noexcept;
	bool				isRMBPressed()	const noexcept;
	std::pair<int, int>	getPos()		const noexcept;
	bool				isEmpty()		const noexcept { return m_buffer.empty(); }

	// Read the Event
	Mouse::Event readEvent() noexcept;

	// Empty the Buffer
	void Clear() noexcept;

	// Is the Mouse in the Window
	bool isInsideWindow() const noexcept;

	// Operator Overloads
	Mouse& operator=(const Mouse&) = delete;

};