#ifndef INTERFACE_DEBUG_TAKTEST_H
#define INTERFACE_DEBUG_TAKTEST_H

#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00000 */ ResourceManager  unk_00000;
    /* 0x11580 */ ResourceManager* unk_11580;
    /* 0x11584 */ s32              unk_11584;
    /* 0x11588 */ s32              unk_11588;
    /* 0x1158C */ MemPool          memPool;
    /* 0x11598 */ u8               memPoolBuffer[0x10000];
    /* 0x21598 */ TaskPool         taskPool;
    /* 0x21618 */ s32              unk_21618;
} TakTestState; // Size: 0x21A38

extern const TaskHandle Tsk_TakTest_BG;
extern const TaskHandle Tsk_TakTest_OBJ;

#endif // INTERFACE_DEBUG_TAKTEST_H