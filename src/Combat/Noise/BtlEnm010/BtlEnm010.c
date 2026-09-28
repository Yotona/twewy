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
                func_ov003_02087f00(0x1D6, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_028));
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

extern s32 func_ov003_020c4c9c(void* p);
extern s32 func_ov003_020cb520(void* p, s32 arg1);
extern s32 func_ov003_020cb594(void* p, s32 arg1);

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
        func_ov003_02087f00(0x1E3, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_028));
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

extern s32 func_ov003_020c6230(void);
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
    s32 r;

    switch (data->unk_1C4) {
        case 0:
            if (func_ov003_020c6230() != 0) {
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
            r             = func_ov011_021277c8(data, (s16*)&data->unk_1F8, (s16*)((u8*)data + 0xFA + 0x100), data->unk_1C0);
            data->unk_1C0 = data->unk_1C0 + 1;
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
                func_ov003_02087f00(0x1D3, (s32(*)(s32, s32))func_ov003_020843b0(1, data->unk_028));
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
    if (data->unk_200 == -1 && data->unk_204 != 0) {
        func_ov003_020c4748(data);
        func_ov003_0208810c((void*)((u8*)data + 0xE4), data);
    } else {
        func_ov003_020c4878(data);
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
void func_ov011_02127c4c(BtlEnm010RG* data) {
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

extern s32 func_ov011_02128070(void* p, s32 arg1);
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
            r = func_ov011_02128070(p, arg2);
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
