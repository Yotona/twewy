#ifndef COMBAT_COMBATTOUCH_H
#define COMBAT_COMBATTOUCH_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatAttack.h"
#include "Combat/Core/CombatList.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"

typedef union Ov003PsychParamsHead {
    struct {
        u16 unk_00;
        u16 unk_02;
    };
    u32 raw; // actor creation copies PsyData records a word at a time
} Ov003PsychParamsHead;

typedef struct Ov003PsychParams {
    /* 0x00 */ Ov003PsychParamsHead head;
    /* 0x04 */ u8                   unk_04;
    /* 0x05 */ u8                   unk_05; // psych kind
    /* 0x06 */ u16                  unk_06;
    /* 0x08 */ u16                  unk_08;
    /* 0x0A */ s16                  unk_0A;
    /* 0x0C */ s16                  unk_0C;
    /* 0x0E */ u16                  unk_0E;
    /* 0x10 */ s16                  unk_10;
    /* 0x12 */ u16                  unk_12;
    /* 0x14 */ u16                  unk_14;
    /* 0x16 */ s16                  unk_16;
    /* 0x18 */ u16                  unk_18;
    /* 0x1A */ u16                  unk_1A;
    /* 0x1C */ s16                  unk_1C;
    /* 0x1E */ u16                  unk_1E;
    /* 0x20 */ u16                  unk_20;
    /* 0x22 */ s16                  unk_22;
    /* 0x24 */ u16                  unk_24;
    /* 0x26 */ u16                  unk_26;
    /* 0x28 */ u16                  unk_28;
    /* 0x2A */ u16                  unk_2A;
    /* 0x2C */ u16                  unk_2C;
    /* 0x2E */ u8                   unk_2E;
    /* 0x2F */ u8                   unk_2F;
    /* 0x30 */ u8                   unk_30;
    /* 0x31 */ u8                   unk_31;
    /* 0x32 */ u16                  unk_32;
} Ov003PsychParams; // Size: 0x34

typedef struct Ov003EffectContext {
    /* 0x0 */ s32 (*callback)(void*); // run on start with `arg`
    /* 0x4 */ TaskHandle* handle;     // optional task spawned on start
    /* 0x8 */ void*       arg;        // passed to `callback`/the task
    /* 0xC */ s32 (*post)(void*);     // run after the effect start callback
} Ov003EffectContext;                 // Size: 0x10

typedef struct Ov003EffectState {
    union {
        struct {
            /* 0x00 */ s32 state; // table index; 0x3C while starting
            /* 0x04 */ s32 index; // per-actor velocity ramp slot
            /* 0x08 */ s16 unk_08;
            /* 0x0A */ s16 unk_0A;
            /* 0x0C */ s16 unk_0C;
            /* 0x0E */ s16 unk_0E;
            /* 0x10 */ s16 unk_10;
            /* 0x12 */ s16 unk_12;
            /* 0x14 */ u8  flag;
            /* 0x15 */ u8  unk_15[3];
        };
        Ov003AttackStreak streak;
    };
    union {
        /* 0x18 */ u8  data[0x10];
        /* 0x18 */ s32 params[4]; // kind-specific PsyData words
    };
    /* 0x28 */ s32                result;
    /* 0x2C */ Ov003EffectContext context;
} Ov003EffectState; // Size: 0x3C

/// The spawn record `func_ov003_020a3b9c` builds a touch actor from (also read by the badge tasks).
typedef struct Ov003ActorSpawn {
    /* 0x00 */ u16                     kind;
    /* 0x02 */ u16                     unk_02;
    /* 0x04 */ u8                      unk_04;
    /* 0x05 */ u8                      unk_05[3];
    /* 0x08 */ const Ov003PsychParams* params;
    /* 0x0C */ u8                      slot;
    /* 0x0D */ u8                      unk_0D[3];
} Ov003ActorSpawn; // Size: 0x10

typedef struct Ov003TouchActor {
    /* 0x000 */ s32              state; // 0..4, 0xFF = free
    /* 0x004 */ s32              request;
    /* 0x008 */ u8               slot;
    /* 0x009 */ u8               unk_009[1];
    /* 0x00A */ u16              kind; // 0xFFFE = the player actor
    /* 0x00C */ u16              unk_00C;
    /* 0x00E */ u8               unk_00E;
    /* 0x00F */ u8               unk_00F;
    /* 0x010 */ Ov003PsychParams params;
    /* 0x044 */ s16              unk_044;
    /* 0x046 */ s16              unk_046;
    /* 0x048 */ s16              unk_048;
    /* 0x04A */ u8               unk_04A[2];
    /* 0x04C */ s32              unk_04C;
    /* 0x050 */ s16              unk_050;
    /* 0x052 */ s16              unk_052;
    /* 0x054 */ s32              unk_054;
    /* 0x058 */ CombatSprite     sprite;
    /* 0x0B8 */ s16              radius; // collision radius, pixels
    /* 0x0BC */ s32              x;      // 20.12 fixed point
    /* 0x0C0 */ s32              y;
    /* 0x0C4 */ s32              velocityX;
    /* 0x0C8 */ s32              velocityY;
    /* 0x0CC */ Ov003EffectState effect;
    /* 0x108 */ CombatListNode   node;
} Ov003TouchActor; // Size: 0x114

typedef struct Ov003AnimSprite {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ s16          timer;
    /* 0x62 */ s16          unk_62;
    /* 0x64 */ s32          animTimer;
    /* 0x68 */ s32          decay;
    /* 0x6C */ u16          angle;
    /* 0x6E */ s16          unk_6E;
} Ov003AnimSprite; // Size: 0x70

// Shared with the battle manager's cursor state machine.
typedef struct BtlIcon {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ u16          animState;
    /* 0x62 */ s16          x;
    /* 0x64 */ s16          y;
    /* 0x66 */ u16          pad_66;
    /* 0x68 */ void (*update)(struct BtlIcon*);
    /* 0x6C */ s32 unk_6C;
} BtlIcon; // Size: 0x70

// The gesture prefix of the battle touch-input state at Ov003Global+0x3D370.
typedef struct BtlTouchInput {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u16 psychFlags;
    /* 0x06 */ u16 iconAnim;
    /* 0x08 */ s16 x;
    /* 0x0A */ s16 y;
    /* 0x0C */ s16 previousX;
    /* 0x0E */ s16 previousY;
    /* 0x10 */ s32 distance;
    /* 0x14 */ u16 angle;
    /* 0x16 */ u16 unk_16;
    /* 0x18 */ s16 tapCount;
    /* 0x1A */ u8  unk_1A[0x28 - 0x1A];
    /* 0x28 */ s16 gestureX;
    /* 0x2A */ s16 gestureY;
    /* 0x2C */ u8  unk_2C[0x34 - 0x2C];
} BtlTouchInput; // Size: 0x34

// Extended touch state shared by psych effects and actor selection.
typedef struct BtlTouchWork {
    /* 0x00 */ BtlTouchInput input;
    /* 0x34 */ s16           unk_34;
    /* 0x36 */ u8            pad_36[4];
    /* 0x3A */ s16           x;
    /* 0x3C */ s16           y;
    /* 0x3E */ s16           velocityX;
    /* 0x40 */ s16           velocityY;
    /* 0x42 */ u8            unk_42[6];
    /* 0x48 */ CombatActor*  candidateActor;
    /* 0x4C */ CombatActor*  heldActor;
    /* 0x50 */ CombatActor*  trackedActor;
    /* 0x54 */ s16           heldTime;
    /* 0x56 */ u8            unk_56[6];
} BtlTouchWork; // Size: 0x5C

void func_ov003_0208ecb4(BtlIcon* icon, void (*update)(BtlIcon*));
void func_ov003_0208ecbc(BtlIcon* icon);

typedef struct Ov003TrailEffect {
    /* 0x000 */ s32          state;
    /* 0x004 */ s32          count;
    /* 0x008 */ s16          points[0x78][2];
    /* 0x1E8 */ CombatSprite spriteA;
    /* 0x248 */ CombatSprite spriteB;
    /* 0x2A8 */ s32          scale; // 20.12 fixed point
    /* 0x2AC */ s16          pivotX;
    /* 0x2AE */ s16          pivotY;
} Ov003TrailEffect; // Size: 0x2B0

typedef struct Ov003PsychTrail {
    /* 0x000 */ Ov003EffectState* effect;
    /* 0x004 */ Ov003TrailEffect  trail;
} Ov003PsychTrail; // Size: 0x2B4

BtlTouchWork* func_ov003_020a62a0(void);
s32           func_ov003_020a5ac4(Ov003TouchActor**, s32 count, Ov003EffectState*, s32 requireReady);

#endif
