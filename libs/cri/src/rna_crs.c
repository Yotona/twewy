#include <cri/private/svm.h>

void RNACRS_Lock(void) {
    SVM_Lock();
}

void RNACRS_Unlock(void) {
    SVM_Unlock();
}
