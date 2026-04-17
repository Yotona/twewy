#ifndef COMBAT_CORE_COMBATACTOR_H
#define COMBAT_CORE_COMBATACTOR_H

#include "Combat/Core/CombatSprite.h"
#include <NitroSDK/fx.h>

typedef struct {
    /* 0x00 */ s32  unk_00;
    /* 0x04 */ s16  unk_04;
    /* 0x06 */ s16  unk_06;
    /* 0x08 */ s16  unk_08;
    /* 0x0A */ s16  unk_0A;
    /* 0x0C */ s32  unk_0C;
    /* 0x10 */ s16  unk_10;
    /* 0x12 */ u16  unk_12;
    /* 0x14 */ u16  unk_14;
    /* 0x16 */ u16  unk_16;
    /* 0x18 */ u16  unk_18;
    /* 0x1A */ u16  unk_1A;
    /* 0x1C */ u16  unk_1C;
    /* 0x20 */ s32  pendingCommand;
    /* 0x24 */ BOOL isFlipped;
    /* 0x28 */ Vec  position;
    /* 0x34 */ s16  screenX;
    /* 0x36 */ s16  screenY;
    /* 0x38 */ s32  zVelocity;
    /* 0x3C */ s32  zGravity;
    /* 0x40 */ s32  xVelocity;
    /* 0x44 */ s32  yVelocity;
    /* 0x48 */ s32  linearDampingStep;
    /* 0x4C */ s32  dampingScale;
    /* 0x50 */ s32  unk_50;
    /* 0x54 */ s32  flags;
    /* 0x58 */ s16  unk_58;
    /* 0x5A */ s16  unk_5A;
    /* 0x5C */ u16  periodicEffectMode;
    /* 0x5E */ s16  periodicEffectTimer;
    /* 0x60 */ s16  periodicEffectAmount;
    /* 0x62 */ s16  unk_62;
    /* 0x64 */ s16  unk_64;
    /* 0x66 */ s16  unk_66;
    /* 0x68 */ s16  unk_68;
    /* 0x6A */ s16  unk_6A;
    /* 0x6C */ s16  currentHp;
    /* 0x6E */ s16  maxHp;
    /* 0x70 */ s16  unk_70;
    /* 0x72 */ s16  unk_72;
    /* 0x74 */ s16  unk_74;
    /* 0x76 */ s16  unk_76;
    /* 0x78 */ s32  unk_78;
} CombatActor;

void CombatActor_SetPendingCommand(CombatActor* actor, s32 arg1);

s32 CombatActor_PopPendingCommand(CombatActor* actor);

void CombatActor_ForceDefeat(CombatActor* actor);

void func_ov003_02082f7c(CombatActor* actor, s32 arg1);

void CombatActor_ClearPeriodicEffect(CombatActor* actor);

void func_ov003_02082fe0(CombatActor* actor);

void CombatActor_UpdateEffects(s32 arg0, CombatActor* actor);

void CombatActor_UpdatePhysics(CombatActor* actor);

void CombatActor_Init(CombatActor* actor, s32 arg1);

void CombatActor_Render(CombatActor* actor, CombatSprite* sprite);

#endif // COMBAT_CORE_COMBATACTOR_H