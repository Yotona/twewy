#ifndef FIELD_FLASH_FO3DVIEWERIMD_H
#define FIELD_FLASH_FO3DVIEWERIMD_H

#include "Field/Flash/FO3D.h"

extern FieldViewerState* gFO3DViewer;

void*             FO3DViewerImd_Alloc(s32 size);
void              FO3DViewerImd_Free(void* data);
FieldViewerState* FO3DViewerImd_Create(void);
void              FO3DViewerImd_Destroy(void);

#endif // FIELD_FLASH_FO3DVIEWERIMD_H
