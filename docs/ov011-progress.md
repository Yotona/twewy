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

**15 of 114 functions byte-identical** (`RELOCC` or better), **0 differing bytes**. The emitted
prefix `0x021256c0..0x021260e8` is contiguous and every function's size matches the original's
exactly.

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

Nothing was deliberately skipped. `021260e8` (620 B) was read in full, its layout and four
helper arities pinned, and then **capped rather than half-written** — see
`build/scratch/AGENT_BRIEF.md` §6 for the plan and the one open question. `.text`/`.rodata`/
`.data` are not byte-identical and `romcmp.py` has not been run - the linked ROM does not exist
until all 114 functions are emitted, so `fbdiff.py` is the only signal available.

`ENM010_POOL` in `BtlEnm010.c` is a local `#define` for
`(TaskPool*)((u32)data_ov003_020e71b8 + 0x118 + 0x10000)`. `data_ov003_020e71b8` is already
declared in `Combat/Core/Combat.h` as an `Ov003Global*` - do not redeclare it.

## Next

1. **`0x021260e8`** (620 B, the Lser mode-2 worker) was read and deliberately **capped, not
   half-written**. `build/scratch/AGENT_BRIEF.md` §6 has the full plan, the four confirmed
   helper arities, and the one open question (`func_ov003_02082750` is called with 3 arguments
   at one site and 2 at another, and the callee clobbers r2 before reading it). The struct it
   needs is pinned. This is the first thing to take on.
2. Then `02126354` (596 B, the Lser's last command handler, dispatched from `02125cf8` case 2)
   and `021265a8` (44 B, case 3).
3. **A wrong declaration in `Combat.h` is confirmed and unfixed** — see
   `build/scratch/AGENT_BRIEF.md` §5. `func_ov003_02087f00`'s second parameter is `s32`, not a
   function pointer: `SndMgr_PlaySEWithPan` takes `(SndMgrSeIdx, s32 sePan)`, and of 106 call
   sites in `src/`, 105 pass an `s32` expression and **zero** pass a real function pointer. 22
   casts in the finished `BtlEnm006.c` exist only to satisfy the bad declaration. Fixing it
   means deleting those casts and re-verifying ov010 is still byte-identical — a separate
   commit, not this overlay's business.
4. `.rodata` and `.data` still need their symbols named and declared. The `symbols.txt`
   entries are all `func_ov011_*` / `data_ov011_*`; the real task names live in the string
   blobs. Do not add or delete `symbols.txt` entries without checking every other overlay's
   `relocs.txt` for the address.
5. Still to characterise: **RG 0x208** (entry `021278d4`), **Rnge 0x06C** (`02127ce0`),
   **SWA 0x020** (`0212801c`), **Sprl 0x0B8** (`02128758`), **Tatt 0x250** (`02129934`),
   **UG 0x214** (`0212b99c`; `0212bac8` is its per-frame handler). Plus the four 0x180-byte
   handles at `0x0212bde8`-`0x0212be18`, whose tasks are still unidentified. For each, the
   task's *initialiser* is the cheap way in: it writes every field, so it pins the layout.
