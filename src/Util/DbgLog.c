#include "Util/DbgLog.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/System.h"
#include "Engine/IO/Str.h"

#define DBGLOG_ENTRIES 256

typedef struct {
    /* 0x000 */ u32   head;
    /* 0x004 */ char* entries[DBGLOG_ENTRIES];
} DbgLogRing; // Size: 0x404

typedef struct {
    /* 0x000 */ u32        unk_00[4];
    /* 0x010 */ DbgLogRing ring;
    /* 0x414 */ Pool       pool;
} DbgLog; // Size: 0x438

static char*  sDbgLogFullMsg = "DbgLog buf full";
static DbgLog sDbgLog;

// Nonmatching: r1/r2 swapped between the loop base and the NULL constant
void DbgLog_Reset(void) {
    Mem_ResetPool(&sDbgLog.pool);
    DbgLogRing* ring  = &sDbgLog.ring;
    sDbgLog.ring.head = 0;
    for (u32 i = 0; i < DBGLOG_ENTRIES; i++) {
        ring->entries[i] = NULL;
    }
}

BOOL DbgLog_Print(const char* str) {
    BOOL  full = FALSE;
    char* copy = Mem_AllocPool(&sDbgLog.pool, Str_Len(str) + 1);

    if (copy == NULL) {
        copy = sDbgLogFullMsg;
    } else {
        Str_Copy(copy, str);
    }

    sDbgLog.ring.entries[(u8)sDbgLog.ring.head] = copy;
    sDbgLog.ring.head                           = (u8)(sDbgLog.ring.head + 1);
    data_02066a58;
    if (sDbgLog.ring.head == 0) {
        if ((data_02066a58 << 26) >> 31) {
            full = TRUE;
        } else {
            DbgLog_Dump();
            DbgLog_Reset();
        }
    }
    return full;
}

const char* DbgLog_GetEntry(s32 index) {
    return sDbgLog.ring.entries[(u8)(sDbgLog.ring.head - index - 1)];
}

void DbgLog_Dump(void) {
    for (s32 i = 0; i < DBGLOG_ENTRIES; i++) {
        DbgLog_GetEntry(i);
    }
}
