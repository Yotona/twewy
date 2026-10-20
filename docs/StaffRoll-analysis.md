# StaffRoll (Overlay 42) ? Analysis

> Source: `src/Debug/Suyama/StaffRoll.c`  
> Header: `include/Debug/Suyama/StaffRoll.h`

The StaffRoll overlay implements the **end-credits / staff-roll sequence** for TWEWY.
It is loaded as a debug-menu overlay (`Suyama` sub-directory), activated after the main game ends.
It drives two NDS display engines simultaneously, scrolling styled name text over blended background graphics, all choreographed by a binary command-list script (`Apl_Suy/staff_cmdlist.bin`).

---

## High-Level Flow

```
StaffRoll_Init
  „¤„Ÿ allocate StaffRollState
  „¤„Ÿ load Overlay 31 (text renderer)
  „¤„Ÿ StaffRoll_InitHardware()   © full hardware init (both engines, VRAM, OAM, palettes, VBlank IRQ)
  „¤„Ÿ create TaskPool
  „¤„Ÿ push 3 callbacks onto DebugOvlDisp stack:
       [bottom] func_ov042_02082f94   © teardown / music stop
       [mid]    func_ov042_02082ed8   © script processor (runs every frame)
       [top]    func_ov042_02082e9c   © one-shot setup (loads command list, starts music, pops self)

StaffRoll_Update (every frame)
  „¤„Ÿ update OAM / display
  „¤„Ÿ DebugOvlDisp_Run()       © runs the script processor callback
  „¤„Ÿ EasyTask_ProcessPendingTasks + EasyTask_UpdateActiveTasks
  „¤„Ÿ if DebugOvlDisp stack is empty ¨ return to Overlay 30

StaffRoll_Destroy
  „¤„Ÿ destroy TaskPool
  „¤„Ÿ unload text renderer (Ov31) resources
  „¤„Ÿ release all BG/screen/pal slots
  „¤„Ÿ unload Overlay 31
  „¤„Ÿ free StaffRollState
```

---

## Data Structures

### `UnkOv31Struct` ¨ **`TextRendererCtx`**

Defined in `include/common_data.h` (size `0x7C`). This is the text-rendering context from Overlay 31.  
Each instance represents one **credit entry** being rendered to a background tilemap.

| Field | Suggested name | Notes |
|---|---|---|
| `unk4` | `fontInfo` | Pointer to `Ov031FontInfo` (char widths, glyph metrics) |
| `unk8` / `unkC` / `unk10` | `charData` / `paletteData` / `stringData` | Loaded `Data*` blobs |
| `unk_14` | `textBuffer` | `u16*` into the current 16-bit string to render; `NULL` = inactive |
| `unk1C` | `isDirty` | Marks the context needs re-render |
| `unk48` / `unk4C` | `alignment` / `canvasWidth` | Text alignment mode and canvas width |
| `unk5A` | `paletteIndex` | Which 16-color palette slot to use |
| `unk_5C` | `charWidth` | Width of one character cell |
| `unk56` / `unk58` | `lineSpacingExtra` / `lineSpacingBase` | Spacing between wrapped lines |
| `unk68` / `unk6C` / `unk_70` | `enableSpacing` / `posX` / `posY` | Enable extra spacing, draw position |

---

### `StaffRollUnkA` ¨ **`CharSlot`**

A single allocated **character (tile) slot** on the BG charmap.

| Field | Suggested name | Notes |
|---|---|---|
| `data` | `data` | `Data*` for the loaded pack entry |
| `unk_04` | `charHandle` | `void*` handle returned by `BgResMgr_AllocChar32` |
| `unk_08` | `packIndex` | Which of the 4 `Apl_Suy/Grp_Staff_Part*_Pack.bin` files |
| `unk_0A` | `entryIndex` | Entry within that pack |
| `unk_0C` | `numTiles` | Number of tiles allocated |

---

### `StaffRollUnkB` ¨ **`ScreenSlot`** (also used for palette/OBJ)

A single allocated **screen (tilemap) or palette** slot.

| Field | Suggested name | Notes |
|---|---|---|
| `data` | `data` | `Data*` for the loaded pack entry |
| `unk_04` | `handle` | `void*` returned by `BgResMgr_AllocScreen` / `func_0200adf8` |

---

### `StaffRollUnkC` ¨ **`EngineResourceBundle`**

Per-display-engine collection of all allocated graphic resources.

| Field | Suggested name | Notes |
|---|---|---|
| `group1[4]` | `charSlots[4]` | Char tile slots for BG layers 0?3 |
| `group2[4]` | `screenSlots[4]` | Tilemap/screen slots for BG layers 0?3 |
| `group3[3]` | `paletteSlots[3]` | Palette / extended BG resource slots |

---

### `StaffRoll_CallbackStruct` ¨ **`ScriptContext`**

The main per-frame script execution context, shared by all three overlay-dispatcher callbacks.

| Field | Suggested name | Notes |
|---|---|---|
| `pool` | `taskPool` | Pointer to the shared `TaskPool` |
| `unk_04` | `tick` | Monotonically increasing frame counter (unless paused) |
| `commandIndex` | `commandIndex` | Index of the next `Command` to evaluate |
| `unk_10[2]` | `resources[2]` | Resource bundles for engine 0 (main) and engine 1 (sub) |
| `unk_100_1` (bit 0) | `paused` | When `1`, the tick counter is not advanced |
| `unk_100_2` (bit 1) | `didWorkThisFrame` | Set when a task was spawned; prevents double-work inside one frame |
| `commandlist` | `commandList` | `Data*` for the loaded `staff_cmdlist.bin` |

---

### `Command` ¨ **`ScriptCommand`**

One entry in `staff_cmdlist.bin`.

| Field | Suggested name | Notes |
|---|---|---|
| `StaffRollUnkCIndex` | `triggerTick` | The `tick` value at which this command fires; `0` = end-of-list sentinel |
| `group1Index` | `commandType` | Dispatch key (0?13); `0` also serves as end-of-list |
| `unk_04` | `args` | `CommandUnk04` payload, interpreted per `commandType` |

---

### `CommandUnk04` ¨ **`CommandArgs`**

Packed argument payload used by all command types (fields re-used per command).

---

### `FontRoll` / `FontRollArgs` ¨ **`FontRollCtx`** / **`FontRollInitArgs`**

State for the scrolling credits text task. The `FontRoll` context rolls through a list of credit names, rendering them to a BG tilemap as a vertical scroll.

| Field | Suggested name | Notes |
|---|---|---|
| `unk_000` | `scriptCtx` | Back-pointer to the `ScriptContext` |
| `engine` | `engine` | Display engine index |
| `bgLayer` | `bgLayer` | BG layer index to scroll on |
| `unk_00C` | `fontSet` | 0 or 1; indexes into `data_ov042_02084924` for total/per-entry counts |
| `unk_00E` | `totalDuration` | Total scroll duration in frames |
| `unk_010[30]` | `textCtxPool[30]` | Ring buffer of 30 `TextRendererCtx` instances |
| `unk_E98` | `dequeueIdx` | Ring-buffer read index (oldest active entry) |
| `unk_E9C` | `enqueueIdx` | Ring-buffer write index (newest active entry) |
| `unk_EA0` | `scrollPos` | Current vertical scroll position (16.12 fixed-point) |
| `unk_EA4` | `scrollSpeed` | Per-frame scroll increment (fixed-point) |
| `fontData` | `fontData` | Loaded `staff_font.bin` |
| `unk_EAC` | `entryIndex` | Index of the next credit entry to enqueue |
| `unk_EAE` | `elapsedFrames` | Frames elapsed |
| `unk_EB0` | `maxFrames` | Copy of `totalDuration` |

---

### `StaffBg` / `StaffBgArgs` ¨ **`BlendAnimCtx`** / **`BlendAnimArgs`**

Animates the `BLDALPHA` blend coefficients (coeff0/coeff1) over time using fixed-point lerp.

| Field | Suggested name | Notes |
|---|---|---|
| `unk_00` | `engine` | Display engine |
| `unk_04` | `blendLayer0` | Source layer for BLDALPHA |
| `unk_08` | `blendLayer1` | Destination layer; 0 = use backplane (32) |
| `unk_0C` | `startCoeff` | Starting blend coefficient 0 |
| `unk_0E` | `endCoeff` | Ending blend coefficient 0 |
| `unk_10` | `duration` | Duration in frames |
| `unk_12` | `coeff1Mode` | 32 = compute as (31 ? coeff0) fade-out; 33 = fade-in; else constant |
| `unk_14` | `currentCoeff` | Current interpolated coeff0 (fixed-point) |
| `unk_18` | `step` | Per-frame change in coeff0 (fixed-point) |
| `unk_1C` | `elapsedFrames` | Frames elapsed |
| `unk_1E` | `maxFrames` | Copy of `duration` |

---

### `BgScroll` / `BgScrollArgs` ¨ **`BgScrollCtx`** / **`BgScrollArgs`**

Animates BG H/V scroll offsets over time.

| Field | Suggested name | Notes |
|---|---|---|
| `unk_00` | `engine` | Display engine |
| `unk_04` | `bgLayer` | BG layer to scroll |
| `unk_08` / `unk_0A` | `startH` / `startV` | Starting scroll position |
| `unk_0C` / `unk_0E` | `endH` / `endV` | Ending scroll position |
| `unk_10` | `duration` | Duration in frames |
| `unk_14` / `unk_18` | `currentH` / `currentV` | Current interpolated position (fixed-point) |
| `unk_1C` / `unk_20` | `stepH` / `stepV` | Per-frame step (fixed-point) |
| `unk_24` | `elapsedFrames` | Frames elapsed |
| `unk_26` | `maxFrames` | Copy of `duration` |

Note: `stepH`/`stepV` are computed lazily on the first `Update` call (when `elapsedFrames == 0`).

---

### `BgBright` / `BgBrightArgs` ¨ keep names (already documented)

Animates screen brightness (`BLDY` / `BLDCNT`) over time.  
Brightness is clamped to `[-16, 16]` per NDS hardware limits.

---

### `data_ov042_02084924` (type `unk2084924[2]`) ¨ **`FontSetMetadata[2]`**

Two entries (one per `fontSet` 0 and 1).

| Field | Suggested name | Notes |
|---|---|---|
| `unk_00` | `totalEntries` | Total number of credit name entries in this set |
| `unk_02` | `entryLength` | Number of font-data bytes per entry (name character count) |

---

## Global Data

| Symbol | Suggested name | Notes |
|---|---|---|
| `data_ov042_020847c0[4]` | `gfxPackIds[4]` | `BinIdentifier` array for the four `Grp_Staff_Part*_Pack.bin` asset packs |
| `data_ov042_020847e0` | `cmdListBinId` | `BinIdentifier` for `staff_cmdlist.bin` |
| `data_ov042_020847e8` | `kBgBrightTask` | `TaskHandle` for `BgBright` task type |
| `data_ov042_020847f4` | `kStaffBgTask` | `TaskHandle` for `StaffBg` (blend) task type |
| `data_ov042_02084800` | `kBgScrollTask` | `TaskHandle` for `BgScroll` task type |
| `data_ov042_0208481c` | `kFontRollTask` | `TaskHandle` for `FontRoll` task type |
| `data_ov042_02084814` | `fontBinId` | `BinIdentifier` for `staff_font.bin` |
| `data_ov042_02084924[2]` | `gFontSetMeta[2]` | Per-font-set metadata (entry count, entry length) |

---

## Functions

### Overlay Lifecycle

| Function | Suggested name | Description |
|---|---|---|
| `ProcessOverlay_StaffRoll` | *(keep)* | Top-level overlay process dispatcher; routes to Init/Update/Destroy based on stage |
| `StaffRoll_Init` | *(keep)* | Allocates `StaffRollState` on `gDebugHeap`, loads OV31, inits hardware, pushes 3 callbacks |
| `StaffRoll_Update` | *(keep)* | Per-frame update: OAM/display sync, run script callbacks, process tasks. Returns to OV30 on completion |
| `StaffRoll_Destroy` | *(keep)* | Releases all tasks, resources, overlays, memory |

---

### Script Command Dispatcher

| Function | Suggested name | Description |
|---|---|---|
| `func_ov042_020827ec` | `ScriptCmd_Dispatch` | Routes a `Command` to the appropriate handler by `commandType` (0?13) |
| `func_ov042_020828c4` | `ScriptCmd_SetLayerVisible` | Type 2: show or hide a BG layer by toggling its bit in `controls[engine].layers` |
| `func_ov042_0208293c` | `ScriptCmd_LoadCharResource` | Type 3: load a tile-gfx pack entry into a `CharSlot`; allocates char memory via `BgResMgr` |
| `func_ov042_02082a30` | `ScriptCmd_LoadScreenResource` | Type 4: load a tilemap pack entry into a `ScreenSlot`; allocates screen via `BgResMgr` |
| `func_ov042_02082b20` | `ScriptCmd_LoadPaletteResource` | Type 5: load a palette/OBJ resource into a `ScreenSlot` (group3) |
| `func_ov042_02082bf4` | `ScriptCmd_ReloadCharTiles` | Type 6: re-uploads tile gfx from an already-loaded `CharSlot` (used after text overwrites tiles) |
| `func_ov042_02082c98` | `ScriptCmd_ReleaseCharSlot` | Type 7: release a `CharSlot` and its `BgResMgr` allocation |
| `func_ov042_02082cf4` | `ScriptCmd_ReleaseScreenSlot` | Type 8: release a `ScreenSlot` and its `BgResMgr` allocation |
| `func_ov042_02082d50` | `ScriptCmd_ReleasePaletteSlot` | Type 9: release a group3 (`ScreenSlot`) resource |
| `StaffRoll_CreateStaffBgTask` | *(keep)* | Type 10: spawn a `BlendAnim` task |
| `StaffRoll_CreateBgScrollTask` | *(keep)* | Type 11: spawn a `BgScroll` task |
| `StaffRoll_CreateFontRollTask` | *(keep)* | Type 12: spawn a `FontRoll` task |
| `StaffRoll_CreateBgBrightTask` | *(keep)* | Type 13: spawn a `BgBright` task |

---

### DebugOvlDisp Callbacks

| Function | Suggested name | Description |
|---|---|---|
| `func_ov042_02082e9c` | `StaffRoll_SetupCb` | One-shot: disables CRI sound loop, starts BGM track 0x22, loads `staff_cmdlist.bin`, pops self off stack |
| `func_ov042_02082ed8` | `StaffRoll_ScriptCb` | Per-frame: walks command list, fires all commands whose `triggerTick ? tick`, then increments tick. Pops self when end-of-list sentinel is reached |
| `func_ov042_02082f94` | `StaffRoll_TeardownCb` | Final: releases command list data, stops BGM, re-enables CRI sound loop, pops remaining stack entries |

---

### Text Rendering Helpers

| Function | Suggested name | Description |
|---|---|---|
| `func_ov042_02082fc4` | `TextCtx_IsActive` | Returns `TRUE` if `textBuffer != NULL` (context has content to display) |
| `func_ov042_02082fd8` | `TextCtx_PatchPaletteIndex` | Iterates through a text context's tile table and replaces the palette nibble in `0xFFBx` tile entries with `r1 & 0xF` |
| `func_ov042_02083b48` | `FontSet_GetEntryPtr` | Returns a pointer into `fontData.buffer` for a specific entry; set 1 is offset by `+0x6A * 2` |
| `func_ov042_02083b64` | `FontSet_CalcTotalHeight` | Sums up pixel heights of all entries in a font set, plus 10px spacing per entry, starting from 0xC0 |
| `func_ov042_02083bb8` | `FontRoll_SendReloadCharCmd` | Dispatches a type-6 (`ReloadCharTiles`) command targeting the FontRoll's own BG layer |

---

### FontRoll Task Logic

| Function | Suggested name | Description |
|---|---|---|
| `func_ov042_02083be8` | `FontRoll_UpdateScroll` | Advances `scrollPos` by `scrollSpeed`, wraps at `0x100000`, sets BG V offset |
| `func_ov042_02083c78` | `FontRoll_ShouldDequeue` | Returns 1 if the oldest queued text entry has fully scrolled past the screen top |
| `func_ov042_02083cf0` | `FontRoll_DequeueEntry` | Finalises the oldest entry: re-renders it to BG (clearing/restoring tiles), destroys its text context, advances dequeue index |
| `func_ov042_02083df8` | `FontRoll_ShouldEnqueue` | Returns 1 if there is space for the next entry and not all entries have been shown |
| `func_ov042_02083e8c` | `FontRoll_EnqueueEntry` | Initialises a new text context for the next credit entry, renders it to BG tiles, advances enqueue index |
| `FontRoll_020840ac` | `FontRoll_Init` | Zeroes `FontRoll`, loads `staff_font.bin`, computes scroll speed as `totalHeight / duration`, primes ring buffer |
| `FontRoll_020841cc` | `FontRoll_Update` | Per-frame: conditionally enqueue/dequeue entries, call scroll update, return 0 when duration expires |
| `FontRoll_0208429c` | `FontRoll_Destroy` | Releases `fontData` and destroys all 30 active text contexts |
| `FontRoll_RunTask` | *(keep)* | EasyTask runner: dispatches to Init (0), Update (1), Destroy (3) |
| `FontRoll_CreateTask` | *(keep)* | Packages `FontRollArgs` and calls `EasyTask_CreateTask` |

---

### BgBright Task Logic

| Function | Suggested name | Description |
|---|---|---|
| `BgBright_CreateTask` | *(keep)* | Packages `BgBrightArgs` and spawns the task |
| `BgBright_RunTask` | *(keep)* | Dispatches Init (0) / Update (1) |
| `BgBright_Init` | *(keep)* | Stores args into `BgBright`, applies `startBrightness` immediately, computes per-frame `step` using `FX_Divide` |
| `BgBright_Update` | *(keep)* | Lerps brightness each frame; clamps to `[-16, 16]`; returns `FALSE` when complete |

---

### StaffBg (Blend) Task Logic

| Function | Suggested name | Description |
|---|---|---|
| `StaffBg_CreateTask` | *(keep)* | Packages `StaffBgArgs` and spawns the task |
| `StaffBg_RunTask` | *(keep)* | Dispatches Init (0) / Update (1) |
| `func_ov042_02083344` | `StaffBg_GetCoeff1` | Computes `blendCoeff1`: mode 32 = `(0x1F000 ? currentCoeff) >> 12` (inverse); mode 33 = `currentCoeff >> 12`; otherwise constant |
| `StaffBg_Init` | *(keep)* | Sets blend mode to alpha-blend (1), configures layers, applies initial coefficients |
| `StaffBg_Update` | *(keep)* | Lerps coeff0 each frame; returns 0 when complete |

---

### BgScroll Task Logic

| Function | Suggested name | Description |
|---|---|---|
| `BgScroll_CreateTask` | *(keep)* | Packages `BgScrollArgs` and spawns the task |
| `BgScroll_RunTask` | `BgScroll_RunTask` | Dispatches Init / Update |
| `func_ov042_02083708` | `BgScroll_Init` | Zeroes state, sets initial H/V offsets; also enables affine dirty flag for rotoscale BG modes |
| `func_ov042_02083838` | `BgScroll_Update` | Lazily computes `stepH`/`stepV` on frame 0, then lerps H/V scroll each frame |

---

### Hardware Initialisation

| Function | Suggested name | Description |
|---|---|---|
| `StaffRoll_InitHardware` | `StaffRoll_HwInit` | Full display initialisation: both engines in BG mode 0, VRAM banks A (main BG) + C (sub BG), 4 BG layers each engine with specific screen/char bases, OAM clear, palette upload, VBlank callback registration |
| `StaffRoll_ReleaseVBlank` | `StaffRoll_HwShutdown` | Deregisters the VBlank callback |
| `StaffRoll_VBlankHandler` | `StaffRoll_VBlankIsr` | VBlank interrupt: commits display registers, flushes DMA, uploads OAM + BG/OBJ palettes for both engines |

---

## Overlay 31 Functions Used

Overlay 31 provides a general **text-renderer** used across multiple overlays (StaffRoll, OtosuMenu, etc.).  
`UnkOv31Struct` ¨ **`TextRendererCtx`**

| Function | Suggested name | Description |
|---|---|---|
| `func_ov031_0210aa94(ctx)` | `TextRenderer_Init` | Initialises a `TextRendererCtx` to its default state (zeroes it, allocates internal font resources via the system font). |
| `func_ov031_0210aabc(ctx)` | `TextRenderer_Destroy` | Releases all `Data*` resources held by the context (`charData`, `paletteData`, `stringData`, `fontPack`); clears `textBuffer` pointer. |
| `func_ov031_0210ab28(ctx, x, y)` | `TextRenderer_SetPosition` | Sets the render origin: `ctx->posX = x`, `ctx->posY = y`. The Y value is stored in `unk_70` which is used as the BG V-offset anchor for the credit entry. |
| `func_ov031_0210ab34(ctx, palette)` | `TextRenderer_SetPalette` | Sets the 4-bit palette index (`ctx->paletteIndex = palette`) applied to all glyphs during render. |
| `func_ov031_0210ab3c(ctx, align, width)` | `TextRenderer_SetAlignment` | Sets text alignment mode (`ctx->alignment`) and canvas width (`ctx->canvasWidth`). Alignment: 0 = left, 1 = right, 2 = centre. |
| `func_ov031_0210ab54(ctx, enable, offset)` | `TextRenderer_SetLineSpacing` | Sets optional extra line-spacing flag (`ctx->enableSpacing`) and base line spacing offset (`ctx->lineSpacingExtra`). |
| `func_ov031_0210b630(ctx, index)` | `TextRenderer_LoadString` | Loads credit string at `index` from the overlay's font pack into the context. Sets `ctx->stringData` and `ctx->textBuffer`. |
| `func_ov031_0210be18(ctx, screen, chars, flag)` | `TextRenderer_Render` | Renders `ctx->textBuffer` into the given tilemap (`screen`) and tile-gfx (`chars`) buffers. Core glyph-layout and blitting routine. |
| `func_ov031_0210c5b4(ctx)` ¨ `u16` | `TextRenderer_MeasureWidth` | Measures the pixel width of the string at `ctx->textBuffer`. Return value is `u16` (despite `void` declaration in ov031.c ? likely a decompiler/signature mismatch; the caller in `func_ov042_02083c78` uses the value as `0x100 - width` to determine when a scrolled entry exits the screen). |

---

## Command Type Reference

| Type | Handler | Description |
|---|---|---|
| 0, 1 | *(no-op)* | Reserved / padding |
| 2 | `ScriptCmd_SetLayerVisible` | Show/hide BG layer |
| 3 | `ScriptCmd_LoadCharResource` | Load tile gfx into char slot |
| 4 | `ScriptCmd_LoadScreenResource` | Load tilemap into screen slot |
| 5 | `ScriptCmd_LoadPaletteResource` | Load palette/OBJ resource |
| 6 | `ScriptCmd_ReloadCharTiles` | Re-upload char tiles (after FontRoll overwrites them) |
| 7 | `ScriptCmd_ReleaseCharSlot` | Free char slot |
| 8 | `ScriptCmd_ReleaseScreenSlot` | Free screen slot |
| 9 | `ScriptCmd_ReleasePaletteSlot` | Free palette slot |
| 10 | `StaffRoll_CreateStaffBgTask` | Spawn blend-coefficient animation |
| 11 | `StaffRoll_CreateBgScrollTask` | Spawn BG H/V scroll animation |
| 12 | `StaffRoll_CreateFontRollTask` | Spawn rolling-text animation |
| 13 | `StaffRoll_CreateBgBrightTask` | Spawn brightness fade animation |

---

## Asset Files

| File | Description |
|---|---|
| `Apl_Suy/Grp_Staff_PartA_Pack.bin` | Tile gfx + tilemaps for credits part A |
| `Apl_Suy/Grp_Staff_PartB_Pack.bin` | Tile gfx + tilemaps for credits part B |
| `Apl_Suy/Grp_Staff_PartC_Pack.bin` | Tile gfx + tilemaps for credits part C |
| `Apl_Suy/Grp_Staff_PartD_Pack.bin` | Tile gfx + tilemaps for credits part D |
| `Apl_Suy/staff_cmdlist.bin` | Binary command list (sequence of `ScriptCommand` structs) |
| `Apl_Suy/staff_font.bin` | Font/name data for the rolling credits text |
