#ifndef COMBAT_CORE_COMBATENEMYSHOT_H
#define COMBAT_CORE_COMBATENEMYSHOT_H

#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatCutin.h"

typedef struct Ov003EnemyShotVisual {
    /* 0x00 */ const BinIdentifier*   file;
    /* 0x04 */ const SpriteAnimEntry* entries;
    /* 0x08 */ u16                    entry;
    /* 0x0A */ u16                    paletteSlot;
} Ov003EnemyShotVisual; // Size: 0xC

typedef struct Ov003EnemyShotTableVisual {
    /* 0x00 */ Ov003EnemyShotVisual base;
    /* 0x0C */ u16                  charFormat;
    /* 0x0E */ u16                  reserved;
} Ov003EnemyShotTableVisual; // Size: 0x10

typedef struct Ov003EnemyShotCore {
    /* 0x00 */ Ov003CutinActor* owner;
    /* 0x04 */ CombatSprite*    sprite;
    /* 0x08 */ s8               hitResult;
    /* 0x09 */ u8               reserved09;
    /* 0x0A */ u16              attackId;
    /* 0x0C */ Vec              position;
    /* 0x18 */ s16              timer;
    /* 0x1A */ u16              flags;
} Ov003EnemyShotCore; // Size: 0x1C

typedef struct Ov003EnemyShotSource {
    /* 0x00 */ Ov003EnemyShotCore core;
    /* 0x1C */ u16                kind;
    /* 0x1E */ u16                animation;
    /* 0x20 */ u16                animationMode;
    /* 0x22 */ u16                reserved;
    /* 0x24 */ union {
        const Ov003EnemyShotVisual*      visual;
        const Ov003EnemyShotTableVisual* tableVisual;
    };
    /* 0x28 */ SndMgrSeIdx sound;
} Ov003EnemyShotSource; // Size: 0x2C

typedef struct Ov003EnemyShotBlurArgs {
    Ov003EnemyShotSource* source;
    s32                   opacity;
    s32                   decay;
} Ov003EnemyShotBlurArgs; // Size: 0xC

typedef struct Ov003EnemyShotBlur {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          position;
    /* 0x6C */ s32          opacity;
    /* 0x70 */ s32          decay;
} Ov003EnemyShotBlur; // Size: 0x74

typedef struct Ov003EnemyShotRootArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ CombatList*           list;
    /* 0x08 */ u16                   angle;
    /* 0x0A */ u16                   reserved;
    /* 0x0C */ Vec                   position;
    /* 0x18 */ s32                   scaleX;
    /* 0x1C */ s32                   scaleY;
} Ov003EnemyShotRootArgs; // Size: 0x20

typedef struct Ov003EnemyShotRoot {
    /* 0x00 */ CombatSprite   sprite;
    /* 0x60 */ Vec            position;
    /* 0x6C */ s32            active;
    /* 0x70 */ CombatListNode node;
    /* 0x7C */ s32            scaleX;
    /* 0x80 */ s32            scaleY;
    /* 0x84 */ u16            angle;
    /* 0x86 */ u16            reserved;
} Ov003EnemyShotRoot; // Size: 0x88

// A source snapshot and the parameters used to emit a fan of shots.
typedef struct Ov003EnemyShotFan {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ s32                  speed;
    /* 0x30 */ u16                  angle;
    /* 0x32 */ u16                  spread;
    /* 0x34 */ u16                  count;
    /* 0x36 */ s16                  delay;
} Ov003EnemyShotFan; // Size: 0x38

typedef struct Ov003EnemyShotRapidSlowArgs {
    /* 0x00 */ Ov003EnemyShotSource* finalSource;
    /* 0x04 */ s32                   finalSpeed;
    /* 0x08 */ u16                   finalAngle;
    /* 0x0A */ u16                   finalSpread;
    /* 0x0C */ u16                   finalCount;
    /* 0x0E */ u16                   reserved;
    /* 0x10 */ Ov003EnemyShotSource* initialSource;
    /* 0x14 */ s32                   initialSpeed;
    /* 0x18 */ u16                   initialAngle;
    /* 0x1A */ u16                   initialSpread;
    /* 0x1C */ u16                   initialCount;
    /* 0x1E */ s16                   delay;
} Ov003EnemyShotRapidSlowArgs; // Size: 0x20

typedef struct Ov003EnemyShotRapidSlow {
    /* 0x00 */ Ov003EnemyShotFan finalShot;
    /* 0x38 */ Ov003EnemyShotFan initialShot;
    /* 0x70 */ s16               timer;
    /* 0x72 */ u16               reserved;
} Ov003EnemyShotRapidSlow; // Size: 0x74

typedef struct Ov003EnemyShotSplitChildArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ Vec                   velocity;
    /* 0x10 */ s32                   damping;
} Ov003EnemyShotSplitChildArgs; // Size: 0x14

typedef struct Ov003EnemyShotSplitChild {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ Vec                  velocity;
    /* 0x98 */ Vec                  dampingStep;
} Ov003EnemyShotSplitChild; // Size: 0xA4

typedef struct Ov003EnemyShotSplitPattern {
    /* 0x00 */ s16 delay;
    /* 0x02 */ u16 count;
    /* 0x04 */ s32 speed;
    /* 0x08 */ s32 velocityZ;
    /* 0x0C */ s32 damping;
} Ov003EnemyShotSplitPattern; // Size: 0x10

typedef struct Ov003EnemyShotSplitArgs {
    /* 0x00 */ Ov003EnemyShotSource*      source;
    /* 0x04 */ Ov003EnemyShotSource*      childSource;
    /* 0x08 */ s16                        travelFrames;
    /* 0x0A */ u16                        reserved;
    /* 0x0C */ Vec                        target;
    /* 0x18 */ Ov003EnemyShotSplitPattern pattern;
    /* 0x28 */ s32                        yScale;
} Ov003EnemyShotSplitArgs; // Size: 0x2C

typedef struct Ov003EnemyShotSplit {
    /* 0x00 */ Ov003EnemyShotSource       source;
    /* 0x2C */ Ov003EnemyShotSource       childSource;
    /* 0x58 */ CombatSprite               sprite;
    /* 0xB8 */ s16                        travelFrames;
    /* 0xBA */ u16                        reserved;
    /* 0xBC */ Vec                        target;
    /* 0xC8 */ Ov003EnemyShotSplitPattern pattern;
    /* 0xD8 */ s32                        yScale;
} Ov003EnemyShotSplit; // Size: 0xDC

typedef struct Ov003EnemyShotWaveArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   angle;
    /* 0x06 */ u16                   reserved;
    /* 0x08 */ s32                   speed;
    /* 0x0C */ s32                   amplitude;
    /* 0x10 */ u16                   phaseStep;
    /* 0x12 */ u16                   reserved12;
} Ov003EnemyShotWaveArgs; // Size: 0x14

typedef struct Ov003EnemyShotWave {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ u16                  angle;
    /* 0x8E */ u16                  reserved;
    /* 0x90 */ s32                  velocityX;
    /* 0x94 */ s32                  velocityY;
    /* 0x98 */ s32                  amplitude;
    /* 0x9C */ u16                  phaseStep;
    /* 0x9E */ u16                  phase;
    /* 0xA0 */ s32                  offsetX;
    /* 0xA4 */ s32                  offsetY;
} Ov003EnemyShotWave; // Size: 0xA8

typedef struct Ov003EnemyShotSingleArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ Vec                   velocity;
    /* 0x10 */ s32                   screenHit;
    /* 0x14 */ s32                   rotate;
    /* 0x18 */ u16                   angle;
    /* 0x1A */ u16                   reserved;
} Ov003EnemyShotSingleArgs; // Size: 0x1C

typedef struct Ov003EnemyShotSingle {
    /* 0x00 */ Ov003EnemyShotCore source;
    /* 0x1C */ CombatSprite       sprite;
    /* 0x7C */ Vec                velocity;
    /* 0x88 */ u8                 flags;
    /* 0x89 */ u8                 reserved;
    /* 0x8A */ u16                angle;
} Ov003EnemyShotSingle; // Size: 0x8C

typedef struct Ov003EnemyShotEffectArgs {
    /* 0x00 */ CombatSprite* source;
    /* 0x04 */ Vec           position;
    /* 0x10 */ s32           flip;
    /* 0x14 */ s32           kind;
} Ov003EnemyShotEffectArgs; // Size: 0x18

typedef struct Ov003EnemyShotEffect {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ Vec          position;
    /* 0x6C */ u16          kind;
    /* 0x6E */ u16          reserved;
} Ov003EnemyShotEffect; // Size: 0x70

typedef struct Ov003EnemyShotEffectCounts {
    /* 0x00 */ u32 reserved00;
    /* 0x04 */ s32 activeParticles;
    /* 0x08 */ u8  reserved08[0x18];
} Ov003EnemyShotEffectCounts; // Size: 0x20

typedef struct Ov003EnemyShotChargeSplitArgs {
    /* 0x00 */ Ov003EnemyShotSource*      source;
    /* 0x04 */ Ov003EnemyShotSource*      childSource;
    /* 0x08 */ s16                        travelFrames;
    /* 0x0A */ u16                        reserved;
    /* 0x0C */ Vec                        target;
    /* 0x18 */ Ov003EnemyShotSplitPattern patterns[2];
    /* 0x38 */ s32                        yScale;
} Ov003EnemyShotChargeSplitArgs; // Size: 0x3C

typedef struct Ov003EnemyShotChargeArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ Ov003EnemyShotSource* secondSource;
    /* 0x08 */ Vec                   velocity;
} Ov003EnemyShotChargeArgs; // Size: 0x14

typedef struct Ov003EnemyShotCharge {
    /* 0x000 */ Ov003EnemyShotSource  source;
    /* 0x02C */ Ov003EnemyShotSource  secondSource;
    /* 0x058 */ Ov003EnemyShotSource* activeSource;
    /* 0x05C */ CombatSprite*         activeSprite;
    /* 0x060 */ CombatSprite          sprites[2];
    /* 0x120 */ Vec                   velocity;
    /* 0x12C */ u16                   reserved;
    /* 0x12E */ s16                   phase;
} Ov003EnemyShotCharge; // Size: 0x130

typedef struct Ov003EnemyShotSpiralArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   angle;
    /* 0x06 */ u16                   reserved06;
    /* 0x08 */ s32                   speed;
    /* 0x0C */ s32                   acceleration;
    /* 0x10 */ u16                   angleStep;
    /* 0x12 */ u16                   reserved12;
    /* 0x14 */ s32                   yScale;
} Ov003EnemyShotSpiralArgs; // Size: 0x18

typedef struct Ov003EnemyShotSpiral {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ u16                  angle;
    /* 0x8E */ u16                  angleStep;
    /* 0x90 */ s32                  speed;
    /* 0x94 */ s32                  acceleration;
    /* 0x98 */ s32                  yScale;
    /* 0x9C */ s32                  velocityX;
    /* 0xA0 */ s32                  velocityY;
} Ov003EnemyShotSpiral; // Size: 0xA4

typedef struct Ov003EnemyShotHomingArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   angle;
    /* 0x06 */ u16                   reserved;
    /* 0x08 */ s32                   initialSpeed;
    /* 0x0C */ s32                   targetSpeed;
    /* 0x10 */ u16                   turnFrames;
    /* 0x12 */ s16                   lifetime;
} Ov003EnemyShotHomingArgs; // Size: 0x14

typedef struct Ov003EnemyShotHoming {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ s32                  targetSpeed;
    /* 0x90 */ u16                  turnFrames;
    /* 0x92 */ u16                  reserved;
    /* 0x94 */ s32                  velocityX;
    /* 0x98 */ s32                  velocityY;
} Ov003EnemyShotHoming; // Size: 0x9C

typedef struct Ov003EnemyShotMachineGunArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ s32                   speed;
    /* 0x08 */ u16                   count;
    /* 0x0A */ u16                   interval;
    /* 0x0C */ s32                   fixedAim;
    /* 0x10 */ s32                   rotate;
    /* 0x14 */ u16                   angle;
    /* 0x16 */ u16                   reserved;
    /* 0x18 */ s32                   stopOnCrossing;
} Ov003EnemyShotMachineGunArgs; // Size: 0x1C

typedef struct Ov003EnemyShotMachineGun {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ s32                  speed;
    /* 0x30 */ s16                  remaining;
    /* 0x32 */ s16                  lastTask;
    /* 0x34 */ s16                  interval;
    /* 0x36 */ s16                  timer;
    /* 0x38 */ Vec                  velocity;
    /* 0x44 */ u32                  taskIds[10];
    /* 0x6C */ s16                  hitCount;
    /* 0x6E */ u16                  reserved6E;
    /* 0x70 */ s32                  fixedAim;
    /* 0x74 */ s32                  rotate;
    /* 0x78 */ u16                  angle;
    /* 0x7A */ u16                  reserved7A;
    /* 0x7C */ s32                  crossing;
} Ov003EnemyShotMachineGun; // Size: 0x80

typedef struct Ov003EnemyShotOptionArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ u16                   angleStep;
    /* 0x06 */ s16                   lifetime;
    /* 0x08 */ s16                   radius;
    /* 0x0A */ s16                   heightRange;
    /* 0x0C */ s16                   minHeight;
    /* 0x0E */ u16                   reserved;
} Ov003EnemyShotOptionArgs; // Size: 0x10

typedef struct Ov003EnemyShotOption {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ CombatSprite         sprite;
    /* 0x8C */ s32                  zVelocity;
    /* 0x90 */ s32                  zOffset;
    /* 0x94 */ u16                  angle;
    /* 0x96 */ u16                  angleStep;
    /* 0x98 */ s16                  periodTimer;
    /* 0x9A */ s16                  radius;
    /* 0x9C */ s16                  heightRange;
    /* 0x9E */ s16                  minHeight;
} Ov003EnemyShotOption; // Size: 0xA0

typedef struct Ov003EnemyShotWindMillArgs {
    /* 0x00 */ Ov003EnemyShotSource* source;
    /* 0x04 */ s32                   speed;
    /* 0x08 */ u16                   angle;
    /* 0x0A */ u16                   angleStep;
    /* 0x0C */ u16                   count;
    /* 0x0E */ u16                   interval;
    /* 0x10 */ s16                   bursts;
    /* 0x12 */ u16                   reserved;
    /* 0x14 */ s32                   yScale;
} Ov003EnemyShotWindMillArgs; // Size: 0x18

typedef struct Ov003EnemyShotWindMill {
    /* 0x00 */ Ov003EnemyShotSource source;
    /* 0x2C */ s32                  speed;
    /* 0x30 */ u16                  angle;
    /* 0x32 */ u16                  angleStep;
    /* 0x34 */ u16                  count;
    /* 0x36 */ u16                  interval;
    /* 0x38 */ s16                  bursts;
    /* 0x3A */ s16                  timer;
    /* 0x3C */ s32                  yScale;
} Ov003EnemyShotWindMill; // Size: 0x40

s32 func_ov003_020d33f0(Ov003EnemyShotSource*, s32, u16, u16, u16, u16, u16, s32);

s32 func_ov003_020cd6f4(Ov003EnemyShotSource* source, s32 opacity, s32 decay);
s32 func_ov003_020d1228(Ov003EnemyShotSource* source, CombatList* list, u16 angle, s32 x, s32 y, s32 z, s32 scaleX,
                        s32 scaleY);

s32 func_ov003_020d0518(Ov003EnemyShotSource*, s32, u16, u16, u16, Ov003EnemyShotSource*, s32, u16, u16, u16, s16);

s32 func_ov003_020d281c(Ov003EnemyShotSource*, s32, s32, s32, s32);

s32 func_ov003_020d2bc0(Ov003EnemyShotSplitArgs*);

s32 func_ov003_020d2fb0(Ov003EnemyShotSource*, u16, s32, s32, u16);

s32 func_ov003_020d1eec(Ov003EnemyShotSource*, s32, s32, s32, s32, s32, u16);

s32 func_ov003_020cde74(CombatSprite*, s32, s32, s32, s32, s32);
s32 func_ov003_020cdfbc(Ov003EnemyShotSource*, s32, s32);
s32 func_ov003_020cdff4(Ov003EnemyShotCore*, s32, s32);

s32 func_ov003_020cd988(Ov003EnemyShotChargeSplitArgs*);
s32 func_ov003_020cda50(Ov003EnemyShotSource*, Ov003EnemyShotSource*, s32, s32, s32);

s32 func_ov003_020d23e8(Ov003EnemyShotSource*, u16, s32, s32, u16, s32);

s32 func_ov003_020cf34c(Ov003EnemyShotSource*, u16, s32, s32, u16, s16);

s32 func_ov003_020cf81c(Ov003EnemyShotSource*, s32, u16, u16, s32, s32, u16, s32);

s32 func_ov003_020cfedc(Ov003EnemyShotSource*, u16, s16, s16, s16, s16);

void                func_ov003_020d07a8(Ov003EnemyShotSource*, CombatList*, Ov003EnemyShotRoot*, u16, s16, s16, s32, s16);
void                func_ov003_020d117c(CombatList*);
Ov003EnemyShotRoot* func_ov003_020d11b8(Ov003EnemyShotRoot*, u16*, s32*, s32*, s32*, s32*, s32*);
void*               func_ov003_0208495c(CombatList*);
void*               func_ov003_02084984(CombatListNode*);
void                func_ov003_020849ac(CombatList*);
void                func_ov003_020849bc(CombatList*, CombatListNode*, void*);

void func_ov003_020cc8b0(Ov003EnemyShotCore*, const Ov003EnemyShotSource*);
s32  func_ov003_020ccb9c(Ov003EnemyShotSource*, CombatSprite*);
s32  func_ov003_020ccc00(Ov003EnemyShotCore*, CombatSprite*);
s32  func_ov003_020ccca4(s32, u32);
void func_ov003_020cccf0(s32, Ov003EnemyShotSource*);
s32  func_ov003_020cce34(s32);
void func_ov003_020d1dc0(Ov003EnemyShotSource*, s32, u16, u16, u16);
s32  func_ov003_020d1ec4(Ov003EnemyShotSource*, s32, s32, s32, s32);

typedef s32 (*BtlActorFilter)(s32 count, Ov003EnemyActor* actor, void* arg);

typedef struct BtlBinEntry {
    /* 0x0 */ u32 id;
    /* 0x4 */ s32 offset; // where the entry starts inside the shared buffer
} BtlBinEntry;            // Size: 0x8

/// A set of packed resources that live in one heap buffer.
typedef struct BtlBinSet {
    /* 0x00 */ void*              buffer;
    /* 0x04 */ s32                size;
    /* 0x08 */ const BtlBinEntry* entries;
    /* 0x0C */ u16                count;
    /* 0x0E */ u16                reserved0E;
    /* 0x10 */ const char*        tag; // name the buffer is registered under
} BtlBinSet;                           // Size: 0x14

typedef struct BtlVec2 {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} BtlVec2;

#endif
