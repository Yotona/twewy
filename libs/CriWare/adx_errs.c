#include <CriWare/private/svm.h>

#define ADXERR_MAX_LEN 0x100

char  adxerr_msg[ADXERR_MAX_LEN] = {0};
void* adxerr_obj                 = NULL;
void (*adxerr_func)(void*, s8*)  = NULL;

char data_0206bc44[32] = {0};

void ADXERR_Init(void) {
    memset(&adxerr_msg, 0, sizeof(adxerr_msg));
    adxerr_func = NULL;
    adxerr_obj  = NULL;
}

void ADXERR_Finish() {
    memset(&adxerr_msg, 0, sizeof(adxerr_msg));
    adxerr_func = NULL;
    adxerr_obj  = NULL;
}

void ADXERR_CallErrFunc1(s8* msg) {
    if (msg == NULL) {
        SVM_CallErr("Error");
        return;
    }
    func_02021624(adxerr_msg, 0x100, msg, ADXERR_MAX_LEN - 1);

    if (adxerr_func != NULL) {
        adxerr_func(adxerr_obj, adxerr_msg);
    }

    SVM_CallErr(adxerr_msg);
}

void ADXERR_CallErrFunc2(s8* arg0, s8* arg1) {
    if (arg0 == NULL || arg1 == NULL) {
        SVM_CallErr("Error");
        return;
    }

    func_02021624(adxerr_msg, 0x100, arg0, ADXERR_MAX_LEN - 1);
    func_02021648(adxerr_msg, 0x100, arg1, ADXERR_MAX_LEN - 1);

    if (adxerr_func != NULL) {
        adxerr_func(adxerr_obj, adxerr_msg);
    }

    SVM_CallErr(adxerr_msg);
}

void ADXERR_ItoA(s32 value, s8* str, s32 base) {
    s32 i;
    s32 len;

    for (i = 0; i < 32; i++) {
        str[i] = value % 10;
        value /= 10;

        if (value == 0) {
            str[i] = '\0';
            break;
        }
    }

    len = strlen(data_0206bc44);
    len = (len < base - 1) ? len : base - 1;

    for (i = 0; i < len; i++) {
        str[i] = data_0206bc44[len - 1 - i];
    }

    str[i] = '\0';
}

void ADXERR_ItoA2(s32 arg0, s32 arg1, s8* str, s32 base) {
    ADXERR_ItoA(arg0, str, base);
    func_02021648(str, base, " ", base - strlen(str) - 1);
    ADXERR_ItoA(arg1, &str[strlen(str)], 4 - strlen(str));
}