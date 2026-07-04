#include "Interface/Menu/Shop.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ void*         unk_000;
    /* 0x004 */ UnkOv31Struct unk_004[3];
} Shop_textScrU; // Size: 0x178

typedef struct {
    /* 0x0 */ s32   unk_0;
    /* 0x4 */ void* unk_4;
} Shop_textScrU_Args;

s32 Shop_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_textScrU = {"Tsk_Shop_textScrU", Shop_textScrU_RunTask, sizeof(Shop_textScrU)};

const u16 data_ov043_020cac70[6] = {36, 8, 36, 21, 36, 153};

void func_ov043_020bc098(Shop_textScrU* textScr) {
    for (s32 i = 0; i < 3; i++) {
        func_ov031_0210aa94(&textScr->unk_004[i]);
        func_ov031_0210ab54(&textScr->unk_004[i], 1, 0);
    }
}

void func_ov043_020bc0d8(Shop_textScrU* textScr) {
    // Not yet implemented
}

s32 Shop_textScrU_Init(TaskPool* pool, Task* task, void* args) {
    Shop_textScrU*      textScr     = task->data;
    Shop_textScrU_Args* textScrArgs = args;

    textScr->unk_000 = textScrArgs->unk_4;
    func_ov043_020bc098(textScr);
    func_ov043_020bc0d8(textScr);
    return 1;
}

s32 Shop_textScrU_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Shop_textScrU_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 Shop_textScrU_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_textScrU* textScr = task->data;

    for (s32 i = 0; i < 3; i++) {
        func_ov031_0210aabc(&textScr->unk_004[i]);
    }
    return 1;
}

s32 Shop_textScrU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_textScrU_Init,
        .update     = Shop_textScrU_Update,
        .render     = Shop_textScrU_Render,
        .cleanup    = Shop_textScrU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_textScrU_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    Shop_textScrU_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    return EasyTask_CreateTask(pool, &Tsk_Shop_textScrU, NULL, 0, NULL, &args);
}
