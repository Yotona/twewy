# Overlay 13 (`Tsk_BtlEnm015_*`) decompilation — progress handoff

**Date:** 2026-09-25
**Branch:** `decomp-ov013` (off `decomp-ov008`)
**Worktree:** `E:\Git\twewy-ov013`
**Source:** `src/Combat/Noise/BtlEnm015/BtlEnm015.c`
**Header:** `include/Combat/Noise/Private/BtlEnm015.h`
**Config:** `config/usa/arm9/overlays/ov013/{delinks,symbols}.txt`

## Overlay summary

Overlay 13 is the enemy-015 combat overlay. It provides six EasyTask handlers:

| Task | Handler | Data size |
|------|---------|-----------|
| `Tsk_BtlEnm015_Eff` | `func_ov013_021260e8` | 0x70 |
| `Tsk_BtlEnm015_EffStamp` | `func_ov013_02126700` | 0x0C |
| `Tsk_BtlEnm015_EffStampSub` | `func_ov013_02126a30` | 0x74 |
| `Tsk_BtlEnm015_RG` | `func_ov013_02126dd8` | 0x1E8 |
| `Tsk_BtlEnm015_Shake` | `func_ov013_02126ff4` | 0x10 |
| `Tsk_BtlEnm015_UG` | `func_ov013_02127370` | 0x1EC |

38 functions total, `.text` = `0x021256c0`–`0x02127444`.

The original disassembly is at `E:\Git\twewy\build\usa\asm\ov013_4.s` (57 KB).

## Workflow

From `E:\Git\twewy-ov013`:

```powershell
ninja build/usa/src/Combat/Noise/BtlEnm015/BtlEnm015.o
ninja objdiff.json build/usa/delinks/src/Combat/Noise/BtlEnm015/BtlEnm015.o
.\objdiff-cli.exe diff -p . -u "src/Combat/Noise/BtlEnm015/BtlEnm015" -o build/ov_diff.json --format json
```

Parse `build/ov_diff.json`: `left.symbols[]` entries with `kind == "SYMBOL_FUNCTION"` have `match_percent`.

`config/usa/arm9/overlays/ov013/delinks.txt` currently maps `.text` from `0x021256c0` to `0x021261e8` and the whole `.rodata`/`.data`. **Extend the `.text` end offset whenever a new function is added**, since the mapping must be a contiguous prefix.

## Status (objdiff)

17 of 38 functions implemented. Exact (100%):

`021256d0`, `021256f0`, `021257a4`, `02125838`, `02125b64`, `02125e90`, `02125eb4`, `02125ed0`, plus `021256c0` (98.75) and `021256e4` (98.33) which are byte-identical and only differ in objdiff's relocation accounting.

Nonmatching (register allocation / scheduling), in rough priority:

| Function | % | Notes |
|----------|---|-------|
| `func_ov013_02125fd0` | 63.5 | Eff task init; struct/field order likely needs work |
| `func_ov013_02125b8c` | 77.5 | RG/UG init; register allocation across the anim build |
| `func_ov013_021258f0` | 97.1 | only `mov r2, r12, lsr #0x1f` scheduling differs |
| `func_ov013_02125efc` | 97.6 | RG/UG render helper |
| `func_ov013_021260e8` | 97.4 | Eff dispatcher |
| `func_ov013_02125d24` | 98.6 | jump-table switch now matches; minor scheduling |
| `func_ov013_02125a04` | 99.9 | nearly exact |

## Remaining functions to implement (address order)

`021261e8`, `02126254`, `021265b0`, `021266c8`, `02126700`, `02126788`, `021268d8`,
`02126950`, `02126960`, `02126a30`, `02126ab0`, `02126b30`, `02126c64`, `02126dd8`,
`02126ef0`, `02126f8c`, `02126ff4`, `021270a0`, `0212710c`, `02127230`, `02127370`.

## Useful symbol resolutions

Many ov003 helpers are unnamed; these are used by the implemented code:

- `0x02082f2c` = `CombatActor_PopPendingCommand` (declared in `CombatActor.h`)
- `0x02082b00` = `CombatSprite_Init`; `0x02082b0c` = `CombatSprite_Update`; `0x02082b64` = `CombatSprite_Render`
- `0x02082cc4` = `CombatSprite_Release`; `0x02082724` = `CombatSprite_SetPosition`; `0x02082730` (unnamed)
- `0x02082940` = `CombatSprite_InitAnim`; `0x02082998` = `CombatSprite_Load`; `0x02082a04` = `CombatSprite_LoadFromTable`
- `0x020824a0` = `CombatSprite_SetAnimFromTable` (multi-module alias; objdiff may show `Mini108_VBlank`)
- `0x020082f2c` area helpers `020c37f8`, `020c3c28`, `020c3c88`, `020c3efc`, `020c427c`, `020c4628`,
  `020c4668`, `020c4748`, `020c4830`, `020c495c`, `020c4ab4`, `020c4cc4`, `020c4e0c`, `020c5bfc`,
  `020c6230`, `020c72b4`, `020ccea8`, `020ccedc`, `020ccefc`, `020ccfec` (all `func_ov003_*`)
- `data_ov003_020e71b8` is `Ov003Global*` (`Combat.h`); task pools at `unk_00000` and `taskPool`
- `0x02082a04` projection helper cluster: `func_ov003_02084348`, `020843b0`, `020843ec`, `02084634`, `02084694`

## Notes

- The user cares primarily about **code sections matching** (objdiff function percentages), not the
  `.rodata`/`.data` layout.
- Overlay TUs are only linked from source when marked `complete` in `delinks.txt`; these are not, so the
  ROM still links the delinked original (i.e. `ninja sha1` passing does not validate this source).
- `data_ov013_02127640` / `data_ov013_02127644` are two separate `s32` BSS task-id slots.
- The `Tsk_BtlEnm015_*` symbols were renamed in `symbols.txt` to match the source task-handle names.
