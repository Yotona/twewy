#ifndef FIELD_TASKS_FLD_MAPJUMPARROW_H
#define FIELD_TASKS_FLD_MAPJUMPARROW_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32 Fld_MapJumpArrow_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, FldSpawnTemplate tmpl,
                                s32 destArea);

#endif // FIELD_TASKS_FLD_MAPJUMPARROW_H
