#include <cri/private/adx_dcd.h>
#include <cri/private/adx_xpnd.h>

#define ADXPD_MAX_OBJ 4

int       adxpd_internal_error     = 0;
ADXPD_OBJ adxpd_obj[ADXPD_MAX_OBJ] = {0};

void ADXPD_Init(void) {
    __builtin__clear(&adxpd_obj, sizeof(adxpd_obj));
}

ADXPD ADXPD_Create(void) {
    int i;

    for (i = 0; i < ADXPD_MAX_OBJ; i++) {
        ADXPD chk_adxpd = &adxpd_obj[i];
        if (chk_adxpd->used == 0) {
            break;
        }
    }

    if (i == ADXPD_MAX_OBJ) {
        return NULL;
    }

    ADXPD adxpd = &adxpd_obj[i];
    memset(adxpd, 0, sizeof(ADXPD_OBJ));
    adxpd->used = 1;
    adxpd->xno  = i;
    adxpd->mode = 0;
    adxpd->stat = 0;

    ADX_GetCoefficient(500, 44100, &adxpd->k[0], &adxpd->k[1]);

    __builtin__clear(adxpd->dly, sizeof(adxpd->dly));
    return adxpd;
}

void ADXPD_SetCoef(ADXPD_OBJ* adxpd, int sfreq, int cof) {
    ADX_GetCoefficient(cof, sfreq, &adxpd->k[0], &adxpd->k[1]);
}

void ADXPD_SetDly(ADXPD_OBJ* arg0, short* arg1, short* arg2) {
    arg0->dly[0][0] = arg1[0];
    arg0->dly[0][1] = arg2[0];
    arg0->dly[1][0] = arg1[1];
    arg0->dly[1][1] = arg2[1];
}

void ADXPD_GetDly(ADXPD_OBJ* adxpd, short* arg1, short* arg2) {
    arg1[0] = adxpd->dly[0][0];
    arg2[0] = adxpd->dly[0][1];
    arg1[1] = adxpd->dly[1][0];
    arg2[1] = adxpd->dly[1][1];
}

void ADXPD_SetExtPrm(ADXPD_OBJ* adxpd, short arg1, short arg2, short arg3) {
    adxpd->key     = arg1;
    adxpd->key_mul = arg2;
    adxpd->key_add = arg3;
}

void ADXPD_GetExtPrm(ADXPD_OBJ* adxpd, unsigned short* arg1, unsigned short* arg2, unsigned short* arg3) {
    *arg1 = adxpd->key;
    *arg2 = adxpd->key_mul;
    *arg3 = adxpd->key_add;
}

void ADXPD_Destroy(ADXPD_OBJ* adxpd) {
    if (adxpd != NULL) {
        adxpd->used = 0;
        memset(adxpd, 0, sizeof(ADXPD_OBJ));
    }
}

int ADXPD_GetStat(ADXPD adxpd) {
    return adxpd->stat;
}

int ADXPD_EntryMono(ADXPD adxpd, int arg1, int arg2, int arg3, int arg4) {
    if (adxpd->stat == 0) {
        adxpd->xprm.nch    = 1;
        adxpd->xprm.ibuf   = arg1;
        adxpd->xprm.nblk   = arg2;
        adxpd->xprm.obuf_l = arg3;
        adxpd->xprm.obuf_r = arg4;
        return 1;
    }

    return 0;
}

int ADXPD_EntrySte(ADXPD adxpd, int arg1, int arg2, int arg3, int arg4) {
    if (adxpd->stat == 0) {
        adxpd->xprm.nch    = 2;
        adxpd->xprm.ibuf   = arg1;
        adxpd->xprm.nblk   = arg2;
        adxpd->xprm.obuf_l = arg3;
        adxpd->xprm.obuf_r = arg4;
        return 1;
    }

    return 0;
}

void ADXPD_Start(ADXPD adxpd) {
    if (adxpd->stat == 0) {
        adxpd->ndecblk = 0;
        adxpd->stat    = 1;
    }
}

void ADXPD_Stop(ADXPD adxpd) {
    adxpd->stat = 0;
    __builtin__clear(adxpd->dly, sizeof(adxpd->dly));
}

void ADXPD_Reset(ADXPD adxpd) {
    if (adxpd->stat == 3) {
        adxpd->stat = 0;
    }
}

int ADXPD_GetNumBlk(ADXPD adxpd) {
    return adxpd->ndecblk;
}

void adxpd_error() {
    adxpd_internal_error = 1;
}

void ADXPD_ExecHndl(ADXPD adxpd) {
    if (adxpd->stat == 1) {
        adxpd->stat = 2;
    }

    if (adxpd->stat != 2) {
        return;
    }

    if (adxpd->xprm.nch == 1) {
        adxpd->ndecblk = ADX_DecodeMono4(adxpd->xprm.ibuf, adxpd->xprm.nblk, adxpd->xprm.obuf_l, adxpd->dly[0], adxpd->k[0],
                                         adxpd->k[1], &adxpd->key, adxpd->key_mul, adxpd->key_add);
    } else {
        adxpd->ndecblk =
            ADX_DecodeSte4(adxpd->xprm.ibuf, adxpd->xprm.nblk, adxpd->xprm.obuf_l, adxpd->dly[0], adxpd->xprm.obuf_r,
                           adxpd->dly[1], adxpd->k[0], adxpd->k[1], &adxpd->key, adxpd->key_mul, adxpd->key_add);

        if ((adxpd->ndecblk % 2) == 1) {
            adxpd_error();
        }
    }

    adxpd->stat = 3;
}