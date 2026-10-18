#ifndef FIELD_BATTLE_FIELDBATTLESETUP_H
#define FIELD_BATTLE_FIELDBATTLESETUP_H

#include "Engine/Resources/PaletteMgr.h"
#include "Field/Event/EventManager.h"
#include "Field/Event/Events.h"
#include "Field/FieldTypes.h"

u8               Field_GetAreaZone(u32 area);
s32              Field_GetCurrentTrendZone(void);
void             FieldBattle_SetupScripted(s32 selector);
void             Field_BeginScript(FieldEvent event, s32 param);
void             Field_CountDrawnHuman(void);
void             func_ov030_020af51c(void);
u16              Field_GetLightColor(void);
void             Field_ApplyVertexColorAt(FieldPos* pos);
PaletteResource* Field_GetHumanPalette(void);
s32              Field_GetPlayerHeight(void);
s32              func_ov030_020af618(void);
void             Field_GetPlayerScreenPos(s16* out0, s16* out1);
s32              Field_ResolvePresetCoord(s32 x);
void             func_ov030_020af6d4(s32 mode);
void             Field_SetTouchEventResult(s32 result);
void             Field_SetTouchEventTarget(s16 target);
s32              func_ov030_020af730(void);
s32              func_ov030_020af748(void);
void             FieldScan_AddTime(s32 frames);
void             FieldScan_RaiseBgLevel(s32 category);
void             func_ov030_020af7f0(void);
s32              Field_GetMaxChainedEncounters(void);
BOOL             FieldScan_CanChainEncounter(void);
void             FieldScan_AddChainedEncounter(void);
s16              Field_AddEncounterHumanCount(s32 delta);
void             Field_SetEncounterHumanCount(s16 count);
FieldPos*        Field_GetCameraOrigin(void);
void             FieldScan_Update(FieldEventManager* mgr);
s16              FieldTalk_GetKeywordCount(void);
BOOL             FieldEvent_IsDebugSkipHeld(void);

#endif // FIELD_BATTLE_FIELDBATTLESETUP_H
