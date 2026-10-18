#ifndef FIELD_TASKS_FLD_SHINIGAMICOIN_H
#define FIELD_TASKS_FLD_SHINIGAMICOIN_H

#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"

extern const BinIdentifier Iden_FldShinigami;

s32  Fld_ShinigamiCoin_CreateTask(TaskPool* pool, s32 arg1);
void Fld_ShinigamiCoin_Reset(Task* task, s32 arg1);
s16  Fld_ShinigamiCoin_GetOnAnim(Task* task);
void Fld_ShinigamiCoin_Suppress(Task* task);
s32  Fld_ShinigamiCoin_IsSuppressed(Task* task);

#endif // FIELD_TASKS_FLD_SHINIGAMICOIN_H
