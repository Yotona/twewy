#ifndef ACSSVR_H
#define ACSSVR_H

#include <CriWare/acsfdr.h>
#include <CriWare/acssnd.h>
#include <CriWare/acsvhl.h>
#include <types.h>

typedef struct ACSSVR {
    /* 0x00 */ ACSSND** snd;
    /* 0x04 */ char     unk_04[0xC];
    /* 0x10 */ ACSVHL*  vhl;
    /* 0x14 */ ACSVHL   unk_14;
    /* 0x74 */ s32      unk_74;
    /* 0x78 */ s32      unk_78;
    /* 0x7C */ s32      unk_7C;
    /* 0x80 */ void (*unk_80)();
    /* 0x84 */ s32 unk_84;
    /* 0x88 */ void (*unk_88)();
    /* 0x8C */ s32 unk_8C;
} ACSSVR; // Size: 0x90

BOOL ACSSVR_Init(ACSSVR* svr);

void ACSSVR_Finish(ACSSVR* svr);

ACSSND* ACSSVR_CreatSnd(ACSSVR* svr, s32* param_2, ACSSND* work, u32 workSize);

void func_0201f18c(ACSSVR* svr, ACSSND* snd);

void func_0201f460(ACSSVR* svr, ACSSND* snd);

void func_0201f4e8(ACSSVR* svr);

BOOL func_0201f520(ACSSVR* svr);

s32 func_0201f534(ACSSVR* svr);

void func_0201f55c(const char* error);

#endif // ACSSVR_H
