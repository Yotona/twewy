#ifndef COMBAT_NOISE_PRIVATE_BTLENM010_H
#define COMBAT_NOISE_PRIVATE_BTLENM010_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// A saved handle to the single task `BtlEnm010` spawns, so the other entry points can find it
/// again. Lives in the overlay's `.bss`.
extern Task* data_ov011_0212cca0;

// ---------------------------------------------------------------------------------------
// Task data structs.
//
// The overlay owns nine tasks and **every one of them has a different data size**, written
// out literally in its `TaskHandle` in `.rodata` as the word after the two function pointers.
// That is the single most useful fact about this overlay: it settles the struct sizes without
// having to guess, and it settles which struct any given function is walking.
//
//   handle    name                    task entry point          data size
//   0212bfa4  Tsk_BtlEnm010_AnmMgr    func_ov011_021259d0       0x03C
//   0212c118  Tsk_BtlEnm010_Lser      func_ov011_02125cf8       0x254
//   0212c130  Tsk_BtlEnm010_RG        func_ov011_021278d4       0x208
//   0212c1d4  Tsk_BtlEnm010_Rnge      func_ov011_02127ce0       0x06C
//   0212c1e0  Tsk_BtlEnm010_SWA       func_ov011_0212801c       0x020
//   0212c1f8  Tsk_BtlEnm010_SingleShot func_ov011_02128348      0x0B4
//   0212c210  Tsk_BtlEnm010_Sprl      func_ov011_02128758       0x0B8
//   0212c240  Tsk_BtlEnm010_Tatt      func_ov011_02129934       0x250
//   0212c448  Tsk_BtlEnm010_UG        func_ov011_0212b99c       0x214
//
// The three most-used ones, with the evidence for every offset, are below. Cross-check any
// offset against `build/usa/asm/ov011_4.s` before trusting it -- `build/scratch/census2.py
// <func>` lists every field access of a named function.
// ---------------------------------------------------------------------------------------

/// One 8-byte record of `BtlEnm010AnmMgr`'s bin table. `func_ov003_020cb128` is literally
/// `stm r0, {r1, r2}`, so both words are written as a pair, which is why the table has to be
/// a struct of two words rather than two parallel arrays.
///
/// Evidence: `func_ov011_02125750` reaches slot 4's *neighbours* as `ldr r1, [r5, #0x34]` and
/// `ldrh r3, [r5, #0x38]`; `func_ov011_021258b4` reaches the table through the single indexed
/// `add r0, r5, #0x14 / add r0, r0, r6, lsl #3`, with the index running 0..2 and 2..3
/// respectively, so only four slots exist. `0x34`/`0x38` are the two fields *after* the table,
/// not a fifth slot.
typedef struct BtlEnm010AnmSlot {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
} BtlEnm010AnmSlot;

/// `Tsk_BtlEnm010_AnmMgr` task data -- **0x3C bytes**, the size in the `TaskHandle` at
/// `0x0212bfa4`. `func_ov011_021256c0` spawns this task and `func_ov011_02125714` finds it
/// again, both through `data_ov011_0212cca0`.
///
/// `unk_00`, `unk_04` and `unk_08` are established from `func_ov003_020cb200`, which walks all
/// three, and from `func_ov003_020cb150`, which allocates `unk_04` bytes off `gMainHeap` and
/// stores the result in `unk_00`. So `unk_00` is the decompression buffer, `unk_04` its size
/// and `unk_08` an **8-byte-stride** table of `{binId, offset}` records.
/// `func_ov003_020cb348`/`020cb304` confirm `unk_08` and its stride, and `func_ov003_020cb194`
/// reads the halfword count at `unk_0C`. `func_ov011_02125a08` fills all of it.
typedef struct BtlEnm010AnmMgr {
    /* 0x00 */ void*            unk_00;
    /* 0x04 */ s32              unk_04;
    /* 0x08 */ void*            unk_08;
    /* 0x0C */ s32              unk_0C;
    /* 0x10 */ s32              unk_10;
    /* 0x14 */ BtlEnm010AnmSlot unk_14[4];
    /* 0x34 */ s32              unk_34;
    /* 0x38 */ u16              unk_38;
    /* 0x3A */ u16              pad_3A;
} BtlEnm010AnmMgr;

/// `Tsk_BtlEnm010_UG` task data -- **0x214 bytes**, the size in the `TaskHandle` at
/// `0x0212c448`.
///
/// This is the struct whose `0x28`/`0x2C`/`0x30` are a position and which therefore looked
/// like it clashed with `BtlEnm010AnmMgr`'s slot table. It does not: different task.
///
/// Evidence, all from `func_ov011_0212bac8`, the per-phase handler whose four entry points
/// are tabulated at `0x0212c36c`:
///   `0x1D0/0x1D4/0x1D8` are added into `0x28/0x2C/0x30` every frame, and `0x1E8/0x1EC/0x1F0`
///   are added into `0x1D0/0x1D4/0x1D8` -- so `0x28/0x2C/0x30` is position x/y/z and
///   `0x1D0..0x1F0` are two velocity triples.
///   `0x1C8` is a callback (`blx r1`, guarded by a `cmp #0` / `beq`).
///   `0x1CC` receives the result of `func_ov003_020c4668` and is returned to the caller.
///   `0x206` is a `u8` whose bit 0 is tested with an `lsl #0x1f / lsr #0x1f` pair.
///   `0x208` is passed as an address to `EasyTask_ValidateTaskId`.
///   `0x20C` is compared against `~0`; `0x210` is incremented when `0x20C != ~0`, so
///   `0x210` + 4 = `0x214` is the last word of the struct -- which is an independent check
///   on the size in the `TaskHandle`.
typedef struct BtlEnm010UG {
    /* 0x000 */ s32 pad_000[10]; // 0x00 .. 0x27
    /* 0x028 */ s32 unk_028;
    /* 0x02C */ s32 unk_02C;
    /* 0x030 */ s32 unk_030;
    /* 0x034 */ s32 pad_034[110]; // 0x34 .. 0x1C3
    /* 0x1C4 */ s32 pad_1C4;
    /* 0x1C8 */ void* (*unk_1C8)(void);
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 pad_1DC[3];
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ s32 unk_1F0;
    /* 0x1F4 */ s32 pad_1F4[3];
    /* 0x200 */ s32 pad_200;
    /* 0x204 */ s32 pad_204;
    /* 0x206 */ u8  unk_206;
    /* 0x207 */ u8  pad_207;
    /* 0x208 */ s32 unk_208;
    /* 0x20C */ s32 unk_20C;
    /* 0x210 */ s32 unk_210;
} BtlEnm010UG;

/// The owner object every task in this overlay is hung off. `Task+0x18` / the spawn argument's
/// first word point at one of these.
///
/// Two roles so far, and both are consistent with the offsets:
///   - `func_ov011_02125c00` receives a *copy* of one (see `BtlEnm010OwnerCopy`) and reads
///     `unk_24`..`unk_30` as a 4.12 position triple behind a mirror flag, then biases `unk_28`
///     by `+/- 0x40000`.
///   - `func_ov011_02125d48` reads `unk_84` (two bits, used as a sprite-variant selector) and
///     `unk_24` off the spawn argument's pointer.
/// `unk_54` is the engine-flags word: `func_ov011_02125e14` tests its bit 2 through the owner
/// pointer, and `func_ov003_020cc354` -- which the same function calls on the *copy* -- tests
/// bits 2 and 8 of the copy's own `0x54` and reads a countdown at its `0x5A`.
typedef struct BtlEnm010Owner {
    /* 0x00 */ s32 pad_00[9];
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 pad_34[8];
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ s32 unk_58;
    /* 0x5C */ s16 unk_5C;
    /* 0x5E */ s16 pad_5E;
    /* 0x60 */ s32 pad_60[9];
    /* 0x84 */ u32 unk_84;
} BtlEnm010Owner;

/// The **0x7C-byte prefix** of `BtlEnm010Owner` that `func_ov011_02125e14` bulk-copies out of
/// `data->unk_00` into the task's own block every time the owner is still valid.
///
/// The copy loop is `ldm r6!, {r0,r1,r2,r3} / stm lr!, {r0,r1,r2,r3}` seven times followed by
/// `ldm r6, {r0,r1,r2} / stm lr, {r0,r1,r2}` -- 7 * 16 + 12 = **0x7C** bytes, which is what pins
/// the size and makes it a struct assignment rather than a hand-written loop.
///
/// It really is a *prefix*: `0x84` is reachable on the owner but not inside the copy, which is
/// why `func_ov011_02125d48` reads it through the pointer and not through `data + 4`.
typedef struct BtlEnm010OwnerCopy {
    /* 0x00 */ s32 pad_00[9];
    /* 0x24 */ s32 unk_24;
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s32 pad_34[8];
    /* 0x54 */ s32 unk_54;
    /* 0x58 */ s32 pad_58[9];
} BtlEnm010OwnerCopy;

/// One 0xC-byte per-emitter record inside the Lser task's data, at `LserData + 0x200` with a
/// 0xC stride, indexed three deep.
///
/// Evidence: `func_ov011_02125c44` reads and writes all three fields, and its caller
/// `func_ov011_02125f24` builds the pointer as `r4 + 0x200` and steps it by `0xC` while it
/// steps the matching sprite by `0x60`. `unk_04` accumulates `unk_08` each call and is
/// compared against `0x8000`, so it is a 4.12 angle or distance.
typedef struct BtlEnm010LserRec {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 pad_02;
    /* 0x04 */ s32 unk_04;
    /* 0x08 */ s32 unk_08;
} BtlEnm010LserRec;

/// The three emitter records plus one halfword, at `LserData + 0x200`.
///
/// The halfword is written as `add r0, r7, #0x200 / strh r1, [r0, #0x28]`, i.e. reached
/// through the 0x200 base rather than as a flat `0x228` offset, which is why it lives in a
/// sub-struct here. It is set from `args->unk_14 != 0`.
///
/// The trailing `pad_2A` exists only to make the struct 0x2C bytes: without it the next
/// `s32` field of `BtlEnm010Lser` cannot start at 0x23C, and every offset from 0x23C to the end
/// comes out 0x38 too high.
typedef struct BtlEnm010LserEmit {
    /* 0x00 */ BtlEnm010LserRec rec[3];
    /* 0x24 */ s16              unk_24;
    /* 0x26 */ s16              unk_26;
    /* 0x28 */ s16              unk_28;
    /* 0x2A */ u16              pad_2A;
} BtlEnm010LserEmit;

/// `Tsk_BtlEnm010_Lser` task data -- **0x254 bytes**, the size in the `TaskHandle` at
/// `0x0212c118`, and confirmed independently by `func_ov011_02125d48`'s
/// `MI_CpuSet(data, 0, 0x254)`, which is the only write to the whole block.
///
/// `func_ov011_02125d48` is the task's initialiser and touches every field below, so each one
/// carries its own evidence:
///   `0x000` the owner, copied from the spawn argument.
///   `0x080` four `CombatSprite`s, 0x60 apart, primed by `func_ov011_021258b4` in a loop whose
///          pointer step is a literal `add r5, r5, #0x60`.
///   `0x200` the emitter block (`BtlEnm010LserEmit`).
///   `0x23C`, `0x240` zeroed from a single `mov r0, #0`.
///   `0x244` the spawn argument's `unk_10`, negated when the owner's `unk_24` is clear
///          (`ldreq / rsbeq / streq`).
///   `0x24C` the spawn argument's `unk_14`.
///   `0x250` a byte whose bit 3 tracks `func_ov003_020c37f8(owner + 0x84)`
///          (`orrne / biceq`).
///
/// **The sprite block is raw padding, not `CombatSprite sprite[4]`.** `sizeof(CombatSprite)`
/// in this header is 0x7D, not the 0x60 the ROM uses, because the `Sprite` bitfield block
/// overruns its 0x40 allocation -- so an array of the type puts the next field at 0x274
/// instead of 0x200. Reach the elements through a walking `CombatSprite*` instead; the stride
/// is the ROM's, not `sizeof`.
typedef struct BtlEnm010Lser {
    /* 0x000 */ BtlEnm010Owner*    unk_00;
    /* 0x004 */ BtlEnm010OwnerCopy copy;
    /* 0x080 */ s32                pad_080[0x60]; /* four CombatSprite, 0x60 apart */
    /* 0x200 */ BtlEnm010LserEmit  emit;
    /* 0x22C */ s32                pad_22C[4];
    /* 0x23C */ s32                unk_23C;
    /* 0x240 */ s32                unk_240;
    /* 0x244 */ s32                unk_244;
    /* 0x248 */ s32                pad_248;
    /* 0x24C */ s32                unk_24C;
    /* 0x250 */ u8                 unk_250;
    /* 0x251 */ u8                 pad_251[3];
} BtlEnm010Lser;

/// The 0x18-byte spawn-argument record `func_ov011_02125b98` hands to the new
/// `Tsk_BtlEnm010_Lser` task.
///
/// `func_ov011_02125b98` writes only `unk_00` and `unk_14`; `func_ov011_02125d48` (the task's
/// initialiser) then reads `unk_00`, `unk_10` and `unk_14`. The words at `0x04..0x0F` are
/// untouched by anything, and the record must **not** be zero-initialised -- doing that emits
/// four stores the original does not have.
typedef struct BtlEnm010LserArgs {
    /* 0x00 */ BtlEnm010Owner* unk_00;
    /* 0x04 */ s32             pad_04[3];
    /* 0x10 */ s32             unk_10;
    /* 0x14 */ s32             unk_14;
} BtlEnm010LserArgs;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM010_H */
