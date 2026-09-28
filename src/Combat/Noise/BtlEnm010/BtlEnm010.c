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
void func_ov011_02125750(s32 arg0, CombatSprite* arg1, s32 arg2) {
    s32              flag = (arg1->flags46 & 1) ? 1 : 0;
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

/// A pair of running maxima, kept in the frame. The original zeroes each pair with a single
/// `str` pair through a base pointer (`add r4, sp, #0xc / str r1, [r4, #0] / str r1, [r4, #4]`)
/// and then reads and writes the halves at constant frame slots, so the two maxima are one
/// 8-byte object, not two scalars. Spelled as two `s32` locals MWCC promotes all four to
/// callee-saved registers instead and the loop bodies lose their stores entirely.
typedef struct BtlEnm010Pair {
    s32 max;
    s32 sec;
} BtlEnm010Pair;

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

/// The `TaskHandle` of the `Tsk_BtlEnm010_Lser` task (0x254 bytes of data), the task this
/// function spawns.
extern const TaskHandle data_ov011_0212c118;

/// `func_ov003_020c37f8` -- reads the two-bit field at offset 0 of its argument and returns
/// whether it is 1. One argument.
extern s32 func_ov003_020c37f8(void* p);

/// Spawns the `Tsk_BtlEnm010_Lser` task. Which pool it goes into depends on a two-bit flag at
/// `arg0 + 0x84`.
///
/// The global is re-read in *both* arms, so it is spelled twice rather than hoisted: the
/// original has a predicated `ldreq` pair on the zero arm and an unpredicated `ldr` pair on
/// the other, with the `+ 0x8C + 0x8000` bias added in the second.
void func_ov011_02125b98(void* arg0, void* arg1) {
    BtlEnm010LserArgs args;
    TaskPool*         pool;

    if (func_ov003_020c37f8((u8*)arg0 + 0x84) == 0) {
        pool = (TaskPool*)data_ov003_020e71b8;
    } else {
        pool = (TaskPool*)((u32)data_ov003_020e71b8 + 0x8C + 0x8000);
    }
    args.unk_00 = arg0;
    args.unk_14 = arg1;
    EasyTask_CreateTask(pool, &data_ov011_0212c118, 0, 0, 0, &args);
}

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

/// `func_ov003_02082d04` -- `Sprite_Restart`, one argument, tail-called.
extern void func_ov003_02082d04(CombatSprite* cSprite);

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
    func_ov003_02082d04(sprite);
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

/// `func_ov003_020c3c28` -- no arguments; returns a global mode bit.
extern s32 func_ov003_020c3c28(void);

/// `func_ov003_020cc354` -- one argument; bails out of the copy block if its `0x54` flags are
/// set or its `0x5A` countdown is non-positive, and returns whether it bailed.
extern s32 func_ov003_020cc354(void* p);

/// `func_ov003_02082b0c` -- one argument, a `CombatSprite*`; ticks a palette timer behind the
/// sprite's `flags46` bit 12.
extern void func_ov003_02082b0c(CombatSprite* cSprite);

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

/// `func_ov003_02082724` -- three arguments; `strh r1, [r0, #0xc] / strh r2, [r0, #0xe]`.
extern void func_ov003_02082724(CombatSprite* cSprite, s32 arg1, s32 arg2);

/// `func_ov003_02082b64` -- one argument, a `CombatSprite*`; ticks the sprite.
extern void func_ov003_02082b64(CombatSprite* cSprite);

/// The fixed-point rounding idiom shared with `BtlEnm014`: convert a 4.12 value to `f32`, bias it
/// by a half, and truncate. The sign test is repeated in both arms, so the value itself is
/// re-derived per arm -- here that means `func_ov003_020843b0` is called three times.
#define ROUND(value) ((s32)((value) > 0 ? (f32)((value) * 0x1000) + 0.5f : (f32)((value) * 0x1000) - 0.5f))

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
        func_ov003_02082b0c(sp);
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
            func_ov003_02082724(sp, (acc0 * 16) >> 16, (acc1 * 16) >> 16);
            func_ov003_02082730(sp, 0x7FFFFFFE - outY);
            func_ov003_02082b64(sp);
            rec = (BtlEnm010LserRec*)((u8*)rec + 0xC);
            sp  = (CombatSprite*)((u8*)sp + 0x60);
        }
    }

    // Bit 1: the centre sprite at `0x1A0`, on the offsets alone.
    if ((u32)(data->unk_250 << 30) >> 31) {
        func_ov003_02082724((CombatSprite*)((u8*)data + 0x1A0), (vx * 16) >> 16, (vy * 16) >> 16);
        func_ov003_02082730((CombatSprite*)((u8*)data + 0x1A0), 0x7FFFFFFF - outY);
        func_ov003_02082b64((CombatSprite*)((u8*)data + 0x1A0));
    }

    // Bit 2: the first sprite at `0x80`, on the owner's own projected position.
    if ((u32)(data->unk_250 << 29) >> 31) {
        func_ov003_02084348(mode, &t1, &t0, data->unk_22C, data->unk_230, data->unk_234);
        func_ov003_02082724((CombatSprite*)((u8*)data + 0x80), t1, t0);
        func_ov003_02082730((CombatSprite*)((u8*)data + 0x80), 0x7FFFFFFD - outY);
        func_ov003_02082b64((CombatSprite*)((u8*)data + 0x80));
    }
    return 1;
}

/// The Lser task's command 3: four identical sprite ticks and nothing else.
s32 func_ov011_021265a8(BtlEnm010Lser* data) {
    s32           i;
    CombatSprite* sp;

    sp = (CombatSprite*)((u8*)data + 0x80);
    for (i = 0; i < 4; i++) {
        func_ov003_02082cc4(sp);
        sp = (CombatSprite*)((u8*)sp + 0x60);
    }
    return 1;
}

/// `func_ov003_020c427c` -- a two-argument thunk: `ldr ip, .L / bx ip` straight into
/// `func_ov003_020c37bc` with r0/r1 untouched.
extern void func_ov003_020c427c(void* p, void* arg1);

/// RG's phase-0 setup. Called with the address of the next phase handler, which it stores at
/// `0x1C8` after clearing the `0x1C0`/`0x1C4` pair. The `add r0, r5, #0x100` base reaching
/// `0x1C4`/`0x1C0` is why those two are a pair rather than two flat fields.
void func_ov011_021265d4(BtlEnm010RG* data, void (*func)(struct BtlEnm010RG*)) {
    func_ov003_020c427c(data, (void*)func);
    data->unk_1C8 = func;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

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

/// `func_ov003_020c5bfc` -- no arguments; a non-zero return aborts the caller.
extern s32 func_ov003_020c5bfc(void);

/// `func_ov003_020c42ec` -- one argument, the task data; its `s32` return is stored with a
/// `strh` at `0x1C2`, so only its low halfword survives.
extern s32 func_ov003_020c42ec(void* p);

extern void func_ov011_02126aec(BtlEnm010RG* data);
extern void func_ov011_0212681c(BtlEnm010RG* data);

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

extern s32 func_ov003_020cb744(s32 arg);
extern s32 func_ov003_020cb7a4(s32 arg);
extern s32 func_ov003_020c4ab4(BtlEnm010RG* data, s32 arg1);
extern s32 func_ov011_02127bf0(BtlEnm010RG* data, s32 arg1);

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

extern s32  func_ov003_020c4c9c(void* p);
extern void func_ov003_020c48b0(void* p);
extern void func_ov003_020c492c(void* p);
extern s32  func_ov003_020cb520(void* p, s32 arg1);
extern s32  func_ov003_020cb594(void* p, s32 arg1);

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

extern s32 func_ov011_02127758(void* p, s32 arg1);

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

extern s32 func_ov003_020c6230(void* p);
extern s32 func_ov011_021277c8(void* p, s16* arg1, s16* arg2, s32 arg3);
extern s32 func_ov011_02127c84(void* p);

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

extern s32 func_ov003_020c703c(void* p);

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

/// The four RG phase entries as one 0x10-byte block of function pointers.
///
/// The size is pinned twice: `func_ov011_021278d4` copies exactly 16 bytes of it with a single
/// `ldm r0, {r0, r1, r2, r3} / stm`, and `data_ov011_0212c168` -- the table `func_ov011_02126bf8`
/// indexes -- starts 0x10 bytes later.
typedef struct BtlEnm010RGEntry {
    /* 0x00 */ void (*func[4])(void*, s32, s32);
} BtlEnm010RGEntry;

extern const BtlEnm010RGEntry data_ov011_0212c158;

/// RG's task entry. Copies the whole 0x10-byte entry block to the frame and then calls word
/// `index` of it -- the `ldm/stm` is the copy regenerating itself, so it has to be a struct
/// assignment and never a hand-written loop.
void func_ov011_021278d4(void* p, s32 arg1, s32 arg2, s32 index) {
    BtlEnm010RGEntry t;

    t = data_ov011_0212c158;
    t.func[index](p, arg1, arg2);
}

extern s32 func_ov003_020c62c4(void* p, s32 arg1);
extern s32 func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);

extern void func_ov003_02082750(CombatSprite* cSprite, s32 arg1, s32 arg2);
extern s32  func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

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
                func_ov003_02082750((CombatSprite*)data, r, 0);
            } else {
                func_ov003_02082750((CombatSprite*)data, 0, 0);
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
            func_ov003_02082b0c((CombatSprite*)data);
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
            func_ov003_02082724((CombatSprite*)data, pan, y);
            func_ov003_02082730((CombatSprite*)data, 0x7FFFFFFF - (data->unk_060->unk_2C + 0x20000));
            func_ov003_02082b64((CombatSprite*)data);
            break;
        case 3:
            func_ov003_02082cc4(data);
            break;
    }
    return r;
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

extern s32 func_ov003_020c3efc(void* p, void* arg1);
extern s32 func_ov003_020c44ac(void* p);
extern s32 func_ov003_020c4b1c(void* p);

/// RG's initialiser, and the only write to the whole 0x208 block.
///
/// Two things are load-bearing here. `unk_1FC`/`unk_200` are written as `unk_1CC - 2`, not as
/// `-1`: the original emits `mov r0, #1 / str [r4, #0x1cc] / sub r0, r0, #2`, which a literal
/// `-1` would have collapsed to a single `mvn`. And `0x28`/`0x2C` are written twice, once
/// biased `+0x40000` through the pool pointer and once biased `-0x40000` through `data` -- the
/// second write wins, and both have to be in the C.
void func_ov011_0212791c(void* arg0, void* arg1, s32 arg2) {
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
}

extern void func_ov003_020c4748(void* p);
extern s32  func_ov003_0208810c(void* p, void* arg1);
extern s32  func_ov003_020cba54(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4, s32 a5);
extern s32  func_ov003_020cb910(void* a0, void* a1, void* a2, s32 a3, s32 a4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9);

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

/// RG's frame callback. Returns 1 unconditionally.
s32 func_ov011_02127bdc(void* arg0, void* arg1) {
    func_ov003_020c48fc(*(void**)((u8*)arg1 + 0x18));
    return 1;
}

extern s32              func_ov003_02082f2c(void* p);
extern s32              func_ov003_020c4628(void* p);
extern const TaskHandle data_ov011_0212c1d4;

/// RG's per-frame handler. Picks a phase off `func_ov003_02082f2c`, integrates two Euler steps
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
    switch (func_ov003_02082f2c(data)) {
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

extern s32 func_ov011_02128070(void* p, void* arg1);
extern s32 func_ov011_02128150(void* p);
extern s32 func_ov011_02128250(void* p);

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

/// `Tsk_BtlEnm010_Rnge`'s task spawn. Nine arguments in, a 0x20-byte block out.
///
/// Two things are load-bearing. `arg7` is a by-value `u16` that the C *assigns*, and MWCC reuses
/// its incoming stack slot for the store -- the original's `moveq r1, #0 / strheq r1, [sp, #0x48]`
/// writes into the caller's outgoing area, which only happens for an assigned parameter whose
/// address is never taken. And the last argument to `EasyTask_CreateTask` is the *address of the
/// block*, not the block.
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

extern const TaskHandle data_ov011_0212c210;

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

extern const TaskHandle data_ov011_0212c1f8;

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

extern s32 func_ov011_021283a8(BtlEnm010SingleShot* data, void* arg1);
extern s32 func_ov011_021284bc(void* p);
extern s32 func_ov011_02128698(void* p);

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

extern const s32 data_ov011_0212c1ec[];
extern s32       func_ov003_0208260c(void* p, s32 a1, s32 a2, s32 a3, s32 a4);

/// SingleShot's phase-2 handler: three passes over a table and a 0x10-stride record block.
///
/// `base` is a genuine second counter in the original -- `mov r4, r6` before the loop and `r4`
/// never stepped again, so arguments 2 and 5 of `func_ov003_0208260c` are always 0 while the
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
        func_ov003_0208260c((void*)((u8*)data + 4), base, data_ov011_0212c1ec[i], data_ov011_0212c1ec[i], base);
        func_ov003_02082724((CombatSprite*)((u8*)data + 4), *(s16*)((u8*)rec + 0xC), *(s16*)((u8*)rec + 0xE));
        func_ov003_02082b64((CombatSprite*)((u8*)data + 4));
        rec = (void*)((u8*)rec + 0x10);
    }
    return 1;
}

/// Ticks the sprite at `+0x04` and nothing else.
s32 func_ov011_02128704(void* p) {
    func_ov003_02082cc4((void*)((u8*)p + 4));
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

extern s32 func_ov011_021287b8(BtlEnm010Sprl* data, void* arg1);
extern s32 func_ov011_021288c8(void* p);
extern s32 func_ov011_02128b80(BtlEnm010Sprl* data);
extern s32 func_ov011_02128c30(void* p);

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

extern const s32 data_ov011_0212c21c[];

/// Sprl's phase-2 handler: five passes, each one projecting, positioning and then animating.
///
/// The two arrays are reached as `*(data + i * 4 + 0x70)` and `*(data + i * 4 + 0x84)`, written
/// here as subscripts off a `data + i * 4` base so the scaled add stays inline -- as a single
/// `+ 0x70` / `+ 0x84` bias it materialises `i * 4` into a register instead, which is what
/// `func_ov011_02128250` does and why that one is two words long.
s32 func_ov011_02128b80(BtlEnm010Sprl* data) {
    s16 v0;
    s16 v1;
    s32 i;
    s32 z;

    z = 0;
    for (i = 0; i < 5; i++) {
        func_ov003_02084348(z, &v0, &v1, data->unk_070[i], data->unk_098, data->unk_084[i]);
        func_ov003_02082724((CombatSprite*)((u8*)data + 4), v0, v1);
        func_ov003_02082730((CombatSprite*)((u8*)data + 4), 0x7FFFFFFF - data->unk_084[i]);
        func_ov003_0208260c((void*)((u8*)data + 4), z, data_ov011_0212c21c[i], data_ov011_0212c21c[i], z);
        func_ov003_02082b64((CombatSprite*)((u8*)data + 4));
    }
    return 1;
}

/// Ticks the sprite at `+0x04` and nothing else. Second of the two identical 20-byte tickers
/// (`0x02128704` was the first).
s32 func_ov011_02128c30(void* p) {
    func_ov003_02082cc4((void*)((u8*)p + 4));
    return 1;
}

extern const TaskHandle data_ov011_0212c240;

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

/// Tatt's phase-0 setup. The RG pair again -- `func_ov003_020c427c` thunk, then the callback at
/// `0x1C8`, then the `0x1C4`-before-`0x1C0` clear pair. Third confirmation of that ordering.
void func_ov011_02129ed0(BtlEnm010Tatt* data, void (*func)(BtlEnm010Tatt*)) {
    func_ov003_020c427c(data, (void*)func);
    data->unk_1C8 = func;
    data->unk_1C4 = 0;
    data->unk_1C0 = 0;
}

/// Four sprite ticks at a 0x88 stride from `+0x04`. `i` is declared before the walking pointer
/// because the original puts the counter in r4 and the pointer in r5.
s32 func_ov011_02129ea4(void* p) {
    s32   i;
    void* sp;

    sp = (void*)((u8*)p + 4);
    for (i = 0; i < 4; i++) {
        func_ov003_02082cc4(sp);
        sp = (void*)((u8*)sp + 0x88);
    }
    return 1;
}

extern s32  func_ov011_02129994(void* p, s32 arg1);
extern s32  func_ov011_02129b84(BtlEnm010Tatt* data);
extern s32  func_ov011_02129cec(BtlEnm010Tatt* p);
extern void func_ov011_0212a134(BtlEnm010Tatt* data);
extern s32  func_ov011_0212b800(BtlEnm010Tatt* data, s32 arg1);

/// `Tsk_BtlEnm010_Tatt`'s task entry. Fifth and last of the identical four-way dispatchers
/// (`0x0212801c`, `0x02128348`, `0x02128758`, and this one).
s32 func_ov011_02129934(void* arg0, void* arg1, s32 arg2, s32 index) {
    void* p;
    s32   r;

    p = *(void**)((u8*)arg1 + 0x18);
    r = 1;
    switch (index) {
        case 0:
            r = func_ov011_02129994(p, arg2);
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

/// Tatt's task entry proper. Bails on whatever `func_ov003_020c5bfc` reports, otherwise installs
/// phase 0 -- the same two-line shape as RG's `func_ov011_021267f4`.
void func_ov011_0212a10c(BtlEnm010Tatt* data) {
    if (func_ov003_020c5bfc() != 0) {
        return;
    }
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

/// One of Tatt's phases: poll a predicate on the `0x1C0` counter, and on the frame it goes quiet
/// bump the counter and advance to the next phase.
void func_ov011_0212a634(BtlEnm010Tatt* data) {
    if (func_ov011_0212b800(data, data->unk_1C0) != 0) {
        return;
    }
    data->unk_1C0 = data->unk_1C0 + 1;
    func_ov011_02129ed0(data, func_ov011_0212a134);
}

extern s32       func_ov011_02128f80(BtlEnm010Tatt* data);
extern const u16 data_ov011_0212c28c[];
extern const u16 data_ov011_0212c28e[];
extern const u16 data_ov011_0212c290[];

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
void func_ov011_02128f10(void* p, u16 v) {
    void* data;
    s32   i;

    data = p;
    for (i = 0; i < 0x10; i++) {
        if (v < data_ov011_0212c28c[i * 4]) {
            break;
        }
    }
    if (i == 0x10) {
        i = 0;
    }
    *(u16*)((u8*)data + 0x70) = v;
    Mini108_VBlank((CombatSprite*)data, data_ov011_0212c28e[i * 4], 0);
    func_ov003_02082750((CombatSprite*)data, data_ov011_0212c290[i * 4], 0);
}

extern void func_ov011_02129188(void* p, void* rec);
extern void func_ov011_021293a8(void* p, void* rec);
extern void func_ov011_02129410(void* p, void* rec);

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

extern s32  func_ov003_020c6bc8(BtlEnm010Tatt* data, s32 arg1);
extern void func_ov011_0212b388(BtlEnm010Tatt* data);

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

extern s32 func_ov003_020c6c2c(BtlEnm010Tatt* data, s32 arg1);

/// The next Tatt phase. Same opening as `func_ov011_0212b318` but the sprite prime comes *after*
/// the three clears, and the poll is on frame 5 rather than frame 4. Both functions are `void`:
/// the original's `cmp r0, #0` / `popne {r4, pc}` is an early-exit off a discarded predicate,
/// not a returned value, and neither epilogue substitutes a literal.
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

extern s32 func_ov003_020c7070(BtlEnm010Tatt* data);

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

extern s32  func_ov003_020c4c9c(void* p);
extern void func_ov003_020c48b0(void* p);
extern void func_ov003_020c492c(void* p);

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

s32 func_ov011_0212b99c(void* arg0, void* arg1, void* arg2, s32 index) {
    BtlEnm010FnTable t;
    BtlEnm010Fn      f;

    t = data_ov011_0212c36c;
    f = t.f[index];
    return f(arg0, arg1, arg2);
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

extern const s16 data_ov011_0212c30c[];
extern const s16 data_ov011_0212c30e[];

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

/// Is the `0x1F4`-based counter for slot `i` exhausted? `<= 0` rather than `< 0`, which is what
/// the `movle`/`movgt` pair against literal 1/0 encodes.
s32 func_ov011_0212bdbc(void* p, s32 i) {
    return *(s16*)((u8*)p + 0x100 + i * 2 + 0xF4) <= 0;
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

extern s32 func_ov003_020c72b4(void* p, s32 arg1, s32 arg2);

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

extern void func_ov011_02128ca4(BtlEnm010Tatt* data, s32 arg1);
extern void func_ov011_02128eb0(BtlEnm010Tatt* data);

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

extern void func_ov011_021256c0(u16 arg0);
extern void func_ov003_020c4520(void* p);
extern void func_ov003_020c4b5c(void* p);

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

extern s32 func_ov011_0212b890(BtlEnm010Tatt* data, s16* arg1, s16* arg2, s32 arg3);

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

extern s32 func_ov003_020c5b2c(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4);

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

extern s32       func_ov003_020cb888(void* p, s32 arg1, s32 arg2);
extern const u32 data_ov011_0212cc7c[];
extern void      func_ov011_0212a134(BtlEnm010Tatt* data);
extern void      func_ov011_0212a2ec(BtlEnm010Tatt* data);
extern void      func_ov011_0212a420(BtlEnm010Tatt* data);
extern void      func_ov011_0212a634(BtlEnm010Tatt* data);

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

extern s32 func_ov003_020cb594(void* p, s32 arg1);

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

extern s16 func_ov011_0212bce0(BtlEnm010Tatt* data, s32 arg1);

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

/// Tatt's velocity integrator. Identical to `func_ov011_02127bf0`'s call shape -- ten arguments,
/// six of them spilled to the outgoing area in the same order -- but against Tatt's `0x1D*` block
/// and it returns an `s16` rather than storing one.
s16 func_ov011_0212bce0(BtlEnm010Tatt* data, s32 arg1) {
    return func_ov003_020cb910(&data->unk_1D0, &data->unk_1D4, (s32*)((u8*)data + 0x1D8), *(s32*)((u8*)data + 0x28),
                               *(s32*)((u8*)data + 0x2C), *(s32*)((u8*)data + 0x30), *(s32*)((u8*)data + 0x1DC),
                               *(s32*)((u8*)data + 0x1E0), *(s32*)((u8*)data + 0x1E4), arg1);
}

extern s32       func_ov003_020cb744(s32 arg0);
extern s32       func_ov003_020cb7a4(s32 arg0);
extern s32       func_ov003_020c6b8c(BtlEnm010Tatt* data, s32 arg1);
extern const s32 data_ov011_0212c35c[];
extern void      func_ov011_0212aa20(BtlEnm010Tatt* data);
extern s32       func_ov011_0212b6e0(BtlEnm010Tatt* data, s32 arg1);
extern s32       func_ov011_0212b5d8(BtlEnm010Tatt* data, s32 arg1);

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
                data->unk_1F8 = 0xBD;
                data->unk_1FA = 0x1E;
            }
            if (func_ov011_0212b890(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), data->unk_1C0) != 0) {
                return;
            }
            if (data->unk_1F8 == 0 && data->unk_1FA == 0xBD) {
                data->unk_208 = func_ov011_02127c84(data);
            }
            func_ov011_02129ed0(data, func_ov011_0212a134);
            return;
    }
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

extern s32 func_ov003_02084348(s32 a0, s16* a1, s16* a2, s32 a3, s32 a4, s32 a5);
extern s32 func_ov003_020cc7c0(s32 a0, s32 a1, s32 a2);

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

extern s32  func_ov003_020c42ec(void* p);
extern s32  func_ov003_020c4348(void* p);
extern void func_ov011_02082d04(void* p);
extern void func_ov011_0212b168(BtlEnm010Tatt* data);
extern void func_ov011_0212a78c(BtlEnm010Tatt* data);
extern void func_ov011_0212ac0c(BtlEnm010Tatt* data);
extern void func_ov011_0212aee8(BtlEnm010Tatt* data);
extern s32  func_ov011_0212bdbc(void* p, s32 i);
extern void func_ov011_02129f80(BtlEnm010Tatt* data);

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
                func_ov011_02082d04((CombatSprite*)((u8*)data + 0x84));
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

extern const s32 data_ov011_0212c37c[];
extern const s32 data_ov011_0212c390[];
extern const s32 data_ov011_0212c330[];
extern const s32 data_ov011_0212c3a8[];

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
    u32 n;

    f = NULL;
    n = *(s16*)((u8*)data + 0x1F6);
    v = 0;
    switch (*(u16*)((u8*)data + 0x80)) {
        case 0:
            v = data_ov011_0212c37c[n % 5];
            break;
        case 1:
            v = data_ov011_0212c390[n % 6];
            break;
        case 2:
            if (func_ov011_0212bdbc(data, 0) != 0) {
                v = 5;
            } else {
                v = data_ov011_0212c330[RNG_Next(3)];
            }
            break;
        case 3:
            v = *(s32*)((u8*)data_ov011_0212c3a8 + (((n << 30) >> 27) + (u32)RNG_Next(2) * 4));
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

extern s32       func_ov003_020c3c28(void);
extern void      func_ov011_02129110(void* p);
extern void      func_ov011_02128e30(BtlEnm010Tatt* data);
extern void      func_ov011_02128f10(void* p, u16 v);
extern const s32 data_ov011_0212c24c[];

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

extern s32  func_ov003_02082f2c(void* p);
extern void func_ov003_02084694(void* p, s32 arg1);
extern s32  func_ov003_020c3bf0(void* p);
extern s32  func_ov003_020c4668(void* p);

/// Tatt's per-frame handler. The switch has no `default` and its first two arms are the same
/// label as the end of the switch, so they fall straight through to the common tail. The six
/// accumulations are six independent read-add-writes -- three into the position triple at
/// `0x28`/`0x2C`/`0x30` and three into the velocity triple at `0x1D0`/`0x1D4`/`0x1D8`.
s32 func_ov011_0212bac8(void* arg0, void* arg1) {
    BtlEnm010Tatt* data;

    data = (BtlEnm010Tatt*)*(void**)((u8*)arg1 + 0x18);
    switch (func_ov003_02082f2c(data)) {
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
            return 1;
        case 1:
            if (*(s16*)((u8*)data + 0x204) < *(s16*)((u8*)data + 0x202)) {
                *(s16*)((u8*)data + 0x204) = *(s16*)((u8*)data + 0x204) + 1;
                return 1;
            }
            Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 2, 1);
            data->unk_1D4 = 0;
            data->unk_1D0 = 0;
            return 1;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                func_ov003_020cb520(data, 1);
                return 0;
            }
            return 1;
    }
    return 1;
}

extern s32 func_ov003_020843ec(s32 a0, s32 a1, s32 a2);

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
    dir = (s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31);
    if (func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) > 0) {
        *(s32*)((u8*)data + 0x23C) = _ffix(_fadd(_fflt(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) << 12), 0.5f));
    } else {
        *(s32*)((u8*)data + 0x23C) = _ffix(_fsub(_fflt(func_ov003_020843b0(dir, *(s32*)((u8*)data + 0x230)) << 12), 0.5f));
    }
    if (func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) > 0) {
        *(s32*)((u8*)data + 0x240) =
            _ffix(_fadd(_fflt(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) << 12), 0.5f));
    } else {
        *(s32*)((u8*)data + 0x240) =
            _ffix(_fsub(_fflt(func_ov003_020843ec(dir, *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238)) << 12), 0.5f));
    }
    if (*(void**)((u8*)data + 0x224) != NULL) {
        (*(void (**)(void*))((u8*)data + 0x224))(data);
    }
    func_ov011_02129110(data);
    p = (s16*)((u8*)data + 0x88);
    q = (s16*)((u8*)data + 4);
    for (i = 0; i < 4; i++) {
        if (((u32)(*p << 30) >> 31) == 1) {
            func_ov003_02082b0c((CombatSprite*)q);
            r = 1;
        }
        p = (s16*)((u8*)p + 0x88);
        q = (s16*)((u8*)q + 0x88);
    }
    return r;
}

extern const s32 data_ov011_0212c204[];

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

    o = NULL;
    r = 0;
    if (func_ov003_020c3c28() != 0) {
        return r;
    }
    o = *(u32**)p;
    if (o != NULL) {
        if (*(s32*)((u8*)o + 0x54) & 4) {
            return r;
        }
    }
    if (*(s16*)((u8*)p + 0x08) == 0) {
        if (func_ov003_020c37f8((void*)((u8*)o + 0x84)) != 0) {
            func_ov003_02087f00(0x1D9, func_ov003_020843b0(1, *(s32*)((u8*)o + 0x28)));
        } else {
            func_ov003_02087f00(0x1D9, func_ov003_020843b0(0, *(s32*)((u8*)o + 0x28)));
        }
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
            return 1;
        case 1:
            *q = *q - 1;
            if (*q <= 0) {
                Mini108_VBlank((CombatSprite*)((u8*)data + 0x84), 3, 1);
            }
            return 1;
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)data + 0x84)) != 0) {
                return 0;
            }
            return 1;
    }
    return 1;
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

extern s32 func_ov003_020cba2c(s32 a0, s32 a1, s32 a2, s32 a3);

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

extern s32 func_ov011_02129cec(BtlEnm010Tatt* data);

/// Rnge's spawn. The 0x20-byte data is a five-slot task-id array at `0x0C`; the argument is the
/// same 0x20-byte spawn block, and `0x18` is the slot count. The per-slot x is a **16-bit**
/// fixed-point sum: the division result and the `0x14 - step/2` bias are each rounded through
/// `lsl #0x10 / lsr #0x10` and then added and rounded again, so the whole expression is `s16`.
/// Rnge's spawn. The 0x20-byte data is a five-slot task-id array at `0x0C`; the argument is the
/// same 0x20-byte spawn block, and `0x18` is the slot count. The per-slot x is a **16-bit**
/// fixed-point sum: the division result and the `0x14 - step/2` bias are each rounded through
/// `lsl #0x10 / lsr #0x10` and then added and rounded again, so the whole expression is `s16`.
s32 func_ov011_02128070(void* p, void* a) {
    s32 count;
    s32 i;
    s32 step;
    s32 v;
    s32 r;

    MI_CpuSet(p, 0, 0x20);
    count = *(s32*)((u8*)a + 0x18);
    for (i = 0; i < count; i++) {
        step = *(u16*)((u8*)a + 0x1C);
        v    = (s16)((s16)_s32_div_f(step * i, count - 1) + (s16)((s32) * (u16*)((u8*)a + 0x14) - step / 2));
        r = func_ov011_021282b8(*(s32*)((u8*)a + 0x00), *(s32*)((u8*)a + 0x04), *(s32*)((u8*)a + 0x08), *(s32*)((u8*)a + 0x0C),
                                v, *(s32*)((u8*)a + 0x10), *(s16*)((u8*)a + 0x16));
        *(s32*)((u8*)p + 0x0C + i * 4) = r;
    }
    for (; i < 5; i++) {
        *(s32*)((u8*)p + 0x0C + i * 4) = -1;
    }
    *(s32*)((u8*)p + 0x04) = (func_ov003_020c37f8((void*)((u8*)*(u32*)a + 0x84)) != 0) ? 1 : 0;
    *(u32*)((u8*)p + 0x00) = *(u32*)a;
    return 1;
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
    s32   vx;
    s32   vy;
    s32   i;
    s32   a;
    s32   b;
    void* p;

    if (func_ov003_020843b0((s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31), *(s32*)((u8*)data + 0x230)) > 0) {
        vx = _ffix(_fadd(
            _fflt(func_ov003_020843b0((s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31), *(s32*)((u8*)data + 0x230)) << 12),
            0.5f));
    } else {
        vx = _ffix(_fsub(
            _fflt(func_ov003_020843b0((s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31), *(s32*)((u8*)data + 0x230)) << 12),
            0.5f));
    }
    if (func_ov003_020843ec((s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31), *(s32*)((u8*)data + 0x234),
                            *(s32*)((u8*)data + 0x238)) > 0)
    {
        vy = _ffix(_fadd(_fflt(func_ov003_020843ec((s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31),
                                                   *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238))
                               << 12),
                         0.5f));
    } else {
        vy = _ffix(_fsub(_fflt(func_ov003_020843ec((s32)((u32)(*(u8*)((u8*)data + 0x24C) << 30) >> 31),
                                                   *(s32*)((u8*)data + 0x234), *(s32*)((u8*)data + 0x238))
                               << 12),
                         0.5f));
    }
    a = 0;
    b = 0;
    p = (void*)((u8*)data + 4);
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
            func_ov003_02082724((CombatSprite*)p, (b * 16) >> 16, (a * 16) >> 16);
            func_ov003_0208260c(p, i, i, *(s32*)((u8*)p + 0x78), i);
            func_ov003_02082b64((CombatSprite*)p);
        }
        p = (void*)((u8*)p + 0x88);
    }
    return 1;
}
