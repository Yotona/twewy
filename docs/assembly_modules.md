# Assembly Module Analysis - The World Ends With You

This document provides analysis of what code each assembly module contains based on string analysis, size metrics, and function patterns. Updated: February 10, 2026.

## Table of Contents
- [Overlays (ov000-ov047)](#overlays)
- [Main Segments](#main-segments)
- [Module Categories](#module-categories)
- [Decompilation Status](#decompilation-status)

---

## Overlays

### Core Game Systems (Very Large)

#### ov003 - Main Battle System
**Size:** 412,928 bytes | **Functions:** 1,859 | **Status:** Not started
- **Purpose:** Core battle system manager
- **Key Features:**
  - Main battle loop (`Seq_Battle()`, `BtlMgr.c`)
  - Battle touch input handling (`Task_BtlTch`)
  - Enemy encounter system
  - Battle asset loading (adge.bin, ly02.bin, ly00.bin)
  - Audio integration (sy_ChargeLaser.b, eLaser_P)
  - Task system infrastructure
- **Insights:** This is the heart of the combat system. Contains 482 unique strings referencing battle mechanics, enemy data, and combat UI. Too large to tackle early in decompilation.
- **Dependencies:** Used by all battle-related overlays (ov004-ov024)

#### ov030 - Field/Exploration System  
**Size:** 555,200 bytes | **Functions:** 1,776 | **Status:** Not started
- **Purpose:** Main field/overworld system
- **Key Features:**
  - Field movement and navigation (`Seq_Fld()`, `Seq_FldSel()`)
  - Event management (`Seq_EventSel()`)
  - NPC system (`Tsk_Fld_Player`, field NPCs)
  - Shop data (Apl_Tak/ShopData)
  - Save/load integration
  - Audio events (r_03_mini_lose, r_04_mini_lose)
  - Brand system (top brand:%d)
  - Tutorial triggers (tutrial_shop1, tutrial_shop2)
- **Insights:** Contains 2,462 strings - the most of any module. Handles all non-combat gameplay. Field selection, event system, and likely story progression. Debug strings indicate extensive testing infrastructure.
- **Dependencies:** Core to progression, interacts with ov002 (menus), ov031 (text), and battle overlays

#### ov038 - Event Graphics & NPC Rendering
**Size:** 216,416 bytes | **Functions:** 94 (very large functions!) | **Status:** Not started
- **Purpose:** Character/NPC graphics and battle object rendering
- **Key Features:**
  - NPC animations (GRP_NPC_TEST_M_N, FIELD NPC)
  - Event cutscene graphics (GRP_EVC, EVE_GRA_007)
  - Battle menu sprites (BIN_BTLMENU_EXP, BIN_BTLMENU_AURA)
  - Pin animations (PIN_A, KU_PSY_NEKU, KU_IDLE_NEKU)
  - Battle objects (GRP_BTLOBS12_12 through GRP_BTLOBS19_19)
  - Enemy graphics (GRP_EneB)
  - Asset management (anime:%d, MaxChr:%d, UseChr:%d)
- **Insights:** Average 2,300 bytes per function - these are massive! Handles all non-player character rendering and complex sprite management. 443 battle-related strings suggest deep integration with combat visuals.

---

### Battle Player & Combat Mechanics (Medium)

#### ov002 - Menu System (Otosu/Delete Menu) ? DECOMPILED
**Size:** 69,696 bytes | **Functions:** 332 | **Status:** 72% fuzzy match
- **Purpose:** In-game menu interface
- **Key Features:**
  - Otosu menu system (tin pin related?)
  - Delete menu
  - Menu UI objects (OtosuMenuObj, OtosuMenu_TitleE, OtosuMenu_Icon2)
  - Link level display (OtosuMenuLinklev)
  - Asset paths (Apl_Fuk/Grp_DelM, Apl_Tak/Grp_Menu, Apl_Fuk/Grp_Otos)
- **Insights:** Successfully decompiled with LLM assistance. Provides template for decompiling similar UI overlays.

#### ov004 - Player Pin Type 01
**Size:** 38,272 bytes | **Functions:** 206 | **Status:** Not started
- **Purpose:** Player pin/psych attacks - Type 01
- **Key Features:**
  - Player battle tasks (Tsk_BtlPlayer01)
  - Arm/weapon mechanics (Tsk_BtlArm_Doll)
  - Feature effects (Tsk_Feat00_S, Tsk_Feat00_EffDo)
  - ESP mode (md_Esp)
- **Insights:** First of several player combat overlays. Likely handles specific pin types or attack styles.

#### ov005 - Player Pin Type 02
**Size:** 39,104 bytes | **Functions:** 229 | **Status:** Not started
- **Purpose:** Player pin/psych attacks - Type 02
- **Key Features:**
  - Different player attack set (Tsk_BtlPlayer02)
  - Lock-on targeting (Tsk_BtlArm_LockO)
  - Obstacle interaction (_BtlArm_Obs)
  - Audio preset (prcset_Main)
- **Insights:** Similar structure to ov004 but different attack types. Lock-on mechanism suggests ranged attacks.

#### ov006 - Player Pin Type 03
**Size:** 40,192 bytes | **Functions:** 200 | **Status:** Not started
- **Purpose:** Player pin/psych attacks - Type 03
- **Key Features:**
  - Third player attack set (Tsk_BtlPlayer03)
  - Board effects (Tsk_BtlEff_Board)
  - Suit command (Tsk_BtlCmd_Suit)
  - Cursor mechanics
- **Insights:** "Board" and "Suit" suggest skateboard-based attacks or ability modifiers.

#### ov007 - Player Last Attack
**Size:** 3,200 bytes | **Functions:** 25 | **Status:** Not started
- **Purpose:** Final/ultimate player abilities
- **Key Features:**
  - Player last attack (Tsk_BtlPlayerLas)
  - Aura last effect (Tsk_BtlAuraLast)
- **Insights:** Very small - likely handles finishing moves or fusion attacks.

#### ov008 - Tutorial Battles
**Size:** 1,920 bytes | **Functions:** 14 | **Status:** Not started
- **Purpose:** Tutorial battle system
- **Key Features:**
  - Tutorial tasks (Tsk_BtlPlayerNon, Tsk_BtlTutorial)
  - Tutorial assets (Apl_Fur/Grp_Tuto, rial.bin)
- **Insights:** Smallest battle overlay. Simplified combat for teaching mechanics.

---

### Enemy Types - Regular (Small to Large)

#### ov009 - Multi-Enemy Group (nm021, nm013, etc.)
**Size:** 214,432 bytes | **Functions:** 875 | **Status:** Not started
- **Purpose:** Multiple enemy types and their behaviors
- **Key Features:**
  - Enemy nm021 variants (nm021.bin, nm021a.bin, nm021b.bin)
  - Red Game/Unchained Game variants (Tsk_BtlEnm021_RG, Tsk_BtlEnm021_UG)
  - Enemy nm013 effects
  - Bomb mechanics (fBomb)
  - Multiple enemy groups (Apl_Mot, Apl_Hor, Apl_Mor)
- **Insights:** Large catch-all for multiple enemy types. 191 strings suggest complex behavior sets. Likely standard enemies rather than bosses.

#### ov010 - Enemy nm006
**Size:** 15,680 bytes | **Functions:** 71 | **Status:** Not started
- **Purpose:** Specific enemy type (nm006)
- **Key Features:**
  - Enemy variants (nm006.bin, nm006a.bin, nm006b.b)
  - State handlers (Tsk_BtlEnm006_De, _RG, _Sw)
  - Head effect (adEff)
  - Assets (Apl_Suy/Grp_BtlE)
- **Insights:** Self-contained enemy with delete/red game/switch states.

#### ov011 - Enemy nm010 (Laser Type)
**Size:** 30,176 bytes | **Functions:** 114 | **Status:** Not started
- **Purpose:** Enemy nm010 with laser attacks
- **Key Features:**
  - Complex animation (nm010_MotLaser.b variants for a/b/c)
  - Movement and turning (nm010_MotMove.bi, nm010_MotTurn.bi)
  - Shot effects (nm010_EffShot.bi)
  - Animation manager (BtlEnm010_AnmMgr)
  - Laser tasks (Tsk_BtlEnm010_Ls)
- **Insights:** 101 strings indicate sophisticated enemy with multiple attack phases. Well-documented through asset names.

#### ov012 - Enemy nm014
**Size:** 9,088 bytes | **Functions:** 40 | **Status:** Not started
- **Purpose:** Enemy nm014
- **Key Features:**
  - Enemy assets (nm014.bin, nm014a.bin, nm014b.bin)
  - Effects and variants (_Ef, _RG, _UG)
- **Insights:** Small, straightforward enemy. Good candidate for learning enemy patterns.

#### ov013 - Enemy nm015 (Stamp Type)
**Size:** 8,064 bytes | **Functions:** 38 | **Status:** Not started
- **Purpose:** Enemy nm015 with stamp attacks
- **Key Features:**
  - Enemy assets (nm015.bin, nm015a.bin)
  - Stamp mechanics (fStamp, fStampSub)
  - Effects and variants
- **Insights:** Another small enemy. Stamp attack suggests area-of-effect or marking mechanics.

#### ov014 - Enemy nm023 (Behemoth) ? RECOMMENDED
**Size:** 64,576 bytes | **Functions:** 237 | **Status:** Not started
- **Purpose:** Complex multi-part enemy (Behemoth)
- **Key Features:**
  - Multiple asset variants (nm023.bin, nm023a/b/c.bin)
  - Wing system (tBHMT, nm023_BhmtWing.b, nm023_BhmtNsc_1)
  - Multiple handlers (Tsk_BtlEnm023_Cu, _Dm, _Ef, etc.)
  - 72 total strings indicating complexity
- **Insights:** Perfect size for next decompilation target. Multi-part enemy (body + wings) shows advanced enemy structure. Self-contained with clear boundaries.

#### ov015 - Enemy nm026 (Ice Type)
**Size:** 11,840 bytes | **Functions:** 57 | **Status:** Not started
- **Purpose:** Enemy nm026 with ice element
- **Key Features:**
  - Enemy assets (nm026.bin variants)
  - Ice variant tasks (Tsk_BtlEnm026_Ic)
  - Screen variants (_RG, _UG)
- **Insights:** Small enemy with elemental mechanics. Ice attack patterns different from standard.

---

### Boss Battles (Medium to Large)

#### ov017 - Boss 02 (Sho Minamimoto)
**Size:** 123,584 bytes | **Functions:** 637 | **Status:** Not started
- **Purpose:** Major boss fight - likely Sho Minamimoto
- **Key Features:**
  - Boss assets (oss02.bin, OSS02_EFF.bin, OSS02_Ply.bin)
  - Boss tasks (Tsk_BtlBoss02_00, Tsk_BtlBoss02_01)
  - Field boss variant (Tsk_BtlFieldBoss)
  - Screen variants (02_00_RG, 02_00_UG)
  - Multiple asset graphics (Apl_Mot/Grp_BtlB, GRP_BTLB)
- **Insights:** 152 strings, 637 functions with small average size. Major boss with complex patterns. Field boss integration suggests arena mechanics.

#### ov018 - Field Boss 00 ? RECOMMENDED
**Size:** 58,816 bytes | **Functions:** 187 | **Status:** Not started
- **Purpose:** Field-based boss encounter
- **Key Features:**
  - Boss assets (ldBoss00_RG.bin, ldBoss00_UG.bin)
  - Task variants (Task_BtlFldBoss0, Tsk_BtlFldBoss00, Tsk_BtlObsBoss00)
  - Small/Large/Supply variants (Task_Boss00Sml_R, Tsk_Boss00Lrg, TaskBoss00_Suppl)
  - Effect objects (00s_0.bin, ldBoss00OBJ_UG.b)
- **Insights:** Good size with substantial functions. 13 task strings suggest complex state machine. Field integration makes it interesting technically.

#### ov019 - Boss 04
**Size:** 39,360 bytes | **Functions:** 203 | **Status:** Not started
- **Purpose:** Boss 04 encounter
- **Key Features:**
  - Boss assets (oss04_exp1.bin, oss04OBJ_RG/UG.bin)
  - Handler tasks (Tsk_BtlBoss04_Ef, _Li, _Ra, etc.)
  - Upper/lower objects (04OBJ_UG_Upper.b)
- **Insights:** Medium-sized boss with effect/light/ray handlers. Multi-layer rendering system.

#### ov020 - Boss 05
**Size:** 26,048 bytes | **Functions:** 99 | **Status:** Not started
- **Purpose:** Boss 05 encounter
- **Key Features:**
  - Boss assets (oss05_RG/UG/RBG.bin)
  - Screen variants (Tsk_BtlBoss05_RG, _UG)
  - Field boss integration
  - Effect variants (oss05_UG_eff2.bi)
- **Insights:** Smaller boss, possibly mid-game. Multiple background layers (RBG).

#### ov021 - Boss 06
**Size:** 28,608 bytes | **Functions:** 117 | **Status:** Not started
- **Purpose:** Boss 06 encounter
- **Key Features:**
  - Boss assets (oss06_RBG.bin, 6_RGBG.bin)
  - PC variant (oss06_RG_PC.bin)
  - Attack systems (oss06_RG_ATTACK_)
  - Screen graphics (Grp_BtlBoss06_UG)
  - Palette objects (oss06_UG_OBJ_PAL)
- **Insights:** 65 strings suggest moderate complexity. PC variant interesting - playable character involvement?

#### ov022 - Boss 10
**Size:** 19,104 bytes | **Functions:** 67 | **Status:** Not started
- **Purpose:** Boss 10 encounter
- **Key Features:**
  - Boss asset (oss10.bin)
  - Movement mechanics (fDash, fJump, fBack)
  - Screen variants (Tsk_BtlBoss10_Ef, _RG, _UG)
- **Insights:** Mobile boss with dash/jump mechanics. Smaller = possibly simpler patterns or reuses existing systems.

#### ov023 - Boss 11 (Piranha/Doll)
**Size:** 27,616 bytes | **Functions:** 132 | **Status:** Not started
- **Purpose:** Boss 11 with minions
- **Key Features:**
  - Boss assets (oss11.bin, iranha.bin)
  - Doll system (Tsk_BtlBossDoll_)
  - Player interaction (ossPly01.bin, Tsk_BtlBossPly01)
  - Multiple graphics (Apl_Mor/Grp_BtlB, BtlP, BtlF)
- **Insights:** "Piranha" and "Doll" suggest summoning mechanics or multi-enemy boss.

#### ov024 - Boss 15 & 16
**Size:** 23,552 bytes | **Functions:** 114 | **Status:** Not started
- **Purpose:** Dual boss system or boss 15/16
- **Key Features:**
  - Dual screen variants (Tsk_BtlBoss15_RG, _UG + Tsk_BtlBoss16_Ef, _RG)
  - Recovery mechanic (fRecover)
- **Insights:** Either two bosses share code or one boss with dual form. Recovery suggests healing phase.

---

### Menu & UI Systems (Small)

#### ov025 - Continue Screen
**Size:** 4,096 bytes | **Functions:** 22 | **Status:** Not started
- **Purpose:** Continue/game over screen
- **Key Features:**
  - Continue sequence (Seq_Continue( ))
  - Assets (inue.bin, Apl_Fur/Grp_Cont)
- **Insights:** Tiny overlay for single screen. Quick win for decompilation practice.

#### ov026 - Tutorial Screen
**Size:** 3,392 bytes | **Functions:** 20 | **Status:** Not started
- **Purpose:** Tutorial/help display
- **Key Features:**
  - Tutorial sequence (Seq_Tutorial( ))
  - Assets (rial.bin, Apl_Fur/Grp_Tuto)
- **Insights:** Matches ov025 size. Complementary to ov008 (tutorial battles).

#### ov027 - Battle Selection Menu
**Size:** 12,032 bytes | **Functions:** 40 | **Status:** Not started
- **Purpose:** Chain battle and encounter selection
- **Key Features:**
  - Battle check (Seq_ChkBtl())
  - Chain battle system (CHAIN BATTLE)
  - Enemy party selection (ENEMY PARTY)
  - Difficulty settings (DIFFICULTY)
  - Badge configuration (BADGE SET)
  - Camera/scroll config (RG: CAMERA Y, UG: SCROLL Y)
  - Selector assets (Apl_Fur/Grp_Sele, ctor.bin)
- **Insights:** Small but information-rich. UI for customizing encounters. Good candidate for understanding menu flow.

---

### Special Systems

#### ov016 - Unknown System ? DECOMPILED
**Size:** 15,360 bytes | **Functions:** 100 | **Status:** 85% fuzzy match
- **Purpose:** Unknown (needs verification from decompiled code)
- **Key Features:** Currently under analysis
- **Insights:** Successfully decompiled. Sprite animation configuration with runtime state management.

#### ov031 - Font & Text Rendering ? DECOMPILED  
**Size:** 14,592 bytes | **Functions:** 91 | **Status:** 71% fuzzy match
- **Purpose:** Text rendering and font system
- **Key Features:**
  - System font functions (SysFont_GetUserD, SysFont_GetAlloc, SysFont_GetMsgBu)
  - Message assets (Apl_Fuk/mestxt.b, Apl_Fuk/mestable)
  - Font graphics (Apl_Fuk/Grp_Font)
- **Insights:** Core text system. Successfully decompiled with LLM assistance. Used by all overlays that display text.

#### ov036 - Message Data Tables
**Size:** 1,824 bytes | **Functions:** 4 | **Status:** Not started
- **Purpose:** Message and UI data definitions
- **Key Features:**
  - Message tables (g_messag, e_messag, p_message.xls)
  - Menu data (menu_map.xls, menu_shop.xls, menu_brand.xls)
  - Test messages (TEST, e05_test_mes, for_test_staffro)
- **Insights:** Pure data overlay. 101 strings but only 4 functions - mostly tables. Excel file references suggest development tool output.

#### ov039 - Tin Pin Slammer Mini-game
**Size:** 100,448 bytes | **Functions:** 561 | **Status:** Not started
- **Purpose:** Tin Pin Slammer mini-game
- **Key Features:**
  - Game sequence (Seq_Otosu())
  - Game tasks (Tsk_OtosuGame_ba, Tsk_OtosuGame_sh)
  - Badge mechanics (Apl_Tak/Grp_Badg, BeBadge_Sin)
  - Player types (adow, rker, mmer - Shadow/Marker/Hammer?)
  - Menu variants (SINGLE MENU 1/2/3)
  - Parameters (mnbg.bin, Parm.bin, Data.bin)
- **Insights:** Self-contained mini-game system. Large but isolated from main game. Good target if interested in game-within-game mechanics.

#### ov040 - Wireless Communication
**Size:** 20,128 bytes | **Functions:** 121 | **Status:** Not started
- **Purpose:** DS wireless/multiplayer system
- **Key Features:**
  - System states (WH_SYSSTATE_STOP, _IDLE, _BUSY)
  - Error codes (WM_ERRCODE_SEND_)
  - Data transfer (T_SEND, TASET)
  - 177 strings, mostly system state names
- **Insights:** Nintendo DS wireless API wrapper. 141 "other" strings are system state constants. Likely mostly glue code for Nintendo's wireless library.

---

### Empty or Minimal Overlays

#### ov000, ov001
**Size:** 3,552 / 3,040 bytes | **Functions:** Unknown | **Status:** Not analyzed
- Likely initialization or stub overlays

#### ov032, ov033, ov034, ov035
**Size:** 110KB, 112KB, 70KB, 140KB | **Functions:** 0 reported | **Status:** Not started
- Large but show 0 functions - likely pure data or compressed resources

#### ov037
**Size:** 6,496 bytes | **Functions:** Unknown | **Status:** Not started
- Minimal overlay, purpose unknown

#### ov041, ov042, ov043, ov044, ov045, ov046
**Size:** Various (3KB-97KB) | **Functions:** Various | **Status:** Not analyzed in string search
- Not yet analyzed, purposes unknown

#### ov047
**Size:** 32 bytes | **Functions:** 0 | **Status:** Empty stub
- Essentially empty

---

## Main Segments

The `main_*` segments contain the core ARM9 code split into sections. Based on function counts and string analysis:

### Analyzed Main Segments

#### main_9 (0%, 7 functions) - String Utilities
- **Purpose:** String formatting functions
- **Key Features:**
  - String printf operations (`Str_SPrintf()`)
- **Insights:** Core string manipulation utilities

#### main_18 (0%, 6 functions) - Debug Logging & Font
- **Purpose:** Debug logging and font data
- **Key Features:**
  - Debug log buffer (`DbgLog buf full`)
  - Font data path (`UsrLib/FontData`)
- **Insights:** Combines logging infrastructure with font resource loading

#### main_22 (0%, 14 functions) - ADX Codec
- **Purpose:** CRI ADX audio codec functions
- **Key Features:**
  - ADX trap entry (`adxt_trap_entry`)
  - ADX statistics (`adxt_stat_decinfo`)
  - Error handling
- **Insights:** Part of CRI Middleware audio decoding system

#### main_28 (0 functions) - ? ACSSND Audio System
- **Purpose:** CRI ACS Sound system
- **Key Features:**
  - 66 strings, heavily documented
  - ACSSND API (`ACSSND_Init`, `ACSSND_Pause`, `ACSSND_SetOutVol`, `ACSSND_SetLpFlg`)
  - Initialization checks
  - Work memory management
- **Insights:** Main audio system initialization and control. Complete audio middleware wrapper.

#### main_31 (0%, 5 functions) - File Headers
- **Purpose:** File format identification
- **Key Features:**
  - RIFF format detection
  - ROFS format (Read-Only File System?)
- **Insights:** File format parsing utilities

#### main_32 (0%, 186 functions) - ? CRI Middleware Core
- **Purpose:** CRI Middleware integration layer
- **Key Features:**
  - 94 strings with version info
  - ADXM threading (`ADXM_SetupThrd`)
  - File system integration (NitroCI, NitroRNA, MFCI)
  - ADXCS/NITRO Ver.0.98 Build:Sep 28 2007
  - Build timestamps and MW4020 compiler references
- **Insights:** **Large and important** - Core CRI audio/file system. Build date: Sep 28 2007. Uses Metrowerks compiler.

#### main_34 (0 functions) - CRI CRW System  
- **Purpose:** CRI CRW subsystem
- **Key Features:**
  - CRI CRW:STD/NITRO Ver.0.82
  - Build: Sep 28 2007
  - Audio-related utilities
- **Insights:** Another CRI component, same build date as main_32

#### main_36 (0%, 97 functions) - ? Game Data Management
- **Purpose:** Core game data and save system
- **Key Features:**
  - 12 strings
  - Badge data (`badge_data`, `../Data/BadgeData`)
  - Food data (`Apl_Tak/FoodData`)
  - Item data (`Apl_Tak/ItemData`)
  - Treasure data (`Apl_Tak/Treasure`)
  - Save data (`BackUpData_Tmp`, `GlobalFriendData`)
  - Font assets (`Static/Font_Funakoshi.bin`)
  - Cell text (`Tsk_CellText`)
- **Insights:** **Important game data segment** - Manages all item/badge/food data and save system

#### main_40 (0%, 11 functions) - Sprite Resources
- **Purpose:** Sprite resource management
- **Key Features:**
  - NNS sprite textures (`SprRscTexForNNS`)
- **Insights:** Nintendo NitroSystem sprite integration

#### main_44 (0%, 378 functions) - ? LARGEST Core System
- **Purpose:** Unknown - **LARGEST** segment
- **Key Features:**
  - Only 1 garbled string found
  - 378 functions
- **Insights:** Minimal string data suggests low-level engine code or heavily optimized system. **High priority for analysis**

#### main_54 (0 functions) - Character/Lookup Tables
- **Purpose:** Character encoding or lookup tables
- **Key Features:**
  - ASCII character sequences
  - Alphanumeric progressions
- **Insights:** Pure data segment - character maps or encoding tables

#### main_58 (0%, 249 functions) - ? Hash/Crypto Functions
- **Purpose:** Hashing or cryptographic operations
- **Key Features:**
  - Character sequences (`abcdbcdecdefdefg` pattern)
  - Repeating numeric patterns
- **Insights:** **Large segment** - SHA/MD5 style patterns suggest hashing or save data verification

#### main_62 (0 functions) - Numeric Constants
- **Purpose:** Mathematical constants or lookup tables
- **Key Features:**
  - Large numeric strings (precision values)
- **Insights:** Likely floating-point lookup tables or mathematical constants

#### main_68 (0 functions) - C++ Runtime Type Info
- **Purpose:** C++ RTTI (Run-Time Type Information)
- **Key Features:**
  - `St9type_info`
  - `__cxxabiv117__class_type_info`
  - `__cxxabiv120__si_class_type_info`
- **Insights:** Standard C++ runtime tables. Part of C++ exception handling infrastructure.

#### main_70 (0%, 28 functions) - SDK/Backup System
- **Purpose:** Nintendo SDK backup/save integration
- **Key Features:**
  - SDK marker (`![SDK+NINTENDO:BACKUP]`)
  - Encoded/compressed data
- **Insights:** Nintendo DS save system integration

### Unanalyzed Main Segments (No Significant Strings)

- **main_4** (0%, 57 functions) - No readable strings
- **main_7** (0%, 11 functions) - No readable strings  
- **main_15** (0%, 11 functions) - No readable strings
- **main_16** (0%, 4 functions) - No readable strings
- **main_17** (0%, 72 functions) - No readable strings
- **main_19** (0%, 2 functions) - No readable strings
- **main_24** (0%, 9 functions) - No readable strings
- **main_30** (0%, 11 functions) - No readable strings
- **main_46** (0%, 19 functions) - No readable strings
- **main_47** (0%, 97 functions) - No readable strings
- **main_48** (0%, 9 functions) - No readable strings
- **main_50** (0%, 31 functions) - No readable strings
- **main_51** (0%, 124 functions) - No readable strings
- **main_60** (0%, 76 functions) - No readable strings
- **main_67** (0%, 6 functions) - No readable strings

**Note:** Segments without strings likely contain low-level engine code, math libraries, or heavily optimized routines.

### Completed/Empty Main Segments

- **main_10, main_21** - 100% complete (0 functions)
- **main_52, main_53, main_57** - 100% complete (0 functions)
- **main_63, main_65, main_69, main_71** - 100% complete (0 functions)

### Main Segment Categories

#### Core Engine (No strings, likely low-level)
- main_4, main_7, main_15, main_16, main_17, main_19, main_24, main_30
- main_46, main_47, main_48, main_50, main_51, main_60, main_67

#### CRI Middleware (Audio/File Systems)  
- **main_22** - ADX codec
- **main_28** - ACSSND audio (66 strings)
- **main_31** - File formats
- **main_32** - CRI core (186 funcs, 94 strings) ?
- **main_34** - CRI CRW

#### Game Data & Resources
- **main_36** - Item/badge/save data (97 funcs) ?
- **main_40** - Sprite resources

#### System Utilities
- **main_9** - String utilities
- **main_18** - Debug logging & fonts
- **main_44** - LARGEST core system (378 funcs) ?
- **main_58** - Hash/crypto (249 funcs) ?
- **main_70** - Save system

#### Runtime/Data Tables
- **main_54** - Character tables
- **main_62** - Numeric constants  
- **main_68** - C++ RTTI

### Priority for Decompilation

**High Priority:**
1. **main_36** (97 funcs) - Game data - Badge/item/food/treasure data
2. **main_32** (186 funcs) - CRI middleware core
3. **main_44** (378 funcs) - Largest segment, unknown critical system
4. **main_58** (249 funcs) - Hashing/crypto system

**Medium Priority:**
- main_28 (audio system - likely well-documented library)
- main_70 (save system)
- main_47, main_51 (large, no strings)

**Low Priority:**
- Smaller segments without strings (likely simple utilities)
- Completed segments (already done)

---

## Module Categories

### By Size (Code Size)
1. **Massive (>200KB):** ov030 (555KB), ov003 (413KB), ov038 (216KB), ov009 (214KB)
2. **Large (100-200KB):** ov035 (140KB), ov017 (124KB), ov032/ov033 (110KB/112KB)
3. **Medium (50-100KB):** ov002 (70KB), ov034 (70KB), ov014 (65KB), ov018 (59KB)
4. **Small-Medium (20-50KB):** ov004-ov006 (38-40KB), ov005/ov019/ov021/ov023/ov024 (27-39KB)
5. **Small (<20KB):** Most enemy overlays, menu overlays, ov027, ov040

### By Purpose
- **Core Systems (3):** ov003 (battle), ov030 (field), ov038 (graphics)
- **Player Combat (5):** ov004, ov005, ov006, ov007, ov008
- **Enemy Types (8):** ov009, ov010, ov011, ov012, ov013, ov014, ov015, and several not in string analysis
- **Boss Battles (8):** ov017, ov018, ov019, ov020, ov021, ov022, ov023, ov024
- **Menus/UI (4):** ov002, ov025, ov026, ov027
- **Special Systems (4):** ov016(?), ov031(text), ov036(data), ov039(mini-game), ov040(wireless)

### By Complexity (Functions)
1. **Very Complex (>500 funcs):** ov003 (1859), ov030 (1776), ov009 (875), ov017 (637), ov039 (561)
2. **Complex (200-500 funcs):** ov002 (332), ov014 (237), ov005 (229), ov004 (206), ov019 (203)
3. **Moderate (100-200 funcs):** ov006 (200), ov018 (187), ov023 (132), ov040 (121), ov021 (117), ov011 (114), ov024 (114), ov016 (100)
4. **Simple (<100 funcs):** Most small enemy and menu overlays

---

## Decompilation Status

### ? Completed (3)
- **ov002** - Menu system (72% fuzzy)
- **ov016** - Unknown system (85% fuzzy)
- **ov031** - Font/text (71% fuzzy)

### ? High Priority Targets (Based on Analysis)
1. **ov014** - Behemoth enemy (65KB, 237 funcs) - Perfect size, self-contained, reveals enemy patterns
2. **ov018** - Field boss (59KB, 187 funcs) - Good size, interesting boss mechanics
3. **ov027** - Battle selection (12KB, 40 funcs) - Small, UI-focused, quick win
4. **ov011** - Laser enemy (30KB, 114 funcs) - Medium enemy with clear mechanics
5. **ov012/ov013/ov015** - Small enemies (8-12KB, 38-57 funcs) - Learning progression

### ?? Medium Priority
- **ov039** - Tin Pin (100KB, 561 funcs) - Self-contained but complex
- **ov004/ov005/ov006** - Player combat (38-40KB, 200-229 funcs) - Important but interdependent
- **ov017** - Major boss (124KB, 637 funcs) - Large but many small functions

### ? Low Priority (Too Large or Dependencies Unknown)
- **ov003** - Core battle (413KB, 1859 funcs) - Save for when more patterns known
- **ov030** - Field system (555KB, 1776 funcs) - Save for when more patterns known
- **ov038** - Graphics (216KB, 94 funcs) - Very large functions, complex rendering
- **ov009** - Multi-enemy (214KB, 875 funcs) - Too large

---

## Insights for Decompilation

### Common Patterns Observed
1. **Dual Screen Architecture:** Most overlays have _RG (top screen) and _UG (bottom screen) variants
2. **Task System:** Frequent `Tsk_` prefixed functions suggest task-based architecture
3. **Sequence Functions:** `Seq_` functions appear to be high-level state machines
4. **Graphics System:** `Grp_` prefixes indicate graphics asset groups (not generic "groups")
5. **Binary Asset Loading:** .bin files are primary asset format
6. **Application Naming:** Different applications/modules have prefixes (Apl_Fur, Apl_Tak, Apl_Mot, etc.)
   - `Apl_` likely means "Application" - different subsystems or developer modules

### Asset Path Patterns
- `Apl_<Name>/Grp_<Type>` - Standard asset path format
  - `Apl_` = Application module (different subsystems/teams)
  - `Grp_` = Graphics group (not generic "group")
- Application prefixes: Fur, Tak, Mot, Hor, Mor, Suy, Abe, Bul, Kit, Fukuda, etc.
  - Likely developer names or team designations
- Graphics types: BtlE (battle enemy), BtlB (battle boss), BtlF (battle field), Menu, Font, etc.

### Function Naming Conventions
- `func_ov###_########` - Standard overlay function
- `Tsk_` - Task functions (async operations)
- `Seq_` - Sequence/state machine functions
- `BtlEnm###_` - Battle enemy functions
- `BtlBoss##_` - Battle boss functions

### Size vs Complexity Trade-offs
- **Sweet Spot:** 10-70KB with 50-250 functions (manageable, good contribution)
- **Avoid Early:** >150KB or >500 functions (too complex without established patterns)
- **Quick Wins:** <10KB with <50 functions (minimal contribution but builds confidence)

---

## Recommended Decompilation Order

### Phase 1: Foundation (Current)
1. ~~ov002~~ ? Menu system
2. ~~ov016~~ ? Unknown system  
3. ~~ov031~~ ? Font system

### Phase 2: Enemy Patterns (Next Steps - Choose One Path)

**Path A - Single Medium Enemy:**
1. **ov014** (Behemoth) - Best single target

**Path B - Small Enemy Sequence:**
1. **ov012** (Enemy nm014)
2. **ov013** (Enemy nm015)
3. **ov015** (Enemy nm026)

### Phase 3: Expand Enemy Knowledge
1. **ov011** (Laser enemy) - More complex mechanics
2. **ov010** (Enemy nm006) - Different patterns
3. **ov018** (Field boss) - Boss mechanics

### Phase 4: Battle Framework
1. **ov004** (Player type 01)
2. **ov005** (Player type 02)  
3. **ov006** (Player type 03)

### Phase 5: UI & Menus
1. **ov027** (Battle selection)
2. **ov025** (Continue)
3. **ov026** (Tutorial)

### Phase 6: Major Systems (When Ready)
1. **ov017** (Major boss) - Large but approachable
2. **ov039** (Tin Pin) - Self-contained system
3. **ov009** (Multi-enemy) - Large enemy group

### Phase 7: Core Systems (Final)
1. **ov003** (Battle core) - After most battle code understood
2. **ov030** (Field core) - After field-related overlays done
3. **ov038** (Graphics) - Complex rendering system

---

## Data Sources

This analysis is based on:
- Assembly file sizes and function counts from `check_progress.py`
- String extraction from overlay assembly files via `analyze_overlay_strings.py`
- Overlay metadata from `extract/usa/arm9_overlays/overlays.yaml`
- File structure analysis from build system
- Pattern recognition from successfully decompiled overlays

Last updated: February 10, 2026
