#ifndef FIELD_TASKS_FLDCHARAWALK_H
#define FIELD_TASKS_FLDCHARAWALK_H

#include "Field/FieldTypes.h"
#include "SpriteMgr.h"

// Walk helpers shared by the player and the partner.

u16 FldCharaWalk_AngleToCell(s32 angle);
u16 FldCharaWalk_GetLoopFrame(Sprite* sprite);
s32 FldCharaWalk_MoveWithCollision(FieldPos* pos, s32 angle, s32 dist);

#endif // FIELD_TASKS_FLDCHARAWALK_H
