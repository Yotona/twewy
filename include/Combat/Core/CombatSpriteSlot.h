#ifndef COMBAT_COMBATSPRITESLOT_H
#define COMBAT_COMBATSPRITESLOT_H

#include "Combat/Core/Combat.h"

typedef struct Ov003SpriteAnim {
    /* 0x00 */ u16 dataId;   // DatMgr pack entry holding this frame's cell/palette data
    /* 0x02 */ s16 entry;    // -1: end of animation, -2: palette switch marker
    /* 0x04 */ s16 duration; // frames to show, or the random range for randomised slots
} Ov003SpriteAnim;           // Size: 0x6

typedef struct Ov003SpriteSlot {
    /* 0x00 */ BinIdentifier*         charIden; // pack the cell data comes from
    /* 0x04 */ BinIdentifier*         palIden;  // pack the palette data comes from
    /* 0x08 */ Data*                  charData;
    /* 0x0C */ Data*                  paletteData;
    /* 0x10 */ PaletteResource*       palette;
    /* 0x14 */ void*                  charDest; // BG VRAM destination for the cell data
    /* 0x18 */ const Ov003SpriteAnim* anims;
    /* 0x1C */ s16                    dataId;   // entry currently loaded, -1 = none
    /* 0x1E */ s16                    animIndex;
    /* 0x20 */ s16                    timer;
    union {
        /* 0x22 */ struct {
            u16 palEngine : 2; // display engine the palette is allocated on
            u16 palStart  : 4; // first palette slot
            u16 palCount  : 4; // slots to allocate
            u16 _pad22    : 6;
        };
        u16 palFlags;
    };
    union {
        /* 0x24 */ struct {
            u16 loop     : 1; // restart at the end of the animation
            u16 oneShot  : 1; // animation has ended and must not restart
            u16 hasPal   : 1; // `palette` is allocated and must be reloaded/released
            u16 finished : 1; // animation reached its end marker
            u16 _pad24   : 12;
        };
        u16 flags;
    };
} Ov003SpriteSlot; // Size: 0x28

#endif
