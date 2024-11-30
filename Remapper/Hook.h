#pragma once

#include <Windows.h>
#include <string>

class LibHook final
{
public:
    LibHook(const std::wstring& filename);
    ~LibHook();

    operator bool() const { return m_Library != NULL; }

private:
    HMODULE m_Library;
};

class KbdHook final
{
public:
    KbdHook();
    ~KbdHook();

    operator bool() const { return s_Hook != NULL; }

private:
    static LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam);
    static HHOOK s_Hook;
};
