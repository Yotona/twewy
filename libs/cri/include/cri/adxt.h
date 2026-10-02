#ifndef ADXT_H
#define ADXT_H

#include <cri/adx_sjd.h>
#include <cri/adx_stmc.h>
#include <cri/cri_xpt.h>
#include <cri/private/adx_rna.h>
#include <cri/sj.h>

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

#define ADXT_CALC_OBUFSIZE(numChan) ((ADXT_OBUF_DIST + ADXT_RNABUF_SIZE) * (numChan) * sizeof(short))

typedef struct _adx_talk {
    /* 0x00 */ char         used;       // Whether ADXT is in use
    /* 0x01 */ char         stat;       // Operation status
    /* 0x02 */ char         pmode;      // Playback mode
    /* 0x03 */ char         maxnch;     // Maximum number of channels
    /* 0x04 */ ADXSJD*      sjd;        // Stream Decoder
    /* 0x08 */ ADXSTM*      stm;        // Stream Controller
    /* 0x0C */ ADXRNA       rna;        // Audio Renderer
    /* 0x10 */ SJ           sjf;        // File Input Stream
    /* 0x14 */ SJ           sji;        // Input Stream
    /* 0x18 */ SJ           sjo[2];     // Output Stream
    /* 0x20 */ char*        ibuf;       // Input buffer
    /* 0x24 */ int          ibuflen;    // Input buffer length
    /* 0x28 */ int          ibufxlen;   // Input buffer extra length
    /* 0x2C */ short*       obuf;       // Output buffer
    /* 0x30 */ int          obufsize;   // Output buffer samples
    /* 0x34 */ int          obufdist;   // Output buffer interval
    /* 0x38 */ int          svrfreq;    // Server callback frequency
    /* 0x3C */ short        maxsct;     // Input buffer max sectors
    /* 0x3E */ short        minsct;     // Input buffer min sectors
    /* 0x40 */ short        outvol;     // Output volume
    /* 0x42 */ short        outpan[2];  // Output pan for each channel
    /* 0x46 */ short        outbalance; // Output balance
    /* 0x48 */ int          maxdecsmpl; // Maximum decoded samples
    /* 0x4C */ int          lpcnt;      // Loop counter
    /* 0x50 */ int          lp_skiplen; // Loop skip length
    /* 0x54 */ int          trp;        // Transpose
    /* 0x58 */ int          wpos;       // Write position
    /* 0x5C */ int          mofst;      // Media offset
    /* 0x60 */ short        ercode;     // Error code
    /* 0x64 */ int          edecpos;    // Decode position
    /* 0x68 */ short        edeccnt;    // Decode counter
    /* 0x6A */ short        eshrtcnt;   // Input buffer empty count
    /* 0x6C */ char         lpflg;      // Loop playback flag
    /* 0x6D */ char         autorcvr;   // Autorecovery flag
    /* 0x6E */ char         filterMode;
    /* 0x6F */ char         execFlag;
    /* 0x70 */ char         waitFlag;
    /* 0x71 */ char         readyFlag;
    /* 0x72 */ char         pause_flag; // Pause status flag
    /* 0x74 */ void*        amp;        // Amplifier
    /* 0x78 */ SJ           ampsji[2];  // Amp input stream
    /* 0x80 */ SJ           ampsjo[2];  // Amp output stream
    /* 0x88 */ int          time_ofst;  // Time offset
    /* 0x8C */ int          lesct;      // Loop playback end sector
    /* 0x90 */ int          trpnsmpl;   // Trap sample
    /* 0x94 */ void*        lsc;        // Loop Stream Controller
    /* 0x98 */ char         lnkflg;     // Link (seamless next-file) switch
    /* 0x99 */ char         pad0;
    /* 0x9A */ short        pad1;
    /* 0x9C */ unsigned int tvofst;  // Start time offset
    /* 0xA0 */ unsigned int svcnt;   // VSync count
    /* 0xA4 */ unsigned int decofst; // Decode offset
    /* 0xA8 */ char         streamStartFlag;
    /* 0xA9 */ char         extraInfoFlag;
    /* 0xAA */ short        pad2;
    /* 0xAC */ char*        workFilename;
    /* 0xB0 */ char*        filename;
    /* 0xB4 */ void*        directory;
    /* 0xB8 */ unsigned int offset;
    /* 0xBC */ unsigned int range;
    /* 0xC0 */ unsigned int loopDecodeLength;
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
ADXT ADXT_Create(int maxChans, void* work, int worksize);

ADXT ADXT_Create3D(void* work, int workSize);

void ADXT_Destroy(ADXT adxt);

/**
 * @brief Destroy all ADXT instances.
 */
void ADXT_DestroyAll(void);

void ADXT_Stop(ADXT adxt);

int ADXT_GetStat(ADXT adxt);

int ADXT_GetTimeReal(ADXT adxt);

int adxt_GetTimeReal();

int ADXT_GetNumChan();

void ADXT_SetOutPan(ADXT adxt, int channel, int pan);

void ADXT_SetOutVol(ADXT adxt, int vol);

int ADXT_GetOutVol(ADXT adxt);

void ADXT_SetDefSvrFreq(int freq);

void ADXT_SetLpFlg(ADXT adxt, int flag);

// void ADXT_Pause(ADXT adxt, int pauseState);

int ADXT_GetStatPause(ADXT adxt);

void adxt_SetTranspose(ADXT adxt, int param_1, int param_2);
void adxt_GetTranspose(ADXT adxt, int param_1, int param_2);
void ADXT_SetLnkSw(ADXT adxt, int param_1);

void ADXT_StartMem2(ADXT adxt, void* adxData, int dataLength);

void func_020177b8();
void ADXT_ExecServer();

void ADXT_StartFname(ADXT adxt, const char* filename);

#endif // ADXT_H