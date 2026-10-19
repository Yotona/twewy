#include "Field/Tasks/FldChara.h"
#include "Display.h"
#include "Engine/Core/OamMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/math.h>

// The sprite owner of a field character: the FldCharaSprite plus its OAM affine slot.
typedef struct {
    /* 0x00 */ FldCharaSprite chara;
    /* 0x50 */ s32            affineIndex;
} FldCharaOwner;

static SpriteFrameInfo* FldChara_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static SpriteAnimation FldChara_Anim = {
    .bits_0_1          = 2,
    .dataType          = 1,
    .bit_6             = 0,
    .bits_7_9          = 6,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0403,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = FldChara_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = NULL,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 0,
    .unk_1E            = 0,
    .unk_20            = 0,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 0,
    .unk_28            = 0,
    .animIndex         = 1,
};

static SpriteFrameInfo* FldChara_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarlyImpl(
        sprite, mode,
        (info_->affine  = &g_OamMgr[DISPLAY_EXTENDED].affine[((FldCharaOwner*)sprite->owner)->affineIndex],
         info_->sortKey = ((FldCharaOwner*)sprite->owner)->chara.sortKey));
}

void FldChara_Load(FldCharaSprite* owner, BinIdentifier* binIden, FldCharaCell* cells, s32 index, u16 palette, u16 charBase) {
    FldCharaCell* cell = &cells[index];

    FldChara_Anim.binIden   = binIden;
    FldChara_Anim.unk_1C    = cell->charIndex;
    FldChara_Anim.unk_1E    = charBase << 5;
    FldChara_Anim.unk_26    = cell->cellIndex;
    FldChara_Anim.unk_28    = cell->animIndex;
    FldChara_Anim.unk_20    = palette;
    FldChara_Anim.animIndex = cell->anim + 1;
    FldChara_Anim.owner     = owner;
    owner->cells            = cells;
    owner->curCell          = -1;
    owner->sortKey          = -1;
    owner->unk46            = 0;
    owner->unk4C            = 0;
    Sprite_Load(&owner->sprite, &FldChara_Anim);
}

s32 FldChara_SetCell(FldCharaSprite* owner, u16 arg1, s32 arg2) {
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

void FldChara_SetFlip(Sprite* sprite, s32 arg1) {
    if (arg1 != 0) {
        sprite->unk_0A.unk_05 = sprite->unk_0A.unk_05 | 8;
    } else {
        sprite->unk_0A.unk_05 = sprite->unk_0A.unk_05 & ~8;
    }
}

s32 FldChara_IsFlipped(Sprite* sprite) {
    if (sprite->unk_0A.unk_05 & 8) {
        return 1;
    }
    return 0;
}

void FldChara_SetSortKey(FldCharaSprite* owner, s32 sortKey) {
    owner->sortKey = sortKey;
}

void FldChara_RenderWithAlpha(Sprite* sprite, s32 arg1) {
    s32 old;

    sprite->unk_0A.raw   = (sprite->unk_0A.raw & ~0xC) | 4;
    old                  = data_0206a890.unk_0C;
    data_0206a890.unk_0C = arg1 & 0x1F;
    Sprite_RenderFrame(sprite);
    sprite->unk_0A.raw &= ~0xC;
    data_0206a890.unk_0C = old & 0x1F;
}

// Nonmatching
void FldChara_MoveAlongAngle(s32* arg0, s32* arg1, s32 arg2, s32 arg3) {
    s32 idx = ((s32)(u16)arg2 >> 4) * 2;
    s32 dx  = MUL_FX32(FX_SinCosTable_[idx + 1], arg3);
    s32 dy  = MUL_FX32(FX_SinCosTable_[idx], arg3);

    *arg0 += dx;
    *arg1 += dy;
}
