#ifndef FIELD_TASKS_FLD_TALKICON_H
#define FIELD_TASKS_FLD_TALKICON_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32  Fld_TalkIcon_CreateTask(TaskPool* pool, s32 dataType, FieldPos* screen, s32* scale, s32 height, s32 animIndex);
void Fld_TalkIcon_Highlight(Task* task);
void Fld_TalkIcon_Unhighlight(Task* task);

#endif // FIELD_TASKS_FLD_TALKICON_H
