# ov011 — `BtlEnm010` (in progress)

## Identity

`ov011` is **`BtlEnm010`**, a Noise (`Apl_Suy`) enemy. Identified from the `.data` string blob at
`0x0212c460`, which holds `Apl_Suy/Grp_BtlEnm010_*.bin` — `MotMove`, `MotTurn`, `EffShot`,
`EffRange`, and several letter-suffixed variants.

Same family as the four already-finished overlays: `BtlEnm006` (ov010), `BtlEnm014` (ov012),
`BtlEnm015` (ov013), `BtlEnm026` (ov015). Those are byte-identical to the original ROM, so their
C, headers and task-data structs are the reference for style and idiom — but **not** for struct
offsets (see "The brief was wrong" below).

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

## The brief was wrong — this cost a session

The `build/scratch/AGENT_BRIEF.md` inherited from ov010 was a **byte-for-byte copy of ov010's
brief with `BtlEnm006` → `BtlEnm010` and `ov010` → `ov011` textually substituted**. Reversing
those two substitutions reproduces ov010's file with zero differing lines. So every statement
in it about `BtlEnm010`, `Enm006SpriteBlock`, `Enm006Swirl`, `Tsk_BtlEnm010_Swirl`, the 0x1FC
struct, `unk_1F4`/`unk_1F8`, "71/71 functions complete, average 95.75%", and each
"`func_ov011_0212xxxx` is at 100%" was **ov010's**, and none of it applies here.

It has been rewritten. The MWCC codegen recipes in it are compiler-level and were kept; the
struct offsets, the helper-signature claims that referenced them, and the completion claims were
removed. **Do not reintroduce them from the ov010 copy.**

## Blocker — RESOLVED

The original symptom: objdiff could not pair functions, so progress was unmeasurable. Our two
functions showed up with `match_percent: null`; the left (original) side had 153
`SYMBOL_OBJECT` and zero `SYMBOL_FUNCTION`; `.text` reported 100 (section granularity only) and
`.rodata`/`.data` 0.

Two independent causes, both now fixed:

1. **`config/usa/arm9/overlays/ov011/delinks.txt` had an empty file-level `.text` range** —
   `start:0x021256c0 end:0x021256c0`. dsd's delink only emits function symbols for the part of
   a source file it is asked to delink, so an empty range means **zero function symbols**.
   Declaring the real end (`0x0212bdd8`) restores all 114. This is what the overlay-level
   `.text` line was always for; the per-file line has to be kept in step with it. `ov010`'s
   committed `delinks.txt` has the same two lines with the file-level end equal to the real
   end — that is the shape to match.

2. **`ninja build/usa/delinks/.../BtlEnm010.o` is a no-op.** The delinks objects are not
   declared ninja outputs; the `delink` rule's only declared output is
   `build/usa/delinks/_dsd_gap@main_47.o`, and the rule's command writes every object as a side
   effect. Ninja sees the stale file on disk and stops. `ov.ps1` now runs that edge.

`objdiff.json` and the `delink` rule in `build.ninja` were otherwise identical to ov010's, and
`tools/configure.py` needed no change.

## Tooling (in `build/scratch/`, untracked)

| file | purpose |
|------|---------|
| `asm.py` | dump the reference disassembly for named functions |
| `pd.py` | per-instruction diff from objdiff's JSON, `L=original`, `R=ours` |
| `fbdiff.py` | **the authoritative measurement** — per-function byte diff, relocation-aware |
| `ov.ps1` | build + relink + objdiff + fbdiff. `-end` defaults to the full `0x0212bdd8`; `-wipe` forces a clean rebuild |
| `romcmp.py` | ROM byte comparison against the original |
| `census.py` | global histogram of every `[rN, #imm]` offset the original uses |
| `census2.py` | the same, per named function, or `-c <token>` to find the callers of a symbol |
| `fnlist.py` | the 114 functions in address order with sizes |
| `audit003.py` | derive `func_ov003_*` arity from `ov003_4.s` (from ov010; has false positives) |
| `elf.py` | minimal ELF32 reader (`sections`, `symbols`) |
| `AGENT_BRIEF.md` | rewritten; read it before trusting any struct offset |

### How measurement works now

`fbdiff.py` reads the two relocatable objects directly and does not depend on objdiff's
pairing at all:

- original `build/usa/delinks/src/Combat/Noise/BtlEnm010/BtlEnm010.o`
- ours `build/usa/src/Combat/Noise/BtlEnm010/BtlEnm010.o`
- boundaries from `config/usa/arm9/overlays/ov011/symbols.txt` (114 entries)

Verdicts: `OK` (byte-identical), `RELOCC` (identical once the bits a relocation overwrites are
masked out — the verdict that matters, since every `bl` and every literal-pool slot is a
placeholder in a relocatable object), `DIFF`, `MISSING`. Detail mode prints both sides'
instruction text, taking the original's from `ov011_4.s` and both sides' from objdiff's JSON.

`romcmp.py` anchors on `"Apl_Suy/Grp_BtlEnm010_MotMove"` (RAM `0x0212C470`, file `0x1407B0`):
`file_offset = ram_address - 0x1FECCC0`. Every overlay loads at RAM `0x021256c0`, so the RAM
address does not locate the overlay — ov010 and ov011 share that base and sit at different file
offsets.

**Always measure the full range `-end 0x0212bdd8`.**

## Done

| function | bytes | status |
|----------|-------|--------|
| `func_ov011_021256c0` | 84 | **byte-exact** (modulo the `bl` relocation) |
| `func_ov011_02125714` | 60 | **byte-exact** (modulo the `bl` relocation) |
| `func_ov011_02125750` | 356 | first pass, 64 of 356 bytes differ — not finished |

So 2 of 114 functions are byte-identical. `.text`/`.rodata`/`.data` are all 0 in objdiff terms
and `romcmp.py` has not been run yet (the linked ROM does not exist; `ninja
build/usa/twewy_usa.nds` first).

`ENM010_POOL` in `BtlEnm010.c` is a local `#define` for
`(TaskPool*)((u32)data_ov003_020e71b8 + 0x118 + 0x10000)`. `data_ov003_020e71b8` is already
declared in `Combat/Core/Combat.h` as an `Ov003Global*` — do not redeclare it.

## Structs — partially established, and one question is open

This overlay owns about **eight tasks** (`.rodata` holds a `TaskHandle` record followed by a
name string for each: `Tsk_BtlEnm010_Lser`, `_RG`, `_Rnge`, `_SWA`, `_SingleShot`, `_Sprl`,
`_Tatt`, `_UG`). So there is more than one task-data struct, as on ov010.

**A second, 0xB4-byte task data is confirmed**: `func_ov011_021283a8` clears exactly 0xB4 bytes
with `MI_CpuSet(data, 0, 0xB4)` and writes nothing beyond it.

**The main task's data is at least 0x214 bytes.** Confirmed offsets are in
`build/scratch/AGENT_BRIEF.md` §4. `func_ov011_0212bac8` is the main task's per-frame handler
(`Task*+0x18` → the data pointer; a 0..6 switch on `func_ov003_02082f2c`).

**Open, do not guess:** `func_ov011_02125750` writes `data + 0x14 + slot*8` for `slot` in 0..2
and `func_ov011_021258b4` writes the same expression for `slot` in 2..4, so that record table
runs to at least `0x34` — but `func_ov011_0212bac8` treats `0x28`/`0x2C`/`0x30` as position
x/y/z, and `0x38` is read as a `u16`. Either two structs are being conflated, or the struct
needs overlapping views the way ov010's `Enm006SpriteAlt` does. `census2.py` is the tool for
settling it.

## Next

1. Resolve the `0x14`-vs-`0x28` overlap above, then verify the struct against three functions
   before trusting it.
2. Finish `func_ov011_02125750` — it is the next in address order, so finishing it keeps the
   emitted prefix contiguous. `fbdiff.py 02125750` is the work list. The known-good first step
   is already in: the original's search loop is a **`do`/`while`**, and rewriting mine as one
   took the function from 76 differing bytes to 64.
3. Then work strictly in address order from `0x021258b4`. Functions must be emitted in address
   order or nothing downstream can line up, even when each matches in isolation.
4. `.rodata` and `.data` still need their symbols named and declared. Beware: the
   `symbols.txt` entries for this overlay are all `func_ov011_*` / `data_ov011_*`; the real task
   names live in the string blobs. Do not add or delete `symbols.txt` entries without checking
   every other overlay's `relocs.txt` for the address.
