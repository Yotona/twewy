#ifndef DEBUG_TAKAMI_SAVE_H
#define DEBUG_TAKAMI_SAVE_H

#include "Engine/EasyTask.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00000 */ char     unk_00000[0x21598];
    /* 0x21598 */ TaskPool taskPool;
    /* 0x21618 */ char     unk_21618[0x80];
    /* 0x21698 */ s16      unk_21698;
} SaveState; // Size: 0x21934

#endif       // DEBUG_TAKAMI_SAVE_H