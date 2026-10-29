#ifndef COMBAT_COMBATBGTILES_H
#define COMBAT_COMBATBGTILES_H

#include "nitro/types.h"

typedef struct CombatBgTiles {
    /* 0x0 */ union {
        u32 header;
        struct {
            u8 format;
            u8 size[3];
        };
    };
    /* 0x4 */ u8 pixels[];
} CombatBgTiles;

#endif
