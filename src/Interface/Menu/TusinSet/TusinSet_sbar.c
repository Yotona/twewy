#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/TusinSet.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x00 */ Sprite          sprites[3];
    /* 0xC0 */ char            unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ TusinSetObject* tusinSet;
    /* 0xC8 */ u16             lastScroll;
} TusinSet_sbar; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
} TusinSet_sbar_Args;

static SpriteFrameInfo* TusinSet_sbar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_sbar = {"Tsk_TusinSet_sbar", TusinSet_sbar_RunTask, sizeof(TusinSet_sbar)};

static const SpriteAnimation TusinSet_sbar_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_sbar_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 7,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

#ifdef REGION_USA
    #define SBAR_KNOB_MAX_Y 143
#else
    #define SBAR_KNOB_MAX_Y 144
#endif

#ifdef REGION_USA
// Nonmatching: the tab reload and the shift are scheduled differently
static u16 TusinSet_sbar_PosYToScroll(TusinSetObject* tusinSet, u16 posY) {
    u32 tmp;
    u16 row;

    if (posY < 107) {
        posY = 107;
    }

    tmp = FX_Divide((posY - 107) << 12, 0x24000 / (tusinSet->scrollBarRange[tusinSet->tab] + 1)) * 0x10;
    row = (u32)tmp >> 16;
    if (tusinSet->maxScrollRow[tusinSet->tab] < row) {
        row = tusinSet->maxScrollRow[tusinSet->tab];
    }
    return row * 8;
}

static u16 TusinSet_sbar_ScrollToPosY(TusinSetObject* tusinSet, u16 scroll) {
    u16 posY = (u16)(scroll / 8) * 36 / tusinSet->scrollBarRange[tusinSet->tab] + 107;

    if (posY > 143) {
        posY = 143;
    }
    return posY;
}
#else
// JP's scroll bar always spans 57 rows.
static u16 TusinSet_sbar_PosYToScroll(u16 posY) {
    u16 row;

    if (posY < 107) {
        posY = 107;
    }

    row = (u16)(posY - 107) * 57 / 37;
    if (row > 57) {
        row = 57;
    }
    return row * 8;
}

static u16 TusinSet_sbar_ScrollToPosY(u16 scroll) {
    u16 posY = (u16)(scroll / 8) * 37 / 57 + 107;

    if (posY > 144) {
        posY = 144;
    }
    return posY;
}
#endif

static SpriteFrameInfo* TusinSet_sbar_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void TusinSet_sbar_Load(Sprite* sprites, TusinSet_sbar_Args* args) {
    SpriteAnimation anim = TusinSet_sbar_Anim;

    anim.dataType = args->dataType;

    anim.animIndex = 19;
    anim.posX      = 248;
    anim.posY      = 107;
    _Sprite_Load(&sprites[0], &anim);

    anim.animIndex = 17;
    anim.posX      = 248;
    anim.posY      = 88;
    _Sprite_Load(&sprites[1], &anim);

    anim.animIndex = 18;
    anim.posX      = 248;
    anim.posY      = 150;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 TusinSet_sbar_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar_Args* sbarArgs = args;
    TusinSetObject*     tusinSet = sbarArgs->tusinSet;

    TusinSet_sbar* sbar = task->data;

    sbar->tusinSet   = tusinSet;
    sbar->lastScroll = tusinSet->scroll;
    TusinSet_sbar_Load(sbar->sprites, sbarArgs);
    return 1;
}

static s32 TusinSet_sbar_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar*  sbar     = task->data;
    TusinSetObject* tusinSet = sbar->tusinSet;
    TouchCoord      coord;

    if (tusinSet->helpOpen != 0) {
        return 1;
    }
#ifdef REGION_USA
    if (tusinSet->fitsOnePage[tusinSet->tab] == 1) {
        return 1;
    }
#endif

    if (tusinSet->flags & 4) {
        TouchInput_GetCoord(&coord);
#ifdef REGION_USA
        tusinSet->scroll = TusinSet_sbar_PosYToScroll(tusinSet, coord.y + 7);
#else
        tusinSet->scroll = TusinSet_sbar_PosYToScroll(coord.y + 7);
#endif
        tusinSet->flags &= ~4;
        tusinSet->flags |= 8;
#ifdef REGION_USA
        sbar->sprites[0].posY = TusinSet_sbar_ScrollToPosY(tusinSet, tusinSet->scroll);
#else
        sbar->sprites[0].posY = TusinSet_sbar_ScrollToPosY(tusinSet->scroll);
#endif
    }

    if (tusinSet->flags & 8) {
        s16 targetY;
        s16 step;

        TouchInput_GetCoord(&coord);
        targetY = coord.y + 7;
        step    = (targetY - sbar->sprites[0].posY) >> 2;
        if (step != 0) {
            sbar->sprites[0].posY += step;
        } else {
            sbar->sprites[0].posY = targetY;
        }

        if (sbar->sprites[0].posY < 107) {
            sbar->sprites[0].posY = 107;
        } else if (sbar->sprites[0].posY > SBAR_KNOB_MAX_Y) {
            sbar->sprites[0].posY = SBAR_KNOB_MAX_Y;
        }

#ifdef REGION_USA
        tusinSet->scroll = TusinSet_sbar_PosYToScroll(tusinSet, sbar->sprites[0].posY);
#else
        tusinSet->scroll = TusinSet_sbar_PosYToScroll(sbar->sprites[0].posY);
#endif
        if (TouchInput_WasTouchReleased()) {
            tusinSet->flags &= ~8;
        }
    } else {
        if (TouchInput_WasTouchPressed()) {
            TouchInput_GetCoord(&coord);
            if (TusinSet_IsPointOnSbar(coord.x, coord.y) == 1) {
                if (TusinSet_IsPointOnSbarKnob(coord.x, coord.y, sbar->sprites[0].posX, sbar->sprites[0].posY) == 1) {
                    tusinSet->flags |= 8;
                } else {
                    tusinSet->flags |= 4;
                }
            } else {
                s32 arrow = TusinSet_GetSbarArrowAtPoint(coord.x, coord.y);

                if (arrow == 0) {
                    if (tusinSet->scroll >= 8) {
                        tusinSet->scroll -= 8;
                    }
                } else if (arrow == 1) {
#ifdef REGION_USA
                    if (tusinSet->scroll <= tusinSet->maxScrollRow[tusinSet->tab] * 8 - 8) {
#else
                    if (tusinSet->scroll <= 56 * 8) {
#endif
                        tusinSet->scroll += 8;
                    }
                }
            }
        }
#ifdef REGION_USA
        sbar->sprites[0].posY = TusinSet_sbar_ScrollToPosY(tusinSet, tusinSet->scroll);
#else
        sbar->sprites[0].posY = TusinSet_sbar_ScrollToPosY(tusinSet->scroll);
#endif
    }

    if (sbar->lastScroll != tusinSet->scroll) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }
    sbar->lastScroll = tusinSet->scroll;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_sbar_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar* sbar = task->data;
#ifdef REGION_USA
    TusinSetObject* tusinSet = sbar->tusinSet;

    if (tusinSet->fitsOnePage[tusinSet->tab] == 0) {
        for (s32 i = 0; i < 3; i++) {
            Sprite_RenderFrame(&sbar->sprites[i]);
        }
    }
#else
    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&sbar->sprites[i]);
    }
#endif
    return 1;
}

static s32 TusinSet_sbar_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_sbar_Init,
        .update     = TusinSet_sbar_Update,
        .render     = TusinSet_sbar_Render,
        .cleanup    = TusinSet_sbar_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_sbar_CreateTask(TaskPool* pool, s32 dataType, TusinSetObject* tusinSet) {
    TusinSet_sbar_Args args;

    args.dataType = dataType;
    args.tusinSet = tusinSet;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_sbar, NULL, 0, NULL, &args);
}
