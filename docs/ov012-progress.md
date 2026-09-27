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

**33 of 40 functions are exact (100%), average 99.67%; `.text` section 99.64%.**
The `.text` delink covers the full range, so any edit affects all 40 functions at once.

| Function | % | Notes |
|----------|---|-------|
| `func_ov012_02126c74` | 91.8 | atan2 chase vector; register-colour cascade (see below) |
| `func_ov012_021256d0` | 98.3 | byte-identical; `.word` literal + branch-dest reloc accounting |
| `func_ov012_021256c0` | 98.8 | byte-identical; `.word` literal reloc accounting |
| `func_ov012_021256dc` | 99.0 | byte-identical; `.word` literal reloc accounting |
| `func_ov012_02125c48` | 99.9 | byte-identical; `.word data_ov012_0212794c` reloc accounting |
| `func_ov012_02126a1c` | 99.6 | byte-identical; forward intra-TU `bl` shown as `branch_dest` |
| `func_ov012_02127134` | 99.2 | one `cmp/cmpne` pair; size is 0x248 vs the original's 0x244 |

The first four are objdiff **relocation-index artifacts**: both sides render
`.word data_ov012_02127894` identically, but the two objects have different
`R_ARM_ABS32` target symbol *indices*, and this objdiff build reports that as
`DIFF_ARG_MISMATCH`. Nothing to fix in the source.

## Key codegen recipes discovered (MWCC 2.0 sp1p5, -O4,p)

Beyond the shared ov013/ov015 notes (see `docs/ov013-progress.md`, `docs/ov015-progress.md`,
`docs/decomp-tricks.md`):

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
  (`func_ov012_02126c74`, 91.8% → jumped from 82.9% to 91.8%).
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
- **`.rodata` (12%) and `.data` (82%) do not match.** MWCC places the non-`const` variant
  records, `BinIdentifier`s, anim tables and the `u16` index table in `.data`, while the
  original keeps them in `.rodata`; the intra-section order also differs. This is the
  same situation as ov013/ov015 and does not affect the code metrics. Fixing it would need
  `const` on the tables plus a different declaration order.
- `ninja sha1` fails on exactly 4 bytes of NDS header metadata
  (`0x6C/0x6D` arm9 overlay-table length, `0x15E/0x15F` overlay-table offset) because
  ov012 is *not* marked `complete` in `delinks.txt` and the ROM still links the delinked
  original. `ninja check_modules` and `ninja check_symbols` both pass. Expected; see the
  same note in `docs/ov015-progress.md`.
- Overlay TUs are only linked from source when marked `complete` in `delinks.txt`; this
  one is not, so a passing `sha1` would not have validated the source anyway.

## Remaining work

- `func_ov012_02126c74` (91.8%): the instruction sequence now matches, but MWCC allocates
  `idx * 2` to `r1` where the original uses `lr`, which cascades into the `|sin|`,
  `(absin + 0x1000)` and table-base registers and moves the second `ldrsh data_0205e4e0`
  earlier. Tried and rejected: declaration reordering, inlining the second table load,
  folding `* 2` into the `idx` assignment, and hoisting the `cos` load. Needs a source
  form that makes MWCC pick `lr` for the first shift result.
- `func_ov012_02127134` (99.2%): the original's `cmp / ldrne / cmpne / beq` chain is one
  instruction shorter than MWCC's `cmp / beq / ldr / cmp / beq`; the `&&` in an `if`/`else`
  and the `goto` form both produce the longer form.
