#include "Window.h"
#include <sstream>
#include <iostream>

// Creating window class
Window::WindowClass Window::WindowClass::wndClass;

// Window Class WindowClass Class Private Methods
Window::WindowClass::WindowClass() noexcept : hInst(GetModuleHandle(nullptr)) {
    // Window Class
    WNDCLASSEXW wcex{ 0 };
    wcex.cbSize = sizeof(wcex);
    wcex.style = CS_OWNDC;
    wcex.lpfnWndProc = HandleMsgSetup;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = GetInstance();
    wcex.hIcon = nullptr;
    wcex.hCursor = nullptr;
    wcex.hbrBackground = nullptr;
    wcex.lpszMenuName = nullptr;
    wcex.lpszClassName = GetName();
    wcex.hIconSm = nullptr;

    // Register Window
    RegisterClassExW(&wcex);
}
Window::WindowClass::~WindowClass() { UnregisterClass(wndClassName, GetInstance()); }
const TCHAR* Window::WindowClass::GetName() noexcept { return wndClassName; }
HINSTANCE Window::WindowClass::GetInstance() noexcept { return wndClass.hInst; }

// Window General Exception
Window::HRESULTException::HRESULTException(int line, const char* file, HRESULT hr) noexcept :
    Exception(line, file),
    hr(hr) {}
const char* Window::HRESULTException::getType() const noexcept { return "Test Window Exception"; }
HRESULT Window::HRESULTException::GetErrorCode() const noexcept { return hr; }
std::string Window::HRESULTException::GetErrorString() const noexcept { return TranslateErrorCode(hr); }
const char* Window::HRESULTException::what() const noexcept {
    std::ostringstream oss;
    oss << getType() << std::endl
        << "[ERROR CODE] " << GetErrorCode() << std::endl
        << "[DESCRIPTION] " << GetErrorString() << std::endl
        << GetOriginString();

    whatBuffer = oss.str();
    return whatBuffer.c_str();
}
std::string Window::Exception::TranslateErrorCode(HRESULT hr) noexcept {
    char* pMsgBuffer = nullptr;
    DWORD nMsgLength = FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        nullptr,
        hr,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        reinterpret_cast<LPWSTR>(&pMsgBuffer),
        0,
        nullptr
    );
    if (nMsgLength == 0) { return "Unknown Error Code."; }
    std::string errorString = pMsgBuffer;
    LocalFree(pMsgBuffer);
    return errorString;
}
const char* Window::NoGraphicsException::getType() const noexcept { return "[Window Exception] - [No Graphics Object]"; }

    // Window Class Private Methods
LRESULT WINAPI Window::HandleMsgSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    if (msg == WM_NCCREATE) {
        const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lParam);
        Window* const pWnd = static_cast<Window*>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd));
        SetWindowLongPtr(hWnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Window::HandleMsgThunk));
        return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}
LRESULT WINAPI Window::HandleMsgThunk(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* const pWnd = reinterpret_cast<Window*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
    return pWnd->HandleMsg(hWnd, msg, wParam, lParam);
}
LRESULT Window::HandleMsg(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) noexcept {
    switch (msg) {
    
        // Close Window
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    
    // ---- KEYBOARD MESSAGES ---- //

        // When Key is Pressed
    case WM_KEYDOWN:
        if (!(lParam & 0x40000000) || kb.IsAutoRepeatEnabled()) {
            kb.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;

        // When System Key is Pressed
    case WM_SYSKEYDOWN:
        if (!(lParam & 0x40000000) || kb.IsAutoRepeatEnabled()) {
            kb.OnKeyPressed(static_cast<unsigned char>(wParam));
        }
        break;
    
        // When Key is Released
    case WM_KEYUP:
        kb.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;
    
        // When System Key is Released
    case WM_SYSKEYUP:
        kb.OnKeyReleased(static_cast<unsigned char>(wParam));
        break;

        // The Value of the Pressed Key
    case WM_CHAR:
        kb.OnChar(static_cast<unsigned char>(wParam));
        break;

    // ---- MOUSE MESSAGES ---- //

        // When the Mouse is Moved
    case WM_MOUSEMOVE: {
        const POINTS point = MAKEPOINTS(lParam);
        
        // If mouse is inside window
        if (point.x >= 0
            && point.x < m_width 
            && point.y >= 0 
            && point.y < m_height) {
            mouse.OnMouseMove(point.x, point.y);
            if (!mouse.isInsideWindow()) {
                SetCapture(hWnd);
                mouse.OnMouseEnter();
            }
        }
        
        // Else (mouse is outside window)
        else {
            
            // If LMB or RMB is pressed
            if (mouse.isLMBPressed() || mouse.isRMBPressed()) {
                mouse.OnMouseMove(point.x, point.y);
            }
            
            // Else (LMB and RMB not pressed)
            else {
                ReleaseCapture();
                mouse.OnMouseExit();
            }
        }
        break;
    }

        // When the LeftMouseButton is Pressed
    case WM_LBUTTONDOWN: {
        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnLMBPressed(point.x, point.y);
        break;
    }

        // When the LeftMouseButton is Released
    case WM_LBUTTONUP: {
        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnLMBReleased(point.x, point.y);
        break;
    }

        // When the RightMouseButton is Pressed
    case WM_RBUTTONDOWN: {
        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnRMBPressed(point.x, point.y);
        break;
    }

        // When the RightMouseButton is Released
    case WM_RBUTTONUP: {
        const POINTS point = MAKEPOINTS(lParam);
        mouse.OnRMBReleased(point.x, point.y);
        break;
    }

        // When the Mouse Wheel is Moved
    case WM_MOUSEWHEEL: {
        const POINTS point = MAKEPOINTS(lParam);
        const int delta = GET_WHEEL_DELTA_WPARAM(wParam);
        mouse.OnWheelDelta(point.x, point.y, delta);
    }

    // ---- MISCELLANEOUS MESSAGES ---- //

        // Losing Focus On Window
    case WM_KILLFOCUS:
        kb.ClearState();
        break;
    }
    return DefWindowProc(hWnd, msg, wParam, lParam);
}

    // Window Class Public Methods

        // Constructor/Destructor
Window::Window(int width, int height, const TCHAR* name) :
    m_width(width),
    m_height(height) {

    RECT rc = {};
    rc.left = 100;
    rc.right = width + rc.left;
    rc.top = 100;
    rc.bottom = height + rc.top;
    if (AdjustWindowRect(&rc, WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, FALSE) == 0) { throw CHWND_LAST_EXCEPT(); }
    
    m_hWnd = CreateWindow(
        WindowClass::GetName(),
        name,
        WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU,
        (1920 / 2) - ((rc.right - rc.left) / 2),
        (1080 / 2) - ((rc.bottom - rc.top) / 2),
        rc.right - rc.left,
        rc.bottom - rc.top,
        nullptr,
        nullptr,
        WindowClass::GetInstance(),
        this
    );
    if (m_hWnd == nullptr) { throw CHWND_LAST_EXCEPT(); }

    // Show the window, since it will be hidden on creation.
    ShowWindow(m_hWnd, SW_SHOWDEFAULT);

    // Define graphics output pointer.
    pGraphicsOutput = std::make_unique<GraphicsOutput>(m_hWnd);
}
Window::~Window() { DestroyWindow(m_hWnd); }

        // Set the Title
void Window::setTitle(const std::string& title) {
    std::wstring wTitle = std::wstring(title.begin(), title.end());
    if (SetWindowText(m_hWnd, wTitle.c_str()) == 0) {
        throw CHWND_LAST_EXCEPT();
    }
}

        // Handle Messages
std::optional<WPARAM> Window::handleMessages() {
    MSG msg;
    while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
        if (msg.message == WM_QUIT) return msg.wParam;
        TranslateMessage(&msg);
        std::cout << "[" << msg.wParam << ']' << '\n';
        DispatchMessage(&msg);
    }
    return {}; // empty optional
}

WINDOWINFO Window::getWindowInfo() noexcept {
    WINDOWINFO wndInfo = {};
    wndInfo.cbSize = sizeof(WINDOWINFO);

    GetWindowInfo(m_hWnd, &wndInfo);
    return wndInfo;
}

GraphicsOutput& Window::getGraphicsOutput() {
    if (!pGraphicsOutput) throw CHWND_MISSINGGFX_EXCEPT();
    return *pGraphicsOutput;
}