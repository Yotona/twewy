#include <NitroSDK/fs.h>
#include <NitroSDK/util.h>

static FS_Record* Record_List = NULL;

/* Current directory position*/
FS_FilePosition Record_Position;

static u32 FS_RecordCondenseName(const char* name, s32 nameLen) {
    u32 ret = 0;
    if (nameLen <= 3) {
        int i = 0;
        for (; i < nameLen; ++i) {
            u32 c = (u32)(*(u8*)(name + i));
            if (c == 0) {
                break;
            }
            c = (u32)(c - 'A');
            if (c <= (u32)('Z' - 'A'))
                c = (u32)(c + 'a');
            else
                c = (u32)(c + 'A');
            ret |= (u32)(c << (i * 8));
        }
    }
    return ret;
}

static FS_CommandResult FS_RecordCommandReadCB(FS_Record* record, void* dest, u32 pos, u32 size) {
    MI_CpuCopyU8(record->baseOffset + pos, dest, size);
    return FS_RESULT_SUCCESS;
}

static FS_CommandResult FS_RecordCommandWriteCB(FS_Record* record, const void* src, u32 pos, u32 size) {
    MI_CpuCopyU8(src, record->baseOffset + pos, size);
    return FS_RESULT_SUCCESS;
}

static FS_CommandResult FS_RecordCommandRead(FS_Record* record, void* dest, u32 pos, u32 size) {
    MI_CpuCopyU8(pos, dest, size);
    return FS_RESULT_SUCCESS;
}

static FS_File* FS_RecordCommandNext(FS_Record* record) {
    ENTER_CRITICAL_SECTION();

    if (FS_IsRecordCanceling(record)) {
        FS_File *p, *q;

        record->flags &= ~FS_RECORD_FLAG_CANCELING;

        for (p = record->fileList.next; p; p = q) {
            q = p->link.next;
            if (FS_IsFileCanceling(p)) {
                if (record->fileList.next == p) {
                    record->fileList.next = q;
                }

                func_0203cf2c(p, FS_RESULT_CANCELED);
                if (q == NULL) {
                    q = record->fileList.next;
                }
            }
        }
    }

    if (FS_IsRecordWaiting(record) == FALSE && FS_IsRecordHalted(record) == FALSE && record->fileList.next) {
        FS_File*   file       = record->fileList.next;
        const BOOL isInactive = !FS_IsRecordActive(record);

        if (isInactive) {
            record->flags |= FS_RECORD_FLAG_ACTIVE;
        }

        LEAVE_CRITICAL_SECTION();

        if (isInactive) {
            if ((record->methodFlags & 0x200) != 0) {
                record->methodCB(file, 9);
            }
        }

        ENTER_CRITICAL_SECTION();

        file->status |= FS_FILE_STATUS_ACTIVE;

        if (FS_IsFileSynchronous(file)) {
            OS_UnpauseThread(&file->unk_18);
            LEAVE_CRITICAL_SECTION();
            return NULL;
        } else {
            LEAVE_CRITICAL_SECTION();
            return file;
        }
    }

    if (FS_IsRecordActive(record)) {
        record->flags &= ~FS_RECORD_FLAG_ACTIVE;
        if ((record->methodFlags & 0x400) != 0) {
            FS_File tmp;
            FS_FileInit(&tmp);
            tmp.record = record;
            record->methodCB(&tmp, 10);
        }
    }

    if (FS_IsRecordWaiting(record)) {
        record->flags &= ~FS_RECORD_FLAG_WAITING;
        record->flags |= FS_RECORD_FLAG_HALTED;
        OS_UnpauseThread(&record->unk_14);
    }
    LEAVE_CRITICAL_SECTION();

    return NULL;
}

static void FS_RecordCommandExecuteAsync(FS_File* file) {
    FS_Record* const record = file->record;

    while (file != NULL) {
        ENTER_CRITICAL_SECTION();

        file->status |= FS_FILE_STATUS_ACTIVE;
        if (FS_IsFileSynchronous(file)) {
            OS_UnpauseThread(&file->unk_18);
            LEAVE_CRITICAL_SECTION();
            break;
        } else {
            file->status |= FS_FILE_STATUS_ASYNC;
        }
        LEAVE_CRITICAL_SECTION();
        if (func_0203cf88(file, file->unk_10) == FS_RESULT_ASYNC) {
            break;
        }
        file = FS_RecordCommandNext(record);
    }
}

BOOL FS_RecordCommandExecute(FS_File* file) {
    FS_CommandResult ret = func_0203cf88(file, file->unk_10);

    func_0203cf2c(file, ret);

    FS_File* p_target = FS_RecordCommandNext(file->record);
    if (p_target != NULL) {
        FS_RecordCommandExecuteAsync(p_target);
    }
    return FS_IsFileSuccessful(file);
}

BOOL FS_RecordCommandSend(FS_File* file, u32 command) {
    FS_Record* const record = file->record;
    const int        bit    = (1 << command);

    file->unk_10 = command;
    file->result = FS_RESULT_BUSY;
    file->status |= FS_FILE_STATUS_BUSY;

    ENTER_CRITICAL_SECTION();

    if (record->flags & FS_RECORD_FLAG_UNLOADING) {
        func_0203cf2c(file, FS_RESULT_CANCELED);
        LEAVE_CRITICAL_SECTION();
        return FALSE;
    }

    if ((bit & 0x1FC) != 0) {
        file->status |= FS_FILE_STATUS_SYNC;
    }
    FS_FileListAppend(file, (FS_File*)&record->fileList);

    if (FS_IsRecordHalted(record) == FALSE && FS_IsRecordActive(record) == FALSE) {
        record->flags |= 0x10;
        LEAVE_CRITICAL_SECTION();
        if ((record->methodFlags & 0x200) != 0)
            record->methodCB(file, 9);
        ENTER_CRITICAL_SECTION();
        file->status |= 0x40;

        if (FS_IsFileSynchronous(file) == FALSE) {
            LEAVE_CRITICAL_SECTION();
            FS_RecordCommandExecuteAsync(file);
            return TRUE;
        }

        LEAVE_CRITICAL_SECTION();
    } else if (FS_IsFileSynchronous(file) == FALSE) {
        LEAVE_CRITICAL_SECTION();
        return TRUE;
    } else {
        do {
            OS_PauseThread(&file->unk_18);
        } while ((file->status & FS_FILE_STATUS_ACTIVE) == FALSE);
        LEAVE_CRITICAL_SECTION();
    }

    return FS_RecordCommandExecute(file);
}

void FS_RecordInit(FS_Record* record) {
    MI_CpuSet(record, 0, sizeof(FS_Record));
    record->unk_0C = record->unk_10 = 0;
    record->unk_14 = record->unk_18 = 0;
}

FS_Record* FS_RecordFind(const char* name, s32 name_len) {
    u32 pack = FS_RecordCondenseName(name, name_len);
    ENTER_CRITICAL_SECTION();
    FS_Record* record = Record_List;

    while (record != NULL && record->name != pack) {
        record = record->next;
    }

    LEAVE_CRITICAL_SECTION();

    return record;
}

BOOL FS_RecordRegister(FS_Record* record, const char* name, u32 name_len) {
    BOOL result = FALSE;
    ENTER_CRITICAL_SECTION();

    if (FS_RecordFind(name, name_len) == FALSE) {
        FS_Record* listNode = Record_List;
        if (listNode == NULL) {
            Record_List              = record;
            Record_Position.record   = record;
            Record_Position.position = 0;
            Record_Position.index    = 0;
            Record_Position.id       = 0;
        } else {
            while (listNode->next) {
                listNode = listNode->next;
            }
            listNode->next = record;
            record->prev   = listNode;
        }
        record->name = FS_RecordCondenseName(name, name_len);
        record->flags |= 1;
        result = TRUE;
    }

    LEAVE_CRITICAL_SECTION();
    return result;
}

void FS_RecordRelease(FS_Record* record) {
    if (record->name != 0) {
        ENTER_CRITICAL_SECTION();

        if (record->next) {
            record->next->prev = record->prev;
        }

        if (record->prev) {
            record->prev->next = record->next;
        }

        record->name = 0;
        record->next = record->prev = NULL;
        record->flags &= ~1;

        if (Record_Position.record == record) {
            Record_Position.record   = Record_List;
            Record_Position.position = 0;
            Record_Position.index    = 0;
            Record_Position.id       = 0;
        }
        LEAVE_CRITICAL_SECTION();
    }
}

BOOL FS_RecordLoad(FS_Record* record, u32 baseOffset, u32 fat, u32 fatSize, u32 fnt, u32 fntSize, FS_RecordReadCB readCB,
                   FS_RecordWriteCB writeCB) {
    record->baseOffset = baseOffset;
    record->fatSize    = fatSize;
    record->fatOffset = record->fatBase = fat;
    record->fntSize                     = fntSize;
    record->fntOffset = record->fntBase = fnt;
    record->readCB                      = (readCB != NULL) ? readCB : FS_RecordCommandReadCB;
    record->writeCB                     = (writeCB != NULL) ? writeCB : FS_RecordCommandWriteCB;
    record->tableCB                     = record->readCB;
    record->loaded                      = NULL;
    record->flags |= FS_RECORD_FLAG_LOADED;
    return TRUE;
}

BOOL FS_RecordUnload(FS_Record* record) {
    ENTER_CRITICAL_SECTION();
    if (FS_IsRecordLoaded(record)) {
        FS_IsRecordTableLoaded(record); // Doesn't seem to do anything

        FS_File *p, *q;
        BOOL     bak_state = FS_RecordWait(record);
        record->flags |= FS_RECORD_FLAG_UNLOADING;

        for (p = record->fileList.next; p; p = q) {
            q = p->link.next;
            func_0203cf2c(p, FS_RESULT_CANCELED);
        }

        record->fileList.next = NULL;
        if (bak_state) {
            FS_RecordResume(record);
        }

        record->baseOffset = 0;
        record->fatOffset  = 0;
        record->fatSize    = 0;
        record->fntOffset  = 0;
        record->fntSize    = 0;
        record->fatBase = record->fntBase = 0;
        record->flags &= ~(FS_RECORD_FLAG_CANCELING | FS_RECORD_FLAG_LOADED | FS_RECORD_FLAG_UNLOADING);
    }
    LEAVE_CRITICAL_SECTION();

    return TRUE;
}

u32 FS_RecordLoadTable(FS_Record* record, void* buffer, u32 max_size) {
    u32 total_size = ALIGN_BYTE(record->fatSize + record->fntSize + 32, 32);
    if (total_size <= max_size) {
        u8*     cache = ALIGN_BYTE(buffer, 32);
        FS_File tmp;
        FS_FileInit(&tmp);
        if (FS_FileOpenImmediate(&tmp, record, record->fatOffset, record->fatOffset + record->fatSize, -1)) {
            if (FS_FileRead(&tmp, cache, record->fatSize) < 0) {
                MI_CpuSet(cache, 0, record->fatSize);
            }
            FS_FileClose(&tmp);
        }
        record->fatOffset = cache;
        cache += record->fatSize;
        if (FS_FileOpenImmediate(&tmp, record, record->fntOffset, record->fntOffset + record->fntSize, -1)) {
            if (FS_FileRead(&tmp, cache, record->fntSize) < 0) {
                MI_CpuSet(cache, 0, record->fntSize);
            }
            FS_FileClose(&tmp);
        }
        record->fntOffset = cache;
        record->loaded    = buffer;
        record->tableCB   = FS_RecordCommandRead;
        record->flags |= FS_RECORD_FLAG_TABLE_LOADED;
    }
    return total_size;
}

BOOL FS_RecordWait(FS_Record* record) {
    ENTER_CRITICAL_SECTION();
    BOOL isHalted = !FS_IsRecordHalted(record);
    if (isHalted) {
        if (FS_IsRecordActive(record)) {
            record->flags |= FS_RECORD_FLAG_WAITING;
            do {
                OS_PauseThread(&record->unk_14);
            } while (FS_IsRecordWaiting(record));
        } else {
            record->flags |= FS_RECORD_FLAG_HALTED;
        }
    }
    LEAVE_CRITICAL_SECTION();
    return isHalted;
}

BOOL FS_RecordResume(FS_Record* record) {
    FS_File* file = NULL;
    ENTER_CRITICAL_SECTION();
    BOOL isHalted = !FS_IsRecordHalted(record);
    if (isHalted == FALSE) {
        record->flags &= ~FS_RECORD_FLAG_HALTED;
        file = FS_RecordCommandNext(record);
    }
    LEAVE_CRITICAL_SECTION();
    if (file != NULL) {
        FS_RecordCommandExecuteAsync(file);
    }
    return isHalted;
}

void FS_RecordSetMethod(FS_Record* record, FS_RecordMethodCB callback, u32 flags) {
    if (flags == 0) {
        callback = NULL;
    } else if (callback == NULL) {
        flags = 0;
    }
    record->methodCB    = callback;
    record->methodFlags = flags;
}

void FS_RecordNotifyEnd(FS_Record* record, FS_CommandResult result) {
    if (FS_IsRecordAsync(record)) {
        FS_File* file = record->fileList.next;
        record->flags &= ~FS_RECORD_FLAG_ASYNC;
        func_0203cf2c(file, result);
        file = FS_RecordCommandNext(record);
        if (file != NULL) {
            FS_RecordCommandExecuteAsync(file);
        }
    } else {
        FS_File* file = record->fileList.next;
        ENTER_CRITICAL_SECTION();
        file->result = result;
        record->flags &= ~FS_RECORD_FLAG_SYNC;
        OS_UnpauseThread(&record->unk_0C);
        LEAVE_CRITICAL_SECTION();
    }
}
