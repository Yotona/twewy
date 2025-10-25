#include "BinMgr.h"
#include "Memory.h"

BinMgr* g_activeBinMgr; // Currently active binary manager instance

Bin* BinMgr_AllocNode(Bin* freeListHead) {
    Bin* allocatedNode = freeListHead->next;
    if (allocatedNode != NULL) {
        freeListHead->next = allocatedNode->next;
    }
    return allocatedNode;
}

void BinMgr_AppendNode(BinMgr* binMgr, Bin* nodeToAppend) {
    if (nodeToAppend != NULL) {
        binMgr->listTail->next = nodeToAppend;
        binMgr->listTail       = nodeToAppend;
        nodeToAppend->next     = NULL;
    }
}

void BinMgr_AddToFreeList(BinMgr* binMgr, Bin* binToAdd) {
    if (binToAdd != NULL) {
        binToAdd->next   = binMgr->freeList;
        binMgr->freeList = binToAdd;
    }
}

Bin* BinMgr_RemoveFromFreeList(BinMgr* binMgr, Bin* binToRemove) {
    Bin** currentBin = &binMgr->freeList;

    while (*currentBin != NULL) {
        if (*currentBin == binToRemove) {
            Bin* ret    = *currentBin;
            *currentBin = ret->next;
            return ret;
        }
        currentBin = &(*currentBin)->next;
    }
    return *currentBin;
}

BinMgr* BinMgr_Init(BinMgr* binMgr, u32 nodeCount) {
    BinMgr* prevMgr = g_activeBinMgr;
    g_activeBinMgr  = binMgr;
    if (binMgr == NULL) {
        return prevMgr;
    }

    binMgr->freeList     = NULL;
    binMgr->rootBin.next = NULL;
    binMgr->nodeCount    = nodeCount;

    Bin* bin = Mem_AllocHeapTail(&gMainHeap, nodeCount * 32);
    Mem_SetSequence(&gMainHeap, bin, "BinMgr_Init()");
    binMgr->nodePool = bin;
    binMgr->listTail = bin;

    for (u16 i = 0; i < binMgr->nodeCount; i++) {
        if (bin != NULL) {
            func_0203b3c0(bin, 0, sizeof(Bin));
        }
        bin->next            = binMgr->rootBin.next;
        binMgr->rootBin.next = bin;
        bin++;
    }
    return prevMgr;
}

void* BinMgr_LoadRawData(Bin* targetBuffer, FS_FileIdentifier* resIden, BinIdentifier* binIden, int offset, u32* outSize) {
    if (resIden == NULL) {
        FS_FileIdentifier localResourceDesc;

        FS_FilePathAsIden(&localResourceDesc, binIden->path);
        resIden = &localResourceDesc;
    }

    FS_File file;

    FS_FileInit(&file);
    FS_FileOpenFromIden(&file, *resIden);

    u32 totalBytesToRead = (file.endPosition - file.startPosition) - offset;
    u32 maxChunkSize     = *outSize;

    if ((maxChunkSize != 0) && (totalBytesToRead > maxChunkSize)) {
        totalBytesToRead = maxChunkSize;
    }

    if (targetBuffer == NULL) {
        char* sequence = binIden->path;
        targetBuffer   = Mem_AllocBestFit(&gMainHeap, totalBytesToRead);
        Mem_SetSequence(&gMainHeap, targetBuffer, sequence);
    }

    *outSize = totalBytesToRead;
    FS_FileSeek(&file, offset, 0);

    void* currentReadBuffer = targetBuffer;
    maxChunkSize            = 0x6800; // Max chunk size for reading (26KB)

    while (totalBytesToRead != 0) {
        if (totalBytesToRead < maxChunkSize) {
            maxChunkSize = totalBytesToRead;
        }
        if (FS_FileRead(&file, currentReadBuffer, maxChunkSize) == -1) {
            break;
        }
        currentReadBuffer = ((char*)currentReadBuffer + maxChunkSize);
        totalBytesToRead -= maxChunkSize;
    };

    FS_FileClose(&file);
    return targetBuffer;
}

/* Nonmatching */
Bin* BinMgr_LoadCompressed(Bin* targetBuffer, FS_FileIdentifier* resIden, int resourceId, int offset, u32* outSize) {
    void* currentReadBuffer;
    u32   totalCompressedSize;
    u32   maxChunkSize;

    if (resIden == NULL) {
        FS_FileIdentifier localResourceDesc;

        FS_FilePathAsIden(&localResourceDesc, *(char**)(resourceId + 4));
        resIden = &localResourceDesc;
    }

    FS_File file;

    FS_FileInit(&file);
    FS_FileOpenFromIden(&file, *resIden);

    totalCompressedSize = (file.endPosition - file.startPosition) - offset;

    s32* compressedDataBuffer = Mem_AllocHeapHead(&gMainHeap, totalCompressedSize);
    Mem_SetSequence(&gMainHeap, compressedDataBuffer, "BinMgr_LoadComp()");

    FS_FileSeek(&file, offset, 0);

    maxChunkSize      = 0x6800; // Max chunk size for reading (26KB)
    currentReadBuffer = compressedDataBuffer;

    while (totalCompressedSize != 0) {
        if (totalCompressedSize < maxChunkSize) {
            maxChunkSize = totalCompressedSize;
        }
        if (FS_FileRead(&file, currentReadBuffer, maxChunkSize) == -1) {
            break;
        }

        currentReadBuffer   = ((char*)currentReadBuffer + maxChunkSize);
        totalCompressedSize = totalCompressedSize - maxChunkSize;
    };

    FS_FileClose(&file);

    // Extract decompressed size from compression header
    u32 decompressedSize = (u32)(*compressedDataBuffer & ~0xFF) >> 8;

    if ((*(u8*)compressedDataBuffer & 0xf0) == 0) {
        decompressedSize = decompressedSize - 4;
    }
    if (*outSize != 0) {
        decompressedSize = *outSize;
    }
    *outSize = decompressedSize;

    void* outputBuffer = targetBuffer;
    if (targetBuffer == NULL) {
        char* sequence = *(char**)(resourceId + 4);
        outputBuffer   = Mem_AllocBestFit(&gMainHeap, decompressedSize);
        Mem_SetSequence(&gMainHeap, outputBuffer, sequence);
    }
    func_02004d60(outputBuffer, compressedDataBuffer);
    Mem_Free(&gMainHeap, compressedDataBuffer);
    return outputBuffer;
}

Bin* BinMgr_LoadUncompressed(FS_FileIdentifier* resIden, u32 resourceId) {
    BinMgr* binMgr = g_activeBinMgr;
    Bin*    bin    = BinMgr_FindById(resourceId);
    if (bin != NULL) {
        bin->refCount++;
        return bin;
    }

    bin = BinMgr_AllocNode(&binMgr->rootBin);
    if (bin != NULL) {
        func_0203b3c0(bin, 0, 0x20);
    }
    // Set flag bits for BinMgr-managed uncompressed data
    bin->flags = bin->flags & ~0x1 | 1; // Set bit 0 (allocated by BinMgr)
    bin->flags &= ~0x2;                 // Clear bit 1 (no secondary data)
    bin->flags &= ~0x4;                 // Clear bit 2 (not compressed)
    bin->refCount      = 1;
    bin->id            = resourceId;
    bin->size          = 0;
    bin->data          = BinMgr_LoadRawData(NULL, resIden, resourceId, 0, &bin->size);
    bin->secondaryData = 0;
    BinMgr_AddToFreeList(binMgr, bin);
    return bin;
}

Bin* BinMgr_LoadResource(FS_FileIdentifier* resIden, u32 resourceId) {
    BinMgr* binMgr;
    Bin*    existingNode;
    Bin*    newNode;

    binMgr       = g_activeBinMgr;
    existingNode = BinMgr_FindById(resourceId);
    if (existingNode != NULL) {
        existingNode->refCount++;
        return existingNode;
    }
    newNode = BinMgr_AllocNode(&binMgr->rootBin);
    if (newNode != NULL) {
        func_0203b3c0(newNode, 0, 0x20);
    }
    // Set flag bits for BinMgr-managed compressed data
    newNode->flags = newNode->flags & ~0x1 | 1; // Set bit 0 (allocated by BinMgr)
    newNode->flags &= ~0x2;                     // Clear bit 1 (no secondary data)
    newNode->flags |= 4;                        // Set bit 2 (compressed data)
    newNode->refCount      = 1;
    newNode->id            = resourceId;
    newNode->size          = 0;
    newNode->data          = BinMgr_LoadCompressed(NULL, resIden, resourceId, 0, &newNode->size);
    newNode->secondaryData = 0;
    BinMgr_AddToFreeList(binMgr, newNode);
    return newNode;
}

Bin* BinMgr_CreateFromData(u32 id, void* data, u32 size) {
    BinMgr* binMgr;
    Bin*    bin;

    binMgr = g_activeBinMgr;
    bin    = BinMgr_FindById(id);
    if (bin != NULL) {
        if (bin->refCount != 0xFFFF) {
            bin->refCount++;
        }
        return bin;
    }

    bin = BinMgr_AllocNode(&binMgr->rootBin);
    if (bin != NULL) {
        func_0203b3c0(bin, 0, sizeof(Bin));
    }
    // Clear all flag bits for external data (not managed by BinMgr)
    bin->flags         = bin->flags &= ~0x1; // Clear bit 0 (not allocated by BinMgr)
    bin->flags         = bin->flags &= ~0x2; // Clear bit 1 (no secondary data)
    bin->flags         = bin->flags &= ~0x4; // Clear bit 2 (not compressed)
    bin->refCount      = 1;
    bin->id            = id;
    bin->size          = size;
    bin->data          = data;
    bin->secondaryData = 0;
    BinMgr_AddToFreeList(binMgr, bin);
    return bin;
}

BOOL BinMgr_ReleaseBin(Bin* bin) {
    BinMgr* activeMgr = g_activeBinMgr;
    BinMgr* self      = (BinMgr*)bin;

    BOOL allReleased = FALSE;

    if (activeMgr == NULL) {
        return allReleased;
    }
    self->rootBin.refCount -= 1;
    if (self->rootBin.refCount == 0) {
        if (((u32)(self->rootBin.flags << 0x1F) >> 0x1F) == 1) {
            Mem_Free(&gMainHeap, self->rootBin.data);
        }
        if ((((u32)(self->rootBin.flags << 0x1E) >> 0x1F) == 1) && (self->rootBin.secondaryData != NULL)) {
            func_02027a9c();
            Mem_Free(&gMainHeap, self->rootBin.secondaryData);
        }
        BinMgr_RemoveFromFreeList(activeMgr, &self->rootBin);
        BinMgr_AppendNode(activeMgr, &self->rootBin);
        allReleased = TRUE;
    }
    return allReleased;
}

Bin* BinMgr_FindById(s32 id) {
    Bin* foundBin = NULL;
    if (&g_activeBinMgr->rootBin == NULL) {
        return NULL;
    }

    for (Bin* currentBin = g_activeBinMgr->freeList; currentBin != NULL; currentBin = currentBin->next) {
        if (currentBin->id == id) {
            foundBin = currentBin;
            break;
        }
    }

    return foundBin;
}
