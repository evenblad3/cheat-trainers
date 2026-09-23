#include "cheat.hpp"
#include "hack.hpp"
#include "gamestate.hpp"
#include "Drawing.h"

void UpdateGameState(Memory& memory)
{
    try {
        memory.Attach(L"yquake2.exe");
        Modules::client = memory.GetModule(L"yquake2.exe");
    }
    catch (const std::exception& e) {
        MessageBoxA(nullptr, e.what(), "Exception", MB_OK | MB_ICONERROR | MB_TOPMOST);
        return;
    }

    while (Drawing::isActive()) {
        try {
            std::uintptr_t entity = memory.Read<std::uintptr_t>(Modules::client + Offsets::LocalPlayer);
            if (!entity) continue;

            std::uintptr_t healthAddress = entity + Offsets::Health;

            if (g_state.godmode) {
                memory.Write<std::int32_t>(healthAddress, 777);
            }
            g_state.playerHealth = memory.Read<std::int32_t>(healthAddress);
        }
        catch (const std::exception&) {}

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}