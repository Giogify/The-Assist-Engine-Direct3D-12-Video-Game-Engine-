//#include "Window.h"
//#include <hidusage.h>
//#include <sstream>
//#include <iostream>
//
////// Window Class WindowClass Class Private Methods
////Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
////	// Window Class
////	WNDCLASSEX wcex{ 0 };
////	wcex.cbSize = sizeof(wcex);
////	wcex.style = CS_OWNDC;
////	wcex.lpfnWndProc = HandleMsgSetup;
////	wcex.cbClsExtra = 0;
////	wcex.cbWndExtra = 0;
////	wcex.hInstance = GetInstance();
////	wcex.hIcon = nullptr;
////	wcex.hCursor = nullptr;
////	wcex.hbrBackground = nullptr;
////	wcex.lpszMenuName = nullptr;
////	wcex.lpszClassName = GetName().c_str();
////	wcex.hIconSm = nullptr;
////
////	// Register Window
////	RegisterClassEx(&wcex);
////}
////Window::WindowClass::~WindowClass() { UnregisterClass(GetName().c_str(), GetInstance()); }
////std::wstring Window::WindowClass::GetName() noexcept { return wndClassName; }
////HINSTANCE Window::WindowClass::GetInstance() noexcept { return wndClass.hInst; }
//
//LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//	if (msg == WM_NCCREATE) {
//		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
//		Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
//		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
//		SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
//		return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
//	Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
//	return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
//}
//LRESULT WINAPI Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
//	switch (msg) {
//
//		// Close Window
//	/*case WM_CLOSE:
//		pGraphicsOutput->flushGPU();
//		CloseHandle(pGraphicsOutput->mhFenceEvent);
//		PostQuitMessage(0);
//		return 0;*/
//	
//	// ---- KEYBOARD MESSAGES ---- //
//
//		// When Key is Pressed
//	case WM_KEYDOWN:
//		if (!(lParam & 0x40000000) || kb.IsAutoRepeatEnabled()) {
//			kb.OnKeyPressed((unsigned char)wParam);
//		}
//		break;
//
//		// When System Key is Pressed
//	case WM_SYSKEYDOWN:
//		if (!(lParam & 0x40000000) || kb.IsAutoRepeatEnabled()) {
//			kb.OnKeyPressed((unsigned char)wParam);
//		}
//		break;
//	
//		// When Key is Released
//	case WM_KEYUP:
//		kb.OnKeyReleased((unsigned char)wParam);
//		break;
//	
//		// When System Key is Released
//	case WM_SYSKEYUP:
//		kb.OnKeyReleased((unsigned char)wParam);
//		break;
//
//		// The Value of the Pressed Key
//	case WM_CHAR:
//		kb.OnChar((unsigned char)wParam);
//		break;
//
//	// ---- MOUSE MESSAGES ---- //
//
//	/*case WM_INPUT: {
//		UINT dwSize = sizeof(RAWINPUT);
//		static BYTE lpb[sizeof(RAWINPUT)];
//
//		GetRawInputData((HRAWINPUT)lParam, RID_INPUT, lpb, &dwSize, sizeof(RAWINPUTHEADER));
//
//		RAWINPUT* raw = (RAWINPUT*)lpb;
//
//		if (raw->header.dwType == RIM_TYPEMOUSE) {
//			mouse.OnMouseMove(raw->data.mouse.lLastX, raw->data.mouse.lLastY);
//		}
//		break;
//	}*/
//
//		// When the Mouse is Moved
//	case WM_MOUSEMOVE: {
//		const POINTS point = MAKEPOINTS(lParam);
//		
//		// If mouse is inside window
//		if (point.x >= 0
//			&& point.x < mWidth 
//			&& point.y >= 0 
//			&& point.y < mHeight) {
//			mouse.OnMouseMove(point.x, point.y);
//			if (!mouse.isInsideWindow()) {
//				SetCapture(hWnd);
//				mouse.OnMouseEnter();
//			}
//		}
//		
//		// Else (mouse is outside window)
//		else {
//			
//			// If LMB or RMB is pressed
//			if (mouse.isLMBPressed() || mouse.isRMBPressed()) {
//				mouse.OnMouseMove(point.x, point.y);
//			}
//			
//			// Else (LMB and RMB not pressed)
//			else {
//				ReleaseCapture();
//				mouse.OnMouseExit();
//			}
//		}
//		break;
//	}
//
//		// When the LeftMouseButton is Pressed
//	case WM_LBUTTONDOWN: {
//		const POINTS point = MAKEPOINTS(lParam);
//		mouse.OnLMBPressed(point.x, point.y);
//		break;
//	}
//
//		// When the LeftMouseButton is Released
//	case WM_LBUTTONUP: {
//		const POINTS point = MAKEPOINTS(lParam);
//		mouse.OnLMBReleased(point.x, point.y);
//		break;
//	}
//
//		// When the RightMouseButton is Pressed
//	case WM_RBUTTONDOWN: {
//		const POINTS point = MAKEPOINTS(lParam);
//		mouse.OnRMBPressed(point.x, point.y);
//		break;
//	}
//
//		// When the RightMouseButton is Released
//	case WM_RBUTTONUP: {
//		const POINTS point = MAKEPOINTS(lParam);
//		mouse.OnRMBReleased(point.x, point.y);
//		break;
//	}
//
//		// When the Mouse Wheel is Moved
//	case WM_MOUSEWHEEL: {
//		const POINTS point = MAKEPOINTS(lParam);
//		const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
//		mouse.OnWheelDelta(point.x, point.y, delta);
//	}
//
//	// ---- MISCELLANEOUS MESSAGES ---- //
//
//		// Losing Focus On Window
//	case WM_KILLFOCUS:
//		kb.ClearState();
//		break;
//	/*case WM_SIZE: {
//		RECT clientRc{};
//		GetClientRect(hWnd, &clientRc);
//		UINT width = clientRc.right - clientRc.left;
//		UINT height = clientRc.bottom = clientRc.top;
//		pGraphicsOutput->resizeWindow(width, height);
//		break;
//	}*/
//	}
//	return DefWindowProc(hWnd, msg, wParam, lParam);
//}
//
//Window::Window(unsigned int width, unsigned int height, const TCHAR* name) :
//	mWidth(width),
//	mHeight(height) {
//
//	// Window Class
//	WNDCLASSEX wcex{};
//	wcex.cbSize = sizeof(wcex);
//	wcex.style = CS_OWNDC;
//	wcex.lpfnWndProc = HandleMsgSetup;
//	wcex.cbClsExtra = 0;
//	wcex.cbWndExtra = 0;
//	wcex.hInstance = hInst;
//	wcex.hIcon = nullptr;
//	wcex.hCursor = nullptr;
//	wcex.hbrBackground = nullptr;
//	wcex.lpszMenuName = nullptr;
//	wcex.lpszClassName = wndClassName.c_str();
//	wcex.hIconSm = nullptr;
//
//	// Register Window
//	RegisterClassEx(&wcex);
//
//	RECT rc = {};
//	rc.left = 100;
//	rc.right = width + rc.left;
//	rc.top = 100;
//	rc.bottom = height + rc.top;
//	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);
//	
//	mhWnd = CreateWindow(
//		wndClassName.c_str(),
//		name,
//		WS_OVERLAPPEDWINDOW,
//		(1920 / 2) - ((rc.right - rc.left) / 2),
//		(1080 / 2) - ((rc.bottom - rc.top) / 2),
//		rc.right - rc.left,
//		rc.bottom - rc.top,
//		nullptr,
//		nullptr,
//		hInst,
//		this
//	);
//
//	// Show the window, since it will be hidden on creation.
//	ShowWindow(mhWnd, SW_SHOWDEFAULT);
//
//	/*mRID.usUsagePage = HID_USAGE_PAGE_GENERIC;
//	mRID.usUsage = HID_USAGE_GENERIC_MOUSE;
//	mRID.dwFlags = RIDEV_INPUTSINK;
//	mRID.hwndTarget = mhWnd;
//	RegisterRawInputDevices(&mRID, 1u, sizeof(mRID));*/
//
//}
//Window::~Window() { DestroyWindow(mhWnd); }
//
//void Window::setTitle(const std::wstring& title) {
//	//std::wstring wTitle = std::wstring(title.begin(), title.end());
//	SetWindowText(mhWnd, title.c_str());
//}
//std::optional<WPARAM> Window::handleMessages() {
//	MSG msg;
//	while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
//		if (msg.message == WM_QUIT) return msg.wParam;
//		TranslateMessage(&msg);
//		DispatchMessage(&msg);
//	}
//	return {}; // empty optional
//}
//WINDOWINFO Window::getWindowInfo() noexcept {
//	WINDOWINFO wndInfo = {};
//	wndInfo.cbSize = sizeof(WINDOWINFO);
//	GetWindowInfo(mhWnd, &wndInfo);
//	return wndInfo;
//}
//HWND& Window::getHandle() noexcept { return mhWnd; }