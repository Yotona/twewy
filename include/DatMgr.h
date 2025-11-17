#ifndef DATMGR_H
#define DATMGR_H

#include "BinMgr.h"
#include "Memory.h"
#include "PacMgr.h"
#include <types.h>

typedef struct Data {
    /* 0x00 */ struct Data* unk_00;
    /* 0x004 */ u32         unk_04;
    /* 0x008 */ void*       unk_08;
    /* 0x00C */ s32         unk_0C;
    /* 0x010 */ s32         unk_10;
    /* 0x014 */ s32         unk_14;
    /* 0x018 */ s32         unk_18;
    /* 0x01C */ Bin*        bin;
    /* 0x020 */ Pack*       pac;
    /* 0x024 */ s32         unk_024;
    /* 0x028 */ s32         unk_028;
    /* 0x02C */ s32         unk_02C;
} Data;

typedef struct DatMgr {
    /* 0x000 */ char  unk_000[0x28];
    /* 0x028 */ Data  unk_028[10];
    /* 0x208 */ Data* unk_208;
    /* 0x20C */ char  unk_20C[0x2C];
    /* 0x238 */ Data* unk_238;
} DatMgr;

extern DatMgr* data_0206b3c0;

Data* func_02008054(DatMgr* mgr);
void  func_0200806c(DatMgr* mgr, s32 idx, Data* data);
void  func_02008090(DatMgr* datMgr, Data* data);
void  func_020080ec(DatMgr* mgr, Data* data);
void  func_02008138(DatMgr* mgr, Data* data);
Data* func_02008814(u32 idx, u32 resourceId);

#endif // DATMGR_H