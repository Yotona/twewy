#include "Otosu/OtosuMenu.h"

static SpriteFrameInfo* OtosuMenu_DelDataWin_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const SpriteAnimation OtosuMenu_DelDataWin_Anim = {
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
    .frameInfoCallback = OtosuMenu_DelDataWin_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = (BinIdentifier*)&OtosuMenu_FldDownScreenBinId,
    .unk_18            = 0x0002,
    .packIndex         = 0x000D,
    .unk_1C            = 0x0002,
    .unk_1E            = 0x0000,
    .unk_20            = 0x0001,
    .unk_22            = 0x0002,
    .unk_24            = 0x0000,
    .unk_26            = 0x0003,
    .unk_28            = 0x0004,
    .animIndex         = 0x0001,
};

static void OtosuMenu_DelDataWin_Load(PrcCtx* ctx, Sprite* sprites) {
    SpriteAnimation anim = OtosuMenu_DelDataWin_Anim;

    u16 i = 0;

    const s16 rows[4][8] = {
        {0x000C, 0x0002, 0x0001, 0x0003, 0x0004, 0x0000, 0xFFF6, 0x0001},
        {0x000D, 0x0002, 0x0001, 0x0003, 0x0004, 0x0000, 0xFFF6, 0x0001},
        {0x000D, 0x0002, 0x0001, 0x0003, 0x0004, 0x0000, 0x000A, 0x0001},
        {0x000C, 0x0002, 0x0001, 0x0003, 0x0004, 0x0000, 0x000A, 0x0002},
    };

    for (; i < 4; i++) {
        anim.packIndex = rows[i][0];
        anim.unk_1C    = rows[i][1];
        anim.unk_20    = rows[i][2];
        anim.unk_26    = rows[i][3];
        anim.unk_28    = rows[i][4];
        anim.posX      = rows[i][5] + 0x80;
        anim.posY      = rows[i][6] + 0x60;
        anim.animIndex = rows[i][7];
        if (_Sprite_Load(&sprites[i], &anim) == 0) {
            OS_WaitForever();
        }
    }
}

static void OtosuMenu_DelDataWin_Destroy(PrcCtx* ctx, Sprite* sprites) {
    for (u16 i = 0; i < 4; i++) {
        Sprite_Destroy(&sprites[i]);
    }
}

static void OtosuMenu_DelDataWin_Update(PrcCtx* ctx, Sprite* sprites) {
    for (u16 i = 0; i < 4; i++) {
        Sprite_UpdateAndCheck(&sprites[i]);
    }
}

static void OtosuMenu_DelDataWin_Render(PrcCtx* ctx, Sprite* sprites) {
    for (u16 i = 0; i < 4; i++) {
        Sprite_Render(&sprites[i]);
    }
}

static PrcStepResult OtosuMenu_DelDataWin_Step_Continue(PrcCtx* ctx, void* object) {
    return PRC_STEP_CONTINUE;
}

static PrcStepFn OtosuMenu_DelDataWin_StepTable[] = {OtosuMenu_DelDataWin_Step_Continue};

PrcFrameDesc OtosuMenu_DelDataWin_FrameDesc = {
    .enter     = OtosuMenu_DelDataWin_Load,
    .stepTable = OtosuMenu_DelDataWin_StepTable,
    .update    = OtosuMenu_DelDataWin_Update,
    .render    = OtosuMenu_DelDataWin_Render,
    .exit      = OtosuMenu_DelDataWin_Destroy,
};
