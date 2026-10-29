#ifndef COMBAT_CORE_COMBATBGRES_H
#define COMBAT_CORE_COMBATBGRES_H

#include "Combat/Core/CombatSpriteSlot.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"

/// One command of a resource-loading script: `run` advances `state->cursor` past its own operands.
/// The operands are two words of packed halfwords; their meaning depends on `run`.
typedef struct Ov003ResCmd {
    /* 0x00 */ s32 (*run)(void* state);
    /* 0x04 */ u32 operands0;
    /* 0x08 */ u32 operands1;
} Ov003ResCmd; // Size: 0xC

/// State of one background/palette resource set (main engine and sub engine each own one).
typedef struct Ov003BgResState {
    /* 0x00 */ s32                engine; // display engine the resources belong to
    /* 0x04 */ BinIdentifier*     iden;   // pack the graphics come from
    /* 0x08 */ const Ov003ResCmd* cursor; // next script command
    /* 0x0C */ void*              scriptArg;
    /* 0x10 */ Data*              data;
    /* 0x14 */ BgResource*        charRes[6];
    /* 0x2C */ PaletteResource*   palettes[3];
    /* 0x38 */ Ov003SpriteSlot    slots[2];
    /* 0x88 */ s16                unk_88;
    /* 0x8A */ s16                paletteCount;
} Ov003BgResState; // Size: 0x8C

#endif
