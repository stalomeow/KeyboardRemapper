#include "Hook.h"
#include "InputStream.h"

template<typename Ret = void, typename... Args>
static Ret CallAPI(HMODULE hModule, LPCSTR name, Args... args)
{
    return reinterpret_cast<Ret(*)(Args...)>(GetProcAddress(hModule, name))(args...);
}

LibHook::LibHook(const std::wstring& filename)
{
    if (m_Library = LoadLibraryW(filename.c_str()))
    {
        CallAPI(m_Library, "Install");
    }
}

LibHook::~LibHook()
{
    if (m_Library)
    {
        CallAPI(m_Library, "Uninstall");
        FreeLibrary(m_Library);
        m_Library = NULL;
    }
}

HHOOK KbdHook::s_Hook = NULL;

KbdHook::KbdHook()
{
    if (!s_Hook)
    {
        s_Hook = SetWindowsHookExW(WH_KEYBOARD_LL, HookProc, GetModuleHandleW(NULL), 0);
    }
}

KbdHook::~KbdHook()
{
    if (s_Hook)
    {
        UnhookWindowsHookEx(s_Hook);
        s_Hook = NULL;
    }
}

static InputStream g_Inputs{};
static bool g_IsCapsLockDown = false;

LRESULT CALLBACK KbdHook::HookProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode == HC_ACTION)
    {
        KBDLLHOOKSTRUCT* p = reinterpret_cast<KBDLLHOOKSTRUCT*>(lParam);

        if (InputStream::IsEventSuppressed(p))
        {
            return 1; // 阻止默认行为
        }

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN)
        {
            switch (p->vkCode)
            {
            case VK_NUMLOCK:
                // https://github.com/microsoft/PowerToys/blob/f565874fc842006f01d34f4f2c8086b029660c20/src/modules/keyboardmanager/KeyboardManagerEngineLibrary/KeyboardManager.cpp#L106
                // Reset Num Lock whenever a NumLock key down event is suppressed
                // since Num Lock key state change occurs before it is intercepted by low level hooks
                g_Inputs.UpDown({ VK_NUMLOCK }, true).Flush();
                return 1; // 阻止默认行为

            case VK_CAPITAL:
                if (!g_IsCapsLockDown)
                {
                    g_IsCapsLockDown = true;
                    g_Inputs.DownUp({ VK_LWIN, VK_SPACE }).Flush();
                }
                return 1; // 阻止默认行为
            }
        }
        else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP)
        {
            switch (p->vkCode)
            {
            case VK_NUMLOCK:
                return 1; // 阻止默认行为

            case VK_CAPITAL:
                g_IsCapsLockDown = false;
                return 1; // 阻止默认行为
            }
        }
    }

    return CallNextHookEx(s_Hook, nCode, wParam, lParam);
}
