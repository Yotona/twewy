#ifndef FIELD_TASKS_FLD_PHONE_H
#define FIELD_TASKS_FLD_PHONE_H

#include "Engine/EasyTask.h"

s32  Fld_Phone_CreateTask(TaskPool* pool, s32 arg1);
void Fld_Phone_Press(Task* task);

#endif // FIELD_TASKS_FLD_PHONE_H
