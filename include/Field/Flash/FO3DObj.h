#ifndef FIELD_FLASH_FO3DOBJ_H
#define FIELD_FLASH_FO3DOBJ_H

#include "Field/Flash/FO3D.h"

void           FO3DObj_SetCurrent(Field3DObject* state);
Field3DObject* FO3DObj_Create(void);
void           FO3DObj_Destroy(void);
void           FO3DObj_SetRotY(s16 rotY);
void           FO3DObj_SetPos(s32 x, s32 y, s32 z);
void           func_ov030_020cde20(s32 value);
void*          FO3DObj_Alloc(s32 size);
void           FO3DObj_Free(void* data);

#endif // FIELD_FLASH_FO3DOBJ_H
