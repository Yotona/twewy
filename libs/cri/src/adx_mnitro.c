#include <cri/cri_xpt.h>
#include <cri/private/svm.h>
#include <mem.h>
#include <nitro/os/cpustat.h>
#include <nitro/os/interrupt.h>
#include <nitro/os/thread.h>
#include <nitro/reg.h>

// ADXNITRO -- the ADXM thread/server manager (ADXM_SetupThrd, ADXM_ExecMain,
// ADXM_ExecVint, ...); the NITRO counterpart of adx_mps2.c / adx_mgc.c /
// adx_mwii.c. Its banner reads "ADXNITRO Ver.1.00" (JP: Ver.0.34).
//
// Three server threads cooperate with the main thread: each vsync the vsync
// thread runs, hands over to the fs thread, which hands back to the main
// thread; the mwidle thread runs only under framework 2 (threaded).

#define ADXM_FRAMEWORK_THREAD 2

// Thread parameters passed to ADXM_SetupThrd: per server thread, a priority,
// a stack buffer (rounded up to 4) and its size.
typedef struct ADXM_TPRM {
    /* 0x00 */ int   prio_vsync;
    /* 0x04 */ int   prio_fs;
    /* 0x08 */ int   prio_mwidle;
    /* 0x0C */ void* stack_vsync;
    /* 0x10 */ void* stack_fs;
    /* 0x14 */ void* stack_mwidle;
    /* 0x18 */ int   stack_size_vsync;
    /* 0x1C */ int   stack_size_fs;
    /* 0x20 */ int   stack_size_mwidle;
} ADXM_TPRM; // Size: 0x24

// Two user hooks: cb0 runs as the main thread waits for vsync, cb1 at most
// once per vsync (from the mwidle thread, or at vint if it never got to run).
typedef struct ADXM_CB {
    /* 0x00 */ void (*cb0_func)(void*);
    /* 0x04 */ void* cb0_obj;
    /* 0x08 */ void (*cb1_func)(void*);
    /* 0x0C */ void* cb1_obj;
    /* 0x10 */ int   cb1_done;
} ADXM_CB; // Size: 0x14

typedef struct ADXM_OBJ {
    /* 0x000 */ int       mwidle_act;
    /* 0x004 */ int       mwidle_end;
    /* 0x008 */ int       vsync_act;
    /* 0x00C */ int       vsync_end;
    /* 0x010 */ int       fs_act;
    /* 0x014 */ int       fs_end;
    /* 0x018 */ int       unk18;
    /* 0x01C */ int       unk1C;
    /* 0x020 */ int       unk20;
    /* 0x024 */ int       framework;
    /* 0x028 */ OSThread  vsync_thread;
    /* 0x0E8 */ OSThread  fs_thread;
    /* 0x1A8 */ OSThread  mwidle_thread;
    /* 0x268 */ OSThread* main_thread;
    /* 0x26C */ int       vsync_cnt;
    /* 0x270 */ int       fs_cnt;
    /* 0x274 */ int       mwidle_cnt;
} ADXM_OBJ; // Size: 0x278

void ADXMNG_SetFramework(int framework);
void ADXMNG_CallMainServerFunctions(void);
void ADXMNG_CallVintServerFunctions(void);
int  CRICFG_Read(const char* name, int* value);

int  adxm_get_init_level(void);
void adxm_inc_init_level(void);
int  adxm_is_vsync_end(void);
void adxm_set_vsync_end(void);
void adxm_clear_vsync_end(void);
void adxm_exec_cb0(void);
void adxm_exec_cb1(void);
void ADXM_SetupThrd(ADXM_TPRM* tprm);

#ifdef REGION_USA
int data_0207078c = 0;
#endif

#ifdef REGION_USA
char* volatile const adxnitro_build = "\nADXNITRO Ver.1.00 Build:Sep 28 2007 13:14:05\n";
#else
char* volatile const adxnitro_build = "\nADXNITRO Ver.0.34 Build:Jun 22 2007 15:54:48\n";
#endif

int adxm_imask_lvl = 15;

int data_02070784   = 0;
int adxm_init_level = 0;
int data_02070788   = 0;
int data_02070778   = 0;
int adxm_lock_level = 0;
int data_02070774   = 0;

ADXM_OBJ  adxm_obj;
ADXM_TPRM adxm_save_tprm;
int       data_020707a8;
int       adxm_save_ie;
ADXM_CB   adxm_cb;

void ADXM_WaitVsync(void) {
    if (adxm_obj.framework == 0 || adxm_obj.framework == 1) {
        OS_Wait();
    } else if (adxm_obj.framework == ADXM_FRAMEWORK_THREAD) {
        adxm_exec_cb0();
        adxm_cb.cb1_done = 0;
        OS_ResumeThreadImmediate(&adxm_obj.mwidle_thread);
        OS_PauseThread(NULL);
    }
}

void ADXM_ExecMain(void) {
    ADXMNG_CallMainServerFunctions();
}

void adxm_lock(void* obj) {
    if (adxm_lock_level == 0) {
        int ie = REG_IE;
        OS_DisableInterrupts(IRQ_VBLANK);
        adxm_save_ie = ie;
    }
    adxm_lock_level++;
}

void adxm_unlock(void* obj) {
    adxm_lock_level--;
    if (adxm_lock_level == 0) {
        OS_EnableInterrupts(IRQ_VBLANK);
    }
}

void adxm_vsync_proc(void* arg) {
    while (adxm_obj.vsync_act == 1) {
        if (adxm_obj.fs_end == 1) {
            OS_ResumeThreadImmediate(adxm_obj.main_thread);
        } else {
            OS_ResumeThreadImmediate(&adxm_obj.fs_thread);
        }
        SVM_ExecSvrUsrVsync();
        SVM_ExecSvrVsync();
        adxm_obj.vsync_cnt++;
        OS_PauseThread(NULL);
    }
    adxm_set_vsync_end();
}

void adxm_fs_proc(void* arg) {
    while (adxm_obj.fs_act == 1) {
        OS_ResumeThreadImmediate(adxm_obj.main_thread);
        SVM_ExecSvrFs();
        adxm_obj.fs_cnt++;
        OS_PauseThread(NULL);
    }
    adxm_obj.fs_end = 1;
}

void adxm_mwidle_proc(void* arg) {
    while (adxm_obj.mwidle_act == 1) {
        if (SVM_ExecSvrMwIdle() == 0) {
            SVM_Lock();
            if (adxm_cb.cb1_done == 0) {
                adxm_cb.cb1_done = 1;
                adxm_exec_cb1();
            }
            SVM_Unlock();
            OS_PauseThread(NULL);
        }
        adxm_obj.mwidle_cnt++;
    }
    adxm_obj.mwidle_end = 1;
}

void ADXM_SetCbErr(void (*func)(void*, char*), void* obj) {
    SVM_SetCbErr(func, obj);
}

int ADXM_SetupFramework(int framework, ADXM_TPRM* tprm) {
    adxm_obj.framework = framework;
    memset(&adxm_cb, 0, sizeof(ADXM_CB));
    if (adxm_obj.framework == 0 || adxm_obj.framework == 1) {
        ADXMNG_SetFramework(1);
        return TRUE;
    }
    if (adxm_obj.framework == ADXM_FRAMEWORK_THREAD) {
        ADXMNG_SetFramework(ADXM_FRAMEWORK_THREAD);
    }
    ADXM_SetupThrd(tprm);
    return TRUE;
}

void ADXM_SetupThrd(ADXM_TPRM* tprm) {
    int   imask;
    void* stack;

    adxnitro_build;
    if (tprm == NULL) {
        SVM_CallErr("ADXM_SetupThrd: It is necessary to set up a thread parameter.");
        return;
    }
    adxm_inc_init_level();
    if (adxm_get_init_level() != 1) {
        return;
    }

    adxm_obj.main_thread = OS_GetCurrentThread();
    if (adxm_obj.framework != ADXM_FRAMEWORK_THREAD) {
        adxm_obj.framework = ADXM_FRAMEWORK_THREAD;
    }
    SVM_Init();
    SVM_SetCbLock(adxm_lock, NULL);
    SVM_SetCbUnlock(adxm_unlock, NULL);
    if (CRICFG_Read("IMASK_LVL", &imask) == 0) {
        adxm_imask_lvl = imask;
    } else {
        adxm_imask_lvl = 15;
    }
    adxm_save_tprm = *tprm;
    adxm_obj.unk18 = 0;
    adxm_obj.unk1C = 0;

    adxm_obj.vsync_act = 1;
    adxm_clear_vsync_end();
    stack = (void*)((((unsigned int)tprm->stack_vsync + 3) & ~3) + tprm->stack_size_vsync);
    OS_CreateThread(&adxm_obj.vsync_thread, adxm_vsync_proc, NULL, stack, tprm->stack_size_vsync, adxm_save_tprm.prio_vsync);

    adxm_obj.fs_act = 1;
    adxm_obj.fs_end = 0;
    stack           = (void*)((((unsigned int)tprm->stack_fs + 3) & ~3) + tprm->stack_size_fs);
    OS_CreateThread(&adxm_obj.fs_thread, adxm_fs_proc, NULL, stack, tprm->stack_size_fs, adxm_save_tprm.prio_fs);

    if (adxm_obj.framework == ADXM_FRAMEWORK_THREAD) {
        adxm_obj.mwidle_act = 1;
        adxm_obj.mwidle_end = 0;
        stack               = (void*)((((unsigned int)tprm->stack_mwidle + 3) & ~3) + tprm->stack_size_mwidle);
        OS_CreateThread(&adxm_obj.mwidle_thread, adxm_mwidle_proc, NULL, stack, tprm->stack_size_mwidle,
                        adxm_save_tprm.prio_mwidle);
    } else {
        adxm_obj.mwidle_act = 0;
        adxm_obj.mwidle_end = 1;
    }
}

int ADXM_IsSetupThrd(void) {
    return adxm_get_init_level() != 0;
}

int adxm_get_init_level(void) {
    return adxm_init_level;
}

void adxm_inc_init_level(void) {
    adxm_init_level++;
}

int adxm_is_vsync_end(void) {
    return adxm_obj.vsync_end;
}

void adxm_set_vsync_end(void) {
    adxm_obj.vsync_end = 1;
}

void adxm_clear_vsync_end(void) {
    adxm_obj.vsync_end = 0;
}

int ADXM_ExecVint(void) {
    if (adxm_get_init_level() == 0) {
        return 0;
    }
    if (adxm_obj.framework == 0 || adxm_obj.framework == 1) {
        return 0;
    }
    if (adxm_is_vsync_end() != 0) {
        return 0;
    }
    if (adxm_cb.cb1_done == 0) {
        adxm_cb.cb1_done = 1;
        adxm_exec_cb1();
    }
    ADXMNG_CallVintServerFunctions();
    OS_ResumeThreadImmediate(&adxm_obj.vsync_thread);
    return 0;
}

void adxm_exec_cb0(void) {
    if (adxm_cb.cb0_func == NULL) {
        return;
    }
    adxm_cb.cb0_func(adxm_cb.cb0_obj);
}

void adxm_exec_cb1(void) {
    if (adxm_cb.cb1_func == NULL) {
        return;
    }
    adxm_cb.cb1_func(adxm_cb.cb1_obj);
}
