#ifndef FIELD_FLASH_FOFILELIST_H
#define FIELD_FLASH_FOFILELIST_H

#include <nitro/types.h>

extern const u32 FO3DViewer_ScenePointCounts[47];

void* FOFileList_Create(void);
void  FOFileList_Destroy(void);
u32   FOFileList_Open(const char* path, BOOL loadTable);
void  FOFileList_Close(void);
void* FOFileList_ReadEntry(void* buffer, u32 wantedIndex);
u32   FOFileList_GetEntryCount(void);
void* FOList_Alloc(s32 size);
void  FOList_Free(void* data);

#endif // FIELD_FLASH_FOFILELIST_H
