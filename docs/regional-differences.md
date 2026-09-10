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
