# Overlay 10 (`Tsk_BtlEnm006_*`) decompilation - progress handoff

**Branch:** `decomp-ov010` (off `decomp-ov012`)
**Worktree:** `E:\Git\twewy-ov010`
**Source:** `src/Combat/Noise/BtlEnm006/BtlEnm006.c`
**Header:** `include/Combat/Noise/Private/BtlEnm006.h`
**Config:** `config/usa/arm9/overlays/ov010/{delinks,symbols}.txt`
**Reference disassembly:** `E:\Git\twewy\build\usa\asm\ov010_3.s` (read-only)

## Setup notes

- `extract/` must be a junction to `E:\Git\twewy\extract` — the **parent**, not
  `extract\usa`. The build needs both `extract/baserom_twewy_usa.nds` and
  `extract/usa/config.yaml`; pointing at `extract\usa` alone makes `ninja delink` fail.
- `build/scratch/ov.ps1` carries the objdiff staleness guard from ov015: it regenerates
  `objdiff.json`, deletes `build/ov_diff.json` first, and exits non-zero if objdiff did
  not recreate it. A missing unit makes `objdiff-cli` fail *silently*, leaving the
  previous run's JSON in place.
- **The generated build does not list the header as a dependency of the translation unit.**
  A header-only edit leaves a stale `.o` in place, `ninja` reports "no work to do", and every
  measurement silently re-reports the previous numbers. `ov.ps1` now touches the source file
  so ninja always relinks. If you ever build by hand, do the same — this cost a long
  misdiagnosis of a struct-offset bug that turned out not to exist.
- PowerShell's `[System.IO.File]` calls do not follow `cd` in this shell. Use absolute
  paths, or Python, for config edits.

## Identity

`ov010` is **`BtlEnm006`**, a Noise (`Apl_Suy`) enemy with five tasks. Identified from the
`.data` name strings plus the per-frame call triple `func_ov003_02087f00` /
`Mini108_VBlank` / `func_ov003_020843b0` (21 calls each) — the same CombatSprite /
CombatActor shape as the Noise overlays, so every recipe from ov012/013/015 applies.

| Task | Handler | Size |
|------|---------|------|
| `Tsk_BtlEnm006_DeadEff` | `func_ov010_02125730` | 0x6C |
| `Tsk_BtlEnm006_RG`      | `func_ov010_021269d0` | 0x1FC |
| `Tsk_BtlEnm006_Swirl`   | `func_ov010_02126d04` | 0xA0 |
| `Tsk_BtlEnm006_Swlo`    | `func_ov010_02127178` | 0x34 |
| `Tsk_BtlEnm006_UG`      | `func_ov010_02128dbc` | 0x200 |

Assets: `Apl_Suy/Grp_BtlEnm006{,.a,.b}.bin`.

## Status (objdiff)

**43 of 71 functions implemented, 24 exact, average 58.89%; `.rodata` 99.2%, `.data` 100%.**
(`.text` is 22.8% and will stay low until the remaining 28 functions land — the section score
covers the whole declared range, not just what is written.)

The 19 implemented-but-imperfect functions are all known and triaged:

| function | % | mechanism |
|----------|---|-----------|
| `func_ov010_02125730`, `021269d0`, `02126d04`, `02128dbc`, `02127178` | 99.7 | reloc artifact — the jump-table `b` entries carry absolute addresses on the original side and section-relative ones on ours; identical once branch targets are normalised |
| `func_ov010_02125878` | 99.9 | same reloc artifact |
| `func_ov010_02128a08` | 99.6 | one register: the original recycles the dead `arg1` register for the constant, MWCC picks a fresh one |
| `func_ov010_02126934`, `02128d20` | 98.6 | reg-colour: the 0x100 base goes in a fresh `r0` at each use in the original, MWCC keeps it in one register |
| `func_ov010_02126420`, `02125b28` | 98.4–98.6 | reg-colour on the loaded s16 (`r1` vs `r0`) |
| `func_ov010_021283c0`, `02128434`, `0212636c`, `021282b8`, `021263c4` | 87.5–92.2 | reg-colour, five instances of one shape (below) |
| `func_ov010_02126c38`, `021256d0`, `021270a8` | 75.0–86.3 | the task-spawn family (below) |

**Jump-table task handlers cost ~0.25% each, systematically.** Six in a row landed in that
bucket, so expect it for any future `addls pc, pc, rX, lsl #2` handler in this codebase.

**The task-spawn family wants a branch, not a predicate.** `02126c38`, `021256d0` and
`021270a8` all do `pool = lookup(...); if (pool == NULL) { pool = global; if (pool != NULL)
{ pool += 0x8C + 0x8000; } }`. The original emits `ldreq`/`beq` — a real branch over the offset
arithmetic — and MWCC if-converts ours into `cmp / addne / addne`. No C spelling found that
produces the branch; an `if/else` with an empty then-arm made it worse, not better.
`021270a8` additionally stores both parameters to dead home slots (`str r0, [sp, #0xc]`,
`str r1, [sp, #0x10]`) that only appear if their addresses are taken, and nothing reproduces
that. What *does* help this family: the 6th argument is `&local`, and naming the incoming
argument as a local (`void* self = arg0; ... &self`) stops MWCC spilling r0-r3.

**Five functions share one unfixed reg-colour difference.** They all open with

```c
if (data->sprite.unk_C0 == 0) {
    data->sprite.unk_C0++;
    data->unk_1E4 = 0;
    data->unk_1E0 = 0;
    data->unk_1DC = 0;
}
```

and the original computes the increment into a *fresh* register and reuses the register the
loaded halfword was in for the constant zero (`add r2, r1, #1 / mov r1, #0 / strh r2, ...`),
whereas MWCC does the increment in place and puts the zero in `r0`. Naming the loaded value as
an `s16` local, as an `s32` local, hoisting the zero into its own local, and introducing an
`Enm006SpriteBlock*` local were each tried; all left the codegen byte-identical or 1 point
worse. The semantics are right; only the register choice differs.

| Section | Bytes | Status |
|---------|-------|--------|
| `.text` | 14,664 | 43 / 71 functions implemented, 24 exact |
| `.rodata` | 772 | 99.2% |
| `.data` | 224 | 100% |

Function size distribution, which is the plan of attack:

| size | count | note |
|------|-------|------|
| <= 32 B | 9 | trivial accessors |
| 33-64 B | 6 | |
| 65-128 B | 29 | the bulk of the tractable work |
| 129-256 B | 9 | |
| > 256 B | 18 | includes 1528 / 1416 / 1372 B monsters |

So ~44 tractable functions before the large ones — comparable to ov012's 40 total, and a
multi-session job.

## Data sections

Both sections are decoded. The recipe from ov012/013/015 applies, with three instances of
the split-literal pattern:

- `0x02129364` [24] `"Apl_Suy/Grp_BtlEnm006b.b"` + `0x0212937c` [4] `"in\0\0"`. This one
  is split **mid-word**, not on a clean prefix, so the 24-byte half has no terminator and
  needs a **brace initializer** — a 24-character literal does not fit a `char[24]`.
  Getting this right took two attempts; the byte image is `Apl_Suy/Grp_BtlEnm006b.bin\0\0`.
- `0x02129028` is one **40-byte** object, not a 16-byte variant: `symbols.txt` has no entry
  at `0x02129038`, so the auto-tagger folded the 24-byte anim triple into the variant's
  extent. Reproduced with a local `Enm006VariantTail { Enm006Variant variant;
  SpriteAnimEntry anim[3]; }`.
- `0x0212925c` is one **84-byte** object (42 `s16`), not an `s32` plus a separate array.

### Remaining `.rodata` gap — all `symbols.txt` granularity artifacts

Three mismatches, all the same class as ov015's phantom symbols. None is fixable in C
without making the code worse:

| symbol | original | ours | cause |
|--------|----------|------|-------|
| `data_ov010_02129216` | 10 B | 8 B | tagger folded 2 bytes of trailing padding into the anim entry |
| `Tsk_BtlEnm006_Swirl` | 8 B | 12 B | a phantom 4-byte symbol at `0x021292c0` splits the real 12-byte `TaskHandle` |
| `data_ov010_021292c0` | 4 B | absent | the phantom itself; exists only on the original side |

Making `data_ov010_02129216` a 10-byte struct would need `&data_ov010_02129216.e` at the
three use sites (including the `data_ov010_02129380` table), which is uglier than the
~6 bytes it recovers. **Fixing all three properly means correcting `symbols.txt`** — the
same ground-truth decision flagged for ov015, and still the user's call.

## Codegen recipes confirmed on ov010

Beyond `docs/decomp-tricks.md` and the sibling overlays:

- **A conditional-pointer "callback", not a task handler.** A body shaped
  `push {r3, lr} / bl <helper> / mov r0, #1 / pop {r3, pc}` passes its incoming `r0`
  straight to the callee. Typing it `(TaskPool*, Task*, void* args, s32 stage)` makes
  MWCC emit a `mov r0, r2` the original does not have — all six of these scored exactly
  75% until re-typed as single-argument. None of them appears in a `TaskHandle`, which is
  the tell.
- **A pointer local to a sub-struct is the wrong tool; field access is the right one.**
  `func_ov010_02125910` needs `data + 0x100` computed *after* a call. A
  `u16* p = (u8*)data + 0x100;` local gets hoisted above the call and costs an extra
  callee-saved register (72.5% vs 100%). Field access through a struct reproduces it. The
  field offsets must be relative to the sub-struct (`0xC0`, not `0x1C0`) because that
  base+0x100-then-+0xC0 fold is what the original codegen depends on.
- **Shared tails need labels on *every* arm, not just the shared one.** In
  `func_ov010_02128c3c` all three tests branch to one `return 0` and `return 1` is its own
  forward branch. Labelling only the zero arm got 89.8%; the rest was MWCC predicating the
  `return 1` (`movlt r0, #1; bxlt lr`). Both arms needed a `goto`.
- **objdiff renders the original side with real symbol names for helpers.** That is how
  `func_ov003_02082b0c` / `02082cc4` were identified as `CombatSprite_Update` /
  `CombatSprite_Release` — both already declared in `Combat/Core/CombatSprite.h`, so the
  `extern`s were redundant.
- **Dense vs sparse switch.** Four stage handlers use `cmp r3, #3 / addls pc, pc, r3, lsl #2`;
  `Swlo` uses a `cmp/beq` chain because it has no case 2. Matches the ov015 rule.
- **Watch for constant folding that changes the value.** `func_ov010_02127500` stores
  `data + 0xF6 + 0x100`. Written as one expression, MWCC reassociates it to
  `(data + 0x100) + 0x100` — 0xA too high, a real behaviour bug, 98% and no warning. The
  original keeps the two steps, so the source has to too: hoist the `+ 0xF6` into its own
  local *above* the `strh`, and both the two-step arithmetic and the register allocation
  (base in `r1`, loaded value in `r0` — the opposite of the sibling functions) fall out.
  Whenever an expected value *and* the register choice are both wrong, suspect folding before
  you suspect the struct.
- **An outer `else` can be load-bearing for the literal pool.** `func_ov010_02125938` picks one
  of three callbacks. Written `if (f() == 0) { A } else if (rand) { B } else { C }` it scored
  65.6% and emitted the literals in the wrong order. Restructured to
  `if (f() != 0) { if (rand) { B } else { C } } else { A }` — same semantics, `A` moves last,
  the `f()==0` branch jumps forward past both random arms, and the pool order matches. 100%.
- **Struct size vs struct extent.** `Enm006SpriteBlock` looked like 0xC6 bytes but has to be
  0xC8: MWCC rounds the sub-struct up to its 4-byte alignment, and the outer struct then has
  two padding bytes *before* the next `void*`, which is 4-aligned and so eats two more. Add
  the trailing pad explicitly (`padC6[0xC8 - 0xC6]`) so the intent is visible even though it
  does not change the generated size.
- **A bit test on a byte is a shift pair, not `tst`.** `if (x & 1)` gives
  `tst r0, #1 / ldrne / bicne`. The original wants the bit moved into a *value* and then
  predicated on it being zero: `lsl #30 / lsr #31 / ldreq / biceq / streq`. So spell it
  `if (((u32)x << 30) >> 31 == 0)`. The `(u32)` is load-bearing — the compiler is built with
  `-char signed`, so without it the final shift is `asr` rather than `lsr`.
- **`(x - 1) % 4` is the fix for a four-instruction sequence.** `sub / lsr #31 / rsb lsl #30
  / adds ror #30` is MWCC's expansion of a signed `% 4`, and it matched exactly.
- **Two overlapping views of one memory range.** The 0x1E8, 0x1F0 and 0x1F2 halfwords are
  reached as `base+0x100` then `+0xE8`/`+0xF0`, while 0x1DC is reached as a direct outer-struct
  access — two different addressing forms for one struct, so two types are needed
  (`Enm006SpriteAlt`, `Enm006SpriteAlt2`) plus an offset cast for 0x144, which is *inside* the
  sprite block's span and so cannot be an outer field at all. Give each view exactly **one**
  field: a two-field `Enm006SpriteAlt` made MWCC place the second field 0x38 bytes early.
- **Re-derive a sub-struct pointer at each use; do not hoist it.** `func_ov010_02126934` reads
  through the 0x100 view four times. Written with a local `Enm006SpriteAlt* alt`, MWCC gives
  `alt` its own callee-saved register and the function spills (68.9%). Written as a bare
  `(Enm006SpriteAlt*)&data->sprite` at each use it re-derives the base and scores 98.6%.
  Same for `02128a08`, where hoisting turns the original's predicated `addeq r0, r4, #0x100`
  into an unconditional `add r1, r4, #0x100`.
- **An 8-byte record of four halfwords indexes differently from `SpriteAnimEntry`.** The
  animation tables are addressed as `table + variant*8 + phase*2`; reading them through
  `SpriteAnimEntry` folds the two index terms into one shifted add and reorders the
  instructions. Casting to a local `struct { u16 slot[4]; }[3]` restores the original form
  (87.6% → 98.6%).

## Next steps

1. 28 functions remain. The biggest tractable one is `func_ov010_02128820` (0x2E8 bytes) — the
   per-frame body that calls `0212847c` twice, does a `Vec_DotProduct` reflection test and runs
   the `unk_E8 % 10` phase counter. Everything it needs is already named.
2. Then the per-stage workers `02126d54`, `02126e58`, `02126fdc`, `021271c0`, `021272e0`,
   `02125780`, `02126a20`, `02126a94`, `02128e80`, plus `02126c94`, `0212847c`, `02128624`,
   `021286e8`, `02125c80`, `02127550`, `02127110`, `02127488`, `02127650`, `02128b48`,
   `02128bcc`, `02128cbc`, `02128820`.
3. `02126d54`, `02126e58` and `02126fdc` belong to `Tsk_BtlEnm006_Swirl` and index a
   `s32[8]` at `+0x80` of a **0xA0-byte** task data — that is a *different* struct from
   `BtlEnm006` (which is 0x1FC and has a `u16` at 0x80). It needs its own type; the offsets
   seen so far are `unk_00` (the `BtlEnm006*`), `unk_04` (a CombatSprite), `unk_68`/`unk_6C`
   (copied position), `unk_70` (added to each frame), `unk_74` (an accumulator clamped to
   0x4000), `unk_7C`/`unk_7E` (s16 angles), and `unk_80[8]`.
4. Leave the three >1 KB functions (`02127cc0`, `02125de4`, `02127764`) for last.

**Watch the field order when you extend the struct.** One field (`unk_144`, at 0x144 — which is
*past* the sprite block's 0x100..0x1C8 span) was inserted at the wrong place in the declaration
once, and everything from there on silently shifted by 0x38. It cost a long misdiagnosis. If a
whole function's offsets move at once, suspect the declaration order before the disassembly.

Triage the residue with `build/scratch/triage.py` as it grows — see `decomp-tricks` §8.9 for
why that matters.

### Tooling

- `build/scratch/asm.py <func>...` pulls named functions out of the reference disassembly.
  Read its output carefully: `Select-Object -First N` truncated a case body and led to a
  wrong dispatch target that the diff caught immediately.

