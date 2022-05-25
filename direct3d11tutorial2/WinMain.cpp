#include "Application.h"
#include <iostream>

int WINAPI wWinMain(
     HINSTANCE hInstance,
     HINSTANCE hPrevInstance,
     LPWSTR    lpCmdLine,
     int       nCmdShow
) {

    FILE *fpstdin = stdin, *fpstdout = stdout, *fpstderr = stderr;

    AllocConsole();
    freopen_s(&fpstdin, "CONIN$", "r", stdin);
    freopen_s(&fpstdout, "CONOUT$", "w", stdout);
    freopen_s(&fpstderr, "CONOUT$", "w", stderr);

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