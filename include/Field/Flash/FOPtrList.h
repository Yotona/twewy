#ifndef FIELD_FLASH_FOPTRLIST_H
#define FIELD_FLASH_FOPTRLIST_H

#include "Field/Flash/FO3D.h"

void          FOPtrList_Destroy(void);
void          FOPtrList_SetCurrent(FieldPtrList* list);
FieldPtrList* FOPtrList_GetCurrent(void);
void          FOPtrList_Init(s32 capacity);
s32           FOPtrList_Add(void* item);
void          FO3D_InitDisplay(void);
void          FO3D_ExitDisplay(void);
void          FO3D_FadeInStep(void);
void          FO3D_FadeOutStep(void);

#endif // FIELD_FLASH_FOPTRLIST_H
