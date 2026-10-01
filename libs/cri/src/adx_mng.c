#include <cri/private/svm.h>

// ADXMNG: dispatches the SVM server phases for the selected framework
// (adx_mng.c in the Wii tree). Only the trivial entry points are in C so far.

int data_020652b0 = -1;

void ADXMNG_SetFramework(int framework) {
    data_020652b0 = framework;
}

int ADXM_ExecSvrAll(void) {
    SVM_ExecSvrVint();
    SVM_ExecSvrUsrVsync();
    SVM_ExecSvrVsync();
    SVM_ExecSvrUhigh();
    SVM_ExecSvrFs();
    SVM_ExecSvrMain();
    SVM_ExecSvrMwIdle();
    SVM_ExecSvrUsrIdle();
    return 0;
}
