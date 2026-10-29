#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"

#include "Combat/Core/CombatApi.h"
typedef struct BtlEffLvUp {
    CombatSprite sprites[2];
} BtlEffLvUp;
extern const BinIdentifier data_ov003_020d7800;
extern s32                 func_ov003_020843b0(s32, s32);
void                       func_ov003_020a0290(s32, BtlEffLvUp*);
s32                        func_ov003_020a033c(BtlEffLvUp*);
void                       func_ov003_020a0368(BtlEffLvUp*);
void                       func_ov003_020a0418(BtlEffLvUp*);
char                       data_ov003_020e3df8[16] = "Tsk_BtlEff_LvUp";
const TaskHandle           data_ov003_020d8738     = {data_ov003_020e3df8, func_ov003_020a05a4, 0x180};
const TaskStages data_ov003_020d8744 = {func_ov003_020a0430, func_ov003_020a0490, func_ov003_020a0548, func_ov003_020a0568};
const SpriteAnimEntry data_ov003_020d8754[2] = {
    {4, 6, 5, 0},
    {4, 6, 5, 1}
};
// Nonmatching: use the loader's declared u16 palette parameters.
void func_ov003_020a0290(s32 engine, BtlEffLvUp* data) {
    s32 palette;
    if (engine == 0)
        palette = 0x3e;
    else
        palette = 0x40;
    CombatSprite_LoadFromTable(engine, &data->sprites[1], &data_ov003_020d7800, data_ov003_020d8754, 1, palette, 0x48);
    CombatSprite_SetAnimFromTable(&data->sprites[1], 1, 1);
    func_ov003_02082738(&data->sprites[1], 2);
    CombatSprite_LoadFromTable(engine, &data->sprites[0], &data_ov003_020d7800, data_ov003_020d8754, 0, palette, 0x34);
    CombatSprite_SetAnimFromTable(&data->sprites[0], 0, 1);
    func_ov003_02082738(&data->sprites[0], 2);
}
s32 func_ov003_020a033c(BtlEffLvUp* data) {
    CombatSprite_Update(&data->sprites[0]);
    CombatSprite_Update(&data->sprites[1]);
    return !SpriteMgr_IsAnimationFinished(&data->sprites[0].sprite);
}
void func_ov003_020a0368(BtlEffLvUp* data) {
    CombatActor* actor;
    s32          x;
    if (data->sprites[0].sprite.bits_0_1 == 0) {
        actor = data_ov003_020e71b8->unk3D898;
        x     = func_ov003_020843b0(0, actor->position.x);
    } else {
        actor = data_ov003_020e71b8->unk3D89C;
        x     = func_ov003_020843b0(1, actor->position.x);
    }
    CombatSprite_SetPosition(&data->sprites[1], x, 0x60);
    func_ov003_02082730(&data->sprites[1], 0x7fffffff - actor->position.y);
    CombatSprite_Render(&data->sprites[1]);
    CombatSprite_SetPosition(&data->sprites[0], x, 0x60);
    func_ov003_02082730(&data->sprites[0], 0);
    CombatSprite_Render(&data->sprites[0]);
}
void func_ov003_020a0418(BtlEffLvUp* data) {
    CombatSprite_Release(&data->sprites[0]);
    CombatSprite_Release(&data->sprites[1]);
}
s32 func_ov003_020a0430(TaskPool* pool, Task* task, void* args) {
    BtlEffLvUp* data = task->data;
    MI_CpuSet(data, 0, 0x180);
    data_ov003_020e71b8->unk3D878 |= 8;
    func_ov003_020a0290(0, &data[0]);
    func_ov003_020a0290(1, &data[1]);
    func_ov003_02087ed8(0x14f);
    return 1;
}
s32 func_ov003_020a0490(TaskPool* pool, Task* task, void* args) {
    BtlEffLvUp* data = task->data;
    s32         active, x;
    active = func_ov003_020a033c(&data[0]);
    func_ov003_020a033c(&data[1]);
    x = func_ov003_020843b0(0, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x);
    // Both effect attachment calls intentionally use the sub-screen actor +4 in retail.
    func_ov003_0208a1a4(func_ov003_0208a114(0x1f), &((CombatActor*)data_ov003_020e71b8->unk3D89C)->unk_04, x, 0);
    x = func_ov003_020843b0(1, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x);
    func_ov003_0208a1a4(func_ov003_0208a114(0x20), &((CombatActor*)data_ov003_020e71b8->unk3D89C)->unk_04, x, 0);
    return active;
}
s32 func_ov003_020a0548(TaskPool* pool, Task* task, void* args) {
    BtlEffLvUp* data = task->data;
    func_ov003_020a0368(&data[0]);
    func_ov003_020a0368(&data[1]);
    return 1;
}
s32 func_ov003_020a0568(TaskPool* pool, Task* task, void* args) {
    BtlEffLvUp* data = task->data;
    func_ov003_020a0418(&data[0]);
    func_ov003_020a0418(&data[1]);
    data_ov003_020e71b8->unk3D878 &= ~8;
    return 1;
}
s32 func_ov003_020a05a4(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8744;
    if (func_ov003_0208b690((u16)stage))
        return 1;
    return stages.iter[stage](pool, task, args);
}
