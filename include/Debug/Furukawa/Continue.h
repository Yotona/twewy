#ifndef DEBUG_FURUKAWA_CONTINUE_H
#define DEBUG_FURUKAWA_CONTINUE_H

#include "DatMgr.h"
#include "EasyTask.h"
#include "Memory.h"

// TODO: Most likely defined by overlay 3
typedef struct {
    /* 0x00 */ char unk_00[0x60];
    /* 0x60 */ s16  unk_60;
    /* 0x62 */ s16  unk_62;
    /* 0x64 */ s16  unk_64;
    /* 0x66 */ char unk_66[0x2];
} ContinueUnkStruct; // Size: 0x68

// TODO: Most likely defined by overlay 3
typedef struct {
    /* 0x00 */ char unk_00[0x60];
} ContinueUnkStruct2; // Size: 0x60

typedef struct {
    /* 0x00000 */ u8                 memBuffer[0x800];
    /* 0x00800 */ MemPool            memPool;
    /* 0x0080C */ TaskPool           taskPool;
    /* 0x0088C */ s32                unk_0088C;
    /* 0x00890 */ char               unk_00890[0x1157C];
    /* 0x11E0C */ Data*              unk_11E0C;
    /* 0x11E10 */ ContinueUnkStruct  unk_11E10[4];
    /* 0x11FB0 */ ContinueUnkStruct2 unk_11FB0;
    /* 0x12010 */ ContinueUnkStruct2 unk_12010;
    /* 0x12070 */ ContinueUnkStruct2 unk_12070;
    /* 0x120D0 */ u16                unk_120D0;
    /* 0x120D4 */ s32                unk_120D4;
    /* 0x120D8 */ s32                unk_120D8;
    /* 0x120DC */ s16                unk_120DC;
    /* 0x120DE */ s16                unk_120DE;
} ContinueObject; // Size: 0x120E0

#endif            // DEBUG_FURUKAWA_CONTINUE_H1