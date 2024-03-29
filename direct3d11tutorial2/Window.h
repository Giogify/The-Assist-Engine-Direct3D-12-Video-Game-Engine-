/*
/  ----------------------------------------------
/  Window.h
/
/  This header / class file provides a decent 
/  enough abstraction of the Win32API window 
/  class register process.
/  
/  Additional Window Message Handling System for 
/  actions and events that take place in the 
/  specific instance of the window. Further 
/  delegated amongst specific handling classes; 
/  e.g. Keyboard.cpp and Mouse.cpp.
/  
/  Additional functionality for multiple windows.
/  
/  Additional functionality for handling 
/  exceptions.
/  ----------------------------------------------
*/
#pragma once
#include "TestException.h"
#include "Keyboard.h"
#include "Mouse.h"
#include "PCWindows.h"
#include "GraphicsOutput.h"

#include <tchar.h>
#include <sstream>
#include <optional>
#include <memory>

class Window {

private: // Private Data Structures

	// Registering the Window Class
	class WindowClass {

	private: // Private Fields

		static constexpr const TCHAR* wndClassName = _T("D3D11 Tutorial Window");
		static WindowClass wndClass;

		HINSTANCE hInst;

	private: // Private Methods

		// Constructor
		WindowClass() noexcept;
		
		// Destructor
		~WindowClass();

		// Operator Overloads

	public: // Public Methods

		static const TCHAR* GetName() noexcept;
		static HINSTANCE GetInstance() noexcept;

	};

public: // Public Data Structures

	// Exceptions
	class Exception : public TestException {

		using TestException::TestException;

	public:

		static std::string TranslateErrorCode(HRESULT hr) noexcept;

	};
	class HRESULTException : public Exception {

	private:

		HRESULT hr;

	public:

		HRESULTException(int, const char*, HRESULT) noexcept;
		const char* getType() const noexcept override;
		HRESULT GetErrorCode() const noexcept;
		std::string GetErrorString() const noexcept;
		const char* what() const noexcept override;


	};
	class NoGraphicsException : public Exception {

	public:

		using Exception::Exception;
		const char* getType() const noexcept override;

	};

private: // Private Fields

	unsigned int					m_width{};
	unsigned int					m_height{};
	HWND							m_hWnd{};
	std::unique_ptr<GraphicsOutput> pGraphicsOutput = nullptr;
	RAWINPUTDEVICE					mRID{};

public: // Public Fields

	Keyboard	kb		= {};
	Mouse		mouse	= {};

private: // Private Methods

	// Message-Handling
	static LRESULT CALLBACK HandleMsgSetup(HWND, UINT, WPARAM, LPARAM);
	static LRESULT CALLBACK HandleMsgThunk(HWND, UINT, WPARAM, LPARAM);
	LRESULT HandleMsg(HWND, UINT, WPARAM, LPARAM) noexcept;

public: // Public Methods

	// Constructor
	Window(unsigned int, unsigned int, const TCHAR*);

	//Destructor
	~Window();

	void setTitle(const std::wstring& title);

	static std::optional<WPARAM> handleMessages();

	GraphicsOutput& getGraphicsOutput();
	
	// Overloading Operators

	WINDOWINFO getWindowInfo() noexcept;

};

// Helper Macros
#define CHWND_EXCEPT(hr) Window::HRESULTException(__LINE__, __FILE__, hr)
#define CHWND_LAST_EXCEPT() Window::HRESULTException(__LINE__, __FILE__, GetLastError())
#define CHWND_MISSINGGFX_EXCEPT() Window::NoGraphicsException(__LINE__, __FILE__)