#ifndef FIELD_BATTLE_FIELDENCOUNTER_H
#define FIELD_BATTLE_FIELDENCOUNTER_H

#include "Field/Event/EventManager.h"

void func_ov030_020aac84(FieldEventManager* mgr);
BOOL Field_HasBustShotsOnBothSides(void);
s32  func_ov030_020aad28(void);
s32  func_ov030_020aad40(void);
void Field_PlayNekuVoice(void);
BOOL func_ov030_020ab3b8(s32 arg0);
BOOL func_ov030_020ab3ec(s32 arg0);
u16  func_ov030_020ab420(s32 arg0);
void Field_MainUpdate(FieldEventManager* mgr);
BOOL func_ov030_020abe64(u32 arg0);
s32  Field_GetChapterGroup(void);
u16  FieldEncounter_RollForHuman(s32 level, s32 limit);
s32  func_ov030_020ac254(void);
void Field_SpawnHumans(FieldEventManager* mgr);
void func_ov030_020acfc4(FieldEventManager* mgr);
void Field_CreateHud(FieldEventManager* mgr);
void Field_DestroyHud(FieldEventManager* mgr);
void Field_CreateObjects(FieldEventManager* mgr);
void Field_DestroyObjects(FieldEventManager* mgr);
void Field_ReleaseEventPicture(FieldEventManager* mgr);
void Field_CloseEventPicture(FieldEventManager* mgr);
void Field_LoadEventPicture(FieldEventManager* mgr);
void Field_LoadTouchEventBg(FieldEventManager* mgr);
void Field_SetupDisplay2D(FieldEventManager* mgr, s32 use256Colors);
void Field_SetupDisplay3D(FieldEventManager* mgr);

#endif // FIELD_BATTLE_FIELDENCOUNTER_H
