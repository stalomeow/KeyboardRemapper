#include "AppMutex.h"
#include "Hook.h"

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR lpCmdLine,
    _In_ int nCmdShow)
{
    AppMutex mutex{ L"Local\\StaloMeow_KeyboardRemapper_Mutex" };

    if (!mutex)
    {
        MessageBoxW(NULL, L"Another instance is already running.", L"KeyboardRemapper", MB_ICONERROR);
        return 0;
    }

    LibHook libHook{ L"KeyboardRemapper.Hook.dll" };
    KbdHook kbdHook{};

    if (!libHook || !kbdHook)
    {
        MessageBoxW(NULL, L"Failed to install hooks.", L"KeyboardRemapper", MB_ICONERROR);
        return 0;
    }

    // 笔记本断电时，避免 Windows 终止该进程
    // https://learn.microsoft.com/en-us/windows/win32/api/winbase/nf-winbase-setthreadexecutionstate
    // To run properly on a power-managed computer, applications such as fax servers, answering machines,
    // backup agents, and network management applications must use both ES_SYSTEM_REQUIRED and ES_CONTINUOUS
    // when they process events.
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED);

    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
