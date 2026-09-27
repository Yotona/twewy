# Overlay 12 (`Tsk_BtlEnm014_*`) decompilation — progress handoff

**Date:** 2026-09-27
**Branch:** `decomp-ov012` (off `decomp-ov015`)
**Worktree:** `E:\Git\twewy-ov012` (`extract/` is a junction to `E:\Git\twewy-ov013\extract`)
**Source:** `src/Combat/Noise/BtlEnm014/BtlEnm014.c`
**Header:** `include/Combat/Noise/Private/BtlEnm014.h`
**Config:** `config/usa/arm9/overlays/ov012/{delinks,symbols}.txt`

## Overlay summary

Overlay 12 is the enemy-014 (Apl_Hor "Noise" flier) combat overlay. It provides three
EasyTask handlers:

| Task | Handler | Data size |
|------|---------|-----------|
| `Tsk_BtlEnm014_Eff` | `func_ov012_02125ab4` | 0x7C |
| `Tsk_BtlEnm014_RG`  | `func_ov012_02126968` | 0x1E8 |
| `Tsk_BtlEnm014_UG`  | `func_ov012_021277b0` | 0x1F0 |

40 functions, `.text` = `0x021256c0`–`0x0212783c` (0x217C bytes).
Original disassembly: `E:\Git\twewy\build\usa\asm\ov012_3.s` (2529 lines).

## Workflow

From `E:\Git\twewy-ov012`:

```powershell
.\build\scratch\ov.ps1 -end 0x0212783c     # patch delink prefix, rebuild, report
python build\scratch\d.py func_ov012_02125de4   # per-function instruction diff
python build\scratch\disfn.py func_ov012_02125de4   # disassemble our .o
```

`build/scratch/` is gitignored; recreate `ov.ps1`/`d.py`/`disfn.py` if lost (they are
tiny wrappers around `ninja`, `objdiff-cli.exe` and `build/ov_diff.json`).

Parsing notes for `build/ov_diff.json`: `left.symbols[]` with `kind == "SYMBOL_FUNCTION"`
carry `match_percent`; `left` = original, `right` = ours. Diff entries are
`{"diff_kind": ..., "instruction": {...}}`; a *matched* instruction is `{"instruction": {...}}`
with no `diff_kind` — test `is_diff = "diff_kind" in entry`, not `"instruction" in entry`.

## Status (objdiff)

**37 of 40 functions are exact (100%), average 99.77%; `.text` 99.65%, `.rodata` 100%,
`.data` 100%.** The `.text` delink covers the full range, so any edit affects all 40
functions at once.

| Function | % | Notes |
|----------|---|-------|
| `func_ov012_02126c74` | 91.8 | atan2 chase vector; register-colour cascade (see below) |
| `func_ov012_02127134` | 99.2 | one `beq` slot: ours 0x248 vs the original's 0x244 |
| `func_ov012_02126a1c` | 99.6 | byte-identical; forward intra-TU `bl` reloc-index accounting |

`func_ov012_02126a1c` is byte-identical to the original: both `bl` targets decode
correctly against `build/usa/build/arm9_ov012.bin` (`EB00002B` → `0x02126ad8`,
`EB000003` → `0x02126a4c`). objdiff reports `DIFF_ARG_MISMATCH` only because the two
objects give the same target symbols different `R_ARM_PC24` symbol *indices*. Nothing
to fix in the source.

## Data-section matching (the big win: +4 functions, `.rodata` 12% → 100%)

`.rodata` and `.data` used to sit at 12% / 82% and were written off as a known
overlay-wide gap. They are fully fixable, and the fix is the same one-liner that
`src/Combat/Friend/Shiki/BtlArm_Doll.c` (a `complete` unit) already uses:

- **Every read-only table must be `const`.** The variant records, the
  `SpriteAnimEntry` tables, the `BinIdentifier`s, the `TaskHandle`s and the `s32`
  score table all belong in `.rodata`. Without `const` MWCC puts them in `.data`
  and the section layout collapses. Marking them `const` took `.rodata` from 12%
  to 94% *and* fixed four functions whose only defect was a literal reloc.
- **The `char[]` name strings must stay non-const** so they land in `.data`, where
  the original has them. Adding `const` to them pushes `.data` to 0%.
- **Give the `char[]` strings their padded sizes explicitly** (`[28]`, `[20]`,
  `[24]`, ...). MWCC otherwise packs them at their natural length and the
  inter-object padding bytes differ.
- **A string literal can be split across two objects.** The original's
  `Tsk_BtlEnm014_RG` name is a 4-byte `"Tsk_"` object at `0x02127a14` plus a
  16-byte remainder at `0x02127a18` (`symbols.txt` marks the latter `ambiguous`).
  Declaring it as two adjacent arrays reproduces both extents and the byte image is
  identical, because the `TaskHandle` still points at `0x02127a14`. `.data` 86% → 100%.

This should be applied to ov013 and ov015 too — both have the same
`BinIdentifier` / `SpriteAnimEntry` / name-string shape and the same 12%/82% symptom,
and it is a prerequisite for ever marking a Noise overlay `complete`.

## Key codegen recipes discovered (MWCC 2.0 sp1p5, -O4,p)

Beyond the shared ov013/ov015 notes (see `docs/ov013-progress.md`, `docs/ov015-progress.md`,
`docs/decomp-tricks.md`):

- **MWCC will not if-convert `A && B` into `ldrne`/`cmpne` here.** In
  `func_ov012_02127134` the original emits `cmp r0,#0 / ldrne r0,[r5,#0x1d0] /
  cmpne r0,#1 / beq` while every source spelling we tried emits
  `cmp r0,#0 / beq / ldr / cmp / beq` — one word longer, which shifts all three
  `ldr [pc, #imm]` literal offsets by 4 and costs the whole function. Fourteen
  shapes were tried (`&&`, nested `if`, De Morgan, the call result in a local, the
  condition in a local, the field in a local, the field through a pointer, bitwise
  `&`, truthiness, `if(1){}`, a plain `return` body, and two label orders); all give
  99.22% or worse. The rest of the 580-byte function is byte-identical, so this one
  slot is the entire remaining gap.
- **The second `sin`/`cos` table read must be hoisted, not inlined at its use site.**
  In `func_ov012_02126c74` the original keeps `idx * 2` in `lr` across the magnitude
  multiply and re-loads the table base from the literal pool afterwards; reading
  `data_0205e4e0[idx * 2]` inline at the second `Mth_MulFixed` instead makes MWCC
  schedule both `ldrsh`s together up front (82.9% vs 91.8%). Nine variants tried;
  hoisting both reads into locals is the best.

- **`if (cond) { A } if (!cond) { B } goto tail;`** produces the original's
  `ands ip, rX, #1 / bne <second> / <first> / cmp ip, #0 / beq <miss> / <second>` shape
  **including the redundant re-test of the flag value**. Writing it as
  `if (cond) {...} else if (...) {...}` collapses it to `tst` + a single branch and
  loses the reuse. Needed for `func_ov012_02125858` and `func_ov012_02126c1c`.
  Conversely, the *branch polarity* of the first test decides which arm is emitted
  first: `if (bits == 0) {…} else {…}` yields `bne` to the second block, and
  `boss = A; if (cond) boss = B;` yields `beq` to the second block.
- **Common tail via `goto`** is required when the original shares one block between
  two arms. `func_ov012_02125858` has a single `unk_62 = 50` tail reached by `blt`
  from the first arm and by fall-through from the second; the if/else form duplicates it.
- **`if (!(A || B || C || D)) { body; return 1; } return 0;`** puts the single `return 0`
  block *after* the body, so all four tests branch forward to it — matching
  `func_ov012_02126a4c` / `func_ov012_02126ad8`. The plain `if (A||B||C||D) return 0;`
  form puts the return-0 block *before* the body and the last test is inverted (`bgt`
  instead of `ble`).
- **Values that must survive a call need a named local.** `func_ov012_02125de4` case 2/3
  keep `boss->position.x` and `data->position.x` live across the close-in branch
  (`sub r0, r0, r1` reuses both registers instead of reloading). Inlining the two
  expressions makes MWCC re-materialise the loads.
- **`Mth_MulFixed(x, 0x800)` strength-reduces to `lsl #0xb`** and
  `Mth_MulFixed(x, 0x2000)` to `lsl #0xd`; the shift amount identifies the constant.
  The atan2 chase magnitude is `Mth_MulFixed(ABS(sin) + 0x1000, 0x800)` (not `0x2000` as
  in ov015 — the two overlays scale the same table differently).
- **Loading the second table entry before computing the magnitude** lets MWCC
  interleave the two `smull`s and match the original's schedule
  (`func_ov012_02126c74`; superseded by the note above, which gives the full picture).
- **Two statements beat one expression for scheduling.** `func_ov012_021265a4` needs
  `hp100 = currentHp * 0x64;` then `pct = hp100 / maxHp;` so the `mov r5, #0x14`
  default lands between the `smulbb` and the `bl _s32_div_f`.
- **`func_ov003_020cd11c` takes an `s32`**, `func_ov003_0208a114` takes a `u16`. When both
  are called with the same local, MWCC keeps the wide value in a callee-saved register
  and materialises the truncated copy in `r0` for the second call (`lsl/lsr #0x10`).
- **The flip ternary order matters**: `(flags46 & 1) ? d : -d` emits `rsbeq`
  (negative arm second). The negation must be written *before* the
  `func_ov003_0208a114` call in the source, otherwise the call is scheduled first.
- **A field can be read at two widths.** `BtlEnm014.unk_1E0` is manipulated as two
  `s16` halves by the attack timers (`ldrh`/`strh` at `0x1E0`/`0x1E2`) and as one
  `s32` by the chase handlers (`ldr`/`str` at `0x1E0`). The header models it as a
  union (`Enm014Chase { s32 word; struct { s16 lo, hi; } half; }`).
- **`(u16)(v + 0xFFFE) <= 1`** reproduces the `add #0xfe / add #0xff00 / lsl #0x10 /
  lsr #0x10 / cmp #1 / bhi` variant test. `(u8)(v - 2)` would mask with `0xFF` instead.
- **Sub-word stores are not store-to-load-forwarded** — the `ldrh`/`ldrsh` reloads after
  every `strh` in this overlay are required, not redundant.
- **Jump-table bodies are emitted in source order**, so when the original's body order
  is not 0,1,2,… the `case` labels have to be written in the original's order.
  `func_ov012_0212768c` emits bodies in the order 1, 3, 4, 5, 2, 6 — the source lists the
  `case`s as 0, 1, 3, 4, 5, 2, 6 to reproduce it.
- **`case 0:` falling through into `case 1:`** with no intervening `break` reproduces the
  original's shared tail (`func_ov012_02127378`).
- **A ternary in an argument position is predicated**:
  `func_ov012_02125768(data, (x > N) ? fnA : fnB)` gives `ldrgt`/`ldrle` +
  one `bl`, whereas an if/else gives two call sites.
- Calls to `0x020824a0` must use the ov000 name `Mini108_VBlank`; `0x02082d04`
  `Sprite_Restart`; `0x02082f1c` `CombatActor_SetPendingCommand`; `0x02082750`
  `CombatSprite_SetFlip`; `0x020827c0` `CombatSprite_SetPaletteMode`; `0x020831e4`
  `CombatActor_Render`.

## Symbol resolutions used

- `0x0200ea4c` = `SpriteMgr_IsAnimationFinished`
- `0x020067ec` = `RNG_Next`
- `0x0203b3c0` = `MI_CpuSet`, `0x0200f68c` = `EasyTask_CreateTask`
- `0x02035094` = `FX_Atan2Idx`; `0x0205e4e0` = 12.12 sin/cos table
- `0x02055fe8`/`0x020554c8`/`0x02056264`/`0x02055f74` = `_fflt`/`_fadd`/`_fsub`/`_ffix` are
  reached through `ROUND()`; `0x020566e4` = `_s32_div_f` via `/`
- `0x02071cf0` = `gSaveState`, `0x02071d10` = `gSaveData` (`func_ov012_021258e8` reads
  `gSaveData + 0x3188` and compares it against `0x654`)
- `data_ov003_020e71b8` is `Ov003Global`; `unk3D875` / `unk3D878` are the fields written
  by `func_ov012_021258e8`, `unk3D898` / `unk3D89C` are the UG / RG actor pointers
  (0x3D88E is an unnamed `u16` past `unk3D878`).

## Notes

- The three `Tsk_BtlEnm014_*` symbols in `symbols.txt` name the **`const TaskHandle`
  structs in `.rodata`** (`0x02127934` / `0x02127940` / `0x0212795c`), matching the ov013
  convention. The task-name strings in `.data` (`0x02127a00` / `0x02127a14` / `0x02127a28`)
  keep their `data_ov012_*` names.
- Naming the data objects with the exact lowercase spellings from `symbols.txt` removes
  four objdiff reloc mismatches; uppercase hex costs ~1% on each affected function.
- `.rodata` and `.data` now both match at 100% — see the data-section section above for
  the `const` / padded-size / split-literal recipe. This is the piece ov013 and ov015 are
  still missing.
- `ninja sha1` fails on exactly 4 bytes of NDS header metadata
  (`0x6C/0x6D` arm9 overlay-table length, `0x15E/0x15F` overlay-table offset) because
  ov012 is *not* marked `complete` in `delinks.txt` and the ROM still links the delinked
  original. `ninja check_modules` and `ninja check_symbols` both pass. Expected; see the
  same note in `docs/ov015-progress.md`.
- Overlay TUs are only linked from source when marked `complete` in `delinks.txt`; this
  one is not, so a passing `sha1` would not have validated the source anyway.

## Remaining work

Three functions, all register-allocation or if-conversion decisions that resisted every
source spelling tried. The bytes are verified correct in two of the three cases.

- `func_ov012_02126c74` (91.8%): the instruction *sequence* matches, but MWCC allocates
  `idx * 2` to `r0` where the original uses `lr` (and puts `sin` in `lr` where the
  original uses `r0` — an exact swap), and it keeps the table base live instead of
  re-loading it from the literal pool. Rejected: `idx2` temp, both reads hoisted, both
  reads inline, products via temporaries, pointer local, and no-`idx2` variants
  (9 total; 91.8% is the ceiling, everything else 82.9% or 74.7%).
- `func_ov012_02127134` (99.2%): one 4-byte slot. The original predicates
  (`cmp / ldrne / cmpne / beq`); MWCC branches over the load
  (`cmp / beq / ldr / cmp / beq`). Everything else in the 580-byte function is
  byte-identical. 14 source shapes tried, all 99.2% or worse.
- `func_ov012_02126a1c` (99.6%): byte-identical; objdiff reloc-index artifact only.

## Applying the `const` recipe to ov013 / ov015

This is the highest-value follow-up and should be done before starting another
overlay. Both siblings have the same data shape and the same 12% / 82% symptom:

1. `const` every variant record, `SpriteAnimEntry` table, `BinIdentifier`,
   `TaskHandle` and scalar table in `BtlEnm015.c` / `BtlEnm026.c`.
2. Leave the `char[]` asset-name and task-name strings non-`const`, and give them
   their padded sizes (derive each from the gap to the next symbol in `symbols.txt`).
3. Check whether any name string is split mid-literal in `symbols.txt`; if so, split the
   array the same way.
4. Re-measure — expect several functions to go to 100% as a side effect, since the
   literal-pool relocs get fixed too.

