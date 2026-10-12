#include "Otosu/OtosuMenu.h"

/** @brief Work object of a rank board ("OtosuMenu_RankBoardObj"). */
typedef struct {
    /* 0x000 */ u8                     unk_000[0xC];
    /* 0x00C */ OtosuMenu_RankBoardArg arg;
    /* 0x018 */ u16                    digitCount;
    /* 0x01A */ u8                     digits[6]; // score digits, least significant first
    /* 0x020 */ Sprite                 board;
    /* 0x060 */ Sprite                 rankLabel;
    /* 0x0A0 */ Sprite                 nameTag;
    /* 0x0E0 */ Sprite                 digitSprites[5];
} OtosuMenu_RankBoardObj; // Size: 0x220

static void          OtosuMenu_RankBoard_Load(PrcCtx* ctx, OtosuMenu_RankBoardObj* board, OtosuMenu_RankBoardArg* arg);
static void          OtosuMenu_RankBoard_Destroy(PrcCtx* ctx, OtosuMenu_RankBoardObj* board);
static void          OtosuMenu_RankBoard_Update(PrcCtx* ctx, OtosuMenu_RankBoardObj* board);
static void          OtosuMenu_RankBoard_Render(PrcCtx* ctx, OtosuMenu_RankBoardObj* board);
static PrcStepResult OtosuMenu_RankBoard_Step_Begin(PrcCtx* ctx, void* unused);
static PrcStepResult OtosuMenu_RankBoard_Step_Continue(PrcCtx* ctx, void* unused);

static PrcStepFn OtosuMenu_RankBoard_StepTable[] = {OtosuMenu_RankBoard_Step_Begin, OtosuMenu_RankBoard_Step_Continue,
                                                    PrcStep_Continue};

PrcFrameDesc OtosuMenu_RankBoard_FrameDesc = {
    .enter     = OtosuMenu_RankBoard_Load,
    .stepTable = OtosuMenu_RankBoard_StepTable,
    .update    = OtosuMenu_RankBoard_Update,
    .render    = OtosuMenu_RankBoard_Render,
    .exit      = OtosuMenu_RankBoard_Destroy,
};

static SpriteFrameInfo* OtosuMenu_RankBoard_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static const SpriteAnimation OtosuMenu_RankBoard_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0x0000,
    .posX              = -13,
    .posY              = 0x000C,
    .frameInfoCallback = OtosuMenu_RankBoard_GetFrameInfo,
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
    s16 y[4]; // indexed by the board's place
} RankBoardRowY;

typedef struct {
    u16 anim[4];
} RankBoardAnims;

typedef struct {
    s16 x[6]; // indexed by digit position, least significant first
} RankBoardDigitX;

typedef struct {
    u16 anim[10]; // indexed by digit value
} RankBoardDigitAnims;

static const RankBoardAnims OtosuMenu_RankBoard_RankAnims = {
    {4, 5, 6, 7}
};
static const RankBoardRowY OtosuMenu_RankBoard_TagY = {
    {-24, 16, 56, 96}
};
static const RankBoardAnims OtosuMenu_RankBoard_WinnerTagAnims = {
    {19, 20, 21, 22}
};
static const RankBoardAnims OtosuMenu_RankBoard_TagAnims = {
    {9, 10, 11, 12}
};
static const RankBoardRowY OtosuMenu_RankBoard_UnusedY = {
    {-20, 20, 60, 100}
};
static const RankBoardRowY OtosuMenu_RankBoard_BoardY = {
    {0, 40, 80, 120}
};
static const RankBoardAnims OtosuMenu_RankBoard_UnusedAnims = {
    {9, 10, 11, 12}
};
static const RankBoardRowY OtosuMenu_RankBoard_LabelY = {
    {0, 40, 80, 120}
};
static const RankBoardRowY OtosuMenu_RankBoard_DigitY = {
    {0, 40, 80, 120}
};
static const RankBoardAnims OtosuMenu_RankBoard_UnusedWinner = {
    {19, 20, 21, 22}
};
static const RankBoardDigitX OtosuMenu_RankBoard_DigitX = {
    {0, -6, -12, -18, -24, -30}
};
static const RankBoardDigitAnims OtosuMenu_RankBoard_DigitAnims = {
    {8, 9, 10, 11, 12, 13, 14, 15, 16, 17}
};
static const RankBoardDigitAnims OtosuMenu_RankBoard_WinnerDigits = {
    {18, 19, 20, 21, 22, 23, 24, 25, 26, 27}
};

static void OtosuMenu_RankBoard_Load(PrcCtx* ctx, OtosuMenu_RankBoardObj* board, OtosuMenu_RankBoardArg* arg) {
    SpriteAnimation anim = OtosuMenu_RankBoard_Anim;

    board->arg = *arg;

    anim.bits_0_1 = 1;
    anim.unk_1C   = 0x1D;
    anim.unk_20   = 0x20;
    anim.unk_22   = 5;
    anim.unk_26   = 0x1E;
    anim.unk_28   = 0x1F;

    RankBoardRowY boardY = OtosuMenu_RankBoard_BoardY;
    anim.posX            = 0x80;
    anim.posY            = boardY.y[board->arg.place] + 0x60;
    anim.animIndex       = board->arg.winner ? 2 : 1;
    anim.unk_02.unk_10   = 2;
    if (_Sprite_Load(&board->board, &anim) == 0) {
        OS_WaitForever();
    }

    RankBoardRowY labelY     = OtosuMenu_RankBoard_LabelY;
    anim.posX                = 0x80;
    anim.posY                = labelY.y[board->arg.place] + 0x60;
    RankBoardAnims rankAnims = OtosuMenu_RankBoard_RankAnims;
    anim.animIndex           = board->arg.winner ? 3 : rankAnims.anim[board->arg.rank];
    anim.unk_02.unk_10       = 1;
    if (_Sprite_Load(&board->rankLabel, &anim) == 0) {
        OS_WaitForever();
    }

    anim.posX                     = 0x80;
    RankBoardRowY tagY            = OtosuMenu_RankBoard_TagY;
    anim.posX                     = 0x8C;
    anim.posY                     = tagY.y[board->arg.place] + 0x60;
    RankBoardAnims winnerTagAnims = OtosuMenu_RankBoard_WinnerTagAnims;
    RankBoardAnims tagAnims       = OtosuMenu_RankBoard_TagAnims;
    anim.animIndex     = board->arg.winner != 0 ? winnerTagAnims.anim[board->arg.player] : tagAnims.anim[board->arg.player];
    anim.unk_02.unk_10 = 1;
    if (_Sprite_Load(&board->nameTag, &anim) == 0) {
        OS_WaitForever();
    }

    anim.posX                   = 0x80;
    RankBoardRowY unusedY       = OtosuMenu_RankBoard_UnusedY;
    anim.posX                   = 0x8C;
    anim.posY                   = unusedY.y[board->arg.place] + 0x60;
    RankBoardAnims unusedWinner = OtosuMenu_RankBoard_UnusedWinner;
    RankBoardAnims unusedAnims  = OtosuMenu_RankBoard_UnusedAnims;
    anim.animIndex = board->arg.winner != 0 ? unusedWinner.anim[board->arg.rank] : unusedAnims.anim[board->arg.rank];

    u16 score = board->arg.score;
    u16 count = 0;

    board->digits[0] = 0;
    while (score != 0) {
        board->digits[count] = score % 10;
        score /= 10;
        count++;
    }
    if (count == 0) {
        count = 1;
    }
    board->digitCount = count;

    RankBoardDigitX     digitX       = OtosuMenu_RankBoard_DigitX;
    RankBoardRowY       digitY       = OtosuMenu_RankBoard_DigitY;
    RankBoardDigitAnims digitAnims   = OtosuMenu_RankBoard_DigitAnims;
    RankBoardDigitAnims winnerDigits = OtosuMenu_RankBoard_WinnerDigits;
    anim.unk_02.unk_10               = 1;
    for (u16 i = 0; i < board->digitCount; i++) {
        u8 digit = board->digits[i];

        anim.posX      = digitX.x[i] + 0x80;
        anim.posY      = digitY.y[board->arg.place] + 0x60;
        anim.animIndex = board->arg.winner != 0 ? winnerDigits.anim[digit] : digitAnims.anim[digit];
        if (_Sprite_Load(&board->digitSprites[i], &anim) == 0) {
            OS_WaitForever();
        }
    }
}

static void OtosuMenu_RankBoard_Destroy(PrcCtx* ctx, OtosuMenu_RankBoardObj* board) {
    Sprite_Destroy(&board->board);
    Sprite_Destroy(&board->rankLabel);
    Sprite_Destroy(&board->nameTag);
    for (u16 i = 0; i < board->digitCount; i++) {
        Sprite_Destroy(&board->digitSprites[i]);
    }
}

static void OtosuMenu_RankBoard_Update(PrcCtx* ctx, OtosuMenu_RankBoardObj* board) {
    Sprite_UpdateAndCheck(&board->board);
    Sprite_UpdateAndCheck(&board->rankLabel);
    Sprite_UpdateAndCheck(&board->nameTag);
    for (u16 i = 0; i < board->digitCount; i++) {
        Sprite_UpdateAndCheck(&board->digitSprites[i]);
    }
}

static void OtosuMenu_RankBoard_Render(PrcCtx* ctx, OtosuMenu_RankBoardObj* board) {
    Sprite_Render(&board->board);
    Sprite_Render(&board->rankLabel);
    Sprite_Render(&board->nameTag);
    for (u16 i = 0; i < board->digitCount; i++) {
        Sprite_Render(&board->digitSprites[i]);
    }
}

static PrcStepResult OtosuMenu_RankBoard_Step_Begin(PrcCtx* ctx, void* unused) {
    PrcCtx_AdvanceStep(ctx);
    return PRC_STEP_REPEAT;
}

static PrcStepResult OtosuMenu_RankBoard_Step_Continue(PrcCtx* ctx, void* unused) {
    return PRC_STEP_CONTINUE;
}
