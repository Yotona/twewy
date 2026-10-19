#include "Otosu/OtosuMenu.h"

static SpriteFrameInfo* OtosuMenu_Icon2_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const SpriteAnimation OtosuMenu_Icon2_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = {.raw = 0x0800},
    .posX              = 0x0080,
    .posY              = 0x0060,
    .frameInfoCallback = OtosuMenu_Icon2_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtosuMenu_ObjBinId,
    .unk_18            = 0x0000,
    .packIndex         = 0,
    .unk_1C            = 0x0021,
    .unk_1E            = 0x0000,
    .unk_20            = 0x0024,
    .unk_22            = 0x0001,
    .unk_24            = 0x0000,
    .unk_26            = 0x0022,
    .unk_28            = 0x0023,
    .animIndex         = 0x0001,
};

static void OtosuMenu_Icon2_Load(s32 ctx, void* sprite) {
    SpriteAnimation anim = OtosuMenu_Icon2_Anim;

    if (_Sprite_Load(sprite, &anim) == 0) {
        OS_WaitForever();
    }
}

static void OtosuMenu_Icon2_Destroy(s32 ctx, void* sprite) {
    Sprite_Destroy(sprite);
}

static void OtosuMenu_Icon2_Update(s32 ctx, void* sprite) {
    Sprite_UpdateAndCheck(sprite);
}

static void OtosuMenu_Icon2_Render(s32 ctx, void* sprite) {
    Sprite_Render(sprite);
}

static PrcStepResult OtosuMenu_Icon2_Step_Continue(PrcCtx* ctx, void* sprite) {
    return PRC_STEP_CONTINUE;
}

static PrcStepFn OtosuMenu_Icon2_StepTable[] = {OtosuMenu_Icon2_Step_Continue};

PrcFrameDesc OtosuMenu_Icon2_FrameDesc = {
    .enter     = OtosuMenu_Icon2_Load,
    .stepTable = OtosuMenu_Icon2_StepTable,
    .update    = OtosuMenu_Icon2_Update,
    .render    = OtosuMenu_Icon2_Render,
    .exit      = OtosuMenu_Icon2_Destroy,
};
