#include <MSL/Runtime/MWException.h>
#include <NitroSDK/fs.h>
#include <NitroSDK/os/cache.h>
#include <NitroSDK/util.h>
#include <bios.h>

// Both are same address (0x02066A1C), uncertain how they are actually populated
extern u8 ARM9_DATA_END[];
extern u8 ARM9_BSS_START[];

const u8 data_0206314c[64] = {
    0x21, 0x06, 0xC0, 0xDE, 0xBA, 0x98, 0xCE, 0x3F, 0xA6, 0x92, 0xE3, 0x9D, 0x46, 0xF2, 0xED, 0x01,
    0x76, 0xE3, 0xCC, 0x08, 0x56, 0x23, 0x63, 0xFA, 0xCA, 0xD4, 0xEC, 0xDF, 0x9A, 0x62, 0x78, 0x34,
    0x8F, 0x6D, 0x63, 0x3C, 0xFE, 0x22, 0xCA, 0x92, 0x20, 0x88, 0x97, 0x23, 0xD2, 0xCF, 0xAE, 0xC2,
    0x32, 0x67, 0x8D, 0xFE, 0xCA, 0x83, 0x64, 0x98, 0xAC, 0xFD, 0x3E, 0x37, 0x87, 0x46, 0x58, 0x24,
};

void* data_0206627c = &data_0206314c;
u32   data_02066280 = sizeof(data_0206314c);

u32 FS_GetOverlayRamSize(OverlayInfo* info) {
    if (FS_IsOverlayCompressed(info)) {
        return info->sizeCompressed;
    } else {
        return info->sizeRam;
    }
}

void FS_InvalidateOverlay(OverlayInfo* info) {
    void* addr      = info->addr;
    u32   ramSize   = info->sizeRam;
    u32   totalSize = ramSize + info->sizeBss;

    IC_InvalidateRange(addr, totalSize);
    DC_InvalidateRange(addr, totalSize);
    func_0203b3c0(addr + ramSize, 0, totalSize - ramSize);
}

FS_FileIdentifier FS_GetOverlayIdentifier(OverlayInfo* info) {
    FS_FileIdentifier chunk;

    chunk.record = &Rom_DefaultRecord;
    chunk.fileID = info->unk_18;
    return chunk;
}

BOOL FS_LoadVerifyOverlay(OverlayInfo* info, u32 param_2, u32 id, void* param_4, u32 param_5, u32 param_6, u32 param_7,
                          u32 param_8) {
    UnkOverlayDetails pr[1];
    FS_File           file[1];

    if (param_2 == 0) {
        pr->unk_00 = param_5;
        pr->unk_04 = param_6;
    } else {
        pr->unk_00 = param_7;
        pr->unk_04 = param_8;
    }

    u32 pos = id * 0x20;
    if (pos >= pr->unk_04) {
        return FALSE;
    }

    FS_FileInit(file);

    if (FS_FileOpenImmediate(file, param_4, pr->unk_00 + pos, pr->unk_00 + pr->unk_04, -1) == FALSE) {
        return FALSE;
    }

    if (FS_FileRead(file, info, 0x20) != 0x20) {
        FS_FileClose(file);
        return FALSE;
    }
    FS_FileClose(file);
    info->unk_20 = param_2;

    if (FS_FileOpenFromIden(file, FS_GetOverlayIdentifier(info)) == FALSE) {
        return FALSE;
    }

    info->unk_24.unk_00 = file->startPosition;
    info->unk_24.unk_04 = file->endPosition - file->startPosition;
    FS_FileClose(file);

    return TRUE;
}

BOOL FS_LoadOverlayInfo(OverlayInfo* info, s32 param_2, u32 id) {
    const CartridgeRegion* details;
    FS_File                file[1];

    if (param_2 == 0) {
        details = &Arm9OverlayTable;
    } else {
        details = &Arm7OverlayTable;
    }

    if (details->offset != 0) {
        const u32 pos = id * 0x20;
        if (pos >= details->size) {
            return FALSE;
        }

        func_0203b454((void*)(details->offset + pos), info, 0x20);
        info->unk_20 = param_2;

        FS_FileInit(file);
        if (FS_FileOpenFromIden(file, FS_GetOverlayIdentifier(info)) == FALSE) {
            return FALSE;
        } else {
            info->unk_24.unk_00 = file->startPosition;
            info->unk_24.unk_04 = file->endPosition - file->startPosition;
            FS_FileClose(file);
        }
        return TRUE;
    }

    UnkOverlayDetails* unkDetail50 = (UnkOverlayDetails*)0x27FFE50;
    UnkOverlayDetails* unkDetail58 = (UnkOverlayDetails*)0x27FFE58;
    return FS_LoadVerifyOverlay(info, param_2, id, &Rom_DefaultRecord, unkDetail50->unk_00, unkDetail50->unk_04,
                                unkDetail58->unk_00, unkDetail58->unk_04);
}

BOOL FS_LoadOverlayFile(OverlayInfo* info) {
    FS_File file[1];

    FS_FileInit(file);

    if (FS_FileOpenFromIden(file, FS_GetOverlayIdentifier(info)) == FALSE) {
        return FALSE;
    } else {
        s32 size = FS_GetOverlayRamSize(info);

        FS_InvalidateOverlay(info);

        if (FS_FileRead(file, info->addr, size) != size) {
            FS_FileClose(file);
            return FALSE;
        }
    }

    FS_FileClose(file);
    return TRUE;
}

static BOOL FS_ValidateOverlay(const u8* query, void* src, s32 size) {
    s32 i;
    u8  array[20];
    u8  key[64];

    func_0203b3c0(array, 0, sizeof(array));
    func_0203b454(data_0206627c, key, data_02066280);
    func_0203f6f0(array, src, size, key, data_02066280);

    for (i = 0; i < sizeof(array); i += sizeof(u32)) {
        if (*(u32*)(array + i) != *(u32*)(query + i)) {
            break;
        }
    }
    return i == sizeof(array);
}

// Nonmatching: ARM9_BSS_START/ARM9_DATA_END are *not* equivalent to data_02066a1c
void FS_StartOverlay(OverlayInfo* info) {
    u32 size = FS_GetOverlayRamSize(info);

    if (BIOS_BOOT_MODE == 2) {
        BOOL valid = FALSE;

        if ((info->flags & 2) != 0) {
            if (info->unk_00 < ((ARM9_DATA_END - ARM9_BSS_START) / 20)) {

                u32 bssAddr = (ARM9_BSS_START + 20 * info->unk_00);

                valid = FS_ValidateOverlay(bssAddr, info->addr, size);
            }
        }
        if (valid == FALSE) {
            func_0203b3c0(info->addr, 0, size);
            func_0203aad0();
            return;
        }
    }

    if (FS_IsOverlayCompressed(info)) {
        func_02000970(info->addr + size);
    }
    DC_PurgeRange(info->addr, info->sizeRam);

    OverlayFunc* initFunc = info->initFunc;
    OverlayFunc* endFunc  = info->endFunc;
    for (; initFunc < endFunc; ++initFunc) {
        if (*initFunc != NULL) {
            (**initFunc)();
        }
    }
}

void FS_DestroyOverlay(OverlayInfo* info) {
    while (TRUE) {
        DestructorChain* head = NULL;
        DestructorChain* tail = NULL;

        const u32 start = info->addr;
        const u32 end   = start + (info->sizeRam + info->sizeBss);

        ENTER_CRITICAL_SECTION();

        DestructorChain* prev    = NULL;
        DestructorChain* base    = __global_destructor_chain;
        DestructorChain* current = base;

        while (current != NULL) {
            DestructorChain* next   = current->next;
            u32              dtor   = current->dtor;
            u32              object = current->object;

            if ((object == 0 && dtor >= start && dtor < end) || (object >= start && object < end)) {
                if (tail != NULL) {
                    tail->next = current;
                } else {
                    head = current;
                }

                if (base == current) {
                    base = __global_destructor_chain = next;
                }

                tail          = current;
                current->next = NULL;

                if (prev != NULL) {
                    prev->next = next;
                }
            } else {
                prev = current;
            }
            current = next;
        }

        LEAVE_CRITICAL_SECTION();

        if (head == NULL) {
            return;
        }

        do {
            DestructorChain* next = head->next;
            if (head->dtor != NULL) {
                head->dtor(head->object);
            }
            head = next;
        } while (head != NULL);
    }
}

BOOL FS_StopOverlay(OverlayInfo* info) {
    FS_DestroyOverlay(info);
    return TRUE;
}

BOOL FS_LoadOverlay(u32 param_1, u32 overlayID) {
    OverlayInfo info;

    if (FS_LoadOverlayInfo(&info, param_1, overlayID) == FALSE || FS_LoadOverlayFile(&info) == FALSE) {
        return FALSE;
    }

    FS_StartOverlay(&info);
    return TRUE;
}

BOOL FS_UnloadOverlay(u32 param_1, u32 overlayID) {
    OverlayInfo info;

    if (FS_LoadOverlayInfo(&info, param_1, overlayID) == FALSE || FS_StopOverlay(&info) == FALSE) {
        return FALSE;
    }
    return TRUE;
}
