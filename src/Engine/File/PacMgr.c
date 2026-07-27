#include "Engine/File/PacMgr.h"
#include "Engine/Core/Memory.h"

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

void* PacMgr_LoadPackEntryData(Pack* pac, Bin* bin, u32* outSize, s32 entryIndex, BOOL compressed) {
    if (compressed == FALSE) {
        if (*outSize == 0) {
            *outSize = pac->entries[entryIndex].size;
        }

        return BinMgr_LoadRawData(bin, &pac->fileIden, pac->binIden, pac->entries[entryIndex].offset + 0x20, outSize);
    }

    return BinMgr_LoadCompressed(bin, &pac->fileIden, pac->binIden, pac->entries[entryIndex].offset + 0x20, outSize);
}

s32 PacMgr_GetPackEntryDataPtr(Pack* pac, s32 packIndex) {
    return (s32)(pac->loadedBin->data + (pac->entries[packIndex].offset + 0x20));
}

static inline PackEntry* PacMgr_GetPackEntryTable(u8* packBuffer) {
    return (PackEntry*)(packBuffer + PAC_PACK_HEADER_SIZE);
}

void* PacMgr_GenPack(Pack* pac, void* buffer, void* unused, s32* entryList) {
    s32 entryCount = entryList[0];
    u32 dataSize   = PAC_PACK_HEADER_SIZE;
    s32 i;

    for (i = 1; i <= entryCount; ++i) {
        PackEntry* entry = &pac->entries[entryList[i]];
        dataSize += ALIGN_TO_8(entry->size);
    }

    s32 tableCount = i;

    u32 entryTableSize = ALIGN_TO_8((u32)tableCount * sizeof(PackEntry));
    u32 orderTableSize;

    dataSize += entryTableSize;

    u8* packBuffer = buffer;

    u32 packEntryCount = (u32)tableCount;
    orderTableSize     = ALIGN_TO_8(packEntryCount * sizeof(s32));

    if (packBuffer == NULL) {
        packBuffer = Mem_AllocBestFit(&gMainHeap, dataSize + orderTableSize);
        Mem_SetSequence(&gMainHeap, packBuffer, "PacMgr_GenPack()");
    }

    FS_File file;
    FS_FileInit(&file);
    FS_FileOpenFromIden(&file, pac->fileIden);

    PackHeader* header           = (PackHeader*)packBuffer;
    PackEntry*  entryTable       = PacMgr_GetPackEntryTable(packBuffer);
    u32         orderTableOffset = PAC_PACK_HEADER_SIZE + entryTableSize;

    entryTable[0].offset = orderTableOffset;
    entryTable[0].size   = orderTableSize;

    MI_CpuCopyU32(entryList, packBuffer + orderTableOffset, orderTableSize);

    u32 writeOffset = orderTableOffset + entryTable[0].size;

    for (i = 1; i < tableCount; ++i) {
        if (entryList[i] <= 0) {
            s32 referencedIndex = (entryList[i] < 0) ? -entryList[i] : entryList[i];

            entryTable[i] = entryTable[referencedIndex];
        } else {
            entryTable[i].offset = writeOffset;
            entryTable[i].size   = pac->entries[entryList[i]].size;

            FS_FileSeek(&file, (s32)(pac->entries[entryList[i]].offset + PAC_PACK_HEADER_SIZE), 0);
            FS_FileRead(&file, packBuffer + writeOffset, (s32)entryTable[i].size);

            writeOffset += ALIGN_TO_8(entryTable[i].size);
        }
    }

    header->magic      = PAC_PACK_HEADER_MAGIC;
    header->entryCount = packEntryCount;
    header->dataSize   = writeOffset;

    FS_FileClose(&file);
    return packBuffer;
}
