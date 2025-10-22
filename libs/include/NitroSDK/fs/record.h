#ifndef NITROSDK_FS_RECORD_H
#define NITROSDK_FS_RECORD_H

#include <types.h>

struct FS_File;
struct FS_Record;

typedef enum {
    FS_RESULT_SUCCESS  = 0,
    FS_RESULT_FAILURE  = 1,
    FS_RESULT_BUSY     = 2,
    FS_RESULT_CANCELED = 3,
    FS_RESULT_BADCMD   = 4,
    FS_RESULT_ASYNC    = 6,
    FS_RESULT_UNKNOWN  = 8,
} FS_CommandResult;

typedef FS_CommandResult (*FS_RecordReadCB)(struct FS_Record* record, void* dest, u32 pos, u32 size);
typedef FS_CommandResult (*FS_RecordWriteCB)(struct FS_Record* record, const void* src, u32 pos, u32 size);
typedef FS_CommandResult (*FS_RecordMethodCB)(struct FS_File* file, u32 type);

typedef struct FS_LinkedFile {
    /* 0x00 */ struct FS_File* prev;
    /* 0x04 */ struct FS_File* next;
} FS_LinkedFile;

typedef struct FS_Record {
    /* 0x00 */ u32               name;
    /* 0x04 */ struct FS_Record* next;
    /* 0x08 */ struct FS_Record* prev;
    /* 0x0C */ void*             unk_0C;
    /* 0x10 */ void*             unk_10;
    /* 0x14 */ void*             unk_14;
    /* 0x18 */ void*             unk_18;
    /* 0x1C */ u32               flags;
    /* 0x20 */ FS_LinkedFile     fileList;
    /* 0x28 */ u32               baseOffset;
    /* 0x2C */ u32               fatOffset;
    /* 0x30 */ u32               fatSize;
    /* 0x34 */ u32               fntOffset;
    /* 0x38 */ u32               fntSize;
    /* 0x3C */ u32               fatBase;
    /* 0x40 */ u32               fntBase;
    /* 0x44 */ void*             loaded;
    /* 0x48 */ FS_RecordReadCB   readCB;
    /* 0x4C */ FS_RecordWriteCB  writeCB;
    /* 0x50 */ FS_RecordReadCB   tableCB;
    /* 0x54 */ FS_RecordMethodCB methodCB;
    /* 0x58 */ u32               methodFlags;
} FS_Record;

typedef enum {
    FS_RECORD_FLAG_LOADED       = 0x002,
    FS_RECORD_FLAG_TABLE_LOADED = 0x004,
    FS_RECORD_FLAG_HALTED       = 0x008,
    FS_RECORD_FLAG_ACTIVE       = 0x010,
    FS_RECORD_FLAG_CANCELING    = 0x020,
    FS_RECORD_FLAG_WAITING      = 0x040,
    FS_RECORD_FLAG_UNLOADING    = 0x080,
    FS_RECORD_FLAG_ASYNC        = 0x100,
    FS_RECORD_FLAG_SYNC         = 0x200,
} FS_RecordFlags;

BOOL FS_RecordWait(FS_Record* record);
BOOL FS_RecordResume(FS_Record* record);

static inline BOOL FS_IsRecordLoaded(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_LOADED) != 0;
}

static inline BOOL FS_IsRecordTableLoaded(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_TABLE_LOADED) != 0;
}

static inline BOOL FS_IsRecordHalted(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_HALTED) != 0;
}

static inline BOOL FS_IsRecordActive(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_ACTIVE) != 0;
}

static inline BOOL FS_IsRecordCanceling(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_CANCELING) != 0;
}

static inline BOOL FS_IsRecordWaiting(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_WAITING) != 0;
}

static inline BOOL FS_IsRecordAsync(volatile FS_Record* record) {
    return (record->flags & FS_RECORD_FLAG_ASYNC) != 0;
}

#endif // NITROSDK_FS_RECORD_H