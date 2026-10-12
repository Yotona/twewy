#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ OamAffineParam affine;
    /* 0x50 */ Vec2p          origin;
    /* 0x58 */ Vec2p          pos;
    /* 0x60 */ Vec2p          dir;
    /* 0x68 */ s32            pinId;
    /* 0x6C */ s32            visible;
    /* 0x70 */ s32            state; // 1 aim, 2 thrust, 3 hold, 4 retract
    /* 0x74 */ s32            timer;
} OtosuGame_hand;                    // Size: 0x78

static s32              OtosuGame_hand_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_hand_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_hand = {"Tsk_OtosuGame_hand", OtosuGame_hand_RunTask, sizeof(OtosuGame_hand)};

static const SpriteAnimation OtosuGame_hand_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_hand_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 0xA,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_hand_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_hand* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(4, owner->pos.y, 0));
}

static void OtosuGame_hand_Load(OtosuGame_hand* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_hand_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x);
    anim.posY     = F2I(self->pos.y);
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_hand_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_hand*   self     = task->data;

    self->pinId           = taskArgs->pinId;
    self->origin.x        = 0;
    self->origin.y        = 0;
    self->pos.x           = 0;
    self->pos.y           = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x2000;
    self->affine.scaleY   = 0x2000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;
    self->visible         = 0;
    self->state           = 0;

    OtosuGame_hand_Load(self, &self->sprite, taskArgs);
    return 1;
}

/**
 * The hand's states: 1 aims backwards along the pin's facing, 2 thrusts out
 * along it while stretching to full length over four frames, 3 waits out the
 * grab animation, 4 shrinks back. `== 1` keeps the target's compare on the
 * isPlaying bit.
 */
static s32 OtosuGame_hand_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_hand* self = task->data;
    void*           pin  = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        switch (self->state) {
            case 0:
                self->visible = 0;
                break;

            case 1:
                self->visible = 1;
                OtosuGame_badge_GetDir(pin, &self->dir);
                OtuVec_Scale(-0x1000, &self->dir, &self->dir);
                self->affine.rotation = (u16)(FX_Atan2Idx(self->dir.y, self->dir.x) + 0xC000);
                self->affine.scaleY   = 0;
                self->state           = 2;
                self->timer           = 4;
                Sprite_SetAnimation(&self->sprite, self->sprite.animData, 1, self->sprite.cellTable);
                /* fall through */
            case 2:
                OtosuGame_badge_GetOrigin(pin, &self->origin);
                OtosuGame_badge_GetPos(pin, &self->pos);
                OtuVec_ScaleAdd(0x38000, &self->dir, &self->pos, &self->pos);
                if (self->timer > 0) {
                    self->affine.scaleY = self->affine.scaleY + (0x2000 - self->affine.scaleY) / self->timer;
                    self->timer         = self->timer - 1;
                }

                if (self->sprite.isPlaying == 1) {
                    self->state = 3;
                    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 2, self->sprite.cellTable);
                }
                break;

            case 3:
                OtosuGame_badge_GetOrigin(pin, &self->origin);

                if (self->sprite.isPlaying == 1) {
                    self->state = 4;
                    self->timer = 4;
                    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 3, self->sprite.cellTable);
                }
                break;

            case 4:
                OtosuGame_badge_GetOrigin(pin, &self->origin);

                if (self->timer > 0) {
                    self->affine.scaleY = self->affine.scaleY + -self->affine.scaleY / self->timer;
                    self->timer         = self->timer - 1;
                } else {
                    self->state   = 0;
                    self->visible = 0;
                }
                break;
        }
    } else {
        self->visible = 0;
    }

    if (self->visible != 0) {
        Sprite_Update(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_hand_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_hand* self = task->data;

    if (self->visible != 0) {
        self->sprite.posX = (s16)((self->pos.x - self->origin.x) >> 12);
        self->sprite.posY = (s16)((self->pos.y - self->origin.y) >> 12);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_hand_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((OtosuGame_hand*)task->data)->sprite);
    return 1;
}

static s32 OtosuGame_hand_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_hand_Init,
        .update     = OtosuGame_hand_Update,
        .render     = OtosuGame_hand_Render,
        .cleanup    = OtosuGame_hand_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_hand_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_hand, NULL, 0, NULL, &args);
}

/** Starts the grab. */
void OtosuGame_hand_Start(OtosuGame_hand* self) {
    self->state = 1;
}
