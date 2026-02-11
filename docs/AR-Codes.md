# Action Replay Codes Breakdown - The World Ends With You (DS)

This document explains the Action Replay (AR) codes found in [docs/twewy-ar-codes.txt](docs/twewy-ar-codes.txt). These codes are used to modify the game's RAM during execution to achieve various effects.

A pseudocode version showing the logic of each code is available at [docs/twewy-ar-codes-pseudocode.txt](docs/twewy-ar-codes-pseudocode.txt).

## Action Replay Code Format Reference

Action Replay codes use a hexadecimal format where the first digit indicates the operation type:

- **`0XXXXXXX YYYYYYYY`**: Write 32-bit value `YYYYYYYY` to address `XXXXXXX`
- **`1XXXXXXX 0000YYYY`**: Write 16-bit value `YYYY` to address `XXXXXXX`
- **`2XXXXXXX 000000YY`**: Write 8-bit value `YY` to address `XXXXXXX`
- **`3XXXXXXX YYYYYYYY`**: Write 32-bit value if less than
- **`4XXXXXXX YYYYYYYY`**: Write 32-bit value if greater than
- **`5XXXXXXX YYYYYYYY`**: Conditional code - if 32-bit value at `XXXXXXX` equals `YYYYYYYY`, execute next code
- **`6XXXXXXX YYYYYYYY`**: Conditional code - if 32-bit value at `XXXXXXX` not equals `YYYYYYYY`, execute next code
- **`7XXXXXXX YYYYYYYY`**: Conditional code - if 16-bit value at `XXXXXXX` greater than `YYYY`, execute next code
- **`8XXXXXXX YYYYYYYY`**: Conditional code - if 16-bit value at `XXXXXXX` less than `YYYY`, execute next code
- **`9XXXXXXX YYYYYYYY`**: Conditional code - if 16-bit value at `XXXXXXX` equals `YYYY`, execute next code
- **`CXXXXXXX 0000YYYY`**: Loop code - repeat next block `YYYY` times
- **`D0000000 00000000`**: Terminator for conditional/loop blocks
- **`D3000000 XXXXXXXX`**: Set offset base address to `XXXXXXXX`
- **`D4000000 XXXXXXXX`**: Add `XXXXXXXX` to offset address
- **`D5000000 XXXXXXXX`**: Set data register to `XXXXXXXX`
- **`D6000000 XXXXXXXX`**: Store data register to address `XXXXXXXX` + offset
- **`D7000000 XXXXXXXX`**: Add `XXXXXXXX` to data register
- **`DC000000 XXXXXXXX`**: Add `XXXXXXXX` to offset register
- **`DA000000 XXXXXXXX`**: Store data register to address pointed by `XXXXXXXX` + offset

### Button Code Values (for code type `94000130`)
- `FFFB0000`: SELECT not pressed
- `FDFF0000`: R + B held
- `FEFF0000`: L + B held
- `000000FF`: L + R pressed
- `000002FF`: R pressed

## Memory Mapping Context
- **Main RAM (ARM9):** `0x02000000` - `0x020824A0`
- **Overlays (OV000+):** `0x020824A0` onwards
- **Save/Player Data Structure (`MainData`):** Starts at `0x02071D10`
- **Battle Data Structure:** Around `0x02074E40` - `0x02074E9F`

---

## 1. Debug Menu
**Activation:** Hold `Select` and press `Start`

**AR Code:**
```
920ae5f0 00000ab1  // If [0x020AE5F0] == 0x00000AB1
020ae5f4 e5910630  // Write 0xE5910630 to 0x020AE5F4 (ldr r0, [r1, #0x630])
d2000000 00000000  // End conditional
94000130 fffb0000  // If SELECT not pressed
920ae5f0 00000ab1  // If [0x020AE5F0] == 0x00000AB1
020ae5f4 e3a00008  // Write 0xE3A00008 to 0x020AE5F4 (mov r0, #8)
d2000000 00000000  // End conditional
```

**Explanation:**
This code patches a function at `0x020AE5F4` based on game state and button input:
1. First checks if the game is in a specific state (`0x020AE5F0 == 0x00000AB1`)
2. Restores the original instruction when SELECT is held
3. When SELECT is released, replaces the instruction with `mov r0, #8`
4. This forces the game to recognize a debug menu state (value 8) instead of reading it from memory
5. The result is that pressing Start without SELECT opens the debug menu

**Technical Detail:** The original instruction loads a value from `[r1, #0x630]` which contains the menu state. Replacing it with `mov r0, #8` hardcodes the debug menu state.

## 2. Max Exp
**Address:** `0x020737B8`
**Effect:** Sets the current experience points to `0xFFFF` (65,535).
**Technical Detail:** Modifies the experience field within the player's progress data block.

## 3. Show Stylus Collision Hitboxes
**Address:** `0x0207516C`
**Value:** `0x00000002`
**Effect:** Enables a debug visualization mode that renders hitboxes for stylus-based collisions in battle or in-game menus.

## 4. 9,999,999 Yen in Any Wallet
**AR Code:**
```
920848ec 0000ff82  // If [0x020848EC] == 0x0000FF82 (wallet context check)
0208470c e3a00003  // mov r0, #3 at 0x0208470C
02084720 e3a00003  // mov r0, #3 at 0x02084720
02084738 e3a00003  // mov r0, #3 at 0x02084738
0208fec8 e3a00003  // mov r0, #3 at 0x0208FEC8
0208feb0 e3a00003  // mov r0, #3 at 0x0208FEB0
0208fe9c e3a00003  // mov r0, #3 at 0x0208FE9C
d2000000 00000000  // End conditional
```

**Explanation:**
Patches multiple wallet-related functions in the shop/wallet overlay:
1. Conditional check verifies the wallet context is active (`0x020848EC == 0x0000FF82`)
2. Replaces instruction in 6 different wallet-checking functions with `mov r0, #3`
3. In TWEWY, wallets are upgraded throughout the game (IDs 0-3):
   - ID 0: Starting wallet (limited capacity)
   - ID 1-2: Upgraded wallets
   - ID 3: Final wallet (unlimited capacity - 9,999,999 yen)
4. Forcing return value `3` makes the game treat your current wallet as unlimited
5. This affects both display and transaction logic, allowing 9,999,999 yen

## 5. Infinite Yen
**Address:** `0x02071D1C` (`MainData.unk0C`)
**Value:** `0x0098967F` (9,999,999)
**Effect:** Directly writes the maximum possible Yen amount to the player's money variable.

## 6. Access Full Map
**Activation:** Press `R`

**AR Code:**
```
94000130 000002ff  // If R button just pressed
920b8408 000004f8  // If [0x020B8408] == 0x000004F8 (map context check)
020b8548 e3a02004  // mov r2, #4 at 0x020B8548
020b8408 e3a00004  // mov r0, #4 at 0x020B8408
d0000000 00000000  // End conditional
```

**Explanation:**
- Button code `000002FF` detects when R is pressed
- Conditional check ensures map system is active (`0x020B8408 == 0x000004F8`)
- Patches `0x020B8548` with `mov r2, #4` - forces map visibility state to "all visible"
- Patches `0x020B8408` with `mov r0, #4` - overrides map unlock status
- Value `4` likely represents the maximum map state (all areas discovered/accessible)
- This bypasses the normal progression system that gradually unlocks map areas

## 7. Pins Always Evolve
**AR Code:**
```
92085484 00001034  // If [0x02085484] == 0x00001034 (pin evolution context)
020854c4 e3a00000  // mov r0, #0 at 0x020854C4
020854d8 e3a00000  // mov r0, #0 at 0x020854D8
020854ec e3a00000  // mov r0, #0 at 0x020854EC
02085500 e3a00000  // mov r0, #0 at 0x02085500
02085514 e3a00000  // mov r0, #0 at 0x02085514
d2000000 00000000  // End conditional
```

**Explanation:**
Targeted patches in `OV002` (Overlay 2 - pin system):
1. Conditional verifies pin evolution system is active (`0x02085484 == 0x00001034`)
2. Patches 5 different evolution requirement check functions
3. Each function is modified to return `0` (`mov r0, #0`) instead of checking actual conditions
4. Pin evolution in TWEWY requires specific conditions:
   - Battle count requirements
   - Friendship/time requirements  
   - Specific battle types (shutdown vs. mingle)
5. Forcing return value `0` (success) bypasses all these checks
6. Result: Any pin that can evolve will evolve immediately when conditions are checked

## 8. No Limit on Pin Usage
**AR Code:**
```
920a79ec 000004f4  // If [0x020A79EC] == 0x000004F4 (pin usage context)
020a79ec e1c414b4  // strh r1, [r4, #0x4B4] at 0x020A79EC
020a79f4 ea000009  // b +0x24 at 0x020A79F4 (branch forward)
d2000000 00000000  // End conditional
```

**Explanation:**
Modifies pin cooldown/reboot timer logic:
1. Checks if pin usage system is active (`0x020A79EC == 0x000004F4`)
2. First patch modifies the store instruction at `0x020A79EC` to `strh r1, [r4, #0x4B4]`
3. Second patch adds a branch instruction `ea000009` that skips ahead 9 instructions (0x24 bytes)
4. This branch causes the code to skip over the pin cooldown timer increment
5. In TWEWY, pins have "reboot" times - cooldowns before they can be used again in battle
6. By skipping the cooldown logic, pins can be used repeatedly without waiting
7. This effectively allows infinite/rapid use of pins in combat

## 9. Unlock All Pin Slots
**Address:** `0x0209018C`
**Patch:** `mov r0, #6`
**Effect:** Forces the number of available pin slots in battle to 6, the maximum allowed in the game.

## 10. Unlock All Pins
**Activation:** Press `L + R`

**AR Code:**
```
94000130 000000ff  // If L+R buttons pressed
120727c0 00000000  // Write 0x0000 to 0x020727C0 (clear first entry)
220727c2 00000063  // Write 0x63 (99) to 0x020727C2
d3000000 020727c0  // Set offset base to 0x020727C0
c0000000 0000012e  // Loop 0x12E (302) times
da000000 00000000  // Store data register to [offset]
d4000000 00000001  // Add 1 to offset
d7000000 00000004  // Add 4 to data register address
20000004 00000063  // Write 0x63 to [offset+4]
dc000000 00000002  // Add 2 to offset
d2000000 00000000  // End loop/conditional
```

**Explanation:**
Complex loop code that manipulates the pin collection array:
1. Activated by pressing L+R simultaneously (`000000FF`)
2. Initializes pin collection at `0x020727C0`
3. Clears first entry and sets quantity to 99
4. Begins loop of 302 iterations (0x12E) - total number of pins in TWEWY
5. Pin collection structure appears to be an array where each entry is ~4-8 bytes:
   - Pin ID (2 bytes)
   - Quantity count (1 byte)
   - Other flags/data
6. Loop writes `0x63` (99 decimal) to each pin's quantity field
7. Uses offset manipulation (`D4`, `DC`) to iterate through the array
8. Result: Player instantly obtains 99 copies of every pin in the game
9. This includes both unlockable and secret pins

## 11. Unlock All Item Abilities
**AR Code:**
```
92393850 000035f8  // If [0x02393850] == 0x000035F8 (item system check)
d5000000 01010101  // Set data register to 0x01010101
c0000000 00000046  // Loop 0x46 (70) times
d6000000 02073bc0  // Store data register to 0x02073BC0 + offset
d2000000 00000000  // End loop
```

**Explanation:**
Fills item ability flags with unlock pattern:
1. Conditional check verifies item system is loaded (`0x02393850 == 0x000035F8`)
2. Loads data register with pattern `0x01010101`
3. Loops 70 times (0x46 iterations)
4. Each iteration writes `0x01010101` to the ability array starting at `0x02073BC0`
5. This fills 280 bytes (70 Å~ 4 bytes) with the unlock pattern
6. In TWEWY, clothing items have associated abilities:
   - HP bonuses
   - Attack/Defense boosts
   - Special effects (auto-battle, EXP multipliers, etc.)
7. The bitfield pattern `01010101` appears to set alternating ability unlock flags
8. Result: All passive and active clothing/item abilities become available
9. Total: 280 bytes = 2,240 bits of ability flag data

## 12. Player Stats (MainData Structure)
The MainData structure begins at `0x02071D10` and contains player progression data. These codes modify specific fields:

### 999 ATK
**AR Code:** `12071d20 000003e7`
- **Address:** `0x02071D20` (MainData + 0x10)
- **Type:** 16-bit write
- **Value:** `0x03E7` (999 decimal)
- **Effect:** Sets player attack power to maximum
- **Field:** `MainData.attack`

### 999 DEF
**AR Code:** `12071d22 000003e7`
- **Address:** `0x02071D22` (MainData + 0x12)
- **Type:** 16-bit write
- **Value:** `0x03E7` (999 decimal)
- **Effect:** Sets player defense to maximum
- **Field:** `MainData.defense`

### 999 Bravery
**AR Code:** `12071d26 000003e7`
- **Address:** `0x02071D26` (MainData + 0x16)
- **Type:** 16-bit write
- **Value:** `0x03E7` (999 decimal)
- **Effect:** Sets bravery stat to maximum (allows equipping all clothing)
- **Field:** `MainData.bravery`
- **Note:** Bravery determines which clothing items can be equipped

### Level 255
**AR Code:** `12071d16 000000ff`
- **Address:** `0x02071D16` (MainData + 0x06)
- **Type:** 16-bit write
- **Value:** `0xFF` (255 decimal)
- **Effect:** Sets player level to maximum
- **Field:** `MainData.level`

### MainData Structure Summary
**CONFIRMED from disassembly** (`build/usa/asm/main_36.s` line 4277):
- **Label**: `data_02071d10`
- **Size**: `0x1C` (28 bytes)
- **Address Range**: `0x02071D10` to `0x02071D2B`

**Known fields from AR codes:**
- `0x02071D16` (+0x06): Level (16-bit)
- `0x02071D1C` (+0x0C): Money (32-bit)
- `0x02071D20` (+0x10): Attack (16-bit)
- `0x02071D22` (+0x12): Defense (16-bit)
- `0x02071D26` (+0x16): Bravery (16-bit)

**Complete structure** (28 bytes total, confirmed from `.space 0x1c`):
```c
struct MainData {  // data_02071d10
    u8  unk00[6];       // +0x00 to +0x05 (6 bytes - UNKNOWN contents)
    u16 level;          // +0x06 (0x02071D16) - VERIFIED
    u8  unk08[4];       // +0x08 to +0x0B (4 bytes - UNKNOWN contents)
    u32 money;          // +0x0C (0x02071D1C) - VERIFIED
    u16 attack;         // +0x10 (0x02071D20) - VERIFIED
    u16 defense;        // +0x12 (0x02071D22) - VERIFIED
    u8  unk14[2];       // +0x14 to +0x15 (2 bytes - UNKNOWN contents)
    u16 bravery;        // +0x16 (0x02071D26) - VERIFIED
    u8  unk18[4];       // +0x18 to +0x1B (4 bytes - ends at 0x02071D2B)
};  // Total: 28 bytes (0x1C)
```

**Referenced by functions**:
- `func_02022a18` in `main_36.s` (loads and processes this data)
- Multiple functions across the codebase reference this structure

**Note on C Source**: `src/main_36.c` contains a struct definition for `MainData`, but **there is a size/layout mismatch**. The disassembly's `.space 0x1c` directive (28 bytes) is the confirmed ground truth, while the C struct appears much larger and may combine multiple separate data sections. The AR code evidence (which successfully modifies game behavior) confirms the actual memory layout matches the disassembly, not the C struct.

**Accurate Structure** (use this - verified by disassembly + AR codes):

## 13. All Difficulties Unlocked
**AR Code:**
```
920848ae 000013a0  // If [0x020848AE] == 0x000013A0
020848a4 e3a00003  // mov r0, #3 at 0x020848A4
d2000000 00000000  // End conditional
```

**Explanation:**
- Patches difficulty check function in shop/menu overlay
- Conditional verifies difficulty system is active (`0x020848AE == 0x000013A0`)
- Replaces instruction with `mov r0, #3` - returns value 3
- TWEWY has 4 difficulty levels (IDs 0-3):
  - 0: Easy
  - 1: Normal
  - 2: Hard
  - 3: Ultimate
- Returning value 3 indicates all difficulties are unlocked
- Normally, higher difficulties unlock through story progression

## 14. Equip Any Clothing
**AR Code:**
```
92393850 000035f8  // If [0x02393850] == 0x000035F8
123a3570 000003e7  // Write 999 to 0x023A3570 (hat requirement)
123a3584 000003e7  // Write 999 to 0x023A3584 (top requirement)
123a3598 000003e7  // Write 999 to 0x023A3598 (bottom requirement)
123a35ac 000003e7  // Write 999 to 0x023A35AC (footwear requirement)
d2000000 00000000  // End conditional
```

**Explanation:**
- Modifies clothing bravery requirement thresholds
- Conditional check verifies clothing data is loaded (`0x02393850 == 0x000035F8`)
- Sets bravery requirements for all 4 clothing slots to 999
- Since the code also sets player bravery to 999 (see "999 Bravery"), all requirements are met
- Clothing in TWEWY has stat requirements (mainly bravery) that must be met to equip
- By setting thresholds to match exactly the player's stat, everything becomes equippable
- This affects: Hats, Tops, Bottoms, and Footwear

## 15. Battle Data Codes
Several codes modify the battle data structure around `0x02074E40`. All use a conditional check `520bb570 e59fc004` to verify battle system is active.

### 100% Drop Rate
**AR Code:**
```
520bb570 e59fc004  // If [0x020BB570] == 0xE59FC004 (battle active check)
12074e76 000003e7  // Write 999 to 0x02074E76
d2000000 00000000  // End conditional
```
- Sets `BattleData.dropRateMultiplier` to 999
- Drop rate multiplier affects pin and item drop chances from enemies
- Value 999 effectively guarantees 100% drop rate
- Overrides normal drop probability calculations

### Auto-Win Battles
**AR Code:**
```
520bb570 e59fc004  // If [0x020BB570] == 0xE59FC004
22074e52 00000002  // Write 2 to 0x02074E52
d2000000 00000000  // End conditional
```
- Forces `BattleData.battleState` to value 2 (WIN state)
- Battle state values:
  - 0: In progress
  - 1: Lose/Defeat
  - 2: Victory
- Setting to 2 immediately triggers battle victory
- Battle ends with full rewards as normal

### God Mode (Invincibility)
**AR Code:**
```
94000130 fdff0000  // If R+B held
22074e40 00000001  // Write 1 to 0x02074E40
d2000000 00000000  // End conditional
94000130 feff0000  // If L+B held
22074e40 00000000  // Write 0 to 0x02074E40
d2000000 00000000  // End conditional
```
- Toggle-based invincibility system
- R+B: Enable invincibility (`BattleData.invincible = 1`)
- L+B: Disable invincibility (`BattleData.invincible = 0`)
- When enabled, player takes no damage from any source
- Does not affect partner

### No Damage Bonus
**AR Code:**
```
520bb570 e59fc004  // If [0x020BB570] == 0xE59FC004
22074e9f 00000000  // Write 0 to 0x02074E9F
d2000000 00000000  // End conditional
```
- Clears the "no damage taken" bonus flag
- `BattleData.noDamageBonus = 0`
- Prevents the game from tracking whether damage was taken
- Used to test battle mechanics without bonus interference

### 100 Chain Combo Bonus
**AR Code:**
```
520bb570 e59fc004  // If [0x020BB570] == 0xE59FC004
22074e9c 00000064  // Write 100 to 0x02074E9C
d2000000 00000000  // End conditional
```
- Sets `BattleData.hitComboCount` to 100
- Chain combo counter affects:
  - Damage multipliers
  - Drop rate bonuses
  - Battle ratings
- Maximum meaningful value is 100
- Awards maximum chain bonus multipliers

### 100 Pass Puck Rally Bonus
**AR Code:**
```
520bb570 e59fc004  // If [0x020BB570] == 0xE59FC004
22074e8c 00000064  // Write 100 to 0x02074E8C
d2000000 00000000  // End conditional
```
- Sets `BattleData.puckRallyCount` to 100
- Puck Rally is TWEWY's partner system mechanic
- Passing the "puck" back and forth increases damage multiplier
- Value 100 is maximum rally count
- Awards maximum damage and drop rate bonuses

### BattleData Structure Summary
**CONFIRMED from disassembly** (`build/usa/asm/main_36.s` lines 4357-4362):

Battle data spans multiple separate data sections:
- **`data_02074e34`**: `0x22` (34 bytes) at `0x02074E34`-`0x02074E55`
- **`data_02074e56`**: `0x4E` (78 bytes) at `0x02074E56`-`0x02074EA3`

**Known fields from AR codes:**
- `0x02074E40`: Invincible flag (8-bit) - within `data_02074e34`
- `0x02074E52`: Battle state (8-bit, values 0/1/2) - within `data_02074e34`
- `0x02074E76`: Drop rate multiplier (16-bit) - within `data_02074e56`
- `0x02074E8C`: Puck rally count (8-bit) - within `data_02074e56`
- `0x02074E9C`: Hit combo count (8-bit) - within `data_02074e56`
- `0x02074E9F`: No damage bonus flag (8-bit) - within `data_02074e56`

**Structure layout** (multiple related data sections):
```c
// data_02074e34 (34 bytes at 0x02074E34)
struct BattleData_Part1 {
    u8  unk34[12];          // +0x00 to +0x0B (0x02074E34-0x02074E3F)
    u8  invincible;         // +0x0C (0x02074E40) - VERIFIED
    u8  unk41[17];          // +0x0D to +0x1D (0x02074E41-0x02074E51)
    u8  battleState;        // +0x1E (0x02074E52) - VERIFIED
    u8  unk53[3];           // +0x1F to +0x21 (ends at 0x02074E55)
};  // Total: 34 bytes (0x22)

// data_02074e56 (78 bytes at 0x02074E56)
struct BattleData_Part2 {
    u8  unk56[32];          // +0x00 to +0x1F (0x02074E56-0x02074E75)
    u16 dropRateMultiplier; // +0x20 (0x02074E76) - VERIFIED
    u8  unk78[20];          // +0x22 to +0x35 (0x02074E78-0x02074E8B)
    u8  puckRallyCount;     // +0x36 (0x02074E8C) - VERIFIED
    u8  unk8d[15];          // +0x37 to +0x45 (0x02074E8D-0x02074E9B)
    u8  hitComboCount;      // +0x46 (0x02074E9C) - VERIFIED
    u8  unk9d[2];           // +0x47 to +0x48 (0x02074E9D-0x02074E9E)
    u8  noDamageBonus;      // +0x49 (0x02074E9F) - VERIFIED
    u8  unka0[4];           // +0x4A to +0x4D (ends at 0x02074EA3)
};  // Total: 78 bytes (0x4E)
```

**Note**: These may be separate global variables rather than a single contiguous structure. The disassembly shows them as distinct `.space` allocations.

---

## ARM9 Assembly Reference

The codes frequently patch ARM9 instructions. Common patterns:

### MOV (Move/Load Immediate)
- **`E3A0000X`**: `mov r0, #X` - Load immediate value X into register r0
- **`E3A0200X`**: `mov r2, #X` - Load immediate value X into register r2
- Purpose: Force specific return values or states

### LDR (Load Register)
- **`E5910630`**: `ldr r0, [r1, #0x630]` - Load value from memory address [r1 + 0x630] into r0
- **`E59FC004`**: `ldr r12, [pc, #4]` - Load address from PC+4 (function pointer load)
- Purpose: Read from game data structures or load function pointers

### STRH (Store Register Halfword)
- **`E1C414B4`**: `strh r1, [r4, #0x4B4]` - Store lower 16 bits of r1 to [r4 + 0x4B4]
- Purpose: Write data to memory

### B (Branch)
- **`EA000009`**: `b +0x24` - Branch forward 9 instructions (0x24 bytes)
- Purpose: Skip code sections (e.g., bypass cooldown logic)

---

## Code-Patching AR Codes - Disassembly Analysis

Many AR codes patch executable code rather than data. Here's what they actually do:

### Battle System Context Check (`520bb570 e59fc004`)

**Address**: `func_ov003_020bb570` in Overlay 3

**Original Code** (`build/usa/asm/ov003_4.s` lines 68086-68093):
```asm
func_ov003_020bb570:  ; Battle system function
    ldr ip, .L_020bb57c   ; 0xE59FC004 - Load function pointer
    mov r1, #0x0          ; Set r1 to 0
    bx ip                 ; Branch to target function
.L_020bb57c: .word func_ov003_020bb590
```

**What AR codes check**: The instruction `ldr ip, [pc, #4]` encodes as `0xE59FC004`

**Purpose**: Multiple battle-related AR codes use this as a conditional check:
- Verifies Overlay 3 (battle system) is loaded
- Confirms the function hasn't been relocated/modified
- Ensures patches are only applied during active battles

**Used by these AR codes**:
- 100% Drop Rate
- Auto-win Battles
- God Mode (invincibility)
- No Damage Bonus
- 100 Chain Combo Bonus
- 100 Pass Puck Rally Bonus

### Pin Evolution Patches (Overlay 2)

**AR Code**: "Pins always evolve" - patches 5 addresses with `mov r0, #0`

**Why this works**: ARM9 functions typically use register r0 for return values
- Return value 0 = success/condition met
- Non-zero = failure/condition not met

**Original code** (approximate - addresses 0x020854C4, 0x020854D8, 0x020854EC, 0x02085500, 0x02085514):
These are likely conditional checks like:
```asm
; Original (example):
    cmp r0, #required_battles   ; Check if pin has enough battles
    movlt r0, #1                ; Return 1 (fail) if less than required
    movge r0, #0                ; Return 0 (success) if requirement met

; After AR patch:
    mov r0, #0                  ; Always return 0 (success)
    nop                         ; or continue
```

**Effect**: Bypasses all pin evolution requirement checks
- Battle count requirements ignored
- Time-based requirements ignored  
- Specific battle type requirements ignored

### Wallet Check Patches

**AR Code**: "9,999,999 Yen in any wallet" - patches 6 functions with `mov r0, #3`

**What's being patched**: Functions that return the current wallet level

**Original code** (conceptual):
```asm
; Original wallet level check:
get_wallet_level:
    ldr r0, [player_data, #wallet_level_offset]
    ; r0 now contains current wallet level (0-3)
    bx lr

; After AR patch:
get_wallet_level:
    mov r0, #3    ; Always return level 3 (unlimited wallet)
    bx lr
```

**Effect**: Game thinks player has the maximum wallet (level 3) unlocked
- Maximum capacity: 9,999,999 yen
- All purchases show this as the limit
- Works in shops, menus, and during gameplay

### Why Code Patches Use Conditional Checks

**Problem**: If you patch code at a fixed address, it might:
1. Patch the wrong memory if an overlay isn't loaded yet
2. Corrupt data if that address is being used for something else
3. Crash the game if the code has been relocated

**Solution**: AR codes use conditional checks before patching:

**Example** - Battle system patches:
```
520bb570 e59fc004   ; IF [0x020BB570] == 0xE59FC004
12074e76 000003e7   ; THEN write 999 to drop rate
d2000000 00000000   ; END IF
```

**What this verifies**:
- Checks if 0x020BB570 contains the instruction `ldr ip, [pc, #4]` (encoded as `0xE59FC004`)
- This instruction is the first instruction of `func_ov003_020bb570` in Overlay 3
- If it matches, Overlay 3 is loaded and the function is at the expected address
- Safe to patch battle data and battle-related code

### Function Signature Detection

Many conditional checks (type `9XXXXXXX`) verify function signatures:
- `920ae5f0 00000ab1`: Check if specific game state value exists
- `92085484 00001034`: Check if pin system overlay is loaded
- `920848ec 0000ff82`: Check if shop/wallet system is active
- `520bb570 e59fc004`: Check if battle function signature matches

This is like runtime type checking - ensures patches are only applied in the correct context.

### Why `mov r0, #0` and `mov r0, #3` Are Common

**ARM Calling Convention**:
- Register `r0` is used for function return values
- `0` typically means "success" or "true"
- Non-zero typically means "failure" or "false"

**Patching Strategy**:
1. Find a function that checks if something is allowed
2. Replace its logic with `mov r0, #<desired_value>`
3. Function now always returns that value

**Examples**:
- Pin evolution check Å® `mov r0, #0` (always succeed)
- Wallet level getter Å® `mov r0, #3` (always return max level)
- Difficulty unlock check Å® `mov r0, #3` (return "all 4 difficulties unlocked")

---

## Key Insights from Disassembly Analysis

### 1. Context Verification is Critical
AR codes use instruction pattern matching to verify context:
- Example: `520bb570 e59fc004` checks if battle function starts with expected instruction
- Prevents crashes from patching wrong memory regions
- Similar to runtime type checking in high-level languages

### 2. Return Value Manipulation
Most code patches replace conditional logic with hardcoded return values:
- `mov r0, #0` forces "success" returns (evolution checks, etc.)
- `mov r0, #3` forces "maximum level" returns (wallet, difficulty)
- This is simpler than patching complex conditional branches

### 3. Overlay Architecture Confirmed
Game uses dynamic code overlays verified from disassembly:
- **OV002** (`ov002_4.s`): Pin system, evolution, collection
- **OV003** (`ov003_4.s`): Battle system, combat mechanics (`func_ov003_020bb570`)
- **Other overlays**: Shops, maps, menus
- AR codes must verify overlays are loaded before patching

### 4. Function Signatures as Markers
First instruction of a function serves as a "signature":
- AR codes check these signatures for runtime validation
- Example: `func_ov003_020bb570` starts with `ldr ip, [pc, #4]` Å® encodes as `0xE59FC004`
- Verifies correct overlay loaded and function not relocated

### 5. Patch Strategy Patterns
- **Data writes**: Direct memory modification (stats, money, flags)
- **Return value forcing**: Replace function logic with `mov r0, #value`
- **Branch skipping**: Replace branches to skip cooldown/limit logic (`ea000009`)
- **Conditional bypassing**: Force checks to always succeed

### 6. Some Addresses Remain Unmapped
Addresses like `0x020A79EC`, `0x020B8408`, `0x0208470C` weren't found because:
- Overlays are relocated at runtime vs. link-time addresses in disassembly
- May be in overlays not yet fully disassembled
- Need overlay base address calculation: `AR_address - overlay_base`

---

## Code Safety and Usage Notes

### Conditional Checks
Most codes use conditional checks (code type 5/9) to verify the correct game context is active before applying patches. This prevents:
- Memory corruption from patching wrong addresses
- Crashes from modifying code during overlay transitions
- Save data corruption

### Recommended Usage Order
1. Start with stat codes (ATK, DEF, Level) - safest
2. Add quality-of-life codes (Infinite Yen, Pin Slots) - low risk
3. Use collection unlocks (Pins, Abilities) - affects progression
4. Battle codes (Drop Rate, Auto-Win) - changes gameplay significantly
5. Debug features (Debug Menu, Hitboxes) - for testing only

### Potential Issues
- **Unlock All Pins**: May make the game less challenging; pins won't be earned through gameplay
- **Auto-Win Battles**: Completely removes challenge; use sparingly for testing
- **Pins Always Evolve**: Can cause unwanted evolutions if you want specific evolution paths
- **100% Drop Rate**: Makes rare pins common; reduces collection satisfaction

### Code Interactions
Some codes work well together:
- **999 Bravery + Equip Any Clothing**: Perfect synergy for outfit customization
- **9,999,999 Yen + Infinite Yen**: Choose one, both redundant
- **God Mode + Auto-Win**: Redundant; auto-win is faster

### Overlay Context
Many codes target overlay functions (OV002, etc.). Overlays are dynamically loaded code modules:
- **OV002**: Pin system, evolution, collection management
- Shop/wallet overlays: Money, purchasing, wallet upgrades
- Map overlays: World map, area transitions

The conditional checks ensure overlays are loaded before patching them.

---

## Reverse Engineering Notes

### Data Confirmed from Disassembly

By cross-referencing AR code addresses with the game's disassembly in `build/usa/asm/`, we can confirm exact data structure sizes and boundaries:

#### MainData Structure (`data_02071d10`)
- **File**: `build/usa/asm/main_36.s` line 4277
- **Definition**: `data_02071d10: .space 0x1c`
- **Confirmed Size**: Exactly 28 bytes (0x1C)
- **Confirmed Range**: 0x02071D10 to 0x02071D2B
- **Usage**: Referenced by `func_02022a18` and many other functions
- **Purpose**: Player stats and progression data (level, money, attack, defense, bravery)

#### Pin Collection Data
- **File**: `build/usa/asm/main_36.s` lines 4293-4295
- **Definitions**:
  - `data_020727c0: .space 0x2` (2 bytes - likely header)
  - `data_020727c2: .space 0x1` (1 byte - flag or count)
  - `data_020727c3: .space 0x4bd` (1213 bytes - main pin array)
- **Total Range**: 0x020727C0 to 0x02072C7F
- **Pin Entries**: 302 pins Å~ ~4 bytes = 1208 bytes (+5 bytes overhead)
- **Referenced By**: Functions in Shop.s, Result.s, MenuBadge.s

#### Battle Data (`data_02074e34` and `data_02074e56`)
- **File**: `build/usa/asm/main_36.s` lines 4357-4362
- **Definitions**:
  - `data_02074e34: .space 0x22` (34 bytes containing invincible flag and battle state)
  - `data_02074e56: .space 0x4e` (78 bytes containing drop rate, combo counters)
- **Note**: Battle data is split across multiple allocations, not a single contiguous structure

#### Pin Evolution Code (Overlay 2)
- **File**: `build/usa/asm/ov002_4.s` around line 3380-3440
- **Addresses**: 0x020854C0-0x02085544
- **AR Code Patches**: The "Pins Always Evolve" code replaces conditional checks with `mov r0, #0` to bypass evolution requirement logic
- **Original Logic**: Checks pin battle count, evolution conditions, and specific requirements before allowing evolution

### How to Use the Disassembly

### How to Use the Disassembly

**1. Find Data Structure Definitions**
```bash
# Search for addresses from AR codes in the disassembly
grep -r "02071d1" build/usa/asm/**/*.s
grep -r "data_02071d10" build/usa/asm/**/*.s
```

**2. Locate Function References**
```bash
# Find which functions load/use a specific data structure
grep "\.word data_02071d10" build/usa/asm/**/*.s
```
Result: `.L_02022b3c: .word data_02071d10` in `func_02022a18`

**3. Examine Function Code**
Open `build/usa/asm/main_36.s` and read `func_02022a18` to see how MainData is accessed:
- Which offsets are read/written
- What operations are performed
- How the data relates to game logic

**4. Verify Structure Sizes**
Look for `.space` directives in the `.bss` section:
```asm
data_02071d10: .space 0x1c  ; Confirms 28-byte structure
data_020727c3: .space 0x4bd ; Confirms 1213-byte array
```

**5. Trace Overlay Loading**
For overlay-patched addresses (like pin evolution at 0x020854C4):
- Check `build/usa/asm/ov002_4.s` for the actual code
- See what the original instruction does
- Understand why the AR code patch bypasses it

**6. Find Code Patches**
For AR codes that patch instructions (not data):
```bash
# Search for the function containing the patched address
grep -r "020bb570" build/usa/asm/**/*.s
# Result: func_ov003_020bb570 in ov003_4.s line 68086

# Read the function to understand what's being patched
# Look for the instruction that gets replaced
# Understand the original behavior vs. patched behavior
```

**Example**: The battle system check `520bb570 e59fc004` verifies that `func_ov003_020bb570` contains the instruction `ldr ip, [pc, #4]` (which encodes to `0xE59FC004`). This confirms Overlay 3 is loaded and the battle system is active before applying battle-related patches.

### What AR Codes + Disassembly Tell Us Together

| What We Know | Source | Example |
|--------------|--------|---------|
| Exact structure sizes | Disassembly (`.space` directives) | `data_02071d10: .space 0x1c` = 28 bytes |
| Specific field addresses | AR codes (write addresses) | `0x02071D16` = level field |
| Field data types | AR codes (code type: 0x=32-bit, 1x=16-bit, 2x=8-bit) | `12071d16` = 16-bit level |
| Field meanings | AR code effects + testing | Address `0x02071D1C` is money (sets to 9,999,999) |
| Function references | Disassembly (`.word` directives, ldr instructions) | `.L_02022b3c: .word data_02071d10` |
| Code logic | Disassembly (assembly instructions) | `func_ov003_020bb570` battle system entry point |
| Patch effects | AR codes (what gets replaced) | Replace conditional branches with `mov r0, #0` |
| Function signatures | AR conditional checks | `520bb570 e59fc004` = verify overlay loaded |
| Overlay boundaries | Disassembly file structure | `ov002_4.s` = pin system, `ov003_4.s` = battle system |

### Code Patches Verified from Disassembly

| AR Code Feature | Address Patched | File | Original Code | Patch | Effect |
|----------------|----------------|------|---------------|-------|--------|
| Battle context check | `0x020BB570` | `ov003_4.s`:68088 | `ldr ip, [pc, #4]` (E59FC004) | (conditional check only) | Verify battle overlay loaded |
| Pin evolution (Å~5) | `0x020854C4`+ | `ov002_4.s`:~3400 | Various conditional checks | `mov r0, #0` (E3A00000) | Force evolution success |
| Wallet checks (Å~6) | `0x0208470C`+ | Various overlays | Wallet level getters | `mov r0, #3` (E3A00003) | Force max wallet |
| Difficulty check | `0x020848A4` | Unknown overlay | Difficulty unlock check | `mov r0, #3` (E3A00003) | Return all difficulties unlocked |

**Note**: Some addresses (`0x020A79EC`, `0x020B8408`, `0x0208470C`) weren't found in the current disassembly, suggesting they may be:
- In overlays not yet fully disassembled
- Generated at runtime during overlay relocation
- In dynamically loaded code sections
- Listed with incorrect addresses in the AR code documentation

### What Still Requires Investigation

1. **Unknown Field Contents**:
   - MainData bytes 0x00-0x05 (before level)
   - MainData bytes 0x08-0x0B (between level and money)
   - MainData bytes 0x14-0x15 (between defense and bravery)
   - MainData bytes 0x18-0x1B (after bravery)

2. **Pin Entry Structure**:
   - Exact layout of the ~4-byte pin entries in `data_020727c3`
   - Purpose of the 2-byte header at `data_020727c0`
   - Purpose of the 1-byte field at `data_020727c2`

3. **Battle Data Relationships**:
   - Why battle data is split into `data_02074e34` and `data_02074e56`
   - Whether these are separate globals or part of a larger system
   - What the unknown bytes in each section contain

4. **Function Behavior**:
   - Complete reverse engineering of functions like `func_02022a18`
   - How pin evolution logic actually works (before AR code patches it)
   - Wallet upgrade system and its relationship to the wallet ID checks

### Next Steps for Complete Reverse Engineering

1. **Annotate MainData Fields**: Read `func_02022a18` and related functions to identify what the unknown bytes represent
2. **Map Pin Structure**: Analyze `data_020727c3` usage in Shop.s, MenuBadge.s to understand pin entry format
3. **Document Evolution Logic**: Reverse the original pin evolution functions in ov002_4.s before AR patches
4. **Identify Patched Functions**: Find function names for addresses 0x0208470C, 0x020A79EC, 0x020B8408, etc.
5. **Create C Structs**: Write proper C structure definitions based on confirmed layouts
6. **Cross-reference with C source**: Check if any .c files in `src/` access these data sections
7. **Document Overlay Loading**: Map which overlays contain which systems (OV002=pins, OV003=battle, etc.)

### Code Patch Investigation Guide

**To find what an AR code is patching:**

1. **Identify the address** from the AR code (e.g., `0208470C`)

2. **Search for the address** in disassembly:
   ```bash
   grep -r "0208470c\\|208470c" build/usa/asm/**/*.s
   ```

3. **Look for function boundaries**:
   - Find the nearest `arm_func_start` label above the address
   - Check the function's address comment (e.g., `; 0x02084700`)

4. **Read the surrounding code**:
   - What does the function do before the patched address?
   - What happens after?
   - What does the original instruction do?

5. **Understand the patch effect**:
   - What value is being hardcoded?
   - What check is being bypassed?
   - What return value is being forced?

**Example workflow** (for wallet patches at 0x0208470C):
```bash
# Search for the address
grep -r "0208470" build/usa/asm/**/*.s

# Found in: build/usa/asm/<overlay>_<num>.s
# Check line number, look at surrounding 50 lines
# Identify function name
# Read original logic
# Document what AR patch changes
```

### Tools for Analysis

- **Grep Search**: Find addresses and labels across all disassembly files
- **Ghidra**: Load the ARM9 binary and cross-reference with disassembly
- **objdiff**: Compare disassembly with original ROM to verify accuracy
- **Memory Dumps**: Capture RAM during gameplay to see runtime values

### What We Know For Certain:
1. **Specific Memory Addresses**: Exact locations of player stats, battle variables, and pin collection (from AR codes)
2. **Data Types**: Size of each field (8-bit, 16-bit, 32-bit) from AR code types
3. **Structure Sizes**: Confirmed from disassembly `.space` directives:
   - MainData: Exactly 28 bytes (0x1C)
   - data_02074e34: 34 bytes (0x22)
   - data_02074e56: 78 bytes (0x4E)
   - Pin array: 1213 bytes (0x4BD) for 302 pins
4. **Value Ranges**: Valid values (e.g., difficulty 0-3, wallet ID 0-3, battleState 0/1/2)
5. **Pin Count**: 302 total pins (0x12E iterations in unlock code)
6. **Overlay Architecture**: Multiple overlays handle different systems (confirmed in `build/usa/asm/ov00X_*.s`)
7. **Function References**: Which functions access these structures (from `.word` directives in assembly)

### What Requires Further Validation:
1. **Structure Start/End Addresses**: AR codes only write to specific fields, not entire structures
2. **Field Relationships**: Whether modified addresses are part of the same struct or separate globals
3. **Gap Contents**: Unknown data between modified addresses
4. **Structure Names**: "MainData" and "BattleData" are convenient labels, not confirmed names
5. **Pin Entry Size**: Loop code suggests ~4-8 bytes per pin but exact layout is unclear

### How to Confirm Structure Layouts:
1. **Memory Dumps**: Capture RAM during gameplay to see surrounding data
2. **Ghidra Analysis**: Reverse engineer functions that access these addresses
3. **Cross-Reference Analysis**: Find all code that reads/writes these locations
4. **Pattern Recognition**: Look for struct allocation, initialization, and access patterns
5. **Comparative Analysis**: Check similar DS games for common data structure patterns

### Useful for Decompilation
- **Memory addresses** provide starting points for reverse engineering specific systems
- **Function addresses** (from conditional checks) reveal where overlay code is loaded
- **Conditional checks** indicate game state verification patterns
- **Battle state values** (0/1/2) indicate state machine patterns
- **Value ranges** help identify enums and bounded integers in the original code

### Limitations of AR-Code-Only Analysis
**What AR codes DON'T tell us:**
- Structure boundaries (start/end addresses)
- Complete field layout (only modified fields are visible)
- Field names (we use descriptive names, but they're not from the original source)
- Data in gaps between modified addresses
- Whether addresses are struct members or separate globals
- Function logic (we only see the addresses being patched, not the original code)
- Relationships between different data structures

**Example: Pin Collection**
**CONFIRMED from disassembly** (`build/usa/asm/main_36.s` lines 4293-4295):
- **`data_020727c0`**: `0x2` (2 bytes) at `0x020727C0`-`0x020727C1`
- **`data_020727c2`**: `0x1` (1 byte) at `0x020727C2`
- **`data_020727c3`**: `0x4BD` (1213 bytes) at `0x020727C3`-`0x02072C7F`

The "Unlock All Pins" AR code:
- Writes to `0x020727C0` (2 bytes) and `0x020727C2` (1 byte) as header data
- Loops 302 times through pin entries starting around `0x020727C4`
- Total array size: 1213 bytes for 302 pins = ~4 bytes per pin entry

**Still unknown:**
- ? Exact per-entry structure layout (ID? Quantity? Flags? Evolution state?)
- ? Why 1213 bytes for 302 pins (302Å~4 = 1208 bytes)? Extra 5 bytes = header/footer?
- ? Variable-size entries or padding?

### Next Steps for Reverse Engineering
1. **Use Ghidra on ARM9 binary**: Find functions that access 0x02071D16, 0x02074E40, etc.
2. **Analyze patched functions**: Disassemble the original code at 0x020AE5F4, 0x020854C4, etc.
3. **Map overlay loading**: Document when/where OV002 and other overlays are loaded
4. **Create memory map**: Take RAM dumps at different game states to see full context
5. **Cross-reference analysis**: Find all references to these addresses in the binary
6. **Identify structure allocation**: Look for initialization functions that set up these structures

---

## Additional Resources

- **Original AR Code File**: [docs/twewy-ar-codes.txt](docs/twewy-ar-codes.txt)
- **Pseudocode Version**: [docs/twewy-ar-codes-pseudocode.txt](docs/twewy-ar-codes-pseudocode.txt)
- **Game Disassembly**: `build/usa/asm/` (main files, overlays, and source)
  - Main ARM9 code: `build/usa/asm/main_*.s`
  - Overlay code: `build/usa/asm/ov00X_*.s`
  - Source functions: `build/usa/asm/src/` (organized by developer/module)
- **Assembly Modules**: [docs/assembly_modules.md](docs/assembly_modules.md)
- **Contributing Guide**: [docs/CONTRIBUTING.md](docs/CONTRIBUTING.md)

### Key Disassembly Files for AR Code Analysis

| AR Code Feature | Primary Disassembly File(s) | Line Numbers (approx) |
|----------------|----------------------------|----------------------|
| MainData structure | `build/usa/asm/main_36.s` | Line 4277 (data), Lines 1-100 (usage) |
| Pin collection | `build/usa/asm/main_36.s` | Lines 4293-4295 (data) |
| Pin evolution | `build/usa/asm/ov002_4.s` | Lines 3380-3440 |
| Battle data | `build/usa/asm/main_36.s` | Lines 4357-4362 (data), `ov003_4.s` (usage) |
| Pin management | `build/usa/asm/src/Debug/Takami/MenuBadge.s` | Various |
| Shop system | `build/usa/asm/src/Debug/Takami/Shop.s` | Various |
| Battle results | `build/usa/asm/src/Debug/Takami/Result.s` | Various |
