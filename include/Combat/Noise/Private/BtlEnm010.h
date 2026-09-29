#ifndef COMBAT_NOISE_PRIVATE_BTLENM010_H
#define COMBAT_NOISE_PRIVATE_BTLENM010_H

#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"

/// A saved handle to the single task `BtlEnm010` spawns, so the other entry points can find it
/// again. Lives in the overlay's `.bss`.
///
/// `symbols.txt` gives this symbol the whole 0x20-byte `.bss` extent -- the auto-tagger folded
/// the unnamed statics that follow the handle into it -- so the object is declared with that
/// granularity, the handle being its first word (the same trick `BtlEnm006.c` uses for
/// `data_ov010_02129028`, and what objdiff compares against).
typedef struct BtlEnm010Bss {
    /* 0x00 */ Task* task;
    /* 0x04 */ u8    tail[0x1C];
} BtlEnm010Bss;

extern BtlEnm010Bss data_ov011_0212cca0;

// ---------------------------------------------------------------------------------------
// Data record shapes.  These live here rather than in `BtlEnm010.c` because the overlay's
// data is a translation unit of its own (`BtlEnm010Data.c`): the reference's codegen loads
// every table through pointers, so the definitions must not be visible to the code -- MWCC's
// `-ipa file` folds reads of in-file `const` tables into immediates and silently changes
// the generated code.
// ---------------------------------------------------------------------------------------

/// One 4-byte record of the aim table `data_ov011_0212c13c`, indexed by
/// `(0x1E - data->unk_1FA) / 5`. `unk_00` is loaded `ldrh` (u16) and added to the per-slot base
/// angle; `unk_02` is loaded `ldrsh` (s16) and passed through to the shot spawn unchanged.
typedef struct BtlEnm010RGAim {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ s16 unk_02;
} BtlEnm010RGAim;

/// The four RG phase entries as one 0x10-byte block of function pointers.
///
/// The size is pinned twice: `func_ov011_021278d4` copies exactly 16 bytes of it with a single
/// `ldm r0, {r0, r1, r2, r3} / stm`, and `data_ov011_0212c168` -- the table `func_ov011_02126bf8`
/// indexes -- starts 0x10 bytes later.
typedef struct BtlEnm010RGEntry {
    /* 0x00 */ void (*func[4])(void*, s32, s32);
} BtlEnm010RGEntry;

/// The 0x10-byte function table `data_ov011_0212c36c`, copied whole onto the stack with a
/// single `ldm`/`stm` pair and called through element `arg3`.
typedef s32 (*BtlEnm010Fn)(void*, void*, void*);

typedef struct BtlEnm010FnTable {
    BtlEnm010Fn f[4];
} BtlEnm010FnTable;

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
    /* 0x034 */ s32 pad_034[8];  // 0x34 .. 0x53
    /* 0x054 */ s32 unk_54;
    /* 0x058 */ s32 pad_058[90]; // 0x58 .. 0x1BF
    /* 0x1C0 */ s16 unk_1C0;
    /* 0x1C2 */ s16 unk_1C2;
    /* 0x1C4 */ s16 unk_1C4;
    /* 0x1C6 */ s16 pad_1C6;
    /* 0x1C8 */ void* (*unk_1C8)(void);
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ s32 unk_1F0;
    /* 0x1F4 */ s32 pad_1F4;
    /* 0x1F8 */ s16 unk_1F8;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ s16 pad_1FC;
    /* 0x1FE */ s16 pad_1FE;
    /* 0x200 */ s32 pad_200;
    /* 0x204 */ s32 unk_204;
    /* 0x206 */ u8  unk_206;
    /* 0x207 */ u8  pad_207;
    /* 0x208 */ s32 unk_208;
    /* 0x20C */ s32 unk_20C;
    /* 0x210 */ s32 unk_210;
} BtlEnm010UG;

/// `Tsk_BtlEnm010_RG` task data -- **0x208 bytes**. Two independent confirmations of the size:
/// the word in the `TaskHandle` at `0x0212c130`, and `func_ov011_0212791c`, the initialiser, which
/// opens with `mov r0, r4 / mov r1, #0 / mov r2, #0x208 / bl MI_CpuSet` -- the only write to the
/// whole block.
///
/// **Barely characterised.** The fields below have evidence, and each of them is pinned:
///   `0x80` a `u16` that selects one of four index tables -- `ldrh r2, [r0, #0x80]` feeding a
///          `cmp #3 / addls pc, pc, r2, lsl #2` jump table in `func_ov011_021265fc`. A `u16`
///          because the load is `ldrh`, not `ldrsh`.
///   `0x84` is passed to `func_ov011_02125750` and `Mini108_VBlank`, both of which take a
///          `CombatSprite*`, and `func_ov011_021268c4` reads `Sprite::frameTimer` at `0x8C` and
///          `Sprite::unk16` at `0x9A` through it -- so there is a `CombatSprite` here.
///          **Still raw padding, not a field**: `sizeof(CombatSprite)` is 0x7D, not the 0x60 this
///          overlay strides sprites by, so a member here would put the next field at 0x101
///          instead of 0x1C0. Reach it through a cast, the same way `BtlEnm010Lser` does.
///   `0x1C0` and `0x1C4` are zeroed together from a single `mov r1, #0` in `func_ov011_021265d4`,
///          reached through an `add r0, r5, #0x100` base -- so they are `s16`, and they are a pair.
///   `0x1C2` takes the `s32` return of `func_ov003_020c42ec` with a `strh`, so it is the
///          matching `s16` of the word at `0x1C0` -- `0x1C0` counts up to it.
///   `0x1C8` takes the second argument of that same function, which every call site fills with
///          the address of a function (`func_ov011_0212681c` from `0x021267f4`), so it is a
///          callback.
///   `0x1F4` a `s16` counter, read as `ldrsh` and incremented on every `func_ov011_021265fc` call,
///          and used as the index into the four tables after a `% 5` / `% 6` / `% 5` / `% 7`.
///   `0x1F6` a `s16` flag byte manipulated one bit at a time, always through the `0x100` base.
///   `0x1FC` compared against `mvn r0, #0` -- a `-1` sentinel, and the only thing that lets
///          `func_ov011_0212681c` run the phase picker instead of just bumping `0x1C0`.
///   `0x24` a word read as a plain flag: `ldr r1, [r4, #0x24] / cmp r1, #0 / bne` sends
///          `func_ov011_0212681c` down its bail-out arm.
///   `0x28` the y of a 4.12 position triple, the second argument to `func_ov003_020843b0`;
///          `0x2C` is its z, read by `func_ov011_02127bf0`.
///   `0x1D0` and `0x1D4` cleared together by `func_ov011_021268c4` when the `0x1C0` counter
///          reaches `0x1C2`; `0x1D0` is also tested `>= 0` there to build a 0/1 argument.
///   `0x1DC` is `func_ov003_020cb744(1) >> 1 - 0x40000`, `0x1E0` is `func_ov003_020cb7a4(1) >> 1`,
///          and `0x1E4` a zero -- all three written from one `mov r1, #0` in the same function.
///   `0x30` a countdown, written `-0x40000` by `func_ov011_02126a04` and then tested `< 0` to
///          decide whether the phase has finished. `0x1D0` is biased off the same `-0x40000`
///          expression by `+0x38000`, which is why that value stays in a register across both.
///   `0x1D8` a `0x2800` step written when `Sprite::unk16 == 3 && Sprite::frameTimer == 1`, and
///          zeroed when the phase ends. `0x1E8`/`0x1EC`/`0x1F0` are the second velocity triple:
///          `func_ov011_02127a64` adds them into `0x1D0`/`0x1D4`/`0x1D8`, having just added
///          `0x1D0`/`0x1D4`/`0x1D8` into the position at `0x28`/`0x2C`/`0x30` -- two chained
///          semi-implicit Euler steps, in that order.
///   `0x18C` a `u16` reached as `data + 0x100 + 0x8C`. Bit 4 tested, bit 5 set:
///          `tst r1, #0x10 / orrne r1, r1, #0x20 / strhne` -- a one-way latch.
///   `0x200` the exit sentinel; `func_ov011_02127a64` bumps `0x204` while it reads `-1`.
///   `0x54` a word whose bit 30 is set by the initialiser -- `orr r1, r1, #0x40000000` -- the
///          same engine-flags word `BtlEnm010Owner` has at its own `0x54`.
///   `0x1AC` and `0x1B0` are each written twice by the initialiser, once as the fresh value and
///          once copied into `0x28`/`0x2C`; `0x1B4` and `0x30` are each written with a single
///          zero. `0x1CC` is 1 and `0x1FC`/`0x200` are `0x1CC - 2`, so the `-1` sentinel is
///          computed from the field rather than written as a literal -- the original emits
///          `mov r0, #1 / sub r0, r0, #2` and would have used `mvn` for a plain `-1`.
typedef struct BtlEnm010RG {
    /* 0x000 */ s32 pad_000[9]; // 0x00 .. 0x23
    /* 0x024 */ s32 unk_024;
    /* 0x028 */ s32 unk_028;
    /* 0x02C */ s32 unk_02C;
    /* 0x030 */ s32 unk_030;
    /* 0x034 */ s32 pad_034[8];  // 0x34 .. 0x53
    /* 0x054 */ s32 unk_054;
    /* 0x058 */ s32 pad_058[10]; // 0x58 .. 0x7F
    /* 0x080 */ u16 unk_080;
    /* 0x082 */ u16 pad_082;
    /* 0x084 */ s32 pad_084[66]; // 0x84 .. 0x18B
    /* 0x18C */ u16 unk_18C;
    /* 0x18E */ u16 pad_18E;
    /* 0x190 */ s32 pad_190[7]; // 0x190 .. 0x1AB
    /* 0x1AC */ s32 unk_1AC;
    /* 0x1B0 */ s32 unk_1B0;
    /* 0x1B4 */ s32 unk_1B4;
    /* 0x1B8 */ s32 pad_1B8[2]; // 0x1B8 .. 0x1BF
    /* 0x1C0 */ s16 unk_1C0;
    /* 0x1C2 */ s16 unk_1C2;
    /* 0x1C4 */ s16 unk_1C4;
    /* 0x1C6 */ s16 pad_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct BtlEnm010RG*);
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 unk_1DC;
    /* 0x1E0 */ s32 unk_1E0;
    /* 0x1E4 */ s32 unk_1E4;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 unk_1EC;
    /* 0x1F0 */ s32 unk_1F0;
    /* 0x1F4 */ s16 unk_1F4;
    /* 0x1F6 */ s16 unk_1F6;
    /* 0x1F8 */ s16 unk_1F8;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ s32 unk_1FC;
    /* 0x200 */ s32 unk_200;
    /* 0x204 */ s32 unk_204;
} BtlEnm010RG;

/// `Tsk_BtlEnm010_Rnge` task data -- **0x6C bytes**, from the word in the `TaskHandle` at
/// `0x0212c1d4` and independently from `func_ov011_02127ce0`'s case 0, which opens with
/// `mov r0, r4 / mov r1, #0 / mov r2, #0x6c / bl MI_CpuSet`.
///
/// **A `CombatSprite` lives at `0x00`, and it is the task data pointer itself** -- the same
/// pointer is handed to `func_ov003_02082724`, `func_ov003_02082730`, `func_ov003_02082b64`,
/// `func_ov003_02082b0c`, `func_ov003_02082cc4`, `Mini108_VBlank` and
/// `SpriteMgr_IsAnimationFinished` with no bias at all. So the two `s16`s below are
/// `Sprite::frameTimer` and `Sprite::unk16`, reached as flat `ldrsh` at `0x08` and `0x16`.
/// The sprite is left as raw padding: `sizeof(CombatSprite)` is 0x7D and `0x60` is the owner
/// pointer.
typedef struct BtlEnm010Rnge {
    /* 0x00 */ s32                    pad_000[2];  // 0x00 .. 0x07, the CombatSprite
    /* 0x08 */ s16                    unk_008;     // Sprite::frameTimer
    /* 0x0A */ s16                    pad_00A;
    /* 0x0C */ s32                    pad_00C[2];  // 0x0C .. 0x13
    /* 0x14 */ s16                    pad_014;     // Sprite::animIndex
    /* 0x16 */ s16                    unk_016;     // Sprite::unk16
    /* 0x18 */ s32                    pad_018[18]; // 0x18 .. 0x5F
    /* 0x60 */ struct BtlEnm010Owner* unk_060;
    /* 0x64 */ s32                    pad_064;
    /* 0x68 */ s32                    pad_068;
} BtlEnm010Rnge;

/// `Tsk_BtlEnm010_Sprl` task data -- **0xB8 bytes**, from the word in the `TaskHandle` at
/// `0x0212c210`.
///
/// The layout is pinned by `func_ov011_02128b80` alone, the only function that touches it: a
/// five-iteration loop whose index `i` reaches the struct three ways at once --
/// `add r3, r10, r9, lsl #0x2` then a `+0x84` displacement, a `+0x70` displacement off the same
/// base, and a scaled index into `data_ov011_0212c21c`.
///
///   `0x04` a `CombatSprite` -- the same pointer is handed to `func_ov003_02082724`,
///        `func_ov003_02082730`, `func_ov003_0208260c` and `func_ov003_02082b64` with a `+0x4`
///        bias and nothing else. Raw padding: `sizeof(CombatSprite)` is 0x7D, which would run
///        past `0x70`.
///   `0x70` five `s32`s, read as `*(data + i * 4 + 0x70)` and passed to `func_ov003_02084348`.
///   `0x84` five `s32`s, read as `*(data + i * 4 + 0x84)`; the fourth goes to
///        `func_ov003_02082730` negated against `0x7FFFFFFF`.
///   `0x98` a single `s32`, read at a *fixed* offset while the two arrays above are indexed --
///         so it is the word just past the `0x84` array, not a sixth element of it.
typedef struct BtlEnm010Sprl {
    /* 0x000 */ s32 pad_000;     // 0x00 .. 0x03
    /* 0x004 */ s32 pad_004[27]; // 0x04 .. 0x6F, the CombatSprite
    /* 0x070 */ s32 unk_070[5];  // 0x70 .. 0x83
    /* 0x084 */ s32 unk_084[5];  // 0x84 .. 0x97
    /* 0x098 */ s32 unk_098;
    /* 0x09C */ s32 pad_09C[7];  // 0x9C .. 0xB7
} BtlEnm010Sprl;

/// `Tsk_BtlEnm010_SingleShot` task data -- **0xB4 bytes**, pinned by the `MI_CpuSet` in
/// `func_ov011_021283a8`, its own initialiser, which clears exactly 0xB4.
///
/// The `CombatSprite` is at `0x04`, not `0x00` -- the same shape as Sprl, and it is the only
/// offset the initialiser pins besides the tail block. The spawn block it is handed is
/// `BtlEnm010RngeArgs` (0x20): owner at `+0x00`, a 16-bit scale at `+0x10`, and `0x14`/`0x16` a
/// `u16`/`s16` pair.
typedef struct BtlEnm010SingleShot {
    /* 0x000 */ u32 pad_000[1];   // 0x00 .. 0x03
    /* 0x004 */ u8  pad_004[108]; // 0x04 .. 0x6F
    /* 0x070 */ s32 unk_070[3];   // 0x70 .. 0x7B
    /* 0x07C */ u8  pad_07C[24];  // 0x7C .. 0x93
    /* 0x094 */ u32 pad_094;      // 0x94 .. 0x97
    /* 0x098 */ s32 unk_098;
    /* 0x09C */ s32 unk_09C;
    /* 0x0A0 */ s32 unk_0A0;
    /* 0x0A4 */ s32 unk_0A4;
    /* 0x0A8 */ s16 unk_0A8;
    /* 0x0AA */ s16 unk_0AA;
    /* 0x0AC */ u16 unk_0AC;
    /* 0x0AE */ u16 unk_0AE;
    /* 0x0B0 */ u16 unk_0B0;
    /* 0x0B2 */ u16 unk_0B2;
} BtlEnm010SingleShot;

/// `Tsk_BtlEnm010_Tatt` task data -- **0x250 bytes**, from the word in the `TaskHandle` at
/// `0x0212c240`.
///
/// Barely characterised so far. `0x1C0`/`0x1C4`/`0x1C8` and `0x224`..`0x22C` are pinned by
/// `func_ov011_02129ed0` and `func_ov011_02128ca4`, which are the RG phase-setup pair again but
/// reached through a `+0x200` base -- the same shape as `func_ov011_021265d4`, which is why the
/// offsets come out the same. `0x224` is a word and the three after it are `s16`s zeroed from one
/// `mov r1, #0`.
typedef struct BtlEnm010Tatt {
    /* 0x000 */ s32 pad_000[21]; // 0x00 .. 0x53
    /* 0x054 */ s32 unk_54;
    /* 0x058 */ s32 pad_058[5];  // 0x58 .. 0x6B
    /* 0x06C */ s32 pad_06C[47]; // 0x6C .. 0x127
    /* 0x128 */ s32 pad_128[38]; // 0x128 .. 0x1BF
    /* 0x1C0 */ s16 unk_1C0;
    /* 0x1C2 */ s16 unk_1C2;
    /* 0x1C4 */ s16 unk_1C4;
    /* 0x1C6 */ s16 pad_1C6;
    /* 0x1C8 */ void (*unk_1C8)(struct BtlEnm010Tatt*);
    /* 0x1CC */ s32 unk_1CC;
    /* 0x1D0 */ s32 unk_1D0;
    /* 0x1D4 */ s32 unk_1D4;
    /* 0x1D8 */ s32 unk_1D8;
    /* 0x1DC */ s32 pad_1DC[7]; // 0x1DC .. 0x1F7
    /* 0x1F8 */ s16 unk_1F8;
    /* 0x1FA */ s16 unk_1FA;
    /* 0x1FC */ s16 unk_1FC;
    /* 0x1FE */ s16 pad_1FE;
    /* 0x200 */ s32 pad_200[2]; // 0x200 .. 0x207
    /* 0x208 */ s32 unk_208;
    /* 0x20C */ s32 pad_20C[6]; // 0x20C .. 0x223
    /* 0x224 */ s32 unk_224;
    /* 0x228 */ s16 unk_228;
    /* 0x22A */ s16 unk_22A;
    /* 0x22C */ s16 unk_22C;
    /* 0x22E */ s16 pad_22E;
    /* 0x230 */ s32 pad_230[7]; // 0x230 .. 0x24B
    /* 0x24C */ u8  unk_24C;
    /* 0x24D */ u8  pad_24D[3]; // 0x24D .. 0x24F
} BtlEnm010Tatt;

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
///   `0x22C`, `0x230`, `0x234` accumulators, and `0x248` a fourth; all four are read-modify-
///          written by `func_ov011_021260e8`, the mode-2 worker.
///   `0x23C`, `0x240` zeroed from a single `mov r0, #0`.
///   `0x244` the spawn argument's `unk_10`, negated when the owner's `unk_24` is clear
///          (`ldreq / rsbeq / streq`).
///   `0x24C` the spawn argument's `unk_14`.
///   `0x250` a byte whose bit 3 tracks `func_ov003_020c37f8(owner + 0x84)`
///          (`orrne / biceq`), and whose bit 1 is set by all three mode workers.
///   `0x004` a 0x7C-byte copy of the owner, refreshed every frame by `func_ov011_02125e14`.
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
    /* 0x22C */ s32                unk_22C;
    /* 0x230 */ s32                unk_230;
    /* 0x234 */ s32                unk_234;
    /* 0x238 */ s32                pad_238;
    /* 0x23C */ s32                unk_23C;
    /* 0x240 */ s32                unk_240;
    /* 0x244 */ s32                unk_244;
    /* 0x248 */ s32                unk_248;
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