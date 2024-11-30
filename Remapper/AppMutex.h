#pragma once

#include <Windows.h>
#include <string>

class AppMutex final
{
public:
    AppMutex(const std::wstring& name);
    ~AppMutex();

    operator bool() const { return m_Mutex != NULL; }

private:
    HANDLE m_Mutex;
};
