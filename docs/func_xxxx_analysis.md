# `func_XXXX` analysis for `Savefile.c` and `Inventory.c`

## Scope and method

This document analyzes `func_XXXX`-style symbols in:

- `src/Savefile.c`
- `src/Inventory.c`

Cross-references used:

- C callsites in `src/**`
- Disassembly in `build/usa/asm/**` (especially `build/usa/asm/src/Savefile.s` and `build/usa/asm/src/Inventory.s`)

I also generated a symbol usage matrix at `tmp_func_xref.csv` (definition type, C refs, asm refs, top files).

---

## High-confidence findings (Savefile pipeline)

### Save validation / load path

- `func_02024724(slotOffset)`
  - Reads primary save block (`0x3444`) from `slotOffset` (`0` or `0x5470`), verifies signature and checksum.
  - On success, copies `unk_1AB6` flag from loaded data.
  - Return semantics: `0=ok`, `1=signature/format fail`, `8=checksum fail`.

- `func_020247f4()`
  - Validates primary save by trying slot A (`0`) then slot B (`0x5470`).
  - Locking + card session wrapper around calls to lower-level card APIs.
  - Return semantics include `2` for hardware/session errors.

- `func_020248e4(slotOffset)` / `func_0202499c()`
  - Same structure as above for backup/friend block (`0x202C`) at offsets `0x3444` and `0x88B4`.
  - `func_0202499c()` additionally checks bit 0 of `unk_1AB6` before treating one-sided backup recovery as valid.

- `func_02024aa4()`
  - Top-level Ågvalidate both save domainsÅh routine.
  - Temporarily clears two `SystemStatusFlags` bits, runs both validations, then restores them.
  - Used from title/opening flow (`OpenEnd_ValidateSaveData`).

### Save write/read state machine

The table `data_0205c2a4[13]` drives a 13-step save process (`func_02025444` dispatcher):

1. `func_02024ca4` init/reset phase
2. `func_02024f00` build+prepare primary block
3. `func_02024fc0` write primary slot A
4. `func_020250e8` wait completion
5. `func_02025058` write primary slot B
6. `func_020250e8` wait completion
7. `func_02025138` close/measure
8. `func_020251d8` build+prepare backup block
9. `func_0202529c` write backup slot A
10. `func_0202533c` write backup slot B
11. `func_020253cc` wait completion
12. `func_02025180` close/measure
13. `func_02024ccc` finalize/restore status flags

Observed from UI/task code:

- `OtosuMenu.c` polls `func_02025444()` and then checks `func_020258ac()` status bits.
- This strongly suggests `func_02025444` should conceptually behave like a step executor (often treated as returning completion in gameplay code, even if currently typed `void` in C).

### Direct load helpers

- `func_020255bc()` loads primary save payload (`MainData`) from either slot.
- `func_0202579c(dst)` loads backup/friend payload from either slot into destination buffer.
- `func_020256bc()` is a wrapper that invokes `func_020255bc()`.

---

## High-confidence findings (checksum and format helpers)

- `func_020258bc(buf, signature, sigLen, payloadLen)`
  - Stage 1: byte-compare signature (`sigLen` bytes).
  - Stage 2: verify 4 packed size nibbles/bytes stored at signature tail + `0x1B` offset logic.
  - Returns `1` on full match, `0` on mismatch.

- `func_0202593c(data, size)`
  - OneÅfs-complement checksum over block (`Internet-style` fold to 16-bit and bitwise not).

Potential decomp mismatch note:

- In `src/Savefile.c`, `func_020258bc` currently computes `actual` as `*(u8*)(arg0[i] + 27)`, which dereferences an address derived from a byte value.
- Disassembly (`build/usa/asm/src/Savefile.s`, `func_020258bc`) shows it should read from `arg0 + i + 0x1B`.
- This appears to be a known nonmatching artifact, not intended final logic.

---

## External save I/O helpers used by `Savefile.c`

These are not defined in `Savefile.c`/`Inventory.c`, but usage patterns are consistent and strong:

- `func_0204237c(lockId)` / `func_0204238c(lockId)`
  - Enter/leave save/card critical section using lock ID.

- `func_020389c0(lockId)`
  - Releases lock resource after save/card session.

- `func_0204296c(0x1001)`
  - Initializes/selects card operation mode before I/O.

- `func_02042944()`
  - Returns card/session readiness state (`==1` expected before I/O).

- `func_0204285c(...)`
  - Main read/write command submission function (offset, buffer, size, mode flags).

- `func_02042aa4()`
  - Wait/sync after read commands (used in immediate read+validate paths).

- `func_02042ab0()`
  - Poll completion for async writes.

- `func_02042330()`
  - Last-operation status/error query.

- `func_0203a444()`
  - Timestamp/cycle source used to measure save operation duration.

---

## Inventory-side unknown helper functions (currently stubs in C)

These are `static` helpers in `Inventory.c` with placeholder C bodies but full asm behavior in `build/usa/asm/src/Inventory.s`.

### Confirmed from asm

- `func_02022284(void*)`
  - Complex table-driven initializer for `MainData.unk_16D0` region.
  - Uses data tables `data_0205c39c` and `data_0205c190`.

- `func_02022424(s32*)`
  - Zeroes 35 `s32` entries (`0x23` words).

- `func_02022488(void*)`
  - Initializes a structured record with:
    - bitfield cleanup/setup in first word,
    - sentinel `0xFFFF` for several arrays,
    - clears additional fixed-size slots.

- `func_020224f4(void*)`
  - Initializes 50 records (`0x32`) of `0x0C` bytes:
    - first 6 bytes = `0xFF`,
    - trailing 4-byte value = `0`.

- `func_02022534(void*)`
  - Clears 3 bytes and one halfword (small struct reset).

- `func_0202254c(void*)`
  - Clears one byte + one word.

- `func_0202255c(u8*)`
  - Clears bits 0 and 1 for 280 bytes (`0x118`).

- `func_0202258c(void*)`
  - Copies a 7-byte table from `data_0205c19d` into two 7-byte spans (`dst[0..6]` and `dst[7..13]`).

- `func_020225c4(void*)`
  - Initializes 96 records (`0x60`) of 4 bytes each to zero (`u8,u8,u16`).

- `func_020225ec(void*)`
  - Zeroes 96 words (`0x60 * 4`).

- `func_02022640(void*)`
  - Major profile bootstrap routine:
    - clears/normalizes subfields,
    - reads firmware user settings,
    - resets experience/player/friend/equipped pin structures,
    - writes key sentinels (`0xFFFF`) and defaults.

### Why this matters

`Savefile_ResetAllGameplay()` depends on these helpers. As long as they remain placeholders in C, gameplay reset behavior may compile but not match retail behavior exactly.

---

## Cross-file callsite highlights

- `OpenEnd.c`
  - `func_02024aa4()` used for title/startup save validation.
  - `func_020256bc()` used for continue/load path decision.
  - `func_02024d04()` used to reset state after validation branch.

- `OtosuMenu.c`
  - save flow polls `func_02025444()` and reads result bits via `func_020258ac()`.

- `main.c` and `OpenEnd.c`
  - `func_02025b1c()` called during early init/new game reset paths.

---

## Reasoning behind each major guess

This section explains *why* each role guess was made, with concrete evidence patterns.

### High-traffic functions (called widely in decompiled/asm code)

- `func_02023208` (very high traffic: `asmRefs=108`)
  - Guess: Åghas-at-least-N copiesÅh inventory predicate with optional level gate.
  - Reasoning:
    - In C body, both pin and non-pin paths accumulate an `owned` count and return `(arg1 <= owned)`.
    - For pins, check includes `arg2 <= pin level` in both equipped and stockpile arrays.
    - Called from save/progress logic to gate conditions, consistent with ownership check API.
  - Confidence: **high**.

- `func_02023010` (`asmRefs=81`)
  - Guess: Ågcount total owned copies of item ID across all storage/equipped domains.Åh
  - Reasoning:
    - C implementation sums ownership from equipped decks, stockpile, mastered pins (or thread equip + stock for non-pins).
    - Callers use it as numeric count (e.g., comparisons against constants, not pure bool checks).
    - Broad menu/shop/save usage in asm is consistent with a central ownership counter.
  - Confidence: **high**.

- `func_02023d1c` (`asmRefs=44`)
  - Guess: Ågquery event/collection bitflag by index.Åh
  - Reasoning:
    - C body directly checks a bit in `unk_2324` using `(1 << arg0)` and returns 0/1.
    - Paired with setter `func_02023d00`, strongly indicating set/query semantics.
    - High usage in menu/report overlays matches a generic unlock/seen-flag checker.
  - Confidence: **high**.

- `func_0202599c` (`asmRefs=39`)
  - Guess: Ågquery story/progress bit in `unk_246C`.Åh
  - Reasoning:
    - Current C is a direct bit-test against `unk_246C` with `1LL << arg0`.
    - High overlay usage indicates a global progression gate helper.
    - Adjacent functions in same region (`func_020259e4`, `func_02025a2c`) also perform bitfield-driven gating.
  - Confidence: **medium-high** (bitfield width/sign details may still need nonmatching cleanup).

- `func_02042330` (`asmRefs=32`, `srcRefs=13`)
  - Guess: Åglast save/card operation result.Åh
  - Reasoning:
    - Called immediately after I/O submit/wait calls.
    - Return value branches to error code paths (`2` etc.) throughout save state machine.
    - Always treated as status, never as payload.
  - Confidence: **high**.

- `func_0204285c` + `func_02042944` (`asmRefs=24` each)
  - Guesses:
    - `func_0204285c`: submit read/write command.
    - `func_02042944`: readiness/media-present status.
  - Reasoning:
    - `func_02042944()==1` gates all real card access.
    - `func_0204285c` receives offset/buffer/size and mode-like flags, then code polls for completion/status.
    - Paired usage pattern repeats across primary/backup read/write paths.
  - Confidence: **high**.

- `func_020250e8` / `func_020253cc` (`asmRefs=20` each)
  - Guess: async write-completion steps in save state machine.
  - Reasoning:
    - Both call completion poll (`func_02042ab0`), then map `func_02042330` result into error flags.
    - Their position in `data_0205c2a4` table is exactly after write-submit steps.
  - Confidence: **high**.

- `func_0202593c` (`asmRefs=20`, `srcRefs=8`)
  - Guess: oneÅfs-complement 16-bit checksum.
  - Reasoning:
    - Disassembly shows additive fold to 16 bits, then bitwise NOT.
    - Used both before write (store checksum) and after read (verify checksum).
  - Confidence: **high**.

- `func_020246d4`/`func_0204237c`/`func_0204296c` (all `asmRefs=20`)
  - Guesses:
    - `func_020246d4`: aligned temp-buffer allocator for save blocks.
    - `func_0204237c`: enter lock-protected card session.
    - `func_0204296c`: initialize card mode/session (`0x1001`).
  - Reasoning:
    - `func_020246d4` asm directly calls `Mem_AllocHeapTail`, aligns size, tags sequence.
    - `func_0204237c` is always called after `OS_GetLockID`; paired with `func_0204238c` and `func_020389c0`.
    - `func_0204296c(0x1001)` appears as fixed preamble before access checks.
  - Confidence: **high**.

### Save validation / load helper guesses (core but less globally called)

- `func_02024724` / `func_020248e4`
  - Guess: load+verify one slot for primary (`0x3444`) or backup (`0x202C`) block.
  - Reasoning: identical sequence of read -> signature check (`func_020258bc`) -> checksum check (`func_0202593c`) -> mapped return code.
  - Confidence: **high**.

- `func_020247f4` / `func_0202499c`
  - Guess: Ågtry both redundant slots and choose valid source/error class.Åh
  - Reasoning: explicit fallback from slot A to slot B with tri-state returns (`0/1/2/8`) and consistency branches.
  - Confidence: **high**.

- `func_02024aa4`
  - Guess: top-level dual-domain save validation wrapper.
  - Reasoning: only routine that calls both primary and backup validators while toggling two `SystemStatusFlags` bits around the operation.
  - Confidence: **high**.

- `func_020255bc` / `func_0202579c` / `func_020256bc`
  - Guess: concrete loaders used by gameplay/menus.
  - Reasoning:
    - `OpenEnd_ContinueGame` calls `func_020256bc` as Ågcan continue?Åh gate.
    - `func_020256bc` is a direct wrapper over `func_020255bc`.
    - `func_0202579c` mirrors same dual-slot logic for backup/friend buffer destination.
  - Confidence: **high**.

### Inventory stub-helper guesses (asm-confirmed behavior)

These guesses are mostly direct transcriptions from `build/usa/asm/src/Inventory.s`, so confidence is high:

- `func_02022424`: loop writes 35 zero words (`cmp r2, #0x23`).
- `func_02022488`: clears multiple bitfields + fills sentinel arrays with `0xFFFF`.
- `func_020224f4`: 50 records, bytes `0..5` to `0xFF`, word at `+8` to `0`.
- `func_02022534`: tiny struct clear (3 bytes + 1 halfword).
- `func_0202254c`: tiny struct clear (1 byte + 1 word).
- `func_0202255c`: per-byte mask clear of bits `0` and `1` for 280 entries.
- `func_0202258c`: two 7-byte template copies from `data_0205c19d`.
- `func_020225c4`: 96 records of `(u8,u8,u16)=0`.
- `func_020225ec`: 96 dwords to zero.
- `func_02022640`: profile bootstrap (firmware settings + reset of nested stats/inventory/profile structures).
- `func_02022284`: table-driven initialization using `data_0205c39c` and `data_0205c190`.

---

## Suggested documentation priority list

Prioritized by: **(a)** call density in decompiled/asm code, **(b)** gameplay/save criticality, **(c)** ambiguity risk.

### Priority 0 (document first: core semantics)

1. `func_02023208` ? ubiquitous ownership predicate; high risk if misunderstood.
2. `func_02023010` ? central ownership count used by systems/UI.
3. `func_02042330` ? error/status root for save outcomes.
4. `func_0204285c` ? main save I/O submit primitive.
5. `func_02042944` ? readiness gate for all card operations.
6. `func_02024aa4` ? title/startup save validation entrypoint.
7. `func_020255bc` ? main save loader used by continue flow.
8. `func_0202579c` ? backup/friend data loader for redundancy path.

### Priority 1 (document next: save state machine + lock/session contract)

9. `func_020250e8` and `func_020253cc` ? async completion/error mapping.
10. `func_020247f4` and `func_0202499c` ? dual-slot validation strategy.
11. `func_020258bc` and `func_0202593c` ? integrity checks (format + checksum).
12. `func_0204237c` / `func_0204238c` / `func_020389c0` ? lock/session lifecycle.
13. `func_0204296c` / `func_02042aa4` / `func_02042ab0` ? card op mode + sync/poll.

### Priority 2 (document after: profile reset internals and table-driven init)

14. `func_02022640` ? broad profile bootstrap behavior.
15. `func_02022284` ? opaque table-driven block init.
16. `func_02022488` / `func_020224f4` ? structured sentinel initializers.
17. `func_0202258c` / `func_020225c4` / `func_020225ec` ? repeated template/zeroing helpers.
18. `func_02022534` / `func_0202254c` / `func_0202255c` / `func_02022424` ? small reset primitives.

### Priority 3 (nice-to-have cleanup docs)

19. `func_0202599c` / `func_020259e4` / `func_02025a2c` ? progress bitfield helpers with known nonmatching aspects.
20. `func_02023480` / `func_020243d4` / `func_02024434` ? formula/table helpers used by UI/economy paths.

---

## Suggested rename candidates (analysis-only)

These names are suggested for readability; not applied in code here.

Each row includes short reasoning so rename intent is auditable.

- `func_02024aa4` Å® `Save_ValidatePrimaryAndBackup`
  - Why: orchestrates both validator branches and combines return state.
- `func_020255bc` Å® `Save_LoadMainDataWithRedundancy`
  - Why: tries redundant slots for `MainData` payload.
- `func_0202579c` Å® `Save_LoadFriendDataWithRedundancy`
  - Why: mirrored redundant load for backup/friend block.
- `func_020258bc` Å® `Save_VerifySignatureAndLengthTag`
  - Why: validates magic signature plus encoded length tag bytes.
- `func_0202593c` Å® `Save_ComputeChecksum16`
  - Why: computes folded 16-bit one's-complement checksum.
- `func_02022424` Å® `Reset_ClearIntArray35`
  - Why: exact asm loop clears 35 words.
- `func_020225c4` Å® `Reset_ClearUnkSmallStructArray96`
  - Why: asm writes zeroed `(u8,u8,u16)` records for 96 entries.
- `func_02022640` Å® `Reset_InitializeProfileBlock`
  - Why: composes multiple resets + firmware-derived defaults into one profile init pass.

---

## Notes

- The broad symbol cross-reference matrix used for this report is available in `tmp_func_xref.csv`.
- For low-level confirmation, inspect:
  - `build/usa/asm/src/Savefile.s`
  - `build/usa/asm/src/Inventory.s`
