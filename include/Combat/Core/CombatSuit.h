#ifndef COMBAT_COMBATSUIT_H
#define COMBAT_COMBATSUIT_H

#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"

typedef struct CardEntry {
    /* 0x00 */ u16            animId;
    /* 0x02 */ s16            order; // stacking order used for the depth sort
    /* 0x04 */ CombatSprite   sprite;
    /* 0x64 */ u16            state;
    /* 0x66 */ s16            unk_66;
    /* 0x68 */ s32            x;
    /* 0x6C */ s32            y;
    /* 0x70 */ s32            offsetY;
    /* 0x74 */ s32            yVelocity;
    /* 0x78 */ s32            sortKey;
    /* 0x7C */ s16            timer;
    /* 0x7E */ s16            unk_7E;
    /* 0x80 */ CombatListNode node;
} CardEntry; // Size: 0x8C

typedef struct CardMgr {
    /* 0x000 */ CombatList active;
    /* 0x008 */ CombatList free;
    /* 0x010 */ u16        flags;
    /* 0x012 */ s16        order;
    /* 0x014 */ s8         count;
    /* 0x015 */ u8         pad_15[3];
    /* 0x018 */ s32        delay;
    /* 0x01C */ CardEntry  entries[16];
} CardMgr; // Size: 0x8DC

#endif
