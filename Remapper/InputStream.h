#pragma once

#include <Windows.h>
#include <vector>

class InputStream final
{
public:
    InputStream();

    InputStream& Down(WORD key, bool suppressEvent = false);
    InputStream& Up(WORD key, bool suppressEvent = false);
    InputStream& DownUp(const std::vector<WORD>& keys, bool suppressEvent = false);
    InputStream& UpDown(const std::vector<WORD>& keys, bool suppressEvent = false);
    void Flush();

    static bool IsEventSuppressed(const KBDLLHOOKSTRUCT* p);

private:
    void Append(WORD key, bool isDown, bool suppressEvent);

    std::vector<INPUT> m_Inputs;
};
