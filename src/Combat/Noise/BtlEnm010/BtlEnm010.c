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

// MARK: Declarations and notes, hoisted above the definitions so every
// function can be emitted in address order (the linker places functions
// in source order).  Order within this block is unchanged.

/// A 0xC-byte record out of the table `func_ov003_0208a114` walks (`base + index * 12`) -- the
/// same table `BtlEnm006.c` names `Enm006CmdTbl`. Only the bytes below are identified: callers
/// copy the whole record (`ldm`/`stm` of three words), overwrite the gray triple at `+5`..`+7`,
/// and hand the copy to `func_ov003_0208a164`/`func_ov003_0208a1a4`.
///
/// The two `s32` words are load-bearing: a byte-array record has alignment 1, and MWCC then
/// copies it with a *byte loop* instead of the `ldm`/`stm` pair the original has.
typedef struct BtlEnm010CmdTbl {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8  unk_04;
    /* 0x05 */ u8  unk_05;
    /* 0x06 */ u8  unk_06;
    /* 0x07 */ u8  unk_07;
    /* 0x08 */ s32 unk_08;
} BtlEnm010CmdTbl;

/// One 4-byte record of the aim table `data_ov011_0212c13c`, indexed by
/// `(0x1E - data->unk_1FA) / 5`. `unk_00` is loaded `ldrh` (u16) and added to the per-slot base
/// angle; `unk_02` is loaded `ldrsh` (s16) and passed through to the shot spawn unchanged.
typedef struct BtlEnm010RGAim {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ s16 unk_02;
} BtlEnm010RGAim;

/// `func_ov003_0208a114` -- one argument; returns a pointer to the 0xC-byte record at
/// `base + index * 12` (`mla r0, 0xc, r0, table; bx lr`). The `u16` parameter is load-bearing:
/// call sites truncate a computed selector with `lsl #0x10 / lsr #0x10`.
extern const BtlEnm010CmdTbl* func_ov003_0208a114(u16 idx);

/// `func_ov003_0208a164` -- five arguments (r0-r3 plus one stack word), returning 1 on success.
extern s32 func_ov003_0208a164(BtlEnm010CmdTbl* rec, void* p, s32 x, s32 y, s32 z);

/// `func_ov003_0208a1a4` -- four register arguments, returning 1 on success; submits the command
/// record at integer pixel x/y.
extern s32 func_ov003_0208a1a4(const BtlEnm010CmdTbl* rec, void* p, s32 x, s32 y);

/// `func_ov003_020cc7f4` -- four arguments in r0-r3; a zero return stops the caller.
extern s32 func_ov003_020cc7f4(s32 a0, s32 a1, s32 a2, s32 a3);

/// `func_ov003_020cbcb4` -- writes a polar angle/radius pair into two x/y out-arrays; five
/// arguments, the fifth on the caller's stack.
extern void func_ov003_020cbcb4(s32* outX, s32* outY, u16 angle, s32 radius, s32 scale);

/// `func_ov003_0208442c` -- two arguments; projects a screen x into the cull test space.
extern s32 func_ov003_0208442c(s32 a0, s32 a1);

/// `func_ov003_020cbd30` -- evaluates a three-point 4.12 curve (six words) at `t` into the
/// two-word out-point. Eight words to the callee (r0-r3 plus five stack words).
extern void func_ov003_020cbd30(s32* out, s32 x0, s32 y0, s32 x1, s32 y1, s32 x2, s32 y2, s32 t);

/// `func_ov003_020cba14` -- four arguments; returns the 16-bit angle from (x0,y0) to (x1,y1).
extern u16 func_ov003_020cba14(s32 x0, s32 y0, s32 x1, s32 y1);

extern s32 FX_Divide(s32 numer, s32 denom);

/// Aim table for `func_ov011_02126bf8`'s spread shot; 4-byte records.
extern const BtlEnm010RGAim data_ov011_0212c13c[];

/// Per-mode shot cap (`func_ov011_02126fb0`, `func_ov011_02126bf8`), indexed by `unk_080`.
extern const s32 data_ov011_0212c148[];
extern const s32 data_ov011_0212c168[];

/// `func_ov011_02129188`'s curve data: two 8-byte origins and two 6-word (3 x/y point) curves.
extern const s32 data_ov011_0212c230[];
extern const s32 data_ov011_0212c25c[];
extern const s32 data_ov011_0212c274[];

/// Per-slot count/limit tables for the Tatt phases.
extern const s32 data_ov011_0212c33c[];
extern const s32 data_ov011_0212c34c[];

/// The `[slot][shot]` 16-byte-record parameter table's four field views (the reference keeps
/// one biased base per field: four literal-pool entries four bytes apart).
extern const s32 data_ov011_0212c3c8[];
extern const u16 data_ov011_0212c3cc[];
extern const s32 data_ov011_0212c3d0[];
extern const s16 data_ov011_0212c3d4[];

// Forward declarations.  The definitions are in address order, so callers
// of a later-address function would otherwise see an implicit declaration.
void       func_ov011_021256c0(u16 arg0);
void       func_ov011_02125714(void);
void       func_ov011_02125750(s32 arg0, CombatSprite* arg1, s32 arg2);
void       func_ov011_021258b4(s32 arg0, CombatSprite* arg1, s32 arg2);
s32        func_ov011_021259d0(s32 arg0, Task* task, s32 arg2, s32 cmd);
s32        func_ov011_02125a08(BtlEnm010AnmMgr* data, u16* arg1);
s32        func_ov011_02125b88(BtlEnm010AnmMgr* data);
s32        func_ov011_02125b98(void* arg0, s32 arg1);
void       func_ov011_02125c00(s32* outX, s32* outY, s32* outZ, BtlEnm010OwnerCopy* owner, s32 index);
void       func_ov011_02125c44(BtlEnm010OwnerCopy* owner, BtlEnm010LserRec* rec, CombatSprite* sprite, s32 enabled);
s32        func_ov011_02125cf8(s32 arg0, Task* task, s32 arg2, s32 cmd);
s32        func_ov011_02125d48(BtlEnm010Lser* data, BtlEnm010LserArgs* args);
s32        func_ov011_02125e14(BtlEnm010Lser* data);
s32        func_ov011_02125f24(BtlEnm010Lser* data);
s32        func_ov011_02126064(BtlEnm010Lser* data);
s32        func_ov011_02126354(BtlEnm010Lser* data);
s32        func_ov011_021265a8(BtlEnm010Lser* data);
void       func_ov011_021265d4(BtlEnm010RG* data, void (*func)(struct BtlEnm010RG*));
void       func_ov011_021265fc(BtlEnm010RG* data);
void       func_ov011_021267f4(BtlEnm010RG* data);
void       func_ov011_0212681c(BtlEnm010RG* data);
void       func_ov011_021268c4(BtlEnm010RG* data);
void       func_ov011_02126a04(BtlEnm010RG* data);
void       func_ov011_02126aec(BtlEnm010RG* data);
void       func_ov011_02126b2c(BtlEnm010RG* data);
void       func_ov011_02126e80(BtlEnm010RG* data);
void       func_ov011_02127240(BtlEnm010RG* data);
void       func_ov011_02127370(BtlEnm010RG* data);
void       func_ov011_02127474(BtlEnm010RG* data);
void       func_ov011_02127628(BtlEnm010RG* data);
void       func_ov011_02127698(BtlEnm010RG* data);
void       func_ov011_021276e0(BtlEnm010RG* data);
s32        func_ov011_02127758(void* p, s32 arg1);
s32        func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3);
void       func_ov011_021278d4(void* p, s32 arg1, s32 arg2, s32 index);
s32        func_ov011_0212791c(void* arg0, void* arg1, s32 arg2);
s32        func_ov011_02127a64(void* arg0, void* arg1);
s32        func_ov011_02127b98(void* arg0, void* arg1);
s32        func_ov011_02127bdc(void* arg0, void* arg1);
s32        func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1);
s32        func_ov011_02127c4c(BtlEnm010RG* data);
s32        func_ov011_02127c84(void* p);
s32        func_ov011_02127ce0(void* arg0, void* arg1, s32 arg2, s32 index);
s32        func_ov011_02127f6c(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5, u16 arg6, s32 arg7, s16 arg8);
s32        func_ov011_0212801c(void* arg0, void* arg1, s32 arg2, s32 index);
s32        func_ov011_02128070(void* p, void* a);
s32        func_ov011_02128150(void* p);
s32        func_ov011_02128250(void* p);
s32        func_ov011_021282b8(void* arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s32 arg5, s16 arg6);
s32        func_ov011_02128348(void* arg0, void* arg1, s32 arg2, s32 index);
s32        func_ov011_021283a8(BtlEnm010SingleShot* data, void* arg1);
s32        func_ov011_02128698(void* p);
s32        func_ov011_02128704(void* p);
s32        func_ov011_02128718(void* p);
s32        func_ov011_02128758(void* arg0, void* arg1, s32 arg2, s32 index);
s32        func_ov011_021287b8(BtlEnm010Sprl* data, void* arg1);
s32        func_ov011_02128b80(BtlEnm010Sprl* data);
s32        func_ov011_02128c30(void* p);
s32        func_ov011_02128c44(void* p);
void       func_ov011_02128ca4(BtlEnm010Tatt* data, s32 arg1);
void       func_ov011_02128cc0(BtlEnm010Tatt* data);
void       func_ov011_02128e30(BtlEnm010Tatt* data);
void       func_ov011_02128eb0(BtlEnm010Tatt* data);
void       func_ov011_02128f10(void* p, u16 v);
void*      func_ov011_02128f80(BtlEnm010Tatt* data);
void       func_ov011_02129110(void* p);
void       func_ov011_021293a8(void* arg0, void* p);
s32        func_ov011_02129934(void* arg0, void* arg1, s32 arg2, s32 index);
s32        func_ov011_02129994(BtlEnm010Tatt* data, void* arg1);
s32        func_ov011_02129b84(BtlEnm010Tatt* data);
s32        func_ov011_02129cec(BtlEnm010Tatt* data);
s32        func_ov011_02129ea4(void* p);
void       func_ov011_02129ed0(BtlEnm010Tatt* data, void (*func)(BtlEnm010Tatt*));
void       func_ov011_02129ef8(BtlEnm010Tatt* data);
void       func_ov011_02129f80(BtlEnm010Tatt* data);
void       func_ov011_0212a10c(BtlEnm010Tatt* data);
void       func_ov011_0212a134(BtlEnm010Tatt* data);
void       func_ov011_0212a2ec(BtlEnm010Tatt* data);
void       func_ov011_0212a420(BtlEnm010Tatt* data);
void       func_ov011_0212a540(BtlEnm010Tatt* data);
void       func_ov011_0212a634(BtlEnm010Tatt* data);
void       func_ov011_0212a674(BtlEnm010Tatt* data);
void       func_ov011_0212aa20(BtlEnm010Tatt* data);
void       func_ov011_0212aee8(BtlEnm010Tatt* data);
void       func_ov011_0212af94(BtlEnm010Tatt* data);
void       func_ov011_0212b0a4(BtlEnm010Tatt* data);
void       func_ov011_0212b168(BtlEnm010Tatt* data);
void       func_ov011_0212b318(BtlEnm010Tatt* data);
void       func_ov011_0212b388(BtlEnm010Tatt* data);
void       func_ov011_0212b3ec(BtlEnm010Tatt* data);
void       func_ov011_0212b4f4(BtlEnm010Tatt* data);
void       func_ov011_0212b558(BtlEnm010Tatt* data);
s32        func_ov011_0212b5d8(BtlEnm010Tatt* data, s32 arg1);
s32        func_ov011_0212b6e0(BtlEnm010Tatt* data, s32 arg1);
s32        func_ov011_0212b800(BtlEnm010Tatt* data, s32 arg1);
s32        func_ov011_0212b890(BtlEnm010Tatt* data, s16* p, s16* q, s32 arg3);
s32        func_ov011_0212b99c(void* arg0, void* arg1, void* arg2, s32 index);
void       func_ov011_0212b9e4(void* arg0, void* arg1, void* arg2);
s32        func_ov011_0212bac8(void* arg0, void* arg1);
s32        func_ov011_0212bc84(void* arg0, void* arg1);
s32        func_ov011_0212bcc8(void* arg0, void* arg1);
s16        func_ov011_0212bce0(BtlEnm010Tatt* data, s32 arg1);
void       func_ov011_0212bd3c(void* p, s32 i);
void       func_ov011_0212bd90(void* p, s32 i);
s32        func_ov011_0212bdbc(void* p, s32 i);
extern s32 func_ov011_021260e8(BtlEnm010Lser* data);

// MARK: Bin-file helpers (ov003), signatures read out of build/usa/asm/ov003_4.s

/// `CombatSprite_Release` (`func_ov003_02082cc4`) -- flushes a sprite's pending animation and
/// tail-calls `Sprite_Release`. One argument. Declared in `Combat/Core/CombatSprite.h`.

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

// MARK: The AnmMgr task's own entry point and its commands

/// `func_ov011_02125a08` -- the AnmMgr task's command 0. Declared here so that
/// `func_ov011_021259d0` can call it; it is defined further down, in address order.
extern s32 func_ov011_02125a08(BtlEnm010AnmMgr* data, u16* arg1);

/// `func_ov003_020cb194` -- walks the 8-byte table at `p + 8` and releases every element,
/// using the halfword count at `p + 0xC`. One argument.
extern void func_ov003_020cb194(void* p);

/// `func_ov003_020cb130` -- builds the pool descriptor: `dst[0] = 0`, `dst[4] = arg3`,
/// `dst[8] = arg1`, `dst[0xC] = (u16)arg2`, `dst[0x10] = arg4`. **Five** arguments; the fifth
/// is passed on the caller's stack.
extern void func_ov003_020cb130(void* dst, void* a, u16 b, s32 c, void* e);

/// `func_ov003_020cb150` -- allocates `data->unk_04` bytes off `gMainHeap`, stores the result
/// in `data->unk_00`, and returns whether it succeeded. One argument.
extern s32 func_ov003_020cb150(void* p);

/// The heap name `func_ov011_02125a08` hands to the pool allocator, and the two bin tables it
/// measures: the 0x28-stride one (ten records per mode) and the flat three-entry one.
extern const char data_ov011_0212cbd4[];

/// A pair of running maxima, kept in the frame. The original zeroes each pair with a single
/// `str` pair through a base pointer (`add r4, sp, #0xc / str r1, [r4, #0] / str r1, [r4, #4]`)
/// and then reads and writes the halves at constant frame slots, so the two maxima are one
/// 8-byte object, not two scalars. Spelled as two `s32` locals MWCC promotes all four to
/// callee-saved registers instead and the loop bodies lose their stores entirely.
typedef struct BtlEnm010Pair {
    s32 max;
    s32 sec;
} BtlEnm010Pair;

/// The `TaskHandle` of the `Tsk_BtlEnm010_Lser` task (0x254 bytes of data), the task this
/// function spawns.
extern const TaskHandle data_ov011_0212c118;

/// `func_ov003_020c37f8` -- reads the two-bit field at offset 0 of its argument and returns
/// whether it is 1. One argument.
extern s32 func_ov003_020c37f8(void* p);

// MARK: Tsk_BtlEnm010_Lser

/// `s32 data_ov011_0212c124[3]` = `{0xFFFF8000, 0xFFFF0000, 0}`, a three-entry offset table.
extern const s32 data_ov011_0212c124[];

/// The other three `Tsk_BtlEnm010_Lser` command handlers, dispatched by `func_ov011_02125cf8`.
extern s32 func_ov011_02125e14(BtlEnm010Lser* data);
extern s32 func_ov011_02126354(BtlEnm010Lser* data);
extern s32 func_ov011_021265a8(BtlEnm010Lser* data);

/// `func_ov011_02125d48` -- the Lser task's command-0 initialiser. Declared ahead of
/// `func_ov011_02125cf8`, which dispatches to it, and defined below in address order.
extern s32 func_ov011_02125d48(BtlEnm010Lser* data, BtlEnm010LserArgs* args);

/// `Mini108_VBlank` lives in ov000; the project spells it with this name. Declared here for
/// the same reason ov010 declares it locally: there is no header for ov000's symbols.
extern void Mini108_VBlank(CombatSprite* cSprite, u16 arg1, s32 arg2);

/// `func_ov003_020c3c28` -- no arguments; returns a global mode bit.
extern s32 func_ov003_020c3c28(void);

/// `func_ov003_020cc354` -- one argument; bails out of the copy block if its `0x54` flags are
/// set or its `0x5A` countdown is non-positive, and returns whether it bailed.
extern s32 func_ov003_020cc354(void* p);

/// `CombatSprite_Update` (`func_ov003_02082b0c`) -- one argument, a `CombatSprite*`; ticks a
/// palette timer behind the sprite's `flags46` bit 12. Declared in `Combat/Core/CombatSprite.h`.

/// `func_ov003_020843b0` -- two arguments; turns a 4.12 y coordinate into a sound pan value.
extern s16 func_ov003_020843b0(s32 a, s32 b);

/// `func_ov003_020843ec` -- three arguments; the same projection as `func_ov003_020843b0` but
/// taking a y/z pair, so the `mode` argument selects the projection plane.
extern s32 func_ov003_020843ec(s32 a, s32 b, s32 c);

/// `func_ov003_02084348` -- six arguments: `mode`, two `s16*` out-parameters, then a raw
/// x/y/z triple. The two out-parameters are written through the pointers and read back as
/// `ldrsh`, so they are `s16`, not `s32`.
extern s32 func_ov003_02084348(s32 a, s16* b, s16* c, s32 d, s32 e, s32 f);

/// `func_ov003_020cbc50` -- four arguments. The third is truncated with `lsl #0x10 / lsr #0x10`
/// on entry, so it is a `u16` parameter; the callee never reads it again after that.
extern s32 func_ov003_020cbc50(s32* a, s32* b, u16 c, s32 d);

/// `CombatSprite_SetPosition` (`func_ov003_02082724`) -- three arguments;
/// `strh r1, [r0, #0xc] / strh r2, [r0, #0xe]`. Declared in `Combat/Core/CombatSprite.h`.

/// `CombatSprite_Render` (`func_ov003_02082b64`) -- one argument, a `CombatSprite*`; ticks the
/// sprite. Declared in `Combat/Core/CombatSprite.h`.

/// The fixed-point rounding idiom shared with `BtlEnm014`: convert a 4.12 value to `f32`, bias it
/// by a half, and truncate. The sign test is repeated in both arms, so the value itself is
/// re-derived per arm -- here that means `func_ov003_020843b0` is called three times.
#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

/// `func_ov003_020c427c` -- a two-argument thunk: `ldr ip, .L / bx ip` straight into
/// `func_ov003_020c37bc` with r0/r1 untouched.
extern void func_ov003_020c427c(void* p, void* arg1);

extern void func_ov011_02126b2c(BtlEnm010RG* data);
extern void func_ov011_02126bf8(BtlEnm010RG* data);
extern void func_ov011_02126e80(BtlEnm010RG* data);
extern void func_ov011_02126fb0(BtlEnm010RG* data);
extern void func_ov011_02127240(BtlEnm010RG* data);
extern void func_ov011_02127370(BtlEnm010RG* data);
extern void func_ov011_02127474(BtlEnm010RG* data);
/// The four per-mode index tables `func_ov011_021265fc` walks. Their sizes are pinned by the
/// table literal addresses and the `0x0212c1d4` `TaskHandle` that follows the last of them:
/// `0x0212c178` 5 entries, `0x0212c18c` 5, `0x0212c1a0` 6, `0x0212c1b8` 7.
extern const s32 data_ov011_0212c178[];
extern const s32 data_ov011_0212c18c[];
extern const s32 data_ov011_0212c1a0[];
extern const s32 data_ov011_0212c1b8[];

/// `func_ov003_020c5bfc` -- no arguments; a non-zero return aborts the caller.
extern s32 func_ov003_020c5bfc(void);

/// `func_ov003_020c42ec` -- one argument, the task data; its `s32` return is stored with a
/// `strh` at `0x1C2`, so only its low halfword survives.
extern s32 func_ov003_020c42ec(void* p);

extern void func_ov011_02126aec(BtlEnm010RG* data);
extern void func_ov011_0212681c(BtlEnm010RG* data);

extern s32 func_ov003_020cb744(s32 arg);
extern s32 func_ov003_020cb7a4(s32 arg);
extern s32 func_ov003_020c4ab4(BtlEnm010RG* data, s32 arg1);
extern s32 func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1);

extern s32  func_ov003_020c4c9c(void* p);
extern void func_ov003_020c48b0(void* p);
extern void func_ov003_020c492c(void* p);
extern s32  func_ov003_020cb520(void* p, s32 arg1);
extern s32  func_ov003_020cb594(void* p, s32 arg1);

extern s32 func_ov011_02127758(void* p, s32 arg1);

extern s32 func_ov003_020c6230(void* p);
extern s32 func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3);
extern s32 func_ov011_02127c84(void* p);

extern s32 func_ov003_020c703c(void* p);

/// The four RG phase entries as one 0x10-byte block of function pointers.
///
/// The size is pinned twice: `func_ov011_021278d4` copies exactly 16 bytes of it with a single
/// `ldm r0, {r0, r1, r2, r3} / stm`, and `data_ov011_0212c168` -- the table `func_ov011_02126bf8`
/// indexes -- starts 0x10 bytes later.
typedef struct BtlEnm010RGEntry {
    /* 0x00 */ void (*func[4])(void*, s32, s32);
} BtlEnm010RGEntry;

extern const BtlEnm010RGEntry data_ov011_0212c158;

extern s32 func_ov003_020c62c4(void* p, s32 arg1);
extern s32 func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);

// `CombatSprite_SetFlip` (`func_ov003_02082750`) takes TWO arguments; its first instruction
// clobbers r2 (`ldrh r2, [r0, #0xa]`) and both call sites in the original set only r0/r1.
// Declared in `Combat/Core/CombatSprite.h`.
extern s32 func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

extern s32 func_ov003_020c3efc(void* p, void* arg1);
extern s32 func_ov003_020c44ac(void* p);
extern s32 func_ov003_020c4b1c(void* p);

extern void func_ov003_020c4748(void* p);
extern s32  func_ov003_0208810c(void* p, void* arg1);
extern s32  func_ov003_020cba54(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern s32  func_ov003_020cb910(void* a0, void* a1, void* a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);

// `CombatActor_PopPendingCommand` (`CombatActor_PopPendingCommand`) -- declared in
// `Combat/Core/CombatActor.h`; the RG phase dispatcher reads its result as a phase number.
extern s32              func_ov003_020c4628(void* p);
extern const TaskHandle data_ov011_0212c1d4;

extern s32 func_ov011_02128070(void* p, void* arg1);
extern s32 func_ov011_02128150(void* p);
extern s32 func_ov011_02128250(void* p);

extern const TaskHandle data_ov011_0212c1e0;

/// The 0x20-byte spawn block `func_ov011_02127f6c` builds for the SWA task, and `func_ov011_02127f6c`'s
/// own outgoing argument. One struct, not nine scalars: the original writes all nine words out of
/// one contiguous `sp+0x8` frame slot and then hands `sp+0x8` itself to `EasyTask_CreateTask`.
///
/// The `u16` at `+0x1C` and the `s16` at `+0x16` are byte fields in the middle of it -- which is
/// why the struct cannot be all `s32`.
typedef struct BtlEnm010RngeArgs {
    /* 0x00 */ void* field_00;
    /* 0x04 */ s32   field_04;
    /* 0x08 */ s32   field_08;
    /* 0x0C */ s32   field_0C;
    /* 0x10 */ s32   field_10;
    /* 0x14 */ u16   field_14;
    /* 0x16 */ s16   field_16;
    /* 0x18 */ s32   field_18;
    /* 0x1C */ u16   field_1C;
    /* 0x1E */ u16   pad_1E;
} BtlEnm010RngeArgs;

/// The 0x18-byte spawn block `func_ov011_021282b8` builds -- the same shape as
/// `BtlEnm010RngeArgs` with the trailing `u16` pair and its pad left off. The size is pinned by the
/// frame: `push {r3, r4, r5, r6, r7, lr}` then `sub sp, sp, #0x20` puts the block at `sp+0x8` and
/// its last written byte at `sp+0x1F`, so it is 0x18 bytes and not 0x20. Nothing is ever stored to
/// what would have been offset `0x1C`.
typedef struct BtlEnm010SprlArgs {
    /* 0x00 */ void* field_00;
    /* 0x04 */ s32   field_04;
    /* 0x08 */ s32   field_08;
    /* 0x0C */ s32   field_0C;
    /* 0x10 */ s32   field_10;
    /* 0x14 */ u16   field_14;
    /* 0x16 */ s16   field_16;
} BtlEnm010SprlArgs;

extern const TaskHandle data_ov011_0212c210;

extern const TaskHandle data_ov011_0212c1f8;

extern s32 func_ov011_021283a8(BtlEnm010SingleShot* data, void* arg1);
extern s32 func_ov011_021284bc(void* p);
extern s32 func_ov011_02128698(void* p);

extern const s32 data_ov011_0212c1ec[];

extern s32 func_ov011_021287b8(BtlEnm010Sprl* data, void* arg1);
extern s32 func_ov011_021288c8(void* p);
extern s32 func_ov011_02128b80(BtlEnm010Sprl* data);
extern s32 func_ov011_02128c30(void* p);

extern const s32 data_ov011_0212c21c[];

extern const TaskHandle data_ov011_0212c240;

extern s32  func_ov011_02129994(BtlEnm010Tatt* p, void* arg1);
extern s32  func_ov011_02129b84(BtlEnm010Tatt* data);
extern s32  func_ov011_02129cec(BtlEnm010Tatt* p);
extern void func_ov011_0212a134(BtlEnm010Tatt* data);
extern s32  func_ov011_0212b800(BtlEnm010Tatt* data, s32 arg1);

extern void*     func_ov011_02128f80(BtlEnm010Tatt* data);
extern const u16 data_ov011_0212c28c[];
extern const u16 data_ov011_0212c28e[];
extern const u16 data_ov011_0212c290[];

extern void func_ov011_02129188(void* p, void* rec);
extern void func_ov011_021293a8(void* p, void* rec);
extern void func_ov011_02129410(void* p, void* rec);

extern s32  func_ov003_020c6bc8(BtlEnm010Tatt* data, s32 arg1);
extern void func_ov011_0212b388(BtlEnm010Tatt* data);

extern s32 func_ov003_020c6c2c(BtlEnm010Tatt* data, s32 arg1);

extern s32 func_ov003_020c7070(BtlEnm010Tatt* data);

extern s32  func_ov003_020c4c9c(void* p);
extern void func_ov003_020c48b0(void* p);
extern void func_ov003_020c492c(void* p);

/// The varargs-forwarding trampoline. It copies a sixteen-byte table of four entry points onto
/// the stack with a single `ldm`/`stm` pair and calls through element `arg3` -- the `ldm`/`stm`
/// is MWCC's whole-aggregate copy, which is why the four assignments below are written as one
/// struct assignment rather than four element stores.
///
/// The task data is **not** pinned, so it is taken as an opaque pointer. If a seventh task
/// struct turns up, this is one of its entries.
typedef s32 (*BtlEnm010Fn)(void*, void*, void*);

typedef struct BtlEnm010FnTable {
    BtlEnm010Fn f[4];
} BtlEnm010FnTable;

extern const BtlEnm010FnTable data_ov011_0212c36c;

extern const s16 data_ov011_0212c30c[];
extern const s16 data_ov011_0212c30e[];

extern s32 func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);

extern void func_ov011_02128ca4(BtlEnm010Tatt* data, s32 arg1);
extern void func_ov011_02128eb0(BtlEnm010Tatt* data);

extern void func_ov011_021256c0(u16 arg0);
extern void func_ov003_020c4520(void* p);
extern void func_ov003_020c4b5c(void* p);

extern s32 func_ov011_0212b890(BtlEnm010Tatt* data, s16* arg1, s16* arg2, s32 arg3);

extern s32 func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

extern s32       func_ov003_020cb888(void* p, s32 arg1, s32 arg2);
extern const u32 data_ov011_0212cc7c[];
extern void      func_ov011_0212a134(BtlEnm010Tatt* data);
extern void      func_ov011_0212a2ec(BtlEnm010Tatt* data);
extern void      func_ov011_0212a420(BtlEnm010Tatt* data);
extern void      func_ov011_0212a634(BtlEnm010Tatt* data);

extern s32 func_ov003_020cb594(void* p, s32 arg1);

extern s16 func_ov011_0212bce0(BtlEnm010Tatt* data, s32 arg1);

extern s32       func_ov003_020cb744(s32 arg0);
extern s32       func_ov003_020cb7a4(s32 arg0);
extern s32       func_ov003_020c6b8c(BtlEnm010Tatt* data, s32 arg1);
extern const s32 data_ov011_0212c35c[];
extern void      func_ov011_0212aa20(BtlEnm010Tatt* data);
extern s32       func_ov011_0212b6e0(BtlEnm010Tatt* data, s32 arg1);
extern s32       func_ov011_0212b5d8(BtlEnm010Tatt* data, s32 arg1);

extern s32 func_ov003_02084348(s32 a0, s16* a1, s16* a2, s32 a3, s32 a4, s32 a5);
extern s32 func_ov003_020cc7c0(s32 a0, s32 a1, s32 a2);

extern s32  func_ov003_020c42ec(void* p);
extern s32  func_ov003_020c4348(void* p);
extern void func_ov011_0212b168(BtlEnm010Tatt* data);

extern void func_ov011_0212a78c(BtlEnm010Tatt* data);
extern void func_ov011_0212ac0c(BtlEnm010Tatt* data);
extern void func_ov011_0212aee8(BtlEnm010Tatt* data);
extern s32  func_ov011_0212bdbc(void* p, s32 i);
extern void func_ov011_02129f80(BtlEnm010Tatt* data);

extern const s32 data_ov011_0212c37c[];
extern const s32 data_ov011_0212c390[];
extern const s32 data_ov011_0212c330[];
extern const s32 data_ov011_0212c3a8[];

extern s32       func_ov003_020c3c28(void);
extern void      func_ov011_02129110(void* p);
extern void      func_ov011_02128e30(BtlEnm010Tatt* data);
extern void      func_ov011_02128f10(void* p, u16 v);
extern const s32 data_ov011_0212c24c[];

extern void func_ov003_02084694(void* p, s32 arg1);
extern s32  func_ov003_020c3bf0(void* p);
extern s32  func_ov003_020c4668(void* p);

extern s32 func_ov003_020843ec(s32 a0, s32 a1, s32 a2);

extern const s32 data_ov011_0212c204[];

extern s32 func_ov003_020cba2c(s32 a0, s32 a1, s32 a2, s32 a3);

extern s32 func_ov011_02129cec(BtlEnm010Tatt* data);

extern void func_ov011_02128cc0(BtlEnm010Tatt* data);

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

/// Picks the bin for `arg2` out of the `AnmMgr` task's table, makes sure it is loaded, and
/// primes the sprite. `arg2` is a "variant" index; `arg1` is the sprite record whose flag
/// halfword the function reads and then sets.
///
/// The two values below deliberately do not share a variable. Written as one `phase`, the slot
/// size and the anim phase are the same live value, so MWCC keeps it in a callee-saved
/// register for the whole function and every `data`-relative load moves up one register.
void func_ov011_02125750(s32 arg0, CombatSprite* arg1, s32 arg2) {
    s32              flag = (arg1->flags46 & 1) ? 1 : 0;
    void*            bin;
    BtlEnm010AnmMgr* data;
    s32              slot;
    s32              size;
    u16              phase;

    CombatSprite_Release(arg1);
    data = EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0);
    // The original builds the record pointer first (`mla r1, r3, r1, r2`) and then indexes
    // it by the variant (`ldr r4, [r1, r8, lsl #2]`).  Spelled as a single subscript
    // `base[mode * 0x28 + arg2]` MWCC instead folds the variant into the displacement and
    // emits `mla r0, r2, r0, r8` / `ldr r4, [r1, r0, lsl #2]` -- same value, six wrong
    // instructions.  The bias has to be spelled as a pointer.
    bin = ((const void* const*)((const u8*)data_ov011_0212cb1c + data->unk_38 * 0x28))[arg2];
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
            size = data->unk_34 - func_ov003_020cb368(bin);
        }
        func_ov003_020cb128(&data->unk_14[slot].unk_00, bin, size);
        func_ov003_020cb200(data, (u16)slot);
    }
    phase = data_ov011_0212bff4[arg2];
    if (data->unk_38 != 3) {
        phase = phase + data->unk_38;
    }
    CombatSprite_LoadFromTable(arg0, arg1, (const BinIdentifier*)bin, (const SpriteAnimEntry*)data_ov011_0212caf4[arg2], 0,
                               phase, data_ov011_0212bfe0[arg2]);
    if (flag == 1) {
        arg1->flags46 |= 1;
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
            size = data->unk_34;
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

/// The `Tsk_BtlEnm010_AnmMgr` task entry point: a three-way command dispatch. Command 0 and
/// command 3 fall through to the two handlers, anything else returns 1.
///
/// The load of the task data is *before* the first branch in the original, so it is written
/// as one load shared by both arms; the handlers take it in `r0` as their first argument, so
/// it costs no register.
s32 func_ov011_021259d0(s32 arg0, Task* task, s32 arg2, s32 cmd) {
    BtlEnm010AnmMgr* data = task->data;

    // Spelled as a switch, not as two `if`s and a `return 1`.  As `if`s, MWCC if-converts the
    // trailing `return 1` into the fall-through of the second arm and emits
    // `bne / movne r0, #1 / ldmneia`; the original branches to all three arms, with the
    // `return 1` block last and reached by a forward `b`.
    switch (cmd) {
        case 0:
            return func_ov011_02125a08(data, arg2);
        case 3:
            return func_ov011_02125b88(data);
        default:
            return 1;
    }
}

/// Command 0: measure every bin the task will ever load, size the decompression buffer from
/// the two largest of each group, then hand the whole thing to the pool allocator.
///
/// The two search loops are the same shape and are *not* factored into a helper: the original
/// has them fully duplicated, with the stride-0x28 table and the 3-entry table walked by
/// separate code, so sharing them would change the block layout.
///
/// `p10` is declared first so that it lands at the higher frame slots (`sp + 0xC` / `sp +
/// 0x10`) and `p3` at `sp + 4` / `sp + 8`, which is the order the original zeroes them in.
s32 func_ov011_02125a08(BtlEnm010AnmMgr* data, u16* arg1) {
    BtlEnm010Pair p10 = {0, 0};
    BtlEnm010Pair p3  = {0, 0};
    s32           i;
    s32           size;

    MI_CpuSet(data, 0, 0x3C);
    for (i = 0; i < 10; i++) {
        size = func_ov003_020cb368(((const void* const*)((const u8*)data_ov011_0212cb1c + arg1[0] * 0x28))[i]);
        // Both tests are **unsigned**: the original's stores and branches are `strhi`/`bhi`,
        // not `strgt`/`bgt`, so the operands need a `(u32)` on both sides.
        if ((u32)size > (u32)p10.max) {
            p10.sec = p10.max;
            p10.max = size;
        } else if ((u32)size > (u32)p10.sec) {
            p10.sec = size;
        }
    }
    for (i = 0; i < 3; i++) {
        size = func_ov003_020cb368(data_ov011_0212cae8[i]);
        if ((u32)size > (u32)p3.max) {
            p3.sec = p3.max;
            p3.max = size;
        } else if ((u32)size > (u32)p3.sec) {
            p3.sec = size;
        }
    }
    func_ov003_020cb128(&data->unk_14[0].unk_00, *(const void* const*)((const u8*)data_ov011_0212cb1c + arg1[0] * 0x28), 0);
    func_ov003_020cb128(&data->unk_14[1].unk_00, 0, 0);
    func_ov003_020cb128(&data->unk_14[2].unk_00, 0, 0);
    func_ov003_020cb128(&data->unk_14[3].unk_00, 0, 0);
    func_ov003_020cb130(data, &data->unk_14[0], 4, p10.max + p10.sec + p3.max + p3.sec, data_ov011_0212cbd4);
    func_ov003_020cb150(data);
    func_ov003_020cb200(data, 0);
    data->unk_38 = arg1[0];
    data->unk_34 = p10.max + p10.sec;
    return 1;
}

/// Command 3: release every element of the table, then return 1. The call's result is
/// discarded in the original, so it is spelled as a discarded call.
s32 func_ov011_02125b88(BtlEnm010AnmMgr* data) {
    func_ov003_020cb194(data);
    return 1;
}

/// Spawns the `Tsk_BtlEnm010_Lser` task and returns its handle. Which pool it goes into depends
/// on a two-bit flag at `arg0 + 0x84`.
///
/// The global is re-read in *both* arms, so it is spelled twice rather than hoisted: the
/// original has a predicated `ldreq` pair on the zero arm and an unpredicated `ldr` pair on
/// the other, with the `+ 0x8C + 0x8000` bias added in the second.
///
/// Returns the `EasyTask_CreateTask` result: every call site in the original stores it (the
/// Lser task handle lands in the caller's `0x1FC`/`0x208` slot), which is why this is `s32` and
/// not `void` -- wrong-prototype class, found while writing 02126fb0/0212ac0c.
s32 func_ov011_02125b98(void* arg0, s32 arg1) {
    BtlEnm010LserArgs args;
    TaskPool*         pool;

    if (func_ov003_020c37f8((u8*)arg0 + 0x84) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u32)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    args.unk_00 = arg0;
    args.unk_14 = arg1;
    return EasyTask_CreateTask(pool, &data_ov011_0212c118, 0, 0, 0, &args);
}

/// `CombatSprite_Restart` (`func_ov003_02082d04`) -- one argument, tail-called. Declared in
/// `Combat/Core/CombatSprite.h`.

/// Resolves the Lser task's owner position into three out-parameters: the x biased by
/// `+/- 0x40000` depending on the mirror flag, the y, and the z nudged by an indexed entry of
/// `data_ov011_0212c124`.
///
/// Five arguments; the fifth is on the caller's stack, which is why the callee reads it at
/// `sp + 8` after its own eight-byte `push {r3, lr}`.
void func_ov011_02125c00(s32* outX, s32* outY, s32* outZ, BtlEnm010OwnerCopy* owner, s32 index) {
    *outX = (owner->unk_24 == 0) ? owner->unk_28 - 0x40000 : owner->unk_28 + 0x40000;
    *outY = owner->unk_2C;
    *outZ = owner->unk_30 + data_ov011_0212c124[index];
}

/// Advances one of the Lser task's three emitter records. Bails out once the accumulated
/// value leaves range, or when the caller has disabled the emitter, then re-seeds the record
/// and kicks the sprite's animation.
void func_ov011_02125c44(BtlEnm010OwnerCopy* owner, BtlEnm010LserRec* rec, CombatSprite* sprite, s32 enabled) {
    s32 v = rec->unk_04 + rec->unk_08;

    rec->unk_04 = v;
    if (v >= 0x8000) {
        return;
    }
    if (enabled == 0) {
        return;
    }
    if (RNG_Next(0x64) < 0x32) {
        Mini108_VBlank(sprite, 1, 1);
    } else {
        Mini108_VBlank(sprite, 2, 1);
    }
    CombatSprite_Restart(sprite);
    rec->unk_00 = RNG_Next(0x4000);
    if (owner->unk_24 == 0) {
        rec->unk_00 = rec->unk_00 + 0x6000;
    } else {
        rec->unk_00 = rec->unk_00 - 0x2000;
    }
    // `rec->unk_04 = 0x20000` comes *after* the adjustment above, not before it. The original
    // materialises 0x20000 into r1 early but does not store it until just after the `strh`,
    // so writing the assignment first moves the store three instructions earlier.
    rec->unk_04 = 0x20000;
    rec->unk_08 = 0 - (RNG_Next(0x1001) + 0x1000);
}

/// The `Tsk_BtlEnm010_Lser` task entry point: a four-way command dispatch over a dense 0..3
/// range, so it gets a jump table (`cmp r3, #3 / addls pc, pc, r3, lsl #2`).
///
/// Only case 0 passes the incoming `arg2` on; the other three handlers take the data alone,
/// which is why their `bl` sites need no `mov r1`.
s32 func_ov011_02125cf8(s32 arg0, Task* task, s32 arg2, s32 cmd) {
    BtlEnm010Lser* data = task->data;

    switch (cmd) {
        case 0:
            return func_ov011_02125d48(data, arg2);
        case 1:
            return func_ov011_02125e14(data);
        case 2:
            return func_ov011_02126354(data);
        case 3:
            return func_ov011_021265a8(data);
        default:
            return 1;
    }
}

/// The `Tsk_BtlEnm010_Lser` task's initialiser, run for command 0. Clears the whole 0x254-byte
/// block, primes the four sprites, then fills in the fields the per-frame code reads.
s32 func_ov011_02125d48(BtlEnm010Lser* data, BtlEnm010LserArgs* args) {
    // A walking pointer, not `&data->sprite[i]`: the ROM's stride is 0x60 but
    // `sizeof(CombatSprite)` is 0x7D here, so the array form lands the next field at 0x274.
    // The mask is spelled `(u32)((s32)x << 30) >> 30` because the all-unsigned form folds to
    // `and r0, r0, #3` and loses the `lsl #30 / lsr #30` pair. Both are initialised *after*
    // the `MI_CpuSet`, which is where the original has `add r5, r7, #0x80`.
    // `i` is declared before `sp` on purpose: MWCC numbers the first-declared long-lived local
    // lower, and the original keeps the counter in r4 and the sprite pointer in r5.
    s32           i;
    CombatSprite* sp;

    MI_CpuSet(data, 0, 0x254);
    sp = (CombatSprite*)((u8*)data + 0x80);
    for (i = 0; i < 4; i++) {
        func_ov011_021258b4((u32)((s32)args->unk_00->unk_84 << 30) >> 30, sp, 2);
        sp = (CombatSprite*)((u8*)sp + 0x60);
    }
    data->unk_00  = args->unk_00;
    data->unk_23C = 0;
    data->unk_240 = 0;
    data->unk_244 = args->unk_10;
    if (args->unk_00->unk_24 == 0) {
        data->unk_244 = 0 - data->unk_244;
    }
    data->unk_24C = args->unk_14;
    // An if/else, not a ternary: the original's `moveq r1, #0` comes *before* its
    // `movne r1, #1`, and a ternary emits them the other way round.
    if (args->unk_14 == 0) {
        data->emit.unk_28 = 0;
    } else {
        data->emit.unk_28 = 1;
    }
    if (func_ov003_020c37f8((u8*)args->unk_00 + 0x84) != 0) {
        data->unk_250 |= 8;
    } else {
        data->unk_250 &= ~8;
    }
    return 1;
}

/// The Lser task's command 1, the per-frame worker. Refreshes the owner's state, drops the owner
/// when it has gone stale, picks one of three sub-workers off the emitter's mode halfword, then
/// ticks all four sprites.
///
/// The three stale-owner tests are one `||` chain because the original branches to the same
/// `unk_00 = 0` block from all three of them.
s32 func_ov011_02125e14(BtlEnm010Lser* data) {
    s32           result = 0;
    s32           i;
    CombatSprite* sp;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    if (data->unk_00 != NULL) {
        if (data->unk_00->unk_54 & 4) {
            return 0;
        }
    }
    // Two separate `if (data->unk_00 != NULL)` blocks, not a nested pair. The original re-tests
    // the owner pointer with its own `cmp` / `beq` after the first block, and folding the two
    // into one nested `if` costs those two instructions.
    if (data->unk_00 != NULL) {
        if (func_ov003_020cc354(&data->copy) != 0 || (data->copy.unk_54 & 0x4000) != 0 || (data->copy.unk_54 & 0x200) != 0) {
            data->unk_00 = NULL;
        }
    }
    if (data->unk_00 != NULL) {
        // A struct assignment, not a hand-written loop: MWCC turns it into the original's
        // `ldm r6!, {r0-r3} / stm lr!, {r0-r3}` x 7 plus a three-word tail, i.e. 0x7C bytes.
        data->copy = *(const BtlEnm010OwnerCopy*)data->unk_00;
    }
    switch (data->emit.unk_28) {
        case 0:
            result = func_ov011_02125f24(data);
            break;
        case 1:
            result = func_ov011_02126064(data);
            break;
        case 2:
            result = func_ov011_021260e8(data);
            break;
    }
    sp = (CombatSprite*)((u8*)data + 0x80);
    for (i = 0; i < 4; i++) {
        CombatSprite_Update(sp);
        sp = (CombatSprite*)((u8*)sp + 0x60);
    }
    return result;
}

/// The Lser task's mode-0 worker: primes the four sprites and their palettes on the first
/// frame, then advances the three emitter records and rolls the animation counter.
///
/// The four `Mini108_VBlank` calls take `data + 0x80`, `+ 0xE0`, `+ 0x140` and `+ 0x1A0` -- that
/// is the 0x60-stride sprite walk, so they are spelled as offsets into the raw block, not as
/// indices into a `CombatSprite[]` (whose `sizeof` is 0x7D).
s32 func_ov011_02125f24(BtlEnm010Lser* data) {
    s32               flag;
    s32               i;
    CombatSprite*     sp;
    BtlEnm010LserRec* rec;

    if (data->unk_00 == NULL) {
        return 0;
    }
    if (data->emit.unk_24 == 0) {
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x80), 1, 1);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0xE0), 1, 1);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x140), 1, 1);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x1A0), 3, 0);
        data->unk_250     = (data->unk_250 & ~1) | 1;
        data->unk_250     = data->unk_250 & ~2;
        data->emit.unk_26 = 0x2F;
        // `func_ov003_02087f00` is declared in Combat.h as taking a function pointer second; the
        // original passes the *value* `func_ov003_020843b0` returned straight through, so it
        // is cast rather than genuinely a callback.  Note the coordinate is
        // `data->copy.unk_28` -- `ldr r1, [r4, #0x2c]`, i.e. the *copy*, not the owner.
        func_ov003_02087f00((SndMgrSeIdx)0x1E5, func_ov003_020843b0(0, data->copy.unk_28));
    }
    // Initialise-then-if, not a ternary: a ternary emits `movle`/`movgt` (a phi), and the
    // original has a plain `mov r5, #1 / cmp / movgt r5, #0`.
    flag = 1;
    if (data->emit.unk_24 > 0x24) {
        flag = 0;
    }
    sp  = (CombatSprite*)((u8*)data + 0x80);
    rec = (BtlEnm010LserRec*)((u8*)data + 0x200);
    for (i = 0; i < 3; i++) {
        func_ov011_02125c44(&data->copy, rec, sp, flag);
        sp  = (CombatSprite*)((u8*)sp + 0x60);
        rec = (BtlEnm010LserRec*)((u8*)rec + 0xC);
    }
    // The guard is `cmp r0, #8 / ldrbeq / orreq / strbeq` -- an `== 8` equality, and the `ldrb`
    // is the load of the `u8` flag field, not of `unk_24`. A `<=` here gives `ldrl**s**b`/`orrls`.
    if (data->emit.unk_24 == 8) {
        data->unk_250 |= 2;
    }
    if (data->emit.unk_24 < data->emit.unk_26) {
        data->emit.unk_24 = data->emit.unk_24 + 1;
    } else {
        data->emit.unk_24 = 0;
        data->emit.unk_28 = 2;
    }
    return 1;
}

/// The Lser task's mode-1 worker: one sprite, one palette, and the same phase-counter roll as
/// mode 0.
s32 func_ov011_02126064(BtlEnm010Lser* data) {
    if (data->unk_00 == NULL) {
        return 0;
    }
    if (data->emit.unk_24 == 0) {
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x1A0), 3, 0);
        data->unk_250     = data->unk_250 | 2;
        data->emit.unk_26 = 4;
    }
    if (data->emit.unk_24 < data->emit.unk_26) {
        data->emit.unk_24 = data->emit.unk_24 + 1;
    } else {
        data->emit.unk_24 = 0;
        data->emit.unk_28 = 2;
    }
    return 1;
}

/// The Lser task's mode-2 worker, the laser-firing phase. On the first frame it primes the two end
/// sprites and resolves the owner's position into the `0x22C`/`0x230`/`0x234` triple, with
/// `0x248`'s sign and the `0x22C` bias picked by the mirror flag; every frame it clears flag bit 2
/// once the `0x1A0` sprite's animation finishes, plays a sound as the emitter's phase counter hits
/// 0x11, and while flag bit 2 is set advances `0x22C` by `0x248` and reports 0 the frame the move
/// completes. From phase 0xA on it emits one command record per frame through
/// `func_ov003_0208a164` -- the record's byte pair and the x bias chosen by a three-way branch on
/// the phase counter -- and plays a sound when the emit succeeds. The counter rolls at the tail;
/// the function returns 1 except for the one early 0.
s32 func_ov011_021260e8(BtlEnm010Lser* data) {
    s32 mode;

    // Flag bit 3, materialised as a 0/1 mode passed as the first argument to
    // `func_ov003_020843b0` at three call sites. The shift pair is the bit-test idiom.
    if (((u32)(data->unk_250 << 28) >> 31) != 0) {
        mode = 1;
    } else {
        mode = 0;
    }
    if (data->emit.unk_24 == 0) {
        data->emit.unk_24 = data->emit.unk_24 + 1;
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x80), 5, 0);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x1A0), 4, 1);
        func_ov011_02125c00(&data->unk_22C, &data->unk_230, &data->unk_234, &data->copy, data->unk_24C);
        if (data->copy.unk_24 == 0) {
            data->unk_248 = 0 - 0x8000;
            data->unk_22C = data->unk_22C - 0x28000;
        } else {
            data->unk_248 = 0x8000;
            data->unk_22C = data->unk_22C + 0x28000;
            CombatSprite_SetFlip((CombatSprite*)((u8*)data + 0x80), 1);
            CombatSprite_SetFlip((CombatSprite*)((u8*)data + 0x1A0), 1);
        }
        data->unk_250 &= ~1;
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x1A0)) != 0) {
        data->unk_250 &= ~2;
    }
    if (data->emit.unk_24 == 0x11) {
        data->unk_250 |= 4;
        func_ov003_02087f00(0x1DD, func_ov003_020843b0(mode, data->copy.unk_28));
    }
    if ((u32)(data->unk_250 << 29) >> 31) {
        data->unk_22C = data->unk_22C + data->unk_248;
        if (func_ov003_020cc7f4(func_ov003_020843b0(mode, data->unk_22C), 0, 0x60, 0) == 0) {
            return 0;
        }
    }
    if (data->emit.unk_24 >= 0xA) {
        BtlEnm010CmdTbl rec;
        s32             x;
        s32             idx;

        if (func_ov003_020c37f8((void*)((u8*)data + 0x80)) != 0) {
            idx = 0x5E;
        } else {
            idx = 0x56;
        }
        rec = *func_ov003_0208a114((u16)idx);
        if (data->emit.unk_24 < 0xE) {
            rec.unk_05 = 0x18;
            rec.unk_07 = 0x20;
            x          = (data->copy.unk_24 == 0) ? data->unk_22C + 0x18000 : data->unk_22C - 0x18000;
        } else if (data->emit.unk_24 < 0x11) {
            rec.unk_05 = 0x20;
            rec.unk_07 = 0x10;
            x          = (data->copy.unk_24 == 0) ? data->unk_22C + 0x28000 : data->unk_22C - 0x18000;
        } else {
            x = data->unk_22C;
        }
        if (func_ov003_0208a164(&rec, (void*)((u8*)data + 8), x, data->unk_230, data->unk_234) == 1) {
            func_ov003_02087f00(0x1DA, func_ov003_020843b0(mode, x));
        }
    }
    data->emit.unk_24 = data->emit.unk_24 + 1;
    return 1;
}

/// The Lser task's command 2. Projects the owner's position into two 4.12 screen offsets, then
/// applies each of three flag bits as a separate pass over the sprite block: bit 0 walks all
/// three emitter records, bit 1 lays down a single centre sprite, and bit 2 lays down a single
/// sprite on the owner's own projected position.
///
/// Declaration order is load-bearing twice over. The seven frame locals are laid out in the
/// original's frame as `0x08 t0, 0x0A t1, 0x0C outZ, 0x10 outY, 0x14 outX, 0x18 acc1, 0x1C acc0`
/// -- the `s32`s in reverse declaration order, with the two `s16` out-parameters of
/// `func_ov003_02084348` sharing the one word at `0x08`/`0x0A` above them. The six register
/// locals are laid out `r5 vx, r6 vy, r7 i, r8 rec, r9 sp`, with `r4` holding the loop-invariant
/// `0x7FFFFFFE - outY`.
s32 func_ov011_02126354(BtlEnm010Lser* data) {
    s16               t1;
    s16               t0;
    s32               acc0;
    s32               acc1;
    s32               outX;
    s32               outY;
    s32               outZ;
    s32               vx;
    s32               vy;
    s32               i;
    BtlEnm010LserRec* rec;
    CombatSprite*     sp;
    s32               mode;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    // Not `mode = 0; if (...) mode = 1;` -- that hoists a `mov r11, #0` above the call and
    // desynchronises the whole function by one instruction. The original gets the value out of
    // a bare `movne`/`moveq` pair straddling the argument setup.
    if (func_ov003_020c37f8((void*)((u8*)data + 0x80)) != 0) {
        mode = 1;
    } else {
        mode = 0;
    }
    func_ov011_02125c00(&outX, &outY, &outZ, &data->copy, data->unk_24C);
    vx = ROUND(func_ov003_020843b0(mode, outX));
    vy = ROUND(func_ov003_020843ec(mode, outY, outZ));

    // Bit 0: the three emitter records, each moving the running pair by the two projected offsets.
    if ((u32)(data->unk_250 << 31) >> 31) {
        rec = &data->emit.rec[0];
        sp  = (CombatSprite*)((u8*)data + 0x80);
        for (i = 0; i < 3; i++) {
            func_ov003_020cbc50(&acc0, &acc1, rec->unk_00, rec->unk_04);
            acc0 += vx;
            acc1 += vy;
            CombatSprite_SetPosition(sp, (acc0 * 16) >> 16, (acc1 * 16) >> 16);
            func_ov003_02082730(sp, 0x7FFFFFFE - outY);
            CombatSprite_Render(sp);
            rec = (BtlEnm010LserRec*)((u8*)rec + 0xC);
            sp  = (CombatSprite*)((u8*)sp + 0x60);
        }
    }

    // Bit 1: the centre sprite at `0x1A0`, on the offsets alone.
    if ((u32)(data->unk_250 << 30) >> 31) {
        CombatSprite_SetPosition((CombatSprite*)((u8*)data + 0x1A0), (vx * 16) >> 16, (vy * 16) >> 16);
        func_ov003_02082730((CombatSprite*)((u8*)data + 0x1A0), 0x7FFFFFFF - outY);
        CombatSprite_Render((CombatSprite*)((u8*)data + 0x1A0));
    }

    // Bit 2: the first sprite at `0x80`, on the owner's own projected position.
    if ((u32)(data->unk_250 << 29) >> 31) {
        func_ov003_02084348(mode, &t1, &t0, data->unk_22C, data->unk_230, data->unk_234);
        CombatSprite_SetPosition((CombatSprite*)((u8*)data + 0x80), t1, t0);
        func_ov003_02082730((CombatSprite*)((u8*)data + 0x80), 0x7FFFFFFD - outY);
        CombatSprite_Render((CombatSprite*)((u8*)data + 0x80));
    }
    return 1;
}

/// The Lser task's command 3: four identical sprite ticks and nothing else.
s32 func_ov011_021265a8(BtlEnm010Lser* data) {
    s32           i;
    CombatSprite* sp;

    sp = (CombatSprite*)((u8*)data + 0x80);
    for (i = 0; i < 4; i++) {
        CombatSprite_Release(sp);
        sp = (CombatSprite*)((u8*)sp + 0x60);
    }
    return 1;
}

/// RG's phase-0 setup. Called with the address of the next phase handler, which it stores at
/// `0x1C8` after clearing the `0x1C0`/`0x1C4` pair. The `add r0, r5, #0x100` base reaching
/// `0x1C4`/`0x1C0` is why those two are a pair rather than two flat fields.
void func_ov011_021265d4(BtlEnm010RG* data, void (*func)(struct BtlEnm010RG*)) {
    func_ov003_020c427c(data, (void*)func);
    data->unk_1C8 = func;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

/// RG's per-frame phase picker. Two switches: the first turns the mode halfword at `0x80` and
/// the running counter at `0x1F4` into a phase index out of one of four tables, the second turns
/// that index into the next phase handler and stores it at `0x1C8`.
///
/// The two `switch`es are both over ranges that start at 0, so each wants a jump table -- the
/// original's `cmp #3 / addls pc, pc, r2, lsl #2` and `cmp #9 / addls pc, pc, r4, lsl #2`.
///
/// `func` is initialised before the first switch, not in the second's `default:`, because the
/// original emits `mov r4, #0 / mov r1, r4` ahead of the first jump table and the `default`
/// arm is a bare `b` to the tail call.
void func_ov011_021265fc(BtlEnm010RG* data) {
    s32   i;
    void* func;

    i    = 0;
    func = NULL;
    switch (data->unk_080) {
        case 0:
            i = data_ov011_0212c18c[(u32)data->unk_1F4 % 5];
            break;
        case 1:
            i = data_ov011_0212c1a0[(u32)data->unk_1F4 % 6];
            break;
        case 2:
            i = data_ov011_0212c178[(u32)data->unk_1F4 % 5];
            break;
        case 3:
            i = data_ov011_0212c1b8[(u32)data->unk_1F4 % 7];
            break;
    }
    data->unk_1F4 = data->unk_1F4 + 1;
    switch (i) {
        case 0:
            func = func_ov011_02126b2c;
            break;
        case 1:
            func          = func_ov011_02126bf8;
            data->unk_1F6 = (data->unk_1F6 & ~1) | 1;
            data->unk_1F6 = data->unk_1F6 & ~2;
            break;
        case 2:
            func          = func_ov011_02126bf8;
            data->unk_1F6 = data->unk_1F6 & ~1;
            data->unk_1F6 = data->unk_1F6 | 2;
            break;
        case 3:
            func          = func_ov011_02126bf8;
            data->unk_1F6 = (data->unk_1F6 & ~1) | 1;
            data->unk_1F6 = data->unk_1F6 | 2;
            break;
        case 4:
            func = func_ov011_02126e80;
            break;
        case 5:
            func = func_ov011_02126fb0;
            break;
        case 7:
            func = func_ov011_02127240;
            break;
        case 8:
            func = func_ov011_02127370;
            break;
        case 9:
            func = func_ov011_02127474;
            break;
    }
    func_ov011_021265d4(data, func);
}

/// RG's task entry. Bails on whatever `func_ov003_020c5bfc` reports, otherwise installs
/// `func_ov011_0212681c` as phase 0.
void func_ov011_021267f4(BtlEnm010RG* data) {
    if (func_ov003_020c5bfc() != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

/// RG's phase-0 handler, and the only place `0x84` is touched: on the first pass it primes that
/// sprite and latches `func_ov003_020c42ec`'s answer at `0x1C2`, then counts `0x1C0` up to that
/// latch. Once the `-1` sentinel at `0x1FC` is written, reaching the latch hands over to the
/// phase picker instead of counting.
void func_ov011_0212681c(BtlEnm010RG* data) {
    if (data->unk_024 == 0) {
        func_ov011_021265d4(data, func_ov011_02126aec);
        return;
    }
    if (data->unk_1C0 == 0) {
        func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 0);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 0);
        data->unk_1C2 = func_ov003_020c42ec(data);
    }
    if (data->unk_1FC == -1 && data->unk_1C0 >= data->unk_1C2) {
        func_ov011_021265fc(data);
        return;
    }
    data->unk_1C0 = data->unk_1C0 + 1;
}

/// RG's phase-1 handler. A three-way chain over `0x1C4` -- a `switch` here compiles to the same
/// `cmp / beq` chain, since the three cases are contiguous and only case 1 falls through.
///
/// The `0x9A == 1 && 0x8C == 1` guard is the MWCC "is this zero" idiom: `sub r0, r0, #1` then
/// the `^ (x << 1)` / `ror #31` sequence, so the `ldrsheq` and `cmpeq` it predicates belong to
/// the *second* half of the `&&`. Spelled as a plain `== 1` this compiles to two instructions
/// instead of five -- see build/scratch/AGENT_BRIEF.md section 8; the rest of the function is
/// byte-exact and the remaining diff is only this guard.
void func_ov011_021268c4(BtlEnm010RG* data) {
    CombatSprite* sp;
    s32           t;

    switch (data->unk_1C4) {
        case 0:
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                sp = (CombatSprite*)((u8*)data + 0x84);
                func_ov011_02125750(1, sp, 1);
                Mini108_VBlank(sp, 0, 0);
                data->unk_1DC = (func_ov003_020cb744(1) >> 1) - 0x40000;
                data->unk_1E0 = func_ov003_020cb7a4(1) >> 1;
                data->unk_1E4 = 0;
                func_ov011_02127bf0(data, 0x1800);
                func_ov003_020c4ab4(data, data->unk_1D0 >= 0 ? 1 : 0);
            }
            sp = (CombatSprite*)((u8*)data + 0x84);
            t  = sp->sprite.unk16;
            if (t == 1 && sp->sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D6, func_ov003_020843b0(1, data->unk_028));
            }
            if (data->unk_1C0 < data->unk_1C2) {
                data->unk_1C0 = data->unk_1C0 + 1;
                return;
            }
            data->unk_1D4 = 0;
            data->unk_1D0 = 0;
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

/// RG's phase-2 handler. The first pass primes the sprite, seeds the `0x30` countdown and its
/// `0x1D0` bias, then plays a sound; after that a `Sprite::unk16 == 3` frame adds `0x2800` at
/// `0x1D8` per step, and the phase ends once `0x30` goes negative.
///
/// `0x1D0`'s two arms share the `-0x40000` that went into `0x30` -- the original keeps it in a
/// register and emits `addeq r0, r1, #0x38000`, so the expression has to be written once and
/// shared or MWCC folds it to a single `mov`.
void func_ov011_02126a04(BtlEnm010RG* data) {
    CombatSprite* sp;
    s32           v;

    sp = (CombatSprite*)((u8*)data + 0x84);
    if (data->unk_1C0 == 0) {
        func_ov011_02125750(1, sp, 6);
        Mini108_VBlank(sp, 0, 1);
        func_ov003_020c4c9c(data);
        v             = 0 - 0x40000;
        data->unk_030 = v;
        if (data->unk_024 == 0) {
            data->unk_1D0 = v + 0x38000;
        } else {
            data->unk_1D0 = 0x8000;
        }
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_02087f00(0x1E3, func_ov003_020843b0(1, data->unk_028));
    }
    if (sp->sprite.unk16 == 3 && sp->sprite.frameTimer == 1) {
        data->unk_1D8 = 0x2800;
    }
    if (data->unk_030 < 0) {
        return;
    }
    data->unk_1D8 = 0;
    data->unk_1D0 = 0;
    func_ov003_020cb520(data, 1);
    func_ov003_020cb594(data, 1);
    func_ov011_021265d4(data, func_ov011_0212681c);
}

/// RG's phase-3 handler. Bumps `0x1C0` and, if `func_ov011_02127758` says the phase is done,
/// re-arms phase 0.
void func_ov011_02126aec(BtlEnm010RG* data) {
    s32 r;

    r             = func_ov011_02127758(data, data->unk_1C0);
    data->unk_1C0 = data->unk_1C0 + 1;
    if (r != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

/// RG's phase-4 handler. Phase 0 just latches a `0x1E`/`0xBD` pair at `0x1F8`/`0x1FA` and
/// waits for `0x1C0` to leave zero; phase 1 hands those two words to `func_ov011_021277c8` as
/// pointers, and latches `0x1FC` the frame they come back as `0` and `0xBD`.
///
/// The third argument is spelled `(u8*)data + 0xFA + 0x100` rather than `&data->unk_1FA` on
/// purpose: the original reaches it as `add r2, r4, #0xfa / add r2, r2, #0x100`, while the
/// *stores* at `0x1F8`/`0x1FA` go through the `0x100` base as struct fields.
void func_ov011_02126b2c(BtlEnm010RG* data) {
    s32  r;
    s16* q;

    q = (s16*)((u8*)data + 0x100);
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x1E;
                data->unk_1FA = 0xBD;
            }
            r       = func_ov011_021277c8(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), data->unk_1C0);
            q[0x60] = q[0x60] + 1;
            if (data->unk_1F8 == 0 && data->unk_1FA == 0xBD) {
                data->unk_1FC = func_ov011_02127c84(data);
            }
            if (r != 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

/// RG's phase-5 handler: the two-flag "spread shot" phase.
///
/// The flag halfword at `0x1F6` is bit-decomposed twice with signed shift pairs (no `(u32)`
/// cast -- the original wants `asr`, not `lsr`): bit 1 into `flags[0]`, bit 0 into `flags[1]`,
/// each as a 0 / -1 mask. `count` is `data_ov011_0212c168[data->unk_080]`, forced to 2 when
/// both flag bits are set (the short-circuit `&&` is load-bearing: the original skips the bit-0
/// extraction entirely when bit 1 is clear). The two masks go into a frame array because the
/// spawn loop below indexes them by its counter.
///
/// `switch (data->unk_1C4)`: case 0 is the usual `func_ov003_020c6230` bail (ONE argument --
/// it derives its own `+0x100` base) then the `0x1C0 = 0, 0x1C4 = 1` latch; case 1 is
/// everything else; anything else falls out of the switch to a bare return. The tail clamp
/// therefore lives *inside* case 1 -- that is what makes the fall-out's epilogue an inline
/// `pop`, exactly like `func_ov011_021268c4`/`func_ov011_02126b2c`.
///
/// Case 1 latches `0x1F8 = 0x14, 0x1FA = 0x1E` when `0x1C0 == 0`, then hands both words to
/// `func_ov011_021277c8` as pointers -- the third argument spelled `(u8*)data + 0xFA + 0x100`
/// for the same reason `func_ov011_02126b2c` spells it that way (the original builds it with
/// `add r2, r4, #0xfa / add r2, r2, #0x100`), and the fourth is the PRE-increment `0x1C0`.
/// The increment store goes out before the `bl`, matching `func_ov011_02126b2c`'s identical
/// call sequence byte for byte.
///
/// The spawn block is guarded by two SEQUENTIAL tests whose failures target different blocks --
/// `unk_1F8 == 0 && unk_1FA % 5 == 0` (both bail to the tail clamp) and then, inside,
/// `slot == 0` (the launch kick) which merely skips to the loop. The second is a plain signed
/// DIVISION compared to zero (`smull` with `0x66666667` = `/5`, NOT `/6` as the plan guessed:
/// the same magic and the same `mov r2, #0x5` serve `unk_1FA % 5`, the division and the two
/// `0x1E8` subtractions below).
///
/// The launch kick writes `0x1D0`/`0x1E8` in opposite directions per the `0x24` mirror flag.
/// The `0x1E8` values are spelled with the shared literal 5 in opposite operand orders --
/// `5 - 0x338` (= -0x333) and `0x338 - 5` (= +0x333) -- because the original emits
/// `sub r0, r2, #0x338` in one arm and `rsb r0, r2, #0x338` in the other with r2 still holding
/// the modulo's 5. They are NOT the same value; do not unify them. `0x1D0` is likewise
/// `0x2000` / `0 - 0x2000`, sharing one materialised `0x2000`.
///
/// The spawn loop runs twice (once per flag bit), each iteration guarded by `flags[i] != 0`
/// and `slot < count`, and calls `func_ov011_02127f6c` with all NINE of its declared arguments
/// (four registers + five stack words): the mirrored x bias (`0 - 0x40000` / `0x40000`), two
/// zeroes, a z of `(0 - 0x40000) + 0x30000` (= -0x10000, sharing the hoisted `0 - 0x40000`
/// with the x bias), 1, a truncated `u16` angle `(u16)(base + aim[slot].unk_00)` whose base is
/// `0x8000`/`0x8C00` unmirrored and `0`/`0xF400` mirrored (slot 0 / slot 1, zero arm first),
/// another zero, the literal `0x333`, and `aim[slot].unk_02`. The result latches `0x1FC`.
///
/// The tail clamp zeroes the `0x1D0`/`0x1E8` pair when its step crosses zero from either
/// side (`else if` is load-bearing: the second compare reuses the first `cmp`'s flags), then
/// returns if `func_ov011_021277c8` said the animation is done, else re-arms phase 1.
void func_ov011_02126bf8(BtlEnm010RG* data) {
    s32 bit1;
    s32 slot;
    s32 count;
    s32 i;
    s32 flags[2];
    s32 r;
    s32 n;
    s32 bias;
    s32 angle;

    bit1  = (data->unk_1F6 << 30) >> 31;
    count = data_ov011_0212c168[data->unk_080];
    if (bit1 != 0 && ((data->unk_1F6 << 31) >> 31) != 0) {
        count = 2;
    }
    flags[0] = bit1;
    flags[1] = (data->unk_1F6 << 31) >> 31;
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x14;
                data->unk_1FA = 0x1E;
            }
            r             = func_ov011_021277c8(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), data->unk_1C0);
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1F8 == 0 && data->unk_1FA % 5 == 0) {
                slot = (0x1E - data->unk_1FA) / 5;
                if (slot == 0) {
                    if (data->unk_024 == 0) {
                        data->unk_1D0 = 0x2000;
                        data->unk_1E8 = 5 - 0x338;
                    } else {
                        data->unk_1D0 = 0 - 0x2000;
                        data->unk_1E8 = 0x338 - 5;
                    }
                }
                for (i = 0; i < 2; i++) {
                    if (flags[i] != 0 && slot < count) {
                        if (data->unk_024 == 0) {
                            if (i == 0) {
                                angle = 0x8000;
                            }
                            bias = 0 - 0x40000;
                            if (i != 0) {
                                angle = 0x8C00;
                            }
                        } else {
                            if (i == 0) {
                                angle = 0;
                            }
                            bias = 0x40000;
                            if (i != 0) {
                                angle = 0xF400;
                            }
                        }
                        data->unk_1FC = func_ov011_02127f6c(data, bias, 0, (0 - 0x40000) + 0x30000, 1,
                                                            (u16)(angle + data_ov011_0212c13c[slot].unk_00), 0, 0x333,
                                                            data_ov011_0212c13c[slot].unk_02);
                    }
                }
            }
            if (data->unk_1D0 > 0) {
                if (data->unk_1D0 + data->unk_1E8 < 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            } else if (data->unk_1D0 < 0) {
                if (data->unk_1D0 + data->unk_1E8 > 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            }
            if (r != 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_021268c4);
            return;
    }
}

/// RG's short-range phase. Same opening as `func_ov011_02127370` with a `0x60000` bias, a frame
/// 3 sprite prime, and one extra block: a `&&` on `0x9A == 6` and `0x8C == 1` that plays a sound
/// the medium-range phase does not.
void func_ov011_02126e80(BtlEnm010RG* data) {
    s32 bias;

    bias = (*(s32*)((u8*)data + 0x24) == 0) ? 0 - 0x60000 : 0x60000;
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 3);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 1);
                func_ov003_02087f00(0x1D7, func_ov003_020843b0(1, *(s32*)((u8*)data + 0x28)));
            }
            if (*(s16*)((u8*)data + 0x9A) == 6 && *(s16*)((u8*)data + 0x8C) == 1) {
                func_ov003_02087f00(0x1D8, func_ov003_020843b0(1, *(s32*)((u8*)data + 0x28)));
            }
            if (*(s16*)((u8*)data + 0x9A) >= 5 && *(s16*)((u8*)data + 0x9A) <= 7) {
                func_ov003_020c5b2c(0x5D, (s32)(u32)data, *(s32*)((u8*)data + 0x28) + bias, *(s32*)((u8*)data + 0x2C),
                                    *(s32*)((u8*)data + 0x30));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

/// RG's "spawning" phase, dispatched from `func_ov011_021265fc`'s table. A three-way switch on the
/// phase state at `0x1C4`: state 0 waits out `func_ov003_020c6230` and latches `0x1C0 = 0`,
/// `0x1C4 = 1`; state 1 primes the `0x84` sprite with anim 9 on the first frame and hands over to
/// state 2 when its animation finishes; state 2 first primes anim 2 and spawns the Lser task
/// (`func_ov011_02125b98(data, 0)` -> `0x1FC`, latch `0x1C2 = 0x4B`), then every 15 frames past
/// 0x28 spawns another Lser with the iteration count while it is below
/// `data_ov011_0212c148[unk_080]`, and every 15 frames past 0x3C kicks the `0x1D0`/`0x1E8`
/// velocity pair (`+/- 0x40000`-class step, then 0) until the second-to-last iteration, where it
/// plays anim 3. The velocity pair decays toward zero (`0x333` per frame, clamped at the
/// crossing), and once the sprite's animation finishes the phase hands back to
/// `func_ov011_021268c4`, otherwise the frame counter rolls.
void func_ov011_02126fb0(BtlEnm010RG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 9);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 2, 0);
                data->unk_1FC = func_ov011_02125b98(data, 0);
                data->unk_1C2 = 0x4B;
            }
            if (data->unk_1C0 >= 0x28) {
                s32 y = data->unk_1C0 - 0x28;

                if (y % 15 == 0) {
                    s32 q = y / 15;

                    if (q > 0 && q < data_ov011_0212c148[data->unk_080]) {
                        data->unk_1FC = func_ov011_02125b98(data, q);
                    }
                }
            }
            if (data->unk_1C0 >= 0x3C) {
                s32 y = data->unk_1C0 - 0x3C;

                if (y % 15 == 0) {
                    s32 q = y / 15;

                    // Two sequential `if`s, not a nested pair: each re-reads the table entry.
                    if (q < data_ov011_0212c148[data->unk_080]) {
                        data->unk_1D0 = (data->unk_024 == 0) ? 0x4000 : 0 - 0x4000;
                        data->unk_1E8 = 0;
                    }
                    if (q == data_ov011_0212c148[data->unk_080] - 1) {
                        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 3, 1);
                    }
                }
            }
            // Decay the velocity pair toward zero, clamping both words at the crossing. The
            // 0x333 step is spelled as two separate statements -- `sub #0x33 / sub #0x300` in the
            // original -- because a folded `- 0x333` becomes a pool constant plus an add (and the
            // pool word lives in the function, i.e. +4 bytes).
            if (data->unk_1D0 > 0) {
                data->unk_1E8 = data->unk_1E8 - 0x33;
                data->unk_1E8 = data->unk_1E8 - 0x300;
                if (data->unk_1D0 + data->unk_1E8 <= 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            } else if (data->unk_1D0 < 0) {
                data->unk_1E8 = data->unk_1E8 + 0x33;
                data->unk_1E8 = data->unk_1E8 + 0x300;
                if (data->unk_1D0 + data->unk_1E8 >= 0) {
                    data->unk_1D0 = 0;
                    data->unk_1E8 = 0;
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                func_ov011_021265d4(data, func_ov011_021268c4);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

/// RG's phase with a third arm. Arm 1 is the only one that reaches the `0x1C0 == 0x1C` spawn
/// check and the `0x1C0` reset, and arm 2 skips the palette prime entirely. The `beq` to the
/// increment is reached from *both* the "animation finished" test and the arm's own exit, which
/// is what the `!= 0` / `if` split above produces.
void func_ov011_02127240(BtlEnm010RG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 8);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
                func_ov003_02087f00(0x1E0, func_ov003_020843b0(1, *(s32*)((u8*)data + 0x28)));
            }
            if (data->unk_1C0 == 0x1C) {
                data->unk_1FC = func_ov011_02128c44(data);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 2;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

/// RG's medium-range phase. The `0x1C4` switch has no third arm here, so the fall-through is a
/// bare `pop`, and the `0x80000` bias is computed before the switch because arm 1 needs it --
/// MWCC hoists it to the top of the block regardless of where it is written.
void func_ov011_02127370(BtlEnm010RG* data) {
    s32 bias;

    bias = (*(s32*)((u8*)data + 0x24) == 0) ? 0 - 0x80000 : 0x80000;
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 4);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
                func_ov003_02087f00(0x1DF, func_ov003_020843b0(1, *(s32*)((u8*)data + 0x28)));
            }
            if (*(s16*)((u8*)data + 0x9A) >= 3 && *(s16*)((u8*)data + 0x9A) <= 4) {
                func_ov003_020c5b2c(0x60, (s32)(u32)data, *(s32*)((u8*)data + 0x28) + bias, *(s32*)((u8*)data + 0x2C),
                                    *(s32*)((u8*)data + 0x30));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_0212681c);
            return;
    }
}

/// RG's phase with three arms and a 0x4 frame. Arm 2 is the only one that reaches
/// `func_ov011_020c5b2c` and `func_ov011_02127c4c`, and it exits through a `>= 0x8000` test on the
/// helper's return -- an `addge`/`popge` pair, so the early exit is a `>=` and not a `>`.
void func_ov011_02127474(BtlEnm010RG* data) {
    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230(data) != 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 5);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
                func_ov003_020cb520(data, 0);
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov003_02087f00(0x1E2, func_ov003_020843b0(1, *(s32*)((u8*)data + 0x28)));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 0);
                *(s32*)((u8*)data + 0x1DC) = func_ov003_020cb744(1) + 0xC0000;
                *(s32*)((u8*)data + 0x1E0) = *(s32*)((u8*)data + 0x2C);
                *(s32*)((u8*)data + 0x1E4) = 0 - 0x20000;
                func_ov011_02127bf0(data, 0x8000);
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov003_02087f00(0x1E3, func_ov003_020843b0(1, *(s32*)((u8*)data + 0x28)));
            }
            func_ov003_020c5b2c(0x61, (s32)(u32)data, *(s32*)((u8*)data + 0x28), *(s32*)((u8*)data + 0x2C),
                                *(s32*)((u8*)data + 0x30));
            if (func_ov011_02127c4c(data) >= 0x8000) {
                return;
            }
            func_ov011_021265d4(data, func_ov011_02126a04);
            return;
    }
}

/// An RG phase entry of the shape the other two take: prime once on `0x1C0 == 0`, then poll a
/// two-argument predicate and re-arm phase 0 when it goes quiet.
///
/// The three zeroes go out in the order `0x1D8`, `0x1D4`, `0x1D0`, and
/// `func_ov003_020cb520(data, 1)` is called *after* the sprite prime here but *before* it in
/// `func_ov011_02127628` -- both orders are load-bearing.
void func_ov011_02127628(BtlEnm010RG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_020cb520(data, 1);
        func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 0);
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
    }
    if (func_ov003_020c62c4(data, 1) != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

/// RG's spawn handler: primes the `0x84` sprite on the first frame, then clears `0x1CC` when
/// `func_ov003_020c703c` reports the owner has gone away.
void func_ov011_02127698(BtlEnm010RG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 0);
    }
    if (func_ov003_020c703c(data) == 0) {
        data->unk_1CC = 0;
    }
}

/// The same shape again, with a three-argument predicate and the prime and the `0xCB520` call
/// the other way round.
void func_ov011_021276e0(BtlEnm010RG* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(1, (CombatSprite*)((u8*)data + 0x84), 0);
        func_ov003_020cb520(data, 1);
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
    }
    if (func_ov003_020c72b4(data, 0, 4) != 0) {
        return;
    }
    func_ov011_021265d4(data, func_ov011_0212681c);
}

/// One step of RG's "wait for the animation, then hand back" phase. On the first call it
/// starts the `0x84` animation; on the last it stops it and resets the owner, returning 0 so
/// the caller re-arms. Returns 1 while the animation is still running.
s32 func_ov011_02127758(void* p, s32 arg1) {
    BtlEnm010RG*  data;
    CombatSprite* sp;

    data = (BtlEnm010RG*)p;
    sp   = (CombatSprite*)((u8*)data + 0x84);
    if (arg1 == 0) {
        func_ov011_02125750(1, sp, 3);
        Mini108_VBlank(sp, 0, 1);
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)sp) == 0) {
        return 1;
    }
    func_ov011_02125750(1, sp, 0);
    Mini108_VBlank(sp, 0, 0);
    func_ov003_020c4c9c(data);
    return 0;
}

/// The per-frame worker behind RG's phase 4. Dispatches on the `0x84` sprite's
/// `animTableIndex` (reached as `data + 0xC8`, i.e. `sprite + 0x44`) and counts down the two
/// `s16`s it was handed by pointer. Returns 0 only from case 3, once the animation is over.
s32 func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3) {
    BtlEnm010RG*  data;
    CombatSprite* sp;

    data = (BtlEnm010RG*)p;
    sp   = (CombatSprite*)((u8*)data + 0x84);
    if (arg3 == 0) {
        func_ov011_02125750(1, sp, 2);
        Mini108_VBlank(sp, 0, 1);
    }
    switch (sp->animTableIndex) {
        case 0:
            if (sp->sprite.unk16 == 4 && sp->sprite.frameTimer == 1) {
                func_ov003_02087f00(0x1D3, func_ov003_020843b0(1, data->unk_028));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)sp) != 0) {
                *arg1 = *arg1 - 1;
                if (*arg1 <= 0) {
                    Mini108_VBlank(sp, 1, 0);
                }
            }
            break;
        case 1:
            *arg2 = *arg2 - 1;
            if (*arg2 <= 0) {
                Mini108_VBlank(sp, 3, 1);
            }
            break;
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)sp) != 0) {
                return 0;
            }
            break;
    }
    return 1;
}

/// RG's task entry. Copies the whole 0x10-byte entry block to the frame and then calls word
/// `index` of it -- the `ldm/stm` is the copy regenerating itself, so it has to be a struct
/// assignment and never a hand-written loop.
void func_ov011_021278d4(void* p, s32 arg1, s32 arg2, s32 index) {
    BtlEnm010RGEntry t;

    t = data_ov011_0212c158;
    t.func[index](p, arg1, arg2);
}

/// RG's initialiser, and the only write to the whole 0x208 block.
///
/// Two things are load-bearing here. `unk_1FC`/`unk_200` are written as `unk_1CC - 2`, not as
/// `-1`: the original emits `mov r0, #1 / str [r4, #0x1cc] / sub r0, r0, #2`, which a literal
/// `-1` would have collapsed to a single `mvn`. And `0x28`/`0x2C` are written twice, once
/// biased `+0x40000` through the pool pointer and once biased `-0x40000` through `data` -- the
/// second write wins, and both have to be in the C.
///
/// Returns 1. The `mov r0, #1` in the original's tail is not dead -- it is the return value,
/// and it is what finally pinned the return type (wrong-prototype class, found while auditing
/// the `0x1F4`/`0x54` tail).
s32 func_ov011_0212791c(void* arg0, void* arg1, s32 arg2) {
    BtlEnm010RG* data;
    BtlEnm010RG* t;
    s32          v;

    data = *(BtlEnm010RG**)((u8*)arg1 + 0x18);
    t    = *(BtlEnm010RG**)((u8*)data_ov003_020e71b8 + 0x3D89C);
    MI_CpuSet(data, 0, 0x208);
    t->unk_028                                  = (func_ov003_020cb744(1) >> 1) + 0x40000;
    t->unk_02C                                  = func_ov003_020cb7a4(1) >> 1;
    t->unk_030                                  = 0;
    *(u32*)((u8*)data_ov003_020e71b8 + 0x3D878) = *(u32*)((u8*)data_ov003_020e71b8 + 0x3D878) | 0x40000000;
    *(u32*)((u8*)data_ov003_020e71b8 + 0x3D838) = (func_ov003_020cb744(1) >> 1) + 0x40000;
    *(u32*)((u8*)data_ov003_020e71b8 + 0x3D83C) = func_ov003_020cb7a4(1) >> 1;
    func_ov003_020c3efc(data, (void*)(u32)arg2);
    func_ov003_020c44ac(data);
    data->unk_1CC = 1;
    v             = data->unk_1CC - 2;
    data->unk_1FC = v;
    data->unk_200 = v;
    func_ov011_021265d4(data, func_ov011_021267f4);
    data->unk_1AC = (func_ov003_020cb744(1) >> 1) - 0x40000;
    data->unk_028 = data->unk_1AC;
    data->unk_1B0 = func_ov003_020cb7a4(1) >> 1;
    data->unk_02C = data->unk_1B0;
    data->unk_1B4 = 0;
    data->unk_030 = 0;
    func_ov003_020c4b1c(data);
    data->unk_1F4 = 0;
    data->unk_054 |= 1 << 30;
    return 1;
}

/// RG's per-frame handler. Picks a phase off `CombatActor_PopPendingCommand`, integrates two Euler steps
/// over the position and velocity triples, then hands back `0x1CC`.
///
/// The two integration passes are in this order and it matters: the position is advanced by the
/// velocity *before* the velocity is advanced by its own acceleration. The six `ldr/add/str`
/// triples read `0x2C` first each time, so `0x28/0x2C/0x30` really are x/y/z in that order.
///
/// `0x18C`'s latch is written as an `if`: the original's `orrne`/`strhne` are predicated on the
/// `tst`, so the store only happens on the bit-4 path.
s32 func_ov011_02127a64(void* arg0, void* arg1) {
    BtlEnm010RG* data;

    data = *(BtlEnm010RG**)((u8*)arg1 + 0x18);
    switch (CombatActor_PopPendingCommand((CombatActor*)data)) {
        case 2:
            func_ov011_021265d4(data, func_ov011_02127628);
            break;
        case 3:
            if (data->unk_18C & 0x10) {
                data->unk_18C = data->unk_18C | 0x20;
            } else {
                func_ov011_021265d4(data, func_ov011_02127698);
            }
            break;
        case 6:
            func_ov011_021265d4(data, func_ov011_021276e0);
            break;
    }
    EasyTask_ValidateTaskId((TaskPool*)((u8*)data_ov003_020e71b8 + 0x8C + 0x8000), (u32*)&data->unk_1FC);
    if (data->unk_200 != -1) {
        data->unk_204 = data->unk_204 + 1;
    }
    if (data->unk_1C8 != NULL) {
        data->unk_1C8(data);
    }
    data->unk_028 = data->unk_028 + data->unk_1D0;
    data->unk_02C = data->unk_02C + data->unk_1D4;
    data->unk_030 = data->unk_030 + data->unk_1D8;
    data->unk_1D0 = data->unk_1D0 + data->unk_1E8;
    data->unk_1D4 = data->unk_1D4 + data->unk_1EC;
    data->unk_1D8 = data->unk_1D8 + data->unk_1F0;
    func_ov003_020c4628(data);
    return data->unk_1CC;
}

/// RG's kill handler. Hands the task data to the engine's deleter, or -- once the `0x200`
/// sentinel says the projectile has left the screen -- takes the exit path and then releases
/// whatever is at `0xE4`.
s32 func_ov011_02127b98(void* arg0, void* arg1) {
    BtlEnm010RG* data;

    data = *(BtlEnm010RG**)((u8*)arg1 + 0x18);
    if (data->unk_200 == -1 || data->unk_204 == 0) {
        func_ov003_020c4878(data);
    } else {
        func_ov003_020c4748(data);
        func_ov003_0208810c((void*)((u8*)data + 0xE4), data);
    }
    return 1;
}

/// RG's frame callback. Returns 1 unconditionally.
s32 func_ov011_02127bdc(void* arg0, void* arg1) {
    func_ov003_020c48fc(*(void**)((u8*)arg1 + 0x18));
    return 1;
}

/// RG's "put the sprite back" handler. Four registers, two stack words, one `strh` at `0x1C2`.
s32 func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1) {
    s32 r;

    r = func_ov003_020cb910(&data->unk_1D0, &data->unk_1D4, &data->unk_1D8, data->unk_028, data->unk_02C, data->unk_030,
                            data->unk_1DC, data->unk_1E0, data->unk_1E4, arg1);
    data->unk_1C2 = r;
    return r;
}

/// RG's position commit: six values straight through, four in registers and two on the stack.
s32 func_ov011_02127c4c(BtlEnm010RG* data) {
    func_ov003_020cba54(data->unk_028, data->unk_02C, data->unk_030, data->unk_1DC, data->unk_1E0, data->unk_1E4);
}

/// Spawns the follow-up Rnge task. The pool is the plain global unless `0x84`'s animation is
/// already running, in which case it is the same base biased by `0x8C + 0x8000`.
///
/// The last argument is `&data` -- the address of the *parameter*, not its value. That is what
/// the prologue's `str r0, [sp, #0x8]` and the `stm sp, {r2, ip}` (which stores `sp + 8` into
/// the outgoing slot) are for: MWCC has to give the callee a pointer to a pointer.
s32 func_ov011_02127c84(void* p) {
    BtlEnm010RG* data;
    TaskPool*    pool;

    data = (BtlEnm010RG*)p;
    if (func_ov003_020c37f8((void*)((u8*)data + 0x84)) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u8*)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    return EasyTask_CreateTask(pool, &data_ov011_0212c1d4, 0, 0, 0, (void*)&data);
}

/// `Tsk_BtlEnm010_Rnge`'s task entry: a four-way switch whose arms all fall through to a
/// single `return r`, and `r` is 1 unless case 1's animation has finished.
///
/// Two details are load-bearing. Case 0's `ldr r0, [r6, #0x0]` appears **three** times, so the
/// spawn-argument read has to be written three times rather than hoisted into a local. And case 1
/// reads `unk_60->unk_54` once into a register and tests bits 9 and 14 off it, in that order --
/// the Lser equivalent at `func_ov011_02125e14` tests 14 before 9, and the two are not
/// interchangeable.
s32 func_ov011_02127ce0(void* arg0, void* arg1, s32 arg2, s32 index) {
    BtlEnm010Rnge* data;
    s32            r;
    s32            snd;
    s32            pan;
    s32            bias;
    s32            y;

    data = *(BtlEnm010Rnge**)((u8*)arg1 + 0x18);
    r    = 1;
    switch (index) {
        case 0:
            MI_CpuSet(data, 0, 0x6C);
            func_ov011_021258b4(((u32)((s32) * (u32*)((u8*)*(u32*)arg2 + 0x84) << 30) >> 30), (CombatSprite*)data, 0);
            Mini108_VBlank((CombatSprite*)data, 0, r);
            data->unk_060 = *(BtlEnm010Owner**)arg2;
            if (*(s32*)((u8*)*(BtlEnm010Owner**)arg2 + 0x24) == 1) {
                CombatSprite_SetFlip((CombatSprite*)data, r);
            } else {
                CombatSprite_SetFlip((CombatSprite*)data, 0);
            }
            break;
        case 1:
            if (func_ov003_020c3c28() != 0) {
                return 0;
            }
            if (func_ov003_020cc354(data->unk_060) != 0 || (data->unk_060->unk_54 & 0x200) != 0 ||
                (data->unk_060->unk_54 & 0x4000) != 0)
            {
                return 0;
            }
            CombatSprite_Update((CombatSprite*)data);
            if (func_ov003_020c37f8((void*)data) != 0) {
                snd = 0x5B;
                pan = func_ov003_020843b0(r, data->unk_060->unk_28);
            } else {
                snd = 0x53;
                pan = func_ov003_020843b0(0, data->unk_060->unk_28);
            }
            bias = 0x80000;
            if (data->unk_060->unk_24 == 0) {
                bias = -bias;
            }
            if (data->unk_016 == 2 && data->unk_008 == 1) {
                func_ov003_02087f00(0x1DB, pan);
            }
            if (data->unk_016 >= 4 && data->unk_016 <= 6) {
                if (func_ov003_020c5b2c((u16)snd, data->unk_060->unk_28 + bias, data->unk_060->unk_2C, data->unk_060->unk_30,
                                        data->unk_060->unk_30) == 1)
                {
                    func_ov003_02087f00(0x1DC, pan);
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)data) != 0) {
                r = 0;
            }
            break;
        case 2:
            if ((u32)((s32) * (u32*)data << 30) >> 30) {
                pan = func_ov003_020843b0(0, data->unk_060->unk_28);
                y   = func_ov003_020843ec(0, data->unk_060->unk_2C, data->unk_060->unk_30);
            } else {
                pan = func_ov003_020843b0(r, data->unk_060->unk_28);
                y   = func_ov003_020843ec(r, data->unk_060->unk_2C, data->unk_060->unk_30);
            }
            if (data->unk_060->unk_24 == 0) {
                pan = (s16)(pan - 0x20);
            } else {
                pan = (s16)(pan + 0x20);
            }
            CombatSprite_SetPosition((CombatSprite*)data, pan, y);
            func_ov003_02082730((CombatSprite*)data, 0x7FFFFFFF - (data->unk_060->unk_2C + 0x20000));
            CombatSprite_Render((CombatSprite*)data);
            break;
        case 3:
            CombatSprite_Release((CombatSprite*)data);
            break;
    }
    return r;
}

/// `Tsk_BtlEnm010_Rnge`'s task spawn. Nine arguments in, a 0x20-byte block out.
///
/// Two things are load-bearing. `arg6` is a by-value `u16` that the C *assigns*, and MWCC reuses
/// its incoming stack slot for the store -- the original's `moveq r1, #0 / strheq r1, [sp, #0x48]`
/// writes into the caller's outgoing area, which only happens for an assigned parameter whose
/// address is never taken. And the last argument to `EasyTask_CreateTask` is the *address of the
/// block*, not the block.
///
/// Open (8 bytes, pure register allocation): the reference loads the five stack arguments in the
/// order arg8, arg5, arg6, arg7, arg4 and assigns them lr, r1, r3, r8, ip -- one temp (arg7)
/// spills to the callee-saved r8. Writing the assignments in that load order matches the loads
/// but hands r8 to arg8 instead (13 bytes), and naming the temps / reordering their declarations
/// does not move the allocation. The field mapping and every value are correct either way; the
/// register choice is not reachable from the declaration list.
s32 func_ov011_02127f6c(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5, u16 arg6, s32 arg7, s16 arg8) {
    BtlEnm010RngeArgs t;
    TaskPool*         pool;

    if (func_ov003_020c37f8((void*)((u8*)arg0 + 0x84)) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u8*)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    if (arg4 == 1) {
        arg6 = 0;
    }
    t.field_00 = arg0;
    t.field_04 = arg1;
    t.field_08 = arg2;
    t.field_0C = arg3;
    t.field_10 = arg7;
    t.field_14 = arg5;
    t.field_16 = arg8;
    t.field_18 = arg4;
    t.field_1C = arg6;
    return EasyTask_CreateTask(pool, &data_ov011_0212c1e0, 0, 0, 0, (void*)&t);
}

/// `Tsk_BtlEnm010_SWA`'s task entry. A 3-way dispatch on the fourth argument, each arm handing
/// back the callee's own result -- and the default arm's result is the literal 1, set up before
/// the switch as `mov r1, #1` and copied into r0 at the end.
s32 func_ov011_0212801c(void* arg0, void* arg1, s32 arg2, s32 index) {
    void* p;
    s32   r;

    p = *(void**)((u8*)arg1 + 0x18);
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_02128070(p, (void*)arg2);
            break;
        case 1:
            r = func_ov011_02128150(p);
            break;
        case 3:
            r = func_ov011_02128250(p);
            break;
    }
    return r;
}

/// Rnge's spawn. The 0x20-byte data is a five-slot task-id array at `0x0C`; the argument is the
/// same 0x20-byte spawn block, and `0x18` is the slot count. The per-slot x is a **16-bit**
/// fixed-point sum: the division result and the `0x14 - step/2` bias are each rounded through
/// `lsl #0x10 / lsr #0x10` and then added and rounded again, so the whole expression is `s16`.
///
/// Open (+28 size): the reference keeps four values live across the `021282b8` call (p, a, i and
/// the bias, pre-shifted `lsl #0x16` into r7) and re-derives `0x18`/`0x1C` at each use. Naming
/// `count`/`step` parks them in callee-saved registers (+4 each; both re-derived now, -3 bytes).
/// The bias wants its own statement before the call -- tried, and it costs 2 bytes on its own;
/// the remaining piece is that whole pre-call schedule taken together.
s32 func_ov011_02128070(void* p, void* a) {
    s32 i;

    MI_CpuSet(p, 0, 0x20);
    for (i = 0; i < *(s32*)((u8*)a + 0x18); i++) {
        s32 v;

        v = (s16)((s16)_s32_div_f(*(u16*)((u8*)a + 0x1C) * i, *(s32*)((u8*)a + 0x18) - 1) +
                  (s16)((s32) * (u16*)((u8*)a + 0x14) - *(u16*)((u8*)a + 0x1C) / 2));
        *(s32*)((u8*)p + 0x0C + i * 4) =
            func_ov011_021282b8(*(s32*)((u8*)a + 0x00), *(s32*)((u8*)a + 0x04), *(s32*)((u8*)a + 0x08), *(s32*)((u8*)a + 0x0C),
                                v, *(s32*)((u8*)a + 0x10), *(s16*)((u8*)a + 0x16));
    }
    for (; i < 5; i++) {
        *(s32*)((u8*)p + 0x0C + i * 4) = -1;
    }
    *(s32*)((u8*)p + 0x04) = (func_ov003_020c37f8((void*)((u8*)*(u32*)a + 0x84)) != 0) ? 1 : 0;
    *(u32*)((u8*)p + 0x00) = *(u32*)a;
    return 1;
}

/// Rnge's per-frame handler. Returns 0 once all five sub-task slots read back as `-1`, and 1
/// otherwise -- the `cmp r4, #0x5 / movne / moveq` pair, so the trailing value is a comparison
/// against the loop bound and not a flag.
///
/// The owner at `+0x00` is a local, not a re-read: the original loads it once and then uses it
/// for the `0x54` test, the `0x84`/`0x28` pair and the sound, so the `arg1` spelling here would
/// cost four loads.
s32 func_ov011_02128150(void* p) {
    u32*  o;
    void* pool;
    s32   r;
    s32   i;

    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    if (*(u32*)p != 0 && (*(s32*)((u8*)*(u32*)p + 0x54) & 4) != 0) {
        return r;
    }
    if (*(s16*)((u8*)p + 0x08) == 0) {
        o = *(u32**)p;
        s32 pan;
        if (func_ov003_020c37f8((void*)((u8*)o + 0x84)) != 0) {
            pan = func_ov003_020843b0(1, *(s32*)((u8*)o + 0x28));
        } else {
            pan = func_ov003_020843b0(0, *(s32*)((u8*)o + 0x28));
        }
        func_ov003_02087f00(0x1D9, pan);
        *(s16*)((u8*)p + 0x08) = *(s16*)((u8*)p + 0x08) + 1;
    }
    if (*(s32*)((u8*)p + 0x04) == 0) {
        pool = *(void**)data_ov003_020e71b8;
    } else {
        pool = (void*)((u8*)(*(void**)data_ov003_020e71b8) + 0x8C + 0x8000);
    }
    for (i = 0; i < 5; i++) {
        EasyTask_ValidateTaskId(pool, (void*)((u8*)p + 0x0C + i * 4));
        if (*(s32*)((u8*)p + 0x0C + i * 4) == -1) {
            r++;
        }
    }
    return (r == 5) ? 0 : 1;
}

/// SWA's phase-3 handler. Walks the five task ids at `0x0C`..`0x1C` and sets each live task's
/// `markedForDel`. The pool is the plain global unless `0x04` is non-zero, in which case it is
/// the same base biased by `0x8C + 0x8000` -- the same pair of arms as `func_ov011_02127c84` and
/// `func_ov011_02127f6c`, third and fourth time.
s32 func_ov011_02128250(void* p) {
    TaskPool* pool;
    s32       i;

    if (*(u32*)((u8*)p + 4) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u8*)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    for (i = 0; i < 5; i++) {
        Task* t = EasyTask_GetTaskById(pool, *((u32*)((u8*)p + i * 4) + 3));
        if (t != NULL) {
            /* Not `t->markedForDel = 1`: that is the same bit, but MWCC narrows the
             * halfword byte-wise and comes out with three instructions where the original has
             * one `ldrh` / `orr` / `strh`. The OR has to be spelled on the whole halfword. */
            *(u16*)((u8*)t + 4) = *(u16*)((u8*)t + 4) | 0x10;
        }
    }
    return 1;
}

/// Sprl's spawn, and the second instance of the `0x20`-byte block shape `func_ov011_02127f6c`
/// established -- same struct, and the 0x1C word is left unset here.
///
/// Seven arguments: four in registers, then a `u16`, a word and an `s16` on the stack.
s32 func_ov011_021282b8(void* arg0, s32 arg1, s32 arg2, s32 arg3, u16 arg4, s32 arg5, s16 arg6) {
    BtlEnm010SprlArgs t;
    TaskPool*         pool;

    if (func_ov003_020c37f8((void*)((u8*)arg0 + 0x84)) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u8*)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    t.field_00 = arg0;
    t.field_04 = arg1;
    t.field_08 = arg2;
    t.field_0C = arg3;
    t.field_10 = arg5;
    t.field_16 = arg6;
    t.field_14 = arg4;
    return EasyTask_CreateTask(pool, &data_ov011_0212c1f8, 0, 0, 0, (void*)&t);
}

/// `Tsk_BtlEnm010_SingleShot`'s task entry. Same shape as `func_ov011_0212801c` and
/// `func_ov011_02128758`: a four-way switch, the default arm's result is the literal 1 set up
/// before the switch, and every arm copies its callee's result into r1 for the shared tail.
s32 func_ov011_02128348(void* arg0, void* arg1, s32 arg2, s32 index) {
    void* p;
    s32   r;

    p = *(void**)((u8*)arg1 + 0x18);
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_021283a8((BtlEnm010SingleShot*)p, arg2);
            break;
        case 1:
            r = func_ov011_021284bc(p);
            break;
        case 2:
            r = func_ov011_02128698(p);
            break;
        case 3:
            r = func_ov011_02128704(p);
            break;
    }
    return r;
}

/// SingleShot's initialiser, and the reason `BtlEnm010SingleShot` is 0xB4 -- the `MI_CpuSet`
/// clears exactly that and nothing else in the function writes further.
///
/// The three-entry table at `data_ov011_0212c204` is scaled by the spawn argument's `0x10` with a
/// **64-bit** multiply and a 64-bit `>> 12`: the `smull`/`adds`/`adc`/`lsr`/`orr` chain is not
/// something a 32-bit product produces, so the C has to name the 64-bit type. The stride is
/// `i * 16` and the destination offset is `+0x70`, so the table lands inside the 0xB4 at
/// `0x70`, `0x80` and `0x90`.
s32 func_ov011_021283a8(BtlEnm010SingleShot* data, void* arg1) {
    s32  i;
    s32  v;
    u32* o;

    MI_CpuSet(data, 0, 0xB4);
    func_ov011_021258b4(((*(u32*)((u8*)*(u32*)arg1 + 0x84) << 30) >> 30), (CombatSprite*)((u8*)data + 4), 2);
    Mini108_VBlank((CombatSprite*)((u8*)data + 4), 0, 0);
    for (i = 0; i < 3; i++) {
        *(s32*)((u8*)data + 0x70 + i * 16) =
            (s32)((((long long)data_ov011_0212c204[i] * (long long)(*(s32*)((u8*)arg1 + 0x10))) + 0x800) >> 12);
    }
    *(u32*)((u8*)data + 0x00) = *(u32*)arg1;
    *(s32*)((u8*)data + 0x98) = *(s32*)((u8*)*(u32*)arg1 + 0x28);
    *(s32*)((u8*)data + 0x9C) = *(s32*)((u8*)*(u32*)arg1 + 0x28) + *(s32*)((u8*)arg1 + 0x4);
    *(s32*)((u8*)data + 0xA0) = *(s32*)((u8*)*(u32*)arg1 + 0x2C) + *(s32*)((u8*)arg1 + 0x8);
    *(s32*)((u8*)data + 0xA4) = *(s32*)((u8*)*(u32*)arg1 + 0x30) + *(s32*)((u8*)arg1 + 0xC);
    *(u16*)((u8*)data + 0xA8) = *(s16*)((u8*)arg1 + 0x16);
    *(u16*)((u8*)data + 0xAA) = *(u16*)((u8*)arg1 + 0x14);
    o                         = *(u32**)arg1;
    *(u16*)((u8*)data + 0xAC) = *(u16*)((u8*)o + 0x4);
    *(u16*)((u8*)data + 0xAE) = *(u16*)((u8*)o + 0x6);
    *(u16*)((u8*)data + 0xB0) = *(u16*)((u8*)o + 0x8);
    *(u16*)((u8*)data + 0xB2) = *(u16*)((u8*)o + 0xA);
}

/// SingleShot's phase-2 worker: steps the three 0x10-byte projectile records at `p + 0x68`,
/// projects each one to screen space, submits one effect per projectile, and returns 0 once all
/// three have left the screen -- so the task retires when the volley is gone.
///
/// The guards and the first-frame block are the Rnge worker's shape: bail to `r` (= 0) when
/// `func_ov003_020c3c28()` is set or the owner's `0x54` flags have bit 2, then tick the
/// `CombatSprite` at `+0x04`. The `func_ov003_020c37f8(p + 4)` arm picks the mode (`0`/`1`, later
/// reused by `func_ov003_0208442c` and `func_ov003_020cb744`) and the `u16` effect-table index
/// (`0x5C`/`0x54`) handed to `func_ov003_0208a114`, and computes the pan from `unk_098`. The
/// `s16` flag at `0x64` fires SE `0x1D9` exactly once, and the increment stores before the call.
///
/// The record block is 3 records, 0x10 apart: `+0x00` position, `+0x04` velocity, `+0x08`
/// acceleration (seeded at 0x70/0x80/0x90 by `func_ov011_021283a8`), `+0x0C`/`+0x0E` the projected
/// `s16` pair `func_ov011_02128698` renders. While the delay halfword at `0xA8` is positive it
/// only counts down; otherwise the physics advances (pos += vel, vel += acc). The projection is
/// `func_ov003_020cbc50(unk_0AA, pos)` biased by the frame's `func_ov003_02084348` offsets, spelled
/// as the 4.12 idiom `(out + (bias << 12)) >> 12`.
///
/// Per projectile, `func_ov003_0208a1a4` submits the effect against the copied halfword block at
/// `+0xAC`; a `== 1` result plays SE `0x1DA` with the projected x as pan. The survivor test is
/// `func_ov003_0208442c(mode, x)` against `[0 - 0x8000, func_ov003_020cb744(mode) + 0x8000]`, with
/// the `||` short-circuiting so `func_ov003_020cb744` only runs on the fall-through; `r` counts
/// the records outside it and the tail is the `cmp r8, #3 / movne / moveq` pair.
s32 func_ov011_021284bc(void* p) {
    s32   r;
    s32   mode;
    u16   id;
    s32   pan;
    s32   oa;
    s32   ob;
    s16   va;
    s16   vb;
    s32   i;
    void* rec;

    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    if (*(u32*)p != 0 && (*(s32*)((u8*)*(u32*)p + 0x54) & 4) != 0) {
        return r;
    }
    CombatSprite_Update((CombatSprite*)((u8*)p + 4));
    if (func_ov003_020c37f8((void*)((u8*)p + 4)) != 0) {
        mode = 1;
        id   = 0x5C;
        pan  = func_ov003_020843b0(mode, ((BtlEnm010SingleShot*)p)->unk_098);
    } else {
        mode = 0;
        id   = 0x54;
        pan  = func_ov003_020843b0(mode, ((BtlEnm010SingleShot*)p)->unk_098);
    }
    if (*(s16*)((u8*)p + 0x64) == 0) {
        *(s16*)((u8*)p + 0x64) = *(s16*)((u8*)p + 0x64) + 1;
        func_ov003_02087f00(0x1D9, pan);
    }
    func_ov003_02084348(mode, &va, &vb, ((BtlEnm010SingleShot*)p)->unk_09C, ((BtlEnm010SingleShot*)p)->unk_0A0,
                        ((BtlEnm010SingleShot*)p)->unk_0A4);
    rec = (void*)((u8*)p + 0x68);
    for (i = 0; i < 3; i++) {
        if (((BtlEnm010SingleShot*)p)->unk_0A8 > 0) {
            ((BtlEnm010SingleShot*)p)->unk_0A8 = ((BtlEnm010SingleShot*)p)->unk_0A8 - 1;
        } else {
            *(s32*)rec               = *(s32*)rec + *(s32*)((u8*)rec + 0x04);
            *(s32*)((u8*)rec + 0x04) = *(s32*)((u8*)rec + 0x04) + *(s32*)((u8*)rec + 0x08);
        }
        func_ov003_020cbc50(&oa, &ob, *(u16*)((u8*)p + 0xAA), *(s32*)rec);
        *(s16*)((u8*)rec + 0x0C) = (oa + (va << 12)) >> 12;
        *(s16*)((u8*)rec + 0x0E) = (ob + (vb << 12)) >> 12;
        if (func_ov003_0208a1a4(func_ov003_0208a114(id), (void*)((u8*)p + 0xAC), *(s16*)((u8*)rec + 0x0C),
                                *(s16*)((u8*)rec + 0x0E)) == 1)
        {
            func_ov003_02087f00(0x1DA, *(s16*)((u8*)rec + 0x0C));
        }
        {
            s32 v = func_ov003_0208442c(mode, *(s16*)((u8*)rec + 0x0C));
            if (v < 0 - 0x8000 || v > func_ov003_020cb744(mode) + 0x8000) {
                r++;
            }
        }
        rec = (void*)((u8*)rec + 0x10);
    }
    return (r == 3) ? 0 : 1;
}

// `CombatSprite_SetAffineTransform` (`CombatSprite_SetAffineTransform`) -- five arguments, declared in
// `Combat/Core/CombatSprite.h`.

/// SingleShot's phase-2 handler: three passes over a table and a 0x10-stride record block.
///
/// `base` is a genuine second counter in the original -- `mov r4, r6` before the loop and `r4`
/// never stepped again, so arguments 2 and 5 of `CombatSprite_SetAffineTransform` are always 0 while the
/// table index and the record pointer both advance. It has to be a separate local, not a second
/// name for `i`.
s32 func_ov011_02128698(void* p) {
    void* data;
    s32   base;
    s32   i;
    void* rec;

    data = p;
    base = 0;
    rec  = (void*)((u8*)data + 0x68);
    for (i = 0; i < 3; i++) {
        CombatSprite_SetAffineTransform((void*)((u8*)data + 4), base, data_ov011_0212c1ec[i], data_ov011_0212c1ec[i], base);
        CombatSprite_SetPosition((CombatSprite*)((u8*)data + 4), *(s16*)((u8*)rec + 0xC), *(s16*)((u8*)rec + 0xE));
        CombatSprite_Render((CombatSprite*)((u8*)data + 4));
        rec = (void*)((u8*)rec + 0x10);
    }
    return 1;
}

/// Ticks the sprite at `+0x04` and nothing else.
s32 func_ov011_02128704(void* p) {
    CombatSprite_Release((void*)((u8*)p + 4));
    return 1;
}

/// Spawns the Sprl task. As in `func_ov011_02127c84`, the last argument is the *address of the
/// data pointer* -- the prologue's `str r0, [sp, #0x8]` and the `str r1, [sp, #0x4]` that stores
/// `sp + 8` into the outgoing slot only make sense if the caller gets a pointer to a pointer.
/// It has to be a **local** copy: address-taken on the parameter itself makes MWCC push r0-r3
/// and the function comes out 8 bytes long.
s32 func_ov011_02128718(void* p) {
    void* arg0;

    arg0 = p;
    return EasyTask_CreateTask((TaskPool*)data_ov003_020e71b8, &data_ov011_0212c210, 0, 0, 0, (void*)&arg0);
}

/// `Tsk_BtlEnm010_Sprl`'s task entry, third of the three identical four-way dispatchers
/// (`0x0212801c`, `0x02128348`, `0x02128758`).
s32 func_ov011_02128758(void* arg0, void* arg1, s32 arg2, s32 index) {
    void* p;
    s32   r;

    p = *(void**)((u8*)arg1 + 0x18);
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_021287b8(p, arg2);
            break;
        case 1:
            r = func_ov011_021288c8(p);
            break;
        case 2:
            r = func_ov011_02128b80(p);
            break;
        case 3:
            r = func_ov011_02128c30(p);
            break;
    }
    return r;
}

/// Sprl's initialiser. The `MI_CpuSet` confirms 0xB8 independently. `arg1` is the owner and it is
/// re-read through `*(u32*)arg1` at every use rather than cached in a local -- the original
/// loads it five times and caching it costs those loads. The five-way fill writes the base
/// position to `0x70 + i * 4` and the base height to `0x84 + i * 4`, interleaved.
s32 func_ov011_021287b8(BtlEnm010Sprl* data, void* arg1) {
    s32 i;
    s32 v;

    MI_CpuSet(data, 0, 0xB8);
    *(u32*)((u8*)data + 0x00) = *(u32*)arg1;
    func_ov011_021258b4(*(u16*)((u8*)*(u32*)arg1 + 0x84), 0, 2);
    Mini108_VBlank((CombatSprite*)((u8*)data + 4), 0, 0);
    if (*(s32*)((u8*)*(u32*)arg1 + 0x24) == 0) {
        *(u16*)((u8*)data + 0xAC) = 0x8000;
        *(u16*)((u8*)data + 0xAE) = 0xFD00;
        v                         = 0x8000 - 0x50000;
    } else {
        *(u16*)((u8*)data + 0xAC) = 0;
        *(u16*)((u8*)data + 0xAE) = 0x300;
        v                         = 0x48000;
    }
    *(s32*)((u8*)data + 0x9C) = *(s32*)((u8*)*(u32*)arg1 + 0x28) + v;
    *(s32*)((u8*)data + 0xA0) = *(s32*)((u8*)*(u32*)arg1 + 0x2C);
    for (i = 0; i < 5; i++) {
        *(s32*)((u8*)data + 0x70 + i * 4) = *(s32*)((u8*)data + 0x9C);
        *(s32*)((u8*)data + 0x84 + i * 4) = *(s32*)((u8*)data + 0xA0);
    }
    *(s32*)((u8*)data + 0x98) = 0 - 0x10000;
    *(s32*)((u8*)data + 0xA4) = 0x8000;
    *(s32*)((u8*)data + 0xA8) = 0x800;
    *(u16*)((u8*)data + 0xB0) = *(u16*)((u8*)*(u32*)arg1 + 0x4);
    *(u16*)((u8*)data + 0xB2) = *(u16*)((u8*)*(u32*)arg1 + 0x6);
    *(u16*)((u8*)data + 0xB4) = *(u16*)((u8*)*(u32*)arg1 + 0x8);
    *(u16*)((u8*)data + 0xB6) = *(u16*)((u8*)*(u32*)arg1 + 0xA);
}

/// Sprl's phase-2 worker: advances a five-joint spiral chain, clamps the joints to a maximum
/// spacing, renders one effect per joint, and returns 0 once the spiral's radius has grown past
/// the global limit -- otherwise it ticks the sprite and returns 1.
///
/// The guards match `func_ov011_021284bc`. The physics head is a polar equation: the radius at
/// `0xA4` accumulates its rate at `0xA8`, the `u16` angle at `0xAC` accumulates `0xAE` (wrapping
/// at the halfword store), and `func_ov003_020cbcb4` writes the head's x/y -- `unk_070[0]` /
/// `unk_084[0]` -- from angle and radius; the base at `0x9C`/`0xA0` is then added in. The
/// `s16` flag at `0x64` plays SE `0x1DE` exactly once, its pan from `func_ov003_020843b0` against
/// the owner's `+0x28`.
///
/// The clamp loop (`i = 1..4`) is a rope constraint: if joint `i` is further than
/// `(data_ov011_0212c21c[i - 1] + data_ov011_0212c21c[i]) * 16 / 2` (two 8.8 radii summed into
/// 4.12, halved) from joint `i - 1` it is pulled back onto that distance. The direction is a
/// 64-bit `dx * half + 0x800 >> 12` round divided by the distance with `FX_Divide`, per axis, and
/// the `x` store lands between the two calls exactly as the original interleaves them. The `* 16`
/// sum and its `>> 1` are spelled twice on purpose: the original folds the shifted operand into
/// the `cmp` and materialises `r5` from a second `asr`.
///
/// The render loop (`i = 0..4`) tints a `func_ov003_0208a114(0x57)` record with the half-radius
/// byte and submits it through `func_ov003_0208a164` against the copied halfword block at `+0xB0`
/// with the joint's x/y and `unk_098` as z; a `== 1` result plays SE `0x1DA`. The exit test is
/// `unk_0A4 > data_ov003_020e71b8 + 0x3D000 + 0x7CC` -- the four-hop global load, spelled with the
/// split bias exactly like the rest of this file.
s32 func_ov011_021288c8(void* p) {
    s32                    dx;
    s32                    dy;
    s32                    dist;
    s32                    half;
    s32                    pan;
    s32                    i;
    struct BtlEnm010CmdTbl fx;
    s32                    t;
    s16                    v;

    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    if (*(u32*)p != 0 && (*(s32*)((u8*)*(u32*)p + 0x54) & 4) != 0) {
        return 0;
    }
    *(s32*)((u8*)p + 0xA4) = *(s32*)((u8*)p + 0xA4) + *(s32*)((u8*)p + 0xA8);
    *(u16*)((u8*)p + 0xAC) = *(u16*)((u8*)p + 0xAC) + *(u16*)((u8*)p + 0xAE);
    func_ov003_020cbcb4(((BtlEnm010Sprl*)p)->unk_070, ((BtlEnm010Sprl*)p)->unk_084, *(u16*)((u8*)p + 0xAC),
                        *(s32*)((u8*)p + 0xA4), 0x800);
    ((BtlEnm010Sprl*)p)->unk_070[0] = ((BtlEnm010Sprl*)p)->unk_070[0] + *(s32*)((u8*)p + 0x9C);
    ((BtlEnm010Sprl*)p)->unk_084[0] = ((BtlEnm010Sprl*)p)->unk_084[0] + *(s32*)((u8*)p + 0xA0);
    if (*(s16*)((u8*)p + 0x64) == 0) {
        *(s16*)((u8*)p + 0x64) = *(s16*)((u8*)p + 0x64) + 1;
        if (func_ov003_020c37f8((void*)((u8*)p + 4)) != 0) {
            pan = func_ov003_020843b0(1, *(s32*)((u8*)*(u32*)p + 0x28));
        } else {
            pan = func_ov003_020843b0(0, *(s32*)((u8*)*(u32*)p + 0x28));
        }
        func_ov003_02087f00(0x1DE, pan);
    }
    for (i = 1; i < 5; i++) {
        dx   = ((BtlEnm010Sprl*)p)->unk_070[i] - ((BtlEnm010Sprl*)p)->unk_070[i - 1];
        dy   = ((BtlEnm010Sprl*)p)->unk_084[i] - ((BtlEnm010Sprl*)p)->unk_084[i - 1];
        dist = func_ov003_020cba2c(0, 0, dx, dy);
        half = ((data_ov011_0212c21c[i - 1] + data_ov011_0212c21c[i]) * 16) >> 1;
        if (dist < (((data_ov011_0212c21c[i - 1] + data_ov011_0212c21c[i]) * 16) >> 1)) {
            continue;
        }
        ((BtlEnm010Sprl*)p)->unk_070[i] =
            ((BtlEnm010Sprl*)p)->unk_070[i - 1] + FX_Divide((s32)(((long long)dx * (long long)half + 0x800) >> 12), dist);
        ((BtlEnm010Sprl*)p)->unk_084[i] =
            ((BtlEnm010Sprl*)p)->unk_084[i - 1] + FX_Divide((s32)(((long long)dy * (long long)half + 0x800) >> 12), dist);
    }
    for (i = 0; i < 5; i++) {
        t         = (data_ov011_0212c21c[i] * 16) >> 12;
        v         = (s16)(t >> 1);
        fx        = *func_ov003_0208a114(0x57);
        fx.unk_05 = (u8)v;
        fx.unk_06 = (u8)v;
        fx.unk_07 = (u8)v;
        if (func_ov003_0208a164(&fx, (void*)((u8*)p + 0xB0), ((BtlEnm010Sprl*)p)->unk_070[i], ((BtlEnm010Sprl*)p)->unk_084[i],
                                ((BtlEnm010Sprl*)p)->unk_098) == 1)
        {
            s32 pan;
            if (func_ov003_020c37f8((void*)((u8*)p + 4)) != 0) {
                pan = func_ov003_020843b0(1, ((BtlEnm010Sprl*)p)->unk_070[i]);
            } else {
                pan = func_ov003_020843b0(0, ((BtlEnm010Sprl*)p)->unk_070[i]);
            }
            func_ov003_02087f00(0x1DA, pan);
        }
    }
    if (*(s32*)((u8*)p + 0xA4) > *(s32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x7CC)) {
        return 0;
    }
    CombatSprite_Update((CombatSprite*)((u8*)p + 4));
    return 1;
}

/// Sprl's phase-2 handler: five passes, each one projecting, positioning and then animating.
///
/// The two arrays are reached as `*(data + i * 4 + 0x70)` and `*(data + i * 4 + 0x84)`, written
/// here as subscripts off a `data + i * 4` base so the scaled add stays inline -- as a single
/// `+ 0x70` / `+ 0x84` bias it materialises `i * 4` into a register instead, which is what
/// `func_ov011_02128250` does and why that one is two words long.
///
/// `func_ov003_02084348`'s arguments are `(mode, out, out, x, y, z)` -- the reference spills
/// `unk_084[i]` into the first stack slot and `unk_098` into the second, so y comes before the
/// z height here, in the same order `func_ov011_021284bc` passes its position triple.
s32 func_ov011_02128b80(BtlEnm010Sprl* data) {
    s16 v0;
    s16 v1;
    s32 i;
    s32 z;

    z = 0;
    for (i = 0; i < 5; i++) {
        func_ov003_02084348(z, &v0, &v1, data->unk_070[i], data->unk_084[i], data->unk_098);
        CombatSprite_SetPosition((CombatSprite*)((u8*)data + 4), v0, v1);
        func_ov003_02082730((CombatSprite*)((u8*)data + 4), 0x7FFFFFFF - data->unk_084[i]);
        CombatSprite_SetAffineTransform((void*)((u8*)data + 4), z, data_ov011_0212c21c[i], data_ov011_0212c21c[i], z);
        CombatSprite_Render((CombatSprite*)((u8*)data + 4));
    }
    return 1;
}

/// Ticks the sprite at `+0x04` and nothing else. Second of the two identical 20-byte tickers
/// (`0x02128704` was the first).
s32 func_ov011_02128c30(void* p) {
    CombatSprite_Release((void*)((u8*)p + 4));
    return 1;
}

/// Spawns the Tatt task. The plain pool-or-biased pair, and -- for the fourth time now -- the
/// last argument to `EasyTask_CreateTask` is the address of a *local* copy of the data pointer.
s32 func_ov011_02128c44(void* p) {
    void*     arg0;
    TaskPool* pool;

    arg0 = p;
    if (func_ov003_020c37f8((void*)((u8*)arg0 + 0x84)) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u8*)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    return EasyTask_CreateTask(pool, &data_ov011_0212c240, 0, 0, 0, (void*)&arg0);
}

/// Tatt's RG-shaped phase reset, at a `+0x200` base: store the incoming word at `0x224`, then
/// clear the three `s16`s at `0x228`/`0x22A`/`0x22C` from a single `mov r1, #0`. The three
/// stores go out in ascending address order.
void func_ov011_02128ca4(BtlEnm010Tatt* data, s32 arg1) {
    data->unk_224 = arg1;
    data->unk_228 = 0;
    data->unk_22A = 0;
    data->unk_22C = 0;
}

/// Tatt's per-frame worker, phase 0. The first-frame block is four "set bit 0, then set bit 3"
/// pairs over the same four halfwords, and **the `& ~1` half of the second pair is missing** --
/// the `bic` is only in the first pair. Transcribed as written; folding the two would lose four
/// instructions. The `0x10000` strides at `0x7C`/`0x104`/`0x18C`/`0x214` and the `0x70000` seed in
/// the fill loop confirm that the 0x88-stride block is four 0x88-byte records.
void func_ov011_02128cc0(BtlEnm010Tatt* data) {
    s32  i;
    s32  v;
    s32* p;

    if (data->unk_228 == 0) {
        *(u16*)((u8*)data + 0x88)  = (*(u16*)((u8*)data + 0x88) & 0xFFFE) | 1;
        *(u16*)((u8*)data + 0x110) = (*(u16*)((u8*)data + 0x110) & 0xFFFE) | 1;
        *(u16*)((u8*)data + 0x198) = *(u16*)((u8*)data + 0x198) & 0xFFFE;
        *(u16*)((u8*)data + 0x220) = *(u16*)((u8*)data + 0x220) & 0xFFFE;
        *(u16*)((u8*)data + 0x88)  = *(u16*)((u8*)data + 0x88) | 8;
        *(u16*)((u8*)data + 0x110) = *(u16*)((u8*)data + 0x110) | 8;
        *(u16*)((u8*)data + 0x198) = *(u16*)((u8*)data + 0x198) | 8;
        *(u16*)((u8*)data + 0x220) = *(u16*)((u8*)data + 0x220) | 8;
        *(s32*)((u8*)data + 0x07C) = 0x800;
        *(s32*)((u8*)data + 0x104) = 0x800;
        *(s32*)((u8*)data + 0x18C) = 0x1000;
        *(s32*)((u8*)data + 0x214) = 0x1000;
        func_ov011_02128f10(data, 0xEE38);
        func_ov011_02128f10((void*)((u8*)data + 0x8C), 0xEE38);
        func_ov011_02128f10((void*)((u8*)data + 0x114), 0xFF49);
        func_ov011_02128f10((void*)((u8*)data + 0x19C), 0xFF49);
        v = 0 - 0x70000;
        p = (s32*)((u8*)data + 0x80);
        for (i = 0; i < 4; i++) {
            p[0] = (((u32)(*(u8*)((u8*)data + 0x24C) << 31) >> 31) != 0) ? 0 - data_ov011_0212c24c[i] : data_ov011_0212c24c[i];
            p[1] = v;
            p    = (s32*)((u8*)p + 0x88);
        }
        data->unk_228 = data->unk_228 + 1;
    }
    p = (s32*)((u8*)data + 0x88);
    for (i = 0; i < 4; i++) {
        if (((u32)(*(u16*)((u8*)p + 0) << 28) >> 31) == 1) {
            break;
        }
        p = (s32*)((u8*)p + 0x88);
    }
    if (i != 4) {
        return;
    }
    func_ov011_02128ca4(data, (s32)func_ov011_02128e30);
}

/// Tatt's arm-throw phase. On the first frame it sets bit 4 of all four `0x88`-strided records;
/// after that it looks for a record that *already* has bit 4 and does nothing if it finds one --
/// so the reset only takes effect once every record has had its turn. The two loops keep their
/// counter and walking pointer in *swapped* registers (`r2`/`r3` then `r3`/`r2`), which means the
/// original's four locals are four distinct variables, not two reused ones.
///
/// The bit-4 test is the same shift-extract as `func_ov011_02129110`, here at `<< 27`, and it is
/// compared against literal 1 rather than tested for truth.
void func_ov011_02128e30(BtlEnm010Tatt* data) {
    s32  i;
    u16* p;
    u16* q;
    s32  j;

    if (data->unk_228 == 0) {
        data->unk_228 = data->unk_228 + 1;
        p             = (u16*)((u8*)data + 0x88);
        for (i = 0; i < 4; i++) {
            *p = *p | 0x10;
            p  = (u16*)((u8*)p + 0x88);
        }
    }
    q = (u16*)((u8*)data + 0x88);
    for (j = 0; j < 4; j++) {
        if (((u32)(*q << 27) >> 31) == 1) {
            break;
        }
        q = (u16*)((u8*)q + 0x88);
    }
    if (j != 4) {
        return;
    }
    func_ov011_02128ca4(data, (s32)func_ov011_02128eb0);
}

/// Every sixtieth frame, set bit 5 of the sub-object's `0x84` halfword. Then bump `0x228`.
///
/// The `% 60` is the signed magic-multiply sequence (`smull` with `0x88888889`), so it is a plain
/// `%` on the `s16` -- no `(u32)` cast, which would give the `umull` form and lose the fixup.
/// The bit-5 set is spelled on the whole halfword, not as a bitfield: see `func_ov011_02128250`.
void func_ov011_02128eb0(BtlEnm010Tatt* data) {
    void* p;

    if (data->unk_228 % 60 == 0) {
        p = func_ov011_02128f80(data);
        if (p != NULL) {
            *(u16*)((u8*)p + 0x84) = *(u16*)((u8*)p + 0x84) | 0x20;
        }
    }
    data->unk_228 = data->unk_228 + 1;
}

/// A linear search over a 0x10-entry table, with a wrap to 0 on overflow, then a palette swap.
///
/// The three tables are two bytes apart in `symbols.txt` but are all indexed with an 8-byte
/// stride, so the subscript is `i * 4` on a `u16*` and not `i`.
///
/// Open (16 bytes): the reference keeps a raw counter (`mov r0, r4, lsl #3` recomputed per
/// iteration) and the parameter in r5; ours CSEs `i * 4` into a scaled induction variable in
/// r3/r12 and parks the parameter in r4.  Three spellings tried -- declaration order, dropping
/// the `data = p` alias, and the do/while loop shape of the 02125750 recipe -- all
/// byte-identical to each other.  The scaled induction is not reachable from the C as written.
void func_ov011_02128f10(void* p, u16 v) {
    s32 i;

    i = 0;
    do {
        if (v < data_ov011_0212c28c[i * 4]) {
            break;
        }
        i++;
    } while (i < 0x10);
    if (i == 0x10) {
        i = 0;
    }
    *(u16*)((u8*)p + 0x70) = v;
    Mini108_VBlank((CombatSprite*)p, data_ov011_0212c28e[i * 4], 0);
    CombatSprite_SetFlip((CombatSprite*)p, data_ov011_0212c290[i * 4]);
}

/// Tatt's "nearest live record" search. The `0x23C` step is computed against one of *two* chase
/// pointers -- `+0x89C` with mode 1 when bit 1 of `0x24C` is SET and `+0x898` with mode 0 when it
/// is clear -- and the branch direction is the `beq` on the bit test, so the nonzero arm is the
/// fall-through and the zero arm is the branch target.
///
/// The `0.5f` round is a ternary with the `(s32)` cast *outside* it: one `_ffix` after the float
/// merge per arm of the mirror test, with the helper re-derived in the condition and in both
/// ternary arms (three calls; the original caches nothing). Arm A's add is commuted
/// (`0.5f + f`, i.e. `_fadd` with the constant in r0), arm B is `f - 0.5f`.
///
/// A record is *skipped* when bit 0 is clear or bit 4 is set, and the two tests are chained with
/// `lslne`/`cmpne` rather than branched, so they are one short-circuiting `&&` in the C. The
/// nearest-so-far update is a paired `movlt` on the distance and the pointer, which is an if/else
/// assigning both -- not two independent ifs.
void* func_ov011_02128f80(BtlEnm010Tatt* data) {
    s32   v;
    s32   d;
    s32   i;
    void* best;
    void* p;

    best = NULL;
    v    = 0x7FFFFFFF;
    if (((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31) != 0) {
        d = (s32)(func_ov003_020843b0(1, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x89C)) + 0x28)) > 0
                      ? 0.5f + (f32)(func_ov003_020843b0(
                                         1, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x89C)) + 0x28))
                                     << 12)
                      : (f32)(func_ov003_020843b0(1,
                                                  *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x89C)) + 0x28))
                              << 12) -
                            0.5f);
    } else {
        d = (s32)(func_ov003_020843b0(0, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28)) > 0
                      ? 0.5f + (f32)(func_ov003_020843b0(
                                         0, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28))
                                     << 12)
                      : (f32)(func_ov003_020843b0(0,
                                                  *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28))
                              << 12) -
                            0.5f);
    }
    p = (void*)((u8*)data + 4);
    for (i = 0; i < 4; i++) {
        if ((((u32)(*(u16*)((u8*)p + 0x84) << 30) >> 31) != 0) && (((u32)(*(u16*)((u8*)p + 0x84) << 26) >> 31) == 0)) {
            d = d - (*(s32*)((u8*)data + 0x23C) + *(s32*)((u8*)p + 0x68));
            if (d < 0) {
                d = 0 - d;
            }
            if (d < v) {
                v    = d;
                best = p;
            }
        }
        p = (void*)((u8*)p + 0x88);
    }
    return best;
}

/// Four sub-objects at a 0x88 stride from `+0x04`, each dispatched on three bits of its `0x84`
/// halfword. The three tests are bits 3, 4 and 5 in that order, and the halfword is re-read for
/// each one -- so it has to be written three times, not hoisted.
///
/// The task struct here is **not** pinned: the stride and the `0x60`/`0x64`/`0x84` offsets are
/// the only evidence, and they do not fit any struct characterised so far. Raw offsets on purpose.
void func_ov011_02129110(void* p) {
    s32   i;
    void* rec;

    rec = (void*)((u8*)p + 4);
    for (i = 0; i < 4; i++) {
        if ((u32)(*(u16*)((u8*)rec + 0x84) << 28) >> 31) {
            func_ov011_02129188(p, rec);
        }
        if ((u32)(*(u16*)((u8*)rec + 0x84) << 27) >> 31) {
            func_ov011_021293a8(p, rec);
        }
        if ((u32)(*(u16*)((u8*)rec + 0x84) << 26) >> 31) {
            func_ov011_02129410(p, rec);
        }
        rec = (void*)((u8*)rec + 0x88);
    }
}

/// One record's arc launcher -- the bit-3 worker `func_ov011_02129110` dispatches, and `p` is the
/// Tatt data the 0x88-stride record block hangs off. Raw offsets on purpose: the record fits no
/// declared struct.
///
/// The `0x64` halfword is a two-state machine. State 0 spends `0x62` frames (latched to 0x14 on
/// the first frame) tweening the record's `0x68`/`0x6C` displacement along one of the two
/// three-point curves `data_ov011_0212c25c` / `data_ov011_0212c274` (six `s32` words each, handed
/// to `func_ov003_020cbd30` with `t` = the frame counter rounded to 4.12 and divided by the
/// limit). Bit 0 of the record's `0x84` flag halfword picks the curve *and* which of the two
/// 8-byte halves of `data_ov011_0212c230` supplies the fixed origin the sprite is aimed from;
/// `p + 0x24C` bit 0 mirrors every x component (negating the origin on the way in and the curve
/// output on the way out). The state-0 tail -- shared by both arms, and the reason the else arm
/// only latches -- aims the sprite: `func_ov003_020cba14(origin, displacement)` measures the
/// angle and `func_ov011_02128f10` converts it into the palette swap. State 1 then walks `0x68`
/// the last 0x4000 per frame to the target displacement latched at `0x7C`/`0x80` (by
/// `func_ov011_02129410`'s state 0), snaps, resets the machine and clears bit 3 so the dispatch
/// stops. The `0x78` accumulator runs on every call regardless of state: +0x19A per call -- the
/// original emits it as two `add`s, `#0x9a` then `#0x100` -- clamped at 0x1000.
///
/// `ROUND` is the file-scope macro in `BtlEnm010.c`; `_fflt`/`_fadd`/`_fsub`/`_ffix`/`_s32_div_f`
/// are the soft-float helpers this file already calls bare. The frame slot pair the original
/// builds for `func_ov003_020cbd30`'s output is spelled `m[2]` here.
void func_ov011_02129188(void* p, void* rec) {
    struct {
        s32 m[2];
        s32 org[2];
    } f;
    s32        state;
    const s32* tpl;
    s32        limit;
    s32        count;
    s32        cur;
    s32        dst;
    s32        d;

    state = *(s16*)((u8*)rec + 0x64);
    switch (state) {
        case 0:
            if (((u32)(*(u16*)((u8*)rec + 0x84) << 31) >> 31) == 1) {
                f.org[0] = data_ov011_0212c230[0];
                f.org[1] = data_ov011_0212c230[1];
                tpl      = data_ov011_0212c25c;
            } else {
                f.org[0] = data_ov011_0212c230[2];
                f.org[1] = data_ov011_0212c230[3];
                tpl      = data_ov011_0212c274;
            }
            if (((u32)(*(u8*)((u8*)p + 0x24C) << 31) >> 31) != 0) {
                f.org[0] = 0 - f.org[0];
            }
            if (*(s16*)((u8*)rec + 0x60) == 0) {
                *(s16*)((u8*)rec + 0x62) = 0x14;
            }
            limit = *(s16*)((u8*)rec + 0x62);
            count = *(s16*)((u8*)rec + 0x60);
            if (count >= limit) {
                *(s16*)((u8*)rec + 0x60) = 0;
                *(s16*)((u8*)rec + 0x64) = 1;
            } else {
                func_ov003_020cbd30(f.m, tpl[0], tpl[1], tpl[2], tpl[3], tpl[4], tpl[5], _s32_div_f(ROUND(count), limit));
                *(s32*)((u8*)rec + 0x68) = (((u32)(*(u8*)((u8*)p + 0x24C) << 31) >> 31) != 0) ? 0 - f.m[0] : f.m[0];
                *(s32*)((u8*)rec + 0x6C) = f.m[1];
                *(s16*)((u8*)rec + 0x60) = *(s16*)((u8*)rec + 0x60) + 1;
            }
            func_ov011_02128f10(rec,
                                func_ov003_020cba14(f.org[0], f.org[1], *(s32*)((u8*)rec + 0x68), *(s32*)((u8*)rec + 0x6C)));
            break;
        case 1:
            if (*(s16*)((u8*)rec + 0x60) == 0) {
                *(s16*)((u8*)rec + 0x60) = *(s16*)((u8*)rec + 0x60) + 1;
                func_ov011_02128f10(rec, 0xC000);
            }
            cur = *(s32*)((u8*)rec + 0x68);
            dst = *(s32*)((u8*)rec + 0x7C);
            d   = cur - dst;
            if (d < 0) {
                d = 0 - d;
            }
            if (d < 0x4000) {
                *(s32*)((u8*)rec + 0x68) = dst;
                *(u16*)((u8*)rec + 0x84) = *(u16*)((u8*)rec + 0x84) & ~8;
                *(s16*)((u8*)rec + 0x60) = 0;
                *(s16*)((u8*)rec + 0x64) = 0;
            } else if (cur > dst) {
                *(s32*)((u8*)rec + 0x68) = *(s32*)((u8*)rec + 0x68) - 0x4000;
            } else {
                *(s32*)((u8*)rec + 0x68) = *(s32*)((u8*)rec + 0x68) + 0x4000;
            }
            break;
    }
    *(s32*)((u8*)rec + 0x78) = *(s32*)((u8*)rec + 0x78) + 0x19A;
    if (*(s32*)((u8*)rec + 0x78) > 0x1000) {
        *(s32*)((u8*)rec + 0x78) = 0x1000;
    }
}

/// One sub-object's teardown, and the second argument is the one that matters -- `mov r4, r1`
/// with no use of r0 anywhere. The `0x84` bit-4 clear is the mirror image of the bit-5 set in
/// `func_ov011_02128eb0`, and again has to be spelled on the whole halfword.
void func_ov011_021293a8(void* arg0, void* p) {
    if (*(s16*)((u8*)p + 0x60) == 0) {
        *(s16*)((u8*)p + 0x60) = *(s16*)((u8*)p + 0x60) + 1;
        Mini108_VBlank((CombatSprite*)p, 5, 1);
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)p) == 0) {
        return;
    }
    Mini108_VBlank((CombatSprite*)p, 6, 1);
    *(u16*)((u8*)p + 0x84) = *(u16*)((u8*)p + 0x84) & ~0x10;
    *(s16*)((u8*)p + 0x60) = 0;
    *(s16*)((u8*)p + 0x64) = 0;
}

/// The largest function in the overlay: one record's projectile worker -- the bit-5 worker
/// `func_ov011_02129110` dispatches, `p` is the Tatt data and `rec` one 0x88-stride record (raw
/// offsets: it fits no declared struct).
///
/// Before anything else it resolves the *target's* screen position into the two long-lived values:
/// `vx` is `func_ov003_020843b0` and `vy` `func_ov003_020843ec`, both `ROUND`-ed to 4.12, of the
/// chase object's `0x28`/`0x2C`/`0x30` position. `p + 0x24C` bit 1 picks the chase pointer:
/// `+0x3D89C` with mode 1 when set, `+0x3D898` with mode 0 when clear. The helpers are spelled
/// out in *both* arms of each round so the calls duplicate exactly as the original's do.
///
/// The record's `0x64` halfword then runs a four-state machine:
///   - 0: on the first of 0x3C frames, latch the target *relative* to the record's home
///     (`0x7C` = vx - `p+0x23C`, `0x80` = vy - `p+0x240` - 0x10000), measure the range into
///     `0x74` (`func_ov003_020cba2c`) and the heading into `0x70` (`func_ov003_020cba14`), and
///     aim the sprite through `func_ov011_02128f10`. Counts up to `0x62` and advances to 1.
///   - 1: on the first of 0xA frames set flag bit 2, snap the displacement `0x68`/`0x6C` to the
///     latched target and arm `0x72` = heading + 0x8000 (the reversed heading). Adds 0x2000 to
///     the `0x74` range each frame and advances to 2.
///   - 2: play SE 0x1E1 once (pan = the home x biased by the displacement, narrowed 4.12 ->
///     s16), burn the `0x74` range down by 0x8000 per frame (zero-clamp advances to 3), and emit
///     a five-particle burst every frame: `func_ov003_020cbc50` decomposes the reversed heading
///     once into the base position and once per particle along it, and `func_ov003_0208a1a4`
///     fires command record `func_ov003_0208a114(index)` at each narrowed position. The command
///     index is 0x5F or 0x58 on `p + 0x24C` bit 1 (the same mirror bit), so the two sides get
///     different particle records.
///   - 3: idle 0xA frames, then clear the record's flag bit 1 -- the "in use" bit
///     `func_ov011_02129994` set at spawn -- leaving the record dead for `func_ov011_02128f80`.
///     Falls out of the switch and shares the function epilogue with the out-of-range path.
///
/// Notes for a codegen pass: the burst loop's counter starts at -1 and its offset seed is spelled
/// `i - 0xF` in the original (`sub r5, r4, #0xf`), so the five particles sit at offsets
/// -16, 0, 16, 32, 48 stepped by 0x10 with the test `i < 4` at the bottom. The 4.12 -> pixel
/// narrowing is the `lsl #4 / asr #16` pair, i.e. bit-exactly `(s16)(x >> 12)`. `ROUND` is the
/// file-scope macro in `BtlEnm010.c`; `func_ov003_02087f00`, `func_ov003_020843b0`,
/// `func_ov003_020843ec`, `func_ov003_020cbc50`, `func_ov003_020cba2c` and
/// `func_ov011_02128f10` keep their `BtlEnm010.c` extern-block spellings.
void func_ov011_02129410(void* p, void* rec) {
    s32 vx;
    s32 vy;
    s32 idx;
    s32 bx;
    s32 by;
    s32 ox;
    s32 oy;
    s32 i;
    s32 off;

    if (((u32)(*(u8*)((u8*)p + 0x24C) << 30) >> 31) != 0) {
        vx = ROUND(func_ov003_020843b0(1, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x89C)) + 0x28)));
        vy = ROUND(func_ov003_020843ec(1, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x89C)) + 0x2C),
                                       *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x89C)) + 0x30)));
    } else {
        vx = ROUND(func_ov003_020843b0(0, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28)));
        vy = ROUND(func_ov003_020843ec(0, *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x2C),
                                       *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x30)));
    }
    switch (*(s16*)((u8*)rec + 0x64)) {
        case 0:
            if (*(s16*)((u8*)rec + 0x60) == 0) {
                *(s16*)((u8*)rec + 0x62) = 0x3C;
                *(s32*)((u8*)rec + 0x7C) = vx - *(s32*)((u8*)p + 0x23C);
                *(s32*)((u8*)rec + 0x80) = vy - *(s32*)((u8*)p + 0x240) - 0x10000;
                *(s32*)((u8*)rec + 0x74) = func_ov003_020cba2c(*(s32*)((u8*)p + 0x23C) + *(s32*)((u8*)rec + 0x68),
                                                               *(s32*)((u8*)p + 0x240) + *(s32*)((u8*)rec + 0x6C), vx, vy);
                *(u16*)((u8*)rec + 0x70) = func_ov003_020cba14(*(s32*)((u8*)rec + 0x68), *(s32*)((u8*)rec + 0x6C),
                                                               *(s32*)((u8*)rec + 0x7C), *(s32*)((u8*)rec + 0x80));
                func_ov011_02128f10(rec, *(u16*)((u8*)rec + 0x70));
            }
            if (*(s16*)((u8*)rec + 0x60) < *(s16*)((u8*)rec + 0x62)) {
                *(s16*)((u8*)rec + 0x60) = *(s16*)((u8*)rec + 0x60) + 1;
                return;
            }
            *(s16*)((u8*)rec + 0x60) = 0;
            *(s16*)((u8*)rec + 0x64) = 1;
            return;
        case 1:
            if (*(s16*)((u8*)rec + 0x60) == 0) {
                *(s16*)((u8*)rec + 0x62) = 0xA;
                *(u16*)((u8*)rec + 0x84) = *(u16*)((u8*)rec + 0x84) | 4;
                *(s32*)((u8*)rec + 0x68) = *(s32*)((u8*)rec + 0x7C);
                *(s32*)((u8*)rec + 0x6C) = *(s32*)((u8*)rec + 0x80);
                *(u16*)((u8*)rec + 0x72) = *(u16*)((u8*)rec + 0x70) + 0x8000;
            }
            if (*(s16*)((u8*)rec + 0x60) >= *(s16*)((u8*)rec + 0x62)) {
                *(s16*)((u8*)rec + 0x60) = 0;
                *(s16*)((u8*)rec + 0x64) = 2;
                return;
            }
            *(s32*)((u8*)rec + 0x74) = *(s32*)((u8*)rec + 0x74) + 0x2000;
            *(s16*)((u8*)rec + 0x60) = *(s16*)((u8*)rec + 0x60) + 1;
            return;
        case 2:
            if (*(s16*)((u8*)rec + 0x60) == 0) {
                *(s16*)((u8*)rec + 0x60) = *(s16*)((u8*)rec + 0x60) + 1;
                func_ov003_02087f00(0x1E1, (s16)((*(s32*)((u8*)p + 0x23C) + *(s32*)((u8*)rec + 0x68)) >> 12));
            }
            if (*(s32*)((u8*)rec + 0x74) > 0) {
                *(s32*)((u8*)rec + 0x74) = *(s32*)((u8*)rec + 0x74) - 0x8000;
                if (*(s32*)((u8*)rec + 0x74) < 0) {
                    *(s32*)((u8*)rec + 0x74) = 0;
                    *(s16*)((u8*)rec + 0x60) = 0;
                    *(s16*)((u8*)rec + 0x64) = 3;
                }
            }
            if (((u32)(*(u8*)((u8*)p + 0x24C) << 30) >> 31) != 0) {
                idx = 0x5F;
            } else {
                idx = 0x58;
            }
            func_ov003_020cbc50(&bx, &by, *(u16*)((u8*)rec + 0x72), *(s32*)((u8*)rec + 0x74));
            bx  = bx + *(s32*)((u8*)p + 0x23C) + *(s32*)((u8*)rec + 0x68);
            by  = by + *(s32*)((u8*)p + 0x240) + *(s32*)((u8*)rec + 0x6C);
            i   = -1;
            off = i - 0xF;
            do {
                func_ov003_020cbc50(&ox, &oy, *(u16*)((u8*)rec + 0x72), off * 0x1000);
                func_ov003_0208a1a4(func_ov003_0208a114(idx), (void*)((u8*)p + 0x244), (s16)((bx + ox) >> 12),
                                    (s16)((by + oy) >> 12));
                off = off + 0x10;
                i   = i + 1;
            } while (i < 4);
            return;
        case 3:
            if (*(s16*)((u8*)rec + 0x60) == 0) {
                *(s16*)((u8*)rec + 0x62) = 0xA;
            }
            if (*(s16*)((u8*)rec + 0x60) < *(s16*)((u8*)rec + 0x62)) {
                *(s16*)((u8*)rec + 0x60) = *(s16*)((u8*)rec + 0x60) + 1;
                return;
            }
            *(u16*)((u8*)rec + 0x84) = *(u16*)((u8*)rec + 0x84) & ~2;
            /* falls out of the switch: shares the epilogue with the out-of-range dispatch */
    }
}

/// `Tsk_BtlEnm010_Tatt`'s task entry. Fifth and last of the identical four-way dispatchers
/// (`0x0212801c`, `0x02128348`, `0x02128758`, and this one).
s32 func_ov011_02129934(void* arg0, void* arg1, s32 arg2, s32 index) {
    void* p;
    s32   r;

    p = *(void**)((u8*)arg1 + 0x18);
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_02129994(p, (void*)arg2);
            break;
        case 1:
            r = func_ov011_02129b84((BtlEnm010Tatt*)p);
            break;
        case 2:
            r = func_ov011_02129cec((BtlEnm010Tatt*)p);
            break;
        case 3:
            r = func_ov011_02129ea4(p);
            break;
    }
    return r;
}

/// Tatt's initialiser, and the second independent confirmation that Tatt is 0x250 -- the
/// `MI_CpuSet` clears exactly that.
///
/// The owner is **not** cached in a local: the original reloads `arg1[0]` before nearly every
/// use, so `*(u32*)arg1` is spelled inline throughout. The two `0x24C` updates are if/else pairs
/// that each do a `bic` *and* an `orr` on one side -- the conditional-bit idioms again, and
/// folding either into a single mask or single set is what costs the instructions.
s32 func_ov011_02129994(BtlEnm010Tatt* data, void* arg1) {
    s32   r;
    s32   i;
    u16*  p;
    void* q;

    MI_CpuSet(data, 0, 0x250);
    r                          = (func_ov003_020c37f8((void*)((u8*)*(u32*)arg1 + 0x84)) != 0) ? 1 : 0;
    *(u32*)((u8*)data + 0x00)  = *(u32*)arg1;
    *(s32*)((u8*)data + 0x230) = *(s32*)((u8*)*(u32*)arg1 + 0x28);
    *(s32*)((u8*)data + 0x234) = *(s32*)((u8*)*(u32*)arg1 + 0x2C);
    *(s32*)((u8*)data + 0x238) = *(s32*)((u8*)*(u32*)arg1 + 0x30);
    *(s32*)((u8*)data + 0x23C) = (s32)(func_ov003_020843b0(r, *(s32*)((u8*)*(u32*)arg1 + 0x28)) > 0
                                           ? 0.5f + (f32)(func_ov003_020843b0(r, *(s32*)((u8*)*(u32*)arg1 + 0x28)) << 12)
                                           : (f32)(func_ov003_020843b0(r, *(s32*)((u8*)*(u32*)arg1 + 0x28)) << 12) - 0.5f);
    *(s32*)((u8*)data + 0x240) =
        (s32)(func_ov003_020843ec(r, *(s32*)((u8*)*(u32*)arg1 + 0x2C), *(s32*)((u8*)*(u32*)arg1 + 0x30)) > 0
                  ? 0.5f +
                        (f32)(func_ov003_020843ec(r, *(s32*)((u8*)*(u32*)arg1 + 0x2C), *(s32*)((u8*)*(u32*)arg1 + 0x30)) << 12)
                  : (f32)(func_ov003_020843ec(r, *(s32*)((u8*)*(u32*)arg1 + 0x2C), *(s32*)((u8*)*(u32*)arg1 + 0x30)) << 12) -
                        0.5f);
    q = (void*)((u8*)data + 4);
    p = (u16*)((u8*)data + 0x88);
    for (i = 1; i < 5; i++) {
        func_ov011_021258b4(((*(u32*)((u8*)*(u32*)arg1 + 0x84) << 30) >> 30), (CombatSprite*)q, i);
        p[0] = p[0] | 2;
        q    = (void*)((u8*)q + 0x88);
        p    = (u16*)((u8*)p + 0x88);
    }
    func_ov011_02128ca4(data, (s32)func_ov011_02128cc0);
    if (*(s32*)((u8*)*(u32*)arg1 + 0x24) == 0) {
        *(u8*)((u8*)data + 0x24C) = *(u8*)((u8*)data + 0x24C) & 0xFE;
    } else {
        *(u8*)((u8*)data + 0x24C) = (u8)((*(u8*)((u8*)data + 0x24C) & 0xFE) | 1);
    }
    if (func_ov003_020c37f8((void*)((u8*)*(u32*)arg1 + 0x84)) != 0) {
        *(u8*)((u8*)data + 0x24C) = *(u8*)((u8*)data + 0x24C) | 2;
    } else {
        *(u8*)((u8*)data + 0x24C) = *(u8*)((u8*)data + 0x24C) & 0xFD;
    }
    *(u16*)((u8*)data + 0x244) = *(u16*)((u8*)*(u32*)arg1 + 0x04);
    *(u16*)((u8*)data + 0x246) = *(u16*)((u8*)*(u32*)arg1 + 0x06);
    *(u16*)((u8*)data + 0x248) = *(u16*)((u8*)*(u32*)arg1 + 0x08);
    *(u16*)((u8*)data + 0x24A) = *(u16*)((u8*)*(u32*)arg1 + 0x0A);
    return 1;
}

/// Tatt's per-frame worker, phase 1. Two half-unit steps toward the target, one on each axis,
/// then a callback at `0x224` if one is installed, then a pass over the four sub-records that
/// returns 1 the moment one of them takes a hit. The `0.5f` nudge is a *conditional* add or
/// subtract, and both branches re-call the helper -- the comparison result is discarded and the
/// call is repeated rather than kept. The step is `x << 12` converted to float, so the scale is
/// in the C and not in a call.
s32 func_ov011_02129b84(BtlEnm010Tatt* data) {
    s32  r;
    s32  dir;
    s32  i;
    s16* p;
    s16* q;

    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    if (*(void**)((u8*)data + 0x00) != NULL) {
        if (*(s32*)((u8*)*(void**)((u8*)data + 0x00) + 0x54) & 4) {
            return r;
        }
    }
    dir                        = (s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31);
    *(s32*)((u8*)data + 0x23C) = (s32)(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) > 0
                                           ? 0.5f + (f32)(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) << 12)
                                           : (f32)(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) << 12) - 0.5f);
    *(s32*)((u8*)data + 0x240) =
        (s32)(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) > 0
                  ? 0.5f + (f32)(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) << 12)
                  : (f32)(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) << 12) - 0.5f);
    if (*(void**)((u8*)data + 0x224) != NULL) {
        (*(void (**)(void*))((u8*)data + 0x224))(data);
    }
    func_ov011_02129110(data);
    p = (s16*)((u8*)data + 0x88);
    q = (s16*)((u8*)data + 4);
    for (i = 0; i < 4; i++) {
        if (((u32)(*p << 30) >> 31) == 1) {
            CombatSprite_Update((CombatSprite*)q);
            r = 1;
        }
        p = (s16*)((u8*)p + 0x88);
        q = (s16*)((u8*)q + 0x88);
    }
    return r;
}

/// Tatt's per-frame worker, phase 2. Accumulates four sub-records into a pair of running totals
/// held in the outgoing-argument area rather than in locals -- `0x020cbc50` is handed the two
/// addresses and writes through them, which is why the frame is 0xC and the totals are not
/// declared.
///
/// Bit 0 of each record's `0x84` selects whether to run the two-pointer update at all, and bit 1
/// selects between the pointer update and adding the record's own `0x68`/`0x6C` directly. The
/// final position is `<< 4 >> 16`, a 16-bit fixed-point conversion that has to be spelled that
/// way to get the `asr` pair rather than a shift.
s32 func_ov011_02129cec(BtlEnm010Tatt* data) {
    s32   dir;
    s32   vx;
    s32   vy;
    s32   i;
    s32   a;
    s32   b;
    void* p;

    dir = (s32)(((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31) != 0) ? 1 : 0;
    vx  = (s32)(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) > 0
                    ? 0.5f + (f32)(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) << 12)
                    : (f32)(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) << 12) - 0.5f);
    vy  = (s32)(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) > 0
                    ? 0.5f + (f32)(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) << 12)
                    : (f32)(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) << 12) - 0.5f);
    a   = 0;
    b   = 0;
    p   = (void*)((u8*)data + 4);
    for (i = 0; i < 4; i++) {
        if (((u32)(*(u16*)((u8*)p + 0x84) << 30) >> 31) != 0) {
            if (((u32)(*(u16*)((u8*)p + 0x84) << 29) >> 31) != 0) {
                func_ov003_020cbc50(&a, &b, *(s16*)((u8*)p + 0x72), *(s16*)((u8*)p + 0x74));
                a = vx + *(s32*)((u8*)p + 0x68) + a;
                b = vy + *(s32*)((u8*)p + 0x6C) + b;
            } else {
                a = vx + *(s32*)((u8*)p + 0x68);
                b = vy + *(s32*)((u8*)p + 0x6C);
            }
            CombatSprite_SetPosition((CombatSprite*)p, (b * 16) >> 16, (a * 16) >> 16);
            CombatSprite_SetAffineTransform(p, i, i, *(s32*)((u8*)p + 0x78), i);
            CombatSprite_Render((CombatSprite*)p);
        }
        p = (void*)((u8*)p + 0x88);
    }
    return 1;
}

/// Four sprite ticks at a 0x88 stride from `+0x04`. `i` is declared before the walking pointer
/// because the original puts the counter in r4 and the pointer in r5.
s32 func_ov011_02129ea4(void* p) {
    s32   i;
    void* sp;

    sp = (void*)((u8*)p + 4);
    for (i = 0; i < 4; i++) {
        CombatSprite_Release(sp);
        sp = (void*)((u8*)sp + 0x88);
    }
    return 1;
}

/// Tatt's phase-0 setup. The RG pair again -- `func_ov003_020c427c` thunk, then the callback at
/// `0x1C8`, then the `0x1C4`-before-`0x1C0` clear pair. Third confirmation of that ordering.
void func_ov011_02129ed0(BtlEnm010Tatt* data, void (*func)(BtlEnm010Tatt*)) {
    func_ov003_020c427c(data, (void*)func);
    data->unk_1C8 = func;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

/// Tatt's mode picker. Draws from a per-slot threshold table, rolls a d100, and installs one of
/// four phases. The switch has no `default`, so the phase pointer is initialised to null at the
/// top of the block and the out-of-range case falls through to the call with it still null.
///
/// The d100 is the *second* argument and 4 the third -- `020cb888`'s argument order is not the
/// obvious one, and getting it wrong swaps r1 and r2 in the emitted code.
void func_ov011_02129ef8(BtlEnm010Tatt* data) {
    void (*f)(BtlEnm010Tatt*);

    f = NULL;
    switch (func_ov003_020cb888(data_ov011_0212cc7c[*(u16*)((u8*)data + 0x80)], 4, RNG_Next(0x64))) {
        case 0:
            f = func_ov011_0212a134;
            break;
        case 1:
            f = func_ov011_0212a2ec;
            break;
        case 2:
            f = func_ov011_0212a420;
            break;
        case 3:
            f = func_ov011_0212a634;
            break;
    }
    func_ov011_02129ed0(data, f);
}

/// Tatt's pattern picker, two switches deep. The first reads a per-slot mode out of `0x80` and
/// produces an *index* 0-7, not a pointer; the second maps that index to a phase. `0x1F6` is
/// advanced between the two, so the first switch must not have it as a side effect.
///
/// Cases 0 and 1 are a `umull` magic-divide chain with no sign fixup, so they are unsigned
/// `% 5` and `% 6` on a sign-extended `s16` -- the `u32` cast is what selects that form. Case 3's
/// row stride is eight bytes, which is why the address is built as
/// `tbl + ((val << 30) >> 27) + roll * 4` rather than as a subscript: that is what produces the
/// `add r1, r1, r2, lsr #27` addressing mode.
void func_ov011_02129f80(BtlEnm010Tatt* data) {
    void (*f)(BtlEnm010Tatt*);
    s32 v;

    v = 0;
    f = NULL;
    switch (*(u16*)((u8*)data + 0x80)) {
        case 0:
            v = data_ov011_0212c37c[(u32)(*(s16*)((u8*)data + 0x1F6)) % 5];
            break;
        case 1:
            v = data_ov011_0212c390[(u32)(*(s16*)((u8*)data + 0x1F6)) % 6];
            break;
        case 2:
            if (func_ov011_0212bdbc(data, 0) != 0) {
                v = 5;
            } else {
                v = data_ov011_0212c330[RNG_Next(3)];
            }
            break;
        case 3:
            v = ((s32*)((u8*)data_ov011_0212c3a8 + (((((u32)(*(s16*)((u8*)data + 0x1F6))) << 30) >> 27))))[(u32)RNG_Next(2)];
            break;
    }
    *(s16*)((u8*)data + 0x1F6) = *(s16*)((u8*)data + 0x1F6) + 1;
    switch (v) {
        case 0:
            f = func_ov011_0212a674;
            break;
        case 1:
            f = func_ov011_0212a78c;
            break;
        case 2:
            f = func_ov011_0212aa20;
            break;
        case 3:
            f = func_ov011_0212ac0c;
            break;
        case 4:
            f = func_ov011_0212aee8;
            break;
        case 5:
            f = func_ov011_0212af94;
            break;
        case 6:
            f = func_ov011_0212b0a4;
            break;
        case 7:
            f = func_ov011_0212b168;
            break;
    }
    func_ov011_02129ed0(data, f);
}

/// Tatt's task entry proper. Bails on whatever `func_ov003_020c5bfc` reports, otherwise installs
/// phase 0 -- the same two-line shape as RG's `func_ov011_021267f4`.
void func_ov011_0212a10c(BtlEnm010Tatt* data) {
    if (func_ov003_020c5bfc() != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// Tatt's phase driver, and the only function here with a three-arm switch that falls through to
/// a shared tail that *decrements* `0x1C2`. Arm 2's "we have passed the target three times" exit
/// is the one path that skips the decrement, so it has to be an early `return` rather than a
/// `break`. The sign test on `0x24` is the `||` of two `&&`s again, but the comparison runs the
/// other way round from `func_ov011_0212a420` -- here it is `0x28` against the chase pointer.
void func_ov011_0212a134(BtlEnm010Tatt* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            data->unk_1C2 = func_ov003_020c42ec(data);
            data->unk_1FC = 0;
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            break;
        case 1:
            if (func_ov011_0212b800(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 2;
                break;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            break;
        case 2:
            v = *(s32*)((u8*)data + 0x24);
            if ((v == 0 &&
                 *(s32*)((u8*)data + 0x28) < *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28)) ||
                (v == 1 &&
                 *(s32*)((u8*)data + 0x28) > *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28)))
            {
                if (data->unk_1FC == 3) {
                    func_ov011_02129ed0(data, func_ov011_0212aa20);
                    data->unk_1FC = 0;
                    return;
                }
                data->unk_1FC = data->unk_1FC + 1;
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 0);
                CombatSprite_Restart((CombatSprite*)((u8*)data + 0x84));
            }
            if (data->unk_1C2 > 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
            } else if (func_ov003_020c4348(data) != 0) {
                func_ov011_02129f80(data);
            } else {
                func_ov011_02129ef8(data);
            }
            break;
    }
    data->unk_1C2 = data->unk_1C2 - 1;
}

/// Tatt's drifting phase. `func_ov011_0212a420` with a second random `0x1000` step from
/// `func_ov003_020cb7a4` in the other axis, and `func_ov011_0212b5d8` instead of
/// `func_ov011_0212b6e0` in the second arm.
void func_ov011_0212a2ec(BtlEnm010Tatt* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                *(s32*)((u8*)data + 0x1DC) = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                *(s32*)((u8*)data + 0x1E0) = RNG_Next((func_ov003_020cb7a4(0) >> 12) + 1) << 12;
                *(s32*)((u8*)data + 0x1E4) = 0;
                v                          = *(s32*)((u8*)data + 0x24);
                if ((v == 0 && *(s32*)((u8*)data + 0x1DC) < *(s32*)((u8*)data + 0x28)) ||
                    (v == 1 && *(s32*)((u8*)data + 0x1DC) >= *(s32*)((u8*)data + 0x28)))
                {
                    data->unk_1C0 = 0;
                    data->unk_1C4 = 1;
                    return;
                }
            }
            if (func_ov011_0212b800(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0) {
                func_ov011_02129ed0(data, func_ov011_0212a134);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

/// Tatt's homing phase. Case 0 picks a random `0x1000` step, then advances to phase 1 either
/// when the step has carried it past `0x28` going one way or failed to carry it past going the
/// other -- a two-arm test on the sign of `0x24`, written as one `||` of two `&&`s so that the
/// second arm's `cmp` is predicated rather than branched. Case 1 polls the coordinate helper.
void func_ov011_0212a420(BtlEnm010Tatt* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                *(s32*)((u8*)data + 0x1DC) = RNG_Next((func_ov003_020cb744(0) >> 12) + 1) << 12;
                *(s32*)((u8*)data + 0x1E0) = *(s32*)((u8*)data + 0x2C);
                *(s32*)((u8*)data + 0x1E4) = 0;
                v                          = *(s32*)((u8*)data + 0x24);
                if ((v == 0 && *(s32*)((u8*)data + 0x1DC) < *(s32*)((u8*)data + 0x28)) ||
                    (v == 1 && *(s32*)((u8*)data + 0x1DC) >= *(s32*)((u8*)data + 0x28)))
                {
                    data->unk_1C0 = 0;
                    data->unk_1C4 = 1;
                    return;
                }
            }
            if (func_ov011_0212b800(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (func_ov011_0212b6e0(data, data->unk_1C0) == 0) {
                func_ov011_02129ed0(data, func_ov011_0212a134);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

/// Tatt's long-range phase. The opening is the `0x1C0` prime; the interesting part is the ternary
/// that seeds `0x1D0` with `(-0x40000) + 0x38000` or `0x8000` depending on the sign of `0x24`,
/// which is the same conditional-negation shape as the `0x80000` bias in `func_ov011_0212b0a4`.
void func_ov011_0212a540(BtlEnm010Tatt* data) {
    s32 v;

    if (data->unk_1C0 == 0) {
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 6);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
        func_ov003_020c4b5c(data);
        v                         = 0 - 0x40000;
        *(s32*)((u8*)data + 0x30) = v;
        data->unk_1D0             = (*(s32*)((u8*)data + 0x24) == 0) ? v + 0x38000 : 0x8000;
        data->unk_1C0             = data->unk_1C0 + 1;
        func_ov003_02087f00(0x1E3, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
    }
    if (*(s16*)((u8*)data + 0x9A) == 3 && *(s16*)((u8*)data + 0x8C) == 1) {
        data->unk_1D8 = 0x2800;
    }
    if (*(s32*)((u8*)data + 0x30) < 0) {
        return;
    }
    data->unk_1D8 = 0;
    data->unk_1D0 = 0;
    data->unk_54  = data->unk_54 & ~0x10000000;
    func_ov003_020cb520(data, 1);
    func_ov003_020cb594(data, 1);
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// One of Tatt's phases: poll a predicate on the `0x1C0` counter. The counter is bumped
/// **unconditionally** every frame -- the reference loads it fresh after the call, increments and
/// stores *before* the `popne` -- and only the hand-over to `func_ov011_02129ed0` is skipped when
/// the predicate is still active.
void func_ov011_0212a634(BtlEnm010Tatt* data) {
    s32 r;

    r             = func_ov011_0212b800(data, data->unk_1C0);
    data->unk_1C0 = data->unk_1C0 + 1;
    if (r != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// Tatt's aimed phase. Case 0 seeds the velocity from `0x28` and from a two-hop chase pointer
/// (`data_ov003_020e71b8 + 0x3D000 + 0x898` then `+0x2C`), and the exit test is a short-circuiting
/// `||` of "the helper gave up" against "we have been at this for 60 frames". Case 1 is the
/// `0x1F8`/`0x1FA` latch that `func_ov011_0212aee8` also has, with `0xBD` in place of `0x1E`.
///
/// The `0x1DC`/`0x1E4` stores both go through one `s32` local: the original loads `0x28` into
/// r3, stores it, then overwrites r3 with 0 and stores that, and a local is what forces the
/// reuse.
void func_ov011_0212a674(BtlEnm010Tatt* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            v                          = *(s32*)((u8*)data + 0x28);
            *(s32*)((u8*)data + 0x1DC) = v;
            *(s32*)((u8*)data + 0x1E0) = *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x2C);
            v                          = 0;
            *(s32*)((u8*)data + 0x1E4) = v;
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0 || data->unk_1C0 >= 0x3C) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x1E;
                data->unk_1FA = 0xBD;
            }
            {
                s32 r;
                s32 old;

                old           = data->unk_1C0;
                data->unk_1C0 = old + 1;
                r             = func_ov011_0212b890(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), old);
                if (data->unk_1F8 == 0 && data->unk_1FA == 0xBD) {
                    data->unk_208 = func_ov011_02127c84(data);
                }
                if (r != 0) {
                    return;
                }
                func_ov011_02129ed0(data, func_ov011_0212a134);
            }
            return;
    }
}

/// Tatt's burst phase (dispatch value 1 in `func_ov011_02129f80`'s picker).
///
/// Case 0 is the shared seed-and-poll arm, byte-for-byte the shape `func_ov011_0212a674`'s case 0
/// has: copy `0x28` into `0x1DC`, the chase pointer's `0x2C` into `0x1E0`, zero `0x1E4` through one
/// reused `s32` local (the original re-uses r3 for the `0x28` value and then for the 0), then run
/// the `func_ov011_0212b5d8` / 60-frame poll and latch `0x1C0 = 0, 0x1C4 = 1` when it gives up or
/// times out.
///
/// Case 1 is the attack proper:
///   - frame 0 latches `0x1F8 = 0x14` and `0x1FA = (s16)(data_ov011_0212c34c[slot] * 10 + 0x14)`
///     -- the `lsl #16 / asr #16` narrowing is live: the guard below subtracts from the narrowed
///     value;
///   - the `0x1F8`/`0x1FA` pair goes through `func_ov011_0212b890` with the *pre-increment* `0x1C0`
///     (the RMW store is emitted before the `bl`, `func_ov011_0212aee8`'s idiom);
///   - while `0x1F8` has run down to 0 and `0x1FA` sits on a ten-frame boundary, and fewer than
///     `data_ov011_0212c34c[slot]` ten-frame ticks have elapsed, spawn a `Tsk_BtlEnm010_Rnge` burst
///     through `func_ov011_02127f6c`: data pointer, the `+/- 0x40000` mirror bias (r1), 0 (r2),
///     `0 - 0x10000` (r3), then the `[slot][shot]` parameter record as the five stack words -- s32,
///     the `0x8000`/0 phase word (u16), u16, s32, s16 -- and the handle lands in `0x208`.  The
///     shot index is `q`, the quotient the guard computed; the table row is re-derived from the
///     `u16` at `0x80` exactly once for all four field loads;
///   - the shared velocity-decay tail: while `0x1D0` is positive `0x1E8` accumulates `-0x333` per
///     frame, while negative `+0x333`, and once `0x1D0 + 0x1E8` crosses zero both are cleared
///     (`0x333` comes out as `sub #0x33 / sub #0x300`, i.e. it is spelled as one constant);
///   - finally, if the `func_ov011_0212b890` result is non-zero the phase stays armed; otherwise
///     the pair is zeroed again and `func_ov011_02129ed0(data, func_ov011_0212a134)` re-arms the
///     phase driver.  (No dead `mov r0, #imm` sits at any return: the reference epilogues never
///     set r0, so the `void` signature is exact.)
void func_ov011_0212a78c(BtlEnm010Tatt* data) {
    s32 v;
    s16 limit;
    s32 r;

    switch (data->unk_1C4) {
        case 0:
            v                          = *(s32*)((u8*)data + 0x28);
            *(s32*)((u8*)data + 0x1DC) = v;
            *(s32*)((u8*)data + 0x1E0) = *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x2C);
            v                          = 0;
            *(s32*)((u8*)data + 0x1E4) = v;
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0 || data->unk_1C0 >= 0x3C) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            limit = data_ov011_0212c34c[*(u16*)((u8*)data + 0x80)] * 10 + 0x14;
            if (data->unk_1C0 == 0) {
                data->unk_1F8 = 0x14;
                data->unk_1FA = limit;
            }
            r             = func_ov011_0212b890(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), data->unk_1C0);
            data->unk_1C0 = data->unk_1C0 + 1;
            if (data->unk_1F8 == 0 && data->unk_1FA % 10 == 0) {
                s32 q = (limit - data->unk_1FA) / 10;

                if (q < data_ov011_0212c34c[*(u16*)((u8*)data + 0x80)]) {
                    s32 bias;
                    u16 a5;

                    if (*(s32*)((u8*)data + 0x24) != 0) {
                        *(s32*)((u8*)data + 0x1D0) = 0 - 0x4000;
                        bias                       = 0x40000;
                        a5                         = 0;
                    } else {
                        *(s32*)((u8*)data + 0x1D0) = 0x4000;
                        bias                       = 0 - 0x40000;
                        a5                         = 0x8000;
                    }
                    *(s32*)((u8*)data + 0x1E8) = 0;
                    {
                        s32 slot = *(u16*)((u8*)data + 0x80);

                        data->unk_208 = func_ov011_02127f6c(data, bias, 0, 0 - 0x10000,
                                                            *(s32*)((u8*)data_ov011_0212c3c8 + slot * 0x20 + q * 0x10), a5,
                                                            *(u16*)((u8*)data_ov011_0212c3cc + slot * 0x20 + q * 0x10),
                                                            *(s32*)((u8*)data_ov011_0212c3d0 + slot * 0x20 + q * 0x10),
                                                            *(s16*)((u8*)data_ov011_0212c3d4 + slot * 0x20 + q * 0x10));
                    }
                }
            }
            if (data->unk_1D0 > 0) {
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) - 0x33;
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) - 0x300;
                if (data->unk_1D0 + *(s32*)((u8*)data + 0x1E8) <= 0) {
                    data->unk_1D0              = 0;
                    *(s32*)((u8*)data + 0x1E8) = 0;
                }
            } else if (data->unk_1D0 < 0) {
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) + 0x33;
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) + 0x300;
                if (data->unk_1D0 + *(s32*)((u8*)data + 0x1E8) >= 0) {
                    data->unk_1D0              = 0;
                    *(s32*)((u8*)data + 0x1E8) = 0;
                }
            }
            if (r != 0) {
                return;
            }
            data->unk_1D0              = 0;
            *(s32*)((u8*)data + 0x1E8) = 0;
            func_ov011_02129ed0(data, func_ov011_0212a134);
            return;
    }
}

/// Tatt's converging phase. Two things the reference does that the C has to be shaped around.
/// First, there is a block *before* the switch guarded on `0x1C4 == 0 && 0x1C0 == 0` in a single
/// short-circuiting `&&`, which is why the second test's `ldrsh` is predicated on the first.
/// Second, the `0x1DC` seed is a conditional offset of the chase pointer by `0x60000` in one
/// direction or the other -- `sublt`/`addge` off one compare, not an if around a subtraction.
void func_ov011_0212aa20(BtlEnm010Tatt* data) {
    s32 bias;

    bias = (*(s32*)((u8*)data + 0x24) == 0) ? 0 - 0x60000 : 0x60000;
    if (data->unk_1C4 == 0 && data->unk_1C0 == 0) {
        *(s16*)((u8*)data + 0x1C4) =
            (func_ov003_020cba2c(*(s32*)((u8*)data + 0x28), *(s32*)((u8*)data + 0x2C),
                                 *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28),
                                 *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x2C)) < 0x80000)
                ? 1
                : 0;
        data->unk_1C0 = 0;
    }
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                *(s32*)((u8*)data + 0x1DC) =
                    (*(s32*)((u8*)data + 0x28) < *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28))
                        ? *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28) - 0x60000
                        : *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28) + 0x60000;
                *(s32*)((u8*)data + 0x1E0) = *(s32*)((u8*)data + 0x2C);
                *(s32*)((u8*)data + 0x1E4) = 0;
            }
            if (func_ov011_0212b6e0(data, data->unk_1C0) == 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 3);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 1);
                func_ov003_02087f00(0x1D7, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
            }
            if (*(s16*)((u8*)data + 0x9A) == 6 && *(s16*)((u8*)data + 0x8C) == 1) {
                func_ov003_02087f00(0x1D8, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
            }
            if (*(s16*)((u8*)data + 0x9A) >= 5 && *(s16*)((u8*)data + 0x9A) <= 7) {
                func_ov003_020c5b2c(0x55, (s32)(u32)data, *(s32*)((u8*)data + 0x28) + bias, *(s32*)((u8*)data + 0x2C),
                                    *(s32*)((u8*)data + 0x30));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            func_ov011_02129ed0(data, func_ov011_0212a134);
            return;
    }
}

/// Tatt's strafe phase (dispatch value 3 in `func_ov011_02129f80`'s picker).
///
/// Case 0 is the shared seed-and-poll arm, the same code `func_ov011_0212a78c` and
/// `func_ov011_0212a674` have: `0x1DC`/`0x1E0`/`0x1E4` seeded from `0x28`, the chase pointer's
/// `0x2C` and a reused zero, then the `func_ov011_0212b5d8` / 60-frame poll latching
/// `0x1C0 = 0, 0x1C4 = 1`.
///
/// Case 1 primes the attack: on frame 0 bump `0x1C0` first, then kick the sprite with
/// `func_ov011_02125750(0, data + 0x84, 9)` and `Mini108_VBlank(..., 0, 1)`.  Once
/// `SpriteMgr_IsAnimationFinished` reports done, latch `0x1C0 = 0, 0x1C4 = 2`.
///
/// Case 2 runs the strafe and shares its tail with `func_ov011_0212a78c`:
///   - frame 0: `Mini108_VBlank(data + 0x84, 2, 0)`, spawn the `Tsk_BtlEnm010_Lser` burst via
///     `func_ov011_02125b98(data, 0)` into `0x208`, and arm the `0x1C2` countdown at `0x4B`;
///   - from frame `0x28`, every 15 frames (a *signed* `% 15` / `/ 15` `smull` pair on
///     `0x1C0 - 0x28`), for shot indices `1 .. count-1`, spawn again with the shot index `q`;
///   - from frame `0x3C`, every 15 frames: while `q < data_ov011_0212c33c[slot]` kick `0x1D0` to
///     `+/- 0x4000` by the sign of `0x24` (a ternary -- the reference is `mov r1, #0x4000` then a
///     flag-predicated `rsbne`) and clear `0x1E8`; when `q == count - 1` (the table re-read!)
///     swap the sprite with `Mini108_VBlank(data + 0x84, 3, 1)`;
///   - the shared `0x1D0`/`0x1E8` decay tail (`+=/-= 0x333` toward zero, both cleared on
///     crossing);
///   - when the animation finishes, `func_ov011_02129ed0(data, func_ov011_0212a134)`; otherwise
///     `0x1C0` advances one frame.  (No dead `mov r0, #imm` at any return: the reference
///     epilogues never set r0, so the `void` signature is exact.)
void func_ov011_0212ac0c(BtlEnm010Tatt* data) {
    s32 v;

    switch (data->unk_1C4) {
        case 0:
            v                          = *(s32*)((u8*)data + 0x28);
            *(s32*)((u8*)data + 0x1DC) = v;
            *(s32*)((u8*)data + 0x1E0) = *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x2C);
            v                          = 0;
            *(s32*)((u8*)data + 0x1E4) = v;
            if (func_ov011_0212b5d8(data, data->unk_1C0) == 0 || data->unk_1C0 >= 0x3C) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 9);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 2;
            return;
        case 2:
            if (data->unk_1C0 == 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 2, 0);
                data->unk_208 = func_ov011_02125b98(data, 0);
                data->unk_1C2 = 0x4B;
            }
            if (data->unk_1C0 >= 0x28 && (data->unk_1C0 - 0x28) % 15 == 0) {
                s32 q = (data->unk_1C0 - 0x28) / 15;

                if (q > 0 && q < data_ov011_0212c33c[*(u16*)((u8*)data + 0x80)]) {
                    data->unk_208 = func_ov011_02125b98(data, q);
                }
            }
            if (data->unk_1C0 >= 0x3C && (data->unk_1C0 - 0x3C) % 15 == 0) {
                s32 q = (data->unk_1C0 - 0x3C) / 15;

                if (q < data_ov011_0212c33c[*(u16*)((u8*)data + 0x80)]) {
                    data->unk_1D0              = (*(s32*)((u8*)data + 0x24) != 0) ? 0 - 0x4000 : 0x4000;
                    *(s32*)((u8*)data + 0x1E8) = 0;
                }
                if (q == data_ov011_0212c33c[*(u16*)((u8*)data + 0x80)] - 1) {
                    Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 3, 1);
                }
            }
            if (data->unk_1D0 > 0) {
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) - 0x33;
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) - 0x300;
                if (data->unk_1D0 + *(s32*)((u8*)data + 0x1E8) <= 0) {
                    data->unk_1D0              = 0;
                    *(s32*)((u8*)data + 0x1E8) = 0;
                }
            } else if (data->unk_1D0 < 0) {
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) + 0x33;
                *(s32*)((u8*)data + 0x1E8) = *(s32*)((u8*)data + 0x1E8) + 0x300;
                if (data->unk_1D0 + *(s32*)((u8*)data + 0x1E8) >= 0) {
                    data->unk_1D0              = 0;
                    *(s32*)((u8*)data + 0x1E8) = 0;
                }
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                func_ov011_02129ed0(data, func_ov011_0212a134);
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
    }
}

/// A Tatt phase: latch the `0x1F8`/`0x1FA` pair on the first frame, hand both to
/// `func_ov011_0212b890` along with the `0x1C0` counter, and when they come back as the sentinel
/// pair stash a handle in `0x208` and play a sound. `0x1C0` is a read-modify-write, so the
/// counter passed to the callee is the *pre-increment* value.
void func_ov011_0212aee8(BtlEnm010Tatt* data) {
    s32 r;

    if (data->unk_1C0 == 0) {
        data->unk_1F8 = 0x14;
        data->unk_1FA = 0x1E;
    }
    r             = func_ov011_0212b890(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), data->unk_1C0);
    data->unk_1C0 = data->unk_1C0 + 1;
    if (data->unk_1F8 == 0 && data->unk_1FA == 0x1E) {
        data->unk_208 = func_ov011_02128718(data);
        func_ov003_02087f00(0x1DE, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
    }
    if (r != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// Tatt's lobbed phase. Case 0 is `func_ov011_02127240`'s arm 1 with Tatt's spawn slot (`0x208`
/// rather than `0x1FC`) and no third arm; case 1 skips the palette prime, advances the phase, and
/// then scatters the `0x1F4` counters.
void func_ov011_0212af94(BtlEnm010Tatt* data) {
    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 8);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
                func_ov003_02087f00(0x1E0, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
            }
            if (data->unk_1C0 == 0x1C) {
                data->unk_208 = func_ov011_02128c44(data);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                data->unk_1C0 = 0;
                data->unk_1C4 = 1;
                return;
            }
            data->unk_1C0 = data->unk_1C0 + 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                data->unk_1C0 = data->unk_1C0 + 1;
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 1);
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            func_ov011_02129ed0(data, func_ov011_0212a134);
            func_ov011_0212bd3c(data, 0);
            return;
    }
}

/// Another Tatt phase. The `0x80000` bias is a *conditional negation* -- `rsbeq` against the test
/// of `0x24` -- so it has to be a ternary, not an `if` around the call. The frame filter on
/// `0x9A` is `>= 3 && <= 4`, which is what the `blt`/`bgt` pair around one block encodes.
void func_ov011_0212b0a4(BtlEnm010Tatt* data) {
    s32 bias;

    bias = (*(s32*)((u8*)data + 0x24) == 0) ? 0 - 0x80000 : 0x80000;
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 4);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
        func_ov003_02087f00(0x1DF, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
    }
    if (*(s16*)((u8*)data + 0x9A) >= 3 && *(s16*)((u8*)data + 0x9A) <= 4) {
        func_ov003_020c5b2c(0x59, (s32)(u32)data, *(s32*)((u8*)data + 0x28) + bias, *(s32*)((u8*)data + 0x2C),
                            *(s32*)((u8*)data + 0x30));
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// Tatt's ballistic phase, and the only function in the overlay with a hand-placed 0xC frame for
/// outgoing arguments. `func_ov003_02084348` takes six arguments and two of them are written back
/// through `s16*` locals at `sp+0x8` and `sp+0xA` -- note the second is at an *odd* offset, which
/// is why the frame has to be at least 0xC.
void func_ov011_0212b168(BtlEnm010Tatt* data) {
    s16 a;
    s16 b;

    switch (data->unk_1C4) {
        case 0:
            if (data->unk_1C0 == 0) {
                func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 5);
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
                func_ov003_020cb520(data, 0);
                func_ov003_020cb594(data, 0);
                data->unk_54  = data->unk_54 | 0x10000000;
                data->unk_1C0 = data->unk_1C0 + 1;
                func_ov003_02087f00(0x1E2, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) == 0) {
                return;
            }
            data->unk_1C0 = 0;
            data->unk_1C4 = 1;
            return;
        case 1:
            if (data->unk_1C0 == 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 0);
                data->unk_1D0 = (*(s32*)((u8*)data + 0x24) == 0) ? 0 - 0x8000 : 0x8000;
                func_ov003_02087f00(0x1E3, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
                data->unk_1C0 = data->unk_1C0 + 1;
            }
            func_ov003_020c5b2c(0x5A, (s32)(u32)data, *(s32*)((u8*)data + 0x28), *(s32*)((u8*)data + 0x2C),
                                *(s32*)((u8*)data + 0x30));
            func_ov003_02084348(0, &a, &b, *(s32*)((u8*)data + 0x28), *(s32*)((u8*)data + 0x2C), *(s32*)((u8*)data + 0x30));
            if (func_ov003_020cc7c0(a, b, 0x100) != 0) {
                return;
            }
            data->unk_1D0 = 0;
            func_ov011_02129ed0(data, func_ov011_0212a540);
            return;
    }
}

/// One of Tatt's phases. The first frame primes the `0x84` sprite, clears the three `0x1D*`
/// words and drops bit 28 of `0x54`; then it polls for the frame 4 animation to end and, on the
/// frame it does, installs `func_ov011_0212b388`.
///
/// The three `0x1D*` stores go out **descending** (`0x1D8`, `0x1D4`, `0x1D0`) -- the reverse-order
/// rule of brief section 15, reached here through three separate assignments of the same value
/// rather than through a struct copy.
void func_ov011_0212b318(BtlEnm010Tatt* data) {
    s32 r;

    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
        data->unk_54  = data->unk_54 & ~0x10000000;
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
    }
    if (func_ov003_020c6bc8(data, 4) != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212b388);
}

/// The next Tatt phase. Same opening as `func_ov011_0212b318` but the sprite prime comes *after*
/// the three clears, and the poll is on frame 5 rather than frame 4. Both functions are `void`:
/// the original's `cmp r0, #0` / `popne {r4, pc}` is an early-exit off a discarded predicate,
/// not a returned value, and neither epilogue substitutes a literal.
///
/// Open (2 bytes): the reference emits the counter `strh` *before* `mov r0, #0` while
/// `func_ov011_0212b318`'s identical block emits the `mov` first. Four spellings tried --
/// `= x + 1`, `+= 1`, a shared zero local feeding the clears and the call arguments, and a local
/// zero -- all byte-identical to each other. It is the scheduler's placement of the constant
/// def and is not reachable from the statement order; do not chase it further.
void func_ov011_0212b388(BtlEnm010Tatt* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
    }
    if (func_ov003_020c6c2c(data, 5) != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// Tatt's chase phase. Rolls against a per-slot threshold, and only on a hit tests the chase
/// pointer -- so the four-hop `data_ov003_020e71b8 + 0x3D000 + 0x898 + 0x28` load appears twice,
/// once per arm of the sign test, and the `||` that joins them keeps it short-circuiting.
void func_ov011_0212b3ec(BtlEnm010Tatt* data) {
    s32 v;

    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_020cb520(data, 1);
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
        data->unk_54  = data->unk_54 & ~0x10000000;
    }
    if (func_ov003_020c6b8c(data, 4) != 0) {
        return;
    }
    if (RNG_Next(0x64) < data_ov011_0212c35c[*(u16*)((u8*)data + 0x80)]) {
        v = *(s32*)((u8*)data + 0x24);
        if ((v == 0 &&
             *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28) < *(s32*)((u8*)data + 0x28)) ||
            (v == 1 && *(s32*)((u8*)(*(u32*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x898)) + 0x28) > *(s32*)((u8*)data + 0x28)))
        {
            func_ov011_02129ed0(data, func_ov011_0212aa20);
            return;
        }
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// A Tatt phase with no phase advance at the end: it primes the sprite, and clears `0x1CC` on
/// whichever frame `func_ov003_020c7070` first reports zero. The store is predicated on the
/// same compare, so it comes out as a `moveq`/`streq` pair rather than an `if` block.
void func_ov011_0212b4f4(BtlEnm010Tatt* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
        data->unk_1D8 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
        data->unk_54  = data->unk_54 & ~0x10000000;
    }
    if (func_ov003_020c7070(data) == 0) {
        data->unk_1CC = 0;
    }
}

/// A Tatt phase, and the last of the family that opens with "bump `0x1C0`, prime the sprite,
/// clear the `0x1D*` words". Three differences from `func_ov011_0212b318`: an extra
/// `func_ov003_020cb520(data, 1)` call, the `0x54` bit-28 clear, and the poll taking two
/// arguments. The `0x1D0` store appears twice -- `0x1D0`, `0x1D4`, `0x1D0` -- which is in the
/// original and is transcribed literally rather than folded.
void func_ov011_0212b558(BtlEnm010Tatt* data) {
    if (data->unk_1C0 == 0) {
        data->unk_1C0 = data->unk_1C0 + 1;
        func_ov003_020cb520(data, 1);
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
        data->unk_1D0 = 0;
        data->unk_1D4 = 0;
        data->unk_1D0 = 0;
        data->unk_54  = data->unk_54 & ~0x10000000;
    }
    if (func_ov003_020c72b4(data, 0, 4) != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// Tatt's countdown, returning 0 on the frame it expires and 1 otherwise. The opening test is
/// `movs r4, r1 / bne` -- MWCC reuses the flags of the register move rather than emitting a
/// compare, which is what `if (arg1 == 0)` produces when the argument is already being copied.
///
/// The frame filter is `(0x9A - 1) % 2 == 0`, a signed magic-divide with no `u32` cast, and it
/// is the *first* operand of the `&&` so the `0x8C` load is predicated on it.
s32 func_ov011_0212b5d8(BtlEnm010Tatt* data, s32 arg1) {
    if (arg1 == 0) {
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 1);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 0);
        *(s16*)((u8*)data + 0x1FE) = func_ov011_0212bce0(data, 0x1800);
        *(s16*)((u8*)data + 0x200) = 0;
    }
    if ((*(s16*)((u8*)data + 0x9A) - 1) % 2 == 0 && *(s16*)((u8*)data + 0x8C) == 1) {
        func_ov003_02087f00(0x1D6, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
    }
    if (arg1 == -1) {
        *(s32*)((u8*)data + 0x1E4) = 0;
        *(s32*)((u8*)data + 0x1E0) = 0;
        *(s32*)((u8*)data + 0x1DC) = 0;
        *(s16*)((u8*)data + 0x200) = 0;
        *(s16*)((u8*)data + 0x1FE) = 0;
        return 0;
    }
    if (*(s16*)((u8*)data + 0x200) < *(s16*)((u8*)data + 0x1FE)) {
        *(s16*)((u8*)data + 0x200) = *(s16*)((u8*)data + 0x200) + 1;
        return 1;
    }
    *(s32*)((u8*)data + 0x1E4) = 0;
    *(s32*)((u8*)data + 0x1E0) = 0;
    *(s32*)((u8*)data + 0x1DC) = 0;
    data->unk_1D8              = 0;
    data->unk_1D4              = 0;
    data->unk_1D0              = 0;
    *(s16*)((u8*)data + 0x200) = 0;
    *(s16*)((u8*)data + 0x1FE) = 0;
    return 0;
}

/// Tatt's homing phase, arm driver. Returns 1 from every path except the one that reports "we
/// have arrived", which returns 0 -- so the caller's `if (... == 0)` is "advance". The switch is
/// on `0xC8`, reached directly and **not** through the `+0x100` base the rest of Tatt uses, while
/// arms 0 and 1 do go through it for `0x202`/`0x204`. Both spellings are in the original.
s32 func_ov011_0212b6e0(BtlEnm010Tatt* data, s32 arg1) {
    if (arg1 == 0) {
        *(s16*)((u8*)data + 0x204) = 0;
        *(s16*)((u8*)data + 0x202) = 0;
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 7);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
        func_ov003_020cb520(data, 0);
    }
    switch (*(s16*)((u8*)data + 0xC8)) {
        case 0:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 0);
                *(s16*)((u8*)data + 0x202) = func_ov011_0212bce0(data, 0x8000);
                func_ov003_02087f00(0x1E4, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
            }
            goto out;
        case 1:
            if (*(s16*)((u8*)data + 0x204) < *(s16*)((u8*)data + 0x202)) {
                *(s16*)((u8*)data + 0x204) = *(s16*)((u8*)data + 0x204) + 1;
                goto out;
            }
            Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 2, 1);
            data->unk_1D4 = 0;
            data->unk_1D0 = 0;
            goto out;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                func_ov003_020cb520(data, 1);
                return 0;
            }
            goto out;
    }
out:
    return 1;
}

/// Tatt's coordinate phase, the predicate `func_ov011_0212a634` polls. Returns 1 while the
/// animation is still running, so the caller's "poll until it goes quiet" reads inverted, and
/// returns 0 on the frame it finishes -- having nudged `0x28` one 0x10000 unit up or down
/// depending on the sign of `0x24`.
s32 func_ov011_0212b800(BtlEnm010Tatt* data, s32 arg1) {
    if (arg1 == 0) {
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 3);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
    }
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 0);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 0);
        func_ov003_020c4c9c(data);
        if (*(s32*)((u8*)data + 0x24) == 0) {
            *(s32*)((u8*)data + 0x28) = *(s32*)((u8*)data + 0x28) + 0x10000;
        } else {
            *(s32*)((u8*)data + 0x28) = *(s32*)((u8*)data + 0x28) - 0x10000;
        }
        return 0;
    }
    return 1;
}

/// Tatt's arm driver. Returns 0 from exactly one path -- case 3 with the animation finished --
/// and 1 from all the others, including the default arm, so the switch needs a trailing `return`
/// outside it rather than a `default:` label.
///
/// Both counters are `s16 *` parameters and are decremented in place with a re-read, and both
/// only reach the `Mini108_VBlank` once the counter has gone non-positive. The `&&` on
/// `0x9A == 4` and `0x8C == 1` is the short-circuiting form, so the second load is predicated.
s32 func_ov011_0212b890(BtlEnm010Tatt* data, s16* p, s16* q, s32 arg3) {
    if (arg3 == 0) {
        func_ov011_02125750(0, (CombatSprite*)((u8*)data + 0x84), 2);
        Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 0, 1);
    }
    switch (*(s16*)((u8*)data + 0xC8)) {
        case 0:
            if (*(s16*)((u8*)data + 0x9A) == 4 && *(s16*)((u8*)data + 0x8C) == 1) {
                func_ov003_02087f00(0x1D3, func_ov003_020843b0(0, *(s32*)((u8*)data + 0x28)));
            }
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                *p = *p - 1;
                if (*p <= 0) {
                    Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 1, 0);
                }
            }
            goto out;
        case 1:
            *q = *q - 1;
            if (*q <= 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 3, 1);
            }
            goto out;
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                return 0;
            }
            goto out;
    }
out:
    return 1;
}

s32 func_ov011_0212b99c(void* arg0, void* arg1, void* arg2, s32 index) {
    BtlEnm010FnTable t;
    BtlEnm010Fn      f;

    t = data_ov011_0212c36c;
    f = t.f[index];
    return f(arg0, arg1, arg2);
}

/// UG's initialiser, and the reason `BtlEnm010UG` is 0x214: the `MI_CpuSet` clears exactly that
/// much and nothing else in the function writes further.
///
/// Three things worth writing down. The `0x100` base is materialised once and carries six
/// halfword stores, so they are written as `unk_1C0`-style fields on the struct and MWCC
/// re-derives the base. The `0x1CC` word is set to 1 and the nine after it to 0, in descending
/// triples. And `0x206` is `(x & ~1) | 1` -- a clear and an immediate re-set that MWCC does not
/// fold, so the C has to spell it as the two-step it is rather than as a plain `|= 1`.
void func_ov011_0212b9e4(void* arg0, void* arg1, void* arg2) {
    BtlEnm010UG* data;
    void*        args;

    data = *(BtlEnm010UG**)((u8*)arg1 + 0x18);
    args = arg2;
    MI_CpuSet(data, 0, 0x214);
    func_ov011_021256c0(*(u16*)((u8*)args + 0x04));
    func_ov003_020c3efc(data, args);
    func_ov003_020c4520(data);
    func_ov003_020c4b5c(data);
    data->unk_1C0              = 0;
    data->unk_1C2              = 0;
    data->unk_1C4              = 0;
    data->unk_1CC              = 1;
    data->unk_1D8              = 0;
    data->unk_1D4              = 0;
    data->unk_1D0              = 0;
    data->unk_1E4              = 0;
    data->unk_1E0              = 0;
    data->unk_1DC              = 0;
    data->unk_1F0              = 0;
    data->unk_1EC              = 0;
    data->unk_1E8              = 0;
    *(s16*)((u8*)data + 0x1F6) = 0;
    data->unk_1F8              = 0;
    data->unk_1FA              = 0;
    data->unk_208              = -1;
    data->unk_20C              = -1;
    func_ov011_0212bd90((void*)data, 0);
    func_ov011_02129ed0((void*)data, func_ov011_0212a10c);
    data->unk_206              = (data->unk_206 & 0xFE) | 1;
    data->unk_54               = data->unk_54 | 0x40000000;
    *(u16*)((u8*)data + 0x18C) = *(u16*)((u8*)data + 0x18C) | 4;
}

/// Tatt's per-frame handler. The switch has no `default` and its first two arms are the same
/// label as the end of the switch, so they fall straight through to the common tail. The six
/// accumulations are six independent read-add-writes -- three into the position triple at
/// `0x28`/`0x2C`/`0x30` and three into the velocity triple at `0x1D0`/`0x1D4`/`0x1D8`.
s32 func_ov011_0212bac8(void* arg0, void* arg1) {
    BtlEnm010Tatt* data;

    data = (BtlEnm010Tatt*)*(void**)((u8*)arg1 + 0x18);
    switch (CombatActor_PopPendingCommand((CombatActor*)data)) {
        case 2:
            if (((u32)(*(u8*)((u8*)data + 0x206) << 31) >> 31) == 0) {
                break;
            }
            func_ov003_02084694((void*)((u8*)data + 0x144), 0);
            func_ov011_02129ed0(data, func_ov011_0212b3ec);
            break;
        case 3:
            func_ov003_02084694((void*)((u8*)data + 0x144), 1);
            func_ov011_02129ed0(data, func_ov011_0212b4f4);
            break;
        case 4:
            func_ov003_02084694((void*)((u8*)data + 0x144), 0);
            func_ov011_02129ed0(data, func_ov011_0212b318);
            break;
        case 5:
            func_ov003_02084694((void*)((u8*)data + 0x144), 0);
            func_ov011_02129ed0(data, func_ov011_0212b388);
            break;
        case 6:
            func_ov011_02129ed0(data, func_ov011_0212b558);
            break;
    }
    if (*(u8*)((u8*)data_ov003_020e71b8 + 0x3D000 + 0x874) == 2) {
        if (func_ov003_020c3bf0(data) == 0) {
            func_ov011_0212bd90(data, 0);
        }
    }
    EasyTask_ValidateTaskId((void*)((u8*)data_ov003_020e71b8 + 0x3D000), (void*)((u8*)data + 0x208));
    if (*(s32*)((u8*)data + 0x20C) != -1) {
        *(s32*)((u8*)data + 0x210) = *(s32*)((u8*)data + 0x210) + 1;
    }
    if (*(void**)((u8*)data + 0x1C8) != NULL) {
        (*(void (**)(void*))((u8*)data + 0x1C8))(data);
    }
    *(s32*)((u8*)data + 0x28) = *(s32*)((u8*)data + 0x28) + *(s32*)((u8*)data + 0x1D0);
    *(s32*)((u8*)data + 0x2C) = *(s32*)((u8*)data + 0x2C) + *(s32*)((u8*)data + 0x1D4);
    *(s32*)((u8*)data + 0x30) = *(s32*)((u8*)data + 0x30) + *(s32*)((u8*)data + 0x1D8);
    data->unk_1D0             = data->unk_1D0 + *(s32*)((u8*)data + 0x1E8);
    data->unk_1D4             = data->unk_1D4 + *(s32*)((u8*)data + 0x1EC);
    data->unk_1D8             = data->unk_1D8 + *(s32*)((u8*)data + 0x1F0);
    func_ov003_020c4668(data);
    return data->unk_1CC;
}

/// A phase of whatever task this is, and the exact twin of `func_ov011_02127b98` at a `+0x20C`
/// base instead of `+0x200`. `0x20C == -1` together with a non-zero `0x210` selects the
/// `020c4748` + `0208810c` pair; anything else -- including the "both are in range" case -- takes
/// the `020c48b0` path. The predicate is a short-circuiting `||`, which is what produces the
/// `ldrne`/`cmpne` pair and not a load-compare per operand.
s32 func_ov011_0212bc84(void* arg0, void* arg1) {
    void* p;

    p = *(void**)((u8*)arg1 + 0x18);
    if (*(s32*)((u8*)p + 0x20C) == -1 || *(s32*)((u8*)p + 0x210) == 0) {
        func_ov003_020c48b0(p);
    } else {
        func_ov003_020c4748(p);
        func_ov003_0208810c((void*)((u8*)p + 0xE4), p);
    }
    return 1;
}

/// The matching task entry: hand the data to `func_ov003_020c492c`, then set bit 4 of the
/// relevant task's `0x04` through the zero-argument `func_ov011_02125714`.
s32 func_ov011_0212bcc8(void* arg0, void* arg1) {
    s32 r;

    func_ov003_020c492c(*(void**)((u8*)arg1 + 0x18));
    func_ov011_02125714();
    return 1;
}

/// Tatt's velocity integrator. Identical to `func_ov011_02127bf0`'s call shape -- ten arguments,
/// six of them spilled to the outgoing area in the same order -- but against Tatt's `0x1D*` block
/// and it returns an `s16` rather than storing one.
s16 func_ov011_0212bce0(BtlEnm010Tatt* data, s32 arg1) {
    return func_ov003_020cb910(&data->unk_1D0, &data->unk_1D4, (s32*)((u8*)data + 0x1D8), *(s32*)((u8*)data + 0x28),
                               *(s32*)((u8*)data + 0x2C), *(s32*)((u8*)data + 0x30), *(s32*)((u8*)data + 0x1DC),
                               *(s32*)((u8*)data + 0x1E0), *(s32*)((u8*)data + 0x1E4), arg1);
}

/// Scatters one of `0x1F4`-based counters by a random walk. Two `s16` tables sit two bytes apart
/// and are both indexed with a four-byte stride, so the subscript is `i * 2` on an `s16*`; the
/// random draw's bound is `2 * lo[i] + 1` and the result is biased by `-lo[i] + hi[i]`, which
/// is why the `rsb` comes before `hi[i]` is even loaded.
void func_ov011_0212bd3c(void* p, s32 i) {
    s32 lo;
    s32 hi;
    s32 r;

    lo                                     = data_ov011_0212c30e[i * 2];
    r                                      = RNG_Next(lo * 2 + 1);
    hi                                     = data_ov011_0212c30c[i * 2];
    *(s16*)((u8*)p + 0x100 + i * 2 + 0xF4) = hi + (r - lo);
}

/// Decrement the `0x1F4`-based counter for slot `i`, but only while it is still positive. The
/// guard and the body reach the same address by two different routes -- `i * 2 + 0x100 + 0xF4`
/// in the test and a `+0x1F4` base in the body -- and that asymmetry is in the original, so the
/// two spellings are kept apart here rather than folded into one pointer.
void func_ov011_0212bd90(void* p, s32 i) {
    s16* q;

    if (*(s16*)((u8*)p + i * 2 + 0x100 + 0xF4) <= 0) {
        return;
    }
    q    = (s16*)((u8*)p + 0x1F4);
    q[i] = q[i] - 1;
}

/// Is the `0x1F4`-based counter for slot `i` exhausted? `<= 0` rather than `< 0`, which is what
/// the `movle`/`movgt` pair against literal 1/0 encodes.
s32 func_ov011_0212bdbc(void* p, s32 i) {
    return *(s16*)((u8*)p + 0x100 + i * 2 + 0xF4) <= 0;
}
