#ifndef FIELD_SCENE_FIELDMAIN_H
#define FIELD_SCENE_FIELDMAIN_H

#include "Field/Event/EventManager.h"

void FieldMain_StepFadeIn(FieldEventManager* mgr);
void FieldMain_StepFadeBoth(FieldEventManager* mgr);
void FieldMain_StepFadeMain(FieldEventManager* mgr);
void FieldMain_StepLeaveArea(FieldEventManager* mgr);
void FieldMain_StepWait(FieldEventManager* mgr);
void FieldMain_StepWaitThenBlend(FieldEventManager* mgr);
void FieldMain_StepFadeInMain(FieldEventManager* mgr);
void FieldMain_StepFadeInBoth(FieldEventManager* mgr);
void FieldMain_StepWaitFade(FieldEventManager* mgr);
void FieldMain_StepStartUGFade(FieldEventManager* mgr);
void FieldMain_StepStartUGFadeWithBg(FieldEventManager* mgr);
void FieldMain_StepOpenEventPicture(FieldEventManager* mgr);
void FieldMain_StepSetup2D(FieldEventManager* mgr);
void FieldMain_StepOpenTouchEvent(FieldEventManager* mgr);
void FieldMain_StepCloseEventPicture(FieldEventManager* mgr);
void FieldMain_StepCloseTouchEvent(FieldEventManager* mgr);
void FieldMain_StepBlendInSub(FieldEventManager* mgr);
void FieldMain_StepBlendOutSub(FieldEventManager* mgr);

#endif // FIELD_SCENE_FIELDMAIN_H
