#include "CriWare/acssnd.h"

void func_0201fe0c(ACSSND*);

ACSSND* ACSSND_Create(ACSSND* work, u16 workSize) {
    if (work == NULL) {
        func_0201f55c("E2003091205 ACSSND_Create : Work pointor is NULL.");
        return NULL;
    }
    if (workSize < sizeof(ACSSND)) {
        func_0201f55c("E2003091206 ACSSND_Create : Work size is too small.");
        return NULL;
    }
    func_0201fe0c(work);
    return work;
}

void func_0201f5b0(ACSSND* snd) {
    if (snd != NULL) {
        func_0201fe0c(snd);
    }
}

void ACSSND_Init() {}

void func_0201f930() {}

s32 func_0201f9b4(ACSSND* snd) {
    return snd->unk_58;
}

void ACSSND_StartFname() {}

void ACSSND_Stop() {}

void ACSSND_Pause() {}

void ACSSND_GetStatPause() {}

void ACSSND_GetStat() {}

void func_0201fb58() {}

void ACSSND_SetOutVol() {}

void ACSSND_SetLpFlg() {}

void func_0201fc38(ACSSND* snd, s32 param_2) {
    snd->unk_B0 = param_2;
}

void func_0201fc40(ACSSND* snd, s32 param_2) {
    snd->unk_B4 = param_2;
}

void func_0201fc48(ACSSND* snd, s32 param_2) {
    snd->unk_B8 = param_2;
}

void func_0201fc50(ACSSND* snd, s32 param_2) {
    snd->unk_BC = param_2;
}

void func_0201fc58(ACSSND* snd, s32 param_2) {
    snd->unk_C0 = param_2;
}

void ACSSND_GetActiveAdxt() {}

void ACSSND_GetAdxt() {}

void ACSSND_GetNumAdxt() {}

void func_0201fd44(ACSSND* snd, int param_2) {
    snd->unk_CC += param_2;
    snd->unk_D0 = 1;
}

void func_0201fd5c(ACSSND* snd) {
    snd->unk_CC = 0;
    snd->unk_D0 = 0;
}

void ACSSND_ApplyAllVol() {}

void func_0201fe0c(ACSSND*) {}

void ACSSND_Start() {}

void func_020203c8(ACSSND* snd, s8 param_2) {
    snd->unk_AC = param_2;
}
