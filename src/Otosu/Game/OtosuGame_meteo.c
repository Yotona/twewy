#include "Otosu/OtosuGame.h"

static s32              OtosuGame_meteo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_meteo_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_meteo = {"Tsk_OtosuGame_meteo", OtosuGame_meteo_RunTask, sizeof(OtosuGame_meteo)};

static const SpriteAnimation OtosuGame_meteo_Anim = {
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
    .frameInfoCallback = OtosuGame_meteo_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_CommonObj,
    .unk_18            = 2,
    .packIndex         = 8,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 2,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 4,
};

static OtuScaleKey OtosuGame_meteo_RecoverScale[6] = {
    { 2,  0xCCD,  0x99A},
    {11, 0x1333, 0x1666},
    { 4, 0x119A, 0x119A},
    { 4, 0x1000,  0xE66},
    { 3,  0xCCD,  0x800},
    { 2,  0x99A,  0x333},
};

static SpriteFrameInfo* OtosuGame_meteo_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_meteo* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(4, owner->pos.y, owner->height));
}

static void OtosuGame_meteo_Load(OtosuGame_meteo* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_meteo_Anim;

    anim.owner    = self;
    anim.dataType = args->dataType;
    anim.posX     = F2I(self->pos.x);
    anim.posY     = F2I(self->pos.y);

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_meteo_Init(TaskPool* pool, Task* task, void* args) {
    OtuPinSpriteArgs* taskArgs = args;
    OtosuGame_meteo*  self     = task->data;

    self->pinId           = taskArgs->pinId;
    self->origin.x        = 0;
    self->origin.y        = 0;
    self->pos.x           = 0;
    self->pos.y           = 0;
    self->height          = 0;
    self->alive           = 0;
    self->state           = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;

    OtosuGame_meteo_Load(self, &self->sprite, taskArgs);
    return 1;
}

static s32 OtosuGame_meteo_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_meteo* self = task->data;
    void*            pin  = EasyTask_GetTaskData(pool, self->pinId);
    s32              i;

    if (pin != NULL) {
        self->alive = OtosuGame_badge_IsUsingMeteo(pin);

        if (self->alive != 0) {
            OtosuGame_badge_GetOrigin(pin, &self->origin);
            OtosuGame_badge_GetPos(pin, &self->pos);
            self->height = OtosuGame_badge_GetHeight(pin);
        } else {
            self->state = 0;
        }

        switch (self->state) {
            case 0:
                self->alive = 0;
                break;

            case 1:
                self->timer = self->timer - 1;
                if (self->timer <= 0) {
                    self->state = 0;
                }
                break;

            case 2:
                if (self->timer > 0) {
                    self->affine.scaleY = self->affine.scaleY + (0x1000 - self->affine.scaleY) / self->timer;
                    self->timer         = self->timer - 1;
                }
                break;

            case 3:
                OtuScaleAnim_Step(&self->scaleAnim, &self->affine);

                if (self->timer == 0x1E) {
                    for (i = 0; i < 8; i++) {
                        OtosuGame_meteohahen_Start(EasyTask_GetTaskData(pool, self->hahenIds[i]), &self->pos);
                    }
                }

                self->timer = self->timer - 1;
                if (self->timer <= 0) {
                    self->state = 0;
                }
                break;
        }
    } else {
        self->alive = 0;
    }

    if (self->alive != 0) {
        Sprite_Update(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_meteo_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_meteo* self = task->data;

    if (self->alive != 0) {
        self->sprite.posX = F2I(self->pos.x - self->origin.x);
        self->sprite.posY = F2I(self->pos.y - self->origin.y + self->height);
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_meteo_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_meteo* self = task->data;
    s32              i;

    Sprite_Release(&self->sprite);

    for (i = 0; i < 8; i++) {
        EasyTask_DeleteTask(pool, self->hahenIds[i]);
    }
    return 1;
}

static s32 OtosuGame_meteo_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_meteo_Init,
        .update     = OtosuGame_meteo_Update,
        .render     = OtosuGame_meteo_Render,
        .cleanup    = OtosuGame_meteo_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_meteo_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    s32              id;
    OtosuGame_meteo* self;
    s32              i;

    args.dataType = dataType;
    args.pinId    = pinId;

    id   = EasyTask_CreateTask(pool, &Tsk_OtosuGame_meteo, NULL, 0, NULL, &args);
    self = EasyTask_GetTaskData(pool, id);

    for (i = 0; i < 8; i++) {
        self->hahenIds[i] = OtosuGame_meteohahen_CreateTask(pool, dataType, pinId);
    }
    return id;
}

void OtosuGame_meteo_GetHitRecord(OtosuGame_meteo* self, OtuPinRecord* out) {
    out->pos   = self->pos;
    out->scale = 0x1D000;
}

void OtosuGame_meteo_Start(OtosuGame_meteo* self, s32 frames) {
    self->timer         = frames;
    self->affine.scaleX = 0x1000;
    self->affine.scaleY = 0x1000;

    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 1, self->sprite.cellTable);
    self->state = 1;
}

void OtosuGame_meteo_Impact(OtosuGame_meteo* self) {
    self->timer         = 4;
    self->affine.scaleX = 0x1000;
    self->affine.scaleY = 0x29;

    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 4, self->sprite.cellTable);
    self->state = 2;
}

void OtosuGame_meteo_Recover(OtosuGame_meteo* self) {
    self->timer = 0x1E;

    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, 5, self->sprite.cellTable);
    OtuScaleAnim_Init(&self->scaleAnim, OtosuGame_meteo_RecoverScale, 6, &self->affine);
    self->state = 3;
}
