#include "Otosu/OtosuGame.h"

static void OtuScene_SinglePlayerBoard_Play(TinPinSlammer_Scene* scene);

s32 OtuDebris_SpeedMin       = 0x3000;
s32 OtuPhys_Gravity          = 0x133;
s32 OtuDebris_Decel          = 0x133;
s32 OtuPhys_JumpVelocity     = -0x2000;
s32 OtuPhys_HandJumpVelocity = -0x2000;
s32 OtuDebris_LaunchMax      = 0x4000;
s32 OtuDebris_Gravity        = 0x400;
s32 OtuDebris_Bounce         = 0x1000;
s32 OtuPhys_ConveyorSpeed    = 0x8000;
s32 OtuDebris_LaunchMin      = 0x2000;

OtuStageHandler OtuScene_SinglePlayerBoardSteps[1] = {
    OtuScene_SinglePlayerBoard_Play,
};

s32 OtuDebris_SpeedMax = 0x9800;
s32 OtuPhys_WallBounce = 0x800;

OtuStageHandler OtuScene_GuestBoardSteps[6] = {
    OtuScene_GuestBoard_ConnectToHost, OtuScene_GuestBoard_SyncDecks, OtuScene_GuestBoard_SetupBoard,
    OtuScene_GuestBoard_WaitStart,     OtuScene_GuestBoard_Play,      OtuScene_GuestBoard_Disconnect,
};

OtuStageHandler OtuScene_HostBoardSteps[6] = {
    OtuScene_HostBoard_StartHost, OtuScene_HostBoard_SyncDecks, OtuScene_HostBoard_SetupBoard,
    OtuScene_HostBoard_WaitStart, OtuScene_HostBoard_Play,      OtuScene_HostBoard_Disconnect,
};

static s32 OtuBoard_TimeLimits[10] = {140, 160, 160, 180, 160, 160, 160, 160, 140, 300};

static const s32 OtuBoard_ObstaclesSizes[10] = {0x0, 0x0, 0x0, 0x18, 0x10, 0x10, 0x0, 0x0, 0x0, 0x0};
static const s32 OtuBoard_LayoutSizes[10]    = {0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
static const s32 OtuBoard_LayoutOffsets[10]  = {0x0, 0x1398, 0x2730, 0x3AC8, 0x4E78, 0x6258, 0x7600, 0x8998, 0x9D30, 0xB0C8};
static const s32 OtuBoard_CellsOffsets[10]   = {0x10, 0x13A8, 0x2740, 0x3AD8, 0x4E88, 0x6268, 0x7610, 0x89A8, 0x9D40, 0xB0D8};
static const s32 OtuBoard_ObstaclesOffsets[10] = {0x1398, 0x2730, 0x3AC8, 0x4E60, 0x6210,
                                                  0x75F0, 0x8998, 0x9D30, 0xB0C8, 0xC460};
static const s32 OtuBoard_CellsSizes[10]   = {0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388, 0x1388};
static const s32 OtuBoard_WarpsSizes[10]   = {0x0, 0x0, 0x0, 0x0, 0x38, 0x0, 0x0, 0x0, 0x0, 0x0};
static const s32 OtuBoard_WarpsOffsets[10] = {0x1398, 0x2730, 0x3AC8, 0x4E78, 0x6220, 0x7600, 0x8998, 0x9D30, 0xB0C8, 0xC460};

void func_ov040_0209cb98(WMBssDesc* parent, s32 which);

void OtuBoard_OnScanResult(WMBssDesc* bss, TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    if (stage == NULL) {
        return;
    }

    if (bss->gameInfoLength == 0 || bss->gameInfo.userGameInfoLength != 0x70 ||
        (bss->gameInfo.gameNameCount_attribute & 1) == 0 || bss->gameInfo.userGameInfo[0] != stage->packetKind)
    {
        return;
    }

    stage->parent    = *bss;
    stage->gotPacket = 1;
    func_ov040_0209cb98(bss, 1);
}

static void OtuBoard_FollowOwnBadge(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    void*          pin;
    s32            event;
    Vec2p          at;
    Vec2p          mine;
    Vec2p          other;
    s32            me;
    s32            i;

    pin = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[OtuScene_GetOwnPlayer(scene->multiplayer)]);
    OtosuGame_badge_GetCameraPos(pin, &at);
    event = OtosuGame_badge_TakeCameraCut(pin);
    OtosuGame_badge_SetOrigin(pin, &at);
    OtosuGame_badge_SetHomeOffset(pin, at.x, at.y);

    pin = EasyTask_GetTaskData(&scene->pool2, stage->floorId);
    OtosuGame_floor_SetScroll(pin, at.x, at.y);
    OtosuGame_floor_Refresh(pin, event);
    pin = EasyTask_GetTaskData(&scene->pool2, stage->bgId);
    OtosuGame_bg_SetScroll(pin, at.x, at.y);
    OtosuGame_bg_Refresh(pin, event);

    for (i = 0; i < stage->badgeCount; i++) {
        if (i != OtuScene_GetOwnPlayer(scene->multiplayer)) {
            pin = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);
            OtosuGame_badge_SetOrigin(pin, &at);
            OtosuGame_badge_GetCameraPos(pin, &other);
            OtosuGame_badge_SetHomeOffset(pin, other.x, other.y);
        }
    }

    for (i = 0; i < 0x40; i++) {
        OtosuGame_spark_SetOrigin(EasyTask_GetTaskData(&scene->pool2, stage->sparkIds[i]), &at);
    }

    for (i = 0; i < stage->obstacleCount; i++) {
        OtosuGame_obstacle_SetOrigin(EasyTask_GetTaskData(&scene->pool2, stage->obstacleIds[i]), &at);
    }
}

static void OtuBoard_CreateTasks(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;

    for (i = 0; i < 0x40; i++) {
        stage->sparkIds[i] = OtosuGame_spark_CreateTask(&scene->pool2, scene->spareDataType);
    }

    for (i = 0; i < stage->obstacleCount; i++) {
        stage->obstacleIds[i] =
            OtosuGame_obstacle_CreateTask(&scene->pool2, scene->spareDataType, stage->layout.kind, stage->layout.variant,
                                          (OtosuGame_obstacle_Params*)(stage->layout.obstacles + i * 8));
    }

    stage->slashId = OtosuGame_slash_CreateTask(&scene->pool2, scene->spareDataType,
                                                stage->badgeIds[OtuScene_GetOwnPlayer(scene->multiplayer)]);
    stage->floorId = OtosuGame_floor_CreateTask(&scene->pool2, scene->spareDataType, &scene->heap, &stage->layout);
    stage->bgId    = OtosuGame_bg_CreateTask(&scene->pool2, scene->spareDataType, &scene->heap, &stage->layout);
    stage->ovbgId  = OtosuGame_ovbg_CreateTask(&scene->pool2, scene->spareDataType, &scene->heap, &stage->layout);
    stage->timerId = OtosuGame_timer_CreateTask(&scene->pool2, scene->spareDataType, stage->timerSeconds);
    stage->gaugeId = OtosuGame_specialgauge_CreateTask(&scene->pool2, scene->spareDataType,
                                                       stage->badgeIds[OtuScene_GetOwnPlayer(scene->multiplayer)]);

    stage->gameoverId = OtosuGame_gameover_CreateTask(&scene->pool1, scene->spareDataType);

    OtuBoard_FollowOwnBadge(scene);
}

static void OtuBoard_CreateSinglePlayerBadges(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = (OtuBoardStage*)OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;

    for (i = 0; i < stage->badgeCount; i++) {
        stage->badgeIds[i] = OtosuGame_badge_CreateTask(
            &scene->pool2, scene->spareDataType, i, (i == 0) ? OtuScene_GetPlayerInput(i) : NULL, &stage->layout, scene,
            scene->badgeParams, scene->decks[i], (i == 0), (i == 0) ? (u8*)0 : scene->ai[stage->match->opponents[i - 1].ai]);
    }

    OtuBoard_CreateTasks(scene);
}

static void OtuBoard_CreateMultiplayerBadges(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = (OtuBoardStage*)OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;

    for (i = 0; i < stage->badgeCount; i++) {
        BOOL local = (i == OtuScene_GetOwnPlayer(scene->multiplayer));

        stage->badgeIds[i] = OtosuGame_badge_CreateTask(&scene->pool2, scene->spareDataType, i, OtuScene_GetPlayerInput(i),
                                                        &stage->layout, scene, scene->badgeParams, scene->decks[i], local, 0);
    }

    OtuBoard_CreateTasks(scene);
}

static void OtuBoard_LoadLayout(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = (OtuBoardStage*)OtuStageDispatch_GetBlock(&scene->stage);
    s32            size;

    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, &stage->layout, OtuBoard_LayoutSizes[stage->stageIndex],
                                                    &OtuFile_MapData, OtuBoard_LayoutOffsets[stage->stageIndex]));

    size = OtuBoard_CellsSizes[stage->stageIndex];
    if (size == 0) {
        stage->layout.cells = NULL;
    } else {
        stage->layout.cells = Mem_AllocHeapTail(&scene->heap, size);
        DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, stage->layout.cells, OtuBoard_CellsSizes[stage->stageIndex],
                                                        &OtuFile_MapData, OtuBoard_CellsOffsets[stage->stageIndex]));
    }

    size = OtuBoard_ObstaclesSizes[stage->stageIndex];
    if (size == 0) {
        stage->layout.obstacles = NULL;
    } else {
        stage->layout.obstacles = Mem_AllocHeapTail(&scene->heap, size);
        DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, stage->layout.obstacles, OtuBoard_ObstaclesSizes[stage->stageIndex],
                                                        &OtuFile_MapData, OtuBoard_ObstaclesOffsets[stage->stageIndex]));
    }

    size = OtuBoard_WarpsSizes[stage->stageIndex];
    if (size == 0) {
        stage->layout.warps = NULL;
    } else {
        stage->layout.warps = Mem_AllocHeapTail(&scene->heap, size);
        DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, stage->layout.warps, OtuBoard_WarpsSizes[stage->stageIndex],
                                                        &OtuFile_MapData, OtuBoard_WarpsOffsets[stage->stageIndex]));
    }

    stage->obstacleCount = stage->layout.obstacleCount;
    stage->sparkCursor   = 0;
    RNG_SetSeed(0);
    Display_SetMainLayers(0x13);
}

static void OtuBoard_SetupSinglePlayer(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    scene->playing = 1;

    stage->match        = &scene->matches[scene->matchIndex];
    stage->badgeCount   = 1;
    stage->stageIndex   = stage->match->board;
    stage->timerSeconds = stage->match->timeLimit;

    for (s32 i = 0; i < 3; i++) {
        MI_CpuCopyU8(stage->match->opponents[i].deck, scene->decks[i + 1], sizeof(scene->decks[0]));

        if (stage->match->opponents[i].ai != 0) {
            stage->badgeCount++;
        }
    }

    OtuBoard_LoadLayout(scene);
    OtuBoard_CreateSinglePlayerBadges(scene);
}

void OtuBoard_SetupMultiplayer(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    scene->playing      = 0;
    stage->stageIndex   = scene->boardIndex;
    stage->timerSeconds = OtuBoard_TimeLimits[stage->stageIndex];
    OtuBoard_LoadLayout(scene);
    OtuBoard_CreateMultiplayerBadges(scene);
}

void OtuBoard_Teardown(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    CriSndMgr_Stop(ADX_B10);
    EasyTask_DeleteTask(&scene->pool1, stage->gameoverId);
    EasyTask_CleanupAllTasks(&scene->pool2);

    if (stage->layout.cells != NULL) {
        Mem_Free(&scene->heap, stage->layout.cells);
    }

    if (stage->layout.obstacles != NULL) {
        Mem_Free(&scene->heap, stage->layout.obstacles);
    }

    if (stage->layout.warps != NULL) {
        Mem_Free(&scene->heap, stage->layout.warps);
    }
}

static void OtuBoard_ResolveAttacks(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;
    s32            j;

    for (i = 0; i < stage->badgeCount; i++) {
        OtosuGame_badge* pin = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);

        if (OtosuGame_badge_UpdateHits(&scene->pool2, pin) != 0) {
            for (j = 0; j < stage->badgeCount; j++) {
                if (i != j) {
                    OtosuGame_badge_TakeHit(pin, EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[j]));
                }
            }
        }
    }
}

void OtuBoard_SpawnSparks(TinPinSlammer_Scene* scene, Vec2p* at) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    for (s32 i = 0; i < 0x10; i++) {
        OtosuGame_spark_Start(EasyTask_GetTaskData(&scene->pool2, stage->sparkIds[i + stage->sparkCursor]), at);
    }

    stage->sparkCursor = stage->sparkCursor + 0x10;
    if (stage->sparkCursor >= 0x40) {
        stage->sparkCursor = 0;
    }
}

static void OtuBoard_OnBadgeHit(TinPinSlammer_Scene* scene, OtosuGame_badge* a, OtosuGame_badge* b) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    Vec2p          pa;
    Vec2p          pb;
    Vec2p          board;

    BOOL bothEmpty = FALSE;
    if (OtosuGame_badge_IsOutOfPins(a) != 0 || OtosuGame_badge_IsOutOfPins(b) != 0) {
        bothEmpty = TRUE;
    }

    OtosuGame_badge_GetPos(a, &pa);
    OtosuGame_badge_GetPos(b, &pb);
    OtuVec_Sub(&pb, &pa, &pb);
    OtuVec_Scale(0x800, &pb, &pb);
    OtuVec_Add(&pb, &pa, &pb);

    if (bothEmpty == FALSE) {
        OtuBoard_SpawnSparks(scene, &pb);
    }

    s32 seIdx;
    if (OtosuGame_badge_IsStunned(a) != 0 || OtosuGame_badge_IsStunned(b) != 0) {
        seIdx = SEIDX_SE_BAYBADGE_PIYORI_HIT;
    } else {
        seIdx = SEIDX_SE_BAYBADGE_HIT;
    }

    OtosuGame_badge_GetOrigin(EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[OtuScene_GetOwnPlayer(scene->multiplayer)]),
                              &board);
    OtuBoard_PlaySEAt(seIdx, &pb, &board);
}

static void OtuBoard_CollideBadges(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;
    s32            j;

    for (i = 0; i < stage->badgeCount - 1; i++) {
        OtosuGame_badge* a = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);
        for (j = i + 1; j < stage->badgeCount; j++) {
            OtosuGame_badge* b = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[j]);
            if (OtosuGame_badge_Collide(a, b)) {
                OtosuGame_badge_ResetAI(a);
                OtosuGame_badge_ResetAI(b);
                OtuBoard_OnBadgeHit(scene, a, b);
            }
        }
    }
}

static void OtuBoard_SeparateBadges(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;
    s32            j;

    for (i = 0; i < stage->badgeCount - 1; i++) {
        void* a = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);

        for (j = i + 1; j < stage->badgeCount; j++) {
            OtosuGame_badge_Separate(a, EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[j]));
        }
    }
}

static void OtuBoard_CollideObstacles(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            i;
    s32            j;

    for (i = 0; i < stage->badgeCount; i++) {
        void* pin = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);

        for (j = 0; j < stage->obstacleCount; j++) {
            OtosuGame_badge_CollideObstacle(pin, EasyTask_GetTaskData(&scene->pool2, stage->obstacleIds[j]));
        }
    }
}

static s32 OtuBoard_GetOutcome(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    s32            me;
    s32            i;

    u32 best = 0;
    u16 mask = 0;

    for (i = 0; i < stage->badgeCount; i++) {
        u16 score = gSaveData.otosuScores[i];
        if (score > best) {
            best = score;
            mask = (1 << i);
        } else if (best == score) {
            mask |= 1 << i;
        }
    }

    s32 result;
    if (mask & (1 << OtuScene_GetOwnPlayer(scene->multiplayer))) {
        if (scene->multiplayer != 0) {
            result = ((u32)func_02047e84(mask) > 1) ? 3 : 1;
        } else {
            result = 1;
        }
    } else {
        result = 2;
    }

    return result;
}

void OtuBoard_Update(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    s32   i;
    void* pin = NULL;

    if (scene->playing == 0) {
        return;
    }

    if (OtosuGame_timer_GetCountdown(EasyTask_GetTaskData(&scene->pool2, stage->timerId)) == 0) {
        OtosuGame_gameover_Show(EasyTask_GetTaskData(&scene->pool1, stage->gameoverId), 0);

        for (i = 0; i < stage->badgeCount; i++) {
            pin                      = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);
            gSaveData.otosuScores[i] = OtosuGame_badge_GetScore(pin);

            if (i == OtuScene_GetOwnPlayer(scene->multiplayer)) {
                OtosuGame_badge_HideEntry(pin);
            }

            OtosuGame_badge_HidePoints(pin);
        }

        stage->outcome = OtuBoard_GetOutcome(scene);

        if (stage->layout.kind == 1) {
            g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~0xC;
        }

        scene->playing = 0;
        return;
    }

    s32 settled     = 0;
    u16 settledMask = 0;

    for (i = 0; i < stage->badgeCount; i++) {
        pin = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);

        if (OtosuGame_badge_HasPin(pin)) {
            settled++;
            settledMask |= 1 << i;
        }
    }

    if ((settled <= 1) && (settled < stage->badgeCount)) {
        for (i = 0; i < stage->badgeCount; i++) {
            pin = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);

            if ((settledMask & (1 << i)) != 0) {
                OtosuGame_badge_AddSurvivalBonus(pin);
            }

            gSaveData.otosuScores[i] = OtosuGame_badge_GetScore(pin);

            if (i == OtuScene_GetOwnPlayer(scene->multiplayer)) {
                OtosuGame_badge_HideEntry(pin);
            }

            OtosuGame_badge_HidePoints(pin);
        }

        stage->outcome = OtuBoard_GetOutcome(scene);

        OtosuGame_gameover_Show(EasyTask_GetTaskData(&scene->pool1, stage->gameoverId), (s16)stage->outcome);

        if (stage->layout.kind == 1) {
            g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~0xC;
        }

        scene->playing = 0;
        return;
    }

    if (OtosuGame_timer_IsAlarmed(EasyTask_GetTaskData(&scene->pool2, stage->timerId)) != 0) {
        OtosuGame_ovbg_StartPaletteAnim(EasyTask_GetTaskData(&scene->pool2, stage->ovbgId));
    }

    OtuBoard_FollowOwnBadge(scene);
    OtuBoard_ResolveAttacks(scene);
    OtuBoard_CollideBadges(scene);
    OtuBoard_SeparateBadges(scene);
    OtuBoard_CollideObstacles(scene);
}

void OtuScene_SinglePlayerBoard_Enter(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    stage->active = 1;
    OtuScene_ReadLocalInput(OtuScene_GetPlayerInput(0));
    OtuBoard_SetupSinglePlayer(scene);
    CriSndMgr_PlayFile(0x18);
    EasyFade_FadeBothDisplays(FADER_INSTANT, 0, 30);
}

void OtuScene_SinglePlayerBoard_Exit(TinPinSlammer_Scene* scene) {
    OtuBoard_Teardown(scene);
}

static void OtuScene_SinglePlayerBoard_Play(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    OtuScene_ReadLocalInput(OtuScene_GetPlayerInput(0));

    if (scene->playing == 0) {
        if (OtosuGame_gameover_IsActive(EasyTask_GetTaskData(&scene->pool1, stage->gameoverId)) == 0) {
            EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[OtuScene_GetOwnPlayer(scene->multiplayer)]);

            if (stage->outcome == 1) {
                gSaveData.unk_24B4 = 6;
                OtuStageDispatch_SetNext(&scene->stage, NULL);
            } else {
                gSaveData.unk_24B4 = 7;
                OtuStageDispatch_SetNext(&scene->stage, NULL);
            }
        }
    }
    OtuBoard_Update(scene);
}

void OtuScene_HostBoard_Enter(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    stage->active     = 1;
    stage->timer      = 600;
    stage->badgeCount = scene->playerCount;
    stage->wriconId   = OtosuGame_wricon_CreateTask(&scene->pool1, scene->spareDataType);
    stage->wrwaitId   = OtosuGame_wrwait_CreateTask(&scene->pool1, scene->spareDataType);
}

void OtuScene_HostBoard_Exit(TinPinSlammer_Scene* scene) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    EasyTask_DeleteTask(&scene->pool1, stage->wriconId);
    OtuBoard_Teardown(scene);
}
