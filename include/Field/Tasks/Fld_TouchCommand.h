#ifndef FIELD_TASKS_FLD_TOUCHCOMMAND_H
#define FIELD_TASKS_FLD_TOUCHCOMMAND_H

#include "Engine/EasyTask.h"

s32  Fld_TouchCommand_CreateTask(TaskPool* pool, s32 arg1, s32 arg2);
s32  Fld_TouchCommand_CreateTaskWithMsg(TaskPool* pool, s32 arg1, u16 arg2, s32 arg3);
void Fld_TouchCommand_SetCommand(Task* task, s32 arg1);

#endif // FIELD_TASKS_FLD_TOUCHCOMMAND_H
