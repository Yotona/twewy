// USA 0x020bd890-0x020bdec8: the watch/clock HUD task. Five sprites draw the
// time; the task data holds the five Sprites, the current digit codes at 0x144/
// 0x148/0x14C and a frame timer at 0x150.
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Field/Ov030Data.h"
#include "Save.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x000 */ s32    unk0;
    /* 0x004 */ Sprite sprites[5];
    /* 0x144 */ u32    hour; // hour/minute/second, filled by func_0204172c
    /* 0x148 */ u32    minute;
    /* 0x14C */ u32    second;
    /* 0x150 */ s16    frames;
} FldWatchData; // Size: 0x154

extern const char data_ov030_021073a8[];

extern void func_0204172c(u32* time);

SpriteFrameInfo* func_ov030_020bd890(Sprite* sprite, s32 arg1, s32 arg2);
void             func_ov030_020bd92c(Sprite* sprite, s32 arg1, s16 arg2, s16 arg3);
u32              func_ov030_020bd9a4(u32 arg0);
s32              func_ov030_020bd9bc(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bdb04(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bdddc(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bde20(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bde50(TaskPool* pool, Task* task, void* args, s32 stage);
s32              func_ov030_020bde98(TaskPool* pool, s32 arg1);

const TaskHandle data_ov030_020e6288 = {data_ov030_021073a8, func_ov030_020bde50, sizeof(FldWatchData)};

const TaskStages data_ov030_020e6294 = {
    {func_ov030_020bd9bc, func_ov030_020bdb04, func_ov030_020bdddc, func_ov030_020bde20}
};

const SpriteAnimation data_ov030_020e62a4 = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0C00,
    .posX              = 0xC0,
    .posY              = 0,
    .frameInfoCallback = func_ov030_020bd890,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020d9944,
    .unk_18            = 2,
    .packIndex         = 4,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 0xF,
};

SpriteFrameInfo* func_ov030_020bd890(Sprite* sprite, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(sprite, arg2);
}

void func_ov030_020bd92c(Sprite* sprite, s32 arg1, s16 arg2, s16 arg3) {
    SpriteAnimation anim = data_ov030_020e62a4;

    anim.posX      = arg2;
    anim.animIndex = arg3;
    anim.dataType  = (u16)arg1;
    _Sprite_Load(sprite, &anim);
}

u32 func_ov030_020bd9a4(u32 arg0) {
    if (arg0 == 0) {
        return 0xC;
    }
    if (arg0 >= 0xD) {
        arg0 -= 0xC;
    }
    return arg0;
}

s32 func_ov030_020bd9bc(TaskPool* pool, Task* task, void* args) {
    FldWatchData* data = task->data;
    s32*          arg2 = args;
    u32           digit;

    data->unk0   = *arg2;
    data->frames = 0;
    func_0204172c(&data->hour);
    if ((u32)data->hour < 0xC) {
        func_ov030_020bd92c(&data->sprites[0], data->unk0, 0xC0, 0xF);
    } else {
        func_ov030_020bd92c(&data->sprites[0], data->unk0, 0xC0, 0x1F);
    }
    digit = func_ov030_020bd9a4(data->hour);
    func_ov030_020bd92c(&data->sprites[1], data->unk0, 0xB2, (s16)(digit / 10 + 0x13));
    func_ov030_020bd92c(&data->sprites[2], data->unk0, 0xB7, (s16)(digit % 10 + 0x13));
    func_ov030_020bd92c(&data->sprites[3], data->unk0, 0xBE, (s16)(data->minute / 10 + 0x13));
    func_ov030_020bd92c(&data->sprites[4], data->unk0, 0xC3, (s16)(data->minute % 10 + 0x13));
    {
        s32 i;

        for (i = 0; i < 5; i++) {
            Sprite_Update(&data->sprites[i]);
        }
    }
    return 1;
}

// SystemFlags.unk_10: when set, the watch re-reads the saved time and
// force-updates every digit instead of counting up on its own.
s32 func_ov030_020bdb04(TaskPool* pool, Task* task, void* args) {
    FldWatchData* data = task->data;
    u32           digit;
    u32           resync;

    resync = SystemStatusFlags.unk_10 != 0;
    if (resync != 0) {
        func_0204172c(&data->hour);
        Sprite_SetAnimation(&data->sprites[3], data->sprites[3].animData, (s16)(data->minute / 10 + 0x13),
                            data->sprites[3].cellTable);
        Sprite_SetAnimation(&data->sprites[4], data->sprites[4].animData, (s16)(data->minute % 10 + 0x13),
                            data->sprites[4].cellTable);
        Sprite_ChangeAnimation(&data->sprites[1], data->sprites[1].animData, (s16)(data->hour / 10 + 0x13),
                               data->sprites[1].cellTable);
        Sprite_ChangeAnimation(&data->sprites[2], data->sprites[2].animData, (s16)(data->hour % 10 + 0x13),
                               data->sprites[2].cellTable);
        Sprite_Update(&data->sprites[3]);
        Sprite_Update(&data->sprites[4]);
        Sprite_Update(&data->sprites[1]);
        Sprite_Update(&data->sprites[2]);
        Sprite_Update(&data->sprites[0]);
        data->frames = 0;
        return 1;
    }
    data->frames = data->frames + 1;
    if (data->frames >= 0x3C) {
        data->frames = 0;
        data->second = data->second + 1;
        if (data->second >= 0x3C) {
            data->second = 0;
            data->minute = data->minute + 1;
            if (data->minute >= 0x3C) {
                data->minute = 0;
                data->hour   = data->hour + 1;
                if (data->hour >= 0x18) {
                    data->hour = 0;
                }
                if ((u32)data->hour < 0xC) {
                    Sprite_ChangeAnimation(&data->sprites[0], data->sprites[0].animData, 0xF, data->sprites[0].cellTable);
                } else {
                    Sprite_ChangeAnimation(&data->sprites[0], data->sprites[0].animData, 0x1F, data->sprites[0].cellTable);
                }
                digit = func_ov030_020bd9a4((s16)data->hour);
                Sprite_ChangeAnimation(&data->sprites[1], data->sprites[1].animData, (s16)(digit / 10 + 0x13),
                                       data->sprites[1].cellTable);
                Sprite_ChangeAnimation(&data->sprites[2], data->sprites[2].animData, (s16)(digit % 10 + 0x13),
                                       data->sprites[2].cellTable);
                Sprite_Update(&data->sprites[1]);
                Sprite_Update(&data->sprites[2]);
            }
            Sprite_SetAnimation(&data->sprites[3], data->sprites[3].animData, (s16)(data->minute / 10 + 0x13),
                                data->sprites[3].cellTable);
            Sprite_SetAnimation(&data->sprites[4], data->sprites[4].animData, (s16)(data->minute % 10 + 0x13),
                                data->sprites[4].cellTable);
            Sprite_Update(&data->sprites[3]);
            Sprite_Update(&data->sprites[4]);
        }
    }
    Sprite_Update(&data->sprites[0]);
    return 1;
}

s32 func_ov030_020bdddc(TaskPool* pool, Task* task, void* args) {
    FldWatchData* data = task->data;
    s32           i;

    for (i = 0; i < 5; i++) {
        // A leading zero of the hour is not drawn.
        if (i != 1 || data->sprites[i].animIndex != 0x13) {
            Sprite_RenderFrame(&data->sprites[i]);
        }
    }
    return 1;
}

s32 func_ov030_020bde20(TaskPool* pool, Task* task, void* args) {
    FldWatchData* data = task->data;
    s32           i;

    for (i = 0; i < 5; i++) {
        Sprite_Release(&data->sprites[i]);
    }
    return 1;
}

s32 func_ov030_020bde50(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov030_020e6294;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020bde98(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &data_ov030_020e6288, NULL, 0, NULL, &param);
}
