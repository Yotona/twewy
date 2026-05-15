#include "Engine/IO/TouchInput.h"
#include "Field/FieldTasks.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ u32 task_plates[8];
    /* 0x28 */ u32 taskId_arrow;
    /* 0x2C */ void (*unk_2C)(TaskPool*, Task*);
    /* 0x30 */ s16  unk_30;
    /* 0x32 */ char unk_32[0x34 - 0x32];
    /* 0x34 */ s16* unk_34;
    /* 0x38 */ s16  unk_38;
    /* 0x3A */ s16  unk_3A;
    /* 0x3C */ s16  unk_3C;
    /* 0x40 */ s32  dataType;
    /* 0x44 */ s32  unk_44;
    /* 0x48 */ s32  unk_48;
} Fld_Choice; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32  unk_0;
    /* 0x4 */ s16  unk_4;
    /* 0x8 */ s16* unk_8;
} Fld_Choice_Args;

static s32 func_ov030_020c8340(TaskPool* pool, Task* task);
s32        Fld_Choice_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Choice = {"Tsk_Fld_Choice", Fld_Choice_RunTask, 0x4C};

void func_ov030_020c7fcc(TaskPool* pool, Task* task) {
    return;
}

void func_ov030_020c7fd0(TaskPool* pool, Task* task) {
    Fld_Choice* choice  = task->data;
    s32         temp_r0 = func_ov030_020c8340(pool, task);

    if (temp_r0 >= 0) {
        if (choice->unk_3A != temp_r0) {
            EasyTask_DeleteTask(pool, choice->taskId_arrow);
            choice->taskId_arrow =
                Fld_WhichArrow_CreateTask(pool, choice->dataType, (s16)(choice->unk_38 + ((s16)temp_r0 * 0x14)), 3, 0);
            choice->unk_3A = (s16)temp_r0;
        } else {
            choice->unk_3C = (s16)temp_r0;
            choice->unk_2C = func_ov030_020c7fcc;
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_EXECUTE);
        }
    }
}

void func_ov030_020c8064(TaskPool* pool, Task* task) {
    Fld_Choice* choice = task->data;

    s32 temp_r0 = func_ov030_020c8340(pool, task);
    if (temp_r0 >= 0) {
        choice->taskId_arrow =
            Fld_WhichArrow_CreateTask(pool, choice->dataType, (s16)(choice->unk_38 + ((s16)temp_r0 * 0x14)), 3, 0);
        choice->unk_2C = func_ov030_020c7fd0;
        choice->unk_3A = temp_r0;
    }
}

// Nonmatching
void func_ov030_020c80cc(TaskPool* pool, Task* task) {
    Fld_Choice* choice = task->data;
    s16         var_r8 = 0x60 - (s16)((choice->unk_30 - 1) * 10);

    choice->unk_38 = var_r8;
    choice->unk_00 = Fld_WhichArrow_CreateTask(pool, choice->dataType, (s16)var_r8, 1, 0);

    for (s16 i = 0; i < choice->unk_30; i++) {
        choice->task_plates[i] =
            Fld_TalkEventStringPlate_CreateTask(pool, choice->dataType, 0x80, var_r8, 4, choice->unk_34[i], 1);
        var_r8 += 0x14;
    }

    choice->unk_04 = Fld_WhichArrow_CreateTask(pool, choice->dataType, (s16)(var_r8 - 0x14), 2, 0);
    choice->unk_2C = func_ov030_020c8064;
}

s32 Fld_Choice_Init(TaskPool* pool, Task* task, void* args) {
    Fld_Choice*      choice     = task->data;
    Fld_Choice_Args* choiceArgs = args;

    choice->dataType     = choiceArgs->unk_0;
    choice->unk_30       = choiceArgs->unk_4;
    choice->unk_34       = choiceArgs->unk_8;
    choice->taskId_arrow = -1;
    choice->unk_3C       = -1;
    choice->unk_2C       = func_ov030_020c80cc;
    SndMgr_StartPlayingSE(SEIDX_EV_SE_SELECT_APPEAR);
    return 1;
}

s32 Fld_Choice_Update(TaskPool* pool, Task* task, void* args) {
    Fld_Choice* choice = task->data;

    if (choice->unk_2C != NULL) {
        choice->unk_2C(pool, task);
    }
    return 1;
}

s32 Fld_Choice_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Fld_Choice_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_Choice* choice = task->data;

    if (choice->taskId_arrow != -1) {
        EasyTask_DeleteTask(pool, choice->taskId_arrow);
    }
    if (choice->unk_30 != 0) {
        EasyTask_DeleteTask(pool, choice->unk_04);
        for (s32 i = 0; i < choice->unk_30; i++) {
            EasyTask_DeleteTask(pool, choice->task_plates[i]);
        }
        EasyTask_DeleteTask(pool, choice->unk_00);
    }
    return 1;
}

s32 Fld_Choice_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Choice_Init,
        .update     = Fld_Choice_Update,
        .render     = Fld_Choice_Render,
        .cleanup    = Fld_Choice_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_Choice_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16* arg3) {
    Fld_Choice_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    args.unk_8 = arg3;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Choice, NULL, 0, NULL, &args);
}

s32 func_ov030_020c8340(TaskPool* pool, Task* task) {
    if (TouchInput_WasTouchPressed() == 0) {
        return -1;
    }

    Fld_Choice* choice = task->data;
    TouchCoord  coord;

    TouchInput_GetCoord(&coord);
    if ((coord.x <= 0x2D) || (coord.x >= 0xD3)) {
        return -1;
    }

    s32 var_r1 = choice->unk_38 - 10;
    if (coord.y < var_r1) {
        return -1;
    }

    for (s16 i = 0; i < choice->unk_30; i++) {
        var_r1 += 0x14;
        if (var_r1 >= coord.y) {
            return i;
        }
    }
    return -1;
}

s16 func_ov030_020c83e8(TaskPool* pool, Task* task) {
    Fld_Choice* choice = task->data;

    return choice->unk_3C;
}
