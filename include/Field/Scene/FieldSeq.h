#ifndef FIELD_SCENE_FIELDSEQ_H
#define FIELD_SCENE_FIELDSEQ_H

#include "Field/Event/EventManager.h"

// The running field's event manager.
extern FieldEventManager* data_ov030_02109d60;
extern s32                data_ov030_02109d64;

s32  func_ov030_020ae2c8(FieldEventManager*);
void func_ov030_020ae92c(void* arg);
void func_ov030_020ae96c(s32 area);
void func_ov030_020aeaa0(s32 area, s32 flags);
void func_ov030_020aeab8(s32 area, s32 flags);
void func_ov030_020aead0(s32 flags);
s32  func_ov030_020aeadc(s32 area, s32 flags);
s32  func_ov030_020aeaf4(void);

#endif // FIELD_SCENE_FIELDSEQ_H
