# Inventory `.bin` formats referenced by `Inventory.c`

This document catalogs every `.bin` resource referenced in `src/Inventory.c`, including:

- exact BinIdentifier mapping,
- table/record format used by the code,
- field usage observed in decompiled functions.

Primary sources:

- `src/Inventory.c`
- `include/Player/Inventory/Items.h`
- `include/Player/Inventory/Pins.h`
- `build/usa/asm/main_29.s`, `build/usa/asm/main_31.s` (for resolved BinIdentifier constants)

---

## 1) BinIdentifier mapping used by `Inventory.c`

`BinIdentifier = { id, path }`.

From `Inventory.c` + rodata in `main_29.s/main_31.s`:

| Symbol | id | path |
|---|---:|---|
| `data_0205c128` | `30` | `Apl_Tak/ItemData.bin` |
| `data_0205c130` | `30` | `Apl_Tak/TreasureData.bin` |
| `data_0205c138` | `3` | `Apl_Tak/ItemData.bin` |
| `data_0205c140` | `30` | `Apl_Tak/ItemData.bin` |
| `data_0205c148` | `30` | `Apl_Tak/ItemData.bin` |
| `data_0205c150` | `43` | `../Data/BadgeData.bin` |
| `data_0205c158` | `30` | `../Data/BadgeData.bin` |
| `data_0205c160` | `30` | `../Data/BadgeData.bin` |
| `data_0205c168` | `30` | `../Data/BadgeData.bin` |
| `data_0205c170` | `3` | `Apl_Tak/ItemData.bin` |
| `data_0205c178` | `30` | `Apl_Tak/FoodData.bin` |
| `data_0205c180` | `3` | `Apl_Tak/ItemData.bin` |
| `data_0205c188` | `30` | `../Data/BadgeData.bin` |

Notes:

- `data_0205c148` and `data_0205c160` are uninitialized in current C, but their values are clear in asm rodata.
- Multiple symbols point to the same file path with different IDs (`3/30/43`), likely for resource/cache domain separation.

---

## 2) How records are loaded

`Inventory.c` uses:

- `Data_LoadToBuffer(type, var, &binId, index)`
- `Data_Load(type, var, &binId, index)`

From `DatMgr.h`, effective offset is:

- `offset = index * sizeof(var)`

So each `.bin` is treated as a **fixed-size record table** (no per-record header in this access path).

---

## 3) `../Data/BadgeData.bin` (pins/badges)

### 3.1 Record structure

Loaded as `RawPinData` (`include/Player/Inventory/Pins.h`), size `0x34`:

| Offset | Type | Field | Observed use |
|---|---|---|---|
| `0x08` | `u32` | `unk_08` | base value in `func_02023be8` (price/value-like) |
| `0x0C` | `s16` | `unk_0C` | per-level increment in `func_02023be8` |
| `0x25` | `u8` | `unk_25` | max/evolution level gate and logic (`Inventory_GetOpenPinStockpileCapacity`, add checks, comparison helpers) |

Most remaining fields are still unknown in this code path.

### 3.2 Entry count and indexing

- Global item IDs `< 304` map to pin IDs.
- Access usually uses either:
  - direct pin ID (`0..303`) when already in pin domain, or
  - `Inventory_GetCategorizedIndex(itemID)` for global IDs.

Implied active table domain in gameplay code: **304 pin records**.

### 3.3 Main callsites

- Capacity and level mismatch logic: `Inventory_GetOpenPinStockpileCapacity`
- Add policy logic: `Inventory_CanAddPin`, `Inventory_AddItem`
- Value calculation: `func_02023be8`
- Completion counting helpers: `func_020240e0`, `func_020241b0`

---

## 4) `Apl_Tak/ItemData.bin` (threads and thread stats)

### 4.1 Record structure

Loaded as `RawItemData` (`include/Player/Inventory/Items.h`), size `0x18`:

| Offset | Type | Field | Observed use |
|---|---|---|---|
| `0x02` | `u8` | `unk_02` | thread-style/category key in `func_02023e58`/`func_02023f60` |
| `0x04` | `u32` | `unk_04` | value/price path in `func_02023be8` (thread branch) |
| `0x0A` | `s16` | `defense` | stat calculations |
| `0x0C` | `s16` | `attack` | stat calculations |
| `0x0E` | `s16` | `health` | HP calculations |

### 4.2 Entry count and indexing

Global item ID partition in `Inventory_GetCategory`:

- `304..583` ¨ thread category

Thread index used against `ItemData.bin`:

- `threadIndex = itemID - 304`
- range `0..279` (280 entries)

### 4.3 Main callsites

- Player/friend effective stat calculation: `Stats_GetEffectiveValue`, `Stats_GetEffectiveFriendValue`, `Stats_GetMaxHealth`
- Thread-type uniformity checks: `func_02023e58`, `func_02023f60`
- Generic item value logic (thread branch): `func_02023be8`

---

## 5) `Apl_Tak/FoodData.bin`

### 5.1 Record structure

Loaded as `RawFoodData`, size `0x14`:

| Offset | Type | Field | Observed use |
|---|---|---|---|
| `0x04` | `s32` | `unk_04` | value/price-like field used in `func_02023be8` |

Other fields are not yet interpreted in current decomp.

### 5.2 Entry count and indexing

Global item IDs:

- `584..625` ¨ food category

Food index:

- `foodIndex = itemID - 584`
- range `0..41` (42 entries)

---

## 6) `Apl_Tak/TreasureData.bin`

### 6.1 Record structure

Loaded as `RawTreasureData`, size `0x08`:

| Offset | Type | Field | Observed use |
|---|---|---|---|
| `0x04` | `s32` | `unk_04` | value/price-like field used in `func_02023be8` |

### 6.2 Entry count and indexing

Global item IDs:

- `626..775` ¨ swag/treasure category

Treasure index:

- `swagIndex = itemID - 626`
- range `0..149` (150 entries)

---

## 7) Global item ID partition inferred from code

From `Inventory_GetCategory` and `Inventory_GetCategorizedIndex`:

| Global ID range | Category | Per-file index range |
|---|---|---|
| `0..303` | Pin | `0..303` (`BadgeData.bin`) |
| `304..583` | Thread | `0..279` (`ItemData.bin`) |
| `584..625` | Food | `0..41` (`FoodData.bin`) |
| `626..775` | Swag/Treasure | `0..149` (`TreasureData.bin`) |

Total modeled item universe in this module: `776` IDs.

---

## 8) Known uncertainties

- Field names `unk_*` remain provisional where no strong behavioral proof exists yet.
- IDs `3`, `30`, and `43` in `BinIdentifier` are resolved numerically but semantic naming for those resource domains is still unknown.
- This document reflects what `Inventory.c` consumes; other modules may use additional fields from the same files.
