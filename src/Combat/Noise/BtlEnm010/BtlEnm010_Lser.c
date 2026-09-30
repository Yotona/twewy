#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"
#include "Engine/Math/Random.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm010LserRec {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 pad_02;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} BtlEnm010LserRec;

typedef struct BtlEnm010LserEmit {
    /* 0x00 */ BtlEnm010LserRec rec[3];
    /* 0x24 */ s16              unk_24;
    /* 0x26 */ s16              unk_26;
    /* 0x28 */ s16              unk_28;
    /* 0x2A */ u16              pad_2A;
} BtlEnm010LserEmit;

typedef struct BtlEnm010Lser {
    /* 0x000 */ BtlEnm010Owner*   unk_00;
    /* 0x004 */ CombatActor       copy; // the owner's actor, refreshed every frame
    /* 0x080 */ CombatSprite      sprite[4];
    /* 0x200 */ BtlEnm010LserEmit emit;
    /* 0x22C */ s32               unk_22C;
    /* 0x230 */ s32               unk_230;
    /* 0x234 */ s32               unk_234;
    /* 0x238 */ s32               pad_238;
    /* 0x23C */ s32               unk_23C;
    /* 0x240 */ s32               unk_240;
    /* 0x244 */ s32               unk_244;
    /* 0x248 */ s32               unk_248;
    /* 0x24C */ s32               unk_24C;
    /* 0x250 */ u8                unk_250;
    /* 0x251 */ u8                pad_251[3];
} BtlEnm010Lser;

typedef struct BtlEnm010LserArgs {
    /* 0x00 */ BtlEnm010Owner* unk_00;
    /* 0x04 */ s32             pad_04[3];
    /* 0x10 */ s32             unk_10;
    /* 0x14 */ s32             unk_14;
} BtlEnm010LserArgs;

extern char data_ov011_0212cbe8[20];

typedef struct BtlEnm010CmdTbl {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8  unk_04;
    /* 0x05 */ u8  unk_05;
    /* 0x06 */ u8  unk_06;
    /* 0x07 */ u8  unk_07;
    /* 0x08 */ s32 unk_08;
} BtlEnm010CmdTbl;

extern const BtlEnm010CmdTbl* func_ov003_0208a114(u16 idx);

extern s32 func_ov003_0208a164(BtlEnm010CmdTbl* rec, void* p, s32 x, s32 y, s32 z);
extern s32 func_ov003_020cc7f4(s32 a0, s32 a1, s32 a2, s32 a3);

s32 func_ov011_02125cf8(s32 arg0, Task* task, s32 arg2, s32 cmd);
s32 func_ov011_02125d48(BtlEnm010Lser* data, BtlEnm010LserArgs* args);
s32 func_ov011_02125e14(BtlEnm010Lser* data);
s32 func_ov011_02125f24(BtlEnm010Lser* data);
s32 func_ov011_02126064(BtlEnm010Lser* data);
s32 func_ov011_02126354(BtlEnm010Lser* data);
s32 func_ov011_021265a8(BtlEnm010Lser* data);

extern s32 func_ov011_021260e8(BtlEnm010Lser* data);
extern s32 func_ov003_020c37f8(void* p);
extern s32 func_ov011_02125e14(BtlEnm010Lser* data);
extern s32 func_ov011_02126354(BtlEnm010Lser* data);
extern s32 func_ov011_021265a8(BtlEnm010Lser* data);
extern s32 func_ov011_02125d48(BtlEnm010Lser* data, BtlEnm010LserArgs* args);
extern s32 func_ov003_020c3c28(void);
extern s32 func_ov003_020cc354(void* p);
extern s16 func_ov003_020843b0(s32 a, s32 b);
extern s32 func_ov003_020843ec(s32 a, s32 b, s32 c);
extern s32 func_ov003_02084348(s32 a, s16* b, s16* c, s32 d, s32 e, s32 f);
extern s32 func_ov003_020cbc50(s32* a, s32* b, u16 c, s32 d);
extern s32 func_ov003_02084348(s32 a0, s16* a1, s16* a2, s32 a3, s32 a4, s32 a5);
extern s32 func_ov003_020c3c28(void);
extern s32 func_ov003_020843ec(s32 a0, s32 a1, s32 a2);

#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

const TaskHandle data_ov011_0212c118 = {(const char*)data_ov011_0212cbe8,
                                        (s32(*)(struct TaskPool*, struct Task*, void*, s32))func_ov011_02125cf8, 596};

const s32 data_ov011_0212c124[3] = {-32768, -65536, 0};

char data_ov011_0212cbe8[20] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D,
                                0x30, 0x31, 0x30, 0x5F, 0x4C, 0x73, 0x65, 0x72, 0x00, 0x00};

s32 func_ov011_02125b98(void* arg0, s32 arg1) {
    BtlEnm010LserArgs args;
    TaskPool*         pool;

    if (func_ov003_020c37f8(&((BtlEnm010Owner*)arg0)->sprite) == 0) {
        pool = &data_ov003_020e71b8->unk_00000;
    } else {
        pool = &data_ov003_020e71b8->taskPool;
    }
    args.unk_00 = arg0;
    args.unk_14 = arg1;
    return EasyTask_CreateTask(pool, &data_ov011_0212c118, 0, 0, 0, &args);
}

void func_ov011_02125c00(s32* outX, s32* outY, s32* outZ, CombatActor* owner, s32 index) {
    *outX = (owner->isFlipped == 0) ? owner->position.x - 0x40000 : owner->position.x + 0x40000;
    *outY = owner->position.y;
    *outZ = owner->position.z + data_ov011_0212c124[index];
}

void func_ov011_02125c44(CombatActor* owner, BtlEnm010LserRec* rec, CombatSprite* sprite, s32 enabled) {
    s32 v = rec->unk_04 + rec->unk_08;

    rec->unk_04 = v;
    if (v >= 0x8000) {
        return;
    }
    if (enabled == 0) {
        return;
    }
    if (RNG_Next(0x64) < 0x32) {
        CombatSprite_SetAnimFromTable(sprite, 1, 1);
    } else {
        CombatSprite_SetAnimFromTable(sprite, 2, 1);
    }
    CombatSprite_Restart(sprite);
    rec->unk_00 = RNG_Next(0x4000);
    if (owner->isFlipped == 0) {
        rec->unk_00 = rec->unk_00 + 0x6000;
    } else {
        rec->unk_00 = rec->unk_00 - 0x2000;
    }
    rec->unk_04 = 0x20000;
    rec->unk_08 = 0 - (RNG_Next(0x1001) + 0x1000);
}

s32 func_ov011_02125cf8(s32 arg0, Task* task, s32 arg2, s32 cmd) {
    BtlEnm010Lser* data = task->data;

    switch (cmd) {
        case 0:
            return func_ov011_02125d48(data, arg2);
        case 1:
            return func_ov011_02125e14(data);
        case 2:
            return func_ov011_02126354(data);
        case 3:
            return func_ov011_021265a8(data);
        default:
            return 1;
    }
}

s32 func_ov011_02125d48(BtlEnm010Lser* data, BtlEnm010LserArgs* args) {
    s32           i;
    CombatSprite* sp;

    MI_CpuSet(data, 0, 0x254);
    sp = data->sprite;
    for (i = 0; i < 4; i++) {
        func_ov011_021258b4(args->unk_00->sprite.sprite.bits_0_1, sp, 2);
        sp++;
    }
    data->unk_00  = args->unk_00;
    data->unk_23C = 0;
    data->unk_240 = 0;
    data->unk_244 = args->unk_10;
    if (args->unk_00->actor.isFlipped == 0) {
        data->unk_244 = 0 - data->unk_244;
    }
    data->unk_24C = args->unk_14;
    if (args->unk_14 == 0) {
        data->emit.unk_28 = 0;
    } else {
        data->emit.unk_28 = 1;
    }
    if (func_ov003_020c37f8(&args->unk_00->sprite) != 0) {
        data->unk_250 |= 8;
    } else {
        data->unk_250 &= ~8;
    }
    return 1;
}

s32 func_ov011_02125e14(BtlEnm010Lser* data) {
    s32           result = 0;
    s32           i;
    CombatSprite* sp;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    if (data->unk_00 != NULL) {
        if (data->unk_00->actor.flags & 4) {
            return 0;
        }
    }
    if (data->unk_00 != NULL) {
        if (func_ov003_020cc354(&data->copy) != 0 || (data->copy.flags & 0x4000) != 0 || (data->copy.flags & 0x200) != 0) {
            data->unk_00 = NULL;
        }
    }
    if (data->unk_00 != NULL) {
        data->copy = data->unk_00->actor;
    }
    switch (data->emit.unk_28) {
        case 0:
            result = func_ov011_02125f24(data);
            break;
        case 1:
            result = func_ov011_02126064(data);
            break;
        case 2:
            result = func_ov011_021260e8(data);
            break;
    }
    sp = data->sprite;
    for (i = 0; i < 4; i++) {
        CombatSprite_Update(sp);
        sp++;
    }
    return result;
}

s32 func_ov011_02125f24(BtlEnm010Lser* data) {
    s32               flag;
    s32               i;
    CombatSprite*     sp;
    BtlEnm010LserRec* rec;

    if (data->unk_00 == NULL) {
        return 0;
    }
    if (data->emit.unk_24 == 0) {
        CombatSprite_SetAnimFromTable(&data->sprite[0], 1, 1);
        CombatSprite_SetAnimFromTable(&data->sprite[1], 1, 1);
        CombatSprite_SetAnimFromTable(&data->sprite[2], 1, 1);
        CombatSprite_SetAnimFromTable(&data->sprite[3], 3, 0);
        data->unk_250     = (data->unk_250 & ~1) | 1;
        data->unk_250     = data->unk_250 & ~2;
        data->emit.unk_26 = 0x2F;
        func_ov003_02087f00((SndMgrSeIdx)0x1E5, func_ov003_020843b0(0, data->copy.position.x));
    }
    flag = 1;
    if (data->emit.unk_24 > 0x24) {
        flag = 0;
    }
    sp  = data->sprite;
    rec = data->emit.rec;
    for (i = 0; i < 3; i++) {
        func_ov011_02125c44(&data->copy, rec, sp, flag);
        sp++;
        rec++;
    }
    if (data->emit.unk_24 == 8) {
        data->unk_250 |= 2;
    }
    if (data->emit.unk_24 < data->emit.unk_26) {
        data->emit.unk_24 = data->emit.unk_24 + 1;
    } else {
        data->emit.unk_24 = 0;
        data->emit.unk_28 = 2;
    }
    return 1;
}

s32 func_ov011_02126064(BtlEnm010Lser* data) {
    if (data->unk_00 == NULL) {
        return 0;
    }
    if (data->emit.unk_24 == 0) {
        CombatSprite_SetAnimFromTable(&data->sprite[3], 3, 0);
        data->unk_250     = data->unk_250 | 2;
        data->emit.unk_26 = 4;
    }
    if (data->emit.unk_24 < data->emit.unk_26) {
        data->emit.unk_24 = data->emit.unk_24 + 1;
    } else {
        data->emit.unk_24 = 0;
        data->emit.unk_28 = 2;
    }
    return 1;
}

s32 func_ov011_021260e8(BtlEnm010Lser* data) {
    s32 mode;

    if (((u32)(data->unk_250 << 28) >> 31) != 0) {
        mode = 1;
    } else {
        mode = 0;
    }
    if (data->emit.unk_24 == 0) {
        data->emit.unk_24 = data->emit.unk_24 + 1;
        CombatSprite_SetAnimFromTable(&data->sprite[0], 5, 0);
        CombatSprite_SetAnimFromTable(&data->sprite[3], 4, 1);
        func_ov011_02125c00(&data->unk_22C, &data->unk_230, &data->unk_234, &data->copy, data->unk_24C);
        if (data->copy.isFlipped == 0) {
            data->unk_248 = 0 - 0x8000;
            data->unk_22C = data->unk_22C - 0x28000;
        } else {
            data->unk_248 = 0x8000;
            data->unk_22C = data->unk_22C + 0x28000;
            CombatSprite_SetFlip(&data->sprite[0], 1);
            CombatSprite_SetFlip(&data->sprite[3], 1);
        }
        data->unk_250 &= ~1;
    }
    if (SpriteMgr_IsAnimationFinished(&data->sprite[3].sprite) != 0) {
        data->unk_250 &= ~2;
    }
    if (data->emit.unk_24 == 0x11) {
        data->unk_250 |= 4;
        func_ov003_02087f00(0x1DD, func_ov003_020843b0(mode, data->copy.position.x));
    }
    if ((u32)(data->unk_250 << 29) >> 31) {
        data->unk_22C = data->unk_22C + data->unk_248;
        if (func_ov003_020cc7f4(func_ov003_020843b0(mode, data->unk_22C), 0, 0x60, 0) == 0) {
            return 0;
        }
    }
    if (data->emit.unk_24 >= 0xA) {
        BtlEnm010CmdTbl rec;
        s32             x;
        s32             idx;

        if (func_ov003_020c37f8(&data->sprite[0]) != 0) {
            idx = 0x5E;
        } else {
            idx = 0x56;
        }
        rec = *func_ov003_0208a114((u16)idx);
        if (data->emit.unk_24 < 0xE) {
            rec.unk_05 = 0x18;
            rec.unk_07 = 0x20;
            x          = (data->copy.isFlipped == 0) ? data->unk_22C + 0x18000 : data->unk_22C - 0x18000;
        } else if (data->emit.unk_24 < 0x11) {
            rec.unk_05 = 0x20;
            rec.unk_07 = 0x10;
            x          = (data->copy.isFlipped == 0) ? data->unk_22C + 0x28000 : data->unk_22C - 0x18000;
        } else {
            x = data->unk_22C;
        }
        if (func_ov003_0208a164(&rec, &data->copy.unk_04, x, data->unk_230, data->unk_234) == 1) {
            func_ov003_02087f00(0x1DA, func_ov003_020843b0(mode, x));
        }
    }
    data->emit.unk_24 = data->emit.unk_24 + 1;
    return 1;
}

s32 func_ov011_02126354(BtlEnm010Lser* data) {
    s16               t1;
    s16               t0;
    s32               acc0;
    s32               acc1;
    s32               outX;
    s32               outY;
    s32               outZ;
    s32               vx;
    s32               vy;
    s32               i;
    BtlEnm010LserRec* rec;
    CombatSprite*     sp;
    s32               mode;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    if (func_ov003_020c37f8(&data->sprite[0]) != 0) {
        mode = 1;
    } else {
        mode = 0;
    }
    func_ov011_02125c00(&outX, &outY, &outZ, &data->copy, data->unk_24C);
    vx = ROUND(func_ov003_020843b0(mode, outX));
    vy = ROUND(func_ov003_020843ec(mode, outY, outZ));

    if ((u32)(data->unk_250 << 31) >> 31) {
        rec = &data->emit.rec[0];
        sp  = data->sprite;
        for (i = 0; i < 3; i++) {
            func_ov003_020cbc50(&acc0, &acc1, rec->unk_00, rec->unk_04);
            acc0 += vx;
            acc1 += vy;
            CombatSprite_SetPosition(sp, (acc0 * 16) >> 16, (acc1 * 16) >> 16);
            func_ov003_02082730(sp, 0x7FFFFFFE - outY);
            CombatSprite_Render(sp);
            rec++;
            sp++;
        }
    }

    if ((u32)(data->unk_250 << 30) >> 31) {
        CombatSprite_SetPosition(&data->sprite[3], (vx * 16) >> 16, (vy * 16) >> 16);
        func_ov003_02082730(&data->sprite[3], 0x7FFFFFFF - outY);
        CombatSprite_Render(&data->sprite[3]);
    }

    if ((u32)(data->unk_250 << 29) >> 31) {
        func_ov003_02084348(mode, &t1, &t0, data->unk_22C, data->unk_230, data->unk_234);
        CombatSprite_SetPosition(&data->sprite[0], t1, t0);
        func_ov003_02082730(&data->sprite[0], 0x7FFFFFFD - outY);
        CombatSprite_Render(&data->sprite[0]);
    }
    return 1;
}

s32 func_ov011_021265a8(BtlEnm010Lser* data) {
    s32           i;
    CombatSprite* sp;

    sp = data->sprite;
    for (i = 0; i < 4; i++) {
        CombatSprite_Release(sp);
        sp++;
    }
    return 1;
}
