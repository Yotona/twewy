#include "Combat/Noise/Private/BtlEnm010.h"
#include "Combat/Core/Combat.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"

#include <nitro/mi/cpumem.h>

// MARK: Data

/// A `TaskHandle` in the overlay's `.rodata`, referenced by the spawn entry point.
extern const TaskHandle data_ov011_0212bfa4;

// `data_ov003_020e71b8` is declared in `Combat/Core/Combat.h` as an `Ov003Global*`; the pool
// this overlay spawns into is a fixed bias into that global.
#define ENM010_POOL ((TaskPool*)((u32)data_ov003_020e71b8 + 0x118 + 0x10000))

// Spawns the one task this overlay owns and stashes the handle for the other entry points.
// The incoming halfword is handed over by pointer as the task's parameter.
void func_ov011_021256c0(u16 arg0) {
    u16   param         = arg0;
    u32   zero          = 0;
    Task* t             = EasyTask_CreateTask(ENM010_POOL, &data_ov011_0212bfa4, 0, 0, zero, &param);
    data_ov011_0212cca0 = t;
}

// Looks the task back up and marks it, setting bit 4 of its flags word. Predicated rather than
// branched in the original: the `cmp`/`ldrhne`/`orrne`/`strhne` chain.
void func_ov011_02125714(void) {
    Task* t = EasyTask_GetTaskById(ENM010_POOL, data_ov011_0212cca0);
    if (t != NULL) {
        *(u16*)((u8*)t + 4) |= 0x10;
    }
}

// MARK: Bin-file helpers (ov003), signatures read out of build/usa/asm/ov003_4.s

/// `func_ov003_02082cc4` -- flushes a sprite's pending animation and tail-calls
/// `Sprite_Release`. One argument.
extern void func_ov003_02082cc4(void* sprite);

/// `func_ov003_020cb32c` -- `BinMgr_FindById(binId) != 0`. One argument.
extern s32 func_ov003_020cb32c(void* binId);

/// `func_ov003_020cb348` -- looks up element `index` of the 8-byte table at `data + 8` and
/// returns the halfword at offset 0xA of the bin it finds, or 0.
extern s32 func_ov003_020cb348(BtlEnm010* data, u16 index);

/// `func_ov003_020cb304` -- the same lookup, but loads and releases the bin. Two arguments.
extern void func_ov003_020cb304(BtlEnm010* data, u16 index);

/// `func_ov003_020cb368` -- opens the bin through `FS_File*` and returns the file size,
/// or 0 for a null bin.
extern s32 func_ov003_020cb368(void* binId);

/// `func_ov003_020cb128` -- `*(s32*)p = a; *(s32*)(p + 4) = b;` Three arguments.
extern void func_ov003_020cb128(s32* p, void* a, s32 b);

/// `func_ov003_020cb200` -- decompresses element `index` of the table at `data + 8` into
/// `data->unk_00`. Two arguments.
extern void func_ov003_020cb200(BtlEnm010* data, u16 index);

// MARK: Data

/// A `TaskHandle` in the overlay's `.rodata`, referenced by the spawn entry point.
extern const TaskHandle data_ov011_0212bfa4;

/// `u16 data_ov011_0212bff4[]` and `u16 data_ov011_0212bfe0[]`, two halfword tables indexed by
/// the variant argument; `func_ov011_021258b4` has the same pair at `...bf78` / `...bf7e`.
extern const u16 data_ov011_0212bff4[];
extern const u16 data_ov011_0212bfe0[];

/// `void* data_ov011_0212cb1c[]` and `void* data_ov011_0212cae8[]`: 0x28-stride records
/// holding a bin id per (variant, mode) pair, read as `base[mode * 0x28 + unk_38]`.
extern const void* data_ov011_0212cb1c[];

/// `void* data_ov011_0212caf4[]`, indexed by the variant, holding an animation table.
extern const void* data_ov011_0212caf4[];

/// Picks the bin for `arg2` out of the task's table, makes sure it is loaded, and primes the
/// sprite. `arg2` is a "variant" index; `arg1` is the sprite record whose flag halfword the
/// function reads and then sets.
void func_ov011_02125750(s32 arg0, BtlEnm010Sprite* arg1, s32 arg2) {
    s32        flag = (arg1->unk_46 & 1) ? 1 : 0;
    BtlEnm010* data;
    void*      bin;
    s32        slot;
    s32        phase;

    func_ov003_02082cc4(arg1);
    data = EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0);
    bin  = data_ov011_0212cb1c[data->unk_38 * 0x28 + arg2];
    if (func_ov003_020cb32c(bin) == 0) {
        slot = 0;
        do {
            if (func_ov003_020cb348(data, ((u32)slot << 16) >> 16) <= 1) {
                break;
            }
            slot++;
        } while (slot < 2);
        func_ov003_020cb304(data, ((u32)slot << 16) >> 16);
        if (slot == 0) {
            phase = 0;
        } else {
            phase = data->unk_34 - func_ov003_020cb368(bin);
        }
        func_ov003_020cb128(&data->unk_14[slot][0], bin, phase);
        func_ov003_020cb200(data, ((u32)slot << 16) >> 16);
    }
    phase = data_ov011_0212bff4[arg2];
    if (data->unk_38 != 3) {
        phase = (u16)(phase + data->unk_38);
    }
    CombatSprite_LoadFromTable(arg0, (CombatSprite*)arg1, (const BinIdentifier*)bin,
                               (const SpriteAnimEntry*)data_ov011_0212caf4[arg2], 0, phase, data_ov011_0212bfe0[arg2]);
    if (flag == 1) {
        arg1->unk_46 |= 1;
    }
}
