#include "Combat/Core/CombatActor.h"
#include "Combat/Core/Combat.h"

extern void func_ov003_0208b3cc(s32, s16);
extern void func_ov003_0208b518(s32, void*, s16);

static void func_ov003_02082d10(CombatActor* actor) {
    actor->unk_10 = 0;
    actor->unk_12 &= ~1;
}

static void CombatActor_ApplyPlanarDamping(CombatActor* actor) {
    if ((actor->flags & 0x4000) == 0) {
        if (actor->xVelocity > 0) {
            actor->xVelocity -= actor->linearDampingStep;
            if (actor->xVelocity < 0) {
                actor->xVelocity = 0;
            }
        } else if (actor->xVelocity < 0) {
            actor->xVelocity += actor->linearDampingStep;
            if (actor->xVelocity > 0) {
                actor->xVelocity = 0;
            }
        }

        if (actor->yVelocity > 0) {
            actor->yVelocity -= actor->linearDampingStep;
            if (actor->yVelocity < 0) {
                actor->yVelocity = 0;
            }
        } else if (actor->yVelocity < 0) {
            actor->yVelocity += actor->linearDampingStep;
            if (actor->yVelocity > 0) {
                actor->yVelocity = 0;
            }
        }
        if (actor->position.z >= 0) {
            actor->xVelocity = F2I((((s64)actor->xVelocity * actor->dampingScale) + 0x800));
            actor->yVelocity = F2I((((s64)actor->yVelocity * actor->dampingScale) + 0x800));
        }
    }
}

static void CombatActor_ApplyPeriodicEffectTick(s32 arg0, CombatActor* actor) {
    if (actor->periodicEffectTimer % 180 != 0) {
        return;
    }

    s32 val = actor->periodicEffectAmount;

    if (actor->unk_00 == 0) {
        func_ov003_0208b3cc(arg0, -val);
        func_ov003_0209b564(arg0, val, actor, 3);
        if (actor->currentHp <= 0) {
            actor->flags |= 0x2000;
            CombatActor_ForceDefeat((CombatActor*)data_ov003_020e71b8->unk3D898);
            CombatActor_ForceDefeat((CombatActor*)data_ov003_020e71b8->unk3D89C);
            data_ov003_020e71b8->unk3D878 |= 0x30000;
        }
    } else {
        func_ov003_0208b518(arg0, actor, -val);
        func_ov003_0209b61c(arg0, val, actor);

        if (actor->currentHp <= 0) {
            actor->flags |= 0x2000;
            CombatActor_ForceDefeat(actor);
            CombatActor_ForceDefeat(actor->unk_78);
        }
    }
}

void CombatActor_SetPendingCommand(CombatActor* actor, s32 command) {
    if ((actor->flags & 4) == 0) {
        actor->pendingCommand = command;
    }
}

s32 CombatActor_PopPendingCommand(CombatActor* actor) {
    s32 prev = actor->pendingCommand;

    actor->pendingCommand = 0;
    if (actor->unk_5A > 0) {
        actor->unk_5A--;
    }
    return prev;
}

void CombatActor_ForceDefeat(CombatActor* actor) {
    if (actor != NULL) {
        actor->currentHp = 0;
        CombatActor_SetPendingCommand(actor, 3);
        actor->flags |= 4;
    }
}

void func_ov003_02082f7c(CombatActor* actor, s32 arg1) {
    s32 var_r0 = 0;
    s32 var_r2 = 0;
    if (arg1 > 0) {
        s32 var_r3 = 0;
        do {
            var_r2++;
            var_r0 += var_r3;
            var_r3 += actor->zGravity;
        } while (var_r2 < arg1);
    }
    actor->zVelocity = -(var_r0 / arg1);
}

void CombatActor_ClearPeriodicEffect(CombatActor* actor) {
    actor->periodicEffectMode   = 0;
    actor->periodicEffectTimer  = 0;
    actor->periodicEffectAmount = 0;
    actor->unk_64               = 100;
    actor->unk_0A               = 100;
}

void func_ov003_02082fe0(CombatActor* actor) {
    actor->unk_14 = 10;
    actor->unk_16 = 0;
    actor->unk_1A = 100;
    actor->unk_1C = 100;
}

void CombatActor_UpdateEffects(s32 arg0, CombatActor* actor) {
    if ((data_ov003_020e71b8->unk3D878 & 6) == 0) {
        if (actor->periodicEffectMode == 1) {
            CombatActor_ApplyPeriodicEffectTick(arg0, actor);
        }

        actor->periodicEffectTimer--;
        if (actor->periodicEffectTimer <= 0) {
            CombatActor_ClearPeriodicEffect(actor);
        }

        actor->unk_10--;
        if (actor->unk_10 <= 0) {
            actor->unk_10 = 0;
        }
    }
}

void CombatActor_UpdatePhysics(CombatActor* actor) {
    actor->position.z += actor->zVelocity;

    if (actor->position.z < 0) {
        actor->zVelocity += actor->zGravity;
        if (actor->zVelocity > 0x5000) {
            actor->zVelocity = 0x5000;
        }
    } else if (actor->position.z > 0) {
        actor->position.z = 0;

        s32 var_r1 = -(s32)((((s64)actor->zVelocity * actor->unk_50) + 0x800) >> 0xC);

        actor->zVelocity = var_r1;
        if (var_r1 < 0) {
            var_r1 = -var_r1;
        }
        if (var_r1 < 0x1000) {
            actor->zVelocity = 0;
        }
    }
    actor->position.x += actor->xVelocity;
    actor->position.y += actor->yVelocity;
    CombatActor_ApplyPlanarDamping(actor);
}

void CombatActor_Init(CombatActor* actor, s32 arg1) {
    actor->unk_00            = arg1;
    actor->position.x        = 0;
    actor->position.y        = 0;
    actor->position.z        = 0;
    actor->zVelocity         = 0;
    actor->zGravity          = 0x400;
    actor->unk_50            = 0;
    actor->xVelocity         = 0;
    actor->yVelocity         = 0;
    actor->linearDampingStep = 410;
    actor->dampingScale      = 0x1000;
    actor->pendingCommand    = 0;
    actor->isFlipped         = FALSE;
    actor->flags             = 0;
    actor->unk_58            = 0;
    actor->unk_5A            = 0;
    actor->unk_70            = 16;
    actor->unk_72            = 40;
    actor->unk_74            = 0;
    actor->unk_76            = 0;
    actor->unk_66            = 0;
    actor->unk_68            = 0;
    actor->unk_6A            = 100;
    actor->unk_04            = 0;
    actor->unk_06            = 50;
    actor->unk_08            = 100;
    actor->unk_0A            = 100;
    actor->unk_62            = 100;
    actor->unk_64            = 100;
    CombatActor_ClearPeriodicEffect(actor);
    func_ov003_02082fe0(actor);
    func_ov003_02082d10(actor);
    actor->maxHp     = 100;
    actor->currentHp = 100;
    actor->unk_78    = 0;
}

void CombatActor_Render(CombatActor* actor, CombatSprite* sprite) {
    if ((func_ov003_02084d80(0) == 1) && (actor->unk_00 != 0)) {
        actor->flags |= 0x80000000;
        return;
    }

    if (actor->flags & 0x1000) {
        actor->flags &= ~0x1000;
        sprite->paletteTimer = 0xC;
        sprite->flags46 |= 0x1000;
        CombatSprite_SetPaletteMode(sprite, 2);
    }

    s32 posX;
    s32 posY;
    if (sprite->sprite.bits_0_1 == 0) {
        posX = func_ov003_020843b0(0, actor->position.x);
        posY = func_ov003_020843ec(0, actor->position.y, actor->position.z);
    } else {
        posX = func_ov003_020843b0(1, actor->position.x);
        posY = func_ov003_020843ec(1, actor->position.y, actor->position.z);
    }
    if (actor->isFlipped == TRUE) {
        CombatSprite_SetFlip(sprite, TRUE);
    } else {
        CombatSprite_SetFlip(sprite, FALSE);
    }

    CombatSprite_SetPosition(sprite, posX, posY);
    func_ov003_02082730(sprite, 0x7FFFFFFE - actor->position.y);
    CombatSprite_Render(sprite);
    actor->screenX = posX;
    actor->screenY = posY;
}
