#ifndef ENGINE_BGRESMGR_H
#define ENGINE_BGRESMGR_H

#include "DMA.h"
#include "Display.h"

enum {
    RESTYPE_CHAR_SM = 0,
    RESTYPE_CHAR_MD = 1,
    RESTYPE_CHAR_LG = 2,
};

typedef struct {
    /* 0x00 */ u8         unk_00;
    /* 0x04 */ DmaRequest request;
} UnkSmallStruct_DispBG;

typedef struct BgResource {
    /* 0x00 */ struct BgResource*     next;
    /* 0x04 */ u16                    flags;
    /* 0x06 */ u16                    refCount;
    /* 0x08 */ u16                    resourceType;
    /* 0x0A */ u16                    unk_0A;
    /* 0x0C */ s32                    unk_0C;
    /* 0x10 */ s32                    bitmapIndex; // Index in the VRAM allocation bitmap
    /* 0x14 */ u32                    vramOffset;  // Offset from vramBase to the resource's VRAM location
    /* 0x18 */ u32                    size;
    /* 0x1C */ UnkSmallStruct_DispBG* unk_1C;
} BgResource; // Size: 0x20

/**
 * @struct BgResMgr
 * @brief Background Resource Manager
 */
typedef struct {
    /* 0x0000 */ s32        engine;
    /* 0x0004 */ s32        unk_04;
    /* 0x0008 */ s32        vramBase; // Base address of VRAM allocated for backgrounds
    /* 0x000C */ s32        vramSize; // Total size in bytes of VRAM allocated for backgrounds
    /* 0x0010 */ s32        unk_10;
    /* 0x0014 */ char       unk_14[0xC];
    /* 0x0020 */ BgResource activeList; // Linked list of active allocated resources
    /* 0x0040 */ BgResource freeList;   // Linked list of available resource slots
    /* 0x0060 */ BgResource resourcePool[64];
    /* 0x0860 */ s32        unk_860;
    /* 0x0864 */ char       unk_864[0x1C80 - 0x864];
} BgResMgr; // Size: 0x1C80

extern BgResMgr* g_BgResourceManagers[2];

void BgResMgr_Init(BgResMgr* mgr, DisplayEngine engine);

BOOL BgResMgr_ReleaseChar(BgResMgr* mgr, BgResource* resource);

BOOL BgResMgr_ReleaseScreen(BgResMgr* mgr, BgResource* resource);

#endif // ENGINE_BGRESMGR_H