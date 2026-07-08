#include "Interface/Menu/MenuBadge.h"

typedef struct {
    /* 0x00 */ Sprite           sprite;
    /* 0x40 */ s32              visible;
    /* 0x44 */ MenuBadgeObject* menuBadge;
    /* 0x48 */ s32              posY;
    /* 0x4C */ s32              velocity;
    /* 0x50 */ s32              accel;
    /* 0x54 */ s32              targetY;
    /* 0x58 */ s16              timer;
    /* 0x5A */ s16              phase;
} MenuBadge_stkmstIn; // Size: 0x5C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuBadgeObject* menuBadge;
    /* 0x8 */ s16              x;
    /* 0xA */ s16              y;
    /* 0xC */ u16              frame;
} MenuBadge_stkmstIn_Args;

typedef struct {
    u16 x;
    u16 y;
} MenuBadgeUPoint;

static SpriteFrameInfo* MenuBadge_stkmstIn_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuBadge_stkmstIn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuBadge_stkmstIn = {"Tsk_MenuBadge_stkmstIn", MenuBadge_stkmstIn_RunTask,
                                                  sizeof(MenuBadge_stkmstIn)};

static const SpriteAnimation MenuBadge_stkmstIn_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x800,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuBadge_stkmstIn_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c82e4,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xD,
    .unk_22     = 2,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static SpriteFrameInfo* MenuBadge_stkmstIn_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;
    }

    return info;
}

// Nonmatching: incorrect instruction arguments
static void MenuBadge_stkmstIn_Load(MenuBadge_stkmstIn* stkmstIn, Sprite* sprite, MenuBadge_stkmstIn_Args* args) {
    SpriteAnimation anim = MenuBadge_stkmstIn_Anim;

    anim.dataType = args->dataType;
    anim.unk_04   = args->x;
    anim.unk_06   = args->y;

    if (anim.unk_04 < 30) {
        anim.unk_04 = 30;
    }
    if (anim.unk_04 > 226) {
        anim.unk_04 = 226;
    }
    if (anim.unk_06 < 48) {
        anim.unk_06 = 48;
    }

    anim.unk_06        = args->x;
    anim.unk_2A        = (args->frame) + 0x5B;
    stkmstIn->visible  = 1;
    stkmstIn->posY     = I2F(anim.unk_06);
    stkmstIn->velocity = 0x6000;
    stkmstIn->accel    = -0x666;
    stkmstIn->targetY  = I2F(anim.unk_06 - 30);
    _Sprite_Load(sprite, &anim);
}

static s32 MenuBadge_stkmstIn_Init(TaskPool* pool, Task* task, void* args) {
    MenuBadge_stkmstIn*      stkmstIn     = task->data;
    MenuBadge_stkmstIn_Args* stkmstInArgs = args;

    stkmstIn->menuBadge = stkmstInArgs->menuBadge;
    stkmstIn->timer     = 0;
    stkmstIn->phase     = 0;
    MenuBadge_stkmstIn_Load(stkmstIn, &stkmstIn->sprite, stkmstInArgs);
    return 1;
}

static s32 MenuBadge_stkmstIn_Update(TaskPool* pool, Task* task, void* args) {
    MenuBadge_stkmstIn* stkmstIn = task->data;

    switch (stkmstIn->phase) {
        case 0: {
            s32 target = stkmstIn->targetY;
            s32 pos    = stkmstIn->posY;

            if (pos < target) {
                stkmstIn->posY = pos + stkmstIn->velocity;
            } else if (pos > target) {
                stkmstIn->posY = pos - stkmstIn->velocity;
            }

            s32 velocity = stkmstIn->velocity;
            if (velocity > 0) {
                s32 newVelocity = velocity + stkmstIn->accel;

                stkmstIn->velocity = newVelocity;
                if (newVelocity <= 0) {
                    stkmstIn->velocity = 0;
                    stkmstIn->timer    = 60;
                    stkmstIn->phase    = 1;
                }
            }
        } break;

        case 1: {
            s16 timer = stkmstIn->timer;

            if (timer > 0) {
                stkmstIn->timer = timer - 1;
            } else {
                return 0;
            }
        } break;
    }

    stkmstIn->sprite.posY = stkmstIn->posY >> 0xC;
    Sprite_Update(&stkmstIn->sprite);
    return 1;
}

static s32 MenuBadge_stkmstIn_Render(TaskPool* pool, Task* task, void* args) {
    MenuBadge_stkmstIn* stkmstIn = task->data;

    if (stkmstIn->visible) {
        Sprite_RenderFrame(&stkmstIn->sprite);
    }
    return 1;
}

static s32 MenuBadge_stkmstIn_Release(TaskPool* pool, Task* task, void* args) {
    MenuBadge_stkmstIn* stkmstIn = task->data;

    Sprite_Release(&stkmstIn->sprite);
    return 1;
}

static s32 MenuBadge_stkmstIn_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuBadge_stkmstIn_Init,
        .update     = MenuBadge_stkmstIn_Update,
        .render     = MenuBadge_stkmstIn_Render,
        .cleanup    = MenuBadge_stkmstIn_Release,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuBadge_stkmstIn_CreateTask(TaskPool* pool, s32 dataType, MenuBadgeUPoint pos, s16 frame, MenuBadgeObject* badge) {
    MenuBadge_stkmstIn_Args args;

    args.dataType  = dataType;
    args.menuBadge = badge;
    args.x         = pos.x;
    args.y         = pos.y;
    args.frame     = frame;

    return EasyTask_CreateTask(pool, &Tsk_MenuBadge_stkmstIn, NULL, 0, NULL, &args);
}
