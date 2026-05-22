#ifndef INTERFACE_SAVE_H
#define INTERFACE_SAVE_H

#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"

s32 Save_HelpCurU_CreateTask(TaskPool* pool, s32 dataType, void* arg2);

extern BinIdentifier data_ov043_020caf88;

#endif // INTERFACE_SAVE_H
