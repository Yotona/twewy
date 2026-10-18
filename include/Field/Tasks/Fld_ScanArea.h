#ifndef FIELD_TASKS_FLD_SCANAREA_H
#define FIELD_TASKS_FLD_SCANAREA_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32  Fld_ScanArea_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos* pos, s32 texAddr, u32 palAddr);
s32  Fld_ScanArea_SetTargetScale(Task* task, s32 target);
s32  Fld_ScanArea_GetScale(Task* task);
s32  Fld_ScanArea_GetRadius(Task* task);
void Fld_ScanArea_FadeOut(Task* task);
void Fld_ScanArea_Open(Task* task);

#endif // FIELD_TASKS_FLD_SCANAREA_H
