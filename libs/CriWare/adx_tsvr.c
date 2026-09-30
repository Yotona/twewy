#include <CriWare/adxt.h>

// adx_tsvr.c -- the ADXT (text-to-lips) state machine / server.
//
// delinks.txt gives this TU .text 0x02017f80-0x02018cdc, .data
// 0x02063edc-0x02063f74 and .bss 0x0206c7fc-0x0206c80c: a contiguous hole between
// adx_tlk2.c and adx_xpnd.c that no source file covered, so the linker filled it
// with a generated _dsd_gap object. 14 functions live there, ending with
// adxt_ExecHndl (0x02018c5c) -- declared in adx_tlk.c but defined nowhere until
// now, which is why adxt_ExecServer() there is still `/* NYI */`.
//
// Function names are deliberately address-based. objdiff pairs target and base
// symbols BY NAME, so giving these their upstream CriWare names would drop all
// 14 out of the report even if the code were byte-identical. The upstream names
// (from the PS2 reference decomp) are noted per function.
//
// Upstream order is: trap_entry_lps, trap_entry, eos_entry, set_outpan,
// nlp_trap_entry, stat_decinfo, stat_prep, stat_playing, stat_decend,
// stat_playend, RcvrReplay, ExecErrChk, ExecRdErrChk, ExecRdCompChk, ExecHndl,
// GetStatRead (16). This build has 14, and ADXT_ExecHndl's dispatch chain pins
// the tail exactly: stat 1 -> func_0201854c, 2 -> func_02018910,
// 3 -> func_02018a64, 4 -> func_02018b30, 5 -> func_02018b74, then
// func_02018bd4 and func_02018b78 unconditionally.

void ADXERR_CallErrFunc1(const char*);
void func_02012f88(); // ADXCRS_Lock
void func_02012f8c(); // ADXCRS_Unlock

// SJCK_LEN_MAX is the "read to the very end" sentinel; the target builds it
// with `mvn r1, #0x80000000`.
#define SJCK_LEN_MAX 0x7FFFFFFF

// callees living in sibling TUs (address-named in the original)
// adx_sjd.c -- no public header declares these
s32  func_020158e4(ADXSJD* sjd); // GetDecDtLen
void func_020158f4(ADXSJD* sjd, s32 pos);
// ADXSJD_EntryTrapFunc. Takes a real callback plus a user pointer, which is
// why unk_48/unk_4C are typed as pointers rather than u32.
void func_02015904(ADXSJD* sjd, void (*trap_func)(ADXT), void* trap_obj);
void func_02015910(ADXSJD* sjd, s32 samples);
void func_02015918(ADXSJD* sjd, s32 cnt);
void func_02015920(ADXSJD* sjd, s32 dt_len);
s32  func_02015948(ADXSJD* sjd); // GetNumChan
s32  func_02015968(ADXSJD* sjd); // GetBlkSmpl
s32  func_02015998(ADXSJD* sjd); // GetCof
s32  func_020159a8(ADXSJD* sjd); // GetAinfLen
s32  func_020159c4(ADXSJD* sjd);
s32* func_02015a7c(ADXSJD* sjd);
s32  func_02015a84(ADXSJD* sjd);
s32  func_02014b50(ADXSJD* sjd); // ADXSJD_TermSupply
void func_02014b3c(ADXSJD* sjd);
void func_020155c0(ADXSJD* sjd);
s32  func_02015928(ADXSJD* sjd); // GetFormat
void ADXSJD_Start(ADXSJD* sjd);
void ADXSJD_Stop(ADXSJD* sjd);
void ADXSJD_SetMaxDecSmpl(ADXSJD* sjd, s32 n);
s8   ADXSJD_GetStat(ADXSJD* sjd);
s32  func_02015a94(ADXSJD* sjd);
// adx_stmc.c
s32  func_02015f5c(ADXSTM* stm); // ADXSTM_GetStat
void ADXSTM_Seek(ADXSTM* stm, int pos);
void ADXSTM_SetEos(ADXSTM* stm, int eos);
// lsc.c -- 4-byte stub
s32 func_020215ec(void* lsc);
// adx_rna.c forwarder
s32  func_0201bf0c(void* rna); // ADXRNA_GetNumData
s32  func_0201bf18(void* rna); // ADXRNA_GetNumRoom
void ADXRNA_SetPlaySw(void* rna, s32 sw);
void ADXRNA_SetTransSw(void* rna, s32 sw);
// ADXRNA forwarders (adx_rna.c has no source file either)
void func_0201bf30(void* rna, s32 n);
void func_0201bf3c(void* rna, s32 n);
void func_0201bf60(void* rna, s32 n);
void func_0201bf6c(void* rna, s32 n);
void func_0201bf70(void* rna, s32* p);
// adx_dcd.c
s32 func_020130dc(s8* data, s32 len, s16* ofst);
s32 func_02013868(s8* data, s32 len, s16* ofst);
// in the sj_utl / LSC-amp gaps, which have no source file yet
// SJ_SplitChunk: four arguments -- (chunk, nbyte, out1, out2), and the
// first and third are normally the *same* chunk, i.e. it splits in place.
// adx_stmc.c already calls it this way; the declaration and the two call
// sites here were wrong, which put the offset in the chunk-pointer slot.
void func_0201a670(SJCK* ck, s32 nbyte, SJCK* ck1, SJCK* ck2);
s32  func_02021534(s8* data, s32 len, s16* ofst);
// adx_tlk.c
void adxt_start_stm(ADXT adxt, const char* filename, void* dir, s32 ofst, s32 range);
void func_02017b20(ADXT adxt, s32 a, s32 b);
void func_02017b4c(ADXT adxt, s32* a, s32* b);
// adx_errs.c
void ADXERR_CallErrFunc2(s8* msg, s8* arg);
void ADXERR_ItoA2(s32 v, s32 base, s8* str, s32 width);
// lsc.c
s32 func_020215bc(void* amp, s32 sfreq);
s32 _s32_div_f(s32 a, s32 b);

extern s32 volatile adxt_vsync_cnt; // adx_inis.c
void memset(void*, int, int);

#define ADXSJD_STAT_PLAYING 3
#define ADXSJD_STAT_DECINFO 2

// sj.h has no macro for GetNumData (vtable slot 0x24).
#define SJ_GetNumData(sj, id) (*(sj)->vtable->GetNumData)(sj, id)

void func_02018004(ADXT adxt);

// .bss 0x0206c7fc-0x0206c80c (four words; delinks.txt). Wrapped in a struct
// because mwcc does not honour declaration order for same-sized file statics:
// as loose globals it emitted 0x800, 0x7fc, 0x808, 0x804 regardless of how
// they were declared. 0x0206c7fc is a callback the host installs;
// 0x0206c804 / 0x0206c808 are the reference's adxt_dbg_ndt / adxt_dbg_nch.
struct {
    s32 f_7fc; // callback: (ADXT, sfreq, num_chan, total_smpl)
    s32 f_800;
    s32 f_804; // adxt_dbg_ndt
    s32 f_808; // adxt_dbg_nch
} data_0206c7fc = {0, 0, 0, 0};

#define data_0206c800 data_0206c7fc.f_800
#define data_0206c804 data_0206c7fc.f_804
#define data_0206c808 data_0206c7fc.f_808

#define ADXT_CB_FUNC ((void (*)(ADXT adxt, s32 sfreq, s32 num_chan, s32 total_smpl))data_0206c7fc.f_7fc)

// .data 0x02063edc-0x02063f74. The three ADXERR_CallErrFunc strings live in
// .data rather than .rodata; the target's literal pools point here. The 0xedc
// and 0xf48 wordings are unverified -- only their addresses are known.
char data_02063edc[] = "E9081102 adxt trap: can't get loop skip data";
char data_02063f08[] = "E9081001 adxt_stat_decinfo: can't play this number of channels";
char data_02063f48[] = "E02080842 ADXT_ExecHndl: parameter error";

void adxt_ExecHndl(ADXT adxt);
void func_0201854c(ADXT adxt);
void func_02018910(ADXT adxt);
void func_02018a64(ADXT adxt);
void func_02018b30(ADXT adxt);
void func_02018b74(ADXT adxt);
void func_02018b78(ADXT adxt);
void func_02018bd4(ADXT adxt);

// adxt_trap_entry: install func_02018004 as the SJD trap callback, with the trap
// positioned at (num_smpl - cof) and the trap span set to the AIFF header
// length. Reached from ADXSJD_EntryTrapFunc, so the third argument is the
// ADXT the callback will be handed back.
void func_02017f80(ADXT adxt) {
    ADXSJD* sjd      = adxt->sjd;
    s32     cof      = func_02015998(sjd);
    s32     ainf_len = func_020159a8(sjd);
    s32     num_smpl = func_020159c4(sjd);
    s32     trp;

    func_02015a84(sjd);
    func_02015918(sjd, 0);

    trp            = num_smpl - cof;
    adxt->trpnsmpl = trp;

    func_02015910(sjd, trp);
    func_02015920(sjd, ainf_len);
    func_020158f4(sjd, cof);
    func_02015904(sjd, func_02018004, adxt);
}

// The trap callback func_02017f80 registers. Re-arms the trap once the skip
// region has been consumed, and for in-memory playback re-primes the input
// stream. ADXSJD_STAT_PLAYING is 3.
void func_02018004(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    SJ      sji = adxt->sji;
    s32     cof;
    s32     ainf_len;
    s32     num_smpl;
    s32     trp;
    SJCK    ck;

    cof      = func_02015998(sjd);
    ainf_len = func_020159a8(sjd);
    num_smpl = func_020159c4(sjd);

    // pmode 2 (mem) or 3 (stream) with looping off: nothing left to trap.
    if ((u8)(s8)(adxt->pmode - ADXT_PLAYBACK_MEM) <= 1 && adxt->lpflg == 0) {
        // Target reloads adxt->sjd here rather than reusing the cached pointer.
        func_02015910(adxt->sjd, -1);
        return;
    }

    SJ_GetChunk(sji, 1, adxt->lp_skiplen, &ck);

    if (ck.length < adxt->lp_skiplen) {
        // String lives in .data at 0x02063edc; wording unverified.
        ADXERR_CallErrFunc1("E9081102 ADXT trap: can't get loop skip data");
    }

    SJ_PutChunk(sji, 0, &ck);

    func_02015918(sjd, 0);

    trp            = num_smpl - cof;
    adxt->trpnsmpl = trp;

    func_02015910(sjd, trp);
    func_02015920(sjd, ainf_len);
    func_020158f4(sjd, cof);

    // The pmode test guards only the stream re-prime: the target's `bne` lands
    // past it but still runs func_02015a94 and the lpcnt bump.
    if (adxt->pmode == ADXT_PLAYBACK_MEM) {
        SJ_Reset(sji);
        SJ_GetChunk(sji, 1, ainf_len, &ck);
        SJ_PutChunk(sji, 0, &ck);
    }

    func_02015a94(sjd);
    adxt->lpcnt++;
}

// Sets the end-of-stream marker, or seeks past the AIFF header when looping.
// Note the two sub-cases are mutually exclusive in the target: the lpflg == 0
// arm always reaches ADXSTM_SetEos, the lpflg != 0 arm never calls it.
void func_02018168(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    s32     ainf_len;

    if (sjd == NULL || adxt->used == 0) {
        return;
    }

    ainf_len = func_020159a8(sjd);

    if (adxt->pmode == ADXT_PLAYBACK_SLFILE) {
        ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
        return;
    }

    if (adxt->lpflg == 0) {
        if (func_020158e4(sjd) >= (s32)adxt->loopDecodeLength) {
            func_02015910(adxt->sjd, -1);
        }

        ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
    } else {
        ADXSTM_Seek((ADXSTM*)sjd, ainf_len / 2048);
    }
}

// Mono sources have no meaningful channel-1 pan, so only channel 0 is applied.
void func_020181f4(ADXT adxt) {
    s32 num_chan = func_02015948(adxt->sjd);

    if (num_chan == 1) {
        ADXT_SetOutPan(adxt, 0, adxt->outpan[0]);
    } else {
        ADXT_SetOutPan(adxt, 0, adxt->outpan[0]);
        ADXT_SetOutPan(adxt, 1, adxt->outpan[1]);
    }
}

// Splits the two input chunks at the offsets the header scan reported, then
// restarts the decoder. Reached as the non-looping arm of the trap callback
// pair (func_02017f80 / func_02018238) that func_0201854c installs.
void func_02018238(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    SJ      sji = adxt->sji;
    SJCK    ckA;
    SJCK    ckB;
    SJCK    ckC;
    SJCK    ckD;
    s16     ofst[2];
    s16     ofst0;
    s16     ofst1;
    s32     zero;
    s32     skip;
    s32     ret0;
    s32     ret1;

    if (adxt->playbackFlag == 0) {
        return;
    }

    ofst[0] = 0;
    ofst[1] = 0;
    zero    = 0;
    skip    = zero;

    func_02012f88(); // ADXCRS_Lock

    SJ_GetChunk(sji, 1, SJCK_LEN_MAX, &ckA);
    SJ_GetChunk(sji, 1, SJCK_LEN_MAX, &ckC);

    // A non-zero here means the header scan already consumed the link, so
    // unwind and bail. (Opposite polarity to the two decoders below.)
    if (func_02015928(sjd) == 0 && func_02013868(ckA.data, ckA.length, &ofst[1]) != 0) {
        ADXT_SetLnkSw(adxt, 0);
        SJ_UngetChunk(sji, 1, &ckC);
        SJ_UngetChunk(sji, 1, &ckA);
        func_02012f8c(); // ADXCRS_Unlock
        return;
    }

    skip += ofst[1];

    if (func_02015928(sjd) == 1) {
        ret0 = func_02021534(ckA.data + skip, ckA.length - skip, &ofst[1]);
        if (ret0 != 0) {
            ret1 = func_02021534(ckC.data, ckC.length, &ofst[0]);
        }
    } else {
        ret0 = func_020130dc(ckA.data + skip, ckA.length - skip, &ofst[1]);
        if (ret0 != 0) {
            ret1 = func_020130dc(ckC.data, ckC.length, &ofst[0]);
        }
    }

    // The target reads both offsets into registers right after the decoder
    // results, before the bail-out test, so they stay live across it -- that is
    // also what forces the extra pair of callee-saved registers in its frame.
    ofst1 = ofst[1];
    ofst0 = ofst[0];

    // Both decoders reporting non-zero means the split did not take; the target
    // flattens this into one `cmp`/`cmpne` pair ahead of a single error block.
    if (ret0 != 0 && ret1 != 0) {
        SJ_UngetChunk(sji, 1, &ckC);
        SJ_UngetChunk(sji, 1, &ckA);
        ADXT_SetLnkSw(adxt, 0);
        func_02012f8c();
        return;
    }

    if (ret0 == 0) {
        SJ_UngetChunk(sji, 1, &ckC);
        func_0201a670(&ckA, skip + ofst1, &ckA, &ckB);
        SJ_PutChunk(sji, 0, &ckA);
        SJ_UngetChunk(sji, 1, &ckB);
    } else {
        SJ_PutChunk(sji, 0, &ckA);
        func_0201a670(&ckC, zero + ofst0, &ckC, &ckD);
        SJ_PutChunk(sji, 0, &ckC);
        SJ_UngetChunk(sji, 1, &ckD);
    }

    func_02012f8c(); // ADXCRS_Unlock

    adxt->decofst += func_020158ec(sjd);
    ADXSJD_Stop(sjd);
    ADXSJD_Start(sjd);
    func_020155c0(sjd);

    if (ADXSJD_GetStat(sjd) != ADXSJD_STAT_DECINFO) {
        ADXT_SetLnkSw(adxt, 0);
        return;
    }

    // func_02014b3c is called with two arguments here but with one in
    // func_0201854c, so the real prototype is not used for either.
    ((void (*)(ADXSJD*, s32))func_02014b3c)(sjd, adxt->maxdecsmpl);
    func_02015910(sjd, ADXSJD_GetTotalNumSmpl(sjd));
    func_02015920(sjd, 0);
    func_02015918(sjd, 0);
}

// adxt_stat_decinfo
void func_0201854c(ADXT adxt) {
    ADXSJD* sjd = adxt->sjd;
    s32     num_chan;
    s32     sfreq;
    s32     num_loop;
    s32     blk_smpl;
    s32     lp_end_ofst;
    s32     stat;
    s32     a;
    s32     b;
    s8      num_chan_str[32];

    a = 0;
    b = 0;

    if ((u8)adxt->pmode <= 1 && adxt->streamStartFlag == 1) {
        if (func_02015f5c(adxt->stm) == 2) {
            return;
        }

        if (adxt->sjf != NULL) {
            SJ_Reset(adxt->sjf);
        }

        adxt_start_stm(adxt, adxt->filename, adxt->directory, adxt->offset, adxt->range);
        adxt->streamStartFlag = 0;
    }

    stat = ADXSJD_GetStat(sjd);

    // Written as if/else rather than an early return: the target branches over
    // the whole body to a trailing `stat == 4` arm, which only happens with this
    // block shape.
    if (stat == ADXSJD_STAT_DECINFO) {
        num_chan = func_02015948(sjd);

        if (num_chan > adxt->maxnch) {
            ADXERR_ItoA2(num_chan, adxt->maxnch, num_chan_str, 16);
            ADXERR_CallErrFunc2(data_02063f08, num_chan_str);
            ADXT_Stop(adxt);
            return;
        }

        sfreq    = ADXSJD_GetSfreq(sjd);
        num_loop = func_02015988(sjd);

        if (func_02015928(sjd) == 10) {
            adxt->maxdecsmpl = _s32_div_f(sfreq, adxt->svrfreq);
            blk_smpl         = func_02015968(sjd);
        } else if (num_loop <= 0) {
            adxt->maxdecsmpl = (_s32_div_f(sfreq, adxt->svrfreq) * 3) / 2;
            blk_smpl         = func_02015968(sjd);
        } else {
            adxt->maxdecsmpl = _s32_div_f(sfreq, adxt->svrfreq) * 3;
            blk_smpl         = func_02015968(sjd);
        }

        blk_smpl <<= 1;
        adxt->maxdecsmpl = _s32_div_f(adxt->maxdecsmpl + blk_smpl, blk_smpl) * blk_smpl;

        func_02014b3c(sjd);

        if (num_loop > 0) {
            if (adxt->pmode == ADXT_PLAYBACK_MEM) {
                adxt->lp_skiplen = 0;
            } else {
                // div is the constant 2048; the target inlines both divisions
                // rather than calling _s32_div_f.
                lp_end_ofst      = func_020159d4(sjd);
                adxt->lp_skiplen = (2048 - (lp_end_ofst / 2048)) / 2048;
                adxt->lesct      = (lp_end_ofst + 2047) / 2048;
                ADXSTM_SetEos(adxt->stm, adxt->lesct);
                ADXSTM_EntryEosFunc(adxt->stm, (s32)func_02018168, (s32)adxt);
            }

            adxt->trpnsmpl = func_02015998(sjd);
            func_02015910(sjd, adxt->trpnsmpl);
            func_02015920(sjd, 0);
            func_02015918(sjd, 0);
            func_02015904(sjd, func_02017f80, adxt);
        } else {
            if (adxt->stm != NULL) {
                ADXSTM_SetEos(adxt->stm, SJCK_LEN_MAX);
            }

            func_02015910(sjd, ADXSJD_GetTotalNumSmpl(sjd));
            func_02015920(sjd, 0);
            func_02015918(sjd, 0);
            func_02015904(sjd, func_02018238, adxt);
        }

        sfreq    = ADXSJD_GetSfreq(sjd);
        num_chan = func_02015948(sjd);
        num_loop = ADXSJD_GetTotalNumSmpl(sjd);

        func_0201bf60(adxt->rna, ADXSJD_GetOutBps(sjd));
        func_0201bf3c(adxt->rna, sfreq);
        func_0201bf30(adxt->rna, num_chan);
        func_0201bf6c(adxt->rna, num_loop);

        ADXT_SetOutVol(adxt, adxt->outvol);

        func_02017b4c(adxt, &a, &b);

        if (a != 0 || b != 0) {
            func_02017b20(adxt, a, b);
        }

        func_020181f4(adxt);

        if (adxt->amp != NULL) {
            func_020215bc(adxt->amp, sfreq);
        }

        // Vestigial in this build: the format probe survives but both arms of the
        // test fall through to the same code, so the target keeps the compare.
        if (func_02015928(sjd) != 2) {
        }

        func_0201bf70(adxt->rna, func_02015a7c(sjd));
        ADXRNA_SetTransSw(adxt->rna, 1);

        if (data_0206c7fc.f_7fc != 0) {
            ADXT_CB_FUNC(adxt, sfreq, num_chan, num_loop);
        }

        adxt->stat = ADXT_STAT_PREPPING;
    } else if (stat == 4) {
        adxt->stat = ADXT_STAT_ERROR;
    }
}

// adxt_stat_prep
void func_02018910(ADXT adxt) {
    void*   rna = adxt->rna;
    ADXSJD* sjd = adxt->sjd;
    s32     num_data;
    s32     num_room;
    s32     num_chan;
    s32     limit;
    s32     size;
    s32     i;
    SJCK    ck;

    num_data = func_0201bf0c(rna);
    num_room = func_0201bf18(rna);

    // NITRO clamps the window to 0x800; the PS2 reference compares against
    // (maxdecsmpl << 1) with no clamp.
    limit = adxt->maxdecsmpl;
    if (limit >= 0x800) {
        limit = 0x800;
    }

    if (num_data >= limit || num_room <= func_02015968(sjd) || ADXSJD_GetStat(sjd) == ADXSJD_STAT_PLAYING) {
        if (adxt->waitFlag == 0) {
            if (adxt->pause_flag == 0) {
                ADXRNA_SetPlaySw(rna, 1);
                adxt->tvofst = 0;
                adxt->svcnt  = adxt_vsync_cnt;
            }

            adxt->stat = ADXT_STAT_PLAYING;
        }

        adxt->readyFlag = 1;
    }

    if (ADXSJD_GetStat(sjd) != ADXSJD_STAT_PLAYING) {
        return;
    }

    num_chan = ADXT_GetNumChan(adxt);
    size     = (adxt->maxdecsmpl * num_chan) << 1;

    for (i = 0; i < num_chan; i++) {
        SJ sj = adxt->sjo[i];

        SJ_GetChunk(sj, i, size, &ck);
        memset(ck.data, i, ck.length);
        SJ_PutChunk(sj, 1, &ck);
    }
}

// adxt_stat_playing
void func_02018a64(ADXT adxt) {
    s32 num_chan;
    s32 i;

    if (adxt->lpflg == 0 && adxt->loopDecodeLength != 0 && func_020158e4(adxt->sjd) >= (s32)adxt->loopDecodeLength) {
        func_02015910(adxt->sjd, -1);
    }

    if (ADXSJD_GetStat(adxt->sjd) != ADXSJD_STAT_PLAYING) {
        return;
    }

    num_chan      = func_02015948(adxt->sjd);
    data_0206c808 = num_chan;

    // Walk the output streams, bailing out as soon as one still holds a
    // sizeable backlog -- the decode side has not caught up yet.
    for (i = 0; i < num_chan; i++) {
        s32 nbyte = SJ_GetNumData(adxt->sjo[i], 1);

        data_0206c804 = nbyte;

        if (nbyte >= 0x40) {
            break;
        }
    }

    if (i != num_chan) {
        return;
    }

    ADXRNA_SetTransSw(adxt->rna, 0);
    adxt->stat = ADXT_STAT_DECEND;
}

// adxt_stat_decend
void func_02018b30(ADXT adxt) {
    data_0206c800 = func_0201bf0c(adxt->rna);

    if (func_0201bf0c(adxt->rna) > 0) {
        return;
    }

    ADXRNA_SetPlaySw(adxt->rna, 0);
    adxt->stat = ADXT_STAT_PLAYEND;
}

// adxt_stat_playend -- "do nothing" in the reference too; this one matches.
void func_02018b74(ADXT adxt) {}

// adxt_RcvrReplay / ADXT_ExecRdErrChk
void func_02018b78(ADXT adxt) {
    if (adxt->stm != NULL && func_02015f5c(adxt->stm) == 4) {
        adxt->ercode = -1;
        adxt->stat   = ADXT_STAT_ERROR;
    }

    if (adxt->lsc != NULL && func_020215ec(adxt->lsc) == 3) {
        adxt->ercode = -1;
        adxt->stat   = ADXT_STAT_ERROR;
    }
}

void func_02018bd4(ADXT adxt) {
    if (adxt->stm == NULL || ADXT_GetStat(adxt) == 0) {
        return;
    }

    // The target lowers this to an `addls pc, pc, pmode, lsl #2` jump table
    // over 0..4, with cases 0 and 1 sharing one block and case 2 another.
    switch (adxt->pmode) {
        case ADXT_PLAYBACK_FILENAME:
        case ADXT_PLAYBACK_AFS:
            if (func_02015f5c(adxt->stm) == 3) {
                func_02014b50(adxt->sjd);
            }
            break;
        case ADXT_PLAYBACK_MEM:
            func_02014b50(adxt->sjd);
            break;
        case ADXT_PLAYBACK_STREAM:
        case ADXT_PLAYBACK_SLFILE:
        default:
            break;
    }
}

void func_02018c40(ADXT adxt) {
    func_02012f88(); // ADXCRS_Lock
    adxt_ExecHndl(adxt);
    func_02012f8c(); // ADXCRS_Unlock
}

void adxt_ExecHndl(ADXT adxt) {
    s8 stat;

    if (adxt == NULL) {
        ADXERR_CallErrFunc1("E02080842 ADXT_ExecHndl: parameter error");
        return;
    }

    stat = adxt->stat;

    if (stat == ADXT_STAT_PLAYING) {
        func_02018a64(adxt);
    } else if (stat == ADXT_STAT_LOADING) {
        func_0201854c(adxt);
    } else if (stat == ADXT_STAT_PREPPING) {
        func_02018910(adxt);
    } else if (stat == ADXT_STAT_DECEND) {
        func_02018b30(adxt);
    } else if (stat == ADXT_STAT_PLAYEND) {
        func_02018b74(adxt);
    }

    func_02018bd4(adxt);
    func_02018b78(adxt);
}