#include "DatMgr.h"

DatMgr* g_activeDatMgr;

static Data* DatMgr_AllocateDataEntry(DatMgr* mgr) {
    Data* data = mgr->freeList.next;
    if (data != NULL) {
        mgr->freeList.next = data->next;
    }
    return data;
}

static void DatMgr_InsertDataEntry(DatMgr* mgr, s32 dataType, Data* data) {
    if (data != NULL) {
        data->next                    = mgr->dataLists[dataType].next;
        mgr->dataLists[dataType].next = data;
    }
}

static void DatMgr_ResetDataEntry(DatMgr* datMgr, Data* data) {
    if (data != NULL) {
        data->slotIndex    = 0;
        data->dataType     = DAT_TYPE_RAW;
        data->ownsData     = FALSE;
        data->isCompressed = FALSE;
        data->isLoaded     = FALSE;
        data->refCount     = 0;
        data->buffer       = 0;
        data->dataSize     = 0;
        data->unk_10       = 7;
        data->binIden      = NULL;
        data->offset       = 0;
        data->bin          = 0;
        data->pack         = 0;
        data->packIndex    = 0;
    }
}

// Nonmatching: Registers swapped
// Scratch: 01q8W
static void DatMgr_RemoveDataEntry(DatMgr* mgr, Data* data) {
    u32    index = data->slotIndex;
    Data** prev  = &mgr->dataLists[index].next;
    while (*prev != NULL) {
        if (*prev == data) {
            *prev = data->next;
            return;
        }
        prev = &(*prev)->next;
    }
}

static void DatMgr_AddToFreeList(DatMgr* mgr, Data* data) {
    if (data != NULL) {
        mgr->freeListTail->next = data;
        mgr->freeListTail       = data;
        data->next              = NULL;
    }
}

DatMgr* DatMgr_Init(DatMgr* mgr, s32 count) {
    DatMgr* prevMgr = g_activeDatMgr;
    g_activeDatMgr  = mgr;

    if (mgr == NULL) {
        return prevMgr;
    }

    for (u16 i = 0; i < ARRAY_COUNT(mgr->dataLists); i++) {
        mgr->slotActive[i]     = FALSE;
        mgr->dataLists[i].next = NULL;
    }

    mgr->freeList.next = NULL;
    mgr->slotActive[0] = TRUE;
    mgr->slotActive[1] = TRUE;
    mgr->dataCount     = count;

    Data* dataPool = Mem_AllocHeapTail(&gMainHeap, mgr->dataCount * sizeof(Data));
    Mem_SetSequence(&gMainHeap, dataPool, "DatMgr_Init()");

    mgr->dataPool     = dataPool;
    mgr->freeListTail = dataPool;

    for (u16 i = 0; i < mgr->dataCount; i++) {
        DatMgr_ResetDataEntry(mgr, dataPool);
        dataPool->next     = mgr->freeList.next;
        mgr->freeList.next = dataPool;
        dataPool++;
    }

    return prevMgr;
}

// Nonmatching: Registers swapped
Data* DatMgr_LoadRawData(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    // Search for matching data entry
    while (data != NULL) {
        if (buffer == NULL && dataSize == 0 && data->dataType == DAT_TYPE_RAW && data->ownsData == TRUE &&
            data->binIden == iden)
        {
            break;
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->isLoaded = TRUE;
            data->refCount++;
        }
        return data;
    }

    // Allocate new data entry
    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_RAW;
    data->refCount  = 1;
    data->binIden   = iden;
    data->buffer    = buffer;
    data->dataSize  = dataSize;

    if (data->buffer == NULL) {
        data->ownsData = TRUE;
    }

    data->buffer = BinMgr_LoadRawData(data->buffer, NULL, iden, 0, &data->dataSize);
    DatMgr_InsertDataEntry(datMgr, dataType, data);

    return data;
}

// Nonmatching: Unexpected stack usage
Data* DatMgr_LoadRawDataWithOffset(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden, s32 offset) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (buffer == NULL && dataSize == 0 && data->dataType == DAT_TYPE_RAW && data->ownsData == TRUE &&
            data->binIden == iden && data->offset == offset && data->dataSize == dataSize)
        {
            break;
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->isLoaded = TRUE;
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_RAW;
    data->refCount  = 1;
    data->binIden   = iden;
    data->offset    = offset;
    data->buffer    = buffer;
    data->dataSize  = dataSize;

    if (data->buffer == NULL) {
        data->ownsData = TRUE;
    }

    data->buffer = BinMgr_LoadRawData(data->buffer, NULL, iden, data->offset, &data->dataSize);
    DatMgr_InsertDataEntry(datMgr, dataType, data);

    return data;
}

// Nonmatching: Registers swapped
Data* DatMgr_LoadCompressedBin(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (buffer == NULL && dataSize == 0 && data->dataType == DAT_TYPE_COMPRESSED && data->ownsData == TRUE &&
            data->binIden == iden)
        {
            break;
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->isLoaded = TRUE;
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_COMPRESSED;
    data->refCount  = 1;

    data->binIden  = iden;
    data->buffer   = buffer;
    data->dataSize = dataSize;

    if (data->buffer == NULL) {
        data->ownsData = TRUE;
    }

    data->buffer = BinMgr_LoadCompressed(data->buffer, NULL, iden, 0, &data->dataSize);
    DatMgr_InsertDataEntry(datMgr, dataType, data);

    return data;
}

// Nonmatching: Registers swapped
Data* DatMgr_LoadPackEntry(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden, s32 packIndex, BOOL isCompressed) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (buffer == NULL && dataSize == 0) {
            if (data->dataType == DAT_TYPE_PACK_ENTRY && data->ownsData == TRUE && data->binIden == iden &&
                data->packIndex == packIndex)
            {
                break;
            }
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->isLoaded = TRUE;
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex    = dataType;
    data->dataType     = DAT_TYPE_PACK_ENTRY;
    data->refCount     = 1;
    data->isCompressed = isCompressed;

    data->binIden   = iden;
    data->pack      = PacMgr_LoadPack(iden);
    data->packIndex = packIndex;
    data->buffer    = buffer;
    data->dataSize  = dataSize;

    if (data->buffer == NULL) {
        data->ownsData = TRUE;
    }

    if (isCompressed == FALSE) {
        data->buffer = PacMgr_LoadPackEntryData(data->pack, (Bin*)data->buffer, (u32*)&data->dataSize, packIndex, FALSE);
    } else {
        data->buffer = PacMgr_LoadPackEntryData(data->pack, (Bin*)data->buffer, (u32*)&data->dataSize, packIndex, TRUE);
    }

    DatMgr_InsertDataEntry(datMgr, dataType, data);
    return data;
}

Data* DatMgr_LoadUncompressedBin(s32 dataType, BinIdentifier* iden) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (data->dataType == DAT_TYPE_UNCOMPRESSED_BIN && data->binIden == iden) {
            break;
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_UNCOMPRESSED_BIN;
    data->isLoaded  = TRUE;
    data->refCount  = 1;
    data->binIden   = iden;

    data->bin      = BinMgr_LoadUncompressed(NULL, iden);
    data->buffer   = data->bin->data;
    data->dataSize = data->bin->size;

    DatMgr_InsertDataEntry(datMgr, dataType, data);
    return data;
}

Data* DatMgr_LoadResource(s32 dataType, BinIdentifier* iden) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (data->dataType == DAT_TYPE_RESOURCE && data->binIden == iden) {
            break;
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_RESOURCE;
    data->isLoaded  = TRUE;
    data->refCount  = 1;
    data->binIden   = iden;
    data->bin       = BinMgr_LoadResource(NULL, iden);
    data->buffer    = data->bin->data;
    data->dataSize  = data->bin->size;

    DatMgr_InsertDataEntry(datMgr, dataType, data);
    return data;
}

// Nonmatching: Differences inside switch statement
Data* DatMgr_LoadPackEntryDirect(s32 dataType, BinIdentifier* iden, s32 packIndex, s32 arg3) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (data->dataType == DAT_TYPE_PACK_ENTRY_DIRECT && data->binIden == iden && data->packIndex == packIndex) {
            break;
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_PACK_ENTRY_DIRECT;
    data->isLoaded  = TRUE;
    data->refCount  = 1;

    data->binIden   = iden;
    data->bin       = BinMgr_LoadUncompressed(NULL, iden);
    data->pack      = PacMgr_LoadPack(iden);
    data->packIndex = packIndex;

    switch (arg3) {
        case 0:
            data->buffer   = (void*)PacMgr_GetPackEntryDataPtr(data->pack, packIndex);
            data->dataSize = data->pack->entries[packIndex].size;
            break;
        case 1: {
            s32* ptr  = (s32*)PacMgr_GetPackEntryDataPtr(data->pack, packIndex);
            u32  size = (*ptr & ~0xFF) >> 8;
            if (!((u8)*ptr & 0xF0)) {
                size -= 4;
            }
            data->dataSize = size;

            void* mem = Mem_AllocHeapTail(&gMainHeap, size);
            Mem_SetSequence(&gMainHeap, mem, iden->path);
            data->buffer   = mem;
            data->ownsData = TRUE;
            func_02004d60(data->buffer, ptr);
            break;
        }
    }

    DatMgr_InsertDataEntry(datMgr, dataType, data);
    return data;
}

// Nonmatching: Loop structure, early returns?
static s32 func_02008c10(s32* arg1, s32* arg2) {
    s32 count = *arg2;
    s32 i;

    for (i = 0; i <= count; i++) {
        if (arg1[i] != arg2[i]) {
            return 0;
        }
    }
    return 1;
}

// Nonmatching: Registers swapped, couple instructions out of order
Data* DatMgr_GeneratePackedData(s32 dataType, void* buffer, void* arg2, BinIdentifier* iden, s32 arg4) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (data->dataType == DAT_TYPE_GENERATED && data->binIden == iden) {
            if (func_02008c10(data->packIndex, arg4) == 1) {
                break;
            }
        }
        data = data->next;
    }

    if (data != NULL) {
        if (data->refCount != 0xFFFF) {
            data->isLoaded = TRUE;
            data->refCount++;
        }
        return data;
    }

    data = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, data);

    data->slotIndex = dataType;
    data->dataType  = DAT_TYPE_GENERATED;
    data->refCount  = 1;

    if (buffer == NULL) {
        data->ownsData = TRUE;
    }

    data->binIden   = iden;
    data->pack      = PacMgr_LoadPack(iden);
    data->packIndex = arg4;

    DatMgr_InsertDataEntry(datMgr, dataType, data);

    data->buffer   = PacMgr_GenPack(data->pack, buffer, arg2, (s32*)arg4);
    data->dataSize = Mem_GetBlockSize(&gMainHeap, data->buffer);

    return data;
}

BOOL DatMgr_ReleaseData(Data* data) {
    DatMgr* local_datMgr = g_activeDatMgr;
    BOOL    ret          = FALSE;

    if (local_datMgr == NULL) {
        return ret;
    }

    if (data == NULL) {
        return ret;
    }

    data->refCount--;

    if (data->refCount == 0) {
        if (data->pack != NULL) {
            PacMgr_ReleasePack(data->pack);
        }

        if (data->bin != NULL) {
            BinMgr_ReleaseBin(data->bin);
        }

        if (data->ownsData == TRUE && data->buffer != NULL) {
            Mem_Free(&gMainHeap, data->buffer);
        }

        DatMgr_RemoveDataEntry(local_datMgr, data);
        DatMgr_AddToFreeList(local_datMgr, data);
        DatMgr_ResetDataEntry(local_datMgr, data);
        ret = TRUE;
    }

    return ret;
}

s32 DatMgr_AllocateSlot(void) {
    s32     found = -1;
    DatMgr* mgr   = g_activeDatMgr;

    for (s32 i = 2; i < 10; i++) {
        if (mgr->slotActive[i] == FALSE) {
            found = i;
            break;
        }
    }
    mgr->slotActive[found] = TRUE;
    return found;
}

void DatMgr_ClearSlot(s32 dataType) {
    DatMgr* mgr = g_activeDatMgr;
    if (mgr == NULL) {
        return;
    }

    Data* data = mgr->dataLists[dataType].next;
    while (data != NULL) {
        while (data->refCount > 0) {
            DatMgr_ReleaseData(data);
        }
        data = data->next;
    }

    mgr->slotActive[dataType] = FALSE;
}
