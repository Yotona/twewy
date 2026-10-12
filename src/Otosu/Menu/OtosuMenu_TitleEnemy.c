#include "Otosu/OtosuMenu.h"

/** @brief Rounds an integer pixel count to fx32 the way the float-based FX32 constant macros do. */
#define FX32_ROUND(value) ((fx32)((value) > 0 ? (f32)((value) << FX32_SHIFT) + 0.5f : (f32)((value) << FX32_SHIFT) - 0.5f))

static PrcStepResult OtosuMenu_TitleEnemy_Step_Begin(PrcCtx* ctx, void* work);
static PrcStepResult OtosuMenu_TitleEnemy_Step_Slide(PrcCtx* ctx, void* work);
static void          OtosuMenu_TitleEnemy_Load(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy, OtosuMenu_TitleEnmArg* arg);
static void          OtosuMenu_TitleEnemy_Destroy(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy);
static void          OtosuMenu_TitleEnemy_Update(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy);
static void          OtosuMenu_TitleEnemy_Render(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy);

static PrcStepFn OtosuMenu_TitleEnemy_StepTable[] = {OtosuMenu_TitleEnemy_Step_Begin, OtosuMenu_TitleEnemy_Step_Slide,
                                                     PrcStep_Continue};

PrcFrameDesc OtosuMenu_TitleEnemy_FrameDesc = {
    .enter     = OtosuMenu_TitleEnemy_Load,
    .stepTable = OtosuMenu_TitleEnemy_StepTable,
    .update    = OtosuMenu_TitleEnemy_Update,
    .render    = OtosuMenu_TitleEnemy_Render,
    .exit      = OtosuMenu_TitleEnemy_Destroy,
};

static SpriteFrameInfo* OtosuMenu_TitleEnemy_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const SpriteAnimation OtosuMenu_TitleEnemy_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0x0000,
    .posX              = -13,
    .posY              = 0x000C,
    .frameInfoCallback = OtosuMenu_TitleEnemy_GetFrameInfo,
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

typedef struct {
    u8 palette[4]; // indexed by enemy
} TitleEnemyPalettes;

static const TitleEnemyPalettes OtosuMenu_TitleEnemy_Palettes = {
    {3, 3, 2, 2}
};

// Nonmatching: instruction selection differs
static void OtosuMenu_TitleEnemy_Load(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy, OtosuMenu_TitleEnmArg* arg) {
    u16 cells[4][4] = {
        { 1,  4,  2,  3},
        { 5,  8,  6,  7},
        { 9, 12, 10, 11},
        {13, 16, 14, 15},
    };
    SpriteAnimation    anim = OtosuMenu_TitleEnemy_Anim;
    TitleEnemyPalettes palettes;

    enemy->x           = 0x80000;
    enemy->y           = 0x1C0000;
    anim.unk_1C        = cells[arg->index][0];
    anim.unk_22        = 4;
    anim.unk_20        = cells[arg->index][1];
    anim.unk_26        = cells[arg->index][2];
    anim.unk_28        = cells[arg->index][3];
    palettes           = OtosuMenu_TitleEnemy_Palettes;
    anim.animIndex     = 1;
    anim.bits_0_1      = 0;
    anim.unk_02.unk_10 = palettes.palette[arg->index];
    if (_Sprite_Load(&enemy->mainSprites[0], &anim) == 0) {
        OS_WaitForever();
    }
    anim.animIndex = 3;
    if (_Sprite_Load(&enemy->mainSprites[1], &anim) == 0) {
        OS_WaitForever();
    }
    anim.bits_0_1  = 1;
    anim.animIndex = 1;
    if (_Sprite_Load(&enemy->subSprites[0], &anim) == 0) {
        OS_WaitForever();
    }
    anim.animIndex = 3;
    if (_Sprite_Load(&enemy->subSprites[1], &anim) == 0) {
        OS_WaitForever();
    }
    enemy->duration = arg->duration;
    enemy->timer    = arg->duration;
    enemy->index    = arg->index;
}

static void OtosuMenu_TitleEnemy_Destroy(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy) {
    u16 i;

    for (i = 0; i < 2; i++) {
        Sprite_Destroy(&enemy->mainSprites[i]);
        Sprite_Destroy(&enemy->subSprites[i]);
    }
}

static inline void OtosuMenu_TitleEnemy_SetSpritePos(Sprite* sprite, fx32 x, fx32 y) {
    sprite->posX = x >> FX32_SHIFT;
    sprite->posY = y >> FX32_SHIFT;
}

static void OtosuMenu_TitleEnemy_Update(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy) {
    u16 i;

    OtosuMenu_TitleEnemy_SetSpritePos(&enemy->mainSprites[0], enemy->x, enemy->y - 0x40000);
    OtosuMenu_TitleEnemy_SetSpritePos(&enemy->subSprites[0], enemy->x, enemy->y + 0x80000);
    OtosuMenu_TitleEnemy_SetSpritePos(&enemy->mainSprites[1], enemy->x, enemy->y + 0x40000);
    OtosuMenu_TitleEnemy_SetSpritePos(&enemy->subSprites[1], enemy->x, enemy->y + 0x100000);
    for (i = 0; i < 2; i++) {
        Sprite_UpdateAndCheck(&enemy->mainSprites[i]);
        Sprite_UpdateAndCheck(&enemy->subSprites[i]);
    }
}

static void OtosuMenu_TitleEnemy_Render(PrcCtx* ctx, OtosuMenu_TitleEnmObj* enemy) {
    u16 i;

    for (i = 0; i < 2; i++) {
        Sprite_Render(&enemy->mainSprites[i]);
        Sprite_Render(&enemy->subSprites[i]);
    }
}

static PrcStepResult OtosuMenu_TitleEnemy_Step_Begin(PrcCtx* ctx, void* work) {
    OtosuMenu_TitleEnmObj* enemy = work;

    enemy->progress = 0;
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

static PrcStepResult OtosuMenu_TitleEnemy_Step_Slide(PrcCtx* ctx, void* work) {
    OtosuMenu_TitleEnmObj* enemy      = work;
    u16                    targetY[4] = {44, 38, 117, 108};
    u16                    timer;

    func_020265d4(&enemy->y, FX32_ROUND(targetY[enemy->index]), enemy->timer);
    func_020265d4(&enemy->progress, 0x10000, enemy->timer);
    timer = enemy->timer;
    if (timer == 0) {
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    if (OtosuMenu_TitleSkipped == 1) {
        enemy->y = FX32_ROUND(targetY[enemy->index]);
        PrcCtx_AdvanceStep(ctx);
        return PRC_STEP_CONTINUE;
    }
    enemy->timer = timer - 1;
    return PRC_STEP_CONTINUE;
}
