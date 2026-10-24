#include "Display.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Text.h"
#include "common_data.h"
#include <nitro/g2.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>

/**
 * A text layer drawn on a background: owns the BG character block and palette it allocates and
 * a TextObject set up from a template. Character tiles 0 and 1 are a blank tile and a tile filled
 * with the template's first color index.
 */
typedef struct {
    /* 0x00 */ s32              engine;
    /* 0x04 */ BgResource*      chr;
    /* 0x08 */ PaletteResource* pal;
    /* 0x0C */ TextObject       text;
} BgText;

typedef struct {
    /* 0x00 */ u32   type;
    /* 0x04 */ void* addr;
    /* 0x08 */ u32   unk_08;
    /* 0x0C */ u32   size;
} BgCharInfo;

// BgResMgr.c defines this with an extra stack argument; its callers pass four.
BgResource* BgResMgr_AllocCharAligned(BgResMgr* mgr, UnkSmallStruct_DispBG* src, u32 charBase, u32 size);
void        func_020105b8(TextObject* obj);

s32  BgText_Init(BgText* bgText, UnkStruct_usedby_02025b68* args);
void BgText_Free(BgText* bgText);

static const TextFontFile sFontFunakosi = {0x80000002, "Static/Font_Funakosi.bin"};

static const u16 sBgTextDefaultPalette[16] = {0x0000, 0x7FFF, 0x3DEF, 0x001F};

const TextFontInfo data_0205c9b0[6] = {
    {&sFontFunakosi, 1, 1, 1,  8,  8,  8, 2,  4, 3,  8},
    {&sFontFunakosi, 1, 1, 2, 16,  8,  8, 1,  5, 3,  8},
    {&sFontFunakosi, 1, 1, 2, 16,  8,  8, 1,  6, 7,  8},
    {&sFontFunakosi, 1, 1, 1,  8,  4,  8, 1,  8, 3,  4},
    {&sFontFunakosi, 1, 1, 1, 20, 10, 10, 2,  9, 3, 10},
    {&sFontFunakosi, 1, 1, 2, 36, 12, 12, 1, 10, 3, 12},
};

static UnkSmallText sBgTextTemplate = {
    0, 0, 0, 0, {0, 0},
        {32, 24},
        {0, 0},
        0, 0, data_0205c9b0, 1, 0, 0, {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
};

static inline BgCharInfo BgText_GetCharInfo(BgResMgr* mgr, BgResource* chr) {
    BgCharInfo info;

    info.type = 7;
    info.addr = (void*)(mgr->vramBase + (chr->bitmapIndex << 8));
    info.size = chr->size;
    return info;
}

static inline s32 BgText_GetTileIndex(BgResource* chr) {
    s32 offset = (chr->bitmapIndex << 8) - chr->vramOffset;

    switch (chr->resourceType) {
        case RESTYPE_CHAR_SM:
            return offset >> 5;
        case RESTYPE_CHAR_MD:
        case RESTYPE_CHAR_LG:
            return offset >> 6;
    }
    return offset;
}

static inline void BgText_SetupTemplate(UnkStruct_usedby_02025b68* args) {
    sBgTextTemplate.unk_00 = args->unk_00;
    sBgTextTemplate.unk_04 = args->unk_04;
    sBgTextTemplate.unk_08 = args->unk_08;
    sBgTextTemplate.font   = (const TextFontInfo*)args->unk_0C;
    sBgTextTemplate.unk_28 = args->unk_10;
    sBgTextTemplate.unk_10 = args->tilePos;
    sBgTextTemplate.unk_14 = args->tileSize;
}

// Nonmatching: register allocation (target keeps g_BgResourceManagers in r7 across the first two calls)
s32 BgText_Init(BgText* bgText, UnkStruct_usedby_02025b68* args) {
    const void* palette;
    BgCharInfo  info;
    void*       chars;
    u32         fill;
    void*       scr;

    BgText_SetupTemplate(args);
    palette = args->unk_14;

    if (palette == NULL) {
        palette = sBgTextDefaultPalette;
    }
    bgText->engine = sBgTextTemplate.unk_00;
    bgText->chr    = BgResMgr_AllocCharAligned(
        g_BgResourceManagers[sBgTextTemplate.unk_00], NULL,
        g_DisplaySettings.engineState[sBgTextTemplate.unk_00].bgSettings[sBgTextTemplate.unk_04].charBase,
        (sBgTextTemplate.unk_14[0] * sBgTextTemplate.unk_14[1] + 2) << 5);
    bgText->pal = PaletteMgr_AcquireContiguous(g_PaletteManagers[sBgTextTemplate.unk_00], palette, 0, 1);

    fill = sBgTextTemplate.unk_30[0];
    fill |= fill << 4;
    fill |= fill << 8;
    info  = BgText_GetCharInfo(g_BgResourceManagers[bgText->engine], bgText->chr);
    chars = info.addr;
    fill |= fill << 16;
    MI_CpuFill(0, chars, 0x20);
    MI_CpuFill(fill, (u8*)chars + 0x20, 0x20);

    sBgTextTemplate.unk_2E = (BgText_GetTileIndex(bgText->chr) + 2) | (bgText->pal->palIndex << 12);
    sBgTextTemplate.unk_2C = (BgText_GetTileIndex(bgText->chr) + 1) | (bgText->pal->palIndex << 12);
    sBgTextTemplate.unk_1C = g_BgResourceManagers[sBgTextTemplate.unk_00]->vramBase + (bgText->chr->bitmapIndex << 8) + 0x40;

    scr = NULL;
    if (sBgTextTemplate.unk_00 == DISPLAY_MAIN) {
        switch (sBgTextTemplate.unk_04) {
            case 0:
                scr = G2_GetBG0ScrPtr();
                break;
            case 1:
                scr = G2_GetBG1ScrPtr();
                break;
            case 2:
                scr = G2_GetBG2ScrPtr();
                break;
            case 3:
                scr = G2_GetBG3ScrPtr();
                break;
        }
    } else {
        switch (sBgTextTemplate.unk_04) {
            case 0:
                scr = G2S_GetBG0ScrPtr();
                break;
            case 1:
                scr = G2S_GetBG1ScrPtr();
                break;
            case 2:
                scr = G2S_GetBG2ScrPtr();
                break;
            case 3:
                scr = G2S_GetBG3ScrPtr();
                break;
        }
    }
    sBgTextTemplate.unk_20 = (u32)scr;

    func_020104c0(&bgText->text, &sBgTextTemplate);
    func_02010b50(&bgText->text);
    return 1;
}

void BgText_Free(BgText* bgText) {
    BgResMgr_ReleaseChar(g_BgResourceManagers[bgText->engine], bgText->chr);
    PaletteMgr_ReleaseResource(g_PaletteManagers[bgText->engine], bgText->pal);
    func_020105b8(&bgText->text);
}
