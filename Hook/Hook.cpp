#include "Hook.h"
#include "CapsLock.h"
#include <Windows.h>

HINSTANCE g_DLL;

// 共享数据段
#pragma data_seg(".shared")
HHOOK g_Hook = NULL;
#pragma data_seg()
#pragma comment(linker, "/SECTION:.shared,RWS")

LRESULT CALLBACK ShellProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HSHELL_LANGUAGE)
    {
        CapsLock::SetIndicator();
    }

    return CallNextHookEx(g_Hook, nCode, wParam, lParam);
}

HOOK_API(void) Install()
{
    if ((g_Hook = SetWindowsHookExW(WH_SHELL, ShellProc, g_DLL, 0)))
    {
        CapsLock::SetIndicator();
    }
}

HOOK_API(void) Uninstall()
{
    if (g_Hook && UnhookWindowsHookEx(g_Hook))
    {
        CapsLock::SetIndicator(false);
        g_Hook = NULL;
    }
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    if (fdwReason == DLL_PROCESS_ATTACH)
    {
        g_DLL = hinstDLL;
    }

    return TRUE;
}
