#include "Field/Flash/FOFileList.h"
#include "Engine/Core/Memory.h"
#include "Field/Flash/FO3DViewer.h"
#include <nitro/fs/file.h>
#include <nitro/os/cache.h>

typedef struct {
    FS_File file;
    u32     dataOffset;
    u32     entryCount;
    u32*    entrySizes;
} FieldCache;

FieldCache* gFOFileList;
extern BOOL FS_GetFileSystemStatus(void);
extern BOOL FS_InitFileSystem(s32 flags);

static inline BOOL FS_IsFileOpen(volatile const FS_File* file) {
    return (file->status & FS_FILE_STATUS_IS_FILE) != 0;
}

void* FOFileList_Create(void) {
    gFOFileList             = FOFileIO_Alloc(0x54);
    gFOFileList->entrySizes = NULL;
    gFOFileList->dataOffset = 0;
    gFOFileList->entryCount = 0;
    return gFOFileList;
}

void FOFileList_Destroy(void) {
    FOFileIO_Free(gFOFileList->entrySizes);
    FOFileIO_Free(gFOFileList);
    gFOFileList = NULL;
}

u32 FOFileList_Open(const char* path, BOOL loadTable) {
    u32 fileSize = 0;

    if (gFOFileList != NULL) {
        if (!FS_GetFileSystemStatus()) {
            FS_InitFileSystem(-1);
        }
        FS_FileInit(&gFOFileList->file);
        if (FS_FileOpen(&gFOFileList->file, path)) {
            fileSize = gFOFileList->file.endPosition - gFOFileList->file.startPosition;
            if (loadTable != FALSE) {
                u32 tableSize;

                FS_FileSeek(&gFOFileList->file, 0, 0);
                FS_FileRead(&gFOFileList->file, &gFOFileList->dataOffset, sizeof(gFOFileList->dataOffset));
                FS_FileRead(&gFOFileList->file, &gFOFileList->entryCount, sizeof(gFOFileList->entryCount));
                tableSize               = gFOFileList->entryCount * sizeof(*gFOFileList->entrySizes);
                gFOFileList->entrySizes = FOFileIO_Alloc(tableSize);
                FS_FileRead(&gFOFileList->file, gFOFileList->entrySizes, tableSize);
            }
        }
    }
    return fileSize;
}

void FOFileList_Close(void) {
    if (gFOFileList != NULL && FS_IsFileOpen(&gFOFileList->file)) {
        FS_FileClose(&gFOFileList->file);
        FS_FileInit(&gFOFileList->file);
    }
}

void* FOFileList_ReadEntry(void* buffer, u32 wantedIndex) {
    void*       dst = NULL;
    u32         offset;
    u32         i;
    FieldCache* cache = gFOFileList;

    if (cache == NULL) {
        return dst;
    }
    if (cache->entryCount == 0) {
        return dst;
    }
    offset = 0;
    for (i = 0; i < cache->entryCount; i++) {
        if (i == wantedIndex) {
            u32 size;
            u32 chunk;
            u32 remaining;
            u8* readDst;

            FS_FileSeek(&gFOFileList->file, gFOFileList->dataOffset + offset, 0);
            size = gFOFileList->entrySizes[i];
            if (buffer != NULL) {
                dst = buffer;
            } else {
                dst = FOFileIO_Alloc(size);
            }
            chunk     = 0x6800;
            remaining = size;
            readDst   = dst;
            while (remaining != 0) {
                if (remaining < chunk) {
                    chunk = remaining;
                }
                FS_FileRead(&gFOFileList->file, readDst, chunk);
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

u32 FOFileList_GetEntryCount(void) {
    u32         count = 0;
    FieldCache* cache = gFOFileList;

    if (cache != NULL) {
        count = cache->entryCount;
    }
    return count;
}

void* FOList_Alloc(s32 size) {
    void* data = Mem_AllocHeapTail(&gMainHeap, size);
    Mem_SetSequence(&gMainHeap, data, "FOList_Alloc");
    return data;
}

void FOList_Free(void* data) {
    Mem_Free(&gMainHeap, data);
}

const u32 FO3DViewer_ScenePointCounts[47] = {
    9,  25, 25, 9,  25, 9,  25, 25, 25, 25, 9,  25, 25, 25, 25, 25, 25, 25, 25, 25, 9, 25, 25,  25,
    25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 25, 9, 25, -16,
};
