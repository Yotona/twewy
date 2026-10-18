#ifndef FIELD_TASKS_FLD_PARTNER_H
#define FIELD_TASKS_FLD_PARTNER_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32  Fld_Partner_CreateTask(TaskPool* pool, const FieldPos* center, const FieldPos* startPos);
void Fld_Partner_WarpToPlayer(Task* task);
void Fld_Partner_SetSpeed(Task* task, s32 value);
s32  Fld_Partner_IsTalkTouched(Task* task);
s32  Fld_Partner_GetPosY(Task* task);
void Fld_Partner_StartBattleIn(Task* task);

#endif // FIELD_TASKS_FLD_PARTNER_H
