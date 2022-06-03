#include "Application.h"
#include <iostream>

int WINAPI wWinMain(
     HINSTANCE hInstance,
     HINSTANCE hPrevInstance,
     LPWSTR    lpCmdLine,
     int       nCmdShow
) {

    SetThreadDpiAwarenessContext(DPI_AWARENESS_CONTEXT_PER_MONITOR_AWARE_V2);

    #if defined(_DEBUG)
        FILE *fpstdin = stdin, *fpstdout = stdout, *fpstderr = stderr;

        // Command Console
        AllocConsole();
        freopen_s(&fpstdin, "CONIN$", "r", stdin);
        freopen_s(&fpstdout, "CONOUT$", "w", stdout);
        freopen_s(&fpstderr, "CONOUT$", "w", stderr);
    #endif

    try {
        return Application{}.applicationStart();
    } catch (const TestException& e) {
        MessageBox(nullptr, reinterpret_cast<LPCWSTR>(e.what()), reinterpret_cast<LPCWSTR>(e.getType()), MB_OK | MB_ICONEXCLAMATION);
    } catch (const std::exception& e) {
        MessageBox(nullptr, reinterpret_cast<LPCWSTR>(e.what()), L"Standard Exception", MB_OK | MB_ICONEXCLAMATION);
    } catch (...) {
        MessageBox(nullptr, L"No Details", L"Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
    }
}