#ifndef NITROSDK_FS_FILE_H
#define NITROSDK_FS_FILE_H

#include <NitroSDK/fs/record.h>
#include <types.h>

typedef struct FS_FileIdentifier {
    /* 0x00 */ struct FS_Record* record;
    /* 0x04 */ u32               fileID;
} FS_FileIdentifier;

typedef struct FS_FilePosition {
    /* 0x00 */ struct FS_Record* record;
    /* 0x04 */ u16               id;
    /* 0x06 */ u16               index;
    /* 0x08 */ u32               position;
} FS_FilePosition;

typedef struct FS_FileArgs_NormalizePath {
    FS_FilePosition position;
    const char*     path;
    BOOL            isDirectory;
    union {
        FS_FileIdentifier* file;
        FS_FilePosition*   directory;
    } response;
} FS_FileArgs_PathNormalize;

typedef struct FS_FileArgs_ProcessRead {
    void* dest;
    u32   sizeExpected;
    u32   sizeActual;
} FS_FileArgs_ProcessRead;

typedef struct FS_FileArgs_Seek {
    FS_FilePosition position;
} FS_FileArgs_Seek;

typedef struct FS_FileArgs_OpenImmediate {
    u32 start;
    u32 end;
    u32 idx;
} FS_FileArgs_OpenImmediate;

typedef struct FS_FileArgs_OpenFromID {
    FS_FileIdentifier iden;
} FS_FileArgs_OpenFromID;

typedef struct FS_File {
    /* 0x00 */ struct FS_LinkedFile link;
    /* 0x08 */ struct FS_Record*    record;
    /* 0x0C */ u32                  status;
    /* 0x10 */ u32                  unk_10;
    /* 0x14 */ FS_CommandResult     result;
    /* 0x18 */ u32                  unk_18;
    /* 0x1C */ u32                  unk_1C;
    /* 0x20 */ u32                  unk_20;
    /* 0x24 */ u32                  startPosition;
    /* 0x28 */ u32                  endPosition;
    /* 0x2C */ u32                  currentPosition;

    /* 0x30 */ union {
        FS_FileArgs_PathNormalize normalize;
        FS_FileArgs_ProcessRead   read;
        FS_FileArgs_OpenImmediate openImmediate;
        FS_FileArgs_OpenFromID    openFromId;
    } methodArgs;
} FS_File;

void FS_FileInit(FS_File* file);

BOOL FS_FilePathNormalize(FS_File* file, const char* path, FS_FileIdentifier* iden, FS_FilePosition* dirPosition);

BOOL FS_FilePathAsIden(FS_FileIdentifier* iden, const char* path);

BOOL FS_FileOpenImmediate(FS_File* file, FS_Record* record, u32 startPos, u32 endPos, u32 fileIdx);

BOOL FS_FileOpenFromIden(FS_File* file, FS_FileIdentifier iden);

BOOL FS_FileOpen(FS_File* file, const char* path);

BOOL FS_FileClose(FS_File* file);

BOOL FS_FileWait(FS_File* file);

void FS_FileStop(FS_File* file);

s32 FS_FileReadAsync(FS_File* file, void* dest, s32 size);

s32 FS_FileRead(FS_File* file, void* dest, s32 size);

BOOL FS_FileSeek(FS_File* file, s32 offset, s32 mode);

typedef enum {
    FS_FILE_STATUS_BUSY      = 0x01,
    FS_FILE_STATUS_CANCELING = 0x02,
    FS_FILE_STATUS_SYNC      = 0x04,
    FS_FILE_STATUS_ASYNC     = 0x08,
    FS_FILE_STATUS_IS_FILE   = 0x10,
    FS_FILE_STATUS_IS_DIR    = 0x20,
    FS_FILE_STATUS_ACTIVE    = 0x40,
} FS_FileStatusFlags;

static inline BOOL FS_IsFileBusy(volatile FS_File* file) {
    return (file->status & FS_FILE_STATUS_BUSY) != 0;
}

static inline BOOL FS_IsFileCanceling(volatile FS_File* file) {
    return (file->status & FS_FILE_STATUS_CANCELING) != 0;
}

static inline BOOL FS_IsFileSynchronous(volatile const FS_File* file) {
    return (file->status & FS_FILE_STATUS_SYNC) != 0;
}

static inline BOOL FS_IsFileActive(volatile const FS_File* file) {
    return (file->status & FS_FILE_STATUS_ACTIVE) != 0;
}

static inline BOOL FS_IsFileSuccessful(volatile const FS_File* file) {
    return (file->result == FS_RESULT_SUCCESS) != 0;
}

static inline void FS_FileListCut(FS_LinkedFile* list) {
    FS_File* prev = list->prev;
    FS_File* next = list->next;

    if (prev != NULL) {
        prev->link.next = next;
    }
    if (next != NULL) {
        next->link.prev = prev;
    }
}

static inline void FS_FileListAppend(FS_File* file, FS_File* list) {
    FS_LinkedFile* const link = &file->link;

    FS_FileListCut(link);

    while (list->link.next != NULL) {
        list = list->link.next;
    }

    list->link.next = file;
    link->prev      = list;
    link->next      = NULL;
}

#endif // NITROSDK_FS_FILE_H