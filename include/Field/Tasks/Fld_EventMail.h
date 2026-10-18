#ifndef FIELD_TASKS_FLD_EVENTMAIL_H
#define FIELD_TASKS_FLD_EVENTMAIL_H

#include "Engine/EasyTask.h"

s32  Fld_EventMail_CreateTask(TaskPool* pool, s32 dataType);
void Fld_EventMail_Press(Task* task);

#endif // FIELD_TASKS_FLD_EVENTMAIL_H
