#include "Otosu/OtosuGame.h"

typedef struct {
    s32 dataType;
    s32 pinId;
    s32 index;
    s32 hasSpriteA;
} OtosuGame_badgecount_Args;

static s32              OtosuGame_badgecount_RunTask(TaskPool* pool, Task* self, void* arg, s32 stage);
static SpriteFrameInfo* OtosuGame_badgecount_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_badgecount = {"Tsk_OtosuGame_badgecount", OtosuGame_badgecount_RunTask,
                                                    sizeof(OtosuGame_badgecount)};

static const SpriteAnimation OtosuGame_badgecount_AnimSpriteA = {
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
    .frameInfoCallback = OtosuGame_badgecount_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
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

static const SpriteAnimation OtosuGame_badgecount_AnimDigit = {
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
    .frameInfoCallback = OtosuGame_badgecount_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
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

static const SpriteAnimation OtosuGame_badgecount_AnimSpriteB = {
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
    .frameInfoCallback = OtosuGame_badgecount_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_OverObj,
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

static SpriteFrameInfo* OtosuGame_badgecount_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 3);
}

static void OtosuGame_badgecount_LoadSpriteA(OtosuGame_badgecount* data, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_badgecount_AnimSpriteA;

    anim.owner    = data;
    anim.dataType = data->dataType;
    anim.posY     = anim.posY + data->index * 0x13;

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_badgecount_LoadSpriteB(OtosuGame_badgecount* data, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_badgecount_AnimSpriteB;

    anim.owner     = data;
    anim.dataType  = data->dataType;
    anim.animIndex = (data->index + 1) * 7;

    _Sprite_Load(sprite, &anim);
}

static void OtosuGame_badgecount_LoadDigit(OtosuGame_badgecount* data, Sprite* sprite, s32 index) {
    SpriteAnimation anim   = OtosuGame_badgecount_AnimDigit;
    s16             lut[4] = {0x21, 0x21, 0x21, 0x1D};

    anim.owner     = data;
    anim.dataType  = data->dataType;
    anim.animIndex = lut[index];

    if (index < 3) {
        anim.posX = anim.posX + (2 - index) * 6;
    }

    anim.posY = anim.posY + data->index * 0x13;

    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_badgecount_Init(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgecount_Args* taskArgs = args;
    OtosuGame_badgecount*      data     = (OtosuGame_badgecount*)self->data;
    s32                        i;

    data->dataType   = taskArgs->dataType;
    data->pinId      = taskArgs->pinId;
    data->index      = taskArgs->index;
    data->resolved   = 0;
    data->hasSpriteA = taskArgs->hasSpriteA;
    data->visible    = 0xC;

    if (data->hasSpriteA != 0) {
        OtosuGame_badgecount_LoadSpriteA(data, &data->spriteA);
    }

    OtosuGame_badgecount_LoadSpriteB(data, &data->spriteB);

    for (i = 0; i < 4; i++) {
        OtosuGame_badgecount_LoadDigit(data, &data->digits[i], i);
    }

    return 1;
}

static s32 OtosuGame_badgecount_Update(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgecount* data = (OtosuGame_badgecount*)self->data;

    data->resolved = EasyTask_GetTaskData(pool, data->pinId) != NULL;
    return 1;
}

static s32 OtosuGame_badgecount_Render(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgecount* data = (OtosuGame_badgecount*)self->data;
    s32                   i;

    if (data->resolved != 0) {
        if (data->hasSpriteA != 0) {
            Sprite_Update(&data->spriteA);
            Sprite_RenderFrame(&data->spriteA);
        }

        Sprite_Update(&data->spriteB);
        Sprite_RenderFrame(&data->spriteB);

        for (i = 0; i < 4; i++) {
            if (data->visible & (1 << i)) {
                Sprite_Update(&data->digits[i]);
                Sprite_RenderFrame(&data->digits[i]);
            }
        }
    }

    return 1;
}

static s32 OtosuGame_badgecount_Destroy(TaskPool* pool, Task* self, void* args) {
    OtosuGame_badgecount* data = (OtosuGame_badgecount*)self->data;
    s32                   i;

    if (data->hasSpriteA != 0) {
        Sprite_Release(&data->spriteA);
    }

    Sprite_Release(&data->spriteB);

    for (i = 0; i < 4; i++) {
        Sprite_Release(&data->digits[i]);
    }

    return 1;
}

static s32 OtosuGame_badgecount_RunTask(TaskPool* pool, Task* self, void* arg, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_badgecount_Init,
        .update     = OtosuGame_badgecount_Update,
        .render     = OtosuGame_badgecount_Render,
        .cleanup    = OtosuGame_badgecount_Destroy,
    };

    return stages.iter[stage](pool, self, arg);
}

s32 OtosuGame_badgecount_CreateTask(TaskPool* pool, s32 dataType, s32 pinId, s32 index, s32 hasSpriteA) {
    OtosuGame_badgecount_Args args;

    args.dataType   = dataType;
    args.pinId      = pinId;
    args.index      = index;
    args.hasSpriteA = hasSpriteA;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_badgecount, NULL, 0, NULL, &args);
}

s32 OtosuGame_badgecount_ShowPinCount(OtosuGame_badgecount* data, u16* slots) {
    s32 i;

    for (i = 0; i < 6; i++) {
        if (slots[i] == OTU_NO_PIN) {
            break;
        }
    }

    return Sprite_ChangeAnimation(&data->spriteB, data->spriteB.animData, (s16)(data->index * 7 + i + 1),
                                  data->spriteB.cellTable);
}

void OtosuGame_badgecount_SetValue(OtosuGame_badgecount* data, s32 value) {
    s16 buf[4];
    s32 count;
    s32 i;

    count = 0;
    for (i = 0; i < 3; i++) {
        buf[i] = (s16)(0x21 + value % 10);
        count++;
        value /= 10;
        if (value == 0) {
            break;
        }
    }

    data->visible = 8;

    count--;
    for (i = 2; i >= 0; i--) {
        Sprite_ChangeAnimation(&data->digits[i], data->digits[i].animData, buf[count], data->digits[i].cellTable);
        data->visible |= (1 << i);

        count--;
        if (count < 0) {
            return;
        }
    }
}
