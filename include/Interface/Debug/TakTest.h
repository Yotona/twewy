#ifndef INTERFACE_DEBUG_TAKTEST_H
#define INTERFACE_DEBUG_TAKTEST_H

#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00000 */ MenuStateBase base;
    /* 0x21618 */ s32           unk_21618;
} TakTestState; // Size: 0x21A38

extern const TaskHandle Tsk_TakTest_BG;
extern const TaskHandle Tsk_TakTest_OBJ;

#endif // INTERFACE_DEBUG_TAKTEST_H