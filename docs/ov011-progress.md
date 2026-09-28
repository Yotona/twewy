# ov011 — `BtlEnm010` (in progress)

## Identity

`ov011` is **`BtlEnm010`**, a Noise (`Apl_Suy`) enemy. Identified from the `.data` string blob at
`0x0212c460`, which holds `Apl_Suy/Grp_BtlEnm010_*.bin` — `MotMove`, `MotTurn`, `EffShot`,
`EffRange`, and several letter-suffixed variants.

Same family as the four already-finished overlays: `BtlEnm006` (ov010), `BtlEnm014` (ov012),
`BtlEnm015` (ov013), `BtlEnm026` (ov015). Those are all byte-identical to the original ROM, so
their C, headers and task-data structs are the reference for style and idiom.

## Layout

| region | start | end | size |
|--------|-------|-----|------|
| `.text` | `0x021256c0` | `0x0212bdd8` | `0x6718` (26,872 B) |
| `.rodata` | `0x0212bdd8` | `0x0212c454` | `0x67C` (1,660 B) |
| `.ctor` | `0x0212c454` | `0x0212c454` | empty |
| `.data` | `0x0212c460` | `0x0212cca0` | `0x840` (2,112 B) |
| `.bss` | `0x0212cca0` | `0x0212ccc0` | `0x20` (32 B) |

**114 functions** in `.text`. This is the largest overlay attempted so far — roughly 1.8x
ov010's 14,664 bytes of `.text`.

## Worktree

`E:\Git\twewy-ov011`, branch `decomp-ov011`, branched off `decomp-ov010` (the most mature branch).

`extract/` is a junction to `E:\Git\twewy\extract` — the **parent** directory, not `extract\usa`.
`objdiff-cli.exe` is an untracked, gitignored binary copied in from `E:\Git\twewy-ov010`.
`build/` is gitignored, so everything in `build/scratch/` is untracked by design; copy the
tooling from ov010's worktree rather than expecting `git` to carry it.

## Blocker — objdiff is not pairing functions yet

The build compiles and `objdiff-cli` runs, but **function-level scoring does not work yet**, so
progress cannot be measured. Symptom, measured on `-end 0x02125750` (the two implemented
functions, 0x90 bytes):

```
.text 100   <- section-level only, NOT a content match
.rodata 0
.data 0
--- ov.ps1 then dies: "Attempted to divide by zero" ($syms.Count == 0)
```

From `build/ov_diff.json`:

- `left` (original) symbols: 153 `SYMBOL_OBJECT`, 5 `SYMBOL_SECTION`, 17 `kind: null` — and
  **zero `SYMBOL_FUNCTION`**.
- `right` (ours): `func_ov011_021256c0` and `func_ov011_02125714` are both present but have
  `match_percent: null`, i.e. **unpaired**.

What is known about the mechanism:

- `objdiff.json` has a correct unit for the TU: `base_dir = "build/usa/delinks"`,
  `build_base = true`, so the **delinks** object is objdiff's *base* (the original) and
  `build/usa/src/.../BtlEnm010.o` (compiled from our `.c`) is the *target*.
- The delinks object for an as-yet-unimplemented TU is generated from the original overlay image
  and therefore carries only data objects — no function symbols. That matches the symptom.
- Copying `arm9_ov011.bin` (the original overlay image, 30,176 B, which *does* exist in the main
  checkout at `E:\Git\twewy\build\usa\build\`) into `build/usa/build/` did **not** fix it. So the
  bin alone is not sufficient, or the delinks object needs a symbol source it is not getting.
- ov010 works, so the mechanism exists and is reachable. The question is what makes ov010's
  delinks base object carry 71 function symbols.

**Unresolved: how ov010's base object gets its function symbols, and what ov011 must replicate.**
Read `build.ninja` for the `delinks` rule that produces
`build/usa/delinks/src/Combat/Noise/BtlEnm010/BtlEnm010.o`, and diff that rule against ov010's
`build.ninja`. Also check `tools/configure.py` for how the unit and the delinks object are
generated, and whether any per-overlay input (`build/usa/asm/ov011_4.s`, the overlay's
`symbols.txt`) has to be regenerated first.

Note `build/usa/asm/ov011_4.s` **is** present in the main checkout and looks complete, with
`arm_func_start`/`arm_func_end` around all 114 functions.

## Tooling (in `build/scratch/`, untracked)

Copied from ov010 and re-pointed at ov011. **Use `ovm.ps1`, not `ov.ps1`, when the change does not
edit the `.c`** (header- or signature-only edits leave a stale object and manufacture a false
regression — this bit the ov010 audit once).

| file | purpose |
|------|---------|
| `asm.py` | dump reference disassembly for named functions. `python -u build\scratch\asm.py func_ov011_021256c0` |
| `pd.py` | per-instruction diff, `L=original`, `R=ours` |
| `ov.ps1` | iterate: patch the delinks `.text end`, build, diff, print. `.\build\scratch\ov.ps1 -end 0x...` |
| `ovm.ps1` | as `ov.ps1`, but wipes both objects + both JSONs first |
| `romcmp.py` | ROM byte comparison against `E:\Git\twewy\build\usa\twewy_usa.nds` |
| `audit003.py` | derive `func_ov003_*` arity from `ov003_4.s` and compare to declarations |
| `AGENT_BRIEF.md` | copied from ov010 — struct layouts, ~30 verified codegen recipes, the `symbols.txt` rules, the `-end` trap, and the one-writer-per-worktree rule. **Read this first.** |

**Always measure with `-end 0x0212bdd8`.** Anything smaller truncates the range and makes the
average look better than it is.

## Done

- `func_ov011_021256c0` — spawns the overlay's single task, stashes the handle in
  `data_ov011_0212cca0` (`.bss`).
- `func_ov011_02125714` — looks the task back up and sets bit 4 of its flags word. Predicated
  (`ldrhne`/`orrne`/`strhne`), not branched.

Neither is verified byte-exact yet, because scoring is blocked. Both look right by inspection.

`ENM010_POOL` in `BtlEnm010.c` is a local `#define` for
`(TaskPool*)((u32)data_ov003_020e71b8 + 0x118 + 0x10000)`. `data_ov003_020e71b8` is already
declared in `Combat/Core/Combat.h` as an `Ov003Global*` — do not redeclare it.

## Next

1. Unblock objdiff function pairing (above). Everything else is blocked on this.
2. De-decompile the remaining 112 functions. `.text`/`.rodata`/`.data` must reach 100% and
   `romcmp.py` must report 0 differing bytes.
3. Watch for the task-data struct(s) — expect one main `BtlEnm010` struct plus a sprite block,
   as in ov010. Getting a struct's size wrong shifts every offset in every function, so
   establish it early and verify it against several functions before trusting it.
