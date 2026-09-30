#ifndef COMBAT_TUTORIAL_BTLTUTORIAL_H
#define COMBAT_TUTORIAL_BTLTUTORIAL_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"

/// Task data for `Tsk_BtlPlayerNone`. Layout is a `CombatActor` plus a few
/// trailing fields.
typedef struct BtlPlayerNone {
    /* 0x00 */ CombatActor actor;
    /* 0x7C */ void (*unk_7C)(struct BtlPlayerNone*);
    /* 0x80 */ s16 unk_80;
    /* 0x82 */ s16 unk_82;
} BtlPlayerNone; // Size: 0x84

/// Task data for `Tsk_BtlTutorial`. Holds the allocated BG resources and a
/// sprite used to draw the tutorial overlay.
typedef struct BtlTutorial {
    /* 0x00 */ void*            unk_00;
    /* 0x04 */ void*            unk_04;
    /* 0x08 */ void*            unk_08;
    /* 0x0C */ void*            unk_0C;
    /* 0x10 */ PaletteResource* unk_10;
    /* 0x14 */ CombatSprite     sprite;
    /* 0x74 */ u8               unk_74[0x8];
} BtlTutorial; // Size: 0x7C

/// Per-variant asset description used by `Tsk_BtlTutorial`.
typedef struct BtlTutorialVariant {
    /* 0x00 */ u8  unk_00;
    /* 0x01 */ u8  unk_01;
    /* 0x02 */ u8  unk_02;
    /* 0x03 */ u8  unk_03;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
} BtlTutorialVariant; // Size: 0x8

extern const TaskHandle Tsk_BtlPlayerNone;
extern const TaskHandle Tsk_BtlTutorial;

#endif // COMBAT_TUTORIAL_BTLTUTORIAL_H
