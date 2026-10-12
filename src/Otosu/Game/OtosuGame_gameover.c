#include "Otosu/OtosuGame.h"

static s32              OtosuGame_gameover_RunTask(TaskPool* pool, Task* task, void* data, s32 stage);
static SpriteFrameInfo* OtosuGame_gameover_GetFrameInfoSprite0(Sprite* sprite, s32 arg, s32 mode);
static SpriteFrameInfo* OtosuGame_gameover_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_gameover = {"Tsk_OtosuGame_gameover", OtosuGame_gameover_RunTask,
                                                  sizeof(OtosuGame_gameover)};

static const SpriteAnimation OtosuGame_gameover_AnimSprite1 = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x57,
    .frameInfoCallback = OtosuGame_gameover_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_UnderObj,
    .unk_18            = 2,
    .packIndex         = 2,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const SpriteAnimation OtosuGame_gameover_AnimSprite2 = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x57,
    .frameInfoCallback = OtosuGame_gameover_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_UnderObj,
    .unk_18            = 2,
    .packIndex         = 7,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const SpriteAnimation OtosuGame_gameover_AnimSprite0 = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 1,
    .posX              = 0x80,
    .posY              = 0x57,
    .frameInfoCallback = OtosuGame_gameover_GetFrameInfoSprite0,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_UnderObj,
    .unk_18            = 2,
    .packIndex         = 6,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static OtuScaleKey OtosuGame_gameover_ScaleKeys[12] = {
    { 1,  0x666,  0x666},
    { 1,  0x99A,  0x99A},
    { 2,  0xCCD,  0xCCD},
    { 8, 0x1000, 0x14CD},
    { 3,  0xCCD,  0xE66},
    { 4,  0xF33, 0x1333},
    { 3,  0xCCD,  0xE66},
    { 4,  0xFAE, 0x1266},
    { 3,  0xE66,  0xE66},
    { 4, 0x1000, 0x119A},
    { 3,  0xF33,  0xF33},
    {50, 0x1000, 0x1000},
};

static SpriteFrameInfo* OtosuGame_gameover_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    OtosuGame_gameover* owner = sprite->owner;

    Sprite_FrameInfoCallbackAffineSorted(sprite, mode, &owner->affine, OtuBoard_SpriteSortKey(7, 0, 0));
}

static SpriteFrameInfo* OtosuGame_gameover_GetFrameInfoSprite0(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, OtuBoard_SpriteSortKey(7, 0, 0));
}

static void OtosuGame_gameover_LoadSprite0(OtosuGame_gameover* self, Sprite* sprite, OtuTaskArgs1* args) {
    SpriteAnimation anim = OtosuGame_gameover_AnimSprite0;

    anim.owner    = self;
    anim.dataType = (u16)args->dataType;
    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_gameover_LoadSprite1(OtosuGame_gameover* self, Sprite* sprite, OtuTaskArgs1* args) {
    SpriteAnimation anim = OtosuGame_gameover_AnimSprite1;

    anim.owner    = self;
    anim.dataType = (u16)args->dataType;
    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_gameover_LoadSprite2(OtosuGame_gameover* self, Sprite* sprite, OtuTaskArgs1* args) {
    SpriteAnimation anim = OtosuGame_gameover_AnimSprite2;

    anim.owner    = self;
    anim.dataType = (u16)args->dataType;
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_gameover_Init(TaskPool* pool, Task* task, void* args) {
    OtuTaskArgs1*       taskArgs = args;
    OtosuGame_gameover* self     = task->data;

    self->state   = 0;
    self->visible = 0;
    OtosuGame_gameover_LoadSprite0(self, &self->sprite0, taskArgs);
    OtosuGame_gameover_LoadSprite1(self, &self->sprite1, taskArgs);
    OtosuGame_gameover_LoadSprite2(self, &self->sprite2, taskArgs);
    return 1;
}

static s32 OtosuGame_gameover_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_gameover* self = task->data;

    switch (self->state) {
        case 0:
            self->visible = 0;
            break;

        case 1:
            self->visible = 1;
            EasyFade_FadeMainDisplay(FADER_INSTANT, 0, 0x3C);
            self->counter = self->counter - 1;
            if (self->counter <= 0) {
                self->counter = 0x3C;
                self->state   = 2;
            }
            break;

        case 2:
            self->visible = 1;
            self->counter = self->counter - 1;
            if (self->counter <= 0) {
                SndMgr_StartPlayingSE(0x34C);
                self->counter = 0x3C;
                self->state   = 3;
            }
            break;

        case 3:
            self->visible = 1;
            EasyFade_FadeBothDisplays(FADER_INSTANT, 0x10, 0x3C);
            self->counter = self->counter - 1;
            if (self->counter <= 0) {
                self->state = 0;
            }
            break;
    }

    if (self->visible != 0) {
        OtuScaleAnim_Step(&self->scaleAnim, &self->affine);

        switch (self->which) {
            case 0:
                Sprite_Update(&self->sprite0);
                break;

            case 1:
            case 2:
                Sprite_Update(&self->sprite1);
                break;

            case 3:
                Sprite_Update(&self->sprite2);
                break;
        }
    }
    return 1;
}

// Nonmatching: argument scheduling of OamMgr_AllocAffineGroup in case 0
static s32 OtosuGame_gameover_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_gameover* self = task->data;

    if (self->visible != 0) {
        switch (self->which) {
            case 0:
                self->sprite0.unk_0A.unk_05 =
                    OamMgr_AllocAffineGroup(&g_OamMgr[self->sprite0.bits_0_1], 0, self->affine.scaleX, self->affine.scaleY, 0);
                Sprite_RenderFrame(&self->sprite0);
                break;

            case 1:
            case 2:
                Sprite_RenderFrame(&self->sprite1);
                break;

            case 3:
                Sprite_RenderFrame(&self->sprite2);
                break;
        }
    }
    return 1;
}

static s32 OtosuGame_gameover_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_gameover* self = task->data;

    Sprite_Release(&self->sprite0);
    Sprite_Release(&self->sprite1);
    return 1;
}

static s32 OtosuGame_gameover_RunTask(TaskPool* pool, Task* task, void* data, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_gameover_Init,
        .update     = OtosuGame_gameover_Update,
        .render     = OtosuGame_gameover_Render,
        .cleanup    = OtosuGame_gameover_Destroy,
    };

    return stages.iter[stage](pool, task, data);
}

s32 OtosuGame_gameover_CreateTask(TaskPool* pool, s32 dataType) {
    OtuTaskArgs1 args;

    args.dataType = dataType;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_gameover, NULL, 0, NULL, &args);
}

void OtosuGame_gameover_Show(OtosuGame_gameover* self, s32 which) {
    self->state   = 1;
    self->counter = 0x3C;
    self->which   = which;

    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;
    OtuScaleAnim_Init(&self->scaleAnim, OtosuGame_gameover_ScaleKeys, 0xC, &self->affine);

    switch (which) {
        case 0:
            SndMgr_StartPlayingSE(0x55F);
            Sprite_ChangeAnimation(&self->sprite0, self->sprite0.animData, 1, self->sprite0.cellTable);
            break;

        case 1:
            SndMgr_StartPlayingSE(0x560);
            Sprite_ChangeAnimation(&self->sprite1, self->sprite1.animData, 1, self->sprite1.cellTable);
            break;

        case 2:
            SndMgr_StartPlayingSE(0x561);
            Sprite_ChangeAnimation(&self->sprite1, self->sprite1.animData, 2, self->sprite1.cellTable);
            break;

        case 3:
            Sprite_ChangeAnimation(&self->sprite2, self->sprite2.animData, 1, self->sprite2.cellTable);
            break;
    }

    SndMgr_StartPlayingSE(0x34B);
}

s32 OtosuGame_gameover_IsActive(OtosuGame_gameover* self) {
    return self->state != 0;
}
