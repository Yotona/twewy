#include <CriWare/adx_sjd.h>

BOOL    data_0206c0c0;
ADXSJD* data_0206c0c8;

ADXSJD adxsjd_obj[16] = {0};

void func_0201575c(ADXSJD* sjd);
void func_0201562c(ADXSJD* sjd);

s32 func_02015aa4(ADXSJD* sjd);

void* adxsjd_get_wr(ADXSJD sjd, s32* arg1, s32* arg2, s32* arg3);

void ADXSJD_Init(void) {}

void ADXSJD_Finish(void) {}

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

    for (i = 0; i < 4; i++) {
        if (adxsjd_obj[i].used == FALSE) {
            break;
        }
    }
    if (i == 4) {
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
        sjd->sjo[i] = sjo[i];
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

u32 func_020158e4(ADXSJD* sjd) {
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

void func_02015928() {}

void ADXSJD_GetSfreq() {}

void func_02015948() {}

s32 ADXSJD_GetOutBps(ADXSJD* sjd) {}

void func_02015968() {}

void ADXSJD_GetTotalNumSmpl() {}

void func_02015988() {}

void func_02015998() {}

s32 func_020159a8(ADXSJD* sjd) {
    if (sjd != NULL) {
        return func_020128c0(sjd->adxb);
    }
    return 0;
}

void func_020159c4() {}

void func_020159d4() {}

s32 ADXSJD_GetDefOutVol(ADXSJD* sjd) {
    if (ADXB_GetAinfLen(sjd->adxb) > 0 && ((u32)(((sjd->state - 2) << 0x18) >> 0x18) & 0xFF) <= 1) {
        return ADXB_GetDefOutVol(sjd->adxb);
    }
    return 0;
}

void func_02015a2c() {}

s32* func_02015a7c(ADXSJD* sjd) {
    return &sjd->unk_60;
}

void func_02015a84() {}

void func_02015a94() {}

s32 func_02015aa4(ADXSJD* sjd) {
    return sjd->unk_B0;
}
