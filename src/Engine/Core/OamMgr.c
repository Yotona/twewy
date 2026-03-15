#include "Engine/Core/OamMgr.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/System.h"
#include "common_data.h"
#include <NitroSDK/fx.h>
#include <NitroSDK/gx.h>
#include <NitroSDK/mi/cpumem.h>
#include <NitroSDK/os/cache.h>
#include <NitroSDK/os/interrupt.h>
#include <registers.h>
#include <types.h>

// Forward declarations for functions without headers
extern void G3X_SetClearColor(s32, s32, s32, s32, s32);
extern void G3i_OrthoW(s32, s32, s32, s32, s32, s32, s32, s32, s32);

// Forward declarations
OamCellShape* OamMgr_GetCellShape(OamCellPiece* piece);

// Static forward declarations
static void OamMgr_SetOamCount(OamManager* mgr, u32 arg1);

static void OamMgr_Reset3DState(void);

void func_020034cc(OamManager* mgr, u32 arg1, OamCellPiece* arg2);
void func_02003704(OamManager* mgr, u32 arg1, OamCellPiece* arg2);
void func_02003904(OamManager* mgr, u32 arg1, OamCellPiece* arg2);

void func_020035b4(OamManager* mgr);
void OamMgr_FlushPriorityLists(OamManager* mgr);
void func_02003a2c(OamManager* mgr);

// Extern data declarations
extern s32           data_02059384[];
extern s32           data_02059390[];
extern void*         data_0205939c;
extern void*         data_020593c0[];
extern OamCharConfig data_02059400[];

void (*data_02059a6c[3])(OamManager*, u32, OamCellPiece*) = {func_020034cc, func_02003704, func_02003904};
void (*data_02059a78[3])(OamManager*)                     = {func_020035b4, OamMgr_FlushPriorityLists, func_02003a2c};

extern u16          data_02059a84[];
extern Vec          data_02059a8c;
extern s32          data_02059a98[];
extern Vec          data_02059aa4;
extern VtxXY        data_02059ab0[4];
extern u8           data_02059ad4[];
extern TexCoordQuad data_02059b14[];
extern OamCellFrame data_02059b94[];
extern OamCellPiece data_02059d1c;
extern s16          data_0205e4e0[];

extern s32 data_0206a9bc;
extern u8  data_02069c24[];
extern s32 data_0206aa78;
extern s32 data_0206aa7c;

static void OamMgr_InitCharBase(s32 arg0, DisplayEngine engine, s32 arg2, s32 arg3) {
    OamCharConfig* charCfg;

    OamManager* mgr = (OamManager*)((u8*)&data_020676ec + engine * 0x108C);
    mgr->engine     = engine;
    switch (engine) {
        case 0:
        case 1:
            mgr->type = 7;
            break;
        case 2:
            mgr->type = 1;
            break;
    }
    charCfg           = &data_02059400[arg2];
    mgr->charBases[0] = charCfg;
    mgr->charBases[1] = charCfg;
    mgr->charBases[2] = charCfg;
    mgr->charBases[3] = &data_02059400[arg3];
}

void OamMgr_Init(s32 unused, DisplayEngine engine) {
    GXOBJTileMode tileMode;
    switch (engine) {
        case DISPLAY_MAIN:
        case DISPLAY_SUB:
            tileMode = g_DisplaySettings.controls[engine].objTileMode;
            break;
        case DISPLAY_EXTENDED:
            tileMode = data_0206aa78;
            break;
        default:
            tileMode = GX_OBJTILEMODE_1D_128K;
            break;
    }

    s32 var_r2;
    switch (tileMode) {
        case GX_OBJTILEMODE_1D_32K:
            var_r2 = 0;
            break;
        case GX_OBJTILEMODE_1D_64K:
            var_r2 = 1;
            break;
        case GX_OBJTILEMODE_1D_128K:
            var_r2 = 2;
            break;
        case GX_OBJTILEMODE_1D_256K:
            var_r2 = 3;
            break;
        default:
            var_r2 = 0;
            break;
    }
    GXOBJBmpMode bmpMode;
    switch (engine) {
        case DISPLAY_MAIN:
        case DISPLAY_SUB:
            bmpMode = g_DisplaySettings.controls[engine].objBmpMode;
            break;
        case DISPLAY_EXTENDED:
            bmpMode = data_0206aa7c;
            break;
        default:
            bmpMode = GX_OBJBMPMODE_1D_128K;
            break;
    }

    s32 var_r3;
    switch (bmpMode) {
        case GX_OBJBMPMODE_1D_128K:
            var_r3 = 2;
            break;
        case GX_OBJBMPMODE_1D_256K:
            var_r3 = 3;
            break;
        default:
            var_r3 = 2;
            break;
    }
    OamMgr_InitCharBase(unused, engine, var_r2, var_r3);
}

void OamMgr_Reset(OamManager* mgr, u32 initialOamCount, u32 initialAffineCount) {
    OamMgr_SetOamCount(mgr, initialOamCount);
    OamMgr_SetAffineCount(mgr, initialAffineCount);
}

static void OamMgr_SetOamCount(OamManager* mgr, u32 arg1) {
    mgr->oamCount = arg1;
    if (arg1 & ~0x7F) {
        return;
    }

    OamAttr* entry = &mgr->oam[arg1];

    while (arg1 < 128) {
        arg1 += 1;
        entry->attr0 = 0x1C0;
        entry++;
    }
}

void OamMgr_SetAffineCount(OamManager* mgr, u32 count) {
    mgr->affineCount = count;
    if (count & ~0x1F) {
        return;
    }

    OamAttr* var_r3 = &mgr->oam[count];
    while (count < 32) {
        var_r3[0].affineParam = 0x100;
        var_r3[1].affineParam = 0;
        var_r3[2].affineParam = 0;
        count += 1;
        var_r3[3].affineParam = 0x100;
        var_r3 += 4;
    }
}

static s32 OamMgr_AppendCellPieces(OamManager* mgr, s32 arg1, s32 arg2, s32 arg3, OamCellPiece* arg4, u16 arg5, s32 arg6) {
    OamCharConfig* charCfg = mgr->charBases[((s32)(arg4->attr0 >> 0xA) << 0x1E) >> 0x1C >> 2];
    u32            sp0     = ((u32)(arg5 << 0x1B) >> 0x13) << 0x10;
    OamAttr*       var_lr  = &mgr->oam[arg1];

    while (!(arg1 & ~0x7F) && ((u16)(arg4->charName + 1) != 0)) {
        arg1 += 1;
        var_lr->attr0 = (arg4->attr0 & 0xFF00);
        var_lr->attr0 = (var_lr->attr0 | (arg4->attr0 + arg3));
        var_lr->attr0 = (var_lr->attr0 | (sp0 >> 0x10));
        var_lr->attr1 = (arg4->attr1 & 0xFE00);
        var_lr->attr1 = (var_lr->attr1 | (u16)((arg4->attr1 + arg2) & 0x1FF));
        var_lr->attr1 = (var_lr->attr1 ^ (u16)((arg5 & 0x3E0) * 0x10));
        var_lr->attr2 = (arg4->attr2 & 0xFC00);
        var_lr->attr2 = (var_lr->attr2 | (u16)(0x3FF & ((s32)((arg4->charName + arg6) & 0x1FFF) >> charCfg->tileShift)));
        var_lr->attr2 = (var_lr->attr2 + (u16)(arg5 & 0xFC00));
        arg4++;
        var_lr++;
    }
    return arg1;
}

void OamMgr_AddCellToOam(OamManager* mgr, s32 x, s32 y, OamCellPiece* cellPieces, u16 attrs, s32 charOffset) {
    if ((mgr->oamCount & ~0x7F) == 0) {
        mgr->oamCount = OamMgr_AppendCellPieces(mgr, mgr->oamCount, x, y, cellPieces, attrs, charOffset) + 1;
    }
}

void OamMgr_CopyCellPiecesToOam(OamManager* mgr, OamCellPiece* cellPieces) {
    if (mgr->oamCount & ~0x7F || cellPieces == NULL) {
        return;
    }

    OamAttr* entry = &mgr->oam[mgr->oamCount];

    while ((mgr->oamCount & ~0x7F) == 0 && (u16)(cellPieces->charName + 1) != 0) {
        entry->attr0 = cellPieces->attr0;
        entry->attr1 = cellPieces->attr1;
        entry->attr2 = (u16)((cellPieces->attr2 & 0xFC00) + (cellPieces->charName & 0x3FF));

        cellPieces++;
        entry++;
        mgr->oamCount++;
    }
}

static void OamMgr_SetAffineGroupParams(OamManager* mgr, s32 arg1, u16 arg2, s32 arg3, s32 arg4, s32 arg5) {
    if (arg1 & ~0x1F) {
        return;
    }
    if (arg5 & 1) {
        arg3 = -arg3;
    }

    s32 temp_r6 = ((s32)arg2 >> 4) * 2;

    OamAffineParam* affine = &mgr->affine[arg1];
    affine->rotation       = arg2;
    affine->scaleX         = arg3;
    if (arg5 & 2) {
        arg4 = -arg4;
    }
    s16 temp_r5    = data_0205e4e0[temp_r6];
    s32 temp_r6_2  = data_0205e4e0[temp_r6 + 1] << 0xC;
    affine->scaleY = arg4;
    affine->unk_0C = 0;

    *(s16*)((u8*)affine + 0xE - 0xC) = 0;

    OamAttr* groupBase       = &mgr->oam[arg1];
    groupBase[0].affineParam = (temp_r6_2 / arg3) >> 4;
    groupBase[1].affineParam = ((temp_r5 << 0xC) / arg3) >> 4;
    groupBase[2].affineParam = ((-temp_r5 << 0xC) / arg4) >> 4;
    groupBase[3].affineParam = (temp_r6_2 / arg4) >> 4;
}

s32 OamMgr_AllocAffineGroup(OamManager* mgr, u16 rotation, s32 scaleX, s32 scaleY, s32 flipFlags) {
    s32 temp_r1;

    temp_r1 = mgr->affineCount;
    if (temp_r1 & ~0x1F) {
        return 0;
    }
    OamMgr_SetAffineGroupParams(mgr, temp_r1, rotation, scaleX, scaleY, flipFlags);
    s32 temp = mgr->affineCount;
    mgr->affineCount++;
    return temp;
}

OamCellShape* OamMgr_GetCellShape(OamCellPiece* piece) {
    return (OamCellShape*)((u8*)data_020593c0[((s32)(u16)(piece->attr0 * 4) >> 0xC)] +
                           ((((s32)piece->attr1 >> 0xE) | (((s32)piece->attr0 >> 0xE) * 4)) << 5));
}

OamCellPiece* OamMgr_BuildVisibleCellPieces(OamManager* mgr, s32 x, s32 y, OamCellPiece* cellPieces, u16 attrs,
                                            s32 charOffset) {
    if (attrs & 1) {
        return OamMgr_BuildVisibleCellPiecesAffine(mgr, x, y, cellPieces, attrs, charOffset, NULL);
    }

    OamCellPiece* temp_r0 = func_02004c8c(&data_0206a9bc, 0x408);
    if (temp_r0 == NULL) {
        return NULL;
    }

    OamCellPiece* var_r4 = temp_r0;

    u32            sp1C    = ((attrs << 0x1B) >> 0x13) << 0x10;
    u32            sp18    = (attrs & 0xFC00) << 0x10;
    u16            temp_r7 = (attrs & 0x3E0) * 0x10;
    OamCharConfig* charCfg = mgr->charBases[(((s32)(cellPieces->attr0 >> 0xA) << 0x1E) >> 0x1C) >> 2];

    s32 var_r5 = 0;
    if ((u16)(cellPieces->charName + 1) != 0) {
        do {
            OamCellShape* cellShape = OamMgr_GetCellShape(cellPieces);
            s32           temp_r1   = cellPieces->attr1 & 0x1FF;
            s32           var_r2    = temp_r1 | data_02059384[((temp_r1 >> 7) & 2)];
            if (temp_r7 & 0x1000) {
                var_r2 = 0 - (var_r2 + cellShape->width);
            }
            s32 temp_r2 = var_r2 + x;
            s32 var_r3  = (u8)cellPieces->attr0 | data_02059384[((s32)(u8)cellPieces->attr0 >> 7)];
            if (temp_r7 & 0x2000) {
                var_r3 = 0 - (var_r3 + cellShape->height);
            }
            s32 temp_r1_3 = var_r3 + y;
            if ((temp_r2 > -cellShape->width) && (temp_r2 < 0x100) && (temp_r1_3 > -cellShape->height) && (temp_r1_3 < 0xC0)) {
                var_r4->charName = (u16)((s32)(charOffset + (cellPieces->charName & 0x1FFF)) >> charCfg->tileShift);
                var_r5 += 1;
                var_r4->attr0 = ((cellPieces->attr0 & 0xFF00) | (sp1C >> 0x10));
                var_r4->attr0 = (var_r4->attr0 | (u8)temp_r1_3);
                var_r4->attr1 = (temp_r7 ^ (cellPieces->attr1 & 0xFE00));
                var_r4->attr1 = (var_r4->attr1 | (u16)(temp_r2 & 0x1FF));
                var_r4->attr2 = ((cellPieces->attr2 & 0xFC00) + (sp18 >> 0x10));
                var_r4->attr2 = (var_r4->attr2 | (u16)((charOffset + (cellPieces->attr2 & 0x3FF)) >> charCfg->tileShift));
                var_r4++;
            }
            cellPieces++;
        } while ((u16)(cellPieces->charName + 1) != 0);
    }
    var_r4->charName = 0xFFFF;
    if (var_r5 <= 0) {
        temp_r0 = &data_02059d1c;
    } else {
        func_02004ce8(&data_0206a9bc, (var_r5 + 1) * 8);
    }
    return temp_r0;
}

OamCellPiece* OamMgr_BuildVisibleCellPiecesAffine(OamManager* mgr, s32 x, s32 y, OamCellPiece* cellPieces, u16 attrs,
                                                  s32 charOffset, void* transform) {
    s32 sp3C;
    s32 sp38;
    s32 sp34;
    s32 sp30;
    u32 sp24;
    u32 sp20;
    s32 sp1C;
    s32 sp14;
    s16 temp_ip;
    s32 temp_lr;
    s32 temp_r0_6;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r2_3;
    s32 temp_r3;

    if (((u32)(attrs << 0x1F) >> 0x1F) == 0) {
        return OamMgr_BuildVisibleCellPieces(mgr, attrs, charOffset, 0, 0, 0);
    }

    OamCellPiece* temp_r0 = func_02004c8c(&data_0206a9bc, 0x408);
    if (temp_r0 == NULL) {
        return NULL;
    }

    u32 sp2C = ((u32)(attrs << 0x1B) >> 0x13) << 0x10;
    u32 sp28 = (attrs & 0x3E0) << 0x14;

    void* var_r11 = transform;
    if (var_r11 == NULL) {
        var_r11 = &data_0205939c;
    }

    sp24                   = (attrs & 0xFC00) << 0x10;
    OamCellPiece*  var_r4  = temp_r0;
    OamCharConfig* charCfg = mgr->charBases[(((s32)(cellPieces->attr0 >> 0xA) << 0x1E) >> 0x1C) >> 2];
    sp14                   = 0;

    while ((u16)(cellPieces->charName + 1) != 0) {
        OamCellShape* cellShape = OamMgr_GetCellShape(cellPieces);
        s32           var_r8    = cellShape->width;
        s32           var_r9    = cellShape->height;
        temp_lr                 = *(s32*)((u8*)var_r11 + 8);
        temp_r3                 = cellPieces->attr1 & 0x1FF;
        s32 var_r3              = temp_r3 | data_02059390[((temp_r3 >> 8) * 2)];
        if (cellPieces->attr1 & 0x1000) {
            var_r3 = 0 - (var_r3 + var_r8);
        }
        s32 temp_r7   = var_r3 + (var_r8 >> 1);
        s32 temp_r3_2 = *(s32*)((u8*)var_r11 + 4);
        s32 var_r0    = (u8)cellPieces->attr0 | data_02059390[((s32)(u8)cellPieces->attr0 >> 7)];
        if (cellPieces->attr1 & 0x2000) {
            var_r0 = 0 - (var_r0 + var_r9);
        }
        s32 temp_r6 = var_r0 + (var_r9 >> 1);
        temp_r2     = ((s32)(u16)(*(u16*)var_r11) >> 4) * 2;
        temp_ip     = data_0205e4e0[temp_r2];
        sp1C        = (s32)data_0205e4e0[temp_r2 + 1];
        sp3C        = temp_r7 * (s32)(((s64)sp1C * temp_r3_2 + 0x800) >> 12);
        sp20        = (u32)(attrs << 0x1E) >> 0x1F;
        temp_r0_6   = y + ((s32)((temp_r7 * (s32)(((s64)temp_ip * temp_r3_2 + 0x800) >> 12)) +
                               (temp_r6 * (s32)(((s64)sp1C * temp_lr + 0x800) >> 12))) >>
                         0xC);
        temp_r2_2   = x + ((s32)(sp3C - (temp_r6 * (s32)(((s64)temp_ip * temp_lr + 0x800) >> 12))) >> 0xC);
        if (sp20 == 1) {
            var_r8 *= 2;
            var_r9 *= 2;
        }
        temp_r1_2 = temp_r0_6 - (var_r9 >> 1);
        temp_r2_3 = temp_r2_2 - (var_r8 >> 1);
        if ((temp_r2_3 > (s32)(0 - var_r8)) && (temp_r2_3 < 0x100) && (temp_r1_2 > (s32)(0 - var_r9)) && (temp_r1_2 < 0xC0)) {
            sp14 += 1;
            var_r4->charName = ((s32)(charOffset + (cellPieces->charName & 0x1FFF)) >> charCfg->tileShift);
            var_r4->attr0    = ((cellPieces->attr0 & 0xFF00) | (sp2C >> 0x10));
            var_r4->attr0    = (var_r4->attr0 | (u16)(u8)temp_r1_2);
            var_r4->attr1    = ((cellPieces->attr1 & 0xFE00) ^ (sp28 >> 0x10));
            var_r4->attr1    = (var_r4->attr1 | (u16)(temp_r2_3 & 0x1FF));
            var_r4->attr2    = ((cellPieces->attr2 & 0xFC00) + (sp24 >> 0x10));
            var_r4->attr2    = (var_r4->attr2 | (u16)((s32)(charOffset + (cellPieces->attr2 & 0x3FF)) >> charCfg->tileShift));
            var_r4++;
        }
        cellPieces++;
    }

    var_r4->charName = 0xFFFF;
    if (sp14 > 0) {
        func_02004ce8(&data_0206a9bc, (sp14 + 1) * 8);
    } else {
        temp_r0 = &data_02059d1c;
    }
    return temp_r0;
}

void OamMgr_QueueCellCharTransfers(OamManager* mgr, s32 transferBase, s32 charOffset, s32 charDataBase,
                                   OamCellPiece* cellPieces) {
    s32 temp_r7 = mgr->charBases[((cellPieces->attr0 >> 0xA) << 0x1E)]->tileShift - 1;

    while ((u16)(cellPieces->charName + 1) != 0) {
        if (cellPieces->charName & 0x8000) {
            OamCellShape* cellShape = OamMgr_GetCellShape(cellPieces);
            s32           temp_r5   = cellShape->texSizeS;
            s32           temp_r4   = cellShape->unk_18;
            func_02001b44(mgr->type, transferBase + charOffset, charDataBase + ((u32)(cellPieces->charName << 0x13) >> 0xE),
                          temp_r5 << temp_r4);
            charOffset += ~temp_r7 & (temp_r7 + (temp_r5 << temp_r4));
        }
        cellPieces++;
        cellPieces->charName = cellPieces->charName;
    }
}

OamCellPiece* OamMgr_CloneCellPieces(OamCellPiece* dstCellPieces, OamCellPiece* srcCellPieces) {
    if (dstCellPieces == NULL) {
        dstCellPieces = func_02004c8c(&data_0206a9bc, 0x408);
        if (dstCellPieces == NULL) {
            return NULL;
        }
        s32 var_r0 = 0;
        while ((u16)(srcCellPieces->charName + 1) != 0) {
            var_r0 += 1;
            dstCellPieces->charName = srcCellPieces->attr2 & 0x3FF;
            dstCellPieces->attr0    = srcCellPieces->attr0;
            dstCellPieces->attr1    = srcCellPieces->attr1;
            dstCellPieces->attr2    = srcCellPieces->attr2;
            srcCellPieces++;
            dstCellPieces++;
        }

        if (var_r0 <= 0) {
            dstCellPieces = &data_02059d1c;
        } else {
            func_02004ce8(&data_0206a9bc, (var_r0 + 1) * 8);
        }
    } else {
        while ((u16)(srcCellPieces->charName + 1) != 0) {
            dstCellPieces->charName = srcCellPieces->attr2 & 0x3FF;
            dstCellPieces++;
            srcCellPieces++;
        }
    }
    dstCellPieces->charName = 0xFFFF;
    return dstCellPieces;
}

void OamMgr_ResetCommandQueues(OamManager* mgr) {
    mgr->cmdCount           = 0;
    mgr->prioGroups[0].head = NULL;
    // Empty-list sentinel points at its own storage for each priority group.
    mgr->prioGroups[0].sentinel   = (OamSpriteCmd*)&mgr->prioGroups[0].sentinel;
    mgr->prioGroups[1].head       = NULL;
    mgr->prioGroups[1].sentinel   = (OamSpriteCmd*)&mgr->prioGroups[1].sentinel;
    mgr->prioGroups[2].head       = NULL;
    mgr->prioGroups[2].sentinel   = (OamSpriteCmd*)&mgr->prioGroups[2].sentinel;
    mgr->prioGroups[3].head       = NULL;
    mgr->prioGroups[3].sentinel   = (OamSpriteCmd*)&mgr->prioGroups[3].sentinel;
    mgr->prioGroups[0].sortedRoot = NULL;
    mgr->prioGroups[1].sortedRoot = NULL;
    mgr->prioGroups[2].sortedRoot = NULL;
    mgr->prioGroups[3].sortedRoot = NULL;
}

void OamMgr_SubmitCommand(OamManager* mgr, u32 sortKey, OamCellPiece* spriteData) {
    data_02059a6c[mgr->renderMode](mgr, sortKey, spriteData);
}

void OamMgr_FlushCommands(OamManager* mgr) {
    data_02059a78[mgr->renderMode](mgr);
}

static void func_020034cc(OamManager* mgr, u32 arg1, OamCellPiece* arg2) {
    if (arg2 == NULL || mgr->cmdCount & ~0x7F) {
        return;
    }

    OamSpriteCmd* cmd = &mgr->cmds[mgr->cmdCount];
    mgr->cmdCount++;
    mgr->sortBuffer[mgr->cmdCount] = cmd;
    cmd->sortKey                   = ((mgr->cmdCount - 1) ^ 0x7F) | (arg1 << 7);
    cmd->spriteData                = arg2;
}

void func_0200351c(OamManager* mgr, s32 arg1, s32 arg2, s32 arg3, OamCellPiece* arg4, u16 arg5, s32 arg6) {
    OamCellPiece* var_r0;

    if (((u32)(arg5 << 0x1F) >> 0x1F) == 1) {
        var_r0 = OamMgr_BuildVisibleCellPiecesAffine(mgr, arg2, arg3, arg4, arg5, arg6,
                                                     (u8*)&mgr->affine[((u32)(arg5 << 0x16) >> 0x1B)]);
    } else {
        var_r0 = OamMgr_BuildVisibleCellPieces(mgr, arg2, arg3, arg4, arg5, arg6);
    }
    func_020034cc(mgr, arg1, var_r0);
}

void func_020035b4(OamManager* mgr) {
    s32 temp_r7;
    u32 temp_r0;
    u32 temp_r1;
    u32 temp_r3;
    u32 temp_r3_2;
    u32 temp_r4;
    u32 temp_r9;
    u32 var_ip;
    u32 var_lr;
    u32 var_r0;
    u32 var_r1;
    u32 var_r2;
    u32 var_r7;
    u32 var_r7_2;
    u8* temp_r10;
    u8* temp_r10_2;
    u8* temp_r5;
    u8* temp_r9_2;
    u8* temp_r9_3;

    temp_r4 = (u32)mgr->cmdCount;
    temp_r3 = temp_r4 >> 1;
    var_r0  = temp_r3;
    temp_r5 = (u8*)mgr->sortBuffer;
    if (temp_r3 != 0) {
        do {
            var_r1 = var_r0;
            if (var_r0 <= temp_r3) {
            loop_2:
                var_r2  = var_r1 * 2;
                temp_r9 = var_r2 + 1;
                if ((temp_r9 <= temp_r4) && ((u32) * (s32*)(*(u8**)(temp_r5 + var_r2 * 4) + 8) <=
                                             (u32) * (s32*)(*(u8**)(temp_r5 + var_r2 * 4 + 4) + 8)))
                {
                    var_r2 = temp_r9;
                }
                temp_r10  = *(u8**)(temp_r5 + var_r2 * 4);
                temp_r9_2 = *(u8**)(temp_r5 + var_r1 * 4);
                if ((u32) * (s32*)(temp_r9_2 + 8) <= (u32) * (s32*)(temp_r10 + 8)) {
                    *(void**)(temp_r5 + var_r1 * 4) = temp_r10;
                    var_r1                          = var_r2;
                    *(void**)(temp_r5 + var_r2 * 4) = temp_r9_2;
                    if (var_r2 <= temp_r3) {
                        goto loop_2;
                    }
                }
            }
            var_r0 -= 1;
        } while (var_r0 != 0);
    }
    var_ip = temp_r4;
    if (temp_r4 > 1U) {
        do {
            temp_r7                       = *(s32*)(temp_r5 + 4);
            temp_r1                       = var_ip - 1;
            *(s32*)(temp_r5 + 4)          = *(s32*)(temp_r5 + var_ip * 4);
            temp_r3_2                     = temp_r1 >> 1;
            var_lr                        = 1;
            *(s32*)(temp_r5 + var_ip * 4) = temp_r7;
            if (temp_r3_2 >= 1U) {
            loop_11:
                var_r7  = var_lr * 2;
                temp_r0 = var_r7 + 1;
                if ((temp_r0 <= temp_r1) && ((u32) * (s32*)(*(u8**)(temp_r5 + var_r7 * 4) + 8) <=
                                             (u32) * (s32*)(*(u8**)(temp_r5 + var_r7 * 4 + 4) + 8)))
                {
                    var_r7 = temp_r0;
                }
                temp_r10_2 = *(u8**)(temp_r5 + var_r7 * 4);
                temp_r9_3  = *(u8**)(temp_r5 + var_lr * 4);
                if ((u32) * (s32*)(temp_r9_3 + 8) <= (u32) * (s32*)(temp_r10_2 + 8)) {
                    *(void**)(temp_r5 + var_lr * 4) = temp_r10_2;
                    var_lr                          = var_r7;
                    *(void**)(temp_r5 + var_r7 * 4) = temp_r9_3;
                    if (var_r7 <= temp_r3_2) {
                        goto loop_11;
                    }
                }
            }
            var_ip -= 1;
        } while (var_ip > 1U);
    }
    var_r7_2 = 1;
    if (temp_r4 < 1) {
        return;
    }
    do {
        OamSpriteCmd* cmd = *(OamSpriteCmd**)(temp_r5 + var_r7_2 * 4);
        OamMgr_CopyCellPiecesToOam(mgr, cmd->spriteData);
        var_r7_2 += 1;
    } while (var_r7_2 <= temp_r4);
}

void func_02003704(OamManager* mgr, u32 arg1, OamCellPiece* arg2) {
    OamSpriteCmd* cmd;
    OamSpriteCmd* next;
    OamSpriteCmd* prev;
    u8*           tailPtr;
    s32           groupOff;
    u8*           cmdBase;
    u8*           groupBase;
    s32           prio;
    s32           mode;

    if (arg2 == NULL) {
        return;
    }

    if (mgr->cmdCount & ~0x7F) {
        return;
    }

    prio = (s32)(arg2->attr1 & 0xC00);
    prio >>= 10;
    groupOff = prio * 0x18;

    cmdBase = (u8*)mgr->cmds;
    mgr->cmdCount++;
    cmd = (OamSpriteCmd*)(cmdBase + (mgr->cmdCount << 4));

    cmd->sortKey    = arg1;
    groupBase       = (u8*)mgr + groupOff;
    cmd->spriteData = arg2;

    mode    = *(s32*)(groupBase + 0x628);
    tailPtr = (u8*)mgr + 0x630;

    if (mode != 0) {
        tailPtr = tailPtr + groupOff;
        next    = *(OamSpriteCmd**)(tailPtr + 4);
        prev    = (OamSpriteCmd*)tailPtr;
        if (next != NULL) {
        loop_7:
            if (cmd->sortKey <= next->sortKey) {
                cmd->right  = prev->right;
                prev->right = cmd;
            } else {
                prev = next;
                next = next->right;
                if (next != NULL) {
                    goto loop_7;
                }
            }
        }
    } else {
        prev    = *(OamSpriteCmd**)(tailPtr + groupOff);
        tailPtr = tailPtr + groupOff;
        next    = prev->right;
    }

    if (next != NULL) {
        return;
    }

    prev->right              = cmd;
    cmd->right               = NULL;
    *(OamSpriteCmd**)tailPtr = cmd;
}

void func_020037d0(OamManager* mgr, u32 arg1, s32 arg2, s32 arg3, OamCellPiece* arg4, u16 arg5, s32 arg6) {
    u16           spC;
    OamCellPiece* var_r0;

    spC = arg5;
    if (((u32)(spC << 0x1F) >> 0x1F) == 1) {
        var_r0 = OamMgr_BuildVisibleCellPiecesAffine(mgr, arg2, arg3, arg4, arg5, arg6,
                                                     (u8*)&mgr->affine[((u32)(spC << 0x16) >> 0x1B)]);
    } else {
        var_r0 = OamMgr_BuildVisibleCellPieces(mgr, arg2, arg3, arg4, arg5, arg6);
    }
    func_02003704(mgr, arg1, var_r0);
}

void OamMgr_FlushPriorityLists(OamManager* mgr) {
    u8* var_r4;

    var_r4 = (u8*)mgr->prioGroups[0].head;
    if (var_r4 != NULL) {
        do {
            OamMgr_CopyCellPiecesToOam(mgr, ((OamSpriteCmd*)var_r4)->spriteData);
            var_r4 = (u8*)((OamSpriteCmd*)var_r4)->right;
        } while (var_r4 != NULL);
    }
    var_r4 = (u8*)mgr->prioGroups[1].head;
    if (var_r4 != NULL) {
        do {
            OamMgr_CopyCellPiecesToOam(mgr, ((OamSpriteCmd*)var_r4)->spriteData);
            var_r4 = (u8*)((OamSpriteCmd*)var_r4)->right;
        } while (var_r4 != NULL);
    }
    var_r4 = (u8*)mgr->prioGroups[2].head;
    if (var_r4 != NULL) {
        do {
            OamMgr_CopyCellPiecesToOam(mgr, ((OamSpriteCmd*)var_r4)->spriteData);
            var_r4 = (u8*)((OamSpriteCmd*)var_r4)->right;
        } while (var_r4 != NULL);
    }
    var_r4 = (u8*)mgr->prioGroups[3].head;
    if (var_r4 == NULL) {
        return;
    }
    do {
        OamMgr_CopyCellPiecesToOam(mgr, ((OamSpriteCmd*)var_r4)->spriteData);
        var_r4 = (u8*)((OamSpriteCmd*)var_r4)->right;
    } while (var_r4 != NULL);
}

void func_02003904(OamManager* mgr, u32 arg1, OamCellPiece* arg2) {
    u8* temp_ip;
    u8* var_r0;
    u8* var_r3;
    s32 temp_r2;
    s32 temp_r3;
    u8* temp_r5;

    if (arg2 == NULL) {
        return;
    }
    temp_r3 = mgr->cmdCount;
    if (temp_r3 & ~0x7F) {
        return;
    }
    temp_r5                              = (u8*)mgr->cmds;
    mgr->cmdCount                        = temp_r3 + 1;
    temp_ip                              = temp_r5 + (temp_r3 * 0x10);
    ((OamSpriteCmd*)temp_ip)->sortKey    = arg1;
    ((OamSpriteCmd*)temp_ip)->spriteData = arg2;
    temp_r2                              = ((s32)(arg2->attr1 & 0xC00) >> 0xA) * 0x18;
    *(s32*)(temp_r5 + (temp_r3 * 0x10))  = 0;
    var_r3                               = (u8*)&mgr->prioGroups[0].sortedRoot;
    *(void**)(temp_ip + 4)               = NULL;
    var_r0                               = *(u8**)(var_r3 + temp_r2);
    var_r3                               = var_r3 + temp_r2;
    if (var_r0 != NULL) {
        do {
            if ((u32) * (s32*)(var_r0 + 8) >= arg1) {
                var_r3 = var_r0;
            } else {
                var_r3 = var_r0 + 4;
            }
            var_r0 = *(u8**)var_r3;
        } while (var_r0 != NULL);
    }
    *(void**)var_r3 = temp_ip;
}

void func_02003994(OamManager* mgr, u32 arg1, s32 arg2, s32 arg3, OamCellPiece* arg4, u16 arg5, s32 arg6) {
    u16           spC;
    OamCellPiece* var_r0;

    spC = arg5;
    if (((u32)(spC << 0x1F) >> 0x1F) == 1) {
        var_r0 = OamMgr_BuildVisibleCellPiecesAffine(mgr, arg2, arg3, arg4, arg5, arg6,
                                                     (u8*)&mgr->affine[((u32)(spC << 0x16) >> 0x1B)]);
    } else {
        var_r0 = OamMgr_BuildVisibleCellPieces(mgr, arg2, arg3, arg4, arg5, arg6);
    }
    func_02003904(mgr, arg1, var_r0);
}

void func_02003a2c(OamManager* mgr) {
    OamSpriteCmd* cmd;
    OamSpriteCmd* node;
    s32           stackSize;
    s32           groupIdx;

    for (groupIdx = 0; groupIdx < 4; groupIdx++) {
        stackSize = 0;
        node      = mgr->prioGroups[groupIdx].sortedRoot;

        while (stackSize != 0 || node != NULL) {
            while (node != NULL) {
                mgr->sortBuffer[stackSize] = node;
                node                       = node->right;
                stackSize++;
            }

            stackSize--;
            cmd = mgr->sortBuffer[stackSize];
            OamMgr_CopyCellPiecesToOam(mgr, cmd->spriteData);
            node = cmd->left;
        }
    }
}

OamCellFrame* func_02003aa8(OamCellPiece* arg0) {
    return &data_02059b94[((s32)arg0->attr1 >> 0xE) | (((s32)arg0->attr0 >> 0xE) * 4)];
}

void OamMgr_Init3DSpritePipeline(void) {
    Vec vec0 = (Vec) {0, 0, 0};
    Vec vec1 = data_02059aa4;
    Vec vec2 = data_02059a8c;

    GFX_FIFO_VIEWPORT = 0xBFFF0000;
    G3i_OrthoW(0, 0xC0000, 0, 0x100000, 0xBFFF0000 << 6, 0x400000, 0x400000, 1, 0);
    GFX_FIFO_MATRIX_STORE = 0;
    GFX_FIFO_MATRIX_MODE  = 1;
    G3i_LookAt(&vec0, &vec1, &vec2, 1, 0);
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 1;
    g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 62;
    REG_DISP3DCNT &= 0xCFFB;
    REG_DISP3DCNT = (REG_DISP3DCNT & ~0x3000) | 8;
    REG_DISP3DCNT &= 0xCFEF;
    G3X_SetClearColor(0, 0, 0x7FFF, 0, 0);
    OamMgr_Reset3DState();
    data_0206a890.unk_0C                = 0x1F;
    data_0206a890.unk_08                = 0;
    data_0206a890.unk_04                = 0;
    GFX_FIFO_LIGHT_DIRECTION            = 0x2D8B6127;
    GFX_FIFO_LIGHT_COLOR                = 0x7FFF;
    GFX_FIFO_MATERIAL_DIFFUSE_AMBIENT   = 0x4210FFFF;
    GFX_FIFO_MATERIAL_SPECULAR_EMISSION = 0x4210;
}

void OamMgr_Reset3DState(void) {
    G3X_Reset();
    data_0206a890.unk_00 = 0;
}

void OamMgr_Swap3DBuffers(void) {
    GFX_FIFO_SWAP_BUFFERS = 1;
}

// Struct that represents the render command passed to func_02003c7c
typedef struct {
    /* 0x00 */ s32           unk_00;
    /* 0x04 */ s32           unk_04;
    /* 0x08 */ s32           unk_08;
    /* 0x0C */ s32           unk_0C;
    /* 0x10 */ u16           unk_10;
    /* 0x12 */ u16           unk_12;
    /* 0x14 */ u16           unk_14;
    /* 0x16 */ u16           unk_16;
    /* 0x18 */ OamCellFrame* cellFrame;
    /* 0x1C */ void*         unk_1C;
    /* 0x20 */ VtxXY*        quadVertices;
} RenderCmd;

void func_02003c7c(s32 arg0, s32 arg1, s32 arg2, RenderCmd* arg3) {
    OamCellFrame* frame   = arg3->cellFrame;
    u8*           temp_r5 = arg3->unk_1C;

    GFX_FIFO_MATRIX_PUSH      = 0;
    GFX_FIFO_MATRIX_TRANSLATE = (s32)(arg0 + (frame->centerX >> 1));
    GFX_FIFO_MATRIX_TRANSLATE = (s32)(arg1 + (frame->centerY >> 1));
    GFX_FIFO_MATRIX_TRANSLATE = arg2;

    s32 temp_r1 = ((s32) * (u16*)temp_r5 >> 4) * 2;
    func_020370cc(data_0205e4e0[temp_r1], data_0205e4e0[temp_r1 + 1]);

    GFX_FIFO_MATRIX_SCALE  = (s32)(frame->width * *(s32*)(temp_r5 + 4));
    GFX_FIFO_MATRIX_SCALE  = (s32)(frame->height * *(s32*)(temp_r5 + 8));
    GFX_FIFO_MATRIX_SCALE  = 0x1000;
    GFX_FIFO_TEXTURE_PARAM = (s32)((arg3->unk_04 << 0x1A) | ((u32)arg3->unk_08 >> 3) | 0x40000000 | (frame->texSizeS << 0x14) |
                                   (frame->texSizeT << 0x17) | 0x20000000);

    s32 var_r0;
    if (arg3->unk_04 == 2) {
        var_r0 = 1;
    } else {
        var_r0 = 0;
    }
    GFX_FIFO_TEXTURE_PALETTE = (u32)(arg3->unk_0C >> (4 - var_r0));
    GFX_FIFO_POLYGON_ATTR    = (s32)((arg3->unk_10 << 0x18) | 0xC0 | (arg3->unk_12 << 0x10));

    VtxXY*        quadVtx   = arg3->quadVertices;
    TexCoordQuad* texCoords = &data_02059b14[arg3->unk_14];

    GFX_FIFO_POLYGONS_BEGIN = 1;
    GX_TexCoord(texCoords->vtx[0].s * frame->width, texCoords->vtx[0].t * frame->height);
    GX_Vtx16(quadVtx[0].x, quadVtx[0].y, 0);

    GX_TexCoord(texCoords->vtx[1].s * frame->width, texCoords->vtx[1].t * frame->height);
    GX_Vtx16(quadVtx[1].x, quadVtx[1].y, 0);

    GX_TexCoord(texCoords->vtx[2].s * frame->width, texCoords->vtx[2].t * frame->height);
    GX_Vtx16(quadVtx[2].x, quadVtx[2].y, 0);

    GX_TexCoord(texCoords->vtx[3].s * frame->width, texCoords->vtx[3].t * frame->height);
    GX_Vtx16(quadVtx[3].x, quadVtx[3].y, 0);
    GFX_FIFO_POLYGONS_END = 0;

    GFX_FIFO_MATRIX_POP = 1;
}

u16 func_02003ef4(s32 arg0, s32 arg1, s32 arg2, OamCellPiece* arg3, u16 arg4, s32 arg5, s32 arg6, void* arg7) {
    RenderCmd     rcmd;
    s32           sp10;
    s32           sp4;
    s32           sp0;
    s16           temp_r10;
    s16           temp_r1_3;
    s32           temp_ip;
    OamCellFrame* frame;
    s32           temp_r1;
    s32           temp_r1_2;
    s32           temp_r2;
    s32           temp_r2_2;
    s32           temp_r3;
    s32           temp_r8;
    s32           temp_r8_2;
    s32           temp_r9;
    s32           var_r0;
    s32           var_r6;
    s32           var_r8;
    u16           temp_r0;
    u16           temp_r0_2;
    u16           temp_r0_3;
    u16           temp_r0_6;
    u16           var_r1;
    OamCellPiece* var_r5;
    u32           temp_r0_4;
    u8*           var_r4;

    var_r6 = arg0;
    var_r4 = (u8*)arg7;
    sp0    = arg1;
    sp4    = arg2;
    var_r5 = arg3;
    if (var_r4 == NULL) {
        temp_r0_3 = arg4;
        temp_r0_4 = (u32)temp_r0_3 << 0x16;
        if (((u32)(temp_r0_3 << 0x1F) >> 0x1F) == 1) {
            var_r4 = data_02069c24 + ((temp_r0_4 >> 0x1B) * 0x10);
        } else {
            var_r4 = data_02059ad4 + (((s32)(temp_r0_4 >> 0x1B) >> 3) * 0x10);
        }
    }
    rcmd.unk_10       = ((u16*)data_02059a84)[0];
    rcmd.unk_12       = ((u16*)data_02059a84)[1];
    rcmd.unk_14       = ((u16*)data_02059a84)[2];
    rcmd.unk_1C       = var_r4;
    rcmd.quadVertices = data_02059ab0;
    rcmd.unk_16       = ((u16*)data_02059a84)[3];
    if (var_r5->attr0 & 0x2000) {
        rcmd.unk_04 = 4;
    } else {
        rcmd.unk_04 = 3;
    }
    if (((u32)(arg4 << 0x1C) >> 0x1E) == 1) {
        rcmd.unk_12 = (u16)data_0206a890.unk_0C;
    }
    var_r1    = var_r5->charName;
    temp_r0_2 = var_r1 + 1;
    if (temp_r0_2 == 0) {
        return temp_r0_2;
    }
    do {
        rcmd.unk_08    = arg5 + ((u32)(var_r1 << 0x13) >> 0xE);
        rcmd.unk_0C    = arg6 + ((u32)((((u32)(arg4 << 0x10) >> 0x1C) + ((s32)var_r5->attr2 >> 0xC)) << 0x1C) >> 0x17);
        rcmd.unk_14    = (u16)(((s32)var_r5->attr1 >> 0xC) & 3);
        frame          = func_02003aa8(var_r5);
        rcmd.cellFrame = frame;
        temp_r1        = frame->width;
        temp_r2        = frame->height;
        temp_r8        = var_r5->attr1 & 0x1FF;
        temp_r9        = *(s32*)(var_r4 + 4);
        var_r8         = temp_r8 | data_02059a98[(temp_r8 >> 8) * 2];
        if (rcmd.unk_14 & 1) {
            var_r8 = 0 - (var_r8 + temp_r1);
        }
        temp_r0_6 = var_r5->attr0;
        temp_ip   = var_r8 + (temp_r1 >> 1);
        var_r0    = (u8)temp_r0_6 | data_02059a98[((s32)(u8)temp_r0_6 >> 7)];
        if (rcmd.unk_14 & 2) {
            var_r0 = 0 - (var_r0 + temp_r2);
        }
        temp_r3   = var_r0 + (temp_r2 >> 1);
        temp_r1_2 = sp4 - (temp_r2 >> 1);
        sp10      = temp_r1_2;
        temp_r2_2 = *(s32*)(var_r4 + 8);
        temp_r8_2 = ((s32) * (u16*)var_r4 >> 4) * 2;
        temp_r10  = data_0205e4e0[temp_r8_2];
        temp_r1_3 = data_0205e4e0[temp_r8_2 + 1];
        if (var_r6 < 0) {
            var_r6               = data_0206a890.unk_00 - 0x200;
            data_0206a890.unk_00 = var_r6;
        }
        func_02003c7c(((temp_ip * (s32)(((s64)temp_r1_3 * temp_r9 + 0x800) >> 12)) -
                       (temp_r3 * (s32)(((s64)temp_r10 * temp_r2_2 + 0x800) >> 12))) +
                          ((sp0 - (temp_r1 >> 1)) << 0xC),
                      (temp_ip * (s32)(((s64)temp_r10 * temp_r9 + 0x800) >> 12)) +
                          (temp_r3 * (s32)(((s64)temp_r1_3 * temp_r2_2 + 0x800) >> 12)) + (temp_r1_2 << 0xC),
                      var_r6, &rcmd);
        var_r5++;
        var_r1  = var_r5->charName;
        temp_r0 = var_r1 + 1;
    } while (temp_r0 != 0);
    return temp_r0;
}
