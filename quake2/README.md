# Quake2
Offsets and stuff goes here, for [Yamagi Quake II client](https://github.com/yquake2/yquake2).

### Yamagi Quake II v8.70
| Description | Address | Offset(s) (Hex) | Size (Bytes) |
| :--- | :--- | :--- | :--- |
| **localPlayerAddr** | `[yquake2.exe+17A9560]` |  | 4
| **health** | `localPlayerAddr` | `0x1E0` | 4
| **maxHealth** | `localPlayerAddr` | `0x1E4` | 4
| **axisX** | `localPlayerAddr` | `0x4` | float
| **axisY** | `localPlayerAddr` | `0xC` | float
| **axisZ** | `localPlayerAddr` | `0x8` | float
| **inventory** | `localPlayerAddr` | `0x54` | 4
| **name** | `localPlayerAddr` | `inventory` + `0x2BC` | string
| **quadDamage** | `localPlayerAddr` | `inventory` + `0x340` | bool
| **invulnerability** | `localPlayerAddr` | `inventory` + `0x344` | bool
| **silencer** | `localPlayerAddr` | `inventory` + `0x348` | bool
| **rebreather** | `localPlayerAddr` | `inventory` + `0x344` | bool
| **environmentSuit** | `localPlayerAddr` | `inventory` + `0x350` | bool
