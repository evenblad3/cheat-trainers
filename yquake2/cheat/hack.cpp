// Original code by https://github.com/WzrterFX
// and can be found here -> https://gist.github.com/evenblad3/3032c24e2115a4204b5e015e4e4c5e9d
// Refactored by https://github.com/evenblad3

#include "hack.hpp"
#include <stdexcept>
#include <tlhelp32.h>

Memory::Memory() : _processId(NULL), _processHandle(nullptr, &CloseHandle) { }

bool Memory::Attach(const std::wstring& name) {
    HANDLE snapShot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (snapShot == INVALID_HANDLE_VALUE)
        throw std::runtime_error(
            "Failed to create list of processes."
        );

    PROCESSENTRY32W process{ sizeof(process) };
    if (Process32FirstW(snapShot, &process)) {
        do {
            if (!_wcsicmp(process.szExeFile, name.data())) {
                _processId = process.th32ProcessID; break;
            }
        } while (Process32NextW(snapShot, &process));
    }

    CloseHandle(snapShot);

    if (!_processId)
        throw std::runtime_error(
            "Failed to find the process."
        );

    _processHandle.reset(OpenProcess(PROCESS_ALL_ACCESS, FALSE, _processId));
    if (!_processHandle)
        throw std::runtime_error(
            "Failed to open the process, try open administrator."
        );
    return true;
}

std::uintptr_t Memory::GetModule(const std::wstring& name) const {
    if (!_processHandle)
        throw std::runtime_error(
            "Process handle missing."
        );

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _processId);
    if (snapshot == INVALID_HANDLE_VALUE)
        throw std::runtime_error(
            "Failed to create list of libraries."
        );

    MODULEENTRY32W module{ sizeof(module) };
    if (Module32FirstW(snapshot, &module)) {
        do {
            if (!_wcsicmp(module.szModule, name.data())) {
                CloseHandle(snapshot); return reinterpret_cast<std::uintptr_t>(module.modBaseAddr);
            }
        } while (Module32NextW(snapshot, &module));
    }
    else throw std::runtime_error(
        "Failed to get library address."
    );

    CloseHandle(snapshot);
    return NULL;
}

std::uintptr_t Memory::ResolvePointerChain(std::uintptr_t base, const std::vector<std::ptrdiff_t>& offsets) const
{
    std::uintptr_t addr = base;
    for (std::size_t i = 0; i + 1 < offsets.size(); ++i) {
        addr = Read<std::uintptr_t>(addr + offsets[i]);
        if (!addr) return 0;
    }
    return addr + offsets.back();
}