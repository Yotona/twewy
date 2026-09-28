#ifndef COMBAT_NOISE_PRIVATE_BTLENM010_H
#define COMBAT_NOISE_PRIVATE_BTLENM010_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// A saved handle to the single task `BtlEnm010` spawns, so the other entry points can find it
/// again. Lives in the overlay's `.bss`.
extern Task* data_ov011_0212cca0;

/// The `BtlEnm010` task's data block.
///
/// Only the fields the implemented functions actually reach are named; everything else is
/// explicit padding so that a later function adding a field cannot silently shift the
/// offsets that are already correct. The record table at `0x14` has an 8-byte stride and is
/// indexed with a `<< 3`, so it is spelled as `s32[3][2]`; `func_ov011_021258b4` reaches
/// indices 2..4, which run into the fields at `0x2C`/`0x34`, so that function will need an
/// overlapping view of the same 0x14..0x3B range. Nothing that has been implemented so far
/// depends on that, so it is left as a note rather than guessed at.
typedef struct BtlEnm010 {
    /* 0x00 */ void* unk_00;
    /* 0x04 */ void* unk_04;
    /* 0x08 */ void* unk_08;
    /* 0x0C */ s32   pad_0C[2];
    /* 0x14 */ s32   unk_14[3][2];
    /* 0x2C */ s32   unk_2C;
    /* 0x30 */ s32   unk_30;
    /* 0x34 */ s32   unk_34;
    /* 0x38 */ u16   unk_38;
    /* 0x3A */ u16   pad_3A;
    /* 0x3C */ s32   pad_3C;
} BtlEnm010;

/// The per-sprite record the loaders take as their second argument. Only `unk_46` (a flag
/// halfword whose bit 0 is tested and then set) is known so far.
typedef struct BtlEnm010Sprite {
    /* 0x00 */ s32 pad_00[0x23];
    /* 0x46 */ u16 unk_46;
} BtlEnm010Sprite;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM010_H */
