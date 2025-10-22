#include <NitroSDK/fs.h>
#include <NitroSDK/util.h>

extern FS_FilePosition Record_Position;

static BOOL FS_IsFileSystemActive = FALSE;

void FS_InitFileSystem(u32 param_1) {
    if (FS_IsFileSystemActive == FALSE) {
        FS_IsFileSystemActive = TRUE;
        FS_RomInit(param_1);
    }
}

BOOL FS_GetFileSystemStatus(void) {
    return FS_IsFileSystemActive;
}

void FS_FileInit(FS_File* file) {
    file->link.prev = NULL;
    file->link.next = NULL;
    file->unk_1C    = 0;
    file->unk_18    = 0;
    file->record    = NULL;
    file->unk_10    = 14;
    file->status    = 0;
}

BOOL FS_FilePathNormalize(FS_File* file, const char* path, FS_FileIdentifier* iden, FS_FilePosition* dirPosition) {
    FS_FilePosition position;

    if (*(u8*)path == '/' || *(u8*)path == '\\') {
        position.record   = Record_Position.record;
        position.id       = 0;
        position.position = 0;
        position.index    = 0;
        path++;
    } else {
        position = Record_Position;

        for (s32 idx = 0; idx <= 3; idx++) {
            u8 nextChar = *(u8*)(path + idx);

            if (nextChar == 0 || nextChar == '/' || nextChar == '\\') {
                break;
            } else if (nextChar == ':') {
                FS_Record* rec = FS_RecordFind(path, idx);

                if (rec == NULL) {
                    return FALSE;
                }
                if (FS_IsRecordLoaded(rec) == FALSE) {
                    return FALSE;
                }
                position.record   = rec;
                position.position = 0;
                position.index    = 0;
                position.id       = 0;
                path += idx + 1;
                if (*(u8*)path == '/' || *(u8*)path == '\\') {
                    path++;
                }
                break;
            }
        }
    }

    file->record                        = position.record;
    file->methodArgs.normalize.path     = path;
    file->methodArgs.normalize.position = position;

    if (dirPosition != NULL) {
        file->methodArgs.normalize.isDirectory        = TRUE;
        file->methodArgs.normalize.response.directory = dirPosition;
    } else {
        file->methodArgs.normalize.isDirectory   = FALSE;
        file->methodArgs.normalize.response.file = iden;
    }

    return FS_RecordCommandSend(file, 4);
}

s32 FS_FileProcessRead(FS_File* file, void* dest, s32 size, BOOL async) {
    s32 currentPos    = (s32)file->currentPosition;
    s32 remainingSize = file->endPosition - currentPos;
    u32 sizeExpected  = (u32)size;

    if (size > remainingSize) {
        size = remainingSize;
    }

    if (size < 0) {
        size = 0;
    }

    file->methodArgs.read.dest         = dest;
    file->methodArgs.read.sizeExpected = sizeExpected;
    file->methodArgs.read.sizeActual   = size;

    if (async == FALSE) {
        file->status |= 4;
    }

    FS_RecordCommandSend(file, 0);

    if (async == FALSE) {
        if (FS_FileWait(file)) {
            size = file->currentPosition - currentPos;
        } else {
            size = -1;
        }
    }

    return size;
}

BOOL FS_FilePathAsIden(FS_FileIdentifier* iden, const char* path) {
    FS_File file;

    FS_FileInit(&file);
    return FS_FilePathNormalize(&file, path, iden, NULL) != FALSE;
}

BOOL FS_FileOpenImmediate(FS_File* file, FS_Record* record, u32 startPos, u32 endPos, u32 fileIdx) {
    file->record                         = record;
    file->methodArgs.openImmediate.idx   = fileIdx;
    file->methodArgs.openImmediate.start = startPos;
    file->methodArgs.openImmediate.end   = endPos;
    if (FS_RecordCommandSend(file, 7) == FALSE) {
        return FALSE;
    }

    file->status |= 0x10;
    file->status &= ~0x20;

    return TRUE;
}

BOOL FS_FileOpenFromIden(FS_File* file, FS_FileIdentifier iden) {
    if (iden.record == NULL) {
        return FALSE;
    }

    file->record                     = iden.record;
    file->methodArgs.openFromId.iden = iden;

    if (FS_RecordCommandSend(file, 6) == FALSE) {
        return FALSE;
    }

    file->status |= 0x10;
    file->status &= ~0x20;

    return TRUE;
}

BOOL FS_FileOpen(FS_File* file, const char* path) {
    FS_FileIdentifier iden;

    return FS_FilePathAsIden(&iden, path) != FALSE && FS_FileOpenFromIden(file, iden) != FALSE;
}

BOOL FS_FileClose(FS_File* file) {
    if (FS_RecordCommandSend(file, 8) == FALSE) {
        return FALSE;
    }

    file->record = NULL;
    file->unk_10 = 14;
    file->status &= 0xFFFFFFCF;
    return TRUE;
}

BOOL FS_FileWait(FS_File* file) {
    BOOL cond = FALSE;
    ENTER_CRITICAL_SECTION();

    if (FS_IsFileBusy(file)) {
        cond = (file->status & 0x44) == 0;

        if (cond) {
            file->status |= 4;
            do {
                OS_PauseThread(&file->unk_18);
            } while ((file->status & 0x40) == 0);
        } else {
            do {
                OS_PauseThread(&file->unk_18);
            } while (FS_IsFileBusy(file));
        }
    }

    LEAVE_CRITICAL_SECTION();

    if (cond) {
        return FS_RecordCommandExecute(file);
    }
    return file->result == 0;
}

void FS_FileStop(FS_File* file) {
    ENTER_CRITICAL_SECTION();

    if (FS_IsFileBusy(file)) {
        file->status |= 2;
        file->record->flags |= 0x20;
    }
    LEAVE_CRITICAL_SECTION();
}

s32 FS_FileReadAsync(FS_File* file, void* dest, s32 size) {
    return FS_FileProcessRead(file, dest, size, TRUE);
}

s32 FS_FileRead(FS_File* file, void* dest, s32 size) {
    return FS_FileProcessRead(file, dest, size, FALSE);
}

BOOL FS_FileSeek(FS_File* file, s32 offset, s32 mode) {
    switch (mode) {
        case 0:
            offset += file->startPosition;
            break;
        case 1:
            offset += file->currentPosition;
            break;
        case 2:
            offset += file->endPosition;
            break;
        default:
            return FALSE;
    }

    if (offset < (s32)file->startPosition) {
        offset = file->startPosition;
    }
    if (offset > (s32)file->endPosition) {
        offset = file->endPosition;
    }
    file->currentPosition = offset;
    return TRUE;
}
