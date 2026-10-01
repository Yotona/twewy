#include <cri/private/adx_crs.h>
#include <cri/private/svm.h>

volatile int adxcrs_msk = 0;
volatile int adxcrs_lvl = 0;
volatile int adxcrs_cnt = 0;

// Never called and dead-stripped, but required for adxcrs_lvl
// to be emitted in its expected position
static void adxcrs_unused(void) {
    adxcrs_lvl = 0;
}

void ADXCRS_Init(void) {
    adxcrs_cnt++;
    if (adxcrs_cnt == 1) {
        adxcrs_msk = 0;
    }
}

void ADXCRS_Finish(void) {
    adxcrs_cnt--;
    if (adxcrs_cnt == 0) {
        adxcrs_msk = 0;
    }
}

void ADXCRS_Lock(void) {
    SVM_Lock();
}

void ADXCRS_Unlock(void) {
    SVM_Unlock();
}

void ADXCRS_Enter(void) {
    return;
}

void ADXCRS_Leave(void) {
    return;
}
