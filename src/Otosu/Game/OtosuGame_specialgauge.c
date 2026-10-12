#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ u16 x;
    /* 0x02 */ u16 y;
} OtuGaugePos; // Size: 0x4

typedef struct {
    /* 0x00 */ s32 dataType;   // SpriteAnimation.dataType, four bits
    /* 0x04 */ s32 pinId;
} OtosuGame_specialgauge_Args; // Size: 0x8

typedef struct {
    /* 0x00 */ Sprite spriteLow;  // units digit
    /* 0x40 */ Sprite spriteHigh; // tens digit
} OtuGaugePair;                   // Size: 0x80

typedef struct {
    /* 0x000 */ s32            dataType;
    /* 0x004 */ s32            pinId;
    /* 0x008 */ s32            running;
    /* 0x00C */ Sprite         spriteA[4];
    /* 0x10C */ Sprite         spriteB[4];
    /* 0x20C */ OtuGaugePair   digit[4];     // the four two-digit displays
    /* 0x40C */ Sprite         plate;        // the wide backing sprite
    /* 0x44C */ Sprite         dial;         // the wide sprite drawn through the affine path
    /* 0x48C */ OamAffineParam affine;       // the dial spins 0x100 per frame
    /* 0x49C */ s16            lastCount[4]; // the count each cell was last drawn for
    /* 0x4A4 */ s32            alive[4];
} OtosuGame_specialgauge;                    // Size: 0x4B4

static s32              OtosuGame_specialgauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_specialgauge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_specialgauge = {"Tsk_OtosuGame_specialgauge", OtosuGame_specialgauge_RunTask,
                                                      sizeof(OtosuGame_specialgauge)};

static const SpriteAnimation OtosuGame_specialgauge_AnimDial = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 7,
    .posX              = 0x20,
    .posY              = 0x98,
    .frameInfoCallback = OtosuGame_specialgauge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
    .unk_18            = 2,
    .packIndex         = 9,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const SpriteAnimation OtosuGame_specialgauge_AnimPlate = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = OtosuGame_specialgauge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
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

static const SpriteAnimation OtosuGame_specialgauge_AnimCell = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = OtosuGame_specialgauge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 5,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static const SpriteAnimation OtosuGame_specialgauge_AnimDigit = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = OtosuGame_specialgauge_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 5,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 2,
};

static OtuGaugePos OtosuGame_specialgauge_OnesPos[4] = {
    {0x37, 0x9C},
    {0x27, 0x83},
    {0x14, 0x90},
    {0x1E, 0xA7},
};

static OtuGaugePos OtosuGame_specialgauge_TwoDigitPos[4][2] = {
    {{0x39, 0x9C}, {0x32, 0x9C}},
    {{0x2A, 0x83}, {0x23, 0x83}},
    {{0x15, 0x90},  {0xE, 0x90}},
    {{0x22, 0xA7}, {0x1B, 0xA7}},
};

extern u8 data_02071cf0[];

static SpriteFrameInfo* OtosuGame_specialgauge_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 3);
}

static void OtosuGame_specialgauge_LoadDial(OtosuGame_specialgauge* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_specialgauge_AnimDial;

    anim.owner    = self;
    anim.dataType = (u16)self->dataType;

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_specialgauge_LoadPlate(OtosuGame_specialgauge* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_specialgauge_AnimPlate;

    anim.owner    = self;
    anim.dataType = (u16)self->dataType;

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_specialgauge_LoadCell(OtosuGame_specialgauge* self, Sprite* sprite, s32 index) {
    SpriteAnimation anim = OtosuGame_specialgauge_AnimCell;

    anim.owner     = self;
    anim.dataType  = (u16)self->dataType;
    anim.packIndex = index + 1;

    _Sprite_Load(sprite, &anim);
}

// Nonmatching: scheduling and register allocation
static void OtosuGame_specialgauge_LoadDigit(OtosuGame_specialgauge* self, Sprite* sprite, s32 index, s32 which) {
    SpriteAnimation anim = OtosuGame_specialgauge_AnimDigit;

    anim.owner     = self;
    anim.dataType  = (u16)self->dataType;
    anim.posX      = OtosuGame_specialgauge_TwoDigitPos[index][which].x;
    anim.posY      = OtosuGame_specialgauge_TwoDigitPos[index][which].y;
    anim.packIndex = index + 1;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_specialgauge_Init(TaskPool* pool, Task* task, void* arg) {
    OtosuGame_specialgauge*      self = (OtosuGame_specialgauge*)task->data;
    OtosuGame_specialgauge_Args* args = (OtosuGame_specialgauge_Args*)arg;
    s32                          i;

    self->dataType = args->dataType;
    self->pinId    = args->pinId;

    OtosuGame_specialgauge_LoadDial(self, &self->dial);
    OtosuGame_specialgauge_LoadPlate(self, &self->plate);

    for (i = 0; i < 4; i++) {
        self->lastCount[i] = 0;
        self->alive[i]     = 0;

        OtosuGame_specialgauge_LoadCell(self, &self->spriteA[i], i);
        Sprite_ChangeAnimation(&self->spriteA[i], self->spriteA[i].animData, 0xC, self->spriteA[i].cellTable);

        OtosuGame_specialgauge_LoadCell(self, &self->spriteB[i], i);
        OtosuGame_specialgauge_LoadDigit(self, &self->digit[i].spriteLow, i, 0);
        OtosuGame_specialgauge_LoadDigit(self, &self->digit[i].spriteHigh, i, 1);
    }

    self->affine.rotation = 0;
    self->affine.scaleX   = 0x1000;
    self->affine.scaleY   = 0x1000;
    self->affine.unk_0C   = 0;
    self->affine.unk_0E   = 0;

    return 1;
}

// Nonmatching: instruction selection differs
static s32 OtosuGame_specialgauge_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_specialgauge* self = (OtosuGame_specialgauge*)task->data;
    OtosuGame_badge*        pin;
    s32                     i;
    s32                     count;
    s32                     alive;

    self->running         = 1;
    self->affine.rotation = (u16)(self->affine.rotation + 0x100);

    Sprite_Update(&self->dial);
    Sprite_Update(&self->plate);

    pin = (OtosuGame_badge*)EasyTask_GetTaskData(pool, self->pinId);

    for (i = 0; i < 4; i++) {
        s16               frame;
        s32               tens;
        UnkSmallInternal* pal;
        Data*             file;

        alive = 0;

        switch (i) {
            case 0:
                alive = OtosuGame_badge_IsUsingNeedle(pin);
                count = OtosuGame_badge_GetNeedleUses(pin);
                break;

            case 1:
                alive = OtosuGame_badge_IsUsingMeteo(pin);
                count = OtosuGame_badge_GetMeteoUses(pin);
                break;

            case 2:
                alive = OtosuGame_badge_IsUsingHammer(pin);
                count = OtosuGame_badge_GetHammerUses(pin);
                break;

            case 3:
                alive = OtosuGame_badge_IsUsingHand(pin);
                count = OtosuGame_badge_GetHandUses(pin);
                break;

            default:
                break;
        }

        if (count == 0 && alive == 0) {
            file = self->spriteB[i].resourceData;

            pal = (UnkSmallInternal*)Data_GetPackEntryData(file, 5);

            self->spriteA[i].unk3C          = pal;
            self->spriteB[i].unk3C          = pal;
            self->digit[i].spriteLow.unk3C  = pal;
            self->digit[i].spriteHigh.unk3C = pal;
        }

        self->alive[i] = alive;

        if (count != (s32)self->lastCount[i]) {
            if (count > 0) {
                file = self->spriteB[i].resourceData;

                pal = (UnkSmallInternal*)Data_GetPackEntryData(file, 4);

                self->spriteA[i].unk3C          = pal;
                self->spriteB[i].unk3C          = pal;
                self->digit[i].spriteLow.unk3C  = pal;
                self->digit[i].spriteHigh.unk3C = pal;
            }

            frame = (s16)(count % 10);
            Sprite_ChangeAnimation(&self->digit[i].spriteLow, self->digit[i].spriteLow.animData, frame + 2,
                                   self->digit[i].spriteLow.cellTable);

            if (count < 0xA) {
                self->digit[i].spriteLow.posX = OtosuGame_specialgauge_OnesPos[i].x;
                self->digit[i].spriteLow.posY = OtosuGame_specialgauge_OnesPos[i].y;
            } else {
                self->digit[i].spriteLow.posX = OtosuGame_specialgauge_TwoDigitPos[i][0].x;
                self->digit[i].spriteLow.posY = OtosuGame_specialgauge_TwoDigitPos[i][0].y;

                tens  = count / 10;
                frame = (s16)(tens % 10);
                Sprite_ChangeAnimation(&self->digit[i].spriteHigh, self->digit[i].spriteHigh.animData, frame + 2,
                                       self->digit[i].spriteHigh.cellTable);

                self->digit[i].spriteHigh.posX = OtosuGame_specialgauge_TwoDigitPos[i][1].x;
                self->digit[i].spriteHigh.posY = OtosuGame_specialgauge_TwoDigitPos[i][1].y;
            }

            self->lastCount[i] = (s16)count;
        }

        if (self->alive[i] != 0) {
            Sprite_Update(&self->spriteA[i]);
        }
        Sprite_Update(&self->spriteB[i]);
        Sprite_Update(&self->digit[i].spriteLow);

        if (self->lastCount[i] >= 0xA) {
            Sprite_Update(&self->digit[i].spriteHigh);
        }
    }

    return 1;
}

// Nonmatching: instruction selection differs
static s32 OtosuGame_specialgauge_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_specialgauge* self = (OtosuGame_specialgauge*)task->data;
    s32                     i;

    if (self->running != 0) {
        self->dial.unk_0A.unk_05 = (u16)OamMgr_AllocAffineGroup(&g_OamMgr[self->dial.bits_0_1], self->affine.rotation,
                                                                self->affine.scaleX, self->affine.scaleY, 0);
        Sprite_RenderFrame(&self->dial);
        Sprite_RenderFrame(&self->plate);

        for (i = 0; i < 4; i++) {
            Sprite_RenderFrame(&self->spriteB[i]);
            Sprite_RenderFrame(&self->digit[i].spriteLow);

            if (self->lastCount[i] >= 0xA) {
                Sprite_RenderFrame(&self->digit[i].spriteHigh);
            }

            if (self->alive[i] != 0) {
                Sprite_RenderFrame(&self->spriteA[i]);
            }
        }
    }

    return 1;
}

static s32 OtosuGame_specialgauge_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_specialgauge* self = (OtosuGame_specialgauge*)task->data;
    s32                     i;

    Sprite_Release(&self->dial);
    Sprite_Release(&self->plate);

    for (i = 0; i < 4; i++) {
        Sprite_Release(&self->spriteA[i]);
        Sprite_Release(&self->spriteB[i]);
        Sprite_Release(&self->digit[i].spriteLow);
        Sprite_Release(&self->digit[i].spriteHigh);
    }

    return 1;
}

static s32 OtosuGame_specialgauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_specialgauge_Init,
        .update     = OtosuGame_specialgauge_Update,
        .render     = OtosuGame_specialgauge_Render,
        .cleanup    = OtosuGame_specialgauge_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_specialgauge_CreateTask(TaskPool* pool, s32 dataType, s32 pinId) {
    OtosuGame_specialgauge_Args args;

    args.dataType = dataType;
    args.pinId    = pinId;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_specialgauge, NULL, 0, NULL, &args);
}
