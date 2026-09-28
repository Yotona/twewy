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

/// The per-sprite record the loaders take as their second argument. Only `unk_46` (a flag
/// halfword whose bit 0 is tested and then set) is known so far.
///
/// Evidence: `func_ov011_02125750` reads it at `ldrh r1, [r9, #0x46]` before the release
/// call and writes it back with a predicated `ldrh`/`orr`/`strh` triple, so the field really
/// is at 0x46 and is a halfword. Note that 0x46 is not a multiple of four, so the padding in
/// front of it has to be halfword-granular -- a `s32` pad array silently puts the field at
/// the next word boundary instead.
typedef struct BtlEnm010Sprite {
    /* 0x00 */ s32 pad_00[0x11];
    /* 0x44 */ u16 pad_44;
    /* 0x46 */ u16 unk_46;
} BtlEnm010Sprite;

#endif /* COMBAT_NOISE_PRIVATE_BTLENM010_H */
