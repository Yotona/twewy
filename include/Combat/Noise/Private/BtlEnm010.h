#ifndef COMBAT_NOISE_PRIVATE_BTLENM010_H
#define COMBAT_NOISE_PRIVATE_BTLENM010_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// A saved handle to the single task `BtlEnm010` spawns, so the other entry points can find it
/// again. Lives in the overlay's `.bss`.
extern Task* data_ov011_0212cca0;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM010_H */
