#ifndef FIELD_TASKS_FLD_TALKEVENTDECIDE_H
#define FIELD_TASKS_FLD_TALKEVENTDECIDE_H

#include "Engine/EasyTask.h"

s32 Fld_TalkEventDecide_CreateTask(TaskPool* pool, s32 dataType, s16 posX, s16 posY);
s32 Fld_TalkEventDecide_CreateSpeakerTask(TaskPool* pool, s32 dataType, s16 posX, s16 posY);

#endif // FIELD_TASKS_FLD_TALKEVENTDECIDE_H
