#ifndef DEBUG_FURUKAWA_TUTORIAL_H
#define DEBUG_FURUKAWA_TUTORIAL_H

#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"

typedef struct {
    /* 0x00000 */ u8              memBuffer[0x800];
    /* 0x00800 */ MemPool         memPool;
    /* 0x0080C */ TaskPool        taskPool;
    /* 0x0088C */ ResourceManager resMgr;
    /* 0x11E0C */ s32             unk_11E0C;
    /* 0x11E10 */ s32             unk_11E10;
    /* 0x11E14 */ CombatSprite    sprite;
    /* 0x11E74 */ s16             unk_11E74;
    /* 0x11E76 */ s16             unk_11E76;
} TutorialObject; // Size: 0x11E78

#endif            // DEBUG_FURUKAWA_TUTORIAL_H