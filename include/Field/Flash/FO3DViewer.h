#ifndef FIELD_FLASH_FO3DVIEWER_H
#define FIELD_FLASH_FO3DVIEWER_H

#include <nitro/fx.h>

void  FO3DViewer_LoadToVram(void);
void  FO3DViewer_LoadExtra(s32 scene, const char* path);
void  FO3DViewer_Init(s32 scene);
BOOL  FO3DViewer_Update(s32 x, s32 y);
void  FO3DViewer_SetOffset(s32 x, s32 y);
BOOL  FO3DViewer_GetNodePos(const char* name, Vec* pos);
s32   FO3DViewer_GetAnimFrame(const char* name);
void  FO3DViewer_RestartAnim(const char* name);
void  FO3DViewer_Show(const char* name);
void  FO3DViewer_Hide(const char* name);
void  FO3DViewer_Destroy(void);
void* FOFileIO_Alloc(s32 size);
void  FOFileIO_Free(void* data);
void  FOFileIO_Release(void* data);

#endif // FIELD_FLASH_FO3DVIEWER_H
