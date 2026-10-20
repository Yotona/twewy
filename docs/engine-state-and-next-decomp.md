# Engine State and Next Decompile Analysis (April 2026)

This report summarizes current knowledge of TWEWY DS, the decompiled vs undecompiled engine state, concrete hypotheses for unknown engine functions, and a practical next-decompile roadmap.

## 1. What Is Known About the Game So Far

### 1.1 Project scope and constraints

- This repository is a reverse-engineering/decompilation project for Nintendo DS TWEWY and intentionally excludes copyrighted assets. See `README.md`.
-- A legitimate base ROM and setup artifacts are required for a functional build flow. See `README.md` and `CONTRIBUTING.md`.

### 1.2 Confirmed engine architecture

From current `src/Engine/**`, the runtime is organized into clear subsystems:

- Core systems:
  - DMA queue and flush pipeline (`src/Engine/Core/DMA.c`)
  - OAM manager, sprite/OAM build and submission (`src/Engine/Core/OamMgr.c`)
  - Display integration through engine managers (`src/Display.c`, `src/Engine/Core/OamMgr.c`)
- Resource/file systems:
  - Binary loading and compressed/uncompressed paths (`src/Engine/File/BinMgr.c`)
  - Pack archive handling (`src/Engine/File/PacMgr.c`)
  - Data manager and generated/pack-entry resources (`src/Engine/File/DatMgr.c`)
  - Palette and object resource managers (`src/Engine/Resources/PaletteMgr.c`, `src/Engine/Resources/ObjResMgr.c`)
- Text pipeline:
  - Font lookup and character-metric utilities (`src/Engine/Text.c`)
  - Multiple charset/renderer dispatch slots (`src/Engine/Text.c`)

### 1.3 Confirmed data/format knowledge

-- PACK archive format (`"pack"` magic `0x6B636170`) and entry-table behavior are documented and reflected by loader code. See `asset-format-analysis.md` and `src/Engine/File/PacMgr.c`.
- Nintendo DS tile/palette conventions (4bpp/8bpp indexed tiles, RGB555 palette expectations) are documented and match usage patterns in engine resource code. See `docs/asset-format-analysis.md`.
-- Save storage format is now well understood (dual-copy primary and backup/friend blocks, signature + checksum validation, staged write state machine). See `savefile-structure.md` and `src/Savefile.c`.

### 1.4 Decompilation status snapshot

Using `objdiff.json` (`metadata.complete`):

- Total units: 205
- Complete: 55
- Incomplete: 150

This indicates strong progress but still substantial undecoded logic. Incomplete samples include central runtime files (for example `src/Display`, `src/DatMgr`, `src/BgResMgr`, `src/SpriteMgr`, `src/Text`) and multiple CriWare/gameplay units.

### 1.5 Decompiled vs undecompiled runtime surface

- Decompiled or mostly available in C:
  - Core initialization and many utility layers (`src/main.c`, `src/Boot.c`, `src/Input.c`, etc.)
  - Significant portions of Engine core/resource code
  - Save pipeline and inventory documentation-level understanding
- Not yet fully decompiled / still external or assembly-backed:
  - Multiple engine helper functions still named `func_XXXX`
  - Several render/decode paths in OAM/palette/text pipelines
  - Large portions of overlay/gameplay logic (see `overlays.md`)

## 2. Decompiled and Not Decompiled Engine Analysis

### 2.1 What the decompiled engine already proves

- DMA is structured around op-handler tables and can route direct transfers vs decode+transfer path (`DMA_OpHandlers` in `src/Engine/Core/DMA.c`).
- OAM has explicit mode dispatch (`data_02059a6c`, `data_02059a78`) and dedicated visible-cell construction paths for affine vs non-affine sprites (`src/Engine/Core/OamMgr.c`).
- Sprite rendering path already integrates object-resource upload, palette interpretation, and OAM submission (`src/Engine/Resources/SpriteMgr.c`).
- Palette manager has explicit slot-type and chunk-mask logic for partial/full palette updates (`src/Engine/Resources/PaletteMgr.c`).

### 2.2 What remains not decompiled inside engine-facing flow

- Core decode/allocator helpers that many systems depend on:
  - `func_02004d60`
  - `func_02004ce8`
  - `func_02004c8c`
- OAM render/flush backends selected by dispatch table:
  - `func_020034cc`, `func_02003704`, `func_02003904`
  - `func_020035b4`, `func_02003a2c`
- Palette conversion/transform family:
  - `func_02002180`, `func_02002254`, `func_02002398`, `func_0200245c`, `func_02002520`, `func_020025e4`
- Text backend renderers are still placeholders:
  - `func_0200fa2c`, `func_0200fad4`, `func_0200fb88`, `func_0200fc70`

## 3. Unknown Engine Function Analysis (Reasoned Guesses)

Notes:
- These are hypotheses from signatures, callers, control flow, data structures, constants, and side effects.
- Confidence levels: High / Medium / Low.

### 3.1 `func_02004d60` (confidence: High)

Probable role: compressed stream decode into destination buffer.

Reasoning:
- Called in compressed-resource paths immediately before cache purge and DMA/GX upload:
  - `src/Engine/Core/DMA.c`: `func_02004d60(buf, &data->unk_00)` in decode branch.
  - `src/Engine/File/BinMgr.c`: `func_02004d60(outputBuffer, compressedDataBuffer)` in compressed file loader.
  - `src/Engine/File/DatMgr.c`: called when loading compressed PACK entry variant.
- Callers derive output size from packed header bits (`(*ptr & ~0xFF) >> 8`, and nibble checks) before decode, matching classic "header + payload" compression wrappers.

Concrete example:
- `BinMgr_LoadCompressed` reads compressed chunks, computes decompressed size, allocates output buffer, decodes via `func_02004d60`, then frees compressed temp buffer. This is textbook resource decompression flow.

### 3.2 `func_02004ce8` and `func_02004c8c` (confidence: High)

Probable roles:
- `func_02004c8c`: reserve/get temporary workspace from shared pool.
- `func_02004ce8`: commit/advance or allocate variable-sized transient chunks from same pool.

Reasoning:
- Both are called with `&data_0206a9bc` pool-like global and explicit byte sizes.
- `OamMgr_BuildVisibleCellPieces*` obtains a large scratch block via `func_02004c8c(..., 0x408)` and later calls `func_02004ce8(..., (count+1)*8)` after filtering visible pieces.
- Palette commit functions repeatedly call `func_02004ce8` for temporary transformed palette buffers just before `func_02001b44` transfer submission.

Concrete example:
- In `PaletteMgr` functions (`func_0200b66c` and siblings), `func_02004ce8` allocates a transform output buffer, conversion function writes into it, then DMA enqueues transfer to target palette region.

### 3.3 Palette transform family (`func_02002180`, `02002254`, `02002398`, `0200245c`, `02002520`, `020025e4`) (confidence: Medium-High)

Probable role: color/palette conversion kernels for different slot and source formats.

Reasoning:
- Called from six wrappers (`func_0200b09c` .. `func_0200b578`) selected by resource slot type and parameters (`colorParam`, `colorBias`, `chunkSize`, `chunkCount`, mask `unk_12`).
- Same wrappers have both full-copy and masked-chunk loops, indicating transform-by-chunk behavior.
- Sign of `colorBias` changes source table base (`data_02059d24` vs `data_0205a128`) for one path, consistent with brighten/darken LUT usage.

Concrete example:
- In `func_0200b198`, masked chunk iteration repeatedly invokes `func_02002254(...)` with color parameters and bias; offsets and source pointers advance by chunk byte spans.

### 3.4 OAM backend dispatch (`func_020034cc`, `02003704`, `02003904`) and flush helpers (`func_020035b4`, `02003a2c`) (confidence: Medium)

Probable role:
- Three rendering backends for different object tile/bmp modes; paired flush routines maintain per-mode priority/queue output behavior.

Reasoning:
- Function pointer tables are explicit:
  - `data_02059a6c[3]` render backends
  - `data_02059a78[3]` flush backends
- OAM manager initialization maps object tile mode into mode indices, matching the need for mode-specific char addressing and OAM write layout rules.
- Existing visible-cell build path already produces normalized `OamCellPiece`; backend likely finalizes mode-specific attr packing/ordering.

Concrete example:
- `OamMgr_SubmitCommand` path (via manager queues) can pass cell lists to mode backend; backend-specific flush then pushes final OAM entries.

### 3.5 `func_02003ef4` (confidence: Medium-High)

Probable role: extended/3D-style sprite draw command builder and emitter using frame metadata and transform state.

Reasoning:
- Accepts mode/position/cell/attrs plus char/palette bases and optional transform pointer.
- Builds a `RenderCmd` struct, resolves frame (`func_02003aa8`), and uses geometry/FIFO helpers (`func_02003c7c` nearby) that write G3/GX FIFO registers.
- Called from sprite render path specifically when `tempbit == 2`, i.e., non-standard OAM path.

Concrete example:
- `SpriteMgr` branches to `func_02003ef4(...)` for the `tempbit == 2` rendering case, while other paths use normal OAM build/submit functions. This strongly suggests an alternate render pipeline (likely 3D-assisted sprite path).

### 3.6 Text backend stubs (`func_0200fa2c`, `0200fad4`, `0200fb88`, `0200fc70`) (confidence: Medium)

Probable role: charset-specific glyph rasterizers/expanders used by text layout dispatcher.

Reasoning:
- `data_0205aea4[20]` dispatch table routes charset/style combinations to these functions.
- `func_0200fd64` indexes that dispatch table by fields from text descriptor structs and passes buffers/metrics/workspace pointers.
- Text file includes Shift-JIS-like mapping table and width helpers (`func_0200f960`, `func_0200f9e0`), so missing pieces are likely glyph copy/compose kernels.

Concrete example:
- `func_0200fd64` computes bounded dimensions and calls backend function pointer with destination buffer + glyph lookup resources, then returns tile-aligned size. Backends are currently placeholders.

### 3.7 DMA external GPU helpers (`func_02037c10`, `02037c28`, `02037c8c`, `02037ccc`, `02037ce4`, `02037d48`) (confidence: Medium)

Probable role: begin/load/end wrappers for alternate palette/object upload path (likely overlay-owned or external module).

Reasoning:
- Called in the exact same loop structure as working GX `BeginLoad.../Load.../End...` functions, but for alternate op indices.
- Parameter masking (`& 0x7fff`, `& 0x1fff`) mirrors ext palette address windows.

Concrete example:
- `func_02001910` and `func_02001948` are structurally analogous to `DMA_LoadBgExtPltt` and `DMA_LoadObjExtPltt`, only replacing GX direct calls with external helper calls.

## 4. What to Decompile Next

Prioritization method:
- Impact on unblock (how many systems remain blocked)
- Dependency centrality (how many call chains depend on it)
- Reverse-engineering tractability (size/complexity)
- Verification ease (can behavior be validated quickly)

### Priority 1: `func_02004d60` (decompression core)

Why first:
- Shared dependency across file loading, DMA decode path, and resource managers.
- Unlocks more downstream behavior than any other single function.

What it unlocks:
- Reliable compressed asset decode for textures/palettes/object data.
- More deterministic behavior for DatMgr/BinMgr resource loading.

### Priority 2: pool helpers `func_02004ce8` + `func_02004c8c`

Why second:
- Likely smaller functions with high leverage.
- Required by both palette transformation staging and OAM visible-piece builders.

What it unlocks:
- Correct transient-buffer behavior in palette and sprite render paths.
- Lower risk of hidden memory-lifecycle mismatches in current stubs.

### Priority 3: palette transform family

Why third:
- Large visible impact (correct palette output is required for readable UI/graphics).
- Current call graph already isolates these as conversion kernels behind wrappers.

Suggested order:
1. `func_02002180`
2. `func_02002254`
3. `func_02002398`
4. `func_0200245c`
5. `func_02002520`
6. `func_020025e4`

What it unlocks:
- Accurate color output across slot types and masked chunk updates.

### Priority 4: OAM backend dispatch functions

Why fourth:
- Core for mode-specific sprite submission/flush.
- Needed for complete sprite correctness outside already-working generic paths.

Suggested order:
1. `func_02003704` (likely closest to standard path)
2. `func_020034cc`
3. `func_02003904`
4. `func_020035b4` and `func_02003a2c`

What it unlocks:
- More complete on-screen sprite behavior and stable frame submission semantics.

### Priority 5: `func_02003ef4` render command path

Why fifth:
- Important for `tempbit == 2` sprite path and potentially 3D-backed draw behavior.
- More complex math/FIFO behavior; easier after pool and OAM support are stable.

What it unlocks:
- Correct rendering in extended mode pathways currently routed away from standard OAM.

### Priority 6: text backend renderer stubs

Why sixth:
- Important for full text fidelity, but less globally blocking than decompression/palette/sprite pipelines.
- Dispatcher and metric helpers are already present, making these a self-contained follow-up.

What it unlocks:
- Higher-quality and complete script rendering across charsets/styles.

## 5. Risks and Caveats

- `objdiff` completion is per-unit and does not directly measure subsystem playability; one missing central function can block many systems.
- Some behavior in docs is intentionally marked nonmatching or inferred; always verify against current `build/usa/asm/**` when implementation details matter.
- Overlay ownership boundaries can hide where external helpers truly live; use callsite + symbol reference scans before naming assumptions as fact.

## 6. Quick Actionable Decomp Sprint Plan

If running a focused 1-2 week sprint:

1. Implement or match `func_02004d60` with test vectors from known compressed assets.
2. Match `func_02004ce8` and `func_02004c8c` and validate OAM/palette temporary buffer lifetimes.
3. Decompile first two palette kernels (`func_02002180`, `func_02002254`) and validate rendered palettes in representative UI scenes.
4. Decompile one OAM backend (`func_02003704`) and confirm mode-specific sprite output parity.
5. Re-evaluate next queue ordering with new match deltas and visual runtime checks.
