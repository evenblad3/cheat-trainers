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

            // health
            std::uintptr_t healthAddress = entity + Offsets::Health;

            if (g_state.godmode) {
                memory.Write<std::int32_t>(healthAddress, 777);
            }
            g_state.playerHealth = memory.Read<std::int32_t>(healthAddress);

            // position
            g_state.playerCoords[g_state.Coord::X] = memory.Read<std::float_t>(entity + Offsets::AxisX);
            g_state.playerCoords[g_state.Coord::Y] = memory.Read<std::float_t>(entity + Offsets::AxisY);
            g_state.playerCoords[g_state.Coord::Z] = memory.Read<std::float_t>(entity + Offsets::AxisZ);

            // read ammo
            g_state.grenades = memory.Read<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Grenades }));
            g_state.shells =   memory.Read<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Shells }));
            g_state.bullets =  memory.Read<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Bullets}));
            g_state.cells =    memory.Read<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory ,  Offsets::Cells}));
            g_state.rockets =  memory.Read<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Rockets }));
            g_state.slugs =    memory.Read<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Slugs}));

            // infinite ammo
            if (g_state.infiniteAmmo) {
                memory.Write<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Grenades }), 999);
                memory.Write<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Shells }),   999);
                memory.Write<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Bullets }),  999);
                memory.Write<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Cells }),    999);
                memory.Write<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Rockets }),  999);
                memory.Write<std::int32_t>(memory.ResolvePointerChain(entity, { Offsets::Inventory , Offsets::Slugs }),    999);
            }
        }
        catch (const std::exception&) {}

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}