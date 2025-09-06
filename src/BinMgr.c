#include "BinMgr.h"
#include "common_data.h"
#include "game.h"

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

/* Nonmatching */
void BinMgr_RemoveFromFreeList(BinMgr* binMgr, Bin* binToRemove) {
    Bin* currentBin = binMgr->freeList;
    while (currentBin->next != NULL) {
        if (currentBin->next == binToRemove) {
            currentBin->next = binToRemove->next;
            return;
        }
        currentBin = currentBin->next;
    }
}

/* Nonmatching */
BinMgr* BinMgr_Init(BinMgr* binMgr, u32 nodeCount) {
    BinMgr* previousMgr = g_activeBinMgr;
    g_activeBinMgr      = binMgr;
    if (binMgr == NULL) {
        return previousMgr;
    }
    binMgr->freeList     = NULL;
    binMgr->rootBin.next = NULL;
    binMgr->nodeCount    = nodeCount;

    GameState* allocatedPool = func_02004618(&data_0206a9a4, nodeCount << 5);
    func_020049a8(&data_0206a9a4, allocatedPool, "BinMgr_Init()");
    binMgr->nodePool = &allocatedPool->binMgr.rootBin;
    binMgr->listTail = &allocatedPool->binMgr.rootBin;

    s16 nodeIndex = 0;
    while (nodeIndex < binMgr->nodeCount) {
        if (allocatedPool != NULL) {
            func_0203b3c0(&allocatedPool->binMgr.rootBin, 0, sizeof(Bin));
            allocatedPool->binMgr.rootBin.next = binMgr->rootBin.next;
            binMgr->rootBin.next               = &allocatedPool->binMgr.rootBin;
            allocatedPool->binMgr.rootBin.next = allocatedPool->binMgr.freeList;
            nodeIndex++;
        }
    }
    return previousMgr;
}

/* Nonmatching */
void* BinMgr_LoadRawData(Bin* targetBuffer, int* resourceDesc, int resourceId, int offset, u32* outSize) {
    u32   maxChunkSize;
    int   currentReadOffset;
    u32   totalBytesToRead;
    char* debugStr;
    Bin*  currentReadBuffer;
    int   localResourceDesc[2];
    int   fileHandle[9];
    int   fileEndOffset;
    int   fileStartOffset;
    int   initialOffset;

    currentReadOffset = offset;
    initialOffset     = offset;
    if (resourceDesc == NULL) {
        func_0203e7bc((int)localResourceDesc, *(char**)(resourceId + 4), resourceId, offset);
        resourceDesc = localResourceDesc;
    }
    func_0203e5d4(fileHandle);
    func_0203e844(fileHandle, *resourceDesc, resourceDesc[1], currentReadOffset);
    maxChunkSize     = *outSize;
    totalBytesToRead = (fileStartOffset - fileEndOffset) - offset;
    if ((maxChunkSize != 0) && (maxChunkSize < totalBytesToRead)) {
        totalBytesToRead = maxChunkSize;
    }
    if (targetBuffer == NULL) {
        debugStr     = *(char**)(resourceId + 4);
        targetBuffer = func_020047d0(&data_0206a9a4, totalBytesToRead);
        func_020049a8(&data_0206a9a4, targetBuffer, debugStr);
    }
    *outSize = totalBytesToRead;
    func_0203ea60(fileHandle, offset, 0);
    maxChunkSize = 0x6800; // Max chunk size for reading (26KB)
    if (totalBytesToRead != 0) {
        currentReadBuffer = targetBuffer;
        do {
            if (totalBytesToRead < maxChunkSize) {
                maxChunkSize = totalBytesToRead;
            }
            if (func_0203ea50(fileHandle, currentReadBuffer, maxChunkSize) == 0xffffffff)
                break;
            currentReadBuffer = (Bin*)((char*)currentReadBuffer + maxChunkSize);
            totalBytesToRead  = totalBytesToRead - maxChunkSize;
        } while (totalBytesToRead != 0);
    }
    func_0203e8fc(fileHandle);
    return targetBuffer;
}

/* Nonmatching */
Bin* BinMgr_LoadCompressed(Bin* targetBuffer, int* resourceDesc, int resourceId, int offset, u32* outSize) {
    Bin*  compressedDataBuffer;
    u32   currentChunkSize;
    u32   decompressedSize;
    int   currentReadOffset;
    char* debugStr;
    Bin*  currentReadBuffer;
    u32   totalCompressedSize;
    u32   maxChunkSize;
    Bin*  outputBuffer;
    int   localResourceDesc[2];
    int   fileHandle[9];
    int   fileStartOffset;
    int   fileEndOffset;

    currentReadOffset = offset;
    if (resourceDesc == NULL) {
        func_0203e7bc(localResourceDesc, *(char**)(resourceId + 4), resourceId, offset);
        resourceDesc = localResourceDesc;
    }
    func_0203e5d4(fileHandle);
    func_0203e844(fileHandle, *resourceDesc, resourceDesc[1], currentReadOffset);
    totalCompressedSize  = (fileEndOffset - fileStartOffset) - offset;
    compressedDataBuffer = func_020046e8(&data_0206a9a4, totalCompressedSize);
    func_020049a8(&data_0206a9a4, compressedDataBuffer, "BinMgr_LoadComp()");
    decompressedSize = 0;
    func_0203ea60(fileHandle, offset, 0);
    maxChunkSize = 0x6800; // Max chunk size for reading (26KB)
    if (totalCompressedSize != 0) {
        currentReadBuffer = compressedDataBuffer;
        do {
            if (totalCompressedSize < maxChunkSize) {
                maxChunkSize = totalCompressedSize;
            }
            currentChunkSize = maxChunkSize;
            if (func_0203ea50(fileHandle, currentReadBuffer, maxChunkSize) == 0xffffffff)
                break;
            currentReadBuffer   = (Bin*)((char*)currentReadBuffer + maxChunkSize);
            totalCompressedSize = totalCompressedSize - maxChunkSize;
        } while (totalCompressedSize != 0);
    }
    func_0203e8fc(fileHandle);

    // Extract decompressed size from compression header
    decompressedSize = (u32)compressedDataBuffer->next >> 8;
    if (((u32)compressedDataBuffer->next & 0xf0) == 0) {
        decompressedSize = decompressedSize - 4;
    }
    if (*outSize != 0) {
        decompressedSize = *outSize;
    }
    *outSize = decompressedSize;

    outputBuffer = targetBuffer;
    if (targetBuffer == NULL) {
        debugStr     = *(char**)(resourceId + 4);
        outputBuffer = func_020047d0(&data_0206a9a4);
        func_020049a8(&data_0206a9a4, outputBuffer, debugStr);
    }
    func_02004d60(outputBuffer, compressedDataBuffer);
    func_020048b4(&data_0206a9a4, compressedDataBuffer);
    return outputBuffer;
}

Bin* BinMgr_LoadUncompressed(int* resourceDesc, u32 resourceId) {
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
    bin->data          = BinMgr_LoadRawData(NULL, resourceDesc, resourceId, 0, &bin->size);
    bin->secondaryData = 0;
    BinMgr_AddToFreeList(binMgr, bin);
    return bin;
}

Bin* BinMgr_LoadResource(int* resourceDesc, u32 resourceId) {
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
    newNode->data          = BinMgr_LoadCompressed(NULL, resourceDesc, resourceId, 0, &newNode->size);
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

/* Nonmatching */
BOOL BinMgr_ReleaseBin(Bin* bin) {
    BinMgr* binMgr = g_activeBinMgr;

    if (g_activeBinMgr == NULL) {
        return FALSE;
    }
    bin->refCount = bin->refCount - 1;
    if (bin->refCount != 0) {
        return FALSE;
    }
    // Check if data was allocated by BinMgr (bit 0 set) and needs to be freed
    if ((int)((u32)bin->flags << 0x1f) < 0) {
        func_020048b4(&data_0206a9a4, (GameState*)bin->data);
    }
    // Check if secondary data (bit 1 set) needs to be freed
    if (((int)((u32)bin->flags << 0x1e) < 0) && ((int*)bin->secondaryData != NULL)) {
        func_02027a9c(bin->secondaryData);
        func_020048b4(&data_0206a9a4, (GameState*)bin->secondaryData);
    }
    BinMgr_RemoveFromFreeList(binMgr, bin);
    BinMgr_AppendNode(binMgr, bin);
    return TRUE;
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
