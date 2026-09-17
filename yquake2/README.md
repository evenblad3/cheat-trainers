# Quake2
Offsets and stuff goes here, for [Yamagi Quake II client](https://github.com/yquake2/yquake2).
Sizes here suggests a better way to access them, doesn't really mean you have to use the exact mentioned size.

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
| **handedness** | `localPlayerAddr` | `inventory` + `0x2CC` | 4
| **grenades** | `localPlayerAddr` | `inventory` + `0x314` | 4
| **shells** | `localPlayerAddr` | `inventory` + `0x32C` | 4
| **bullets** | `localPlayerAddr` | `inventory` + `0x330` | 4
| **cells** | `localPlayerAddr` | `inventory` + `0x334` | 4
| **rockets** | `localPlayerAddr` | `inventory` + `0x338` | 4
| **slugs** | `localPlayerAddr` | `inventory` + `0x33C` | 4
| **quadDamage** | `localPlayerAddr` | `inventory` + `0x340` | bool
| **invulnerability** | `localPlayerAddr` | `inventory` + `0x344` | bool
| **silencer** | `localPlayerAddr` | `inventory` + `0x348` | bool
| **rebreather** | `localPlayerAddr` | `inventory` + `0x344` | bool
| **environmentSuit** | `localPlayerAddr` | `inventory` + `0x350` | bool
| **bulletsMax** | `localPlayerAddr` | `inventory` + `0x6E4` | 4
| **shellsMax** | `localPlayerAddr` | `inventory` + `0x6E8` | 4
| **rocketsMax** | `localPlayerAddr` | `inventory` + `0x6EC` | 4
| **grenadesMax** | `localPlayerAddr` | `inventory` + `0x6F0` | 4
| **cellsMax** | `localPlayerAddr` | `inventory` + `0x6F4` | 4
| **slugsMax** | `localPlayerAddr` | `inventory` + `0x6F8` | 4
