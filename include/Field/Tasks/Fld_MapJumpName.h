#ifndef FIELD_TASKS_FLD_MAPJUMPNAME_H
#define FIELD_TASKS_FLD_MAPJUMPNAME_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"

s32      Fld_Mess_StartText(TaskPool* pool, Sprite* sprite, SysCode* text, u8* displayed, s32 arg4);
SysCode* Fld_Mess_GetStrCat(SysFont* font, u16 msgIndex1, u16 msgIndex2);
void     Fld_Mess_CenterText(SysFont* font, SysCode* text, s32 align, Sprite* sprite);
s32      Fld_MapJumpName_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 dataType, s32 code, s32 animIndex,
                                    s32 useBig);

#endif // FIELD_TASKS_FLD_MAPJUMPNAME_H
