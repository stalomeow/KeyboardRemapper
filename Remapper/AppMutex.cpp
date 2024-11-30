#include "AppMutex.h"

AppMutex::AppMutex(const std::wstring& name)
{
    m_Mutex = CreateMutexW(NULL, FALSE, name.c_str());

    // https://learn.microsoft.com/en-us/windows/win32/api/synchapi/nf-synchapi-createmutexw#return-value
    // If the mutex is a named mutex and the object existed before this function call,
    // the return value is a handle to the existing object,
    // and the GetLastError function returns ERROR_ALREADY_EXISTS.
    if (m_Mutex != NULL && GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(m_Mutex);
        m_Mutex = NULL;
    }
}

AppMutex::~AppMutex()
{
    if (m_Mutex)
    {
        CloseHandle(m_Mutex);
        m_Mutex = NULL;
    }
}
