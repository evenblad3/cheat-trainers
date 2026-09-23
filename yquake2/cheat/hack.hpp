#ifndef HACK_HPP
#define HACK_HPP

#include "pch.h"

class Memory {
private:
    std::uint32_t _processId;
    std::unique_ptr<void, decltype(&CloseHandle)> _processHandle;

public:
    Memory();

    bool Attach(const std::wstring& name);

    std::uintptr_t GetModule(const std::wstring& name) const;
    std::uintptr_t ResolvePointerChain(std::uintptr_t base, const std::vector<std::ptrdiff_t>& offsets) const;

    template <typename Type> Type Read(const std::uintptr_t& address) const;
    template <typename Type> void Write(const std::uintptr_t& address, const Type& contain) const;
};

template <typename Type> Type Memory::Read(const std::uintptr_t& address) const {
    if (!_processHandle)
        throw std::runtime_error(
            "Process handle is missing."
        );

    Type variable = Type{ };
    if (!ReadProcessMemory(

        _processHandle.get(),

        reinterpret_cast<LPCVOID>(address),
        &variable, sizeof(Type),

        nullptr
    )) return Type();

    return variable;
}

template <typename Type> void Memory::Write(const uintptr_t& address, const Type& contain) const {
    if (!_processHandle)
        throw std::runtime_error(
            "Process handle is missing."
        );

    WriteProcessMemory(
        _processHandle.get(),

        reinterpret_cast<LPVOID>(address),
        &contain, sizeof(Type),

        nullptr
    );
}

#endif