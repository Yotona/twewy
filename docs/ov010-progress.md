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

**0 of 71 functions exact** — no functions written yet. **`.rodata` 99.2%, `.data` 100%.**

| Section | Bytes | Status |
|---------|-------|--------|
| `.text` | 14,664 | 0 / 71 functions implemented |
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

## Next steps

1. Derive the `BtlEnm006` struct layout from the disassembly (the header currently only
   has `Enm006Variant` and `Enm006Spawn`).
2. Implement the 9 tiny + 6 small functions first; they establish the accessor patterns
   and the struct offsets everything else depends on.
3. Work up through the 29 medium functions.
4. Leave the three >1 KB functions (`02127cc0`, `02125de4`, `02127764`) for last.

Triage the residue with `build/scratch/triage.py` as it grows — see `decomp-tricks` §8.9 for
why that matters.
