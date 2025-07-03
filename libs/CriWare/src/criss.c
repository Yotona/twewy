#include <criss.h>
#include <types.h>

static CRISS* criSsPly_Create(s32, s32, s32, s32);

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

void  cri_ss_initialize(void* arg0, s32 arg1, void* arg2, void* arg3, s32 arg4);
void* func_02021ef4();
void  func_02021f94(const char* funcName, const char* status);

void func_02021728(void* arg0, s32 arg1, void* arg2, void* arg3) {
    cri_ss_initialize(arg0, arg1, arg2, arg3, 1);
}

void cri_ss_initialize(void* arg0, s32 arg1, void* arg2, void* arg3, s32 arg4) {}

void func_020218ec(void) {
    if (data_02071b00.unk_00 != 0) {
        func_0201b930();
    }
}

void func_0202190c() {}

void func_02021938(u32 param_1) {
    CRISS* pCVar1;
    u32    uVar2;

    pCVar1 = &data_02071b00;
    uVar2  = data_02071b00.unk_00;
    if (data_02071b00.unk_00 != 0) {
        pCVar1 = data_02071b00.unk_04;
        if (pCVar1 != 0) {
            func_0201b8dc(pCVar1, data_02071b00.unk_00);
        }
    }
}

CRISS* func_02021960(GameState* (**arg0)(s32), s32 arg1, s32 arg2) {
    return criSsPly_Create(arg0, arg1, arg2, 0);
}

CRISS* criSsPly_Create(s32, s32, s32, s32) {}

s32 func_02021bb4(s32 param_1) {
    func_0201ee60(**(s32**)(param_1 + 4));
}

void func_02021bc8(s32 param_1, s32 param_2) {
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

    s32* puVar1 = func_02021ef4(param_1);
    func_02021f4c(puVar1);
    func_0201ec88((ACSSND*)*puVar1, param_2);
}

void criSsPly_Stop(s32 param_1) {
    s32* puVar1;

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

void criSsPly_Pause(int param_1, s32 param_2) {
    s32* puVar1;

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

void criSsPly_SetVolume(CRISS* criss, s32 volume) {
    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.setVolume, data_020659e0.notInitialized);
        return;
    }
    if (criss == 0) {
        func_02021f94(data_020659e0.setVolume, data_020659e0.handleNull);
        return;
    }
    s32* puVar1 = func_02021ef4(criss);
    func_0201ed10((ACSSND*)*puVar1, volume);
    return;
}

void criSsPly_SetFadeTime(CRISS* criss, u32 param_2, u32 param_3) {}

int criSsPly_Play(int param_1) {
    ADXT* adxt;
    int   iVar1;

    if (data_02071b00.unk_00 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.notInitialized);
        return 0;
    }
    if (param_1 == 0) {
        func_02021f94(data_020659e0.play, data_020659e0.handleNull);
        return 0;
    }
    adxt  = (ADXT*)func_02021bb4(param_1);
    iVar1 = ADXT_Play(adxt);
    return iVar1;
}

void criSsPly_SetLpFlg(int param_1, s32 param_2) {
    s32* puVar1;

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

void func_02021f4c(s32*) {}

void func_02021f74() {}

void func_02021f90() {
    return;
}

void func_02021f94(const char* funcName, const char* status) {}

void func_02021fcc() {}

void func_020220a8() {}

void func_02022140() {}

void func_02022194() {}

void func_020221e4() {}

void func_0202223c() {}

void func_02022284() {}

void func_02022424() {}

void func_02022440() {}

void func_02022488() {}

void func_020224f4() {}

void func_02022534() {}

void func_0202254c() {}

void func_0202255c() {}

void func_0202258c() {}

void func_020225c4() {}

void func_020225ec() {}

void func_02022608() {}

void func_02022640() {}

void func_020226fc() {}

void func_0202296c() {}

void func_020229d4() {}
