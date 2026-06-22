---
name: task-tu-refactoring
description: 'Refactor raw m2c output into a Task translation unit pattern for TWEWY decomp. Use when converting menu/interface Task files that exhibit a standardized format containing an Init/Update/Render/Destroy API accessed via TaskStages RunTask wiring, CreateTask args packing, and optionally GetFrameInfo/Load paths utilizing SpriteAnimation structures.'
argument-hint: 'Target file containing raw m2c output or partial decomp of a Task TU.'
user-invocable: true
---

# Task TU Refactoring

## Outcome
Produce a clean Task translation unit (TU) from raw m2c output that follows the project Task template and is ready for matching-focused iteration.

## Use When
- Converting m2c decompiler output into a normal Task TU layout.
- Rebuilding lifecycle callbacks and task creation flow.
- Standardizing menu/interface Task files to known local patterns.
- Doing match work where control-flow and struct layout consistency matter.

## Canonical Shape
Every Task TU should include at a minimum:
- Two local structs:
1. Primary task state struct.
2. Task args struct for external inputs.
- One `TaskHandle` with the task name, runner, and state size.
- Lifecycle callbacks: `*_Init`, `*_Update`, `*_Render`, `*_Destroy`.
- A `*_RunTask` function that dispatches the lifecycle callbacks by arranging them within a `TaskStages` union.
- A `*_CreateTask` function that packs args and returns the result of `EasyTask_CreateTask`.

Many Task TUs also include:
- At least one local `SpriteAnimation` struct.
- A `*_GetFrameInfo` callback referenced by the SpriteAnimation(s).
- A `*_Load` helper to initialize one or more sprites with the SpriteAnimation(s).

## Required Inputs
- Raw m2c output or partially reconstructed TU contained within a file named accordingly to its `TaskHandle` string.

## Hard Constraints
- Do not rename any functions or globals during this refactor workflow. These symbols may be referenced in other files and could negatively affect external linkage and/or match progress should not every single instance of the symbol be updated. Instead, suggest the expected names for these symbols in the chat after applying the refactor for the developer to review and apply manually at their discretion.
- Do not create any new functions or globals during refactoring. Work only with functions and globals that already exist in the raw m2c output. If callback signatures do not match, alter their implementations to meet the required signature. Do not create any form of wrapper or adapter.
- Local variables may be created sparingly within existing functions as needed to meet the required signatures and structure, but avoid introducing new variables if unnecessary. Do not create new variables for the sole purpose of renaming or restructuring code; only introduce them when necessary to achieve the required function signatures.
- Never create macros intended to simplify structure access. All structure access should be simple and direct, without indirection or obfuscation.
- Macros to improve readability of complex expressions are acceptable, provided they appear multiple times in the TU and warrant the abstraction. Do not create macros for simple one-off expressions or to rename fields in existing structs.
- Create type definitions conservatively. Beyond the two required local structs (`<FileName>` and `<FileName>_Args`), only define additional types if they are explicitly required by the callback signatures and memory layout.
- NEVER recreate or locally redefine Task system types such as `TaskPool`, `Task`, `TaskHandle`, `TaskStages`, or their callback signatures in the TU.
- Always include and use the canonical Task definitions from `include/Engine/EasyTask.h`.
- If there is any mismatch between decompiler output and local guessed task types, resolve it by adapting TU code to the `EasyTask.h` definitions, not by creating substitute structs/unions/typedefs.
- Function parameter names and function-local variable names may be renamed to descriptive names for readability and maintenance, as long as behavior and matching intent are preserved.
- Member names of file-local structs defined within the TU may also be renamed to descriptive names when it improves readability and maintainability.
- Local structs (the primary task state struct and the args struct) are file-scoped and not exposed outside the TU. They must be named after the filename: For example, a file named `MenuTop_numMoney.c` would define `MenuTop_numMoney` and `MenuTop_numMoney_Args`. No other naming format for these two structs is acceptable.
- Define and initialize `TaskStages` directly inside the `_RunTask` function body. Do not move `TaskStages` to file-scope/static data or any helper outside `_RunTask`.

- Example of required `_RunTask` function structure:
  ```C
    s32 _RunTask(TaskPool* pool, Task* task, s32 stage, void* args) {
      TaskStages stages = {
        .initialize = _Init,
        .update = _Update,
        .render = _Render,
        .cleanup = _Destroy,
      };
      return stages.iter[stage](pool, task, args);
    }
    ```
  This exact line must be used to dispatch the lifecycle callbacks in `RunTask`. Do not use any other form of dispatch, such as direct calls, switch statements, if statements, or inline conditionals.
- Do not add explicit unused-parameter suppression lines such as `(void)pool;`, `(void)task;`, or `(void)args;`. Leave unused parameters unreferenced.

## Procedure
1. Create file skeleton.
- Add includes used by nearby Task TUs (usually EasyTask + local feature header).
- Import `Engine/EasyTask.h` and rely on its canonical task types; do not duplicate task infrastructure types locally.
- Declare local state/args structs first.

2. Rebuild the state model.
- Name the primary task struct after the filename.
- Define the primary task struct in observed memory order.
- Keep unknown fields with stable `unk_XX` names until proven.
- Prefix all fields with comments denoting their address offset in the structure.
- Append a comment with the total size of the struct at the end of the definition, which can be found in the `TaskHandle`.
- The structure cannot be anything other than that canonical size. If known values do not fulfill the expected size, add padding fields as needed to reach the required total size.
- For example:
  ```C
    typedef struct {
      /* 0x00 */ s32 unk_00;
      /* 0x04 */ s16 unk_04;
      /* 0x06 */ s16 unk_06;
      /* 0x08 */ void* unk_08;
      /* 0x0C */ s32 unk_0C;
    } Struct; // Size: 0x10
  ```
- Name the args struct after the filename with an `_Args` suffix.
- Define args struct with only creation-time inputs.

3. Define task handle and static assets.
- Add `TaskHandle` with task name string, `RunTask`, and state size.
- Optional in a TU, a `SpriteAnimation` constant near top of file which gets used by `_Load`  and references `_GetFrameInfo`.

4. Reconstruct sprite path (branch).
- If sprite-backed:
1. Implement `GetFrameInfo` using the local project pattern.
2. Implement `Load` helper, clone animation template, patch runtime fields.
3. For multi-sprite tasks, load each sprite deterministically.
- If not sprite-backed: skip `SpriteAnimation`, `GetFrameInfo`, and `Load`.

5. Implement lifecycle callbacks.
- `Init`: copy args into state, initialize positions/flags, call load helper when used.
- `Update`: pull live state from owner/context pointers, then update runtime objects.
- `Render`: render only when the task-visible flag(s) permit.
- `Destroy`: release every acquired sprite/resource.

6. Wire dispatcher and factory.
- `RunTask`: declare `TaskStages stages = { .initialize = ..., .update = ..., .render = ..., .cleanup = ... };` locally inside the function.
- Every callback field must use its named designator (`.initialize`, `.update`, `.render`, `.cleanup`), not positional ordering.
- Return `stages.iter[stage](pool, task, args);` exactly.
- `CreateTask`: fill local args struct then return the result of passing it and local task handle into `EasyTask_CreateTask`.

7. Normalize for match-oriented style.
- Keep control flow simple and template-like before micro-optimizing.
- Prefer local temps and branch shape consistent with nearby matched files.
- Avoid speculative renames or type upgrades unless verified by usage/build impact.
- Do not perform any renaming; preserve existing symbol names exactly.
- You may improve names of function parameters and function-local variables where it helps readability; this does not count as symbol renaming for this workflow.
- You may also improve names of members in TU-local structs; this does not count as symbol renaming for this workflow.

8. Validate and iterate.
- Build and ensure TU compiles.
- Compare behavior and generated code against expected assembly/object diff.
- Iterate on field order, temporary variables, and branch structure only as needed.

## Decision Points
- m2c source referencing functions that are already named?: Attempt to identify a header file which declares the referenced functions and include it in the TU. If no such header can be found, declare the function locally with an extern prototype and the anticipated signature based on usage in the TU. Do not rename the function or create a wrapper.
- m2c source is missing expected lifecycle callbacks, distinctly appears to use a switch instead of TaskStages in `_RunTask`, or is missing a `_CreateTask` function?: It likely was originally written inconsistent to the template described and should not be refactored using this format. Notify the developer that the TU is not a good candidate for this refactor and suggest manual intervention. Do not attempt to force the TU into the canonical shape.
- m2c source contains calls to `_Sprite_Load`
- Sprite-backed task?: If no, sprite template plumbing will not be found in the provided m2c output.
- Single sprite or array?: Choose struct layout and load/update/render loops accordingly.
- Visibility source?: Static init flag, owner-struct field, or per-element flags.
- Args ownership?: Copy immediately into task state; do not persist stack references.
- Matching pressure high?: Prefer preserving known branch/temporary structure over readability edits.

## Quality Gates
A refactor is complete when all checks pass:
- Mandatory Task shape exists: 2 local structs, `TaskHandle`, 4 lifecycle callbacks, `RunTask`, `CreateTask`.
- `RunTask` declares a local `TaskStages stages` variable with all four callbacks assigned via named designators (`.initialize`, `.update`, `.render`, `.cleanup`).
- The TU uses task infrastructure types directly from `EasyTask.h` and does not contain local replacements for `TaskPool`, `Task`, `TaskHandle`, or `TaskStages`.
- `CreateTask` packs args and calls `EasyTask_CreateTask` once.
- Sprite-backed tasks release exactly what they load.
- Local task structs are named after the filename (`<FileName>` and `<FileName>_Args`).
- No functions or globals were renamed as part of the refactor.
- No no-op unused-parameter suppression statements were introduced (for example `(void)pool;` or equivalent casts used only to silence warnings).
- TU compiles and participates in match iteration without structural churn.

## Example Prompts
- Refactor this WIP TU into the canonical Task TU shape. Reshape functions to meet required signatures but preserve existing names.
- Convert this pseudo-C decomp into a non-sprite Task TU.
