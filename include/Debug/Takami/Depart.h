#ifndef DEBUG_TAKAMI_DEPART_H
#define DEBUG_TAKAMI_DEPART_H

#include "BgResMgr.h"
#include "EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Data*       data;
    /* 0x04 */ BgResource* screenResource;
    /* 0x08 */ BgResource* charResource;
    /* 0x0C */ void*       unk_0C;
    /* 0x10 */ u32*        unk_10;
    /* 0x14 */ u8*         unk_14;
    /* 0x18 */ u8*         unk_18;
} DepartResources; // Size: 0x1C

typedef struct {
    /* 0x00 */ u16             unk_00;
    /* 0x02 */ u8              unk_02;
    /* 0x03 */ u8              unk_03;
    /* 0x04 */ u16             unk_04;
    /* 0x06 */ u16             unk_06;
    /* 0x08 */ u16             unk_08;
    /* 0x0A */ char            unk_0A[0x2];
    /* 0x0C */ u16             unk_0C;
    /* 0x0E */ char            unk_0E[0x22];
    /* 0x30 */ u8              unk_30;
    /* 0x31 */ u8              unk_31;
    /* 0x32 */ char            unk_32[0x2];
    /* 0x34 */ DepartResources unk_34[4];
    /* 0xA4 */ DepartResources unk_A4[4];
} DepartUnk;

typedef struct {
    /* 0x00000 */ char      unk_00000[0x11580];
    /* 0x11580 */ s32       unk_11580;
    /* 0x11584 */ s32       unk_11584;
    /* 0x11588 */ s32       unk_11588;
    /* 0x1158C */ MemPool   memPool;
    /* 0x11598 */ u8        memPoolBuffer[0x10000];
    /* 0x21598 */ TaskPool  taskPool;
    /* 0x21618 */ s32       taskId_Exit;
    /* 0x2161C */ s32       taskIds_Panel[5];
    /* 0x21630 */ s32       taskIds_Board[5];
    /* 0x21644 */ s32       taskId_Cur;
    /* 0x21648 */ s32       taskId_TextScr;
    /* 0x2164C */ s32       taskId_TextScrU;
    /* 0x21650 */ s32       unk_21650;
    /* 0x21654 */ s16       unk_21654;
    /* 0x21656 */ s16       unk_21656;
    /* 0x21658 */ char      unk_21658[0x4];
    /* 0x2165C */ DepartUnk unk_2165C;
} DepartState; // Size: 0x21770

#endif         // DEBUG_TAKAMI_DEPART_H