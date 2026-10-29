#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatBgScroll.h"
#include "Combat/Core/CombatBossExp.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ScreenMapper.h"
#include "SpriteMgr.h"

#include <nitro/g2.h>
#include <nitro/mi/cpumem.h>

// The boss defeat sequence (`Tsk_BossExp1`) and the tables that describe every enemy sprite. The task replays the
// boss's cut-in sprites in four steps: sprites flash in front of the battle, a second backdrop scrolls past, a field of
// small sprites fills both screens and finally a wipe closes the scene. The first half of the unit is the actor
// helpers that the enemy tasks use to start and end their defeat animation.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

struct BossExp1;

typedef void (*BossExp1State)(struct BossExp1*);

/// The sprite sheet of the sequence and the two sizes (in frames) its wipes use.
typedef struct BossExp1Sheet {
    /* 0x00 */ BinIdentifier iden;
    /* 0x08 */ s32           wipeLength;
    /* 0x0C */ s32           wipeHeight;
} BossExp1Sheet; // Size: 0x10

/// What one group of the backdrop loads: the layer it lands on and the pack entries of its characters and tilemap.
typedef struct BossExp1Group {
    /* 0x0 */ s32 layer;
    /* 0x4 */ s32 charEntry;
    /* 0x8 */ s32 screenEntry;
} BossExp1Group; // Size: 0xC

/// One flying sprite: how many palette slots it uses and how fast its blink advances.
typedef struct BossExp1Fly {
    /* 0x0 */ s32 paletteCount;
    /* 0x4 */ s32 speed;
} BossExp1Fly; // Size: 0x8

typedef struct BossExp1 {
    /* 0x000 */ Data*            data;
    /* 0x004 */ BossExp1Anims    anims;
    /* 0x0C8 */ CombatSprite     groupA[2][2]; // [engine][group]
    /* 0x248 */ s32              loadedA[2][2];
    /* 0x258 */ s32              visibleA[2][2];
    /* 0x268 */ CombatSprite     groupB[2][4];
    /* 0x568 */ s32              loadedB[2][4];
    /* 0x588 */ s32              cursor; // the flying sprite the scene is on
    /* 0x58C */ CombatSprite     groupC[2][6];
    /* 0xA0C */ s32              loadedC[2][6];
    /* 0xA3C */ s32              tintC[2][6];
    /* 0xA6C */ u16              frameC[2][6];
    /* 0xA84 */ PaletteResource* palettes[2];
    /* 0xA8C */ BgResource*      chars[4][2]; // [group][engine]
    /* 0xAAC */ ScreenMapEntry   maps[2][2];  // [layer - 1][engine]
    /* 0xB4C */ void*            bgData[2][2];
    /* 0xB5C */ BossExp1State    state;
    /* 0xB60 */ s16              timer;
    /* 0xB62 */ s16              reservedB62;
    /* 0xB64 */ s16              limit;
    /* 0xB66 */ s16              phase;
    /* 0xB68 */ s32              running;
    /* 0xB6C */ s32              tint;
    /* 0xB70 */ s32              closeB;
    /* 0xB74 */ s32              closeA;
    /* 0xB78 */ s32              stepB;
    /* 0xB7C */ s32              stepA;
    /* 0xB80 */ s32              scroll;
} BossExp1; // Size: 0xB84

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern char             data_ov003_020e6b38[];
extern char             data_ov003_020e6b48[];
extern char             data_ov003_020e6b64[];
extern s32              data_ov003_020e7254;
extern const TaskHandle data_ov003_020d81e0;

extern s32               func_ov003_020c9478(TaskPool* pool, Task* task, void* args, s32 stage);
extern Ov003ActorParams* func_ov003_020c3c88(Ov003CutinActor* actor);
extern void              func_ov003_020c4af0(Ov003CutinActor* actor);
extern void              func_ov003_020c6f40(Ov003CutinActor* actor);
extern void              func_ov003_020c6f68(Ov003CutinActor* actor);
extern void              func_ov003_020c6f90(Ov003CutinActor* actor);
extern s32               func_ov003_020c37f8(CombatSprite* sprite);
extern s32               func_ov003_020c38b8(CombatSprite* sprite, u16 anim, s32 arg2);
extern s32               func_ov003_020bd2b8(void);
extern void              func_ov003_020824a0(CombatSprite* sprite, u16 anim, s32 mode);
extern void              func_ov003_020827c0(CombatSprite* sprite, s32 arg1);
extern void              func_ov003_0208260c(CombatSprite* sprite, s32 arg1, s32 arg2, s32 arg3, s32 arg4);
extern void              func_ov003_02082730(CombatSprite* sprite, s32 sortKey);
extern void              func_ov003_02082738(CombatSprite* sprite, s32 arg1);
extern s32  func_ov003_02082a04(s32 engine, Sprite* sprite, const BinIdentifier* iden, const SpriteAnimEntry* table, u16 entry,
                                s32 palette, s32 paletteCount);
extern void func_ov003_02087ed8(s32 seIdx);
extern void func_ov003_02083e50(s32 arg0);
extern void func_020265d4(void* dst, s32 step, u16 count);
extern void func_0200d858(ScreenMapEntry* entry, s32 wrapH, s32 wrapV, void* defaultTile);
extern void func_0200d954(s32 engine, s32 bg);
extern BgResource* BgResMgr_AllocChar32(BgResMgr* mgr, void* charData, u32 charBase, u32 offset, u32 size);

void func_ov003_020c7f58(BossExp1* self);
void func_ov003_020c80b8(BossExp1* self);
void func_ov003_020c8328(BossExp1* self);
void func_ov003_020c8520(BossExp1* self);
void func_ov003_020c86b4(BossExp1* self);
void func_ov003_020c8be8(BossExp1* self);
void func_ov003_020c8ddc(BossExp1* self);
s32  func_ov003_020c8e3c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020c8f30(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020c8f64(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020c8f8c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020c8fc0(TaskPool* pool, Task* task, void* args, s32 stage);

const BossExp1Sheet data_ov003_020e0a38 = {
    {3, data_ov003_020e6b48},
    0x14, 0x10
};

const TaskHandle data_ov003_020e0a48 = {data_ov003_020e6b38, func_ov003_020c8fc0, sizeof(BossExp1)};

/// The stage table of the sequence: build, update, draw, release.
const TaskStages data_ov003_020e0a54 = {
    func_ov003_020c8e3c,
    func_ov003_020c8f30,
    func_ov003_020c8f64,
    func_ov003_020c8f8c,
};

/// The animation entries of the group B sprites.
const SpriteAnimEntry data_ov003_020e0a64[4] = {
    {  1,   3,   2, 0},
    {  4,   6,   5, 0},
    {0xD, 0xF, 0xE, 0},
    {0xD, 0xF, 0xE, 0},
};

/// What each backdrop group loads.
const BossExp1Group data_ov003_020e0a84[4] = {
    {1, 0x14, 0x15},
    {2, 0x16, 0x17},
    {1, 0x10, 0x11},
    {2, 0x12, 0x13},
};

/// The animation entries of the flying sprites of each engine.
const SpriteAnimEntry data_ov003_020e0ab4[2][6] = {
    {{0xA, 0xC, 0xB, 0}, {0xA, 0xC, 0xB, 2}, {0xA, 0xC, 0xB, 2}, {0xA, 0xC, 0xB, 3}, {0xA, 0xC, 0xB, 4}, {0xA, 0xC, 0xB, 5}},
    {      {7, 9, 8, 1},       {7, 9, 8, 1},       {7, 9, 8, 2},       {7, 9, 8, 3},       {7, 9, 8, 4},       {7, 9, 8, 5}},
};

const BossExp1Fly data_ov003_020e0b14[2][6] = {
    {    {0x56, 0}, {0x34, 0x20}, {0x34, -0x80}, {0x54, 0}, {0x4C, 0}, {0x40, 0}},
    {{0x34, -0x20}, {0x34, 0x80},     {0x4C, 0}, {0x54, 0}, {0x56, 0}, {0x48, 0}},
};

/// The task of the flying piece started by `BtlEff_SideMove`.
const TaskHandle data_ov003_020e0b74 = {data_ov003_020e6b64, func_ov003_020c9478, 0x80};

u8  data_ov003_020e6a30[8] = {0, 0, 0, 0, 1, 0, 0, 0};
s32 data_ov003_020e6a38[3] = {0, 1, 0};

extern char data_ov009_02124840[];
extern char data_ov009_02124904[];
extern char data_ov009_02124a34[];
extern char data_ov009_02124af4[];
extern char data_ov009_02124c44[];
extern char data_ov009_02124cc4[];
extern char data_ov009_02124d58[];
extern char data_ov009_02124dc0[];
extern char data_ov009_02124e84[];
extern char data_ov009_02124ff8[];
extern char data_ov009_02125240[];
extern char data_ov009_02125328[];
extern char data_ov009_021253ec[];
extern char data_ov009_02125564[];
extern char data_ov010_02129160[];
extern char data_ov010_02129320[];
extern char data_ov011_0212c460[];
extern char data_ov012_02127980[];
extern char data_ov013_02127540[];
extern char data_ov014_02134e0c[];
extern char data_ov015_021283f8[];
extern char data_ov017_02142d9c[];
extern char data_ov017_02142f00[];
extern char data_ov017_02142f6c[];
extern char data_ov017_02142fc4[];
extern char data_ov017_021430a0[];
extern char data_ov017_021430e8[];
extern char data_ov017_02143138[];
extern char data_ov017_02143164[];
extern char data_ov017_02143168[];
extern char data_ov017_0214316c[];
extern char data_ov017_02143170[];
extern char data_ov017_02143174[];
extern char data_ov018_020ff42c[];
extern char data_ov018_020ff4b0[];
extern char data_ov019_020fa7a0[];
extern char data_ov020_020f70a0[];
extern char data_ov020_020f70a4[];
extern char data_ov021_020f7940[];
extern char data_ov021_020f7944[];
extern char data_ov021_020f79a0[];
extern char data_ov021_020f7a24[];
extern char data_ov022_020f5a60[];
extern char data_ov024_020f6ba0[];
extern char data_ov024_020f6bd4[];

void* data_ov003_020e6a44[61] = {
    data_ov009_02124af4, data_ov009_02124c44, data_ov009_02124cc4, data_ov009_02125564, data_ov009_02124d58,
    data_ov010_02129320, data_ov011_0212c460, data_ov009_02124dc0, data_ov009_02124ff8, data_ov009_02124904,
    data_ov012_02127980, data_ov013_02127540, data_ov009_02125240, data_ov009_02124840, data_ov009_02124e84,
    data_ov014_02134e0c, data_ov009_02124a34, data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8,
    data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8,
    data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8, data_ov015_021283f8,
    data_ov009_02125328, data_ov009_021253ec, data_ov018_020ff42c, data_ov018_020ff4b0, data_ov010_02129160,
    data_ov017_02142d9c, data_ov017_02142f00, data_ov017_02142f6c, data_ov017_02142fc4, data_ov017_021430a0,
    data_ov017_02143138, data_ov017_021430e8, data_ov017_021430e8, data_ov017_02143170, data_ov017_02143174,
    data_ov017_02143168, data_ov017_0214316c, data_ov017_02143164, data_ov019_020fa7a0, data_ov020_020f70a4,
    data_ov020_020f70a0, data_ov021_020f7944, data_ov021_020f7940, data_ov022_020f5a60, data_ov021_020f79a0,
    data_ov024_020f6ba0, data_ov024_020f6ba0, data_ov024_020f6bd4, data_ov024_020f6bd4, data_ov021_020f7a24,
    data_ov003_020e6a30,
};

// ---------------------------------------------------------------------------------------------
// Actor helpers
// ---------------------------------------------------------------------------------------------

void func_ov003_020c703c(Ov003CutinActor* actor) {
    if (actor->flags196.bits.b8 && (actor->actor.flags & 0x2000)) {
        func_ov003_020c6f90(actor);
    } else {
        func_ov003_020c6f40(actor);
    }
}

void func_ov003_020c7070(Ov003CutinActor* actor) {
    if (actor->flags196.bits.b8 && (actor->actor.flags & 0x2000)) {
        func_ov003_020c6f90(actor);
    } else {
        func_ov003_020c6f68(actor);
    }
}

/// One frame of an enemy's defeat animation, which plays animations `anim`, `anim + 1` and `anim + 2` in turn.
s32 func_ov003_020c70a4(Ov003CutinActor* actor, s32 anim) {
    switch (actor->unk_1A8) {
        case 0:
            if (actor->unk_1A4 == 0) {
                func_ov003_020824a0(&actor->sprite, anim, 1);
                CombatSprite_Restart(&actor->sprite);
                if (actor->actor.unk_0C == 0) {
                    func_ov003_020c4af0(actor);
                } else {
                    if (actor->actor.position.x > actor->actor.unk_0C) {
                        actor->actor.isFlipped = 0;
                    }
                    if (actor->actor.position.x < actor->actor.unk_0C) {
                        actor->actor.isFlipped = 1;
                    }
                }
                actor->actor.flags |= 8;
            }
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite)) {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            } else {
                actor->unk_1A4++;
            }
            break;
        case 1:
            if (actor->unk_1A4 == 0) {
                func_ov003_020824a0(&actor->sprite, (u16)(anim + 1), 0);
            }
            if (actor->unk_1A4 <= actor->unk_1A6) {
                actor->unk_1A4++;
            } else {
                actor->unk_1A4 = 0;
                actor->unk_1A8++;
            }
            break;
        case 2:
            if (actor->unk_1A4 == 0) {
                func_ov003_020824a0(&actor->sprite, (u16)(anim + 2), 1);
                actor->actor.flags &= ~8;
            }
            if (SpriteMgr_IsAnimationFinished(&actor->sprite.sprite)) {
                return 0;
            }
            actor->unk_1A4++;
            break;
    }
    return 1;
}

/// Puts an actor back into its idle state after its defeat animation.
void func_ov003_020c7224(Ov003CutinActor* actor) {
    Ov003ActorParams* params = func_ov003_020c3c88(actor);

    actor->flags18C &= ~1;
    actor->flags18C &= ~0x10;
    actor->unk_1B8 = 0x1000;
    actor->unk_1BC = 0x1000;
    func_ov003_020827c0(&actor->sprite, 0);
    actor->sprite.flags46 &= ~2;
    actor->actor.flags &= ~0x10;
    actor->actor.flags &= ~0x80000000;
    actor->actor.zVelocity = 0;
    if (params->unk_19 > 0) {
        actor->actor.zGravity = 0;
    }
    actor->actor.xVelocity         = 0;
    actor->actor.yVelocity         = 0;
    actor->actor.linearDampingStep = 0x19A;
}

/// Starts an actor's death animation `a` (or `b` in the second variant) and waits for the transition.
s32 func_ov003_020c72b4(Ov003CutinActor* actor, s32 a, s32 b) {
    if (actor->unk_1A4 == 0) {
        switch (func_ov003_020bd2b8()) {
            case 1:
                func_ov003_020c38b8(&actor->sprite, a, 0);
                break;
            case 2:
                func_ov003_020c38b8(&actor->sprite, b, 0);
                break;
            default:
                func_ov003_020c38b8(&actor->sprite, a, 0);
                break;
        }
        func_ov003_020c7224(actor);
        actor->unk_1A4++;
    }
    if (data_ov003_020e71b8->unk3D878 & 2) {
        return 1;
    }
    func_ov003_020c7224(actor);
    if (func_ov003_020c37f8(&actor->sprite) != 0) {
        actor->actor.position.x = actor->unk_1AC;
        actor->actor.position.y = actor->unk_1B0;
        actor->actor.position.z = actor->unk_1B4;
    }
    func_ov003_020c4af0(actor);
    return 0;
}

// ---------------------------------------------------------------------------------------------
// The sequence
// ---------------------------------------------------------------------------------------------

/// Sets BG1 and BG2 of both engines up as the text layers the sequence draws on.
void func_ov003_020c73a0(void) {
    s32 e;

    for (e = 0; e <= 1; e++) {
        DisplayBGSettings* bg = &g_DisplaySettings.engineState[e].bgSettings[1];

        bg->bgMode         = DISPLAY_BGMODE_TEXT;
        bg->screenSizeText = 0;
        bg->colorMode      = 0;
        bg->screenBase     = 0;
        bg->charBase       = 1;
        bg->extPlttSlot    = 0;
        if (e == 0) {
            G2_SetBG1Control(0, 0, 0, 1, 0);
        } else if (e == 1) {
            G2S_SetBG1Control(0, 0, 0, 1, 0);
        }
        bg                 = &g_DisplaySettings.engineState[e].bgSettings[2];
        bg->bgMode         = DISPLAY_BGMODE_TEXT;
        bg->screenSizeText = 0;
        bg->colorMode      = 0;
        bg->screenBase     = 2;
        bg->charBase       = 3;
        bg->extPlttSlot    = 1;
        if (e == 0) {
            G2_SetBG2Control(0, 0, 2, 3, 0);
        } else if (e == 1) {
            G2S_SetBG2Control(0, 0, 2, 3, 0);
        }
        g_DisplaySettings.engineState[e].bgSettings[1].priority = 1;
        g_DisplaySettings.engineState[e].bgSettings[2].priority = 2;
        g_DisplaySettings.controls[e].layers                    = 0x10;
        Ov003_ScrollBgById(e, 1, 0, 0);
        Ov003_ScrollBgById(e, 2, 0, 0);
        g_DisplaySettings.controls[e].windows                 = 7;
        g_DisplaySettings.engineState[e].window0              = 4;
        g_DisplaySettings.engineState[e].window0Effects       = 0;
        g_DisplaySettings.engineState[e].window1              = 0x18;
        g_DisplaySettings.engineState[e].window1Effects       = 0;
        g_DisplaySettings.engineState[e].windowObj            = 0x18;
        g_DisplaySettings.engineState[e].windowObjEffects     = 0;
        g_DisplaySettings.engineState[e].windowOutside        = 0x1E;
        g_DisplaySettings.engineState[e].windowOutsideEffects = 0;
    }
}

/// Restores the windows of the battle screens.
void func_ov003_020c759c(void) {
    g_DisplaySettings.controls[0].layers |= 1;
    g_DisplaySettings.controls[0].windows                 = 4;
    g_DisplaySettings.engineState[0].windowObj            = 8;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x17;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;
    g_DisplaySettings.controls[1].windows                 = 4;
    g_DisplaySettings.engineState[1].windowObj            = 8;
    g_DisplaySettings.engineState[1].windowObjEffects     = 0;
    g_DisplaySettings.engineState[1].windowOutside        = 0x17;
    g_DisplaySettings.engineState[1].windowOutsideEffects = 0;
}

/// Loads the characters and the tilemap of backdrop group `group` onto both engines.
void func_ov003_020c75ec(BossExp1* self, s32 group) {
    s32 kind = data_ov003_020e0a84[group].layer;
    u8* chars;
    u8* screen;
    s32 size;
    s32 map;

    if (kind == 1) {
        g_DisplaySettings.controls[0].layers |= 2;
        g_DisplaySettings.controls[1].layers |= 2;
    } else if (kind == 2) {
        g_DisplaySettings.controls[0].layers |= 4;
        g_DisplaySettings.controls[1].layers |= 4;
    }
    chars                 = Data_GetPackEntryData(self->data, data_ov003_020e0a84[group].charEntry);
    size                  = ((*(u32*)chars & ~0xFF) >> 8) - (((*chars & 0xF0) == 0) ? 4 : 0);
    self->chars[group][0] = BgResMgr_AllocChar32(g_BgResourceManagers[0], chars,
                                                 g_DisplaySettings.engineState[0].bgSettings[kind].charBase, 0, size);
    chars                 = Data_GetPackEntryData(self->data, data_ov003_020e0a84[group].charEntry);
    size                  = ((*(u32*)chars & ~0xFF) >> 8) - (((*chars & 0xF0) == 0) ? 4 : 0);
    self->chars[group][1] = BgResMgr_AllocChar32(g_BgResourceManagers[1], chars,
                                                 g_DisplaySettings.engineState[1].bgSettings[kind].charBase, 0, size);
    map                   = kind - 1;
    screen                = Data_GetPackEntryData(self->data, data_ov003_020e0a84[group].screenEntry);
    self->bgData[map][0]  = screen + 4;
    screen                = Data_GetPackEntryData(self->data, data_ov003_020e0a84[group].screenEntry);
    self->bgData[map][1]  = screen + 4;
    func_0200d1d8(&self->maps[map][0], 0, kind, NULL, &self->bgData[map][0], 1, 1);
    func_0200d1d8(&self->maps[map][1], 1, kind, NULL, &self->bgData[map][1], 1, 1);
    func_0200d858(&self->maps[map][0], 1, 1, NULL);
    func_0200d858(&self->maps[map][1], 1, 1, NULL);
}

/// Releases what `func_ov003_020c75ec` loaded.
void func_ov003_020c7874(BossExp1* self, s32 group) {
    s32 kind = data_ov003_020e0a84[group].layer;

    if (kind == 1) {
        g_DisplaySettings.controls[0].layers &= ~2;
        g_DisplaySettings.controls[1].layers &= ~2;
    } else if (kind == 2) {
        g_DisplaySettings.controls[0].layers &= ~4;
        g_DisplaySettings.controls[1].layers &= ~4;
    }
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->chars[group][0]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], self->chars[group][1]);
    kind = data_ov003_020e0a84[group].layer;
    func_0200d954(0, kind);
    func_0200d954(1, kind);
}

/// Loads the group A sprites of `group` onto both engines.
void func_ov003_020c7940(BossExp1* self, s32 group) {
    s32 e;

    for (e = 0; e <= 1; e++) {
        SpriteAnimationEx* anim = &self->anims.slots[e * 2 + group];

        anim->anim.bits_0_1      = e;
        self->loadedA[e][group]  = CombatSprite_Load(&self->groupA[e][group], anim);
        self->visibleA[e][group] = 1;
        if (self->loadedA[e][group] != 0) {
            self->groupA[e][group].flags46 |= 2;
            CombatSprite_Update(&self->groupA[e][group]);
        }
    }
}

void func_ov003_020c79f4(BossExp1* self, s32 group) {
    s32 e;

    for (e = 0; e <= 1; e++) {
        if (self->loadedA[e][group] != 0) {
            CombatSprite_Release(&self->groupA[e][group]);
            self->loadedA[e][group]  = 0;
            self->visibleA[e][group] = 0;
        }
    }
}

void func_ov003_020c7a54(BossExp1* self) {
    s32 e;
    s32 i;

    for (e = 0; e <= 1; e++) {
        for (i = 0; i < 2; i++) {
            if (self->loadedA[e][i] != 0 && self->visibleA[e][i] != 0) {
                CombatSprite_Render(&self->groupA[e][i]);
            }
        }
    }
}

/// Loads the group B sprite `group` onto both engines.
void func_ov003_020c7ac0(BossExp1* self, s32 group) {
    s32 e;

    for (e = 0; e <= 1; e++) {
        self->loadedB[e][group] = func_ov003_02082a04(e, &self->groupB[e][group].sprite, &data_ov003_020e0a38.iden,
                                                      data_ov003_020e0a64, (u16)group, 0x18, 0);
        if (self->loadedB[e][group] != 0) {
            CombatSprite_SetPosition(&self->groupB[e][group], 0x80, 0x60);
            func_ov003_02082738(&self->groupB[e][group], 0);
        }
    }
}

void func_ov003_020c7b6c(BossExp1* self, s32 group) {
    s32 e;

    for (e = 0; e <= 1; e++) {
        if (self->loadedB[e][group] != 0) {
            CombatSprite_Release(&self->groupB[e][group]);
            self->loadedB[e][group] = 0;
        }
    }
}

void func_ov003_020c7bc8(BossExp1* self) {
    s32 e;
    s32 i;

    for (e = 0; e <= 1; e++) {
        for (i = 0; i <= 3; i++) {
            if (self->loadedB[e][i] != 0) {
                CombatSprite_Update(&self->groupB[e][i]);
            }
        }
    }
}

void func_ov003_020c7c34(BossExp1* self) {
    s32 e;
    s32 i;

    for (e = 0; e <= 1; e++) {
        for (i = 0; i <= 3; i++) {
            if (self->loadedB[e][i] != 0) {
                CombatSprite_Render(&self->groupB[e][i]);
            }
        }
    }
}

/// Loads the flying sprite `group` onto both engines.
void func_ov003_020c7ca0(BossExp1* self, s32 group) {
    s32 e;

    for (e = 0; e <= 1; e++) {
        self->loadedC[e][group] =
            func_ov003_02082a04(e, &self->groupC[e][group].sprite, &data_ov003_020e0a38.iden, data_ov003_020e0ab4[e],
                                (u16)group, 0x18, (u16)data_ov003_020e0b14[e][group].paletteCount);
        if (self->loadedC[e][group] != 0) {
            CombatSprite_SetPosition(&self->groupC[e][group], 0x80, 0x60);
            func_ov003_02082738(&self->groupC[e][group], 0);
        }
    }
}

void func_ov003_020c7d74(BossExp1* self) {
    s32 e;
    s32 i;

    for (e = 0; e <= 1; e++) {
        for (i = 0; i < 6; i++) {
            if (self->loadedC[e][i] != 0) {
                CombatSprite_Release(&self->groupC[e][i]);
                self->loadedC[e][i] = 0;
            }
        }
    }
}

void func_ov003_020c7de8(BossExp1* self) {
    s32 e;
    s32 i;

    for (e = 0; e <= 1; e++) {
        for (i = 0; i < 6; i++) {
            if (self->loadedC[e][i] != 0) {
                self->frameC[e][i] = self->frameC[e][i] + data_ov003_020e0b14[e][i].speed;
                CombatSprite_Update(&self->groupC[e][i]);
            }
        }
    }
}

void func_ov003_020c7e84(BossExp1* self) {
    s32 e;
    s32 i;

    for (e = 0; e <= 1; e++) {
        for (i = 0; i < 6; i++) {
            if (self->loadedC[e][i] != 0) {
                func_ov003_0208260c(&self->groupC[e][i], 0, self->tintC[e][i], self->tintC[e][i],
                                    self->frameC[e][i] != 0 ? 1 : 0);
                CombatSprite_Render(&self->groupC[e][i]);
            }
        }
    }
}

void func_ov003_020c7f40(BossExp1* self, BossExp1State state) {
    self->state = state;
    self->timer = 0;
    self->phase = 0;
}

/// Fades out and waits for the battle screens to be free.
void func_ov003_020c7f58(BossExp1* self) {
    switch (self->phase) {
        case 0:
            EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
            data_ov003_020e71b8->unk3D878 |= 0x1000;
            data_ov003_020e71b8->unk3D878 |= 0x2000;
            self->phase++;
            return;
        case 1:
            if (!(data_ov003_020e71b8->unk3D878 & 1) && !(data_ov003_020e71b8->unk3D7C0[0].unk_02 & 1) &&
                !(data_ov003_020e71b8->unk3D7C0[1].unk_02 & 1))
            {
                func_ov003_020c73a0();
                data_ov003_020e71b8->unk3D884 = 1;
                data_ov003_020e71b8->unk3D878 &= ~0x2000;
                EasyTask_CreateTask(&data_ov003_020e71b8->unk_3C2BC, &data_ov003_020d81e0, NULL, 0, NULL, NULL);
                self->phase++;
                return;
            }
            self->timer++;
            return;
        case 2:
            func_ov003_020c7f40(self, func_ov003_020c80b8);
            return;
    }
}

/// Loads the sequence's resources and flashes the first sprites in.
void func_ov003_020c80b8(BossExp1* self) {
    switch (self->phase) {
        case 0: {
            void* palette;

            self->data        = DatMgr_LoadRawData(1, NULL, 0, &data_ov003_020e0a38.iden);
            palette           = Data_GetPackEntryData(self->data, 0x18);
            self->palettes[0] = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 0, 1);
            self->palettes[1] = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0, 1);
            func_ov003_020c75ec(self, 0);
            func_ov003_020c75ec(self, 1);
            func_ov003_020c7940(self, 0);
            if (self->anims.actor->group == 0x21) {
                func_ov003_02087ed8(0x1C4);
            } else {
                func_ov003_02087ed8(0x6F);
            }
            self->phase++;
            return;
        }
        case 1:
            EasyFade_FadeBothDisplays(0, 0, 0x800);
            if (!EasyFade_IsFading()) {
                self->timer = 0;
                self->phase++;
                return;
            }
            self->timer++;
            return;
        case 2:
            if (self->timer == 0) {
                self->limit = 0xF;
                func_ov003_020c7ac0(self, 1);
            }
            func_020265d4(&self->tint, 0x1D00, (u16)(self->limit - self->timer));
            func_ov003_0208260c(&self->groupB[0][1], 0, self->tint, self->tint, 1);
            func_ov003_0208260c(&self->groupB[1][1], 0, self->tint, self->tint, 1);
            if (self->timer < self->limit) {
                self->timer++;
                return;
            }
            func_ov003_020c7b6c(self, 1);
            self->timer = 0;
            self->phase++;
            return;
        case 3:
            if (self->timer == 0) {
                self->limit = 0xC;
            }
            if (self->timer < self->limit) {
                self->timer++;
                return;
            }
            self->timer = 0;
            self->phase++;
            return;
        case 4:
            func_ov003_020c7f40(self, func_ov003_020c8328);
            return;
    }
}

/// The first scene: the sprites of group A flicker, then the screen fades twice.
void func_ov003_020c8328(BossExp1* self) {
    switch (self->phase) {
        case 0:
            if (self->timer == 0) {
                func_ov003_020c7940(self, 1);
            }
            if (self->timer % 6 < 3) {
                self->visibleA[0][0] = 0;
                self->visibleA[1][0] = 0;
                self->visibleA[0][1] = 1;
                self->visibleA[1][1] = 1;
            } else {
                self->visibleA[0][0] = 1;
                self->visibleA[1][0] = 1;
                self->visibleA[0][1] = 0;
                self->visibleA[1][1] = 0;
            }
            if (self->timer <= 0xC) {
                self->timer++;
                return;
            }
            func_ov003_020c79f4(self, 0);
            self->timer = 0;
            self->phase++;
            return;
        case 1:
            if (self->timer == 0) {
                EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
            }
            if (self->timer > 0) {
                EasyFade_FadeBothDisplays(0, 0, 0x2000);
            }
            if (self->timer > 0 && !EasyFade_IsFading()) {
                self->timer = 0;
                self->phase++;
                return;
            }
            self->timer++;
            return;
        case 2:
            if (self->timer == 0) {
                EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
            }
            if (self->timer > 0) {
                EasyFade_FadeBothDisplays(0, 0, 0x1000);
            }
            if (self->timer > 0 && !EasyFade_IsFading()) {
                self->timer = 0;
                self->phase++;
                return;
            }
            self->timer++;
            return;
        case 3:
            func_ov003_020c7f40(self, func_ov003_020c8520);
            return;
    }
}

/// The second scene: the flying sprites fade in one after the other.
void func_ov003_020c8520(BossExp1* self) {
    switch (self->phase) {
        case 0:
            if (self->timer == 0) {
                func_ov003_020c7ca0(self, self->cursor);
                self->limit = 5;
                if (self->cursor == 2 || self->cursor == 4) {
                    EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
                }
            }
            if (self->timer > 0) {
                if (self->cursor == 2) {
                    EasyFade_FadeBothDisplays(0, 0, 0x666);
                }
                if (self->cursor == 4) {
                    EasyFade_FadeBothDisplays(0, 0, 0x2E1);
                }
            }
            if (self->timer < self->limit) {
                s32 e;

                for (e = 0; e <= 1; e++) {
                    func_020265d4(&self->tintC[e][self->cursor],
                                  data_ov003_020e0b14[e][self->cursor].speed == 0 ? 0x1000 : 0x1333,
                                  (u16)(self->limit - self->timer));
                }
            }
            if (self->timer >= 0xA) {
                self->timer = 0;
                self->cursor++;
                if (self->cursor < 6) {
                    return;
                }
                self->phase++;
                return;
            }
            self->timer++;
            return;
        case 1:
            func_ov003_020c7f40(self, func_ov003_020c86b4);
            return;
    }
}

/// The third scene: two wipes close over both screens.
void func_ov003_020c86b4(BossExp1* self) {
    s32 width  = data_ov003_020e0a38.wipeLength;
    s32 height = data_ov003_020e0a38.wipeHeight;
    s32 half   = height / 2;

    switch (self->phase) {
        case 0: {
            s32 e;

            if (self->timer == 0) {
                self->closeB = 0;
                self->closeA = 0;
                self->stepB  = 0xC0000 / width / width;
                self->stepA  = 0xC0000 / height / height;
            }
            if (self->timer == 1) {
                func_ov003_020c7ac0(self, 2);
                for (e = 0; e <= 1; e++) {
                    self->groupB[e][2].flags46 |= 2;
                }
            }
            EasyFade_FadeBothDisplays(0, 0, 0x2E1);
            if (self->timer <= height) {
                self->closeA = self->stepA * self->timer + self->closeA;
                if (self->closeA > 0x60000) {
                    self->closeA = 0x60000;
                }
                for (e = 0; e <= 1; e++) {
                    s32 scale = FX_Divide(self->closeA * 2, 0xC0000);

                    g_DisplaySettings.engineState[e].window1Left   = 0;
                    g_DisplaySettings.engineState[e].window1Top    = 0x60000 - self->closeA;
                    g_DisplaySettings.engineState[e].window1Right  = 0xFF000;
                    g_DisplaySettings.engineState[e].window1Bottom = (0x60000 - self->closeA) + self->closeA * 2;
                    func_ov003_0208260c(&self->groupB[e][2], 0, 0x1000, scale, 0);
                }
            }
            if (self->timer == half + 1) {
                func_ov003_020c7ac0(self, 3);
            }
            if (self->timer >= half) {
                self->closeB = self->stepB * (self->timer - half) + self->closeB;
                if (self->closeB > 0x60000) {
                    self->closeB = 0x60000;
                }
                for (e = 0; e <= 1; e++) {
                    s32 scale = FX_Divide(self->closeB * 2, 0xC0000);

                    g_DisplaySettings.engineState[e].window0Left   = 0;
                    g_DisplaySettings.engineState[e].window0Top    = 0x60000 - self->closeB;
                    g_DisplaySettings.engineState[e].window0Right  = 0xFF000;
                    g_DisplaySettings.engineState[e].window0Bottom = (0x60000 - self->closeB) + self->closeB * 2;
                    func_ov003_0208260c(&self->groupB[e][3], 0, 0x1000, scale, 0);
                }
            }
            if (self->timer <= half + width) {
                self->timer++;
                return;
            }
            g_DisplaySettings.controls[0].windows = 0;
            g_DisplaySettings.controls[1].windows = 0;
            EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
            func_ov003_020c7874(self, 0);
            func_ov003_020c7874(self, 1);
            func_ov003_020c7d74(self);
            func_ov003_020c79f4(self, 1);
            func_ov003_020c7b6c(self, 2);
            func_ov003_020c7b6c(self, 3);
            self->timer = 0;
            self->phase++;
            return;
        }
        case 1:
            if (self->timer == 0) {
                self->limit = 0x1E;
            }
            if (self->timer < self->limit) {
                self->timer++;
                return;
            }
            func_ov003_020c7f40(self, func_ov003_020c8be8);
            return;
    }
}

/// Scrolls all four backdrop layers up by twice the scroll value.
void func_ov003_020c8a18(BossExp1* self, s32 time) {
    func_020265d4(&self->scroll, 0xA00000, (u16)(0xB9 - time));
    Ov003_ScrollBg(&g_DisplaySettings.engineState[self->maps[0][0].engineId], self->maps[0][0].bgLayer, 0, self->scroll * 2);
    Ov003_ScrollBg(&g_DisplaySettings.engineState[self->maps[0][1].engineId], self->maps[0][1].bgLayer, 0, self->scroll * 2);
    Ov003_ScrollBg(&g_DisplaySettings.engineState[self->maps[1][0].engineId], self->maps[1][0].bgLayer, 0, self->scroll * 2);
    Ov003_ScrollBg(&g_DisplaySettings.engineState[self->maps[1][1].engineId], self->maps[1][1].bgLayer, 0, self->scroll * 2);
}

/// The last scene: a second backdrop scrolls past and the screen fades out.
void func_ov003_020c8be8(BossExp1* self) {
    switch (self->phase) {
        case 0:
            if (self->timer == 0) {
                EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
                func_ov003_020c75ec(self, 2);
                func_ov003_020c75ec(self, 3);
                if (self->anims.actor->group == 0x21) {
                    func_ov003_02087ed8(0x1C5);
                } else {
                    func_ov003_02087ed8(0x70);
                }
            }
            if (self->timer <= 0) {
                self->timer++;
                return;
            }
            self->timer = 0;
            self->phase++;
            return;
        case 1:
            EasyFade_FadeBothDisplays(0, 0, 0x333);
            func_ov003_020c8a18(self, (u16)self->timer);
            self->timer++;
            if (EasyFade_IsFading()) {
                return;
            }
            self->phase++;
            return;
        case 2:
            func_ov003_020c8a18(self, (u16)self->timer);
            self->timer++;
            if (self->timer <= 0x5A) {
                return;
            }
            self->phase++;
            return;
        case 3:
            EasyFade_FadeBothDisplays(0, -0x10, 0x266);
            func_ov003_020c8a18(self, (u16)self->timer);
            self->timer++;
            if (EasyFade_IsFading()) {
                return;
            }
            self->timer = 0;
            self->phase++;
            return;
        case 4:
            func_ov003_020c7874(self, 2);
            func_ov003_020c7874(self, 3);
            PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->palettes[0]);
            PaletteMgr_ReleaseResource(g_PaletteManagers[1], self->palettes[1]);
            DatMgr_ReleaseData(self->data);
            func_ov003_020c7f40(self, func_ov003_020c8ddc);
            return;
    }
}

/// Hands the screens back to the battle.
void func_ov003_020c8ddc(BossExp1* self) {
    func_ov003_020c759c();
    data_ov003_020e71b8->unk3D884 = 0;
    data_ov003_020e71b8->unk3D878 &= ~0x1000;
    func_ov003_02083e50(1);
    EasyFade_FadeBothDisplays(0, 0, 0xCCD);
    self->running = 0;
}

s32 func_ov003_020c8e3c(TaskPool* pool, Task* task, void* args) {
    BossExp1*      self  = task->data;
    BossExp1Anims* param = args;

    MI_CpuSet(self, 0, sizeof(*self));
    data_ov003_020e71b8->unk3D878 |= 2;
    data_ov003_020e7254  = 1;
    self->anims.slots[0] = param->slots[0];
    self->anims.slots[1] = param->slots[1];
    self->anims.slots[2] = param->slots[2];
    self->anims.slots[3] = param->slots[3];
    self->anims.actor    = param->actor;
    self->running        = 1;
    func_ov003_020c7f40(self, func_ov003_020c7f58);
    return 1;
}

s32 func_ov003_020c8f30(TaskPool* pool, Task* task, void* args) {
    BossExp1* self = task->data;

    if (self->state != NULL) {
        self->state(self);
    }
    func_ov003_020c7bc8(self);
    func_ov003_020c7de8(self);
    return self->running;
}

s32 func_ov003_020c8f64(TaskPool* pool, Task* task, void* args) {
    BossExp1* self = task->data;

    func_ov003_020c7a54(self);
    func_ov003_020c7c34(self);
    func_ov003_020c7e84(self);
    return 1;
}

s32 func_ov003_020c8f8c(TaskPool* pool, Task* task, void* args) {
    data_ov003_020e71b8->unk3D878 &= ~2;
    data_ov003_020e7254 = 0;
    return 1;
}

s32 func_ov003_020c8fc0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020e0a54;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov003_020c9008(void) {
    return data_ov003_020e7254;
}
