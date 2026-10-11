// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020be114-0x020be85c; 17 functions.
// Research draft: docs/decomp/ov030/Fld_ScanKirakira.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Display.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Field/FieldChara.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/math.h>

// The scan-kirakira task data: a Sprite at +0x00 plus a countdown at +0x40 and
// the per-animation parameters handed to func_ov030_020beba0.
typedef struct {
    /* 0x00 */ Sprite sprite; // 0x40
    /* 0x40 */ s16    unk40;
    /* 0x42 */ s16    unk42;
    /* 0x44 */ s32    unk44;
    /* 0x48 */ s32    unk48;
    /* 0x4C */ s32    unk4C;
    /* 0x50 */ s32    unk50;
    /* 0x54 */ Task*  human;
    /* 0x58 */ s32    unk58;
} ScanKirakiraSprite;

// Spawn parameters (also passed through to func_ov030_020be540).
typedef struct {
    /* 0x00 */ u16   unk0;
    /* 0x02 */ u16   unk2;
    /* 0x04 */ s32   unk4;
    /* 0x08 */ s32   unk8;
    /* 0x0C */ s32   unkC;
    /* 0x10 */ s32   unk10;
    /* 0x14 */ s32   unk14;
    /* 0x18 */ Task* human;
} ScanKirakiraArgs; // Size: 0x1C

typedef TaskStages ScanKirakiraFns;

extern s16             data_0205e4e0[];
extern SpriteAnimation data_ov030_021073c8;
extern SpriteAnimation data_ov030_020e6334;
extern ScanKirakiraFns data_ov030_020e6324;
extern TaskHandle      data_ov030_020e6318;

extern s32  func_ov030_020aef44(void);
extern void func_ov030_020beba0(TaskPool* pool, s32 unk44, s16 x, s16 y, s32 unk48, s32 unk4C, s32 unk50, Task* human);

SpriteFrameInfo* func_ov030_020be114(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarlyImpl(
        sprite, mode,
        (info_->affine  = &g_OamMgr[DISPLAY_EXTENDED].affine[((ScanKirakiraSprite*)sprite->owner)->unk50],
         info_->sortKey = ((ScanKirakiraSprite*)sprite->owner)->unk48));
}

// Nonmatching: template address rematerialized into a second pool word for the
// Sprite_Load call (USA 95.00%; JP unvalidated).
void func_ov030_020be1d8(FldCharaSprite* owner, BinIdentifier* binIden, FldCharaCell* cells, s32 index, u16 palette,
                         u16 charBase) {
    FldCharaCell* cell = &cells[index];

    data_ov030_021073c8.binIden   = binIden;
    data_ov030_021073c8.unk_1C    = cell->charIndex;
    data_ov030_021073c8.unk_1E    = charBase << 5;
    data_ov030_021073c8.unk_26    = cell->cellIndex;
    data_ov030_021073c8.unk_28    = cell->animIndex;
    data_ov030_021073c8.unk_20    = palette;
    data_ov030_021073c8.animIndex = cell->anim + 1;
    data_ov030_021073c8.owner     = owner;
    owner->cells                  = cells;
    owner->curCell                = -1;
    owner->sortKey                = -1;
    owner->unk46                  = 0;
    owner->unk4C                  = 0;
    Sprite_Load(&owner->sprite, &data_ov030_021073c8);
}

s32 func_ov030_020be258(FldCharaSprite* owner, u16 arg1, s32 arg2) {
    FldCharaCell* cell;
    Data*         resourceData;
    void*         animData;
    void*         cellData;
    void*         charData;
    u32           mode;

    if (arg1 == owner->curCell) {
        return 1;
    }
    owner->curCell              = arg1;
    owner->sprite.animationMode = arg2;
    resourceData                = owner->sprite.resourceData;
    cell                        = &owner->cells[arg1];
    animData                    = Data_GetPackEntryData(resourceData, cell->animIndex);
    cellData                    = Data_GetPackEntryData(resourceData, cell->cellIndex);
    Sprite_ChangeAnimation(&owner->sprite, animData, cell->anim + 1, cellData);
    mode = owner->sprite.bits_3_4;
    switch (mode) {
        case 0:
        case 2:
        case 3:
            break;
        default:
            break;
        case 1:
            charData            = Data_GetPackEntryData(owner->sprite.resourceData, cell->charIndex);
            owner->sprite.unk34 = charData;
            break;
    }
    return 0;
}

void func_ov030_020be368(Sprite* sprite, s32 arg1) {
    if (arg1 != 0) {
        sprite->unk_0A.unk_05 = sprite->unk_0A.unk_05 | 8;
    } else {
        sprite->unk_0A.unk_05 = sprite->unk_0A.unk_05 & ~8;
    }
}

s32 func_ov030_020be3c0(Sprite* sprite) {
    if (sprite->unk_0A.unk_05 & 8) {
        return 1;
    }
    return 0;
}

void func_ov030_020be3dc(FldCharaSprite* owner, s32 sortKey) {
    owner->sortKey = sortKey;
}

void func_ov030_020be3e4(Sprite* sprite, s32 arg1) {
    s32 old;

    sprite->unk_0A.raw   = (sprite->unk_0A.raw & ~0xC) | 4;
    old                  = data_0206a890.unk_0C;
    data_0206a890.unk_0C = arg1 & 0x1F;
    Sprite_RenderFrame(sprite);
    sprite->unk_0A.raw &= ~0xC;
    data_0206a890.unk_0C = old & 0x1F;
}

// Nonmatching: the target interleaves two independent 64-bit smull chains
// (reusing the arg3 register r3 as the 2nd product's hi-limb; body uses
// {r3,r4,r5}); ours serializes them over {r4,r5,r6}. Every equivalent source
// shape (temps / products / inlined) yields the same serialized output.
// (USA 63.28%; JP unvalidated). Same residual as Fld_Human func_ov030_020b4d5c.
void func_ov030_020be430(s32* arg0, s32* arg1, s32 arg2, s32 arg3) {
    s32 idx = ((s32)(u16)arg2 >> 4) * 2;
    s32 dx  = MUL_FX32(data_0205e4e0[idx + 1], arg3);
    s32 dy  = MUL_FX32(data_0205e4e0[idx], arg3);

    *arg0 += dx;
    *arg1 += dy;
}

SpriteFrameInfo* func_ov030_020be4a4(Sprite* sprite, s32 arg, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;
        case SPRITE_FRAME_RENDER: {
            SpriteFrameInfo* frameInfo = &g_SpriteFrameInfo;

            Sprite_FillFrameInfo(frameInfo, sprite);
            info = frameInfo;
        }
        default:
            return info;
    }
}

void func_ov030_020be540(Sprite* sprite, ScanKirakiraArgs* args) {
    SpriteAnimation anim = data_ov030_020e6334;
    s16             posX;

    anim.dataType  = (u16)args->unk8;
    posX           = args->unk0;
    anim.posY      = args->unk2;
    anim.posX      = posX;
    anim.animIndex = (s16)args->unk4;
    if (args->unk4 == 5) {
        anim.animIndex = 1;
        anim.unk_20    = 9;
    }
    if (args->unkC >= 0x10000) {
        anim.unk_20 = 5;
    }
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020be5ec(TaskPool* pool, Task* task, ScanKirakiraArgs* args) {
    ScanKirakiraSprite* data = task->data;

    data->unk44 = args->unk8;
    data->unk48 = args->unkC;
    data->unk4C = args->unk10;
    data->unk50 = args->unk14;
    data->human = args->human;
    func_ov030_020be540(&data->sprite, args);
    data->unk40 = 0x14;
    data->unk58 = 0;
    return 1;
}

s32 func_ov030_020be640(s32 arg0, s32 arg1) {
    if (gSaveData.unk_2458 == 1) {
        return 1;
    }
    if (arg0 > 0x100) {
        return 0;
    }
    if (arg0 < 0) {
        return 0;
    }
    if (arg1 < 0) {
        return 0;
    }
    if (arg1 <= 0xC0) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020be68c(TaskPool* pool, Task* task) {
    ScanKirakiraSprite* data = task->data;
    s32                 temp;

    if (data->unk40 != 0) {
        data->unk40 = (s16)(data->unk40 - 1);
    }
    if (data->unk40 == 0 && data->unk48 != -2 && func_ov030_020be640(data->sprite.posX, data->sprite.posY) != 0) {
        temp = data->unk58;
        if (temp == 0) {
            data->unk58 = temp + 1;
            Sprite_SetAnimation(&data->sprite, data->sprite.animData, 2, data->sprite.cellTable);
            data->unk40 = 0x12;
        } else {
            func_ov030_020beba0(pool, data->unk44, data->sprite.posX, data->sprite.posY, data->unk48, data->unk4C, data->unk50,
                                data->human);
            return 0;
        }
    }
    if (func_ov030_020aefbc() == 0) {
        return 0;
    }
    if (func_ov030_020aef44() != 0) {
        return 0;
    }
    Sprite_Update(&data->sprite);
    return 1;
}

s32 func_ov030_020be78c(TaskPool* pool, Task* task) {
    Sprite_RenderFrame(task->data);
    return 1;
}

s32 func_ov030_020be7a0(TaskPool* pool, Task* task) {
    Sprite_Release(task->data);
    return 1;
}

s32 func_ov030_020be7b4(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    ScanKirakiraFns funcs = data_ov030_020e6324;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020be7fc(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7, Task* arg8) {
    ScanKirakiraArgs params;

    params.unk8  = arg1;
    params.unk0  = arg2;
    params.unk2  = arg3;
    params.unk4  = arg4;
    params.unk14 = arg7;
    params.human = arg8;
    params.unkC  = arg5;
    params.unk10 = arg6;
    return EasyTask_CreateTask(pool, &data_ov030_020e6318, NULL, 0, NULL, &params);
}
