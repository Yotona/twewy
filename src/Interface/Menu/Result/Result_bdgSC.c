#include "Interface/Menu/Result.h"

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ BOOL          visible;
    /* 0x44 */ ResultObject* owner;
} Result_bdgSC; // Size: 0x48

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           index;
    /* 0xA */ u16           graphicIndex;
    /* 0xC */ u16           status;
    /* 0xE */ u8            badgeCount;
} Result_bdgSC_Args;

static SpriteFrameInfo* Result_bdgSC_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_bdgSC_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const SpriteAnimation Result_bdgSC_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_bdgSC_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[13],
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

static SpriteFrameInfo* Result_bdgSC_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x2BC000);
}

static void Result_bdgSC_Load(Result_bdgSC* bdgSC, Sprite* sprite, Result_bdgSC_Args* args) {
    SpriteAnimation anim    = Result_bdgSC_Anim;
    s16             posX[6] = {0x2C, 0x4E, 0x70, 0x92, 0xB4, 0xD6};

    anim.dataType = args->dataType;
    anim.unk_04   = posX[args->index] + 9;
    anim.unk_06   = 0x9C;
    anim.bits_7_9 = 6;

    if (args->index >= args->badgeCount || args->graphicIndex == 0xFFFF) {
        anim.unk_2A    = 0x25;
        bdgSC->visible = FALSE;
    } else {
        switch (args->status) {
            case 0: {
                anim.unk_2A    = 0x25;
                bdgSC->visible = FALSE;
            } break;

            case 1: {
                anim.unk_2A    = 0x26;
                bdgSC->visible = TRUE;
            } break;

            case 2: {
                anim.unk_2A    = 0x27;
                bdgSC->visible = TRUE;
            } break;
        }
    }

    _Sprite_Load(sprite, &anim);
}

static s32 Result_bdgSC_Init(TaskPool* pool, Task* task, void* args) {
    Result_bdgSC*      bdgSC     = task->data;
    Result_bdgSC_Args* bdgSCArgs = args;

    bdgSC->owner = bdgSCArgs->owner;
    Result_bdgSC_Load(bdgSC, &bdgSC->sprite, bdgSCArgs);
    return 1;
}

static s32 Result_bdgSC_Update(TaskPool* pool, Task* task, void* args) {
    Result_bdgSC* bdgSC = task->data;

    Sprite_Update(&bdgSC->sprite);
    return 1;
}

static s32 Result_bdgSC_Render(TaskPool* pool, Task* task, void* args) {
    Result_bdgSC* bdgSC = task->data;

    if (bdgSC->visible != 0) {
        Sprite_RenderFrame(&bdgSC->sprite);
    }
    return 1;
}

static s32 Result_bdgSC_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_bdgSC* bdgSC = task->data;

    Sprite_Release(&bdgSC->sprite);
    return 1;
}

static s32 Result_bdgSC_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_bdgSC_Init,
        .update     = Result_bdgSC_Update,
        .render     = Result_bdgSC_Render,
        .cleanup    = Result_bdgSC_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

static const TaskHandle Tsk_Result_bdgSC = {"Tsk_Result_bdgSC", Result_bdgSC_RunTask, sizeof(Result_bdgSC)};

s32 Result_bdgSC_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner) {
    Result_bdgSC_Args args;

    args.dataType     = dataType;
    args.owner        = owner;
    args.index        = index;
    args.graphicIndex = owner->badges[index].pinID;
    args.status       = owner->badges[index].unk_2F;
    args.badgeCount   = owner->badgeCount;

    return EasyTask_CreateTask(pool, &Tsk_Result_bdgSC, NULL, 0, NULL, &args);
}
