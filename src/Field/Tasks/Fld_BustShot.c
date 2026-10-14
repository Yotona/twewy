#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SpriteMgr.h"

// USA 0x020b3128-0x020b3d00: the bust-shot sprite task. A 0xCC-byte task data
// block holds a Sprite at +0x00, a mirror Sprite at +0x5C, the fixed-point
// position/target fields at +0x40..+0x58 and the driver state at +0x9C..+0xC8.
// Per-animation parameters live in the 0x10-stride table at data_ov030_020df6e4.

typedef struct {
    /* 0x00 */ s16 unk0;
    /* 0x02 */ s16 unk2;
    /* 0x04 */ s16 unk4;
    /* 0x06 */ s16 unk6;
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
    /* 0x0C */ s16 unkC;
    /* 0x0E */ s16 unkE;
} BustShotEntry; // Size: 0x10

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ s32            unk40;
    /* 0x44 */ s32            unk44;
    /* 0x48 */ s32            unk48;
    /* 0x4C */ s32            unk4C;
    /* 0x50 */ s32            unk50;
    /* 0x54 */ s32            unk54;
    /* 0x58 */ s16            unk58;
    /* 0x5A */ u8             pad_5A[2];
    /* 0x5C */ Sprite         sprite2;
    /* 0x9C */ s32            unk9C;
    /* 0xA0 */ s32            unkA0;
    /* 0xA4 */ s32            unkA4;
    /* 0xA8 */ s32            unkA8;
    /* 0xAC */ s32            unkAC;
    /* 0xB0 */ s32            unkB0;
    /* 0xB4 */ s32            unkB4;
    /* 0xB8 */ s32            unkB8;
    /* 0xBC */ s16            unkBC;
    /* 0xBE */ s16            unkBE;
    /* 0xC0 */ s16            unkC0;
    /* 0xC2 */ u8             pad_C2[2];
    /* 0xC4 */ BustShotEntry* unkC4;
    /* 0xC8 */ s32            unkC8;
} BustShotSprite; // Size: 0xCC

typedef struct {
    /* 0x00 */ s32 unk0;
    /* 0x04 */ s32 unk4;
    /* 0x08 */ s32 unk8;
    /* 0x0C */ s32 unkC;
    /* 0x10 */ s16 unk10;
    /* 0x12 */ s16 unk12;
    /* 0x14 */ s16 unk14;
    /* 0x16 */ s16 unk16;
} BustShotArgs; // Size: 0x18

typedef struct {
    /* 0x00 */ void* owner;
    /* 0x04 */ void* sprite2;
    /* 0x08 */ s32   unk8;
    /* 0x0C */ s32   unkC;
    /* 0x10 */ s32   unk10;
    /* 0x14 */ float unk14;
    /* 0x18 */ s32   unk18;
    /* 0x1C */ s32   unk1C;
    /* 0x20 */ s32   unk20;
} BustShotSubArgs; // Size: 0x24

typedef TaskStages BustShotStages;

extern s16                 data_ov030_020df67c[];
extern BustShotEntry       data_ov030_020df6e4[];
extern SpriteAnimation     data_ov030_020df6b8;
extern const BinIdentifier data_ov030_020df6a0[];
extern TaskHandle          data_ov030_020df684;
extern BustShotStages      data_ov030_020df690;
extern TaskHandle          data_ov030_020ee998;

extern s32       func_ov030_020aeecc(void);
extern s32       func_ov030_020b26bc(void);
extern s32       func_ov030_020b310c(s32 arg0);
extern FieldPos* func_ov030_020af99c(void);
extern s32       func_ov030_020b0454(void);
extern s32       FX_Atan2Idx(s32 y, s32 x);

s32 func_ov030_020b38a4(Task* task);

s32 func_ov030_020b3128(s32* out) {
    TouchCoord pos;
    TouchCoord coord;

    if (func_ov030_020aeecc() == 0) {
        return 0;
    }
    if (SysControl.buttonState.currButtons & 0xCF3) {
        *out = func_ov030_020b310c(func_ov030_020b26bc());
    } else {
        FieldPos* origin;

        TouchInput_GetCoord(&coord);
        pos    = coord;
        origin = func_ov030_020af99c();
        *out   = func_ov030_020b310c(FX_Atan2Idx((origin->y + (pos.y << 0xC)) - gSaveData.fieldPos.y,
                                                 (origin->x + (pos.x << 0xC)) - gSaveData.fieldPos.x));
    }
    return 1;
}

SpriteFrameInfo* func_ov030_020b31d0(Sprite* sprite, s32 arg, s32 mode) {
    BustShotSprite* owner = sprite->owner;

    Sprite_FrameInfoCallbackWith(sprite, mode, temp_->sortKey = owner->unkB4)
}

// Nonmatching: register swap in the anim.dataType / unk_02 update tail plus a
// 3-instruction bitfield read-back round-trip (USA 95.34%; JP unvalidated).
void func_ov030_020b3278(Sprite* sprite, s32 arg1, BustShotEntry cfg, s32 unk9c, FldScreenPos pos, void* owner, s32 index) {
    SpriteAnimation anim = data_ov030_020df6b8;

    anim.dataType  = arg1;
    anim.packIndex = cfg.unk2;
    anim.unk_1E    = cfg.unk6;
    anim.binIden   = &data_ov030_020df6a0[cfg.unk0];
    anim.unk_20    = cfg.unkE;
    anim.posX      = pos.x;
    anim.posY      = pos.y;
    anim.animIndex += cfg.unk4;
    anim.owner = owner;

    if (unk9c != 0) {
        Unk_Bitfield bits = anim.unk_02;

        bits.unk_05 = (u16)(bits.unk_05 | 8);
        anim.unk_02 = bits;
    }
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020b3384(s32 arg0) {
    if (arg0 < -0x3F2) {
        return arg0;
    }
    if (arg0 >= -0x3EF) {
        return arg0;
    }
    return data_ov030_020df67c[arg0 + 0x3F2];
}

// Nonmatching: stack reloads and register scheduling (USA 96.46%; JP unvalidated).
// Screen coordinates are signed, including positions outside the screen.
void func_ov030_020b33bc(TaskPool* pool, Task* task, BustShotArgs* args) {
    BustShotSprite* sprite = task->data;
    BustShotEntry   cfg;
    FldScreenPos    pos;
    u32             idx;
    BustShotEntry*  entry;
    s16             dx;
    s16             dy;
    s32             yOffset;
    s32             mirrored;

    idx = args->unk4;
    if (idx >= 0x118U) {
        idx = 0;
    }
    entry         = &data_ov030_020df6e4[idx];
    sprite->unkC4 = entry;
    sprite->unkC8 = 0;
    sprite->unk58 = (s16)args->unkC;
    pos.sx        = func_ov030_020af6a0(args->unk12);
    if (pos.sx < 0x80) {
        mirrored = 1;
    } else {
        mirrored = 0;
    }
    sprite->unk9C = mirrored;
    if (sprite->unk9C == 0) {
        dx = entry->unk8;
    } else {
        dx = 0 - entry->unk8;
    }
    pos.sx        = (s16)(pos.sx + dx);
    yOffset       = func_ov030_020b3384(args->unk16);
    sprite->unk48 = pos.sx << 0xC;
    pos.sy        = (s16)(entry->unkC + yOffset);
    if (sprite->unk9C == 0) {
        dy = entry->unkA;
    } else {
        dy = 0 - entry->unkA;
    }
    sprite->unk4C = (dy + func_ov030_020af6a0(args->unk10)) << 0xC;
    sprite->unk40 = sprite->unk48;
    sprite->unk44 = sprite->unk4C;
    sprite->unk50 = pos.sy << 0xC;
    sprite->unk54 = (entry->unkC + func_ov030_020b3384(args->unk16)) << 0xC;
    cfg           = *entry;
    func_ov030_020b3278(&sprite->sprite, args->unk0, cfg, sprite->unk9C, pos, sprite, idx);
    sprite->unkA0 = 1;
    sprite->unkA4 = 0;
    sprite->unkA8 = args->unk0;
    sprite->unkAC = -1;
    sprite->unkB0 = 0;
    sprite->unkB4 = 0x16;
    sprite->unkB8 = 0;
    sprite->unkBC = 0;
    sprite->unkBE = 0;
    sprite->unkC0 = 0;
}
s32 func_ov030_020b356c(TaskPool* pool, Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->unkAC != -1) {
        if (EasyTask_ValidateTaskId(pool, (u32*)&sprite->unkAC) != 0) {
            return 1;
        }
        sprite->unkAC = -1;
        Sprite_Destroy(&sprite->sprite);
        sprite->sprite = sprite->sprite2;
        sprite->unkA0  = 1;
    }
    if (sprite->unk58 != 0) {
        if (func_ov030_020b0454() != 0) {
            sprite->unk58 = 1;
        }
        func_020265d4(&sprite->unk40, sprite->unk44, (u16)sprite->unk58);
        func_020265d4(&sprite->unk50, sprite->unk54, (u16)sprite->unk58);
        sprite->unk58 = (s16)(sprite->unk58 - 1);
    }
    sprite->sprite.posX = (s16)(sprite->unk40 >> 0xC);
    sprite->sprite.posY = (s16)(sprite->unk50 >> 0xC);
    if (sprite->unk58 != 0 && sprite->unkB8 != 0) {
        sprite->unkBC = (s16)(sprite->unkBC + 1);
        if (sprite->unkBC % 2 == 0) {
            sprite->unkBE = (s16)(RNG_Next(8) - 4);
            sprite->unkC0 = (s16)(RNG_Next(8) - 4);
        }
        sprite->sprite.posX += sprite->unkBE;
        sprite->sprite.posY += sprite->unkC0;
    } else {
        sprite->unkB8 = 0;
    }
    if (sprite->unkA0 != 0) {
        Sprite_UpdateAndCheck(&sprite->sprite);
    }
    if (sprite->unkB0 != 0 && func_ov030_020b38a4(task) != 0) {
        return 0;
    }
    return 1;
}

s32 func_ov030_020b36f8(TaskPool* pool, Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->unkA0 != 0) {
        Sprite_Render(&sprite->sprite);
    }
    return 1;
}

s32 func_ov030_020b3718(TaskPool* pool, Task* task) {
    Sprite_Destroy(&((BustShotSprite*)task->data)->sprite);
    return 1;
}

s32 func_ov030_020b372c(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    BustShotStages stages = data_ov030_020df690;

    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020b3774(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7) {
    BustShotArgs args;

    args.unk0  = arg1;
    args.unk4  = arg2;
    args.unkC  = arg3;
    args.unk12 = arg4;
    args.unk16 = arg5;
    args.unk10 = arg6;
    args.unk14 = arg7;
    return EasyTask_CreateTask(pool, &data_ov030_020df684, NULL, 0, NULL, &args);
}

void func_ov030_020b37cc(Task* task, s32 arg1, s32 arg2) {
    BustShotSprite* sprite = task->data;
    s16             dx;
    s32             x;
    s32             slideX;

    sprite->unkB0 = arg2;
    if (sprite->unkC8 != 0) {
        return;
    }
    sprite->unkC8 = 1;
    if (sprite->unk9C == 0) {
        dx = sprite->unkC4->unk8;
    } else {
        dx = 0 - sprite->unkC4->unk8;
    }
    x = sprite->unk40;
    if (arg1 != 0) {
        if (x < 0x80000) {
            slideX = (dx + func_ov030_020af6a0(-0x3FD)) << 0xC;
        } else {
            slideX = (dx + func_ov030_020af6a0(0xFFFFFC0D)) << 0xC;
        }
    } else if (x >= 0x80000) {
        slideX = (dx + func_ov030_020af6a0(-0x3FD)) << 0xC;
    } else {
        slideX = (dx + func_ov030_020af6a0(0xFFFFFC0D)) << 0xC;
    }
    sprite->unk44 = slideX;
    sprite->unk48 = slideX;
    sprite->unk58 = (s16)(0x14 - sprite->unk58);
    if ((s32)sprite->unk58 < 1) {
        sprite->unk58 = 1;
    }
}

s32 func_ov030_020b38a4(Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->unk58 == 0 && sprite->unk44 == sprite->unk48) {
        sprite->unkC8 = 0;
        return 1;
    }
    return 0;
}

s32 func_ov030_020b38d4(Task* task) {
    return ((BustShotSprite*)task->data)->unk58 == 0;
}

s16 func_ov030_020b38ec(Task* task) {
    return ((BustShotSprite*)task->data)->sprite.posX;
}

void func_ov030_020b38f8(Task* task, s32 arg1) {
    BustShotSprite* sprite = task->data;
    s32             nudge;

    nudge = 0x10000;
    if ((s32)sprite->sprite.posX < 0x60) {
        nudge = -0x10000;
    }
    if (arg1 != 0) {
        nudge = 0 - nudge;
    }
    sprite->unk44 = sprite->unk44 + nudge;
    if (sprite->unk58 == 0) {
        sprite->unk58 = 8;
    }
}

void func_ov030_020b3934(Task* task, s16 arg1) {
    BustShotSprite* sprite = task->data;

    sprite->unk44 = 0x80000;
    sprite->unk58 = arg1;
}

void func_ov030_020b3948(Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->sprite.unk_0A.unk_05 & 8) {
        Unk_Bitfield bits = sprite->sprite.unk_0A;

        bits.unk_05           = 0;
        sprite->sprite.unk_0A = bits;
        sprite->unk9C         = 0;
        return;
    }
    {
        Unk_Bitfield bits = sprite->sprite.unk_0A;

        bits.unk_05 |= 8;
        sprite->sprite.unk_0A = bits;
    }
    sprite->unk9C = 1;
}

void func_ov030_020b39e0(TaskPool* pool, Task* task, u32 idx, s32 arg3, s32 arg4) {
    BustShotSprite* sprite = task->data;
    BustShotEntry   cfg;
    BustShotSubArgs sub;

    if (sprite->unkAC != -1) {
        return;
    }
    sprite->unkA0 = 0;
    sprite->unkA4 = 1;
    if (idx >= 0x118U) {
        idx = 0;
    }
    cfg = data_ov030_020df6e4[idx];
    func_ov030_020b3278(&sprite->sprite2, sprite->unkA8, cfg, sprite->unk9C, *(FldScreenPos*)&sprite->sprite.posX, sprite,
                        idx);

    sub.owner   = sprite;
    sub.sprite2 = &sprite->sprite2;
    sub.unk8    = 0;
    sub.unkC    = 1;
    if (arg4 == 0xA) {
        sub.unk14 = 1.0f;
    } else if (arg4 == 0x3C) {
        sub.unk14 = 0.25f;
    } else if (arg4 == 0) {
        sub.unk14 = 4.0f;
    } else {
        sub.unk14 = 0.5f;
    }
    sub.unk10     = arg3;
    sprite->unkAC = EasyTask_CreateTask(pool, &data_ov030_020ee998, NULL, 0, NULL, &sub);
}

s32 func_ov030_020b3b10(Task* task) {
    BustShotSprite* sprite = task->data;

    if (sprite->unkA4 != 0 && sprite->unkA0 != 0) {
        sprite->unkA4 = 0;
        return 1;
    }
    return 0;
}

void func_ov030_020b3b3c(Task* task, s16 arg1) {
    BustShotSprite* sprite = task->data;
    s32             v;

    sprite->unk48 = 0x100000 - sprite->unk48;
    v             = 0x100000 - sprite->unk4C;
    sprite->unk4C = v;
    sprite->unk44 = v;
    sprite->unk58 = arg1;
}

void func_ov030_020b3b64(Task* task, s32 arg1) {
    ((BustShotSprite*)task->data)->unkB4 = arg1;
}

void func_ov030_020b3b70(Task* task, s16 arg1, s32 arg2, s16 arg3) {
    BustShotSprite* sprite = task->data;
    s16             dy;

    if (sprite->unk9C == 0) {
        dy = sprite->unkC4->unkA;
    } else {
        dy = 0 - sprite->unkC4->unkA;
    }
    sprite->unk44 = (dy + func_ov030_020af6a0(arg1)) << 0xC;
    sprite->unk54 = (sprite->unkC4->unkC + func_ov030_020b3384(arg2)) << 0xC;
    sprite->unk58 = arg3;
}

void func_ov030_020b3bdc(TaskPool* pool, Task* task, u32 idx, s32 arg3) {
    BustShotSprite* sprite = task->data;
    BustShotEntry   cfg;
    FldScreenPos    pos;

    if (idx >= 0x118U) {
        idx = 0;
    }
    pos = *(FldScreenPos*)&sprite->sprite.posX;
    Sprite_Destroy(&sprite->sprite);
    cfg = data_ov030_020df6e4[idx];
    func_ov030_020b3278(&sprite->sprite, sprite->unkA8, cfg, sprite->unk9C, pos, sprite, idx);
    sprite->unkA4 = 1;
}

s32 func_ov030_020b3c84(Task* task) {
    BustShotSprite* sprite = task->data;
    s32             pending;

    if (sprite->unkA4 != 0) {
        sprite->unkA4 = 0;
        pending       = 1;
    } else {
        pending = 0;
    }
    return pending;
}

void func_ov030_020b3ca0(Task* task) {
    BustShotSprite* sprite = task->data;

    sprite->unkB8 = 1;
    sprite->unkBC = 0;
    if (sprite->unk58 == 0) {
        sprite->unk58 = 0x14;
    }
}

s16 func_ov030_020b3cc8(Task* task) {
    return ((BustShotSprite*)task->data)->sprite.posX;
}

void func_ov030_020b3cd4(Task* task, u16 arg1) {
    BustShotSprite* sprite = task->data;

    sprite->sprite.unk_0A.unk_10 = arg1;
}

void func_ov030_020b3cf0(Task* task) {
    ((BustShotSprite*)task->data)->unkA4 = 0;
}
