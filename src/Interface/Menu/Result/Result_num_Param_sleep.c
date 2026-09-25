#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x000 */ Sprite        sprites[29];
    /* 0x740 */ BOOL          visible[29];
    /* 0x7B4 */ ResultObject* owner;
} Result_num_Param_sleep; // Size: 0x7B8

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
} Result_num_Param_sleep_Args;

static SpriteFrameInfo* Result_num_Param_sleep_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_num_Param_sleep_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_num_Param_sleep = {"Tsk_Result_num_Param_sleep", Result_num_Param_sleep_RunTask,
                                                      sizeof(Result_num_Param_sleep)};

static const SpriteAnimation Result_num_Param_sleep_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_num_Param_sleep_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[12],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* Result_num_Param_sleep_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_num_Param_sleep_LoadLabels(Result_num_Param_sleep* numParam, ResultObject* owner, Sprite* sprites,
                                              SpriteAnimation* anim) {
    s16 positions[3][2] = {
        {0x1F, 0x1B},
        {0x1F, 0x74},
        {0x51, 0x7E},
    };
    s16 frames[3] = {0x32, 0x33, 0x3A};

    for (s16 i = 0; i < 3; i++) {
        anim->unk_2A = frames[i];
        anim->unk_04 = positions[i][0];
        anim->unk_06 = positions[i][1];
        _Sprite_Load(&sprites[i], anim);
    }
}

// Nonmatching: register allocation
static void Result_num_Param_sleep_LoadRows(Result_num_Param_sleep* numParam, ResultObject* owner, Sprite* sprites,
                                            SpriteAnimation* anim) {
    u16 digits[3];
    s16 row;
    s16 i;
    s32 flagIndex;
    s32 rowX;
    s32 posX;
    s16 frameBase;
    s32 xOffset;
    u16 value;

    for (flagIndex = 0, row = 0, rowX = 0x25; row < 7; row++) {
        value     = owner->dailyPP[row];
        digits[0] = value / 100;
        value %= 100;
        digits[1] = value / 10;
        digits[2] = value % 10;

        frameBase = 0xF;

        if (digits[0] == 0) {
            if (digits[1] == 0) {
                numParam->visible[(row + 1) * 3] = FALSE;
                numParam->visible[flagIndex + 4] = FALSE;
                numParam->visible[flagIndex + 5] = TRUE;
                xOffset                          = -7;
                if (digits[2] == 0) {
                    frameBase = 0x31;
                }
            } else {
                numParam->visible[(row + 1) * 3] = FALSE;
                numParam->visible[flagIndex + 4] = TRUE;
                numParam->visible[flagIndex + 5] = TRUE;
                xOffset                          = -4;
            }
        } else {
            numParam->visible[(row + 1) * 3] = TRUE;
            numParam->visible[flagIndex + 4] = TRUE;
            numParam->visible[flagIndex + 5] = TRUE;
            xOffset                          = 0;
        }

        for (i = 0, posX = rowX; i < 3; i++) {
            anim->unk_2A = frameBase + digits[i];
            anim->unk_04 = xOffset + posX;
            anim->unk_06 = 0x62;
            _Sprite_Load(&sprites[(row + 1) * 3 + i], anim);
            posX += 7;
        }

        flagIndex += 3;
        rowX += 0x1C;
    }
}

static void Result_num_Param_sleep_LoadPoints(Result_num_Param_sleep* numParam, ResultObject* owner, Sprite* sprites,
                                              SpriteAnimation* anim) {
    s32 digits[5];
    u32 value;
    s32 posX;
    s16 i;

    value     = owner->displayedPP;
    digits[0] = value / 10000;
    value %= 10000;
    digits[1] = value / 1000;
    value %= 1000;
    digits[2] = value / 100;
    value %= 100;
    digits[3] = value / 10;
    digits[4] = value % 10;

    for (i = 0; i < 5; i++) {
        numParam->visible[i + 24] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 24] = FALSE;
    }

    for (i = 0, posX = 0x8E; i < 5; i++) {
        anim->unk_2A = digits[i] + 0x21;
        anim->unk_04 = posX;
        anim->unk_06 = 0x7B;
        _Sprite_Load(&sprites[i + 24], anim);
        posX += 7;
    }
}

static void Result_num_Param_sleep_UpdatePoints(Result_num_Param_sleep* numParam) {
    s32 digits[5];
    u32 value;
    s16 i;

    value = numParam->owner->displayedPP;
    if (value > 99999) {
        value = 99999;
    }

    digits[0] = value / 10000;
    value %= 10000;
    digits[1] = value / 1000;
    value %= 1000;
    digits[2] = value / 100;
    value %= 100;
    digits[3] = value / 10;
    digits[4] = value % 10;

    for (i = 0; i < 5; i++) {
        numParam->visible[i + 24] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 24] = FALSE;
    }

    for (i = 0; i < 5; i++) {
        Result_SetSpriteFrame(&numParam->sprites[i + 24], digits[i] + 0x21);
    }
}

static void Result_num_Param_sleep_Load(Result_num_Param_sleep* numParam, Sprite* sprites, Result_num_Param_sleep_Args* args) {
    SpriteAnimation anim = Result_num_Param_sleep_Anim;

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 29; i++) {
        numParam->visible[i] = TRUE;
    }

    Result_num_Param_sleep_LoadLabels(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_sleep_LoadRows(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_sleep_LoadPoints(numParam, numParam->owner, sprites, &anim);
}

static s32 Result_num_Param_sleep_Init(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sleep*      numParam     = task->data;
    Result_num_Param_sleep_Args* numParamArgs = args;

    numParam->owner = numParamArgs->owner;
    Result_num_Param_sleep_Load(numParam, numParam->sprites, numParamArgs);
    return 1;
}

static s32 Result_num_Param_sleep_Update(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sleep* numParam = task->data;

    Result_num_Param_sleep_UpdatePoints(numParam);

    for (s32 i = 0; i < 29; i++) {
        Sprite_Update(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_sleep_Render(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sleep* numParam = task->data;

    for (s32 i = 0; i < 29; i++) {
        if (numParam->visible[i] != 0) {
            Sprite_RenderFrame(&numParam->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_num_Param_sleep_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sleep* numParam = task->data;

    for (s32 i = 0; i < 29; i++) {
        Sprite_Release(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_sleep_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_num_Param_sleep_Init,
        .update     = Result_num_Param_sleep_Update,
        .render     = Result_num_Param_sleep_Render,
        .cleanup    = Result_num_Param_sleep_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_num_Param_sleep_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_num_Param_sleep_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_num_Param_sleep, NULL, 0, NULL, &args);
}
