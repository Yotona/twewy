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

/// `func_ov003_020cb32c` -- `BinMgr_FindById(binId) != 0`.
///
/// **Two** arguments, not one: the body is `push {r3, lr} / mov r0, r1 / bl BinMgr_FindById`,
/// so the bin id arrives in **r1** and the first argument is ignored. Both call sites in this
/// overlay pass the task data as argument 1 and leave it in `r0`, which is why the original
/// emits a bare `mov r1, r4` and no `mov r0`.
extern s32 func_ov003_020cb32c(BtlEnm010AnmMgr* data, void* binId);

/// `func_ov003_020cb348` -- looks up element `index` of the 8-byte table at `data + 8` and
/// returns the halfword at offset 0xA of the bin it finds, or 0.
extern s32 func_ov003_020cb348(BtlEnm010AnmMgr* data, u16 index);

/// `func_ov003_020cb304` -- the same lookup, but loads and releases the bin. Two arguments.
extern void func_ov003_020cb304(BtlEnm010AnmMgr* data, u16 index);

/// `func_ov003_020cb368` -- opens the bin through `FS_File*` and returns the file size,
/// or 0 for a null bin.
extern s32 func_ov003_020cb368(void* binId);

/// `func_ov003_020cb128` -- `*(s32*)p = a; *(s32*)(p + 4) = b;` Three arguments.
extern void func_ov003_020cb128(s32* p, void* a, s32 b);

/// `func_ov003_020cb200` -- decompresses element `index` of the table at `data + 8` into
/// `data->unk_00`. Two arguments.
extern void func_ov003_020cb200(BtlEnm010AnmMgr* data, u16 index);

// MARK: Data

/// A `TaskHandle` in the overlay's `.rodata`, referenced by the spawn entry point.
extern const TaskHandle data_ov011_0212bfa4;

/// `u16 data_ov011_0212bff4[]` and `u16 data_ov011_0212bfe0[]`, two halfword tables indexed by
/// the variant argument.
extern const u16 data_ov011_0212bff4[];
extern const u16 data_ov011_0212bfe0[];

/// `u16 data_ov011_0212bf78[]` and `u16 data_ov011_0212bf7e[]`: `func_ov011_021258b4`'s pair,
/// passed as the element offset and the palette index of the `CombatSprite_LoadFromTable`
/// call. Note the two tables are only six bytes apart, so they have to be separate symbols.
extern const u16 data_ov011_0212bf78[];
extern const u16 data_ov011_0212bf7e[];

/// `void* data_ov011_0212cb1c[]` holds 0x28-stride records of bin ids, read as
/// `base[unk_38 * 0x28 + variant]`. `data_ov011_0212cae8[]` is indexed by the variant alone.
extern const void* data_ov011_0212cb1c[];
extern const void* data_ov011_0212cae8[];

/// `void* data_ov011_0212caf4[]` and `data_ov011_0212cadc[]`, indexed by the variant, holding
/// an animation table.
extern const void* data_ov011_0212caf4[];
extern const void* data_ov011_0212cadc[];

/// Picks the bin for `arg2` out of the `AnmMgr` task's table, makes sure it is loaded, and
/// primes the sprite. `arg2` is a "variant" index; `arg1` is the sprite record whose flag
/// halfword the function reads and then sets.
///
/// The two values below deliberately do not share a variable. Written as one `phase`, the slot
/// size and the anim phase are the same live value, so MWCC keeps it in a callee-saved
/// register for the whole function and every `data`-relative load moves up one register.
void func_ov011_02125750(s32 arg0, BtlEnm010Sprite* arg1, s32 arg2) {
    s32              flag = (arg1->unk_46 & 1) ? 1 : 0;
    void*            bin;
    BtlEnm010AnmMgr* data;
    s32              slot;
    s32              size;
    u16              phase;

    func_ov003_02082cc4(arg1);
    data = EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0);
    // The original builds the record pointer first (`mla r1, r3, r1, r2`) and then indexes
    // it by the variant (`ldr r4, [r1, r8, lsl #2]`).  Spelled as a single subscript
    // `base[mode * 0x28 + arg2]` MWCC instead folds the variant into the displacement and
    // emits `mla r0, r2, r0, r8` / `ldr r4, [r1, r0, lsl #2]` -- same value, six wrong
    // instructions.  The bias has to be spelled as a pointer.
    bin = ((const void* const*)((const u8*)data_ov011_0212cb1c + data->unk_14[4].unk_04 * 0x28))[arg2];
    if (func_ov003_020cb32c(data, bin) == 0) {
        slot = 0;
        do {
            if (func_ov003_020cb348(data, (u16)slot) <= 1) {
                break;
            }
            slot++;
        } while (slot < 2);
        func_ov003_020cb304(data, (u16)slot);
        if (slot == 0) {
            size = 0;
        } else {
            size = data->unk_14[4].unk_00 - func_ov003_020cb368(bin);
        }
        func_ov003_020cb128(&data->unk_14[slot].unk_00, bin, size);
        func_ov003_020cb200(data, (u16)slot);
    }
    phase = data_ov011_0212bff4[arg2];
    if (data->unk_14[4].unk_04 != 3) {
        phase = phase + data->unk_14[4].unk_04;
    }
    CombatSprite_LoadFromTable(arg0, (CombatSprite*)arg1, (const BinIdentifier*)bin,
                               (const SpriteAnimEntry*)data_ov011_0212caf4[arg2], 0, phase, data_ov011_0212bfe0[arg2]);
    if (flag == 1) {
        arg1->unk_46 |= 1;
    }
}

/// `func_ov011_021258b4` is `func_ov011_02125750` with three differences: the bin table is
/// indexed by the variant alone instead of through the 0x28-stride bias, the free-slot search
/// runs from 2 to 4 instead of 0 to 2, and the tail passes the two halfword tables straight
/// through without the mode adjustment or the flag write-back.
void func_ov011_021258b4(s32 arg0, CombatSprite* arg1, s32 arg2) {
    void*            bin;
    BtlEnm010AnmMgr* data;
    s32              slot;
    s32              size;
    u16              elem;
    u16              pal;

    data = EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0);
    bin  = data_ov011_0212cae8[arg2];
    if (func_ov003_020cb32c(data, bin) == 0) {
        slot = 2;
        do {
            if (func_ov003_020cb348(data, (u16)slot) <= 1) {
                break;
            }
            slot++;
        } while (slot < 4);
        func_ov003_020cb304(data, (u16)slot);
        if (slot == 2) {
            size = data->unk_14[4].unk_00;
        } else {
            // Split into two statements so that the load of `unk_04` is issued *before* the
            // call.  In one compound expression MWCC sinks it past the call and keeps the
            // value in a scratch register; the original loads it into a callee-saved one.
            s32 base = data->unk_04;
            size     = base - func_ov003_020cb368(bin);
        }
        func_ov003_020cb128(&data->unk_14[slot].unk_00, bin, size);
        func_ov003_020cb200(data, (u16)slot);
    }
    elem = data_ov011_0212bf78[arg2];
    pal  = data_ov011_0212bf7e[arg2];
    CombatSprite_LoadFromTable(arg0, arg1, (const BinIdentifier*)bin, (const SpriteAnimEntry*)data_ov011_0212cadc[arg2], 0,
                               elem, pal);
}
