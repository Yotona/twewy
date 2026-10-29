#ifndef COMBAT_CORE_BTLBAR_H
#define COMBAT_CORE_BTLBAR_H

#include "Combat/Core/CombatSprite.h"

/// Task data shared by the HP bar and the BS bar HUDs: ten sprites, then the widths, flags and
/// drain state the per-frame state machine publishes for the render stage. Each bar mirrors one
/// gauge pair of `Ov003Global`; the comments name the HP bar's reading of the fields.
typedef struct BtlBar {
    /* 0x000 */ CombatSprite sprites[10];
    /* 0x3C0 */ s16          unk_3C0; // player bar width in pixels (0..0xA2)
    /* 0x3C2 */ s16          unk_3C2; // partner bar width, mirrored (negative)
    /* 0x3C4 */ s16          unk_3C4;
    /* 0x3C6 */ s16          unk_3C6;
    /* 0x3C8 */ s16          unk_3C8;
    /* 0x3CA */ s16          unk_3CA;
    /* 0x3CC */ s16          unk_3CC;
    /* 0x3CE */ s16          unk_3CE;
    /* 0x3D0 */ s16          unk_3D0;
    /* 0x3D2 */ s16          unk_3D2;
    /* 0x3D4 */ s16          unk_3D4;
    /* 0x3D6 */ s16          unk_3D6;
    /* 0x3D8 */ u16          flags;   // bit set = draw the matching bar segment
    /* 0x3DA */ s16          unk_3DA; // player gauge value already sampled
    /* 0x3DC */ s16          unk_3DC; // partner gauge value already sampled
    /* 0x3DE */ s16          unk_3DE; // player drain timer
    /* 0x3E0 */ s16          unk_3E0; // partner drain timer
    /* 0x3E2 */ s16          unk_3E2;
    /* 0x3E4 */ s32          unk_3E4; // player interpolated width, 12.20 fixed point
    /* 0x3E8 */ s32          unk_3E8; // partner interpolated width, 12.20 fixed point
    /* 0x3EC */ s16          unk_3EC; // player width of the sampled gauge
    /* 0x3EE */ s16          unk_3EE; // partner width of the sampled gauge
    /* 0x3F0 */ s32          unk_3F0; // player drain state (0 none, 1 filling, 2 draining)
    /* 0x3F4 */ s32          unk_3F4; // partner drain state
    /* 0x3F8 */ s32          unk_3F8; // low-value warning blink / filling branch is running
} BtlBar;                             // Size: 0x3FC

#endif                                // COMBAT_CORE_BTLBAR_H
