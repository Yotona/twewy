#include "Otosu/OtosuGame.h"

static s32              OtosuGame_warp_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_warp_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_warp = {"Tsk_OtosuGame_warp", OtosuGame_warp_RunTask, sizeof(OtosuGame_warp)};

static const SpriteAnimation OtosuGame_warp_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 2,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = OtosuGame_warp_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 5,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_warp_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_warp* owner = sprite->owner;

    Sprite_FrameInfoCallbackSorted(sprite, mode, OtuBoard_SpriteSortKey(3, owner->pos.y, 0));
}

static void OtosuGame_warp_Load(OtosuGame_warp* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_warp_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x);
    anim.posY     = F2I(self->pos.y);
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_warp_Init(TaskPool* pool, Task* task, void* args) {
    OtosuGame_warp*   self = task->data;
    OtuPinSpriteArgs* a    = args;

    self->active   = 0;
    self->visible  = 0;
    self->pinId    = a->pinId;
    self->origin.x = 0;
    self->origin.y = 0;
    self->pos.x    = 0;
    self->pos.y    = 0;
    OtosuGame_warp_Load(self, &self->sprite, a);

    return 1;
}

static s32 OtosuGame_warp_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_warp* self = task->data;
    void*           pin  = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        if (self->active != 0) {
            if (SpriteMgr_IsAnimationFinished(&self->sprite) == 0) {
                OtosuGame_badge_GetOrigin(pin, &self->origin);
                Sprite_Update(&self->sprite);
                self->visible = 1;
            } else {
                self->active  = 0;
                self->visible = 0;
            }
        }
    } else {
        self->active  = 0;
        self->visible = 0;
    }

    return 1;
}

static s32 OtosuGame_warp_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_warp* self = task->data;

    if (self->visible) {
        self->sprite.posX = F2I((self->pos.x - self->origin.x));
        self->sprite.posY = F2I((self->pos.y - self->origin.y));
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_warp_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_warp* self = task->data;

    Sprite_Release(&self->sprite);
    return 1;
}

static s32 OtosuGame_warp_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_warp_Init,
        .update     = OtosuGame_warp_Update,
        .render     = OtosuGame_warp_Render,
        .cleanup    = OtosuGame_warp_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_warp_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_warp, NULL, 0, NULL, &args);
}

void OtosuGame_warp_Start(OtosuGame_warp* self, Vec2p* at, s32 animation) {
    self->active = 1;
    self->pos    = *at;
    Sprite_SetAnimation(&self->sprite, self->sprite.animData, (s16)animation, self->sprite.cellTable);
}

s32 OtosuGame_warp_IsActive(OtosuGame_warp* self) {
    return self->active;
}
