#include "Field/Tasks/Fld_ShinigamiCoin.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

// A drop region of the coin minigame. Holding the coin inside it long enough
// reports one of its three results.
typedef struct {
    /* 0x00 */ s16 unk0; // result when entered from state 6/3
    /* 0x02 */ s16 unk2; // result when entered from state 4
    /* 0x04 */ s16 unk4; // result when left in state 4
    /* 0x06 */ s16 xMin;
    /* 0x08 */ s16 xMax;
    /* 0x0A */ s16 yMin;
    /* 0x0C */ s16 yMax;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 holdTime;
    /* 0x14 */ s8  state;
    /* 0x15 */ u8  pad15[3];
} Ov030CoinRegion; // Size: 0x18

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s16    unk40; // current animation index
    /* 0x42 */ s16    pad42;
    /* 0x44 */ s32    unk44; // active flag
    /* 0x48 */ s32    unk48; // dragging flag
    /* 0x4C */ s32    unk4C; // drag offset X
    /* 0x50 */ s32    unk50; // drag offset Y
    /* 0x54 */ s32    unk54; // last X
    /* 0x58 */ s32    unk58; // last Y
    /* 0x5C */ s32    unk5C; // hold flag
    /* 0x60 */ s32    unk60; // hold counter
    /* 0x64 */ s32    unk64; // tap state
    /* 0x68 */ s32    unk68; // stationary counter
    /* 0x6C */ s16    unk6C; // tap countdown
    /* 0x6E */ s16    unk6E; // suppressed flag
} Ov030CoinSprite;           // Size: 0x70

static s32 Fld_ShinigamiCoin_IsTouched(Sprite*, TouchCoord);
static u32 Fld_ShinigamiCoin_UpdateRegions(Ov030CoinSprite*, Ov030CoinSprite*);

static SpriteFrameInfo* Fld_ShinigamiCoin_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_ShinigamiCoin_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const BinIdentifier Iden_FldShinigami = {30, "Apl_Abe/Grp_FldShinigami.bin"};

static const TaskHandle Tsk_Fld_ShinigamiCoin = {"Tsk_Fld_ShinigamiCoin", Fld_ShinigamiCoin_RunTask, sizeof(Ov030CoinSprite)};

static const SpriteAnimation Fld_ShinigamiCoin_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_ShinigamiCoin_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldShinigami,
    .unk_18            = 2,
    .packIndex         = 2,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 1,
};

static Ov030CoinRegion Fld_ShinigamiCoin_Regions[3] = {
    {0, 4, 7, 120, 136,  52,  68, 0, 0, 6},
    {1, 5, 8,  64,  80, 120, 136, 0, 0, 6},
    {2, 6, 9, 176, 192, 120, 136, 0, 0, 6},
};

static SpriteFrameInfo* Fld_ShinigamiCoin_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

// Nonmatching: SpriteAnimation build; `dataType` is now written last so its store
// follows the posY/posX stores, but retail loads `*arg1`/computes the field before
// them and stores posY before posX; ours defers the load and stores posX first
// (USA 71.97%; JP unvalidated).
static void Fld_ShinigamiCoin_Load(Sprite* sprite, s32* arg1) {
    SpriteAnimation anim = Fld_ShinigamiCoin_Anim;

    anim.posY     = 0x64;
    anim.posX     = 0x78;
    anim.dataType = (u16)*arg1;
    SndMgr_PlaySEWithPan(0xC7, 0x78);
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_ShinigamiCoin_UpdateDrag(Sprite* sprite, Ov030CoinSprite* arg1) {
    TouchCoord coord;

    if (TouchInput_IsTouchActive() != 0) {
        TouchInput_GetCoord(&coord);
        if (arg1->unk48 == 0) {
            if (Fld_ShinigamiCoin_IsTouched(sprite, coord) != 0) {
                arg1->unk4C = sprite->posX - coord.x;
                arg1->unk50 = sprite->posY - coord.y;
                arg1->unk48 = 1;
                SndMgr_PlaySEWithPan(0x131, coord.x);
            }
        } else {
            sprite->posX = coord.x + arg1->unk4C;
            sprite->posY = coord.y + arg1->unk50;
            if (sprite->posX < 0x10) {
                sprite->posX = 0x10;
            }
            if (sprite->posX > 0xF0) {
                sprite->posX = 0xF0;
            }
            if (sprite->posY < 0x10) {
                sprite->posY = 0x10;
            }
            if (sprite->posY > 0xB0) {
                sprite->posY = 0xB0;
            }
        }
    } else {
        if (arg1->unk48 != 0) {
            SndMgr_PlaySEWithPan(0x132, sprite->posX);
        }
        arg1->unk48 = 0;
    }
    return 0;
}

static s32 Fld_ShinigamiCoin_IsTouched(Sprite* sprite, TouchCoord coord) {
    s32 x  = coord.x;
    s32 y  = coord.y;
    s32 px = sprite->posX;
    s32 py = sprite->posY;

    if ((s32)(px - 0xF) < x && x < (s32)(px + 0xF) && (s32)(py - 0xF) < y && y < (s32)(py + 0xF)) {
        return 1;
    }
    return 0;
}

static s32 Fld_ShinigamiCoin_CheckStartMove(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 dx = arg0->sprite.posX - arg1->unk54;
    s32 dy = arg0->sprite.posY - arg1->unk58;
    s32 r  = 0;

    if (dx == 0 && dy == 0 && arg1->unk5C == 1) {
        arg1->unk60++;
        if (arg1->unk60 > 0x78) {
            arg1->unk5C = 0;
            arg1->unk60 = 0;
        }
    }
    if (dx > 0 || dx < 0 || dy > 0 || dy < 0) {
        if (arg1->unk5C == 0) {
            arg1->unk60 = 0;
            arg1->unk5C = 1;
            r           = 1;
        }
    }
    return r;
}

// Nonmatching: region-walk register allocation now matches (declared `i`, `t`, `r` in
// that order); residual rows are the inner `v == 0x258` block, which mwcc if-converts
// to `streq/moveq` instead of retail's `bne` (USA 92.19%; JP unvalidated).
static s32 Fld_ShinigamiCoin_CheckIdle(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 i;
    s8  t;

    for (i = 0; i < 3; i++) {
        t = Fld_ShinigamiCoin_Regions[i].state;
        if (t != 6) {
            break;
        }
    }
    if (arg1->unk5C == 0 && t == 6) {
        s32 v       = arg1->unk68 + 1;
        arg1->unk68 = v;
        if (v == 0x258) {
            arg1->unk68 = 0;
            return 1;
        }
        return 0;
    }
    arg1->unk68 = 0;
    return 0;
}

static s32 Fld_ShinigamiCoin_GetMoveDistSq(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 dx = arg0->sprite.posX - arg1->unk54;
    s32 dy = arg0->sprite.posY - arg1->unk58;

    return dx * dx + dy * dy;
}

static s32 Fld_ShinigamiCoin_CheckFlick(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 r = Fld_ShinigamiCoin_GetMoveDistSq(arg0, arg1);
    s32 t = arg1->unk64;

    if (r > 9) {
        if (t == 0) {
            arg1->unk64 = 1;
            arg1->unk6C = 0x46;
            arg1->unk6E = 1;
            return 1;
        }
    } else if (t > 0xF) {
        arg1->unk64 = 0;
        arg1->unk6C = -1;
    } else if (t != 0) {
        arg1->unk64 = t + 1;
    }
    return 0;
}

static s32 Fld_ShinigamiCoin_TickFlickTimer(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s16 t = arg1->unk6C;

    if (t > 0) {
        arg1->unk6C = t - 1;
    } else if (t == 0) {
        arg1->unk6C = -1;
        return 1;
    }
    return 0;
}

static s32 Fld_ShinigamiCoin_UpdateMoveEvents(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    if (arg1->unk6E == 0) {
        if (Fld_ShinigamiCoin_CheckStartMove(arg0, arg1) != 0) {
            Field_SetTouchEventResult(5);
            Field_SetTouchEventTarget(0);
            SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
        }
        if (Fld_ShinigamiCoin_CheckFlick(arg0, arg1) != 0) {
            Field_SetTouchEventResult(5);
            Field_SetTouchEventTarget(1);
            SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
        }
        if (Fld_ShinigamiCoin_CheckIdle(arg0, arg1) != 0) {
            Field_SetTouchEventResult(5);
            Field_SetTouchEventTarget(2);
            SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
        }
    }
    if (Fld_ShinigamiCoin_TickFlickTimer(arg0, arg1) != 0) {
        Field_SetTouchEventResult(5);
        Field_SetTouchEventTarget(0xC);
        SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
    }
    arg1->unk54 = arg0->sprite.posX;
    arg1->unk58 = arg0->sprite.posY;
    return 1;
}

static s32 Fld_ShinigamiCoin_UpdateRegionEvents(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    u32 r = Fld_ShinigamiCoin_UpdateRegions(arg0, arg1);

    if (arg1->unk6E == 0) {
        switch (r) {
            default:
                break;
            case 2:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(4);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 1:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(3);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 0:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(5);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 6:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(0xA);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                arg1->unk6E = 1;
                break;
            case 5:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(9);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                arg1->unk6E = 1;
                break;
            case 4:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(0xB);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                arg1->unk6E = 1;
                break;
            case 9:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(7);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 8:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(6);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 7:
                Field_SetTouchEventResult(5);
                Field_SetTouchEventTarget(8);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
        }
    }
    return 0;
}

static u32 Fld_ShinigamiCoin_UpdateRegions(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    TouchCoord coord;
    s32        i;

    TouchInput_GetCoord(&coord);
    arg1->unk40 = 3;
    for (i = 0; i < 3; i++) {
        Ov030CoinRegion* r = &Fld_ShinigamiCoin_Regions[i];

        if (arg0->sprite.posX > r->xMin && arg0->sprite.posX < r->xMax && arg0->sprite.posY > r->yMin &&
            arg0->sprite.posY < r->yMax)
        {
            if (r->state == 6 || r->state == 3) {
                r->holdTime++;
                arg1->unk40 = i;
                r->state    = 3;
                if (r->holdTime > 0x64) {
                    Fld_ShinigamiCoin_Regions[i].holdTime = 0;
                    Fld_ShinigamiCoin_Regions[i].state    = 4;
                    return Fld_ShinigamiCoin_Regions[i].unk0;
                }
            } else if (r->state == 4) {
                arg1->unk40 = i;
                r->holdTime++;
                if (r->holdTime > 0x64) {
                    Fld_ShinigamiCoin_Regions[i].holdTime = 0;
                    Fld_ShinigamiCoin_Regions[i].state    = 6;
                    return Fld_ShinigamiCoin_Regions[i].unk2;
                }
            }
        } else {
            r->holdTime = 0;
            if (r->state == 4) {
                Fld_ShinigamiCoin_Regions[i].state = 6;
                return Fld_ShinigamiCoin_Regions[i].unk4;
            }
            r->state = 6;
        }
    }
    return 3;
}

static s32 Fld_ShinigamiCoin_Init(TaskPool* pool, Task* task, void* args) {
    s32*             arg2   = args;
    Ov030CoinSprite* sprite = task->data;
    s32              i;

    sprite->unk40 = 3;
    sprite->unk48 = 0;
    sprite->unk60 = 0;
    sprite->unk44 = 0;
    sprite->unk5C = 0;
    sprite->unk54 = 0x78;
    sprite->unk58 = 0x64;
    sprite->unk64 = 0;
    sprite->unk68 = 0;
    sprite->unk6C = -1;
    sprite->unk6E = 0;
    for (i = 0; i < 3; i++) {
        Fld_ShinigamiCoin_Regions[i].holdTime = 0;
        Fld_ShinigamiCoin_Regions[i].state    = 6;
    }
    Fld_ShinigamiCoin_Load(&sprite->sprite, arg2);
}

static s32 Fld_ShinigamiCoin_Update(TaskPool* pool, Task* task, void* args) {
    Ov030CoinSprite* sprite = task->data;

    if (sprite->unk44 != 0) {
        Fld_ShinigamiCoin_UpdateDrag(&sprite->sprite, sprite);
        Fld_ShinigamiCoin_UpdateMoveEvents(sprite, sprite);
        Fld_ShinigamiCoin_UpdateRegionEvents(sprite, sprite);
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

static s32 Fld_ShinigamiCoin_Render(TaskPool* pool, Task* task, void* args) {
    Sprite_RenderFrame(task->data);
    return 1;
}

static s32 Fld_ShinigamiCoin_Destroy(TaskPool* pool, Task* task, void* args) {
    Sprite_Release(task->data);
    return 1;
}

static s32 Fld_ShinigamiCoin_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_ShinigamiCoin_Init,
        .update     = Fld_ShinigamiCoin_Update,
        .render     = Fld_ShinigamiCoin_Render,
        .cleanup    = Fld_ShinigamiCoin_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_ShinigamiCoin_CreateTask(TaskPool* pool, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(pool, &Tsk_Fld_ShinigamiCoin, NULL, 0, NULL, &param);
}

void Fld_ShinigamiCoin_Reset(Task* task, s32 arg1) {
    Ov030CoinSprite* sprite = task->data;
    s32              i;

    sprite->unk48 = 0;
    sprite->unk5C = 0;
    sprite->unk60 = 0;
    sprite->unk64 = 0;
    sprite->unk68 = 0;
    for (i = 0; i < 3; i++) {
        Fld_ShinigamiCoin_Regions[i].holdTime = 0;
    }
    sprite->unk44 = arg1;
}

s16 Fld_ShinigamiCoin_GetOnAnim(Task* task) {
    Ov030CoinSprite* sprite = task->data;
    s16              t      = sprite->unk40;
    s16              r      = t + 1;

    if (t == 3) {
        r = 0;
    }
    return r;
}

void Fld_ShinigamiCoin_Suppress(Task* task) {
    ((Ov030CoinSprite*)task->data)->unk6E = 1;
}

s32 Fld_ShinigamiCoin_IsSuppressed(Task* task) {
    if (((Ov030CoinSprite*)task->data)->unk6E != 0) {
        return 1;
    }
    return 0;
}
