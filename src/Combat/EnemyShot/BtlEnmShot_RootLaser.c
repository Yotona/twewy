#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Combat/Core/CombatList.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

// Rounds `v` (20.12 fixed point built from an integer) to the nearest integer the way the original build does.
#define ROUND_FX(v) ((v) > 0 ? 0.5f + (f32)((v) << 12) : (f32)((v) << 12) - 0.5f)

/// The task parameter: the shot to snapshot, the root shot it repeats and the placement ranges of the roots.
typedef struct Ov003EnemyShotRootLaserArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   ownerAnimation;
    /* 0x06 */ u16                   reserved06;
    /* 0x08 */ Ov003EnemyShotSource* childSource;
    /* 0x0C */ u16                   childAnimation;
    /* 0x0E */ u16                   kind;
    /* 0x10 */ s16                   animation; // flash animation, < 0 for none
    /* 0x12 */ s16                   spacing;
    /* 0x14 */ s16                   horizontalMin;
    /* 0x16 */ s16                   horizontalMax;
    /* 0x18 */ s16                   verticalMin;
    /* 0x1A */ s16                   verticalMax;
    /* 0x1C */ s32                   speed;
    /* 0x20 */ s16                   delay;
    /* 0x22 */ s16                   length;
    /* 0x24 */ u16                   override;
    /* 0x26 */ u16                   reserved26;
    /* 0x28 */ SndMgrSeIdx           sound;
    /* 0x2C */ SndMgrSeIdx           sound2;
} Ov003EnemyShotRootLaserArgs; // Size: 0x30

typedef struct Ov003EnemyShotRootLaser {
    /* 0x00 */ Ov003EnemyShotSource owner;  // the source snapshot
    /* 0x2C */ Ov003EnemyShotSource source; // the root shot source
    /* 0x58 */ u16                  kind;
    /* 0x5A */ u16                  reserved5A;
    /* 0x5C */ CombatSprite         sprite;
    /* 0xBC */ s16                  spacing;
    /* 0xBE */ s16                  horizontalMin;
    /* 0xC0 */ s16                  horizontalMax;
    /* 0xC2 */ s16                  verticalMin;
    /* 0xC4 */ s16                  verticalMax;
    /* 0xC6 */ u16                  reservedC6;
    /* 0xC8 */ s32                  speed;
    /* 0xCC */ s16                  delay;
    /* 0xCE */ s16                  length;
    /* 0xD0 */ CombatList           list; // the roots placed so far
    /* 0xD8 */ s16                  timer;
    /* 0xDA */ s16                  state;
    /* 0xDC */ s16                  override; // 2 forces the next root downwards, other non-zero values upwards
    /* 0xDE */ u16                  reservedDE;
    /* 0xE0 */ SndMgrSeIdx          sound;
    /* 0xE4 */ SndMgrSeIdx          sound2;
} Ov003EnemyShotRootLaser; // Size: 0xE8

extern void             func_ov003_02084348(s32, s16*, s16*, s32, s32, s32);
extern void             func_ov003_02087f00(SndMgrSeIdx seIdx, s32 sePan);
extern s32              func_ov003_020c3c28(void);
extern s32              func_ov003_020c37f8(CombatSprite*);
extern const TaskHandle data_ov003_020e0ca4;

void func_ov003_020d1538(Ov003EnemyShotRootLaserArgs* args) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &data_ov003_020e0ca4, NULL, 0, NULL, args);
}

/// Lays the roots of the beam across the screen from the shot's position, alternating a vertical and a horizontal
/// root, and stops once the next root would fall outside the camera.
s32 func_ov003_020d1570(Ov003EnemyShotRootLaser* self) {
    Ov003CutinActor* target = data_ov003_020e71b8->unk3D898;
    Ov003Camera*     camera = &data_ov003_020e71b8->unk3D7C0[0];
    u16              parity = 0;
    s32              scale;
    s32              x, y, z;
    s32              limit;
    s32              half;
    u16              angle;
    s32              result;

    half  = (s32)ROUND_FX(target->actor.unk_72);
    z     = self->owner.core.position.z;
    x     = self->owner.core.position.x;
    y     = self->owner.core.position.y;
    limit = target->actor.position.y - (half >> 1) - z;
    while (x <= camera->unk_0C && x >= 0) {
        parity = (u16)((parity + 1) % 2);
        if (parity == 0) {
            f32 jitter = RNG_Next(self->verticalMax - self->verticalMin + 1) > 0
                             ? 0.5f + (f32)(RNG_Next(self->verticalMax - self->verticalMin + 1) << 12)
                             : (f32)(RNG_Next(self->verticalMax - self->verticalMin + 1) << 12) - 0.5f;
            scale      = (s32)ROUND_FX(self->verticalMin) + (s32)jitter;
            angle      = (y < limit) ? 0xC000 : 0x4000;
            if (self->override != 0) {
                angle          = (self->override == 2) ? 0xC000 : 0x4000;
                self->override = 0;
            }
            result = func_ov003_020d1228(&self->source, &self->list, angle, x, y, z, 0x1000, scale);
            if (angle == 0xC000) {
                x += FX_Mul(scale, (s32)ROUND_FX(self->spacing));
            } else {
                x += FX_Mul(scale, (s32)ROUND_FX(-self->spacing));
            }
        } else {
            f32 jitter = RNG_Next(self->horizontalMax - self->horizontalMin + 1) > 0
                             ? 0.5f + (f32)(RNG_Next(self->horizontalMax - self->horizontalMin + 1) << 12)
                             : (f32)(RNG_Next(self->horizontalMax - self->horizontalMin + 1) << 12) - 0.5f;
            scale      = (s32)ROUND_FX(self->horizontalMin) + (s32)jitter;
            angle      = (self->owner.core.owner->sprite.flags46 & 1) ? 0x8000 : 0;
            result     = func_ov003_020d1228(&self->source, &self->list, angle, x, y, z, scale, 0x1000);
            if (angle == 0x8000) {
                x += FX_Mul(scale, (s32)ROUND_FX(self->spacing));
            } else {
                x += FX_Mul(scale, (s32)ROUND_FX(-self->spacing));
            }
        }
        if (result == -1)
            return 0;
    }
    return 1;
}

s32 func_ov003_020d19b0(Ov003EnemyShotRootLaser* self, Ov003EnemyShotRootLaserArgs* args) {
    MI_CpuSet(self, 0, sizeof(*self));
    MI_CpuCopyU8(args->source, &self->owner, sizeof(Ov003EnemyShotSource));
    self->owner.animation = args->ownerAnimation;
    MI_CpuCopyU8(args->childSource, &self->source, sizeof(Ov003EnemyShotSource));
    self->source.animation = args->childAnimation;
    self->kind             = args->kind;
    self->spacing          = args->spacing;
    self->horizontalMin    = args->horizontalMin;
    self->horizontalMax    = args->horizontalMax;
    self->verticalMin      = args->verticalMin;
    self->verticalMax      = args->verticalMax;
    self->speed            = args->speed;
    self->delay            = args->delay;
    self->length           = args->length;
    self->override         = args->override;
    self->sound            = args->sound;
    self->sound2           = args->sound2;
    func_ov003_020849ac(&self->list);
    if (args->animation >= 0) {
        const Ov003EnemyShotVisual* visual = args->childSource->visual;
        const SpriteAnimEntry*      entry  = &visual->entries[visual->entry];
        CombatSprite_LoadDirect(2, &self->sprite, visual->file, entry->charDataIndex, entry->paletteDataIndex,
                                entry->frameDataIndex, visual->paletteSlot, entry->animDataIndex);
        CombatSprite_SetAnim(&self->sprite, args->animation, self->source.animationMode);
    }
    func_ov003_020d1570(self);
    self->timer = self->delay;
    return 1;
}

s32 func_ov003_020d1ae8(Ov003EnemyShotRootLaser* self) {
    if (func_ov003_020c3c28())
        return 0;
    switch (self->state) {
        case 0: {
            s32 flags = self->owner.core.owner->actor.flags;
            if ((flags & 2) || (flags & 4) || (flags & 0x4000) || (flags & 0x200))
                return 0;
            self->timer--;
            if (self->timer >= 0)
                break;
            if (!SndMgr_IsSEPlaying(self->sound) && self->sound != 0x56C)
                func_ov003_02087f00(self->sound, func_ov003_020843b0(0, self->owner.core.position.x));
            if (!SndMgr_IsSEPlaying(self->sound2) && self->sound2 != 0x56C)
                func_ov003_02087f00(self->sound2, func_ov003_020843b0(0, self->owner.core.position.x));
            Ov003EnemyShotRoot* root = func_ov003_0208495c(&self->list);
            if (root == NULL)
                return 0;
            func_ov003_020d07a8(&self->owner, &self->list, root, self->kind, self->spacing,
                                self->horizontalMax <= self->verticalMax ? self->verticalMax : self->horizontalMax,
                                self->speed, self->length);
            self->state++;
            break;
        }
        case 1:
            if (func_ov003_0208495c(&self->list) == NULL)
                return 0;
            break;
    }
    if (self->sprite.sprite.resourceData != NULL) {
        CombatSprite_Update(&self->sprite);
        if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite)) {
            CombatSprite_Release(&self->sprite);
            self->sprite.sprite.resourceData = NULL;
        }
    }
    return 1;
}

s32 func_ov003_020d1c98(Ov003EnemyShotRootLaser* self) {
    s16 x, y;
    s32 rootX, rootY, rootZ;
    if (self->sprite.sprite.resourceData != NULL) {
        Ov003EnemyShotRoot* root = func_ov003_0208495c(&self->list);
        if (root == NULL)
            return 1;
        func_ov003_020d11b8(root, NULL, NULL, NULL, &rootX, &rootY, &rootZ);
        func_ov003_02084348(0, &x, &y, rootX, rootY, rootZ);
        CombatSprite_SetPosition(&self->sprite, x, y);
        func_ov003_02082730(&self->sprite, 0);
        CombatSprite_Render(&self->sprite);
    }
    return 1;
}

s32 func_ov003_020d1d44(Ov003EnemyShotRootLaser* self) {
    func_ov003_020d117c(&self->list);
    if (self->sprite.sprite.resourceData != NULL)
        CombatSprite_Release(&self->sprite);
    return 1;
}

s32 func_ov003_020d1d70(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003EnemyShotRootLaser* self = task->data;
    switch (stage) {
        case 0:
            return func_ov003_020d19b0(self, args);
        case 1:
            return func_ov003_020d1ae8(self);
        case 2:
            return func_ov003_020d1c98(self);
        case 3:
            return func_ov003_020d1d44(self);
        default:
            return 1;
    }
}

char             data_ov003_020e6d2c[28] = "Tsk_BtlEnmShot_RootLaser";
const TaskHandle data_ov003_020e0ca4     = {data_ov003_020e6d2c, func_ov003_020d1d70, sizeof(Ov003EnemyShotRootLaser)};

/// Fires `count` copies of the shot on a circle of `radius`, spread `step` apart (in angle units) and centred on `angle`.
void func_ov003_020d1dc0(Ov003EnemyShotSource* source, s32 radius, u16 angle, u16 step, u16 count) {
    s32 i;
    s32 n = count;
    s32 current;

    current = (n % 2 != 0) ? angle - ((n - 1) >> 1) * step : angle - (n >> 1) * step - (step >> 1);
    if (n <= 0)
        return;
    for (i = 0; i < n; i++) {
        func_ov003_020d1ec4(source, FX_Mul(FX_CosIdx((u16)current), radius), FX_Mul(FX_SinIdx((u16)current), radius), 0, 1);
        current = (u16)(current + step);
    }
}

/// Thin wrapper that forwards to the beam builder with the trailing arguments zeroed.
s32 func_ov003_020d1ec4(Ov003EnemyShotSource* source, s32 a1, s32 a2, s32 a3, s32 a4) {
    return func_ov003_020d1eec(source, a1, a2, a3, a4, 0, 0);
}
