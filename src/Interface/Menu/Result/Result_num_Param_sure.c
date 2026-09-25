#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x000 */ Sprite        sprites[20];
    /* 0x500 */ BOOL          visible[20];
    /* 0x550 */ ResultObject* owner;
} Result_num_Param_sure; // Size: 0x554

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
} Result_num_Param_sure_Args;

static SpriteFrameInfo* Result_num_Param_sure_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_num_Param_sure_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_num_Param_sure = {"Tsk_Result_num_Param_sure", Result_num_Param_sure_RunTask,
                                                     sizeof(Result_num_Param_sure)};

static const SpriteAnimation Result_num_Param_sure_Anim = {
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
    .frameInfoCallback = Result_num_Param_sure_GetFrameInfo,
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

static SpriteFrameInfo* Result_num_Param_sure_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_num_Param_sure_LoadLabels(Result_num_Param_sure* numParam, ResultObject* owner, Sprite* sprites,
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

// Nonmatching: register allocation in the second and third digit loops
static void Result_num_Param_sure_LoadCounts(Result_num_Param_sure* numParam, ResultObject* owner, Sprite* sprites,
                                             SpriteAnimation* anim) {
    u16 value;
    s16 i;
    u16 espers[4];
    u16 civvies[4];
    u16 aliens[4];
    s32 posX;

    value     = owner->esperPP;
    espers[0] = value / 1000;
    value %= 1000;
    espers[1] = value / 100;
    value %= 100;
    espers[2] = value / 10;
    espers[3] = value % 10;

    value      = owner->civvyPP;
    civvies[0] = value / 1000;
    value %= 1000;
    civvies[1] = value / 100;
    value %= 100;
    civvies[2] = value / 10;
    civvies[3] = value % 10;

    value     = owner->alienPP;
    aliens[0] = value / 1000;
    value %= 1000;
    aliens[1] = value / 100;
    value %= 100;
    aliens[2] = value / 10;
    aliens[3] = value % 10;

    for (i = 0, posX = 0xBF; i < 4; i++) {
        anim->unk_2A = espers[i] + 0xF;
        anim->unk_04 = posX;
        anim->unk_06 = 0x46;
        _Sprite_Load(&sprites[i + 3], anim);
        posX += 7;
    }

    for (i = 0; i < 3; i++) {
        if (espers[i] != 0) {
            break;
        }
        numParam->visible[i + 3] = FALSE;
    }

    for (i = 0, posX = 0xBF; i < 4; i++) {
        anim->unk_2A = civvies[i] + 0xF;
        anim->unk_04 = posX;
        anim->unk_06 = 0x55;
        _Sprite_Load(&sprites[i + 7], anim);
        posX += 7;
    }

    for (i = 0; i < 3; i++) {
        if (civvies[i] != 0) {
            break;
        }
        numParam->visible[i + 7] = FALSE;
    }

    for (i = 0, posX = 0xBF; i < 4; i++) {
        anim->unk_2A = aliens[i] + 0xF;
        anim->unk_04 = posX;
        anim->unk_06 = 0x64;
        _Sprite_Load(&sprites[i + 11], anim);
        posX += 7;
    }

    for (i = 0; i < 3; i++) {
        if (aliens[i] != 0) {
            return;
        }
        numParam->visible[i + 11] = FALSE;
    }
}

static void Result_num_Param_sure_LoadPoints(Result_num_Param_sure* numParam, ResultObject* owner, Sprite* sprites,
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
        numParam->visible[i + 15] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 15] = FALSE;
    }

    for (i = 0, posX = 0x8E; i < 5; i++) {
        anim->unk_2A = digits[i] + 0x21;
        anim->unk_04 = posX;
        anim->unk_06 = 0x7B;
        _Sprite_Load(&sprites[i + 15], anim);
        posX += 7;
    }
}

static void Result_num_Param_sure_UpdatePoints(Result_num_Param_sure* numParam) {
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
        numParam->visible[i + 15] = TRUE;
    }

    for (i = 0; i < 4; i++) {
        if (digits[i] != 0) {
            break;
        }
        numParam->visible[i + 15] = FALSE;
    }

    for (i = 0; i < 5; i++) {
        Result_SetSpriteFrame(&numParam->sprites[i + 15], digits[i] + 0x21);
    }
}

static void Result_num_Param_sure_Load(Result_num_Param_sure* numParam, Sprite* sprites, Result_num_Param_sure_Args* args) {
    SpriteAnimation anim = Result_num_Param_sure_Anim;

    anim.dataType = args->dataType;

    for (s16 i = 0; i < 20; i++) {
        numParam->visible[i] = TRUE;
    }

    Result_num_Param_sure_LoadLabels(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_sure_LoadCounts(numParam, numParam->owner, sprites, &anim);
    Result_num_Param_sure_LoadPoints(numParam, numParam->owner, sprites, &anim);
}

static s32 Result_num_Param_sure_Init(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sure*      numParam     = task->data;
    Result_num_Param_sure_Args* numParamArgs = args;

    numParam->owner = numParamArgs->owner;
    Result_num_Param_sure_Load(numParam, numParam->sprites, numParamArgs);
    return 1;
}

static s32 Result_num_Param_sure_Update(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sure* numParam = task->data;

    Result_num_Param_sure_UpdatePoints(numParam);

    for (s32 i = 0; i < 20; i++) {
        Sprite_Update(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_sure_Render(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sure* numParam = task->data;

    for (s32 i = 0; i < 20; i++) {
        if (numParam->visible[i] != 0) {
            Sprite_RenderFrame(&numParam->sprites[i]);
        }
    }
    return 1;
}

static s32 Result_num_Param_sure_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_num_Param_sure* numParam = task->data;

    for (s32 i = 0; i < 20; i++) {
        Sprite_Release(&numParam->sprites[i]);
    }
    return 1;
}

static s32 Result_num_Param_sure_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_num_Param_sure_Init,
        .update     = Result_num_Param_sure_Update,
        .render     = Result_num_Param_sure_Render,
        .cleanup    = Result_num_Param_sure_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_num_Param_sure_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner) {
    Result_num_Param_sure_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_Result_num_Param_sure, NULL, 0, NULL, &args);
}
