#ifndef COMBAT_CORE_COMBATPSYCHEFFECTS_H
#define COMBAT_CORE_COMBATPSYCHEFFECTS_H

#include "Combat/Core/Combat.h"
#include "Engine/Core/OamMgr.h"
#include <nitro/fx.h>
#include <nitro/math.h>

// List payload shared by target markers and psych target lookup helpers.
typedef struct Ov003TargetArgs {
    CombatList*  list;
    CombatActor* actor;
} Ov003TargetArgs; // Size: 0x8

typedef struct Ov003TargetEntry {
    /* 0x00 */ CombatActor*   actor;
    /* 0x04 */ u32            taskId;
    /* 0x08 */ CombatListNode node;
} Ov003TargetEntry; // Size: 0x14

typedef struct Ov003Target {
    /* 0x00 */ CombatList*      list;
    /* 0x04 */ Ov003TargetEntry entry;
    /* 0x18 */ CombatSprite     sprite;
} Ov003Target; // Size: 0x78

void              func_ov003_020b3050(CombatList* list, Ov003TargetEntry* entry);
void              func_ov003_020b3078(CombatList* list);
Ov003TargetEntry* func_ov003_020b30a0(CombatList* list, CombatActor* actor);
s32               func_ov003_020b30d8(CombatList* list);

typedef struct Ov003ThunderArgs {
    Ov003EffectState* effect;
    CombatList*       targets;
    s32*              finished;
} Ov003ThunderArgs; // Size: 0xC

typedef struct Ov003Thunder {
    /* 0x00 */ s32              phase;
    /* 0x04 */ Ov003ThunderArgs args;
    /* 0x10 */ CombatActor*     target;
    /* 0x14 */ CombatSprite     sprite;
    /* 0x74 */ Vec2p            position;
    /* 0x7C */ s32              speed;
    /* 0x80 */ s32              angle;
} Ov003Thunder; // Size: 0x84

Ov003TargetEntry* func_ov003_020b3094(CombatList* list);
s32               func_ov003_020b30f0(CombatList* list, CombatActor* actor);
s32               func_ov003_020b3108(Ov003EffectState* effect, CombatList* targets, s32* finished);

typedef struct Ov003LockOnThunder {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ CombatList        targets;
    /* 0x0C */ s32               launched;
    /* 0x10 */ s32               finished;
} Ov003LockOnThunder; // Size: 0x14

void func_ov003_020b2fd8(CombatList* list);
void func_ov003_020b2fe4(CombatList* list);
void func_ov003_020b3014(CombatList* list, CombatActor* actor);
void func_ov003_020b33a4(Ov003Thunder* self);
void func_ov003_020b3400(Ov003Thunder* self);
void func_ov003_020b34c0(Ov003Thunder* self);
void func_ov003_020b362c(Ov003Thunder* self);
void func_ov003_020b371c(Ov003Thunder* self);

typedef struct Ov003Lime {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               phase;
    /* 0x08 */ s32               entered;
    /* 0x0C */ CombatSprite      sprite;
    /* 0x6C */ Vec               position;
    /* 0x78 */ Vec               target;
    /* 0x84 */ s32               angle;
    /* 0x88 */ s32               speed;
    /* 0x8C */ CombatSprite      icon;
} Ov003Lime; // Size: 0xEC

s32 func_ov003_020b2e30(CombatList* list, CombatActor* actor);
s32 func_ov003_020b2e58(TaskPool* pool, Task* task, void* args, s32 stage);

typedef struct Ov003LockOnThunderP {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ CombatList        targets;
    /* 0x0C */ CombatSprite      sprite;
    /* 0x6C */ s32               scaleX;
    /* 0x70 */ s32               scaleY;
    /* 0x74 */ s32               opacity;
    /* 0x78 */ s32               angle;
    /* 0x7C */ s16               x;
    /* 0x7E */ s16               y;
    /* 0x80 */ s32               launched;
    /* 0x84 */ s32               finished;
} Ov003LockOnThunderP; // Size: 0x88

typedef struct Ov003LockOnThunderT {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ CombatList        targets;
    /* 0x0C */ u8                timer;
    /* 0x0D */ u8                reserved[3];
    /* 0x10 */ s32               ready;
    /* 0x14 */ s32               launched;
    /* 0x18 */ s32               finished;
    /* 0x1C */ s32               cancelled;
} Ov003LockOnThunderT; // Size: 0x20

void func_ov003_020b3034(CombatList* list, CombatActor* actor);
s32  func_ov003_020b4048(Ov003EffectState* effect, s32 x, s32 y, s32 z, s32 vx, s32 vy, s32 vz, s32 animation, s32 trail);

typedef struct Ov003LineThunder {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ u8                timer;
    /* 0x09 */ u8                reserved;
    /* 0x0A */ s16               x;
    /* 0x0C */ s16               y;
    /* 0x0E */ s16               previousX;
    /* 0x10 */ s16               previousY;
    /* 0x12 */ u16               padding;
} Ov003LineThunder; // Size: 0x14

s32 func_ov003_020b9040(Ov003EffectState* effect, s32 kind, s32 x, s32 y, s32 previousX, s32 previousY, s32 lifetime);

typedef struct Ov003SlashThunderKind {
    s32 animations[3];
    s32 attackId;
    s32 offset;
} Ov003SlashThunderKind; // Size: 0x14

typedef struct Ov003StraightShotKind {
    s32 kind;
    s32 alternateKind;
    u8  mode;
    u8  reserved[3];
    s32 angleLimit;
    u8  count;
    u8  padding[3];
} Ov003StraightShotKind; // Size: 0x14

typedef struct Ov003ShockWaveArgs {
    Ov003EffectState* effect;
    s32               kind;
    Vec               position;
    s32               angle;
    s32               unk_18;
    s32               mode;
    u8                flag;
    u8                reserved[3];
    s32               trail;
} Ov003ShockWaveArgs; // Size: 0x28

typedef struct Ov003BoomerangKind {
    s32 kind;
    u32 angleLimit;
} Ov003BoomerangKind; // Size: 0x8

typedef struct Ov003BoomerangShotS {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ s16               x;
    /* 0x0A */ u16               reserved;
    /* 0x0C */ u32               angle;
    /* 0x10 */ u8                timer;
    /* 0x11 */ u8                padding[3];
} Ov003BoomerangShotS; // Size: 0x14

typedef struct Ov003BoomerangShotT {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ s16               x;
    /* 0x0A */ s16               y;
    /* 0x0C */ u8                timer;
    /* 0x0D */ u8                reserved[3];
} Ov003BoomerangShotT; // Size: 0x10

typedef struct Ov003StraightShot {
    s32               phase;
    Ov003EffectState* effect;
    s16               x;
    s16               y;
    u8                count;
    u8                timer;
    u16               reserved;
    u32               effectTaskId;
} Ov003StraightShot; // Size: 0x14

typedef struct Ov003SlashThunderArgs {
    Ov003EffectState* effect;
    s32               kind;
    s32               x;
    s32               y;
    s32               previousX;
    s32               previousY;
    s32               lifetime;
} Ov003SlashThunderArgs; // Size: 0x1C

typedef struct Ov003SlashThunder {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               phase;
    /* 0x08 */ s32               kind;
    /* 0x0C */ s32               lifetime;
    /* 0x10 */ Vec2p             renderPosition;
    /* 0x18 */ Vec2p             position;
    /* 0x20 */ s32               angle;
    /* 0x24 */ CombatSprite      sprite;
    /* 0x84 */ OamAffineParam    affine;
} Ov003SlashThunder; // Size: 0x94

extern const Ov003SlashThunderKind data_ov003_020dd00c[4];
extern const Ov003StraightShotKind data_ov003_020dcf38[10];
s32                                func_ov003_020b8f3c(Ov003StraightShot* self);

typedef struct Ov003PsychArgs {
    Ov003EffectState* effect;
} Ov003PsychArgs; // Size: 0x4

typedef struct Ov003Psychithunder {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ u8                timer;
    /* 0x09 */ u8                reserved;
    /* 0x0A */ s16               x;
    /* 0x0C */ s16               y;
    /* 0x0E */ u16               padding;
    /* 0x10 */ u32               taskIds[2];
} Ov003Psychithunder; // Size: 0x18

typedef struct Ov003SpeakerArgs {
    Ov003EffectState* effect;
    s32               x;
    s32               y;
    s32               kind;
    const s32*        phase;
} Ov003SpeakerArgs; // Size: 0x14

s32 func_ov003_020b9518(TaskPool* pool, Task* task, void* args, s32 stage);
s32 func_ov003_020b9be0(Ov003EffectState* effect, s32 x, s32 y, s32 kind, const s32* phase);

typedef struct Ov003Speaker {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ s32               phase;
    /* 0x008 */ Vec               position;
    /* 0x014 */ s32               kind;
    /* 0x018 */ const s32*        parentPhase;
    /* 0x01C */ CombatSprite      sprites[3];
    /* 0x13C */ const Vec2p*      scales[3];
    /* 0x148 */ s32               frame;
} Ov003Speaker; // Size: 0x14C

typedef struct Ov003VoiceShockArgs {
    Ov003EffectState* effect;
    s32               x;
    s32               y;
} Ov003VoiceShockArgs; // Size: 0xC

typedef struct Ov003VoiceShock {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Vec               position;
    /* 0x10 */ s32               scale;
    /* 0x14 */ CombatSprite      sprite;
} Ov003VoiceShock; // Size: 0x74

s32 func_ov003_020ba08c(Ov003EffectState* effect, s32 x, s32 y);

typedef struct Ov003StreetLiveSpeaker {
    s16 x;
    s16 y;
    s32 kind;
} Ov003StreetLiveSpeaker; // Size: 0x8

typedef struct Ov003StreetLive {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ s32               speakerPhase;
    /* 0x0C */ s32               quietFrames;
    /* 0x10 */ s32               timer;
    /* 0x14 */ CombatSprite      sprite;
    /* 0x74 */ Ov003CameraAnim   camera;
} Ov003StreetLive; // Size: 0x8C

typedef struct Ov003PsychoTerritoryArgs {
    Ov003EffectState* effect;
    s32               x;
    s32               y;
    s32               angle;
} Ov003PsychoTerritoryArgs; // Size: 0x10

typedef struct Ov003PsychoTerritory {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Vec               position;
    /* 0x14 */ s32               angle;
    /* 0x18 */ CombatSprite      sprite;
    /* 0x78 */ OamAffineParam    affine;
    /* 0x88 */ s32               timer;
} Ov003PsychoTerritory; // Size: 0x8C

s32 func_ov003_020ba71c(Ov003EffectState* effect, s32 x, s32 y, s32 angle);

// Parameters consumed by the common psych animation task.
typedef struct Ov003PsychEffectArgs {
    /* 0x00 */ u32 flags;
    /* 0x04 */ Vec position;
    /* 0x10 */ union {
        s32          unk_10;
        CombatActor* actor;
    };
    /* 0x14 */ s32 engine;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 kind;
    /* 0x20 */ s32 sortKey;
    /* 0x24 */ s32 opacity;
    /* 0x28 */ s32 opacityStep;
    /* 0x2C */ s32 angle;
    /* 0x30 */ s32 scaleX;
    /* 0x34 */ s32 scaleY;
    /* 0x38 */ Vec velocity;
    /* 0x44 */ Vec unk_44;
    /* 0x50 */ u16 unk_50;
    /* 0x52 */ u16 pad_52;
} Ov003PsychEffectArgs; // Size: 0x54

// Runtime form of the common psych animation, including its optional affine transform.
typedef struct Ov003PsychAnimation {
    /* 0x00 */ u32            flags;
    /* 0x04 */ Vec            position;
    /* 0x10 */ CombatActor*   actor;
    /* 0x14 */ CombatSprite   sprite;
    /* 0x74 */ OamAffineParam affine;
    /* 0x84 */ s32            sortKey;
    /* 0x88 */ s32            opacity;
    /* 0x8C */ s32            opacityStep;
    /* 0x90 */ s32            angle;
    /* 0x94 */ s32            scaleX;
    /* 0x98 */ s32            scaleY;
    /* 0x9C */ Vec            velocity;
    /* 0xA8 */ Vec            acceleration;
    /* 0xB4 */ u16            timer;
    /* 0xB6 */ u16            pad_B6;
} Ov003PsychAnimation; // Size: 0xB8

typedef struct Ov003BrilliantArgs {
    /* 0x00 */ Vec2p position;
    /* 0x08 */ s32   engine;
    /* 0x0C */ s32   kind;
    /* 0x10 */ s32   sortKey;
    /* 0x14 */ s32   radius;
    /* 0x18 */ s32   fixedRadius;
    /* 0x1C */ s32   lifetime;
} Ov003BrilliantArgs; // Size: 0x20

typedef struct Ov003BrilliantEffect {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec2p        position;
    /* 0x68 */ Vec2p        offset;
    /* 0x70 */ s32          kind;
    /* 0x74 */ s32          radius;
    /* 0x78 */ s32          fixedRadius;
    /* 0x7C */ s32          lifetime;
} Ov003BrilliantEffect; // Size: 0x80

typedef struct Ov003AbsorbArgs {
    Ov003EffectSlots* slots;
    s32               value;
} Ov003AbsorbArgs; // Size: 0x8

void func_ov003_020a912c(Ov003EffectSlots* slots, s32 value);

typedef struct Ov003FireChildArgs {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Vec2p             position;
    /* 0x0C */ s32               kind;
    /* 0x10 */ s32               attackId;
    /* 0x14 */ s32               scale;
    /* 0x18 */ s32               speed;
    /* 0x1C */ s32               angle;
    /* 0x20 */ u32               parentTaskId;
} Ov003FireChildArgs; // Size: 0x24

typedef struct Ov003FireChild {
    /* 0x00 */ s32               state;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Vec2p             position;
    /* 0x10 */ CombatSprite      sprite;
    /* 0x70 */ s32               attackId;
    /* 0x74 */ s32               speed;
    /* 0x78 */ s32               angle;
    /* 0x7C */ OamAffineParam    affine;
    /* 0x8C */ s32               scale;
    /* 0x90 */ s32               opacity;
    /* 0x94 */ u32               parentTaskId;
} Ov003FireChild; // Size: 0x98

typedef struct Ov003FireWave {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Vec2p             position;
    /* 0x0C */ s16               activeTicks;
    /* 0x0E */ u16               flags;
    /* 0x10 */ s16               wasHeld;
    /* 0x12 */ s16               phase;
    /* 0x14 */ s32               hasSpawned;
    /* 0x18 */ s32               exhausted;
    /* 0x1C */ s16               emitTimer;
    /* 0x1E */ u16               pad_1E;
    /* 0x20 */ u64               seed;
    /* 0x28 */ u64               multiplier;
    /* 0x30 */ u64               increment;
} Ov003FireWave; // Size: 0x38

typedef struct Ov003BadgeFormationArgs {
    Ov003EffectState* effect;
    Ov003TouchActor*  actors[4];
} Ov003BadgeFormationArgs; // Size: 0x14

typedef struct Ov003BadgeFormation {
    /* 0x00 */ s32               state;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Ov003TouchActor*  actors[4];
    /* 0x18 */ s32               index;
    /* 0x1C */ s32               firstIndex;
    /* 0x20 */ CombatSprite      sprite;
    /* 0x80 */ Vec2p             position;
    /* 0x88 */ s32               angle;
} Ov003BadgeFormation; // Size: 0x8C

typedef struct Ov003BarrierSpawn {
    Ov003EffectState* effect;
    s32               mode;
    s32               kind;
} Ov003BarrierSpawn; // Size: 0xC

typedef struct Ov003BarrierArgs {
    Ov003BarrierSpawn spawn;
    s32               automatic;
} Ov003BarrierArgs; // Size: 0x10

typedef struct Ov003Barrier {
    /* 0x000 */ Ov003BarrierSpawn spawn;
    /* 0x00C */ s32               animationKind;
    /* 0x010 */ CombatSprite      sprites[4];
    /* 0x190 */ OamAffineParam    affine;
    /* 0x1A0 */ s32               unk_1A0;
    /* 0x1A4 */ s32               cooldown;
    /* 0x1A8 */ s32               phase;
} Ov003Barrier; // Size: 0x1AC

void func_ov003_020aa32c(Ov003EffectState*, s32 mode, s32 kind);

typedef struct Ov003BlackHollArgs {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s16               screenX;
    /* 0x06 */ s16               screenY;
    /* 0x08 */ s32               mode;
    /* 0x0C */ s32               radius;
    /* 0x10 */ s32               minSpeed;
    /* 0x14 */ s32               maxSpeed;
    /* 0x18 */ s32               lifetime;
} Ov003BlackHollArgs; // Size: 0x1C

typedef struct Ov003BlackHoll {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ s32               mode;
    /* 0x008 */ s32               lifetime;
    /* 0x00C */ CombatSprite      sprites[2];
    /* 0x0CC */ Ov003AnimSprite   particles[6];
    /* 0x36C */ Vec               position;
    /* 0x378 */ s32               minSpeed;
    /* 0x37C */ s32               slope;
    /* 0x380 */ s32               intercept;
} Ov003BlackHoll; // Size: 0x384

void func_ov003_020aacd4(Ov003EffectState*, s16 screenX, s16 screenY, s32 mode, s32 radius, s32 minSpeed, s32 maxSpeed,
                         s32 lifetime);

typedef struct Ov003BombArgs {
    /* 0x00 */ Ov003AttackStreak* streak;
    /* 0x04 */ s32                kind;
    /* 0x08 */ Vec                position;
    /* 0x14 */ s32                angle;
    /* 0x18 */ s32                speed;
} Ov003BombArgs; // Size: 0x1C

typedef struct Ov003Bomb {
    /* 0x00 */ Ov003BombArgs args;
    /* 0x1C */ CombatSprite  sprite;
} Ov003Bomb; // Size: 0x7C

typedef struct Ov003PsychPositionArgs {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Vec               position;
} Ov003PsychPositionArgs; // Size: 0x10

typedef struct Ov003FireWorks {
    /* 0x00 */ s32                    phase;
    /* 0x04 */ Ov003PsychPositionArgs args;
    /* 0x14 */ CombatSprite           sprite;
    /* 0x74 */ s32                    timer;
    /* 0x78 */ s32                    particleTimer;
} Ov003FireWorks; // Size: 0x7C

void func_ov003_020ab524(Ov003AttackStreak*, s32 kind, s32 x, s32 y, s32 z, s32 angle, s32 speed);
void func_ov003_020ab7f8(Ov003EffectState*, s32 x, s32 y, s32 z);

/// The effect state plus the unit a unit-bound psych task acts on.
typedef struct Ov003PsychUnitArgs {
    Ov003EffectState* effect;
    Ov003Unit*        unit;
} Ov003PsychUnitArgs; // Size: 8

typedef struct Ov003ObsBomb {
    /* 0x00 */ s32                phase;
    /* 0x04 */ Ov003PsychUnitArgs args;
    /* 0x0C */ CombatSprite       sprite;
    /* 0x6C */ s32                timer;
} Ov003ObsBomb; // Size: 0x70

typedef struct Ov003PsychoBomb {
    /* 0x00 */ Ov003PsychPositionArgs args;
    /* 0x10 */ CombatSprite           sprites[2];
    /* 0xD0 */ s32                    timer;
    /* 0xD4 */ s32                    phase;
} Ov003PsychoBomb; // Size: 0xD8

void func_ov003_020abb34(Ov003EffectState*, Ov003Unit*);
void func_ov003_020abe30(Ov003EffectState*, s32 x, s32 y, s32 z);

typedef struct Ov003ChargeLaserArgs {
    /* 0x00 */ s32               mode;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Ov003TouchActor*  actors[2];
} Ov003ChargeLaserArgs; // Size: 0x10

typedef struct Ov003ChargeLaser {
    /* 0x00 */ s32               phase;
    /* 0x04 */ s32               mode;
    /* 0x08 */ Ov003EffectState* effect;
    /* 0x0C */ Ov003TouchActor*  actors[2];
    /* 0x14 */ s32               soundPlayed;
    /* 0x18 */ CombatSprite      sprite;
    /* 0x78 */ s32               animationKind;
    /* 0x7C */ s16               x;
    /* 0x7E */ s16               y;
    /* 0x80 */ s16               targetX;
    /* 0x82 */ s16               pad_82;
    /* 0x84 */ s32               facingLeft;
    /* 0x88 */ u8                timer;
    /* 0x89 */ u8                pad_89[3];
    /* 0x8C */ s32               particleTimer;
} Ov003ChargeLaser; // Size: 0x90

void func_ov003_020acc60(Ov003ChargeLaser*);

typedef struct Ov003Blur {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s32          captured;
    /* 0x64 */ Vec          position;
    /* 0x70 */ s8           opacity;
    /* 0x71 */ u8           pad_71[3];
    /* 0x74 */ s32          fixedOpacity;
    /* 0x78 */ s32          active;
} Ov003Blur; // Size: 0x7C

typedef struct Ov003ShockWaveBlurArgs {
    /* 0x00 */ s32 animation;
    /* 0x04 */ Vec position;
    /* 0x10 */ s32 angle;
} Ov003ShockWaveBlurArgs; // Size: 0x14

typedef struct Ov003ShockWaveBlur {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          position;
    /* 0x6C */ s8           opacity;
    /* 0x6D */ u8           reserved[3];
    /* 0x70 */ s32          fixedOpacity;
    /* 0x74 */ s32          angle;
} Ov003ShockWaveBlur; // Size: 0x78

typedef struct Ov003HealArgs {
    Ov003EffectState* effect;
    Ov003TouchActor*  actor;
} Ov003HealArgs; // Size: 0x8

typedef struct Ov003HealAnimations {
    s32 foreground;
    s32 background;
} Ov003HealAnimations; // Size: 0x8

typedef struct Ov003DragAttack {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               phase;
    /* 0x08 */ CombatSprite      sprite;
    /* 0x68 */ s32               angle;
    /* 0x6C */ s32               opacity;
    /* 0x70 */ s32               opacityStep;
    /* 0x74 */ s32               attackId;
} Ov003DragAttack; // Size: 0x78

typedef struct Ov003SparkArgs {
    /* 0x00 */ Ov003AttackStreak* streak;
    /* 0x04 */ s32                animation;
    /* 0x08 */ Vec2p              position;
    /* 0x10 */ s32                angle;
    /* 0x14 */ s32                scale;
    /* 0x18 */ s32                speed;
    /* 0x1C */ s32                acceleration;
    /* 0x20 */ u32                lifetime;
    /* 0x24 */ s32                range;
} Ov003SparkArgs; // Size: 0x28

typedef struct Ov003Spark {
    /* 0x00 */ Ov003AttackStreak* streak;
    /* 0x04 */ CombatSprite       sprite;
    /* 0x64 */ OamAffineParam     affine;
    /* 0x74 */ Vec2p              position;
    /* 0x7C */ s32                angle;
    /* 0x80 */ s32                scale;
    /* 0x84 */ s32                speed;
    /* 0x88 */ s32                acceleration;
    /* 0x8C */ u32                ticks; // retail tests this counter without incrementing it in this task
    /* 0x90 */ s32                traveled;
    /* 0x94 */ u32                lifetime;
    /* 0x98 */ s32                range;
    /* 0x9C */ u32                trailIds[3];
} Ov003Spark; // Size: 0xA8

typedef struct Ov003ElementKRay {
    /* 0x00 */ s32          active;
    /* 0x04 */ CombatSprite sprite;
    /* 0x64 */ s32          angle;
} Ov003ElementKRay; // Size: 0x68

typedef struct Ov003ElementK {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ s32               phase;
    /* 0x008 */ s32               heldFrames;
    /* 0x00C */ s32               releasedFrames;
    /* 0x010 */ CombatSprite      sprite;
    /* 0x070 */ s16               x;
    /* 0x072 */ s16               y;
    /* 0x074 */ Ov003ElementKRay  rays[3];
    /* 0x1AC */ s32               emitTimer;
    /* 0x1B0 */ u8                directions[8];
    /* 0x1B8 */ s32               directionIndex;
    /* 0x1BC */ CombatSprite      aura;
    /* 0x21C */ s32               auraOpacity;
    /* 0x220 */ s32               auraScale;
    /* 0x224 */ s32               cancelled;
} Ov003ElementK; // Size: 0x228

typedef struct Ov003ElementR {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ Vec2p             position;
    /* 0x00C */ s32               delay;
    /* 0x010 */ s32               releasedFrames;
    /* 0x014 */ CombatSprite      sprites[3];
    /* 0x134 */ s32               phase;
    /* 0x138 */ u8                directions[16];
    /* 0x148 */ s32               directionIndex;
    /* 0x14C */ s32               auraOpacity;
    /* 0x150 */ s32               cancelled;
} Ov003ElementR; // Size: 0x154

typedef struct Ov003ElementArgs {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               mode;
    /* 0x08 */ s32               kind;
    /* 0x0C */ s16               x;
    /* 0x0E */ s16               y;
    /* 0x10 */ s32               angle;
    /* 0x14 */ s32               speed;
    /* 0x18 */ s32               scale;
    /* 0x1C */ s32               uses;
} Ov003ElementArgs; // Size: 0x20

typedef struct Ov003Element {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               mode;
    /* 0x08 */ s32               kind;
    /* 0x0C */ Vec               position;
    /* 0x18 */ Vec2p             velocity;
    /* 0x20 */ s32               radius;
    /* 0x24 */ s32               opacity;
    /* 0x28 */ s32               angle;
    /* 0x2C */ s32               speed;
    /* 0x30 */ s32               deflectionAngle;
    /* 0x34 */ s32               deflectionSpeed;
    /* 0x38 */ s32               firstTick;
    /* 0x3C */ s32               remainingUses;
    /* 0x40 */ s32               remainingTicks;
    /* 0x44 */ CombatSprite      sprite;
    /* 0xA4 */ s32               scale;
    /* 0xA8 */ OamAffineParam    affine;
    /* 0xB8 */ CombatActor*      target;
    /* 0xBC */ s32               paused;
    /* 0xC0 */ s32               turnTimer;
} Ov003Element; // Size: 0xC4

typedef struct Ov003ElementKind {
    s32 startSound;
    s32 loopSound;
    s32 hitSound;
    s32 attackId;
    s32 animation;
} Ov003ElementKind; // Size: 0x14

extern const Ov003ElementKind data_ov003_020dc7b8[8];
s32 func_ov003_020ae414(Ov003EffectState*, s32 mode, s32 kind, s16 x, s16 y, s32 angle, s32 speed, s32 scale, s32 uses);

typedef struct Ov003PsychByteTimer {
    Ov003EffectState* effect;
    u8                timer;
    u8                reserved[3];
} Ov003PsychByteTimer; // Size: 8

typedef struct Ov003ElementTArgs {
    Ov003EffectState* effect;
    s32               kind;
} Ov003ElementTArgs; // Size: 8

typedef struct Ov003ElementT {
    Ov003ElementTArgs args;
    u8                timer;
    u8                reserved[3];
} Ov003ElementT; // Size: 0xC

typedef struct Ov003ElementBindArgs {
    Ov003EffectState* effect;
    Ov003Unit*        unit;
    s32               kind;
    s32               attackId;
} Ov003ElementBindArgs; // Size: 0x10

s32 func_ov003_020af75c(Ov003EffectState* effect, Ov003Unit* unit, s32 kind, s32 attackId);

typedef struct Ov003ElementBind {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Ov003Unit*        unit;
    /* 0x08 */ Vec               position;
    /* 0x14 */ s16               height;
    /* 0x16 */ s16               reserved;
    /* 0x18 */ s32               kind;
    /* 0x1C */ CombatSprite      sprite;
    /* 0x7C */ s32               timer;
    /* 0x80 */ s32               attackId;
} Ov003ElementBind; // Size: 0x84

typedef struct Ov003ElementBindSounds {
    s32 start;
    s32 hold;
    s32 release;
} Ov003ElementBindSounds; // Size: 0xC

typedef struct Ov003SnowArgs {
    s32   kind;
    Vec2p position;
    s32   angle;
    s32   speed;
} Ov003SnowArgs; // Size: 0x14

typedef struct Ov003Snow {
    /* 0x00 */ s32            kind;
    /* 0x04 */ CombatSprite   sprite;
    /* 0x64 */ OamAffineParam affine;
    /* 0x74 */ Vec2p          position;
    /* 0x7C */ s32            angle;
    /* 0x80 */ s32            rotation;
    /* 0x84 */ s32            speed;
    /* 0x88 */ s32            acceleration;
    /* 0x8C */ s32            ticks;
} Ov003Snow; // Size: 0x90

typedef struct Ov003IceBind {
    Ov003PsychUnitArgs args;
    s32                frames;
    s32                particleTimer;
} Ov003IceBind; // Size: 0x10

typedef struct Ov003PsychUnitTimer {
    Ov003PsychUnitArgs args;
    u8                 timer;
    u8                 reserved[3];
} Ov003PsychUnitTimer; // Size: 0xC

typedef struct Ov003EnemyAirFlowArgs {
    Ov003EffectState* effect;
    Ov003Unit*        unit;
    s32*              releaseFlag;
} Ov003EnemyAirFlowArgs; // Size: 0xC

typedef struct Ov003EnemyAirFlow {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Ov003Unit*        unit;
    /* 0x0C */ CombatSprite      sprite;
    /* 0x6C */ s32*              releaseFlag;
    /* 0x70 */ Vec               origin;
    /* 0x7C */ s32               radius;
    /* 0x80 */ s32               angle;
    /* 0x84 */ s32               zVelocity;
    /* 0x88 */ s32               landed;
    /* 0x8C */ s32               firstTick;
} Ov003EnemyAirFlow; // Size: 0x90

typedef struct Ov003PsychoQuake {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               phase;
    /* 0x08 */ Ov003CameraAnim   camera;
    /* 0x20 */ s32               remainingTicks;
    /* 0x24 */ s32               releaseTimer;
    /* 0x28 */ s32               sampleTimer;
    /* 0x2C */ CombatSprite      spriteA;
    /* 0x8C */ CombatSprite      spriteB;
    /* 0xEC */ s32               spriteOffset;
    /* 0xF0 */ s32               opacity;
} Ov003PsychoQuake; // Size: 0xF4

typedef struct Ov003PsychoTyphoon {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               phase;
    /* 0x08 */ s32               remainingTicks;
    /* 0x0C */ s32               releaseTimer;
    /* 0x10 */ s32               releaseFlag;
} Ov003PsychoTyphoon; // Size: 0x14

typedef struct Ov003FireHold {
    /* 0x00 */ s32               state; // 0 = ignite, 1 = held
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Ov003Unit*        unit;
    /* 0x0C */ s32               timer;
    /* 0x10 */ s32               particleTimer;
    /* 0x14 */ s32               holdTimer; // 0x3C while the actor stays held
    /* 0x18 */ s32               frames;
} Ov003FireHold;                            // Size: 0x1C

typedef struct Ov003IceHold {
    /* 0x00 */ s32               state; // 0 = freeze in, 1 = held, 2 = breaking, 3 = thawing
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ Ov003Unit*        unit;
    /* 0x0C */ s32               timer;
    /* 0x10 */ s32               particleTimer;
    /* 0x14 */ s32               frames;
    /* 0x18 */ CombatSprite      spriteA;
    /* 0x78 */ CombatSprite      spriteB;
} Ov003IceHold; // Size: 0xD8

typedef struct Ov003OpticalHold {
    /* 0x000 */ s32               state;
    /* 0x004 */ Ov003EffectState* effect;
    /* 0x008 */ Ov003Unit*        unit;
    /* 0x00C */ s32               spawnTimer;
    /* 0x010 */ s32               holdTimer;
    /* 0x014 */ s32               heldFrames;
    /* 0x018 */ CombatSprite      sprites[3];
    /* 0x138 */ s32               angle;
    /* 0x13C */ Ov003AnimSprite   particles[6];
} Ov003OpticalHold; // Size: 0x3DC

typedef struct Ov003IcePillarArgs {
    Ov003EffectState* effect;
    Vec2p             position;
    s32               attackId;
} Ov003IcePillarArgs; // Size: 0x10

typedef struct Ov003IcePillar {
    /* 0x000 */ Ov003IcePillarArgs args;
    /* 0x010 */ CombatSprite       sprites[4];
} Ov003IcePillar; // Size: 0x190

s32 func_ov003_020b1ca0(Ov003EffectState* effect, s32 x, s32 y, s32 attackId);

typedef struct Ov003IcePillarR {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ u8                count;
    /* 0x005 */ u8                pad_005[3];
    /* 0x008 */ s32               x[3];
    /* 0x014 */ s32               y[3];
    /* 0x020 */ s32               z;
    /* 0x024 */ s32               phase;
    /* 0x028 */ CombatSprite      spriteA;
    /* 0x088 */ CombatSprite      spriteB;
    /* 0x0E8 */ CombatSprite      spriteC;
    /* 0x148 */ s32               xOffset;
    /* 0x14C */ u8                timer;
    /* 0x14D */ u8                pad_14D[3];
} Ov003IcePillarR; // Size: 0x150

typedef struct Ov003IcePillarS {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               x;
    /* 0x08 */ s32               y;
    /* 0x0C */ u8                count;
    /* 0x0D */ u8                timer;
    /* 0x0E */ u8                pad_0E[2];
    /* 0x10 */ s32               step;
} Ov003IcePillarS; // Size: 0x14

typedef struct Ov003PsychoFallT {
    /* 0x00 */ s32               state;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ s16               x;
    /* 0x0A */ s16               y;
    /* 0x0C */ u8                timer;
    /* 0x0D */ u8                pad_0D[3];
} Ov003PsychoFallT; // Size: 0x10

typedef struct Ov003PsychoFallZ {
    /* 0x000 */ Ov003PsychTrail trail;
    /* 0x2B4 */ s32             phase;
    /* 0x2B8 */ s16             x;
    /* 0x2BA */ s16             y;
    /* 0x2BC */ u8              spawnCount;
    /* 0x2BD */ u8              spawnTimer;
    /* 0x2BE */ u8              pad_2BE[2];
} Ov003PsychoFallZ; // Size: 0x2C0

typedef struct Ov003PsychoFallKind {
    /* 0x00 */ s32 animation;
    /* 0x04 */ u16 frames;
    /* 0x06 */ s16 pad_06;
    /* 0x08 */ s32 shake;
    /* 0x0C */ s32 attackId;
    /* 0x10 */ s32 particleCount;
    /* 0x14 */ s32 radiusBase;
    /* 0x18 */ s32 trail;
} Ov003PsychoFallKind; // Size: 0x1C

typedef struct Ov003PsychoFallArgs {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               index;
    /* 0x08 */ s32               x;
    /* 0x0C */ s32               y;
    /* 0x10 */ s32               z;
    /* 0x14 */ s32               zStep;
} Ov003PsychoFallArgs; // Size: 0x18

typedef struct Ov003BrokenMeteorArgs {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Vec               position;
    /* 0x10 */ Vec               velocity;
    /* 0x1C */ s32               animation;
    /* 0x20 */ s32               trail;
} Ov003BrokenMeteorArgs; // Size: 0x24

typedef struct Ov003BrokenMeteor {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ CombatSprite      sprite;
    /* 0x64 */ s32               loopAnimation;
    /* 0x68 */ Vec               position;
    /* 0x74 */ s32               baseZ;
    /* 0x78 */ Vec               velocity;
    /* 0x84 */ CombatSprite      icon;
    /* 0xE4 */ s32               trail;
} Ov003BrokenMeteor; // Size: 0xE8

s32 func_ov003_020b42b4(Ov003EffectState* effect, s32 index, s32 x, s32 y, s32 z, s32 zStep);

typedef struct Ov003PsychoFall {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ s32               index;
    /* 0x008 */ s32               phase;
    /* 0x00C */ CombatSprite      sprite;
    /* 0x06C */ s32               x;
    /* 0x070 */ s32               y;
    /* 0x074 */ s32               z;
    /* 0x078 */ s32               baseZ;
    /* 0x07C */ s32               zStep;
    /* 0x080 */ CombatSprite      icon;
    /* 0x0E0 */ u16               variant;
    /* 0x0E2 */ u8                pad_E2[2];
} Ov003PsychoFall; // Size: 0xE4

typedef struct Ov003PsychoHitContext {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               activated; // the parent landed its attack
    /* 0x08 */ s32               finished;  // the parent task has ended
    /* 0x0C */ CombatActor*      target;
    /* 0x10 */ Vec               targetPosition;
    /* 0x1C */ u8                refCount;
} Ov003PsychoHitContext; // Size: 0x20

typedef struct Ov003PsychoT {
    /* 0x00 */ Ov003EffectState*     effect;
    /* 0x04 */ s32                   phase;
    /* 0x08 */ Ov003PsychoHitContext hit;
    /* 0x28 */ u8                    timer;
    /* 0x29 */ u8                    reserved[3];
    /* 0x2C */ CombatSprite          sprite;
} Ov003PsychoT; // Size: 0x8C

typedef struct Ov003PsychoUnitArgs {
    Ov003Unit*             unit;
    Ov003PsychoHitContext* context;
} Ov003PsychoUnitArgs; // Size: 0x8

typedef struct Ov003PsychoUnit {
    /* 0x00 */ s32                    phase;
    /* 0x04 */ Ov003Unit*             unit;
    /* 0x08 */ Ov003PsychoHitContext* context;
    /* 0x0C */ u16                    timer;
    /* 0x0E */ u16                    pad_0E;
    /* 0x10 */ CombatSprite           sprite;
    /* 0x70 */ s32                    angle;
    /* 0x74 */ s32                    unk_74;
} Ov003PsychoUnit; // Size: 0x78

typedef struct Ov003PsychoTuning {
    u16 holdFrames[3];
    u16 lifetimeCost[3];
    u16 turnStep;
} Ov003PsychoTuning; // Size: 0xE

typedef struct Ov003PsychoTuningTable {
    Ov003PsychoTuning rows[3];
    u16               padding;
} Ov003PsychoTuningTable; // Size: 0x2C

s32 func_ov003_020b0188(Ov003EffectState* effect, Ov003Unit* unit, s32* releaseFlag);

s32 func_ov003_020afb64(s32 kind, s32 x, s32 y, s32 angle, s32 speed);

void         func_ov003_020ae980(Ov003Element*);
void         func_ov003_020aeb0c(Ov003Element*);
void         func_ov003_020aecbc(Ov003Element*);
void         func_ov003_020aedf0(Ov003Element*);
void         func_ov003_020aee9c(Ov003Element*);
void         func_ov003_020aefc0(Ov003Element*);
CombatActor* func_ov003_020af1e0(Ov003Element*);

s32  func_ov003_020aced0(Ov003EffectState*);
void func_ov003_020acef4(s32 taskId);
void func_ov003_020acf0c(s32 taskId, u16 angle);
s32  func_ov003_020ad280(Ov003AttackStreak*, s32 x, s32 y, s32 angle, s32 speed, s32 scale);

void func_ov003_020a94d8(Ov003EffectState* effect, s32 x, s32 y, s32 kind, s32 attackId, s32 scale, s32 speed, s32 angle,
                         u32 parentTaskId);

s32  func_ov003_020a8e74(s32 x, s32 y, s32 engine, s32 kind, s32 radius, s32 fixedRadius, s32 lifetime);
void func_ov003_020a90c4(u32 taskId);
BOOL func_ov003_020a90e8(u32* taskId);
void func_ov003_020a910c(u32 taskId, s32 x, s32 y);

s32  func_ov003_020a88a8(Ov003PsychEffectArgs*);
void func_ov003_020a88c0(Ov003PsychEffectArgs*, s32 x, s32 y, s32 z, s32 engine, s32 kind);
void func_ov003_020a8908(Ov003PsychEffectArgs*, s32 x, s32 y, s32 engine, s32 kind);
void func_ov003_020a8958(Ov003PsychEffectArgs*, CombatActor* actor, s32 engine, s32 kind);
void func_ov003_020a899c(Ov003PsychEffectArgs*, s32 sortKey);
void func_ov003_020a89b0(Ov003PsychEffectArgs*, s32 opacity);
void func_ov003_020a89c8(Ov003PsychEffectArgs*, s32 angle, s32 scaleX, s32 scaleY);
void func_ov003_020a89e4(Ov003PsychEffectArgs*);
void func_ov003_020a89f4(Ov003PsychEffectArgs*, s32, s32);
void func_ov003_020a8a0c(Ov003PsychEffectArgs*, s32 x, s32 y, s32 z);
void func_ov003_020a8a28(Ov003PsychEffectArgs*, u16);

// Recovered burst, stroke, drag and shock-wave task layouts.
typedef struct Ov003BurstShot {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ s16               x;
    /* 0x0A */ s16               y;
    /* 0x0C */ s16               originX;
    /* 0x0E */ s16               originY;
    /* 0x10 */ u8                timer;
    /* 0x11 */ u8                reserved[3];
    /* 0x14 */ s32               burstTimer;
    /* 0x18 */ s32               soundPlayed;
} Ov003BurstShot; // Size: 0x1C

typedef struct Ov003BurstShotKind {
    /* 0x0 */ s32 kind;
    /* 0x4 */ s32 mode;
    /* 0x8 */ s32 animation;
    /* 0xC */ s32 angleLimit;
} Ov003BurstShotKind; // Size: 0x10

typedef struct Ov003StraightShotS {
    /* 0x00 */ s32               phase;
    /* 0x04 */ Ov003EffectState* effect;
    /* 0x08 */ s32               angle;
    /* 0x0C */ u8                timer;
    /* 0x0D */ u8                burstTimer;
    /* 0x0E */ u8                burstCount;
    /* 0x0F */ u8                reserved;
    /* 0x10 */ s16               x;
    /* 0x12 */ u16               padding;
    /* 0x14 */ u32               taskId;
} Ov003StraightShotS; // Size: 0x18

typedef struct Ov003StraightShotSKind {
    /* 0x0 */ s32 kind;
    /* 0x4 */ u8  count;
    /* 0x5 */ u8  mode;
    /* 0x6 */ u8  directAngle; // 1 = aim along the stroke instead of fanning out
    /* 0x7 */ u8  repeat;      // 1 = the held stroke may launch another volley
    /* 0x8 */ s32 angleLimit;
} Ov003StraightShotSKind;      // Size: 0xC

typedef struct Ov003PsychoD {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ Ov003Unit*        unit;
    /* 0x08 */ u16               flags;
    /* 0x0A */ u16               pad_0A;
    /* 0x0C */ s32               unk_0C;
    /* 0x10 */ s32               x;
    /* 0x14 */ s32               y;
    /* 0x18 */ s32               angle;
    /* 0x1C */ s32               radius;
    /* 0x20 */ s32               phase;
    /* 0x24 */ s16               subPhase;
    /* 0x26 */ s16               timer;
    /* 0x28 */ CombatSprite      spriteA;
    /* 0x88 */ CombatSprite      spriteB;
} Ov003PsychoD; // Size: 0xE8

typedef struct Ov003ShockWaveKind {
    /* 0x00 */ s32 speed;           // initial travel speed
    /* 0x04 */ s32 acceleration;
    /* 0x08 */ s32 animation;       // sprite animation
    /* 0x0C */ s32 effectAnimation; // travelling effect animation
    /* 0x10 */ s32 attackKind;
    /* 0x14 */ s32 trailSound;      // played while the wave trails
    /* 0x18 */ s32 spawnSound;      // played while spawning child waves
} Ov003ShockWaveKind;               // Size: 0x1C

typedef struct Ov003ShockWaveBody {
    /* 0x00 */ Ov003EffectState* effect;
    /* 0x04 */ s32               kind;
    /* 0x08 */ s32               x;
    /* 0x0C */ s32               y;
    /* 0x10 */ s32               z;
    /* 0x14 */ s32               angle;
    /* 0x18 */ s32               speed;
    /* 0x1C */ s32               acceleration;
    /* 0x20 */ s32               distance;
    /* 0x24 */ s32               distanceLimit;
    /* 0x28 */ s32               scale;
    /* 0x2C */ s32               targetAngle;
    /* 0x30 */ s32               phase;
    /* 0x34 */ CombatSprite      sprite;
    /* 0x94 */ OamAffineParam    affine;
    /* 0xA4 */ s32               animation;
    /* 0xA8 */ s32               renderAngle;
    /* 0xAC */ s32               spinTimer;
    /* 0xB0 */ s32               mode;
    /* 0xB4 */ s32               firstTick;
    /* 0xB8 */ u8                trailIndex;
    /* 0xB9 */ u8                pad_B9[3];
} Ov003ShockWaveBody; // Size: 0xBC

#endif
