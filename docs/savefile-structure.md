# Savefile structure analysis (TWEWY DS)

This document summarizes how save data is laid out and validated based on:

- `src/Savefile.c`
- `include/Save.h`
- `build/usa/asm/src/Savefile.s`

It describes the on-media block format and the in-memory structures used to serialize/deserialize save data.

---

## 1) High-level save media layout

The game keeps **two redundant copies** of each domain:

- Primary game data block (size `0x3444`)
- Backup/friend block (size `0x202C`)

Observed offsets used by `func_0204285c` calls:

| Region | Offset | Size |
|---|---:|---:|
| Primary copy A | `0x0000` | `0x3444` |
| Backup copy A | `0x3444` | `0x202C` |
| Primary copy B | `0x5470` | `0x3444` |
| Backup copy B | `0x88B4` | `0x202C` |

Total covered range is `0xA8E0` bytes (`0x88B4 + 0x202C`).

---

## 2) Primary block (`0x3444`) format

### 2.1 Byte layout

| Offset | Size | Meaning |
|---|---:|---|
| `0x0000` | `0x3420` | Serialized `MainData` payload (`data_02071cf0.unk_20`) |
| `0x3420` | `0x1B` | Signature string: `"SubarAsikiKonosEkai070613b"` |
| `0x343B` | `0x04` | Encoded payload length tag (derived from `0x3420`) |
| `0x343F` | `0x01` | Padding/reserved (zeroed by buffer init) |
| `0x3440` | `0x02` | 16-bit checksum (`func_0202593c`) |
| `0x3442` | `0x02` | Padding/reserved |

### 2.2 Integrity checks

Validation path (`func_02024724` / `func_020254ec`) does:

1. Signature+length-tag validation via `func_020258bc(buf+0x3420, signature, 0x1B, 0x3420)`
2. Whole-block checksum validation via `func_0202593c(buf, 0x3444) == 0`

Write path (`func_02024f00`) does:

1. Build block payload (`func_02024d48`)
2. Zero checksum field at `+0x3440`
3. Compute checksum over full `0x3444`
4. Write computed 16-bit checksum back at `+0x3440`

---

## 3) Backup/friend block (`0x202C`) format

### 3.1 Byte layout

| Offset | Size | Meaning |
|---|---:|---|
| `0x0000` | `0x2008` | Serialized global friend payload |
| `0x2008` | `0x1B` | Same signature string |
| `0x2023` | `0x04` | Encoded payload length tag (derived from `0x2008`) |
| `0x2027` | `0x01` | Padding/reserved |
| `0x2028` | `0x02` | 16-bit checksum (`func_0202593c`) |
| `0x202A` | `0x02` | Padding/reserved |

### 3.2 Integrity checks

Validation path (`func_020248e4` / `func_020256c8`) mirrors primary logic:

1. `func_020258bc(buf+0x2008, signature, 0x1B, 0x2008)`
2. `func_0202593c(buf, 0x202C) == 0`

Write path (`func_020251d8`) mirrors primary checksum handling, using checksum field at `+0x2028`.

---

## 4) Redundancy and status semantics

### 4.1 Slot fallback policy

- Primary validate/load tries A (`0x0000`) then B (`0x5470`).
- Backup validate/load tries A (`0x3444`) then B (`0x88B4`).

### 4.2 Return codes observed in save logic

Common code meanings across `func_020247*`, `func_020248*`, `func_020255*`, `func_020257*`:

- `0`: success
- `1`: signature/format mismatch
- `2`: card/session/system error
- `8`: integrity failure (corrupt/invalid checksum path)

### 4.3 Backup validity gating with `unk_1AB6`

`func_0202499c` uses bit 0 of `MainData.unk_1AB6` as a condition when one backup copy fails.
This acts as an additional acceptance guard for backup recovery.

---

## 5) In-memory structures tied to serialization

### 5.1 Main payload source

- `data_02071cf0.unk_20` is `MainData` (see `include/Save.h`).
- `func_02024d48` serializes this to the primary block payload region (`0x3420` bytes).
- `func_020254ec` deserializes validated primary payload back into `MainData`.

### 5.2 Friend payload source

- `data_02071cf0.globalFriendData` holds a `0x2008` runtime buffer.
- `func_02024e04` serializes this into backup block payload.
- `func_020256c8` deserializes validated backup payload into caller destination.

---

## 6) Save operation state machine

`data_0205c2a4[13]` in `Savefile.c` defines a staged save pipeline executed by `func_02025444`.

High level:

1. init/reset temporary state
2. build+write primary copy A
3. build+write primary copy B
4. build+write backup copy A
5. build+write backup copy B
6. finalize/restore status flags

Async completion is handled by polling helpers (`func_020250e8`, `func_020253cc`) using `func_02042ab0` + `func_02042330`.

---

## 7) Low-level lock/session protocol

Every save/read operation wraps media access in a lock/session sequence:

1. `OS_GetLockID`
2. `func_0204237c(lockId)`
3. `func_0204296c(0x1001)`
4. readiness check `func_02042944() == 1`
5. submit IO `func_0204285c(...)`
6. completion/status checks (`func_02042aa4`, `func_02042ab0`, `func_02042330`)
7. `func_0204238c(lockId)`
8. `func_020389c0(lockId)`

---

## 8) Known decomp caveat

`func_020258bc` in C currently contains a known nonmatching expression:

- Current C: `*(u8*)(arg0[i] + 27)`
- asm behavior: read from `arg0 + i + 0x1B`

When implementing tooling around signature checks, prefer asm behavior.
