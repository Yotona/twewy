#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "Engine/Math/Rect.h"
#include "Save.h"
#include "SndMgr.h"

#include <nitro/fx.h>

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The 3D motion track of a cut-in actor pair: an engine id plus two 20.12 positions.
/// `func_ov003_02084634` seeds it, `func_ov003_020846ac` retargets it and
/// `func_ov003_020846c0` integrates the offsets at 0x20/0x24 into the actor position.

/// The per-actor tuning record fetched from `data_ov003_020e70ac[group][sprite]`.

/// Four entries of `data_ov003_020e071c`/`data_ov003_020e0720` (0xC bytes per actor group).
typedef struct Ov003CutinCmdIds {
    /* 0x0 */ s32 main;
    /* 0x4 */ s32 sub;
    /* 0x8 */ s32 extra;
} Ov003CutinCmdIds; // Size: 0xC

/// The four adjacent .bss objects at 0x020e71e4..0x020e7204, as one block.

/// The engine-wide block at 0x02071cf0; only the field this unit polls is named.
typedef struct Ov02071cf0State {
    /* 0x000 */ u8  unk_000[0x130];
    /* 0x130 */ s32 unk_130;
} Ov02071cf0State;

// ---------------------------------------------------------------------------------------------
// Data owned by this unit
// ---------------------------------------------------------------------------------------------

char data_ov003_020e69c0[0x1C] = "Apl_Mor/Grp_BtlBoss16.bin";
char data_ov003_020e69dc[0x1C] = "Apl_Mor/Grp_BtlBoss15.bin";
char data_ov003_020e69f8[0x1C] = "Apl_Mor/Grp_BtlBoss15b.bin";
char data_ov003_020e6a14[0x1C] = "Apl_Mor/Grp_BtlBoss16b.bin";

/// Sprite resource descriptors: a resource kind plus the pack the sprite is loaded from.
typedef struct Ov003CutinSpriteRes {
    /* 0x0 */ u32         type;
    /* 0x4 */ const char* path;
} Ov003CutinSpriteRes; // Size: 0x8

const Ov003CutinSpriteRes data_ov003_020e06f4    = {3, data_ov003_020e69c0};
const Ov003CutinSpriteRes data_ov003_020e06fc    = {3, data_ov003_020e6a14};
const Ov003CutinSpriteRes data_ov003_020e0704    = {3, data_ov003_020e69dc};
const u32                 data_ov003_020e070c[4] = {3, (u32)data_ov003_020e69f8, 0x14, 0x00010000};

const u32 data_ov003_020e071c[1]    = {0x56C};
const u32 data_ov003_020e0720[1]    = {0x18F};
const u32 data_ov003_020e0724[0xB5] = {
    0x191, 0x181, 0x188, 0x189, 0x192, 0x199, 0x19A, 0x19B, 0x1A0, 0x1A1, 0x1B1, 0x1A9, 0x1AA, 0x1C6, 0x1C7, 0x1C8, 0x1D2,
    0x1D4, 0x1D5, 0x1E6, 0x1E7, 0x1E8, 0x1F6, 0x1F7, 0x1F8, 0x204, 0x206, 0x207, 0x215, 0x216, 0x217, 0x21F, 0x221, 0x222,
    0x229, 0x22A, 0x22B, 0x238, 0x23A, 0x23B, 0x56C, 0x243, 0x244, 0x254, 0x255, 0x256, 0x266, 0x267, 0x268, 0x275, 0x276,
    0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275,
    0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277, 0x275, 0x276, 0x277,
    0x275, 0x276, 0x277, 0x297, 0x56C, 0x56C, 0x282, 0x283, 0x284, 0x56C, 0x1A9, 0x1AA, 0x56C, 0x1B9, 0x1BA, 0x56C, 0x56C,
    0x56C, 0x2F2, 0x56C, 0x56C, 0x2F2, 0x56C, 0x56C, 0x2F2, 0x56C, 0x56C, 0x2F2, 0x56C, 0x56C, 0x2F2, 0x56C, 0x56C, 0x2F2,
    0x56C, 0x56C, 0x2F2, 0x56C, 0x56C, 0x2F2, 0x56C, 0x56C, 0x2D6, 0x56C, 0x56C, 0x2D6, 0x56C, 0x56C, 0x2ED, 0x56C, 0x56C,
    0x2EE, 0x56C, 0x56C, 0x2ED, 0x56C, 0x56C, 0x56C, 0x56C, 0x2C5, 0x56C, 0x56C, 0x56C, 0x56C, 0x56C, 0x56C, 0x56C, 0x56C,
    0x56C, 0x56C, 0x56C, 0x56C, 0x2B7, 0x56C, 0x56C, 0x304, 0x56C, 0x56C, 0x29E, 0x56C, 0x56C, 0x29E, 0x56C, 0x56C, 0x2AC,
    0x56C, 0x56C, 0x2AC, 0x56C, 0x56C, 0x165, 0x56C, 0x56C, 0x56C, 0x56C, 0x56C,
};

const u8 data_ov003_020e09f8[0x40] = {
    0x07, 0x07, 0x08, 0x09, 0x0A, 0x0C, 0x0D, 0x0E, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x19, 0x1B, 0x1A,
    0x0A, 0x0B, 0x0F, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x21, 0x20, 0x20, 0x20, 0x20, 0x20,
    0x1F, 0x23, 0x23, 0x24, 0x24, 0x1E, 0x22, 0x1C, 0x1C, 0x1D, 0x1D, 0x04, 0x00, 0x00, 0x00, 0x00,
};

u32 data_ov003_020e7204[0xA];
u32 data_ov003_020e722c[0xA];

// ---------------------------------------------------------------------------------------------
// Data owned by neighbouring units / arm9
// ---------------------------------------------------------------------------------------------

extern u8               data_ov003_020e6a30[];
extern const TaskHandle data_ov003_020e0b74;
extern void**           data_ov003_020e6a44[];

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern void func_02026590(s32* dst, s32 target, u16 weight);

extern s32  func_ov003_0208a028(s32 mode, Ov003EnemyActor* unit, u16 attackId);
extern void func_ov003_0208a234(s32 arg0, s32 arg1, CombatActor* actor);
extern s32  func_ov003_0208a25c(CombatActor* actor);
extern void func_ov003_0208c970(s32 engine, s16 arg1);
extern void func_ov003_0208c9e0(s32 engine, s16 arg1);
extern BOOL func_ov003_0208ca04(s32 arg0, u32 arg1);

// Defined by the previous unit (0x020c2064..0x020c41e8).

// Defined by the following unit (0x020c703c..).

// ---------------------------------------------------------------------------------------------
// Forward declarations (the retail unit defines these in address order)
// ---------------------------------------------------------------------------------------------

void func_ov003_020c439c(Ov003EnemyActor* actor, SpriteAnimationEx* anim);
void func_ov003_020c4428(Ov003CutinActor* actor, SpriteAnimationEx* anim);

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Resets a cut-in actor to its idle defaults (a fresh 0x1C0-byte actor block).
void func_ov003_020c41e8(Ov003CutinActor* actor) {
    MI_CpuSet(actor, 0U, 0x1C0U);
    CombatActor_Init(&actor->base.actor, 1);
    actor->base.actor.flags |= 0x20000000;
    actor->base.actor.maxHp     = 1;
    actor->base.actor.currentHp = 1;
    actor->base.group           = 0x3C;
    actor->base.unk_192         = 0x64;
    actor->base.unk_194         = 0x64;
    actor->base.flags196.raw    = (u16)((actor->base.flags196.raw & ~1) | 1);
    actor->base.flags196.raw    = (u16)(actor->base.flags196.raw | 0x3C);
    actor->base.unk_1B8         = 0x1000;
    actor->base.unk_1BC         = 0x1000;
}

void func_ov003_020c4264(Ov003EnemyActor* actor) {}

/// Releases the main sprite's resources.
void func_ov003_020c4268(Ov003CutinActor* actor) {
    CombatSprite_SetPaletteMode(&actor->base.sprite, 0);
}

void func_ov003_020c427c(Ov003EnemyActor* actor) {
    func_ov003_020c37bc(actor);
}

void func_ov003_020c4288(Ov003EnemyActor* actor) {
    actor->actor.flags &= ~0x80000000;
    if (actor->sprite.paletteMode == 1) {
        CombatSprite_SetPaletteMode(&actor->sprite, 0);
    }
    actor->flags18C &= ~1;
    actor->unk_1B8 = 0x1000;
    actor->unk_1BC = 0x1000;
}

void func_ov003_020c42d4(Ov003CutinActor* actor) {
    MI_CpuSet(&actor->base.actor.unk_0C, 0U, 8U);
}

/// Random rise speed for the sub (0x19E) or main (0x198) partner.
s16 func_ov003_020c42ec(Ov003EnemyActor* actor) {
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        return (s16)(actor->unk_19E + RNG_Next((u32)actor->unk_1A0));
    }
    return (s16)(actor->unk_198 + RNG_Next((u32)actor->unk_19A));
}

/// Rolls the cut-in's hit chance for the active partner.
s32 func_ov003_020c4348(Ov003EnemyActor* actor) {
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        if ((s32)RNG_Next(0x64U) < (s32)actor->unk_1A2) {
            return 1;
        }
        return 0;
    }
    if ((s32)RNG_Next(0x64U) < (s32)actor->unk_19C) {
        return 1;
    }
    return 0;
}

/// Fills the sprite-load template `anim` from the actor's load parameters.
void func_ov003_020c439c(Ov003EnemyActor* actor, SpriteAnimationEx* anim) {
    const BtlSpriteDesc*   params = func_ov003_020c3c6c(actor);
    const SpriteAnimEntry* entry;

    CombatSprite_InitAnim(&anim->anim, actor->sprite.sprite.bits_0_1, params->iden);
    entry                 = &params->table[params->entry];
    anim->anim.bits_10_11 = 1;
    anim->anim.unk_1C     = entry->charDataIndex;
    anim->anim.unk_1E     = (s16)(params->paletteCount << 5);
    anim->anim.unk_26     = entry->frameDataIndex;
    anim->anim.unk_28     = entry->paletteDataIndex;
    anim->anim.unk_20     = params->palette;
    anim->anim.animIndex  = (s16)(entry->animDataIndex + 1);
    anim->animTable       = params->table;
}

/// Same as `func_ov003_020c439c`, but without the frame offset/animation index overrides.
void func_ov003_020c4428(Ov003CutinActor* actor, SpriteAnimationEx* anim) {
    const BtlSpriteDesc*   params = func_ov003_020c3c6c(&actor->base);
    const SpriteAnimEntry* entry  = &params->table[params->entry];

    CombatSprite_InitAnim(&anim->anim, actor->base.sprite.sprite.bits_0_1, params->iden);
    anim->anim.bits_10_11 = 0;
    anim->anim.unk_1C     = entry->charDataIndex;
    anim->anim.unk_26     = entry->frameDataIndex;
    anim->anim.unk_28     = entry->paletteDataIndex;
    anim->anim.unk_20     = params->palette;
    anim->anim.animIndex  = (s16)(entry->animDataIndex + 1);
    anim->animTable       = NULL;
}

s32 func_ov003_020c44a4(Ov003EnemyActor* actor) {
    return 1;
}

/// Loads the main sprite; returns the sprite-load result (0 on failure).
s32 func_ov003_020c44ac(Ov003EnemyActor* actor) {
    SpriteAnimationEx anim;
    s32               result;

    func_ov003_020c439c(actor, &anim);
    anim.anim.bits_0_1 = 1;
    result             = CombatSprite_Load(&actor->sprite, &anim);
    if (result == 0) {
        return 0;
    }
    func_ov003_020c4264(actor);
    func_ov003_020880e4(1, &actor->subSprite, actor);
    func_ov003_020c4988(actor);
    func_ov003_020c44a4(actor);
    return result;
}

/// Loads the main sprite and seeds the actor's motion track.
s32 func_ov003_020c4520(Ov003EnemyActor* actor) {
    Ov003ActorParams* params = func_ov003_020c3c88(actor);
    SpriteAnimationEx anim;
    s32               result;

    func_ov003_020c439c(actor, &anim);
    anim.anim.bits_0_1 = 0;
    result             = CombatSprite_Load(&actor->sprite, &anim);
    if (result == 0) {
        return 0;
    }
    func_ov003_020880e4(0, &actor->subSprite, actor);
    func_ov003_020c49c8(actor);
    func_ov003_02084634(&actor->collider, 1, params->unk_03, params->unk_04);
    func_ov003_020c44a4(actor);
    return result;
}

/// Same as `func_ov003_020c4520`, using the secondary load template.
s32 func_ov003_020c45a4(Ov003CutinActor* actor) {
    Ov003ActorParams* params = func_ov003_020c3c88(&actor->base);
    SpriteAnimationEx anim;
    s32               result;

    func_ov003_020c4428(actor, &anim);
    anim.anim.bits_0_1 = 0;
    result             = CombatSprite_Load(&actor->base.sprite, &anim);
    if (result == 0) {
        return 0;
    }
    func_ov003_020880e4(0, &actor->base.subSprite, &actor->base);
    func_ov003_020c49c8(&actor->base);
    func_ov003_02084634(&actor->base.collider, 1, params->unk_03, params->unk_04);
    func_ov003_020c44a4(&actor->base);
    return result;
}

void func_ov003_020c4628(Ov003EnemyActor* actor) {
    CombatActor_UpdateEffects(1, &actor->actor);
    if (actor->actor.zGravity == 0) {
        actor->actor.zVelocity = 0;
    }
    CombatActor_UpdatePhysics(&actor->actor);
    CombatSprite_Update(&actor->sprite);
    CombatSprite_Update(&actor->subSprite);
}

void func_ov003_020c4668(Ov003EnemyActor* actor) {
    s16 half;

    CombatActor_UpdateEffects(0, &actor->actor);
    if (actor->actor.zGravity == 0) {
        actor->actor.zVelocity = 0;
    }
    CombatActor_UpdatePhysics(&actor->actor);
    if (!(actor->flags18C & 4) && (actor->collider.flags != 0)) {
        func_ov003_020846c0(&actor->collider, &actor->actor.position.x, &actor->actor.position.y, &actor->actor.position.z);
    }
    if (func_ov003_020c59f4(actor) == 0) {
        if (actor->actor.flags & 0x10000000) {
            s16 v = actor->actor.unk_70;
            func_ov003_02083bfc(&actor->actor.position.y, (v / 2) << 0xC, actor->actor.position.x);
        } else {
            s16 v = actor->actor.unk_70;
            func_ov003_02083ca0(&actor->actor.position.x, &actor->actor.position.y, v << 0xC, (v / 2) << 0xC);
        }
    }
    half = 0;
    func_ov003_020846ac(&actor->collider, actor->actor.position.x, actor->actor.position.y, actor->actor.position.z);
    CombatSprite_Update(&actor->sprite);
    CombatSprite_Update(&actor->subSprite);
}

/// Registers (or re-budgets) the sprite's OAM affine group from the actor's scale fields.
void func_ov003_020c4748(Ov003EnemyActor* actor) {
    s32 scaleX = actor->unk_1B8;
    s32 flip;

    if ((scaleX != 0x1000) || (actor->unk_1BC != 0x1000)) {
        flip                            = (actor->sprite.flags46 & 1) ? 1 : 0;
        actor->sprite.sprite.unk_0A.raw = (u16)((actor->sprite.sprite.unk_0A.raw & ~0x3E0) |
                                                ((u32)(u16)OamMgr_AllocAffineGroup(&g_OamMgr[actor->sprite.sprite.bits_0_1],
                                                                                   0U, scaleX, actor->unk_1BC, flip)
                                                 << 0x1B) >>
                                                    0x16);
        actor->sprite.sprite.unk_0A.raw = (u16)((actor->sprite.sprite.unk_0A.raw & ~1) | 1);
        actor->sprite.sprite.unk_0A.raw = (u16)(actor->sprite.sprite.unk_0A.raw & ~2);
        return;
    }
    actor->sprite.sprite.unk_0A.raw &= ~0x3E0;
    actor->sprite.sprite.unk_0A.raw &= ~1;
    if (actor->sprite.flags46 & 1) {
        CombatSprite_SetFlip(&actor->sprite, 1);
    } else {
        CombatSprite_SetFlip(&actor->sprite, 0);
    }
}

/// True while the actor is hidden or either scale is (nearly) zero.
s32 func_ov003_020c4830(Ov003EnemyActor* actor) {
    s32 scale;

    if (actor->flags18C & 1) {
        return 1;
    }
    scale = actor->unk_1B8;
    if (scale < 0) {
        scale = -scale;
    }
    if (scale < 0x10) {
        return 1;
    }
    scale = actor->unk_1BC;
    if (scale < 0) {
        scale = -scale;
    }
    if (scale < 0x10) {
        return 1;
    }
    return 0;
}

void func_ov003_020c4878(void* arg) {
    Ov003CutinActor* actor = arg;

    if (func_ov003_020c4830(&actor->base) != 0) {
        return;
    }
    func_ov003_020c4748(&actor->base);
    CombatActor_Render(&actor->base.actor, &actor->base.sprite);
    func_ov003_0208810c(&actor->base.subSprite, &actor->base.actor);
}

void func_ov003_020c48b0(Ov003EnemyActor* actor) {
    s32 flags = actor->actor.flags;

    if (flags & 1) {
        actor->actor.flags = flags & ~1;
    }
    if (func_ov003_020c4830(actor) != 0) {
        return;
    }
    func_ov003_020c4748(actor);
    CombatActor_Render(&actor->actor, &actor->sprite);
    func_ov003_0208810c(&actor->subSprite, &actor->actor);
}

void func_ov003_020c48fc(void* arg) {
    Ov003CutinActor* actor = arg;

    func_ov003_020c4ee0(&actor->base);
    func_ov003_020c4a34(&actor->base);
    func_ov003_020c4268(actor);
    CombatSprite_Release(&actor->base.sprite);
    CombatSprite_Release(&actor->base.subSprite);
}

void func_ov003_020c492c(Ov003EnemyActor* actor) {
    func_ov003_020c4f00(actor);
    func_ov003_020c4a74(actor);
    CombatSprite_Release(&actor->sprite);
    CombatSprite_Release(&actor->subSprite);
    func_ov003_02084678(&actor->collider);
}

void func_ov003_020c495c(Ov003EnemyActor* actor) {
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        func_ov003_020c4988(actor);
        return;
    }
    func_ov003_020c49c8(actor);
}

void func_ov003_020c4988(Ov003EnemyActor* actor) {
    if (actor->flags18C & 2) {
        return;
    }
    actor->flags18C = (u16)(actor->flags18C | 2);
    func_ov003_020849bc(&data_ov003_020e71b8->unk3D8AC, &actor->node, actor);
}

void func_ov003_020c49c8(Ov003EnemyActor* actor) {
    if (actor->flags18C & 2) {
        return;
    }
    actor->flags18C = (u16)(actor->flags18C | 2);
    func_ov003_020849bc(&data_ov003_020e71b8->unk3D8A4, &actor->node, actor);
}

void func_ov003_020c4a08(Ov003CutinActor* actor) {
    if (func_ov003_020c37f8(&actor->base.sprite) != 0) {
        func_ov003_020c4a34(&actor->base);
        return;
    }
    func_ov003_020c4a74(&actor->base);
}

s32 func_ov003_020c4a34(Ov003EnemyActor* actor) {
    if (!(actor->flags18C & 2)) {
        return 0;
    }
    actor->flags18C = (u16)(actor->flags18C & ~2);
    return (s32)func_ov003_020849dc(&data_ov003_020e71b8->unk3D8AC, &actor->node);
}

s32 func_ov003_020c4a74(Ov003EnemyActor* actor) {
    if (!(actor->flags18C & 2)) {
        return 0;
    }
    actor->flags18C = (u16)(actor->flags18C & ~2);
    return (s32)func_ov003_020849dc(&data_ov003_020e71b8->unk3D8A4, &actor->node);
}

/// Faces the main sprite towards (1) or away from (0) the sub screen.
void func_ov003_020c4ab4(Ov003EnemyActor* actor, s32 arg1) {
    if (arg1 != 0) {
        CombatSprite_SetFlip(&actor->sprite, 1);
        actor->actor.isFlipped = 1;
        return;
    }
    CombatSprite_SetFlip(&actor->sprite, 0);
    actor->actor.isFlipped = 0;
}

void func_ov003_020c4af0(Ov003EnemyActor* actor) {
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        func_ov003_020c4b1c(actor);
        return;
    }
    func_ov003_020c4b5c(actor);
}

/// Declared `int`: noise callers treat it like an undeclared (implicitly `int`) function.
s32 func_ov003_020c4b1c(Ov003EnemyActor* actor) {
    if (actor->actor.position.x < ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x) {
        func_ov003_020c4ab4(actor, 1);
        return;
    }
    func_ov003_020c4ab4(actor, 0);
}

void func_ov003_020c4b5c(Ov003EnemyActor* actor) {
    if (actor->actor.position.x < ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x) {
        func_ov003_020c4ab4(actor, 1);
        return;
    }
    func_ov003_020c4ab4(actor, 0);
}

void func_ov003_020c4b9c(Ov003CutinActor* actor) {
    if (actor->base.actor.position.x < ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x) {
        func_ov003_020c4ab4(&actor->base, 0);
        return;
    }
    func_ov003_020c4ab4(&actor->base, 1);
}

void func_ov003_020c4bdc(Ov003CutinActor* actor) {
    if (actor->base.actor.position.x < ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x) {
        func_ov003_020c4ab4(&actor->base, 0);
        return;
    }
    func_ov003_020c4ab4(&actor->base, 1);
}

void func_ov003_020c4c1c(Ov003EnemyActor* actor) {
    if (actor->actor.position.x < data_ov003_020e71b8->unk3D7C0[1].unk_0C / 2) {
        func_ov003_020c4ab4(actor, 1);
        return;
    }
    func_ov003_020c4ab4(actor, 0);
}

void func_ov003_020c4c5c(Ov003EnemyActor* actor) {
    if (actor->actor.position.x < data_ov003_020e71b8->unk3D7C0[0].unk_0C / 2) {
        func_ov003_020c4ab4(actor, 1);
        return;
    }
    func_ov003_020c4ab4(actor, 0);
}

void func_ov003_020c4c9c(Ov003EnemyActor* actor) {
    if (actor->sprite.flags46 & 1) {
        func_ov003_020c4ab4(actor, 0);
        return;
    }
    func_ov003_020c4ab4(actor, 1);
}

/// Plays a cut-in sound effect, ignoring the "no command" sentinel 0x56C.
void func_ov003_020c4cc4(Ov003EnemyActor* actor, s32 command) {
    if (command == 0x56C) {
        return;
    }
    func_ov003_020c3acc(actor->sprite.sprite.bits_0_1, command, actor->actor.position.x);
}

/// Returns 1 if `value` is already in `table`.
s32 func_ov003_020c4cf0(s32 value, s32* table, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (value == table[i]) {
            return 1;
        }
    }
    return 0;
}

/// Stores `value` into the first free slot of `table`.
s32 func_ov003_020c4d20(s32 value, s32* table, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (table[i] == 0) {
            table[i] = value;
            return 1;
        }
    }
    return 0;
}

/// Clears every slot of `table` holding `value`.
void func_ov003_020c4d54(s32 value, s32* table, s32 count) {
    s32 i;

    for (i = 0; i < count; i++) {
        if (value == table[i]) {
            table[i] = 0;
        }
    }
}

void func_ov003_020c4d84(void) {
    func_ov003_020c4cf0(0, (s32*)data_ov003_020e722c, (s32)g_Cutin->unk_02);
}

void func_ov003_020c4da8(void) {
    func_ov003_020c4cf0(0, (s32*)data_ov003_020e7204, (s32)g_Cutin->unk_03);
}

s32 func_ov003_020c4dcc(s32 value) {
    func_ov003_020c4cf0(value, (s32*)data_ov003_020e722c, (s32)g_Cutin->unk_02);
}

s32 func_ov003_020c4dec(s32 value) {
    return func_ov003_020c4cf0(value, (s32*)data_ov003_020e7204, (s32)g_Cutin->unk_03);
}

s32 func_ov003_020c4e0c(s32 value) {
    u8 partner = data_ov003_020e71b8->unk3D88D;

    if ((partner != 0) && (partner != 1) && (partner != 2)) {
        return 0;
    }
    if (func_ov003_020c4dcc(value) != 0) {
        return 1;
    }
    return func_ov003_020c4d20(value, (s32*)data_ov003_020e722c, (s32)g_Cutin->unk_02);
}

s32 func_ov003_020c4e7c(s32 value) {
    if (func_ov003_020c4dec(value) != 0) {
        return 1;
    }
    return func_ov003_020c4d20(value, (s32*)data_ov003_020e7204, (s32)g_Cutin->unk_03);
}

void func_ov003_020c4eb4(Ov003CutinActor* actor) {
    if (func_ov003_020c37f8(&actor->base.sprite) != 0) {
        func_ov003_020c4ee0(&actor->base);
        return;
    }
    func_ov003_020c4f00(&actor->base);
}

/// Retires the actor's sub-screen command slot.
void func_ov003_020c4ee0(Ov003EnemyActor* actor) {
    func_ov003_020c4d54((s32)actor, (s32*)data_ov003_020e722c, (s32)g_Cutin->unk_02);
}

/// Retires the actor's main-screen command slot.
void func_ov003_020c4f00(Ov003EnemyActor* actor) {
    func_ov003_020c4d54((s32)actor, (s32*)data_ov003_020e7204, (s32)g_Cutin->unk_03);
}

/// True if any actor on `list` has cut-in instance id `id`.
s32 func_ov003_020c4f20(s32 id, CombatList* list) {
    Ov003CutinActor* entry = func_ov003_0208495c(list);

    while (entry != NULL) {
        if (entry->base.group == id) {
            return 1;
        }
        entry = func_ov003_02084984(&entry->base.node);
    }
    return 0;
}

void func_ov003_020c4f60(s32 id) {
    func_ov003_020c4f20(id, &data_ov003_020e71b8->unk3D8AC);
}

void func_ov003_020c4f80(s32 id) {
    func_ov003_020c4f20(id, &data_ov003_020e71b8->unk3D8A4);
}

/// Knocks the actor out: zero HP, defeat command, "defeated" flag.
void func_ov003_020c4fa0(Ov003EnemyActor* actor) {
    actor->actor.currentHp = 0;
    CombatActor_SetPendingCommand(&actor->actor, 3);
    actor->actor.flags |= 4;
}

void func_ov003_020c4fc8(Ov003EnemyActor* actor) {
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        func_ov003_020c4ff4(actor);
        return;
    }
    func_ov003_020c5058(actor);
}

s32 func_ov003_020c4ff4(Ov003EnemyActor* actor) {
    if (actor->actor.flags & 4) {
        return 0;
    }
    func_ov003_020c4fa0(actor);
    func_ov003_020c4ee0(actor);
    if (actor->partner != NULL) {
        func_ov003_020c4fa0(actor->partner);
        func_ov003_020c4f00(actor->partner);
    }
    actor->actor.unk_5A           = 5;
    data_ov003_020e71b8->unk3D90A = 5;
    return 1;
}

s32 func_ov003_020c5058(Ov003EnemyActor* actor) {
    if (actor->actor.flags & 4) {
        return 0;
    }
    func_ov003_020c4fa0(actor);
    func_ov003_020c4f00(actor);
    if (actor->partner != NULL) {
        func_ov003_020c4fa0(actor->partner);
        func_ov003_020c4ee0(actor->partner);
    }
    actor->actor.unk_5A           = 5;
    data_ov003_020e71b8->unk3D906 = 5;
    return 1;
}

void func_ov003_020c50bc(void) {
    Ov003CutinActor* entry;
    Ov003CutinActor* next;

    entry = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    while (entry != NULL) {
        func_ov003_020c5058(&entry->base);
        entry = func_ov003_02084984(&entry->base.node);
    }
    next = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    while (next != NULL) {
        func_ov003_020c4ff4(&next->base);
        next = func_ov003_02084984(&next->base.node);
    }
}

/// Pushes the actor's hit effects to the combo counters.
void func_ov003_020c5130(Ov003EnemyActor* actor) {
    if (gSaveData.unk_3130 == 1) {
        return;
    }
    func_ov003_0208c970(actor->sprite.sprite.bits_0_1, actor->unk_18E);
    func_ov003_0208c9e0(actor->sprite.sprite.bits_0_1, actor->unk_190);
    if (func_ov003_0208ca04(actor->sprite.sprite.bits_0_1, 0x1D) != 0) {
        return;
    }
    func_ov003_020bb580(actor);
}

/// Nudges the actor one eighth of the way towards (x, y), clamped to +/-0x8000.
void func_ov003_020c51a4(Ov003CutinActor* actor, s32 x, s32 y) {
    s32 stepX = (x - actor->base.actor.position.x) / 8;
    s32 stepY = (y - actor->base.actor.position.y) / 8;

    if (stepX > 0x8000) {
        stepX = 0x8000;
    } else if (stepX < -0x8000) {
        stepX = -0x8000;
    }
    if (stepY > 0x8000) {
        stepY = 0x8000;
    } else if (stepY < -0x8000) {
        stepY = -0x8000;
    }
    actor->base.actor.position.x += stepX;
    actor->base.actor.position.y += stepY;
}

/// True if the actor and its partner swapped sides of the sub-screen centre.
s32 func_ov003_020c5220(Ov003EnemyActor* actor) {
    s32 x      = actor->actor.position.x;
    s32 centre = data_ov003_020e71b8->unk3D7C0[1].unk_0C / 2;
    s32 lower  = centre - 0x10000;

    if ((x < lower) && (actor->unk_1AC > centre + 0x10000)) {
        return 1;
    }
    if ((x > centre + 0x10000) && (actor->unk_1AC < lower)) {
        return 1;
    }
    return 0;
}

/// True if the actor's box does not overlap any actor on the sub-screen list.
s32 func_ov003_020c5288(Ov003EnemyActor* actor, s32 x, s32 y) {
    s32              hitbox[4];
    s32              other[4];
    s32              result;
    Ov003CutinActor* entry;
    s16              width;
    s16              width2;

    width     = actor->actor.unk_70;
    hitbox[3] = FX32_CONVERT(width);
    hitbox[2] = FX32_CONVERT(width) * 2;
    hitbox[1] = y - FX32_CONVERT(width) / 2;
    hitbox[0] = x - FX32_CONVERT(width);

    entry = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    while (entry != NULL) {
        width2   = entry->base.actor.unk_70;
        other[3] = FX32_CONVERT(width2);
        other[2] = FX32_CONVERT(width2) * 2;
        other[1] = entry->base.unk_1B0 - FX32_CONVERT(width2) / 2;
        other[0] = entry->base.unk_1AC - FX32_CONVERT(width2);
        if (Rect_Intersect(&result, hitbox, other) != 0) {
            return 0;
        }
        entry = func_ov003_02084984(&entry->base.node);
    }
    return 1;
}

/// Counts the sub-screen cut-in actors, and how many of them are "large" (width > 0x40).
void func_ov003_020c54e0(s32* over, s32* total) {
    Ov003CutinActor* entry;

    *over  = 0;
    *total = 0;
    entry  = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);
    while (entry != NULL) {
        if (entry->base.actor.unk_72 > 0x40) {
            (*over)++;
        }
        (*total)++;
        entry = func_ov003_02084984(&entry->base.node);
    }
}

/// Picks a randomised rise speed for the cut-in, scaled by how crowded the sub screen is.
void func_ov003_020c554c(Ov003EnemyActor* actor, s32* outX, s32* outY, u16 arg3) {
    s32 over;
    s32 total;
    s32 maxSpeed;
    s32 minSpeed;
    s32 speed;
    s32 centre;
    s32 size;
    u16 flags196;

    if (arg3 != 0) {
        maxSpeed = (data_ov003_020e71b8->unk3D7C0[1].unk_0C / 4) >> 12;
    } else {
        maxSpeed = (data_ov003_020e71b8->unk3D7C0[1].unk_0C / 4 * 3) >> 12;
    }
    flags196 = actor->flags196.raw;
    if ((((u32)flags196 << 0x19) >> 0x1F) != 0 && (((u32)flags196 << 0x18) >> 0x1F) == 0) {
        if (arg3 != 0) {
            *outX = FX32_CONVERT(maxSpeed + RNG_Next(0x14U));
        } else {
            *outX = FX32_CONVERT(maxSpeed - RNG_Next(0x14U));
        }
    } else if ((((u32)flags196 << 0x19) >> 0x1F) == 0 && (((u32)actor->flags196.raw << 0x18) >> 0x1F) != 0) {
        if (arg3 != 0) {
            *outX = FX32_CONVERT(maxSpeed - RNG_Next(0x14U));
        } else {
            *outX = FX32_CONVERT(maxSpeed + RNG_Next(0x14U));
        }
    } else {
        *outX = FX32_CONVERT((maxSpeed + RNG_Next(0x28U)) - 0x14);
    }

    func_ov003_020c54e0(&over, &total);
    size     = data_ov003_020e71b8->unk3D7C0[1].unk_10;
    centre   = actor->actor.unk_70 / 2;
    speed    = ((size * over) / total) >> 0xC;
    minSpeed = size >> 0xC;
    if (speed < centre) {
        speed = centre;
    }
    if (speed > minSpeed) {
        speed = minSpeed;
    }
    if (actor->actor.unk_72 > 0x40) {
        *outY = (centre + RNG_Next((speed - centre) + 1)) << 0xC;
        return;
    }
    *outY = (speed + RNG_Next((minSpeed - speed) + 1)) << 0xC;
}

s32 func_ov003_020c5864(Ov003CutinActor* actor, s32* outX, s32* outY) {
    s32 attempts = 0;

    while (1) {
        func_ov003_020c554c(&actor->base, outX, outY, RNG_Next(2U));
        if (func_ov003_020c5288(&actor->base, *outX, *outY) != 0) {
            return 1;
        }
        attempts++;
        if (attempts >= 0x64) {
            return 0;
        }
    }
}

s32 func_ov003_020c58c8(Ov003CutinActor* actor, s32* outX, s32* outY, u16 arg3) {
    s32 attempts = 0;

    while (1) {
        func_ov003_020c554c(&actor->base, outX, outY, arg3);
        if (func_ov003_020c5288(&actor->base, *outX, *outY) != 0) {
            return 1;
        }
        attempts++;
        if (attempts >= 0x64) {
            return 0;
        }
    }
}

s32 func_ov003_020c5924(Ov003EnemyActor* actor, s32* out) {
    s32 attempts = 0;
    u16 dir;

    while (1) {
        dir = actor->unk_1AC > data_ov003_020e71b8->unk3D7C0[1].unk_0C / 2;
        func_ov003_020c554c(actor, out, out + 1, dir);
        if (func_ov003_020c5288(actor, out[0], out[1]) != 0) {
            return 1;
        }
        attempts++;
        if (attempts >= 0x64) {
            return 0;
        }
    }
}

s32 func_ov003_020c59a0(Ov003EnemyActor* actor) {
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        return func_ov003_020c59cc(actor);
    }
    return func_ov003_020c59e0(actor);
}

s32 func_ov003_020c59cc(Ov003EnemyActor* actor) {
    return actor->actor.flags & 0x4006;
}

s32 func_ov003_020c59e0(Ov003EnemyActor* actor) {
    return actor->actor.flags & 0x4606;
}

s32 func_ov003_020c59f4(Ov003EnemyActor* actor) {
    return actor->actor.flags & 0x200;
}

s32 func_ov003_020c5a00(Ov003CutinActor* actor) {
    actor->base.actor.flags |= 0x200;
    return actor->base.actor.flags;
}

s32 func_ov003_020c5a14(Ov003CutinActor* actor) {
    actor->base.actor.flags &= ~0x200;
    return actor->base.actor.flags;
}

s32 func_ov003_020c5a28(Ov003CutinActor* actor) {
    if ((actor->base.actor.unk_5A > 0) || (actor->base.actor.flags & 0x200) || (actor->base.actor.flags & 4) ||
        (actor->base.actor.flags & 0x10))
    {
        return 0;
    }
    return 1;
}

s32 func_ov003_020c5a60(Ov003CutinActor* actor) {
    if ((actor->base.actor.unk_5A > 0) || (actor->base.actor.flags & 0x200) || (actor->base.actor.flags & 4) ||
        (actor->base.actor.flags & 0x10))
    {
        return 0;
    }
    return 1;
}

void func_ov003_020c5a98(Ov003CutinActor* actor) {
    actor->base.actor.unk_58 = 0;
    CombatActor_SetPendingCommand(&actor->base.actor, 5);
    func_ov003_02082fe0(&actor->base.actor);
}

void func_ov003_020c5ac0(s32 arg0, s32 arg1, Ov003CutinActor* actor, s32 arg3, s32 arg4, s32 arg5) {
    actor->base.actor.xVelocity = arg3;
    actor->base.actor.yVelocity = arg4;
    actor->base.actor.zVelocity = arg5;
    actor->base.actor.unk_58    = 0;
    CombatActor_SetPendingCommand(&actor->base.actor, 5);
    func_ov003_0208a234(arg1, arg0 + 4, &actor->base.actor);
}

s32 func_ov003_020c5b0c(u16 id, void* owner, s32 arg2) {
    return func_ov003_0208a1e8(func_ov003_0208a114(id), &((CombatActor*)owner)->baseArgs, arg2);
}

s32 func_ov003_020c5b2c(u16 id, void* owner, s32 x, s32 y, s32 z) {
    return func_ov003_0208a164(func_ov003_0208a114(id), &((CombatActor*)owner)->baseArgs, x, y, z);
}

s32 func_ov003_020c5b64(u16 id, void* owner, s32 x, s32 y) {
    return func_ov003_0208a1a4(func_ov003_0208a114(id), &((CombatActor*)owner)->baseArgs, x, y);
}

s32 func_ov003_020c5b8c(const Ov003AttackInfo* info, void* owner, s32 x, s32 y, s32 z) {
    return func_ov003_0208a164(info, &((CombatActor*)owner)->baseArgs, x, y, z);
}

s32 func_ov003_020c5ba4(u16 id, void* args, s32 arg2) {
    return func_ov003_0208a1e8(func_ov003_0208a114(id), args, arg2);
}

s32 func_ov003_020c5bc4(u16 id, Ov003AttackArgs* args, s32 x, s32 y, s32 z) {
    return func_ov003_0208a164(func_ov003_0208a114(id), args, x, y, z);
}

/// Cut-in state machine: the actor swoops in, alternates facing, then fires its command.
s32 func_ov003_020c5bfc(Ov003EnemyActor* actor) {
    switch (actor->unk_1A8) {
        default:
            return 1;

        case 0:
            if (actor->unk_1A4 == 0) {
                actor->sprite.flags46 |= 2;
                actor->actor.flags |= 0x80000000;
                if ((func_ov003_020c37f8(&actor->sprite) != 0) && (actor->partner != NULL)) {
                    actor->unk_1A6 = actor->partner->unk_1A6;
                } else {
                    actor->unk_1A6 = RNG_Next(0x33U) + 0x64;
                }
                actor->unk_1B8 = 0;
                actor->unk_1BC = 0x1000;
                if (((actor->flags196.raw << 0x17) >> 0x1F) != 0) {
                    func_ov003_020c3ec0(&actor->actor);
                    actor->actor.flags |= 0x08000000;
                }
            }
            if (actor->unk_1A4 < actor->unk_1A6) {
                actor->unk_1A4++;
            } else {
                actor->unk_1A4 = 0;
                actor->unk_1A6 = 0x14;
                actor->unk_1A8++;
                func_ov003_020c4cc4(actor, ((Ov003CutinCmdIds*)data_ov003_020e071c)[actor->group].main);
            }
            return 1;

        case 1:
            if (actor->unk_1A6 > 0) {
                func_02026590(&actor->unk_1B8, 0x1000, (u16)actor->unk_1A6);
                actor->unk_1A6--;
            }
            if (actor->unk_1A6 <= 0) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            }
            return 1;

        case 2:
            if (actor->unk_1A4 < 0xA) {
                actor->actor.flags ^= 0x80000000;
                actor->sprite.flags46 ^= 2;
            } else {
                s32 flags = actor->actor.flags;
                if (actor->unk_1A4 % 3 == 0) {
                    actor->actor.flags = flags | 0x80000000;
                    actor->sprite.flags46 |= 2;
                } else {
                    actor->actor.flags = flags & ~0x80000000;
                    actor->sprite.flags46 &= ~2;
                }
            }
            if (actor->unk_1A4 <= 0x14) {
                actor->unk_1A4++;
            } else {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
                actor->actor.flags &= ~0x80000000;
                actor->sprite.flags46 &= ~2;
            }
            return 1;

        case 3:
            if (data_ov003_020e71b8->unk3D874 == 2) {
                if (func_ov003_020a3330(0xF) != 0) {
                    func_ov003_0208a028(actor->sprite.sprite.bits_0_1, actor, (u16)func_ov003_020a3468(0xF));
                }
                return 0;
            }
            return 1;
    }
}

/// Second cut-in state machine: launch, rise, track the partner, then finish.
s32 func_ov003_020c5e98(Ov003EnemyActor* actor) {
    s32 target[2];
    u8  partner;

    if (((g_Cutin->flags.raw << 0x1E) >> 0x1F) != 0) {
        return 0;
    }
    partner = data_ov003_020e71b8->unk3D88D;
    if ((partner != 0) && (partner != 1) && (partner != 2)) {
        return 0;
    }
    switch (actor->unk_1A8) {
        default:
            return 1;

        case 0:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c4cc4(actor, 0x19);
            }
            if (actor->unk_1A4 <= 0x14) {
                actor->unk_1A4++;
            } else {
                actor->unk_1A4 = 0;
                actor->unk_1A6 = 0xA;
                actor->unk_1A8++;
                actor->actor.flags |= 0x80000010;
            }
            return 1;

        case 1:
            if (actor->unk_1A6 > 0) {
                func_02026590(&actor->unk_1B8, 0x40, (u16)(actor->unk_1A6 / 5));
                func_02026590(&actor->unk_1BC, 0, (u16)actor->unk_1A6);
                actor->unk_1A6--;
            }
            if (actor->unk_1A6 <= 0) {
                actor->unk_1A4 = 0;
                actor->unk_1A6 = 0x14;
                actor->unk_1A8++;
                actor->flags18C |= 0x11;
            }
            return 1;

        case 2:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c5924(actor, target);
                actor->unk_1AC = target[0];
                actor->unk_1B0 = target[1];
                EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov003_020e0b74, NULL, 0, NULL, actor);
                func_ov003_020c4cc4(actor, 0x1A);
            }
            if (actor->unk_1A4 < actor->unk_1A6) {
                func_02026590(&actor->actor.position.x, actor->unk_1AC, (u16)actor->unk_1A6);
                func_02026590(&actor->actor.position.y, actor->unk_1B0, (u16)actor->unk_1A6);
                SndMgr_UpdateSEPan(SEIDX_SE_QUICKJUMP02, func_ov003_020843b0(1, actor->actor.position.x));
                actor->unk_1A4++;
                return 1;
            }
            actor->actor.position.x = actor->unk_1AC;
            actor->actor.position.y = actor->unk_1B0;
            func_ov003_020c4b1c(actor);
            actor->unk_1A4 = 0;
            actor->unk_1A6 = 0xA;
            actor->unk_1A8++;
            actor->unk_1B8 = 0;
            actor->unk_1BC = 0;
            actor->flags18C &= ~0x10;
            if (actor->flags18C & 0x20) {
                return 0;
            }
            func_ov003_020c4cc4(actor, 0x1B);
            return 1;

        case 3:
            if (actor->unk_1A6 > 0) {
                func_02026590(&actor->unk_1BC, 0x1000, (u16)actor->unk_1A6);
                func_02026590(&actor->unk_1B8, 0x1000, (u16)actor->unk_1A6);
                actor->flags18C &= ~1;
                actor->unk_1A6--;
            }
            if (actor->unk_1A6 <= 0) {
                actor->actor.flags &= ~0x10 & ~0x80000000;
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            }
            return 1;

        case 4:
            if (actor->unk_1A4 <= 0x14) {
                actor->unk_1A4++;
            } else {
                CombatSprite_SetPaletteMode(&actor->sprite, 0);
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            }
            return 1;

        case 5:
            if (data_ov003_020e71b8->unk3D874 == 2) {
                return 0;
            }
            return 1;
    }
}

/// Blinks the main sprite while the cut-in counts down.
s32 func_ov003_020c6230(Ov003EnemyActor* actor) {
    if (actor->unk_1A4 == 0) {
        func_ov003_020c4cc4(actor, 0x1C);
    }
    if ((actor->unk_1A4 / 5) % 2 == 0) {
        CombatSprite_SetPaletteMode(&actor->sprite, 1);
    } else {
        CombatSprite_SetPaletteMode(&actor->sprite, 0);
    }
    if (actor->unk_1A4 <= 0x13) {
        actor->unk_1A4++;
        return 1;
    }
    CombatSprite_SetPaletteMode(&actor->sprite, 0);
    return 0;
}

/// Attack cut-in state machine (main actor).
s32 func_ov003_020c62c4(Ov003EnemyActor* actor, u16 anim) {
    s32               target[2];
    Ov003ActorParams* params;
    s32               scale;

    params = func_ov003_020c3c88(actor);
    switch (actor->unk_1A8) {
        default:
            return 1;

        case 0:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c4cc4(actor, ((Ov003CutinCmdIds*)data_ov003_020e0720)[actor->group].main);
                func_ov003_020c4288(actor);
                func_ov003_020c38b8(&actor->sprite, anim, 1);
                CombatSprite_Restart(&actor->sprite);
                actor->actor.flags |= 2;
                func_ov003_020c4ee0(actor);
                if (func_ov003_020c5220(actor) != 0) {
                    func_ov003_020c5924(actor, target);
                    actor->unk_1AC = target[0];
                    actor->unk_1B0 = target[1];
                }
                actor->unk_1A4++;
            }
            scale = params->unk_19 - actor->unk_1A4;
            if (scale < 0) {
                scale = 0;
            }
            func_02026590(&actor->actor.position.x, actor->unk_1AC, (u16)scale);
            func_02026590(&actor->actor.position.y, actor->unk_1B0, (u16)scale);
            func_02026590(&actor->actor.position.z, actor->unk_1B4, (u16)scale);
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 1:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)(anim + 1), 1);
            }
            scale = params->unk_19 - actor->unk_1A4;
            if (scale < 0) {
                scale = 0;
            }
            func_02026590(&actor->actor.position.x, actor->unk_1AC, (u16)scale);
            func_02026590(&actor->actor.position.y, actor->unk_1B0, (u16)scale);
            func_02026590(&actor->actor.position.z, actor->unk_1B4, (u16)scale);
            if ((actor->actor.position.x == actor->unk_1AC) && (actor->actor.position.y == actor->unk_1B0) &&
                (actor->actor.position.z == actor->unk_1B4) && (actor->actor.unk_10 == 0))
            {
                actor->unk_1A4 = 0;
                if (actor->actor.periodicEffectMode == 4) {
                    actor->unk_1A8++;
                } else {
                    actor->unk_1A8 += 2;
                }
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 2:
            if (actor->unk_1A4 == 0) {
                actor->actor.flags &= ~2;
            }
            if (actor->actor.periodicEffectMode != 4) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 3:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)(anim + 2), 1);
            }
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) {
                actor->actor.flags &= ~2;
                return 0;
            }
            actor->unk_1A4++;
            return 1;
    }
}

/// Attack cut-in state machine with the trailing command/sound wiring.
s32 func_ov003_020c65cc(Ov003EnemyActor* actor, u16 anim) {
    Ov003ActorParams* params;
    s32               scale;

    params = func_ov003_020c3c88(actor);
    switch (actor->unk_1A8) {
        default:
            return 1;

        case 0:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c4cc4(actor, ((Ov003CutinCmdIds*)data_ov003_020e0720)[actor->group].main);
                func_ov003_020c38b8(&actor->sprite, anim, 1);
                CombatSprite_Restart(&actor->sprite);
                actor->actor.flags |= 2;
                if (actor->actor.xVelocity < 0) {
                    func_ov003_020c4ab4(actor, 1);
                }
                if (actor->actor.xVelocity > 0) {
                    func_ov003_020c4ab4(actor, 0);
                }
                actor->unk_1B4 = actor->actor.position.z;
                if (actor->actor.zVelocity == 0) {
                    actor->actor.linearDampingStep = 0x19A;
                    actor->actor.zGravity          = 0x400;
                    if (params->unk_19 > 0) {
                        scale = params->unk_19 << 0xC;
                        scale = (s32)(scale > 0 ? 0.5f + (f32)scale : (f32)scale - 0.5f);
                        if (actor->actor.position.z >= -scale) {
                            actor->actor.zGravity = 0;
                        }
                    }
                } else {
                    actor->actor.linearDampingStep = 0;
                    actor->actor.zGravity          = 0x400;
                }
                func_ov003_020c4f00(actor);
                func_ov003_020c3700(actor);
            }
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 1:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)(anim + 1), 1);
            }
            if (actor->actor.linearDampingStep != 0) {
                if ((actor->actor.xVelocity != 0) || (actor->actor.yVelocity != 0)) {
                    goto advancing;
                }
                goto landed;
            }
            if (actor->actor.zGravity != 0) {
                scale = actor->actor.position.z;
                if (scale >= actor->unk_1B4) {
                    scale = params->unk_19 << 0xC;
                    scale = (s32)(scale > 0 ? 0.5f + (f32)scale : (f32)scale - 0.5f);
                    if (actor->actor.position.z >= -scale) {
                        goto grounded;
                    }
                }
                goto advancing;
            }
        grounded:
            if (actor->actor.unk_10 == 0) {
                actor->unk_1A4 = 0;
                if (actor->actor.periodicEffectMode == 4) {
                    actor->unk_1A8++;
                } else {
                    actor->unk_1A8 += 2;
                }
            } else {
                goto advancing;
            }
            return 1;

        landed:
            if (actor->actor.zGravity != 0) {
                scale = actor->actor.position.z;
                if (scale >= actor->unk_1B4) {
                    scale = params->unk_19 << 0xC;
                    scale = (s32)(scale > 0 ? 0.5f + (f32)scale : (f32)scale - 0.5f);
                    if (actor->actor.position.z >= -scale) {
                        goto grounded;
                    }
                }
            }
        advancing:
            actor->unk_1A4++;
            return 1;

        case 2:
            if (actor->unk_1A4 == 0) {
                actor->actor.flags &= ~2;
            }
            if (actor->actor.periodicEffectMode != 4) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 3:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)(anim + 2), 1);
                actor->actor.linearDampingStep = 0x19A;
                if (params->unk_19 > 0) {
                    actor->actor.zGravity = 0;
                } else {
                    actor->actor.zGravity = 0x400;
                }
            }
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) {
                actor->actor.flags &= ~2;
                return 0;
            }
            actor->unk_1A4++;
            return 1;
    }
}

/// Third cut-in state machine: dash in, bounce and recover.
s32 func_ov003_020c6920(Ov003EnemyActor* actor, u16 anim, s32 arg2) {
    switch (actor->unk_1A8) {
        case 0:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c4cc4(actor, ((Ov003CutinCmdIds*)data_ov003_020e0720)[actor->group].main);
                actor->actor.flags |= 0x4002;
                if (actor->actor.position.x > actor->actor.unk_0C) {
                    actor->actor.isFlipped = 0;
                }
                if (actor->actor.position.x < actor->actor.unk_0C) {
                    actor->actor.isFlipped = 1;
                }
                if (actor->actor.xVelocity < 0) {
                    func_ov003_020c4ab4(actor, 1);
                }
                if (actor->actor.xVelocity > 0) {
                    func_ov003_020c4ab4(actor, 0);
                }
                func_ov003_020c38b8(&actor->sprite, anim, 1);
                CombatSprite_Restart(&actor->sprite);
                func_ov003_020c4f00(actor);
                func_ov003_020c3700(actor);
                actor->actor.linearDampingStep = 0x19A;
                actor->actor.zGravity          = 0x400;
            }
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)((anim + arg2) - 2), 0);
                actor->sprite.sprite.animationMode = 1;
            }
            if ((actor->unk_1A4 > 0) && (actor->actor.position.z >= 0)) {
                actor->unk_1A4 = 0;
                if (actor->actor.periodicEffectMode == 4) {
                    actor->unk_1A8++;
                } else {
                    actor->unk_1A8 += 2;
                }
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 1:
            if (actor->unk_1A4 == 0) {
                actor->actor.flags &= 0xFFFFBFFD;
            }
            if (actor->actor.periodicEffectMode != 4) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 2:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)((anim + arg2) - 1), 1);
                actor->actor.flags &= 0xFFFFBFFD;
            }
            if ((SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) && (actor->actor.xVelocity == 0) &&
                (actor->actor.yVelocity == 0))
            {
                return 0;
            }
            actor->unk_1A4++;
            return 1;

        default:
            return 1;
    }
}

s32 func_ov003_020c6b8c(Ov003EnemyActor* actor, u16 anim) {
    return func_ov003_020c6920(actor, anim, 2);
}

void func_ov003_020c6b9c(Ov003CutinActor* actor, u16 anim) {
    func_ov003_020c6920(&actor->base, anim, 2);
}

s32 func_ov003_020c6bac(Ov003EnemyActor* actor, u16 anim) {
    return func_ov003_020c6920(actor, anim, 3);
}

void func_ov003_020c6bbc(Ov003CutinActor* actor, u16 anim) {
    func_ov003_020c65cc(&actor->base, anim);
}

s32 func_ov003_020c6bc8(Ov003EnemyActor* actor, u16 anim) {
    if (actor->unk_1A4 == 0) {
        func_ov003_020c38b8(&actor->sprite, anim, 0);
        actor->actor.flags |= 0x200;
        func_ov003_020c4f00(actor);
        if (actor->actor.periodicEffectMode == 4) {
            CombatActor_ClearPeriodicEffect(&actor->actor);
        }
        actor->unk_1A4++;
    }
    return 1;
}

s32 func_ov003_020c6c2c(Ov003EnemyActor* actor, u16 anim) {
    Ov003ActorParams* params;

    params = func_ov003_020c3c88(actor);
    switch (actor->unk_1A8) {
        default:
            return 1;

        case 0:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, anim, 0);
                actor->actor.flags |= 0x400;
                func_ov003_020c4f00(actor);
                func_ov003_020c3700(actor);
            }
            func_ov003_0208a25c(&actor->actor);
            if ((actor->unk_1A4 > 0) && (actor->actor.position.z >= 0)) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 1:
            if (actor->unk_1A4 == 0) {
                func_ov003_020c38b8(&actor->sprite, (u16)(anim + 1), 1);
                actor->actor.linearDampingStep = 0x19A;
                actor->actor.zGravity          = 0x400;
            }
            func_ov003_0208a25c(&actor->actor);
            if ((actor->actor.xVelocity == 0) && (actor->actor.yVelocity == 0)) {
                actor->actor.flags &= ~0x400;
                actor->unk_1A4 = 0;
                if (actor->actor.periodicEffectMode == 4) {
                    actor->unk_1A8++;
                } else {
                    actor->unk_1A8 += 2;
                }
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 2:
            if (actor->actor.periodicEffectMode != 4) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            return 1;

        case 3:
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite) != 0) {
                if (params->unk_19 > 0) {
                    actor->actor.zGravity = 0;
                }
                return 0;
            }
            actor->unk_1A4++;
            return 1;
    }
}

void func_ov003_020c6e0c(Ov003EnemyActor* actor) {
    actor->flags18C = (u16)((actor->flags18C | 0x40) & ~0x20);
    func_ov003_020c4ee0(actor);
    func_ov003_020c4a34(actor);
    if (data_ov003_020e71b8->unk3D8A0 == actor) {
        data_ov003_020e71b8->unk3D8A0 = NULL;
    }
}

void func_ov003_020c6e58(Ov003EnemyActor* actor) {
    func_ov003_020c4f00(actor);
}

/// Final cut-in sequence: camera fade-out and sprite release.
s32 func_ov003_020c6e64(Ov003EnemyActor* actor) {
    if (actor->unk_1A4 == 0) {
        if (!(actor->flags18C & 0x1000)) {
            s32 hasPartner = ((actor->actor.flags & 0x2000) && (actor->partner != NULL)) ? 1 : 0;
            func_ov003_0209f444(actor->sprite.sprite.bits_0_1, actor->actor.position.x, actor->actor.position.y,
                                actor->actor.position.z, hasPartner);
            actor->sprite.flags46 |= 2;
        }
        if (actor->actor.flags & 0x2000) {
            func_ov003_020c5130(actor);
        }
        func_ov003_020c4cc4(actor, ((Ov003CutinCmdIds*)data_ov003_020e0724)[actor->group].main);
    }
    if (actor->unk_1A4 == 1) {
        actor->flags18C |= 1;
    }
    if (actor->unk_1A4 == 0x3C) {
        return 0;
    }
    actor->unk_1A4++;
    return 1;
}

s32 func_ov003_020c6f40(Ov003EnemyActor* actor) {
    if (actor->unk_1A4 == 0) {
        func_ov003_020c6e0c(actor);
    }
    return func_ov003_020c6e64(actor);
}

s32 func_ov003_020c6f68(Ov003EnemyActor* actor) {
    if (actor->unk_1A4 == 0) {
        func_ov003_020c6e58(actor);
    }
    return func_ov003_020c6e64(actor);
}

/// Drives the end-of-cut-in teardown task until the fade completes.
s32 func_ov003_020c6f90(Ov003EnemyActor* actor) {
    s32 result;

    if (actor->unk_1A4 == 0) {
        func_ov003_020c9118(actor);
        data_ov003_020e6a30[4] = 1;
    }
    result = func_ov003_020c9338(actor);
    if ((func_ov003_020c9008() != 0) && (data_ov003_020e6a30[4] != 0)) {
        CombatSprite_Release(&actor->sprite);
        CombatSprite_Release(&actor->partner->sprite);
        data_ov003_020e6a30[4] = 0;
    }
    if (result == 0) {
        func_ov003_020c50bc();
    }
    if ((result == 0) && (EasyFade_IsFading() == 0)) {
        return 0;
    }
    actor->unk_1A4++;
    return 1;
}
