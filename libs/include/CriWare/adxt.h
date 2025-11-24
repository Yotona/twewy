#ifndef ADXT_H
#define ADXT_H

#include <CriWare/adx_sjd.h>
#include <CriWare/adx_stmc.h>
#include <CriWare/private/adx_rna.h>
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
#define ADXT_ERR_NONE 0  // No error
#define ADXT_ERR_BUFF -1 // Buffer is empty
#define ADXT_ERR_MEM  -2 // Sound Block error

/* Playback Modes*/
#define ADXT_PLAYBACK_FILENAME 0 // Play the specified filename/id
#define ADXT_PLAYBACK_AFS      1 // Play the specified AFS file
#define ADXT_PLAYBACK_MEM      2 // Play data from memory
#define ADXT_PLAYBACK_STREAM   3 // Play data from a stream
#define ADXT_PLAYBACK_SLFILE   4 // Seamless continous play from a file

#define ADXT_OBUF_DIST   0x0860
#define ADXT_RNABUF_SIZE 0x0C00

#define ADXT_MAX_OBJ 4

#define ADXT_CALC_OBUFSIZE(numChan) ((ADXT_OBUF_DIST + ADXT_RNABUF_SIZE) * (numChan) * sizeof(s16))

typedef struct _adx_talk {
    /* 0x00 */ s8      used;       // Whether ADXT is in use
    /* 0x01 */ s8      stat;       // Operation status
    /* 0x02 */ s8      pmode;      // Playback mode
    /* 0x03 */ s8      maxnch;     // Maximum number of channels
    /* 0x04 */ ADXSJD* sjd;        // Stream Decoder
    /* 0x08 */ ADXSTM* stm;        // Stream Controller
    /* 0x0C */ ADXRNA  rna;        // Audio Renderer
    /* 0x10 */ SJ      sjf;        // File Input Stream
    /* 0x14 */ SJ      sji;        // Input Stream
    /* 0x18 */ SJ      sjo[2];     // Output Stream
    /* 0x20 */ s8*     ibuf;       // Input buffer
    /* 0x24 */ s32     ibuflen;    // Input buffer length
    /* 0x28 */ s32     ibufxlen;   // Input buffer extra length
    /* 0x2C */ s16*    obuf;       // Output buffer
    /* 0x30 */ s32     obufsize;   // Output buffer samples
    /* 0x34 */ s32     obufdist;   // Output buffer interval
    /* 0x38 */ s32     svrfreq;    // Server callback frequency
    /* 0x3C */ s16     maxsct;     // Input buffer max sectors
    /* 0x3E */ s16     minsct;     // Input buffer min sectors
    /* 0x40 */ s16     outvol;     // Output volume
    /* 0x42 */ s16     outpan[2];  // Output pan for each channel
    /* 0x46 */ s16     outbalance; // Output balance
    /* 0x48 */ s32     maxdecsmpl; // Maximum decoded samples
    /* 0x4C */ s32     lpcnt;      // Loop counter
    /* 0x50 */ s32     lp_skiplen; // Loop skip length
    /* 0x54 */ s32     trp;        // Transpose
    /* 0x58 */ s32     wpos;       // Write position
    /* 0x5C */ s32     mofst;      // Media offset
    /* 0x60 */ s16     ercode;     // Error code
    /* 0x64 */ s32     edecpos;    // Decode position
    /* 0x68 */ s16     edeccnt;    // Decode counter
    /* 0x6A */ s16     eshrtcnt;   // Input buffer empty count
    /* 0x6C */ s8      lpflg;      // Loop playback flag
    /* 0x6D */ s8      autorcvr;   // Autorecovery flag
    /* 0x6E */ s8      filterMode;
    /* 0x6F */ s8      execFlag;
    /* 0x70 */ s8      waitFlag;
    /* 0x71 */ s8      readyFlag;
    /* 0x72 */ s8      pause_flag; // Pause status flag
    /* 0x74 */ void*   amp;        // Amplifier
    /* 0x78 */ SJ      ampsji[2];  // Amp input stream
    /* 0x80 */ SJ      ampsjo[2];  // Amp output stream
    /* 0x88 */ s32     time_ofst;  // Time offset
    /* 0x8C */ s32     lesct;      // Loop playback end sector
    /* 0x90 */ s32     trpnsmpl;   // Trap sample
    /* 0x94 */ void*   lsc;        // Loop Stream Controller
    /* 0x98 */ s8      playbackFlag;
    /* 0x99 */ s8      pad0;
    /* 0x9A */ s16     pad1;
    /* 0x9C */ u32     tvofst;  // Start time offset
    /* 0xA0 */ u32     svcnt;   // VSync count
    /* 0xA4 */ u32     decofst; // Decode offset
    /* 0xA8 */ s8      streamStartFlag;
    /* 0xA9 */ s8      extraInfoFlag;
    /* 0xAA */ s16     pad2;
    /* 0xAC */ char*   workFilename;
    /* 0xB0 */ char*   filename;
    /* 0xB4 */ void*   directory;
    /* 0xB8 */ u32     offset;
    /* 0xBC */ u32     range;
    /* 0xC0 */ u32     loopDecodeLength;
} ADX_TALK; // Size: 0xC4
typedef ADX_TALK* ADXT;

/// MARK: Functions

/**
 * @brief Create an ADXT instance
 * @param maxChans Maximum number of channels (1: mono, 2: stereo)
 * @param work Pointer to work memory
 * @param worksize Size of work memory
 * @return Pointer to ADXT instance, or NULL on failure
 */
ADXT ADXT_Create(s32 maxChans, void* work, s32 worksize);

ADXT func_02016c64(void* work, s32 workSize);

void ADXT_Destroy(ADXT adxt);

/**
 * @brief Destroy all ADXT instances.
 */
void ADXT_DestroyAll(void);

void ADXT_Stop(ADXT adxt);

s32 ADXT_GetStat(ADXT adxt);

s32 ADXT_Play(ADXT adxt);

s32 func_020174c8();

s32 ADXT_GetNumChan();

void ADXT_SetOutPan(ADXT adxt, s32 channel, s32 pan);

void ADXT_SetOutVol(ADXT adxt, s32 vol);

s32 ADXT_GetOutVol(ADXT adxt);

void ADXT_SetDefSvrFreq(s32 freq);

void ADXT_SetLpFlg(ADXT adxt, s32 flag);

// void ADXT_Pause(ADXT adxt, s32 pauseState);

s32 ADXT_GetStatPause(ADXT adxt);

void func_02017b78(ADXT adxt, s32 param_1, s32 param_2);
void func_02017b7c(ADXT adxt, s32 param_1, s32 param_2);
void ADXT_SetLnkSw(ADXT adxt, s32 param_1);

void ADXT_StartMem2(ADXT adxt, void* adxData, s32 dataLength);

void func_020177b8();
void ADXT_ExecServer();

void func_02017d80(ADXT adxt, const char* filename);

#endif // ADXT_H