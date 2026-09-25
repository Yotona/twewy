#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x000 */ Sprite        sprites[41];
    /* 0xA40 */ BOOL          visible[41];
    /* 0xAE4 */ ResultObject* owner;
} Result_num_Param; // Size: 0xAE8

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
} Result_num_Param_Args;

static SpriteFrameInfo* Result_num_Param_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_num_Param_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static SpriteFrameInfo* Result_num_Param_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_num_Param_LoadLabels(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                        SpriteAnimation* anim) {
    s16 positions[9][2] = {
        {0x1F, 0x1B},
        {0x1F, 0x74},
        {0x47, 0x29},
        {0x47, 0x38},
        {0x47, 0x47},
        {0x47, 0x56},
        {0x47, 0x65},
        {0x2C, 0x7E},
        {0x74, 0x7E},
    };
    s16 frames[9] = {0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A};

    for (s16 i = 0; i < 9; i++) {
        anim->unk_2A = frames[i];
        anim->unk_04 = positions[i][0];
        anim->unk_06 = positions[i][1];
        _Sprite_Load(&sprites[i], anim);
    }
}

static void Result_num_Param_LoadTime(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                      SpriteAnimation* anim) {
    s32 digits[5];
    u32 value;
    s16 i;
    s32 posX;

    value = owner->basePP;
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

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 9] = FALSE;
    }

    for (i = 0, posX = 0x7F; i < 5; i++) {
        anim->unk_2A = digits[i] + 0x21;
        anim->unk_04 = posX;
        anim->unk_06 = 0x26;
        _Sprite_Load(&sprites[i + 9], anim);
        posX += 7;
    }
}

static void Result_num_Param_LoadLevels(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                        SpriteAnimation* anim) {
    u16 digitsA[2];
    u16 digitsB[4];
    u16 posX[4] = {0, 0, 0, 0xA};
    u16 valueA;
    u16 valueB;
    s16 i;
    s32 x;
    s32 offset;

    valueA = owner->battleCount;
    valueB = owner->battleCountMultiplier;
    if (valueA > 99) {
        valueA = 99;
    }
    if (valueB > 99) {
        valueB = 99;
    }

    digitsA[0] = valueA / 10;
    digitsA[1] = valueA % 10;
    digitsB[0] = 0xB;
    digitsB[1] = valueB / 10;
    digitsB[2] = 0xA;
    digitsB[3] = valueB % 10;

    if (digitsA[0] == 0) {
        numParam->visible[14] = FALSE;
        offset                = -3;
    } else {
        offset = 0;
    }

    for (i = 0, x = 0x7F; i < 2; i++) {
        anim->unk_2A = digitsA[i] + 0xF;
        anim->unk_04 = offset + x;
        anim->unk_06 = 0x35;
        _Sprite_Load(&sprites[i + 14], anim);
        x += 7;
    }

    for (i = 0; i < 4; i++) {
        anim->unk_2A = digitsB[i] + 0xF;
        anim->unk_04 = posX[i] + 0xAD;
        anim->unk_06 = 0x35;
        _Sprite_Load(&sprites[i + 16], anim);
    }
}

// Nonmatching: digits[2]/digits[3] stack stores scheduled in swapped order
static void Result_num_Param_LoadRateA(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                       SpriteAnimation* anim) {
    u16 digits[4];
    u16 posX[4] = {0, 0, 0, 0xA};
    u16 value;

    value = owner->timeMultiplier;
    if (value > 99) {
        value = 99;
    }

    digits[0] = 0xB;
    digits[1] = value / 10;
    digits[3] = value % 10;
    digits[2] = 0xA;

    anim->unk_2A = owner->timeRank + 0x1B;
    anim->unk_04 = 0x83;
    anim->unk_06 = 0x44;
    _Sprite_Load(&sprites[20], anim);

    for (s16 i = 0; i < 4; i++) {
        anim->unk_2A = digits[i] + 0xF;
        anim->unk_04 = posX[i] + 0xAD;
        anim->unk_06 = 0x44;
        _Sprite_Load(&sprites[i + 21], anim);
    }
}

// Nonmatching: digits[2]/digits[3] stack stores scheduled in swapped order
static void Result_num_Param_LoadRateB(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                       SpriteAnimation* anim) {
    u16 digits[4];
    u16 posX[4] = {0, 0, 0, 0xA};

    u16 value = owner->rateBMultiplier;
    if (value > 99) {
        value = 99;
    }

    digits[0] = 0xB;
    digits[1] = value / 10;
    digits[3] = value % 10;
    digits[2] = 0xA;

    anim->unk_2A = owner->rateBRank + 0x1B;
    anim->unk_04 = 0x83;
    anim->unk_06 = 0x53;
    _Sprite_Load(&sprites[25], anim);

    for (s16 i = 0; i < 4; i++) {
        anim->unk_2A = digits[i] + 0xF;
        anim->unk_04 = posX[i] + 0xAD;
        anim->unk_06 = 0x53;
        _Sprite_Load(&sprites[i + 26], anim);
    }
}

// Nonmatching: digits[2]/digits[3] stack stores scheduled in swapped order
static void Result_num_Param_LoadRateC(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                       SpriteAnimation* anim) {
    u16 digits[4];
    u16 posX[4] = {0, 0, 0, 0xA};

    u16 level = owner->specialBonusLevel;
    u16 value = owner->specialBonusMultiplier;

    if (level > 9) {
        level = 9;
    }
    if (value > 99) {
        value = 99;
    }

    digits[0] = 0xB;
    digits[1] = value / 10;
    digits[3] = value % 10;
    digits[2] = 0xA;

    anim->unk_2A = level + 0xF;
    anim->unk_04 = 0x83;
    anim->unk_06 = 0x62;
    _Sprite_Load(&sprites[30], anim);

    for (s16 i = 0; i < 4; i++) {
        anim->unk_2A = digits[i] + 0xF;
        anim->unk_04 = posX[i] + 0xAD;
        anim->unk_06 = 0x62;
        _Sprite_Load(&sprites[i + 31], anim);
    }
}

static void Result_num_Param_LoadGrade(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                       SpriteAnimation* anim) {
    anim->unk_2A = owner->overallRank + 0x2B;
    anim->unk_04 = 0x56;
    anim->unk_06 = 0x7B;
    _Sprite_Load(&sprites[35], anim);
}

static void Result_num_Param_LoadPoints(Result_num_Param* numParam, ResultObject* owner, Sprite* sprites,
                                        SpriteAnimation* anim) {
    s32 digits[5];
    s16 i;
    s32 posX;

    for (i = 0; i < 5; i++) {
        numParam->visible[i + 36] = FALSE;
        digits[i]                 = 0;
    }
    numParam->visible[40] = TRUE;

    for (i = 0, posX = 0xB0; i < 5; i++) {
        anim->unk_2A = digits[i] + 0x21;
        anim->unk_04 = posX;
        anim->unk_06 = 0x7B;
        _Sprite_Load(&sprites[i + 36], anim);
        posX += 7;
    }
}

static void Result_num_Param_UpdatePoints(Result_num_Param* numParam) {
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
        numParam->visible[i + 36] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 36] = FALSE;
    }

    for (i = 0; i < 5; i++) {
        Result_SetSpriteFrame(&numParam->sprites[i + 36], digits[i] + 0x21);
    }
}

static const SpriteAnimation Result_num_Param_Anim = {
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
    .frameInfoCallback = Result_num_Param_GetFrameInfo,
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

static void Result_num_Param_Load(Result_num_Param* numParam, Sprite* sprites, Result_num_Param_Args* args) {
    SpriteAnimation anim = Result_num_Param_Anim;

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 41; i++) {
        numParam->visible[i] = TRUE;
    }
    numParam->visible[35] = FALSE;

    Result_num_Param_LoadLabels(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadTime(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadLevels(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadRateA(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadRateB(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadRateC(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadGrade(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_LoadPoints(numParam, numParam->owner, sprites, &anim);
}

static s32 Result_num_Param_Init(TaskPool* pool, Task* task, void* args) {
    Result_num_Param*      numParam     = task->data;
    Result_num_Param_Args* numParamArgs = args;

    numParam->owner = numParamArgs->owner;
    Result_num_Param_Load(numParam, numParam->sprites, numParamArgs);
    return 1;
}

static s32 Result_num_Param_Update(TaskPool* pool, Task* task, void* args) {
    Result_num_Param* numParam = task->data;
    ResultObject*     owner    = numParam->owner;

    Result_num_Param_UpdatePoints(numParam);

    if (owner->stageFlags & 8) {
        numParam->visible[35] = TRUE;
    }

    for (s32 i = 0; i < 41; i++) {
        Sprite_Update(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_Render(TaskPool* pool, Task* task, void* args) {
    Result_num_Param* numParam = task->data;

    for (s32 i = 0; i < 41; i++) {
        if (numParam->visible[i] != 0) {
            Sprite_RenderFrame(&numParam->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_num_Param_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_num_Param* numParam = task->data;

    for (s32 i = 0; i < 41; i++) {
        Sprite_Release(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_num_Param_Init,
        .update     = Result_num_Param_Update,
        .render     = Result_num_Param_Render,
        .cleanup    = Result_num_Param_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_num_Param = {"Tsk_Result_num_Param", Result_num_Param_RunTask, sizeof(Result_num_Param)};

s32 Result_num_Param_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_num_Param_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_num_Param, NULL, 0, NULL, &args);
}
