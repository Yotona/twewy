#include "Engine/Core/Memory.h"
#include <nitro/fs/file.h>
#include <nitro/os/cache.h>

typedef struct {
    FS_File file;
    u32     dataOffset;
    u32     entryCount;
    u32*    entrySizes;
} FieldCache;

extern FieldCache* data_ov030_02109de0;
extern void*       func_ov030_020d2188(s32 size);
extern void        func_ov030_020d21bc(void* data);
extern BOOL        FS_GetFileSystemStatus(void);
extern BOOL        FS_InitFileSystem(s32 flags);

static inline BOOL FS_IsFileOpen(volatile const FS_File* file) {
    return (file->status & FS_FILE_STATUS_IS_FILE) != 0;
}

char data_ov030_021090ac[16] = "FOList_Alloc";

void* func_ov030_020d21e0(void) {
    data_ov030_02109de0             = func_ov030_020d2188(0x54);
    data_ov030_02109de0->entrySizes = NULL;
    data_ov030_02109de0->dataOffset = 0;
    data_ov030_02109de0->entryCount = 0;
    return data_ov030_02109de0;
}

void func_ov030_020d2218(void) {
    func_ov030_020d21bc(data_ov030_02109de0->entrySizes);
    func_ov030_020d21bc(data_ov030_02109de0);
    data_ov030_02109de0 = NULL;
}

u32 func_ov030_020d224c(const char* path, BOOL loadTable) {
    u32 fileSize = 0;

    if (data_ov030_02109de0 != NULL) {
        if (!FS_GetFileSystemStatus()) {
            FS_InitFileSystem(-1);
        }
        FS_FileInit(&data_ov030_02109de0->file);
        if (FS_FileOpen(&data_ov030_02109de0->file, path)) {
            fileSize = data_ov030_02109de0->file.endPosition - data_ov030_02109de0->file.startPosition;
            if (loadTable != FALSE) {
                u32 tableSize;

                FS_FileSeek(&data_ov030_02109de0->file, 0, 0);
                FS_FileRead(&data_ov030_02109de0->file, &data_ov030_02109de0->dataOffset,
                            sizeof(data_ov030_02109de0->dataOffset));
                FS_FileRead(&data_ov030_02109de0->file, &data_ov030_02109de0->entryCount,
                            sizeof(data_ov030_02109de0->entryCount));
                tableSize                       = data_ov030_02109de0->entryCount * sizeof(*data_ov030_02109de0->entrySizes);
                data_ov030_02109de0->entrySizes = func_ov030_020d2188(tableSize);
                FS_FileRead(&data_ov030_02109de0->file, data_ov030_02109de0->entrySizes, tableSize);
            }
        }
    }
    return fileSize;
}

void func_ov030_020d2334(void) {
    if (data_ov030_02109de0 != NULL && FS_IsFileOpen(&data_ov030_02109de0->file)) {
        FS_FileClose(&data_ov030_02109de0->file);
        FS_FileInit(&data_ov030_02109de0->file);
    }
}

// Nonmatching: register allocation of the chunked read loop
void* func_ov030_020d2378(void* buffer, u32 wantedIndex) {
    void*       dst    = NULL;
    u32         offset = 0;
    u32         i;
    u32         count;
    FieldCache* cache = data_ov030_02109de0;

    if (cache == NULL) {
        return dst;
    }
    count = cache->entryCount;
    if (count == 0) {
        return dst;
    }
    for (i = 0; i < count; i++) {
        if (i == wantedIndex) {
            u32 size;
            u32 remaining;
            u32 chunk;
            u8* readDst;

            FS_FileSeek(&data_ov030_02109de0->file, data_ov030_02109de0->dataOffset + offset, 0);
            size      = data_ov030_02109de0->entrySizes[i];
            dst       = buffer != NULL ? buffer : func_ov030_020d2188(size);
            chunk     = 0x6800;
            readDst   = dst;
            remaining = size;
            while (remaining != 0) {
                if (remaining < chunk) {
                    chunk = remaining;
                }
                FS_FileRead(&data_ov030_02109de0->file, readDst, chunk);
                readDst += chunk;
                remaining -= chunk;
            }
            DC_PurgeRange(dst, size);
            break;
        }
        offset += cache->entrySizes[i];
    }
    return dst;
}

u32 func_ov030_020d246c(void) {
    u32         count = 0;
    FieldCache* cache = data_ov030_02109de0;

    if (cache != NULL) {
        count = cache->entryCount;
    }
    return count;
}

void* func_ov030_020d2488(s32 size) {
    void* data = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, data, data_ov030_021090ac);
    return data;
}

void func_ov030_020d24bc(void* data) {
    Mem_Free(&gMainHeap, data);
}

const u32 data_ov030_020ee880[47] = {
    9,  25, 25, 9,  25, 9,  25, 25, 25, 25, 9,  25, 25, 25, 25, 25, 25, 25, 25, 25, 9, 25, 25,  25,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 9, 25, -16,
};
