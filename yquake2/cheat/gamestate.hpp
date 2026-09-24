#ifndef GAMESTATE_HPP
#define GAMESTATE_HPP

#include "pch.h"

namespace Modules {
    inline std::uintptr_t client;
}

namespace Offsets {
    constexpr std::uintptr_t LocalPlayer = 0x17A9560;
    constexpr std::ptrdiff_t Health = 0x1E0;
    constexpr std::ptrdiff_t Inventory = 0x54;
    constexpr std::ptrdiff_t Grenades = 0x314;
    constexpr std::ptrdiff_t AxisX = 0x4;
    constexpr std::ptrdiff_t AxisY = 0xC;
    constexpr std::ptrdiff_t AxisZ = 0x8;

}

struct GameState {
    int playerHealth = 0;
    enum Coord {
        X = 0,
        Y = 1,
        Z = 2
    };
    float playerCoords[3] = { 123.0f, 456.0f, 789.0f };
    bool godmode = false;
    
};

inline GameState g_state;

#endif