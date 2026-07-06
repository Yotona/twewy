#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Save.h"
#include "Save/FriendData.h"
#include "Save/MainData.h"
#include "nitro/mi/cpumem.h"
#include <nitro/types.h>

extern void func_020389c0(u16);
extern s64  func_0203a444(void);
extern s32  func_02042330(void);
extern void func_0204237c(u16);
extern void func_0204238c(u16);
extern u8   func_02042944(void);
extern void func_0204296c(s32);
extern void func_02042aa4(void);
extern s32  func_02042ab0(void);

void        Savefile_InitNewGameDefaults(void);
static BOOL SaveIO_HasValidFooter(u8* buffer, const u8* magic, s32 magicLength, s32 arg3);
static u16  SaveIO_GetChecksum16(void*, s16);

extern s32 data_0205c23c;
extern u16 data_0205c5be[];

/**
 * Magic string used to validate the save file footer.
 */
const u8 SaveFooterSignature[27] = "SubarAsikiKonosEkai070613b";

/**
 * @param size Size of the buffer to allocate, in bytes.
 * @returns Pointer to allocated temporary backup data buffer, aligned to 8 bytes.
 */
static void* SaveIO_AllocTempAlignedBuffer(s32 size) {
    void* backupData = Mem_AllocHeapTail(&gMainHeap, (size + 7) & ~7);
    Mem_SetSequence(&gMainHeap, backupData, "BackUpData_Tmp");
    return backupData;
}

/**
 * @param buffer Pointer to the temporary backup data buffer to free.
 */
static void SaveIO_FreeTempBuffer(void* buffer) {
    Mem_Free(&gMainHeap, buffer);
}

static s32 Savefile_ValidMainSlotAtOffset(u16 offset) {
    s32 result;

    MainSaveImage* image = SaveIO_AllocTempAlignedBuffer(sizeof(MainSaveImage));

    func_0204285c(offset, image, sizeof(MainSaveImage), 0, 0, 1, 6, 1, 0);
    func_02042aa4();
    func_02042330();
    if (SaveIO_HasValidFooter(image->footer.magic, SaveFooterSignature, sizeof(SaveFooterSignature), sizeof(MainData))) {
        if (SaveIO_GetChecksum16(image, sizeof(MainSaveImage)) != 0) {
            result = 8;
        } else {
            result                     = 0;
            gSaveState.unk_20.unk_1AB6 = image->mainData.unk_1AB6;
        }
    } else {
        result = 1;
    }
    Mem_Free(&gMainHeap, image);
    return result;
}

s32 Savefile_ValidateMainSlots(void) {
    s32 result;

    gSaveState.lockID = OS_GetLockID();
    if (gSaveState.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(gSaveState.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        result = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        s32 slot0Status = Savefile_ValidMainSlotAtOffset(0);
        if (slot0Status != 0) {

            s32 slot1Status = Savefile_ValidMainSlotAtOffset(0x5470);
            if (slot1Status != 0) {
                if ((slot0Status == 1) && (slot1Status == 1)) {
                    result = 1;
                } else if ((slot0Status == 2) || (slot1Status == 2)) {
                    result = 2;
                } else {
                    result = 8;
                }
            } else {
                result = 0;
            }
        } else {
            result = 0;
        }
    } else {
        result = 2;
    }

    func_0204238c(gSaveState.lockID);
    func_020389c0(gSaveState.lockID);
    return result;
}

s32 Savefile_ValidateFriendSlotAtOffset(s32 arg0) {
    FriendSaveImage* friendImage = SaveIO_AllocTempAlignedBuffer(sizeof(FriendSaveImage));

    func_0204285c(arg0, friendImage, sizeof(FriendSaveImage), 0, 0, 1, 6, 1, 0);
    func_02042aa4();
    func_02042330();
    s32 result;
    if (SaveIO_HasValidFooter(friendImage->footer.magic, SaveFooterSignature, sizeof(SaveFooterSignature),
                              sizeof(GlobalFriendData)))
    {
        if (SaveIO_GetChecksum16(friendImage, sizeof(FriendSaveImage)) == 0) {
            result = 0;
        } else {
            result = 8;
        }
    } else {
        result = 1;
    }
    Mem_Free(&gMainHeap, friendImage);
    return result;
}

s32 Savefile_ValidateFriendSlots(void) {
    s32 result;

    gSaveState.lockID = OS_GetLockID();
    if (gSaveState.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(gSaveState.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        result = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        s32 slot1Status = Savefile_ValidateFriendSlotAtOffset(sizeof(MainSaveImage));
        if (slot1Status != 0) {
            s32 slot2Status = Savefile_ValidateFriendSlotAtOffset(0x88B4);
            if (slot2Status != 0) {
                if ((slot1Status == 1) && (slot2Status == 1)) {
                    result = 1;
                } else if ((slot1Status == 2) || (slot2Status == 2)) {
                    result = 2;
                } else {
                    result = 8;
                }
            } else if (!(gSaveState.unk_20.unk_1AB6 & 1)) {
                result = 8;
            } else {
                result = 0;
            }
        } else {
            result = 0;
        }
    } else {
        result = 2;
    }
    func_0204238c(gSaveState.lockID);
    func_020389c0(gSaveState.lockID);
    return result;
}

s32 Savefile_ValidateAllSlots(void) {
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 0;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 0;

    s32 validation = 0;
    validation |= Savefile_ValidateMainSlots();
    validation |= Savefile_ValidateFriendSlots();

    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;

    return validation;
}

static s32 FriendData_InitAllEntriesEmpty(GlobalFriendData* friendData) {
    u16 i, j;
    for (i = 0; i < 50; i++) {
        for (j = 0; j < 6; j++) {
            friendData->unk_0000[i].unk_00[j] = -1;
        }

        friendData->unk_0000[i].unk_6A = -1;

        for (j = 0; j < 6; j++) {
            friendData->unk_0000[i].unk_6C[j] = 0xFFFF;
        }

        for (j = 0; j < 16; j++) {
            friendData->unk_0000[i].unk_78[j] = 0xFFFF;
        }
    }

    for (i = 0; i < 50; i++) {
        for (j = 0; j < 6; j++) {
            friendData->unk_1DB0[i].unk_0[j] = 0xFF;
        }
    }
    return 0;
}

void Savefile_InitNewGameMainData(void) {
    Savefile_InitNewGameDefaults();
    gSaveState.unk_20.unk_1AB6 &= ~1;
}

static void FriendData_Alloc(void) {
    GlobalFriendData* globalFriendData = Mem_AllocHeapTail(&gMainHeap, sizeof(GlobalFriendData));
    Mem_SetSequence(&gMainHeap, globalFriendData, "GlobalFriendData");
    MI_CpuFill(0, globalFriendData, Mem_GetBlockSize(&gMainHeap, globalFriendData));
    gSaveState.globalFriendData = globalFriendData;
    FriendData_InitAllEntriesEmpty(globalFriendData);
}

static void FriendData_Free(void) {
    Mem_Free(&gMainHeap, gSaveState.globalFriendData);
}

s32 SavePipeline_PrepareNewGame(void) {
    Savefile_InitNewGameMainData();
    FriendData_Alloc();
    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_FreeFriendData(void) {
    FriendData_Free();
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = TRUE;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = TRUE;
    return 1;
}

// Nonmatching
void Savefile_ResetIOPipeline(void) {
    gSaveState.savePipelineStepIndex = 0;
    gSaveState.saveIoErrorFlags      = 0;
    gSaveState.unk_08                = 0;
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 0;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 0;
}

s32 Savefile_BuildMainSaveImage(void) {
    MI_CpuSet(gSaveState.mainImage, 0, sizeof(MainSaveImage));

    MainSaveImage* image = gSaveState.mainImage;
    for (u16 i = 0; i < 27; ++i) {
        image->footer.magic[i] = SaveFooterSignature[i];
    }

    for (u16 i = 0; i < 4; ++i) {
        image->footer.nibbles[i] = (u8)((s8)((u32)(0x3420 & (0xF << (i * 4))) >> (i * 4)));
    }

    image->mainData = gSaveData;
    return 0;
}

s32 Savefile_BuildFriendSaveImage(GlobalFriendData* friendData) {
    MI_CpuSet(gSaveState.friendImage, 0, sizeof(FriendSaveImage));

    FriendSaveImage* image = gSaveState.friendImage;

    for (u16 i = 0; i < 27; ++i) {
        image->footer.magic[i] = SaveFooterSignature[i];
    }

    for (u16 i = 0; i < 4; ++i) {
        image->footer.nibbles[i] = (u8)((s8)((u32)(0x2008 & (0xF << (i * 4))) >> (i * 4)));
    }

    image->friendData = *friendData;

    return 0;
}

void Savefile_ReleaseLockAndFree(void* buffer) {
    func_0204238c(gSaveState.lockID);
    func_020389c0(gSaveState.lockID);
    SaveIO_FreeTempBuffer(buffer);
}

static s32 SavePipeline_PrepareMainImageWrite(void) {
    MainSaveImage* image = SaveIO_AllocTempAlignedBuffer(sizeof(MainSaveImage));

    gSaveState.mainImage = image;
    Savefile_BuildMainSaveImage();

    image->footer.checksum = 0;
    image->footer.checksum = SaveIO_GetChecksum16(image, sizeof(MainSaveImage));

    gSaveState.lockID = OS_GetLockID();
    if (gSaveState.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(gSaveState.lockID);

    func_0204296c(0x1001);

    if (func_02042330() != 0) {
        gSaveState.saveIoErrorFlags = 4;
        Savefile_ReleaseLockAndFree(gSaveState.mainImage);
        return 1;
    }

    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_StartPrimaryMainImageWrite(void) {
    MainSaveImage* image = gSaveState.mainImage;

    gSaveState.unk_08 = 0;
    if ((func_02042944() & 0xFF) == 1) {
        gSaveState.unk_18 = func_0203a444();
        func_0204285c(image, 0, sizeof(MainSaveImage), 0, 0, 1, 8, 10, 2);
    } else {
        OS_WaitForever();
    }
    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_StartBackupMainImageWrite(void) {
    MainSaveImage* image = gSaveState.mainImage;

    gSaveState.unk_08 = 0;
    if ((func_02042944() & 0xFF) == 1) {
        func_0204285c(image, 0x5470, sizeof(MainSaveImage), 0, 0, 1, 8, 10, 2);
    } else {
        OS_WaitForever();
    }
    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_PollMainImageWriteCompletion(void) {
    if (func_02042ab0() != 0) {
        if (func_02042330() != 0) {
            gSaveState.saveIoErrorFlags |= 4;
        } else {
            gSaveState.saveIoErrorFlags = 0;
        }
        gSaveState.savePipelineStepIndex++;
    }
    return 0;
}

s32 SavePipeline_FinalizeMainImageWrite(void) {
    Savefile_ReleaseLockAndFree(gSaveState.mainImage);
    gSaveState.unk_18 = func_0203a444() - gSaveState.unk_18;
    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_FinalizeFriendImageWrite(void) {
    Savefile_ReleaseLockAndFree(gSaveState.friendImage);
    gSaveState.unk_18 = func_0203a444() - gSaveState.unk_18;
    gSaveState.savePipelineStepIndex++;
    return 0;
}

void FriendData_Set(GlobalFriendData* globalFriendData) {
    gSaveState.globalFriendData = globalFriendData;
}

s32 SavePipeline_PrepareFriendImageWrite(void) {
    s32 ioStatus;

    FriendSaveImage* image = SaveIO_AllocTempAlignedBuffer(sizeof(FriendSaveImage));
    gSaveState.friendImage = image;

    Savefile_BuildFriendSaveImage(gSaveState.globalFriendData);

    image->footer.checksum = 0;
    image->footer.checksum = SaveIO_GetChecksum16(image, sizeof(FriendSaveImage));

    gSaveState.lockID = OS_GetLockID();
    if (gSaveState.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(gSaveState.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        gSaveState.saveIoErrorFlags = 4;
        Savefile_ReleaseLockAndFree(gSaveState.friendImage);
        return 1;
    }
    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_StartPrimaryFriendImageWrite(void) {
    FriendSaveImage* image = gSaveState.friendImage;

    gSaveState.unk_08 = 0;
    if ((func_02042944() & 0xFF) == 1) {
        gSaveState.unk_18 = func_0203a444();
        func_0204285c(image, 0x3444, sizeof(FriendSaveImage), 0, 0, 1, 8, 10, 2);
    } else {
        OS_WaitForever();
    }

    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_StartBackupFriendImageWrite(void) {
    FriendSaveImage* image = gSaveState.friendImage;

    gSaveState.unk_08 = 0;
    if ((func_02042944() & 0xFF) == 1) {
        func_0204285c(image, 0x88B4, sizeof(FriendSaveImage), 0, 0, 1, 8, 0xA, 2);
    } else {
        OS_WaitForever();
    }

    gSaveState.savePipelineStepIndex++;
    return 0;
}

s32 SavePipeline_PollFriendImageWriteCompletion(void) {
    if (func_02042ab0() != 0) {
        if (func_02042330() != 0) {
            gSaveState.saveIoErrorFlags = 4;
        } else {
            gSaveState.saveIoErrorFlags = 0;
        }
        gSaveState.savePipelineStepIndex++;
    }
    return 0;
}

s32 func_02025414(void) {
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    return 1;
}

typedef s32 (*SavePipelineFunc)(void);

s32 Savefile_RunSavePipelineStep(void) {
    const SavePipelineFunc steps[14] = {
        SavePipeline_PrepareNewGame,
        SavePipeline_PrepareMainImageWrite,
        SavePipeline_StartPrimaryMainImageWrite,
        SavePipeline_PollMainImageWriteCompletion,
        SavePipeline_StartBackupMainImageWrite,
        SavePipeline_PollMainImageWriteCompletion,
        SavePipeline_FinalizeMainImageWrite,
        SavePipeline_PrepareFriendImageWrite,
        SavePipeline_StartPrimaryFriendImageWrite,
        SavePipeline_StartBackupFriendImageWrite,
        SavePipeline_PollFriendImageWriteCompletion,
        SavePipeline_FinalizeFriendImageWrite,
        SavePipeline_FreeFriendData,
    };
    return steps[gSaveState.savePipelineStepIndex]();
}

s32 Savefile_RunAlternatePipelineStep(void) {
    const SavePipelineFunc steps[13] = {
        SavePipeline_PrepareMainImageWrite,
        SavePipeline_StartPrimaryMainImageWrite,
        SavePipeline_PollMainImageWriteCompletion,
        SavePipeline_StartBackupMainImageWrite,
        SavePipeline_PollMainImageWriteCompletion,
        SavePipeline_FinalizeMainImageWrite,
        SavePipeline_PrepareFriendImageWrite,
        SavePipeline_StartPrimaryFriendImageWrite,
        SavePipeline_PollFriendImageWriteCompletion,
        SavePipeline_StartBackupFriendImageWrite,
        SavePipeline_PollFriendImageWriteCompletion,
        SavePipeline_FinalizeFriendImageWrite,
        func_02025414,
    };
    return steps[gSaveState.savePipelineStepIndex]();
}

s32 Savefile_TryLoadMainImage(s32 saveSlot) {
    func_0204285c(saveSlot, gSaveState.mainImage, sizeof(MainSaveImage), 0, 0, 1, 6, 1, 0);
    func_02042aa4();
    func_02042330();

    MainSaveImage* image = gSaveState.mainImage;

    if (SaveIO_HasValidFooter(image->footer.magic, SaveFooterSignature, sizeof(SaveFooterSignature), sizeof(MainData))) {
        if (SaveIO_GetChecksum16(image, sizeof(MainSaveImage)) == 0) {
            gSaveData = image->mainData;
            return 0;
        }
        return 8;
    }
    return 1;
}

s32 Savefile_LoadMainImage(void) {
    s32 slot0Status;
    s32 slot1Status;
    s32 loadResult;

    gSaveState.mainImage = SaveIO_AllocTempAlignedBuffer(sizeof(MainSaveImage));

    gSaveState.lockID = OS_GetLockID();
    if (gSaveState.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(gSaveState.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        loadResult = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        slot0Status = Savefile_TryLoadMainImage(0);
        if (slot0Status != 0) {
            slot1Status = Savefile_TryLoadMainImage(0x5470);
            if (slot1Status != 0) {
                if ((slot0Status == 2) || (slot1Status == 2)) {
                    loadResult = 2;
                } else {
                    loadResult = 8;
                }
            } else {
                loadResult = 0;
            }
        } else {
            loadResult = 0;
        }
    } else {
        loadResult = 2;
    }
    func_0204238c(gSaveState.lockID);
    func_020389c0(gSaveState.lockID);
    SaveIO_FreeTempBuffer(gSaveState.mainImage);
    return loadResult;
}

s32 Savefile_Load(void) {
    return Savefile_LoadMainImage();
}

s32 Savefile_TryLoadFriendImage(GlobalFriendData* outputBuffer, s32 saveSlot) {
    func_0204285c(saveSlot, gSaveState.friendImage, sizeof(FriendSaveImage), 0, 0, 1, 6, 1, 0);
    func_02042aa4();
    func_02042330();

    FriendSaveImage* image = gSaveState.friendImage;
    if (SaveIO_HasValidFooter(image->footer.magic, SaveFooterSignature, sizeof(SaveFooterSignature), sizeof(GlobalFriendData)))
    {
        if (SaveIO_GetChecksum16(image, sizeof(FriendSaveImage)) == 0) {
            *outputBuffer = image->friendData;
            return 0;
        }
        return 8;
    }
    return 1;
}

s32 Savefile_LoadFriendImage(GlobalFriendData* outputBuffer) {
    s32 loadResult;

    gSaveState.friendImage = SaveIO_AllocTempAlignedBuffer(sizeof(FriendSaveImage));

    gSaveState.lockID = OS_GetLockID();
    if (gSaveState.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(gSaveState.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        loadResult = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        s32 slot1Status = Savefile_TryLoadFriendImage(outputBuffer, sizeof(MainSaveImage));
        if (slot1Status != 0) {
            s32 slot2Status = Savefile_TryLoadFriendImage(outputBuffer, 0x88B4);
            if (slot2Status != 0) {
                if ((slot1Status == 2) || (slot2Status == 2)) {
                    loadResult = 2;
                } else {
                    loadResult = 8;
                }
            } else {
                loadResult = 0;
            }
        } else {
            loadResult = 0;
        }
    } else {
        loadResult = 2;
    }
    func_0204238c(gSaveState.lockID);
    func_020389c0(gSaveState.lockID);
    SaveIO_FreeTempBuffer(gSaveState.friendImage);
    return loadResult;
}

u16 Savefile_GetWriteErrorFlags(void) {
    return gSaveState.saveIoErrorFlags;
}

static BOOL SaveIO_HasValidFooter(u8* buffer, const u8* magic, s32 magicLength, s32 expectedSize) {
    // Verify checksums match
    for (u16 i = 0; i < magicLength; i++) {
        if (buffer[i] != magic[i]) {
            return FALSE;
        }
    }

    // Verify nibble flags match
    u32 mask = 0xF;

    for (u16 i = 0; i < 4; i++) {
        u32 shift    = i * 4;
        u32 expected = expectedSize & (mask << shift);
        u8  actual   = buffer[i + 27];

        expected >>= shift;
        if ((expected & 0xFF) != actual) {
            return FALSE;
        }
    }

    return TRUE;
}

/**
 * @brief Calculates the 16-bit ones-complement checksum for the given data.
 * @param data Pointer to the data
 * @param size Size of the data in bytes
 * @return 16-bit checksum
 */
static u16 SaveIO_GetChecksum16(void* data, s16 size) {
    u32  checksum = 0;
    u16* ptr      = (u16*)data;

    while (size > 1) {
        checksum += *ptr;
        ptr++;
        size -= 2;
    }

    if (size > 0) {
        checksum += (u8)*ptr;
    }

    checksum = (checksum & 0xFFFF) + (checksum >> 16);
    checksum = (checksum & 0xFFFF) + (checksum >> 16);

    return ~checksum;
}

s32 func_0202599c(s32 flagIndex) {
    return (gSaveState.unk_20.unk_246C & (1LL << flagIndex)) != 0;
}

// Nonmatching
s32 func_020259e4(s32 itemId) {
    s32  addressOffset = (itemId + ((u32)(itemId >> 4) >> 0x1B)) >> 5;
    u32* flagPtr       = &gSaveData.unk_2474;

    if (addressOffset > 1) {
        flagPtr = &gSaveData.unk_247C;
        addressOffset -= 2;
    }
    {
        s32 mask = 1 << (itemId % 32);
        if (addressOffset != 0) {
            flagPtr += addressOffset;
        }
        return (*flagPtr) & mask;
    }
}

s32 func_02025a2c(s32 playTime) {
    s16 unlockedCount = 0;
    for (unsigned long itemIndex = 0; itemIndex < 67; itemIndex++) {
        u16 flags = data_0205c5be[(itemIndex * 7) + 6];
        if ((flags & 2) || ((flags & 1) && (Inventory_HasRequiredQuantity(ITEM_STICKER_GAME_CLEARED, 1, 0) == 0))) {
            continue;
        }
        if ((*((s16*)((data_0205c5be + (itemIndex * 7)) + 0))) <= playTime) {
            do {
            } while (0);
            if (playTime <= (*((s16*)((data_0205c5be + (7 * itemIndex)) + 1)))) {
                if (func_020259e4(itemIndex) == 0) {
                    unlockedCount += 1;
                }
            }
        }
    }

    return unlockedCount;
}

void func_02025ac4(MainData* arg0) {
    arg0->unk_246C = 0;
    arg0->unk_2474 = 0;
    arg0->unk_2478 = 0;
    arg0->unk_247C = 0;
    arg0->unk_2480 = 0;
    arg0->unk_3106 = 0;
    arg0->unk_3108 = -1;
    arg0->unk_3112 = 0;
    arg0->unk_3114 = 0;
    arg0->unk_3116 = 0;
    arg0->unk_3118 = 0;
    arg0->unk_311A = 0;
    arg0->unk_311C = 0;
    arg0->unk_3124 = -1;
    arg0->unk_312C = 1;
}

void Savefile_InitNewGameDefaults(void) {
    gSaveState.unk_20.unk_2484 = 1;
    gSaveState.unk_20.unk_3130 = 0;
    gSaveState.unk_20.unk_3238 = 127;
    gSaveState.unk_20.unk_3234 = 0;
    gSaveState.unk_20.unk_313C = 0;
    Savefile_ResetAllGameplay(&gSaveState.unk_20);
    func_02025ac4(&gSaveState.unk_20);
}