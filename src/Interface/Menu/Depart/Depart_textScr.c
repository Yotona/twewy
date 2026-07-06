#include "Interface/Menu/Depart.h"
#include "common_data.h"

typedef struct {
    /* 0x000 */ DepartObject* depart;
    /* 0x004 */ UnkOv31Struct unk_004[5];
    /* 0x270 */ UnkOv31Struct unk_270[5];
} DepartTextScr; // Size: 0x4DC

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ DepartObject* depart;
} DepartTextScrArgs;

s32 DepartTextScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle data_ov043_020caec4 = {"Tsk_Depart_textScr", DepartTextScr_RunTask, sizeof(DepartTextScr)};

void func_ov043_020beb44(DepartTextScr* textScr) {
    s32 i;
    for (i = 0; i < 5; i++) {
        func_ov031_0210aa94(&textScr->unk_004[i]);
        func_ov031_0210aa94(&textScr->unk_270[i]);
    }

    for (i = 0; i < 5; i++) {
        func_ov031_0210ab34(&textScr->unk_004[i], 14);
        func_ov031_0210ab34(&textScr->unk_270[i], 14);
    }
}

void func_ov043_020bebc0(DepartTextScr* textScr) {
    // Not yet implemented
}

s32 DepartTextScr_Init(TaskPool* pool, Task* task, void* args) {
    DepartTextScr*     textScr     = task->data;
    DepartTextScrArgs* textScrArgs = args;

    textScr->depart = textScrArgs->depart;
    func_ov043_020beb44(textScr);
    func_ov043_020bebc0(textScr);
    return 1;
}

s32 DepartTextScr_Update(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 DepartTextScr_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 DepartTextScr_Release(TaskPool* pool, Task* task, void* args) {
    DepartTextScr* textScr = task->data;

    for (s32 i = 0; i < 5; i++) {
        func_ov031_0210aabc(&textScr->unk_004[i]);
        func_ov031_0210aabc(&textScr->unk_270[i]);
    }
    return 1;
}

s32 DepartTextScr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = DepartTextScr_Init,
        .update     = DepartTextScr_Update,
        .render     = DepartTextScr_Render,
        .cleanup    = DepartTextScr_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 DepartTextScr_CreateTask(TaskPool* pool, s32 dataType, DepartObject* depart) {
    DepartTextScrArgs args;
    args.dataType = dataType;
    args.depart   = depart;
    return EasyTask_CreateTask(pool, &data_ov043_020caec4, NULL, 0, NULL, &args);
}