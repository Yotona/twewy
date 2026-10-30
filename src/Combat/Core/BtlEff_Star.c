#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"

#include "Combat/Core/CombatApi.h"
typedef struct BtlStarTrail {
    s32 x, y;
    s16 timer;
    u16 pad;
    s32 active;
} BtlStarTrail;
typedef struct BtlEffStar {
    CombatSprite     sprites[4];
    Ov003TouchActor* owner;
    BtlStarTrail     trails[5];
    s8               level, count;
    s16              timer;
    s32              active;
} BtlEffStar;
typedef struct BtlEffStarArgs {
    Ov003TouchActor* owner;
    s8               level;
} BtlEffStarArgs;
extern const BinIdentifier data_ov003_020d7800, data_ov003_020d7810;
extern void                func_020265d4(s32* dst, s32 target, u16 weight);
void                       func_ov003_0209fd98(BtlEffStar*);
void                       func_ov003_0209fddc(BtlEffStar*);
void                       func_ov003_0209fe58(BtlEffStar*);
void                       func_ov003_0209fef0(BtlEffStar*);
const SpriteAnimEntry      data_ov003_020d870c     = {1, 3, 2, 0};
const SpriteAnimEntry      data_ov003_020d8714     = {0x1c, 0x1e, 0x1d, 0};
char                       data_ov003_020e3de8[16] = "Tsk_BtlEff_Star";
const TaskHandle           data_ov003_020d871c     = {data_ov003_020e3de8, func_ov003_020a01fc, 0x1dc};
const TaskStages data_ov003_020d8728 = {func_ov003_0209ff9c, func_ov003_020a00e8, func_ov003_020a0180, func_ov003_020a01cc};
void             func_ov003_0209fd48(s32 engine, CombatSprite* sprite) {
    CombatSprite_LoadDirect(engine, sprite, &data_ov003_020d7800, 0x19, 0x1b, 0x1a, 0x3f, 12);
    func_ov003_02082738(sprite, 0);
}
void func_ov003_0209fd98(BtlEffStar* data) {
    s32 i;
    data->count = 0;
    for (i = 0; i < 5; i++)
        data->trails[i].active = 0;
    func_ov003_0209fd48(1, &data->sprites[2]);
    func_ov003_0209fd48(0, &data->sprites[3]);
}
void func_ov003_0209fddc(BtlEffStar* data) {
    s32           max = 0;
    BtlStarTrail* trail;
    switch (data->level) {
        case 1:
            max = 3;
            break;
        case 2:
            max = 4;
            break;
        case 3:
            max = 5;
            break;
    }
    if (data->count >= max)
        return;
    trail         = &data->trails[data->count];
    trail->x      = 0x80000;
    trail->y      = 0;
    trail->timer  = 20;
    trail->active = 1;
    data->count++;
}
void func_ov003_0209fe58(BtlEffStar* data) {
    s32           x = data->owner->x, y = data->owner->y;
    s32           i;
    BtlStarTrail* trail;
    for (i = 0; i < data->count; i++) {
        trail = &data->trails[i];
        if (trail->active) {
            if (trail->timer > 0) {
                func_020265d4(&trail->x, x, (u16)trail->timer);
                func_020265d4(&trail->y, y + 0xc0000, (u16)trail->timer);
                trail->timer--;
            }
            if (trail->timer <= 0)
                trail->active = 0;
        }
    }
}
void func_ov003_0209fef0(BtlEffStar* data) {
    s32           i;
    BtlStarTrail* trail;
    for (i = 0; i < data->count; i++) {
        trail = &data->trails[i];
        if (trail->active) {
            if (trail->y < 0xd0000) {
                CombatSprite_SetPosition(&data->sprites[2], (s16)(trail->x >> 12), (s16)(trail->y >> 12));
                CombatSprite_Render(&data->sprites[2]);
            }
            if (trail->y > 0xb0000) {
                CombatSprite_SetPosition(&data->sprites[3], (s16)(trail->x >> 12), (s16)((trail->y >> 12) - 192));
                CombatSprite_Render(&data->sprites[3]);
            }
        }
    }
}
// Nonmatching: the descriptor flag halfword load and stores are scheduled differently.
// All assigned descriptor values and subsequent calls agree with retail.
s32 func_ov003_0209ff9c(TaskPool* pool, Task* task, void* args) {
    BtlEffStar*       data = task->data;
    BtlEffStarArgs*   p    = args;
    SpriteAnimationEx anim;
    MI_CpuSet(data, 0, sizeof(*data));
    data->level  = p->level;
    data->owner  = p->owner;
    data->timer  = 0;
    data->active = 1;
    CombatSprite_LoadFromTable(1, &data->sprites[0], &data_ov003_020d7800, &data_ov003_020d8714, 0, 0x3f, 0x10);
    CombatSprite_SetAnimFromTable(&data->sprites[0], 0, 1);
    func_ov003_02082738(&data->sprites[0], 0);
    CombatSprite_SetPosition(&data->sprites[0], 128, 0);
    CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020d7810);
    anim.anim.unk_18     = 2;
    anim.anim.unk_26     = 2;
    anim.anim.packIndex  = 1;
    anim.anim.unk_1C     = 1;
    anim.anim.animIndex  = 1;
    anim.anim.unk_1E     = 0x800;
    anim.anim.bits_10_11 = 1;
    anim.anim.unk_28     = 3;
    anim.anim.unk_20     = 4;
    anim.animTable       = &data_ov003_020d870c;
    CombatSprite_Load(&data->sprites[1], &anim);
    CombatSprite_SetAnimFromTable(&data->sprites[1], 0, 1);
    func_ov003_02082738(&data->sprites[1], 0);
    func_ov003_02082730(&data->sprites[1], 0x3e9000);
    func_ov003_0209fd98(data);
    return 1;
}
s32 func_ov003_020a00e8(TaskPool* pool, Task* task, void* args) {
    BtlEffStar* data = task->data;
    if (data_ov003_020e71b8->unk3D878 & 6)
        return 0;
    if (data->timer % 4 == 0)
        func_ov003_0209fddc(data);
    data->timer++;
    if (data->timer > 40)
        return 0;
    func_ov003_0209fe58(data);
    CombatSprite_Update(&data->sprites[0]);
    CombatSprite_Update(&data->sprites[1]);
    CombatSprite_Update(&data->sprites[2]);
    CombatSprite_Update(&data->sprites[3]);
    return 1;
}
s32 func_ov003_020a0180(TaskPool* pool, Task* task, void* args) {
    BtlEffStar* data = task->data;
    CombatSprite_Render(&data->sprites[0]);
    CombatSprite_SetPosition(&data->sprites[1], (s16)(data->owner->x >> 12), (s16)(data->owner->y >> 12));
    CombatSprite_Render(&data->sprites[1]);
    func_ov003_0209fef0(data);
    return 1;
}
s32 func_ov003_020a01cc(TaskPool* pool, Task* task, void* args) {
    BtlEffStar* data = task->data;
    CombatSprite_Release(&data->sprites[0]);
    CombatSprite_Release(&data->sprites[1]);
    CombatSprite_Release(&data->sprites[2]);
    CombatSprite_Release(&data->sprites[3]);
    return 1;
}
s32 func_ov003_020a01fc(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8728;
    return stages.iter[stage](pool, task, args);
}
s32 func_ov003_020a0244(s32 level, Ov003TouchActor* owner) {
    BtlEffStarArgs p;
    p.owner = owner;
    p.level = level;
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d871c, NULL, 0, NULL, &p);
}
