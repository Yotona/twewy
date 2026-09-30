#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Fusion/BtlFusion.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/Input.h"

#include <nitro/mi/cpumem.h>

extern s16  func_ov003_020843b0(s32, s32);
extern s16  func_ov003_020843ec(s32, s32, s32);
extern void func_ov021_020f11bc(s32, s32*, s32*, s32*);
extern void func_ov021_020f13b0(void);
extern s32  func_ov021_020f1174(void);
extern void func_ov003_02083ab0(s32, s32, s32, s32);
extern s32  func_ov003_0208b690(u16);

s32  func_ov007_020e7478(TaskPool*, Task*, void*);
s32  func_ov007_020e7534(TaskPool*, Task*, void*);
s32  func_ov007_020e756c(TaskPool*, Task*, void*);
s32  func_ov007_020e7574(TaskPool*, Task*, void*);
s32  func_ov007_020e757c(TaskPool*, Task*, void*, s32);
void func_ov007_020e7360(BtlPlayerLast*, void (*)(BtlPlayerLast*));
void func_ov007_020e7374(BtlPlayerLast*, u16);
void func_ov007_020e73d8(BtlPlayerLast*);

const TaskHandle Tsk_BtlPlayerLast = {"Tsk_BtlPlayerLast", func_ov007_020e757c, 0x84};

void func_ov007_020e7360(BtlPlayerLast* data, void (*callback)(BtlPlayerLast*)) {
    data->unk_7C = callback;
    data->unk_80 = 0;
    data->unk_82 = 0;
}

void func_ov007_020e7374(BtlPlayerLast* data, u16 arg) {
    func_ov021_020f11bc((s16)arg, &data->actor.position.x, &data->actor.position.y, &data->actor.position.z);
    data->actor.screenX           = func_ov003_020843b0(1, data->actor.position.x);
    data->actor.screenY           = func_ov003_020843ec(1, data->actor.position.y, data->actor.position.z);
    data_ov003_020e71b8->unk3D8EC = 2;
    func_ov021_020f13b0();
}

void func_ov007_020e73d8(BtlPlayerLast* data) {
    s32 state = func_ov021_020f1174();

    switch (state) {
        case 0:
            if (InputStatus.buttonState.pressedButtons & 0x820) {
                func_ov007_020e7374(data, (u16)state);
            }
            break;
        case 2:
            if (InputStatus.buttonState.pressedButtons & 0x11) {
                func_ov007_020e7374(data, (u16)state);
            }
            break;
        case 1:
            if (InputStatus.buttonState.pressedButtons & 0x4C2) {
                func_ov007_020e7374(data, (u16)state);
            }
            break;
    }
}

s32 func_ov007_020e7478(TaskPool* pool, Task* task, void* args) {
    BtlPlayerLast* data = task->data;

    MI_CpuSet(data, 0, sizeof(BtlPlayerLast));
    CombatActor_Init(&data->actor, 0);
    data_ov003_020e71b8->unk3D89C = data;
    data->actor.isFlipped         = FALSE;
    data->actor.position.x        = data_ov003_020e71b8->unk3D7C0[1].unk_20;
    data->actor.position.y        = data_ov003_020e71b8->unk3D7C0[1].unk_24;
    data->actor.position.z        = 0;
    data->actor.zGravity          = 0x800;
    data->actor.unk_70            = 0xC;
    data->actor.unk_72            = 0x30;
    data->actor.unk_76            = 0x18;
    data->actor.flags |= 0x10;
    func_ov003_02083ab0(1, data->actor.position.x, data->actor.position.y, data->actor.position.z);
    func_ov007_020e7360(data, func_ov007_020e73d8);
    return 1;
}

s32 func_ov007_020e7534(TaskPool* pool, Task* task, void* args) {
    BtlPlayerLast* data = task->data;

    if (data->unk_7C != NULL) {
        data->unk_7C(data);
    }
    CombatActor_UpdateEffects(1, &data->actor);
    CombatActor_UpdatePhysics(&data->actor);
    return 1;
}

s32 func_ov007_020e756c(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov007_020e7574(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov007_020e757c(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = func_ov007_020e7478,
        .update     = func_ov007_020e7534,
        .render     = func_ov007_020e756c,
        .cleanup    = func_ov007_020e7574,
    };

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
