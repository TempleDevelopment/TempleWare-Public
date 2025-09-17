#pragma once
#include <windows.h>
#include <cstddef>
#include <cstdint>
#include <string_view>

namespace I {
    using InstantiateInterfaceFn = void* (*)();
    class CInterfaceReg
    {
    public:
        InstantiateInterfaceFn m_create_fn;
        const char* m_name;
        CInterfaceReg* m_next;
    };

    inline const CInterfaceReg* Find(const char* module_name)
    {
        const HMODULE module_base = GetModuleHandleA(module_name);
        if (module_base == nullptr)
            return nullptr;

        const auto symbol = reinterpret_cast<std::uintptr_t>(GetProcAddress(module_base, "CreateInterface"));
        const std::uintptr_t list = symbol + *reinterpret_cast<std::int32_t*>(symbol + 3) + 7;
        return *reinterpret_cast<CInterfaceReg**>(list);
    }
    template <typename T = void*>
    T* Get(const char* module_name, const char* interface_partial_version)
    {
        for (const CInterfaceReg* current = Find(module_name); current; current = current->m_next)
        {
            if (std::string_view(current->m_name).find(interface_partial_version) != std::string_view::npos)
            {
                return reinterpret_cast<T*>(current->m_create_fn());
            }
        }

        return nullptr;
    }
}
