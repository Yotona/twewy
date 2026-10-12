#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ s32                        dataType; // the sprite template's dataType
    /* 0x04 */ s16                        boardKind;
    /* 0x06 */ u16                        slot;     // Which of the three palette slots to use.
    /* 0x08 */ OtosuGame_obstacle_Params* params;
} OtosuGame_obstacle_Args;                          // Size: 0xC

static s32              OtosuGame_obstacle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_obstacle_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const s16 OtosuGame_obstacle_AnimField26[3] = {
    0x2,
    0x2,
    0x2,
};

static const s16 OtosuGame_obstacle_AnimField28[3] = {
    0x3,
    0x3,
    0x3,
};

static const s16 OtosuGame_obstacle_AnimField1C[3] = {
    0x1,
    0x1,
    0x1,
};

static const s16 OtosuGame_obstacle_PackIndices[3] = {
    0x1,
    0x2,
    0x3,
};

static const TaskHandle Tsk_OtosuGame_obstacle = {"Tsk_OtosuGame_obstacle", OtosuGame_obstacle_RunTask,
                                                  sizeof(OtosuGame_obstacle)};

static const s16 OtosuGame_obstacle_AnimField20[3][3] = {
    0x4, 0x5, 0x6, 0x4, 0x5, 0x6, 0x4, 0x5, 0x6,
};

static const SpriteAnimation OtosuGame_obstacle_Anim = {
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
    .frameInfoCallback = OtosuGame_obstacle_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_Stage2Obj,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_obstacle_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_obstacle* owner = sprite->owner;

    Sprite_FrameInfoCallbackSorted(sprite, mode, OtuBoard_SpriteSortKey(3, owner->pos.y, 0));
}

static void OtosuGame_obstacle_Load(OtosuGame_obstacle* self, Sprite* sprite, OtosuGame_obstacle_Args* args) {
    SpriteAnimation params = OtosuGame_obstacle_Anim;
    u16             kind   = args->params->kind;

    params.owner     = self;
    params.dataType  = (u16)args->dataType;
    params.posX      = (s16)((self->pos.x - self->origin.x) >> 12);
    params.posY      = (s16)((self->pos.y - self->origin.y) >> 12);
    params.packIndex = OtosuGame_obstacle_PackIndices[kind];
    params.unk_1C    = OtosuGame_obstacle_AnimField1C[kind];
    params.unk_20    = OtosuGame_obstacle_AnimField20[kind][args->slot];
    params.unk_26    = OtosuGame_obstacle_AnimField26[kind];
    params.unk_28    = OtosuGame_obstacle_AnimField28[kind];
    _Sprite_Load(sprite, &params);
}

static s32 OtosuGame_obstacle_Init(TaskPool* pool, Task* task, void* args) {
    OtosuGame_obstacle*      self      = task->data;
    OtosuGame_obstacle_Args* a         = args;
    s32                      scales[3] = {0x20000, 0x18000, 0x10000};
    s32                      x;
    s32                      y;

    self->origin.x    = 0;
    self->origin.y    = 0;
    x                 = a->params->targetX;
    y                 = a->params->targetY;
    self->pos.x       = x << 12;
    self->pos.y       = y << 12;
    self->scale       = scales[a->params->kind];
    self->animPending = 0;

    OtosuGame_obstacle_Load(self, &self->sprite, a);
    return 1;
}

static s32 OtosuGame_obstacle_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_obstacle* self = task->data;

    if (self->animPending != 0) {
        self->sprite.animationMode = ANIM_MODE_ONCE;
        Sprite_SetAnimation(&self->sprite, self->sprite.animData, 2, self->sprite.cellTable);
        self->animPending = 0;
    }

    Sprite_Update(&self->sprite);
    return 1;
}

static s32 OtosuGame_obstacle_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_obstacle* self = task->data;

    self->sprite.posX = (s16)((self->pos.x - self->origin.x) >> 12);
    self->sprite.posY = (s16)((self->pos.y - self->origin.y) >> 12);

    Sprite_RenderFrame(&self->sprite);
    return 1;
}

static s32 OtosuGame_obstacle_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_obstacle* self = task->data;

    Sprite_Release(&self->sprite);
    return 1;
}

static s32 OtosuGame_obstacle_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_obstacle_Init,
        .update     = OtosuGame_obstacle_Update,
        .render     = OtosuGame_obstacle_Render,
        .cleanup    = OtosuGame_obstacle_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_obstacle_CreateTask(TaskPool* pool, s32 dataType, s16 boardKind, s16 slot, OtosuGame_obstacle_Params* params) {
    OtosuGame_obstacle_Args args;

    args.dataType  = dataType;
    args.boardKind = boardKind;
    args.slot      = slot;
    args.params    = params;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_obstacle, NULL, 0, NULL, &args);
}

void OtosuGame_obstacle_SetOrigin(OtosuGame_obstacle* self, Vec2p* origin) {
    self->origin = *origin;
}

void OtosuGame_obstacle_GetPos(OtosuGame_obstacle* self, Vec2p* out) {
    *out = self->pos;
}

s32 OtosuGame_obstacle_GetScale(OtosuGame_obstacle* self) {
    return self->scale;
}

void OtosuGame_obstacle_PlayHit(OtosuGame_obstacle* self) {
    self->animPending = 1;
}
