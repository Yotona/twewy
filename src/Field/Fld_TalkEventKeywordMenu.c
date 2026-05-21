#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/FieldTasks.h"
#include "Save.h"
#include "SndMgr.h"
#include "types.h"

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08[8];
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ void (*unk_34)(TaskPool*, Task*);
    /* 0x38 */ void (*unk_38)(TaskPool*, Task*);
    /* 0x3C */ s16  unk_3C;
    /* 0x3E */ s16  unk_3E;
    /* 0x40 */ s16  unk_40;
    /* 0x42 */ s16  unk_42;
    /* 0x44 */ s16  unk_44;
    /* 0x46 */ char unk_46[0x48 - 0x46];
    /* 0x48 */ s32  unk_48;
    /* 0x4C */ s16  unk_4C;
} Fld_TalkEventKeywordMenu; // Size: 0x50

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s16 unk_4;
} Fld_TalkEventKeywordMenu_Args;

s32 Fld_TalkEventKeywordMenu_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_TalkEventKeywordMenu = {"Tsk_Fld_TalkEventKeywordMenu", Fld_TalkEventKeywordMenu_RunTask,
                                                        0x50};

void func_ov030_020c3d5c(TaskPool* pool, Task* task) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    if (menu->unk_44 != 0) {
        menu->unk_44--;
    } else {
        menu->unk_34 = menu->unk_38;
    }
}

void func_ov030_020c3d7c(TaskPool* pool, Task* task) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    s32 temp_r0 = func_ov030_020c420c();
    if (temp_r0 < 0) {
        return;
    }
    if (menu->unk_42 != temp_r0) {
        EasyTask_DeleteTask(pool, menu->unk_28);
        menu->unk_28 = func_ov030_020c45b4(pool, menu->unk_48, menu->unk_40, (s16)(menu->unk_3E + ((s16)temp_r0 * 0x14)));
        menu->unk_42 = temp_r0;
    }
}

void func_ov030_020c3ddc(TaskPool* pool, Task* task) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    s32 temp_r0 = func_ov030_020c420c();
    if (temp_r0 < 0) {
        return;
    }
    menu->unk_28 = func_ov030_020c45b4(pool, menu->unk_48, menu->unk_40, (s16)(menu->unk_3E + ((s16)temp_r0 * 20)));
    menu->unk_34 = func_ov030_020c3d7c;
    menu->unk_42 = temp_r0;
}

// Nonmatching
void func_ov030_020c3e30(TaskPool* pool, Task* task) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    s32 var_r5 = 2;

    menu->unk_3C = func_ov030_020b0418();
    if (menu->unk_3C == 0) {
        return;
    }

    s16 var_r8 = 182;
    if (menu->unk_4C >= 0x80) {
        var_r8 = 92;
        var_r5 = 9;
    }
    menu->unk_40 = var_r8;

    s16 var_r9   = 0x60 - (s16)((menu->unk_3C - 1) * 10);
    menu->unk_3E = var_r9;
    menu->unk_00 = func_ov030_020c3908(pool, menu->unk_48, var_r8, var_r9, var_r5);

    for (s16 i = 0; i < menu->unk_3C; i++) {
        menu->unk_08[i] = Fld_TalkEventStringPlate_CreateTask(
            pool, menu->unk_48, var_r8, var_r9, gSaveState.unk_20.unk_24C8[i + 0x10], gSaveState.unk_20.unk_24C8[i], 1);
        var_r9 += 20;
    }
    menu->unk_04 = func_ov030_020c3908(pool, menu->unk_48, var_r8, (s16)(var_r9 - 0x14), 3);
    menu->unk_44 = 0x14;
    menu->unk_34 = func_ov030_020c3d5c;
    menu->unk_38 = func_ov030_020c3ddc;
}

void func_ov030_020c3f84(TaskPool* pool, Task* task) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    if (TouchInput_WasTouchPressed() && func_ov030_020b4514(EasyTask_GetTaskById(pool, menu->unk_2C))) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        EasyTask_DeleteTask(pool, menu->unk_30);
        EasyTask_DeleteTask(pool, menu->unk_2C);
        menu->unk_2C = -1U;
        menu->unk_44 = 2;
        menu->unk_34 = func_ov030_020c3d5c;
        menu->unk_38 = func_ov030_020c3e30;
    }
}

void func_ov030_020c4000(TaskPool* pool, Task* task) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    menu->unk_30 = func_ov030_020c3690(pool, menu->unk_48, menu->unk_4C, 0x78);
    menu->unk_2C = func_ov030_020b44b8(pool, menu->unk_48, menu->unk_4C, 0x78, 0, 1);
    func_ov030_020b4570(EasyTask_GetTaskById(pool, menu->unk_2C), 2);
    menu->unk_34 = func_ov030_020c3f84;
}

// Nonmatching: Structure doesn't seem to match the rest of the TU's usage, what is this?
s32 Fld_TalkEventKeywordMenu_Init(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventKeywordMenu*      menu     = task->data;
    Fld_TalkEventKeywordMenu_Args* menuArgs = args;

    menu->unk_48 = menuArgs->unk_0;
    menu->unk_4C = menuArgs->unk_4;
    menu->unk_3C = 0;
    menu->unk_28 = -1;
    menu->unk_2C = -1;
    menu->unk_44 = 30;
    menu->unk_34 = func_ov030_020c3d5c;
    menu->unk_38 = func_ov030_020c4000;
    return 1;
}

s32 Fld_TalkEventKeywordMenu_Update(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    if (menu->unk_34 != NULL) {
        menu->unk_34(pool, task);
    }
    return 1;
}

s32 Fld_TalkEventKeywordMenu_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Fld_TalkEventKeywordMenu_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_TalkEventKeywordMenu* menu = task->data;

    if (menu->unk_28 != -1) {
        EasyTask_DeleteTask(pool, menu->unk_28);
    }
    if (menu->unk_2C != -1U) {
        EasyTask_DeleteTask(pool, menu->unk_30);
        EasyTask_DeleteTask(pool, menu->unk_2C);
    }
    if (menu->unk_3C != 0) {
        EasyTask_DeleteTask(pool, menu->unk_04);
        for (s32 i = 0; i < menu->unk_3C; i++) {
            EasyTask_DeleteTask(pool, menu->unk_08[i]);
        }
        EasyTask_DeleteTask(pool, menu->unk_00);
    }
    return 1;
}

s32 Fld_TalkEventKeywordMenu_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_TalkEventKeywordMenu_Init,
        .update     = Fld_TalkEventKeywordMenu_Update,
        .render     = Fld_TalkEventKeywordMenu_Render,
        .cleanup    = Fld_TalkEventKeywordMenu_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_TalkEventKeywordMenu_CreateTask(TaskPool* arg0, s32 arg1, s16 arg2) {
    Fld_TalkEventKeywordMenu_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(arg0, &Tsk_Fld_TalkEventKeywordMenu, NULL, 0, NULL, &args);
}
