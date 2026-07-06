#include "Interface/Menu/Depart.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ DepartObject* unk_00;
    /* 0x04 */ UnkOv31Struct unk_04;
} DepartTextScrU; // Size: 0x80

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ DepartObject* depart;
} DepartTextScrUArgs;

static s32 Depart_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Depart_textScrU = {"Tsk_Depart_textScrU", Depart_textScrU_RunTask, sizeof(DepartTextScrU)};

static void func_ov043_020bee2c(DepartTextScrU* textScrU) {
    func_ov031_0210aa94(&textScrU->unk_04);
    func_ov031_0210ab34(&textScrU->unk_04, 8);
}

static void func_ov043_020bee4c(DepartTextScrU* textScrU) {
    DepartObject* temp_r7 = textScrU->unk_00;

    u32* temp_r4 = temp_r7->unk_34[2].unk_14;
    u32* temp_r5 = temp_r7->unk_34[2].unk_10;

    if ((temp_r4 == 0) || (temp_r5 == 0)) {
        OS_WaitForever();
    }

    func_ov031_0210ab28(&textScrU->unk_04, 2, 6);
    func_ov031_0210ab54(&textScrU->unk_04, 1, 0);
    func_ov031_0210b630(&textScrU->unk_04, (u16)(temp_r7->unk_0A + 0x2FB8));
    func_ov031_0210be18(&textScrU->unk_04, temp_r4 + 1, temp_r5 + 1, 0);
}

static s32 Depart_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    DepartTextScrU*     textScrU     = task->data;
    DepartTextScrUArgs* textScrUArgs = args;

    textScrU->unk_00 = textScrUArgs->depart;
    func_ov043_020bee2c(textScrU);
    func_ov043_020bee4c(textScrU);
    return 1;
}

static s32 Depart_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Depart_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Depart_textScrU_Release(TaskPool* pool, Task* task, void* args) {
    DepartTextScrU* textScrU = task->data;

    func_ov031_0210aabc(&textScrU->unk_04);
    return 1;
}

static s32 Depart_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Depart_textScrU_Init,
        .update     = Depart_textScrU_Update,
        .render     = Depart_textScrU_Render,
        .cleanup    = Depart_textScrU_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Depart_textScrU_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart) {
    DepartTextScrUArgs args;
    args.dataType = dataType;
    args.depart   = depart;
    return EasyTask_CreateTask(pool, &Tsk_Depart_textScrU, NULL, 0, NULL, &args);
}