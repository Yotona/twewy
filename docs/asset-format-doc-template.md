# <Asset family> format notes

## Scope

- Files covered: `<path/file1.bin>`, `<path/file2.bin>`
- Primary code consumers: `<src/...>`
- Loader path: `<DatMgr_LoadRawDataWithOffset | DatMgr_LoadPackEntry | ...>`
- Status: `draft | partial | mature`

---

## 1) Evidence sources

- Code: `<src/...>`
- Headers/structs: `<include/...>`
- Runtime observations: `<if any>`
- External/community references: `<if any, clearly marked>`

Notes:

- Keep code-derived claims separate from external assumptions.
- Prefer in-repo evidence when conflicts exist.

---

## 2) Access pattern classification

Choose one (or both) and justify with callsites:

- Fixed-record table (`index * sizeof(record)`)
- Pack/container (`PackHeader` + `PackEntry[]` + entry index)

### Callsite map

| BinIdentifier | Loader API | Index/Entry source | Immediate consumer |
|---|---|---|---|
| `<id,path>` | `<API>` | `<var/expression>` | `<function/system>` |

---

## 3) Binary layout

## A) Header/table (if present)

```c
typedef struct {
    // fill with known fields only
} <HeaderName>;
```

- Endianness: `<little/big/unknown>`
- Offset base rules: `<e.g., entry.offset + 0x20>`
- Alignment rules: `<confirmed/tentative>`

## B) Record / entry body

| Offset | Type | Name | Confidence | Observed use |
|---|---|---|---|---|
| `0x00` | `u32` | `unk_00` | tentative | `<where/how used>` |

Confidence legend:

- `confirmed`: direct structural/API proof
- `strong`: repeated consistent behavior
- `tentative`: plausible, limited evidence

---

## 4) Runtime semantics

- ID/index domain mapping: `<global ID -> local row/entry>`
- Constraints/invariants: `<caps, sentinel values, bounds>`
- Special entries: `<entry 0 meta/order/etc. if applicable>`

### Key functions

- `<func>`: `<what it proves>`
- `<func>`: `<what it proves>`

---

## 5) Validation checklist

- [ ] File bounds checks pass for all offsets/sizes
- [ ] Record count matches code-side index range
- [ ] Parsed fields match observed runtime behavior
- [ ] One controlled edit produced expected in-game effect
- [ ] Unknown fields explicitly documented as unknown

---

## 6) Open questions / next experiments

1. `<question>`
2. `<question>`
3. `<experiment to disambiguate>`

---

## 7) Revision log

- `YYYY-MM-DD`: `<summary of what changed and why>`
