#include "Combat/Core/CombatActorParams.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Engine/Math/Random.h"
#include "Save.h"
#include <nitro/mi/cpumem.h>
#include <stddef.h>

typedef struct BtlPrizeLookup {
    s32 record, group;
} BtlPrizeLookup;
typedef struct BtlPrizeArgs {
    /* 0x00 */ s32 engine;
    /* 0x04 */ s8  grade;
    /* 0x05 */ u8  reserved[3];
    /* 0x08 */ Vec position;
    /* 0x14 */ s32 mode;
} BtlPrizeArgs;
typedef struct BtlPrizeItem {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          position; // Y is stored at triple world scale.
    /* 0x6C */ s16          phase;
    /* 0x6E */ s16          timer;
    /* 0x70 */ u32          reserved[2];
    /* 0x78 */ s32          targetZ;
    /* 0x7C */ s32          radius;
    /* 0x80 */ u32          reserved80;
    /* 0x84 */ u16          angle;
    /* 0x86 */ u16          reserved86;
} BtlPrizeItem;

typedef char                 check_PrizeArgs[(sizeof(BtlPrizeArgs) == 0x18) ? 1 : -1];
typedef char                 check_PrizeItem[(sizeof(BtlPrizeItem) == 0x88) ? 1 : -1];
typedef char                 check_PrizeItem_phase[(offsetof(BtlPrizeItem, phase) == 0x6C) ? 1 : -1];
typedef char                 check_PrizeItem_angle[(offsetof(BtlPrizeItem, angle) == 0x84) ? 1 : -1];
extern const BtlPrizeLookup  data_ov003_020dd228[13], data_ov003_020dd290[96];
extern const BinIdentifier   data_ov003_020d7800;
extern const TaskHandle      data_ov003_020dd3d8;
extern const SpriteAnimEntry data_ov003_020dd3c8, data_ov003_020dd3e4[5];
extern const BinIdentifier   data_ov003_020dd3d0;
typedef union BtlPrizeCount {
    u32 raw;
    struct {
        u16 count, reserved;
    };
} BtlPrizeCount;
extern BtlPrizeCount data_ov003_020e71c4;
extern s32           func_ov003_0208b1e8(u16);
extern s32           func_ov003_020c37f8(CombatSprite*);
extern s32           func_ov003_020c3c28(void);
extern void          func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern s32           func_ov003_020843b0(s32 index, s32 x);
extern void          func_ov003_0209c984(u32, s32, s32, s32, s32);
extern void          func_02026590(s32*, s32, u16);
extern void          func_020265d4(s32*, s32, u16);
s32                  func_ov003_020bb590(Ov003CutinActor*, s32);
s32                  func_ov003_020bb6d4(const Ov003ActorSpriteParams*);
void                 func_ov003_020bb73c(s32, s16);
void                 func_ov003_020bb7c4(s32, s16, s8);
s32                  func_ov003_020bb878(u32);
s32                  func_ov003_020bb8bc(void);

s32 func_ov003_020babf8(s32 group, u16 variant) {
    switch ((u32)(group - 0x11)) {
        case 0:
            if (variant != 1)
                return -1;
            break;
        case 1:
            if (variant != 1)
                return -1;
            break;
        case 4:
            if (variant != 5)
                return -1;
            break;
        case 6:
            if (variant != 0)
                return -1;
            break;
        case 7:
            if (variant != 1)
                return -1;
            break;
        case 8:
            if (variant != 0)
                return -1;
            break;
        case 9:
            if (variant != 1)
                return -1;
            break;
        case 10:
            if (variant != 1)
                return -1;
            break;
        case 11:
            if (variant != 0)
                return -1;
            break;
        case 12:
            if (variant != 0)
                return -1;
            break;
        case 2:
            return -1;
        case 3:
            return -1;
        case 5:
            return -1;
    }
    s32 index;
    for (index = 12; index >= 0; index--)
        if (group == data_ov003_020dd228[index].group)
            return data_ov003_020dd228[index].record;
    return -1;
}

// Nonmatching: record and variant switch layout.
s32 func_ov003_020bad24(s32 group, s16 variant) {
    u32 relativeBase;
    s16 adjusted = variant;
    s32 record   = 0;
    s32 index;
    for (index = 0; index < 96; index++) {
        if (group == data_ov003_020dd290[index].group) {
            record = data_ov003_020dd290[index].record;
            break;
        }
    }
    switch ((u32)(group - 0x11)) {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            record = func_ov003_020babf8(group, adjusted);
            if (record == -1)
                return -1;
            adjusted = 0;
            break;
        case 15:
            record = 0x11;
            break;
        case 42:
            record   = 0x56;
            adjusted = 0;
            break;
        case 24:
            return -1;
        case 25:
            return -1;
        case 18:
        case 19:
        case 20:
        case 21:
            record = 0x5C;
            break;
        case 28:
        case 29:
            record = 0x5B;
            break;
        case 38:
        case 39:
            record = 0x52;
            break;
        case 40:
        case 41:
            record = 0x50;
            break;
        case 23:
        case 30:
            record = 0x5F;
            break;
    }
    switch (record) {
        default:
            relativeBase = record - 0x30;
            switch (relativeBase) {
                default:
                    break;
                case 9:
                    if (adjusted == 3) {
                        adjusted = 0;
                    }
                    break;
                case 0:
                    switch (adjusted) {
                        case 2:
                            adjusted = 3;
                            break;
                        case 3:
                            adjusted = 2;
                            break;
                    }
                    break;
                case 15:
                    switch (adjusted) {
                        case 1:
                            adjusted = 2;
                            break;
                        case 2:
                            adjusted = 1;
                            break;
                    }
                    break;
                case 28:
                case 29:
                case 30:
                    switch (adjusted) {
                        case 3:
                            adjusted = 0;
                            break;
                        case 4:
                            adjusted = 1;
                            break;
                        case 5:
                            adjusted = 2;
                            break;
                    }
                    break;
                case 32:
                case 33:
                case 34:
                case 35:
                    if ((s32)adjusted > 0) {
                        adjusted -= 1;
                    }
                    break;
                case 31:
                case 36:
                case 37:
                case 38:
                case 41:
                case 42:
                case 43:
                case 44:
                case 45:
                case 46:
                case 47:
                    adjusted = 0;
                    break;
            }
            break;
        case 0:
            switch (adjusted) {
                default:
                    break;
                case 5:
                    adjusted = 8;
                    break;
                case 6:
                    adjusted = 5;
                    break;
                case 7:
                    adjusted = 6;
                    break;
                case 8:
                    adjusted = 7;
                    break;
                case 9:
                    adjusted = 1;
                    break;
                case 10:
                    adjusted = 4;
                    break;
                case 11:
                    adjusted = 0;
                    break;
            }
            break;
        case 19:
            switch (adjusted) {
                case 3:
                    adjusted = 2;
                    break;
                case 4:
                    adjusted = 3;
                    break;
            }
            break;
        case 44:
            switch (adjusted) {
                case 2:
                    adjusted = 3;
                    break;
                case 3:
                    adjusted = 2;
                    break;
            }
            break;
        case 32:
            switch (adjusted) {
                case 4:
                    adjusted = 5;
                    break;
                case 5:
                    adjusted = 4;
                    break;
            }
            break;
        case 28:
            switch (adjusted) {
                case 2:
                    adjusted = 1;
                    break;
                case 1:
                    adjusted = 2;
                    break;
            }
            break;
    }
    return record + adjusted;
}

void func_ov003_020bb0f0(s32 group, s16 variant) {
    s32 record = func_ov003_020bad24(group, variant);

    if (record == -1) {
        return;
    }
    if (gSaveData.battleDifficulty >= 1) {
        if (!gSaveData.unk_326A[record].unk_2 || !gSaveData.unk_326A[record].minimumBattleLevel ||
            gSaveData.battleLevel < gSaveData.unk_326A[record].minimumBattleLevel)
        {
            gSaveData.unk_326A[record].minimumBattleLevel = gSaveData.battleLevel;
        }
    }
    gSaveData.unk_326A[record].unk_2++;
    if ((s16)gSaveData.unk_326A[record].unk_2 > 9999) {
        gSaveData.unk_326A[record].unk_2 = 9999;
    }
}

// Nonmatching: best-time selection switch and register allocation.
void func_ov003_020bb190(s32 group, u32 time, s32 unused) {
    s32 record;
    switch (group) {
        case 7:
            record = 0x57;
            break;
        case 9:
            record = 0x59;
            break;
        case 0x13:
            record = 0x5A;
            break;
        case 0xF:
            record = 0x5B;
            break;
        case 0xE:
            record = 0x5C;
            break;
        case 0x14:
        case 0x15:
            record = 0x5D;
            break;
        case 0x24:
            record = 0x15;
            break;
        case 0x49:
            record = 0x2B;
            break;
        case 0x30:
            record = 0x41;
            break;
        case 0x31:
            record = 0x2E;
            break;
        case 8:
            record = 0x58;
            break;
        case 0x1B:
            record = 0x56;
            break;
        case 0x21:
            record = 0x55;
            break;
        case 0x1F:
            record = 0x51;
            break;
        case 0x1D:
            record = 0x50;
            break;
        case 0x1C:
            record = 0x52;
            break;
        case 0x17:
        case 0x18:
        case 0x19:
        case 0x1A:
            record = 0x4F;
            break;
        case 0x16:
            record = 0x5E;
            break;
        case 0x20:
            record = 0x5F;
            break;
        default:
            return;
    }
    if (gSaveData.battleDifficulty >= 2 && (!gSaveData.bestDefeatTimes[record] || time < gSaveData.bestDefeatTimes[record]))
        gSaveData.bestDefeatTimes[record] = time;
    if (group == 0xF && gSaveData.battleDifficulty >= 2 &&
        (!gSaveData.bestDefeatTimes[0x54] || time < gSaveData.bestDefeatTimes[0x54]))
        gSaveData.bestDefeatTimes[0x54] = time;
    if (group != 0x1F)
        return;
    if (gSaveData.battleDifficulty < 2)
        return;
    if (gSaveData.bestDefeatTimes[0x53] && time >= gSaveData.bestDefeatTimes[0x53])
        return;
    gSaveData.bestDefeatTimes[0x53] = time;
}

void func_ov003_020bb3e4(s32 group, s16 variant, u32 grade) {
    s32 record = func_ov003_020bad24(group, variant);
    if (record == -1)
        return;
    switch (grade) {
        case 0:
            gSaveData.unk_326A[record].dropFlags |= 8;
            return;
        case 1:
            gSaveData.unk_326A[record].dropFlags |= 4;
            return;
        case 2:
            gSaveData.unk_326A[record].dropFlags |= 2;
            return;
        case 3:
            gSaveData.unk_326A[record].dropFlags |= 1;
            return;
    }
}

s32 func_ov003_020bb47c(void) {
    return data_ov003_020e71c4.count;
}

// Nonmatching: creator argument scheduling.
s32 func_ov003_020bb48c(s32 engine, s8 grade, u16 item, s32 x, s32 y, s32 z, s32 special) {
    if (!func_ov003_0208b1e8(item))
        return -1;
    BtlPrizeArgs args;
    MI_CpuSet(&args, 0, sizeof(args));
    if (engine == 2)
        engine = 0;
    args.engine     = engine;
    args.grade      = grade;
    args.position.x = x;
    args.position.y = y;
    args.position.z = z;
    if (item == 0x105)
        args.mode = 2;
    else if (special)
        args.mode = 1;
    else
        args.mode = 0;
    TaskPool* pool;
    if (args.engine == 0)
        pool = &data_ov003_020e71b8->unk_00000;
    else
        pool = &data_ov003_020e71b8->taskPool;
    return EasyTask_CreateTask(pool, &data_ov003_020dd3d8, NULL, 0, NULL, &args);
}

void func_ov003_020bb570(Ov003CutinActor* actor) {
    func_ov003_020bb590(actor, 0);
}
void func_ov003_020bb580(void* actor) {
    func_ov003_020bb590(actor, 1);
}

// Nonmatching: actor/drop-selection register allocation.
s32 func_ov003_020bb590(Ov003CutinActor* actor, s32 spawn) {
    s32 result = -1;
    if (actor->group < 0 || actor->group >= 60)
        return -1;
    func_ov003_020bb73c(actor->group, actor->spriteIndex);
    const Ov003ActorSpriteParams* params = data_ov003_020e70ac[actor->group][actor->spriteIndex];
    s32                           grade  = func_ov003_020bb6d4(params);
    if (grade == -1)
        return -1;
    if (spawn) {
        s32 special = 0;
        if (actor->group >= 0x11 && actor->group <= 0x1D)
            special = 1;
        s32 engine = func_ov003_020c37f8(&actor->sprite) ? 1 : 0;
        result     = func_ov003_020bb48c(engine, func_ov003_020bb878(grade), params->dropItems[grade], actor->actor.position.x,
                                         actor->actor.position.y, actor->actor.position.z, special);
        if (result != -1)
            func_ov003_020bb7c4(actor->group, actor->spriteIndex, func_ov003_020bb878(grade));
        return result;
    }
    if (!func_ov003_0208b1e8(params->dropItems[grade]))
        return -1;
    func_ov003_020bb7c4(actor->group, actor->spriteIndex, func_ov003_020bb878(grade));
    return result;
}

s32 func_ov003_020bb6d4(const Ov003ActorSpriteParams* params) {
    s8 grade = func_ov003_020bb8bc();
    do {
        s32 rate = (u16)data_ov003_020e71b8->unk3D892 * params->dropRates[grade];
        if (rate > RNG_Next(10000))
            return grade;
        grade--;
    } while (grade >= 0);
    return -1;
}

void func_ov003_020bb73c(s32 group, s16 variant) {
    if (data_ov003_020e71b8->unk3D88E == 0x1E) {
        func_ov003_020bb0f0(0x37, 0);
        func_ov003_020bb0f0(0x39, 0);
        return;
    }
    if (data_ov003_020e71b8->unk3D88E == 0x1F) {
        func_ov003_020bb0f0(0x37, 2);
        func_ov003_020bb0f0(0x39, 2);
        return;
    }
    func_ov003_020bb0f0(group, variant);
    if ((u32)(group - 0x2D) > 1)
        return;
    func_ov003_020bb0f0(0x2B, variant);
}

// Nonmatching: drop-flag helper call layout.
void func_ov003_020bb7c4(s32 group, s16 variant, s8 grade) {
    if (group >= 0x11 && group <= 0x1D) {
        func_ov003_020bb3e4(group, variant, func_ov003_020bb878(0));
        func_ov003_020bb3e4(group, variant, func_ov003_020bb878(1));
        func_ov003_020bb3e4(group, variant, func_ov003_020bb878(2));
        func_ov003_020bb3e4(group, variant, func_ov003_020bb878(3));
        return;
    }
    func_ov003_020bb3e4(group, variant, grade);
    if ((u32)(group - 0x2D) > 1)
        return;
    func_ov003_020bb3e4(0x2B, variant, grade);
}

s32 func_ov003_020bb878(u32 grade) {
    switch (grade) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        default:
            return 0;
    }
}
s32 func_ov003_020bb8bc(void) {
    switch (gSaveData.battleDifficulty) {
        case 0:
            return 0;
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        default:
            return 0;
    }
}

void func_ov003_020bb90c(BtlPrizeItem* self) {
    CombatActor* owner;
    if (func_ov003_020c37f8(&self->sprite))
        owner = data_ov003_020e71b8->unk3D89C;
    else
        owner = data_ov003_020e71b8->unk3D898;
    FX_Atan2Idx(self->position.x - owner->position.x, self->position.y - FX_Mul(owner->position.y, 0x3000));
    s32 dy       = self->position.y - FX_Mul(owner->position.y, 0x3000);
    s32 dx       = self->position.x - owner->position.x;
    u16 oldPhase = self->phase;
    s32 distance = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    u16 phase    = self->radius >= distance - 0xC000 ? 1 : 0;
    self->phase  = phase;
    if (self->phase == 1 && oldPhase == 0)
        self->radius = distance;
}

// Nonmatching: sprite-loading and fixed-point register allocation.
s32 func_ov003_020bba2c(BtlPrizeItem* self, BtlPrizeArgs* args) {
    CombatActor* owner;
    if (args->engine == 0)
        owner = data_ov003_020e71b8->unk3D898;
    else
        owner = data_ov003_020e71b8->unk3D89C;
    MI_CpuSet(self, 0, sizeof(*self));
    self->position.x = args->position.x;
    self->position.y = FX_Mul(args->position.y, 0x3000);
    self->position.z = args->position.z;
    // Retail callers pass grade 0..3.
    s32 animation;
    switch (args->grade) {
        case 0:
            animation = 0;
            break;
        case 1:
            animation = 1;
            break;
        case 2:
            animation = 2;
            break;
        case 3:
            animation = 3;
            break;
    }
    if (args->mode == 2) {
        CombatSprite_LoadFromTable(args->engine, &self->sprite, &data_ov003_020dd3d0, &data_ov003_020dd3c8, 0, 4, 4);
        CombatSprite_SetAnimFromTable(&self->sprite, 0, 0);
    } else {
        if (args->mode == 1)
            animation = 4;
        CombatSprite_LoadFromTable(args->engine, &self->sprite, &data_ov003_020d7800, data_ov003_020dd3e4, animation, 0x3D, 4);
        CombatSprite_SetAnimFromTable(&self->sprite, animation, 0);
    }
    self->radius = 0x50000;
    self->timer  = 0xB4;
    self->angle  = FX_Atan2Idx(self->position.x - owner->position.x, self->position.y - FX_Mul(owner->position.y, 0x3000));
    func_ov003_020bb90c(self);
    data_ov003_020e71c4.count++;
    return 1;
}

// Nonmatching: orbit/approach arithmetic and register allocation.
s32 func_ov003_020bbc14(BtlPrizeItem* self) {
    CombatActor* owner;
    if (func_ov003_020c37f8(&self->sprite))
        owner = data_ov003_020e71b8->unk3D89C;
    else
        owner = data_ov003_020e71b8->unk3D898;
    s32 active = 1;
    if (func_ov003_020c3c28())
        return 1;
    func_ov003_020bb90c(self);
    switch (self->phase) {
        case 0: {
            self->angle =
                FX_Atan2Idx(self->position.x - owner->position.x, self->position.y - FX_Mul(owner->position.y, 0x3000));
            s16 cosine = FX_CosIdx(self->angle);
            if (cosine < 0)
                cosine = -cosine;
            s32 speed = FX_Mul(cosine + 0x1000, 0xC000);
            self->position.x -= FX_Mul(FX_SinIdx(self->angle), speed);
            self->position.y -= FX_Mul(FX_CosIdx(self->angle), speed);
            break;
        }
        case 1:
            func_02026590(&self->radius, 0, self->timer);
            self->timer--;
            self->position.x = owner->position.x + FX_Mul(FX_SinIdx(self->angle), self->radius);
            self->position.y = FX_Mul(owner->position.y, 0x3000) + FX_Mul(FX_CosIdx(self->angle), self->radius);
            self->angle -= 0x7FF;
            break;
    }
    s32 height    = owner->unk_72 >> 1;
    s32 fixed     = height << 12;
    self->targetZ = owner->position.z - (s32)(height > 0 ? 0.5f + (f32)fixed : (f32)fixed - 0.5f);
    func_020265d4(&self->position.z, self->targetZ, self->timer);
    if (self->timer < 0)
        active = 0;
    CombatSprite_Update(&self->sprite);
    return active;
}

s32 func_ov003_020bbec0(BtlPrizeItem* self) {
    s32 engine = func_ov003_020c37f8(&self->sprite) ? 1 : 0;
    if (func_ov003_020c3c28())
        return 1;
    s16 x, y;
    func_ov003_02084348(engine, &x, &y, self->position.x, FX_Divide(self->position.y, 0x3000), self->position.z);
    CombatSprite_SetPosition(&self->sprite, x, y);
    func_ov003_02082730(&self->sprite, 0x7FFFFFFF - FX_Divide(self->position.y, 0x3000));
    CombatSprite_Render(&self->sprite);
    return 1;
}

s32 func_ov003_020bbf5c(BtlPrizeItem* self) {
    if ((u16)(data_ov003_020e71b8->unk3D874 + 0xFFFE) <= 1) {
        s32 pan;

        func_ov003_0209c984(self->sprite.sprite.bits_0_1, 1, self->position.x, FX_Divide(self->position.y, 0x3000),
                            self->position.z);
        if (self->sprite.sprite.bits_0_1 == 0) {
            pan = func_ov003_020843b0(0, self->position.x);
        } else {
            pan = func_ov003_020843b0(1, self->position.x);
        }
        func_ov003_02087f00(SEIDX_SE_COIN_GET, pan);
    }
    CombatSprite_Release(&self->sprite);
    data_ov003_020e71c4.count--;
    return 1;
}

s32 func_ov003_020bc018(TaskPool* pool, Task* task, void* args, s32 stage) {
    BtlPrizeItem* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020bba2c(self, args);
        case 1:
            return func_ov003_020bbc14(self);
        case 2:
            return func_ov003_020bbec0(self);
        case 3:
            return func_ov003_020bbf5c(self);
        default:
            return 1;
    }
}

char                  data_ov003_020e50a0[20] = "Tsk_BtlPrizeItem";
char                  data_ov003_020e50b4[32] = "Apl_Hor/Grp_BtlPrize_Lime.bin";
const SpriteAnimEntry data_ov003_020dd3c8     = {1, 3, 2, 0};
const BinIdentifier   data_ov003_020dd3d0     = {3, data_ov003_020e50b4};
const TaskHandle      data_ov003_020dd3d8     = {data_ov003_020e50a0, func_ov003_020bc018, sizeof(BtlPrizeItem)};
const SpriteAnimEntry data_ov003_020dd3e4[5]  = {
    {0x37, 0x39, 0x38, 0},
    {0x37, 0x39, 0x38, 1},
    {0x37, 0x39, 0x38, 2},
    {0x37, 0x39, 0x38, 3},
    {0x37, 0x39, 0x38, 4}
};
BtlPrizeCount data_ov003_020e71c4;
