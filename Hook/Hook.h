#pragma once

#define HOOK_API(ret) extern "C" ret __declspec(dllexport)

HOOK_API(void) Install();
HOOK_API(void) Uninstall();
