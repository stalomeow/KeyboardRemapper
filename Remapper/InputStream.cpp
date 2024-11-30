#include "InputStream.h"

static const ULONG_PTR INPUT_KEYBOARD_SUPPRESS_FLAG = 0x111;

InputStream::InputStream() : m_Inputs() {}

bool InputStream::IsEventSuppressed(const KBDLLHOOKSTRUCT* p)
{
    return p->dwExtraInfo == INPUT_KEYBOARD_SUPPRESS_FLAG;
}

void InputStream::Append(WORD key, bool isDown, bool suppressEvent)
{
    INPUT& input = m_Inputs.emplace_back();
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = key;
    input.ki.wScan = 0;
    input.ki.dwFlags = isDown ? 0 : KEYEVENTF_KEYUP;
    input.ki.time = 0;
    input.ki.dwExtraInfo = suppressEvent ? INPUT_KEYBOARD_SUPPRESS_FLAG : 0;
}

InputStream& InputStream::Down(WORD key, bool suppressEvent)
{
    Append(key, true, suppressEvent);
    return *this;
}

InputStream& InputStream::Up(WORD key, bool suppressEvent)
{
    Append(key, false, suppressEvent);
    return *this;
}

InputStream& InputStream::DownUp(const std::vector<WORD>& keys, bool suppressEvent)
{
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        Down(*it, suppressEvent);
    }

    for (auto it = keys.rbegin(); it != keys.rend(); ++it)
    {
        Up(*it, suppressEvent);
    }

    return *this;
}

InputStream& InputStream::UpDown(const std::vector<WORD>& keys, bool suppressEvent)
{
    for (auto it = keys.begin(); it != keys.end(); ++it)
    {
        Up(*it, suppressEvent);
    }

    for (auto it = keys.rbegin(); it != keys.rend(); ++it)
    {
        Down(*it, suppressEvent);
    }

    return *this;
}

void InputStream::Flush()
{
    if (m_Inputs.empty())
    {
        return;
    }

    SendInput(static_cast<UINT>(m_Inputs.size()), m_Inputs.data(), sizeof(INPUT));
    m_Inputs.clear();
}
