#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ Sprite         sprite;
    /* 0x40 */ OamAffineParam affine;
    /* 0x50 */ Vec2p          origin;
    /* 0x58 */ Vec2p          pos;
    /* 0x60 */ s32            height;
    /* 0x64 */ Vec2p          dir;
    /* 0x6C */ s32            speed;
    /* 0x70 */ s32            vz;
    /* 0x74 */ s32            pinId;
    /* 0x78 */ s32            active;
    /* 0x7C */ s32            visible;
    /* 0x80 */ s32            lifeMax;
    /* 0x84 */ s32            life;
} MeteoHahen; // Size: 0x88

static s32              OtosuGame_meteohahen_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_meteohahen_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_meteohahen = {"Tsk_OtosuGame_meteohahen", OtosuGame_meteohahen_RunTask,
                                                    sizeof(MeteoHahen)};

static const SpriteAnimation OtosuGame_meteohahen_Anim = {
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
    .frameInfoCallback = OtosuGame_meteohahen_GetFrameInfo,
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

static SpriteFrameInfo* OtosuGame_meteohahen_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    MeteoHahen* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(3, owner->pos.y, owner->height));
}

static void OtosuGame_meteohahen_Load(MeteoHahen* self, Sprite* sprite, OtuPinSpriteArgs* args) {
    SpriteAnimation anim = OtosuGame_meteohahen_Anim;

    anim.owner    = self;
    anim.dataType = (u16)args->dataType;
    anim.posX     = self->pos.x >> 12;
    anim.posY     = self->pos.y >> 12;
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_meteohahen_Init(TaskPool* pool, Task* task, void* args) {
    MeteoHahen*       self = task->data;
    OtuPinSpriteArgs* a    = args;

    self->active          = 0;
    self->visible         = 0;
    self->pinId           = a->pinId;
    self->origin.x        = 0;
    self->origin.y        = 0;
    self->pos.x           = 0;
    self->pos.y           = 0;
    self->height          = 0;
    self->dir.x           = 0;
    self->dir.y           = 0;
    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1800;
    self->affine.scaleY   = 0x1800;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;
    OtosuGame_meteohahen_Load(self, &self->sprite, a);
    return 1;
}

static s32 OtosuGame_meteohahen_Update(TaskPool* pool, Task* task, void* args) {
    MeteoHahen* self = task->data;
    void*       pin  = EasyTask_GetTaskData(pool, self->pinId);

    if (pin != NULL) {
        if (self->active != 0) {
            self->life = self->life - 1;

            if (self->life <= 0) {
                self->active  = 0;
                self->visible = 0;
            } else {
                OtosuGame_badge_GetOrigin(pin, &self->origin);
                self->affine.scaleY = self->life * 0x1800 / self->lifeMax;
                self->affine.scaleX = self->affine.scaleY;

                self->speed = self->speed - OtuDebris_Decel;

                if (self->speed < 0) {
                    self->speed = 0;
                }

                self->vz = self->vz + OtuDebris_Gravity;
                OtuVec_ScaleAdd(self->speed, &self->dir, &self->pos, &self->pos);

                self->height = self->height + self->vz;

                if (self->height > 0) {
                    self->height = 0;

                    if (self->vz > 0) {
                        self->vz = (s32)(((s64)self->vz * -OtuDebris_Bounce + 0x800) >> 0xC);
                    }
                }

                Sprite_Update(&self->sprite);
                self->visible = 1;
            }
        }
    } else {
        self->active  = 0;
        self->visible = 0;
    }

    return 1;
}

static s32 OtosuGame_meteohahen_Render(TaskPool* pool, Task* task, void* args) {
    MeteoHahen* self = task->data;

    if (self->visible != 0) {
        self->sprite.posX = (s16)((self->pos.x - self->origin.x) >> 0xC);
        self->sprite.posY = (s16)((self->pos.y - self->origin.y + self->height) >> 0xC);
        Sprite_RenderFrame(&self->sprite);
    }

    return 1;
}

static s32 OtosuGame_meteohahen_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(&((MeteoHahen*)task->data)->sprite);
    return 1;
}

static s32 OtosuGame_meteohahen_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_meteohahen_Init,
        .update     = OtosuGame_meteohahen_Update,
        .render     = OtosuGame_meteohahen_Render,
        .cleanup    = OtosuGame_meteohahen_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_meteohahen_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtuPinSpriteArgs args;

    args.dataType = dataType;
    args.pinId    = pinId;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_meteohahen, NULL, 0, NULL, &args);
}

void OtosuGame_meteohahen_Start(MeteoHahen* self, Vec2p* at) {
    s32 index;

    self->active        = 1;
    self->affine.scaleX = 0x1800;
    self->affine.scaleY = 0x1800;
    self->pos           = *at;
    self->height        = 0;

    self->speed = OtuDebris_SpeedMin;
    self->speed = self->speed + RNG_Next(OtuDebris_SpeedMax - OtuDebris_SpeedMin);
    self->vz    = 0 - OtuDebris_LaunchMin;
    self->vz    = self->vz - RNG_Next(OtuDebris_LaunchMax - OtuDebris_LaunchMin);

    self->lifeMax = OTU_ABS_AIRTIME(self->vz);
    self->life    = self->lifeMax;

    index       = (RNG_Next(0x10000) >> 4) * 2;
    self->dir.x = FX_SinCosTable_[index + 1];
    self->dir.y = FX_SinCosTable_[index];

    Sprite_ChangeAnimation(&self->sprite, self->sprite.animData, (s16)(RNG_Next(7) + 0xA), self->sprite.cellTable);
}
