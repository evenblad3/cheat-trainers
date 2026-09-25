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
    constexpr std::ptrdiff_t AxisX = 0x4;
    constexpr std::ptrdiff_t AxisY = 0xC;
    constexpr std::ptrdiff_t AxisZ = 0x8;
    constexpr std::ptrdiff_t Grenades = 0x314;
    constexpr std::ptrdiff_t Shells  = 0x32C;
    constexpr std::ptrdiff_t Bullets = 0x330;
    constexpr std::ptrdiff_t Cells	 = 0x334;
    constexpr std::ptrdiff_t Rockets = 0x338;
    constexpr std::ptrdiff_t Slugs	 = 0x33C;
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
    bool infiniteAmmo = false;
    int grenades = 0;
    int shells = 0;
    int bullets = 0;
    int cells = 0;
    int rockets = 0;
    int slugs = 0;
};

inline GameState g_state;

#endif