#include <cri/private/adx_crs.h>
#include <cri/private/svm.h>

volatile int adxcrs_msk = 0;
volatile int adxcrs_lvl = 0;
volatile int adxcrs_cnt = 0;

// Nonmatching: 99.6%. Body and instruction count are exact; the only remaining
// difference is which object mwcc anchors the literal-pool base register to.
// The target anchors to 0x0206bc30 (adxcrs_lvl, the lowest object in the TU's
// bss) and reaches the two live variables at +4/+8; ours anchors to
// adxcrs_msk and reaches them at +0/+4, i.e. the same relative layout one dword
// lower, because adxcrs_lvl is unreferenced in this TU and so is excluded from
// mwcc's addressing cluster. Needs a source form that references 0x0206bc30
// without emitting an access to it.
void ADXCRS_Init(void) {
    adxcrs_cnt++;
    if (adxcrs_cnt == 1) {
        adxcrs_msk = 0;
    }
}

// Nonmatching: 99.6%, same pool-base anchor issue as ADXCRS_Init above.
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
