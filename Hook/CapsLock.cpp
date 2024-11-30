#include "CapsLock.h"
#include <Windows.h>
#include <ntddkbd.h>
#include <cstring>
#include <string>
#include <stdexcept>

namespace CapsLock
{
    void SetIndicator(bool value)
    {
        try
        {
            KEYBOARD_INDICATOR_PARAMETERS kip{};

            if (!DefineDosDeviceW(DDD_RAW_TARGET_PATH, L"myKBD", L"\\Device\\KeyboardClass0"))
            {
                throw std::runtime_error("DefineDosDevice DDD_RAW_TARGET_PATH failed");
            }

            HANDLE device = CreateFileW(L"\\\\.\\myKBD", GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
            if (device == INVALID_HANDLE_VALUE)
            {
                throw std::runtime_error("CreateFile failed");
            }

            DWORD bytesReturned;
            if (!DeviceIoControl(device, IOCTL_KEYBOARD_QUERY_INDICATORS, NULL, 0, &kip, sizeof(kip), &bytesReturned, NULL))
            {
                throw std::runtime_error("DeviceIoControl IOCTL_KEYBOARD_QUERY_INDICATORS failed");
            }

            if (value)
            {
                kip.LedFlags = static_cast<USHORT>(kip.LedFlags | KEYBOARD_CAPS_LOCK_ON);
            }
            else
            {
                kip.LedFlags = static_cast<USHORT>(kip.LedFlags & ~KEYBOARD_CAPS_LOCK_ON);
            }

            if (!DeviceIoControl(device, IOCTL_KEYBOARD_SET_INDICATORS, &kip, sizeof(kip), NULL, 0, &bytesReturned, NULL))
            {
                throw std::runtime_error("DeviceIoControl IOCTL_KEYBOARD_SET_INDICATORS failed");
            }

            if (!CloseHandle(device))
            {
                throw std::runtime_error("CloseHandle failed");
            }

            if (!DefineDosDeviceW(DDD_REMOVE_DEFINITION, L"myKBD", NULL))
            {
                throw std::runtime_error("DefineDosDevice DDD_REMOVE_DEFINITION failed");
            }
        }
        catch (const std::exception& e)
        {
            std::string msg = "CapsLock::SetIndicator Error: ";
            msg += e.what();
            MessageBoxA(NULL, msg.c_str(), "KeyboardRemapper", MB_ICONERROR);
        }
    }

    static bool IsChineseKeyboardLayout()
    {
        CHAR name[KL_NAMELENGTH];

        // https://learn.microsoft.com/en-us/globalization/keyboards/kbdus_2
        return GetKeyboardLayoutNameA(name) && strcmp(name, "00000804") == 0;
    }

    void SetIndicator()
    {
        SetIndicator(IsChineseKeyboardLayout());
    }
}
