# Main ASM Hotspots and Decompile Priority

Date: 2026-04-13

## Scope

This report covers disassembled-but-not-decompiled symbols defined in main-unit assembly under [build/usa/asm](build/usa/asm), including [build/usa/asm/main_*.s](build/usa/asm/main_*.s) and [build/usa/asm/src/main_*.s](build/usa/asm/src/main_*.s).

Filtering used:
- Included symbols starting with `func_`.
- Excluded symbols already marked `defined` in [tmp_func_xref.csv](tmp_func_xref.csv).
- Ranked by total BL/BLX callsites across all assembly files.

Generated artifacts:
- [tmp_main_unit_call_ranking.csv](tmp_main_unit_call_ranking.csv)
- [tmp_main_unit_undecompiled_top.csv](tmp_main_unit_undecompiled_top.csv)

## Counting Notes

- Some counts are inflated by mirrored files (for example `main_54.s` and `main_57.s`, and `main_44.s` and `unk_main_44.s`).
- Raw counts are still useful for hotspot ranking, but normalized counts are often about half in mirrored regions.

## Top Called Undecompiled Main-Unit Functions

1. `func_02026590` (275 calls), defined in [build/usa/asm/main_35.s](build/usa/asm/main_35.s#L42)
2. `func_020265d4` (138 calls), defined in [build/usa/asm/main_35.s](build/usa/asm/main_35.s#L69)
3. `func_0200d1d8` (90 calls), defined in [build/usa/asm/src/main_17.s](build/usa/asm/src/main_17.s#L3651)
4. `func_0200d858` (86 calls), defined in [build/usa/asm/src/main_17.s](build/usa/asm/src/main_17.s#L4125)
5. `func_0200d954` (76 calls), defined in [build/usa/asm/src/main_17.s](build/usa/asm/src/main_17.s#L4214)
6. `func_0203cd8c` (41 calls), defined in [build/usa/asm/main_47.s](build/usa/asm/main_47.s#L2037)
7. `func_020439d4` (40 calls), defined in [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5482)
8. `func_020437d4` (38 calls), defined in [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5317)
9. `func_02043844` (30 calls), defined in [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5355)
10. `func_02041d68` (28 calls), defined in [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L3223)
11. `func_02043960` (28 calls), defined in [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5438)
12. `func_02006930` (27 calls), defined in [build/usa/asm/main_7.s](build/usa/asm/main_7.s#L92)
13. `func_0203ab7c` (25 calls), defined in [build/usa/asm/main_46.s](build/usa/asm/main_46.s#L188)
14. `func_0203cd1c` (25 calls), defined in [build/usa/asm/main_47.s](build/usa/asm/main_47.s#L1999)
15. `func_020533f0` (24 calls), defined in [build/usa/asm/main_56.s](build/usa/asm/main_56.s#L5273)

## Behavior Guesses (Concrete Evidence)

### 1) func_02026590

Best guess: fixed-step smoothing/interpolation toward a target value.

Evidence:
- Body computes `(target - current) / step` and writes back to `current` ([build/usa/asm/main_35.s](build/usa/asm/main_35.s#L42)).
- Uses `_s32_div_f`, and clamps divisor to at least 1 (`movlo r2, #0x1`).

Example callsites:
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L18659)
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L26727)

### 2) func_020265d4

Best guess: variant of the same interpolation helper with a normalized/clamped divisor path.

Evidence:
- Calls `func_020265c0` before division ([build/usa/asm/main_35.s](build/usa/asm/main_35.s#L69)).
- Then performs `(target - current) / adjusted_step`, and accumulates into `current`.

Example callsites:
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L24747)
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L30521)

### 3) func_0200d1d8

Best guess: register/configure a BG tile transfer job in a global slot table.

Evidence:
- Writes a job pointer into `data_0206b3e8` indexed by engine/layer ([build/usa/asm/src/main_17.s](build/usa/asm/src/main_17.s#L3651)).
- Initializes multiple fields in a job struct (`+0x8`, `+0xC`, `+0x10`, `+0x14`, `+0x1C`, `+0x20`, `+0x24`).
- Immediately calls `func_0200d858` to set flags/default source.

Example callsites:
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L19768)
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L20854)

### 4) func_0200d858

Best guess: set transfer mode bits and fallback resource pointer for a BG transfer job.

Evidence:
- Explicitly toggles bits `0x4` and `0x8` in flags word at `job+0x8` ([build/usa/asm/src/main_17.s](build/usa/asm/src/main_17.s#L4125)).
- If pointer arg is null, stores default `data_0205a128` into `job+0x18`.

Example callsites:
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L19773)
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L20864)

### 5) func_0200d954

Best guess: unregister/clear BG transfer job slot.

Evidence:
- Writes zero into `data_0206b3e8[engine][slot]` ([build/usa/asm/src/main_17.s](build/usa/asm/src/main_17.s#L4214)).
- Very small function and used as cleanup counterpart in same call families.

Example callsites:
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L20307)
- [build/usa/asm/ov003_4.s](build/usa/asm/ov003_4.s#L20963)

### 6) func_0203cd8c

Best guess: enqueue IPC/FIFO command word to ARM7/IPC register with IRQ-safe handshake.

Evidence:
- Writes command word to `0x4000184` and checks status bits before enqueue ([build/usa/asm/main_47.s](build/usa/asm/main_47.s#L2037)).
- Uses `OS_DisableIRQ`/`OS_RestoreIRQ` around write.
- Returns negative error values for busy/failure paths.

Example callsites:
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L1004)
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L1085)

### 7) func_0203cd1c

Best guess: register callback/handler bit for IPC events.

Evidence:
- Stores handler pointer in callback array `data_0207fc08[index]` and updates a bitmask in `0x27ffc00+0x388` ([build/usa/asm/main_47.s](build/usa/asm/main_47.s#L1999)).
- Protected by IRQ disable/restore.

Example callsites:
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L859)
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L2681)

### 8) func_02043844

Best guess: build and submit a command packet to a shared IPC queue/buffer.

Evidence:
- Acquires packet buffer via `func_020437ec`, fills fields, cleans cache, and submits via `func_0203cd8c` ([build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5355)).
- Uses status-style return codes `0x2` and `0x8`.

Example callsites:
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L6219)
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L6381)

### 9) func_020437d4 / func_02043960 / func_020439d4 cluster

Best guess: command channel state accessors and request path over shared state block `data_02080700`.

Evidence:
- `func_020437d4` writes callback/entry slots (`base + idx*4 + 0x18`) ([build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5317)).
- `func_02043960` returns channel base pointer from that same state block ([build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5438)).
- `func_020439d4` performs state checks, reads small status struct, and writes arguments into command payload ([build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5482)).

Example callsites:
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L6252)
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L5976)

### 10) func_02041d68

Best guess: packed BCD/decimal digit unpack to integer with validation.

Evidence:
- Verifies each 4-bit nibble is less than 10, else returns 0 ([build/usa/asm/main_54.s](build/usa/asm/main_54.s#L3223)).
- Accumulates value using base-10 multiplier loop.

Example callsites:
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L2924)
- [build/usa/asm/main_54.s](build/usa/asm/main_54.s#L2954)

### 11) func_0203ab7c

Best guess: generalized DMA transfer wrapper to hardware DMA engine.

Evidence:
- Performs size preprocessing, waits on DMA busy flag, then dispatches to low-level transfer helper ([build/usa/asm/main_46.s](build/usa/asm/main_46.s#L188)).
- Closely related to `func_0203abec` and `func_0203aafc` DMA variants in same file.

Example callsites:
- [build/usa/asm/main_42.s](build/usa/asm/main_42.s#L1074)
- [build/usa/asm/main_42.s](build/usa/asm/main_42.s#L1428)

## Decompile Priority (Practical Order)

Priority was scored by call frequency, cross-module centrality, and dependency-unblock potential.

1. `func_0203cd8c` + `func_0203cd1c` + nearby IPC helpers (`main_47` cluster)
- High fan-in from `main_54`, appears central for command dispatch and callback wiring.
- Clear boundaries and easy hardware-register validation.

2. `func_02043844`, `func_020439d4`, `func_020437d4`, `func_02043960` (`main_54` command path cluster)
- High usage and tightly connected to IPC submission status.
- Decompiling together preserves semantics and avoids partial misnaming.

3. `func_0200d1d8`, `func_0200d858`, `func_0200d954` (`main_17` BG transfer control cluster)
- Very high call counts and likely in frame-critical rendering paths.
- Good immediate readability impact for BG update pipeline.

4. `func_0203ab7c` (and related DMA wrappers in `main_46`)
- Broad reuse from graphics-transfer code.
- Good candidate for well-scoped, high-confidence reverse.

5. `func_02026590` + `func_020265d4` (`main_35` numeric smoothing helpers)
- Highest raw call counts, likely low complexity.
- Great quick-win for naming and readability in many gameplay/UI loops.

6. `func_02041d68`
- Medium use, very high confidence semantics.
- Excellent low-risk cleanup target.

## Suggested Next Implementation Pass

1. Start with `main_47` IPC cluster and create typed signatures.
2. Then decompile `main_54` command wrappers using shared state struct reconstruction around `data_02080700`.
3. Validate by matching status-code behavior (`0x2`, `0x3`, `0x8`) and command-payload writes.
