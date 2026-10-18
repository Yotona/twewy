#include "Field/TaskHelpers.h"
// USA TU; original assembly remains linked (incomplete).
// Range: 0x020bf360-0x020bfb80; 12 functions.
// Research draft: docs/decomp/ov030/Fld_Touch.c
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

s32  func_ov030_020bfee4(TaskPool* arg0, s32 arg1, s32 arg2);
void func_ov030_020bff68(Task* arg0, s32 arg1);
s32  func_ov030_020c0184(TaskPool* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
void func_ov030_020c01c4(Task* arg0, s32 arg1);
void func_ov030_020c050c(Task* arg0, s32 arg1);
s32  func_ov030_020c0730(TaskPool* arg0, s32 arg1);

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

extern TaskHandle     data_ov030_020e6d94;
extern TaskStages     data_ov030_020e6da0;
extern Ov030TouchPair data_ov030_020e6db0[];

extern Ov030TouchRegion data_ov030_020e6dcc[0xB];

s16 func_ov030_020bf360(void) {
    TouchCoord pos;
    TouchCoord coord;
    u32        i;

    TouchInput_GetCoord(&coord);
    pos = coord;
    for (i = 0; i < 0xB; i++) {
        if (data_ov030_020e6dcc[i].xMin <= pos.x && pos.x <= data_ov030_020e6dcc[i].xMax &&
            data_ov030_020e6dcc[i].yMin <= pos.y && pos.y <= data_ov030_020e6dcc[i].yMax)
        {
            return data_ov030_020e6dcc[i].id;
        }
    }
    return 6;
}

s32 func_ov030_020bf3e4(TaskPool* arg0, Ov030TouchData* arg1) {
    s16 i;

    for (i = 0; i < 3; i++) {
        if (arg1->taskIds[i] != -1) {
            if (func_ov030_020c01ec(EasyTask_GetTaskById(arg0, arg1->taskIds[i])) != 0) {
                return i;
            }
        }
    }
    return -1;
}

void func_ov030_020bf43c(TaskPool* arg0, Ov030TouchData* arg1, s32 arg2) {
    s32 i;

    for (i = 0; i < 3; i++) {
        if (arg1->taskIds[i] != -1) {
            Task* task = EasyTask_GetTaskById(arg0, arg1->taskIds[i]);
            s32   state;

            if (i == arg2) {
                state = 1;
            } else {
                state = 2;
            }
            func_ov030_020c01c4(task, state);
        }
    }
}

void func_ov030_020bf494(Ov030TouchData* arg0) {
    s32 i = 0;

    do {
        arg0->entries[i].value = 0;
        i++;
    } while (i < 7);
}

void func_ov030_020bf4b4(Ov030TouchData* arg0) {
    s32 i = 0;

    do {
        arg0->entries[i].count = 0;
        i++;
    } while (i < 7);
}

s32 func_ov030_020bf4d4(TaskPool* arg0, Task* task, s32* arg2) {
    Ov030TouchData* data = task->data;

    data->taskIds[0] = func_ov030_020c0184(arg0, *arg2, 0, 0xE2, 0xA7);
    data->taskIds[1] = -1;
    data->taskIds[2] = -1;
    data->taskIds[4] = -1;
    if (func_0202599c(2) != 0) {
        data->taskIds[1] = func_ov030_020c0184(arg0, *arg2, 2, 0x23, 0xA0);
    }
    if (func_0202599c(3) != 0) {
        data->taskIds[2] = func_ov030_020c0184(arg0, *arg2, 1, 0xE2, 0x21);
        func_ov030_020c01c4(EasyTask_GetTaskById(arg0, data->taskIds[2]), 1);
        data->taskIds[3] = func_ov030_020bfee4(arg0, *arg2, 1);
        data->unk1C      = 2;
        data->taskIds[4] = func_ov030_020c04d8(arg0, *arg2, data->unk1C);
    } else if (func_0202599c(2) != 0) {
        func_ov030_020c01c4(EasyTask_GetTaskById(arg0, data->taskIds[1]), 1);
        data->taskIds[3] = func_ov030_020bfee4(arg0, *arg2, 2);
        data->unk1C      = 1;
        data->taskIds[4] = func_ov030_020c04d8(arg0, *arg2, data->unk1C);
    } else {
        data->taskIds[3] = -1;
        data->unk1C      = -1;
    }
    data->unk0 = *arg2;
    data->unk4 = 0;
    func_ov030_020bf494(data);
    func_ov030_020bf4b4(data);
    data->unk3C = -1;
    data->unk40 = -1;
    data->unk44 = 0;
    data->unk4C = 7;
    data->unk48 = 0;
    return 1;
}

s32 func_ov030_020bf660(Ov030TouchData* arg0, s32 arg1) {
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
            func_ov030_020bf494(arg0);
            if (arg1 == 6) {
                Field_SetTouchEventResult(4);
                Field_SetTouchEventTarget(0x17);
            } else if (entry->count < data_ov030_020e6db0[arg1].b) {
                Field_SetTouchEventResult(4);
                Field_SetTouchEventTarget((s16)(entry->count + data_ov030_020e6db0[arg1].a));
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

s32 func_ov030_020bf7c4(TaskPool* arg0, Task* task) {
    Ov030TouchData* data = task->data;
    s32             r5;
    s16             t0;

    if (data->unk48 != 0) {
        return 1;
    }
    EasyFade_FadeBothDisplays(0, 0, 0x1000);
    if (TouchInput_WasTouchPressed() != 0) {
        r5 = func_ov030_020bf3e4(arg0, data);
        if (r5 != -1) {
            func_ov030_020bf43c(arg0, data, r5);
            if (r5 == 0) {
                Field_SetTouchEventResult(3);
                SndMgr_PlaySEWithPan(0xC4, 0xE2);
                data->unk48 = 1;
            } else if (r5 == 1) {
                func_ov030_020bf494(data);
                func_ov030_020bff68(EasyTask_GetTaskById(arg0, data->taskIds[3]), 2);
                func_ov030_020c050c(EasyTask_GetTaskById(arg0, data->taskIds[4]), 1);
                if (data->unk1C != 1) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget(0x19);
                    SndMgr_PlaySEWithPan(0xC4, 0x23);
                }
            } else if (r5 == 2) {
                func_ov030_020bff68(EasyTask_GetTaskById(arg0, data->taskIds[3]), 1);
                func_ov030_020c050c(EasyTask_GetTaskById(arg0, data->taskIds[4]), 2);
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
                s16 id = func_ov030_020bf360();

                if (id != data->unk4C) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget((s16)(func_ov030_020bf360() + 0x1A));
                }
                data->unk4C = func_ov030_020bf360();
            } else if (t0 == 2) {
                r5 = func_ov030_020bf360();
                if (r5 != data->unk4C && r5 != 6) {
                    Field_SetTouchEventResult(4);
                    Field_SetTouchEventTarget((s16)(r5 + 0x22));
                }
                data->unk4C = func_ov030_020bf360();
            }
        }
    } else if (TouchInput_IsTouchActive() != 0 && data->unk1C == 2) {
        r5 = func_ov030_020bf360();
        if (r5 == data->unk4C && data->unk4C != 6) {
            func_ov030_020c050c(EasyTask_GetTaskById(arg0, data->taskIds[4]), 5);
        } else {
            func_ov030_020c050c(EasyTask_GetTaskById(arg0, data->taskIds[4]), 2);
        }
        if (func_ov030_020bf660(data, r5) != 0 && data->unk4 == 0 && r5 != 6 && r5 == data->unk4C) {
            func_ov030_020c0730(arg0, data->unk0);
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

s32 func_ov030_020bfac0(void) {
    return 1;
}

s32 func_ov030_020bfac8(TaskPool* arg0, Task* task) {
    Ov030TouchData* data = task->data;
    s32             i    = 0;

    do {
        if (data->taskIds[i] != -1) {
            EasyTask_DeleteTask(arg0, data->taskIds[i]);
        }
        i++;
    } while (i < 5);
    return 1;
}

s32 func_ov030_020bfb08(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = data_ov030_020e6da0;

    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020bfb50(TaskPool* arg0, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(arg0, &data_ov030_020e6d94, NULL, 0, NULL, &param);
}
