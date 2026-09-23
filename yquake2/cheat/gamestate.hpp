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
}

struct GameState {
    int playerHealth = 0;
    bool godmode = false;
};

inline GameState g_state;

#endif