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

## Structs - resolved, and the answer to the open question

The overlay owns **nine tasks, each with a different data size**, and the size is written out
literally in each `TaskHandle` in `.rodata` (the word after the two function pointers). That
is the fact that settles the struct question without any guessing.

| `TaskHandle` | name | task entry point | data size |
|---|---|---|---|
| `0x0212bfa4` | `Tsk_BtlEnm010_AnmMgr` | `func_ov011_021259d0` | **0x03C** |
| `0x0212c118` | `Tsk_BtlEnm010_Lser` | `func_ov011_02125cf8` | **0x254** |
| `0x0212c130` | `Tsk_BtlEnm010_RG` | `func_ov011_021278d4` | **0x208** |
| `0x0212c1d4` | `Tsk_BtlEnm010_Rnge` | `func_ov011_02127ce0` | **0x06C** |
| `0x0212c1e0` | `Tsk_BtlEnm010_SWA` | `func_ov011_0212801c` | **0x020** |
| `0x0212c1f8` | `Tsk_BtlEnm010_SingleShot` | `func_ov011_02128348` | **0x0B4** |
| `0x0212c210` | `Tsk_BtlEnm010_Sprl` | `func_ov011_02128758` | **0x0B8** |
| `0x0212c240` | `Tsk_BtlEnm010_Tatt` | `func_ov011_02129934` | **0x250** |
| `0x0212c448` | `Tsk_BtlEnm010_UG` | `func_ov011_0212b99c` | **0x214** |

Plus four handles at `0x0212bde8` / `0x0212bdf8` / `0x0212be08` / `0x0212be18`, all size
**0x180**, sharing one of two `initData` records and one entry point (`0x0212bde0`).

### The `0x14+slot*8` vs `0x28/0x2C/0x30` question - ANSWERED: no conflict

They belong to **different tasks**, so no overlapping views are needed anywhere in this
overlay.

- `func_ov011_02125750` and `func_ov011_021258b4` get their data from
  `EasyTask_GetTaskData(ENM010_POOL, data_ov011_0212cca0)` -- the one task
  `func_ov011_021256c0` spawns from the handle at `0x0212bfa4`. That is
  **`Tsk_BtlEnm010_AnmMgr`, 0x3C bytes.**
- `0x28`/`0x2C`/`0x30` as position x/y/z and `0x1C8` as a callback come from
  `func_ov011_0212bac8`, whose data comes from `Task* + 0x18` -- a different task,
  `Tsk_BtlEnm010_UG`, **0x214 bytes**.

The lesson, and the reason ov010 needed `Enm006SpriteAlt` and ov011 does not: when two
functions disagree about what an offset means, **check which task each is working on before
reaching for overlapping views.** Real overlap inside one struct is much rarer than it looks.

### Confirmed layouts

`BtlEnm010AnmMgr` (0x3C), `BtlEnm010UG` (0x214), `BtlEnm010Owner` (0x88),
`BtlEnm010Lser` (0x254), `BtlEnm010LserArgs` (0x18), `BtlEnm010LserEmit` (0x2C) and
`BtlEnm010LserRec` (0xC) are in `include/Combat/Noise/Private/BtlEnm010.h`, each field
carrying its evidence in a comment. Tables also in `build/scratch/AGENT_BRIEF.md`.

**`BtlEnm010Lser` (0x254) is fully characterised** -- `func_ov011_02125d48` is the task's
initialiser and writes every field: `0x00` the owner; `0x80` four `CombatSprite`s 0x60 apart;
`0x200` the emitter block; `0x23C`/`0x240` zeroed from one constant; `0x244` the negated
`args->unk_10`; `0x24C` `args->unk_14`; `0x250` a byte whose bit 3 tracks
`func_ov003_020c37f8`. `0x54`/`0x58` are read by the *next* function, `02125e14`, and are
still padding.

`BtlEnm010Owner` (0x88) is the object every task hangs off: `unk_24` a mirror flag, `unk_28`/
`unk_2C`/`unk_30` a 4.12 position triple, `unk_84` two bits used as a sprite-variant selector.
`02125e14` also reads the owner's `unk_54` (its engine-flags word, `tst #4`), matching ov010.

`BtlEnm010AnmMgr`: `0x00` the decompression buffer, `0x04` its size, `0x08` an 8-byte-stride
`{binId, offset}` table (all three from `func_ov003_020cb200`; `0x04`/`0x00` confirmed by
`func_ov003_020cb150`, which allocates `unk_04` bytes off `gMainHeap` and stores the result in
`unk_00`); `0x0C` a halfword count (`func_ov003_020cb194`); `0x14` **four** 8-byte slots
`0x14..0x33`; `0x34` s32 and `0x38` u16 are the two fields *after* the table, **not** a fifth
slot -- `func_ov011_02125a08` stores `max10 + sec10` in `unk_34` and the mode in `unk_38`.
(An earlier revision of this doc claimed five slots; the index ranges in `02125750` (0..2) and
`021258b4` (2..3) only ever reach four.)

`BtlEnm010UG` (0x214): `0x28/0x2C/0x30` position x/y/z; `0x1C8` a callback; `0x1CC` a result
word; `0x1D0/0x1D4/0x1D8` and `0x1E8/0x1EC/0x1F0` two velocity triples, the second added into
the first; `0x206` a `u8` flag; `0x208` an `EasyTask_ValidateTaskId` argument; `0x20C`/`0x210` a
counter pair. All from `func_ov011_0212bac8`, whose four entry points are tabulated at
`0x0212c36c`.

**A header gotcha worth knowing about the rest of the overlay: `sizeof(CombatSprite)` is
0x7D, not the 0x60 the ROM uses** -- the `Sprite` bitfield block overruns its 0x40 allocation.
An array of the type therefore puts every later field 0x74 bytes too high. Use raw padding plus
a walking `CombatSprite*` with a literal stride.

## Done

**49 of 114 functions byte-identical** (`RELOCC` or better), 12 written but not yet exact
(315 differing bytes between them), **53 not yet written**.

| function | bytes | status |
|----------|-------|--------|
| `func_ov011_021256c0` | 84 | **byte-exact** |
| `func_ov011_02125714` | 60 | **byte-exact** |
| `func_ov011_02125750` | 356 | **byte-exact** |
| `func_ov011_021258b4` | 284 | **byte-exact** |
| `func_ov011_021259d0` | 56 | **byte-exact** |
| `func_ov011_02125a08` | 384 | **byte-exact** |
| `func_ov011_02125b88` | 16 | **byte-exact** |
| `func_ov011_02125b98` | 104 | **byte-exact** |
| `func_ov011_02125c00` | 68 | **byte-identical** (raw, no reloc difference either) |
| `func_ov011_02125c44` | 180 | **byte-exact** |
| `func_ov011_02125cf8` | 80 | **byte-exact** |
| `func_ov011_02125d48` | 204 | **byte-exact** |
| `func_ov011_02125e14` | 272 | **byte-exact** |
| `func_ov011_02125f24` | 320 | **byte-exact** |
| `func_ov011_02126064` | 132 | **byte-exact** |
| `func_ov011_02126354` | 596 | **byte-exact** |
| `func_ov011_021265a8` | 44 | **byte-exact** |
| `func_ov011_021265d4` | 40 | **byte-exact** |
| `func_ov011_021265fc` | 504 | **byte-exact** |
| `func_ov011_021267f4` | 40 | **byte-exact** |
| `func_ov011_0212681c` | 168 | **byte-exact** |
| `func_ov011_02126a04` | 232 | **byte-exact** |
| `func_ov011_02126aec` | 64 | **byte-exact** |
| `func_ov011_02127628` | 112 | **byte-exact** |
| `func_ov011_02127698` | 72 | **byte-exact** |
| `func_ov011_021276e0` | 120 | **byte-exact** |
| `func_ov011_02127758` | 112 | **byte-exact** |
| `func_ov011_021277c8` | 268 | **byte-exact** |
| `func_ov011_021278d4` | 72 | **byte-identical** (raw, no reloc difference either) |
| `func_ov011_02127a64` | 308 | **byte-exact** |
| `func_ov011_02127bf0` | 92 | **byte-exact** |
| `func_ov011_02127bdc` | 20 | **byte-exact** |
| `func_ov011_02127c4c` | 56 | **byte-exact** |
| `func_ov011_02127c84` | 92 | **byte-exact** |
| `func_ov011_0212801c` | 84 | **byte-exact** |
| `func_ov011_021268c4` | 320 | written, 30 B out — brief §8 |
| `func_ov011_02126b2c` | 204 | written, 4 B long — brief §9 |
| `func_ov011_0212791c` | 328 | written, 4 B short — brief §12 |
| `func_ov011_02127b98` | 68 | written, 8 B long — brief §12 |
| `func_ov011_02127f6c` | 176 | written, load order off — brief §12 |
| `func_ov011_02128348` | 96 | **byte-exact** |
| `func_ov011_02128698` | 108 | **byte-exact** |
| `func_ov011_02128704` | 20 | **byte-exact** |
| `func_ov011_02128718` | 64 | **byte-exact** |
| `func_ov011_02128758` | 96 | **byte-exact** |
| `func_ov011_021282b8` | 144 | **byte-exact** |
| `func_ov011_02128250` | 104 | written, 8 B long — brief §14 |
| `func_ov011_02127ce0` | 652 | written, 4 B short — brief §14 |
| `func_ov011_02128b80` | 176 | written, load order off — brief §14 |
| `func_ov011_02128c30` | 20 | **byte-exact** |
| `func_ov011_02128ca4` | 28 | **byte-identical** (raw, no reloc difference either) |
| `func_ov011_02129ed0` | 40 | **byte-exact** |
| `func_ov011_02129ea4` | 44 | **byte-exact** |
| `func_ov011_02129110` | 120 | **byte-exact** |
| `func_ov011_021293a8` | 104 | **byte-exact** |
| `func_ov011_02129934` | 96 | **byte-exact** |
| `func_ov011_0212a10c` | 40 | **byte-exact** |
| `func_ov011_02128c44` | 96 | written, 4 B short — brief §14 |
| `func_ov011_02128eb0` | 96 | written, base register — brief §14 |
| `func_ov011_02128f10` | 112 | written, 20 B long — brief §14 |
| `func_ov011_0212a634` | 64 | written, base register — brief §14 |

**The contiguous-prefix framing was dropped two rounds ago**, and it was costing throughput: the
count is what matters, and a byte-exact function at a higher address is worth more than a
contiguous prefix that stalls. Consequence to remember: `fbdiff.py` measures per symbol, so
the count above is honest, but **sixteen functions are currently in the file out of address
order**. The run `02127628`..`02128758` is written, and `02128348`/`02128698`/`021282b8` were
*inserted* at their addresses rather than appended, so what is left inside it is only the three
still-unwritten `021282b8`-adjacent functions `021283a8` and `021284bc` plus the unwritten
`02126bf8`..`02127628` block ahead of it. They must be
reinserted in order before the final link, and `romcmp.py` is what will catch it if they are not.

Three functions were read in full and deliberately **skipped rather than half-written**:
`021260e8` (620 B, brief §6) and `02126bf8` (648 B, brief §10) — the two largest in the overlay.
Both have their layouts and open questions recorded, so neither restarts from a blank page.
`.text`/`.rodata`/`.data` are not byte-identical and `romcmp.py` has not been run — the linked ROM
does not exist until all 114 functions are emitted, so `fbdiff.py` is the only signal available.

`ENM010_POOL` in `BtlEnm010.c` is a local `#define` for
`(TaskPool*)((u32)data_ov003_020e71b8 + 0x118 + 0x10000)`. `data_ov003_020e71b8` is already
declared in `Combat/Core/Combat.h` as an `Ov003Global*` - do not redeclare it.

## Next

1. **The twelve written-not-exact, they are the cheapest bytes on the board.** All in
   `build/scratch/AGENT_BRIEF.md` §8, §9, §12 and §14. Four of them (`02126b2c`, `02127b98`,
   `0212a634`, `02128eb0`) are the *same* r0-vs-r1 base allocation and are grouped under one
   heading in §14 — do not spend more than one build each on that.
2. **New: the reverse-order rule** (brief §15). MWCC loads a struct assignment's sources in
   reverse of the order the C assigns the fields, and stores them in reverse of that again. It
   is what made `021282b8` byte-exact, and it is the open question on `02128b80`.
3. **The unattempted list is now 53 and getting short.** Next in address order after the last
   landed function: `02128cc0` (0x170), `02128e30` (0x80), `02128f80` (0x190), `02129188` (0x220),
   `02129410` (0x524), `02129994` (0x1F0), `02129b84`, `02129cec`, `02129ef8`, `02129f80`,
   `0212a134`, `0212a2ec`, `0212a420`, `0212a540`, `0212a674`, then the UG block from `0212b99c`
   and the tail to `0212bdd8`.
4. **`0x02126bf8`** (648 B, RG's phase-5 worker) — read in full, deliberately **skipped, not
   half-written**. `build/scratch/AGENT_BRIEF.md` §10 has the eight-step plan and the three open
   questions. It is the largest thing left and packs four hard idioms into 648 bytes.
5. **`0x021260e8`** (620 B, the Lser mode-2 worker) is still outstanding, recorded in brief §6.
   Try the 2-argument declaration for `func_ov003_02082750` first.
6. **A wrong declaration in `Combat.h` is confirmed and unfixed** — brief §5.
   `func_ov003_02087f00`'s second parameter is `s32`, not a function pointer: of 106 call sites
   in `src/`, 105 pass an `s32` expression and **zero** pass a real function pointer. 22 casts in
   the finished `BtlEnm006.c` exist only to satisfy the bad declaration. Fixing it means deleting
   those casts and re-verifying ov010 is still byte-identical — a separate commit.
7. **Structs.** `BtlEnm010RG` (0x208) and `BtlEnm010Lser` (0x254) done. `BtlEnm010Rnge` (0x6C),
   `BtlEnm010Sprl` (0xB8) and `BtlEnm010Tatt` (0x250) created and partly pinned. Spawn blocks:
   `BtlEnm010RngeArgs` (0x20), `BtlEnm010SprlArgs` (0x18). `BtlEnm010RngeArgs` is used by two
   different spawns with different arities — see the commit for `02127f6c`. Still to create:
   **SingleShot 0x0B4** (`0x021283a8` clears exactly 0xB4 via `MI_CpuSet`), **UG 0x214**
   (`0212b99c`; `0212bac8` is its per-frame handler; `0x28`/`0x2C`/`0x30` a position triple and
   `0x210` a counter reaching `0x214`), plus the four 0x180-byte handles at
   `0x0212bde8`-`0x0212be18`, whose tasks are still unidentified. **`02129110`/`021293a8` use a
   0x88-stride record block that fits none of these** — raw offsets there are deliberate.
8. `.rodata` and `.data` still need their symbols named and declared.
9. **The address-order debt is unchanged in size but still mostly unwritten functions.** Keep
   inserting at address. When the unattempted list is down to a handful, do the reordering pass
   as its own piece of work with its own verification.
