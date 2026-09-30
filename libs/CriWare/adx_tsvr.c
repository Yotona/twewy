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

// callees living in sibling TUs (address-named in the original)
s32  func_02014b50(void* sjd); // adx_sjd.c: ADXSJD_TermSupply
s32  func_02015f5c(void* stm); // adx_stmc.c: ADXSTM_GetStat
s32  func_020215ec(void* lsc); // lsc.c: 4-byte stub
s32  func_0201bf0c(void* rna); // adx_rna.c forwarder
void ADXRNA_SetPlaySw(void* rna, s32 sw);

// .bss 0x0206c7fc-0x0206c80c (four words; delinks.txt). func_02018b30 writes the
// second of them, so the word order below is load-bearing.
s32 data_0206c7fc = 0;
s32 data_0206c800 = 0;
s32 data_0206c804 = 0;
s32 data_0206c808 = 0;

void adxt_ExecHndl(ADXT adxt);
void func_0201854c(ADXT adxt);
void func_02018910(ADXT adxt);
void func_02018a64(ADXT adxt);
void func_02018b30(ADXT adxt);
void func_02018b74(ADXT adxt);
void func_02018b78(ADXT adxt);
void func_02018bd4(ADXT adxt);

// Nonmatching: stub (0.2% - 2.4%). Upstream: adxt_trap_entry_lps /
// adxt_trap_entry / adxt_eos_entry / adxt_set_outpan / adxt_nlp_trap_entry.
// Decoded target bodies are in docs/criware-reference-notes.md.
void func_02017f80(ADXT adxt) {}

// Nonmatching: stub (0.4%). Upstream position matches adxt_eos_entry.
void func_02018004(ADXT adxt) {}

// Nonmatching: stub (1.1%). Upstream: adxt_set_outpan.
void func_02018168(ADXT adxt) {}

// Nonmatching: stub (2.4%). Upstream: adxt_nlp_trap_entry.
void func_020181f4(ADXT adxt) {}

// Nonmatching: stub (0.2%). Not adxt_stat_decinfo; ADXT_ExecHndl routes stat 1
// to func_0201854c instead. Unidentified in the PS2 reference.
void func_02018238(ADXT adxt) {}

// Nonmatching: stub (0.2%). Upstream: adxt_stat_decinfo.
void func_0201854c(ADXT adxt) {}

// Nonmatching: stub (0.5%). Upstream: adxt_stat_prep.
void func_02018910(ADXT adxt) {}

// Nonmatching: stub (0.8%). Upstream: adxt_stat_playing.
void func_02018a64(ADXT adxt) {}

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
