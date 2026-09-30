#include <cri/adx_sjd.h>

// adx_sjd.c owns bss 0x0206c0b4-0x0206c398 (0x2E4 bytes) per delinks.txt.
// ADXSJD_Init/Finish load the address 0x0206c0b4 and reach the refcount at
// [r0 + 0xc] == 0x0206c0c0, so the refcount is a FIELD of one 0x14-byte object
// anchored at 0x0206c0b4 rather than a standalone global -- symbols.txt has no
// separate symbol at 0x0206c0c0, and a group of standalone `int`s gets pruned by
// mwcc instead of holding the address. The object array follows at 0x0206c0c8,
// and Init/Finish clear 0x2D0 bytes (45x `stmia lr!`), which is exactly
// ADXSJD_MAX_OBJ * sizeof(ADXSJD) == 4 * 0xB4.
// 0x00/0x08 hold a callback and 0x04/0x10 its user pointer -- but crossed
// round, not paired: the 0x08 callback is handed the 0x04 object and the 0x00
// callback the 0x10 one. That is how the target's func_02015878 reads them.
typedef struct {
    /* 0x00 */ void (*unk00)(void*);
    /* 0x04 */ void* unk04;
    /* 0x08 */ void (*unk08)(void*);
    /* 0x0C */ volatile int init_cnt;
    /* 0x10 */ void*        unk10;
} ADXSJD_CTRL; // Size: 0x14

ADXSJD_CTRL data_0206c0b4              = {0};
ADXSJD      adxsjd_obj[ADXSJD_MAX_OBJ] = {0};

void  ADXB_SetAhxDecSmpl(ADXB adxb, int decsmpl);
void  ADXB_AhxTermSupply(ADXB adxb);
int   ADXB_GetStat(ADXB adxb);
void  ADXB_ExecHndl(ADXB adxb);
int   ADXB_GetNumChan(ADXB adxb);
void* ADXB_GetPcmBuf(ADXB adxb);
void* SJRBF_GetBufPtr(SJ sj);
int   func_02012258(void);
int   func_020127ec(ADXB adxb, unsigned short* header, int len);
void  func_02012748(ADXB adxb);
int   ADXB_GetSfreq(ADXB adxb);
int   ADXB_GetTotalNumSmpl(ADXB adxb);
int   ADXB_GetDecDtLen(ADXB adxb);
int   ADXB_GetDecNumSmpl(ADXB adxb);
void  ADXB_Reset(ADXB adxb);

void func_0201a670(SJCK* ck, int nbyte, SJCK* ck1, SJCK* ck2);

void func_0201575c(ADXSJD* sjd);
void func_0201562c(ADXSJD* sjd);

int func_02015aa4(ADXSJD* sjd);

void* adxsjd_get_wr(ADXSJD sjd, int* arg1, int* arg2, int* arg3);

void ADXB_Init();

void ADXSJD_Init(void) {
    if (data_0206c0b4.init_cnt == 0) {
        ADXB_Init();
        __builtin__clear(adxsjd_obj, sizeof(adxsjd_obj));
    }

    data_0206c0b4.init_cnt++;
}

void ADXSJD_Finish(void) {
    if (--data_0206c0b4.init_cnt == 0) {
        __builtin__clear(adxsjd_obj, sizeof(adxsjd_obj));
    }
}

void ADXSJD_Clear(ADXSJD* sjd) {
    sjd->hdrlen         = 0;
    sjd->total_decsmpl  = 0;
    sjd->total_decdtlen = 0;
    sjd->decpos         = 0;
    sjd->maxdecsmpl     = 0x7FFFFFFF;
    sjd->dtrpsmpl       = -1;
    sjd->dtrpcnt        = 0;
    sjd->dtrpdtlen      = 0;
    sjd->empty_end      = 0;
    sjd->unk_A8         = 0;
    sjd->unk_AC         = 0;
}

ADXSJD* ADXSJD_Create(SJ sj, int maxChans, SJ* sjo) {
    ADXSJD* sjd;
    SJ      out;
    void*   buf_ptr;
    int     i;
    int     j;
    int     buf_size;
    int     xtr_size;

    out = sjo[0];

    for (i = 0; i < ADXSJD_MAX_OBJ; i++) {
        if (adxsjd_obj[i].used == FALSE) {
            break;
        }
    }
    if (i == ADXSJD_MAX_OBJ) {
        return NULL;
    }

    sjd      = &adxsjd_obj[i];
    buf_ptr  = SJRBF_GetBufPtr(out);
    buf_size = SJRBF_GetBufSize(out) / 2;
    xtr_size = SJRBF_GetXtrSize(out) / 2;

    sjd->adxb = ADXB_Create(maxChans, buf_ptr, buf_size, buf_size + xtr_size);
    if (sjd->adxb == NULL) {
        return NULL;
    }

    ADXB_EntryGetWrFunc(sjd->adxb, adxsjd_get_wr, sjd);

    sjd->sji    = sj;
    sjd->maxnch = maxChans;

    for (j = 0; j < maxChans; j++) {
        sjd->sjo[j] = sjo[j];
    }

    sjd->state = 0;
    ADXSJD_Clear(sjd);
    sjd->dtrpfunc = 0;
    sjd->dtrpobj  = 0;
    sjd->dfltfunc = 0;
    sjd->dfltobj  = 0;
    sjd->used     = TRUE;
    return sjd;
}

void ADXSJD_Destroy(ADXSJD* sjd) {
    if (sjd == NULL) {
        return;
    }

    ADXB adxb = sjd->adxb;
    if (adxb != NULL) {
        sjd->adxb = NULL;
        ADXB_Destroy(adxb);
    }
    ADXCRS_Lock();
    memset(sjd, 0, sizeof(ADXSJD));
    ADXCRS_Unlock();
}

char ADXSJD_GetStat(ADXSJD* sjd) {
    return sjd->state;
}

void ADXSJD_SetInSj(ADXSJD* sjd, SJ sj) {
    sjd->sji = sj;
    ADXB_SetAhxInSj(sjd->adxb, sj);
}

// ADXSJD_SetMaxDecSmpl. The PS2 reference also calls ADXB_SetAc3DecSmpl
// here; NITRO has no AC3, and the target's tail call names ADXB_SetAhxDecSmpl.
void func_02014b3c(ADXSJD* sjd, int nsmpl) {
    sjd->maxdecsmpl = nsmpl;
    ADXB_SetAhxDecSmpl(sjd->adxb, nsmpl);
}

// ADXSJD_TermSupply -- AHX only, for the same reason.
void func_02014b50(ADXSJD* sjd) {
    ADXB_AhxTermSupply(sjd->adxb);
}

void ADXSJD_Start(ADXSJD* sjd) {
    ADXSJD_Clear(sjd);
    sjd->state = 1;
}

void ADXSJD_Stop(ADXSJD* sjd) {
    ADXB_Stop(sjd->adxb);
    sjd->state = 0;
}

// Nonmatching: 99.92% -- every instruction in the body matches. The only
// residue is three literal-pool words whose relocations objdiff names
// `data_020639cc` / `data_020639ec` / `data_02063a0c` in the delinked target
// and `@635` / `@636` / `@637` here, because the strings are anonymous
// .rodata in a C file and have no symbol to pair with. Not fixable from source.
//
// The state-1 arm of ADXSJD_ExecHndl, upstream adxsjd_decode_prep. It pulls a
// chunk off the input stream, finds where the real audio starts by skipping
// leading zero bytes, and either bails out (bad alignment, or a header the ADXB
// will not take) or hands the header to the ADXB and moves the object to state
// 2.
//
// The two failure strings are recovered from the target object's rodata:
//   0x020639ac "E04102501 adxsjd_decode_prep: "
//   0x020639cc "The data alignment is illegal."
//   0x020639ec "E03010901 ADXB_DecodeHeader: "
//   0x02063a0c "Can not decode this file format."
void adxsjd_decode_prep(ADXSJD* sjd) {
    ADXB  adxb = sjd->adxb;
    SJ    sji  = sjd->sji;
    SJCK  ck;
    SJCK  ck2;
    char* p;
    int   i;
    int   hdrlen;
    int   fmt;

    SJ_GetChunk(sji, 1, 0xC800, &ck);

    // Skip the leading zero bytes to find the split point. The length test is
    // the loop's entry guard, so a zero-length chunk never dereferences it.
    i = 0;

    if (ck.length > 0) {
        p = ck.data;

        // Written as an unrolled loop with two breaks on purpose: a plain
        // `while (*p == 0 && i < ck.length)` gets bottom-tested by mwcc, and the
        // bound test has to use the already-incremented i (so this runs one
        // pass fewer than `i < ck.length` would).
        for (;;) {
            if (*p != 0) {
                break;
            }

            i++;
            p++;

            if (i >= ck.length) {
                break;
            }
        }
    }

    // An odd number of leading zero bytes means the data is not 16-bit
    // aligned. This is a % not a /: the modulo lowering is a three-instruction
    // sequence where the divide is two, and the target has the three.
    if (i % 2 == 1) {
        SJ_UngetChunk(sji, 1, &ck);

        if (func_02012258() == 0) {
            ADXERR_CallErrFunc2("E04102501 adxsjd_decode_prep: ", "The data alignment is illegal.");
        }

        sjd->state = 4;
        return;
    }

    // Put the padding back, then keep the header half for the ADXB to read.
    func_0201a670(&ck, i, &ck2, &ck);
    SJ_PutChunk(sji, 0, &ck2);

    if (ck.length < 0x10) {
        SJ_UngetChunk(sji, 1, &ck);
        return;
    }

    hdrlen = func_020127ec(adxb, (unsigned short*)ck.data, ck.length);

    if (hdrlen == 0 || hdrlen > ck.length) {
        SJ_UngetChunk(sji, 1, &ck);
        return;
    }

    if (hdrlen < 0) {
        if (adxb->unk9A != 0) {
            // The ADXB will not take this format, but unk9A names one it will,
            // so fall back to that and carry on with no header.
            func_02012748(adxb);
            hdrlen = 0;
        } else {
            SJ_UngetChunk(sji, 1, &ck);

            if (func_02012258() == 0) {
                ADXERR_CallErrFunc2("E03010901 ADXB_DecodeHeader: ", "Can not decode this file format.");
            }

            sjd->state = 4;
            return;
        }
    }

    sjd->hdrlen = hdrlen;

    if (sjd->dfltfunc != NULL) {
        sjd->dfltfunc(sjd->dfltobj, ADXB_GetFormat(adxb), ADXB_GetNumChan(adxb), ADXB_GetSfreq(adxb),
                      ADXB_GetTotalNumSmpl(adxb));
    }

    if (ADXB_GetFormat(adxb) == 4) {
        sjd->empty_end = 1;
    }

    if (ADXB_GetFormat(adxb) == 2) {
        memcpy(sjd->spsdinfo, ck.data, (ck.length < 0x40) ? ck.length : 0x40);
    }

    // These formats want the whole chunk handed straight back; anything else
    // gets trimmed to the header length first.
    fmt = ADXB_GetFormat(adxb);

    if (fmt == 10 || fmt == 11 || fmt == 12 || fmt == 20 || fmt == 15) {
        SJ_UngetChunk(sji, 1, &ck);
    } else {
        func_0201a670(&ck, hdrlen, &ck, &ck2);
        SJ_PutChunk(sji, 0, &ck);
        SJ_UngetChunk(sji, 1, &ck2);
    }

    sjd->state = 2;
}

// Fills three caller-supplied counts and returns the decoder's PCM buffer.
void func_02014e74(ADXSJD* sjd, int* out_wpos, int* out_room, int* out_loop) {
    SJCK* cko  = sjd->cko;
    SJ    sjo0 = sjd->sjo[0];
    int   nch;
    int   i;

    nch = ADXB_GetNumChan(sjd->adxb);

    for (i = 0; i < nch; i++) {
        SJ_GetChunk(sjd->sjo[i], i, 0x4000, &cko[i]);
    }

    *out_wpos = (int)(cko[0].data - SJRBF_GetBufPtr(sjo0)) / 2;
    *out_room = (cko[0].length / 2 < sjd->maxdecsmpl) ? cko[0].length / 2 : sjd->maxdecsmpl;

    // unk_3C is the trap sample count. When it has been cleared to a negative
    // sentinel the remaining-loop count is saturated rather than computed --
    // ~0xE0000000 == 0x1FFFFFFF.
    *out_loop = (sjd->dtrpsmpl >= 0) ? sjd->dtrpsmpl - sjd->dtrpcnt : 0x1FFFFFFF;

    // Result discarded: the target's epilogue pops straight to pc without
    // writing r0, so the original called this without returning it.
    ADXB_GetPcmBuf(sjd->adxb);
}

// BSWAP_U16_EX from retail CriWare's sj.h, which this tree does not carry.
// The argument is read as a signed 16-bit word, hence the asr in the codegen.
#define BSWAP_U16_EX(x) ((unsigned short)(short)(((((short)(x)) >> 8) & 0xFF) | ((((short)(x)) << 8) & 0xFF00)))

// Nonmatching: 97.2% (0x400 bytes, the largest function in this file). All the
// control flow, both short-circuit chains and the tag byte-swap now match
// instruction for instruction; what is left is register choice. sjd->sjo[0] is
// hoisted into r8 instead of being passed straight through r0, decpos lands in
// r1 rather than r8, and the near-done flag folds to a single movne where the
// target keeps a separate mov + b. The two .word pool entries differ only in
// relocation name (anonymous .rodata vs the delinked data_02063a30/54).
//
// The main decode step, entered from adxsjd_decode_exec when adxb->stat == 0. Two
// distinct jobs live here: recognising and consuming a leading ADX sub-header
// (the big-endian 0x8001 tag), and otherwise handing the decoded audio to the
// ADXB. The sub-header path always returns, so the tail is the PCM path.
void adxsjd_decexec_start(ADXSJD* sjd) {
    ADXB  adxb = sjd->adxb;
    SJ    sji  = sjd->sji;
    SJCK  ck1;
    SJCK  ck2;
    SJCK* cki = &sjd->cki;
    short ofst;
    int   i;
    int   len;
    int   total;
    int   done;

    done = 0;

    // The loop callback only fires once the trap has been fully consumed.
    if (sjd->dtrpsmpl >= 0 && sjd->dtrpcnt >= sjd->dtrpsmpl) {
        if (sjd->dtrpfunc != NULL) {
            sjd->dtrpfunc(sjd->dtrpobj);
        }
    }

    if (sjd->empty_end == 1 && SJ_GetNumData(sji, 1) == 0) {
        sjd->state = 3;
        return;
    }

    SJ_GetChunk(sji, 1, 0x7FFFFFFF, cki);

    // A leading big-endian 0x8001 tag marks this chunk as an ADX sub-header
    // rather than PCM.
    if (ADXB_GetFormat(adxb) == 0 && cki->length >= 4 && BSWAP_U16_EX(*(short*)cki->data) == 0x8001) {
        sjd->state = 3;

        if (func_02013868(cki->data, cki->length, &ofst) == 0) {
            if (ofst > cki->length) {
                SJ_UngetChunk(sji, 1, cki);
                return;
            }

            func_0201a670(cki, ofst, cki, &ck1);
            SJ_PutChunk(sji, 0, cki);
            SJ_UngetChunk(sji, 1, &ck1);
        }

        if (sjd->unk_A4 == 0) {
            return;
        }

        // Peel leading zero bytes off the stream one chunk at a time. The byte
        // test runs before the bound test, so an all-zero chunk makes no
        // progress and the loop only ends once a chunk is fully consumed.
        for (;;) {
            char* p;

            SJ_GetChunk(sji, 1, 0x7FFFFFFF, cki);

            len = cki->length;

            if (len == 0) {
                return;
            }

            i = 0;

            if (len > 0) {
                p = cki->data;

                // Same two-break shape as adxsjd_decode_prep's scan: a combined while
                // condition gets bottom-tested here, and the bound test has to
                // use the already-incremented i.
                for (;;) {
                    if (*p != 0) {
                        break;
                    }

                    i++;
                    p++;

                    if (i >= len) {
                        break;
                    }
                }
            }

            func_0201a670(cki, i, cki, &ck1);
            SJ_PutChunk(sji, 0, cki);
            SJ_UngetChunk(sji, 1, &ck1);

            if (i < len) {
                return;
            }
        }
    }

    total = ADXSJD_GetTotalNumSmpl(sjd);

    if (sjd->decpos >= total) {
        if (ADXB_GetFormat(adxb) == 1) {
            if (func_02015aa4(sjd) != 1) {
                done = 1;
            }
        } else {
            done = 1;
        }
    } else if (ADXB_GetFormat(adxb) == 10 && sjd->decpos + 0x240 >= total) {
        done = 1;
    }

    if (done != 0) {
        sjd->state = 3;
        SJ_UngetChunk(sji, 1, cki);
        return;
    }

    if (func_02015968(sjd) > SJ_GetNumData(sjd->sjo[0], 0) / 2) {
        SJ_UngetChunk(sji, 1, cki);
        return;
    }

    if (func_02015aa4(sjd) != 1 && ADXB_GetFormat(adxb) == 1) {
        if (ADXB_GetBitdepth(adxb) == 0x10) {
            int nch  = ADXB_GetNumChan(sjd->adxb);
            int have = sjd->decpos + cki->length / nch / 2;

            // Trim the chunk down to the audio that is still outstanding.
            if (have > total) {
                func_0201a670(cki, nch * (total - have) * 2, cki, &ck2);
                SJ_UngetChunk(sji, 1, &ck2);
            }
        } else {
            ADXERR_CallErrFunc2("E07021901 adxsjd_decexec_start: ", "8 or 4bit WAV file can't playback continuously.");
        }
    }

    if (ADXB_GetFormat(adxb) == 10) {
        SJ_UngetChunk(sji, 1, cki);
    }

    ADXB_EntryData(adxb, cki->data, cki->length);
    ADXB_Start(adxb);
}

// Nonmatching: 99.07%. Every call and every branch matches. The residue is
// sji: the target loads it into r4 in the prologue and keeps it there, while
// mwcc sinks the load to the first use and passes it in r0 directly, saving
// the copy. Six declaration orders and an inlined-sji variant were measured;
// none changed it.
//
// Decode-done bookkeeping, entered from adxsjd_decode_exec once ADXB_ExecHndl leaves
// adxb->stat == 3. Upstream this is adxsjd_decexec_end, but NITRO drops the
// dtrpsmpl/dtrpfunc tail: after accumulating the five counters it goes straight
// to ADXB_Reset, and the per-channel callback is a second pair at 0x58/0x5C
// rather than the dfltfunc at 0x50/0x54 that adxsjd_decode_prep uses.
void adxsjd_decexec_end(ADXSJD* sjd) {
    ADXB adxb = sjd->adxb;
    SJ   sji  = sjd->sji;
    SJCK ck;
    SJCK ck2;
    int  total_nsmpl;
    int  dlen;
    int  ndecsmpl;
    int  i;

    total_nsmpl = ADXB_GetTotalNumSmpl(adxb);
    dlen        = ADXB_GetDecDtLen(adxb);
    ndecsmpl    = ADXB_GetDecNumSmpl(adxb);

    if (ADXB_GetFormat(adxb) != 1 || func_02015aa4(sjd) != 1) {
        if (ndecsmpl >= total_nsmpl - sjd->decpos) {
            ndecsmpl = total_nsmpl - sjd->decpos;
        }
    }

    func_0201a670(&sjd->cki, dlen, &ck, &ck2);
    SJ_PutChunk(sji, 0, &ck);
    SJ_UngetChunk(sji, 1, &ck2);

    for (i = 0; i < ADXB_GetNumChan(sjd->adxb); i++) {
        func_0201a670(&sjd->cko[i], ndecsmpl * 2, &ck, &ck2);

        if (sjd->unk_58 != NULL) {
            sjd->unk_58(sjd->unk_5C, i, ck.data, ck.length);
        }

        SJ_PutChunk(sjd->sjo[i], 1, &ck);
        SJ_UngetChunk(sjd->sjo[i], 0, &ck2);
    }

    sjd->total_decsmpl += ndecsmpl;
    sjd->total_decdtlen += dlen;
    sjd->decpos += ndecsmpl;
    sjd->dtrpcnt += ndecsmpl;
    sjd->dtrpdtlen += dlen;

    ADXB_Reset(adxb);
}

void func_020154f0(ADXSJD* sjd) {
    ADXB adxb        = sjd->adxb;
    int  total_nsmpl = ADXB_GetTotalNumSmpl(sjd->adxb);
    int  dlen        = ADXB_GetDecDtLen(sjd->adxb);
    int  ndecsmpl    = ADXB_GetDecNumSmpl(sjd->adxb);

    total_nsmpl -= sjd->decpos;

    if (ndecsmpl >= total_nsmpl) {
        ndecsmpl = total_nsmpl;
    }
    sjd->total_decsmpl += ndecsmpl;
    sjd->total_decdtlen += dlen;
    sjd->decpos += ndecsmpl;
}

// The state-2 arm of ADXSJD_ExecHndl. The target tests adxb->stat twice
// around the decode: once to decide whether to run it, and once afterwards to
// decide whether to finish up. The five-value test on adxb->format is written
// as an || chain because mwcc folds only the last adjacent constant pair into
// a range check.
void adxsjd_decode_exec(ADXSJD* sjd) {
    ADXB adxb = sjd->adxb;

    if (ADXB_GetStat(adxb) == 0) {
        adxsjd_decexec_start(sjd);
    }

    ADXB_ExecHndl(adxb);

    if (ADXB_GetStat(adxb) == 3) {
        adxsjd_decexec_end(sjd);
    }

    if (adxb->format == 10 || adxb->format == 20 || adxb->format == 11 || adxb->format == 12 || adxb->format == 15) {
        func_020154f0(sjd);
    }
}

void func_020155c0(ADXSJD* sjd) {
    if (0 < sjd->unk_A8) {
        ADXCRS_Lock();
        func_0201562c(sjd);
        ADXCRS_Unlock();
    }
    if (sjd->state == 2) {
        adxsjd_decode_exec(sjd);
    } else if (sjd->state == 1) {
        adxsjd_decode_prep(sjd);
    }
    if (sjd->unk_AC > 0) {
        ADXCRS_Lock();
        func_0201575c(sjd);
        ADXCRS_Unlock();
    }
}

// Nonmatching: 99.14%. One register swap: the target gives the loop counter
// r8 and the byte count r9, mwcc hands them out the other way round. Six
// declaration orders were measured and none of them changes it.
//
// Supply-side, entered from ADXSJD_ExecHndl under ADXCRS_Lock when
// sjd->unk_A8 > 0. It pads every output channel with silence: work out how
// many bytes are actually left, round that down to a whole sample, then push
// that many zero bytes onto each channel and advance unk_A8 past them.
//
// NITRO-only -- neither PS2 reference has a counterpart, so this is read
// straight off the target.
void func_0201562c(ADXSJD* sjd) {
    SJCK ck;
    int  nbyte;
    int  nsmpl;
    int  i;

    nbyte = sjd->unk_A8 * 2;

    // Clamp against the shortest output channel.
    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 0, 0x7FFFFFFF, &ck);

        if (nbyte >= ck.length) {
            nbyte = ck.length;
        }

        SJ_UngetChunk(sjd->sjo[i], 0, &ck);
    }

    // Whole samples only, so drop any odd trailing byte.
    nsmpl = nbyte / 2;
    nbyte = nsmpl * 2;

    if (nbyte <= 0) {
        return;
    }

    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 0, nbyte, &ck);
        memset(ck.data, 0, nbyte);
        SJ_PutChunk(sjd->sjo[i], 1, &ck);
    }

    sjd->unk_A8 -= nsmpl;
}

// Nonmatching: 99.23%. The same single register swap as func_0201562c -- the
// target gives the loop counter r9 and the byte count r10, mwcc reverses it.
//
// The second supply-side arm, entered from ADXSJD_ExecHndl under ADXCRS_Lock
// when sjd->unk_AC > 0. Same shape as func_0201562c and on the same two
// counters, but it works on stream id 1 and moves the data to id 0 rather than
// zero-filling it -- so where func_0201562c pads with fresh silence, this one
// shifts silence that is already buffered on the other channel.
//
// NITRO-only -- neither PS2 reference has a counterpart, so this is read
// straight off the target.
void func_0201575c(ADXSJD* sjd) {
    SJCK ck;
    int  nbyte;
    int  nsmpl;
    int  i;

    nbyte = sjd->unk_AC * 2;

    // Clamp against the shortest output channel.
    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 1, 0x7FFFFFFF, &ck);

        if (nbyte >= ck.length) {
            nbyte = ck.length;
        }

        SJ_UngetChunk(sjd->sjo[i], 1, &ck);
    }

    // Whole samples only, so drop any odd trailing byte.
    nsmpl = nbyte / 2;
    nbyte = nsmpl * 2;

    if (nbyte <= 0) {
        return;
    }

    for (i = 0; i < sjd->maxnch; i++) {
        SJ_GetChunk(sjd->sjo[i], 1, nbyte, &ck);
        SJ_PutChunk(sjd->sjo[i], 0, &ck);
    }

    sjd->unk_AC -= nsmpl;
}

// ADXSJD_ExecServer. The per-object loop always runs; each of the two
// callbacks is skipped when its slot is null. Recvx's ADXT_ExecServer calls
// this from inside its own stage machine, and NITRO inlines that whole chain
// into adxt_ExecServer.
void func_02015878(void) {
    int i;

    if (data_0206c0b4.unk08 != 0) {
        data_0206c0b4.unk08(data_0206c0b4.unk04);
    }

    for (i = 0; i < ADXSJD_MAX_OBJ; i++) {
        if (adxsjd_obj[i].used == 1) {
            func_020155c0(&adxsjd_obj[i]);
        }
    }

    if (data_0206c0b4.unk00 != 0) {
        data_0206c0b4.unk00(data_0206c0b4.unk10);
    }
}

int func_020158e4(ADXSJD* sjd) {
    return sjd->total_decdtlen;
}

int func_020158ec(ADXSJD* sjd) {
    return sjd->total_decsmpl;
}

void func_020158f4(ADXSJD* sjd, int param_2) {
    sjd->decpos = param_2;
}

void ADXSJD_SetLnkSw(ADXSJD* sjd, int param_2) {
    sjd->unk_A4 = param_2;
}

void func_02015904(ADXSJD* sjd, int param_2, int param_3) {
    sjd->dtrpfunc = param_2;
    sjd->dtrpobj  = param_3;
}

void func_02015910(ADXSJD* sjd, int param_2) {
    sjd->dtrpsmpl = param_2;
}

void func_02015918(ADXSJD* sjd, int param_2) {
    sjd->dtrpcnt = param_2;
}

void func_02015920(ADXSJD* sjd, int param_2) {
    sjd->dtrpdtlen = param_2;
}

int func_02015928(ADXSJD* sjd) {
    return ADXB_GetFormat(sjd->adxb);
}

int ADXSJD_GetSfreq(ADXSJD* sjd) {
    return ADXB_GetSfreq(sjd->adxb);
}

int func_02015948(ADXSJD* sjd) {
    return ADXB_GetNumChan(sjd->adxb);
}

int ADXSJD_GetOutBps(ADXSJD* sjd) {
    return ADXB_GetOutBps(sjd->adxb);
}

int func_02015968(ADXSJD* sjd) {
    return ADXB_GetBlkSmpl(sjd->adxb);
}

int ADXSJD_GetTotalNumSmpl(ADXSJD* sjd) {
    return ADXB_GetTotalNumSmpl(sjd->adxb);
}

int func_02015988(ADXSJD* sjd) {
    return ADXB_GetNumLoop(sjd->adxb);
}

int func_02015998(ADXSJD* sjd) {
    return func_020128b8(sjd->adxb);
}

int func_020159a8(ADXSJD* sjd) {
    if (sjd != NULL) {
        return func_020128c0(sjd->adxb);
    }
    return 0;
}

int func_020159c4(ADXSJD* sjd) {
    return func_020128d0(sjd->adxb);
}

int func_020159d4(ADXSJD* sjd) {
    return func_020128d8(sjd->adxb);
}

int ADXSJD_GetDefOutVol(ADXSJD* sjd) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((unsigned int)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefOutVol(sjd->adxb);
    }
    return 0;
}

int func_02015a2c(ADXSJD* sjd, int chan) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((unsigned int)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefPan(sjd->adxb, chan);
    }
    return -0x80;
}

int* func_02015a7c(ADXSJD* sjd) {
    return sjd->spsdinfo;
}

int func_02015a84(ADXSJD* sjd) {
    return func_020128fc(sjd->adxb);
}

int func_02015a94(ADXSJD* sjd) {
    return func_0201292c(sjd->adxb);
}

int func_02015aa4(ADXSJD* sjd) {
    return sjd->unk_B0;
}
