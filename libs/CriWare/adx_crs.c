#include <CriWare/private/adx_crs.h>
#include <CriWare/private/svm.h>

struct {
    /* 0x0 */ char unk_00[0x4];
    /* 0x4 */ s32  unk_04;
    /* 0x8 */ s32  unk_08;
} data_0206bc30 = {0};

void func_02012f20(void) {
    data_0206bc30.unk_08++;
    if (data_0206bc30.unk_08 == 1) {
        data_0206bc30.unk_04 = 0;
    }
}

void func_02012f48(void) {
    data_0206bc30.unk_08--;
    if (data_0206bc30.unk_08 == 0) {
        data_0206bc30.unk_04 = 0;
    }
}

void ADXCRS_Lock(void) {
    SVM_Lock();
}

void ADXCRS_Unlock(void) {
    SVM_Unlock();
}
