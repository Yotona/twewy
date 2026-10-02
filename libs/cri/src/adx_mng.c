#include <cri/private/svm.h>

static int func_0201eba0(int framework);
static int ADXM_ExecSvrAll(void);

int data_020652b0 = -1;

void ADXMNG_SetFramework(int framework) {
    data_020652b0 = framework;
}

int ADXMNG_CallMainServerFunctions(void) {
    switch (func_0201eba0(data_020652b0)) {
        case 0:
            break;
        case 1:
            ADXM_ExecSvrAll();
            break;
        case 2:
            SVM_ExecSvrMain();
            break;
        case 3:
            SVM_ExecSvrMwIdle();
            SVM_ExecSvrUsrIdle();
            break;
        case -1:
            break;
    }
    return 0;
}

int ADXMNG_CallVintServerFunctions(void) {
    switch (func_0201eba0(data_020652b0)) {
        case 0:
            break;
        case 1:
            break;
        case 2:
            SVM_ExecSvrVint();
            break;
        case 3:
            SVM_ExecSvrVint();
            SVM_ExecSvrUsrVsync();
            SVM_ExecSvrVsync();
            SVM_ExecSvrUhigh();
            SVM_ExecSvrMain();
            break;
        case -1:
    }
    return 0;
}

static int ADXM_ExecSvrAll(void) {
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

static int func_0201eba0(int framework) {
    if (framework == -1) {
        if (ADXM_IsSetupThrd() == 1) {
            return 2;
        } else {
            return 1;
        }
    }
    return framework;
}
