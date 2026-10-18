#ifndef FIELD_FLASH_FO3DRES_H
#define FIELD_FLASH_FO3DRES_H

#include "Field/Flash/FO3D.h"

FieldResourceState* FO3DRes_Create(void);
void                FO3DRes_Destroy(void);
void                FO3DRes_SetCurrent(FieldResourceState* state);
FieldResourceState* FO3DRes_GetCurrent(void);
u8                  FO3DRes_GetModelCount(void* resource);
s32                 FO3DRes_LoadToVram(void* resource);
void                FO3DRes_SetModel(void* resource, s32 index);
void                FO3DRes_SetTexture(void* resource);
void                FO3DRes_SetAnim(void* resource, s32 index);
void*               FO3DBase_Alloc(s32 size);
void                FO3DBase_Free(void* data);

#endif // FIELD_FLASH_FO3DRES_H
