#ifndef FIELD_TASKS_FLD_BADGE_H
#define FIELD_TASKS_FLD_BADGE_H

#include "Engine/EasyTask.h"

s32  Fld_Badge_CreateTask(TaskPool* pool, s32 arg1, s16 arg2, s16 arg3, s32 arg4, u16 arg5);
s32  Fld_Badge_IsHeld(Task* task);
s32  Fld_Badge_IsTouched(Task* task);
s32  Fld_Badge_SetAnimation(Task* task, s16 anim);
void Fld_Badge_SetVisible(Task* task, s32 visible);
void Fld_Badge_SetPriority(Task* task, u16 mode);

#endif // FIELD_TASKS_FLD_BADGE_H
