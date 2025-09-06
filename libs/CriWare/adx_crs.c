#include <CriWare/private/adx_crs.h>
#include <CriWare/private/svm.h>

void ADXCRS_Lock(void) {
    SVM_Lock();
}

void ADXCRS_Unlock(void) {
    SVM_Unlock();
}
