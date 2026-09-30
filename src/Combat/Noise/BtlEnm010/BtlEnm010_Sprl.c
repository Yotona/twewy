#include "Combat/Core/Combat.h"
#include "Combat/Noise/Private/BtlEnm010.h"

#include <nitro/mi/cpumem.h>

typedef struct BtlEnm010Sprl {
    /* 0x00 */ BtlEnm010Owner* unk_000;
    /* 0x04 */ CombatSprite    sprite;
    /* 0x64 */ s16             unk_064;    // launch SE played
    /* 0x66 */ u8              unk_066[0x70 - 0x66];
    /* 0x70 */ s32             unk_070[5]; // x of each link, head first
    /* 0x84 */ s32             unk_084[5]; // y of each link
    /* 0x98 */ s32             unk_098;    // z
    /* 0x9C */ s32             unk_09C;    // spiral centre
    /* 0xA0 */ s32             unk_0A0;
    /* 0xA4 */ s32             unk_0A4;    // radius
    /* 0xA8 */ s32             unk_0A8;    // radius step
    /* 0xAC */ u16             unk_0AC;    // angle
    /* 0xAE */ u16             unk_0AE;    // angle step
    /* 0xB0 */ u16             unk_0B0[4]; // copy of the owner's actor.unk_04..unk_0A
} BtlEnm010Sprl;                           // Size: 0xB8

extern char data_ov011_0212cc54[20];

// MARK: Declarations and notes, hoisted above the definitions so every
// function can be emitted in address order (the linker places functions
// in source order).  Order within this block is unchanged.

/// A 0xC-byte record out of the table `func_ov003_0208a114` walks (`base + index * 12`) -- the
/// same table `BtlEnm006_UG.c` names `Enm006CmdTbl`. Only the bytes below are identified: callers
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

/// `func_ov003_0208a114` -- one argument; returns a pointer to the 0xC-byte record at
/// `base + index * 12` (`mla r0, 0xc, r0, table; bx lr`). The `u16` parameter is load-bearing:
/// call sites truncate a computed selector with `lsl #0x10 / lsr #0x10`.
extern const BtlEnm010CmdTbl* func_ov003_0208a114(u16 idx);

/// `func_ov003_0208a164` -- five arguments (r0-r3 plus one stack word), returning 1 on success.
extern s32 func_ov003_0208a164(BtlEnm010CmdTbl* rec, void* p, s32 x, s32 y, s32 z);

/// `func_ov003_020cbcb4` -- writes a polar angle/radius pair into two x/y out-arrays; five
/// arguments, the fifth on the caller's stack.
extern void func_ov003_020cbcb4(s32* outX, s32* outY, u16 angle, s32 radius, s32 scale);

extern s32 FX_Divide(s32 numer, s32 denom);
s32        func_ov011_02128758(TaskPool* arg0, Task* arg1, void* arg2, s32 index);
s32        func_ov011_021287b8(BtlEnm010Sprl* data, BtlEnm010Owner** arg1);
s32        func_ov011_02128b80(BtlEnm010Sprl* data);
s32        func_ov011_02128c30(BtlEnm010Sprl* p);

/// `func_ov003_020c37f8` -- reads the two-bit field at offset 0 of its argument and returns
/// whether it is 1. One argument.
extern s32 func_ov003_020c37f8(void* p);

/// `func_ov003_020c3c28` -- no arguments; returns a global mode bit.
extern s32 func_ov003_020c3c28(void);

/// `CombatSprite_Update` (`func_ov003_02082b0c`) -- one argument, a `CombatSprite*`; ticks a
/// palette timer behind the sprite's `flags46` bit 12. Declared in `Combat/Core/CombatSprite.h`.

/// `func_ov003_020843b0` -- two arguments; turns a 4.12 y coordinate into a sound pan value.
extern s16 func_ov003_020843b0(s32 a, s32 b);

/// `func_ov003_02084348` -- six arguments: `mode`, two `s16*` out-parameters, then a raw
/// x/y/z triple. The two out-parameters are written through the pointers and read back as
/// `ldrsh`, so they are `s16`, not `s32`.
extern s32 func_ov003_02084348(s32 a, s16* b, s16* c, s32 d, s32 e, s32 f);

extern s32 func_ov011_021287b8(BtlEnm010Sprl* data, BtlEnm010Owner** arg1);
extern s32 func_ov011_021288c8(BtlEnm010Sprl* p);
extern s32 func_ov011_02128b80(BtlEnm010Sprl* data);
extern s32 func_ov011_02128c30(BtlEnm010Sprl* p);
extern s32 func_ov003_02084348(s32 a0, s16* a1, s16* a2, s32 a3, s32 a4, s32 a5);
extern s32 func_ov003_020c3c28(void);
extern s32 func_ov003_020cba2c(s32 a0, s32 a1, s32 a2, s32 a3);

const TaskHandle data_ov011_0212c210 = {(const char*)data_ov011_0212cc54, func_ov011_02128758, 184};

const s32 data_ov011_0212c21c[5] = {4096, 3584, 3072, 2560, 2048};

char data_ov011_0212cc54[20] = {0x54, 0x73, 0x6B, 0x5F, 0x42, 0x74, 0x6C, 0x45, 0x6E, 0x6D,
                                0x30, 0x31, 0x30, 0x5F, 0x53, 0x70, 0x72, 0x6C, 0x00, 0x00};

/// Spawns the Sprl task. As in `func_ov011_02127c84`, the last argument is the *address of the
/// data pointer* -- the prologue's `str r0, [sp, #0x8]` and the `str r1, [sp, #0x4]` that stores
/// `sp + 8` into the outgoing slot only make sense if the caller gets a pointer to a pointer.
/// It has to be a **local** copy: address-taken on the parameter itself makes MWCC push r0-r3
/// and the function comes out 8 bytes long.
s32 func_ov011_02128718(void* p) {
    void* arg0;

    arg0 = p;
    return EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &data_ov011_0212c210, 0, 0, 0, (void*)&arg0);
}

/// `Tsk_BtlEnm010_Sprl`'s task entry, third of the three identical four-way dispatchers
/// (`0x0212801c`, `0x02128348`, `0x02128758`).
s32 func_ov011_02128758(TaskPool* arg0, Task* arg1, void* arg2, s32 index) {
    void* p;
    s32   r;

    p = arg1->data;
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
/// re-read through `*arg1` at every use rather than cached in a local -- the original
/// loads it five times and caching it costs those loads. The five-way fill writes the base
/// position to `unk_070[i]` and the base height to `unk_084[i]`, interleaved.
///
/// The variant selector is the owner's `Sprite.bits_0_1` bitfield: the reference reads the word
/// and extracts with `lsl #0x1e / lsr #0x1e`, and a hand-written shift pair folds to `and #0x3`.
///
/// Open (51 bytes): the `+0x24`/arm block runs one register higher than the reference (chain in
/// r1/v in r2 vs r0/r1) with the same live-value count -- the scratch rotation, not a
/// declaration. Scoping `i` did not move it.
s32 func_ov011_021287b8(BtlEnm010Sprl* data, BtlEnm010Owner** arg1) {
    s32 v;

    MI_CpuSet(data, 0, 0xB8);
    data->unk_000 = *arg1;
    func_ov011_021258b4((*arg1)->sprite.sprite.bits_0_1, &data->sprite, 2);
    CombatSprite_SetAnimFromTable(&data->sprite, 0, 0);
    if ((*arg1)->actor.isFlipped == 0) {
        data->unk_0AC = 0x8000;
        data->unk_0AE = 0xFD00;
        v             = 0x8000 - 0x50000;
    } else {
        data->unk_0AC = 0;
        data->unk_0AE = 0x300;
        v             = 0x48000;
    }
    data->unk_09C = (*arg1)->actor.position.x + v;
    data->unk_0A0 = (*arg1)->actor.position.y;
    for (s32 i = 0; i < 5; i++) {
        data->unk_070[i] = data->unk_09C;
        data->unk_084[i] = data->unk_0A0;
    }
    data->unk_098    = 0 - 0x10000;
    data->unk_0A4    = 0x8000;
    data->unk_0A8    = 0x800;
    data->unk_0B0[0] = (*arg1)->actor.unk_04;
    data->unk_0B0[1] = (*arg1)->actor.unk_06;
    data->unk_0B0[2] = (*arg1)->actor.unk_08;
    data->unk_0B0[3] = (*arg1)->actor.unk_0A;
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
s32 func_ov011_021288c8(BtlEnm010Sprl* p) {
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
    if (p->unk_000 != NULL && (p->unk_000->actor.flags & 4) != 0) {
        return 0;
    }
    p->unk_0A4 = p->unk_0A4 + p->unk_0A8;
    p->unk_0AC = p->unk_0AC + p->unk_0AE;
    func_ov003_020cbcb4(p->unk_070, p->unk_084, p->unk_0AC, p->unk_0A4, 0x800);
    p->unk_070[0] = p->unk_070[0] + p->unk_09C;
    p->unk_084[0] = p->unk_084[0] + p->unk_0A0;
    if (p->unk_064 == 0) {
        p->unk_064 = p->unk_064 + 1;
        if (func_ov003_020c37f8(&p->sprite) != 0) {
            pan = func_ov003_020843b0(1, p->unk_000->actor.position.x);
        } else {
            pan = func_ov003_020843b0(0, p->unk_000->actor.position.x);
        }
        func_ov003_02087f00(0x1DE, pan);
    }
    for (i = 1; i < 5; i++) {
        dx   = p->unk_070[i] - p->unk_070[i - 1];
        dy   = p->unk_084[i] - p->unk_084[i - 1];
        dist = func_ov003_020cba2c(0, 0, dx, dy);
        half = ((data_ov011_0212c21c[i - 1] + data_ov011_0212c21c[i]) * 16) >> 1;
        if (dist < (((data_ov011_0212c21c[i - 1] + data_ov011_0212c21c[i]) * 16) >> 1)) {
            continue;
        }
        p->unk_070[i] = p->unk_070[i - 1] + FX_Divide((s32)(((long long)dx * (long long)half + 0x800) >> 12), dist);
        p->unk_084[i] = p->unk_084[i - 1] + FX_Divide((s32)(((long long)dy * (long long)half + 0x800) >> 12), dist);
    }
    for (i = 0; i < 5; i++) {
        t         = (data_ov011_0212c21c[i] * 16) >> 12;
        v         = (s16)(t >> 1);
        fx        = *func_ov003_0208a114(0x57);
        fx.unk_05 = (u8)v;
        fx.unk_06 = (u8)v;
        fx.unk_07 = (u8)v;
        if (func_ov003_0208a164(&fx, p->unk_0B0, p->unk_070[i], p->unk_084[i], p->unk_098) == 1) {
            s32 pan;
            if (func_ov003_020c37f8(&p->sprite) != 0) {
                pan = func_ov003_020843b0(1, p->unk_070[i]);
            } else {
                pan = func_ov003_020843b0(0, p->unk_070[i]);
            }
            func_ov003_02087f00(0x1DA, pan);
        }
    }
    if (p->unk_0A4 > data_ov003_020e71b8->unk3D7C0[0].unk_0C) {
        return 0;
    }
    CombatSprite_Update(&p->sprite);
    return 1;
}

/// Sprl's phase-2 handler: five passes, each one projecting, positioning and then animating.
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
        CombatSprite_SetPosition(&data->sprite, v0, v1);
        func_ov003_02082730(&data->sprite, 0x7FFFFFFF - data->unk_084[i]);
        CombatSprite_SetAffineTransform(&data->sprite, z, data_ov011_0212c21c[i], data_ov011_0212c21c[i], z);
        CombatSprite_Render(&data->sprite);
    }
    return 1;
}

/// Ticks the sprite at `+0x04` and nothing else. Second of the two identical 20-byte tickers
/// (`0x02128704` was the first).
s32 func_ov011_02128c30(BtlEnm010Sprl* p) {
    CombatSprite_Release(&p->sprite);
    return 1;
}
