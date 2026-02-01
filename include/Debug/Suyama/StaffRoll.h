#ifndef DEBUG_SUYAMA_STAFFROLL_H
#define DEBUG_SUYAMA_STAFFROLL_H

#include "DatMgr.h"
#include "Display.h"
#include "EasyTask.h"

typedef struct {
    /* 0x00 */ Data* data;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ u16   unk_08;
    /* 0x0A */ u16   unk_0A;
    /* 0x0C */ u16   unk_0C;
} StaffRollUnkA; // Size = 0x10

typedef struct {
    /* 0x00 */ Data* data;
    /* 0x04 */ void* unk_04;
} StaffRollUnkB; // Size = 0x8

typedef struct {
    /*0x00*/ StaffRollUnkA group1[4];
    /*0x40*/ StaffRollUnkB group2[4];
    /*0x60*/ StaffRollUnkB group3[3];
} StaffRollUnkC; // Size = 0x78

typedef struct {
    /* 0x000 */ TaskPool*     pool;
    /* 0x004 */ s16           unk_04;
    /* 0x006 */ char          pad_06[0x6];
    /* 0x00C */ s32           commandIndex;
    /* 0x010 */ StaffRollUnkC unk_10[2];
    /* 0x100 */ u8            unk_100_1 : 1;
    /* 0x100 */ u8            unk_100_2 : 1;
    /* 0x101 */ char          pad_101[0x3];
    /* 0x104 */ Data*         commandlist;
} StaffRoll_CallbackStruct; // Size: 0x108

typedef struct {
    /* 0x00 */ char unk_00[0x14];
    /* 0x14 */ s16* unk_14;
    /* 0x18 */ char unk_18[0x58];
    /* 0x70 */ s32  unk_70;
    /* 0x74 */ char unk_74[0x8];
} FontRollInternal; // Size: 0x7C

typedef struct {
    /* 0x000 */ StaffRoll_CallbackStruct* unk_000;
    /* 0x004 */ DisplayEngine             engine;
    /* 0x008 */ DisplayBGLayer            bgLayer;
    /* 0x00C */ u16                       unk_00C;
    /* 0x00E */ s16                       unk_00E;
    /* 0x010 */ FontRollInternal          unk_010[30];
    /* 0xE98 */ s32                       unk_E98;
    /* 0xE9C */ s32                       unk_E9C;
    /* 0xEA0 */ s32                       unk_EA0;
    /* 0xEA4 */ s32                       unk_EA4;
    /* 0xEA8 */ Data*                     unk_EA8;
    /* 0xEAC */ s16                       unk_EAC;
    /* 0xEAE */ s16                       unk_EAE;
    /* 0xEB0 */ s16                       unk_EB0;
} FontRoll; // Size: 0xEB4

typedef struct {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ u16 unk_0C;
    /* 0x0E */ s16 unk_0E;
} FontRollArgs;

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s32 unk_1C;
    /* 0x20 */ s32 unk_20;
    /* 0x24 */ s16 unk_24;
    /* 0x26 */ s16 unk_26;
} BgScroll;

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ s16 unk_08;
    /* 0x0A */ s16 unk_0A;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s16 unk_10;
} BgScrollArgs;

typedef struct {
    /* 0x00 */ DisplayEngine engine;            // Which display engine to affect
    /* 0x04 */ u16           startBrightness;   // Initial brightness
    /* 0x06 */ s16           endBrightness;     // Target brightness
    /* 0x08 */ u16           duration;          // Animation duration in frames
    /* 0x0A */ char          _pad[2];
    /* 0x0C */ s32           currentBrightness; // Current brightness value
    /* 0x10 */ s32           step;              // Per-frame brightness change
    /* 0x14 */ s16           elapsedFrames;     // Frames elapsed since start
    /* 0x16 */ s16           maxFrames;         // Total duration in frames
} BgBright;

typedef struct {
    DisplayEngine engine;          // Which display engine to affect
    s16           startBrightness; // Initial brightness
    s16           endBrightness;   // Target brightness
    s16           duration;        // Animation duration in frames
} BgBrightArgs;

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s32 unk_18;
    /* 0x1C */ s16 unk_1C;
    /* 0x20 */ s16 unk_1E;
} StaffBg;

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 unk_04;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ s16 unk_0C;
    /* 0x0E */ s16 unk_0E;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ s16 unk_12;
} StaffBgArgs;

typedef struct {
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u16 unk_06;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 pad_0E;
} CommandUnk04;

typedef struct {
    u16          StaffRollUnkCIndex;
    u16          group1Index;
    CommandUnk04 unk_04;
} Command; // Size = 0x14

typedef struct {
    /* 0x00000 */ char                     unk_00000[0x11580];
    /* 0x11580 */ s32                      unk_11580;
    /* 0x11584 */ MemPool                  memPool;
    /* 0x11590 */ u8                       memPoolBuffer[0x10000];
    /* 0x21590 */ char                     unk_21590[0x4];
    /* 0x21594 */ TaskPool                 taskPool;
    /* 0x21614 */ StaffRoll_CallbackStruct unk_21614;
    /* 0x2171C */ void*                    unk_2171C;
    /* 0x21720 */ char                     pad_21720[0x78];
} StaffRollState; // Size: 0x21798

#endif            // DEBUG_SUYAMA_STAFFROLL_H