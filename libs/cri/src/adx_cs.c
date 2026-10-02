#include <cri/acssvr.h>
#include <cri/adxt.h>
#include <cri/cri_xpt.h>
#include <cri/private/svm.h>

void    ACSSND_StartFname(ACSSND* snd, const char* fname);
void    ACSSND_Stop(ACSSND* snd);
void    ACSSND_Pause(ACSSND* snd, int sw);
void    ACSSND_SetOutVol(ACSSND* snd, int vol);
void    ACSSND_SetLpFlg(ACSSND* snd, int flg);
void    func_0201fc38(ACSSND* snd, int param_2);
void    func_0201fc40(ACSSND* snd, int param_2);
void    func_0201fc48(ACSSND* snd, int param_2);
void    func_0201fc50(ACSSND* snd, int param_2);
void    func_0201fc58(ACSSND* snd, int param_2);
ADXT    ACSSND_GetActiveAdxt(ACSSND* snd);
ADXT    ACSSND_GetAdxt(ACSSND* snd, int idx);
int     ACSSND_GetNumAdxt(ACSSND* snd);
int     func_0201f1d4(ACSSVR* svr, int* cprm);
ACSVHL* func_0201f4e0(ACSSVR* svr);
void    func_02021284(ACSVHL* vhl);

ACSSND* func_0201ec1c(int* param_1, ACSSND* work, unsigned int workSize);
int     func_0201ec6c(int* cprm);
void    func_0201ecac(ACSSND* snd, const char* fname);
void    func_0201ecd4(ACSSND* snd);
void    func_0201ed04(ACSSND* snd, int sw);
void    func_0201ed34(ACSSND* snd, int vol);
void    func_0201ed64(ACSSND* snd, int flg);
void    func_0201ed94(ACSSND* snd, int param_2);
void    func_0201edc4(ACSSND* snd, int param_2);
void    func_0201edf4(ACSSND* snd, int param_2);
void    func_0201ee24(ACSSND* snd, int param_2);
void    func_0201ee54(ACSSND* snd, int param_2);
ADXT    func_0201ee84(ACSSND* snd);
ADXT    func_0201eebc(ACSSND* snd, int idx);
int     func_0201eeec(ACSSND* snd);
void    func_0201ef0c(void);
void    func_0201ef40(void);
void    func_0201ef44(void);

static ACSSVR adxcs_svr_obj;
ACSSVR*       adxcs_svr = &adxcs_svr_obj;

#ifdef REGION_USA
char* volatile const adxcs_build = "\nADXCS/NITRO Ver.1.23 Build:Sep 28 2007 13:14:10\n";
#else
char* volatile const adxcs_build = "\nADXCS/NITRO Ver.1.23 Build:Jun 22 2007 15:54:53\n";
#endif

void ADXCS_Init(void) {
    adxcs_build;
    ACSSVR_Init(adxcs_svr);
}

ACSSND* ADXCS_Create(int* param_1, ACSSND* work, unsigned int workSize) {
    ACSSND* snd;

    func_0201ef40();
    snd = func_0201ec1c(param_1, work, workSize);
    func_0201ef44();
    return snd;
}

ACSSND* func_0201ec1c(int* param_1, ACSSND* work, unsigned int workSize) {
    return ACSSVR_CreatSnd(adxcs_svr, param_1, work, workSize);
}

int ADXCS_CalcWorkCprm(int* cprm) {
    int size;

    func_0201ef40();
    size = func_0201ec6c(cprm);
    func_0201ef44();
    return size;
}

int func_0201ec6c(int* cprm) {
    return func_0201f1d4(adxcs_svr, cprm);
}

void func_0201ec88(ACSSND* snd, const char* fname) {
    func_0201ef40();
    func_0201ecac(snd, fname);
    func_0201ef44();
}

void func_0201ecac(ACSSND* snd, const char* fname) {
    ACSSND_StartFname(snd, fname);
}

void func_0201ecb8(ACSSND* snd) {
    func_0201ef40();
    func_0201ecd4(snd);
    func_0201ef44();
}

void func_0201ecd4(ACSSND* snd) {
    ACSSND_Stop(snd);
}

void func_0201ece0(ACSSND* snd, int sw) {
    func_0201ef40();
    func_0201ed04(snd, sw);
    func_0201ef44();
}

void func_0201ed04(ACSSND* snd, int sw) {
    ACSSND_Pause(snd, sw);
}

void func_0201ed10(ACSSND* snd, int vol) {
    func_0201ef40();
    func_0201ed34(snd, vol);
    func_0201ef44();
}

void func_0201ed34(ACSSND* snd, int vol) {
    ACSSND_SetOutVol(snd, vol);
}

void func_0201ed40(ACSSND* snd, int flg) {
    func_0201ef40();
    func_0201ed64(snd, flg);
    func_0201ef44();
}

void func_0201ed64(ACSSND* snd, int flg) {
    ACSSND_SetLpFlg(snd, flg);
}

void func_0201ed70(ACSSND* snd, int param_2) {
    func_0201ef40();
    func_0201ed94(snd, param_2);
    func_0201ef44();
}

void func_0201ed94(ACSSND* snd, int param_2) {
    func_0201fc38(snd, param_2);
}

void func_0201eda0(ACSSND* snd, int param_2) {
    func_0201ef40();
    func_0201edc4(snd, param_2);
    func_0201ef44();
}

void func_0201edc4(ACSSND* snd, int param_2) {
    func_0201fc40(snd, param_2);
}

void func_0201edd0(ACSSND* snd, int param_2) {
    func_0201ef40();
    func_0201edf4(snd, param_2);
    func_0201ef44();
}

void func_0201edf4(ACSSND* snd, int param_2) {
    func_0201fc48(snd, param_2);
}

void func_0201ee00(ACSSND* snd, int param_2) {
    func_0201ef40();
    func_0201ee24(snd, param_2);
    func_0201ef44();
}

void func_0201ee24(ACSSND* snd, int param_2) {
    func_0201fc50(snd, param_2);
}

void func_0201ee30(ACSSND* snd, int param_2) {
    func_0201ef40();
    func_0201ee54(snd, param_2);
    func_0201ef44();
}

void func_0201ee54(ACSSND* snd, int param_2) {
    func_0201fc58(snd, param_2);
}

ADXT func_0201ee60(ACSSND* snd) {
    ADXT adxt;

    func_0201ef40();
    adxt = func_0201ee84(snd);
    func_0201ef44();
    return adxt;
}

ADXT func_0201ee84(ACSSND* snd) {
    return ACSSND_GetActiveAdxt(snd);
}

ADXT func_0201ee90(ACSSND* snd, int idx) {
    ADXT adxt;

    func_0201ef40();
    adxt = func_0201eebc(snd, idx);
    func_0201ef44();
    return adxt;
}

ADXT func_0201eebc(ACSSND* snd, int idx) {
    return ACSSND_GetAdxt(snd, idx);
}

int func_0201eec8(ACSSND* snd) {
    int num;

    func_0201ef40();
    num = func_0201eeec(snd);
    func_0201ef44();
    return num;
}

int func_0201eeec(ACSSND* snd) {
    return ACSSND_GetNumAdxt(snd);
}

void func_0201eef8(void) {
    func_0201ef40();
    func_0201ef0c();
    func_0201ef44();
}

void func_0201ef0c(void) {
    func_02021284(func_0201f4e0(adxcs_svr));
}

void func_0201ef28(void) {
    func_0201a798();
}

void func_0201ef34(void) {
    func_0201a7a8();
}

void func_0201ef40(void) {}

void func_0201ef44(void) {}
