#ifndef UTIL_SYSFONT_H
#define UTIL_SYSFONT_H

#include "Engine/File/DatMgr.h"
#include "SpriteMgr.h"
#include <nitro/types.h>

/**
 * @file SysFont.h
 * @brief The system font renderer (overlay 31).
 *
 * A SysFont draws message strings into OBJ tile data, a BG screen, or the
 * current cell of a Sprite, using glyph bitmaps loaded from Apl_Fuk/Grp_Font.bin
 * and message text from Apl_Fuk/mestxt.bin.
 */

/**
 * @brief Character of a font string, representing a glyph index in the font's code space,
 * or a control code when (code & SYSFONT_CODE_MASK) == SYSFONT_CODE_MASK.
 */
typedef u16 SysCode;

#define SYSFONT_CODE_MASK      0xFF00
#define SYSFONT_CODE_STR_END   0xFFFF // end of the whole string
#define SYSFONT_CODE_MSG_END   0xFFFD // end of one message within a string
#define SYSFONT_CODE_LINEBREAK 0xFFFE
#define SYSFONT_CODE_WIDTH_1X  0xFFE0 // single-width glyph cells from here on
#define SYSFONT_CODE_WIDTH_2X  0xFFE1 // double-width glyph cells from here on

// Format specifiers; each consumes one vararg in SysFont_Format.
#define SYSFONT_CODE_FMT_U32 0xFFD0
#define SYSFONT_CODE_FMT_S32 0xFFD1
#define SYSFONT_CODE_FMT_STR 0xFFD2 // SysCode* string, spliced in verbatim
#define SYSFONT_CODE_FMT_CHR 0xFFD3 // single u16 code
// 0xFFC0..0xFFC4: fixed-point value printed with (code % 10) fractional digits.
#define SYSFONT_CODE_FMT_FX0 0xFFC0 // N in FXn is the fractional digit count
#define SYSFONT_CODE_FMT_FX1 0xFFC1
#define SYSFONT_CODE_FMT_FX2 0xFFC2
#define SYSFONT_CODE_FMT_FX3 0xFFC3
#define SYSFONT_CODE_FMT_FX4 0xFFC4
// 0xFFB1..0xFFBF: set the text colour to (code & 0xF).
#define SYSFONT_CODE_COLOR_FIRST 0xFFB1
#define SYSFONT_CODE_COLOR_LAST  0xFFBF

#define SYSFONT_NO_LIMIT      0xFFFF
#define SYSFONT_PACK_NONE     0xFFFF // absent pack entry in a SysFontInfo
#define SYSFONT_MSG_NOT_FOUND 0xFFFC // requested message index does not exist

// Glyph indices in the font's own code space.
#define SYSFONT_GLYPH_SPACE 0x000 // advances SYSFONT_SPACE_ADVANCE px
// '0'..'9' are SYSFONT_GLYPH_DIGIT_0 + n.
#ifdef REGION_USA
    #define SYSFONT_GLYPH_DIGIT_0 0x010
#else
    #define SYSFONT_GLYPH_DIGIT_0 0x1CA
#endif
#define SYSFONT_GLYPH_PERIOD     0x05C
#define SYSFONT_GLYPH_MINUS      0x208
#define SYSFONT_GLYPH_WIDE_SPACE 0x1C9 // advances one full glyph cell
#define SYSFONT_SPACE_ADVANCE    3

/**
 * @brief Describes one of the four font variants.
 *
 * Glyph bitmaps live in Grp_Font.bin, split across up to five pack entries of
 * (glyphsPerRow * glyphRows) glyphs each. A glyph index selects the entry and
 * the cell within it; the cell is srcCellWidth x srcCellHeight in the sheet,
 * of which cellWidth x cellHeight pixels are blitted.
 */
typedef struct {
    /* 0x00 */ u16 charPackId;     ///< Grp_Font.bin pack holding the glyph bitmaps
    /* 0x02 */ u16 charEntry[5];   ///< entries within it; SYSFONT_PACK_NONE if absent
    /* 0x0C */ u16 widthPackId;    ///< pack holding the per-glyph advance widths
    /* 0x0E */ u16 widthEntry[5];  ///< entries within it; SYSFONT_PACK_NONE if absent
    /* 0x18 */ u8  baseColorIndex; ///< subtracted from source pixel + colour index
    /* 0x19 */ u8  unk19;
    /* 0x1A */ u16 unk1A;
    /* 0x1C */ u16 unk1C;
    /* 0x1E */ u16 unk1E;
    /* 0x20 */ u16 unk20;
    /* 0x22 */ u16 unk22;
    /* 0x24 */ u16 cellWidth;    ///< blitted glyph size, in pixels
    /* 0x26 */ u16 cellHeight;
    /* 0x28 */ u16 srcCellWidth; ///< cell pitch within the glyph sheet
    /* 0x2A */ u16 srcCellHeight;
    /* 0x2C */ u16 glyphsPerRow; ///< glyph sheet dimensions, in cells
    /* 0x2E */ u16 glyphRows;
} SysFontInfo;                   // Size: 0x30

/**
 * @brief One system font renderer instance.
 *
 * Set up with SysFont_Init / SysFont_InitWithFont, pointed at a message
 * with SysFont_SetMsg or SysFont_SetMsgPtr, positioned and styled with
 * the SysFont_Set* setters, then drawn with one of the SysFont_Draw*
 * render entry points. Released with SysFont_Destroy.
 */
typedef struct {
    /* 0x00 */ s32                fontId;
    /* 0x04 */ const SysFontInfo* info;
    /* 0x08 */ void*              charData;
    /* 0x0C */ void*              widthData;
    /* 0x10 */ void*              msgData;
    /* 0x14 */ void*              msg;
    /* 0x18 */ Data*              mesTableData;
    /* 0x1C */ s32                hasMsg;
    /* 0x20 */ void*              glyphBanks[5];
    /* 0x34 */ void*              widthBanks[5];
    /* 0x48 */ s32                hAlign;
    /* 0x4C */ u16                boxWidth;
    /* 0x4E */ u16                unk4E;
    /* 0x50 */ s32                vAlign;
    /* 0x54 */ u16                boxHeight;
    /* 0x56 */ u16                letterSpacing;
    /* 0x58 */ u16                lineSpacing;
    /* 0x5A */ u8                 color;
    /* 0x5B */ u8                 unk5B;
    /* 0x5C */ u16                widthScale;
    /* 0x5E */ s16                unk5E;
    /* 0x60 */ s16                unk60;
    /* 0x64 */ s32                extendedGlyphs;
    /* 0x68 */ s32                proportional;
    /* 0x6C */ s32                x;
    /* 0x70 */ s32                y;
    /* 0x74 */ s32                clipRight;
    /* 0x78 */ s32                clipBottom;
} SysFont; // Size: 0x7C

/// @brief Top-left corner of the bounding box over the OamCellPieces of the sprite's current frame.
void SysFont_GetCellBoundsMin(const Sprite* sprite, s32* minX, s32* minY);

/**
 * @brief printf for font strings: copies @p fmt to @p dst, replacing each SYSFONT_CODE_FMT_* with
 *        its vararg, and terminates with SYSFONT_CODE_STR_END.
 * @return the number of u16 written, not counting the terminator
 */
u16 SysFont_Format(SysCode* dst, const SysCode* fmt, ...);

/// @brief TRUE for SYSFONT_CODE_MSG_END and SYSFONT_CODE_STR_END - the codes that end a walk.
BOOL SysFont_IsTerminator(SysCode code);

/**
 * @brief Initialises a SysFont in place: points it at SysFontInfoTable[fontId] and loads its
 *        advance widths and message table.
 * @param extendedGlyphs   0 leaves glyph banks 1..3 (the extended sheets) unloaded
 * @param loadGlyphBitmaps 0 skips loading the glyph bitmaps; widths still load, so measurement
 *                         keeps working
 */
void SysFont_InitEx(SysFont* font, s32 fontId, s32 extendedGlyphs, s32 loadGlyphBitmaps);

/// @brief SysFont_InitEx(font, 1, 1, 1) - the default font, fully loaded.
void SysFont_Init(SysFont* font);

/// @brief SysFont_InitEx(font, fontId, extendedGlyphs, 1) - pick a variant, load the bitmaps.
void SysFont_InitWithFont(SysFont* font, s32 fontId, s32 extendedGlyphs);

/// @brief Releases everything the font owns and leaves the struct safe to discard.
void SysFont_Destroy(SysFont* font);

/// @brief Sets the pen origin, in pixels, that the SysFont_Draw* entry points start from.
void SysFont_SetPos(SysFont* font, s32 x, s32 y);

/// @brief Sets the text colour: a palette index added to each source pixel. Defaults to 0xE.
void SysFont_SetColor(SysFont* font, s32 color);

/**
 * @brief Sets horizontal alignment within a box of @p boxWidth pixels.
 * @param align 0 centres each line, 2 right-aligns it, anything else starts at the pen x
 */
void SysFont_SetHAlign(SysFont* font, s32 align, s32 boxWidth);

/**
 * @brief Sets vertical alignment within a box of @p boxHeight pixels.
 * @param align 0 centres the block, 4 bottom-aligns it, anything else starts at the pen y
 */
void SysFont_SetVAlign(SysFont* font, s32 align, u16 boxHeight);

/**
 * @brief Sets glyph spacing.
 * @param proportional 0 advances every glyph by the full cell width; non-zero uses the
 *                     per-glyph widths
 */
void SysFont_SetSpacing(SysFont* font, s32 proportional, u16 letterSpacing);

/// @brief Extra pixels added to the cell height at each SYSFONT_CODE_LINEBREAK.
void SysFont_SetLineSpacing(SysFont* font, u16 lineSpacing);

/**
 * @brief Copies one of the font palettes into a fresh 0x20-byte gDebugHeap block.
 * @param paletteIndex index into PaletteEntryOrder, not the pack entry number
 * @return the copy; the caller frees it with Mem_Free
 */
void* SysFont_GetAllocPal(s32 paletteIndex);

// A "message" is a run of codes ending in SYSFONT_CODE_MSG_END; a string holds one or more,
// terminated with SYSFONT_CODE_STR_END.

/// @brief Points the font at a caller-owned string; ownership of @p msg stays with the caller.
void SysFont_SetMsgPtr(SysFont* font, SysCode* msg);

/// @brief Loads message-table entry @p msgIndex from mestxt.bin and makes it the current message.
void SysFont_SetMsg(SysFont* font, u16 msgIndex);

/**
 * @brief Copies message-table entry @p msgIndex into a fresh gDebugHeap block.
 * @return the copy; the caller frees it with Mem_Free
 */
void* SysFont_GetMsgBuf(SysFont* font, u16 msgIndex);

/**
 * @brief Draws into 4bpp OBJ/BG character data.
 * @param widthTiles  destination size in 8x8 tiles; also clips the glyphs horizontally
 * @param heightTiles destination size in 8x8 tiles; also clips the glyphs vertically
 */
s32 SysFont_DrawToCharRange(SysFont* font, SysCode* msg, void* charData, u16 widthTiles, u16 heightTiles, u16 firstGlyph,
                            u16 glyphCount);
s32 SysFont_DrawToChar(SysFont* font, SysCode* msg, void* charData, u16 widthTiles, u16 heightTiles);
s32 SysFont_DrawCurrentToChar(SysFont* font, void* charData, u16 widthTiles, u16 heightTiles);

/**
 * @brief Draws through a BG screen: each pixel is routed via @p map to the tile that owns it.
 * @param screenSize BG map dimensions - 0: 256x256, 1: 512x256, 2: 256x512, 3: 512x512
 */
s32 SysFont_DrawToScreenRange(SysFont* font, SysCode* msg, u16* map, void* charData, u32 screenSize, u16 firstGlyph,
                              u16 glyphCount);
s32 SysFont_DrawToScreen(SysFont* font, SysCode* msg, u16* map, void* charData, u32 screenSize);
s32 SysFont_DrawCurrentToScreen(SysFont* font, u16* map, void* charData, s32 screenSize);

/**
 * @brief Draws into the sprite's current cell, resolving each pixel through its OamCellPieces.
 * @param toVram non-zero pokes OBJ VRAM directly; 0 edits the CPU-side char source and
 *               invalidates it so SpriteMgr re-uploads the edited data
 */
s32 SysFont_DrawToSpriteRange(SysFont* font, SysCode* msg, Sprite* sprite, s32 toVram, u16 firstGlyph, u16 glyphCount);
s32 SysFont_DrawToSprite(SysFont* font, SysCode* msg, Sprite* sprite, s32 toVram);
s32 SysFont_DrawCurrentToSprite(SysFont* font, Sprite* sprite, s32 toVram);

/**
 * @brief Draws message @p msgIndex of the current string into a sprite.
 * @return SYSFONT_MSG_NOT_FOUND if the string has no such message
 */
s32 SysFont_DrawMsgToSprite(SysFont* font, s32 msgIndex, Sprite* sprite, s32 toVram);
s32 SysFont_DrawMsgToSpriteRange(SysFont* font, s32 msgIndex, Sprite* sprite, s32 toVram, u16 firstGlyph, u16 glyphCount);

/// @brief Number of messages in @p msg (SYSFONT_CODE_MSG_END separated); always at least 1.
u16 SysFont_CountMsgs(SysCode* msg);

/// @brief Counts the messages inside message-table entry @p entryIndex.
u16 SysFont_LoadMsgCount(s32 entryIndex);

/// @brief Pointer to message @p msgIndex within @p msg, or NULL if the string is shorter.
SysCode* SysFont_GetMsgAt(SysCode* msg, s32 msgIndex);

/**
 * @brief Width in pixels of the widest line.
 * @param from  index of the first code to look at
 * @param limit stop after this many codes; SYSFONT_NO_LIMIT for the whole string
 */
u32 SysFont_MeasureWidthEx(SysFont* font, SysCode* msg, s32 from, u32 limit);
u32 SysFont_MeasureWidth(SysFont* font, SysCode* msg);
u32 SysFont_MeasureCurrentWidth(SysFont* font);

/**
 * @brief Width of message @p msgIndex of the current string.
 * @return 0 if there is no such message, indistinguishable from an empty message's width
 */
u32 SysFont_MeasureMsgWidth(SysFont* font, s32 msgIndex);

/**
 * @brief Fills @p lineWidths with the pixel width of each line.
 * @return the number of lines written (always at least 1)
 */
u32 SysFont_MeasureLineWidthsEx(SysFont* font, SysCode* msg, u16* lineWidths, s32 from, u32 limit);
u32 SysFont_MeasureLineWidths(SysFont* font, SysCode* msg, u16* lineWidths);

/// @brief Line widths of message @p msgIndex of the current string.
u32 SysFont_MeasureMsgLineWidths(SysFont* font, s32 msgIndex, u16* lineWidths);

/**
 * @brief Total height in pixels, counting the tallest width scale used on each line.
 * @param limit stop after this many glyphs; SYSFONT_NO_LIMIT for the whole string
 */
u16 SysFont_MeasureHeight(SysFont* font, SysCode* msg);
u16 SysFont_MeasureHeightEx(SysFont* font, SysCode* msg, u16 limit);
u16 SysFont_MeasureCurrentHeight(SysFont* font);

/// @brief Height of message @p msgIndex of the current string.
u16 SysFont_MeasureMsgHeight(SysFont* font, s32 msgIndex);

/**
 * @brief Get the length of a string buffer, including the terminator.
 * @param msg pointer to the string buffer
 * @return the length of the string buffer, including the terminator
 */
u16 SysFont_GetStrBufLen(s32 unused, u16* msg);

/**
 * @brief Translate DS firmware character codes into the font code space.
 * @param dsCodes pointer to the array of DS firmware character codes to translate
 * @param count number of codes to translate; codes with no mapping become SYSFONT_CODE_STR_END
 * @return a gDebugHeap buffer of count+1 u16, terminated with SYSFONT_CODE_STR_END
 */
SysCode* SysFont_GetSysCodeBuf_from_DsCode(const u16* dsCodes, s32 count);

/// @brief The DS owner name from the firmware user settings.
SysCode* SysFont_GetOwnerName(void);

/// @brief The DS owner message from the firmware user settings.
SysCode* SysFont_GetOwnerMessage(void);

/// @brief Retrieve the DS owner comment from the firmware user settings, formatted to be split
/// across two lines of 13 SysFont character codes.
SysCode* SysFont_GetUserDSCommentBufReturn(void);

#endif // UTIL_SYSFONT_H
