#include "Engine/EasyTask.h"
#include "Otosu/OtosuGame.h"

typedef struct {
    s32              score;
    OtosuGame_badge* task;
} OtuPick; // Size: 0x8

OtosuGame_badge* OtuBoard_FindNearestTargetable(TaskPool* pool, TinPinSlammer_Scene* scene, s32 which) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    OtuPick        picks[3];
    Vec2p          selfPos;

    OtosuGame_badge_GetPos(EasyTask_GetTaskData(pool, stage->badgeIds[which]), &selfPos);
    s32 i;
    s32 found;
    for (i = 0, found = 0; i < stage->badgeCount; i++) {
        if (i != which) {
            OtosuGame_badge* cand = EasyTask_GetTaskData(pool, stage->badgeIds[i]);
            if (OtosuGame_badge_CanBeTargeted(cand, 0x444)) {
                Vec2p candPos;
                OtosuGame_badge_GetPos(cand, &candPos);
                picks[found].score = OtuVec_Distance(&selfPos, &candPos);
                picks[found].task  = cand;
                found++;
            }
        }
    }

    if (found <= 0) {
        return 0;
    }
    if (found <= 1) {
        return picks[0].task;
    }
    s32 best;
    for (i = 1, best = 0; i < found; i++) {
        if (picks[best].score > picks[i].score) {
            best = i;
        }
    }

    return picks[best].task;
}

OtosuGame_badge* OtuBoard_FindNearestStunned(TaskPool* pool, TinPinSlammer_Scene* scene, s32 which) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    OtuPick        picks[3];
    Vec2p          selfPos;
    Vec2p          candPos;

    OtosuGame_badge_GetPos(EasyTask_GetTaskData(pool, stage->badgeIds[which]), &selfPos);

    s32 i;
    s32 found;
    for (i = 0, found = 0; i < stage->badgeCount; i++) {
        if (i != which) {
            OtosuGame_badge* cand = EasyTask_GetTaskData(pool, stage->badgeIds[i]);

            if (OtosuGame_badge_IsStunned(cand)) {
                OtosuGame_badge_GetPos(cand, &candPos);

                picks[found].score = OtuVec_Distance(&selfPos, &candPos);
                picks[found].task  = cand;
                found++;
            }
        }
    }

    if (found <= 0) {
        return NULL;
    }

    if (found <= 1) {
        return picks[0].task;
    }

    s32 best;
    for (i = 1, best = 0; i < found; i++) {
        if (picks[best].score > picks[i].score) {
            best = i;
        }
    }

    return picks[best].task;
}

OtosuGame_badge* OtuBoard_FindNearestNeedle(TaskPool* pool, TinPinSlammer_Scene* scene, s32 which) {
    OtuBoardStage* stage = (OtuBoardStage*)OtuStageDispatch_GetBlock(&scene->stage);
    OtuPick        picks[3];
    Vec2p          selfPos;
    Vec2p          candPos;

    OtosuGame_badge_GetPos(EasyTask_GetTaskData(pool, stage->badgeIds[which]), &selfPos);

    s32 i;
    s32 found;
    for (i = 0, found = 0; i < stage->badgeCount; i++) {
        if (i != which) {
            OtosuGame_badge* cand = EasyTask_GetTaskData(pool, stage->badgeIds[i]);

            if (OtosuGame_badge_IsUsingNeedle(cand)) {
                OtosuGame_badge_GetPos(cand, &candPos);

                picks[found].score = OtuVec_Distance(&selfPos, &candPos);
                picks[found].task  = cand;
                found++;
            }
        }
    }

    if (found <= 0) {
        return NULL;
    }

    if (found <= 1) {
        return picks[0].task;
    }

    s32 best;
    for (i = 1, best = 0; i < found; i++) {
        if (picks[best].score > picks[i].score) {
            best = i;
        }
    }

    return picks[best].task;
}

OtosuGame_badge* OtuBoard_FindNearestHammer(TaskPool* pool, TinPinSlammer_Scene* scene, s32 which) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    OtuPick        picks[3];
    Vec2p          selfPos;
    Vec2p          candPos;

    OtosuGame_badge_GetPos(EasyTask_GetTaskData(pool, stage->badgeIds[which]), &selfPos);

    s32 i;
    s32 found;
    for (i = 0, found = 0; i < stage->badgeCount; i++) {

        if (i != which) {
            OtosuGame_badge* cand = EasyTask_GetTaskData(pool, stage->badgeIds[i]);

            if (OtosuGame_badge_IsUsingHammer(cand)) {
                OtosuGame_badge_GetPos(cand, &candPos);

                picks[found].score = OtuVec_Distance(&selfPos, &candPos);
                picks[found].task  = cand;
                found++;
            }
        }
    }

    if (found <= 0) {
        return NULL;
    }

    if (found <= 1) {
        return picks[0].task;
    }

    s32 best;
    for (i = 1, best = 0; i < found; i++) {
        if (picks[best].score > picks[i].score) {
            best = i;
        }
    }

    return picks[best].task;
}

OtosuGame_badge* OtuBoard_FindNearestMeteo(TaskPool* pool, TinPinSlammer_Scene* scene, s32 which) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    OtuPick        picks[3];
    Vec2p          selfPos;
    Vec2p          candPos;

    OtosuGame_badge_GetPos(EasyTask_GetTaskData(pool, stage->badgeIds[which]), &selfPos);

    s32 i;
    s32 found;
    for (i = 0, found = 0; i < stage->badgeCount; i++) {
        if (i != which) {
            OtosuGame_badge* cand = EasyTask_GetTaskData(pool, stage->badgeIds[i]);

            if (OtosuGame_badge_IsUsingMeteo(cand)) {
                OtosuGame_badge_GetPos(cand, &candPos);

                picks[found].score = OtuVec_Distance(&selfPos, &candPos);
                picks[found].task  = cand;
                found++;
            }
        }
    }

    if (found <= 0) {
        return NULL;
    }

    if (found <= 1) {
        return picks[0].task;
    }

    s32 best;
    for (i = 1, best = 0; i < found; i++) {
        if (picks[best].score > picks[i].score) {
            best = i;
        }
    }

    return picks[best].task;
}
