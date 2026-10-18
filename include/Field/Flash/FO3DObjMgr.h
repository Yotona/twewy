#ifndef FIELD_FLASH_FO3DOBJMGR_H
#define FIELD_FLASH_FO3DOBJMGR_H

#include "Field/FieldTypes.h"
#include "Field/Flash/FO3D.h"

FO3DModel* FO3DObjMgr_Create(void);
void       FO3DObjMgr_Destroy(void);
void       FO3DObjMgr_SetCurrent(FO3DModel* model);
void       FO3DObjMgr_SetSampleFrames(const s32* frames, s32 count);
BOOL       FO3DObjMgr_UpdateBlendWeights(FieldPos* pos, FieldPos* points);
void       FO3DObjMgr_ResetObj(void);
void       FO3DObjMgr_SetModel(FieldResourceState* res);
void       FO3DObjMgr_SetTexture(FieldResourceState* res);
void       FO3DObjMgr_AddAnim(FieldResourceState* res);
void       FO3DObjMgr_UpdateAndDraw(FieldPos* pos, FieldPos* points);
void       FO3DObjMgr_SetOffset(Vec* offset);
BOOL       FO3DObjMgr_GetNodePos(const char* name, Vec* pos);
fx32       FO3DObjMgr_GetFrame(void);
BOOL       FO3DObjMgr_HasTexPatAnm(void);
BOOL       FO3DObjMgr_IsNamed(const char* name);
void       FO3DObjMgr_RestartAnim(void);
void       FO3DObjMgr_Show(void);
void       FO3DObjMgr_Hide(void);

#endif // FIELD_FLASH_FO3DOBJMGR_H
