#include "Field/Tasks/Task_Mess.h"
#include "Engine/EasyTask.h"
#include "Util/SysFont.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ Sprite* sprite;
    /* 0x04 */ s32     messageId;
    /* 0x08 */ u8*     completionFlag;
    /* 0x0C */ u8      mode;
    /* 0x0D */ u8      unk_0D;
    /* 0x0E */ u16     glyphCount;
    /* 0x10 */ u32     unk_10;
    /* 0x14 */ s32     alignRight;
    /* 0x18 */ u32     unk_18;
    /* 0x1C */ u32     unk_1C;
    /* 0x20 */ s32     fontId;
} Task_Mess_Args; // Size: 0x24

typedef struct Task_Mess Task_Mess;
typedef void (*Task_Mess_UpdateFunc)(Task_Mess*);

struct Task_Mess {
    /* 0x00 */ Task_Mess_UpdateFunc update;
    /* 0x04 */ s32                  active;
    /* 0x08 */ Task_Mess_Args       args;
    /* 0x2C */ u16                  delay;
    /* 0x2E */ u8                   unk_2E[2];
    /* 0x30 */ SysFont              font;
    /* 0xAC */ u16                  progress;
    /* 0xAE */ u16                  unk_AE;
    /* 0xB0 */ s32                  alignRight;
}; // Size: 0xB4

extern s32 Task_Mess_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static void Task_Mess_StateDone(Task_Mess* data) {
    data->active = FALSE;
}

static void Task_Mess_Setup(Task_Mess* data) {
    s32 minX;
    s32 minY;
    s32 width;
    s32 height;
    s32 x;
    s32 y;

    data->progress   = 0;
    data->alignRight = data->args.alignRight;
    data->delay      = data->args.mode;
    SysFont_InitWithFont(&data->font, data->args.fontId, TRUE);
    if (data->args.fontId == 3) {
        SysFont_SetLineSpacing(&data->font, 1);
    }
    SysFont_SetSpacing(&data->font, TRUE, 0);
    SysFont_SetMsg(&data->font, data->args.messageId);
    SysFont_GetCellBoundsMin(data->args.sprite, &minX, &minY);
    width  = SysFont_MeasureMsgWidth(&data->font, data->args.glyphCount);
    height = SysFont_MeasureMsgHeight(&data->font, data->args.glyphCount);
    x      = -(width) / 2;
    y      = -(height) / 2;
    if (data->alignRight == 0) {
        SysFont_SetHAlign(&data->font, 1, width);
    } else {
        SysFont_SetHAlign(&data->font, 0, width);
    }
    x -= minX;
    y -= minY;
    SysFont_SetPos(&data->font, (u16)x, (u16)y);
}

static s32 Task_Mess_DrawNext(Task_Mess* data) {
    BOOL (*isTerminator)();

    if (data->args.mode == 0) {
        SysFont_DrawMsgToSprite(&data->font, data->args.glyphCount, data->args.sprite, TRUE);
        return FALSE;
    }
    isTerminator = (BOOL(*)())SysFont_IsTerminator;
    if (isTerminator(
            SysFont_DrawMsgToSpriteRange(&data->font, data->args.glyphCount, data->args.sprite, TRUE, data->progress, 1)))
    {
        return FALSE;
    }
    data->progress++;
    return TRUE;
}

// Nonmatching: argument/store scheduling in the pacing loop
static void Task_Mess_StateDraw(Task_Mess* data) {
    if (*data->args.completionFlag == 0) {
        return;
    }
    if (data->delay == 0) {
        do {
            data->delay = data->args.mode;
            if (Task_Mess_DrawNext(data) == 0) {
                data->update = Task_Mess_StateDone;
                return;
            }
        } while (data->args.mode == 0);
    } else {
        data->delay--;
    }
}

static void Task_Mess_StateIdle(Task_Mess* data) {}

static s32 Task_Mess_Init(TaskPool* pool, Task* task, void* args) {
    Task_Mess* data = task->data;

    MI_CpuCopyU8(args, &data->args, sizeof(data->args));
    data->update = Task_Mess_StateIdle;
    data->active = TRUE;
    Task_Mess_Setup(data);
    return TRUE;
}

static s32 Task_Mess_Update(TaskPool* pool, Task* task, void* args) {
    Task_Mess* data = task->data;

    if (data->update != NULL) {
        data->update(data);
    }
    return data->active;
}

static s32 Task_Mess_Render(TaskPool* pool, Task* task, void* args) {
    Task_Mess_StateDraw(task->data);
    return TRUE;
}

static s32 Task_Mess_Destroy(TaskPool* pool, Task* task, void* args) {
    Task_Mess* data = task->data;

    SysFont_Destroy(&data->font);
    return TRUE;
}

const TaskHandle Tsk_Task_Mess = {"Task_Mess", Task_Mess_RunTask, sizeof(Task_Mess)};

static s32 Task_Mess_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Task_Mess_Init,
        .update     = Task_Mess_Update,
        .render     = Task_Mess_Render,
        .cleanup    = Task_Mess_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
