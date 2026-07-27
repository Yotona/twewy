#include "Engine/File/DatMgr.h"

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

    for (u16 i = 0; i < ARRAY_LEN(mgr->dataLists); i++) {
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
    Data* loadedData = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, loadedData);

    loadedData->slotIndex = dataType;
    loadedData->dataType  = DAT_TYPE_RAW;
    loadedData->refCount  = 1;
    loadedData->binIden   = iden;
    loadedData->buffer    = buffer;
    loadedData->dataSize  = dataSize;

    if (loadedData->buffer == NULL) {
        loadedData->ownsData = TRUE;
    }

    loadedData->buffer = BinMgr_LoadRawData(loadedData->buffer, NULL, iden, 0, &loadedData->dataSize);
    DatMgr_InsertDataEntry(datMgr, dataType, loadedData);

    return loadedData;
}

Data* DatMgr_LoadRawDataWithOffset(s32 dataType, void* buffer, s32 dataSize, const BinIdentifier* iden, s32 offset) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (buffer == NULL && dataSize == 0 && data->dataType == DAT_TYPE_RAW && data->ownsData == TRUE &&
            data->binIden == iden && (data->offset & 0xFFFFFFFF) == offset && data->dataSize == dataSize)
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

    Data* loadedData = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, loadedData);
    loadedData->slotIndex = dataType;
    loadedData->dataType  = DAT_TYPE_RAW;
    loadedData->refCount  = 1;
    loadedData->binIden   = iden;
    loadedData->offset    = offset;
    loadedData->buffer    = buffer;
    loadedData->dataSize  = dataSize;
    if (loadedData->buffer == NULL) {
        loadedData->ownsData = TRUE;
    }
    loadedData->buffer = BinMgr_LoadRawData(loadedData->buffer, NULL, iden, loadedData->offset, &loadedData->dataSize);
    DatMgr_InsertDataEntry(datMgr, dataType, loadedData);
    return loadedData;
}

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

    Data* loadedData = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, loadedData);

    loadedData->slotIndex = dataType;
    loadedData->dataType  = DAT_TYPE_COMPRESSED;
    loadedData->refCount  = 1;

    loadedData->binIden  = iden;
    loadedData->buffer   = buffer;
    loadedData->dataSize = dataSize;

    if (loadedData->buffer == NULL) {
        loadedData->ownsData = TRUE;
    }

    loadedData->buffer = BinMgr_LoadCompressed(loadedData->buffer, NULL, iden, 0, &loadedData->dataSize);
    DatMgr_InsertDataEntry(datMgr, dataType, loadedData);

    return loadedData;
}

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

    Data* loadedData = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, loadedData);

    loadedData->slotIndex    = dataType;
    loadedData->dataType     = DAT_TYPE_PACK_ENTRY;
    loadedData->refCount     = 1;
    loadedData->isCompressed = isCompressed;

    loadedData->binIden   = iden;
    loadedData->pack      = PacMgr_LoadPack(iden);
    loadedData->packIndex = packIndex;
    loadedData->buffer    = buffer;
    loadedData->dataSize  = dataSize;

    if (loadedData->buffer == NULL) {
        loadedData->ownsData = TRUE;
    }

    if (isCompressed == FALSE) {
        loadedData->buffer = PacMgr_LoadPackEntryData(loadedData->pack, (Bin*)loadedData->buffer, (u32*)&loadedData->dataSize,
                                                      packIndex, FALSE);
    } else {
        loadedData->buffer =
            PacMgr_LoadPackEntryData(loadedData->pack, (Bin*)loadedData->buffer, (u32*)&loadedData->dataSize, packIndex, TRUE);
    }

    DatMgr_InsertDataEntry(datMgr, dataType, loadedData);
    return loadedData;
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

    Data* loadedData = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, loadedData);
    loadedData->slotIndex = dataType;
    loadedData->dataType  = DAT_TYPE_PACK_ENTRY_DIRECT;
    loadedData->isLoaded  = 1;
    loadedData->refCount  = 1;
    loadedData->binIden   = iden;
    loadedData->bin       = BinMgr_LoadUncompressed(NULL, iden);
    loadedData->pack      = PacMgr_LoadPack(iden);
    loadedData->packIndex = packIndex;
    switch (arg3) {
        case 0:
            loadedData->buffer   = (void*)(PacMgr_GetPackEntryDataPtr(loadedData->pack, packIndex) & 0xFFFFFFFF);
            loadedData->dataSize = loadedData->pack->entries[packIndex].size;
            break;

        case 1: {
            s32*  ptr     = (s32*)PacMgr_GetPackEntryDataPtr(loadedData->pack, packIndex);
            u32   size    = (((*ptr) & 0xFFFFFFFF) & (~0xFF)) >> 8;
            char* new_var = ptr;
            if (!(((u8)(*new_var)) & 0xF0)) {
                size -= 4;
            }
            loadedData->dataSize = size;
            new_var              = iden->path;
            void* mem            = Mem_AllocHeapTail(&gMainHeap, size);
            Mem_SetSequence(&gMainHeap, mem, new_var);
            loadedData->buffer   = mem;
            loadedData->ownsData = 1;
            func_02004d60(loadedData->buffer, ptr);
            break;
        }
    }

    DatMgr_InsertDataEntry(datMgr, dataType, loadedData);
    return loadedData;
}

// Nonmatching
static s32 func_02008c10(Data* data, s32* arg2) {
    s32* p     = (s32*)data->packIndex;
    s32  count = *arg2;
    s32  ret   = 1;

    for (s32 i = 0; i <= count; i++) {
        if (arg2[i] != p[i]) {
            ret = 0;
            break;
        }
    }
    return ret;
}

Data* DatMgr_GeneratePackedData(s32 dataType, void* buffer, void* arg2, BinIdentifier* iden, s32 arg4) {
    DatMgr* datMgr = g_activeDatMgr;
    if (datMgr == NULL) {
        return NULL;
    }

    Data* data = datMgr->dataLists[dataType].next;

    while (data != NULL) {
        if (data->dataType == DAT_TYPE_GENERATED && data->binIden == iden) {
            if (func_02008c10(data, (s32*)arg4) == 1) {
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

    Data* loadedData = DatMgr_AllocateDataEntry(datMgr);
    DatMgr_ResetDataEntry(datMgr, loadedData);

    loadedData->slotIndex = dataType;
    loadedData->dataType  = DAT_TYPE_GENERATED;
    loadedData->refCount  = 1;

    if (buffer == NULL) {
        loadedData->ownsData = TRUE;
    }

    loadedData->binIden   = iden;
    loadedData->pack      = PacMgr_LoadPack(iden);
    loadedData->packIndex = arg4;

    DatMgr_InsertDataEntry(datMgr, dataType, loadedData);

    loadedData->buffer   = PacMgr_GenPack(loadedData->pack, buffer, arg2, (s32*)arg4);
    loadedData->dataSize = Mem_GetBlockSize(&gMainHeap, loadedData->buffer);

    return loadedData;
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
