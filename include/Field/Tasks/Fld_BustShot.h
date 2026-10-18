#ifndef FIELD_TASKS_FLD_BUSTSHOT_H
#define FIELD_TASKS_FLD_BUSTSHOT_H

#include "Engine/EasyTask.h"

s32  Fld_BustShot_GetMoveInputDir(s32* out);
s32  Fld_BustShot_CreateTask(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7);
void Fld_BustShot_SlideOut(Task* task, s32 arg1, s32 arg2);
s32  Fld_BustShot_IsSlideOutDone(Task* task);
s32  Fld_BustShot_IsMoveDone(Task* task);
s16  Fld_BustShot_GetPosX(Task* task);
void Fld_BustShot_Nudge(Task* task, s32 arg1);
void Fld_BustShot_MoveToCenter(Task* task, s16 arg1);
void Fld_BustShot_ToggleFlip(Task* task);
void Fld_BustShot_CrossFadeTo(TaskPool* pool, Task* task, u32 idx, s32 arg3, s32 arg4);
s32  Fld_BustShot_IsCrossFadeDone(Task* task);
void Fld_BustShot_SwapSide(Task* task, s16 arg1);
void Fld_BustShot_SetSortKey(Task* task, s32 arg1);
void Fld_BustShot_MoveTo(Task* task, s16 arg1, s32 arg2, s16 arg3);
void Fld_BustShot_ChangePose(TaskPool* pool, Task* task, u32 idx, s32 arg3);
s32  Fld_BustShot_ConsumePoseChanged(Task* task);
void Fld_BustShot_StartShake(Task* task);
s16  Fld_BustShot_GetScreenX(Task* task);
void Fld_BustShot_SetPriority(Task* task, u16 arg1);
void Fld_BustShot_ClearPoseChanged(Task* task);

#endif // FIELD_TASKS_FLD_BUSTSHOT_H
