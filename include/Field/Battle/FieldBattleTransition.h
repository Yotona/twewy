#ifndef FIELD_BATTLE_FIELDBATTLETRANSITION_H
#define FIELD_BATTLE_FIELDBATTLETRANSITION_H

#include <nitro/types.h>

void FieldScan_MarkSeen(s32 id);
BOOL FieldScan_WasSeen(s32 id);
BOOL FieldScan_IsTouchActive(void);
BOOL Field_IsMoveTouchActive(void);
BOOL Field_IsMoveInputActive(void);
s32  func_ov030_020aef2c(void);
BOOL FieldScan_IsEnteringBattle(void);
BOOL FieldScan_IsBattleStarting(void);
BOOL Field_IsBattlePending(void);
BOOL FieldScan_IsOpen(void);
s32  Field_IsTalkEventActive(void);
BOOL FieldScan_IsClosing(void);
BOOL func_ov030_020af01c(void);
BOOL Field_IsIdle(void);
void FieldDebug_DrawAreaName(void);
s32  FieldScan_GetRadius(void);
s32  FieldScan_IsActive(void);
s32  func_ov030_020af2fc(void);

#endif // FIELD_BATTLE_FIELDBATTLETRANSITION_H
