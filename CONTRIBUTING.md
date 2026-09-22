# Contributing Guidelines
## General Advice
- This repository is for helping developers who make cheats for old games, for fun.
- Open issues for any open ended questions.
- Make sure you're not submitting cheat tables for any MMO games.
- Try to contribute the cheats as a table as mentioned below.

## Contributing a cheat table
- Make sure they're organized as a struct and/or a table. For example:
```cpp
namespace Offsets {
    constexpr std::uintptr_t LocalPlayer = 0x17A9560;
    constexpr std::ptrdiff_t Health    = 0x1E0;
    constexpr std::ptrdiff_t Inventory = 0x54;
    constexpr std::ptrdiff_t Grenades  = 0x314;
}
```
```
### Game Name verX
| Description | Address | Offset(s) (Hex) | Size (Bytes) |
| :--- | :--- | :--- | :--- |
| **localPlayerAddr** | `[gamename.exe+17A9560]` |  | 4
| **health** | `localPlayerAddr` | `0x1E0` | 4
| **inventory** | `localPlayerAddr` | `0x54` | 4
| **grenades** | `localPlayerAddr` | `inventory` + `0x314` | 4
```
- Mentioning how to access an variable will help the reader to catch up with rest of the information.
