# ov011 — `BtlEnm010`

Status as of the session that ended at **74 of 114 functions byte-exact**. This file is the
authoritative hand-off. Everything below is measured, not estimated.

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
114 total | 6 raw byte-identical | 68 identical-modulo-relocation | 31 differ | 9 missing
1327 differing bytes
```

- **74 byte-exact.** `.text` prefix `0x021256c0..0x021260e8` is contiguous.
- **31 written-not-exact**, 1,327 differing bytes. Nearly all are register-allocation residue.
- **9 unstarted**, 476–1,316 bytes.
- **Address-order debt: ~16 functions** out of address order in the source file. See
  "The real blocker" below.

Progression across the session: 2 → 12 → 15 → 29 → 41 → 49 → 62 → 73 → **74**. The last six
rounds moved it 62 → 73 → 74 → 74 → 74 → 74. The cheap phase is over.

`.text`/`.rodata`/`.data` are **not** yet byte-identical, and `romcmp.py` has never been able to
run for this overlay — see "The real blocker".

## The real blocker: the overlay has never linked

`romcmp.py` compares the built ROM against `E:\Git\twewy\build\usa\twewy_usa.nds`. It is the only
check that has ever actually mattered on this project — an objdiff section percentage measures
object granularity and reloc artefacts, not correctness. **For ov011 it cannot run at all**, because
the overlay does not link until all 114 functions are emitted, and the address-order debt must be
paid before it can.

So the whole of ov011's verification rests on `fbdiff.py` (per-function byte comparison) and has
never had the authoritative end-to-end check. **Paying the address-order debt is the highest-value
remaining action**, more so than any individual function.

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

## The wrong-prototype class — eight found, and now closed

A "dead instruction" in the original — a value computed and discarded — has turned out to be a
mis-declared callee **eight times** here. Every one was initially written off as allocator residue
or a register-choice artefact, and every one of those write-offs was wrong:

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

**The class is now closed, by argument rather than by exhaustion:**

- *Arity cannot hide in a byte-exact function.* Every call it makes has the right register setup,
  because the `mov`/`add` pairs in front of each `bl` are part of the compared bytes. So every
  helper reached from the 74 byte-exact functions is already validated.
- *Return type is the invisible class*, because a caller that only stores a result tolerates any
  declaration. But the eight declarations where one could still hide are all functions **not yet
  written**, and writing a definition forces MWCC to reject a mismatch. There are no extern-only
  helpers left that will never be defined.

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

**The 9 unstarted**, smallest first:

```
476 B  021284bc     disassembly was already read by the previous session
544 B  02129188
620 B  021260e8     plan recorded in the brief, §6
648 B  02126bf8     plan recorded in the brief, §10
656 B  02126fb0
660 B  0212a78c     five table lookups, 0x14 frame
696 B  021288c8
732 B  0212ac0c
1316 B 02129410     largest in the overlay; needs a round of its own
```

**The 31 written-not-exact** are almost all register-allocation residue — the same class the four
finished overlays were shipped with. Now that prototypes are ruled out, the cause has exactly one
remaining explanation: wrong live-local counts. That is a bounded mechanical search over a known
list, which is why it is the better ratio of effort to result.

**Not yet measured, and a real open question:** how many of the 31's sub-99% functions are
*actually* correct and merely carrying the known `bl func_ov003_*` `-0x8` symbol addend? That
artefact shows up in ~100% of calls including otherwise-perfect functions. Auditing it means
reading every diff line of twenty-odd functions, which was deliberately deferred. The two functions
sampled were both real multi-instruction diffs in non-`bl` positions, so it explains none of those.

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
| `audit003.py` / `audit011.py` | ov003 and ov011-local arity triage. ~90% false positive. |
| `retcheck.py` | Return-type classification. Only its "every return site sets r0" verdicts are trustworthy; the 3-instruction window is too narrow. |

**Always measure with `-end 0x0212bdd8`.** Anything smaller truncates the range and makes the
average look better than it is.

## Traps, learned the hard way

- **Address-order debt blocks the link.** Functions must be emitted in address order. Insert at
  address as you write; `fbdiff` measures per symbol so the count stays honest, but the link will
  not.
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
- **Do not edit `symbols.txt` before checking whether another overlay's `relocs.txt` targets the
  address.** Deleting a referenced symbol breaks the delink outright.
- **Calls to `0x020824a0` must be spelled `Mini108_VBlank`.**
- Do **not** mark the overlay `complete` in `delinks.txt`. That is the user's call.

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
