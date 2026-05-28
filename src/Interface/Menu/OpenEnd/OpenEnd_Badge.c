#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/OpenEnd.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite badge;
    /* 0x40 */ Sprite badgeShadow;
    /* 0x80 */ u32    badgeIndex;
} OpenEnd_Badge; // Size: 0x84

s32 OpenEnd_TaskBadge_RunTask(struct TaskPool* pool, struct Task* task, void* taskParam, s32 index);

static const SpriteAnimation OpenEnd_Badge_Anim;

static const TaskHandle TaskHandle_OpenEnd_Badge = {"Tsk_OpenEnd_Badge", OpenEnd_TaskBadge_RunTask, 0x84};

void OpenEnd_OnButtonSelect(u32 r0) {
    if (g_OpenEndstate->alreadySelected == 0) {
        g_OpenEndstate->selectedOption  = r0;
        g_OpenEndstate->alreadySelected = 1;
        SndMgr_StartPlayingSE(SEIDX_SE_PAUSE);
    }
}

BOOL OpenEnd_IsInCircle(s32* coords, s32 x, s32 y) {
    s32 xx   = (x - coords[0]) * (x - coords[0]);
    s32 yy   = (y - coords[1]) * (y - coords[1]);
    s32 dSqr = xx + yy;
    s32 maxD = coords[2] * coords[2];
    return dSqr <= maxD;
}

void OpenEnd_InitBadgeAnim(SpriteAnimation* anim, s16 r1, s16 r2, s16 r3, s16 s1) {
    *anim         = OpenEnd_Badge_Anim;
    anim->binIden = OpenEnd_FileList;
    anim->unk_1C  = r1;
    anim->unk_26  = r2;
    anim->unk_28  = r3;
    anim->unk_2A  = s1;
}

s32 OpenEnd_TaskBadge_Init(struct TaskPool* unused_r0, struct Task* r1, void* taskParam) {
    SpriteAnimation anim;
    u32*            r2 = (u32*)taskParam;

    OpenEnd_Badge* badgeData = r1->data;
    MI_CpuSet(badgeData, 0, sizeof(OpenEnd_Badge));

    u32 badgeIndex        = *r2;
    badgeData->badgeIndex = badgeIndex;
    OpenEnd_InitBadgeAnim(&anim, (badgeIndex * 3) + 2, (badgeIndex * 3) + 3, (badgeIndex * 3) + 4, 1);

    Sprite_Load(&badgeData->badgeShadow, &anim);

    anim.unk_2A = 2;
    Sprite_Load(&badgeData->badge, &anim);

    return 1;
}

const u32 OpenEnd_TitleScreen_BadgeInfo[][5] = {
    {0x22, 0x1F, 0x19, 0x00, 0x00},
    {0xD7, 0x1F, 0x19, 0x01, 0x01}
};

// Nonmatching
s32 OpenEnd_TaskBadge_Update(TaskPool* pool, Task* task, void* args) {
    OpenEnd_Badge* badgeData = task->data;

    TouchCoord coords, coords2;
    TouchInput_GetCoord(&coords2);

    coords = coords2;

    if (flag_screenTouched != 0 && TouchInput_IsTouchActive() != FALSE &&
        OpenEnd_IsInCircle(&OpenEnd_TitleScreen_BadgeInfo[badgeData->badgeIndex][0], coords.x, coords.y) != FALSE)
    {
        badgeData->badge.posX = 130;
        badgeData->badge.posY = 98;
    } else {
        badgeData->badge.posX = 128;
        badgeData->badge.posY = 96;
    }

    if (flag_screenTouched != 0 && TouchInput_WasTouchReleased() != FALSE) {
        if (OpenEnd_IsInCircle(&OpenEnd_TitleScreen_BadgeInfo[badgeData->badgeIndex][0], coords.x, coords.y) != FALSE) {
            OpenEnd_OnButtonSelect(OpenEnd_TitleScreen_BadgeInfo[badgeData->badgeIndex][1]);
        }
    }
    Sprite_Update(&badgeData->badge);
    Sprite_Update(&badgeData->badgeShadow);
    return 1;
}

s32 OpenEnd_TaskBadge_Render(TaskPool* pool, Task* task, void* args) {
    OpenEnd_Badge* badgeData = task->data;

    Sprite_RenderFrame(&badgeData->badgeShadow);
    Sprite_RenderFrame(&badgeData->badge);
    return 1;
}

s32 OpenEnd_TaskBadge_CleanUp(TaskPool* pool, Task* task, void* args) {
    OpenEnd_Badge* badgeData = task->data;

    Sprite_Release(&badgeData->badge);
    Sprite_Release(&badgeData->badgeShadow);
    return 1;
}

s32 OpenEnd_TaskBadge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = OpenEnd_TaskBadge_Init,
        .update     = OpenEnd_TaskBadge_Update,
        .render     = OpenEnd_TaskBadge_Render,
        .cleanup    = OpenEnd_TaskBadge_CleanUp,
    };
    return stages.iter[stage](pool, task, args);
}

s32 OpenEnd_CreateBadgeTask(u32 idx) {
    u32 badge = idx;
    return EasyTask_CreateTask(g_taskPool, &TaskHandle_OpenEnd_Badge, NULL, 0, 0, &badge);
}