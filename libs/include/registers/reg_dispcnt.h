#ifndef REG_DISPCNT_H
#define REG_DISPCNT_H

#include <types.h>

/// MARK: Main Engine

typedef volatile struct {
    u32 bgMode          : 3;
    u32 dimension       : 1;
    u32 objMapTile      : 1;
    u32 objMapBmp       : 2;
    u32 forceBlank      : 1;
    u32 scrDispBG0      : 1;
    u32 scrDispBG1      : 1;
    u32 scrDispBG2      : 1;
    u32 scrDispBG3      : 1;
    u32 scrDispOBJ      : 1;
    u32 scrDispWin0     : 1;
    u32 scrDispWin1     : 1;
    u32 scrDispWinOBJ   : 1;
    u32 dispMode        : 2;
    u32 vramBlock       : 2;
    u32 tileObjBoundary : 2;
    u32 bmpObjBoundary  : 1;
    u32 hblankProcess   : 1; // Whether to allow OBJ processing during H-Blank
    u32 charBase        : 3;
    u32 screenBase      : 3;
    u32 bgExtPltt       : 1; // Whether BG extended palette is enabled
    u32 objExtPltt      : 1; // Whether OBJ extended palette is enabled
} DispCnt;

#define REG_DISPCNT (*(vu32*)0x04000000)

static inline DispCnt* REG_DISPCNT_GET(void) {
    return (DispCnt*)&REG_DISPCNT;
}

/// MARK: Sub Engine

typedef volatile struct {
    u32 bgMode          : 3;
    u32 _unused1        : 1;
    u32 objMapTile      : 1;
    u32 objMapBmp       : 2;
    u32 forceBlank      : 1;
    u32 scrDispBG0      : 1;
    u32 scrDispBG1      : 1;
    u32 scrDispBG2      : 1;
    u32 scrDispBG3      : 1;
    u32 scrDispOBJ      : 1;
    u32 scrDispWin0     : 1;
    u32 scrDispWin1     : 1;
    u32 scrDispWinOBJ   : 1;
    u32 dispMode        : 1;
    u32 _unused2        : 3;
    u32 tileObjBoundary : 2;
    u32 _unused3        : 1;
    u32 hblankProcess   : 1; // Whether to allow OBJ processing during H-Blank
    u32 _unused4        : 6;
    u32 bgExtPltt       : 1; // Whether BG extended palette is enabled
    u32 objExtPltt      : 1; // Whether OBJ extended palette is enabled
} DispCntSub;

#define REG_DISPCNT_SUB (*(vu32*)0x04001000)

static inline DispCntSub* REG_DISPCNT_SUB_GET(void) {
    return (DispCntSub*)&REG_DISPCNT_SUB;
}

#endif // REG_DISPCNT_H