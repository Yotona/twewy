#ifndef DEBUG_TAKAMI_NREP_H
#define DEBUG_TAKAMI_NREP_H

#include "BgResMgr.h"
#include "DatMgr.h"
#include "EasyTask.h"
#include "Memory.h"

typedef struct {
    /* 0x00 */ Data*       data;
    /* 0x04 */ BgResource* screenResource;
    /* 0x08 */ BgResource* charResource;
    /* 0x0C */ void*       unk_0C;
    /* 0x10 */ u32*        unk_10;
    /* 0x14 */ u8*         unk_14;
    /* 0x18 */ u8*         unk_18;
} NRepBgRes; // Size: 0x1C

typedef struct {
    /* 0x00 */ u16       unk_00;
    /* 0x02 */ u8        unk_02;
    /* 0x03 */ u8        unk_03;
    /* 0x04 */ u8        unk_04;
    /* 0x05 */ u8        unk_05;
    /* 0x06 */ u8        unk_06;
    /* 0x07 */ u8        unk_07;
    /* 0x08 */ NRepBgRes mainRes[4];
    /* 0x78 */ NRepBgRes subRes[4];
} NRepContext; // Size: 0xE8

typedef struct {
    /* 0x00000 */ u8          pad_00000[0x11580];
    /* 0x11580 */ s32         unk_11580;
    /* 0x11584 */ s32         unk_11584;
    /* 0x11588 */ s32         unk_11588;
    /* 0x1158C */ MemPool     memPool;
    /* 0x11598 */ u8          memPoolBuffer[0x10000];
    /* 0x21598 */ TaskPool    taskPool;
    /* 0x21618 */ s32         taskId_Menu;
    /* 0x2161C */ s32         taskId_Text;
    /* 0x21620 */ s32         taskIds_Cursor[24];
    /* 0x21680 */ s32         unk_21680;
    /* 0x21684 */ s16         unk_21684;
    /* 0x21686 */ s16         unk_21686;
    /* 0x21688 */ s16         unk_21688;
    /* 0x2168C */ NRepContext context;
} NRepState; // Size: 0x21774

#endif       // DEBUG_TAKAMI_NREP_H