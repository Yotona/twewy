#include <CriWare/private/adx_crs.h>
#include <CriWare/private/svm.h>

s32 data_0206bc30 = 0;
s32 data_0206bc34 = 0;
s32 data_0206bc38 = 0;

void ADXCRS_Init(void) {
    data_0206bc38++;
    if (data_0206bc38 == 1) {
        data_0206bc34 = 0;
    }
}

void func_02012f48(void) {
    data_0206bc38--;
    if (data_0206bc38 == 0) {
        data_0206bc34 = 0;
    }
}

void ADXCRS_Lock(void) {
    SVM_Lock();
}

void ADXCRS_Unlock(void) {
    SVM_Unlock();
}
