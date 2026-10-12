#ifndef ENGINE_SCREENMAPPER_H
#define ENGINE_SCREENMAPPER_H

#include <nitro/types.h>

/** @brief A BG tilemap built from a grid of screen blocks that is bound to one engine's BG layer */
typedef struct ScreenMapEntry {
    /* 0x00 */ s32   engineId;
    /* 0x04 */ s32   bgLayer;
    /* 0x08 */ s32   flags; // bit 1: the map needs re-uploading
    /* 0x0C */ s32   hOffset;
    /* 0x10 */ s32   vOffset;
    /* 0x14 */ void* tilemap;
    /* 0x18 */ void* unk_18;
    /* 0x1C */ s32   width;
    /* 0x20 */ s32   height;
    /* 0x24 */ void* tileData;
} ScreenMapEntry; // Size: 0x28

s32  func_0200d1d8(ScreenMapEntry* entry, s32 engineId, s32 bgLayer, void* tilemap, void* tileData, s32 width, s32 height);
void func_0200d8f0(void);

#endif // ENGINE_SCREENMAPPER_H
