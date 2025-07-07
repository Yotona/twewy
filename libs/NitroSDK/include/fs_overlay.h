#ifndef FS_OVERLAY_H
#define FS_OVERLAY_H

#include <types.h>

typedef void (*OverlayFunc)(void);

typedef struct {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ u32   unk_04;
} UnkInfoChunk;

typedef struct {
    /* 0x00 */ char unk_00[0x24];
    /* 0x24 */ u32  unk_24;
    /* 0x28 */ u32  unk_28;
    /* 0x2C */ char unk_2C[0x1C];
} UnkOverlayBlock;

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
} UnkOverlayDetails;

typedef struct {
    /* 0x00 */ u32               unk_00;
    /* 0x04 */ void*             addr;
    /* 0x08 */ u32               sizeRam;
    /* 0x0C */ u32               sizeBss;
    /* 0x10 */ OverlayFunc*      initFunc;
    /* 0x14 */ OverlayFunc*      endFunc;
    /* 0x18 */ u32               unk_18;
    /* 0x1C */ u32               sizeCompressed : 24;
    /* 0x1C */ u32               flags          : 8;
    /* 0x20 */ u32               unk_20;
    /* 0x24 */ UnkOverlayDetails unk_24;
} OverlayInfo;

u32 FS_GetOverlayRamSize(OverlayInfo* info);

void FS_InvalidateOverlay(OverlayInfo* info);

BOOL FS_LoadOverlay(u32 param_1, u32 overlayID);

BOOL FS_UnloadOverlay(u32 param_1, u32 overlayID);

static inline FS_IsOverlayCompressed(OverlayInfo* info) {
    return (info->flags & 1) != 0;
}

#endif // FS_OVERLAY_H