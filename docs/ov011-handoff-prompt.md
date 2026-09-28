# Hand-off prompt — ov011 (`BtlEnm010`)

Paste the block below into a fresh session. It assumes **no prior context** and points at
everything it needs. `docs/ov011-progress.md` is the authoritative status document;
`build/scratch/AGENT_BRIEF.md` is the codegen playbook.

---

You are continuing the decompilation of overlay **ov011** (`BtlEnm010`) in a TWEWY (The World
Ends With You) Nintendo DS decompilation project, working toward a **byte-exact match with the
original ROM**.

## Read these first, in this order

1. **`E:\Git\twewy-ov011\docs\ov011-progress.md`** — full status, layout, struct table, the
   remaining work, and every trap. This is the authoritative hand-off.
2. **`E:\Git\twewy-ov011\build\scratch\AGENT_BRIEF.md`** — the codegen playbook: ~40 verified
   MWCC recipes, each with the model function that measured it, plus per-function state for all 40
   remaining functions and step-by-step plans for two of them (§6, §10).
3. **`E:\Git\twewy-ov010\src\Combat\Noise\BtlEnm006\BtlEnm006.c`** and its header — the finished,
   byte-identical sibling overlay. The model for style, idiom, and struct declaration.

## Where things stand

**74 of 114 functions byte-exact.** Six raw byte-identical, 68 identical-modulo-relocation.
31 written-not-exact (1,327 differing bytes), 9 unstarted (476–1,316 bytes).

The overlay **has never linked**, so `romcmp.py` — the only check that has actually mattered on this
project — has never been able to run for ov011. Everything rests on `fbdiff.py`.

**The address-order debt (~16 functions out of address order in the source) is the real blocker.**
It must be paid before the overlay can link, so it gates the authoritative verification. Paying it
is worth more than landing individual functions.

The last six rounds moved the count 62 → 73 → 74 → 74 → 74 → 74. The cheap phase is over; be
straight with the user about diminishing returns rather than reporting activity as progress.

## Recommended order of work

1. **Pay the address-order debt**, then get the overlay to link and run `romcmp.py`. This is the
   highest-value action and it is the gate on real verification.
2. **The 31 written-not-exact.** With prototypes now ruled out (see below), their cause has exactly
   one remaining explanation: wrong live-local counts. That is a bounded mechanical search over a
   known list, and a better effort-to-result ratio than the 9 cold large functions.
3. **The 9 unstarted**, smallest first: `021284bc` (476 B, disassembly already read) · `02129188`
   (544 B) · `021260e8` (620 B, plan in brief §6) · `02126bf8` (648 B, plan in brief §10) ·
   `02126fb0` (656 B) · `0212a78c` (660 B) · `021288c8` (696 B) · `0212ac0c` (732 B) · `02129410`
   (1,316 B, needs a round of its own). Do not start one you cannot finish — a half-read 600-byte
   function costs the same budget and lands nothing.

There is also one **open measurement never taken**: how many of the sub-99% functions are actually
correct and merely carrying the known `bl func_ov003_*` `-0x8` symbol addend? That artefact appears
in ~100% of calls, including otherwise-perfect functions. The two sampled were both real
multi-instruction diffs in non-`bl` positions, so it explains none of those.

## Two lessons that cost real time — please do not relearn them

**A dead instruction in the original means a wrong prototype.** Not allocator residue, not a
register-choice artefact. It happened **eight times** in this overlay, and every one of those
write-offs was wrong. A dead instruction is a value computed and discarded, which is exactly what a
mis-declared arity produces.

**But a wrong register allocation does not mean a wrong prototype.** That distinction was only
learned after several rounds were wasted hunting prototypes in functions that were merely
misallocated. A register-allocation diff means the C has the wrong number of live locals, and the
fix is more or fewer declarations.

The class is now closed by argument, so you should not expect more of them: an arity mistake cannot
hide in a byte-exact function, because the register setup in front of each `bl` is part of the
compared bytes. A wrong *return type* is the invisible class, but every declaration where one could
hide belongs to a function not yet written, and writing the definition forces MWCC to reject the
mismatch.

Note also that `audit003.py` and `audit011.py` are **~90% false positive** and must not be used as a
signal. The heuristic marks r0–r3 read on first sight, but in almost every function here they are
first touched as the *destination of a call result*, long after arguments have moved to r4 or
higher. Confirm every hit by reading the callee's prologue by hand.

## Working method that has paid

- `fbdiff.py` is the signal. `OK` is raw byte-identical; `RELOCC` is identical once relocation bits
  are masked, and it is the reachable verdict — in a relocatable object every `bl` and literal-pool
  slot is a placeholder, so `OK` can essentially never be reached.
- Apply the brief's recipes; do not re-derive them. They are all measured, with model functions.
- When something resists, write it down in the brief with its address, what you tried, and its
  remaining diff, then move on. A written-not-exact function is a committed, resumable artifact.
- **Never leave the worktree dirty.** Commit per coherent group. If the pre-commit hook reformats,
  `git add` and commit again.

## Traps that have bitten, in brief

Full detail in `docs/ov011-progress.md`. The costly ones:

- **A stale `build/usa/delinks/**.o` mimics a code regression.** It is never a declared ninja
  output — it is a side effect of the delink rule. Suspect it before your C.
- **Headers are not tracked as dependencies of a TU.** A header-only edit leaves a stale `.o` and
  every measurement silently re-reports old numbers. Use `ovm.ps1`, not `ov.ps1`, in that case.
- **`sizeof(CombatSprite)` is 0x7D but the ROM strides sprites by 0x60.** A `CombatSprite[]` array
  puts every later field 0x38 too high.
- **A sub-struct whose size is not a multiple of 4 shifts every later `s32`.** Check sizes, not just
  offsets.
- **`dsd` renders register-shifted operand2 shifts at 2x the ARM field.** Read bytes, not text.
- **Always measure `-end 0x0212bdd8`.** Anything smaller truncates the range.
- **Use the edit tool for source edits.** A PowerShell `Get-Content`/`Set-Content` round-trip
  collapsed an entire source file onto one line and cost a third of a session.
- **PowerShell `[System.IO.File]` calls do not follow `cd`.** Use absolute paths or Python.
- Do **not** mark the overlay `complete` in `delinks.txt` — that is the user's call.

## House rules

C99, 4-space indent, pointer-left (`char* p`), 127-column lines. The pre-commit hook runs
clang-format and mixed-line-ending.

`E:\Git\twewy` is the main checkout and holds unrelated work in progress: **read-only**. Its
`build\usa\asm\ov011_4.s` is the ground-truth disassembly (114 functions) and
`build\usa\twewy_usa.nds` is the original ROM.

Worktrees: `decomp-ov011` in `E:\Git\twewy-ov011` (this one), plus `ov008`, `ov010`, `ov012`,
`ov013`, `ov015`. The four finished Noise overlays are byte-identical to the original ROM. Nothing
has been pushed and nothing is marked `complete`.

`build/` is gitignored, so **`build/scratch/` is untracked** — the tooling exists on this machine
but would not survive a fresh clone. Copy it from `E:\Git\twewy-ov010\build\scratch` when setting
up a new worktree.

## Report back

Function count byte-exact and the contiguous `.text` prefix. Whether the overlay links and what
`romcmp.py` reports. The address-order debt. Anything that contradicts `docs/ov011-progress.md` or
the brief. Be honest about diminishing returns rather than padding the report.
