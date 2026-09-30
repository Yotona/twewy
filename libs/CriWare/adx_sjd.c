#include <CriWare/adx_sjd.h>

// adx_sjd.c owns bss 0x0206c0b4-0x0206c398 (0x2E4 bytes) per delinks.txt.
// ADXSJD_Init/Finish load the address 0x0206c0b4 and reach the refcount at
// [r0 + 0xc] == 0x0206c0c0, so the refcount is a FIELD of one 0x14-byte object
// anchored at 0x0206c0b4 rather than a standalone global -- symbols.txt has no
// separate symbol at 0x0206c0c0, and a group of standalone `s32`s gets pruned by
// mwcc instead of holding the address. The object array follows at 0x0206c0c8,
// and Init/Finish clear 0x2D0 bytes (45x `stmia lr!`), which is exactly
// ADXSJD_MAX_OBJ * sizeof(ADXSJD) == 4 * 0xB4.
typedef struct {
    /* 0x00 */ s32          unk00;
    /* 0x04 */ s32          unk04;
    /* 0x08 */ s32          unk08;
    /* 0x0C */ volatile s32 init_cnt;
} ADXSJD_CTRL; // Size: 0x14

ADXSJD_CTRL data_0206c0b4              = {0};
ADXSJD      adxsjd_obj[ADXSJD_MAX_OBJ] = {0};

void func_0201575c(ADXSJD* sjd);
void func_0201562c(ADXSJD* sjd);

s32 func_02015aa4(ADXSJD* sjd);

void* adxsjd_get_wr(ADXSJD sjd, s32* arg1, s32* arg2, s32* arg3);

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
    sjd->unk_A0 = 0;
    sjd->unk_2C = 0;
    sjd->unk_30 = 0;
    sjd->unk_34 = 0;
    sjd->unk_38 = 0x7FFFFFFF;
    sjd->unk_3C = -1;
    sjd->unk_40 = 0;
    sjd->unk_44 = 0;
    sjd->unk_03 = 0;
    sjd->unk_A8 = 0;
    sjd->unk_AC = 0;
}

ADXSJD* ADXSJD_Create(SJ sj, s32 maxChans, SJ* sjo) {
    ADXSJD* sjd;
    SJ      out;
    void*   buf_ptr;
    s32     i;
    s32     j;
    s32     buf_size;
    s32     xtr_size;

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
    sjd->unk_48 = 0;
    sjd->unk_4C = 0;
    sjd->unk_50 = 0;
    sjd->unk_54 = 0;
    sjd->used   = TRUE;
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

s8 ADXSJD_GetStat(ADXSJD* sjd) {
    return sjd->state;
}

void ADXSJD_SetInSj(ADXSJD* sjd, SJ sj) {
    sjd->sji = sj;
    ADXB_SetAhxInSj(sjd->adxb, sj);
}

void func_02014b3c() {}

void func_02014b50() {}

void ADXSJD_Start(ADXSJD* sjd) {
    ADXSJD_Clear(sjd);
    sjd->state = 1;
}

void ADXSJD_Stop(ADXSJD* sjd) {
    ADXB_Stop(sjd->adxb);
    sjd->state = 0;
}

void func_02014b94(ADXSJD* sjd) {}

void func_02014e74() {}

void func_02014f44() {}

void func_02015344() {}

void func_020154f0(ADXSJD* sjd) {
    ADXB adxb        = sjd->adxb;
    s32  total_nsmpl = ADXB_GetTotalNumSmpl(adxb);
    s32  dlen        = ADXB_GetDecDtLen(adxb);
    s32  ndecsmpl    = ADXB_GetDecNumSmpl(adxb);

    total_nsmpl -= sjd->unk_34;

    if (ndecsmpl >= total_nsmpl) {
        ndecsmpl = total_nsmpl;
    }
    sjd->unk_2C += ndecsmpl;
    sjd->unk_30 += dlen;
    sjd->unk_34 += ndecsmpl;
}

void func_02015554(ADXSJD* sjd) {}

void func_020155c0(ADXSJD* sjd) {
    if (0 < sjd->unk_A8) {
        ADXCRS_Lock();
        func_0201562c(sjd);
        ADXCRS_Unlock();
    }
    if (sjd->state == 2) {
        func_02015554(sjd);
    } else if (sjd->state == 1) {
        func_02014b94(sjd);
    }
    if (sjd->unk_AC > 0) {
        ADXCRS_Lock();
        func_0201575c(sjd);
        ADXCRS_Unlock();
    }
}

void func_0201562c(ADXSJD* sjd) {}

void func_0201575c(ADXSJD* sjd) {}

void func_02015878() {}

s32 func_020158e4(ADXSJD* sjd) {
    return sjd->unk_30;
}

s32 func_020158ec(ADXSJD* sjd) {
    return sjd->unk_2C;
}

void func_020158f4(ADXSJD* sjd, s32 param_2) {
    sjd->unk_34 = param_2;
}

void ADXSJD_SetLnkSw(ADXSJD* sjd, s32 param_2) {
    sjd->unk_A4 = param_2;
}

void func_02015904(ADXSJD* sjd, s32 param_2, s32 param_3) {
    sjd->unk_48 = param_2;
    sjd->unk_4C = param_3;
}

void func_02015910(ADXSJD* sjd, s32 param_2) {
    sjd->unk_3C = param_2;
}

void func_02015918(ADXSJD* sjd, s32 param_2) {
    sjd->unk_40 = param_2;
}

void func_02015920(ADXSJD* sjd, s32 param_2) {
    sjd->unk_44 = param_2;
}

s32 func_02015928(ADXSJD* sjd) {
    return ADXB_GetFormat(sjd->adxb);
}

s32 ADXSJD_GetSfreq(ADXSJD* sjd) {
    return ADXB_GetSfreq(sjd->adxb);
}

s32 func_02015948(ADXSJD* sjd) {
    return ADXB_GetNumChan(sjd->adxb);
}

s32 ADXSJD_GetOutBps(ADXSJD* sjd) {
    return ADXB_GetOutBps(sjd->adxb);
}

s32 func_02015968(ADXSJD* sjd) {
    return ADXB_GetBlkSmpl(sjd->adxb);
}

s32 ADXSJD_GetTotalNumSmpl(ADXSJD* sjd) {
    return ADXB_GetTotalNumSmpl(sjd->adxb);
}

s32 func_02015988(ADXSJD* sjd) {
    return ADXB_GetNumLoop(sjd->adxb);
}

s32 func_02015998(ADXSJD* sjd) {
    return func_020128b8(sjd->adxb);
}

s32 func_020159a8(ADXSJD* sjd) {
    if (sjd != NULL) {
        return func_020128c0(sjd->adxb);
    }
    return 0;
}

s32 func_020159c4(ADXSJD* sjd) {
    return func_020128d0(sjd->adxb);
}

s32 func_020159d4(ADXSJD* sjd) {
    return func_020128d8(sjd->adxb);
}

s32 ADXSJD_GetDefOutVol(ADXSJD* sjd) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((u32)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefOutVol(sjd->adxb);
    }
    return 0;
}

s32 func_02015a2c(ADXSJD* sjd, s32 chan) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((u32)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefPan(sjd->adxb, chan);
    }
    return -0x80;
}

s32* func_02015a7c(ADXSJD* sjd) {
    return &sjd->unk_60;
}

s32 func_02015a84(ADXSJD* sjd) {
    return func_020128fc(sjd->adxb);
}

s32 func_02015a94(ADXSJD* sjd) {
    return func_0201292c(sjd->adxb);
}

s32 func_02015aa4(ADXSJD* sjd) {
    return sjd->unk_B0;
}
