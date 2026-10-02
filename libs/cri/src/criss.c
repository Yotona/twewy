#include <cri/cri_xpt.h>
#include <cri/criss.h>

static CRISS* criSsPly_Create(int, int, int, int);

struct {
    const char* setFadeTime;
    const char* notInitialized;
    const char* stop;
    const char* heapIfNull;
    const char* filenameNull;
    const char* handleNull;
    const char* pause;
    const char* setVolume;
    const char* create;
    const char* destroy;
    const char* play;
    const char* getStatus;
    const char* setLpFlg;
} data_020659e0 = {"criSsPly_SetFadeTime", "CRISS is not initialized.", "criSsPly_Stop",  "CriSsHeapIf is null.",
                   "filename is null.",    "handle is null.",           "criSsPly_Pause", "criSsPly_SetVolume",
                   "criSsPly_Create",      "criSsPly_Destroy",          "criSsPly_Play",  "criSsPly_GetStatus",
                   "criSsPly_SetLpFlg"};

CRISS data_02071b00;

void  cri_ss_initialize(void* arg0, int arg1, void* arg2, void* arg3, int arg4);
void* func_02021ef4();
void  func_02021f94(const char* funcName, const char* status);

void func_02021728(void* arg0, int arg1, void* arg2, void* arg3) {
    cri_ss_initialize(arg0, arg1, arg2, arg3, 1);
}

void cri_ss_initialize(void* arg0, int arg1, void* arg2, void* arg3, int arg4) {}

void func_020218ec(void) {
    if (data_02071b00.unk_00 != 0) {
        ADXM_ExecMain();
    }
}

void func_0202190c() {}

void func_02021938(unsigned int param_1) {
    CRISS*       pCVar1;
    unsigned int uVar2;

    pCVar1 = &data_02071b00;
    uVar2  = data_02071b00.unk_00;
    if (data_02071b00.unk_00 != 0) {
        pCVar1 = data_02071b00.unk_04;
        if (pCVar1 != 0) {
            ADXM_WaitVsync(pCVar1, data_02071b00.unk_00);
        }
    }
}

CRISS* func_02021960(void* (**arg0)(int), int arg1, int arg2) {
    return criSsPly_Create(arg0, arg1, arg2, 0);
}

CRISS* criSsPly_Create(int, int, int, int) {}

int func_02021bb4(int param_1) {
    func_0201ee60(**(int**)(param_1 + 4));
}

void func_02021bc8(int param_1, int param_2) {
    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.notInitialized);
        return;
    }
    if (param_1 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.handleNull);
        return;
    }
    if (param_2 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.filenameNull);
        return;
    }

    int* puVar1 = func_02021ef4(param_1);
    func_02021f4c(puVar1);
    func_0201ec88((ACSSND*)*puVar1, param_2);
}

void criSsPly_Stop(int param_1) {
    int* puVar1;

    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.stop, data_020659e0.notInitialized);
        return;
    }
    if (param_1 == 0) {
        func_02021f94(data_020659e0.stop, data_020659e0.handleNull);
        return;
    }
    puVar1 = func_02021ef4(param_1);
    func_0201ecb8((ACSSND*)*puVar1);
}

void criSsPly_Pause(int param_1, int param_2) {
    int* puVar1;

    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.pause, data_020659e0.notInitialized);
        return;
    }
    if (param_1 == 0) {
        func_02021f94(data_020659e0.pause, data_020659e0.handleNull);
        return;
    }
    puVar1 = func_02021ef4(param_1);
    func_0201ece0((ACSSND*)*puVar1, param_2);
}

void criSsPly_SetVolume(CRISS* criss, int volume) {
    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.setVolume, data_020659e0.notInitialized);
        return;
    }
    if (criss == 0) {
        func_02021f94(data_020659e0.setVolume, data_020659e0.handleNull);
        return;
    }
    int* puVar1 = func_02021ef4(criss);
    func_0201ed10((ACSSND*)*puVar1, volume);
    return;
}

void criSsPly_SetFadeTime(CRISS* criss, unsigned int param_2, unsigned int param_3) {}

int criSsPly_Play(int param_1) {
    ADXT adxt;
    int  iVar1;

    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.notInitialized);
        return 0;
    }
    if (param_1 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.handleNull);
        return 0;
    }
    adxt  = (ADXT)func_02021bb4(param_1);
    iVar1 = ADXT_GetTimeReal(adxt);
    return iVar1;
}

void criSsPly_SetLpFlg(int param_1, int param_2) {
    int* puVar1;

    if (data_02071b00.unk_00 == '\0') {
        func_02021f94(data_020659e0.setLpFlg, data_020659e0.notInitialized);
        return;
    }
    if (param_1 == 0) {
        func_02021f94(data_020659e0.setLpFlg, data_020659e0.handleNull);
        return;
    }
    puVar1 = func_02021ef4(param_1);
    func_0201ed40(*puVar1, param_2);
}

void* func_02021ef4() {}

void func_02021f4c(int*) {}

void func_02021f74() {}

void func_02021f90() {
    return;
}

void func_02021f94(const char* funcName, const char* status) {}
