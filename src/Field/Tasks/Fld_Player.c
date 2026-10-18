#include "Field/Tasks/Fld_Player.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Event/EventFlags.h"
#include "Field/FieldTypes.h"
#include "Field/Scene/FieldArea.h"
#include "Field/Tasks/FldChara.h"
#include "Field/Tasks/FldCharaWalk.h"
#include "Field/Tasks/Fld_BattleIn.h"
#include "Field/Tasks/Fld_Shinigami.h"
#include "Field/Tasks/Fld_TalkIcon.h"
#include "Field/Tasks/Fld_Wall.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

extern s32 func_0202599c(s32 flagIndex);

typedef struct Ov030Player Ov030Player;
typedef void (*Ov030PlayerCallback)(TaskPool*, Ov030Player*);

// The field player task object: a Sprite extended with control state, plus a
// second Sprite used for the shadow/aura pass.
struct Ov030Player {
    /* 0x00 */ FldCharaSprite      chara;
    /* 0x50 */ s32                 unk50;
    /* 0x54 */ s32                 unk54;
    /* 0x58 */ u8                  pad58[0x4];
    /* 0x5C */ Sprite              sprite2;
    /* 0x9C */ s16                 unk9C;
    /* 0x9E */ s16                 unk9E;
    /* 0xA0 */ s16                 unkA0;
    /* 0xA2 */ s16                 unkA2;
    /* 0xA4 */ Ov030PlayerCallback unkA4;
    /* 0xA8 */ u16                 unkA8;
    /* 0xAA */ u8                  padAA[0x2];
    /* 0xAC */ s32                 unkAC;
    /* 0xB0 */ s32                 unkB0;
    /* 0xB4 */ u8                  padB4[0x4];
    /* 0xB8 */ FieldPos*           origin; // field camera origin
    /* 0xBC */ FieldPos*           pos;    // map position (MainData::fieldPos)
    /* 0xC0 */ FieldPos            screen; // pos - origin
    /* 0xC8 */ s32                 unkC8;
    /* 0xCC */ u8                  padCC[0x4];
    /* 0xD0 */ s16                 unkD0;
    /* 0xD2 */ u8                  padD2[0x2];
    /* 0xD4 */ s32                 unkD4;
    /* 0xD8 */ s32                 unkD8;
    /* 0xDC */ u8                  padDC[0xC];
    /* 0xE8 */ s32                 unkE8;
    /* 0xEC */ s32                 unkEC;
    /* 0xF0 */ s32                 unkF0;
}; // Size: 0xF4

typedef struct {
    /* 0x0 */ FieldPos* origin;
    /* 0x4 */ FieldPos* pos;
} Ov030PlayerArgs;

static void Fld_Player_StateWalk(TaskPool* pool, Ov030Player* obj);

static SpriteFrameInfo* Fld_Player_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static void             Fld_Player_StateIdle(TaskPool* pool, Ov030Player* arg1);
static s32              Fld_Player_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const BinIdentifier Iden_PlayerUGTex = {30, "Apl_Kit/GRP_PlayerUGTex.bin"};

static const TaskHandle Tsk_Fld_Player = {"Tsk_Fld_Player", Fld_Player_RunTask, sizeof(Ov030Player)};

static const s32 Fld_Player_AngleDirs[9] = {2, 1, 0, 7, 6, 5, 4, 3, 2};

static const SpriteAnimation Fld_Player_MarkerAnim = {
    .bits_0_1          = 0,
    .dataType          = 1,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x0400,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = Fld_Player_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_PlayerUGTex,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 36,
    .unk_1E            = 0,
    .unk_20            = 39,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 37,
    .unk_28            = 38,
    .animIndex         = 1,
};

static const FldCharaCell Fld_Player_Cells[13] = {
    { 1,  3,  2, 0},
    { 1,  3,  2, 1},
    { 1,  3,  2, 2},
    {10, 12, 11, 0},
    {13, 15, 14, 0},
    {16, 18, 17, 0},
    { 7,  9,  8, 0},
    { 4,  6,  5, 0},
    {22, 24, 23, 0},
    {28, 30, 29, 0},
    {25, 27, 26, 0},
    {31, 33, 32, 0},
    {19, 21, 20, 0},
};

static SpriteFrameInfo* Fld_Player_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_Player_LoadTargetMarker(Sprite* sprite) {
    SpriteAnimation anim = Fld_Player_MarkerAnim;

    _Sprite_Load(sprite, &anim);
}

static void Fld_Player_SetState(Ov030Player* arg0, Ov030PlayerCallback arg1) {
    arg0->unkA4 = arg1;
    arg0->unkA2 = 0;
    arg0->unkA0 = 0;
}

static void Fld_Player_ClampCamera(Ov030Player* arg0) {
    s32 lo;
    s32 hi;
    s32 loSh;
    s32 hiSh;

    lo   = (func_ov030_020b7b80() - func_ov030_020b7b00() / 2) + 1;
    loSh = lo << 0xC;
    hi   = (func_ov030_020b7b80() + func_ov030_020b7b00() / 2) - 0x101;
    hiSh = hi << 0xC;
    if (arg0->origin->x < (lo << 0xC)) {
        arg0->origin->x = loSh;
    } else if (arg0->origin->x > hiSh) {
        arg0->origin->x = hiSh;
    }
    if (gSaveData.fieldArea == 0) {
        hi = func_ov030_020b7d04(arg0->origin->x);
        lo = 0xE000;
    } else {
        hi = (func_ov030_020b7b40() + func_ov030_020b7b60() - 0xC1) << 0xC;
        lo = (func_ov030_020b7bf8() + 1) << 0xC;
    }
    if (arg0->origin->y < lo) {
        arg0->origin->y = lo;
        return;
    }
    if (arg0->origin->y > hi) {
        arg0->origin->y = hi;
    }
}

static void Fld_Player_ScrollCamera(Ov030Player* arg0, s32 arg1, s32 arg2) {
    s32       dx;
    FieldPos* p;
    s32       dy;
    s32       halfH;
    s32       t;

    p     = arg0->origin;
    dx    = arg1 - p->x;
    halfH = ((s32)arg0->unk9E / 2) + 0x10;
    dy    = (arg2 - (halfH << 0xC)) - func_ov030_020b7c18() - p->y;
    if (dx < 0) {
        t = -dx;
    } else {
        t = dx;
    }
    if (t < 0x666) {
        dx = 0;
    }
    if (dy < 0) {
        t = -dy;
    } else {
        t = dy;
    }
    if (t < 0x666) {
        dy = 0;
    }
    p->x = p->x + dx;
    p    = arg0->origin;
    p->y = p->y + dy;
    Fld_Player_ClampCamera(arg0);
}

static void Fld_Player_PlayFootsteps(s32 arg0, Sprite* sprite) {
    if (sprite->bit_11 == 1) {
        switch (FldCharaWalk_GetLoopFrame(sprite)) {
            case 1:
                SndMgr_PlaySEWithPan(SEIDX_SE_FOOT01, arg0);
                break;
            case 5:
                SndMgr_PlaySEWithPan(SEIDX_SE_FOOT02, arg0);
                break;
        }
    }
}

static void Fld_Player_AnimateWalk(Ov030Player* arg0) {
    FldChara_SetCell(&arg0->chara, (u16)(s16)(arg0->unkD0 + 8), 1);
    if (arg0->unkD4 != 0) {
        arg0->unkD4 = 0;
    }
    Fld_Player_PlayFootsteps((arg0->pos->x - arg0->origin->x) >> 0xC, &arg0->chara.sprite);
}

static void Fld_Player_StateStart(TaskPool* pool, Ov030Player* arg1) {
    Fld_Player_SetState(arg1, Fld_Player_StateIdle);
    arg1->unkA0 = arg1->unkA0 + 1;
}

// Nonmatching
static void Fld_Player_StateBattleIn(TaskPool* pool, Ov030Player* arg1) {
    if (arg1->unkA0 == 0) {
        FldChara_SetCell(&arg1->chara, 4, 1);
    }
    if (arg1->unkA0 < 5) {
        arg1->unkA0++;
    }
    if (arg1->unkA0 == 4) {
        s32 offsetX, offsetY;
        offsetY     = arg1->screen.y;
        offsetX     = arg1->screen.x;
        arg1->unkF0 = Fld_BattleIn_CreateTask(pool, 1, (offsetX * 0x10) >> 0x10, (offsetY * 0x10) >> 0x10,
                                              FldChara_IsFlipped(&arg1->chara.sprite), arg1->unkC8, 0);
    }
}

static void Fld_Player_StateIdle(TaskPool* pool, Ov030Player* arg1) {
    FieldPos* p;
    s16       state;

    arg1->unkAC = arg1->pos->x;
    arg1->unkB0 = arg1->pos->y;
    p           = arg1->pos;
    Fld_Player_ScrollCamera(arg1, p->x - 0x80000, p->y - 0x60000);
    if (Field_IsMoveInputActive() != 0 && Field_IsIdle() != 0 && func_ov030_020af748() != 0) {
        Fld_Player_SetState(arg1, Fld_Player_StateWalk);
        if (arg1->unkD4 == 0) {
            Fld_Player_PlayFootsteps((arg1->pos->x - arg1->origin->x) >> 0xC, &arg1->chara.sprite);
        }
        if (arg1->unkE8 == -1) {
            return;
        }
        EasyTask_DeleteTask(pool, arg1->unkE8);
        arg1->unkE8 = -1;
        return;
    }
    if (gSaveData.unk_2498 != 0) {
        return;
    }
    arg1->unkD4 = 1;
    state       = arg1->unkA0;
    if (state == 0) {
        FldChara_SetCell(&arg1->chara, (u16)(s16)(arg1->unkD0 + 3), 1);
    } else if (state == 0xF0) {
        FldChara_SetCell(&arg1->chara, 2, 1);
    } else if (state == 0xF8) {
        if (arg1->unkE8 == -1) {
            if (func_ov030_020aad40() != 0 && gSaveData.unk_2488 != 0) {
                if (func_0202599c(1) != 0 && gSaveData.chapter != 0x15) {

                } else {
                    arg1->unkE8 = Fld_TalkIcon_CreateTask(pool, 1, &arg1->screen, &arg1->unkC8, 0x34000, 1);
                }
            }
        }
        FldChara_SetCell(&arg1->chara, 1, 1);
    }
    if (Field_IsIdle() == 0) {
        arg1->unkA0 = 0;
        if (arg1->unkE8 == -1) {
            return;
        }
        EasyTask_DeleteTask(pool, arg1->unkE8);
        arg1->unkE8 = -1;
        return;
    }
    arg1->unkA0 = arg1->unkA0 + 1;
}

s32 Fld_Player_GetDPadAngle(void) {
    s32 v;

    v = SysControl.buttonState.currButtons & 0xF0;
    if (v == 0) {
        if (SysControl.buttonState.currButtons & 1) {
            v |= 0x10;
        }
        if (SysControl.buttonState.currButtons & 2) {
            v |= 0x80;
        }
        if (SysControl.buttonState.currButtons & 0x400) {
            v |= 0x40;
        }
        if (SysControl.buttonState.currButtons & 0x800) {
            v |= 0x20;
        }
    }
    if (v == 0x60) {
        return 0xA000;
    }
    if (v == 0x50) {
        return 0xE000;
    }
    if (v == 0xA0) {
        return 0x6000;
    }
    if (v == 0x90) {
        return 0x2000;
    }
    if (v & 0x40) {
        return 0xC000;
    }
    if (v & 0x80) {
        return 0x4000;
    }
    if (v & 0x20) {
        return 0x8000;
    }
    return 0;
}

static void Fld_Player_SaveFacing(Ov030Player* arg0) {
    gSaveData.unk_245A = arg0->unkD0;
    if (FldChara_IsFlipped(&arg0->chara.sprite) != 0) {
        return;
    }
    gSaveData.unk_245A = 8 - (s16)gSaveData.unk_245A;
    if ((s32)(s16)gSaveData.unk_245A >= 8) {
        gSaveData.unk_245A = 0;
    }
}

static s32 Fld_Player_IsExitBlocked(TaskPool* pool, Ov030Player* obj, s32* out) {
    s32 value = 0;

    if (func_ov030_020b84a4(gSaveData.fieldArea, &value, out) == 0) {
        return value;
    }
    return 0;
}

static void Fld_Player_StateWalk(TaskPool* pool, Ov030Player* arg1) {
    FieldPos   saved;
    s32        exitDir;
    TouchCoord pos;
    TouchCoord coord;
    s32        dir;
    s32        dx;
    s32        dy;
    s32        len;
    s32        ret;

    arg1->unkA8 |= 1;
    if (FieldScan_IsOpen() != 0) {
        arg1->unkA8 &= ~1;
        Fld_Player_SetState(arg1, Fld_Player_StateIdle);
        arg1->unkA0 = 0xF8;
        return;
    }
    if (Field_IsIdle() == 0 || func_ov030_020af748() == 0) {
        arg1->unkA8 &= ~1;
        Fld_Player_ScrollCamera(arg1, arg1->pos->x - 0x80000, arg1->pos->y - 0x60000);
        Fld_Player_PlayFootsteps((arg1->pos->x - arg1->origin->x) >> 0xC, &arg1->chara.sprite);
        return;
    }
    if (SysControl.buttonState.currButtons & 0xCF3) {
        len = 9;
        dir = Fld_Player_GetDPadAngle();
        arg1->unkA8 &= ~1;
        arg1->unkAC = arg1->pos->x;
        arg1->unkB0 = arg1->pos->y;
    } else {
        if (Field_IsMoveTouchActive() != 0) {
            TouchInput_GetCoord(&coord);
            pos         = coord;
            arg1->unkAC = arg1->origin->x + (pos.x << 0xC);
            arg1->unkB0 = arg1->origin->y + (pos.y << 0xC);
        }
        dx  = arg1->unkAC - arg1->pos->x;
        dy  = arg1->unkB0 - arg1->pos->y;
        len = FX_Sqrt((s32)(((s64)dx * dx + 0x800) >> 0xC) + (s32)(((s64)dy * dy + 0x800) >> 0xC)) >> 0xC;
        dir = FX_Atan2Idx(dy, dx);
    }
    if (len >= 9) {
        arg1->unkD0 = FldCharaWalk_AngleToCell(dir);
        if ((s32)(u16)(dir - 0x4000) < 0x8000) {
            FldChara_SetFlip(&arg1->chara.sprite, 0);
        } else {
            FldChara_SetFlip(&arg1->chara.sprite, 1);
        }
        saved = *arg1->pos;
        ret   = Fld_Player_IsExitBlocked(pool, arg1, &exitDir);
        if (FldCharaWalk_MoveWithCollision(arg1->pos, dir, (s32)(((s64)arg1->unkD8 * arg1->unkC8 + 0x800) >> 0xC)) != 0) {
            arg1->unkA8 &= ~1;
            Fld_Player_SetState(arg1, Fld_Player_StateIdle);
        }
        if (ret == 0 && Fld_Player_IsExitBlocked(pool, arg1, &exitDir) != 0) {
            *arg1->pos = saved;
            if (arg1->unkEC == -1) {
                arg1->unkEC = Fld_Wall_CreateTask(pool, 1, arg1->origin, exitDir);
            }
            arg1->unkA8 &= ~1;
            Fld_Player_SetState(arg1, Fld_Player_StateIdle);
        }
        Fld_Player_AnimateWalk(arg1);
        Fld_Player_ScrollCamera(arg1, arg1->pos->x - 0x80000, arg1->pos->y - 0x60000);
        return;
    }
    if (Field_IsMoveInputActive() != 0) {
        Fld_Player_AnimateWalk(arg1);
        return;
    }
    arg1->unkA8 &= ~1;
    Fld_Player_SetState(arg1, Fld_Player_StateIdle);
}

// Nonmatching
static s32 Fld_Player_Init(TaskPool* pool, Task* task, void* args) {
    Ov030PlayerArgs* params = args;
    Ov030Player*     obj    = task->data;
    s32              y;

    MI_CpuSet(obj, 0, sizeof(Ov030Player));
    obj->unk9C         = 0xC;
    obj->unk9E         = 0x30;
    obj->unkD8         = 0x2CCD;
    obj->origin        = params->origin;
    obj->pos           = params->pos;
    obj->unkC8         = func_ov030_020aeca4(obj->pos->y);
    gSaveData.unk_2454 = obj->unkC8;
    obj->unkAC         = obj->pos->x;
    obj->unkB0         = obj->pos->y;
    obj->origin->x     = obj->pos->x - 0x80000;
    y                  = ((s32)obj->unk9E / 2) + 0x10 + func_ov030_020b7c18();
    y                  = (y + 0x60) << 0xC;
    obj->origin->y     = obj->pos->y - y;
    obj->screen.x      = obj->pos->x - obj->origin->x;
    obj->screen.y      = obj->pos->y - obj->origin->y;
    obj->unkE8         = -1;
    obj->unkEC         = -1;
    obj->unkF0         = -1;
    Fld_Player_ClampCamera(obj);
    Fld_Player_SetState(obj, Fld_Player_StateStart);
    FldChara_Load(&obj->chara, &Iden_PlayerUGTex, Fld_Player_Cells, 1, 0x22, 0x50);
    if ((s32)(s16)gSaveData.unk_245A <= 4) {
        FldChara_SetFlip(&obj->chara.sprite, 1);
        obj->unkD0 = (s16)gSaveData.unk_245A;
    } else {
        FldChara_SetFlip(&obj->chara.sprite, 0);
        obj->unkD0 = (s16)(8 - (s16)gSaveData.unk_245A);
    }
    Fld_Player_LoadTargetMarker(&obj->sprite2);
    Sprite_Update(&obj->chara.sprite);
    return 1;
}

static s32 Fld_Player_Update(TaskPool* pool, Task* task, void* args) {
    Ov030Player* obj = task->data;

    if (obj->unkEC != -1) {
        EasyTask_ValidateTaskId(pool, (u32*)&obj->unkEC);
    }
    if (obj->unkA4 != NULL) {
        obj->unkA4(pool, obj);
    }
    obj->unkC8         = func_ov030_020aeca4(obj->pos->y);
    gSaveData.unk_2454 = obj->unkC8;
    if (gSaveData.unk_2498 == 0) {
        Sprite_Update(&obj->chara.sprite);
    }
    if (obj->unkA8 & 1) {
        Sprite_Update(&obj->sprite2);
    }
    Fld_Player_SaveFacing(obj);
    return 1;
}

// Nonmatching
static s32 Fld_Player_Render(TaskPool* pool, Task* task, void* args) {
    Ov030Player* obj = task->data;
    s32          dy;
    s32          flip;
    s32          scale;

    if (func_ov030_020af730() != 0 && gSaveData.unk_2498 == 0) {
        return 1;
    }
    if (obj->unkF0 != -1 && (s32)obj->unkA0 >= 5) {
        return 1;
    }
    obj->screen.x          = obj->pos->x - obj->origin->x;
    dy                     = obj->pos->y - obj->origin->y;
    obj->screen.y          = dy;
    obj->chara.sprite.posX = (s16)(obj->screen.x >> 0xC);
    obj->chara.sprite.posY = (s16)(obj->screen.y >> 0xC);
    if (Fld_Shinigami_IsOccluded(obj->pos) == 1) {
        FldChara_SetSortKey(&obj->chara, obj->screen.y + 0x1C3000);
    } else {
        FldChara_SetSortKey(&obj->chara, obj->screen.y + 0x1000);
    }
    flip = 0;
    if (FldChara_IsFlipped(&obj->chara.sprite) != 0) {
        flip = 1;
    }
    scale      = obj->unkC8;
    obj->unk50 = OamMgr_AllocAffineGroup(&g_OamMgr[obj->chara.sprite.bits_0_1], 0, scale, scale, flip);
    Field_ApplyVertexColorAt(obj->pos);
    Sprite_RenderFrame(&obj->chara.sprite);
    flip       = 2;
    flip       = flip | (FldChara_IsFlipped(&obj->chara.sprite) != 0);
    scale      = obj->unkC8;
    obj->unk50 = OamMgr_AllocAffineGroup(&g_OamMgr[obj->chara.sprite.bits_0_1], 0, scale, scale / 6, flip);
    FldChara_SetSortKey(&obj->chara, 0x1000);
    REG_GFX_FIFO_VERTEX_COLOR = 0x421;
    FldChara_RenderWithAlpha(&obj->chara.sprite, 0xC);
    REG_GFX_FIFO_VERTEX_COLOR = Field_GetLightColor();
    if (obj->unkA8 & 1) {
        dy                = obj->unkAC - obj->origin->x;
        flip              = obj->unkB0 - obj->origin->y;
        obj->sprite2.posX = (s16)(dy >> 0xC);
        obj->sprite2.posY = (s16)(flip >> 0xC);
        Sprite_RenderFrame(&obj->sprite2);
    }
    return 1;
}

static s32 Fld_Player_Destroy(TaskPool* pool, Task* task, void* args) {
    Ov030Player* obj = task->data;

    Sprite_Release(&obj->chara.sprite);
    Sprite_Release(&obj->sprite2);
    if (obj->unkE8 != -1) {
        EasyTask_DeleteTask(pool, obj->unkE8);
    }
    if (obj->unkEC != -1) {
        EasyTask_DeleteTask(pool, obj->unkEC);
    }
    if (obj->unkF0 != -1) {
        EasyTask_DeleteTask(pool, obj->unkF0);
    }
    return 1;
}

static s32 Fld_Player_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Player_Init,
        .update     = Fld_Player_Update,
        .render     = Fld_Player_Render,
        .cleanup    = Fld_Player_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Player_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos* pos) {
    Ov030PlayerArgs params;

    params.origin = origin;
    params.pos    = pos;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Player, NULL, 0, NULL, &params);
}

s32 Fld_Player_IsWalking(Task* task) {
    Ov030Player* obj    = task->data;
    s32          result = 0;

    if (obj->unkA4 == Fld_Player_StateWalk) {
        result = 1;
    }
    return result;
}

void Fld_Player_SetSpeed(Task* task, s32 arg1) {
    ((Ov030Player*)task->data)->unkD8 = arg1;
}

s32 Fld_Player_GetHeight(Task* task) {
    Ov030Player* obj = task->data;

    return obj->unk9E * obj->unkC8;
}

void Fld_Player_StopWalking(Task* task) {
    Ov030Player* obj = task->data;

    obj->unkA8 &= ~1;
    Fld_Player_SetState(obj, Fld_Player_StateIdle);
}

s32 Fld_Player_IsTouched(Task* task) {
    Ov030Player* obj = task->data;
    s32          right;
    s32          half = obj->unk9C;
    s32          left;
    s32          top;
    s32          cx = obj->screen.x;
    s32          cy = obj->screen.y;
    TouchCoord   coord;
    s32          y = Fld_Player_GetHeight(task);

    left  = (cx - (half << 12)) >> 12;
    top   = (cy - (y + 0x18000)) >> 12;
    right = (cx + (half << 12)) >> 12;

    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    TouchInput_GetCoord(&coord);
    if (left < coord.x && coord.x < right && top < coord.y && coord.y < (cy >> 12)) {
        return 1;
    }
    return 0;
}

s32 Fld_Player_IsTalkTouched(Task* task) {
    if (((Ov030Player*)task->data)->unkE8 == -1) {
        return 0;
    }
    return Fld_Player_IsTouched(task);
}

void Fld_Player_StartBattleIn(Task* task) {
    Fld_Player_SetState(task->data, Fld_Player_StateBattleIn);
}

s32 Fld_Player_AngleToDir(s32 arg0) {
    return Fld_Player_AngleDirs[((arg0 >> 12) & 1) + (arg0 >> 13)];
}
