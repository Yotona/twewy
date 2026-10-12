#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Field/FieldTasks.h"
#include "SpriteMgr.h"

typedef struct LocaTitle {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ s32    leftX;  // 20.12 fixed point
    /* 0x84 */ s32    rightX; // 20.12 fixed point
    /* 0x88 */ s32    posY;
    /* 0x8C */ void (*update)(struct LocaTitle*);
    /* 0x90 */ s16 timer;
    /* 0x92 */ u16 pad_92;
    /* 0x94 */ s32 keepRunning;
} LocaTitle; // Size: 0x98

typedef struct {
    /* 0x0 */ s32 packIndex;
    /* 0x4 */ s32 display;
    /* 0x8 */ s32 dataType;
} LocaTitle_Args;

extern void MI_CpuSet(void* dest, u8 value, u32 control);
extern void func_020265d4(s32* value, s32 target, u16 duration);
extern void func_02026590(s32* value, s32 target, u16 duration);
extern void func_02026608(s32* value, s32 target, u16 duration, s32 mode);

void LocaTitle_SetUpdate(LocaTitle* locaTitle, void (*update)(LocaTitle*));
void func_ov030_020d2dbc(LocaTitle* locaTitle);
void func_ov030_020d2e4c(LocaTitle* locaTitle);
void func_ov030_020d2ec0(LocaTitle* locaTitle);

void LocaTitle_Load(LocaTitle* locaTitle, LocaTitle_Args* args);
s32  LocaTitle_Init(TaskPool* pool, Task* task, void* args);
s32  LocaTitle_Update(TaskPool* pool, Task* task, void* args);
s32  LocaTitle_Render(TaskPool* pool, Task* task, void* args);
s32  LocaTitle_Destroy(TaskPool* pool, Task* task, void* args);
s32  LocaTitle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
s32  LocaTitle_CreateTask(TaskPool* pool, s32 dataType, s32 packIndex);

char                data_ov030_021090f4[28] = "Apl_Kit/Grp_LocaTitle.bin";
const BinIdentifier data_ov030_020ee948     = {30, data_ov030_021090f4};

static const SpriteAnimation LocaTitle_Anim = {
    .bits_0_1          = 0,
    .dataType          = 1,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0xB0,
    .frameInfoCallback = NULL,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov030_020ee948,
    .unk_18            = 2,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const TaskHandle Tsk_LocaTitle = {"Tsk_LocaTitle", LocaTitle_RunTask, sizeof(LocaTitle)};
void                    LocaTitle_SetUpdate(LocaTitle* locaTitle, void (*update)(LocaTitle*)) {
    locaTitle->update = update;
    locaTitle->timer  = 0;
}

void func_ov030_020d2dbc(LocaTitle* locaTitle) {
    if (locaTitle->timer == 0) {
        locaTitle->leftX  = 0x180000;
        locaTitle->rightX = locaTitle->leftX - 0x200000;
        locaTitle->timer  = 30;
    }
    if (locaTitle->timer > 0) {
        func_02026608(&locaTitle->leftX, 0x8C000, locaTitle->timer, 0);
        func_02026608(&locaTitle->rightX, 0x74000, locaTitle->timer, 0);
        locaTitle->timer--;
    }
    if (locaTitle->timer > 0) {
        return;
    }
    LocaTitle_SetUpdate(locaTitle, func_ov030_020d2e4c);
}

void func_ov030_020d2e4c(LocaTitle* locaTitle) {
    if (locaTitle->timer == 0) {
        locaTitle->timer = 40;
    }
    if (locaTitle->timer > 0) {
        func_02026590(&locaTitle->leftX, 0x74000, locaTitle->timer);
        func_02026590(&locaTitle->rightX, 0x8C000, locaTitle->timer);
        locaTitle->timer--;
    }
    if (locaTitle->timer > 0) {
        return;
    }
    LocaTitle_SetUpdate(locaTitle, func_ov030_020d2ec0);
}

void func_ov030_020d2ec0(LocaTitle* locaTitle) {
    if (locaTitle->timer == 0) {
        locaTitle->timer = 20;
    }
    if (locaTitle->timer > 0) {
        func_020265d4(&locaTitle->leftX, -0x80000, locaTitle->timer);
        func_020265d4(&locaTitle->rightX, 0x180000, locaTitle->timer);
        locaTitle->timer--;
    }
    if (locaTitle->timer <= 0) {
        locaTitle->keepRunning = 0;
    }
}

void LocaTitle_Load(LocaTitle* locaTitle, LocaTitle_Args* args) {
    SpriteAnimation anim = LocaTitle_Anim;
    anim.bits_0_1        = args->display;
    anim.dataType        = args->dataType;
    anim.packIndex       = args->packIndex + 1;
    anim.animIndex       = 1;
    anim.owner           = &locaTitle->sprites[0];
    Sprite_Load(&locaTitle->sprites[0], &anim);
    anim.animIndex = 2;
    anim.owner     = &locaTitle->sprites[1];
    Sprite_Load(&locaTitle->sprites[1], &anim);
}

s32 LocaTitle_Init(TaskPool* pool, Task* task, void* args) {
    LocaTitle*      locaTitle     = task->data;
    LocaTitle_Args* locaTitleArgs = args;
    MI_CpuSet(locaTitle, 0, 0x98);
    locaTitle->posY        = 0x60;
    locaTitle->keepRunning = 1;
    LocaTitle_Load(locaTitle, locaTitleArgs);
    LocaTitle_SetUpdate(locaTitle, func_ov030_020d2dbc);
    return 1;
}

s32 LocaTitle_Update(TaskPool* pool, Task* task, void* args) {
    LocaTitle* locaTitle = task->data;
    if (locaTitle->update != NULL) {
        locaTitle->update(locaTitle);
    }
    Sprite_Update(&locaTitle->sprites[0]);
    Sprite_Update(&locaTitle->sprites[1]);
    return locaTitle->keepRunning;
}

s32 LocaTitle_Render(TaskPool* pool, Task* task, void* args) {
    LocaTitle* locaTitle       = task->data;
    s32        x               = locaTitle->rightX >> 12;
    s32        y               = locaTitle->posY;
    locaTitle->sprites[1].posX = x;
    locaTitle->sprites[1].posY = y;
    Sprite_RenderFrame(&locaTitle->sprites[1]);
    x                          = locaTitle->leftX >> 12;
    y                          = locaTitle->posY;
    locaTitle->sprites[0].posX = x;
    locaTitle->sprites[0].posY = y;
    Sprite_RenderFrame(&locaTitle->sprites[0]);
    return 1;
}

s32 LocaTitle_Destroy(TaskPool* pool, Task* task, void* args) {
    LocaTitle* locaTitle = task->data;
    Sprite_Release(&locaTitle->sprites[0]);
    Sprite_Release(&locaTitle->sprites[1]);
    return 1;
}

s32 LocaTitle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    typedef s32 (*TaskStage)(TaskPool*, Task*, void*);
    TaskStage stages[4] = {
        LocaTitle_Init,
        LocaTitle_Update,
        LocaTitle_Render,
        LocaTitle_Destroy,
    };
    return stages[stage](pool, task, args);
}

s32 LocaTitle_CreateTask(TaskPool* pool, s32 dataType, s32 packIndex) {
    LocaTitle_Args args;
    args.packIndex = packIndex;
    args.dataType  = dataType;
    args.display   = 0;
    EasyTask_CreateTask(pool, &Tsk_LocaTitle, NULL, 0, NULL, &args);
    args.display = 1;
    return EasyTask_CreateTask(pool, &Tsk_LocaTitle, NULL, 0, NULL, &args);
}
