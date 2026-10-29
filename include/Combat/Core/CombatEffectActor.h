#ifndef COMBAT_CORE_COMBATEFFECTACTOR_H
#define COMBAT_CORE_COMBATEFFECTACTOR_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatList.h"

typedef struct Ov003EffectSpawn {
    /* 0x00 */ u16 config;
    /* 0x02 */ u16 id;
    /* 0x04 */ s32 arg1;
    /* 0x08 */ s32 arg2;
} Ov003EffectSpawn; // Size: 0xC

// The existing cut-in motion record is also the prefix of a pooled effect visual.
typedef struct Ov003EffectSprite {
    /* 0x00 */ Ov003CutinMotion motion;
    /* 0x34 */ u16              flags;
    /* 0x36 */ u16              unk_36;
} Ov003EffectSprite; // Size: 0x38

typedef struct Ov003EffectActor {
    /* 0x000 */ u16               id;
    /* 0x002 */ u16               config;
    /* 0x004 */ CombatActor       actor;
    /* 0x080 */ Ov003EffectSprite sprite;
    /* 0x0B8 */ CombatListNode    node;
} Ov003EffectActor; // Size: 0xC4

// Sprite pack and allocation settings shared by the obstacle loader and battle tables.
typedef struct Ov003AnimSet {
    /* 0x00 */ const SpriteAnimEntry* entries;
    /* 0x04 */ u16                    unk_04;
    /* 0x06 */ u16                    unk_06;
    /* 0x08 */ u16                    unk_08;
    /* 0x0A */ u16                    unk_0A;
} Ov003AnimSet; // Size: 0xC

#endif
