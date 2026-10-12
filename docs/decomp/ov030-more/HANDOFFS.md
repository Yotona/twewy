# Additional ov030 handoffs

Branch: `delink-ov030-more`, based directly on main `2b8fcfedc8aca72cdfa61b1de3375963f1f13e38`.
Excluded field work: `decomp-ov030-field` at `963b8c704b6233064eb1ec0196805d48e949c7ed`.

21 new incomplete USA TUs / 181 functions. Original delinked assembly remains linked; these are preparation stubs, not decompiled implementations. JP configuration is unchanged.

Use one prompt per LLM in its own branch/worktree from this branch. Each prompt owns one source file. Shared headers, symbols and configuration are coordination points: collect proposed shared changes for a single integrator rather than applying conflicting edits. Commit only the assigned source and any explicitly coordinated changes.

Ranges below are half-open. All new code, rodata and data ranges were checked against main, each other, and the pinned field branch. No BSS is assigned. Boundaries are working hypotheses, especially C-rated helper cuts; non-overlap does not prove original TU or data ownership. See [boundary evidence](boundary-evidence.md) and the machine-readable [manifest](manifest.json).

Run `python tools/check_ov030_more.py` to repeat the ownership/function checks. Setup: `python tools/configure.py --version usa --no-extract`; on this Windows checkout use `--dsd ./dsd.exe --compiler tools/mwccarm` to use the installed tool explicitly. Then `ninja dis` produces assembly, and `ninja` builds/checks the USA ROM. Use the configured objdiff unit names from objdiff.json; do not assume an old gap assembly filename.

The earlier boundary analysis proposed conflicting data windows. This branch uses only the nonconflicting adoption ranges, with uncertain BSS/global ownership left in generated gaps. Do not move or claim those gaps without an independent review.

| TU | Functions | Boundary confidence |
|---|---:|---|
| `Fld_TalkEventCursor` | 9 | edge |
| `Fld_TalkEventDecide` | 9 | A+ |
| `Fld_TalkEventTitle` | 8 | A+ |
| `Fld_MapJumpName` | 12 | A+ |
| `Fld_MapName` | 10 | A+ |
| `Field_020c5404` | 3 | C |
| `Fld_NSymbolUGAppear` | 8 | A+ |
| `Fld_Piranha` | 13 | C |
| `Field_020c5f70` | 8 | A+ |
| `Field_020c683c` | 3 | C |
| `Fld_TouchNext` | 8 | C |
| `Fld_GetPlate` | 8 | A+ |
| `Fld_BattleIn` | 8 | edge |
| `Fld_Brush` | 10 | A+ |
| `Fld_BrushHati` | 8 | A+ |
| `Field_020c91f4` | 11 | A+ |
| `Task_Mess` | 10 | edge |
| `Field_020ca4bc` | 10 | A+ |
| `Task_Window` | 4 | C |
| `LocaTitle` | 7 | C |
| `XFade` | 14 | C |

## 1. Fld_TalkEventCursor

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_TalkEventCursor.c (USA ov030, 9 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: edge (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c420c-0x020c45ec
  .rodata: 0x020e7218-0x020e7260
  .data: 0x021075d8-0x021075f0
Functions in original order:
  func_ov030_020c420c (0x020c420c, size 0xdc)
  func_ov030_020c42e8 (0x020c42e8, size 0xa4)
  func_ov030_020c438c (0x020c438c, size 0xf8)
  func_ov030_020c4484 (0x020c4484, size 0x4c)
  func_ov030_020c44d0 (0x020c44d0, size 0x34)
  func_ov030_020c4504 (0x020c4504, size 0x34)
  func_ov030_020c4538 (0x020c4538, size 0x34)
  func_ov030_020c456c (0x020c456c, size 0x48)
  func_ov030_020c45b4 (0x020c45b4, size 0x38)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 2. Fld_TalkEventDecide

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_TalkEventDecide.c (USA ov030, 9 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c45ec-0x020c4850
  .rodata: 0x020e7260-0x020e72a8
  .data: 0x021075f0-0x02107608
Functions in original order:
  func_ov030_020c45ec (0x020c45ec, size 0x9c)
  func_ov030_020c4688 (0x020c4688, size 0x84)
  func_ov030_020c470c (0x020c470c, size 0x2c)
  func_ov030_020c4738 (0x020c4738, size 0x28)
  func_ov030_020c4760 (0x020c4760, size 0x14)
  func_ov030_020c4774 (0x020c4774, size 0x14)
  func_ov030_020c4788 (0x020c4788, size 0x48)
  func_ov030_020c47d0 (0x020c47d0, size 0x40)
  func_ov030_020c4810 (0x020c4810, size 0x40)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 3. Fld_TalkEventTitle

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_TalkEventTitle.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c4850-0x020c4a78
  .rodata: 0x020e72a8-0x020e72f0
  .data: 0x02107608-0x02107634
Functions in original order:
  func_ov030_020c4850 (0x020c4850, size 0x9c)
  func_ov030_020c48ec (0x020c48ec, size 0x6c)
  func_ov030_020c4958 (0x020c4958, size 0x38)
  func_ov030_020c4990 (0x020c4990, size 0x48)
  func_ov030_020c49d8 (0x020c49d8, size 0x14)
  func_ov030_020c49ec (0x020c49ec, size 0x14)
  func_ov030_020c4a00 (0x020c4a00, size 0x48)
  func_ov030_020c4a48 (0x020c4a48, size 0x30)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 4. Fld_MapJumpName

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_MapJumpName.c (USA ov030, 12 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c4a78-0x020c5088
  .rodata: 0x020e72f0-0x020e7324
  .data: 0x02107634-0x02107648
Functions in original order:
  func_ov030_020c4a78 (0x020c4a78, size 0x70)
  func_ov030_020c4ae8 (0x020c4ae8, size 0xa4)
  func_ov030_020c4b8c (0x020c4b8c, size 0x94)
  func_ov030_020c4c20 (0x020c4c20, size 0x9c)
  func_ov030_020c4cbc (0x020c4cbc, size 0x20)
  func_ov030_020c4cdc (0x020c4cdc, size 0xa4)
  func_ov030_020c4d80 (0x020c4d80, size 0x15c)
  func_ov030_020c4edc (0x020c4edc, size 0x8)
  func_ov030_020c4ee4 (0x020c4ee4, size 0xb4)
  func_ov030_020c4f98 (0x020c4f98, size 0x3c)
  func_ov030_020c4fd4 (0x020c4fd4, size 0x48)
  func_ov030_020c501c (0x020c501c, size 0x6c)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 5. Fld_MapName

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_MapName.c (USA ov030, 10 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c5088-0x020c5404
  .rodata: 0x020e7324-0x020e7380
  .data: 0x02107648-0x02107658
Functions in original order:
  func_ov030_020c5088 (0x020c5088, size 0x9c)
  func_ov030_020c5124 (0x020c5124, size 0x84)
  func_ov030_020c51a8 (0x020c51a8, size 0xdc)
  func_ov030_020c5284 (0x020c5284, size 0x8)
  func_ov030_020c528c (0x020c528c, size 0x68)
  func_ov030_020c52f4 (0x020c52f4, size 0x38)
  func_ov030_020c532c (0x020c532c, size 0x48)
  func_ov030_020c5374 (0x020c5374, size 0x38)
  func_ov030_020c53ac (0x020c53ac, size 0x3c)
  func_ov030_020c53e8 (0x020c53e8, size 0x1c)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 6. Field_020c5404

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Field_020c5404.c (USA ov030, 3 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c5404-0x020c548c
  .rodata: 0x020e7380-0x020e7418
Functions in original order:
  func_ov030_020c5404 (0x020c5404, size 0x2c)
  func_ov030_020c5430 (0x020c5430, size 0x30)
  func_ov030_020c5460 (0x020c5460, size 0x2c)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 7. Fld_NSymbolUGAppear

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_NSymbolUGAppear.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c548c-0x020c5744
  .rodata: 0x020e7418-0x020e7460
  .data: 0x02107658-0x02107670
Functions in original order:
  func_ov030_020c548c (0x020c548c, size 0xac)
  func_ov030_020c5538 (0x020c5538, size 0xc8)
  func_ov030_020c5600 (0x020c5600, size 0x2c)
  func_ov030_020c562c (0x020c562c, size 0x68)
  func_ov030_020c5694 (0x020c5694, size 0x14)
  func_ov030_020c56a8 (0x020c56a8, size 0x14)
  func_ov030_020c56bc (0x020c56bc, size 0x48)
  func_ov030_020c5704 (0x020c5704, size 0x40)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 8. Fld_Piranha

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_Piranha.c (USA ov030, 13 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c59b0-0x020c5f70
  .rodata: 0x020ea438-0x020eae80
  .data: 0x02108464-0x02108490
Functions in original order:
  func_ov030_020c59b0 (0x020c59b0, size 0x4c)
  func_ov030_020c59fc (0x020c59fc, size 0x2c)
  func_ov030_020c5a28 (0x020c5a28, size 0xa8)
  func_ov030_020c5ad0 (0x020c5ad0, size 0x108)
  func_ov030_020c5bd8 (0x020c5bd8, size 0x94)
  func_ov030_020c5c6c (0x020c5c6c, size 0x20)
  func_ov030_020c5c8c (0x020c5c8c, size 0x190)
  func_ov030_020c5e1c (0x020c5e1c, size 0x58)
  func_ov030_020c5e74 (0x020c5e74, size 0x14)
  func_ov030_020c5e88 (0x020c5e88, size 0x48)
  func_ov030_020c5ed0 (0x020c5ed0, size 0x38)
  func_ov030_020c5f08 (0x020c5f08, size 0x54)
  func_ov030_020c5f5c (0x020c5f5c, size 0x14)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 9. Field_020c5f70

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Field_020c5f70.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c5f70-0x020c683c
  .rodata: 0x020eae80-0x020ec824
Functions in original order:
  func_ov030_020c5f70 (0x020c5f70, size 0x90)
  func_ov030_020c6000 (0x020c6000, size 0xa4)
  func_ov030_020c60a4 (0x020c60a4, size 0xac)
  func_ov030_020c6150 (0x020c6150, size 0xa0)
  func_ov030_020c61f0 (0x020c61f0, size 0x110)
  func_ov030_020c6300 (0x020c6300, size 0xc8)
  func_ov030_020c63c8 (0x020c63c8, size 0xd0)
  func_ov030_020c6498 (0x020c6498, size 0x3a4)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 10. Field_020c683c

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Field_020c683c.c (USA ov030, 3 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c683c-0x020c6c58
  .rodata: 0x020ec824-0x020ec844
Functions in original order:
  func_ov030_020c683c (0x020c683c, size 0x28)
  func_ov030_020c6864 (0x020c6864, size 0x240)
  func_ov030_020c6aa4 (0x020c6aa4, size 0x1b4)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 11. Fld_TouchNext

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_TouchNext.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c6c58-0x020c6e90
  .rodata: 0x020ec844-0x020ec88c
  .data: 0x02108490-0x021084a4
Functions in original order:
  func_ov030_020c6c58 (0x020c6c58, size 0x9c)
  func_ov030_020c6cf4 (0x020c6cf4, size 0x8c)
  func_ov030_020c6d80 (0x020c6d80, size 0x34)
  func_ov030_020c6db4 (0x020c6db4, size 0x20)
  func_ov030_020c6dd4 (0x020c6dd4, size 0x20)
  func_ov030_020c6df4 (0x020c6df4, size 0x20)
  func_ov030_020c6e14 (0x020c6e14, size 0x48)
  func_ov030_020c6e5c (0x020c6e5c, size 0x34)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 12. Fld_GetPlate

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_GetPlate.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c6e90-0x020c71b0
  .rodata: 0x020ec88c-0x020ec8d4
  .data: 0x021084a4-0x021084b8
Functions in original order:
  func_ov030_020c6e90 (0x020c6e90, size 0x9c)
  func_ov030_020c6f2c (0x020c6f2c, size 0x74)
  func_ov030_020c6fa0 (0x020c6fa0, size 0xe0)
  func_ov030_020c7080 (0x020c7080, size 0x40)
  func_ov030_020c70c0 (0x020c70c0, size 0x60)
  func_ov030_020c7120 (0x020c7120, size 0x14)
  func_ov030_020c7134 (0x020c7134, size 0x48)
  func_ov030_020c717c (0x020c717c, size 0x34)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 13. Fld_BattleIn

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_BattleIn.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: edge (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c85a4-0x020c8868
  .rodata: 0x020eca64-0x020ecab4
  .data: 0x021085c8-0x021085dc
Functions in original order:
  func_ov030_020c85a4 (0x020c85a4, size 0xe4)
  func_ov030_020c8688 (0x020c8688, size 0xe0)
  func_ov030_020c8768 (0x020c8768, size 0x2c)
  func_ov030_020c8794 (0x020c8794, size 0x14)
  func_ov030_020c87a8 (0x020c87a8, size 0x14)
  func_ov030_020c87bc (0x020c87bc, size 0x14)
  func_ov030_020c87d0 (0x020c87d0, size 0x48)
  func_ov030_020c8818 (0x020c8818, size 0x50)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 14. Fld_Brush

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_Brush.c (USA ov030, 10 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c8a58-0x020c900c
  .rodata: 0x020ecafc-0x020ecb88
  .data: 0x021085f0-0x02108600
Functions in original order:
  func_ov030_020c8a58 (0x020c8a58, size 0x88)
  func_ov030_020c8ae0 (0x020c8ae0, size 0x58)
  func_ov030_020c8b38 (0x020c8b38, size 0x58)
  func_ov030_020c8b90 (0x020c8b90, size 0x194)
  func_ov030_020c8d24 (0x020c8d24, size 0x30)
  func_ov030_020c8d54 (0x020c8d54, size 0x1f8)
  func_ov030_020c8f4c (0x020c8f4c, size 0x8)
  func_ov030_020c8f54 (0x020c8f54, size 0x40)
  func_ov030_020c8f94 (0x020c8f94, size 0x48)
  func_ov030_020c8fdc (0x020c8fdc, size 0x30)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 15. Fld_BrushHati

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Fld_BrushHati.c (USA ov030, 8 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c900c-0x020c91f4
  .rodata: 0x020ecb88-0x020ecbd0
  .data: 0x02108600-0x02108614
Functions in original order:
  func_ov030_020c900c (0x020c900c, size 0x9c)
  func_ov030_020c90a8 (0x020c90a8, size 0x7c)
  func_ov030_020c9124 (0x020c9124, size 0x18)
  func_ov030_020c913c (0x020c913c, size 0x14)
  func_ov030_020c9150 (0x020c9150, size 0x14)
  func_ov030_020c9164 (0x020c9164, size 0x14)
  func_ov030_020c9178 (0x020c9178, size 0x48)
  func_ov030_020c91c0 (0x020c91c0, size 0x34)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 16. Field_020c91f4

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Field_020c91f4.c (USA ov030, 11 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020c91f4-0x020c9990
  .rodata: 0x020ecbd0-0x020ecd0c
  .data: 0x02108614-0x0210882c
Functions in original order:
  func_ov030_020c91f4 (0x020c91f4, size 0x88)
  func_ov030_020c927c (0x020c927c, size 0x94)
  func_ov030_020c9310 (0x020c9310, size 0x14)
  func_ov030_020c9324 (0x020c9324, size 0x114)
  func_ov030_020c9438 (0x020c9438, size 0x40)
  func_ov030_020c9478 (0x020c9478, size 0x38)
  func_ov030_020c94b0 (0x020c94b0, size 0x2ac)
  func_ov030_020c975c (0x020c975c, size 0x114)
  func_ov030_020c9870 (0x020c9870, size 0x8c)
  func_ov030_020c98fc (0x020c98fc, size 0x54)
  func_ov030_020c9950 (0x020c9950, size 0x40)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 17. Task_Mess

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Task_Mess.c (USA ov030, 10 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: edge (a working hypothesis). Assigned half-open ranges:
  .text: 0x020ca1f4-0x020ca4bc
  .rodata: 0x020ecda8-0x020ecdc4
Functions in original order:
  func_ov030_020ca1f4 (0x020ca1f4, size 0xc)
  func_ov030_020ca200 (0x020ca200, size 0x104)
  func_ov030_020ca304 (0x020ca304, size 0x7c)
  func_ov030_020ca380 (0x020ca380, size 0x64)
  func_ov030_020ca3e4 (0x020ca3e4, size 0x4)
  func_ov030_020ca3e8 (0x020ca3e8, size 0x3c)
  func_ov030_020ca424 (0x020ca424, size 0x24)
  func_ov030_020ca448 (0x020ca448, size 0x14)
  func_ov030_020ca45c (0x020ca45c, size 0x18)
  func_ov030_020ca474 (0x020ca474, size 0x48)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 18. Field_020ca4bc

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Field_020ca4bc.c (USA ov030, 10 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: A+ (a working hypothesis). Assigned half-open ranges:
  .text: 0x020ca4bc-0x020cb12c
  .rodata: 0x020ecdc4-0x020ed0d8
  .data: 0x021088ec-0x02108920
Functions in original order:
  func_ov030_020ca4bc (0x020ca4bc, size 0x58)
  func_ov030_020ca514 (0x020ca514, size 0x74)
  func_ov030_020ca588 (0x020ca588, size 0x1f0)
  func_ov030_020ca778 (0x020ca778, size 0x1c8)
  func_ov030_020ca940 (0x020ca940, size 0xa8)
  func_ov030_020ca9e8 (0x020ca9e8, size 0x154)
  func_ov030_020cab3c (0x020cab3c, size 0x28)
  func_ov030_020cab64 (0x020cab64, size 0x2ac)
  func_ov030_020cae10 (0x020cae10, size 0x28c)
  func_ov030_020cb09c (0x020cb09c, size 0x90)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 19. Task_Window

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/Task_Window.c (USA ov030, 4 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020cb12c-0x020cb284
  .rodata: 0x020ed0d8-0x020ed0e4
Functions in original order:
  func_ov030_020cb12c (0x020cb12c, size 0x3c)
  func_ov030_020cb168 (0x020cb168, size 0x6c)
  func_ov030_020cb1d4 (0x020cb1d4, size 0x48)
  func_ov030_020cb21c (0x020cb21c, size 0x68)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 20. LocaTitle

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/LocaTitle.c (USA ov030, 7 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020d2f2c-0x020d317c
  .rodata: 0x020ee948-0x020ee998
  .data: 0x021090e4-0x02109110
Functions in original order:
  func_ov030_020d2f2c (0x020d2f2c, size 0xbc)
  func_ov030_020d2fe8 (0x020d2fe8, size 0x50)
  func_ov030_020d3038 (0x020d3038, size 0x34)
  func_ov030_020d306c (0x020d306c, size 0x48)
  func_ov030_020d30b4 (0x020d30b4, size 0x20)
  func_ov030_020d30d4 (0x020d30d4, size 0x48)
  func_ov030_020d311c (0x020d311c, size 0x60)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## 21. XFade

```text
Start a new branch/worktree from delink-ov030-more. Decompile and match only src/Field/XFade.c (USA ov030, 14 functions).
Read docs/CONTRIBUTING.md and docs/decomp/ov030-more/manifest.json first. The source is an incomplete stub; the original assembly is still linked.
Boundary confidence: C (a working hypothesis). Assigned half-open ranges:
  .text: 0x020d3b3c-0x020d4830
  .rodata: 0x020ee9d4-0x020eea04
  .data: 0x0210911c-0x0210916c
Functions in original order:
  func_ov030_020d3b3c (0x020d3b3c, size 0xa4)
  func_ov030_020d3be0 (0x020d3be0, size 0x70)
  func_ov030_020d3c50 (0x020d3c50, size 0x38)
  func_ov030_020d3c88 (0x020d3c88, size 0x38)
  func_ov030_020d3cc0 (0x020d3cc0, size 0xe0)
  func_ov030_020d3da0 (0x020d3da0, size 0x20)
  func_ov030_020d3dc0 (0x020d3dc0, size 0x78)
  func_ov030_020d3e38 (0x020d3e38, size 0x3c)
  func_ov030_020d3e74 (0x020d3e74, size 0x3c)
  func_ov030_020d3eb0 (0x020d3eb0, size 0x1c8)
  func_ov030_020d4078 (0x020d4078, size 0x6b0)
  func_ov030_020d4728 (0x020d4728, size 0x38)
  func_ov030_020d4760 (0x020d4760, size 0x88)
  func_ov030_020d47e8 (0x020d47e8, size 0x48)
Recover readable C, structs and task lifecycle where present, using the existing EasyTask/TaskStages conventions and neighboring completed files as references. Preserve USA placeholder symbols until a deliberate rename is coordinated. Inspect target assembly and relocations; use m2c if available, treating its output as a draft. Do not invent function prototypes or shared data ownership to silence compiler errors.
Your write scope is this source file. Do not change other assigned TUs, shared headers, symbols or delinks without coordinating with the integrator; report required shared edits separately. Existing owners on main and decomp-ov030-field are reserved. Do not expand ranges or claim generated gaps. Keep unresolved globals extern and describe data-layout issues rather than stealing neighboring bytes.
Build the source object and compare every function using objdiff-cli and objdiff.json. Record per-function matches and remaining causes. Mark nonmatching functions with the repository comment convention. Leave the TU incomplete until all its assigned code/data reproduce the target and the full USA build reports build/usa/twewy_usa.nds: OK; propose the complete flag to the integrator once verified. Run python tools/check_ov030_more.py. Report changes, validation, shared-edit requests and unresolved hypotheses. Do not modify JP based on a guessed address offset.
```

## Validation of this preparation batch

Verified on Windows, USA only:

- `python tools/check_ov030_more.py`: PASS, 21 TUs and 181 distinct functions; no overlaps with existing main/field ownership or within the batch.
- `python tools/configure.py --no-extract --dsd ./dsd.exe --compiler tools/mwccarm`: succeeded using installed tools.
- `ninja sha1 check -j 6`: all module and symbol checks passed; `build/usa/twewy_usa.nds: OK`.
- `ninja dis`: succeeded; target assembly is available locally.
- `git diff --check`: passed.

JP was not modified or built. The full default Ninja target additionally tries to download objdiff in this fresh checkout and encountered a missing Python requests dependency; the ROM, module and symbol validation targets above completed using installed tools. No decompilation or per-function C match is claimed for these stubs.
