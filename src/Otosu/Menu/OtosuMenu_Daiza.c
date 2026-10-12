#include "Otosu/OtosuMenu.h"

static SpriteFrameInfo* OtosuMenu_Daiza_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const SpriteAnimation OtosuMenu_Daiza_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0x0000,
    .posX              = 0x0080,
    .posY              = 0x0060,
    .frameInfoCallback = OtosuMenu_Daiza_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = (BinIdentifier*)&OtosuMenu_ObjBinId,
    .unk_18            = 0x0000,
    .packIndex         = 0x0000,
    .unk_1C            = 0x0015,
    .unk_1E            = 0x0000,
    .unk_20            = 0x0018,
    .unk_22            = 0x0002,
    .unk_24            = 0x0000,
    .unk_26            = 0x0016,
    .unk_28            = 0x0017,
    .animIndex         = 0x0001,
};

static void OtosuMenu_Daiza_Load(PrcCtx* ctx, void* sprite) {
    SpriteAnimation anim = OtosuMenu_Daiza_Anim;

    anim.bits_0_1  = 1;
    anim.unk_1C    = 0x19;
    anim.unk_20    = 0x1C;
    anim.animIndex = 1;
    anim.unk_26    = 0x1A;
    anim.unk_28    = 0x1B;
    anim.animIndex = 1;
    anim.unk_22    = 1;

    if (_Sprite_Load(sprite, &anim) == 0) {
        OS_WaitForever();
    }
}

static void OtosuMenu_Daiza_Destroy(PrcCtx* ctx, void* sprite) {
    Sprite_Destroy(sprite);
}

static void OtosuMenu_Daiza_Update(PrcCtx* ctx, void* sprite) {
    Sprite_UpdateAndCheck(sprite);
}

static void OtosuMenu_Daiza_Render(PrcCtx* ctx, void* sprite) {
    Sprite_Render(sprite);
}

static PrcStepResult OtosuMenu_Daiza_Step_Continue(PrcCtx* ctx, void* unused) {
    return PRC_STEP_CONTINUE;
}

static PrcStepFn OtosuMenu_Daiza_StepTable[] = {OtosuMenu_Daiza_Step_Continue};

PrcFrameDesc OtosuMenu_Daiza_FrameDesc = {
    .enter     = OtosuMenu_Daiza_Load,
    .stepTable = OtosuMenu_Daiza_StepTable,
    .update    = OtosuMenu_Daiza_Update,
    .render    = OtosuMenu_Daiza_Render,
    .exit      = OtosuMenu_Daiza_Destroy,
};
