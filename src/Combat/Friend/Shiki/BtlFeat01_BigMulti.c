#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "SndMgr.h"

extern BinIdentifier data_ov003_020dd78c;

typedef enum {
    SIDE_LEFT,
    SIDE_RIGHT
} ResSide;

typedef struct BtlFeat01_BigMulti {
    /* 0x00 */ s32 (**unk_00)(struct BtlFeat01_BigMulti*);
    /* 0x04 */ ResSide      side;
    /* 0x08 */ CombatSprite mainSprite;    // Primary sprite
    /* 0x68 */ s32          posX;          // Horizontal position
    /* 0x6C */ s32          posY;          // Vertical position
    /* 0x70 */ BOOL         movePositive;  // Whether acceleration should be positive or negative
    /* 0x74 */ CombatSprite overlaySprite; // Secondary sprite for effects
    /* 0xD4 */ s32          ovlPosX;       // Mirror of main sprite X position for overlaySprite
    /* 0xD8 */ s32          ovlPosY;       // Mirror of main sprite Y position for overlaySprite
    /* 0xDC */ BOOL         ovlFlip;       // Whether overlaySprite should be flipped
    /* 0xE0 */ u16          stepTimer;     // Countdown timer in movement steps
    /* 0xE2 */ s16          stepIndex;     // Index in step function table
    /* 0xE4 */ char         unk_E4[0xEC - 0xE4];
    /* 0xEC */ s32          velocity;      // Per-frame X delta magnitude
    /* 0xF0 */ s32          accel;         // Per-frame X delta acceleration, added to velocity each frame
    /* 0xF4 */ char         unk_F4[0xF8 - 0xF4];
    /* 0xF8 */ s16          unk_F8;
    /* 0xFA */ s16          unk_FA;
    /* 0xFC */ s16          unk_FC;
} BtlFeat01_BigMulti; // Size: 0x100

typedef struct {
    /* 0x0 */ ResSide side;
} BtlFeat01_BigMulti_Args;

s32 BtlFeat01_BigMulti_Step_SetInwardAccel(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_Step_MoveInward(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_Step_SetTimer(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_Step_MoveOnTimer(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_Step_SetOutwardAccel(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_Step_MoveOutward(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_Step_EndTask(BtlFeat01_BigMulti* bigMulti);
s32 BtlFeat01_BigMulti_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static s32 (*data_ov004_020f081c[6])(BtlFeat01_BigMulti*) = {
    BtlFeat01_BigMulti_Step_SetInwardAccel, BtlFeat01_BigMulti_Step_MoveInward,  BtlFeat01_BigMulti_Step_SetTimer,
    BtlFeat01_BigMulti_Step_MoveOnTimer,    BtlFeat01_BigMulti_Step_MoveOutward, BtlFeat01_BigMulti_Step_EndTask,
};

static s32 (*data_ov004_020f0834[7])(BtlFeat01_BigMulti*) = {
    BtlFeat01_BigMulti_Step_SetInwardAccel, BtlFeat01_BigMulti_Step_MoveInward,      BtlFeat01_BigMulti_Step_SetTimer,
    BtlFeat01_BigMulti_Step_MoveOnTimer,    BtlFeat01_BigMulti_Step_SetOutwardAccel, BtlFeat01_BigMulti_Step_MoveOutward,
    BtlFeat01_BigMulti_Step_EndTask,
};

static const SpriteAnimEntry data_ov004_020f0504[3] = {
    {  5,   7,   6, 0},
    {  8, 0xA,   9, 0},
    {0xB, 0xD, 0xC, 3}
};

static const TaskHandle Tsk_BtlFeat01_BigMulti = {"Tsk_BtlFeat01_BigMulti", BtlFeat01_BigMulti_RunTask, 0x100};

void BtlFeat01_BigMulti_NextStep(BtlFeat01_BigMulti* bigMulti) {
    bigMulti->stepIndex++;
    bigMulti->unk_F8 = 0;
    bigMulti->unk_FA = 0;
    bigMulti->unk_FC = 0;
}

void BtlFeat01_BigMulti_SetStepTable(BtlFeat01_BigMulti* bigMulti, s32 (*cbTable[])(BtlFeat01_BigMulti*)) {
    bigMulti->unk_00 = cbTable;
}

s32 BtlFeat01_BigMulti_Step_EndTask(BtlFeat01_BigMulti* bigMulti) {
    return 1;
}

s32 BtlFeat01_BigMulti_Step_SetInwardAccel(BtlFeat01_BigMulti* bigMulti) {
    bigMulti->stepTimer = 35;
    bigMulti->velocity  = 0x1800;
    bigMulti->accel     = 0x666;
    BtlFeat01_BigMulti_NextStep(bigMulti);
    return 2;
}

s32 BtlFeat01_BigMulti_Step_MoveInward(BtlFeat01_BigMulti* bigMulti) {
    if (bigMulti->movePositive) {
        bigMulti->posX += bigMulti->velocity;
        if (bigMulti->posX > 0x50000) {
            BtlFeat01_BigMulti_NextStep(bigMulti);
        }
    } else {
        bigMulti->posX -= bigMulti->velocity;
        if (bigMulti->posX < 0xB0000) {
            BtlFeat01_BigMulti_NextStep(bigMulti);
        }
    }
    bigMulti->velocity += bigMulti->accel;
    bigMulti->stepTimer--;
    return 0;
}

s32 BtlFeat01_BigMulti_Step_SetTimer(BtlFeat01_BigMulti* bigMulti) {
    bigMulti->velocity  = 0x1333;
    bigMulti->stepTimer = 30;
    BtlFeat01_BigMulti_NextStep(bigMulti);
    return 2;
}

s32 BtlFeat01_BigMulti_Step_MoveOnTimer(BtlFeat01_BigMulti* bigMulti) {
    if (bigMulti->stepTimer != 0) {
        bigMulti->stepTimer--;
    } else {
        BtlFeat01_BigMulti_NextStep(bigMulti);
    }

    if (bigMulti->movePositive) {
        bigMulti->posX += bigMulti->velocity;
    } else {
        bigMulti->posX -= bigMulti->velocity;
    }

    return 0;
}

s32 BtlFeat01_BigMulti_Step_SetOutwardAccel(BtlFeat01_BigMulti* bigMulti) {
    bigMulti->velocity = 0x2000;
    bigMulti->accel    = 0x666;
    BtlFeat01_BigMulti_NextStep(bigMulti);
    return 2;
}

s32 BtlFeat01_BigMulti_Step_MoveOutward(BtlFeat01_BigMulti* bigMulti) {
    if (bigMulti->movePositive) {
        bigMulti->posX += bigMulti->velocity;
        if (bigMulti->posX > 0x11E000) {
            BtlFeat01_BigMulti_NextStep(bigMulti);
        }
    } else {
        bigMulti->posX -= bigMulti->velocity;
        if (bigMulti->posX < -0x1E000) {
            BtlFeat01_BigMulti_NextStep(bigMulti);
        }
    }
    bigMulti->velocity += bigMulti->accel;
    bigMulti->stepTimer--;
    return 0;
}

s32 BtlFeat01_BigMulti_Init(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_BigMulti*      bigMulti     = task->data;
    BtlFeat01_BigMulti_Args* bigMultiArgs = args;

    MI_CpuSet(bigMulti, 0, sizeof(BtlFeat01_BigMulti));

    bigMulti->side = bigMultiArgs->side;
    if (bigMulti->side == SIDE_RIGHT) {
        CombatSprite_LoadDirect(bigMulti->side, &bigMulti->overlaySprite, &data_ov003_020dd78c, 11, 13, 12, 3, 3);
        CombatSprite_SetAnim(&bigMulti->overlaySprite, 3, 0);
    } else {
        CombatSprite_LoadDirect(bigMulti->side, &bigMulti->overlaySprite, &data_ov003_020dd78c, 11, 13, 12, 4, 3);
        CombatSprite_SetAnim(&bigMulti->overlaySprite, 3, 0);
    }

    if (bigMulti->side == SIDE_LEFT) {
        CombatSprite_LoadFromTable(bigMulti->side, &bigMulti->mainSprite, &data_ov003_020dd78c, data_ov004_020f0504, 0, 1,
                                   0x50);
        bigMulti->posX         = -0x1E000;
        bigMulti->posY         = 0x64000;
        bigMulti->movePositive = TRUE;
        bigMulti->ovlFlip      = TRUE;
        CombatSprite_SetAnimFromTable(&bigMulti->mainSprite, 0, 0);
        CombatSprite_SetFlip(&bigMulti->mainSprite, bigMulti->movePositive);
    } else {
        CombatSprite_LoadFromTable(bigMulti->side, &bigMulti->mainSprite, &data_ov003_020dd78c, data_ov004_020f0504, 1, 2,
                                   0x50);
        bigMulti->posX         = 0x11E000;
        bigMulti->posY         = 0x64000;
        bigMulti->movePositive = FALSE;
        bigMulti->ovlFlip      = FALSE;
        CombatSprite_SetAnimFromTable(&bigMulti->mainSprite, 1, 0);
        CombatSprite_SetFlip(&bigMulti->mainSprite, bigMulti->movePositive);
    }

    CombatSprite_SetFlip(&bigMulti->overlaySprite, bigMulti->ovlFlip);
    func_ov003_02082738(&bigMulti->mainSprite, 0);
    func_ov003_02082738(&bigMulti->overlaySprite, 0);
    if (bigMulti->side == SIDE_LEFT) {
        BtlFeat01_BigMulti_SetStepTable(bigMulti, data_ov004_020f081c);
    } else {
        BtlFeat01_BigMulti_SetStepTable(bigMulti, data_ov004_020f0834);
    }
    return 1;
}

s32 BtlFeat01_BigMulti_Update(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_BigMulti* bigMulti = task->data;

    if (bigMulti->unk_00[bigMulti->stepIndex] == NULL) {
        return 0;
    }

    s32 temp_r0;
    do {
        temp_r0 = bigMulti->unk_00[bigMulti->stepIndex](bigMulti);
    } while (temp_r0 == 2);

    if (temp_r0 == 1) {
        return 0;
    }

    CombatSprite_Update(&bigMulti->mainSprite);
    CombatSprite_Update(&bigMulti->overlaySprite);
    return 1;
}

s32 BtlFeat01_BigMulti_Render(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_BigMulti* bigMulti = task->data;

    CombatSprite_SetPosition(&bigMulti->mainSprite, F2Ix(bigMulti->posX), F2Ix(bigMulti->posY));
    func_ov003_02082730(&bigMulti->mainSprite, 0x7FFFFFFF - bigMulti->posY);
    CombatSprite_Render(&bigMulti->mainSprite);

    bigMulti->ovlPosX = bigMulti->posX;
    bigMulti->ovlPosY = bigMulti->posY;

    CombatSprite_SetPosition(&bigMulti->overlaySprite, F2Ix(bigMulti->ovlPosX), F2Ix(bigMulti->ovlPosY));
    func_ov003_02082730(&bigMulti->overlaySprite, 0x7FFFFFFF - bigMulti->ovlPosY);
    CombatSprite_Render(&bigMulti->overlaySprite);
    return 1;
}

s32 BtlFeat01_BigMulti_Destroy(TaskPool* pool, Task* task, void* args) {
    BtlFeat01_BigMulti* bigMulti = task->data;

    CombatSprite_Release(&bigMulti->mainSprite);
    CombatSprite_Release(&bigMulti->overlaySprite);
    return 0;
}

s32 BtlFeat01_BigMulti_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages data_ov004_020f04f4 = {
        .initialize = BtlFeat01_BigMulti_Init,
        .update     = BtlFeat01_BigMulti_Update,
        .render     = BtlFeat01_BigMulti_Render,
        .cleanup    = BtlFeat01_BigMulti_Destroy,
    };
    return data_ov004_020f04f4.iter[stage](pool, task, args);
}

void BtlFeat01_BigMulti_CreateTask(s32 arg0) {
    s32 sp8;

    sp8 = arg0;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &Tsk_BtlFeat01_BigMulti, NULL, 0, NULL, &sp8);
}
