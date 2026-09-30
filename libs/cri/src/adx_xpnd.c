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
    adxpd->unk4 = i;
    adxpd->mode = 0;
    adxpd->stat = 0;

    ADX_GetCoefficient(500, 44100, &adxpd->unk30, &adxpd->unk32);

    __builtin__clear(&adxpd->unk28, sizeof(ADXPD_OBJ_SUB));
    return adxpd;
}

void ADXPD_SetCoef(ADXPD_OBJ* adxpd, int arg1, short* arg2) {
    ADX_GetCoefficient(arg2, arg1, &adxpd->unk30, &adxpd->unk32);
}

void ADXPD_SetDly(ADXPD_OBJ* arg0, short* arg1, short* arg2) {
    arg0->unk28.unk0 = arg1[0];
    arg0->unk28.unk2 = arg2[0];
    arg0->unk28.unk4 = arg1[1];
    arg0->unk28.unk6 = arg2[1];
}

void ADXPD_GetDly(ADXPD_OBJ* adxpd, short* arg1, short* arg2) {
    arg1[0] = adxpd->unk28.unk0;
    arg2[0] = adxpd->unk28.unk2;
    arg1[1] = adxpd->unk28.unk4;
    arg2[1] = adxpd->unk28.unk6;
}

void ADXPD_SetExtPrm(ADXPD_OBJ* adxpd, short arg1, short arg2, short arg3) {
    adxpd->unk34 = arg1;
    adxpd->unk36 = arg2;
    adxpd->unk38 = arg3;
}

void ADXPD_GetExtPrm(ADXPD_OBJ* adxpd, unsigned short* arg1, unsigned short* arg2, unsigned short* arg3) {
    *arg1 = adxpd->unk34;
    *arg2 = adxpd->unk36;
    *arg3 = adxpd->unk38;
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
        adxpd->unk14 = 1;
        adxpd->unk18 = arg1;
        adxpd->unk1C = arg2;
        adxpd->unk20 = arg3;
        adxpd->unk24 = arg4;
        return 1;
    }

    return 0;
}

int ADXPD_EntrySte(ADXPD adxpd, int arg1, int arg2, int arg3, int arg4) {
    if (adxpd->stat == 0) {
        adxpd->unk14 = 2;
        adxpd->unk18 = arg1;
        adxpd->unk1C = arg2;
        adxpd->unk20 = arg3;
        adxpd->unk24 = arg4;
        return 1;
    }

    return 0;
}

void ADXPD_Start(ADXPD adxpd) {
    if (adxpd->stat == 0) {
        adxpd->num_blk = 0;
        adxpd->stat    = 1;
    }
}

void ADXPD_Stop(ADXPD adxpd) {
    adxpd->stat = 0;
    __builtin__clear(&adxpd->unk28, sizeof(ADXPD_OBJ_SUB));
}

void ADXPD_Reset(ADXPD adxpd) {
    if (adxpd->stat == 3) {
        adxpd->stat = 0;
    }
}

int ADXPD_GetNumBlk(ADXPD adxpd) {
    return adxpd->num_blk;
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

    if (adxpd->unk14 == 1) {
        adxpd->num_blk = ADX_DecodeMono4(adxpd->unk18, adxpd->unk1C, adxpd->unk20, &adxpd->unk28.unk0, adxpd->unk30,
                                         adxpd->unk32, &adxpd->unk34, adxpd->unk36, adxpd->unk38);
    } else {
        adxpd->num_blk =
            ADX_DecodeSte4(adxpd->unk18, adxpd->unk1C, adxpd->unk20, &adxpd->unk28.unk0, adxpd->unk24, &adxpd->unk28.unk4,
                           adxpd->unk30, adxpd->unk32, &adxpd->unk34, adxpd->unk36, adxpd->unk38);

        if ((adxpd->num_blk % 2) == 1) {
            adxpd_error();
        }
    }

    adxpd->stat = 3;
}