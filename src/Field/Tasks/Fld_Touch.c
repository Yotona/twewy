#include "Field/Tasks/Fld_Touch.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Tasks/Fld_TouchCommand.h"
#include "Field/Tasks/Fld_TouchCursor.h"
#include "Field/Tasks/Fld_TouchSeal.h"
#include "Field/Tasks/Fld_TouchShine.h"
#include "SndMgr.h"

extern s32 func_0202599c(s32 flagIndex);

typedef struct {
    /* 0x00 */ s16 id;
    /* 0x02 */ s16 xMin;
    /* 0x04 */ s16 yMin;
    /* 0x06 */ s16 xMax;
    /* 0x08 */ s16 yMax;
} Ov030TouchRegion;

typedef struct {
    /* 0x00 */ s16 value;
    /* 0x02 */ s16 count;
} Ov030TouchEntry;

typedef struct {
    /* 0x00 */ s16 a;
    /* 0x02 */ s16 b;
} Ov030TouchPair;

typedef struct {
    /* 0x00 */ s32             unk0;
    /* 0x04 */ s16             unk4;
    /* 0x06 */ s16             pad_06;
    /* 0x08 */ s32             taskIds[5];
    /* 0x1C */ s16             unk1C;
    /* 0x1E */ Ov030TouchEntry entries[7];
    /* 0x3A */ s16             pad_3A;
    /* 0x3C */ s32             unk3C;
    /* 0x40 */ s32             unk40;
    /* 0x44 */ s16             unk44;
    /* 0x46 */ s16             pad_46;
    /* 0x48 */ s32             unk48;
    /* 0x4C */ s16             unk4C;
} Ov030TouchData; // Size: 0x50
static s32 Fld_Touch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Touch = {"Tsk_Fld_Touch", Fld_Touch_RunTask, sizeof(Ov030TouchData)};

static const Ov030TouchPair Fld_Touch_AreaEvents[7] = {
    { 6, 3},
    { 9, 3},
    {12, 3},
    {15, 3},
    {18, 3},
    {21, 2},
    {23, 1},
};

static const Ov030TouchRegion Fld_Touch_Areas[11] = {
    {0,  95,  23, 138,  50},
    {1,  97,  51, 138,  93},
    {1, 138,  66, 151,  93},
    {2,  95,  94, 112, 119},
    {2,  85, 119, 112, 143},
    {2, 113,  94, 135, 143},
    {3, 136,  94, 166, 143},
    {4, 167, 125, 184, 150},
    {5,  72, 144, 187, 169},
    {5,  44, 166,  97, 191},
    {5, 100, 169, 173, 175},
};

static s16 Fld_Touch_GetTouchedArea(void) {
    TouchCoord pos;
    TouchCoord coord;
    u32        i;

    TouchInput_GetCoord(&coord);
    pos = coord;
    for (i = 0; i < 0xB; i++) {
        if (Fld_Touch_Areas[i].xMin <= pos.x && pos.x <= Fld_Touch_Areas[i].xMax && Fld_Touch_Areas[i].yMin <= pos.y &&
            pos.y <= Fld_Touch_Areas[i].yMax)
        {
            return Fld_Touch_Areas[i].id;
        }
    }
    return 6;
}

static s32 Fld_Touch_FindTouchedSeal(TaskPool* pool, Ov030TouchData* arg1) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (arg1->taskIds[i] != -1) {
            if (Fld_TouchSeal_IsTouched(EasyTask_GetTaskById(pool, arg1->taskIds[i])) != 0) {
                return i;
            }
        }
    }
    return -1;
}

static void Fld_Touch_SelectSeal(TaskPool* pool, Ov030TouchData* arg1, s32 arg2) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (arg1->taskIds[i] != -1) {
            Task* task = EasyTask_GetTaskById(pool, arg1->taskIds[i]);
            s32   state;

            if (i == arg2) {
                state = 1;
            } else {
                state = 2;
            }
            ((void (*)())Fld_TouchSeal_SetAnimation)(task, state);
        }
    }
}

static void Fld_Touch_ResetRubProgress(Ov030TouchData* arg0) {
    s32 i = 0;

    do {
        arg0->entries[i].value = 0;
        i++;
    } while (i < 7);
}

static void Fld_Touch_ResetRubCounts(Ov030TouchData* arg0) {
    s32 i = 0;

    do {
        arg0->entries[i].count = 0;
        i++;
    } while (i < 7);
}

static s32 Fld_Touch_Init(TaskPool* pool, Task* task, void* args) {
    s32*            arg2 = args;
    Ov030TouchData* data = task->data;

    data->taskIds[0] = Fld_TouchSeal_CreateTask(pool, *arg2, 0, 0xE2, 0xA7);
    data->taskIds[1] = -1;
    data->taskIds[2] = -1;
    data->taskIds[4] = -1;
    if (func_0202599c(2) != 0) {
        data->taskIds[1] = Fld_TouchSeal_CreateTask(pool, *arg2, 2, 0x23, 0xA0);
    }
    if (func_0202599c(3) != 0) {
        data->taskIds[2] = Fld_TouchSeal_CreateTask(pool, *arg2, 1, 0xE2, 0x21);
        Fld_TouchSeal_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[2]), 1);
        data->taskIds[3] = Fld_TouchCommand_CreateTask(pool, *arg2, 1);
        data->unk1C      = 2;
        data->taskIds[4] = Fld_TouchCursor_CreateTask(pool, *arg2, data->unk1C);
    } else if (func_0202599c(2) != 0) {
        Fld_TouchSeal_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[1]), 1);
        data->taskIds[3] = Fld_TouchCommand_CreateTask(pool, *arg2, 2);
        data->unk1C      = 1;
        data->taskIds[4] = Fld_TouchCursor_CreateTask(pool, *arg2, data->unk1C);
    } else {
        data->taskIds[3] = -1;
        data->unk1C      = -1;
    }
    data->unk0 = *arg2;
    data->unk4 = 0;
    Fld_Touch_ResetRubProgress(data);
    Fld_Touch_ResetRubCounts(data);
    data->unk3C = -1;
    data->unk40 = -1;
    data->unk44 = 0;
    data->unk4C = 7;
    data->unk48 = 0;
    return 1;
}

static s32 Fld_Touch_RubArea(Ov030TouchData* arg0, s32 arg1) {
    TouchCoord       pos;
    TouchCoord       coord;
    s32              result = 0;
    Ov030TouchEntry* entry;

    TouchInput_GetCoord(&coord);
    pos   = coord;
    entry = &arg0->entries[arg1];
    if (arg0->unk4C == arg1 || arg0->unk4C == 6) {
        s32 dx = pos.x - arg0->unk3C;

        if (dx < 0) {
            dx = -dx;
        }
        if (dx <= 4) {
            s32 dy = pos.y - arg0->unk40;

            if (dy < 0) {
                dy = -dy;
            }
            if (dy <= 4) {
                goto after;
            }
        }
        arg0->unk3C = pos.x;
        arg0->unk40 = pos.y;
        result      = 1;
        entry->value += 4;
        arg0->unk4C = arg1;
        if (entry->value >= 0x64) {
            Fld_Touch_ResetRubProgress(arg0);
            if (arg1 == 6) {
                Field_SetTouchEventResult(4);
                Field_SetTouchEventTarget(0x17);
            } else if (entry->count < Fld_Touch_AreaEvents[arg1].b) {
                Field_SetTouchEventResult(4);
                Field_SetTouchEventTarget((s16)(entry->count + Fld_Touch_AreaEvents[arg1].a));
                entry->count += 1;
            } else if (arg1 == 5) {
                Field_SetTouchEventResult(1);
                arg0->unk48 = 1;
            } else {
                Field_SetTouchEventResult(2);
                arg0->unk48 = 1;
            }
        }
    }
after:
    if (arg1 != 6) {
        arg0->entries[6].value = 0;
    }
    return result;
}

static s32 Fld_Touch_Update(TaskPool* pool, Task* task, void* args) {
    Ov030TouchData* data = task->data;
    s32             r5;
    s16             t0;

    if (data->unk48 != 0) {
        return 1;
    }
    EasyFade_FadeBothDisplays(0, 0, 0x1000);
    if (TouchInput_WasTouchPressed() != 0) {
        r5 = Fld_Touch_FindTouchedSeal(pool, data);
        if (r5 != -1) {
            Fld_Touch_SelectSeal(pool, data, r5);
            if (r5 == 0) {
                Field_SetTouchEventResult(3);
                SndMgr_PlaySEWithPan(0xC4, 0xE2);
                data->unk48 = 1;
            } else if (r5 == 1) {
                Fld_Touch_ResetRubProgress(data);
                Fld_TouchCommand_SetCommand(EasyTask_GetTaskById(pool, data->taskIds[3]), 2);
                Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[4]), 1);
                if (data->unk1C != 1) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget(0x19);
                    SndMgr_PlaySEWithPan(0xC4, 0x23);
                }
            } else if (r5 == 2) {
                Fld_TouchCommand_SetCommand(EasyTask_GetTaskById(pool, data->taskIds[3]), 1);
                Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[4]), 2);
                if (data->unk1C != 2) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget(5);
                    SndMgr_PlaySEWithPan(0xC4, 0xE2);
                }
            }
            data->unk4C = 7;
            data->unk1C = r5;
        } else {
            t0 = data->unk1C;
            if (t0 == 1) {
                s16 id = Fld_Touch_GetTouchedArea();

                if (id != data->unk4C) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget((s16)(Fld_Touch_GetTouchedArea() + 0x1A));
                }
                data->unk4C = Fld_Touch_GetTouchedArea();
            } else if (t0 == 2) {
                r5 = Fld_Touch_GetTouchedArea();
                if (r5 != data->unk4C && r5 != 6) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget((s16)(r5 + 0x22));
                }
                data->unk4C = Fld_Touch_GetTouchedArea();
            }
        }
    } else if (TouchInput_IsTouchActive() != 0 && data->unk1C == 2) {
        r5 = Fld_Touch_GetTouchedArea();
        if (r5 == data->unk4C && data->unk4C != 6) {
            Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[4]), 5);
        } else {
            Fld_TouchCursor_SetAnimation(EasyTask_GetTaskById(pool, data->taskIds[4]), 2);
        }
        if (Fld_Touch_RubArea(data, r5) != 0 && data->unk4 == 0 && r5 != 6 && r5 == data->unk4C) {
            Fld_TouchShine_CreateTask(pool, data->unk0);
            data->unk4 = 5;
        }
    }
    if (TouchInput_IsTouchActive() != 0) {
        data->unk44 = 0;
    } else {
        data->unk44++;
        if (data->unk44 > 0x258) {
            data->unk44 = 0;
            Field_SetTouchEventResult(4);
            if (data->unk1C == 2) {
                Field_SetTouchEventTarget(0x18);
            } else {
                Field_SetTouchEventTarget(0x21);
            }
        }
    }
    t0 = data->unk4;
    if (t0 != 0) {
        data->unk4 = t0 - 1;
    }
    return 1;
}

static s32 Fld_Touch_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_Touch_Destroy(TaskPool* pool, Task* task, void* args) {
    Ov030TouchData* data = task->data;
    s32             i    = 0;

    do {
        if (data->taskIds[i] != -1) {
            EasyTask_DeleteTask(pool, data->taskIds[i]);
        }
        i++;
    } while (i < 5);
    return 1;
}

static s32 Fld_Touch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Touch_Init,
        .update     = Fld_Touch_Update,
        .render     = Fld_Touch_Render,
        .cleanup    = Fld_Touch_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Touch_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &Tsk_Fld_Touch, NULL, 0, NULL, &param);
}
