#ifndef DEBUG_TAKAMI_SAVE_H
#define DEBUG_TAKAMI_SAVE_H

#include "EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00 */ Sprite sprite;
    /* 0x40 */ s32    unk_40;
    /* 0x44 */ void*  unk_44;
} SaveSlotCoverU; // Size: 0x48

typedef struct {
    /* 0x00 */ s32   unk_00;
    /* 0x08 */ void* unk_04;
    /* 0x04 */ u16   unk_08;
} SlotCoverUArgs;

typedef struct {
    /* 0x00 */ Sprite sprites[2];
    /* 0x80 */ s32    unk_80[2];
    /* 0x88 */ s32    unk_88;
} SaveHelpCurU; // Size: 0x8C

typedef struct {
    /* 0x00 */ s32   unk_00;
    /* 0x04 */ void* unk_04;
} HelpCurUArgs;

typedef struct {
    /* 0x00000 */ char     unk_00000[0x21598];
    /* 0x21598 */ TaskPool taskPool;
    /* 0x21618 */ char     unk_21618[0x80];
    /* 0x21698 */ s16      unk_21698;
} SaveState; // Size: 0x21934

#endif       // DEBUG_TAKAMI_SAVE_H