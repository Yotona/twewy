#ifndef COMBAT_CORE_COMBATAURA_H
#define COMBAT_CORE_COMBATAURA_H

#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatList.h"
#include "Combat/Core/CombatSprite.h"
#include "SpriteMgr.h"

typedef struct BtlAuraFlameSprite {
    /* 0x00 */ CombatSprite   sprite;
    /* 0x60 */ CombatListNode node;
} BtlAuraFlameSprite; // Size: 0x6C

typedef struct BtlEnmAuraState {
    /* 0x00 */ Ov003CutinActor* ownerMain; // actor whose height sets the main-screen depth
    /* 0x04 */ Ov003CutinActor* ownerSub;  // actor whose height sets the sub-screen depth
    /* 0x08 */ u16              mode;      // 1 = regular aura, 2 = charged aura
    /* 0x0A */ u8               unk_0A[0x2];
    /* 0x0C */ s32              activeMain;
    /* 0x10 */ s32              activeSub;
    /* 0x14 */ s16              ready; // aura may engage this frame
    /* 0x16 */ s16              unk_16;
    /* 0x18 */ s16              scale; // per-cent scale of the aura's value
} BtlEnmAuraState;                     // Size: 0x1C

extern BtlEnmAuraState data_ov003_020e7324;

/// The task data of the player-side (`Tsk_BtlAuraSub`) and enemy-side (`Tsk_BtlEnmAuraSub`) aura.

/// One of the sixteen flame sprites (0x70 bytes each); the helpers address it as a `Sprite` at +0.
typedef struct BtlAuraFlame {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ u8     unk_40[0x20];
    /* 0x60 */ s32    height;    // how far above the ground the flame sits
    /* 0x64 */ s32    riseSpeed; // vertical drift added every frame, 0 = static
    /* 0x68 */ s32    x;
    /* 0x6C */ s32    y;
} BtlAuraFlame; // Size: 0x70

/// The aura task data, `TaskHandle.dataSize` = 0x774.
typedef struct BtlAuraSub {
    /* 0x000 */ CombatSprite sprite; // the burst sprite, mostly unused here
    /* 0x060 */ BtlAuraFlame flames[16];
    /* 0x760 */ s32          x;
    /* 0x764 */ s32          y;
    /* 0x768 */ s32          sortKey;
    /* 0x76C */ s16          timer;  // descent timer; while it runs the burst below the screen plays
    /* 0x76E */ s16          unk_76E;
    /* 0x770 */ s32          active; // any flame still animating
} BtlAuraSub;                        // Size: 0x774

/// Parameters the spawner (`func_ov003_020974f8` / `func_ov003_020cac38`) hands to the new task.
typedef struct BtlAuraArgs {
    /* 0x00 */ s32 x;
    /* 0x04 */ s32 y;
} BtlAuraArgs;

#endif
