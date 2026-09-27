#ifndef COMBAT_NOISE_PRIVATE_BTLENM006_H
#define COMBAT_NOISE_PRIVATE_BTLENM006_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// One of the `Tsk_BtlEnm006` asset variants.
typedef struct Enm006Variant {
    /* 0x00 */ BinIdentifier*   binIden;
    /* 0x04 */ SpriteAnimEntry* animTable;
    /* 0x08 */ u16              unk_08;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ u16              unk_0C;
    /* 0x0E */ u16              unk_0E;
} Enm006Variant; // Size: 0x10

typedef struct BtlEnm006 BtlEnm006;

/// Arguments handed to the `Tsk_BtlEnm006_*` task constructors.
typedef struct Enm006Spawn {
    /* 0x00 */ BtlEnm006* unk_00;
    /* 0x04 */ void*      unk_04;
    /* 0x08 */ s32        unk_08;
    /* 0x0C */ s32        unk_0C;
} Enm006Spawn;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM006_H */
