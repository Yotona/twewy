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
extern s32 func_ov003_020843b0(s32 a, s32 b);

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
        func_ov003_02087f00((SndMgrSeIdx)0x1E5, (s32(*)(s32, s32))func_ov003_020843b0(0, data->copy.unk_28));
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
