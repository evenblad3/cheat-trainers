#ifndef HACK_HPP
#define HACK_HPP

#include <vector>
#include <Windows.h>
#include <TlHelp32.h>

namespace hack
{
	DWORD GetProcId(const wchar_t* procName);

	uintptr_t GetModuleBaseAddress(DWORD procId, const wchar_t* modName);

	uintptr_t FindDMAAddy(HANDLE hproc, uintptr_t ptr, std::vector<unsigned int> offsets);
}

#endif