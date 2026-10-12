#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ s32 live;
    /* 0x04 */ s32 delay;
    /* 0x08 */ s32 offsetX;
    /* 0x0C */ s32 accum;
    /* 0x10 */ s32 phase;
} OtuPointSlot; // Size: 0x14

typedef struct {
    /* 0x000 */ Sprite       sprite[4];
    /* 0x100 */ Vec2p        origin;
    /* 0x108 */ Vec2p        pos;
    /* 0x110 */ s32          pinId;
    /* 0x114 */ s32          active;
    /* 0x118 */ s32          visible;
    /* 0x11C */ s32          count; // the value shown
    /* 0x120 */ OtuPointSlot slot[4];
    /* 0x170 */ s32          timer;
} OtosuGame_point; // Size: 0x174

static s32              OtosuGame_point_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_point_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const s16 OtosuGame_point_AnimIndices[4] = {
    0xC,
    0xB,
    0x1,
    0x1,
};

static const TaskHandle Tsk_OtosuGame_point = {"Tsk_OtosuGame_point", OtosuGame_point_RunTask, sizeof(OtosuGame_point)};

static const SpriteAnimation OtosuGame_point_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_point_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_UnderObj,
    .unk_18            = 2,
    .packIndex         = 3,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_point_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_point* owner = sprite->owner;

    Sprite_FrameInfoCallbackSorted(sprite, mode, OtuBoard_SpriteSortKey(5, owner->pos.y, 0));
}

static void OtosuGame_point_Load(OtosuGame_point* self, Sprite* sprite, OtuPinSpriteArgs* args, s32 index) {
    SpriteAnimation anim = OtosuGame_point_Anim;

    anim.owner     = self;
    anim.dataType  = args->dataType;
    anim.posX      = (self->pos.x - self->origin.x) >> 12;
    anim.posY      = (self->pos.y - self->origin.y) >> 12;
    anim.animIndex = OtosuGame_point_AnimIndices[index];

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_point_Init(TaskPool* pool, Task* task, void* args) {
    OtosuGame_point*  self = task->data;
    OtuPinSpriteArgs* a    = args;
    s32               i;

    self->pinId    = a->pinId;
    self->origin.x = 0;
    self->origin.y = 0;
    self->pos.x    = 0;
    self->pos.y    = 0;
    self->active   = 0;
    self->visible  = 0;

    for (i = 0; i < 4; i++) {
        OtosuGame_point_Load(self, &self->sprite[i], a, i);
    }
    return 1;
}

/**
 * Bounces each live digit: once its delay is spent it falls under gravity
 * (0x800 per frame) and, on landing, rebounds at a quarter of its speed. The
 * whole popup hides once `timer` runs out.
 */
static s32 OtosuGame_point_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_point* self = task->data;
    s32              i;

    if (self->active != 0) {
        for (i = 0; i < 4; i++) {
            if (self->slot[i].live != 0) {
                if (self->slot[i].delay > 0) {
                    self->slot[i].delay = self->slot[i].delay - 1;
                } else {
                    self->slot[i].accum = self->slot[i].accum + self->slot[i].phase;
                    self->slot[i].phase = self->slot[i].phase + 0x800;

                    if (self->slot[i].accum > 0) {
                        self->slot[i].accum = 0;

                        if (self->slot[i].phase > 0) {
                            // A Q12 multiply by -0.25
                            self->slot[i].phase = (s32)(((s64)self->slot[i].phase * -0x400 + 0x800) >> 12);
                        }
                    }
                    Sprite_Update(&self->sprite[i]);
                }
            }
        }

        self->visible = 1;
        self->timer   = self->timer - 1;

        if (self->timer <= 0) {
            self->active  = 0;
            self->visible = 0;
        }
    }
    return 1;
}

static s32 OtosuGame_point_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_point* self = task->data;
    OtosuGame_badge* pin;
    s32              i;

    if (self->visible != 0) {
        pin = EasyTask_GetTaskData(pool, self->pinId);

        if (pin != NULL) {
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pos);

            for (i = 0; i < 4; i++) {
                if (self->slot[i].live != 0 && self->slot[i].delay <= 0) {
                    // The target computes y before x.
                    s32 py = self->pos.y - self->origin.y + self->slot[i].accum - 0x10000;
                    s32 px = self->pos.x - self->origin.x + self->slot[i].offsetX;

                    self->sprite[i].posX = px >> 12;
                    self->sprite[i].posY = py >> 12;
                    Sprite_RenderFrame(&self->sprite[i]);
                }
            }
        }
    }
    return 1;
}

static s32 OtosuGame_point_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_point* self = task->data;
    s32              i;

    for (i = 0; i < 4; i++) {
        Sprite_Release(&self->sprite[i]);
    }
    return 1;
}

static s32 OtosuGame_point_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_point_Init,
        .update     = OtosuGame_point_Update,
        .render     = OtosuGame_point_Render,
        .cleanup    = OtosuGame_point_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_point_CreateTask(TaskPool* pool, s32 dataType, s32 pin) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pin;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_point, NULL, 0, NULL, &args);
}

/**
 * Pops up `count` for 60 frames: the digits drop in staggered by four frames,
 * sprite 2 shows the ones digit and sprite 3 the tens (hidden when zero).
 */
void OtosuGame_point_Show(OtosuGame_point* self, s32 count) {
    s32 i;
    s16 first;
    s16 last;

    self->active = 1;
    self->timer  = 0x3C;

    for (i = 0; i < 4; i++) {
        self->slot[i].live    = 1;
        self->slot[i].delay   = (3 - i) * 4;
        self->slot[i].offsetX = (0xC - i * 8) << 12;
        self->slot[i].accum   = 0;
        self->slot[i].phase   = -0x3000;
    }

    self->count = count;

    // Both digits are narrowed before use; routing the first through `last`
    // is what the register allocation needs.
    last  = (count % 10) + 1;
    first = (s16)last;
    Sprite_ChangeAnimation(&self->sprite[2], self->sprite[2].animData, first, self->sprite[2].cellTable);

    last = (s16)((self->count / 10) % 10 + 1);

    if ((s16)last <= 1) {
        self->slot[3].live = 0;
        return;
    }

    Sprite_ChangeAnimation(&self->sprite[3], self->sprite[3].animData, last, self->sprite[3].cellTable);
}

/** Hides the popup. */
void OtosuGame_point_Hide(OtosuGame_point* self) {
    self->active  = 0;
    self->visible = 0;
}
