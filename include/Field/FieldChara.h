#ifndef FIELD_FIELDCHARA_H
#define FIELD_FIELDCHARA_H

// Field character sprites (player, partner): a Sprite driven by a table of
// pack-entry cells. The helpers live in Fld_ScanKirakira.c.

#include "Engine/File/BinMgr.h"
#include "SpriteMgr.h"

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

void func_ov030_020be1d8(FldCharaSprite* chara, BinIdentifier* binIden, FldCharaCell* cells, s32 index, u16 palette,
                         u16 charBase);
s32  func_ov030_020be258(FldCharaSprite* chara, u16 cell, s32 animationMode);
void func_ov030_020be368(Sprite* sprite, s32 flip);
s32  func_ov030_020be3c0(Sprite* sprite);
void func_ov030_020be3dc(FldCharaSprite* chara, s32 sortKey);
void func_ov030_020be3e4(Sprite* sprite, s32 alpha);
void func_ov030_020be430(s32* x, s32* y, s32 angle, s32 dist);

#endif // FIELD_FIELDCHARA_H
