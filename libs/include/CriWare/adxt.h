#ifndef ADXT_H
#define ADXT_H

#include <CriWare/adxsjd.h>
#include <CriWare/adxstm.h>
#include <CriWare/sj.h>
#include <types.h>

/* Status Codes */
#define ADXT_STAT_STOPPED  0 // Standby
#define ADXT_STAT_LOADING  1 // Retrieving header information
#define ADXT_STAT_PREPPING 2 // Preparing to play
#define ADXT_STAT_PLAYING  3 // Decoding/Playing
#define ADXT_STAT_DECEND   4 // Decoding ended
#define ADXT_STAT_PLAYEND  5 // Playback ended
#define ADXT_STAT_ERROR    6 // Error occurred

/* Error Codes */
#define ADXT_ERR_NONE 0 // No error
#define ADXT_ERR_BUFF -1 // Buffer is empty
#define ADXT_ERR_MEM  -2 // Sound Block error

/* Playback Modes*/
#define ADXT_PLAYBACK_FILENAME 0 // Play the specified filename/id
#define ADXT_PLAYBACK_AFS      1 // Play the specified AFS file
#define ADXT_PLAYBACK_MEM      2 // Play data from memory
#define ADXT_PLAYBACK_STREAM   3 // Play data from a stream
#define ADXT_PLAYBACK_SLFILE   4 // Seamless continous play from a file

typedef void* ADXRNA;

typedef struct {
    /* 0x00 */ s8      used; // Whether ADXT is in use
    /* 0x01 */ s8      stat; // Operation status
    /* 0x02 */ s8      playbackMode; // Playback mode
    /* 0x03 */ s8      maxChannels; // Maximum number of channels
    /* 0x04 */ ADXSJD* streamDecoder;
    /* 0x08 */ ADXSTM* streamController;
    /* 0x0C */ ADXRNA  audioRenderer;
    /* 0x10 */ SJ*     fileInputStream;
    /* 0x14 */ SJ*     inputStream;
    /* 0x18 */ SJ*     outputStream[2];
    /* 0x20 */ s8*     inputBuf;
    /* 0x24 */ s32     inputBufLength;
    /* 0x28 */ s32     inputBufExtra;
    /* 0x2C */ s16*    outputBuf;
    /* 0x30 */ s32     outputBufSamples;
    /* 0x34 */ s32     outputBufInterval;
    /* 0x38 */ s32     serverCbFreq;
    /* 0x3C */ s16     inputBufMaxSectors;
    /* 0x3E */ s16     inputBufMinSectors;
    /* 0x40 */ s16     outVol;
    /* 0x42 */ s16     outPan[2];
    /* 0x46 */ s16     outBalance;
    /* 0x48 */ s32     maxDecodedSamples;
    /* 0x4C */ s32     loopCounter;
    /* 0x50 */ s32     loopSkip;
    /* 0x54 */ s32     transpose;
    /* 0x58 */ s32     writePos;
    /* 0x5C */ s32     mediaOffset;
    /* 0x60 */ s16     errorCode;
    /* 0x64 */ s32     decodePos;
    /* 0x68 */ s16     decodeCounter;
    /* 0x6A */ s16     inputBufEmptyCount;
    /* 0x6C */ s8      loopFlag;
    /* 0x6D */ s8      autoRecoverFlag;
    /* 0x6E */ s8      filterMode;
    /* 0x6F */ s8      execFlag;
    /* 0x70 */ s8      waitFlag;
    /* 0x71 */ s8      readyFlag;
    /* 0x72 */ s8      pauseFlag;
    /* 0x74 */ void*   amp;
    /* 0x78 */ SJ*     ampInputStream[2];
    /* 0x80 */ SJ*     ampOutputStream[2];
    /* 0x88 */ s32     timeOffset;
    /* 0x8C */ s32     loopPlaybackEndSect;
    /* 0x90 */ s32     trapSample;
    /* 0x94 */ void*   loopStreamController;
    /* 0x98 */ s8      playbackFlag;
    /* 0x99 */ s8      pad0;
    /* 0x9A */ s16     pad1;
    /* 0x9C */ u32     startTimeOffset;
    /* 0xA0 */ u32     vsyncCount;
    /* 0xA4 */ u32     decodeOffset;
    /* 0xA8 */ s8      streamStartFlag;
    /* 0xA9 */ s8      extraInfoFlag;
    /* 0xAA */ s16     pad2;
    /* 0xAC */ char*   workFilename;
    /* 0xB0 */ char*   filename;
    /* 0xB4 */ void*   directory;
    /* 0xB8 */ u32     offset;
    /* 0xBC */ u32     range;
    /* 0xC0 */ u32     loopDecodeLength;
} ADXT; // Size: 0xC4

ADXT* ADXT_Create(s32 maxChans, void* work, s32 worksize);

ADXT* func_02016c64(void* work, s32 workSize);

void ADXT_Destroy(ADXT* adxt);

void ADXT_DestroyAll(void);

void ADXT_Stop(ADXT* adxt);

s32 ADXT_GetStat(ADXT* adxt);

s32 ADXT_Play(ADXT* adxt);

s32 func_020174c8();

s32 ADXT_GetNumChan();

void ADXT_SetOutPan(ADXT* adxt, s32 channel, s32 pan);

void ADXT_SetOutVol(ADXT* adxt, s32 vol);

s32 ADXT_GetOutVol(ADXT* adxt);

void func_020177a4(ADXT* adxt);

void ADXT_SetLpFlg(ADXT* adxt, s32 flag);

// void ADXT_Pause(ADXT* adxt, s32 pauseState);

s32 ADXT_GetStatPause(ADXT* adxt);

void func_02017b78(ADXT* adxt, s32 param_1, s32 param_2);
void func_02017b7c(ADXT* adxt, s32 param_1, s32 param_2);
void func_02017ba4(ADXT* adxt, s32 param_1);

void ADXT_StartMem2(ADXT* adxt, void* adxData, s32 dataLength);

void func_020177b8();
void func_020177cc();

void func_02017d80(ADXT* adxt, const char* filename);

#endif // ADXT_H