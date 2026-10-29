#ifndef COMBAT_CORE_COMBATBOSSEXP_H
#define COMBAT_CORE_COMBATBOSSEXP_H

#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"

/// The four sprite animations the boss defeat burst task (`Tsk_BossExp1`) is started with, plus the actor it belongs to.
typedef struct BossExp1Anims {
    /* 0x00 */ SpriteAnimationEx slots[4]; // [engine * 2 + group]
    /* 0xC0 */ Ov003EnemyActor*  actor;
} BossExp1Anims;                           // Size: 0xC4

#endif
