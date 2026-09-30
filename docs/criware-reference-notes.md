# CriWare ADX — knowledge transfer from the PS2 reference decomp

Reference: [`crowded-street/3s-decomp`](https://github.com/crowded-street/3s-decomp/tree/main/src/anniversary/cri)
— a partial decomp of the same CriWare ADX middleware, for **PS2**.

This document records what transfers to our DS decomp in `libs/CriWare/`, what
does not, and the concrete bugs/renames that came out of the comparison.

> **Note on `AGENTS.md`.** `AGENTS.md` is not present on the current working
> branch; it was last seen on `origin/decomp-ov011` at commit `965c78b`. Its
> instructions were followed (read-only analysis first, `// Nonmatching`
> annotations, decomp.me-style scratch comments, no assets).

---

## 1. Which versions are we actually comparing?

| | Ours (DS) | Reference (PS2) |
|---|---|---|
| ADXT | `"\nADXT/NITRO Ver.10.36 Build:Sep 28 2007 13:14:01\n\0Append: MW4020\n"` | `"\nADXT/PS2EE Ver.9.00 Build:Sep 18 2003 10:00:00\n"` |
| SVM | (stripped) | `Ver.1.51` |
| SJ | (stripped) | `Ver.6.18` |
| CVFS | `"\nADX_NITRO Ver."` (truncated) | `Ver.2.34` |

**The DS build is a whole major version newer and ~4 years newer.** So the
default expectation is *divergence, not equivalence*. The useful signal splits
into three buckets:

1. **Identical logic** → our decomp is correct; "Nonmatching" is codegen-level
   (MWCC/MIPS vs MWCC/ARM), not a logic error. *Do not churn these.*
2. **DS-only code** → newer-revision features absent from the PS2 build. *Keep.*
3. **Same shape, different logic** → candidate genuine bug. *Verify against ROM.*

A fourth category turns out to be the most valuable of all:

4. **Functions the reference has *names* for but no body** (`INCLUDE_ASM`) —
   these still hand us the authoritative CriWare symbol name and position for
   functions we have as `func_020XXXXX`.

---

## 2. Headline: **five whole translation units are missing from our tree**

`config/usa/arm9/delinks.txt` records the exact `.text` range the original build
emitted for each source file. Sorting those ranges by address exposes the gaps
directly. Every one of them is a module the reference decomp has, except the
last two:

| Gap (`.text`) | Size | Contents | Reference file |
|---|---|---|---|
| `0x02012f88`–`0x02012f90` | 0x8 | 2 × 4-byte forwarders (`ADXM_Lock`/`ADXM_Unlock`) | inlined there |
| **`0x02017f80`–`0x02018cdc`** | **0xD4C** | **14 functions — the ADXT state machine / server (`adx_tsvr.c`)** | **`adx_tsvr.c` (10264 B)** |
| **`0x0201a534`–`0x0201a6b4`** | **0x180** | **9 functions — `sj_crs.c` + `sj_uni.c` + `sj_utl.c`** | **all three** |
| **`0x02021534`–`0x020215c4`** | **0x90** | **5 functions — `adx_amp.c`** | **`adx_amp.c` (2422 B)** |
| `0x020215fc`–`0x02021728` | 0x12c | 11 functions — CriWare-NDK string/printf module | — |
| `0x0201b8dc`–`0x0201ef48` | 0x3670 | 186 functions — CVFS device driver + ACSS support | ~ |

### 2.1 `adx_tsvr.c` — the largest gap, and it is the playback brain

`adxt_ExecHndl` (`0x02018c5c`, 0x80 bytes) **is declared in `adx_tlk.c:18` and
defined nowhere**. It is the last of the 14:

```
func_02017f80  0x84   func_02018b30  0x44
func_02018004  0x164  func_02018b74  0x4
func_02018168  0x8c   func_02018b78  0x5c
func_020181f4  0x44   func_02018bd4  0x6c
func_02018238  0x314  func_02018c40  0x1c
func_0201854c  0x3c4  adxt_ExecHndl  0x80
func_02018910  0x154
func_02018a64  0xcc
```

The reference's `adx_tsvr.c` has, in order: `adxt_trap_entry_lps`,
`adxt_trap_entry`, `adxt_eos_entry`, `adxt_set_outpan`, `adxt_nlp_trap_entry`,
`adxt_stat_decinfo`, `adxt_stat_prep`, `adxt_stat_playing`,
`adxt_stat_decend`, `adxt_stat_playend`, `adxt_RcvrReplay`,
`ADXT_ExecErrChk`, `ADXT_ExecRdErrChk`, `ADXT_ExecRdCompChk`,
`ADXT_ExecHndl`, `ADXT_GetStatRead` (16; the DS has 14, so two were trimmed).
`ADXT_ExecHndl` is positionally identical in both — that is the certain anchor.
The `adxt_stat_*` trio is what the 0x314/0x3c4/0x154-byte functions almost
certainly are; `func_0201854c` is confirmed by relocation fingerprint to be one
of them (it calls `func_02014b3c` = `ADXSJD_SetOutSj` twice, next to
`ADXSJD_Start`/`ADXSJD_GetStat`).

**Consequence:** `adxt_ExecServer` in `adx_tlk.c` is `/* NYI */` *and cannot be
written* until this exists, because it calls `adxt_ExecHndl` for every live
handle.

### 2.2 `sj_crs.c` / `sj_uni.c` / `sj_utl.c` — called but never defined

| Addr | Size | Identification | Reference |
|---|---|---|---|
| `0x0201a534` | 0x28 | `SJCRS_Init` | `sj_crs.c` |
| `0x0201a55c` | 0x28 | `SJCRS_Finish` | (DS-only refcount pair) |
| `0x0201a584` | 0x0c | `SJCRS_Lock` | `sj_crs.c` |
| `0x0201a590` | 0x0c | `SJCRS_Unlock` | `sj_crs.c` |
| `0x0201a59c` | 0x18 | `SJUNI_Init` | `sj_uni.c` |
| `0x0201a5b4` | 0x58 | `sjuni_Init` | `sj_uni.c` |
| `0x0201a60c` | 0x18 | `SJUNI_Finish` | `sj_uni.c` |
| `0x0201a624` | 0x4c | `sjuni_Finish` | `sj_uni.c` |
| `0x0201a670` | 0x44 | **`SJ_SplitChunk`** | `sj_utl.c` |

`SJCRS_Init`/`SJCRS_Finish` are **already called** from `sj_mem.c:80,98` and
`sj_rbf.c:87,104`; `SJ_SplitChunk` is **already called** from
`adx_stmc.c:326`. None are defined. `SJ_SplitChunk` ports verbatim (20 lines, no
platform conditionals). Proof of the identification: `relocs.txt` contains
seven `arm_call`s to `0x0201a670` from inside `adx_sjd.c`
(`0x02014c54, 0x02014e20, 0x0201507c, 0x02015134, 0x020152c4, 0x020153bc,
0x02015428`) — the exact split-points of the reference's `adxsjd_decode_prep` /
`decexec_start` / `decexec_end`.

### 2.3 `adx_amp.c`

`ADXAMP_Start`/`Stop`/`Destroy` are all **4-byte stubs** and `func_02021534` is
a 0x80-byte real body, so the reference's `ADXAMP_Destroy`/`Start`/`Stop`
(the only three it decompiles) map onto the two no-ops, and `func_02021534` is
the DS-only bulk of `ADXAMP_Destroy`. Only 0x90 bytes; low priority since the
game never uses the amplifier.

### 2.4 The 0x3670-byte block is a CVFS device driver, **not** `cri_srd.c`

186 functions with `ExecServer`/`GetStat`/`ReqRd`/`Seek`/`Tell`/`Open`/`Close`/
`OptFn1` shapes, called from `criss.c:39,54`, providing the `"MFS"` and
`"NITRO"` device factories (`func_0201d540`, `func_0201bfa0`). The reference's
nearest analogues are `dvci.c`/`htci.c`, not `cri_srd.c` (DVD/host I/O, absent
from this build entirely).

---


## 3. `adx_sjd.c` — our largest gap, and the reference covers it well

`ADXSJD` is the ADX/SuperJewel decode wrapper. Our file has 21 functions of
which ~15 are empty stubs, including two **1 KB-class** functions. The reference
decompiles all 38 of its own.

Stubbed in our tree but non-trivial in the ROM:

| Ours | Size | Reference | Note |
|---|---|---|---|
| `ADXSJD_Init` | 0x60 | `ADXSJD_Init` | stub |
| `ADXSJD_Finish` | 0x58 | `ADXSJD_Finish` | stub |
| `func_02014b94` | 0x2e0 | `adxsjd_decode_prep` | stub |
| `func_02014e74` | 0xd0 | `adxsjd_get_wr` | stub |
| `func_02014f44` | 0x400 | `adxsjd_decexec_start` | stub |
| `func_02015344` | 0x1ac | `adxsjd_decexec_end` | stub |
| `func_0201562c` | 0x130 | `adxsjd_decexec_extra` (1st half) | stub |
| `func_0201575c` | 0x11c | `adxsjd_decexec_extra` (2nd half) | stub |
| `func_02015878` | 0x6c | `ADXSJD_ExecServer` | stub |

The 0x8/0x10-byte stubs in the getter tail are trivially
`return ADXB_GetXxx(sjd->adxb);` tail calls; the reference gives the exact list.

### Alignment method used

Identification was by **address order + size + relocation fingerprints**, not
guessing. E.g. `relocs.txt` has `from:0x02014adc kind:load to:0x02014e74`,
i.e. `func_02014b94` takes the *address* of `func_02014e74` — that is
`ADXB_EntryGetWrFunc(sjd->adxb, adxsjd_get_wr, sjd)` in
`adxsjd_decode_prep`. Size cross-checks agree throughout
(`decode_prep` 0x2e0 vs ref 0x278; `decexec_end` 0x1ac vs ref 0x1c8).

### Struct field names (transfers directly)

| Off | Ours | Reference |
|---|---|---|
| 0x03 | `unk_03` | `unk3` (set to 1 when `ADXB_GetFormat()==4`) |
| 0x14 | `unk_14[0x18]` | `SJCK unk14` + `SJCK chunks[]` |
| 0x38 | `unk_38` | max-dec-samples (both set `0x7FFFFFFF`) |
| 0x3C | `unk_3C` | **`trap_num_samples`** (both set `-1`) |
| 0x40 | `unk_40` | **`trap_count`** |
| 0x44 | `unk_44` | **`trap_dt_len`** |
| 0x48 | `unk_48` | **`trap_func`** (`ADXSJD_EntryTrapFunc` writes 0x48/0x4C) |
| 0x4C | `unk_4C` | **`trap_obj`** |
| 0x50 | `unk_50` | **`flt_func`** |
| 0x54 | `unk_54` | **`flt_obj`** |
| 0x58 | `unk_58`…`unk_64[0x3C]` | **`char unk58[0x40]`** — a byte scratch buffer, *not* four scalars (ref does `memcpy(sjd->unk58, ck->data, MIN(ck->len, 0x40))`) |
| 0xA4 | `unk_A4` | **`lnkflg`** (`ADXSJD_SetLnkSw` writes it) |

0xA8 / 0xAC / 0xB0 are DS-only fields that gate `func_0201562c` /
`func_0201575c` inside `ADXSJD_ExecHndl`.

### Functions the DS build genuinely lacks

`ADXSJD_TermSupply`, `ADXSJD_GetDecPos`, `ADXSJD_GetLnkSw`,
`ADXSJD_SetExtString`/`SetDefExtString`, `ADXSJD_EntryFltFunc`,
`ADXSJD_GetTrapNumSmpl`/`GetTrapCnt`/`GetTrapDtLen`, `ADXSJD_GetBlkLen`,
`GetCof`, `GetLpInsNsmpl`, `GetLpStartPos/Ofst`, `GetLpEndPos/Ofst`,
`RestoreSnapshot`. No relocation references any of them — the DS build has no
seamless/link playback and no `lsc`. **Expected, not a gap.**

---

## 4. `adx_stmc.c` — a stub that initialises 11 struct fields

### 4.1 `ADXSTMF_SetupHandleMember` (0x02015b54, 0xe4 bytes) is `{}`

The reference supplies the complete body. It sets `stat`, `read_flg`, `sj`,
`cvfs`, `unkC`, `req_rd_size = 0x200`, the 0xFFFFF sector cap, `file_sct`,
`eos`, `cur_ofst`, `file_len`, `unk40` (from `SJ_GetNumData`), `unk0 = 1` and
`pause = 0` — **eleven fields our `Create` paths currently leave as garbage.**
Highest-value single transfer in this file.

### 4.2 `ADXSTM_Init` (0x02015aac, 0x58 bytes) is missing from our source

`adx_inis.c:68` calls it. The reference's version is a no-op `memset` + `return
1`; our build has clearly moved it into a refcount pair with `ADXSTM_Finish`
(which already reads `data_0206c3a4`). Size arithmetic fits:
`sjmem_Init` = 0x5c for the same `if (cnt==0) clear; cnt++`, `ADXSJD_Init` =
0x60 (one extra `BL` for `ADXB_Init`). **Our `ADXSTM_Finish` also has three
unused locals that should be deleted.**

### 4.3 `func_0201654c` (`ADXSTMF_ExecHndl`) reads an uninitialised variable

`adx_stmc.c` computes `uVar3 = iVar4 * 0x800; size = uVar3 >> 0x1F;` on the
`unk_58 != 0` path, then uses **both** in the sentinel test
`if (stm->unk_14 == 0 && stm->file_len == 0x7FFFF800)`. On the `unk_58 == 0`
path neither is ever assigned. These are the decompiler's split of a single
`s64`:

```c
Sint64 byte_len;
...
stm->file_len    = (s32)byte_len;
stm->unk_14      = (s32)(byte_len >> 32);   /* file_len_hi */
```

The sentinel `0x7FFFF800` is **identical in the reference** (`R:441`), and the
DS added the `unk_14 == 0` half of the 64-bit test. ⇒ `unk_14` is the *high*
word of the file length; rename to `file_len_hi`.

### 4.4 64-bit file lengths are a real DS-revision feature

`adx_tlk.c:328` calls
`ADXSTM_BindFileNw(stm, fname, dir, arg3, arg4 << 11, ((arg4 >> 0x1F) << 0xB) | ((u32)arg4 >> 0x15))`
— six args where the reference has five. The last two are the lo/hi halves of
`sectors << 11`, and `func_020564ec(lo, hi, 0x800, 0)` in `adx_stmc.c` is a
64-bit-safe `ceil(x / 0x800)` (the `0xfffff800 < lo` term is the carry out of
`lo + 0x7FF`). **Not a bug — keep, but document.**

### 4.5 `ADXSTM` field renames (high value; the `unk_*` names actively mislead)

| Off | Ours | Reference | Why it matters |
|---|---|---|---|
| 0x01 | `unk_01` | **`stat`** | 1=idle 2=streaming 3=eof 4=error |
| 0x02 | `unk_02` | **`read_flg`** | |
| 0x03 | `unk_03` | — | DS-only: realtime-stream flag |
| 0x08 | `fileHndl` | `cvfs` | |
| 0x0C | `unk_0C` | `unkC` (file offset, sectors) | |
| 0x14 | `unk_14` | — | **`file_len_hi`** |
| 0x18 | `unk_18` | **`file_sct`** | |
| 0x1C | `unk_1C` | **`unk18`** (SJ read-chunk size) | ⚠ name collision with ref's `unk1C` |
| 0x20 | `unk_20` | **`unk1C`** (flow limit) | ⚠ |
| 0x24 | `unk_24` | **`unk20`** (sectors in last request) | ⚠ |
| 0x28 | `unk_28` | `unk24` (in-flight `SJCK`) | |
| 0x34 | `unk_34` | **`eos`** | |
| 0x38 | `unk_38` | **`unk34`** (bytes read) | |
| 0x3C/0x40 | `unk_3C/unk_40` | `eos_callback` / `eos_callback_context` | |
| 0x44 | `unk_44` | `unk40` (total SJ data size) | |
| 0x48 | `unk_48` | **`pause`** | |
| 0x49–0x4D | `unk_49`…`unk_4D` | `unk45`…`unk49` (bind/close/start/stop/open pending) | |
| 0x50 | `unk_50` | `unk3` (retry count) | |
| 0x58 | `unk_58` | `dir` | |
| 0x5C | `unk_5C` | **`cur_ofst`** | ⚠ **the single most confusing one** |
| 0x60 | `unk_60` | `unk5C` (max sectors, `0xFFFFF`) | ⚠ |

### 4.6 Logic that is *identical* despite looking different

`func_02016188` (`adxstmf_stat_exec`) — the one comparison that looks inverted
is not:

```c
/* reference */
if (stm->cur_ofst >= stream_len || ((stm->unk34 >> 11) >= stm->unk5C && stm->unk5C <= 0xFFFFE)) stm->stat = 3;
/* ours */
if (stm->unk_5C >= iVar2) { stm->unk_01 = 3; }
else if (stm->unk_60 <= stm->unk_38 >> 0xb && stm->unk_60 < 0xfffff) { stm->unk_01 = 3; }
```

`a <= b` ≡ `b >= a`; `x < 0xFFFFF` ≡ `x <= 0xFFFFE`. Also verified identical:
the `SJ_SplitChunk` + `PutChunk`/`UngetChunk` + `unk_5C`/`unk_38` advance
block, the eos-callback condition, the retry counter, the
`unk_44 - SJ_GetNumData(sj,0) >= unk_20` flow check, and the
`unk_60 != 0xFFFFF` re-clamp. **No change needed.**

### 4.7 `SJRBF_PutChunk` lock scope — worth a ROM check

Our `SJRBF_PutChunk` (0x020196c4) is **0xc bytes — a bare tail call**, whereas
every other public `SJRBF_*` is 0x24–0x34 because it does
`SJCRS_Lock(); …; SJCRS_Unlock();`. In the reference the *whole* body is inside
the lock. Ours takes the lock only around the two counter updates. Either the
DS original genuinely leaves the memcpys outside the critical section, or a
wrapper was lost. Verify against the ROM bytes.

---

## 5. `svm.c` — header was a verbatim copy of the PS2 SDK header

`libs/include/CriWare/private/svm.h` was byte-for-byte the PS2 CriWare SDK
header, including declarations for functions that **do not exist in the DS
build**: `SVM_LockVar`, `SVM_UnlockVar`, `SVM_LockRsc`, `SVM_UnlockRsc`,
`SVM_SetCbBdr`. Fixed — the header now matches the DS binary and the reasoning
is recorded in comments.

### 5.1 `SVM_SetCbSvrId` was called with the wrong signature (fixed)

`adx_inis.c:81-82` calls:

```c
adxt_svr_fs_id   = SVM_SetCbSvrId(4, adxt_exec_fssvr, NULL, "adxt_exec_fssvr");
adxt_svr_main_id = SVM_SetCbSvrId(5, adxt_exec_main_thrd, NULL, "adxt_exec_main_thrd");
```

Four args, the last a **tag string**, and the result is used as a slot id by
`SVM_DelCbSvr(4, adxt_svr_fs_id)`. But `svm.c` declared
`SVM_SetCbSvrId(s32 svtype, s32 id, s32 (*func)(void*), void* object)` and
forwarded `(svtype, id, func, object)` into the inner
`svm_SetCbSvrId(s32 svtype, s32 (*func)(void*), void* object, char* tag)`.
Because ARM passes all four in `r0`–`r3` either way this still *compiled* and
still produced the same instructions — but semantically the callee was storing
`object = adxt_exec_fssvr` and `tag = NULL` ("Unknown") instead of
`object = NULL, tag = "adxt_exec_fssvr"`. Now fixed; generated code unchanged.

### 5.2 Naming evidence from the in-binary error strings

The error strings the DS binary itself contains are decisive:

| Function | String | Implied CriWare name |
|---|---|---|
| `svm_SetCbSvrId` (0x0201a8bc, static) | `"1051001:SVM_SetCbSvr:too many server function"` | body of `SVM_SetCbSvr` |
| `func_0201aa38` (0x0201aa38, static) | `"1071201:SVM_SetCbSvrId:illegal id"`, `"1071202:…:illegal svtype"`, `"2100801:SVM_SetCbSvrId:over write callback function."` | `svm_SetCbSvrId` |
| `svm_DelCbSvr` | `"1071206:SVM_SetCbSvrId:illegal svtype"` | copy-paste bug **in the original middleware**, not in our decomp |

Reference order is `SVM_SetCbSvr` (auto-slot, returns index) →
`SVM_DelCbSvr` → `SVM_SetCbSvrId` (explicit slot, void). The DS binary has the
same two operations but in the order `SVM_SetCbSvrId`(public, returns s32) →
`svm_SetCbSvrId` → `SVM_DelCbSvr` → `svm_DelCbSvr` → `func_0201a9fc` →
`func_0201aa38`, and both DS variants carry the extra `tag` parameter. So:

* our public `SVM_SetCbSvrId` (returns s32, auto-slot) ≡ reference `SVM_SetCbSvr`
* `func_0201a9fc`/`func_0201aa38` (explicit `id`, void) ≡ reference `SVM_SetCbSvrId`

Whether CriWare named the auto-slot variant `SVM_SetCbSvr` or `SVM_SetCbSvrId`
in the DS revision is genuinely ambiguous (its own error strings use both), so
the current name was kept and the alternative documented.

### 5.3 Struct layout — verified independently

`svm_svr_callbacks` spans `0x020700e0`–`0x02070320` = 0x240 = **8 × 6 × 12**,
confirming `SVMSVRCallback` is `{func, object, tag}` (the reference's has no
`tag`, hence 8 bytes — a genuine revision difference, *not* something to
"fix"). `svm_error_callback`/`svm_unlock_callback`/`svm_lock_callback` are 8
bytes each at `0x02070088/90/98`. The 0x40-byte gap before
`svm_svr_callbacks` has no relocations (unnamed padding).

### 5.4 Lock types

`func_0201a798`/`func_0201a7a8` are the only lock/unlock pair beyond
`SVM_Lock`/`SVM_Unlock`, and both use type **3**. The PS2 SDK numbers
1=base, 2=Var, 3=Sync, 4=Rsc, 5=Thrd, 6=Etc. The DS build has no threading, so
3 there is most plausibly `Rsc` with a renumbered enum — but this is **unproven**
and has been left unnamed.

---

## 6. `adx_crs.c` — provably wrong bss symbols (annotated in-tree)

`delinks.txt` gives `adx_crs.c` bss `0x0206bc30`–`0x0206bc3c` (12 bytes, three
dwords). `relocs.txt` shows the **only** relocation into that block is
`to:0x0206bc30`, from both `ADXCRS_Init` (`0x02012f44`) and `func_02012f48`
(`0x02012f6c`). Our decomp referenced `0x0206bc38` and `0x0206bc34`, which have
**zero** relocations anywhere — so those bodies cannot be right.

Renamed to the reference's names (`adxcrs_lvl` / `adxcrs_msk`, plus
`adxcrs_cnt` for the refcount) and marked `// Nonmatching` with the evidence.
`func_02012f48` → **`ADXCRS_Finish`** is certain: it is called from
`ADXT_Finish` at `0x02014838`, inside `ADXT_Finish`'s own range
`0x020147ac`–`0x02014888`.

Also: `func_02012f88` (0x4) and `func_02012f8c` (0x4) are 4-byte
unconditional branches with **no relocation**, which is only possible for a
short backwards branch — they are local forwarders to `ADXCRS_Lock`
(`0x02012f70`) and `ADXCRS_Unlock` (`0x02012f7c`). Every public entry point in
`adx_tlk.c`/`adx_stmc.c`/`adx_sjd.c`/`adx_fsvr.c`/`adx_tlk2.c` opens and closes
with them, i.e. they are the library's global critical section.

---

## 7. `adx_bsc.c` — one severe bug (fixed) plus a bss layout problem

### 7.1 `ADXB_EvokeExpandSte` passed 4 of 5 arguments — **breaks stereo decode**

Our definition of `ADXPD_EntrySte` takes five arguments and writes
`unk18 = arg1, unk1C = arg2, unk20 = arg3, unk24 = arg4`. The caller passed
four, with the last two collapsed into one sum:

```c
/* ours, before */
ADXPD_EntrySte(temp_r4, arg0->unk48.unk0, arg1 * 2,
               arg0->unk48.unk14 + (arg0->unk48.unk20 * 2) + (arg0->unk48.unk1C * 2));
/* reference */
a3 = unk->unk14 + (unk->unk20 * 2);
t0 = a3 + (unk->unk1C * 2);
ADXPD_EntrySte(adxpd, unk->unk0, arg1 * 2, a3, t0);
```

So `unk20` got `a3 + t0` instead of `a3`, and **`unk24` was never written at
all**. `ADXPD_ExecHndl` forwards `adxpd->unk24` to `ADX_DecodeSte4` as the
extra/inter-channel buffer pointer, so every stereo ADX playback was handing
decode a garbage pointer. It went unnoticed because `adx_xpnd.h` declared **no
prototypes at all**, so the compiler had nothing to complain about. Both the fix
and the missing declarations are in this pass.

### 7.2 `adx_bsc.c` declares two phantom 4-byte globals

The module's bss window is exactly `[0x0206b864, 0x0206b880)` — 28 bytes, 5
words + 3 `s16` — and `adxb_obj` starts at `0x0206b880`. Our source declares
7 words + 3 `s16` (34 bytes), which would push `adxb_obj` to `0x0206b888`.
**At least two of `{skg_err_func, skg_err_obj, pl2encodefunc, pl2resetfunc}`
must not exist in the DS binary.** `pl2encodefunc`/`pl2resetfunc` are the best
candidates — nothing in the tree references them — but the reference's
`SKG_EntryErrFunc`/`SKG_CallErrFunc` are what consume the other pair, so this
needs the ROM to settle.

### 7.3 Two empty stubs that are real code

| Ours | Size | Identification | Basis |
|---|---|---|---|
| `func_020122fc` | 0x40 | `adxb_clear` / `ADXB_InitObj` | the reference inlines exactly these four initialisations at the tail of `ADXB_Create`, and they explain the otherwise-uninitialised `ainf_len`, `unkC4` (`ADX_UNK`), `def_out_vol`, `def_pan[2]` |
| `func_02012ed8` | 0x48 | `ADXB_ExecOnePl2`-equivalent | the only remaining consumer of `unkE4`, which the reference gates `ADXB_EvokeExpandPl2` on |

### 7.4 Verified-not-a-bug

`ADXB_EvokeDecode`'s differences are pure reassociation of the same `MIN` chain
(ours nests `MIN(temp_lo, …)`, the reference builds up `temp_t2`), and our
`MIN` is `(a) <= (b)` vs the reference's `(a) < (b)` — numerically identical.

`ADXB_GetNumChan` and `ADXB_GetOutBps` *are* genuinely different: the reference
dispatches on `format` and does PL2 mono upmixing, while ours returns
`channel_count` / a constant `16`. That is NITRO trimming — the DS only
supports `format` 0 (ADX) and 0xA (AHX), both 16-bit — and our symbol sizes
(0x24 / 0x8) agree. **Keep.**

### 7.5 Renames available

`func_020127ec` → `ADXB_DecodeHeader` (high);
`func_020128b8/c0/d0/d8` → `ADXB_GetLpStartPos` / `GetLpStartOfst` /
`GetLpEndPos` / `GetLpEndOfst` (medium-high, 4-for-4 positional);
`func_020128fc` / `func_0201292c` → `ADXB_TakeSnapshot` /
`ADXB_RestoreSnapshot` (medium-high, only two `INCLUDE_ASM`s survive in that
slot).

---

## 8. `adx_dcd.c` — six "Nonmatching" markers that should stay

The reference's `ADX_DecodeInfo`, `ADX_DecodeInfoExADPCM2`, `…ExVer`,
`…ExIdly`, `…ExLoop`, `…Ainf` and `ADX_GetCoefficient` are **line-for-line
identical in logic** to ours, down to the `BSWAP_*` macro definitions and the
`*samples_per_block = (*block_size - 2) * 8 / *sample_bitdepth` expression.
⇒ The `// Nonmatching` markers on those are **codegen-level only**
(MWCC/MIPS vs MWCC/ARM float and comparison lowering). **Do not spend time on
them from this source.**

Data layout differs (reference global order `adxerr_*` vs ours
`adxerr_msg`/`adxerr_obj`/`adxerr_func`), so `adx_errs.c` must keep its own
ordering. `ADXERR_EntryErrFunc` exists in the reference but not the DS build
(no symbol between `ADXERR_Finish` and `ADXERR_CallErrFunc1`).

### 8.1 `adxt_vsync_cnt` is never incremented — probable real bug

The reference has `ADXT_VsyncProc() { adxt_vsync_cnt += 1; ADXT_ExecServer(); }`
plus a second increment in its PS2 vint hook. On DS, `adxt_vsync_cnt` is
**written only by `ADXT_Init` (`= 0`)** and read three times in `adx_tlk.c`
(`adxt->svcnt = adxt_vsync_cnt`, and in the `ADXT_GetTime*` timebase math).
Either the game bumps it from its own VBlank handler — **which must be
checked** — or DS playback-time and the `ADXT_Pause` resync are silently broken.
The reference makes the intended contract explicit.

### 8.2 Useful constants the reference names

`ADXT_OBUF_SIZE 2048` (we hardcode it), `ADXT_IBUF_XLEN 0x24` (we hardcode
`36` and `+ 0x24` — same value), `ADXT_PAN_AUTO -128`, `ADXT_DEF_SVRFREQ 60`,
`ADXT_MIN_BUFDATA 64`, `ADXT_MAX_NCH 2`, `ADXT_CH_L/R 0/1`,
`ADXT_FMT_ADX 1`, `ADXT_FMT_AHX 2`. AHX frames are `samples / 96`, AC3 frames
`samples / 1536`. `0x7FFFFFFF` is CriWare's "infinite" marker throughout.

---


## 9. `lsc.c` — nine `bx lr` stubs are **correct**

Every function is a 4-byte no-op, matching the reference's LSC being fully
compiled out. There is no `lsc_obj` pool in bss and no `ADXT_*Seamless*`
caller. **Do not "fix" this file.** Only cosmetic: `func_020215ec` is
positionally `LSC_EntryFname`.

Cosmetic C issues in this file worth cleaning:
* `LSC LSC_Create(SJ sj) { return; }` — returns no value from a non-void
  function (invalid C99).
* `adx_bahx.c`: `return ahxexecfunc(adxb);` inside a `void` function; the
  reference types `ahxexecfunc` as `Sint32`, we as a function pointer.

---

## 10. `cri_cvfs.c` — constants cross-validate; error strings do not

Every constant was verified against the ROM's bss spans and agrees:
`CVFS_HANDLE_MAX` = 10 (`cvfs_handles` 0x020705ec → 0x0207063c = 0x50 = 10×8),
`CVFSNamedDevice` = 32 × 16 bytes (`0x020703ec` → `0x020705ec`),
the 300-byte scratch buffer (`0x0207063c` → `def_dev` 0x02070768 = 0x12C),
`CVFS_MAX_NAME_LENGTH` = 297, `CVFS_DEVICE_MAX` = 32.

**But four error strings differ from the reference and string contents are part
of the match** — check these against `.rodata` before assuming our wording is
right:

| Case | Reference | Ours |
|---|---|---|
| `cvFsAddDev #3` | `"failed added a device"` | `"can not add device"` |
| `cvFsOpen #3` | `"failed handle alloced"` | `"can not allocate handle"` |
| `cvFsOpen #6` | `"open failed"` | `"can not open file"` |
| `cvFsGetStat #2` | `"vtbl error"` (8-byte padded) | same |

Also `data_020703e0` is loaded by both `cvFsCallUsrErrFn` (`0x0201ad90`) and
`cvFsEntryErrFunc` (`0x0201b664`), so it is one of
`{cvfs_errfn, cvfs_errobj}` rather than a separate counter — and `func_0201b884`
(0x58) is **`cvFsInit`**: it has exactly one call site, `0x020217bc`, which sits
inside `criss.c`'s own `.text` range (`0x02021728`–`0x02021fcc`), i.e. from the
empty `cri_ss_initialize` stub. It should own the refcount.

Note our decomp is *more complete* than the reference here: `cvFsGetFileSize`,
`cvFsGetFileSizeByHndl`, `cvFsGetVolumeInfo` and `cvFsSetDefVol` are
`INCLUDE_ASM` there and full C here.

---

## 11. House style of the DS revision (useful as a reconstruction template)

Every module in this build splits its public API into a thin wrapper plus a
`static` inner that takes the lock:

```c
void SJRBF_GetChunk(SJ sjrbf, ...) {   /* 0x34 bytes */
    SJCRS_Lock();
    sjrbf_GetChunk(sjrbf, ...);        /* static, 0x198 bytes */
    SJCRS_Unlock();
}
```

and pairs `Xxx_Init`/`Xxx_Finish` with a refcount:

```c
static void sjmem_Init(void) {
    if (sjmem_init_cnt == 0) { __builtin__clear(sjmem_obj, sizeof(sjmem_obj)); }
    sjmem_init_cnt++;
}
static void sjmem_Finish(void) {
    if (--sjmem_init_cnt == 0) { __builtin__clear(sjmem_obj, sizeof(sjmem_obj)); }
}
```

This is how to reconstruct the stubbed `ADXSJD_Init`/`ADXSJD_Finish` and
`ADXSTM_Init`. `ADXSJD` uses exactly this pattern — `relocs.txt` loads
`0x0206c0b4` (the counter) and `0x0206c0c8` (the array base) together at the
tails of both `ADXSJD_Init` (`0x02014934/38`) and `ADXSJD_Finish`
(`0x0201498c/90`), mirroring `sj_mem.c`.

The DS revision also adds an `Xxx_Log(ecode, edesc)` error-code family that the
reference has none of: `SJRBF` uses `E2004090201`…`E2004090226` and `SJMEM`
`E2004090231`…`E2004090248`, sequentially numbered in function order (201/202 =
NULL pointer / bad handle). Useful for filling in stubs.

---

## 12. What does **not** transfer

| Reference module | Why |
|---|---|
| `cri_srd.c` | DVD/host I/O via `sceCd*`/`sceIoctl` — no counterpart. (The unlabeled DS block at `0x0201b8dc` is a **CVFS device driver**, not SRD: it has `ExecServer`/`GetStat`/`ReqRd`/`Seek`/`Tell`/`Open`/`Close`/`OptFn1` shapes and provides the `"MFS"` and `"NITRO"` device factories.) |
| `sjx.c`, `sjr_clt.c`, `dtx.c`, `dtr.c` | SIF/IOP RPC — no counterpart |
| `dvci.c`, `htci.c` | PS2 CD-drive I/O — replaced by the DS CVFS device above |
| `adx_suht/sudv/mps2.c`, AC3 half of `adx_bahx.c` | codec/platform; confirmed by the complete absence of `ac3*` symbols in DS bss |
| `adx_amp.c` | exists in ref, not in our build |
| `adx_lsc.c`, `lsc_svr.c` | seamless playback — not compiled on DS |
| All of `criss.c`, `acss*.c`, `acsvhl.c`, `adx_f.c` | CRI Sound System (CRISS/ACSS/ACSV) — **the reference is ADX-only; it contains zero ACSS/CRSS/ACSV symbols.** No transfer possible for these. |

---

## 13. Ranked action list

### 13.0 Measured results of what was ported

USA baseline `39.158726%` fuzzy / `28.537209%` matched code / `4958` functions.
Current: **`39.382282%` / `28.604269%` / `4986` functions**, with
`build/usa/twewy_usa.nds: OK` throughout (the ROM SHA-1 never broke, which also
validates the bss layout changes).

> **Do not trust the intermediate number from commit `a4bb935`.** Its script
> replaced each `void func_...(ADXSJD* sjd) {}` stub with only the comment above
> it, leaving five functions called but undefined. They vanished from the report
> rather than scoring low, so that commit's `28.60854` was measured against a
> build that did not contain them and reads *higher* than the true value.
> Fixed in `ade5fc8`. When a script rewrites a stub into a comment, keep the
> definition -- the call sites are still there.

| Function | Before | After |
|---|---|---|
| `adx_bsc/ADXB_EvokeExpandSte` | 41.44 | **100.0** |
| `adx_crs/ADXCRS_Init` | 89.60 | 99.60 |
| `adx_crs/func_02012f48` | 79.10 | 99.60 |
| `adx_sjd/ADXSJD_Create` | 91.07 | 99.46 |
| `adx_sjd/ADXSJD_Init` | 1.67 | **100.0** |
| `adx_sjd/ADXSJD_Finish` | 1.82 | **100.0** |
| `adx_sjd/func_02015a2c` | 2.00 | **100.0** |
| `adx_sjd/{func_02015928, 02015948, 02015968, 02015988, 02015998, 020159c4, 020159d4, 02015a84, 02015a94}` | 23.75 ea | **100.0 ea** |
| `adx_sjd/{ADXSJD_GetSfreq, GetOutBps, GetTotalNumSmpl}` | 23.75 ea | **100.0 ea** |
| `adx_sjd/func_02014b3c` | 0.70 | **100.0** |
| `adx_sjd/func_02014b50` | 0.68 | **100.0** |
| `adx_sjd/func_02015554` (state-2 decode arm) | 1.48 | **100.0** |
| `adx_sjd/func_02014e74` (4-arg header reader) | 0.77 | 83.92 |
| `adx_sjd/func_02014f44` (main decode step, 0x400) | 0.16 | 95.42 |
| `adx_sjd/func_02015878` (`ADXSJD_ExecServer`) | 0.42 | **100.0** |
| `adx_sjd/func_020122fc`→`adx_bsc/adxb_clear` | 6.30 | 79.61 |
| `adx_stmc/ADXSTM_Init` | **0 (absent)** | 99.91 |
| `adx_stmc/ADXSTMF_SetupHandleMember` | 0.70 | 70.53 |
| `adx_tsvr/{adxt_ExecHndl, func_02018b30, func_02018b74, func_02018b78, func_02018bd4, func_02018c40}` | **not built** | **100.0 ea** |
| `adx_tsvr/func_02017f80` (trap entry) | **not built** | **100.0** |
| `adx_tsvr/func_02018004` (trap callback) | **not built** | **100.0** |
| `adx_tsvr/func_020181f4` (nlp trap entry) | **not built** | **100.0** |
| `adx_tsvr/func_02018a64` (`adxt_stat_playing`) | **not built** | 94.7 |
| `adx_tsvr/func_02018910` (`adxt_stat_prep`) | **not built** | 91.9 |
| `adx_tsvr/func_02018168` (eos/seek) | **not built** | 90.9 |
| `adx_tsvr/func_0201854c` (`adxt_stat_decinfo`) | **not built** | 89.6 |
| `adx_tsvr/func_02018238` | **not built** | 93.9 |
| `adx_tsvr/func_0201a670` (`SJ_SplitChunk`, 4 args) | 0.00 | 86.9 (arg order) |

### 13.0.0 mwcc findings from `func_02014f44`

The largest function in `adx_sjd.c`, and the one that took the longest to get
right. What the codegen settled that reading the target does not:

- **A `&&` chain is order-sensitive down to the byte.** `ADXB_GetFormat` → length
  → tag. Hoisting the tag read into a statement ahead of the `if` reorders all
  three tests and cost ~7%; the value must be computed inline in the condition.
- **`ldrsh` on the tag means the byte swap is on a *signed* 16-bit word**, and
  the result round-trips through `s16` before the `u16` compare. That is what
  produces the `lsl #16 / asr #16 / lsl #16` triple and the
  `cmp rConst, rVal, lsr #16`. Retail calls the macro `BSWAP_U16_EX`; this tree
  does not carry it, so it is defined locally in the `.c`.
- **A four-byte-immediate-looking `sub rN, rM, #0x8000000K` is a materialised
  constant, and the `K` is off by one from the value you would guess.** The
  target's `sub r2, r1, #0x80000002` is `0x7FFFFFFF`, not `0x7FFFFFFE`; the
  latter emits `#0x80000003` and is one byte-count low.
- **A loop that reloads its bound once and keeps it in a register needs a local.**
  The zero-scan reads `cki->length` into `r9` per pass, not per iteration; writing
  `i < cki->length` inline re-loads it every time and changes the shape.
- **Fall-through choice is per-branch and not consistent between the two
  branches of the same function.** The near-done test falls through on
  `decpos >= total` while the bitdepth test falls through on `== 0x10`. Getting
  either backwards only costs a couple of instructions, so try both.
- **`unk_14` is `SJCK cki` at `0x14` plus `cko[2]` at `0x1C`**, which the header
  models as one `0x18` byte array. Anything reading `sjd + 0x14..0x20` has to
  cast back into it. Same trap cost a build error in `func_02014e74`.
- **Error strings are recoverable from the target object** by scanning
  `build/<region>/delinks/<tu>.o` for printable runs. The pair passed to
  `ADXERR_CallErrFunc2` are adjacent in `.rodata`, and the split matches the
  `"EXX... <func>: "` / `"<message>"` convention already used in `adx_bsc.c`.


### 13.0.1 What `adx_tsvr.c` needed, and what it taught

The TU was missing, not broken. Getting it in required three things beyond the
source file:

1. **`config/usa/arm9/delinks.txt` entry.** dsd splits the original module by
   `delinks.txt` and generates `_dsd_gap@main_N.o` objects for whatever no entry
   covers; that filler is what had been standing in for these 0xD4C bytes.
   The new entry is `.text 0x02017f80-0x02018cdc`, `.data 0x02063edc-0x02063f74`,
   `.bss 0x0206c7fc-0x0206c80c` — the `.data`/`.bss` extents come from the
   `load` relocations originating in that range, cross-checked against the
   neighbouring TUs' ranges. After adding it, `arm9.lcf` lists `adx_tsvr.o(.text)`
   between `adx_tlk2.o` and `adx_xpnd.o` with no gap filler between them.
2. **An `objdiff.json` unit**, or there is no target to diff against and none of
   the 14 functions appear in the report.
3. **Address-based function names**, as in 13.1 — including `adxt_ExecHndl`,
   which `adx_tlk.c` already forward-declares as `static`, and which the symbol
   table already names (so it keeps that one name).

`adxt_ExecHndl`'s dispatch chain pins the tail of the mapping exactly and
**corrected two guesses**: stat 1 -> `func_0201854c` (not `func_02018238`),
stat 2 -> `func_02018910`, stat 3 -> `func_02018a64`, stat 4 -> `func_02018b30`,
stat 5 -> `func_02018b74`, then `func_02018bd4` and `func_02018b78` always. So
`func_0201854c` (0x3c4) is `adxt_stat_decinfo` and `func_02018238` (0x314) is
something else. It also shows the body is an `if`/`else if` chain in the order
3, 1, 2, 4, 5 — not a switch — while `func_02018bd4` really is a `switch`
(`addls pc, pc, pmode, lsl #2`), so the two must be written differently.

Note `adxt_ExecServer` in `adx_tlk.c` is *still* `/* NYI */` — it calls
`adxt_ExecHndl`, which now exists, but it also needs `adxt_tsvr_enter_cnt` and the
not-yet-written state handlers to be worth enabling.

### 13.0.2 mwcc findings from the eight handlers

Three of these cost real time and are worth recording, because none of them are
guessable from the C:

1. **`mov r1, r0, asr #10 / add r1, r0, r1, lsr #21 / asr #11` is a signed
   division by 2048**, not by 1000. The rounding term is what distinguishes a
   division from a plain shift. Both `func_02018168` and `func_0201854c` use it,
   and reading it as `/1000` costs ~16% and ~4% respectively. The
   `rsb`/`ror #21` variant is the same operation for a divisor the compiler
   materialised into a register.
2. **mwcc does not honour declaration order for same-sized file statics.** Four
   loose `s32` globals came out `0x800, 0x7fc, 0x808, 0x804` regardless of how
   they were declared, which silently broke both `func_0201854c`'s callback load
   and `func_02018b30`'s store. Wrapping them in a struct pins the offsets.
   Cost: `func_02018b30` went 100 -> 99.6 because the target reaches that word
   through its own literal rather than a struct base. Net win.
3. **Block layout encodes the source's control-flow *shape*, not just its
   semantics.** `func_0201854c`'s `ADXSJD_GetStat` guard written as an early
   return puts the `stat == 4` arm inline; written as `if (stat == 2) { ... }
   else if (stat == 4) { ... }` mwcc branches over the whole body to a trailing
   arm, which is what the target does. Worth 4% on a 241-instruction function.
   The same applies to `func_02018004`, where the target's `bne` lands past only
   the stream re-prime -- so `func_02015a94` and the `lpcnt` bump are *outside*
   the `if`, not inside it.

Where the PS2 reference and the NITRO build disagree, the NITRO target wins and
the reference is only a structural guide. The `func_02018910` clamp to 0x800, the
unsigned `(u8)pmode <= 1` guard, and the use of the channel index as both the SJ
chunk id and the `memset` fill byte are all NITRO-only.

### 13.1 Deliberately **rejected**: renames

objdiff pairs target and base symbols **by name**, so a rename does not improve
matching — it removes the function from the accounting entirely. `func_02012f48`
measured 79.1%, was renamed to `ADXCRS_Finish`, and then reported as *absent*
even though the emitted code was byte-identical. That rename has been reverted
and the rename proposals in the sections above are **not applied**. Names live in
`config/*/arm9/symbols.txt`; change them there (`ninja apply`) if you want them,
never in the `.c` file.

Two earlier findings also turned out to be wrong and are corrected here:

* The ADXSJD refcount is **not** at `0x0206c0b4`. It is a field at `+0xc` of one
  0x14-byte object anchored at `0x0206c0b4` (now `ADXSJD_CTRL` in the source).
  The original `BOOL data_0206c0c0;` was not a phantom.
* `ADXCRS_Init`/`func_02012f48`'s bodies were already right; they only needed the
  three globals marked **`volatile`**. The store-then-reload-then-`cmp` in the
  target is the classic volatile-global signature, and that alone took both from
  89.6/79.1 to 99.6.

### 13.2 Done in this pass

| # | Change | Effect |
|---|---|---|
| 1 | `adx_bsc.c`: `ADXB_EvokeExpandSte` passes all five `ADXPD_EntrySte` args | 41% -> 100% |
| 2 | `adx_xpnd.h`: added the `ADXPD_*` prototypes whose absence hid #1 | prevents regression |
| 3 | `adx_sjd.c`: `adxsjd_obj[16]` -> `[ADXSJD_MAX_OBJ]` (4) | bss span 0x2D0 = 4 x 0xB4 |
| 4 | `adx_sjd.c`: `sjd->sjo[i]` -> `sjd->sjo[j]` in `ADXSJD_Create` | 91% -> 99.5% |
| 5 | `adx_sjd.c`: `ADXSJD_Init`/`Finish` implemented against the `ADXSJD_CTRL` layout | both -> 100% |
| 6 | `adx_sjd.c`: 13 tail-call getters implemented | 23.75/2.0% -> 100% each |
| 7 | `adx_crs.c`: three globals made `volatile` | 89.6/79.1% -> 99.6% |
| 8 | `adx_stmc.c`: `ADXSTM_Init` implemented (was entirely absent) | 0% -> 99.9% |
| 9 | `adx_stmc.c`: `ADXSTMF_SetupHandleMember` implemented | 0.7% -> 29.1% |
| 10 | `svm.c`/`svm.h`: `SVM_SetCbSvrId` signature + header corrected | semantics; codegen-neutral |
| 11 | **`libs/CriWare/adx_tsvr.c` created** (0xD4C bytes) + `delinks.txt` entry + `objdiff.json` unit | TU measurable at all; 6 functions -> 100% |

### 13.3 Next, highest value first

| # | Action | Payoff |
|---|---|---|
| 1 | **Finish `adx_tsvr.c`**: only `func_02018238` (0x314) is still a stub. Its target is decoded in the objdiff JSON; the PS2 reference has no counterpart, but its shape is two near-identical halves differing only in which decoder they call (`func_02021534` vs `func_020130dc`). | 0x314 bytes at 0.2% |
| 2 | Close the register-allocation gaps in `func_0201854c` (89.6), `func_02018910` (91.9), `func_02018168` (90.9) and `func_02018a64` (94.7). In each case the body is instruction-for-instruction correct and only mwcc's choice of callee-saved registers (and, for 0x18a64, how it clusters the two bss literals) differs. | ~0.4 KB |
| 3 | Enable `adxt_ExecServer` in `adx_tlk.c` (still `/* NYI */`, 0xe8 bytes) — `adxt_ExecHndl` now exists, but it also needs `adxt_tsvr_enter_cnt` (unnamed bss dword near `adxt_time_mode`) | 0xe8 bytes |
| 4 | **Add `adx_rna.c` / `adx_rna2.c`.** `func_02018004`–`func_0201854c` call nine ADXRNA entry points (`func_0201bf0c`..`func_0201bf70`) that have delinks entries and target bytes but *no source file at all* — a second unfilled hole, same shape as `adx_tsvr`. | unblocks the ADXRNA call sites |
| 3 | Close `ADXSTMF_SetupHandleMember`'s round-up division (currently 29%). The target's sector count is `(file_len >> 11) + (magic_div_quotient > 0 ? 1 : 0)` where the magic divide is by `0x200000`; neither `file_len / 0x800` nor the PS2 reference's round-up form reproduces it (the latter scores worse, 23%). | ~0xb4 bytes -> 100% |
| 4 | Model the `0x0206c398` 0x18-byte control object in `adx_stmc.c` so `ADXSTM_Init` anchors its pool at `0x398` and reaches the refcount at `+0xc` | Init 99.9% -> 100% |
| 5 | Same for `adx_crs.c`: the target anchors at `0x0206bc30` and reaches its two globals at `+4`/`+8`; ours anchors at `+0`/`+4`. Needs `adxcrs_lvl` inside mwcc's addressing cluster. | 2 x 99.6% -> 100% |
| 6 | Implement `adxt_SetLpFlg` from the reference (`/* NYI */`, 0xdc bytes) | 8.4% -> ? |
| 7 | Add `sj_utl.c` / `sj_crs.c` / `sj_uni.c` (2.2) - 0x180 bytes, 9 functions already called from `sj_mem.c`/`sj_rbf.c`/`adx_stmc.c` | fixes 3 broken call sites each |
| 8 | Remaining `adx_sjd` core: `func_02014b94` (0x2e0), `func_02014f44` (0x400), `func_02015344` (0x1ac), `func_0201562c` (0x130), `func_0201575c` (0x11c), `func_02014e74` (0xd0) - all <=1% | ~2.2 KB of near-zero code |
| 9 | Rewrite the `func_0201654c` sentinel block with a real `Sint64` (4.3) | fixes a read of an uninitialised variable |
| 10 | Settle the `adx_bsc.c` phantom-globals problem (7.2) | bss layout for the whole module |
| 11 | Check whether the game's VBlank handler bumps `adxt_vsync_cnt` (8.1) | possible playback-timing bug |
| 12 | `func_020122fc` (`adxb_clear`, 6.25%) and `func_02012ed8` (2.22%) | 0x88 bytes |
| 13 | Verify the four `cri_cvfs.c` error strings against `.rodata` (10) | 4 strings in the match |
| 14 | Check `SJRBF_PutChunk`'s lock scope against the ROM (4.7) | possible missing wrapper |
| 15 | Re-check `ACSSBADX_StartFnameRange` in `acssbadx.c`, which calls our 4-arg `ADXT_StartFnameRange` with one argument | probable bug |

### 13.4 Explicitly *not* worth doing
* The six `// Nonmatching` markers in `adx_dcd.c` (§7) — reference confirms our logic.
* `lsc.c` (§9).
* Anything in `criss.c` / `acss*.c` / `acsvhl.c` / `adx_f.c` (§12).
* `adx_sjd/func_02014e74`'s residual 16% — the three out-pointer registers and
  the loop's `GetNumChan` reload are the only differences, and the two ways of
  spelling the guard (hoisted `nch` vs. re-called) both score below the current
  form, so there is nothing left to try short of the permuter.
* `adx_sjd/func_02014f44`'s residual 4.6% — the zero-scan's loop rotation is
  fixed by mwcc regardless of which order the two conditions are written in
  (both `*p == 0 && i < len` and the explicit `break` form were measured), and
  what is left is register choice.
* Reconstructing `adxt_tsvr_enter_cnt`, `ADXSJD_ExecServer`, `ADXRNA_ExecServer`
  call graphs before the ADXSJD core exists — `adxt_ExecServer` needs
  `adxt_tsvr_enter_cnt` (an unnamed dword near `adxt_time_mode` in bss) and
  `ADXRNA_ExecServer`, which does not exist as a symbol on DS (the DS RNA
  module is a thin `ADXRNA_Init`/`Finish`/`SetTransSw`/`SetPlaySw`/`SetOutVol`
  veneer only).
