#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x000 */ Sprite        sprites[26];
    /* 0x680 */ BOOL          visible[26];
    /* 0x6E8 */ ResultObject* owner;
} Result_num_Param_mabs; // Size: 0x6EC

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
} Result_num_Param_mabs_Args;

static SpriteFrameInfo* Result_num_Param_mabs_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_num_Param_mabs_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static SpriteFrameInfo* Result_num_Param_mabs_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_num_Param_mabs_LoadLabels(Result_num_Param_mabs* numParam, ResultObject* owner, Sprite* sprites,
                                             SpriteAnimation* anim) {
    s16 positions[6][2] = {
        {0x1F, 0x1B},
        {0x1F, 0x6A},
        {0x47, 0x2A},
        {0x47, 0x77},
        {0x47, 0x40},
        {0x47, 0x56},
    };
    s16 frames[6] = {0x32, 0x33, 0x34, 0x3A, 0x3B, 0x3C};

    for (s16 i = 0; i < 6; i++) {
        anim->unk_2A = frames[i];
        anim->unk_04 = positions[i][0];
        anim->unk_06 = positions[i][1];
        _Sprite_Load(&sprites[i], anim);
    }
}

static void Result_num_Param_mabs_LoadTime(Result_num_Param_mabs* numParam, ResultObject* owner, Sprite* sprites,
                                           SpriteAnimation* anim) {
    s32 digits[5];
    u32 value;
    s16 i;
    s32 posX;

    value     = owner->basePP;
    digits[0] = value / 10000;
    value %= 10000;
    digits[1] = value / 1000;
    value %= 1000;
    digits[2] = value / 100;
    value %= 100;
    digits[3] = value / 10;
    digits[4] = value % 10;

    for (i = 0; i < 5; i++) {
        numParam->visible[i + 6] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 6] = FALSE;
    }

    for (i = 0, posX = 0x8E; i < 5; i++) {
        anim->unk_2A = digits[i] + 0x21;
        anim->unk_04 = posX;
        anim->unk_06 = 0x27;
        _Sprite_Load(&sprites[i + 6], anim);
        posX += 7;
    }
}

// Nonmatching: digits[2]/digits[3] stack stores scheduled in swapped order
static void Result_num_Param_mabs_LoadRateA(Result_num_Param_mabs* numParam, ResultObject* owner, Sprite* sprites,
                                            SpriteAnimation* anim) {
    u16 digits[4];
    u16 posX[4] = {0, 0, 0, 0xA};

    u16 value = owner->mabsPinCountMultiplier;
    if (value > 99) {
        value = 99;
    }

    digits[0] = 11;
    digits[1] = value / 10;
    digits[2] = 10;
    digits[3] = value % 10;

    anim->unk_2A = owner->mabsPinCountRank + 0x1B;
    anim->unk_04 = 0x93;
    anim->unk_06 = 0x3D;
    _Sprite_Load(&sprites[11], anim);

    for (s16 i = 0; i < 4; i++) {
        anim->unk_2A = digits[i] + 0xF;
        anim->unk_04 = posX[i] + 0xBD;
        anim->unk_06 = 0x3D;
        _Sprite_Load(&sprites[i + 12], anim);
    }
}

// Nonmatching: digits[2]/digits[3] stack stores scheduled in swapped order
static void Result_num_Param_mabs_LoadRateB(Result_num_Param_mabs* numParam, ResultObject* owner, Sprite* sprites,
                                            SpriteAnimation* anim) {
    u16 digits[4];
    u16 posX[4] = {0, 0, 0, 0xA};

    u16 value = owner->mabsRateBMultiplier;
    if (value > 99) {
        value = 99;
    }

    digits[0] = 0xB;
    digits[1] = value / 10;
    digits[2] = 0xA;
    digits[3] = value % 10;

    anim->unk_2A = owner->mabsRateBRank + 0x1B;
    anim->unk_04 = 0x93;
    anim->unk_06 = 0x53;
    _Sprite_Load(&sprites[16], anim);

    for (s16 i = 0; i < 4; i++) {
        anim->unk_2A = digits[i] + 0xF;
        anim->unk_04 = posX[i] + 0xBD;
        anim->unk_06 = 0x53;
        _Sprite_Load(&sprites[i + 17], anim);
    }
}

static void Result_num_Param_mabs_LoadPoints(Result_num_Param_mabs* numParam, ResultObject* owner, Sprite* sprites,
                                             SpriteAnimation* anim) {
    s32 digits[5];
    u32 value;
    s16 i;
    s32 posX;

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
        numParam->visible[i + 21] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 21] = FALSE;
    }

    for (i = 0, posX = 0x8E; i < 5; i++) {
        anim->unk_2A = digits[i] + 0x21;
        anim->unk_04 = posX;
        anim->unk_06 = 0x74;
        _Sprite_Load(&sprites[i + 21], anim);
        posX += 7;
    }
}

static void Result_num_Param_mabs_UpdatePoints(Result_num_Param_mabs* numParam) {
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
        numParam->visible[i + 21] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 21] = FALSE;
    }

    for (i = 0; i < 5; i++) {
        Result_SetSpriteFrame(&numParam->sprites[i + 21], digits[i] + 0x21);
    }
}

static const SpriteAnimation Result_num_Param_mabs_Anim = {
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
    .frameInfoCallback = Result_num_Param_mabs_GetFrameInfo,
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

static void Result_num_Param_mabs_Load(Result_num_Param_mabs* numParam, Sprite* sprites, Result_num_Param_mabs_Args* args) {
    SpriteAnimation anim = Result_num_Param_mabs_Anim;

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 26; i++) {
        numParam->visible[i] = TRUE;
    }

    Result_num_Param_mabs_LoadLabels(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_mabs_LoadTime(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_mabs_LoadRateA(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_mabs_LoadRateB(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_mabs_LoadPoints(numParam, numParam->owner, sprites, &anim);
}

static s32 Result_num_Param_mabs_Init(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_mabs*      numParam     = task->data;
    Result_num_Param_mabs_Args* numParamArgs = args;

    numParam->owner = numParamArgs->owner;
    Result_num_Param_mabs_Load(numParam, numParam->sprites, numParamArgs);
    return 1;
}

static s32 Result_num_Param_mabs_Update(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_mabs* numParam = task->data;

    Result_num_Param_mabs_UpdatePoints(numParam);

    for (s32 i = 0; i < 26; i++) {
        Sprite_Update(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_mabs_Render(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_mabs* numParam = task->data;

    for (s32 i = 0; i < 26; i++) {
        if (numParam->visible[i] != 0) {
            Sprite_RenderFrame(&numParam->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_num_Param_mabs_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_mabs* numParam = task->data;

    for (s32 i = 0; i < 26; i++) {
        Sprite_Release(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_mabs_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_num_Param_mabs_Init,
        .update     = Result_num_Param_mabs_Update,
        .render     = Result_num_Param_mabs_Render,
        .cleanup    = Result_num_Param_mabs_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_num_Param_mabs = {"Tsk_Result_num_Param_mabs", Result_num_Param_mabs_RunTask,
                                                     sizeof(Result_num_Param_mabs)};

s32 Result_num_Param_mabs_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_num_Param_mabs_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_num_Param_mabs, NULL, 0, NULL, &args);
}
