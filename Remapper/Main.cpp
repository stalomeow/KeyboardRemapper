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

    MSG msg;

    while (GetMessageW(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }

    return 0;
}
