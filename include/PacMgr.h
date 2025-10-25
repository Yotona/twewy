#ifndef PACMGR_H
#define PACMGR_H

#include "BinMgr.h"

typedef struct {
    /* 0x00 */ u32 magic;
    /* 0x04 */ u32 entryCount;
    /* 0x08 */ u32 dataSize;
    /* 0x0C */ u32 reserved[5];
} PackHeader; // Size: 0x20

typedef struct PackEntry {
    /* 0x00 */ u32 offset;
    /* 0x04 */ u32 size;
} PackEntry;

typedef struct Pack {
    /* 0x00 */ struct Pack*      next;
    /* 0x04 */ u16               refCount;
    /* 0x06 */ u16               entryCount;
    /* 0x08 */ Bin*              loadedBin;
    /* 0x0C */ BinIdentifier*    binIden;
    /* 0x10 */ FS_FileIdentifier fileIden;
    /* 0x18 */ PackEntry*        entries;
    /* 0x1C */ u32               unk_1C;
} Pack;

typedef struct {
    /* 0x00 */ Pack  rootPack;
    /* 0x20 */ Pack* loadedPacksHead;
    /* 0x24 */ char  unk_24[0x1C];
    /* 0x40 */ Pack* freeListTail;
    /* 0x44 */ s32   packPoolSize;
    /* 0x48 */ Pack* packPool; // Allocated pool of Pack structures
} PacMgr;

#endif // PACMGR_H