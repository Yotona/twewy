#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "CriSndMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Save.h"
#include "SndMgr.h"

// The pause menu: three sprites (the frame, the "PAUSED" banner and the resuming banner) that
// are shown while the battle is paused with START. The task also pauses and resumes the BGM
// stream through `CriSndMgr_Pause` and swallows the touch input while the menu is up.

typedef struct BtlPause {
    /* 0x000 */ CombatSprite sprites[3];
    /* 0x120 */ s32          resumed;
} BtlPause; // Size: 0x124

extern const BinIdentifier data_ov003_020d7800;
extern const TaskHandle    data_ov003_020d8848;
extern const TaskStages    data_ov003_020d8854;

extern void func_ov003_0208b278(s32 arg0);
extern void func_ov003_020a3a40(void);

// The two touch reads are one comparison chain over the touch state.
// Nonmatching: the target CSEs the touch base pointer between the x and y loads; mwcc reloads it.
void func_ov003_020a210c(BtlPause* self) {
    s16  touchX;
    s16  touchY;
    BOOL overButton;

    if (gSaveData.unk_3140 & 1) {
        return;
    }
    if (TouchInput_WasTouchPressed() == 0) {
        return;
    }
    touchX = data_ov003_020e71b8->touchWork.x;
    if (touchX >= 0x38 && touchX <= 0xC8) {
        touchY = data_ov003_020e71b8->touchWork.y;
        if (touchY >= 0x56 && touchY <= 0x6A) {
            overButton = 1;
        } else {
            overButton = 0;
        }
    } else {
        overButton = 0;
    }
    if (overButton == 0) {
        self->resumed = 0;
        return;
    }
    if (self->resumed == 0) {
        func_ov003_02087ed8(0x2A);
        CombatSprite_SetAnim(&self->sprites[2], 2, 1);
        CombatSprite_Restart(&self->sprites[2]);
        self->resumed = 1;
        return;
    }
    func_ov003_02087ed8(0x2B);
    func_ov003_0208b278(5);
}

s32 func_ov003_020a21d8(TaskPool* pool, Task* task, void* args) {
    BtlPause* self = task->data;
    s32       anim;

    self->resumed = 0;
    CombatSprite_LoadDirect(1, &self->sprites[0], &data_ov003_020d7800, 0x28, 0x2A, 0x29, 0x3F, 0);
    CombatSprite_SetPosition(&self->sprites[0], 0x80, 0x60);
    func_ov003_02082730(&self->sprites[0], 0);
    func_ov003_02082738(&self->sprites[0], 0);
    // The second banner shares the file and the frame range, only its first animation differs
    // between the "new game" and the resumed battle.
    anim = (gSaveData.unk_3140 & 1) ? 3 : 1;
    CombatSprite_LoadDirect(0, &self->sprites[1], &data_ov003_020d7800, 0x28, 0x2A, 0x29, 0x3D, anim);
    CombatSprite_LoadDirect(0, &self->sprites[2], &data_ov003_020d7800, 0x28, 0x2A, 0x29, 0x3D, 2);
    CombatSprite_SetAnim(&self->sprites[2], 2, 1);
    CombatSprite_SetPosition(&self->sprites[1], 0x80, 0x60);
    CombatSprite_SetPosition(&self->sprites[2], 0x80, 0x60);
    func_ov003_02082730(&self->sprites[1], 0);
    func_ov003_02082730(&self->sprites[2], 0);
    func_ov003_02082738(&self->sprites[1], 0);
    func_ov003_02082738(&self->sprites[2], 0);
    return 1;
}

s32 func_ov003_020a2330(TaskPool* pool, Task* task, void* args) {
    BtlPause* self = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x10000) {
        return 1;
    }
    if (InputStatus.buttonState.pressedButtons & INPUT_BUTTON_START) {
        s32 paused = data_ov003_020e71b8->unk3D87C == 0;

        data_ov003_020e71b8->unk3D87C = paused;
        if (data_ov003_020e71b8->unk3D87C != 0) {
            self->resumed = 0;
            CriSndMgr_Pause(data_ov003_020e71b8->unk3D870, 1);
        } else {
            func_ov003_020a3a40();
            CriSndMgr_Pause(data_ov003_020e71b8->unk3D870, 0);
        }
        func_02027200(data_ov003_020e71b8->unk3D87C);
        func_ov003_02087ed8(2);
    }
    if (data_ov003_020e71b8->unk3D87C != 0 && data_ov003_020e71b8->unk3D874 == 2) {
        func_ov003_020a210c(self);
    }
    if (data_ov003_020e71b8->unk3D87C != 0) {
        CombatSprite_Update(&self->sprites[0]);
        CombatSprite_Update(&self->sprites[1]);
        CombatSprite_Update(&self->sprites[2]);
    }
    return 1;
}

s32 func_ov003_020a2458(TaskPool* pool, Task* task, void* args) {
    BtlPause* self = task->data;

    if (data_ov003_020e71b8->unk3D87C != 0) {
        if (data_ov003_020e71b8->unk3D88D != 3) {
            CombatSprite_Render(&self->sprites[0]);
        }
        CombatSprite_Render(&self->sprites[1]);
        if (self->resumed != 0) {
            CombatSprite_Render(&self->sprites[2]);
        }
    }
    return 1;
}

s32 func_ov003_020a24b4(TaskPool* pool, Task* task, void* args) {
    BtlPause* self = task->data;

    CombatSprite_Release(&self->sprites[0]);
    CombatSprite_Release(&self->sprites[1]);
    CombatSprite_Release(&self->sprites[2]);
    return 1;
}

s32 func_ov003_020a24dc(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8854;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov003_020a250c(void) {
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d8848, NULL, 0, NULL, NULL);
}

/// `Tsk_BtlPause` is pooled into the `.data` of the unit before this one; the two names below belong to
/// the handles of the units after it.
extern char      data_ov003_020e3e34[];
char             data_ov003_020e3e44[12] = "Tsk_BtlDbg";
char             data_ov003_020e3e50[20] = "Tsk_BtlInf_ShowVal";
const TaskHandle data_ov003_020d8848     = {data_ov003_020e3e34, func_ov003_020a24dc, sizeof(BtlPause)};
const TaskStages data_ov003_020d8854     = {
    func_ov003_020a21d8,
    func_ov003_020a2330,
    func_ov003_020a2458,
    func_ov003_020a24b4,
};
