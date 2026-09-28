#ifndef INTERFACE_SAVE_H
#define INTERFACE_SAVE_H

#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Interface/Menu/MenuCommon.h"
#include "SpriteMgr.h"

typedef struct {
    /* 0x00000 */ MenuStateBase base;
    /* 0x21618 */ char          unk_21618[0x80];
    /* 0x21698 */ s16           unk_21698;
} SaveState; // Size: 0x21934

s32 Save_HelpCurU_CreateTask(TaskPool* pool, s32 dataType, void* arg2);

extern BinIdentifier data_ov043_020caf88;

#endif // INTERFACE_SAVE_H
