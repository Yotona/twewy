#include "Otosu/OtosuMenu.h"

static void          OtosuMenu_Icon_Load(void* ctx, void* sprite, s16* y);
static void          OtosuMenu_Icon_Destroy(PrcCtx* ctx, void* sprite);
static void          OtosuMenu_Icon_Update(PrcCtx* ctx, void* sprite);
static void          OtosuMenu_Icon_Render(PrcCtx* ctx, void* sprite);
static PrcStepResult OtosuMenu_Icon_Step_Idle(PrcCtx* ctx, void* unused);

static PrcStepFn OtosuMenu_Icon_StepTable[] = {OtosuMenu_Icon_Step_Idle};

PrcFrameDesc OtosuMenu_Icon_FrameDesc = {
    .enter     = OtosuMenu_Icon_Load,
    .stepTable = OtosuMenu_Icon_StepTable,
    .update    = OtosuMenu_Icon_Update,
    .render    = OtosuMenu_Icon_Render,
    .exit      = OtosuMenu_Icon_Destroy,
};

static SpriteFrameInfo* OtosuMenu_Icon_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const SpriteAnimation OtosuMenu_Icon_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = {.raw = 0x0400},
    .posX              = -13,
    .posY              = 0x000C,
    .frameInfoCallback = OtosuMenu_Icon_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtosuMenu_ObjBinId,
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

static void OtosuMenu_Icon_Load(void* ctx, void* sprite, s16* y) {
    SpriteAnimation anim = OtosuMenu_Icon_Anim;

    anim.bits_0_1  = 0;
    anim.animIndex = 1;
    anim.posX      = 0x80;
    anim.posY      = *y;
    anim.unk_1C    = 0x11;
    anim.unk_20    = 0x14;
    anim.unk_22    = 2;
    anim.unk_26    = 0x12;
    anim.unk_28    = 0x13;

    if (_Sprite_Load((Sprite*)sprite, &anim) == 0) {
        OS_WaitForever();
    }
}

static void OtosuMenu_Icon_Destroy(PrcCtx* ctx, void* sprite) {
    Sprite_Destroy(sprite);
}

static void OtosuMenu_Icon_Update(PrcCtx* ctx, void* sprite) {
    Sprite_UpdateAndCheck(sprite);
}

static void OtosuMenu_Icon_Render(PrcCtx* ctx, void* sprite) {
    Sprite_Render(sprite);
}

static PrcStepResult OtosuMenu_Icon_Step_Idle(PrcCtx* ctx, void* unused) {
    return PRC_STEP_CONTINUE;
}

void OtosuMenu_DestroyResultObjects(OtosuMenuObj* menuObj) {
    PrcCtx* icons[4] = {&menuObj->objCtx[0], &menuObj->objCtx[1], &menuObj->objCtx[2], &menuObj->objCtx[3]};
    u16     i;

    for (i = 0; i < gSaveData.otosuPlayerCount; i++) {
        PrcMaster_UnregisterContext(&menuObj->prcMaster, icons[i]);
    }
    PrcMaster_UnregisterContext(&menuObj->prcMaster, &menuObj->objCtx[4]);
}

// Nonmatching: Several instructions reordered
void OtosuMenu_ComputeRanks(OtosuMenuObj* menuObj) {
    u16 prevScore = 0xFFFF;
    u16 placed    = 0;
    u16 place     = 0;
    u16 rank      = 0;

    menuObj->firstPlaceUnique = 0;
    for (place = 0; place < gSaveData.otosuPlayerCount; place++) {
        u16 player = 0xFFFF;
        u16 best   = 0;
        u16 i;
        for (i = 0; i < gSaveData.otosuPlayerCount; i++) {
            if ((!(placed & (1 << i))) && (best <= gSaveData.otosuScores[i])) {
                best   = gSaveData.otosuScores[i];
                player = i;
            }
        }

        if ((prevScore != 0xFFFF) && (prevScore != best)) {
            if (place == 1) {
                menuObj->firstPlaceUnique = 1;
            }
            rank++;
        }
        prevScore                    = best;
        menuObj->placeRanks[place]   = rank;
        menuObj->playerRanks[player] = rank;
        placed |= 1 << player;
        menuObj->placePlayers[place] = player;
    }
}

void OtosuMenu_ComputeSinglePlayerRanks(OtosuMenuObj* menuObj) {
    u16 placed    = 0;
    u16 place     = 0;
    u16 rank      = 0;
    u16 prevScore = 0xFFFF;
    u8  winner;

    menuObj->firstPlaceUnique = TRUE;
    for (place = 0; place < gSaveData.otosuPlayerCount; place++) {
        u16 i      = 0;
        u16 player = 0xFFFF;
        u16 best   = 0;

        for (i = 0; i < gSaveData.otosuPlayerCount; i++) {
            if (!(placed & (1 << i)) && best <= gSaveData.otosuScores[i]) {
                best   = gSaveData.otosuScores[i];
                player = i;
            }
        }
        if (prevScore != 0xFFFF && best != prevScore) {
            rank++;
        } else if (place == 1 && player == 0) {
            rank++;
        }
        menuObj->placeRanks[place]   = rank;
        menuObj->playerRanks[player] = rank;
        menuObj->placePlayers[place] = player;
        prevScore                    = best;
        placed |= 1 << player;
    }
    if (menuObj->placePlayers[1] != 0) {
        return;
    }
    winner = menuObj->placePlayers[0];
    if (gSaveData.otosuScores[0] == gSaveData.otosuScores[winner]) {
        menuObj->placePlayers[1] = winner;
        menuObj->placePlayers[0] = 0;
    }
}

u16* OtosuMenu_GetPlaceName(OtosuMenuObj* menuObj, u16 place) {
    u8  player    = menuObj->placePlayers[place];
    u32 unused[5] = {0, 0, 0, 0, 0};

    return menuObj->players[player].name;
}
