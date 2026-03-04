#ifndef DEBUG_TAKAMI_TAKTEST_H
#define DEBUG_TAKAMI_TAKTEST_H

#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"

typedef struct {
    /* 0x00 */ BgResource*      resScreen;
    /* 0x04 */ BgResource*      resChar;
    /* 0x08 */ PaletteResource* resPal;
    /* 0x0C */ char             unk_0C[4];
} TakTestUnknown; // Size: 0x90

typedef struct {
    /* 0x00000 */ ResourceManager  unk_00000;
    /* 0x11580 */ ResourceManager* unk_11580;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ s32              unk_11588;
    /* 0x1158C */ MemPool          memPool;
    /* 0x11598 */ char             unk_11598[0x10000];
    /* 0x21598 */ TaskPool         taskPool;
    /* 0x21618 */ s32              unk_21618;
} TakTestState; // Size: 0x21A38

#endif          // DEBUG_TAKAMI_TAKTEST_H