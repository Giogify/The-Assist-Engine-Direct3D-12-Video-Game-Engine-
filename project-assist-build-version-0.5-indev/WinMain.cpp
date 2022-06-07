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
        AllocConsole();
        freopen_s(&fpstdin, "CONIN$", "r", stdin);
        freopen_s(&fpstdout, "CONOUT$", "w", stdout);
        freopen_s(&fpstderr, "CONOUT$", "w", stderr);
    #endif
    
        return Application{}.applicationStart();

}