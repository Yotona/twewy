#include "PacMgr.h"
#include "Memory.h"

#define PAC_PACK_HEADER_MAGIC 0x6b636170 // 'pack'
#define PAC_PACK_HEADER_SIZE  0x20

#define ALIGN_TO_8(value) (((value) + 7U) & ~7U)

PacMgr* g_activePacMgr;

Pack* PacMgr_DequeueFreePack(Pack* pac) {
    Pack* pPVar1 = pac->next;

    if (pPVar1 != NULL) {
        pac->next = pPVar1->next;
    }
    return pPVar1;
}

void PacMgr_EnqueueFreePack(PacMgr* mgr, Pack* pac) {
    if (pac != NULL) {
        mgr->freeListTail->next = pac;
        mgr->freeListTail       = pac;
        pac->next               = NULL;
    }
}

void PacMgr_AddLoadedPack(PacMgr* pacMgr, Pack* pac) {
    if (pac != NULL) {
        pac->next               = pacMgr->loadedPacksHead;
        pacMgr->loadedPacksHead = pac;
    }
}

Pack* PacMgr_RemoveLoadedPack(PacMgr* binMgr, Pack* pacToRemove) {
    Pack** currentPac = &binMgr->loadedPacksHead;

    while (*currentPac != NULL) {
        if (*currentPac == pacToRemove) {
            Pack* ret   = *currentPac;
            *currentPac = ret->next;
            return ret;
        }
        currentPac = &(*currentPac)->next;
    }
    return *currentPac;
}

void PacMgr_ResetPack(PacMgr* mgr, Pack* pac) {
    if (pac != NULL) {
        pac->refCount   = 0;
        pac->entryCount = 0;
        pac->loadedBin  = NULL;
        pac->binIden    = NULL;
        pac->entries    = NULL;
    }
}

PacMgr* PacMgr_Init(PacMgr* pacMgr, u32 nodeCount) {
    PacMgr* prevMgr = g_activePacMgr;
    g_activePacMgr  = pacMgr;
    if (pacMgr == NULL) {
        return prevMgr;
    }

    pacMgr->loadedPacksHead = NULL;
    pacMgr->rootPack.next   = NULL;
    pacMgr->packPoolSize    = nodeCount;

    Pack* pac = Mem_AllocHeapTail(&gMainHeap, nodeCount * 32);
    Mem_SetSequence(&gMainHeap, pac, "PacMgr_Init()");
    pacMgr->packPool     = pac;
    pacMgr->freeListTail = pac;

    for (u16 i = 0; i < pacMgr->packPoolSize; i++) {
        PacMgr_ResetPack(pacMgr, pac);
        pac->next             = pacMgr->rootPack.next;
        pacMgr->rootPack.next = pac;
        pac++;
    }
    return prevMgr;
}

// Todo: clean up "data" when type is known
Pack* PacMgr_LoadPack(BinIdentifier* iden) {
    PacMgr* mgr = g_activePacMgr;
    if (mgr == NULL) {
        return NULL;
    }

    Pack* pac = mgr->loadedPacksHead;
    while (pac != NULL) {
        if (pac->binIden == iden) {
            break;
        }
        pac = pac->next;
    }

    if (pac != NULL) {
        if (pac->refCount != 0xFFFF) {
            pac->refCount++;
        }
        return pac;
    }

    Pack* newPac = PacMgr_DequeueFreePack(&mgr->rootPack);
    PacMgr_ResetPack(mgr, newPac);
    newPac->refCount = 1;
    newPac->binIden  = iden;
    FS_FilePathAsIden(&newPac->fileIden, iden->path);

    Bin* bin          = BinMgr_FindById((s32)iden);
    newPac->loadedBin = bin;
    if (bin != NULL) {
        bin               = BinMgr_LoadUncompressed(&newPac->fileIden, (u32)iden);
        newPac->loadedBin = bin;

        u8* data           = bin->data;
        newPac->entryCount = ((u32*)data)[1];
        newPac->entries    = (PackEntry*)(data + 0x20);
    } else {
        FS_File file;
        FS_FileInit(&file);
        FS_FileOpenFromIden(&file, newPac->fileIden);

        PackHeader header;

        FS_FileRead(&file, &header, sizeof(PackHeader));

        u32 remaining = header.entryCount << 3;

        char* seq    = iden->path;
        u8*   buffer = Mem_AllocBestFit(&gMainHeap, remaining);
        Mem_SetSequence(&gMainHeap, buffer, seq);

        newPac->entries    = (PackEntry*)buffer;
        newPac->entryCount = header.entryCount;

        u8* dest   = (u8*)newPac->entries;
        s32 length = 0x6800;
        while (remaining != 0) {
            if (remaining < length) {
                length = remaining;
            }
            if (FS_FileRead(&file, dest, length) == -1) {
                break;
            }
            dest += length;
            remaining -= length;
        }

        FS_FileClose(&file);
    }

    PacMgr_AddLoadedPack(mgr, newPac);
    return newPac;
}

BOOL PacMgr_ReleasePack(Pack* pac) {
    PacMgr* activeMgr = g_activePacMgr;

    BOOL allReleased = FALSE;

    if (activeMgr == NULL) {
        return allReleased;
    }

    pac->refCount--;
    if (pac->refCount == 0) {
        if (pac->loadedBin == NULL) {
            Mem_Free(&gMainHeap, pac->entries);
        } else {
            BinMgr_ReleaseBin(pac->loadedBin);
        }
        PacMgr_RemoveLoadedPack(activeMgr, pac);
        PacMgr_EnqueueFreePack(activeMgr, pac);
        PacMgr_ResetPack(activeMgr, pac);

        allReleased = TRUE;
    }

    return allReleased;
}

void PacMgr_LoadPackEntryData(Pack* pac, Bin* bin, u32* outSize, s32 entryIndex, s32 compressed) {
    if (compressed == 0) {
        if (*outSize == 0) {
            *outSize = pac->entries[entryIndex].size;
        }

        BinMgr_LoadRawData(bin, &pac->fileIden, pac->binIden, pac->entries[entryIndex].offset + 0x20, outSize);
        return;
    }

    BinMgr_LoadCompressed(bin, &pac->fileIden, pac->binIden, pac->entries[entryIndex].offset + 0x20, outSize);
}

s32 PacMgr_GetPackEntryDataPtr(Pack* pac, s32 arg1) {
    return (s32)(pac->loadedBin->data + (pac->entries[arg1].offset + 0x20));
}

// Nonmatching: Several alignment differences
// Scratch: 03pF3
void* PacMgr_GenPack(Pack* pac, void* buffer, void* unused, s32* entryList) {
    s32 entryCount = entryList[0];
    s32 tableCount = entryCount;
    u32 dataSize   = PAC_PACK_HEADER_SIZE;

    for (s32 i = 1; i <= entryCount; ++i) {
        PackEntry* entry = &pac->entries[entryList[i]];
        dataSize += ALIGN_TO_8(entry->size);
    }

    u32 entryTableSize = ALIGN_TO_8((u32)tableCount * sizeof(PackEntry));
    u32 orderTableSize = ALIGN_TO_8((u32)tableCount * sizeof(s32));

    u32 totalRequiredSize = dataSize + entryTableSize + orderTableSize;

    u8* packBuffer = buffer;
    if (packBuffer == NULL) {
        packBuffer = Mem_AllocBestFit(&gMainHeap, totalRequiredSize);
        Mem_SetSequence(&gMainHeap, packBuffer, "PacMgr_GenPack");
    }

    FS_File file;
    FS_FileInit(&file);
    FS_FileOpenFromIden(&file, pac->fileIden);

    PackHeader* header           = (PackHeader*)packBuffer;
    PackEntry*  entryTable       = (PackEntry*)(packBuffer + PAC_PACK_HEADER_SIZE);
    u32         orderTableOffset = PAC_PACK_HEADER_SIZE + entryTableSize;

    entryTable[0].offset = orderTableOffset;
    entryTable[0].size   = orderTableSize;

    func_0203b2a0(entryList, packBuffer + orderTableOffset, orderTableSize);

    u32 currentWriteOffset = orderTableOffset + entryTable[0].size;

    for (s32 i = 1; i < tableCount; ++i) {
        s32        entryIndex = entryList[i];
        PackEntry* dstEntry   = &entryTable[i];

        if (entryIndex <= 0) {
            s32        referencedIndex = (entryIndex < 0) ? -entryIndex : entryIndex;
            PackEntry* srcEntry        = &entryTable[referencedIndex];
            dstEntry->offset           = srcEntry->offset;
            dstEntry->size             = srcEntry->size;
        } else {
            PackEntry* srcEntry = &pac->entries[entryIndex];

            dstEntry->offset = currentWriteOffset;
            dstEntry->size   = srcEntry->size;

            FS_FileSeek(&file, (s32)(srcEntry->offset + PAC_PACK_HEADER_SIZE), 0);
            FS_FileRead(&file, packBuffer + currentWriteOffset, (s32)dstEntry->size);

            currentWriteOffset += ALIGN_TO_8(dstEntry->size);
        }
    }

    header->magic      = PAC_PACK_HEADER_MAGIC;
    header->entryCount = (u32)tableCount;
    header->dataSize   = currentWriteOffset;

    FS_FileClose(&file);
    return packBuffer;
}
