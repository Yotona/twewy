#include "Field/Tasks/Fld_Partner.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/Battle/FieldBattleSetup.h"
#include "Field/Battle/FieldBattleTransition.h"
#include "Field/Battle/FieldEncounter.h"
#include "Field/Event/EventFlags.h"
#include "Field/FieldTypes.h"
#include "Field/Tasks/FldChara.h"
#include "Field/Tasks/FldCharaWalk.h"
#include "Field/Tasks/Fld_BattleIn.h"
#include "Field/Tasks/Fld_Shinigami.h"
#include "Field/Tasks/Fld_TalkIcon.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

extern s32 func_0202599c(s32 flagIndex);

typedef struct {
    /* 0x0 */ const FieldPos* center;
    /* 0x4 */ const FieldPos* startPos;
} FldPartnerArgs;

typedef struct FldPartner FldPartner;
typedef void (*FldPartnerCallback)(TaskPool*, FldPartner*);

struct FldPartner {
    /* 0x00 */ FldCharaSprite     chara;
    /* 0x50 */ s32                unk_50;
    /* 0x54 */ char               unk_54[0x8];
    /* 0x5C */ s16                unk_5C;
    /* 0x5E */ s16                unk_5E;
    /* 0x60 */ FldPartnerCallback callback;
    /* 0x64 */ const FieldPos*    center;
    /* 0x68 */ FieldPos           pos;
    /* 0x70 */ FieldPos           target;
    /* 0x78 */ FieldPos           prev;
    /* 0x80 */ FieldPos           home;
    /* 0x88 */ s32                unk_88;
    /* 0x8C */ s32                unk_8C;
    /* 0x90 */ s32                unk_90;
    /* 0x94 */ s32                unk_94;
    /* 0x98 */ s32                unk_98;
    /* 0x9C */ s16                unk_9C;
    /* 0x9E */ s16                unk_9E;
    /* 0xA0 */ s32                unk_A0;
    /* 0xA4 */ char               unk_A4[0xC];
    /* 0xB0 */ s32                taskId_0;
    /* 0xB4 */ FieldPos           delta;
    /* 0xBC */ s32                taskId_1;
    /* 0xC0 */ s16                unk_C0;
    /* 0xC2 */ char               unk_C2[2];
    /* 0xC4 */ s32                unk_C4;
    /* 0xC8 */ s32                unk_C8;
    /* 0xCC */ u8                 unk_CC;
    /* 0xCD */ char               unk_CD[3];
}; // Size: 0xD0

static void Fld_Partner_StateIdle(TaskPool* pool, FldPartner* obj);
static void Fld_Partner_StateWalk(TaskPool* pool, FldPartner* obj);
static s32  Fld_Partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_Partner = {"Tsk_Fld_Partner", Fld_Partner_RunTask, 0xD0};

static const BinIdentifier Fld_Partner_Packs[3] = {
    {30, "Apl_Kit/GRP_PartnerUGTEX.bin"},
    {30,  "Apl_Kit/GRP_JoshuaUGTEX.bin"},
    {30,    "Apl_Kit/GRP_BeatUGTEX.bin"},
};

static const s32 Fld_Partner_SearchAngles[8] = {0x0000, 0x8000, 0xE000, 0xA000, 0x2000, 0x6000, 0x4000, 0xC000};

static const s32 Fld_Partner_FollowAngles[8] = {0x4000, 0x2000, 0x0000, 0xE000, 0xC000, 0xA000, 0x8000, 0x6000};

static const FldCharaCell Fld_Partner_Cells[14] = {
    {0x01, 0x03, 0x02, 0},
    {0x04, 0x06, 0x05, 0},
    {0x07, 0x09, 0x08, 0},
    {0x0D, 0x0F, 0x0E, 0},
    {0x0A, 0x0C, 0x0B, 0},
    {0x10, 0x12, 0x11, 0},
    {0x13, 0x15, 0x14, 0},
    {0x16, 0x18, 0x17, 0},
    {0x19, 0x1B, 0x1A, 0},
    {0x1C, 0x1E, 0x1D, 0},
    {0x1F, 0x21, 0x20, 0},
    {0x1F, 0x21, 0x20, 1},
    {0x1F, 0x21, 0x20, 2},
    {0x1F, 0x21, 0x20, 3},
};

static void Fld_Partner_SetState(FldPartner* obj, FldPartnerCallback callback) {
    obj->callback = callback;
    obj->unk_5E   = 0;
    obj->unk_5C   = 0;
}

static s32 Fld_Partner_IsPresent(void) {
    if (gSaveData.chapter == 0x15) {
        return 0;
    }
    if (func_0202599c(1) == 0) {
        return 0;
    }
    return func_ov030_020af730() == 0;
}

static void Fld_Partner_PlayFootsteps(s32* counter, s32 pan, FldPartner* obj) {
    if (Fld_Partner_IsPresent() == 0) {
        *counter += 1;
        return;
    }
    if (obj->chara.sprite.bit_11 != 1) {
        return;
    }
    switch (FldCharaWalk_GetLoopFrame(&obj->chara.sprite)) {
        case 1:
            SndMgr_PlaySEWithPan(3, pan);
            return;
        case 5:
            SndMgr_PlaySEWithPan(4, pan);
            return;
    }
}

static void Fld_Partner_AnimateWalk(FldPartner* obj) {
    FldChara_SetCell(&obj->chara, (u16)(s16)(obj->unk_8C + 5), 1);
    if (obj->unk_94 != 0) {
        obj->unk_94 = 0;
        obj->unk_90 = 0;
    }
    Fld_Partner_PlayFootsteps(&obj->unk_90, (obj->pos.x - obj->center->x) >> 0xC, obj);
}

// Nonmatching: register allocation: scaled-shift temps in r1/r3 vs r2/r1 (USA 99.58%; JP unvalidated).
static void Fld_Partner_StateBattleIn(TaskPool* pool, FldPartner* obj) {
    s32 deltaX;
    s32 deltaY;
    s32 flavor;

    if (Fld_Partner_IsPresent() == 0) {
        return;
    }
    if (obj->unk_5C == 0) {
        FldChara_SetCell(&obj->chara, 1, 1);
    }
    if (obj->unk_5C < 5) {
        obj->unk_5C = obj->unk_5C + 1;
    }
    if (obj->unk_5C != 4) {
        return;
    }
    if (obj->unk_CC == 0) {
        flavor = 1;
    } else if (obj->unk_CC == 1) {
        flavor = 2;
    } else {
        flavor = 3;
    }
    deltaY        = obj->delta.y;
    deltaX        = obj->delta.x;
    obj->taskId_1 = Fld_BattleIn_CreateTask(pool, 1, (deltaX * 0x10) >> 0x10, (deltaY * 0x10) >> 0x10,
                                            FldChara_IsFlipped(&obj->chara.sprite), obj->unk_88, flavor);
}

static void Fld_Partner_StateStart(TaskPool* pool, FldPartner* obj) {
    Fld_Partner_SetState(obj, Fld_Partner_StateIdle);
    obj->unk_5C = obj->unk_5C + 1;
}

static s32 Fld_Partner_IsOffScreen(FldPartner* obj) {
    s32 dx = obj->pos.x - obj->center->x;
    s32 dy = obj->pos.y - obj->center->y;

    if (dx < -0x4000 || dx > 0x104000) {
        return 1;
    }
    if (dy < -0x4000 || dy > 0xF0000) {
        return 1;
    }
    return 0;
}

static s32 Fld_Partner_ShouldWarp(FldPartner* obj) {
    return Fld_Partner_IsOffScreen(obj);
}

static s32 Fld_Partner_UpdateTargetTimer(FldPartner* obj) {
    if (!(obj->prev.x == obj->target.x && obj->prev.y == obj->target.y)) {
        if (obj->unk_A0 != 0) {
            obj->unk_A0 = obj->unk_A0 - 1;
        } else {
            return 1;
        }
    } else {
        obj->unk_A0 = 0x10;
    }
    return 0;
}

static s32 Fld_Partner_IsTouched(FldPartner* obj) {
    TouchCoord coord;
    s32        yTop;
    s32        xLeft;
    s32        xRight;
    s32        yBottom;

    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    xLeft   = obj->delta.x;
    yTop    = obj->delta.y;
    xRight  = xLeft + 0x10000;
    yBottom = (yTop - ((obj->unk_88 * 0x30) + 0x18000)) >> 0xC;
    TouchInput_GetCoord(&coord);
    if (coord.x > (xLeft - 0x10000) >> 0xC && coord.x < xRight >> 0xC && yBottom < coord.y && coord.y < yTop >> 0xC) {
        return 1;
    }
    return 0;
}

static void Fld_Partner_StateIdle(TaskPool* pool, FldPartner* obj) {
    s16 state;

    if (obj->unk_A0 == 0 && Field_IsIdle() != 0) {
        Fld_Partner_SetState(obj, Fld_Partner_StateWalk);
        if (obj->taskId_0 == -1) {
            return;
        }
        EasyTask_DeleteTask(pool, obj->taskId_0);
        obj->taskId_0 = -1;
        return;
    }
    if (gSaveData.unk_2498 != 0) {
        return;
    }
    obj->unk_94 = 1;
    state       = obj->unk_5C;
    if (state == 0) {
        FldChara_SetCell(&obj->chara, (u16)(s16)obj->unk_8C, 1);
    } else if (state == 0xF8) {
        if (Field_IsIdle() != 0 && obj->taskId_0 == -1 && func_ov030_020aad40() != 0 && Fld_Partner_IsPresent() != 0) {
            obj->taskId_0 = Fld_TalkIcon_CreateTask(pool, 1, &obj->delta, &obj->unk_88, 0x34000, 1);
        }
        if (obj->unk_CC == 0) {
            FldChara_SetCell(&obj->chara, 0xC, 1);
        } else {
            FldChara_SetCell(&obj->chara, 0xB, 1);
        }
    }
    if (Field_IsIdle() == 0 || func_ov030_020aad40() == 0) {
        if (obj->taskId_0 != -1) {
            EasyTask_DeleteTask(pool, obj->taskId_0);
            obj->taskId_0 = -1;
            obj->unk_5C   = 0;
        }
    }
    if (Field_IsIdle() != 0) {
        obj->unk_5C = obj->unk_5C + 1;
    }
}

static void Fld_Partner_FacePlayer(FldPartner* obj) {
    s32 angle = FX_Atan2Idx(gSaveData.fieldPos.y - obj->pos.y, gSaveData.fieldPos.x - obj->pos.x);

    obj->unk_8C = FldCharaWalk_AngleToCell(angle);
    if ((s32)(u16)(angle - 0x4000) < 0x8000) {
        FldChara_SetFlip(&obj->chara.sprite, 0);
    } else {
        FldChara_SetFlip(&obj->chara.sprite, 1);
    }
}

static void Fld_Partner_StateWalk(TaskPool* pool, FldPartner* obj) {
    s32 len;
    s32 angle;
    s32 dx;
    s32 dy;

    if (FieldScan_IsOpen() != 0) {
        Fld_Partner_SetState(obj, Fld_Partner_StateIdle);
        return;
    }
    if (Field_IsIdle() == 0) {
        Fld_Partner_SetState(obj, Fld_Partner_StateIdle);
        return;
    }
    obj->unk_C4 = 1;
    dx          = obj->target.x - obj->pos.x;
    dy          = obj->target.y - obj->pos.y;
    len         = FX_Sqrt((s32)(((s64)dx * dx + 0x800) >> 0xC) + (s32)(((s64)dy * dy + 0x800) >> 0xC));
    angle       = FX_Atan2Idx(dy, dx);
    if (len >> 0xC >= 9) {
        obj->unk_8C = FldCharaWalk_AngleToCell(angle);
        if ((s32)(u16)(angle - 0x4000) < 0x8000) {
            FldChara_SetFlip(&obj->chara.sprite, 0);
        } else {
            FldChara_SetFlip(&obj->chara.sprite, 1);
        }
        if (FldCharaWalk_MoveWithCollision(&obj->pos, angle, (s32)(((s64)obj->unk_98 * obj->unk_88 + 0x800) >> 0xC)) != 0) {
            Fld_Partner_FacePlayer(obj);
            Fld_Partner_SetState(obj, Fld_Partner_StateIdle);
        }
        Fld_Partner_AnimateWalk(obj);
        return;
    }
    Fld_Partner_FacePlayer(obj);
    Fld_Partner_SetState(obj, Fld_Partner_StateIdle);
}

static void Fld_Partner_GetFollowPos(FieldPos* out) {
    FieldPos tmp = gSaveData.fieldPos;
    s32      i;
    s32      dist;

    i    = (s16)gSaveData.unk_245A;
    dist = gSaveData.unk_2454 * 0x30;
    i += 4;
    if (i >= 8) {
        i -= 8;
    }
    FldChara_MoveAlongAngle(&tmp.x, &tmp.y, Fld_Partner_FollowAngles[i], dist);
    *out = tmp;
}

static void Fld_Partner_FindFreePos(FieldPos* out) {
    FieldPos tmp;
    s32      dist = gSaveData.unk_2454 * 0x30;
    u32      i;

    for (i = 0; i < 8; i++) {
        tmp = gSaveData.fieldPos;
        if (FldCharaWalk_MoveWithCollision(&tmp, Fld_Partner_SearchAngles[i], dist) == 0) {
            *out = tmp;
            return;
        }
    }
    *out = gSaveData.fieldPos;
}

static s32 Fld_Partner_Init(TaskPool* pool, Task* task, void* args) {
    FldPartner*     obj = task->data;
    FldPartnerArgs* a   = args;
    u32             type;
    s32             y;

    MI_CpuSet(obj, 0, sizeof(FldPartner));
    obj->unk_9C   = 0xC;
    obj->unk_9E   = 0x30;
    obj->unk_98   = 0x2CCD;
    obj->center   = a->center;
    obj->home     = gSaveData.fieldPos;
    obj->pos      = *a->startPos;
    y             = obj->pos.y;
    obj->target   = obj->pos;
    obj->prev.x   = obj->target.x;
    obj->prev.y   = y;
    obj->unk_C0   = 8;
    obj->unk_C4   = 0;
    obj->unk_C8   = 0;
    obj->delta.x  = obj->pos.x - obj->center->x;
    obj->delta.y  = obj->pos.y - obj->center->y;
    obj->taskId_0 = -1;
    obj->taskId_1 = -1;
    obj->unk_88   = func_ov030_020aeca4(obj->pos.y);
    Fld_Partner_SetState(obj, Fld_Partner_StateStart);
    type = Field_GetChapterGroup();
    if (type > 2) {
        type = 0;
    }
    obj->unk_CC = (u8)type;
    FldChara_Load(&obj->chara, &Fld_Partner_Packs[type], Fld_Partner_Cells, 0, 0x22, 0x50);
    if ((s16)gSaveData.unk_245A <= 4) {
        FldChara_SetFlip(&obj->chara.sprite, 1);
        obj->unk_8C = (s16)gSaveData.unk_245A;
    } else {
        FldChara_SetFlip(&obj->chara.sprite, 0);
        obj->unk_8C = 8 - (s16)gSaveData.unk_245A;
    }
    Sprite_Update(&obj->chara.sprite);
    return 1;
}

static s32 Fld_Partner_Update(TaskPool* pool, Task* task, void* args) {
    FldPartner* obj = task->data;

    if (obj->home.x == gSaveData.fieldPos.x && obj->home.y == gSaveData.fieldPos.y) {

    } else {
        obj->unk_C8 = 1;
    }
    if (Fld_Partner_ShouldWarp(obj) != 0) {
        FieldPos tmp;

        obj->prev = obj->target;
        Fld_Partner_GetFollowPos(&tmp);
        obj->target = tmp;
        obj->unk_A0 = 0;
    } else if (obj->unk_C8 != 0) {
        FieldPos tmp;

        if (Fld_Partner_UpdateTargetTimer(obj) != 0) {
            obj->prev = obj->target;
        }
        Fld_Partner_GetFollowPos(&tmp);
        obj->target = tmp;
    } else if (obj->unk_C4 == 0) {
        s16 timer = obj->unk_C0;

        if (timer != 0) {
            obj->unk_C0 = timer - 1;
        }
        if (obj->unk_C0 == 0) {
            FieldPos tmp;

            obj->unk_A0 = 0;
            Fld_Partner_FindFreePos(&tmp);
            obj->target = tmp;
        }
    } else {
        obj->unk_A0 = 1;
    }
    if (obj->callback != NULL) {
        obj->callback(pool, obj);
    }
    obj->unk_88 = func_ov030_020aeca4(obj->pos.y);
    if (gSaveData.unk_2498 == 0) {
        Sprite_Update(&obj->chara.sprite);
    }
    return 1;
}

// Nonmatching: delta.y temp lands in r1 (retail r2, forcing a posY reload) and the
// OamMgr_AllocAffineGroup argument setup schedules scale/flag work before the pointer math
// (USA 82.61%; JP unvalidated).
static s32 Fld_Partner_Render(TaskPool* pool, Task* task, void* args) {
    FldPartner* obj = task->data;
    s32         flip;
    s32         flags;

    if (func_ov030_020af730() != 0 && gSaveData.unk_2498 == 0) {
        return 1;
    }
    if (Fld_Partner_IsPresent() == 0) {
        return 1;
    }
    if (obj->taskId_1 != -1 && obj->unk_5C >= 5) {
        return 1;
    }
    obj->delta.x           = obj->pos.x - obj->center->x;
    obj->delta.y           = obj->pos.y - obj->center->y;
    obj->chara.sprite.posX = (s16)(obj->delta.x >> 0xC);
    obj->chara.sprite.posY = (s16)(obj->delta.y >> 0xC);
    if (Fld_Shinigami_IsOccluded(&obj->pos) == 1) {
        FldChara_SetSortKey(&obj->chara, (obj->pos.y + 0x1C3000) - obj->center->y);
    } else {
        FldChara_SetSortKey(&obj->chara, (obj->pos.y + 0x1000) - obj->center->y);
    }
    flip = 0;
    if (FldChara_IsFlipped(&obj->chara.sprite) != 0) {
        flip = 1;
    }
    obj->unk_50 = OamMgr_AllocAffineGroup(&g_OamMgr[obj->chara.sprite.bits_0_1], 0, obj->unk_88, obj->unk_88, flip);
    Field_ApplyVertexColorAt(&obj->pos);
    Sprite_RenderFrame(&obj->chara.sprite);
    flags = 2;
    if (FldChara_IsFlipped(&obj->chara.sprite) != 0) {
        flags |= 1;
    }
    obj->unk_50 = OamMgr_AllocAffineGroup(&g_OamMgr[obj->chara.sprite.bits_0_1], 0, obj->unk_88, obj->unk_88 / 6, flags);
    FldChara_SetSortKey(&obj->chara, 0x1000);
    REG_GFX_FIFO_VERTEX_COLOR = 0x421;
    FldChara_RenderWithAlpha(&obj->chara.sprite, 0xC);
    REG_GFX_FIFO_VERTEX_COLOR = Field_GetLightColor();
    return 1;
}

static s32 Fld_Partner_Destroy(TaskPool* pool, Task* task, void* args) {
    FldPartner* obj = task->data;

    Sprite_Release(&obj->chara.sprite);
    if (obj->taskId_0 != -1) {
        EasyTask_DeleteTask(pool, obj->taskId_0);
    }
    if (obj->taskId_1 != -1) {
        EasyTask_DeleteTask(pool, obj->taskId_1);
    }
    return 1;
}

static s32 Fld_Partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Partner_Init,
        .update     = Fld_Partner_Update,
        .render     = Fld_Partner_Render,
        .cleanup    = Fld_Partner_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 Fld_Partner_CreateTask(TaskPool* pool, const FieldPos* center, const FieldPos* startPos) {
    FldPartnerArgs initArgs;

    initArgs.center   = center;
    initArgs.startPos = startPos;
    return EasyTask_CreateTask(pool, &Tsk_Fld_Partner, NULL, 0, NULL, &initArgs);
}

void Fld_Partner_WarpToPlayer(Task* task) {
    FldPartner* obj = task->data;
    s32         y;

    obj->home   = gSaveData.fieldPos;
    obj->pos    = gSaveData.fieldPos;
    y           = obj->pos.y;
    obj->target = obj->pos;
    obj->prev.x = obj->target.x;
    obj->prev.y = y;
    obj->unk_C0 = 8;
    obj->unk_C4 = 0;
    obj->unk_C8 = 0;
    Fld_Partner_FacePlayer(obj);
    Fld_Partner_SetState(obj, Fld_Partner_StateIdle);
}

void Fld_Partner_SetSpeed(Task* task, s32 value) {
    FldPartner* obj = task->data;

    obj->unk_98 = value;
}

s32 Fld_Partner_IsTalkTouched(Task* task) {
    FldPartner* obj = task->data;

    if (obj->taskId_0 == -1) {
        return 0;
    }
    return Fld_Partner_IsTouched(obj);
}

s32 Fld_Partner_GetPosY(Task* task) {
    FldPartner* obj = task->data;

    return obj->pos.y;
}

void Fld_Partner_StartBattleIn(Task* task) {
    Fld_Partner_SetState(task->data, Fld_Partner_StateBattleIn);
}
