# ov011 — `BtlEnm010`

Status as of the session that ended at **74 of 114 functions byte-exact, all 114 written, the
overlay linking**. This file is the authoritative hand-off. Everything below is measured, not
estimated.

## Identity

`ov011` is **`BtlEnm010`**, a Noise (`Apl_Suy`) enemy. Identified from the `.data` string blob at
`0x0212c460`: `Apl_Suy/Grp_BtlEnm010_*.bin` — `MotMove`, `MotTurn`, `EffShot`, `EffRange`, and
several letter-suffixed variants.

Sibling overlays, all four finished and **byte-identical to the original ROM**, and the models for
style and idiom: `BtlEnm006` (ov010), `BtlEnm014` (ov012), `BtlEnm015` (ov013), `BtlEnm026`
(ov015).

## Layout

| region | start | end | size |
|--------|-------|-----|------|
| `.text` | `0x021256c0` | `0x0212bdd8` | `0x6718` (26,872 B) |
| `.rodata` | `0x0212bdd8` | `0x0212c454` | `0x67C` (1,660 B) |
| `.ctor` | `0x0212c454` | `0x0212c454` | empty |
| `.data` | `0x0212c460` | `0x0212cca0` | `0x840` (2,112 B) |
| `.bss` | `0x0212cca0` | `0x0212ccc0` | `0x20` (32 B) |

**114 functions** in `.text` — the largest overlay attempted, about 1.8x ov010.

## Current state

```
functions: 114 total | 6 raw byte-identical | 80 identical-modulo-relocation | 28 differ | 0 missing
1163 differing bytes
data:     154 symbols compared against the delink reference, 0 problems (bytes + relocations)
```

- **86 byte-exact functions.** `.text` prefix `0x021256c0..0x021260e8` is contiguous.
- **28 written-not-exact**, 1,163 differing bytes — the diff queue (largest: 02126bf8 130,
  02129410 121, 0212aa20 97, 021284bc 79, 02129cec 75, 02129188 76, 02128070 55, 021287b8 51,
  02128cc0 50, 02128150 49).
- **0 unstarted.** All 114 definitions exist and compile.
- **The data is transcribed.** All 154 `.rodata`/`.data`/`.bss` symbols live in
  `BtlEnm010Data.c`, byte-exact against the delink reference (`datadiff.py`: 0 problems) and at
  objdiff's `.rodata`/`.data` 100%. See "The data" below for the two toolchain walls this
  uncovered — one of them forced the data into its own translation unit.
- **Address-order debt: PAID.** All 114 definitions are emitted in ascending address order and
  the object's per-function `.text` sections verify ascending. It was **46** functions out of
  order, not "~16" — the earlier estimate was an eyeball; a longest-increasing-subsequence count
  is the honest number.
- **28 functions carry wrong codegen sizes** — see "What is left". Until those are exact, every
  symbol after them in `.text` lands shifted and the ROM comparison avalanches.

Progression across the earlier sessions: 2 → 12 → 15 → 29 → 41 → 49 → 62 → 73 → 74 → **86**.

## The data: transcribed, verified, and the two toolchain walls

`BtlEnm010Data.c` holds every data symbol in the original's layout order, generated from the
delink reference object by `build/scratch/datagen.py` and verified symbol-by-symbol by
`build/scratch/datadiff.py` (bytes **and** relocation targets: 154/154). objdiff scores
`.rodata` and `.data` at **100%**. Two measured facts govern everything about this file:

1. **The data must not share a translation unit with the code.** The reference codegen loads
   every table through pointers (`ldr r0, =table; ldr r1, [r0]`). With the definitions visible
   to the code, MWCC's `-ipa file` folds those reads into immediates and silently rewrites the
   functions — `func_ov011_02129188` alone loses 40 bytes of match, and even without `-ipa` the
   visible array sizes (an `extern s32 x[]` versus a defined `s32 x[4]`) perturb register
   allocation for another 5. The measured baseline is 1,163 bytes of `.text` diff with the data
   out of the TU, 1,203 with it in. This is why the file exists at all.

2. **The compiler and linker do not order the data entries, and no source ordering can make
   them.** MWCC emits data objects size-sorted (with same-shape objects packed into shared
   sections and an opaque tie-break); mwldarm preserves input section order; the result is that
   arbitrary data layouts cannot be expressed in the linked image from C. The emission order is
   a pure function of the symbol set — proven invariant to source order, declaration order,
   type spelling, `#pragma define_section`/`section`/`pool`/`constpool`, `-constpool`, and
   `-str reuse`. This is a toolchain limitation, not a property of our data: objdiff compares
   the data per symbol (100%), and the byte image is verbatim. The ROM-level `complete` endgame
   for the data is blocked on it; the ROM's data currently comes from the delink object, as it
   does for every sibling overlay.

The `.bss` symbol is declared `BtlEnm010Bss` (handle + 0x1C tail) rather than `Task*` + pad:
`symbols.txt` gives `data_ov011_0212cca0` the whole 0x20 extent, and objdiff compares that
granularity — the same trick `BtlEnm006.c` uses for `data_ov010_02129028`.

## The link: achieved experimentally, and how to reproduce it

`romcmp.py` now **runs against our code** — the first end-to-end check this overlay has ever
had. Three findings gate it, all measured:

1. **The link only uses our object when the delinks entry is marked `complete`.** Without it,
   `build\usa\delinks\src\Combat\Noise\BtlEnm010\BtlEnm010.o` (the original bytes) is what
   links, and a `romcmp` 0-diff is the ROM compared against itself — a vacuous pass. This is
   project-wide: `object_to_link` flips to `build\usa\src\...` only for `complete` entries
   (see `dsd json delinks`). **Marking it is the user's call** (hard rule 3); it is deliberately
   *not* marked in the tree.
2. **The data does NOT gate the link.** The `data_ov011_*` symbols resolve from the
   `_dsd_gap@ov011_0.o` gap object when the per-file entry claims only `.text` — i.e. dropping
   the `.rodata`/`.data` claim lines from `src/Combat/Noise/BtlEnm010/BtlEnm010.c:` lets the gap
   carry the original data while our TU owns `.text`. To run the link + `romcmp`, apply exactly
   that (drop the two claim lines, add `complete`): a two-line, fully reversible config change,
   kept out of the tree. The endgame still wants the data transcribed into C.
3. **Every function had to exist first** — with the names fixed, the 9 unwritten functions were
   the only undefined symbols left.

Measured result of the first truthful `romcmp`: `.text` 79% differing, `.rodata` 61%, `.data`
95%, **and the anchor is +0xC0 off** ("`.data` is at the wrong place"). Read that carefully:

- **The 79% is inflated by a placement shift, not by codegen.** Our `.text` is ~0xC0 bytes too
  big (the size debt below), so every symbol after it moves, and every literal-pool word naming
  a moved symbol differs by exactly the shift. The 2-byte diff runs inside *byte-exact*
  functions are **literal-pool words, not wrong calls** — e.g. at `0x0212570C` the pool word
  for `data_ov011_0212bfa4` reads `0x0212c06c` instead of `0x0212bfa4`. Do not chase them as
  call-target bugs.
- **The honest per-function truth remains `fbdiff`: 1,956 differing bytes in 40 functions.**
  `romcmp` becomes fully meaningful once the sizes are exact — which is precisely what it is for.

Two `romcmp.py` details: its recorded `DELTA` constant (`0x1FECCC0`) is wrong by `0x1000` (the
tool self-corrects from the anchor; the original ROM measures `0x1FEBCC0`), and it compares
using the *original's* delta even when the new ROM's anchor has moved — read the anchor warning
before trusting any region numbers.

## Structs — all closed

Nine tasks, and the useful fact that makes this cheap: **every `TaskHandle` in `.rodata` literally
stores its task's data size** in the word after its two function pointers. Nine tasks, nine sizes,
no inference needed.

| `TaskHandle` | name | entry | data size |
|---|---|---|---|
| `0x0212bfa4` | `AnmMgr` | `021259d0` | `0x03C` |
| `0x0212c118` | `Lser` | `02125cf8` | `0x254` |
| `0x0212c130` | `RG` | `021278d4` | `0x208` |
| `0x0212c1d4` | `Rnge` | `02127ce0` | `0x06C` |
| `0x0212c1e0` | `SWA` | `0212801c` | `0x020` — needs no struct; handlers take `void*` |
| `0x0212c1f8` | `SingleShot` | `02128348` | `0x0B4` |
| `0x0212c210` | `Sprl` | `02128758` | `0x0B8` |
| `0x0212c240` | `Tatt` | `02129934` | `0x250` |
| `0x0212c448` | `UG` | `0212b99c` | `0x214` |

Plus four `0x180`-byte handles at `0x0212bde8`–`0x0212be18`, **tasks not yet identified**.

Method that worked: the task's initialiser writes every field and so pins the layout; confirm each
offset against the instruction that touches it; `census2.py <func>` for an offset histogram first;
`MI_CpuSet(data, 0, <size>)` confirms the size independently. `Lser` and `RG` are the fullest
worked examples.

**One loose end:** `02129110`/`021293a8`/`02128cc0`/`02128e30` walk a **0x88-stride record block** —
four 0x88-byte records, a `u16` flag halfword at `+0x84` within each, a signed displacement at
`+0x68`. It fits none of the seven task structs, so those offsets are deliberately left raw. It may
be a seventh type.

## The wrong-prototype class — eleven found; the "closed" verdict was wrong

A "dead instruction" in the original — a value computed and discarded — has turned out to be a
mis-declared callee **eleven times** here. Every one was initially written off as allocator
residue or a register-choice artefact, and every one of those write-offs was wrong:

| helper | was | actually |
|---|---|---|
| `func_ov003_020c3c28` | 1 arg | 0 args |
| `func_ov003_020c3c88` | 0 args | 1 arg |
| `func_ov003_020c4ab4` | `void` | returns `arg1 != 0` |
| `func_ov003_020c6230` | 0 args | 1 arg |
| `func_ov003_02084348` | — | 6 args, two written back through `s16*` |
| `func_ov011_0212b800` | 1 arg | 2 args |
| `func_ov011_02128f80` | 0 args | 1 arg |
| `func_ov011_02128f80` | `s32` | returns `void*` |
| `func_ov011_021258b4` | `u16*` | `CombatSprite*` |
| `func_ov011_02127c4c` | `void` | returns `s32` |
| `func_ov011_021283a8` / `021287b8` | `void` | return `s32`, no epilogue constant |
| `func_ov003_02082750` (`CombatSprite_SetFlip`) | 3 args | **2 args** — its first instruction clobbers r2 (`ldrh r2, [r0, #0xa]`) and every reference call site sets only r0/r1. The phantom third argument cost 102 bytes of diff in `02127ce0` alone. |
| `func_ov011_02125b98` | `void`, 2nd arg `void*` | **`s32`** (the `EasyTask_CreateTask` handle, stored by every call site into `0x1FC`/`0x208`), 2nd arg `s32` (the shot index) |

**A third variant of the class was found this session: wrong *names*.** Nine externs spelled
`func_ov003_0208xxxx` for addresses that carry real names in `ov003`'s `symbols.txt`
(`CombatSprite_SetAffineTransform`, `SetPosition`, `SetFlip`, `Update`, `Render`, `Release`,
`Restart`, `CombatActor_PopPendingCommand`) — invisible to `fbdiff` (the `bl` bytes are
identical), fatal at link. The finished sibling `BtlEnm006.c` already uses the canonical names.

**The earlier "class is closed" argument was wrong — do not trust it.** It claimed a return
type cannot hide once every callee is written; yet both `CombatSprite_SetFlip` (an arity bug
inside *written* callers) and `func_ov011_02125b98` (a return type that only became visible
when new callers stored it) survived that argument. What is actually true:

- *Arity cannot hide in a byte-exact function's own call sites* — but it hides fine in the
  **callee's declaration** while the callers are the only witnesses, which is what `SetFlip`
  was. When a byte-exact function has diff residue near a `bl`, read the callee's prologue.
- *Return type is the invisible class*: a caller that only stores a result tolerates any
  declaration, and the wrongness surfaces only when a *new* caller starts using the result.

**The correctly-scoped test, which took several rounds to get right:**

> A dead *instruction* implies a wrong signature. A wrong *register allocation* does not.

A dead instruction is a value computed and discarded, which is exactly what a mis-declared arity
produces. A register-allocation diff means the C has the wrong number of live locals, and the fix
is more or fewer declarations — never a different prototype. Conflating the two sent the earlier
rounds hunting for prototypes in functions that were merely misallocated.

**`audit003.py` and `audit011.py` are ~90% false positive and must not be trusted as a signal.**
The heuristic marks r0–r3 read on first sight, but in almost every function here r0–r3 are first
touched as the *destination of a call result*, long after arguments have been copied to r4 or
higher. Nearly every function that takes an argument moves it to a callee-saved register
immediately. Confirm every hit by reading the callee's prologue by hand.

## What is left, and what it is worth

**The size debt is the top of the list** — 34 functions emit the wrong number of bytes (net
**+200**), and until every size is exact the overlay cannot be placed where the original put
it (`romcmp` measured the anchor +0xC0 off). Size exactness is what `romcmp` exists to police:

```
addr       ref    ours   delta   addr       ref    ours   delta
021260e8   0x26C  0x280   +20    02128cc0   0x170  0x174    +4
021268c4   0x140  0x134   -12    02128f10   0x070  0x080   +16
02126b2c   0x0CC  0x0D0    +4    02128f80   0x190  0x1A4   +20
02126bf8   0x288  0x294   +12    02129188   0x220  0x20C   -20
02126fb0   0x290  0x294    +4    02129410   0x524  0x51C    -8
0212791c   0x148  0x144    -4    02129994   0x1F0  0x208   +24
02127ce0   0x28C  0x288    -4    02129b84   0x168  0x174   +12
02128070   0x0E0  0x0FC   +28    02129cec   0x1B8  0x1D0   +24
02128150   0x100  0x114   +20    02129f80   0x18C  0x184    -8
02128250   0x068  0x070    +8    0212a674   0x118  0x10C   -12
021284bc   0x1DC  0x1D4    -8    0212a78c   0x294  0x29C    +8
021287b8   0x110  0x118    +8    0212aa20   0x1EC  0x1E8    -4
021288c8   0x2B8  0x2D8   +32    0212ac0c   0x2DC  0x2E0    +4
02128c44   0x060  0x05C    -4    0212aee8   0x0AC  0x0B0    +4
0212b5d8   0x108  0x10C    +4    0212b6e0   0x120  0x134   +20
0212b890   0x10C  0x118   +12    0212b9e4   0x0E4  0x0E0    -4
0212bac8   0x1BC  0x1C0    +4    0212bd3c   0x054  0x050    -4
```

(25 of the 34 are the older written-not-exact set — mostly register-allocation residue, the
class the four finished overlays were shipped with; 9 are the new first-pass transcriptions,
which are *semantically* faithful but spilled codegen. Fixing the 25 older ones is the better
ratio: the recipes in the brief are measured against them.)

**The old "`bl func_ov003_*` `-0x8` symbol addend" open question is now answered: there is no
addend bug.** The 2-byte diffs that `romcmp` shows inside byte-exact functions are literal-pool
words naming *shifted data symbols* (placement, see above). `fbdiff`'s `RELOCC` masking is
object-level and sound.

**The data endgame is still open and does not block the link.** The `.rodata`/`.data` gap
bridge is a legitimate interim state, but a finished overlay wants the 154 `data_ov011_*`
symbols transcribed into C (TaskHandles with function pointers, the string blob, the tables) —
`BtlEnm006.c` is the model. Until then `romcmp`'s `.rodata`/`.data` numbers compare gap-provided
original bytes and are vacuous.

## Worktree

`E:\Git\twewy-ov011`, branch `decomp-ov011`, branched off `decomp-ov010`.

- `extract/` is a **junction** to `E:\Git\twewy\extract` — the parent directory, not `extract\usa`.
- `objdiff-cli.exe` is an untracked, gitignored binary copied in from `E:\Git\twewy-ov010`.
- `build/` is gitignored, so **everything in `build/scratch/` is untracked**. It exists on this
  machine and the tooling is all there, but it would not survive a fresh clone. Copy from
  `E:\Git\twewy-ov010\build\scratch` when setting up a new worktree.
- `E:\Git\twewy` is the main checkout and holds unrelated work in progress. **Read-only.**

## Tooling (`build/scratch/`, untracked)

| file | purpose |
|------|---------|
| **`AGENT_BRIEF.md`** | **the pipeline brief — read this first.** Struct layouts, every codegen recipe with the model function that measured it, per-function state for all 40 remaining, the plans in §6/§10, and the `symbols.txt` rules. |
| `fbdiff.py` | **the measurement tool.** Reads both objects directly, boundaries from `symbols.txt`, prints `OK` (byte-identical) / `RELOCC` (identical once relocation bits are masked) / diff size. The primary signal. |
| `objtext.py` | Hashes an object's concatenated per-function `.text` sections. A whole-file hash moves whenever a type or comment changes, so it is useless for this. |
| `asm.py` | Dumps reference disassembly for named functions. `python -u build\scratch\asm.py func_ov011_021256c0` |
| `pd.py` | Per-instruction diff, `L=original`, `R=ours` |
| `mine.py` | Decodes **our** object; objdiff's formatter mis-renders shift immediates |
| `ov.ps1` / `ovm.ps1` | Iterate: patch the delinks `.text end`, build, diff, print. **Use `ovm.ps1` for anything that does not edit the `.c`.** |
| `romcmp.py` | ROM byte comparison. **Cannot run until the overlay links.** Correct mapping (`ram - 0x1FECCC0`, anchored on `Apl_Suy/Grp_BtlEnm010_MotMove`). |
| `census.py` / `census2.py` | Offset histograms across a task's functions — the fastest way to characterise a struct |
| `datagen.py` | Generates the data definitions (`.rodata`/`.data`/`.bss`) from the delink reference object: bytes verbatim, typed where the code or the relocations demand it. Output: `data_gen.c`. |
| `datadiff.py` | **the data measurement tool.** Compares every data symbol against the delink reference — bytes and relocation targets, symbol by symbol (objdiff's standard; section order is the toolchain's problem). |
| `mk_datafile.py` | Assembles `BtlEnm010Data.c` from the generated block + the preamble (the `-ipa` split rationale lives there). |
| `audit003.py` / `audit011.py` | ov003 and ov011-local arity triage. ~90% false positive. |
| `retcheck.py` | Return-type classification. Only its "every return site sets r0" verdicts are trustworthy; the 3-instruction window is too narrow. |

**Always measure with `-end 0x0212bdd8`.** Anything smaller truncates the range and makes the
average look better than it is.

## Traps, learned the hard way

- **Never let the data share a TU with the code that reads it.** MWCC's `-ipa file` folds reads
  of in-file `const` tables into immediates and silently rewrites the callers — 40 bytes of
  match gone in `func_ov011_02129188` alone — and even without `-ipa`, the *visible array size*
  (`extern s32 x[]` vs a defined `s32 x[4]`) shifts register allocation. The reference loads
  every table through pointers, which is itself the proof its data was a separate TU. Measure
  the `.text` diff (1,163 vs 1,203) after any experiment that moves definitions around.
- **Data section *placement* is not expressible from C with this toolchain.** MWCC emits data
  objects size-sorted (same-shape objects packed into shared sections, opaque tie-break);
  mwldarm keeps input order. The emission order is a pure function of the symbol set — source
  order, declaration order, `#pragma define_section`/`section`/`pool`/`constpool`, `-constpool`
  and `-str reuse` all leave it unchanged. Do not build post-processing band-aids against it:
  verify the data symbol-by-symbol (`datadiff.py`, objdiff) and treat the ROM-level placement as
  blocked on the toolchain.
- **Address-order debt blocks the link — and it is now paid.** Functions must be emitted in
  address order (all 114 are, and the object's `.text` sections verify ascending). Insert at
  address as you write anything new; `fbdiff` measures per symbol so the count stays honest, but
  the link will not. When the file was sorted, 46 of 105 definitions had to move — count with a
  longest-increasing-subsequence, not by eyeball.
- **`build/usa/delinks/**.o` is never a declared ninja output** — it is a side effect of the delink
  rule, and its only declared output is `_dsd_gap@main_*.o`. Ninja therefore sees a stale file and
  stops. A stale base object mimics a code regression. Suspect it before your C.
- **The build does not track headers as dependencies of a TU.** A header-only edit leaves a stale
  `.o` and every measurement silently re-reports old numbers. This cost a long misdiagnosis of a
  struct bug that did not exist. `ovm.ps1` handles it.
- **Stale `objdiff.json` invalidates measurements silently**: a missing unit makes
  `objdiff-cli diff -u` fail *without writing output*, so a harness reprints the previous JSON.
- **A delinks TU entry needs the FULL `.text` range.** With `end == start`, dsd emits zero function
  symbols — 153 `SYMBOL_OBJECT` and no `SYMBOL_FUNCTION`, and `.text` reporting a meaningless 100%.
- **`dsd` renders register-shifted operand2 shifts at 2x the ARM field** (`e1a00201` prints as
  `lsl #0x4`). Read the bytes, not the text.
- **`sizeof(CombatSprite)` is 0x7D but the ROM strides sprites by 0x60.** A `CombatSprite[]` array
  puts every later field 0x38 too high. Use raw padding plus a walking `CombatSprite*`.
- **A sub-struct whose size is not a multiple of 4 shifts every later `s32`.** Check sizes, not
  just offsets.
- **PowerShell `[System.IO.File]` calls do not follow `cd`.** Use absolute paths or Python.
- **Use the edit tool for source edits.** A `Get-Content` → `[ArrayList]` → `RemoveAt` →
  `Set-Content -NoNewline` round-trip collapsed an entire source file onto one line and cost a
  third of a session.
- **A `git commit` that dies on the clang-format hook leaves the work UNCOMMITTED.** The hook
  rewrites the files and the retry is easy to forget — and a later `git checkout` or `git stash`
  silently drops the whole landing. This cost one full function landing (recovered by re-doing
  it). Verify `git log --oneline -1` after every commit, and never `git checkout` a file without
  checking `git status` first.
- **`fbdiff` reads the built object, not the source.** A `git stash`/`checkout` without a rebuild
  measures the *old* build and will happily confirm a conclusion about code you no longer have.
- **Do not edit `symbols.txt` before checking whether another overlay's `relocs.txt` targets the
  address.** Deleting a referenced symbol breaks the delink outright.
- **Calls to `0x020824a0` must be spelled `Mini108_VBlank`.**
- **Cross-module calls must be spelled with the canonical name in the target module's
  `symbols.txt`**, not `func_ov003_XXXXXXXX`: where the address carries a real name
  (`CombatSprite_Restart` at `0x02082d04`, etc.) the placeholder spelling is an undefined symbol
  at link time and *identical bytes* at `fbdiff` time. Nine of these existed; `BtlEnm006.c` shows
  the canonical spellings.
- **A `romcmp` 0-diff can be vacuous.** Until the delinks entry is marked `complete`, the link
  uses the delinks (original-bytes) object and `romcmp` compares the ROM against itself. Check
  `dsd json delinks` for `object_to_link` before believing a ROM-level number.
- **Literal-pool words move when any function's size is wrong**, and the ROM comparison then
  avalanches from the first wrong size — 2-byte diffs appear inside *byte-exact* functions and
  look like wrong call targets. They are shifted data symbols. Measure per-function sizes
  (`fbdiff`, or the object's symbol table against `symbols.txt`) before reading `romcmp`.
- **MWCC rejects implicit-declaration-then-definition** (`identifier redeclared: was declared as
  'int (...)'`) and rejects taking the address of an undeclared function. Any reordering that
  creates forward references needs real prototypes first.
- **Run the scratch PowerShell scripts with `pwsh`, not Windows PowerShell 5.1** — 5.1 turns
  native stderr into a terminating `NativeCommandError` under `$ErrorActionPreference = 'Stop'`
  and the script dies after doing its work.
- Do **not** mark the overlay `complete` in `delinks.txt`. That is the user's call. (It is also
  the only switch that makes the link use our object — see "The link" above.)

## Cross-branch work done this session

Both fixes are in **all five worktrees** and verified byte-neutral:

- **`func_ov003_02087f00` takes `(SndMgrSeIdx, s32 sePan)`, not a callback pointer.** The body
  tail-calls `SndMgr_PlaySEWithPan` with r0/r1 untouched and builds r2 itself, so the second
  argument is a pan value. 27 bogus casts removed across `BtlEnm006.c` and `BtlEnm010.c`.
  Commits `5bd9c64` (ov010), `b4a0dca` (ov011), `a09868c` (ov008), `d135a7a` (ov012), `e25b1b6`
  (ov013), `d7ea679` (ov015).
- **`func_ov003_020843b0` returns `s16`, not `s32`.** Its body ends `lsl r0,r0,#4 / asr r0,r0,#16`,
  so r0 is already a sign-extended 16-bit value. A four-case codegen probe showed the width is
  invisible except where a caller narrows, and there `s16` is 8 bytes shorter. Every call site is
  the pass-through shape, so it is byte-neutral. Commits `3a93a78` (ov010), `bceb388` (ov011).

The four finished overlays remain byte-identical to the original ROM.
