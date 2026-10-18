#ifndef FIELD_TASKS_FLD_PLAYER_H
#define FIELD_TASKS_FLD_PLAYER_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

s32  Fld_Player_GetDPadAngle(void);
s32  Fld_Player_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos* pos);
s32  Fld_Player_IsWalking(Task* task);
void Fld_Player_SetSpeed(Task* task, s32 arg1);
s32  Fld_Player_GetHeight(Task* task);
void Fld_Player_StopWalking(Task* task);
s32  Fld_Player_IsTouched(Task* task);
s32  Fld_Player_IsTalkTouched(Task* task);
void Fld_Player_StartBattleIn(Task* task);
s32  Fld_Player_AngleToDir(s32 arg0);

#endif // FIELD_TASKS_FLD_PLAYER_H
