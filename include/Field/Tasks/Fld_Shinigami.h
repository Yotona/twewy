#ifndef FIELD_TASKS_FLD_SHINIGAMI_H
#define FIELD_TASKS_FLD_SHINIGAMI_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 width;
    /* 0x06 */ u16 height;
    /* 0x08 */ u16 cellWidth;
    /* 0x0A */ u16 cellHeight;
    /* 0x0C */ u32 unk_0C[3];
    /* 0x18 */ u32 wallsOffset;
    /* 0x1C */ u32 unk_1C;
} ShiniGridInfo; // Size: 0x20

typedef struct {
    /* 0x0 */ s32 originX;
    /* 0x4 */ s32 originY;
    /* 0x8 */ u32 count;
    /* 0xC */ s32 offset;
} ShiniCell; // Size: 0x10

typedef struct {
    /* 0x00 */ FieldPos v[3];
    /* 0x18 */ s32      unk_18;
    /* 0x1C */ u16      category;
    /* 0x1E */ u16      attr;
} ShiniWall; // Size: 0x20

typedef struct {
    /* 0x00 */ u8*            indexTable;
    /* 0x04 */ ShiniGridInfo* grid;
    /* 0x08 */ ShiniCell*     cells;
    /* 0x0C */ ShiniWall*     walls;
    /* 0x10 */ u8*            unk_10;
} ShiniWorld; // Size: 0x14

void Fld_Shinigami_SetWorld(ShiniWorld* world, void* arg1, u8* file);
s32  Fld_Shinigami_MoveWithCollision(FieldPos* pos, s32 x, s32 y);
s32  Fld_Shinigami_GetFloorAttr(FieldPos* pos, u16* outAttr);
s32  Fld_Shinigami_IsOccluded(FieldPos* pos);
s32  Fld_Shinigami_CreateTask(TaskPool* pool, s32 kind);

#endif // FIELD_TASKS_FLD_SHINIGAMI_H
