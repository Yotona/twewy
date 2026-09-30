#include "Interface/Menu/FriendList.h"

// JP has no month-name sprite: the date is drawn as plain digits.
#ifdef REGION_USA
    #define FRIENDLIST_NUMDATE_SPRITE_COUNT 12
#else
    #define FRIENDLIST_NUMDATE_SPRITE_COUNT 11
#endif

typedef struct {
    /* 0x000 */ Sprite sprites[FRIENDLIST_NUMDATE_SPRITE_COUNT]; // [0]: "met" label, [1]-[10]: digits, [11]: date label
    /* 0x300 */ BOOL   visible[FRIENDLIST_NUMDATE_SPRITE_COUNT];
    /* 0x330 */ FriendListObject* friendList;
} FriendList_numDate; // Size: 0x334 (JP: 0x2F0)

typedef struct {
    /* 0x0 */ s32               dataType;
    /* 0x4 */ FriendListObject* friendList;
    /* 0x8 */ u16               timesMet;
    /* 0xA */ u16               year;
    /* 0xC */ u16               month;
    /* 0xE */ u16               day;
} FriendList_numDate_Args;

static SpriteFrameInfo* FriendList_numDate_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              FriendList_numDate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_FriendList_numDate = {"Tsk_FriendList_numDate", FriendList_numDate_RunTask,
                                                  sizeof(FriendList_numDate)};

static const SpriteAnimation FriendList_numDate_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = FriendList_numDate_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &FriendList_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* FriendList_numDate_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

#ifdef REGION_USA
static void FriendList_numDate_Load(FriendList_numDate* numDate, Sprite* sprites, FriendList_numDate_Args* args) {
    SpriteAnimation anim     = FriendList_numDate_Anim;
    s16             posX[12] = {128, 88, 94, 210, 216, 222, 228, 0, 132, 191, 197, 202};
    s16             digits[10];
    s16             offsetX[10];
    s16             i;

    for (i = 0; i < 11; i++) {
        offsetX[i] = 0;
    }

    digits[0] = args->timesMet / 10;
    digits[1] = args->timesMet % 10;

    for (i = 0; i < 12; i++) {
        numDate->visible[i] = TRUE;
    }

    anim.dataType  = args->dataType;
    anim.animIndex = 24;
    anim.posX      = posX[0];
    anim.posY      = 183;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 36;
    anim.posX      = posX[11];
    anim.posY      = 183;
    _Sprite_Load(&sprites[11], &anim);

    if (args->year == 0 && args->month == 0 && args->day == 0) {
        for (i = 2; i <= 9; i++) {
            digits[i] = 10;
        }
        numDate->visible[1]  = FALSE;
        numDate->visible[6]  = FALSE;
        numDate->visible[7]  = FALSE;
        numDate->visible[8]  = FALSE;
        numDate->visible[9]  = FALSE;
        numDate->visible[10] = FALSE;
        numDate->visible[11] = FALSE;
        offsetX[0]           = -3;
        offsetX[2]           = -16;
        offsetX[3]           = -14;
        offsetX[4]           = -12;
    } else {
        digits[2] = 2;
        digits[3] = 0;
        digits[4] = args->year / 10;
        digits[5] = args->year % 10;
        digits[6] = args->month / 10;
        digits[7] = args->month % 10;
        digits[8] = args->day / 10;
        digits[9] = args->day % 10;
        if (args->timesMet < 10) {
            numDate->visible[1] = FALSE;
            offsetX[0]          = -3;
        }
        digits[7]           = args->month + 12;
        numDate->visible[7] = FALSE;
        offsetX[6]          = 0;
        if (args->day < 10) {
            numDate->visible[9] = FALSE;
            offsetX[8]          = -3;
        }
    }

    for (i = 0; i < 10; i++) {
        anim.animIndex = digits[i] + 25;
        anim.posX      = posX[i + 1] + offsetX[i];
        if (i == 7) {
            anim.posY = 183;
        } else {
            anim.posY = 182;
        }
        _Sprite_Load(&sprites[i + 1], &anim);
    }
}
#else
static void FriendList_numDate_Load(FriendList_numDate* numDate, Sprite* sprites, FriendList_numDate_Args* args) {
    SpriteAnimation anim     = FriendList_numDate_Anim;
    s16             posX[11] = {128, 88, 96, 165, 173, 181, 189, 205, 213, 230, 238};
    s16             digits[10];
    s16             offsetX = 0;
    s16             i;

    digits[0] = args->timesMet / 10;
    digits[1] = args->timesMet % 10;

    if (args->year == 0) {
        for (i = 2; i <= 9; i++) {
            digits[i] = 10;
        }
    } else {
        digits[2] = 2;
        digits[3] = 0;
        digits[4] = args->year / 10;
        digits[5] = args->year % 10;
        digits[6] = args->month / 10;
        digits[7] = args->month % 10;
        digits[8] = args->day / 10;
        digits[9] = args->day % 10;
    }

    for (i = 0; i < 11; i++) {
        numDate->visible[i] = TRUE;
    }

    if (args->timesMet < 10) {
        numDate->visible[1] = FALSE;
        offsetX             = -4;
    }

    anim.dataType  = args->dataType;
    anim.animIndex = 24;
    anim.posX      = posX[0];
    anim.posY      = 183;
    _Sprite_Load(&sprites[0], &anim);

    for (i = 0; i < 10; i++) {
        anim.animIndex = digits[i] + 25;
        if (i == 1) {
            anim.posX = offsetX + posX[i + 1];
        } else {
            anim.posX = posX[i + 1];
        }
        anim.posY = 183;
        _Sprite_Load(&sprites[i + 1], &anim);
    }
}
#endif

static s32 FriendList_numDate_Init(TaskPool* pool, Task* task, void* args) {
    FriendList_numDate_Args* numArgs = args;
    FriendList_numDate*      numDate = task->data;

    numDate->friendList = numArgs->friendList;
    FriendList_numDate_Load(numDate, numDate->sprites, numArgs);
    return 1;
}

static s32 FriendList_numDate_Update(TaskPool* pool, Task* task, void* args) {
    FriendList_numDate* numDate = task->data;

    for (s32 i = 0; i < FRIENDLIST_NUMDATE_SPRITE_COUNT; i++) {
        Sprite_Update(&numDate->sprites[i]);
    }
    return 1;
}

static s32 FriendList_numDate_Render(TaskPool* pool, Task* task, void* args) {
    FriendList_numDate* numDate = task->data;

    for (s32 i = 0; i < FRIENDLIST_NUMDATE_SPRITE_COUNT; i++) {
        if (numDate->visible[i]) {
            Sprite_RenderFrame(&numDate->sprites[i]);
        }
    }
    return 1;
}

static s32 FriendList_numDate_Destroy(TaskPool* pool, Task* task, void* args) {
    FriendList_numDate* numDate = task->data;

    for (s32 i = 0; i < FRIENDLIST_NUMDATE_SPRITE_COUNT; i++) {
        Sprite_Release(&numDate->sprites[i]);
    }
    return 1;
}

static s32 FriendList_numDate_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = FriendList_numDate_Init,
        .update     = FriendList_numDate_Update,
        .render     = FriendList_numDate_Render,
        .cleanup    = FriendList_numDate_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 FriendList_numDate_CreateTask(TaskPool* pool, s32 dataType, FriendListObject* friendList) {
    FriendList_numDate_Args args;

    args.dataType   = dataType;
    args.friendList = friendList;
    args.timesMet   = friendList->friends[friendList->cursor].timesMet;
    args.year       = friendList->friends[friendList->cursor].lastMet.year;
    args.month      = friendList->friends[friendList->cursor].lastMet.month;
    args.day        = friendList->friends[friendList->cursor].lastMet.day;

    return EasyTask_CreateTask(pool, &Tsk_FriendList_numDate, NULL, 0, NULL, &args);
}
