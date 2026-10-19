#include "Engine/Core/System.h"
#include "Engine/Text.h"
#include "Otosu/OtosuGame.h"

static void OtuScene_Disconnect_Enter(void);
static void OtuScene_Disconnect_Exit(void);
static void OtuScene_Disconnect_FadeOut(TinPinSlammer_Scene* scene);
static void OtuScene_Disconnect_ShutdownWireless(TinPinSlammer_Scene* scene);
static void OtuScene_Disconnect_Finish(TinPinSlammer_Scene* scene);

static void OtuScene_Disconnect_Enter(void) {
    return; // do nothing
}

static void OtuScene_Disconnect_Exit(void) {
    return; // do nothing
}

static void OtuScene_Disconnect_FadeOut(TinPinSlammer_Scene* scene) {
    EasyFade_FadeBothDisplays(0, 0x10, 0x1000);

    if (EasyFade_IsFading() == FALSE) {
        OtuStageDispatch_NextStep(&scene->stage);
    }
}

static void OtuScene_Disconnect_ShutdownWireless(TinPinSlammer_Scene* scene) {
    switch (func_ov040_0209cb78()) {
        case 0:
        case 10:
            OtuStageDispatch_NextStep(&scene->stage);
            break;
        case 1:
            func_ov040_0209d6cc();
            break;
        case 8:
        case 9:
            func_ov040_0209d540();
            break;
        case 3:
            break;
        default:
            func_ov040_0209d588();
            break;
    }
}

static void OtuScene_Disconnect_Finish(TinPinSlammer_Scene* scene) {
    OtuStageDispatch_NextStep(&scene->stage);
    OtuStageDispatch_SetNext(&scene->stage, 0);
}

static OtuStageHandler OtuScene_DisconnectSteps[3] = {
    OtuScene_Disconnect_FadeOut,
    OtuScene_Disconnect_ShutdownWireless,
    OtuScene_Disconnect_Finish,
};

OtuSceneStage OtuScene_DisconnectStage = {
    OtuScene_Disconnect_Enter,
    OtuScene_Disconnect_Exit,
    OtuScene_DisconnectSteps,
    0,
};

void OtuScaleAnim_Init(OtuScaleAnim* anim, const OtuScaleKey* keys, u16 count, OamAffineParam* affine) {
    anim->keys  = keys;
    anim->index = 0;
    anim->count = count;
    anim->hold  = anim->keys[anim->index].frames;

    affine->scaleX = anim->keys[anim->index].scaleX;
    affine->scaleY = anim->keys[anim->index].scaleY;
}

void OtuScaleAnim_Step(OtuScaleAnim* anim, OamAffineParam* affine) {
    affine->scaleX = anim->keys[anim->index].scaleX;
    affine->scaleY = anim->keys[anim->index].scaleY;

    if (anim->index < anim->count - 1) {
        anim->hold--;
        if (anim->hold == 0) {
            anim->index++;

            if (anim->index >= anim->count) {
                anim->index = 0;
            }

            anim->hold = anim->keys[anim->index].frames;
        }
    }
}

void OtuBoard_SetSyncStep(TinPinSlammer_Scene* scene, s32 step) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    stage->step = step;
}

void OtuBoard_ReceiveDeck(TinPinSlammer_Scene* scene, s32 slot, const void* src, s32 len) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);

    MI_CpuCopyU8(src, scene->decks[slot], len);
    stage->readyMask |= 1 << slot;
}

void OtuBoard_PlaySEAt(SndMgrSeIdx seIdx, Vec2p* from, Vec2p* to) {
    Vec2p anchor;
    s32   sePan;
    s32   distance;

    anchor.x = 0x80000;
    anchor.y = 0x60000;

    OtuVec_Add(to, &anchor, &anchor);

    distance = F2I(OtuVec_Distance(from, &anchor)) / 8;
    sePan    = F2I(from->x - to->x);

    if (sePan > 0xFF) {
        sePan = 0xFF;
    } else if (sePan < 0) {
        sePan = 0;
    }

    SndMgr_StartPlayingSE(seIdx);
    s32 maxVolume = SndMgr_GetSeIdxVolume(seIdx);

    if (distance > maxVolume) {
        distance = maxVolume;
    } else if (distance < 0) {
        distance = 0;
    }

    func_02027170(seIdx, maxVolume - distance);
    SndMgr_UpdateSEPan(seIdx, sePan);
}

void OtuBoard_ForgetPartner(TinPinSlammer_Scene* scene, void* badge) {
    OtuBoardStage* stage = OtuStageDispatch_GetBlock(&scene->stage);
    for (s32 i = 0; i < stage->badgeCount; i++) {
        void* child = EasyTask_GetTaskData(&scene->pool2, stage->badgeIds[i]);

        if (child != badge) {
            OtosuGame_badge_ForgetPartner(child, badge);
        }
    }
}
