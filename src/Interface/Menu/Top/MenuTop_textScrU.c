#include "Interface/Menu/Top.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ UnkStruct_TopMenu* unk_000;
    /* 0x004 */ UnkOv31Struct      unk_004[10];
} MenuTop_textScrU; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ s32 unk_4;
} MenuTop_textScrU_Args;

s32 MenuTop_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const Point data_ov043_020c8164[3] = {
    {24, 11},
    {24, 10},
    {14, 27},
};

static const TaskHandle Tsk_MenuTop_textScrU = {"Tsk_MenuTop_textScrU", MenuTop_textScrU_RunTask, 0x4DC};

// static const ? data_ov043_020c8180;

void func_ov043_0208a630(MenuTop_textScrU* textScrU) {
    for (s32 i = 0; i < 10; i++) {
        func_ov031_0210aa94(&textScrU->unk_004[i]);
        func_ov031_0210ab34(&textScrU->unk_004[i], 14);
    }
}

void func_ov043_0208a668(MenuTop_textScrU* textScrU) {
    // Not yet implemented
}

void func_ov043_0208ab08(MenuTop_textScrU* textScrU) {
    return;
}

// Nonmatching
void func_ov043_0208ab0c(MenuTop_textScrU* textScrU) {
    UnkStruct_TopMenu* temp_r6 = textScrU->unk_000;
    Point              temp[3];

    temp = data_ov043_020c8164;

    s32 temp_r9 = temp_r6->unk_78;
    s32 temp_r8 = temp_r6->unk_7C;
    if ((temp_r8 == NULL) || (temp_r9 == NULL)) {
        OS_WaitForever();
    }

    for (u16 i = 0; i < 3; i++) {
        func_ov031_0210ab28(&textScrU->unk_004[i], temp[i].x, temp[i].y);
        func_ov031_0210ab54(&textScrU->unk_004[i], 1, 0);
    }

    func_ov031_0210b630(&textScrU->unk_004, (u16)(temp_r6->unk_60 + 0x2A97));
    func_ov031_0210ab3c(&textScrU->unk_004, 1, 0xE0);
    func_ov031_0210ab48(&textScrU->unk_004, 3, 0xFFFF);
    func_ov031_0210be18(&textScrU->unk_004, temp_r8 + 4, temp_r9 + 4, 0);

    s32 temp_r0 = func_ov031_0210b698(&textScrU->unk_004[1], 0x333D);
    s32 sp10;

    func_ov031_0210a5fc(&sp10, temp_r0, temp_r6->unk_60 + 1, 7);
    func_ov031_0210ab3c(&textScrU->unk_004[1], 2, 0xDC);
    func_ov031_0210bde8(&textScrU->unk_004[1], &sp10, temp_r8 + 4, temp_r9 + 4, 0);
    Mem_Free(&gDebugHeap, temp_r0);

    func_ov031_0210b630(&textScrU->unk_004[2], (u16)(temp_r6->unk_60 + 0x2A9E));
    func_ov031_0210ab3c(&textScrU->unk_004[2], 1, 0xE0);
    func_ov031_0210be18(&textScrU->unk_004[2], temp_r8 + 4, temp_r9 + 4, 0);
}

s32 MenuTop_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScrU*      textScrU     = task->data;
    MenuTop_textScrU_Args* textScrUArgs = args;

    textScrU->unk_000 = textScrUArgs->unk_4;
    func_ov043_0208a630(textScrU);
    func_ov043_0208a668(textScrU);
    return 1;
}

s32 MenuTop_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScrU*  textScrU = task->data;
    UnkStruct_TopMenu* temp_r5  = textScrU->unk_000;

    if (temp_r5->unk_00 & 2) {
        temp_r5->unk_00 &= ~2;
        func_ov043_02085670(&temp_r5->unk_68, 0, 0, 9, 15, 1);

        if (temp_r5->unk_56 == 0) {
            func_ov043_0208a668(textScrU);
        } else if (temp_r5->unk_56 == 1) {
            func_ov043_0208ab08(textScrU);
        } else {
            func_ov043_0208ab0c(textScrU);
        }
    }
    return 1;
}

s32 MenuTop_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 MenuTop_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_textScrU* textScrU = task->data;

    for (s32 i = 0; i < 10; i++) {
        func_ov031_0210aabc(&textScrU->unk_004[i]);
    }

    return 1;
}

s32 MenuTop_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_textScrU_Init,
        .update     = MenuTop_textScrU_Update,
        .render     = MenuTop_textScrU_Render,
        .cleanup    = MenuTop_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

void MenuTop_textScrU_CreateTask(TaskPool* pool, s32 arg1, s32 arg2) {
    MenuTop_textScrU_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    EasyTask_CreateTask(pool, &Tsk_MenuTop_textScrU, NULL, 0, NULL, &args);
}
