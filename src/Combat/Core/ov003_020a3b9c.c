#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Core/CombatTouch.h"
#include "EasyFade.h"
#include "Engine/Color.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Save.h"
#include "common_data.h"

#include <nitro/fx.h>
#include <nitro/gx.h>
#include <nitro/mi/cpumem.h>

// The touch action actors: creation, per-frame state machine, the touch-hit push-back and the
// psych animation objects they spawn. The event queue and the pin bookkeeping live in
// `ov003_020a30d4.c`; the actors themselves are linked into `Ov003Global + 0x3D8BC`.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 0x3C-byte effect block at 0xCC, driven by `ov003_020a5250.c`.

/// The psych trail effect object: a point trail plus the two sprites it draws.

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

/// Defined with a second argument (the armed battle state) in `BtlBadge_ConditionBoot.c`; the callers here never pass
/// it, so the declaration is deliberately unprototyped.
extern void func_ov003_020a6f90();
extern s32  func_020405d4(void);
extern s32  func_02040ac0(void);
extern void func_02041030(s32);
extern void func_02041048(s32);

extern s32 CombatSprite_Load(CombatSprite*, SpriteAnimationEx*);

extern Ov003TouchActor* func_ov003_02084970(CombatList*);
extern void             func_ov003_020849bc(CombatList*, CombatListNode*, void*);
extern s32              func_ov003_020a3330(s32);
extern s32              func_ov003_020a3468(s32);
extern s32              func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void             func_ov003_020a5540(Ov003EffectState*, Ov003TouchActor*);
extern void             func_ov003_020a563c(Ov003EffectState*);
extern void             func_ov003_020a58e4(Ov003EffectState*, u16, u8);
extern s32              func_ov003_020a632c(s16, s16, s16, s16);
extern s32              func_ov003_020a6c7c(s32);
extern s32              func_ov003_020a6cc4(u8);

extern s32 SndMgr_IsSEPlaying(s32);

/// Defined below; the state machine calls itself through these.
s16                func_ov003_020a40cc(Ov003TouchActor*, s32);
s32                func_ov003_020a410c(Ov003TouchActor*, s16, s16);
void               func_ov003_020a45d4(Ov003TouchActor*);
s32                func_ov003_020a4908(Ov003PsychParams*, u16);
s32                func_ov003_020a49b4(Ov003PsychParams*, u16);
s32                func_ov003_020a49c8(Ov003PsychParams*, u16);
const EquippedPin* func_ov003_020a4a24(s32 deck);

// ---------------------------------------------------------------------------------------------
// Ov003Global views
// ---------------------------------------------------------------------------------------------

/// The actor list and the live-actor counter, both split out of the middle of `Ov003Global`.
#define OV003_TOUCH_ACTORS (&data_ov003_020e71b8->unk3D8BC)
#define OV003_TOUCH_SLOTS  (&gSaveData.battleTouchSlots)

/// The table of 6-byte per-slot start positions (see `ov003_020a30d4.c`).

/// The live actor count at `Ov003Global + 0x3D8C4`.
#define OV003_ACTOR_COUNT data_ov003_020e71b8->touchActorCount

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern const BinIdentifier      data_ov003_020dc03c;
extern const BinIdentifier      data_ov003_020dc044;
extern const BinIdentifier      data_ov003_020dc04c;
extern const EquippedPin* const data_ov003_020dc054[12];
extern const EquippedPin        data_ov003_020dc084[6];
extern const EquippedPin        data_ov003_020dc0c0[6];
extern const EquippedPin        data_ov003_020dc0fc[6];
extern const EquippedPin        data_ov003_020dc138[6];
extern const EquippedPin        data_ov003_020dc174[6];
extern const EquippedPin        data_ov003_020dc1b0[6];
extern const EquippedPin        data_ov003_020dc1ec[6];
extern char                     data_ov003_020e4868[0x14];
extern char                     data_ov003_020e487c[0x18];
extern char                     data_ov003_020e4894[0x1C];
char                            data_ov003_020e4868[0x14] = "Data/BadgeData.bin";

char data_ov003_020e487c[0x18] = "Apl_Tak/Grp_Badge.bin";

char                data_ov003_020e4894[0x1C] = "Apl_Hir/Grp_BtlBadge.bin";
const BinIdentifier data_ov003_020dc03c       = {3, (char*)&data_ov003_020e4868};

const BinIdentifier data_ov003_020dc044 = {3, (char*)&data_ov003_020e4894};

const BinIdentifier data_ov003_020dc04c = {3, (char*)&data_ov003_020e487c};

const EquippedPin* const data_ov003_020dc054[12] = {
    data_ov003_020dc0c0, data_ov003_020dc174, data_ov003_020dc174, data_ov003_020dc174,
    data_ov003_020dc174, data_ov003_020dc174, data_ov003_020dc138, data_ov003_020dc0fc,
    data_ov003_020dc1b0, data_ov003_020dc0c0, data_ov003_020dc084, data_ov003_020dc1ec,
};

const EquippedPin data_ov003_020dc084[6] = {
    {0x0105, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

const EquippedPin data_ov003_020dc0c0[6] = {
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

const EquippedPin data_ov003_020dc0fc[6] = {
    {0x010E, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0x010C, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

const EquippedPin data_ov003_020dc138[6] = {
    {0x010A, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

const EquippedPin data_ov003_020dc174[6] = {
    {0x0109, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

const EquippedPin data_ov003_020dc1b0[6] = {
    {0x0109, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0x010D, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

const EquippedPin data_ov003_020dc1ec[6] = {
    {0x0022, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0x0063, 0x0000, 0x0000, 0x0000, 0x00, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
    {0xFFFF, 0x0000, 0x0000, 0x0000, 0x01, {.raw = 0x01}},
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Builds the touch action actor described by `spawn` and links it into the actor list.
// Nonmatching: field store scheduling after CombatSprite_InitAnim.
void func_ov003_020a3b9c(Ov003TouchActor* actor, const Ov003ActorSpawn* spawn) {
    SpriteAnimationEx anim;
    s32               angle;

    actor->state   = 0;
    actor->request = 0;
    actor->slot    = spawn->slot;
    actor->kind    = spawn->kind;
    actor->unk_00C = spawn->unk_02;
    actor->unk_00E = spawn->unk_04;
    actor->params  = *spawn->params;
    actor->unk_044 = 0;
    actor->unk_046 = 0;
    actor->unk_04C = 0;
    actor->unk_050 = 0;
    actor->unk_052 = 0;
    actor->unk_054 = 0;
    CombatSprite_InitAnim(&anim.anim, 2, &data_ov003_020dc04c);
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_1C     = 1;
    anim.anim.animIndex  = 1;
    anim.anim.unk_28     = 3;
    anim.anim.unk_20     = 4;
    anim.animTable       = NULL;
    anim.anim.unk_18     = 2;
    anim.anim.unk_26     = 2;
    anim.anim.packIndex  = actor->params.head.unk_00 + 1;
    CombatSprite_Load(&actor->sprite, &anim);
    func_ov003_02082738(&actor->sprite, 0);
    func_ov003_02082730(&actor->sprite, 0x3E8000);
    actor->radius    = 0x10;
    actor->x         = OV003_TOUCH_SLOTS->x[actor->slot] << 12;
    actor->y         = OV003_TOUCH_SLOTS->y[actor->slot] << 12;
    angle            = (RNG_Next(0x10000) >> 4) * 2;
    actor->velocityX = FX_Mul(FX_SinCosTable_[angle + 1], FX32_ONE);
    actor->velocityY = FX_Mul(FX_SinCosTable_[angle], FX32_ONE);
    func_ov003_020a58e4(&actor->effect, actor->params.head.unk_02, actor->params.unk_04);
    func_ov003_020a7284(actor);
    func_ov003_020a7ba8(actor);
    OV003_ACTOR_COUNT++;
    func_ov003_020849bc(OV003_TOUCH_ACTORS, &actor->node, actor);
}

void func_ov003_020a3d98(Ov003TouchActor* actor) {
    func_ov003_020a5540(&actor->effect, actor);
}

void func_ov003_020a3dac(Ov003TouchActor* actor) {
    func_ov003_020a563c(&actor->effect);
}

/// TRUE while the actor is allowed to run its action.
// Nonmatching: flag reload and register allocation.
s32 func_ov003_020a3dbc(Ov003TouchActor* actor) {
    s32 waiting;

    if (func_ov003_02084d80(0) == 1) {
        return 0;
    }
    waiting = data_ov003_020e71b8->unk3D878 & 6;
    if (waiting != 0 || data_ov003_020e71b8->unk3D884 != 0) {
        return 0;
    }
    if (actor->kind == 0xFFFE) {
        return actor->unk_044 > 0;
    }
    if (actor->state == 1 || actor->state == 2 || actor->state == 4) {
        return 0;
    }
    return 1;
}

/// TRUE while the actor can be pushed around by the other actors.
// Nonmatching: flag reload and register allocation.
s32 func_ov003_020a3e50(Ov003TouchActor* actor) {
    s32 waiting;

    if (func_ov003_02084d80(0) == 1) {
        return 0;
    }
    waiting = data_ov003_020e71b8->unk3D878 & 6;
    if (waiting != 0 || data_ov003_020e71b8->unk3D884 != 0) {
        return 0;
    }
    if (actor->kind == 0xFFFE) {
        return actor->unk_044 > 0;
    }
    return actor->state != 4;
}

/// TRUE while the actor's kind matches the current battle mode.
// Nonmatching: load of the battle-mode flag is not hoisted out of the branches the same way.
s32 func_ov003_020a3edc(Ov003TouchActor* actor) {
    u8  mode = actor->unk_00E;
    s32 flag = data_ov003_020e71b8->unk3D880;

    if (mode == 0) {
        return flag == 0;
    }
    if (mode == 1) {
        return flag;
    }
    return 1;
}

/// TRUE when a touch is inside one of the actor's active touch areas.
s32 func_ov003_020a3f20(Ov003TouchActor* actor) {
    if (func_ov003_02085cfc(2) != 0 &&
        func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC) != 0)
    {
        return 1;
    }
    if (func_ov003_02085cfc(6) != 0 &&
        func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D378, data_ov003_020e71b8->unk3D37A) != 0 &&
        func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC) != 0)
    {
        return 1;
    }
    if (func_ov003_02085cfc(3) != 0 &&
        func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D37C, data_ov003_020e71b8->unk3D37E) != 0)
    {
        return 1;
    }
    return 0;
}

/// TRUE when the release point of the current touch is inside the actor.
s32 func_ov003_020a401c(Ov003TouchActor* actor) {
    if (func_ov003_02085cfc(2) != 0 &&
        func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC) != 0)
    {
        return 1;
    }
    if (func_ov003_02085cfc(3) != 0 &&
        func_ov003_020a410c(actor, data_ov003_020e71b8->unk3D37C, data_ov003_020e71b8->unk3D37E) != 0)
    {
        return 1;
    }
    return 0;
}

void func_ov003_020a40b0(Ov003TouchActor* actor) {
    actor->unk_050 = 0xF;
}

s16 func_ov003_020a40bc(Ov003TouchActor* actor) {
    return func_ov003_020a40cc(actor, 1);
}

/// Ticks the actor's lifetime counter down and returns it.
// Nonmatching: the s16 lifetime clamp is narrowed in a different order.
s16 func_ov003_020a40cc(Ov003TouchActor* actor, s32 amount) {
    if (func_ov003_02084d80(8) == 0 && actor->params.unk_12 != 0) {
        s16 value = actor->unk_044 - amount;

        if (value < 0) {
            value = 0;
        }
        actor->unk_044 = value;
    }
    return actor->unk_044;
}

/// TRUE when the screen point (x, y) is inside the actor's touch rectangle.
s32 func_ov003_020a410c(Ov003TouchActor* actor, s16 x, s16 y) {
    s16 radius = actor->radius;
    s32 posX   = actor->x >> 12;
    s32 posY   = actor->y >> 12;

    if (x >= posX - radius && x <= posX + radius && y >= posY - radius && y <= posY + radius) {
        return 1;
    }
    return 0;
}

/// Pushes overlapping actors apart, then keeps every actor inside the touch area.
// Nonmatching: pairwise push-back loop: bookkeeping registers and branch layout.
void func_ov003_020a4158(void) {
    Ov003TouchActor* a;
    Ov003TouchActor* b;

    for (a = func_ov003_0208495c(OV003_TOUCH_ACTORS); a != NULL; a = func_ov003_02084984(&a->node)) {
        s32 aActive = func_ov003_020a3e50(a);

        for (b = func_ov003_02084970(OV003_TOUCH_ACTORS); b != NULL; b = func_ov003_02084998(&b->node)) {
            s32 bActive = func_ov003_020a3e50(b);
            s32 radius;
            s32 dx;
            s32 dy;
            s32 push;

            if (a == b) {
                continue;
            }
            if (aActive == 0 || bActive == 0) {
                continue;
            }
            radius = (a->radius + b->radius) - 6;
            dx     = a->x - b->x;
            dy     = a->y - b->y;
            if (FX_Abs(dx) >= radius << 12 || FX_Abs(dy) >= radius << 12) {
                continue;
            }
            push = (radius << 12) - FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
            if (push > 0) {
                s32 angle = (FX_Atan2Idx(dy, dx) >> 4) * 2;
                s32 pushX = FX_Mul(FX_SinCosTable_[angle + 1], push);
                s32 pushY = FX_Mul(FX_SinCosTable_[angle], push);

                a->x += pushX;
                a->y += pushY;
                b->x -= pushX;
                b->y -= pushY;
            }
        }
    }
    for (a = func_ov003_0208495c(OV003_TOUCH_ACTORS); a != NULL; a = func_ov003_02084984(&a->node)) {
        s32 radius;

        if (func_ov003_020a3e50(a) == 0) {
            continue;
        }
        radius = a->radius;
        if (a->x < radius << 12) {
            a->x = radius << 12;
        } else if (a->x > (0x100 - radius) << 12) {
            a->x = (0x100 - radius) << 12;
        }
        if (a->y < radius << 12) {
            a->y = radius << 12;
        } else if (a->y > (0xC0 - radius) << 12) {
            a->y = (0xC0 - radius) << 12;
        }
    }
}

/// Requests state `request` from the actor; a "revive" request while the actor is still fading
/// turns into the death state instead.
void func_ov003_020a4390(Ov003TouchActor* actor, s32 request) {
    if (request == 5) {
        func_ov003_020a45d4(actor);
        func_ov003_02087ed8(0x2D);
        return;
    }
    if (request == 2 && actor->params.unk_1E == 0) {
        actor->request = 6;
        return;
    }
    actor->request = request;
}

/// Runs the pending actor request and returns it (0 = none).
// Nonmatching: request dispatch: per-case reloads of the psych parameters.
u32 func_ov003_020a43c8(Ov003TouchActor* actor) {
    u32                  request = actor->request;
    Ov003TouchSlotTable* slots   = &gSaveData.battleTouchSlots;

    switch (request) {
        case 1:
            if (actor->params.unk_18 != 0) {
                if (actor->params.unk_18 == 1) {
                    s16 value = func_ov003_020a49b4(&actor->params, actor->unk_00C);

                    func_ov003_020a8300(actor, value, value);
                } else {
                    func_ov003_020a6f90(actor);
                }
            }
            actor->state = 1;
            break;
        case 8:
            if (actor->params.unk_18 != 0) {
                if (actor->params.unk_18 == 1) {
                    func_ov003_020a8300(actor, slots->unk_20[actor->slot],
                                        func_ov003_020a49b4(&actor->params, actor->unk_00C));
                } else {
                    func_ov003_020a6f90(actor);
                }
            }
            actor->state = 1;
            break;
        case 2:
            if (actor->params.unk_1E == 1) {
                s16 value = func_ov003_020a49c8(&actor->params, actor->unk_00C);

                func_ov003_020a8300(actor, value, value);
            }
            actor->state = 2;
            break;
        case 9:
            if (actor->params.unk_1E == 1) {
                func_ov003_020a8300(actor, slots->unk_20[actor->slot], func_ov003_020a49c8(&actor->params, actor->unk_00C));
            }
            actor->state = 2;
            break;
        case 3:
            actor->unk_046 = func_ov003_020a4908(&actor->params, actor->unk_00C);
            actor->unk_044 = actor->unk_046;
            actor->state   = 3;
            break;
        case 7:
            actor->unk_046 = func_ov003_020a4908(&actor->params, actor->unk_00C);
            actor->unk_044 = slots->unk_14[actor->slot];
            actor->state   = 3;
            break;
        case 4:
            actor->state = 4;
            break;
        case 6:
            actor->state = 5;
            break;
    }
    actor->request = 0;
    return request;
}

/// TRUE while both of the partner's psych actions are idle.
s32 func_ov003_020a4594(void) {
    if (func_ov003_020a3330(0x46) != 0 && func_ov003_020a3330(0x47) != 0 && func_ov003_020a3330(0x48) != 0) {
        return 1;
    }
    return 0;
}

/// Picks the next state request for the actor from the current battle state.
// Nonmatching: request selection branch layout.
void func_ov003_020a45d4(Ov003TouchActor* actor) {
    u16 value = actor->params.unk_18;

    switch (value) {
        case 2:
            actor->request = 1;
            break;
        case 0:
            actor->request = 3;
            break;
        case 1:
            if (func_ov003_020a4594() != 0 || func_ov003_020a49b4(&actor->params, actor->unk_00C) == 0) {
                actor->request = 3;
            } else {
                actor->request = 1;
            }
            break;
    }
    if (((s32)gSaveData.unk_3142 > 1) && data_ov003_020e71b8->unk3D890 == 0xD) {
        switch (OV003_TOUCH_SLOTS->kind[actor->slot]) {
            case 3:
                actor->request = 7;
                break;
            case 1:
                actor->request = 8;
                break;
            case 2:
                actor->request = 9;
                break;
            case 5:
                actor->request = 6;
                break;
        }
    }
    if (data_ov003_020e71b8->unk3D88D == 5) {
        actor->request = 6;
    }
    if (func_ov003_020a6cc4(actor->params.unk_05) != 0) {
        actor->request = 6;
    }
    if (func_ov003_02084d80(4) == 0x3B) {
        actor->request = 6;
    }
}

/// The equipped pin of deck slot `slot` for the player `deck`.
const EquippedPin* func_ov003_020a4718(s32 deck, s32 slot) {
    if (deck == 0xD) {
        return &gSaveData.equippedPins[slot];
    }
    return &func_ov003_020a4a24(deck)[slot];
}

/// Loads the raw pin record `pinId` into `out`.
s32 func_ov003_020a474c(u16 pinId, void* out) {
    DatMgr_ReleaseData(DatMgr_LoadRawDataWithOffset(1, out, 0x34, &data_ov003_020dc03c, pinId * 0x34));
    return 1;
}

/// The scaled duration of psych kind `params->unk_05` at level `level`.
// Nonmatching: duration scaling: FX_Divide/FX_Mul operand order.
s32 func_ov003_020a4778(Ov003PsychParams* params, s32 level) {
    s32 value = (params->unk_10 * (level - 1)) + params->unk_0E;
    u8  kind  = params->unk_05;

    if (kind < 0xD && func_ov003_020a3330(kind + 0x24) != 0) {
        s32 scale = FX_Divide(func_ov003_020a3468(kind + 0x24) << 12, 0x64000) + 0x1000;

        value = FX_Mul(value << 12, scale) >> 12;
    }
    switch (func_ov003_020a6c14()) {
        case 1:
            if (kind < 0xD) {
                switch (func_ov003_020a6c7c(kind)) {
                    case 0:
                        value = FX_Mul(value << 12, 0x2000) >> 12;
                        break;
                    case 1:
                        value = FX_Mul(value << 12, 0x1800) >> 12;
                        break;
                    case 2:
                        value = FX_Mul(value << 12, 0x1333) >> 12;
                        break;
                    case 12:
                        value = FX_Mul(value << 12, 0x800) >> 12;
                        break;
                }
            }
            break;
    }
    return (s16)value;
}

/// The duration of one use of the actor's action at level `level`.
// Nonmatching: duration switch: case block layout.
s32 func_ov003_020a4908(Ov003PsychParams* params, u16 level) {
    s32 value = (params->unk_16 * (level - 1)) + params->unk_14;

    switch (params->unk_12) {
        case 2:
            if (func_ov003_020a3330(0x10) != 0) {
                value += func_ov003_020a3468(0x10);
            }
            if (value > 0xA) {
                value = 0xA;
            }
            break;
        case 1:
        case 3:
            if (func_ov003_020a3330(0x11) != 0) {
                s32 scale = FX_Mul(func_ov003_020a3468(0x11) << 12, value << 12);

                scale = FX_Divide(scale, 0x64000);
                if (scale < 0x1000) {
                    scale = 0x1000;
                }
                value += scale >> 12;
            }
            break;
    }
    return (s16)value;
}

// Nonmatching: the target narrows the result to s16 at the return, mwcc keeps it s32.
s32 func_ov003_020a49b4(Ov003PsychParams* params, u16 level) {
    return (params->unk_1C * (level - 1)) + params->unk_1A;
}

// Nonmatching: scaled-duration arithmetic shape (FX_Mul/FX_Divide nesting).
s32 func_ov003_020a49c8(Ov003PsychParams* params, u16 level) {
    s16 value = (params->unk_22 * (level - 1)) + params->unk_20;

    if (func_ov003_020a3330(0x49) != 0) {
        value -= FX_Divide(FX_Mul(value << 12, func_ov003_020a3468(0x49) << 12), 0x64000) >> 12;
    }
    return value;
}

/// The equipped-pin layout of player `deck`.
const EquippedPin* func_ov003_020a4a24(s32 deck) {
    return data_ov003_020dc054[deck];
}

/// Sets up the two sprite slots and the trail of the psych effect object.
void func_ov003_020a4a34(Ov003TrailEffect* effect) {
    effect->state = 0;
    effect->count = 0;
    effect->scale = 0x1F000;
    func_ov003_020a3498(&effect->spriteA, 2, 1, 0xB5, 1);
    func_ov003_02082730(&effect->spriteA, 0x3E7);
    func_ov003_020a3498(&effect->spriteB, 2, 1, 0xB6, 0);
    func_ov003_02082730(&effect->spriteB, 0x3E8);
}

/// Updates the psych effect trail; returns 0 when it is finished.
// Nonmatching: trail update: duplicated effect-position loads are scheduled differently.
s32 func_ov003_020a4aac(Ov003TrailEffect* effect) {
    s32 count;

    CombatSprite_Update(&effect->spriteA);
    CombatSprite_Update(&effect->spriteB);
    switch (effect->state) {
        case 0:
            effect->points[effect->count][0] = data_ov003_020e71b8->unk3D3AA;
            effect->points[effect->count][1] = data_ov003_020e71b8->unk3D3AC;
            effect->count++;
            if (func_ov003_02085cfc(6) == 0) {
                effect->state = 3;
                return 1;
            }
            count = effect->count;
            if (count > 2) {
                if (func_ov003_020a632c(effect->points[count - 1][0], effect->points[count - 1][1],
                                        data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC) > 0x6000)
                {
                    if (effect->count < 0x78) {
                        s16 x = effect->points[count - 1][0];
                        s16 y = effect->points[count - 1][1];

                        effect->points[count - 1][0]     = x + (data_ov003_020e71b8->unk3D3AA - x) / 2;
                        effect->points[count - 1][1]     = y + (data_ov003_020e71b8->unk3D3AC - y) / 2;
                        effect->points[effect->count][0] = data_ov003_020e71b8->unk3D3AA;
                        effect->points[effect->count][1] = data_ov003_020e71b8->unk3D3AC;
                        effect->count++;
                    }
                }
                if (func_ov003_020a4d6c(effect) != 0) {
                    func_ov003_02087f00(SEIDX_SE_PSY_FIREBALL01_02, effect->pivotX);
                    effect->state = 1;
                    return 1;
                }
                if (effect->count >= 0x78) {
                    effect->state = 3;
                    return 1;
                }
            }
            if (SndMgr_IsSEPlaying(0x45) == 0) {
                func_ov003_02087f00(SEIDX_SE_PSY_FIREBALL01_01, data_ov003_020e71b8->unk3D3AA);
            }
            return 1;
        case 1:
            effect->scale -= 0x1800;
            if (effect->scale < 0x1000) {
                effect->state = 2;
            }
            return 1;
        case 2:
            return 0;
        case 3:
            return 2;
    }
    return 1;
}

/// Renders the psych trail.
// Nonmatching: trail render loop register allocation.
void func_ov003_020a4c8c(Ov003TrailEffect* effect) {
    s32 i;

    for (i = 0; i < effect->count; i++) {
        CombatSprite_SetPosition(&effect->spriteA, effect->points[i][0], effect->points[i][1]);
        func_ov003_02082730(&effect->spriteA, 0x3B6000);
        if (effect->scale >= 0x1000) {
            if (effect->scale < 0x1F000) {
                CombatSprite_RenderWithPalette(&effect->spriteA, effect->scale >> 12);
            } else {
                CombatSprite_Render(&effect->spriteA);
            }
        }
    }
    if (effect->state != 0) {
        return;
    }
    CombatSprite_SetPosition(&effect->spriteB, data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC);
    func_ov003_02082730(&effect->spriteB, 0x3B7000);
    CombatSprite_Render(&effect->spriteB);
}

// Nonmatching: tail-call register allocation.
void func_ov003_020a4d50(Ov003TrailEffect* effect) {
    CombatSprite_Release(&effect->spriteA);
    CombatSprite_Release(&effect->spriteB);
}

/// Checks that the trail is long and straight enough to fire the psych.
s32 func_ov003_020a4d6c(Ov003TrailEffect* effect) {
    s16(*points)[2] = effect->points;
    s32 last        = effect->count - 1;
    s32 mid         = last / 2;
    s32 midX;
    s32 midY;
    s32 min = 0x3E8000;
    s32 max = 0;
    s32 dist;
    s32 i;

    if (func_ov003_020a632c(points[0][0], points[0][1], points[last][0], points[last][1]) > 0x10000) {
        return 0;
    }
    dist = func_ov003_020a632c(points[0][0], points[0][1], points[mid][0], points[mid][1]);
    if (dist < 0x30000 || dist > 0x100000) {
        return 0;
    }
    midX = points[0][0] + (points[mid][0] - points[0][0]) / 2;
    midY = points[0][1] + (points[mid][1] - points[0][1]) / 2;
    for (i = 0; i < last; i++) {
        dist = func_ov003_020a632c(midX, midY, points[i][0], points[i][1]);
        if (min > dist) {
            min = dist;
        }
        if (max < dist) {
            max = dist;
        }
    }
    if (max - min > 0x30000) {
        return 0;
    }
    effect->pivotX = midX;
    effect->pivotY = midY;
    return 1;
}

/// Binds camera `index` to the animation driven by `obj`.
void func_ov003_020a4e84(Ov003CameraAnim* obj, s32 index, s32 arg2) {
    obj->camera         = &data_ov003_020e71b8->unk3D7C0[index];
    obj->camera->unk_44 = 0;
    obj->camera->unk_48 = 0;
    obj->camera->unk_4C = 0;
    obj->timer          = -1;
    obj->index          = arg2;
}

void func_ov003_020a4ed0(Ov003CameraAnim* obj, s32 period, s32 offset) {
    obj->period = period;
    obj->timer  = period;
    obj->dir    = 0;
    obj->offset = offset;
}

/// Steps the camera shake animation; returns 1 when it hit a limit this frame.
s32 func_ov003_020a4ee8(Ov003CameraAnim* obj, s32 delta, s32 min, s32 max) {
    s32 hit = 0;
    s32 value;

    if (obj->timer == -1) {
        return 0;
    }
    if (--obj->timer == 0) {
        obj->timer = obj->period;
        obj->dir   = obj->dir == 0 ? 1 : 0;
        if (obj->index == 0) {
            obj->camera->unk_44 = (RNG_Next(3) - 1) << 12;
        } else {
            obj->camera->unk_48 = (RNG_Next(3) - 1) << 12;
        }
    }
    obj->offset += delta;
    if (obj->offset > max) {
        obj->offset = max;
        hit         = 1;
    } else if (obj->offset < min) {
        obj->offset = min;
        hit         = 1;
    }
    if (obj->dir != 0) {
        value = -obj->offset;
    } else {
        value = obj->offset;
    }
    if (obj->index == 0) {
        obj->camera->unk_48 = value;
    } else {
        obj->camera->unk_44 = value;
    }
    return hit;
}

void func_ov003_020a4fd0(Ov003CameraAnim* obj) {
    obj->camera->unk_44 = 0;
    obj->camera->unk_48 = 0;
    obj->camera->unk_4C = 0;
}

/// Sets up the 3D layer the touch action sprites are rendered through.
/// The layer block at `Ov003Global + 0x3D760`: 0x20 bytes of data followed by the parameters of the 3D layer.
typedef struct Ov003TouchLayer {
    /* 0x00 */ u8  data[0x20];
    /* 0x20 */ u32 mode;
    /* 0x24 */ u32 source;
    /* 0x28 */ u32 size;
    /* 0x2C */ u32 id;
    /* 0x30 */ u32 unk_30;
    /* 0x34 */ u32 callback;
    /* 0x38 */ u32 target;
    /* 0x3C */ u32 unk_3C;
    /* 0x40 */ u8  alpha;
} Ov003TouchLayer;

void func_ov003_020a4ff0(void) {
    Ov003TouchLayer* layer;
    s32              i;

    func_020405d4();
    func_02040ac0();
    layer = (Ov003TouchLayer*)data_ov003_020e71b8->touchLayerData;
    for (i = 0; i < 0x20; i++) {
        layer->data[i] = 0;
    }
    layer->mode     = 2;
    layer->source   = (u32)layer;
    layer->size     = 0x20;
    layer->id       = 0x105D;
    layer->unk_30   = 0;
    layer->callback = (u32)&func_ov003_020a50f4;
    layer->target   = (u32)layer;
    func_02041030(1);
    func_02041048(1);
    layer->alpha = 0xB4;
}
