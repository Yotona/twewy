#include <CriWare/private/adx_b.h>
#include <CriWare/sj.h>

void (*ahxsetsjifunc)(s32, SJ)      = NULL;
void (*ahxsetdecsmplfunc)(s32, s32) = NULL;
void (*ahxtermsupplyfunc)(s32)      = NULL;
void (*ahxexecfunc)(ADXB)           = NULL;

void ADXB_SetAhxInSj(ADXB adxb, SJ sj) {
    if (adxb->unkB4 != 0) {
        ahxsetsjifunc(adxb->unkB4, sj);
    }
}

void ADXB_SetAhxDecSmpl(ADXB adxb, s32 decsmpl) {
    if (adxb->unkB4 != 0) {
        ahxsetdecsmplfunc(adxb->unkB4, decsmpl);
    }
    adxb->unkB8 = decsmpl;
    adxb->unkBC = decsmpl / 96;
}

void ADXB_ExecOneAhx(ADXB adxb) {
    return ahxexecfunc(adxb);
}

void ADXB_AhxTermSupply(ADXB adxb) {
    if (adxb->unkB4 != 0) {
        ahxtermsupplyfunc(adxb->unkB4);
    }
}