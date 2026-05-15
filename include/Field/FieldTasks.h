#ifndef FIELD_FIELDTASKS_H
#define FIELD_FIELDTASKS_H

#include "Engine/EasyTask.h"

s32 Fld_UGFade_CreateTask(TaskPool* pool, u16 arg1, s32 arg2, s32* arg3);

s32 Fld_TalkEventStringPlate_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, s16 arg5, s32 arg6);

s32 Fld_WhichArrow_CreateTask(TaskPool* pool, s32 dataType, s16 arg2, s16 arg3, s32 arg4);

s32 Fld_Choice_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16* arg3);

s32 Fld_GetBase_CreateTask(TaskPool* arg0, s32 arg1, s32 arg2);

s32 Fld_GetMess_CreateTask(TaskPool* arg0, s32 arg1, s16 arg2, s16 arg3);

s32 Fld_GetItem_CreateTask(TaskPool* pool, s32 arg1, s16 arg2);

#endif // FIELD_FIELDTASKS_H
