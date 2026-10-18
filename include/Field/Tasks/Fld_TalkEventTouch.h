#ifndef FIELD_TASKS_FLD_TALKEVENTTOUCH_H
#define FIELD_TASKS_FLD_TALKEVENTTOUCH_H

#include "Engine/EasyTask.h"

s32 Fld_TalkEventTouch_CreateTask(TaskPool* pool, s32 dataType, s16 x, s16 y);
s32 Fld_TalkEventTouch_CreateKeywordTask(TaskPool* pool, s32 dataType, s16 x, s16 y);

#endif // FIELD_TASKS_FLD_TALKEVENTTOUCH_H
