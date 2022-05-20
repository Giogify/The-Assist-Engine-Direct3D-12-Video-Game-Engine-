#include "Application.h"

int WINAPI wWinMain(
     HINSTANCE hInstance,
     HINSTANCE hPrevInstance,
     LPWSTR    lpCmdLine,
     int       nCmdShow
) {
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