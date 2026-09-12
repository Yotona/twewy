# Regional Differences

This file serves as a master list of regional differences, documenting all known locations where code differs between released editions, warranting either `#ifdef` preprocessor directives or separate source files.

## Documented Differences

The entirety of USA's overlay 36 is not present in the JP release, leaving it with 47 overlays versus USA's 48. Overlay 36 exclusively contains MsgXls.c

### src/main.c

- **[USA only](../src/main.c#L160)**: Conditional call to `func_020218ec` depending on `SystemStatusFlags.unk_03` value.

### src/Util/MsgXls.c

- **[USA only](../src/Util/MsgXls.c)**: Utility TU providing names and cumulative row counts of Excel sheets which seemingly contain message data. Those Excel sheets are not shipped with the ROM.

### libs/nitro/src/fx/fx_matrix.c

- **[USA only](../libs/nitro/src/fx/fx_matrix.c#L198)**: `Mtx_LookAt` function is defined.
- **[USA only](../libs/nitro/src/fx/fx_matrix.c#L333)**: `Mtx_OrthoW` function is defined.

### include/Util/SysFont.h

- **[Both](../include/Util/SysFont.h#L69)**: `SYSFONT_GLYPH_DIGIT_0`, the glyph index of `'0'`, is `0x010` in USA and `0x1CA` in JP.

### src/Util/SysFont.c

- JP decodes a glyph at its native size and upscales the bitmap in place before blitting. USA hands the unscaled glyph to the blitters and scales while writing pixels.
  - **[JP only](../src/Util/SysFont.c#L1172)**: `SysFont_ScaleGlyph`, a nearest-neighbour in-place upscale, called by `SysFont_DrawInternal` after `SysFont_DecodeGlyph`.
  - **[Both](../src/Util/SysFont.c#L36)**: `SYSFONT_GLYPH_PITCH`, the decoded-glyph buffer row pitch, is `0x10` in USA and `0x30` in JP.
  - **[Both](../src/Util/SysFont.c#L1236)**, **[Both](../src/Util/SysFont.c#L1349)**, **[Both](../src/Util/SysFont.c#L1528)**: the three blitters use the scaled cell width as both extents in JP, and never read `SysFontInfo::cellHeight`. USA uses both extents.
  - **[Both](../src/Util/SysFont.c#L29)**: `SYSFONT_ADVANCE` - JP multiplies glyph widths and letter spacing by `widthScale` when advancing the pen, USA does not.
  - **[Both](../src/Util/SysFont.c#L1762)**: JP scales the line-break advance by `widthScale`.
  - **[Both](../src/Util/SysFont.c#L2007)**, **[Both](../src/Util/SysFont.c#L2086)**: `SysFont_MeasureWidthEx` and `SysFont_MeasureLineWidthsEx` measure in scaled pixels in JP, honouring `SYSFONT_CODE_WIDTH_1X` / `_2X` as a running scale. USA skips those two codes and measures unscaled.
- **[USA only](../src/Util/SysFont.c#L1118)**: `SysFont_DecodeGlyph` blanks the cell for a glyph outside the loaded banks. JP decodes unconditionally.
- **[Both](../src/Util/SysFont.c#L678)**: `SysFont_GetCellBoundsMin` sign-extends OAM cell coordinates from a different bit per release. Both mask `attr1 & 0x1FF` / `attr0 & 0xFF` and both OR in `-0x200` / `-0x100`, but USA tests the true sign bit of each masked field (`0x100` / `0x80`) while JP tests one bit lower (`0x80` / `0x40`), so JP sign-extends a band of coordinates USA leaves positive.
- **[USA only](../src/Util/SysFont.c#L1981)**: `SysFont_GetGlyphWidth` returns 0 for a glyph outside the loaded banks.
- **[USA only](../src/Util/SysFont.c#L2263)**: `SysFont_DsCodesToSysCodes` substitutes sysCode 4 with 358. JP stores the mapped code as-is.
- **[USA only](../src/Util/SysFont.c#L1736)**: `SysFont_DrawInternal` ignores the `SYSFONT_CODE_FMT_*` codes. JP draws them as glyphs.
- **[USA only](../src/Util/SysFont.c#L78)**: `sFontInfo_10x12` (`fontId` 3) is defined, making [`SysFontInfoTable`](../src/Util/SysFont.c#L86) four entries rather than three.
- **[USA only](../src/Util/SysFont.c#L915)**: `SysFont_InitEx` defaults `lineSpacing` to 1 for `fontId` 3. JP always uses 2.
- **[USA only](../src/Util/SysFont.c#L1893)**: `SysFont_CountMsgs` and `SysFont_LoadMsgCount` are defined.
- **[USA only](../src/Util/SysFont.c#L2215)**: `SysFont_GetStrBufLen` is defined.

### src/Interface/Menu/MenuEquip.c

USA's overlay 43 defines 37 functions in this TU against JP's 36 (JP is overlay 42; see the overlay 36 shift above).

- **[USA only](../src/Interface/Menu/MenuEquip.c#L460)**: `MenuEquip_RefreshCursorInfo`, a wrapper pairing `MenuEquip_UpdateCursorItem` and `MenuEquip_RefreshItemInfo`. JP defines both, but never invokes them at its only USA call site.
- **[USA only](../src/Interface/Menu/MenuEquip.c#L190)**: `MenuEquip_CreateTasks` creates the pointer task. JP never does.
- **[USA only](../src/Interface/Menu/MenuEquip.c#L220)**: `MenuEquip_CreateTasks` creates the second scrollbar task. JP never does.
- **[USA only](../src/Interface/Menu/MenuEquip.c#L681)**: `MenuEquip_UpdateButtonInput` previews a CD item held under the cursor, via `MenuEquip_IsCdItem` / `MenuEquip_PlayCdTrack`. JP's button path omits this; both releases still preview from the touch path in [`MenuEquip_StageMain`](../src/Interface/Menu/MenuEquip.c#L781), and both define the two CD helpers.

### src/Interface/Menu/MenuEquipData.c

USA defines 57 functions in this TU against JP's 52. The five extras all belong to two USA-added features: per-thread *ability* bonuses and *swag* stat totals, plus the item list's page rounding.

- **[USA only](../src/Interface/Menu/MenuEquipData.c#L144)**: `MenuEquip_AddSwagBonus`, accumulating the per-character attack/defense totals `unk_DB38` through `unk_DB3D` from six swag item IDs. Called only from [`MenuEquip_BuildItemEntries`](../src/Interface/Menu/MenuEquipData.c#L569), whose JP counterpart has no such call.
- **[USA only](../src/Interface/Menu/MenuEquipData.c#L167)**, **[USA only](../src/Interface/Menu/MenuEquipData.c#L243)**, **[USA only](../src/Interface/Menu/MenuEquipData.c#L316)**: `MenuEquip_GetAbilityAttackBonus`, `MenuEquip_GetAbilityDefenseBonus` and `MenuEquip_GetAbilityHealthBonus`, the per-character bonus tables keyed on a thread's ability id.
- **[Both](../src/Interface/Menu/MenuEquipData.c#L389)**, **[Both](../src/Interface/Menu/MenuEquipData.c#L417)**, **[Both](../src/Interface/Menu/MenuEquipData.c#L445)**: `MenuEquip_CalcAttackBonus`, `MenuEquip_CalcDefenseBonus` and `MenuEquip_CalcHealthBonus`. JP sums the raw per-thread stat across the four slots and clamps (0x54/0x54/0x90 bytes). USA additionally skips empty (`0xFFFF`) entries, adds the ability bonus when `unk_12 == 1`, and, for attack and defense, adds the swag totals (0xF8/0xF8/0x110 bytes).
- **[USA only](../src/Interface/Menu/MenuEquipData.c#L853)**: `MenuEquip_RoundUpToPages`, rounding a list length up to whole pages of 8 rows.
- **[Both](../src/Interface/Menu/MenuEquipData.c#L875)**: `MenuEquip_BuildTabs`. JP fills the nine tab pointer tables and returns. USA also counts each tab and derives the paging state `unk_DAF0`, `unk_DB02`, `unk_DB14` and `unk_DB26` through `MenuEquip_RoundUpToPages`.
- **[USA only](../src/Interface/Menu/MenuEquipData.c#L979)**: `MenuEquip_LoadFromSave` refills every character's food capacity when flag `0x2CB` is set. JP returns after storing the date, keeping only the preceding date-change refill.

### src/Interface/Menu/MenuEquip/

- **[USA only](../src/Interface/Menu/MenuEquip/MenuEquip_pointer.c)**: task TU with no JP counterpart.
- **[USA only](../src/Interface/Menu/MenuEquip/MenuEquip_sbar2.c)**: task TU with no JP counterpart; JP ships only [`MenuEquip_sbar.c`](../src/Interface/Menu/MenuEquip/MenuEquip_sbar.c). USA's overlay delinks 28 MenuEquip task TUs against JP's 26.
