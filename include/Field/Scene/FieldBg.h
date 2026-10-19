#ifndef FIELD_SCENE_FIELDBG_H
#define FIELD_SCENE_FIELDBG_H

#include "Field/Event/EventManager.h"

s32  FieldBg_IsAreaTitleDone(FieldEventManager* mgr);
void FieldBg_UpdateWalkSpeed(FieldEventManager* mgr);
void FieldBg_ResetScanSeen(FieldEventManager* mgr);
void FieldBg_LoadScanPicture(FieldEventManager* mgr);
void FieldBg_ShowEventPicture(FieldEventManager* mgr);
void FieldBg_UpdateScanPicturePalette(FieldEventManager* mgr);
void FieldBg_UpdateTopBgPalette(FieldEventManager* mgr);
void FieldBg_ReleaseSubBg0(FieldEventManager* mgr);
void FieldBg_LoadScanSubBg0(FieldEventManager* mgr);
void FieldBg_ReleaseSubBg1(FieldEventManager* mgr);
void FieldBg_LoadScanSubBg1(FieldEventManager* mgr);
void FieldBg_LoadScanFrameSubBg0(FieldEventManager* mgr);
void FieldBg_LoadDownScreenSubBg(FieldEventManager* mgr, s32 arg1);
void FieldBg_LoadScanLevelSubBg1(FieldEventManager* mgr);
void FieldBg_LoadScanSubBg3(FieldEventManager* mgr);
void FieldBg_LoadScanLevelTopBg(FieldEventManager* mgr);
s32  FieldBg_AreTrendsVisible(void);
void FieldBg_LoadAreaBg(FieldEventManager* mgr, s32 arg1, s32 arg2);
void FieldBg_SetTopBgMode(FieldEventManager* mgr, s32 arg1);
s32  FieldBg_IsBadgeTapped(FieldEventManager* mgr);
s32  FieldBg_IsBadgeHeld(FieldEventManager* mgr);
s32  FieldBg_GetTouchedHuman(FieldEventManager* mgr);
s32  FieldBg_IsPlayerNear(s32 arg0, s32 arg1, s32 arg2);
s32  FieldBg_IsPlayerInRect(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
s32  FieldBg_CheckExit(FieldEventManager* mgr, s32 arg1);
void FieldBg_SetAreaPopulation(s32 arg0, s32 arg1);
s32  func_ov030_02084978(FieldEventManager* mgr);

#endif // FIELD_SCENE_FIELDBG_H
