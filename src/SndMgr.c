#include "SndMgr.h"
#include "CriSndMgr.h"
#include "Memory.h"
#include "common_data.h"

SndMgr sndMgr = {};

typedef struct UnkStruct_usedby_020310e8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8  volume;
} UnkStruct_usedby_020310e8;
extern UnkStruct_usedby_020310e8* func_020310e8(s16);

typedef struct {
    /* 0x0 */ s16 unk_00;
    /* 0x2 */ u8  volume;
} UnkStruct_usedby_02030f78;
extern UnkStruct_usedby_02030f78* func_02030f78(u32 param_1, u32 param_2); // External function to lookup sound data in archive

static void SndMgr_ClearSeqArcTracking(void) {
    sndMgr.trackedSeqArcsCount = 0;
    for (u32 i = 0; i < 3; i++) {
        sndMgr.trackedSeqArcs[i] = -1;
    }
}

static BOOL SndMgr_IsSeqArcAlreadyTracked(s16 param_1) {
    if ((param_1 < 7) || (param_1 > 37)) {
        return TRUE;
    }

    for (u32 i = 0; i < 3; i++) {
        if (param_1 == sndMgr.trackedSeqArcs[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void SndMgr_Init(void) {
    SndMgr_SetPriorityLoadFlag(0);
    void* heap = Mem_AllocHeapTail(&gMainHeap, 0x40000);
    Mem_SetSequence(&gMainHeap, heap, "SndMgr");
    MI_CpuFill(0, heap, Mem_GetBlockSize(&gMainHeap, heap));
    sndMgr.heapPointer = heap;
    SndMgr_ClearSeqArcTracking();
    func_0202f8a0();
    sndMgr.heapHandle = snd_initHeapHandle(sndMgr.heapPointer, 0x40000);
    snd_initSdatFile(&sndMgr.sdatInfo, "/Sound/sound_data.sdat", sndMgr.heapHandle, 0);
    func_02031f34(sndMgr.heapHandle);
    func_020316d0(0, sndMgr.heapHandle);
    func_020316fc(0, sndMgr.heapHandle);

    for (s32 i = 0; i < 5; i++) {
        sndMgr.playbackSlots[i].index = -1; // uninitialized
        func_0202fc84(&sndMgr.playbackSlots[i].handle);
    }

    SndMgr_InitStreamHandle();
    func_02031510(sndMgr.heapHandle);
    SndMgr_LoadSEVolumes();
    CriSndMgr_Init();
}

void SndMgr_Finalize(void) {
    func_02027330();
    func_0202f928();
}

s32 SndMgr_StopPlaybackForTrack(s32 idx) { // Stops playback of one of the 5 playback tracks.
    s32 sVar1 = sndMgr.playbackSlots[idx].index;
    func_0202fb80(&sndMgr.playbackSlots[idx].handle, 1);
    sndMgr.playbackSlots[idx].index = -1;
    return sVar1;
}

void SndMgr_StartPlayingSeqWithCustomSeqArc(s32 currentSeIdx, s32 seqArc, s32 se) {
    SndMgr_LoadSeqArc(g_SequenceDataTable[se].seqArc);
    func_02031fd0(&sndMgr.playbackSlots[currentSeIdx].handle, seqArc, se);
}

void SndMgr_StartPlayingSE(s32 seIdx) {
    s32 seqArc = g_SequenceDataTable[seIdx].seqArc;
    if (seqArc < 0) {
        SndMgr_PlayStreamSequence(seIdx);
        return;
    }

    SndMgr_LoadSeqArc(seqArc);
    s32** psVar2 = &sndMgr.playbackSlots[g_SequenceDataTable[seIdx].playbackSlot].handle;
    func_02031fd0(psVar2, seqArc, g_SequenceDataTable[seIdx].se);
    func_0202fd24(psVar2, sndMgr.seIdxVolume[seIdx]);
}

void SndMgr_StopPlayingSE(s32 seIdx) {
    if (g_SequenceDataTable[seIdx].seqArc < 0) {
        SndMgr_StopStreamPlayback(0);
    } else {
        func_0202fb90(g_SequenceDataTable[seIdx].seqArc, g_SequenceDataTable[seIdx].se, 0);
    }
}

static s32 SndMgr_GetDefaultSEVolume(s16 seqID) {
    return func_020310e8(seqID)->volume;
}

static u8 SndMgr_GetSeIdxVolumeFromFile(u32 seqArc, u32 seqID) {
    UnkStruct_usedby_02030f78* iVar2 = func_02030f78(seqArc, seqID);
    if (iVar2 == NULL) {
        return 0;
    }
    return iVar2->volume;
}

void SndMgr_LoadSEVolumes(void) {
    u32 uVar2 = -1;
    s8  seqArc;

    for (s32 seIdx = 0; seIdx < 0x56C; seIdx++) {
        seqArc = g_SequenceDataTable[seIdx].seqArc;
        if (seqArc < 0) {
            sndMgr.seIdxVolume[seIdx] = SndMgr_GetDefaultSEVolume(g_SequenceDataTable[seIdx].se);
        } else {
            if (uVar2 != seqArc) {
                uVar2 = seqArc;
                func_0203155c(sndMgr.heapHandle, 1);
                SndMgr_LoadSeqArc(seqArc);
            }
            sndMgr.seIdxVolume[seIdx] = SndMgr_GetSeIdxVolumeFromFile(seqArc, g_SequenceDataTable[seIdx].se);
        }
    }
    func_0203155c(sndMgr.heapHandle, 1);
}

s32 SndMgr_GetSeIdxVolume(s32 seIdx) {
    return sndMgr.seIdxVolume[seIdx];
}

void SndMgr_SetSeIdxVolume(s32 seIdx, s32 seIdxVolume) {
    sndMgr.seIdxVolume[seIdx] = seIdxVolume;
    if (g_SequenceDataTable[seIdx].seqArc >= 0) {
        func_0202fd24(&sndMgr.playbackSlots[g_SequenceDataTable[seIdx].playbackSlot].handle, seIdxVolume);
    }
}

void SndMgr_UpdateSEPan(s32 seIdx, s32 sePan) {
    u32 uVar2 = g_SequenceDataTable[seIdx].playbackSlot;

    if ((uVar2 + 0xfe & 0xff) <= 1) {
        func_0202fd78(&sndMgr.playbackSlots[uVar2].handle, 0xffff, SndMgr_ConvertPanToSigned(sePan));
    } else if (uVar2 == 6) {
        func_020323fc(&sndMgr.streamHandle, 0, SndMgr_ConvertPanToVolume(sePan));
    }
}

void SndMgr_PlaySEWithPan(s32 seIdx, s32 sePan) {
    SndMgr_StartPlayingSE(seIdx);
    SndMgr_UpdateSEPan(seIdx, sePan);
}

s32 SndMgr_ConvertPanToSigned(s32 sePan) {
    if (sePan < 0) {
        return -0x80;
    }
    if (sePan >= 0x100) {
        return 0x7f;
    }
    return sePan - 0x80;
}

s32 SndMgr_ConvertPanToVolume(s32 sePan) {
    if (sePan < 0) {
        return 0;
    }
    if (sePan >= 0x100) {
        return 0x7f;
    }
    return sePan / 2;
}

void SndMgr_InitStreamHandle(void) {
    func_02032114(10, sndMgr.heapHandle);
    func_02032418(&sndMgr.streamHandle);
}

BOOL SndMgr_PlayStreamSequence(s32 seIdx) {
    return func_0203235c(&sndMgr.streamHandle, g_SequenceDataTable[seIdx].se, 0);
}

BOOL SndMgr_PlayStreamSequenceWithSeqID(s32 se) {
    return func_0203235c(&sndMgr.streamHandle, se, 0);
}

void SndMgr_StopStreamPlayback(s32 param_1) {
    func_02032384(param_1);
}

void func_02026e50(s32 param_1) {
    func_020323c8(&sndMgr.streamHandle, param_1, 0);
    func_020323c8(&sndMgr.streamHandle, param_1, 0);
}

s32 SndMgr_GetBankForSeqArc(s32 seqArc) {
    switch (seqArc) {
        case 0:
            return 0;
        case 2:
            return 2;
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
            return 1;
        case 7:
            return 3;
        case 8:
            return 4;
        case 9:
            return 5;
        case 10:
            return 6;
        case 11:
            return 7;
        case 12:
            return 8;
        case 13:
            return 9;
        case 14:
            return 10;
        case 15:
            return 11;
        case 16:
            return 12;
        case 17:
            return 13;
        case 18:
            return 14;
        case 19:
            return 15;
        case 20:
            return 16;
        case 21:
            return 17;
        case 22:
            return 18;
        case 23:
            return 19;
        case 24:
            return 20;
        case 25:
            return 21;
        case 26:
            return 22;
        case 27:
            return 23;
        case 28:
            return 24;
        case 29:
            return 25;
        case 30:
            return 26;
        case 31:
            return 27;
        case 32:
            return 28;
        case 33:
            return 29;
        case 34:
            return 30;
        case 35:
            return 31;
        case 36:
            return 32;
        case 38:
            return 34;
        case 37:
        default:
            return -1;
    }
}

u32 SndMgr_LoadSeqArc(s32 seqArc) {
    SndMgr_IsSeqArcAlreadyTracked(seqArc); // Why is this call here? It doesn't do anything with the return value.

    s32* seqArcMetadata = func_02030fbc(seqArc);
    s32  uVar3          = func_020312fc(*seqArcMetadata);

    if (uVar3 != 0) {
        return uVar3;
    }
    func_020316d0(seqArc, sndMgr.heapHandle);

    uVar3 = SndMgr_GetBankForSeqArc(seqArc);
    if (uVar3 >= 0) {
        if ((uVar3 >= 3) && (sndMgr.priorityFlag != FALSE)) {
            uVar3 = func_02031728(uVar3, 2, sndMgr.heapHandle);
        } else {
            uVar3 = func_020316fc(uVar3, sndMgr.heapHandle);
        }
    }
    if (seqArc != 1) {
        return uVar3;
    }

    return func_02031510(sndMgr.heapHandle);
}

void SndMgr_ResetSequenceHeap(void) {
    func_0203155c(sndMgr.heapHandle, 1);
    SndMgr_ClearSeqArcTracking();
}

void SndMgr_DeepResetSequenceHeap(void) {
    func_0203155c(sndMgr.heapHandle, 2);
    SndMgr_ClearSeqArcTracking();
}

BOOL SndMgr_IsSEPlaying(s32 seIdx) {
    BOOL result;

    if (g_SequenceDataTable[seIdx].seqArc < 0) {
        result = func_0203243c(&sndMgr.streamHandle);
    } else {
        result = func_0202fcb0(g_SequenceDataTable[seIdx].seqArc, g_SequenceDataTable[seIdx].se);
    }

    if (result == 0) {
        return FALSE;
    }
    return TRUE;
}

void func_02027170(s32 param_1, s32 param_2) {
    if (g_SequenceDataTable[param_1].seqArc < 0) {
        func_02026e50(param_2);
    } else {
        func_0202fd10(&sndMgr.playbackSlots[g_SequenceDataTable[param_1].playbackSlot].handle, param_2);
    }
}

void SndMgr_SetSequenceSoundPitch(s32 seIdx, s32 sePitch) {
    if (g_SequenceDataTable[seIdx].seqArc >= 0) {
        func_0202fd58(&sndMgr.playbackSlots[g_SequenceDataTable[seIdx].playbackSlot].handle, 0xFFFF, sePitch);
    }
}

void func_02027200(u32 param_1) {
    func_0202fc34(param_1);
    if (param_1 != 0) {
        SndMgr_StopStreamPlayback(0);
    }
}

void SndMgr_SetPriorityLoadFlag(s32 flag) {
    sndMgr.priorityFlag = flag;
}

void SndMgr_TrackSeqArc(s32 seqArc) {
    for (u32 i = 0; i < 3; i++) {
        if (sndMgr.trackedSeqArcs[i] == seqArc) {
            return;
        }
        if (sndMgr.trackedSeqArcs[i] < 0) {
            sndMgr.trackedSeqArcs[i] = seqArc;
            sndMgr.trackedSeqArcsCount++;
            return;
        }
    }
}
