# Asset parsing methodology (code-first)

This guide focuses on **how to study and document TWEWY asset formats from game code**, not just on final format tables.

It is built from patterns in:

- `src/Engine/File/DatMgr.c`
- `include/Engine/File/DatMgr.h`
- `src/Engine/File/PacMgr.c`
- `include/Engine/File/PacMgr.h`
- `src/Inventory.c`
- `src/Debug/Suyama/StaffRoll.c`
- `src/Engine/Resources/SpriteMgr.c`

---

## 1) Start from loaders, not bytes

When reverse-engineering formats, start with the functions that load data. In this repo, those are in `DatMgr`/`PacMgr`.

### Core loader families

From `DatMgr`:

- `DatMgr_LoadRawData(...)`
- `DatMgr_LoadRawDataWithOffset(...)`
- `DatMgr_LoadPackEntry(...)`
- `DatMgr_LoadPackEntryDirect(...)`
- `Data_Load(...)` and `Data_LoadToBuffer(...)` macros

From `PacMgr`:

- `PacMgr_LoadPack(...)`
- `PacMgr_LoadPackEntryData(...)`
- `PacMgr_GetPackEntryDataPtr(...)`

### Why this matters

A lot of format structure can be recovered *without opening a hex editor first*:

- whether files are read as whole blobs or indexed records,
- whether data is compressed,
- where offsets are relative to,
- what entry indices are used by each subsystem.

---

## 2) Classify the access pattern first

Most assets in this codebase fall into one of these patterns.

## A) Fixed-size record table (`.bin` as array of structs)

Signal:

- `Data_Load(...)` or `Data_LoadToBuffer(...)` is used with a typed struct variable.

In `DatMgr.h`, both macros call:

- `DatMgr_LoadRawDataWithOffset(..., index * sizeof(structVar))`

This means each element is a fixed-size record at:

- `fileOffset = index * recordSize`

Example:

- `src/Inventory.c` loads `RawPinData`, `RawItemData`, `RawFoodData`, `RawTreasureData` from different `.bin` files using indexed loads.

Documentation output for this pattern should include:

1. Record size (`sizeof` target struct),
2. field offsets and observed uses,
3. index domain (how IDs map to row index),
4. known/unknown field confidence.

## B) Pack/container file (table of entry offsets/sizes)

Signal:

- `DatMgr_LoadPackEntry(...)` or raw pack load + `Data_GetPackEntryData(...)`.

`PacMgr.h` defines:

- `PackHeader` (size `0x20`): `magic`, `entryCount`, `dataSize`, reserved
- `PackEntry`: `offset`, `size`

`PacMgr.c` confirms magic `0x6b636170` (`"pack"`).

Important addressing behavior:

- entry data is read using `entry.offset + 0x20`
- entry table starts after the 0x20-byte header

`Data_GetPackEntryData(data, entryIndex)` returns a pointer into already-loaded pack data by using:

- base = `data->buffer + sizeof(PackHeader)`
- pointer = `base + entries[entryIndex].offset`

In practice, many callsites use entry indices like `1`, `2`, `3`, ... (index `0` is often special/meta in game-specific packs).

Documentation output for this pattern should include:

1. Header struct and endianness,
2. entry table format,
3. offset base rules (`+0x20`),
4. known entry-index semantics per pack family.

---

## 3) Build a callsite map per asset file

After identifying loader pattern, find every callsite for a `BinIdentifier` path.

For each file path, capture:

- `BinIdentifier` ID + path pair,
- loading API used,
- index or entry ID source,
- consuming function(s),
- immediate consumers (BG manager, sprite system, gameplay stats, etc.).

### Example 1: inventory gameplay tables

`src/Inventory.c` shows `Apl_Tak/ItemData.bin`, `FoodData.bin`, `TreasureData.bin`, `../Data/BadgeData.bin` being loaded as indexed fixed records.

This lets you document:

- global item-ID partitions,
- per-category index conversions,
- field-level semantics inferred from arithmetic/comparisons.

### Example 2: graphics packs in debug/UI code

`src/Debug/Suyama/StaffRoll.c` and other modules load pack entries, then pass the returned pointers into BG/palette/sprite functions.

This lets you map Ågentry N = tile data / map / palette / animation tableÅh by tracing which engine API consumes each pointer.

---

## 4) Infer field semantics from behavior, not names

Decomp symbol names are frequently provisional (`unk_*`). Use behavioral evidence levels.

Recommended confidence labels:

- **Confirmed**: directly constrained by struct access and clear API contract.
- **Strongly inferred**: repeatedly used in one coherent role (e.g., stat addend).
- **Tentative**: plausible but weakly constrained.

Good evidence patterns:

- value used in bounds checks (`<`, `<=`, caps),
- value used as array index/category selector,
- value passed to a domain-specific API (palette, tilemap, text renderer),
- same field used consistently across multiple functions.

Avoid overfitting:

- do not rename `unk_*` to a semantic name unless evidence is strong,
- document alternatives when two interpretations remain plausible.

---

## 5) Validate hypotheses with round-trip tests

For a candidate format spec:

1. write a tiny parser that reads header/table and extracts fields,
2. verify counts/sizes against code expectations,
3. repack or patch one value,
4. observe runtime behavior change in a controlled context.

For pack files specifically:

- verify `magic == 0x6b636170`,
- iterate `entryCount`,
- ensure each `offset+0x20` and `size` stays in file bounds,
- test alignment assumptions separately (do not assume global alignment rule unless code enforces it).

For fixed-record files:

- verify `fileSize % recordSize == 0`,
- compute expected record count from ID ranges in code,
- test field edits that should produce visible/statistical effect.

---

## 6) Recommended documentation structure

Use a repeatable template for each format/family.

## A) Metadata

- file path(s)
- producer/consumer modules
- loader API path (`RawDataWithOffset`, `PackEntry`, etc.)

## B) Binary layout

- header/table definitions (if any)
- record size and field offset table
- endianness and pointer/offset base rules

## C) Runtime semantics

- index mapping and ID domains
- field usage with function references
- constraints and invariants

## D) Confidence + open questions

- confirmed vs inferred fields
- unresolved flags/unused bytes
- next best experiments

---

## 7) Practical workflow for this repository

1. Find `BinIdentifier` declarations for a target path.
2. Find all callsites using that identifier.
3. Classify loader pattern (fixed-record vs pack entry).
4. Trace first consumer of returned pointer/buffer.
5. Extract field/entry semantics from arithmetic and API contracts.
6. Write doc with confidence labels and explicit unknowns.
7. Add a small extraction script only after the spec is stable enough.

---

## 8) Common pitfalls

- Treating a pack entry as raw file offset without applying pack base (`+0x20`).
- Assuming entry index `0` is always data; some systems use it as metadata/order table.
- Renaming unknown fields too early.
- Mixing conclusions from external tools with in-repo code evidence without labeling source confidence.
- Ignoring cache/refcount behavior in `DatMgr`/`PacMgr` when reproducing loads.

---

## 9) How this complements existing docs

- `asset-format-analysis.md` is useful as a broad catalog.
- `inventory-bin-formats.md` is a concrete fixed-record case study.
- `StaffRoll-analysis.md` shows command-list + pack-driven rendering use.
- `asset-format-doc-template.md` provides a copy/paste scaffold for new format pages.

Use this methodology doc to keep future format docs consistent, evidence-based, and easier to review.
