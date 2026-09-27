# TWEWY DS Decompilation Tricks & Patterns

> Catalog of byte-matching techniques derived from analyzing **500+ 100%-matching functions**
> across the TWEWY DS decompilation (Metrowerks MWCC `-O4,p`, ARM946E target).

---

## Table of Contents

1. [Volatile & Memory-Mapped I/O](#1-volatile--memory-mapped-io)
2. [Bitfield Access Patterns](#2-bitfield-access-patterns)
3. [Loop Forms & Control Flow](#3-loop-forms--control-flow)
4. [Function Pointer Dispatch Tables](#4-function-pointer-dispatch-tables)
5. [Linked List Idioms](#5-linked-list-idioms)
6. [Struct Layout & Pointer Arithmetic](#6-struct-layout--pointer-arithmetic)
7. [Fixed-Point Arithmetic](#7-fixed-point-arithmetic)
8. [ARM Code Generation Patterns](#8-arm-code-generation-patterns)
9. [Task & Overlay Lifecycle Patterns](#9-task--overlay-lifecycle-patterns)
10. [Resource Manager Patterns](#10-resource-manager-patterns)
11. [Const Data & Lookup Tables](#11-const-data--lookup-tables)
12. [Nonmatching Pitfalls](#12-nonmatching-pitfalls)

---

## 1. Volatile & Memory-Mapped I/O

### 1.1 Dummy Volatile Reads

**Problem**: MWCC emits redundant `ldr` instructions for bare volatile variable access.

**C pattern**:
```c
SystemStatusFlags;           // bare statement — no assignment
SystemStatusFlags.unk_07 = TRUE;
```

**Assembly**:
```asm
ldr r0, [r4, #0x0]    ; volatile read (dummy — result unused)
mov r1, #0x1
strb r1, [r0, #0x7]   ; actual write
```

**Why**: The `SystemStatusFlags` struct is declared `volatile`. A bare expression statement forces MWCC to emit a load from memory. The original code had these reads, so the decompilation must preserve them. Each `SystemStatusFlags;` line corresponds to exactly one `ldr` in the binary.

**Source**: `src/main.c:66,72,75,84,87,92,95,99`

### 1.2 Hardware Register Access via `vu8`/`vu16`/`vu32`

```c
u32 scanline = *(vu8*)0x04000006;   // volatile u8 read
```
```asm
ldr r2, .L_02004214    ; = 0x4000006
ldrb r2, [r2, #0x0]    ; byte load from HW register
```

**Trick**: Use `vu8` (volatile u8) for byte-width hardware registers. Using `vu32` would generate an `ldr` instead of `ldrb`, which reads 4 bytes and won't match.

**Source**: `src/Engine/Core/HBlank.c:32`

### 1.3 Volatile Store-to-Stack for Loop Persistence

```c
do {} while (REG_VCOUNT < 0xC0);
```
```asm
.L_loop:
    ldrh r0, [r1, #0x0]    ; volatile read REG_VCOUNT
    str r0, [sp, #0x0]     ; spill to stack (compiler quirk)
    ldr r0, [sp, #0x0]     ; reload from stack
    cmp r0, #0xc0
    blt .L_loop
```

**Trick**: MWCC sometimes spills a volatile read to the stack and reloads it. The C code is a simple `do-while`, but the compiler inserts a stack round-trip to ensure the volatile read isn't optimized away.

**Source**: `src/Engine/IO/Display.c` (VBlank spin loop)

---

## 2. Bitfield Access Patterns

### 2.1 Shift-Chain Extraction (Not `AND`)

MWCC extracts bitfields via shift-left then shift-right-logical, not via `AND`:

```c
if (SystemStatusFlags.unk_03 != FALSE) { ... }
```
```asm
ldr r0, [r4, #0x0]
lsl r0, r0, #0x1c     ; shift left to isolate bits 28-31
lsrs r0, r0, #0x1f    ; shift right to get bit 3 (unsigned)
```

**Trick**: When decompiling, if you see `lsl` + `lsrs` pairs, the original C was a bitfield read. The shift amounts reveal which bits: `lsl #0x1c` = bits 28-31, `lsrs #0x1f` = extract bit 31 → bit 3 of the original word.

**Source**: `src/main.c:161`

### 2.2 Individual Load-Modify-Store for Each Bitfield Assignment

```c
data_02066a58 &= ~1;
data_02066a58 &= ~2;
data_02066a58 &= ~4;
// ... 9 separate operations
```
```asm
ldr r0, [r4, #0x0]
bic r0, r0, #0x1
str r0, [r4, #0x0]
ldr r0, [r4, #0x0]
bic r0, r0, #0x2
str r0, [r4, #0x0]
; ... repeated for each bit
```

**Trick**: MWCC does NOT combine adjacent bit-clear operations into a single mask. Each `&= ~N` generates a separate `ldr`/`bic`/`str` triplet. If the original code had 9 separate statements, the decompilation must too.

**Source**: `src/main.c:102-110`

### 2.3 `tst` for Masked Comparisons

```c
if ((func_02039380() & 0xF0000000) == 0) { ... }
```
```asm
bl func_02039380
tst r0, #0xf0000000
bne .L_else
```

**Trick**: MWCC uses `tst` + branch for `(value & mask) == 0` checks rather than `and` + `cmp`. This saves an instruction and a register.

**Source**: `src/main.c:112`

### 2.4 Bitfield Packing Order

MWCC packs bitfields **LSB-first**. For a `u32` with:
```c
struct {
    u32 slotType   : 3;   // bits 0-2
    u32 chunkSize  : 5;   // bits 3-7
    u32 chunkCount : 5;   // bits 8-12
    u32 colorParam : 6;   // bits 13-18
    u32 _pad0C     : 13;  // bits 19-31
};
```

The compiler generates `AND`/`ORR`/`BFI`/`BFC` sequences matching this layout. Field names like `bits_0_1`, `bit_2`, `bits_3_4` in headers directly reflect bit positions.

**Source**: `include/Engine/Resources/PaletteMgr.h`, `include/Engine/Resources/SpriteMgr.h`

---

## 3. Loop Forms & Control Flow

### 3.1 `do-while` Emission

MWCC with `-O4,p` generates `do-while` form (condition at bottom) even for `for` loops:

```c
do {
    prevGuess = guess;
    guess = (guess + n / guess) >> 1;
} while (prevGuess > guess);
```
```asm
.L_loop:
    mov r5, r4            ; prevGuess = guess
    mov r0, r6
    mov r1, r4
    bl _s32_div_f         ; n / guess
    add r0, r4, r0
    cmp r5, r0, asr #0x1  ; prevGuess > (guess + n/guess) >> 1
    asr r4, r0, #0x1
    bgt .L_loop
```

**Trick**: If the loop body always executes at least once, use `do-while`. MWCC prefers this form.

**Source**: `src/Engine/Math/Sqrt.c:17-20`

### 3.2 Sentinel-Terminated Linked List Traversal

```c
while (current != sentinelBlock) {
    if (current == target) break;
    current = current->physMemNext;
}
```
```asm
.L_loop:
    cmp r0, r1
    ldrne r0, [r0, #0x1c]    ; physMemNext
    cmpne r0, r2              ; sentinelBlock
    bne .L_loop
```

**Trick**: MWCC fuses the comparison and conditional load into a single `ldrne`/`cmpne` chain. The decompiler writes a `while` with `break` to match the conditional-continue pattern.

**Source**: `src/Engine/Core/Memory.c`

### 3.3 Countdown Loop with `while (i > 0)`

```c
u32 i = 10;
while (i > 0) {
    // ... body ...
    i--;
}
```
```asm
mov r5, #0xa
.L_loop:
    ; ... body ...
    sub r5, r5, #0x1
    cmp r5, #0x0
    bne .L_loop
```

**Trick**: MWCC generates countdown loops with `sub` + `cmp` + `bne`. Use `while (i > 0)` with post-decrement to match, not `for (i = 0; i < 10; i++)`.

**Source**: `src/Engine/Core/HBlank.c:39-56`

### 3.4 `goto` for Loop Exit

```c
loop_2:
    var_r2 = var_r1 * 2;
    if (condition) {
        var_r2 = temp_r9;
    }
    if (var_r2 <= temp_r3) {
        goto loop_2;
    }
```

**Trick**: MWCC sometimes generates branch-to-label patterns that look like `goto` rather than structured control flow. Preserve the `goto` if `for`/`while` would alter the control flow graph.

**Source**: `src/Engine/Core/OamMgr.c` (heap sort inner loop)

### 3.5 Conditional `pop` for Early Return

```c
if (idx >= 10) {
    return;
}
```
```asm
cmp r0, #0xa
pophs {r4, pc}     ; if (idx >= 10) return
```

**Trick**: MWCC folds early returns into conditional `pop` instructions. The `pophs` (pop if higher or same) is a single-instruction return.

**Source**: `src/Engine/Core/HBlank.c:116-118`

---

## 4. Function Pointer Dispatch Tables

### 4.1 Named Struct Wrapper for Jump Tables

```c
const struct FaderUpdateDispatch {
    void (*entries[4])(DisplayEngine, Fader*);
} FaderUpdateDispatchFuncs = {
    {EasyFade_UpdateLinear, EasyFade_UpdateInterpolated, EasyFade_UpdateSmooth, EasyFade_UpdateInstant}
};

// Usage:
const struct FaderUpdateDispatch funcTable = FaderUpdateDispatchFuncs;
funcTable.entries[fader->mode](engine, fader);
```

**Trick**: Wrapping the function pointer array in a named `const struct` forces MWCC to place it in `.rodata` as a literal table. The function then copies it locally before use. This ensures the compiler generates a proper jump table rather than inlining.

**Source**: `src/EasyFade.c:155-165`

### 4.2 Parallel Function Pointer Arrays

```c
void (*data_02059a6c[3])(OamManager*, u32, OamCellPiece*) = {func_020034cc, func_02003704, func_02003904};
void (*data_02059a78[3])(OamManager*) = {func_020035b4, OamMgr_FlushPriorityLists, func_02003a2c};

// Dispatch:
data_02059a6c[mgr->renderMode](mgr, sortKey, spriteData);
data_02059a78[mgr->renderMode](mgr);
```

**Trick**: Two parallel arrays with the same index form a manual vtable. The assembly loads a function pointer from each table and calls via `blx`.

**Source**: `src/Engine/Core/OamMgr.c:40-41`

### 4.3 `TaskStages` Union for 4-Stage Dispatch

```c
typedef union {
    struct {
        TaskStageFn initialize;
        TaskStageFn update;
        TaskStageFn render;
        TaskStageFn cleanup;
    };
    TaskStageFn iter[4];
} TaskStages;

s32 RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = MyInit,
        .update     = MyUpdate,
        .render     = MyRender,
        .cleanup    = MyDestroy,
    };
    return stages.iter[stage](pool, task, args);
}
```

**Trick**: The union allows both named initialization (`.initialize = ...`) and indexed dispatch (`stages.iter[stage]`). MWCC places the function pointers contiguously. This is the project's **core idiom** for task lifecycle — found in **61+ files**.

**Source**: `src/Field/Fld_WhichArrow.c:149-157`, and dozens more

### 4.4 `OverlayProcess` 3-Stage Dispatch

```c
static const OverlayProcess OvlProc_TakTest = {
    .init = TakTest_Init,
    .main = TakTest_Update,
    .exit = TakTest_Destroy,
};

void ProcessOverlay_TakTest(TakTestState* state) {
    s32 step = MainOvlDisp_GetProcessStage();
    if (step == PROCESS_STAGE_EXIT) {
        TakTest_Destroy(state);
    } else {
        OvlProc_TakTest.funcs[step](state);
    }
}
```

**Trick**: `PROCESS_STAGE_EXIT = 0x7FFFFFFF` — an extreme value that can never be reached by incrementing from `PROCESS_STAGE_MAIN`. The exit handler is explicitly checked *before* the table lookup.

**Source**: `src/Interface/Debug/TakTest/TakTest.c:75-88`

---

## 5. Linked List Idioms

### 5.1 Intrusive Singly-Linked List with Sentinel

Every resource manager uses the same pattern:

```c
// Pop from free list (remove head)
Resource* res = mgr->freeList;
mgr->freeList = res->next;

// Push to active list (prepend)
res->next = mgr->activeList;
mgr->activeList = res;

// Remove from active list (walk with prev pointer trick)
Resource** prev = &mgr->activeList;
while (*prev != NULL) {
    if (*prev == target) {
        *prev = target->next;
        break;
    }
    prev = &(*prev)->next;
}
```

**Trick**: The `&mgr->activeList` trick treats the pointer field itself as a `Resource**` node, enabling uniform removal without a special case for the head.

**Source**: `src/Engine/Resources/PaletteMgr.c`, `src/Engine/Resources/BgResMgr.c`, `src/Engine/Resources/ObjResMgr.c`

### 5.2 Self-Referencing Sentinel

```c
mgr->prioGroups[0].sentinel = (OamSpriteCmd*)&mgr->prioGroups[0].sentinel;
```

**Trick**: The sentinel's `right` pointer points to itself in memory, creating a circular reference as a linked-list terminator.

**Source**: `src/Engine/Core/OamMgr.c`

---

## 6. Struct Layout & Pointer Arithmetic

### 6.1 `(block + 1)` = Adding Struct Size

```c
void* data = (block + 1);  // C: pointer arithmetic
```
```asm
add r0, r3, #0x20     ; ASM: add sizeof(MemBlock)
```

**Trick**: `(block + 1)` in C adds `sizeof(*block)` bytes. If `MemBlock` is 0x20 bytes, the assembly adds `#0x20`.

**Source**: `src/Engine/Core/Memory.c`

### 6.2 Stride-Based Pool Iteration

```c
cur = (PaletteResource*)((u8*)cur + 0x20);  // Manual pointer arithmetic
```

**Trick**: MWCC doesn't optimize `array[i]` with non-standard element sizes into pointer arithmetic. Use `(u8*)ptr + sizeof(entry)` to match the assembly's `add rN, rN, #0x20`.

**Source**: `src/Engine/Resources/PaletteMgr.c:250-255`

### 6.3 Struct Copy via `ldm`/`stm`

```c
*newBlock = *alloc;  // struct copy
```
```asm
ldm r6!, {r0, r1, r2, r3}
stm r5!, {r0, r1, r2, r3}
ldm r6, {r0, r1, r2, r3}
stm r5, {r0, r1, r2, r3}
```

**Trick**: MWCC generates `ldm`/`stm` pairs for 32-byte struct copies. The C code uses a simple assignment.

**Source**: `src/Engine/Core/Memory.c` (Mem_AllocHeapHead)

### 6.4 Array of Non-Standard-Sized Structs

```c
OamManager* mgr = (OamManager*)((u8*)&g_OamMgr + engine * 0x108C);
```
```asm
mul r0, r1, r0           ; engine * 0x108C
ldr ip, .L_addr          ; = g_OamMgr
add ip, ip, r0           ; &g_OamMgr[engine]
```

**Trick**: For arrays where `sizeof(element)` isn't a power of 2, MWCC uses explicit multiply + add. The C code casts to `u8*` and adds the byte offset.

**Source**: `src/Engine/Core/OamMgr.c:62`

---

## 7. Fixed-Point Arithmetic

### 7.1 12.4 Fixed-Point (Standard)

```c
#define I2F(i) ((i) << 12)
#define F2I(f) ((f) >> 12)

s32 brightness = fader->currentBrightness >> 0xC;  // F2I
```
```asm
asr r2, r1, #0xc    ; arithmetic shift right by 12
```

**Trick**: The game uses 12.4 fixed-point throughout. `I2F` = `<< 12`, `F2I` = `>> 12` (arithmetic shift for signed values).

**Source**: `src/EasyFade.c`, `src/Engine/Color.c`

### 7.2 Fixed-Point Linear Interpolation

```c
F2I(((blendStrength * (targetFixed - I2F(channel))) >> 5) + I2F(channel))
```

**Trick**: This is `channel + (target - channel) * blendStrength / 32` in fixed point. The `>> 5` divides by 32 (the blend strength range).

**Source**: `src/Engine/Color.c`

### 7.3 Per-Channel RGB555 Manipulation Without Unpacking

```c
s32 mask = 0x1F;        // 5-bit mask
s32 result = 0;
do {
    result |= mask & ((scale * (color & mask)) >> 5);
    mask <<= 5;         // shift mask to next channel
} while ((mask >> 0xF) == 0);  // stop after bit 15
```

**Trick**: Processes all 3 RGB555 channels (bits 0-4, 5-9, 10-14) in a single loop without extracting individual channels. This is a software SIMD approach.

**Source**: `src/Engine/Color.c`

---

## 8. ARM Code Generation Patterns

### 8.1 Conditional Execution for Simple If-Then

```c
if (softReset == 0x30C) {
    SystemStatusFlags.reset = TRUE;
}
```
```asm
cmp r0, #0x30c
ldreq r0, [r4, #0x0]
orreq r0, r0, #0x2
streq r0, [r4, #0x0]
```

**Trick**: MWCC uses ARM conditional instructions (`ldreq`, `orreq`, `streq`) for simple if-then bodies without else clauses.

**Source**: `src/main.c:166-168`

### 8.2 Tail Call via `bx ip`

```c
void HBlank_RestoreCallbacks(void) {
    HBlank_DisableInterrupt();
    Interrupts_SaveVBlankCallback(g_HBlankManager.savedVBlankCb);
    Interrupts_SaveHBlankCallback(g_HBlankManager.savedHBlankCb);
}
```

For a simpler wrapper:
```c
void Interrupts_ResetHBlankCallback(void) {
    Interrupts_SaveHBlankCallback(Interrupts_TriggerHBlank);
}
```
```asm
ldr ip, .L_SaveHBlank
ldr r0, .L_TriggerHBlank
bx ip                  ; tail call
```

**Trick**: MWCC uses `bx ip` for tail calls when the function is a simple wrapper. The `ip` register is used as the target because it's caller-saved.

**Source**: `src/Engine/Core/Interrupts.c`

### 8.3 `rsb` for Negation

```c
0xFFFF0000   // or equivalently: 0 - 0x10000
```
```asm
mov r1, #0x10000
rsb r1, r1, #0x0     ; r1 = 0 - 0x10000 = 0xFFFF0000
```

**Trick**: MWCC uses `rsb` (reverse subtract) for negative constants that aren't directly encodable as immediate values.

**Source**: `src/Engine/Math/Random.s:14`

### 8.4 `orr` with Shift Fold

```c
u32 seed = (RngSeed[1] << 0x10) | RngSeed[0];
```
```asm
ldrh r1, [r0, #0x2]
ldrh r0, [r0, #0x0]
orr r0, r0, r1, lsl #0x10
```

**Trick**: MWCC folds `<< N` into the `orr` instruction as a shifted operand. The C code is a straightforward OR, but the assembly shows the shift folded in.

**Source**: `src/Engine/Math/Random.s:31`

### 8.5 `mul` Followed by `lsr` for Scale

```c
scale = (((low << 16) >> 16) * scale) >> 16;
```
```asm
lsl r1, r3, #0x10
lsr r1, r1, #0x10    ; mask to u16
mul r0, r1, r0        ; multiply
lsr r0, r0, #0x10    ; >> 16
```

**Trick**: The `(low << 16) >> 16` pattern masks a value to 16 bits. MWCC generates `lsl` + `lsr` pairs for this.

**Source**: `src/Engine/Math/Random.s:49-52`

### 8.6 Division as Library Call

```c
guess = (guess + n / guess) >> 1;
```
```asm
bl _s32_div_f    ; signed 32-bit division library call
```

**Trick**: MWCC calls the runtime library for signed 32-bit division. The decompiled code must use the `/` operator — no manual division trick will match.

**Source**: `src/Engine/Math/Sqrt.c:19`

### 8.7 `moveq` for Conditional Return Value

```c
BOOL result = FALSE;
if (condition1 && condition2 && condition3) {
    result = TRUE;
}
return result;
```
```asm
tst r0, #0x1
beq .L_false
; ... more checks ...
bl func_check
cmp r0, #0x1
moveq r4, #0x1     ; result = TRUE if all checks passed
.L_false:
mov r0, r4
bx lr
```

**Trick**: MWCC sets the return value conditionally using `moveq`/`movne` rather than branching to set it.

### 8.9 Triaging Sub-100% Functions by Mechanism

A residue of "not 100%" is not one problem. Before spending attempts on source
restructuring, sort each sub-100% function into a bucket — the buckets have very
different tractability. `build/scratch/triage.py` does this from `build/ov_diff.json`.

The discriminator that matters: compare the two instruction streams with **condition
suffixes stripped** (`ldrne` == `ldr`) and **branch/literal targets normalised**. If the
mnemonic multisets still differ, the two sides are doing different work and no amount of
register-nudging will help. If they match, the difference is only how the work is
predicated, scheduled or coloured — and those *are* often steerable.

Buckets, in descending order of tractability:

| bucket | test | what fixes it |
|---|---|---|
| `reloc-artifact` | streams identical after normalising targets | nothing — objdiff relocation-index accounting |
| `schedule` | same work, same predication, reordered | statement order in the source |
| `reg-colour` | same work, same predication, different registers | named locals to reshape live ranges |
| `if-conversion` | same work, different predicated count | invert the condition to an early `return` |
| `mixed` | mnemonic multisets differ | genuinely different code — expect to lose |

As of the ov012/013/015 pass, the 23 residue functions sorted as 2 `reloc-artifact`,
3 `schedule`, 7 `reg-colour`, 13 `mixed` — i.e. **the bulk is `mixed` and is not worth
attacking without a specific hypothesis**. Note that the `if-conversion` bucket nearly
empties once suffixes are stripped: predication differences are almost always accompanied
by real work differences, so "MWCC refused to if-convert" is usually a symptom of a
different problem, not the problem itself.

Two cautions from that pass:

- **Do not chase the register choice directly.** In `func_ov012_02126c74` the original
  holds a value in `lr` where ours uses `r1`, but that is downstream of a *schedule*
  difference: the original's longer live range forces MWCC to rematerialise an address,
  ours is short enough to keep it live. Fixing the schedule is the only route; renaming
  variables to "use lr" does nothing.
- **Watch for store-aliasing barriers.** Moving a load later in the source can be
  self-defeating: an intervening store to a struct field creates a dependency MWCC cannot
  disambiguate from a global load, and it will serialise the block (in that same function,
  91.8% → 74.7% and +9 instructions). Measure instruction *count*, not just percentage,
  when a variant makes things worse.

---

## 9. Task & Overlay Lifecycle Patterns

### 9.1 Init/Update/Render/Release Lifecycle

Every task type follows this pattern:
```c
Task_Init(pool, task, args);       // stage 0
Task_Update(pool, task, args);     // stage 1
Task_Render(pool, task, args);     // stage 2
Task_Destroy(pool, task, args);    // stage 3
```

Dispatched via `TaskStages` union (see §4.3).

### 9.2 TaskHandle Registration

```c
static const TaskHandle Tsk_Fld_WhichArrow = {
    "Tsk_Fld_WhichArrow",
    Fld_WhichArrow_RunTask,
    0x4C   // data size
};
```

**Trick**: Each task type registers with a name, dispatch function, and data size. The size is used for pool allocation.

### 9.3 Overlay Stages

```c
#define PROCESS_STAGE_INIT 0
#define PROCESS_STAGE_MAIN 1
#define PROCESS_STAGE_EXIT 0x7FFFFFFF
```

**Trick**: EXIT uses `0x7FFFFFFF` so `stage++` from MAIN (1) can never reach it. The exit handler must be explicitly checked.

---

## 10. Resource Manager Patterns

### 10.1 Standard Resource Lifecycle

1. **Init**: Allocate pool as array, chain into free list, store global pointer
2. **Acquire**: Pop from free → init defaults → allocate VRAM slot → push to active → set source
3. **Release**: Decrement refcount → if zero: free VRAM slot → remove from active → push to free
4. **Flush/Commit**: Walk active list, check dirty flag, dispatch through function pointer table

### 10.2 Dirty Flag Pattern

```c
if (resource->flags & 0x8000) {  // dirty flag
    // commit to VRAM
    resource->flags &= ~0x8000;  // clear dirty
}
```

### 10.3 Reference Counting with Flag Guard

```c
if (resource->flags & 0x10) {  // in active list
    resource->refCount--;
    if (resource->refCount == 0) {
        // actually release
    }
}
```

**Trick**: The `flags & 0x10` guard prevents double-free.

### 10.4 Bitmap Allocator with Sign Encoding

```c
// Positive values = free block size
// Negative values = allocated block size (negated)
s16 vramSlots[ENTRY_COUNT];
```

**Source**: `src/Engine/Resources/BgResMgr.c`

---

## 11. Const Data & Lookup Tables

### 11.1 Copy-on-Use Pattern

```c
static const SpriteAnimation defaultAnim = { ... };

void Load(Sprite* sprite, s16 arg2) {
    SpriteAnimation anim = defaultAnim;   // copy from ROM to stack
    anim.dataType = arg2;                  // modify locally
    _Sprite_Load(sprite, &anim);
}
```

**Trick**: MWCC generates a `memcpy` from `.rodata` to stack, then in-place modifications. This avoids mutable global state while allowing per-instance customization.

**Source**: `src/Field/Fld_WhichArrow.c:82-94`

### 11.2 Static String Pointer in `.data`

```c
static const char* BootSequence = "Seq_Boot(void *)";
```
```asm
.section .data, 4, 1, 4
BootSequence:
    .word @str
@str:
    .byte 0x53, 0x65, 0x71, ...  ; "Seq_Boot(void *)"
```

**Trick**: `static const char*` places the **pointer** in `.data` (mutable) and the **string literal** also in `.data`. MWCC does NOT put these in `.rodata`.

**Source**: `src/Engine/Core/Boot.c:20`, `src/Engine/EasyTask.c`

### 11.3 Sentinel Palette Pointers

```c
if (arg1 == (u16*)&data_0205a128) {
    func_02001c74(...);  // Fade-to-black path
} else if (arg1 == &data_02059d24) {
    func_02001ce0(...);  // Fade-to-white path
}
```

**Trick**: Pointer identity checks against known global addresses select optimized code paths without extra parameters.

**Source**: `src/Engine/Color.c:210-218`

### 11.4 ASCII-to-Shift-JIS Table

```c
static const u16 data_0205aef4[256] = { ... };  // 512 bytes
```

**Trick**: A 256-entry `u16` table maps single-byte characters to 2-byte Shift-JIS equivalents. Critical for Japanese text rendering.

**Source**: `src/Engine/Text.c:74`

---

## 12. Nonmatching Pitfalls

### 12.1 Common Nonmatching Categories

| Issue | Description | Fix |
|-------|-------------|-----|
| **Regswap** | Register allocation differs from original | Reorder variable declarations or add/remove temporaries |
| **Instruction reordering** | Same instructions, different order | MWCC schedules differently; reorder expressions |
| **Stack allocation** | Compiler spills a local to stack | Force a local to be live across a function call |
| **Switch table** | Jump table layout mismatch | Ensure case ordering matches original |
| **Missing instruction** | Extra `mov` or `nop` for alignment | Add a dummy variable or reorder code |

### 12.2 MWCC Lazy Stack Frame

MWCC doesn't do `sub sp, sp, #N` at function entry if locals aren't needed until later. The stack allocation only happens when the local variable is needed for a function call. If your decompilation has locals declared too early, the stack frame may differ.

### 12.3 Dead Code Preservation

Functions like `OvlDisp_InitUnused1()` and `OvlDisp_InitUnused2()` are present in the original binary and must be included even though they're never called. Similarly, calls like `SndMgr_IsSeqArcAlreadyTracked(seqArc)` where the return value is discarded must be preserved — they're genuine patterns in the original.

**Source**: `src/Engine/Overlay/OverlayDispatcher.c`, `src/SndMgr.c`

### 12.4 Unused Parameter Artifacts

```c
void OamMgr_InitEngine(s32 unused, DisplayEngine engine)
```

**Trick**: The first parameter is preserved because the original function's calling convention required it (visible in assembly where `r0` is never read).

### 12.5 Explicit Cast Width Matching

```c
item->flags = ((u8)(item->flags & ~0xF) | 0x10) & ~0x20;
```

**Trick**: The `(u8)` cast forces truncation before the `| 0x10` to match the original assembly's register width. Without it, MWCC might generate a 32-bit operation.

**Source**: `src/Inventory.c:138`

---

## 13. Why 100% Function-Matching TUs Can Still Fail to Link

objdiff reports **function-level** matching (`.text` section only). But the `complete` flag in `delinks.txt` requires **TU-level** matching across **all sections**. A TU can have every function at 100% and still not be linkable. Here are all the failure modes:

### 13.1 Data Section Mismatches (`.data`, `.rodata`, `.bss`)

The most common blocker. objdiff validates code, but the linker also needs:

- **`.rodata`** — lookup tables, string constants, switch jump tables
- **`.data`** — initialized globals, vtables, function pointer tables, `static const char*` pointers
- **`.bss`** — zero-initialized globals (size and ordering must match)

If a single `const u16[]` table has one wrong entry, or a `static` variable's size differs by a byte, the entire section fails to match and the TU can't link.

**Detection**: objdiff shows per-section percentages in its UI — check `.data`/`.rodata`/`.bss` bars, not just `.text`.

### 13.2 Exception Handling Metadata (`.exception` / `.exceptix`)

MWCC-specific sections contain exception table entries with **link-time address constants**. Even if all code matches, these sections may not.

Explicit blocker documented in `config/usa/arm9/delinks.txt`:
```
libs/c/src/abort_exit_arm_eabi.c:
    // TODO: complete, needs dsd support for exception table address link-time constants
```

The TU is fully decompiled but `dsd` itself lacks the capability to handle relocations in `.exceptix`.

### 13.3 Gap Objects (`_dsd_gap@module_N`)

Between TUs, `dsd` generates **gap objects** for code/data regions not covered by any source file — compiler-generated padding, alignment stubs, unreferenced functions, or literal pool data. These gaps must also be byte-identical. If a gap adjacent to your TU doesn't match, the link fails even though your TU is perfect.

There are ~70+ gap objects in the main module alone. Each one is a separate "unit" in `objdiff.json`.

### 13.4 Missing or Incorrect Symbol Definitions

The compiled object references symbols that must be resolvable at link time. Failures include:

- Symbol not defined in `symbols.txt`
- Wrong `kind` — `function(arm)` vs `function(thumb)` (ARM vs Thumb interworking)
- Wrong `size` — linker may overwrite adjacent data
- Missing `local` qualifier — causes export/visibility issues

The linker (`mwld`) will either error or silently produce a different binary.

### 13.5 Relocation Mismatch

Every cross-TU reference is tracked in `relocs.txt`. The compiled object must generate **identical relocations** to the original. Issues include:

- Different relocation types (e.g., `arm_call` vs `load`)
- Wrong target addresses
- Missing or extra relocations
- Cross-overlay references with wrong module IDs (e.g., `module:overlays(3,30)`)

The main module has 9,000+ relocation entries.

### 13.6 Compiler Version / Flag Mismatches

Different files require different MWCC versions:

| Path | Compiler | Key Flag Difference |
|------|----------|---------------------|
| `src/Debug/Abe/Mini108.c` | v1.2/sp4 | `-str noreuse` |
| `libs/c/**` | v2.0/sp1p5 | `-str reuse` |
| `libs/runtime/**` | v2.0/sp1p5 | `-char unsigned` |
| Everything else | v2.0/sp1p5 | `-ipa file -str noreuse` |

Using the wrong version/flags produces different code even from identical source. The compiler version is set per-file in `config.yaml` → `configure.py`.

### 13.7 Overlay-Specific Constraints

Overlay TUs have additional requirements:

- **Cross-module relocations** must reference correct overlay IDs
- **Address aliasing**: Overlays share address ranges (e.g., `ov000`–`ov047` all start at `0x020824a0`). Relocations must specify which overlay module
- **No overlay TU is currently compiled** — all 48 overlays use delinked objects. Even ov004's 7 `complete` TUs are compiled but the overlay's non-complete TUs keep the whole overlay delinked

### 13.8 ITCM/DTCM Placement

ITCM code (`0x01ff8000`) and DTCM data (`0x027e0000`) have strict placement rules. The linker script uses `AFTER()` ordering to position modules relative to each other. If the compiled object's section sizes differ even slightly, the cascading `AFTER()` calculations shift all subsequent modules, breaking the entire ROM.

### 13.9 Section Ordering Within TUs

The `delinks.txt` file specifies section order per TU:
```
.text → .exception → .exceptix → .rodata → .ctor → .data → .bss
```

MWCC may emit sections in a different order than expected. The linker script (`arm9.lcf`) places them in the order listed in `objects.txt`. A mismatch causes address shifts across the whole binary.

### 13.10 Dead Code / Unused Symbol Preservation

Functions present in the original binary but unreferenced must still be compiled and included. The linker flag `-nodead` prevents stripping, but the compiled object must contain them at the correct size. Missing a single unreferenced function changes the section layout.

### 13.11 Verification Pipeline

The build runs two checks:

1. **`ninja check_modules`** — Verifies built module binaries match the original ROM hashes (per `config.yaml` `hash` fields)
2. **`ninja check_symbols`** — Verifies symbol addresses in the linked ELF match expected addresses from `symbols.txt`

Both must pass for a clean build. The `complete` flag is the **manual declaration** that all conditions above are met for a given TU.

### Summary

| # | Failure Mode | Affects | Detection |
|---|-------------|---------|-----------|
| 1 | Data section mismatch | `.data`, `.rodata`, `.bss` | objdiff section % |
| 2 | Exception table issues | `.exception`, `.exceptix` | Manual / dsd TODO |
| 3 | Gap objects | Adjacent address ranges | dsd check_modules |
| 4 | Missing symbols | Link-time resolution | mwld linker errors |
| 5 | Relocation mismatch | Cross-TU calls/loads | dsd check_symbols |
| 6 | Wrong compiler version | Code generation | objdiff fuzzy % |
| 7 | Overlay constraints | Cross-module refs | dsd check_modules |
| 8 | ITCM/DTCM placement | Address-sensitive code | ROM SHA1 |
| 9 | Section ordering | Address layout | ROM SHA1 |
| 10 | Dead code preservation | Unused functions | dsd check_modules |

**Bottom line**: objdiff's 100% function match only validates `.text` at the function level. Linkability requires byte-perfect matching of the **entire object file** across **all sections**, correct **symbol/relocation tables**, and proper **section alignment** within the linker's address space.

---

## Quick Reference Table

| Pattern | C Code | Assembly |
|---------|--------|----------|
| Dummy volatile read | `volatile_var;` | `ldr r0, [addr]` (unused) |
| Bitfield read | `struct.bitfield` | `lsl` → `lsrs` |
| Bit clear | `&= ~mask` | `bic` |
| Conditional store | `if (c) x = y` | `streq/strne` |
| Early return | `if (x >= N) return;` | `pophs {rN, pc}` |
| Tail call | `return func(args);` | `bx ip` |
| Shift+OR fold | `(high << 16) \| low` | `orr r0, r0, r1, lsl #16` |
| Division | `a / b` | `bl _s32_div_f` |
| Negation constant | `0 - value` | `rsb r1, r1, #0x0` |
| NULL default | `if (!ptr) ptr = default;` | `cmp r0, #0` / `ldreq` |
| Struct copy | `*dst = *src` | `ldm`/`stm` pairs |
| Fixed-point | `>> 12` / `<< 12` | `asr #0xc` / `lsl #0xc` |
| Jump table | `switch` + `case` | `addls pc, pc, rN, lsl #2` |
| u16 mask | `(x << 16) >> 16` | `lsl` + `lsr` |
| do-while loop | `do { } while (cond)` | body → `cmp`/`bne` at end |
