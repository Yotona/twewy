#ifndef SNDMGR_H
#define SNDMGR_H

#include <types.h>

extern struct {
    /* 0x0 */ s8  seqArc;       // Sequence Archive Index
    /* 0x1 */ u8  playbackSlot; // Playback slot index the Sequence uses.
    /* 0x2 */ s16 se;           // Sequence ID in the .sdat (or .SSAR?) file.
} g_SequenceDataTable[1388];

typedef struct SndMgr {
    /* 0x0000 */ void* heapPointer;       // Pointer to the sound heap memory buffer
    /* 0x0004 */ s32   heapHandle;        // Handle for managing the sound data heap
    /* 0x0008 */ char  unk_0008[0x5C];    // Padding or unknown internal field
    /* 0x0064 */ s32   streamHandle;      // Direct audio channel handle
    /* 0x0068 */ s32   seIdxVolume[1388]; // Cached volume levels for each sound effect index
    /* 0x1618 */ struct {
        /* 0x00 */ s32  index;
        /* 0x04 */ s32* handle;
    } playbackSlots[5];
    /* 0x1640 */ char sdatInfo[0x90];      // SDAT management information block
    /* 0x16D0 */ s32  priorityFlag;        // Priority flag for sound resource loading
    /* 0x16D4 */ s16  trackedSeqArcs[3];   // Tracked sequence archive indices
    /* 0x16DA */ s8   trackedSeqArcsCount; // Count of tracked sequence archives
} SndMgr;

void SndMgr_Init(void);

void SndMgr_Finalize(void);

s32 SndMgr_StopPlaybackForTrack(s32 idx);

void SndMgr_StartPlayingSeqWithCustomSeqArc(s32 param_1, s32 seqArc, s32 se);

void SndMgr_StartPlayingSE(s32 seIdx);

void SndMgr_StopPlayingSE(s32 seIdx);

void SndMgr_LoadSEVolumes(void);

s32 SndMgr_GetSeIdxVolume(s32 seIdx);

void SndMgr_SetSeIdxVolume(s32 seIdx, s32 seIdxVolume);

void SndMgr_UpdateSEPan(s32 seIdx, s32 sePan);

void SndMgr_InitStreamHandle(void);

BOOL SndMgr_PlayStreamSequence(s32);

BOOL SndMgr_PlayStreamSequenceWithSeqID(s32 se);

void SndMgr_StopStreamPlayback(s32);

/**
 * @brief Converts a pan value to a signed 8-bit range.
 *
 * - If sePan is less than 0, returns -0x80 (-128).
 * - If sePan is greater than or equal to 0x100 (256), returns 0x7f (127).
 * - Otherwise, returns sePan offset by -0x80.
 *
 * @param sePan The input pan value to convert.
 * @return The signed 8-bit pan value in the range [-128, 127].
 */
s32 SndMgr_ConvertPanToSigned(s32 sePan);

/**
 * @brief Converts a pan value to a corresponding appropriate volume adjustment for sound playback.
 *
 * @param sePan The pan value to convert.
 * @return The calculated volume value corresponding to the given pan.
 */
s32 SndMgr_ConvertPanToVolume(s32 sePan);

u32 SndMgr_LoadSeqArc(s32 seqArc);

void SndMgr_ResetSequenceHeap(void);

BOOL SndMgr_IsSEPlaying(s32 seIdx);

void SndMgr_SetSequenceSoundPitch(s32 seIdx, s32 sePitch);

void func_02027200(u32 param_1);

void SndMgr_SetPriorityLoadFlag(s32 param_1);

#endif // SNDMGR_H