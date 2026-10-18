#ifndef FIELD_TASKS_FLDCHARA_H
#define FIELD_TASKS_FLDCHARA_H

#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"

// Field character sprites (player, partner): a Sprite driven by a table of pack-entry cells.

// One animation of a character: pack entries of its char/anim/cell data.
typedef struct {
    /* 0x0 */ u16 charIndex;
    /* 0x2 */ u16 animIndex; // pack entry of the animation data
    /* 0x4 */ u16 cellIndex; // pack entry of the cell table
    /* 0x6 */ s16 anim;      // animation within the data, 0-based
} FldCharaCell;              // Size: 0x8

typedef struct {
    /* 0x00 */ Sprite        sprite;
    /* 0x40 */ FldCharaCell* cells;
    /* 0x44 */ s16           curCell;
    /* 0x46 */ s16           unk46;
    /* 0x48 */ s32           sortKey;
    /* 0x4C */ s32           unk4C;
} FldCharaSprite; // Size: 0x50

void FldChara_Load(FldCharaSprite* owner, BinIdentifier* binIden, FldCharaCell* cells, s32 index, u16 palette, u16 charBase);
s32  FldChara_SetCell(FldCharaSprite* owner, u16 arg1, s32 arg2);
void FldChara_SetFlip(Sprite* sprite, s32 arg1);
s32  FldChara_IsFlipped(Sprite* sprite);
void FldChara_SetSortKey(FldCharaSprite* owner, s32 sortKey);
void FldChara_RenderWithAlpha(Sprite* sprite, s32 arg1);
void FldChara_MoveAlongAngle(s32* arg0, s32* arg1, s32 arg2, s32 arg3);

#endif // FIELD_TASKS_FLDCHARA_H
