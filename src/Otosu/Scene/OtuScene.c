#include "EasyFade.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Otosu/OtosuGame.h"
#include "Save.h"
#include "common_data.h"

void OtuScene_LoadPlayerDeck(TinPinSlammer_Scene* scene) {
    s32 i;
    s32 count = 0;

    for (i = 0; i < 6; i++) {
        u16 pinID = gSaveData.equippedPins[i].pinID;

        scene->decks[0][count] = pinID;
        if (pinID != 0xFFFF) {
            count++;
        }
    }

    while (count < 7) {
        scene->decks[0][count] = OTU_NO_PIN;
        count++;
    }

    if (scene->decks[0][0] == OTU_NO_PIN) {
        scene->decks[0][0] = 0xE5;
    }
}

void OtuScene_InitText(TinPinSlammer_Scene* scene) {
    UnkStruct_usedby_02025b68 args;

    if (scene->textReady != 0) {
        return;
    }

    args.unk_00 = 1;
    args.unk_04 = 0;
    args.unk_08 = scene->dataType;
    args.unk_0C = (s32)&data_0205c9b0;
    args.unk_10 = 0;
    args.unk_14 = NULL;
    args.unk_18 = 0;
    args.unk_1A = 0;
    args.unk_1C = 0x20;
    args.unk_1E = 0x18;

    BgText_Init(scene->textRes, &args);
    scene->textReady = 1;
}

void OtuScene_ReleaseText(TinPinSlammer_Scene* scene) {
    if (scene->textReady == 0) {
        return;
    }

    BgText_Free(scene->textRes);
    scene->textReady = 0;
}

void OtuScene_Init(TinPinSlammer_Scene* scene) {
    s32 j;
    s32 i;

    scene->done      = 0;
    scene->linkError = 0;
    scene->unk_41AF4 = 1;
    scene->playing   = 1;

    DatMgr_ReleaseData(
        DatMgr_LoadRawDataWithOffset(1, scene->badgeParams, sizeof(scene->badgeParams), &OtuFile_BadgeParams, 0));
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, scene->matches, sizeof(scene->matches), &OtuFile_SingleMatches, 0));
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, scene->ai, sizeof(scene->ai), &OtuFile_AI, 0));

    for (i = 0; i < 4; i++) {
        for (j = 0; j < 7; j++) {
            scene->decks[i][j] = OTU_NO_PIN;
        }
    }

    scene->fadeTask = EasyTask_CreateTask(&scene->pool1, &Task_EasyFade, NULL, 0, NULL, NULL);
    EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
    OtuStageDispatch_Init(&scene->stage, &scene->heap);
}

void OtuScene_InitSinglePlayer(TinPinSlammer_Scene* scene) {
    scene->multiplayer = 0;
    scene->playerCount = 4;
}

void OtuScene_InitMultiplayer(TinPinSlammer_Scene* scene) {
    scene->multiplayer = 1;
    scene->playerCount = 0;
}

void OtuScene_DeleteFade(TinPinSlammer_Scene* scene) {
    EasyTask_DeleteTask(&scene->pool1, scene->fadeTask);
}

void OtuScene_RunStages(TinPinSlammer_Scene* scene) {
    OtuStageDispatch_Enter(&scene->stage, scene);
    OtuStageDispatch_Run(&scene->stage, scene);
    OtuStageDispatch_Exit(&scene->stage, scene);

    if (OtuStageDispatch_IsFinished(&scene->stage) != 0) {
        scene->done = 1;
    }
}

void OtuScene_RunStagesWireless(TinPinSlammer_Scene* scene) {
    u8* base = (u8*)scene;
    s32 reset;
    OtuStageDispatch_Enter(&scene->stage, scene);
    OtuStageDispatch_Run(&scene->stage, scene);

    if (OtuStageDispatch_GetStage(&scene->stage) != &OtuScene_DisconnectStage &&
        OtuStageDispatch_IsFinished(&scene->stage) == 0)
    {
        switch (func_ov040_0209cb78()) {
            case 8:
            case 9:
            case 10: {
                if (scene->linkOpen != 0) {
                    func_ov040_0209ed30();
                    func_ov040_0209ece4();
                    func_ov040_0209ed20();
                    func_02047338();
                    func_ov040_0209d970(0);
                    func_ov040_0209d434(0, 0);
                    scene->linkOpen = 0;
                }
                scene->linkError = 1;
                func_ov040_0209d990();
                OtuStageDispatch_SetNext(&scene->stage, &OtuScene_DisconnectStage);
            } break;
        }

        reset = (SystemStatusFlags.reset != 0);
        if (reset) {
            func_ov040_0209d990();
            OtuStageDispatch_SetNext(&scene->stage, &OtuScene_DisconnectStage);
        }
    }

    if (scene->linkLost != 0) {
        scene->linkError = 1;
        func_ov040_0209d990();
        OtuStageDispatch_SetNext(&scene->stage, &OtuScene_DisconnectStage);
        scene->linkLost = 0;
    }

    if (scene->linkTimeout != 0) {
        scene->linkError = 1;
        func_ov040_0209d990();
        OtuStageDispatch_SetNext(&scene->stage, &OtuScene_DisconnectStage);
        scene->linkTimeout = 0;
    }

    OtuStageDispatch_Exit(&scene->stage, scene);

    if (OtuStageDispatch_IsFinished(&scene->stage) != 0) {
        scene->done = 1;
    }
}
