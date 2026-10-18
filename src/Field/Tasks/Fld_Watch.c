#include "Field/Tasks/Fld_Watch.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x000 */ s32    unk0;
    /* 0x004 */ Sprite sprites[5];
    /* 0x144 */ u32    hour; // hour/minute/second
    /* 0x148 */ u32    minute;
    /* 0x14C */ u32    second;
    /* 0x150 */ s16    frames;
} FldWatchData; // Size: 0x154

extern void func_0204172c(u32* time);

static SpriteFrameInfo* Fld_Watch_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2);
static s32              Fld_Watch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

#ifdef REGION_USA
static const s16 Fld_Watch_DigitPosX[5] = {192, 178, 183, 190, 195};
#else
static const s16 Fld_Watch_DigitPosX[5] = {192, 192, 197, 204, 209};
#endif

static const TaskHandle Tsk_Fld_Watch = {"Tsk_Fld_Watch", Fld_Watch_RunTask, sizeof(FldWatchData)};

static const SpriteAnimation Fld_Watch_Anim = {
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
    .frameInfoCallback = Fld_Watch_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
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

static SpriteFrameInfo* Fld_Watch_GetFrameInfo(Sprite* sprite, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(sprite, arg2);
}

static void Fld_Watch_LoadSprite(Sprite* sprite, s32 arg1, s16 arg2, s16 arg3) {
    SpriteAnimation anim = Fld_Watch_Anim;

    anim.posX      = arg2;
    anim.animIndex = arg3;
    anim.dataType  = (u16)arg1;
    _Sprite_Load(sprite, &anim);
}

static u32 Fld_Watch_To12Hour(u32 arg0) {
    if (arg0 == 0) {
        return 0xC;
    }
    if (arg0 >= 0xD) {
        arg0 -= 0xC;
    }
    return arg0;
}

static s32 Fld_Watch_Init(TaskPool* pool, Task* task, void* args) {
    FldWatchData* data = task->data;
    s32*          arg2 = args;
    u32           digit;

    data->unk0   = *arg2;
    data->frames = 0;
    func_0204172c(&data->hour);
    if ((u32)data->hour < 0xC) {
        Fld_Watch_LoadSprite(&data->sprites[0], data->unk0, Fld_Watch_DigitPosX[0], 0xF);
    } else {
        Fld_Watch_LoadSprite(&data->sprites[0], data->unk0, Fld_Watch_DigitPosX[0], 0x1F);
    }
    digit = Fld_Watch_To12Hour(data->hour);
    Fld_Watch_LoadSprite(&data->sprites[1], data->unk0, Fld_Watch_DigitPosX[1], (s16)(digit / 10 + 0x13));
    Fld_Watch_LoadSprite(&data->sprites[2], data->unk0, Fld_Watch_DigitPosX[2], (s16)(digit % 10 + 0x13));
    Fld_Watch_LoadSprite(&data->sprites[3], data->unk0, Fld_Watch_DigitPosX[3], (s16)(data->minute / 10 + 0x13));
    Fld_Watch_LoadSprite(&data->sprites[4], data->unk0, Fld_Watch_DigitPosX[4], (s16)(data->minute % 10 + 0x13));

    {
        s32 i;

        for (i = 0; i < 5; i++) {
            Sprite_Update(&data->sprites[i]);
        }
    }
    return 1;
}

// When SystemFlags.unk_10 is set, the watch re-reads the saved time and
// force-updates every digit instead of counting up on its own.
static s32 Fld_Watch_Update(TaskPool* pool, Task* task, void* args) {
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
                digit = Fld_Watch_To12Hour((s16)data->hour);
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

static s32 Fld_Watch_Render(TaskPool* pool, Task* task, void* args) {
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

static s32 Fld_Watch_Destroy(TaskPool* pool, Task* task, void* args) {
    FldWatchData* data = task->data;
    s32           i;

    for (i = 0; i < 5; i++) {
        Sprite_Release(&data->sprites[i]);
    }
    return 1;
}

static s32 Fld_Watch_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Watch_Init,
        .update     = Fld_Watch_Update,
        .render     = Fld_Watch_Render,
        .cleanup    = Fld_Watch_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Watch_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &Tsk_Fld_Watch, NULL, 0, NULL, &param);
}
