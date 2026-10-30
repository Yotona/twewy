#ifndef COMBAT_CORE_COMBATSPRITE_H
#define COMBAT_CORE_COMBATSPRITE_H

#include "Engine/File/BinMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "SpriteMgr.h"

typedef struct SpriteAnimEntry {
    u16 charDataIndex;
    u16 paletteDataIndex;
    u16 frameDataIndex;
    s16 animDataIndex;
} SpriteAnimEntry;

typedef struct {
    /* 0x00 */ Sprite                 sprite;
    /* 0x40 */ SpriteAnimEntry*       animTable;
    /* 0x44 */ s16                    animTableIndex;
    /* 0x46 */ u16                    flags46;
    /* 0x48 */ s32                    sortKey; // frame info sortKey, -1 = unsorted
    /* 0x4C */ struct OamAffineParam* affine;  // frame info affine (3D path)
    /* 0x50 */ UnkSmallInternal*      basePaletteSource;
    /* 0x54 */ PaletteResource*       basePalette;
    /* 0x58 */ s32                    paletteMode;
    /* 0x5C */ s16                    paletteTimer;
    /* 0x5E */ char                   unk_5E[0x2];
} CombatSprite;

typedef struct {
    SpriteAnimation        anim;
    const SpriteAnimEntry* animTable; // 0x2C
} SpriteAnimationEx;

/// MARK: Functions

s32 CombatSprite_SetAnimFromTable(CombatSprite* cSprite, u16 animTableIndex, s32 arg2);

s32 CombatSprite_SetAnim(CombatSprite* cSprite, u16 animTableIndex, s32 arg2);

void CombatSprite_SetAffineTransform(CombatSprite* cSprite, u32 rotation, s32 scaleX, s32 scaleY, s32 arg4);

void CombatSprite_SetPaletteSource(CombatSprite* cSprite, s32 arg1);

void CombatSprite_SetPosition(CombatSprite* cSprite, s32 posX, s32 posY);

void func_ov003_02082730(CombatSprite* cSprite, s32 arg1);

void func_ov003_02082738(CombatSprite* cSprite, s32 arg1);

void CombatSprite_SetFlip(CombatSprite* cSprite, s32 arg1);

void CombatSprite_SetPaletteMode(CombatSprite* cSprite, s32 arg1);

void CombatSprite_RenderWithPalette(CombatSprite* cSprite, s32 arg1);

SpriteFrameInfo* CombatSprite_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

void CombatSprite_InitAnim(SpriteAnimation* anim, s32 arg1, const BinIdentifier* iden);

s32 CombatSprite_Load(CombatSprite* cSprite, SpriteAnimationEx* anim);

s32 CombatSprite_LoadFromTable(s32 arg0, CombatSprite* arg1, const BinIdentifier* arg2, const SpriteAnimEntry* arg3, u16 arg4,
                               u16 arg5, u16 arg6);

void CombatSprite_LoadDirect(s32 arg0, CombatSprite* cSprite, const BinIdentifier* iden, s32 arg3, u16 arg4, u16 arg5,
                             u16 arg6, u16 arg7);

#define CombatSprite_LoadFromTableNoProto                                                                                   \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, const SpriteAnimEntry*, s32, s32, s32))CombatSprite_LoadFromTable)

#define CombatSprite_LoadDirectNoProto                                                                     \
    ((void (*)(s32, CombatSprite*, const BinIdentifier*, s32, s32, s32, s32, s32))CombatSprite_LoadDirect)

void CombatSprite_Init(CombatSprite* cSprite);

void CombatSprite_Update(CombatSprite* cSprite);

void CombatSprite_Render(CombatSprite* cSprite);

void CombatSprite_Release(CombatSprite* cSprite);

void CombatSprite_Restart(CombatSprite* cSprite);

/// Where a battle sprite comes from: its file, animation table, entry and palette slot.
/// The sprite descriptor `func_ov003_020ccf94` loads from.
typedef struct BtlSpriteDesc {
    /* 0x0 */ const BinIdentifier*   iden;
    /* 0x4 */ const SpriteAnimEntry* table;
    /* 0x8 */ u16                    entry;
    /* 0xA */ u16                    palette;
    /* 0xC */ u16                    paletteCount;
} BtlSpriteDesc;

#endif // COMBAT_CORE_COMBATSPRITE_H
