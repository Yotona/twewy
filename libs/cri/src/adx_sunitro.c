#include <cri/cri_xpt.h>

void func_020216b4(char*);
void func_02021720(void);
void func_02021724(void);

void func_02021698(char* arg0) {
    func_02021720();
    func_020216b4(arg0);
    func_02021724();
}

void func_020216b4(char* arg0) {
    char stack[8];

    if (arg0 != NULL && strcmp(arg0, "ROFS") != 0) {
        cvFsSetDefDev(arg0);
        if (cvFsGetVolumeInfo("ROFS", arg0, stack) >= 0) {
            cvFsSetDefVol("ROFS", arg0);
        }
    }
}

void func_02021720(void) {
    return;
}

void func_02021724(void) {
    return;
}