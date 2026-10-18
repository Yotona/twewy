#include "Debug/Mori/GrpCheck.h"
#include "Display.h"
#include "Engine/Color.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Str.h"
#include "Engine/Math/Rect.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "common_data.h"
#include <string.h>

void GrpCheck_RegisterVBlank(void);
void GrpCheck_ClearVBlank(void);

extern char data_ov038_020ad3e4[15];

extern const GrpCheckUnk data_ov038_02086fe4[294];

struct {
    /* 0x0 */ const char*         sequence;
    /* 0x4 */ const SpriteConfig* unk_04;
} data_ov038_020ac980 = {
    .sequence = data_ov038_020ad3e4,
    .unk_04   = NULL,
};

struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} data_ov038_020b7200;

typedef struct {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} GrpCheckPoint;

typedef struct {
    /* 0x0 */ s32 left;
    /* 0x4 */ s32 top;
    /* 0x8 */ s32 right;
    /* 0xC */ s32 bottom;
} GrpCheckRect;

GrpCheckPoint func_ov038_020824a0(OamCellPiece* piece) {
    GrpCheckPoint pos;

    pos.x = piece->attr1 & 0x1FF;
    pos.y = piece->attr0 & 0xFF;

    if (pos.x >= 0x100) {
        pos.x -= 0x200;
    }
    if (pos.y >= 0x80) {
        pos.y -= 0x100;
    }
    return pos;
}

void func_ov038_02082500(GrpCheckRect* out, OamCellPiece* piece) {
    GrpCheckPoint pos   = func_ov038_020824a0(piece);
    s16           x     = pos.x;
    s16           y     = pos.y;
    OamCellShape* shape = OamMgr_GetCellShape(piece);
    GrpCheckRect  rect;

    rect.left   = x;
    rect.top    = y;
    rect.right  = x + shape->width;
    rect.bottom = y + shape->height;

    *out = rect;
}

u16 func_ov038_02082578(GrpCheckBigUnk* arg0) {
    return ((u16*)arg0->sprite.cellTable)[arg0->sprite.cellIndex * 4 + 1];
}

u16 func_ov038_02082594(GrpCheckBigUnk* arg0) {
    return ((u16*)arg0->sprite.cellTable)[arg0->sprite.cellIndex * 4 + 2];
}

OamCellPiece* func_ov038_020825b0(GrpCheckBigUnk* arg0) {
    return (OamCellPiece*)((u16*)arg0->sprite.cellTable + arg0->sprite.cellTable[arg0->sprite.cellIndex].pieceOffset);
}

s32 func_ov038_020825c8(GrpCheckBigUnk* arg0, s16 arg1, s16 arg2) {
    s32  i;
    s32  count;
    u16* pieceData;
    s32  offsetX;
    s32  offsetY;
    s32  rect[4];

    if ((arg0->sprite.cellTable == NULL) || (arg0->sprite.cellIndex < 0)) {
        return 0;
    }

    count     = func_ov038_02082578(arg0);
    pieceData = (u16*)func_ov038_020825b0(arg0);
    offsetX   = arg0->sprite.posX + arg0->sprite.scaleX;
    offsetY   = arg0->sprite.posY + arg0->sprite.scaleY;

    for (i = 0; i < count; i++) {
        func_ov038_02082500((GrpCheckRect*)rect, (OamCellPiece*)&pieceData[i * 4]);
        rect[0] += offsetX;
        rect[1] += offsetY;
        rect[2] += offsetX;
        rect[3] += offsetY;
        if (Rect_ContainsPoint(rect, arg1, arg2) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov038_020826b4(GrpCheckBigUnk* arg0) {
    s32  counts[0xC0];
    s32  max = 0;
    s32  pieceCount;
    u16* pieceData;
    s32  i;
    u32  k;
    s32  j;

    if ((arg0->sprite.cellTable == NULL) || (arg0->sprite.cellIndex < 0)) {
        return 0;
    }
    for (k = 0; k < 0xC0; k++) {
        counts[k] = 0;
    }
    pieceCount = func_ov038_02082578(arg0);
    pieceData  = (u16*)func_ov038_020825b0(arg0);

    if (pieceCount > 0) {
        for (i = 0; i < pieceCount; i++) {
            OamCellPiece* piece = (OamCellPiece*)&pieceData[i * 4];
            GrpCheckPoint pos   = func_ov038_020824a0(piece);
            OamCellShape* shape = OamMgr_GetCellShape(piece);

            for (j = 0; j < shape->height; j++) {
                s32 index = pos.y + 0x60 + j;

                if ((index >= 0) && (index < 0xC0)) {
                    counts[index] += shape->unk_00;
                }
            }
        }
    }

    for (k = 0; k < 0xC0; k++) {
        if (counts[k] > max) {
            max = counts[k];
        }
    }
    return max;
}

void* func_ov038_020827e4(s32 arg0, s32 arg1, s32* arg2, s32 arg3, s32 arg4, u32 arg5) {
    switch (arg0) {
        case 0:
            return BgResMgr_AllocChar32(g_BgResourceManagers[arg1], arg2, arg3, arg4, arg5);
        case 1:
            return BgResMgr_AllocChar64(g_BgResourceManagers[arg1], arg2, arg3, arg4, arg5);
        case 2:
            return BgResMgr_AllocCharExtended(g_BgResourceManagers[arg1], arg2, arg3, arg4, arg5);
        default:
            return NULL;
    }
}

s32  func_0200d1d8(void* entry, s32 engineId, s32 bgLayer, void* tilemap, void* tileData, s32 width, s32 height);
void func_0200d858(void* entry, s32 wrapH, s32 wrapV, void* defaultTile);

static inline void* GrpCheck_LookupBgEntry(Data* data, s32 index) {
    u8* base;

    if ((data == NULL) || (index <= 0)) {
        return NULL;
    }
    base = (u8*)data->buffer + 0x20;
    return base + *(s32*)(base + index * 8);
}

void func_ov038_0208287c(GrpCheckBg* bg, GrpCheckBgParams* params) {
    MI_CpuSet(bg, 0, sizeof(GrpCheckBg));
    bg->params = *params;

    if (params->unk_1C == 0) {
        if (params->unk_20 == 0) {
            bg->data = DatMgr_LoadRawData(params->unk_14, NULL, 0, params->unk_18);
        } else {
            bg->data = DatMgr_LoadCompressedBin(params->unk_14, NULL, 0, params->unk_18);
        }
    } else {
        bg->data = DatMgr_LoadPackEntry(params->unk_14, NULL, 0, params->unk_18, params->unk_1C, params->unk_20);
    }

    bg->paletteRes =
        PaletteMgr_AllocPalette(g_PaletteManagers[params->unk_00], GrpCheck_LookupBgEntry(bg->data, params->unk_24),
                                params->unk_34, 0, params->unk_38);

    {
        s32* charData = GrpCheck_LookupBgEntry(bg->data, params->unk_26);
        s32  charSize = params->unk_3A;

        if (charSize == 0) {
            charSize = ((u32)*charData & ~0xFF) >> 8;
            if (((u8)*charData & 0xF0) == 0) {
                charSize -= 4;
            }
        }
        bg->charRes1 = func_ov038_020827e4(
            g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_04].bgMode, params->unk_00, charData,
            g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_04].charBase, 0, (u32)charSize);
    }
    if (params->unk_28 != 0) {
        s32* charData = GrpCheck_LookupBgEntry(bg->data, params->unk_28);
        s32  charSize = params->unk_3C;

        if (charSize == 0) {
            charSize = ((u32)*charData & ~0xFF) >> 8;
            if (((u8)*charData & 0xF0) == 0) {
                charSize -= 4;
            }
        }
        bg->charRes2 = func_ov038_020827e4(
            g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_08].bgMode, params->unk_00, charData,
            g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_08].charBase, 0, (u32)charSize);
    }

    if ((params->unk_2C == 0) && (params->unk_2E == 0)) {
        switch (g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_04].bgMode) {
            case DISPLAY_BGMODE_AFFINE:
            case DISPLAY_BGMODE_PLTT:
            case DISPLAY_BGMODE_BMP256:
            case DISPLAY_BGMODE_BMPDIRECT:
            case DISPLAY_BGMODE_BMPLARGE:
                g_DisplaySettings.engineState[params->unk_00].bgAffines[params->unk_04].unk_14 = 1;
                break;
        }
        g_DisplaySettings.engineState[params->unk_00].bgOffsets[params->unk_04].hOffset = 0;
        g_DisplaySettings.engineState[params->unk_00].bgOffsets[params->unk_04].vOffset = 0;
        bg->screenRes =
            BgResMgr_AllocScreen(g_BgResourceManagers[params->unk_00], GrpCheck_LookupBgEntry(bg->data, params->unk_2A),
                                 g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_04].screenBase,
                                 g_DisplaySettings.engineState[params->unk_00].bgSettings[params->unk_04].screenSizeText);
        return;
    }

    if ((params->unk_28 != 0) && (params->unk_2C == 0) && (params->unk_2E != 0)) {
        bg->tile1 = (u8*)GrpCheck_LookupBgEntry(bg->data, params->unk_2A) + 4;
        bg->tile3 = (u8*)GrpCheck_LookupBgEntry(bg->data, params->unk_2E) + 4;
        func_0200d1d8(&bg->screenMap1, params->unk_00, params->unk_04, 0, &bg->tile1, 1, 1);
        func_0200d1d8(&bg->screenMap2, params->unk_00, params->unk_08, 0, &bg->tile3, 1, 1);
        func_0200d858(&bg->screenMap1, params->unk_0C, params->unk_10, 0);
        func_0200d858(&bg->screenMap2, params->unk_0C, params->unk_10, 0);
        return;
    }

    bg->tile1 = (u8*)GrpCheck_LookupBgEntry(bg->data, params->unk_2A) + 4;
    bg->tile2 = (u8*)GrpCheck_LookupBgEntry(bg->data, params->unk_2C) + 4;
    bg->tile3 = (u8*)GrpCheck_LookupBgEntry(bg->data, params->unk_2E) + 4;
    bg->tile4 = (u8*)GrpCheck_LookupBgEntry(bg->data, params->unk_30) + 4;
    func_0200d1d8(&bg->screenMap1, params->unk_00, params->unk_04, 0, &bg->tile1, 1, 2);
    func_0200d1d8(&bg->screenMap2, params->unk_00, params->unk_08, 0, &bg->tile3, 1, 2);
    func_0200d858(&bg->screenMap1, params->unk_0C, params->unk_10, 0);
    func_0200d858(&bg->screenMap2, params->unk_0C, params->unk_10, 0);
}

void func_0200d954(s32 engine, s32 arg);

void func_ov038_02082e44(GrpCheckBg* bg) {
    if (bg->screenRes == NULL) {
        func_0200d954(bg->params.unk_00, bg->params.unk_04);
        func_0200d954(bg->params.unk_00, bg->params.unk_08);
    }
    PaletteMgr_ReleaseResource(g_PaletteManagers[bg->params.unk_00], bg->paletteRes);
    BgResMgr_ReleaseChar(g_BgResourceManagers[bg->params.unk_00], bg->charRes1);
    BgResMgr_ReleaseChar(g_BgResourceManagers[bg->params.unk_00], bg->charRes2);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[bg->params.unk_00], bg->screenRes);
    DatMgr_ReleaseData(bg->data);
}

void func_ov038_02082ed0(GrpCheckBg* bg) {
    if ((bg->params.unk_28 != 0) && (bg->params.unk_2E != 0)) {
        DisplayEngineState* engine1 = &g_DisplaySettings.engineState[bg->screenMap1.engineId];
        s32                 layer1  = bg->screenMap1.bgLayer;

        switch (engine1->bgSettings[layer1].bgMode) {
            case DISPLAY_BGMODE_AFFINE:
            case DISPLAY_BGMODE_PLTT:
            case DISPLAY_BGMODE_BMP256:
            case DISPLAY_BGMODE_BMPDIRECT:
            case DISPLAY_BGMODE_BMPLARGE:
                engine1->bgAffines[layer1].unk_14 = 1;
                break;
        }
        engine1->bgOffsets[layer1].hOffset = bg->offsetX;
        engine1->bgOffsets[layer1].vOffset = bg->offsetY;

        DisplayEngineState* engine2 = &g_DisplaySettings.engineState[bg->screenMap2.engineId];
        s32                 layer2  = bg->screenMap2.bgLayer;

        switch (engine2->bgSettings[layer2].bgMode) {
            case DISPLAY_BGMODE_AFFINE:
            case DISPLAY_BGMODE_PLTT:
            case DISPLAY_BGMODE_BMP256:
            case DISPLAY_BGMODE_BMPDIRECT:
            case DISPLAY_BGMODE_BMPLARGE:
                engine2->bgAffines[layer2].unk_14 = 1;
                break;
        }
        engine2->bgOffsets[layer2].hOffset = bg->offsetX - 0x100000;
        engine2->bgOffsets[layer2].vOffset = bg->offsetY;
    }
}

void func_ov038_02082fc4(GrpCheckBg* bg, s32 offsetX, s32 offsetY) {
    bg->offsetX = offsetX;
    bg->offsetY = offsetY;
}

s32 func_02048210(const char* str1, const char* str2);

s32 func_ov038_02082fd0(const void* arg0, const void* arg1, s32 count) {
    const char* const* str1   = arg0;
    const char* const* str2   = arg1;
    s32                result = 0;
    s32                i      = 0;

    if (count < 0) {
        return result;
    }
    do {
        result = func_02048210(str1[i], str2[i]);
        if (result != 0) {
            return result;
        }
        i++;
    } while (i <= count);
    return result;
}

u32 func_ov038_02083010(GrpCheckState* state) {
    u32 current = state->unk_1165C;

    while (current < 294) {
        const GrpCheckUnk* address = &data_ov038_02086fe4[current];
        if (func_ov038_02082fd0(address, &data_ov038_02086fe4[state->unk_1165C], 0) != 0) {
            return current;
        }
        current++;
    }

    return 0;
}

s32 func_ov038_0208307c(GrpCheckState* state) {
    s32                k = state->unk_1165C;
    s32                j;
    const GrpCheckUnk* base = data_ov038_02086fe4;

    if (k >= 0) {
        const GrpCheckUnk* entry = &base[k];
        do {
            if (func_ov038_02082fd0(entry, &base[state->unk_1165C], 0) != 0) {
                break;
            }
            entry--;
            k--;
        } while (k >= 0);
    }
    if (k <= 0) {
        k = 0x125;
    }
    j = k;
    if (k >= 0) {
        const GrpCheckUnk* entry = &base[k];
        do {
            if (func_ov038_02082fd0(entry, &base[k], 0) != 0) {
                break;
            }
            k = j;
            entry--;
            j--;
        } while (j >= 0);
    }
    return k;
}

s32 func_ov038_0208312c(GrpCheckState* state, s32 arg1) {
    s32 i = state->unk_11B8C;

    while (i < data_ov038_020b7200.unk_0) {
        if (func_ov038_02082fd0(&data_ov038_020ac980.unk_04[i], &data_ov038_020ac980.unk_04[state->unk_11B8C], arg1) != 0) {
            return i;
        }
        i++;
    }
    return 0;
}

s32 func_ov038_020831a4(GrpCheckState* state, s32 arg1) {
    s32 i = state->unk_11B8C;
    s32 j;

    if (i >= 0) {
        do {
            if (func_ov038_02082fd0(&data_ov038_020ac980.unk_04[i], &data_ov038_020ac980.unk_04[state->unk_11B8C], arg1) != 0)
            {
                break;
            }
            i--;
        } while (i >= 0);
    }
    if (i <= 0) {
        i = data_ov038_020b7200.unk_0 - 1;
    }
    j = i;
    if (i >= 0) {
        do {
            if (func_ov038_02082fd0(&data_ov038_020ac980.unk_04[i], &data_ov038_020ac980.unk_04[i], arg1) != 0) {
                break;
            }
            i = j;
            j--;
        } while (j >= 0);
    }
    return i;
}

s32 func_ov038_02083260(GrpCheckState* state) {
    if (state->unk_11B8C < data_ov038_020b7200.unk_0 - 1) {
        return state->unk_11B8C + 1;
    }
    return 0;
}

s32 func_ov038_02083288(GrpCheckState* state) {
    s32 value = state->unk_11B8C;

    if (value <= 0) {
        value = data_ov038_020b7200.unk_0;
    }
    return value - 1;
}

s32 func_ov038_020832a8(GrpCheckState* state, s32 arg1) {
    return data_ov038_020ac980.unk_04[arg1].unk_28 != 0;
}

s32 func_ov038_020832d0(s32 arg0) {
    while (1) {
        if (data_ov038_020ac980.unk_04[arg0].unk_28 == 1) {
            return arg0;
        }
        arg0--;
    }
}

s32 func_ov038_02083300(s32 arg0, s32 arg1, s32 arg2) {
    if (data_ov038_020ac980.unk_04[arg0].unk_28 - 1 <= 1) {
        return arg0 + 1;
    }
    if (data_ov038_020ac980.unk_04[arg0].unk_28 != 3) {
        return -1;
    }
    return func_ov038_020832d0(arg0);
}

void func_02025b68(void* textObj, GrpCheckTextParams* params);

void func_ov038_02083340(GrpCheckState* state) {
    GrpCheckTextParams params;

    params.unk_00 = 1;
    params.unk_04 = 0;
    params.unk_08 = state->unk_00004;
    params.unk_0C = (s32)&data_0205c9b0;
    params.unk_10 = 0;
    params.unk_14 = 0;
    params.unk_18 = 0;
    params.unk_1A = 0;
    params.unk_1C = 0x20;
    params.unk_1E = 0x18;
    func_02025b68(&state->unk_11BA4, &params);
}

void func_ov038_020833a4(GrpCheckState* state) {
    func_02025e30(&state->unk_11BA4);
}

void GrpCheck_PrintRowIndicator(GrpCheckState* state) {
    func_02010b84(&state->textObj, 0, 0, 8, 176);
    Text_RenderToScreen(&state->textObj, 0, state->currentRow * 8, ">");
}

void GrpCheck_PrintSpritePath(GrpCheckState* state) {
    GrpCheckBigUnk* temp_r4 = &state->unk_116C0[state->unk_11B80];

    func_02010b84(&state->textObj, 8, 0, 0x100, 0x20);
    func_02010b84(&state->textObj, 8, 0x20, 0xA8, 8);
    Text_RenderToScreen(&state->textObj, 8, 0, Str_SPrintf("%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_00));
    Text_RenderToScreen(&state->textObj, 8, 8, Str_SPrintf("��%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_04));
    Text_RenderToScreen(&state->textObj, 8, 0x10, Str_SPrintf("�@��%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_08));
    Text_RenderToScreen(&state->textObj, 8, 0x18, Str_SPrintf("%s", data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_24));
    Text_RenderToScreen(&state->textObj, 8, 0x20, Str_SPrintf("anime:%d", temp_r4->sprite.animIndex));
}

// Defined with explicit Shift-JIS byte escapes in GrpCheckData.inc.
extern char data_ov038_020b6fe0[39]; // "�\�t�g�E�F�A�X�v���C�g��\���ł��܂���"

void GrpCheck_PrintSpriteDetails(GrpCheckState* state) {
    GrpCheckBigUnk* temp_r4 = &state->unk_116C0[state->unk_11B80];
    func_02010b84(&state->textObj, 8, 0x28, 0xA8, 0x58);

    if ((data_ov038_020b7200.unk_4 != 2) && (data_ov038_020ac980.unk_04[temp_r4->unk_40].unk_0C == 2)) {
        Text_RenderToScreen(&state->textObj, 8, 0x28, Str_SPrintf(data_ov038_020b6fe0));
        return;
    }

    u16 useChr      = func_ov038_02082594(temp_r4);
    u16 useAtr      = func_ov038_02082578(temp_r4);
    s32 useLineChar = func_ov038_020826b4(temp_r4);

    s32 var_r1 = state->maxChr;
    if (var_r1 < useChr) {
        var_r1 = useChr;
    }
    state->maxChr = var_r1;

    s32 var_r1_2 = state->maxAtr;
    if (var_r1_2 < useAtr) {
        var_r1_2 = useAtr;
    }
    state->maxAtr = var_r1_2;

    s32 var_r1_3 = state->maxLineChr;
    if (var_r1_3 < useLineChar) {
        var_r1_3 = useLineChar;
    }
    state->maxLineChr = var_r1_3;

    Text_RenderToScreen(&state->textObj, 8, 0x28, Str_SPrintf("MaxAtr:%d", state->maxAtr));
    Text_RenderToScreen(&state->textObj, 8, 0x30, Str_SPrintf("UseAtr:%d", useAtr));
    Text_RenderToScreen(&state->textObj, 8, 0x38, Str_SPrintf("MaxChr:%d", state->maxChr));
    Text_RenderToScreen(&state->textObj, 8, 0x40, Str_SPrintf("UseChr:%d", useChr));
    Text_RenderToScreen(&state->textObj, 8, 0x48, Str_SPrintf("MaxLineChr:%d", state->maxLineChr));
    Text_RenderToScreen(&state->textObj, 8, 0x50, Str_SPrintf("UseLineChr:%d", useLineChar));
    Text_RenderToScreen(&state->textObj, 8, 0x58, Str_SPrintf("pic:%d", temp_r4->sprite.cellIndex - 1));
    Text_RenderToScreen(&state->textObj, 8, 0x60,
                        Str_SPrintf("aob:%d", temp_r4->sprite.currentFrame - temp_r4->sprite.loopFrame));
    Text_RenderToScreen(&state->textObj, 8, 0x68, Str_SPrintf("frame:%d", temp_r4->sprite.frameTimer));
    Text_RenderToScreen(&state->textObj, 8, 0x70, Str_SPrintf("cmd_ofs:%d", temp_r4->sprite.currentFrame));
    Text_RenderToScreen(&state->textObj, 8, 0x78, Str_SPrintf("cmd_lop:%d", temp_r4->sprite.loopFrame));
}

void GrpCheck_PrintScale(GrpCheckState* state) {
    GrpCheckBigUnk* entry   = &state->unk_116C0[state->unk_11B80];
    s32             integer = F2I(entry->unk_48);

    func_02010b84(&state->textObj, 8, 0x80, 0xa8, 8);

    s32 fractional = F2I(entry->unk_48 * 100) - (integer * 100);
    Text_RenderToScreen(&state->textObj, 8, 128, Str_SPrintf("scale:%d.%02d", integer, fractional));
}

void GrpCheck_PrintColorChannels(GrpCheckState* state) {
    func_02010b84(&state->textObj, 8, 136, 168, 24);
    Text_RenderToScreen(&state->textObj, 8, 136, Str_SPrintf("BG R:%d", state->bgR));
    Text_RenderToScreen(&state->textObj, 8, 144, Str_SPrintf("BG G:%d", state->bgG));
    Text_RenderToScreen(&state->textObj, 8, 152, Str_SPrintf("BG B:%d", state->bgB));
}

void GrpCheck_PrintScreenPos(GrpCheckState* state) {
    func_02010b84(&state->textObj, 8, 160, 256, 24);
    Text_RenderToScreen(&state->textObj, 8, 160, Str_SPrintf("%s", data_ov038_02086fe4[state->unk_1165C].unk_00));
    Text_RenderToScreen(&state->textObj, 8, 168, Str_SPrintf("��%s", data_ov038_02086fe4[state->unk_1165C].entryName));
    Text_RenderToScreen(&state->textObj, 8, 176,
                        Str_SPrintf("SCREEN POS %d %d", F2I(state->screenPosX), F2I(state->screenPosY)));
}

void GrpCheck_PrintWarnings(GrpCheckState* state) {
    s32 yOffset = 23;
    func_02010b84(&state->textObj, 8, 184, 256, 16);

    if (state->isVramLimitReached) {
        yOffset++;
        // "VRAM limit reached. Please delete objects by pressing X or Touch + X."
        Text_RenderToScreen(&state->textObj, 8, 184, "VRAM����BX�܂��̓^�b�`+X��OBJ���폜���Ă��������B");
    }

    if (state->isPaused != FALSE) {
        Text_RenderToScreen(&state->textObj, 8, yOffset * 8, "pause");
    }
}

void GrpCheck_PrintControls(GrpCheckState* state) {
    Text_RenderToScreen(&state->textObj, 176, 40, "�����F�A�j���ւ񂱂�"); // "����: Change animation"
    Text_RenderToScreen(&state->textObj, 176, 48, "�����F�J�[�\\���㉺");                         // "����: Move cursor up/down"
    Text_RenderToScreen(&state->textObj, 176, 56, "�k�q�F�������ւ񂱂�"); // "LR: Change value"
    Text_RenderToScreen(&state->textObj, 176, 64, "�^�b�`�F���ǂ�");                              // "Touch: Move"
    Text_RenderToScreen(&state->textObj, 176, 72, "�^�b�`�{�w�F��������");                       // "Touch + X: Delete"
    Text_RenderToScreen(&state->textObj, 176, 80, "�w�F���ׂĂ�������");                            // "X: Delete all"
    Text_RenderToScreen(&state->textObj, 176, 88, "�`�F�R�}������");                             // "A: Step forward"
    Text_RenderToScreen(&state->textObj, 176, 96, "�@�@�A�j����������");                         // "  Restart animation"
    Text_RenderToScreen(&state->textObj, 176, 104, "�a�F�A�j���ЂЂ傤��");                         // "B: Display animation"
    Text_RenderToScreen(&state->textObj, 176, 112, "�r�s�`�q�s�F�|�[�Y");                        // "START: Pause"
    Text_RenderToScreen(&state->textObj, 176, 120, "�r�d�k�d�b�s�F�����");                       // "SELECT: Exit"
}

s32 func_ov038_02083c64(GrpCheckState* state) {
    for (s32 i = 0; i < 16; i++) {
        if (state->unk_116C0[i].unk_44 == 0) {
            return i;
        }
    }
    return -1;
}

SpriteFrameInfo* func_ov038_02083c94(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info      = NULL;
    s32*             ownerData = sprite->owner;

    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER:
            if (*ownerData != 0x1000) {
                sprite->unk_0A.unk_00 = 1;
                sprite->unk_0A.unk_01 = 1;
                sprite->unk_0A.unk_05 = OamMgr_AllocAffineGroup(&g_OamMgr[sprite->bits_0_1], 0, *ownerData, *ownerData, 0);
            } else {
                sprite->unk_0A.unk_00 = 0;
                sprite->unk_0A.unk_01 = 0;
                sprite->unk_0A.unk_05 = 0;
            }
            info                         = &g_SpriteFrameInfo;
            g_SpriteFrameInfo.pieceCount = 0;
            g_SpriteFrameInfo.cellPieces = NULL;
            g_SpriteFrameInfo.affine     = NULL;
            g_SpriteFrameInfo.sortKey    = -1;
            if ((sprite->animData != NULL) && (sprite->cellTable != NULL)) {
                if (sprite->cellIndex >= 0) {
                    g_SpriteFrameInfo.pieceCount = *((u16*)sprite->cellTable + (sprite->cellIndex * 4 + 1));
                    g_SpriteFrameInfo.cellPieces =
                        (OamCellPiece*)((u16*)sprite->cellTable + sprite->cellTable[sprite->cellIndex].pieceOffset);
                }
            }
            /* fallthrough */
        default:
            return info;
    }
}

extern const BinIdentifier data_ov038_02085c6c[185];

const BinIdentifier* func_ov038_02083de0(const char* name) {
    for (u32 i = 0; i < 0xB9; i++) {
        if (strcmp(data_ov038_02085c6c[i].path, name) == 0) {
            return &data_ov038_02085c6c[i];
        }
    }
    return NULL;
}

typedef struct {
    /* 0x0 */ u16 unk_0;
    /* 0x2 */ u16 unk_2;
    /* 0x4 */ u16 unk_4;
    /* 0x6 */ u16 unk_6;
} GrpCheckUnkSmall;

extern const GrpCheckUnkSmall data_ov038_02085b50;
extern const SpriteAnimation  data_ov038_02085bb8;

void func_ov038_02083e28(GrpCheckState* state, u16 engine, void* arg2) {
    GrpCheckBigUnk*     entry = arg2;
    SpriteAnimation     anim  = data_ov038_02085bb8;
    const SpriteConfig* config;

    anim.bits_0_1 = engine;
    anim.owner    = &entry->unk_48;
    anim.posX     = data_ov038_02085b50.unk_4;
    anim.posY     = data_ov038_02085b50.unk_6;
    anim.dataType = state->unk_00004;

    config         = &data_ov038_020ac980.unk_04[entry->unk_40];
    anim.binIden   = func_ov038_02083de0(config->unk_10);
    anim.unk_18    = config->unk_20;
    anim.packIndex = config->unk_22;
    anim.bits_7_9  = config->unk_14;
    anim.unk_1C    = config->unk_16;
    anim.unk_20    = config->unk_18;
    anim.unk_26    = config->unk_1A;
    anim.unk_28    = config->unk_1C;
    anim.unk_22    = config->unk_1E;
    if (func_ov038_020832a8(state, entry->unk_40) != 0) {
        anim.animIndex = config->unk_2C;
    }
    if (_Sprite_Load(&entry->sprite, &anim) != 0) {
        entry->unk_44             = 1;
        state->isVramLimitReached = 0;
        return;
    }
    entry->unk_44             = 0;
    state->isVramLimitReached = 1;
}

void func_ov038_02083ff8(GrpCheckState* state) {
    state->maxChr     = 0;
    state->maxAtr     = 0;
    state->maxLineChr = 0;
    state->unk_116B4  = state->unk_11B8C;
    state->unk_116BC  = 0x1000;
    if (data_ov038_020b7200.unk_4 != 2) {
        func_ov038_02083e28(state, 1, &state->unk_11674);
    }
    GrpCheck_PrintScale(state);
}

void func_ov038_02084054(GrpCheckState* state) {
    s32 index = func_ov038_02083c64(state);

    state->unk_11B80 = index;
    if (index < 0) {
        state->isVramLimitReached = TRUE;
        return;
    }
    GrpCheckBigUnk* entry = &state->unk_116C0[index];

    entry->unk_40 = state->unk_11B8C;
    entry->unk_48 = 0x1000;

    if (data_ov038_020b7200.unk_4 == 2) {
        if (data_ov038_020ac980.unk_04[state->unk_11B8C].unk_0C == 2) {
            func_ov038_02083e28(state, 2, entry);
            return;
        }
    }
    if (data_ov038_020ac980.unk_04[state->unk_11B8C].unk_0C == 2) {
        return;
    }
    func_ov038_02083e28(state, 0, entry);
}

void GrpCheck_DestroySprite(GrpCheckState* state, s32 index) {
    Sprite_Destroy(&state->unk_116C0[index].sprite);
    state->unk_116C0[index].unk_44 = 0;
}

void GrpCheck_DestroyAllSprites(GrpCheckState* state) {
    if (state->hasSprite) {
        Sprite_Destroy(&state->unk_11674);
    }

    for (s32 i = 0; i < 16; i++) {
        if (state->unk_116C0[i].unk_44 != 0) {
            GrpCheck_DestroySprite(state, i);
        }
    }
}

void GrpCheck_Delete(GrpCheckState* state) {
    s32 i = 0;

    state->isVramLimitReached = 0;
    if (state->unk_11B9C != 0) {
        s32 index = state->unk_11B88;

        if (index < 0) {
            return;
        }
        if (index == state->unk_11B80) {
            return;
        }
        GrpCheck_DestroySprite(state, index);
        return;
    }
    for (i = 0; i < 16; i++) {
        if ((i != state->unk_11B80) && (state->unk_116C0[i].unk_44 != 0)) {
            GrpCheck_DestroySprite(state, i);
        }
    }
}

extern const GrpCheckBgParams data_ov038_02085be4;

void func_ov038_0208424c(GrpCheckState* state) {
    GrpCheckBgParams   params = data_ov038_02085be4;
    const GrpCheckUnk* entry  = &data_ov038_02086fe4[state->unk_1165C];

    params.unk_14 = state->unk_00000;
    params.unk_18 = (void*)&entry->unk_04;
    params.unk_1C = *(s16*)((u8*)entry + 0xC);
    params.unk_20 = *(s16*)((u8*)entry + 0xE);
    params.unk_24 = *(s16*)((u8*)entry + 0x10);
    params.unk_26 = *(s16*)((u8*)entry + 0x12);
    params.unk_28 = *(s16*)((u8*)entry + 0x14);
    params.unk_2A = *(s16*)((u8*)entry + 0x16);
    params.unk_2C = *(s16*)((u8*)entry + 0x18);
    params.unk_2E = *(s16*)((u8*)entry + 0x1A);
    params.unk_30 = *(s16*)((u8*)entry + 0x1C);
    func_ov038_0208287c(&state->unk_115A0, &params);
    GrpCheck_PrintScreenPos(state);
}

void func_ov038_020843a8(GrpCheckState* state) {
    func_ov038_02082e44(&state->unk_115A0);
}

void func_ov038_020843bc(GrpCheckState* state) {
    u32 value = state->unk_1165C + 1;

    state->unk_1165C = value;
    if (value >= 0x126) {
        state->unk_1165C = 0;
    }
    func_ov038_020843a8(state);
    func_ov038_0208424c(state);
}

void func_ov038_020843fc(GrpCheckState* state) {
    s32 value = state->unk_1165C - 1;

    state->unk_1165C = value;
    if (value < 0) {
        state->unk_1165C = 0x125;
    }
    func_ov038_020843a8(state);
    func_ov038_0208424c(state);
}

void func_ov038_02084434(GrpCheckState* state) {
    s32 offsetX = -state->screenPosX;
    s32 offsetY = -state->screenPosY;

    if (state->unk_115A0.params.unk_2E != 0) {
        offsetX += 0x80000;
    }
    if (state->unk_115A0.params.unk_2C == 0) {
        offsetY += 0x40000;
    } else {
        offsetY += 0x140000;
    }
    func_ov038_02082fc4(&state->unk_115A0, offsetX, offsetY);
    func_ov038_02082ed0(&state->unk_115A0);
}

void func_ov038_02084498(GrpCheckState* state) {
    state->unk_1165C = func_ov038_02083010(state);
    func_ov038_020843a8(state);
    func_ov038_0208424c(state);
}

void func_ov038_020844c0(GrpCheckState* state) {
    state->unk_1165C = func_ov038_0208307c(state);
    func_ov038_020843a8(state);
    func_ov038_0208424c(state);
}

s32 func_ov038_020844e8(GrpCheckState* state, s16 arg1, s16 arg2) {
    for (s32 i = 0; i < 16; i++) {
        if (state->unk_116C0[i].unk_44 != 0) {
            if (func_ov038_020825c8(&state->unk_116C0[i], arg1, arg2) != 0) {
                return i;
            }
        }
    }
    return -1;
}

void func_02040100(void);
s32  func_020401e0(u16* rawInput);

void func_ov038_0208454c(GrpCheckState* state) {
    u16 rawInput[4];
    s16 deltaX;
    s16 deltaY;

    do {
        func_02040100();
    } while (func_020401e0(rawInput) != 0);

    if (rawInput[2] != 0) {
        if (state->unk_11B9C == 0) {
            state->unk_11B9C = 1;
            state->unk_11B88 = func_ov038_020844e8(state, rawInput[0], rawInput[1]);
            state->unk_11BA0 = rawInput[0];
            state->unk_11BA2 = rawInput[1];
        }
        if (state->unk_11B88 >= 0) {
            state->unk_116C0[state->unk_11B88].sprite.posX = rawInput[0];
            state->unk_116C0[state->unk_11B88].sprite.posY = rawInput[1] + 0x10;
        } else {
            f32 var;
            s32 fixed;

            deltaX = rawInput[0] - state->unk_11BA0;
            deltaY = rawInput[1] - state->unk_11BA2;

            fixed = deltaX << 12;
            if (deltaX > 0) {
                var = 0.5f + (f32)fixed;
            } else {
                var = (f32)fixed - 0.5f;
            }
            state->screenPosX += (s32)var;

            fixed = deltaY << 12;
            if (deltaY > 0) {
                var = 0.5f + (f32)fixed;
            } else {
                var = (f32)fixed - 0.5f;
            }
            state->screenPosY += (s32)var;

            GrpCheck_PrintScreenPos(state);
        }
        state->unk_11BA0 = rawInput[0];
        state->unk_11BA2 = rawInput[1];
        return;
    }
    state->unk_11B9C = 0;
}

s32 func_ov038_020846f4(GrpCheckState* state) {
    s32 index = state->unk_11B80;

    if (index < 0) {
        return 0;
    }
    if (index >= 16) {
        return 0;
    }
    if (state->unk_116C0[index].unk_44 != 0) {
        return 1;
    }
    return 0;
}

s32 func_ov038_02084734(GrpCheckState* state) {
    GrpCheckBigUnk* entry = &state->unk_116C0[state->unk_11B80];

    if ((entry->sprite.posX == 0x80) && (entry->sprite.posY == 0x60)) {
        return 0;
    }
    return 1;
}

void func_ov038_0208476c(GrpCheckState* state) {
    GrpCheckBigUnk*     entry  = &state->unk_116C0[state->unk_11B80];
    const SpriteConfig* config = &data_ov038_020ac980.unk_04[state->unk_11B8C];

    if (func_02048210(entry->sprite.resourceData->binIden->path, config->unk_10) != 0) {
        if (state->hasSprite) {
            Sprite_Destroy(&state->unk_11674);
        }
        if ((func_ov038_020846f4(state) != 0) && (func_ov038_02084734(state) == 0)) {
            GrpCheck_DestroySprite(state, state->unk_11B80);
        }
        func_ov038_02083ff8(state);
        func_ov038_02084054(state);
        return;
    }
    if (state->hasSprite) {
        Sprite_Destroy(&state->unk_11674);
    }
    func_ov038_02083ff8(state);
    if ((func_ov038_020846f4(state) != 0) && (func_ov038_02084734(state) == 0)) {
        GrpCheck_DestroySprite(state, state->unk_11B80);
    }
    func_ov038_02084054(state);
}

void func_ov038_02084874(GrpCheckState* state) {
    if (state->hasSprite) {
        Sprite_ChangeAnimation(&state->unk_11674, state->unk_11674.animData, state->unk_11B90, state->unk_11674.cellTable);
    }

    if (func_ov038_020846f4(state) != 0) {
        Sprite* sprite = &state->unk_116C0[state->unk_11B80].sprite;
        Sprite_ChangeAnimation(sprite, sprite->animData, state->unk_11B90, sprite->cellTable);
    }

    GrpCheck_PrintSpritePath(state);
    state->updateForced = TRUE;
    state->maxChr       = 0;
    state->maxAtr       = 0;
    state->maxLineChr   = 0;
}

void func_ov038_02084910(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_0208312c(state, state->currentRow);
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_02084968(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_020831a4(state, state->currentRow);
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_020849c0(GrpCheckState* state) {
    if (func_ov038_020832a8(state, state->unk_11B8C) != 0) {
        state->unk_11B8C = func_ov038_020832d0(state->unk_11B8C);
    }
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_02084a2c(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_02083288(state);

    if (func_ov038_020832a8(state, state->unk_11B8C)) {
        func_ov038_020849c0(state);
    } else {
        func_ov038_0208476c(state);
        state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animData[0] - 1;
        func_ov038_02084874(state);
    }
}

void func_ov038_02084a9c(GrpCheckState* state) {
    while (data_ov038_020ac980.unk_04[state->unk_11B8C].unk_28 != 3) {
        state->unk_11B8C = func_ov038_02083260(state);
    }

    state->unk_11B8C = func_ov038_02083260(state);
    func_ov038_0208476c(state);
    state->unk_11B90 = state->unk_116C0[state->unk_11B80].sprite.animIndex;
    func_ov038_02084874(state);
}

void func_ov038_02084b3c(GrpCheckState* state) {
    state->unk_11B8C = func_ov038_020832d0(state->unk_11B8C);
    func_ov038_02084a2c(state);
}

void func_ov038_02084b64(GrpCheckState* state) {
    s16 maxFrame = state->unk_116C0[state->unk_11B80].sprite.animData[0];

    state->unk_11B90++;
    if (state->unk_11B90 >= maxFrame) {
        state->unk_11B8C = func_ov038_02083260(state);
        func_ov038_020849c0(state);
        return;
    }

    if ((func_ov038_020846f4(state) != 0) && (func_ov038_02084734(state) != 0)) {
        func_ov038_02084054(state);
    }
    func_ov038_02084874(state);
}

void func_ov038_02084bf0(GrpCheckState* state) {
    state->unk_11B90--;
    if (state->unk_11B90 < 1) {
        func_ov038_02084a2c(state);
        return;
    }
    if ((func_ov038_020846f4(state) != 0) && (func_ov038_02084734(state) != 0)) {
        func_ov038_02084054(state);
    }
    func_ov038_02084874(state);
}

void GrpCheck_IncreaseValue(GrpCheckState* state) {
    if (func_ov038_020832a8(state, state->unk_11B8C) != 0) {
        func_ov038_02084a9c(state);
    } else {
        func_ov038_02084b64(state);
    }
}

void GrpCheck_DecreaseValue(GrpCheckState* state) {
    if (func_ov038_020832a8(state, state->unk_11B8C) != 0) {
        func_ov038_02084b3c(state);
    } else {
        func_ov038_02084bf0(state);
    }
}

void func_ov038_02084cb0(GrpCheckState* state, Sprite* sprite) {
    GrpCheckBigUnk* entry = (GrpCheckBigUnk*)sprite;
    s16             pos[2];

    Sprite_UpdateAndCheck(sprite);
    if (func_ov038_020832a8(state, entry->unk_40) == 0) {
        return;
    }
    if (sprite->isPlaying != 1) {
        return;
    }
    u32 engine    = sprite->bits_0_1;
    pos[0]        = sprite->posX;
    pos[1]        = sprite->posY;
    entry->unk_40 = func_ov038_02083300(entry->unk_40, pos[1], pos[0]);
    Sprite_Destroy(sprite);
    func_ov038_02083e28(state, engine, sprite);
    sprite->posX = pos[0];
    sprite->posY = pos[1];
    Sprite_UpdateAndCheck(sprite);
}

void GrpCheck_RenderAllSprites(GrpCheckState* state) {
    for (s32 i = 0; i < 16; i++) {
        if (state->unk_116C0[i].unk_44 != 0) {
            if ((state->isPaused == FALSE) || (state->updateForced)) {
                func_ov038_02084cb0(state, &state->unk_116C0[i].sprite);
            }
            if ((i == state->unk_11B80) && (state->isAnimDisplayed == FALSE)) {

            } else {
                Sprite_Render(&state->unk_116C0[i].sprite);
            }
        }
    }

    if (state->hasSprite) {
        if ((state->isPaused == FALSE) || (state->updateForced)) {
            func_ov038_02084cb0(state, &state->unk_11674);
            state->unk_11B8C = state->unk_116B4;
            GrpCheck_PrintSpritePath(state);
        }
        if (state->isAnimDisplayed) {
            Sprite_Render(&state->unk_11674);
        }
    }
}

void func_ov038_02084e48(GrpCheckState* state) {
    if (state->isPaused) {
        state->updateForced = TRUE;
    } else {
        func_ov038_020849c0(state);
    }
}

void GrpCheck_ToggleAnimDisplay(GrpCheckState* state) {
    state->isAnimDisplayed = !state->isAnimDisplayed;
}

void GrpCheck_TogglePause(GrpCheckState* state) {
    state->isPaused = !state->isPaused;
}

void GrpCheck_Exit(GrpCheckState* state) {
    MainOvlDisp_NextProcessStage();
}

void GrpCheck_MoveCursorUp(GrpCheckState* state) {
    state->currentRow--;
    if (state->currentRow < 0) {
        state->currentRow = 21;
    }

    if ((state->currentRow > 2) && (state->currentRow < 16)) {
        state->currentRow = 2;
    }
    GrpCheck_PrintRowIndicator(state);
}

void GrpCheck_MoveCursorDown(GrpCheckState* state) {
    state->currentRow++;
    if (state->currentRow > 21) {
        state->currentRow = 0;
    }

    if ((state->currentRow > 2) && (state->currentRow < 16)) {
        state->currentRow = 16;
    }
    GrpCheck_PrintRowIndicator(state);
}

void GrpCheck_IncreaseScale(GrpCheckState* state) {
    state->unk_116BC += 41;
    state->unk_116C0[state->unk_11B80].unk_48 += 41;

    if (state->unk_116BC >= 0x2000) {
        state->unk_116BC                          = 0x2000;
        state->unk_116C0[state->unk_11B80].unk_48 = 0x2000;
    }
    GrpCheck_PrintScale(state);
}

void GrpCheck_DecreaseScale(GrpCheckState* state) {
    state->unk_116BC -= 41;
    state->unk_116C0[state->unk_11B80].unk_48 -= 41;

    if (state->unk_116BC <= 0) {
        state->unk_116BC                          = 41;
        state->unk_116C0[state->unk_11B80].unk_48 = 41;
    }
    GrpCheck_PrintScale(state);
}

void GrpCheck_AdjustBGColorChannel(GrpCheckState* state, RGBAChannel channel, s32 delta) {
    s32* value;

    switch (channel) {
        case RGB_RED:
            value = &state->bgR;
            break;
        case RGB_GREEN:
            value = &state->bgG;
            break;
        case RGB_BLUE:
            value = &state->bgB;
            break;
        default:
            return;
    }
    *value = (*value + delta) & 0x1F;

    data_02066eec = Color_PackRGB(state->bgR, state->bgG, state->bgB, state->bgA);
    GrpCheck_PrintColorChannels(state);
}

void GrpCheck_IncreaseBGRed(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_RED, 1);
}

void GrpCheck_DecreaseBGRed(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_RED, -1);
}

void GrpCheck_IncreaseBGGreen(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_GREEN, 1);
}

void GrpCheck_DecreaseBGGreen(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_GREEN, -1);
}

void GrpCheck_IncreaseBGBlue(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_BLUE, 1);
}

void GrpCheck_DecreaseBGBlue(GrpCheckState* state) {
    GrpCheck_AdjustBGColorChannel(state, RGB_BLUE, -1);
}

typedef struct {
    /* 0x0 */ u32 menuOption;
    /* 0x4 */ void (*decCallback)(GrpCheckState*);
    /* 0x8 */ void (*incCallback)(GrpCheckState*);
} GrpCheckScalarCallback;

typedef struct {
    /* 0x0 */ u16 button;
    /* 0x4 */ void (*callback)(GrpCheckState*);
} GrpCheckInputCallback;

const GrpCheckScalarCallback data_ov038_02085c24[6] = {
    {16,   GrpCheck_DecreaseScale,   GrpCheck_IncreaseScale},
    {17,   GrpCheck_DecreaseBGRed,   GrpCheck_IncreaseBGRed},
    {18, GrpCheck_DecreaseBGGreen, GrpCheck_IncreaseBGGreen},
    {19,  GrpCheck_DecreaseBGBlue,  GrpCheck_IncreaseBGBlue},
    {20,      func_ov038_020844c0,      func_ov038_02084498},
    {21,      func_ov038_020843fc,      func_ov038_020843bc},
};

const GrpCheckInputCallback data_ov038_02085b70[4] = {
    {     INPUT_BUTTON_B, GrpCheck_ToggleAnimDisplay},
    {     INPUT_BUTTON_X,            GrpCheck_Delete},
    { INPUT_BUTTON_START,       GrpCheck_TogglePause},
    {INPUT_BUTTON_SELECT,              GrpCheck_Exit},
};

const GrpCheckInputCallback data_ov038_02085b90[5] = {
    {   INPUT_BUTTON_UP,   GrpCheck_MoveCursorUp},
    { INPUT_BUTTON_DOWN, GrpCheck_MoveCursorDown},
    { INPUT_BUTTON_LEFT,  GrpCheck_DecreaseValue},
    {INPUT_BUTTON_RIGHT,  GrpCheck_IncreaseValue},
    {    INPUT_BUTTON_A,     func_ov038_02084e48},
};

void GrpCheck_ProcessUserInput(GrpCheckState* state) {
    u32 i;

    func_ov038_0208454c(state);

    for (i = 0; i < 5; i++) {
        if (SysControl.buttonState.holdButtons & data_ov038_02085b90[i].button) {
            data_ov038_02085b90[i].callback(state);
        }
    }

    for (i = 0; i < 4; i++) {
        if (SysControl.buttonState.pressedButtons & data_ov038_02085b70[i].button) {
            data_ov038_02085b70[i].callback(state);
        }
    }

    for (i = 0; i < 6; i++) {
        GrpCheckScalarCallback* scalarCb = &data_ov038_02085c24[i];
        if (state->currentRow == scalarCb->menuOption) {
            if (SysControl.buttonState.holdButtons == INPUT_BUTTON_L) {
                scalarCb->decCallback(state);
            }
            if (SysControl.buttonState.holdButtons == INPUT_BUTTON_R) {
                scalarCb->incCallback(state);
            }
        }
    }

    if (state->currentRow >= 0 && (u32)state->currentRow < 3) {
        if (SysControl.buttonState.holdButtons == INPUT_BUTTON_L) {
            func_ov038_02084968(state);
        }
        if (SysControl.buttonState.holdButtons == INPUT_BUTTON_R) {
            func_ov038_02084910(state);
        }
    }
    func_ov038_02084434(state);
    GrpCheck_RenderAllSprites(state);

    // Reset manual animation advance flag after processing input, so that it only takes effect for one frame
    state->updateForced = FALSE;

    GrpCheck_PrintSpriteDetails(state);
    GrpCheck_PrintWarnings(state);
}

extern const SpriteConfig data_ov038_0208993c[];
extern const SpriteConfig data_ov038_02086234[];

void func_ov038_02085268(GrpCheckState* state) {
    s32 i = 0;

    state->unk_11B8C                               = 0;
    state->unk_11B90                               = 1;
    state->unk_11B80                               = 0;
    state->unk_116C0[0].sprite.posX                = 0x80;
    state->unk_116C0[state->unk_11B80].sprite.posY = 0x60;
    for (i = 0; i < 16; i++) {
        state->unk_116C0[i].unk_44 = 0;
    }
    state->unk_1165C          = 0;
    state->isAnimDisplayed    = 1;
    state->unk_11B88          = -1;
    state->currentRow         = 0;
    state->isPaused           = 0;
    state->unk_11B9C          = 0;
    state->isVramLimitReached = 0;
    state->bgR                = 8;
    state->bgG                = 0x10;
    state->bgB                = 0x10;
    state->bgA                = 0;
    state->screenPosX         = 0;
    state->screenPosY         = 0;

    if (data_ov038_020b7200.unk_4 != 2) {
        data_ov038_020ac980.unk_04 = data_ov038_0208993c;
        data_ov038_020b7200.unk_0  = 0xBAC;
        return;
    }
    data_ov038_020ac980.unk_04 = data_ov038_02086234;
    data_ov038_020b7200.unk_0  = 0x49;
}

void GrpCheck_InitOverlay(GrpCheckState* state) {
    if (state == NULL) {
        const char* sequence = data_ov038_020ac980.sequence;

        state = Mem_AllocHeapTail(&gDebugHeap, sizeof(GrpCheckState));
        Mem_SetSequence(&gDebugHeap, state, sequence);
        MainOvlDisp_SetCbArg(state);
    }
    MI_CpuFill(0, state, Mem_GetBlockSize(&gDebugHeap, state));
    GrpCheck_RegisterVBlank();

    u16 stack[4];
    func_0203ff44(&stack);
    func_0203ffd8(&stack);

    func_0200d8f0();
    state->unk_00000 = DatMgr_AllocateSlot();
    state->unk_00004 = DatMgr_AllocateSlot();
    state->unk_1158C = ResourceMgr_ReinitManagers(&state->unk_0000C);
    func_ov038_02085268(state);
    func_ov038_02083340(state);
    func_ov038_0208424c(state);
    func_ov038_02083ff8(state);
    func_ov038_02084054(state);
    GrpCheck_PrintRowIndicator(state);
    GrpCheck_PrintSpritePath(state);
    GrpCheck_PrintSpriteDetails(state);
    GrpCheck_PrintScale(state);
    GrpCheck_PrintColorChannels(state);
    GrpCheck_PrintScreenPos(state);
    GrpCheck_PrintControls(state);
    data_02066aec = 0;
    data_02066eec = Color_PackRGB(state->bgR, state->bgG, state->bgB, state->bgA);
    DebugOvlDisp_Init();
    MainOvlDisp_NextProcessStage();
}

void GrpCheck_InitOverlayForHwSprites(GrpCheckState* state) {
    data_ov038_020b7200.unk_4 = 1;
    GrpCheck_InitOverlay(state);
}

void GrpCheck_InitOverlayForSwSprites(GrpCheckState* state) {
    data_ov038_020b7200.unk_4 = 2;
    GrpCheck_InitOverlay(state);
}

void GrpCheck_RunOverlay(GrpCheckState* state) {
    OamMgr_Reset(&g_OamMgr[DISPLAY_MAIN], 0, 0);
    OamMgr_Reset(&g_OamMgr[DISPLAY_SUB], 0, 0);
    if (data_ov038_020b7200.unk_4 == 2) {
        OamMgr_Reset3DState();
        OamMgr_SetAffineCount(&g_OamMgr[DISPLAY_EXTENDED], 0);
    }
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    GrpCheck_ProcessUserInput(state);

    if (data_ov038_02086fe4[state->unk_1165C].unk_18 != 0) {
        func_0200d90c();
    }

    DebugOvlDisp_Run();
    OamMgr_Swap3DBuffers();
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_FlushCommands(&g_OamMgr[DISPLAY_SUB]);
    if (data_ov038_020b7200.unk_4 == 2) {
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_EXTENDED], NULL);
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], NULL);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], NULL);
}

void GrpCheck_TerminateOverlay(GrpCheckState* state) {
    OverlayTag tag;

    GrpCheck_ClearVBlank();
    func_ov038_020833a4(state);
    GrpCheck_DestroyAllSprites(state);
    ResourceMgr_SetActiveManagers(state->unk_1158C);
    DatMgr_ClearSlot(state->unk_00004);
    DatMgr_ClearSlot(state->unk_00000);
    DatMgr_ReleaseData(state->unk_00008);
    Mem_Free(&gDebugHeap, state);
    MainOvlDisp_SetProcessStage(PROCESS_STAGE_EXIT);
    MainOvlDisp_Pop(&tag);
}

static const OverlayProcess OvlProc_GrpCheck_HwSprites = {
    .init = GrpCheck_InitOverlayForHwSprites,
    .main = GrpCheck_RunOverlay,
    .exit = GrpCheck_TerminateOverlay,
};

static const OverlayProcess OvlProc_GrpCheck_SwSprites = {
    .init = GrpCheck_InitOverlayForSwSprites,
    .main = GrpCheck_RunOverlay,
    .exit = GrpCheck_TerminateOverlay,
};

void ProcessOverlay_GrpCheck_HwSprites(void* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        GrpCheck_TerminateOverlay(state);
    } else {
        OvlProc_GrpCheck_HwSprites.funcs[stage](state);
    }
}

void ProcessOverlay_GrpCheck_SwSprites(void* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        GrpCheck_TerminateOverlay(state);
    } else {
        OvlProc_GrpCheck_SwSprites.funcs[stage](state);
    }
}

void func_ov038_020856f4(void) {
    Interrupts_Init();
    HBlank_Init();
    DMA_Init(0x100);
    GX_Init();
    G3X_Init();
    G3X_InitMtxStack();
    GX_DispOn();
    REG_DISPCNT_SUB |= 0x10000;
    Display_Init();
    GX_DisableBankForLcdc();
    GX_SetBankForLcdc(0x1FF);
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForBgExtPltt(0x20);
    GX_SetBankForObjExtPltt(0x40);
    GX_SetBankForSubBg(4);
    if (data_ov038_020b7200.unk_4 == 2) {
        GX_SetBankForTex(8);
        GX_SetBankForTexPltt(0x10);
    } else {
        GX_SetBankForSubObj(8);
        GX_SetBankForSubObjExtPltt(0x100);
    }
    OamMgr_Init3DSpritePipeline();
    REG_POWER_CNT &= ~0x8000;

    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);

    // DisplayBGSettings* mainBg1 = Display_GetBG1Settings(DISPLAY_MAIN);
    // mainBg1->bgMode            = 0;
    // mainBg1->screenSizeText    = 1;
    // mainBg1->colorMode         = 0;
    // mainBg1->screenBase        = 0;
    // mainBg1->charBase          = 1;
    // mainBg1->extPlttSlot       = 0;
    // REG_BG1CNT                 = REG_BG1CNT & 0x43 | 0x4004;

    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4004);

    DisplayBGSettings* mainBg2 = Display_GetBG2Settings(DISPLAY_MAIN);
    mainBg2->bgMode            = 0;
    mainBg2->screenSizeText    = 1;
    mainBg2->colorMode         = 0;
    mainBg2->screenBase        = 2;
    mainBg2->charBase          = 3;
    mainBg2->extPlttSlot       = 1;
    REG_BG2CNT                 = REG_BG2CNT & 0x43 | 0x420c;

    g_DisplaySettings.engineState[0].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;

    g_DisplaySettings.engineState[0].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[0].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    Display_SetMainLayers(LAYER_BG0 | LAYER_BG1 | LAYER_BG2 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_SUB].bgMode = GX_BGMODE_0;

    GXs_SetGraphicsMode(0);

    Display_InitSubBG0(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 0, 1, 0, 0x4);

    g_DisplaySettings.engineState[1].bgSettings[0].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 1;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 1;

    g_DisplaySettings.engineState[1].bgSettings[0].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].mosaic = 0;
    g_DisplaySettings.engineState[1].bgSettings[3].mosaic = 0;

    g_DisplaySettings.controls[DISPLAY_SUB].objTileMode = GX_OBJTILEMODE_1D_128K;
    Display_SetSubLayers(LAYER_BG0 | LAYER_OBJ);
    g_DisplaySettings.controls[DISPLAY_MAIN].brightness = 0;
    g_DisplaySettings.controls[DISPLAY_SUB].brightness  = 0;

    MI_CpuFill(0, 0x06800000, 0xA4000);
    MI_CpuFill(0, 0x06000000, 0x80000);
    MI_CpuFill(0, 0x06200000, 0x20000);
    MI_CpuFill(0, 0x06400000, 0x40000);
    MI_CpuFill(0, 0x06600000, 0x20000);
    OamMgr_InitExtended();
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_MAIN]);
    OamMgr_ResetCommandQueues(&g_OamMgr[DISPLAY_SUB]);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    Color_CopyRange(&data_02066eec, &data_0205a128, 0, 0x200, 1);
}

void GrpCheck_VBlank(void) {
    if (SystemStatusFlags.vblank) {
        Display_Commit();
        DMA_Flush();
        OamMgr_Commit();
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }
}

void GrpCheck_RegisterVBlank(void) {
    func_ov038_020856f4();
    Interrupts_RegisterVBlankCallback(GrpCheck_VBlank, 1);
}

void GrpCheck_ClearVBlank(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

#include "GrpCheckData.inc"
