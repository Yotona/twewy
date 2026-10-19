#ifndef FIELD_FLASH_FOALLOCATOR_H
#define FIELD_FLASH_FOALLOCATOR_H

#include "Field/Flash/FO3D.h"

void                 FOAllocator_Init(void);
FieldAllocatorState* FOAllocator_Get(void);
BOOL                 FO3D_LoadToVram(void* resource);

#endif // FIELD_FLASH_FOALLOCATOR_H
