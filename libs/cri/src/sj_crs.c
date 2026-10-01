#include <cri/private/svm.h>
#include <cri/sj.h>

volatile int sjcrs_msk = 0;
volatile int sjcrs_lvl = 0;
volatile int sjcrs_cnt = 0;

void SJCRS_Init(void) {
    sjcrs_cnt++;
    if (sjcrs_cnt == 1) {
        sjcrs_msk = 0;
    }
}

void SJCRS_Finish(void) {
    sjcrs_cnt--;
    if (sjcrs_cnt == 0) {
        sjcrs_msk = 0;
    }
}

void SJCRS_Lock(void) {
    SVM_Lock();
}

void SJCRS_Unlock(void) {
    SVM_Unlock();
}
