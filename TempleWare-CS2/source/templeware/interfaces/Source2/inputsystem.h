#pragma once

#include <cstdint>

#include "../../menu/menu.h"

class IInputSystem {
public:
    void SetRelativeMouseMode(bool enable);
    void SetWindowsGrab(bool enable);

    bool IsRelativeMouseMode()
    {
        return *reinterpret_cast<bool**>((uintptr_t)(this) + 0x68);
    }

    void* GetSDLWindow()
    {
        return *reinterpret_cast<void**>((uintptr_t)(this) + 0x26A8);
    }
};