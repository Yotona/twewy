#include <MSL/ARM/stdarg.h>
#include <MSL/Common/mem_funcs.h>
#include <types.h>

typedef struct {
    void (*func)(void*);
    void* object;
} SVMLockCallback;

typedef struct {
    void (*func)(void*, char*);
    void* object;
} SVMErrorCallback;

typedef struct {
    s32 (*func)(void*);
    void* object;
    char* tag;
} SVMSVRCallback;

typedef struct {
    void (*unk0)();
    s32 unk4;
} SVM_UNK;

// data
volatile s32 svm_init_level       = 0;
volatile s32 svm_lock_level       = 0;
volatile s32 svm_locking_type     = 0;
s32          svm_exec_cnt[8]      = {0};
s32          svm_svr_exec_flag[8] = {0};
char         svmerr_msg[128]      = {0};

// bss
s32 (*svm_test_and_set_callback)(s32*);
SVMErrorCallback svm_error_callback;
SVMLockCallback  svm_unlock_callback;
SVMLockCallback  svm_lock_callback;

SVM_UNK data_02070074;
SVM_UNK data_0207007c;

SVMSVRCallback svm_svr_callbacks[8][6];

void SVM_CallErr(const char* format, ...);
s32  svm_SetCbSvrId(s32 svtype, s32 (*func)(void*), void* object, char* tag);
void svm_DelCbSvr(s32 group, s32 id);
void func_0201aa38(s32 svtype, s32 id, s32 (*func)(void*), void* object, char* tag);

void svm_lock(s32 type) {
    if (svm_lock_callback.func == NULL) {
        return;
    }

    svm_lock_callback.func(svm_lock_callback.object);

    if (svm_lock_level == 0) {
        svm_locking_type = type;
    }

    svm_lock_level += 1;
}

void svm_unlock(s32 type) {
    if (svm_unlock_callback.func == NULL) {
        return;
    }

    svm_lock_level -= 1;

    if (svm_lock_level == 0) {
        if (svm_locking_type != type) {
            SVM_CallErr("2103102:SVM:svm_unlock:lock type miss match.(type org=%d, type now=%d)", svm_locking_type, type);
        }

        svm_locking_type = 0;
    }

    svm_unlock_callback.func(svm_unlock_callback.object);
}

void SVM_Lock(void) {
    svm_lock(1);
}

void SVM_Unlock(void) {
    svm_unlock(1);
}

void func_0201a798(void) {
    svm_lock(3);
}

void func_0201a7a8(void) {
    svm_unlock(3);
}

void SVM_CallErr(const char* format, ...) {
    va_list args;

    memset(svmerr_msg, 0, sizeof(svmerr_msg));
    va_start(args, format);
    func_02021684(&svmerr_msg, 0x80, format, args);

    if (svm_error_callback.func != NULL) {
        svm_error_callback.func(svm_error_callback.object, svmerr_msg);
    }
}

void SVM_CallErr1(const char* msg) {
    if (msg == NULL) {
        svm_error_callback.func(svm_error_callback.object, NULL);
        return;
    }

    func_02021624(svmerr_msg, sizeof(svmerr_msg), msg, sizeof(svmerr_msg) - 1);

    if (svm_error_callback.func != NULL) {
        svm_error_callback.func(svm_error_callback.object, svmerr_msg);
    }
}

s32 SVM_SetCbSvrId(s32 svtype, s32 id, s32 (*func)(void*), void* object) {
    SVM_Lock();
    s32 svrId = svm_SetCbSvrId(svtype, id, func, object);
    SVM_Unlock();
    return svrId;
}

s32 svm_SetCbSvrId(s32 svtype, s32 (*func)(void*), void* object, char* tag) {
    if (svtype < 0 || svtype >= 8) {
        SVM_CallErr1("1071205:SVM_SetCbSvrId:illegal svtype");
        return -1;
    }

    SVMSVRCallback* callback = svm_svr_callbacks[svtype];

    s32 i;
    for (i = 0; i < 6; i++) {
        if (callback->func == NULL) {
            callback->func   = func;
            callback->object = object;
            if (tag != NULL) {
                callback->tag = tag;
            } else {
                callback->tag = "Unknown";
            }
            break;
        }
        callback++;
    }

    if (i != 6) {
        return i;
    }

    SVM_CallErr1("1051001:SVM_SetCbSvr:too many server function");
    return -1;
}

void SVM_DelCbSvr(s32 group, s32 id) {
    SVM_Lock();
    svm_DelCbSvr(group, id);
    SVM_Unlock();
}

void svm_DelCbSvr(s32 svtype, s32 id) {
    if (id < 0 || id >= 6) {
        SVM_CallErr1("1051002:SVM_DelCbSvr:illegal id");
        return;
    }

    if (svtype < 0 || svtype >= 8) {
        SVM_CallErr1("1071206:SVM_SetCbSvrId:illegal svtype");
        return;
    }

    svm_svr_callbacks[svtype][id].tag    = NULL;
    svm_svr_callbacks[svtype][id].func   = NULL;
    svm_svr_callbacks[svtype][id].object = NULL;
}

void func_0201a9fc(s32 svtype, s32 id, s32 (*func)(void*), void* object, char* tag) {
    SVM_Lock();
    func_0201aa38(svtype, id, func, object, tag);
    SVM_Unlock();
}

void func_0201aa38(s32 svtype, s32 id, s32 (*func)(void*), void* object, char* tag) {
    if (id < 0 || id >= 6) {
        SVM_CallErr1("1071201:SVM_SetCbSvrId:illegal id");
        return;
    }

    if (svtype < 0 || svtype >= 8) {
        SVM_CallErr1("1071202:SVM_SetCbSvrId:illegal svtype");
        return;
    }

    SVMSVRCallback* pSVar1 = &svm_svr_callbacks[svtype][id];
    if (pSVar1->func != NULL) {
        SVM_CallErr1("2100801:SVM_SetCbSvrId:over write callback function.");
    }

    pSVar1->func   = func;
    pSVar1->object = object;
    if (tag != NULL) {
        pSVar1->tag = tag;
    } else {
        pSVar1->tag = "Unknown";
    }
}

void SVM_SetCbErr(void (*func)(void*, char*), void* object) {
    SVM_Lock();
    svm_error_callback.func   = func;
    svm_error_callback.object = object;
    SVM_Unlock();
}

void SVM_SetCbLock(void (*func)(void*), void* object) {
    svm_lock_callback.func   = func;
    svm_lock_callback.object = object;
}

void SVM_SetCbUnlock(void (*func)(void*), void* object) {
    svm_unlock_callback.func   = func;
    svm_unlock_callback.object = object;
}

s32 SVM_ExecSvrFunc(s32 svtype) {
    s32 i;
    s32 func_result;
    s32 result = 0;

    for (i = 0; i < 6; i++) {
        SVMSVRCallback* callback = &svm_svr_callbacks[svtype][i];
        void*           object   = callback->object;

        if (callback->func != NULL) {
            svm_svr_exec_flag[svtype] = 1;
            func_result               = callback->func(object);
            svm_svr_exec_flag[svtype] = 0;
            result |= func_result;
        }
    }

    svm_exec_cnt[svtype]++;
    return result;
}

s32 SVM_ExecSvrVint(void) {
    return SVM_ExecSvrFunc(0);
}

s32 SVM_ExecSvrUsrVsync(void) {
    return SVM_ExecSvrFunc(1);
}

s32 SVM_ExecSvrVsync(void) {
    return SVM_ExecSvrFunc(2);
}

s32 SVM_ExecSvrUhigh(void) {
    return SVM_ExecSvrFunc(3);
}

s32 SVM_ExecSvrFs(void) {
    return SVM_ExecSvrFunc(4);
}

s32 SVM_ExecSvrMain(void) {
    return SVM_ExecSvrFunc(5);
}

s32 SVM_ExecSvrMwIdle(void) {
    return SVM_ExecSvrFunc(6);
}

s32 SVM_ExecSvrUsrIdle(void) {
    return SVM_ExecSvrFunc(7);
}

void svm_reset_variable(void) {
    __builtin__clear(svm_svr_exec_flag, sizeof(svm_svr_exec_flag));
    __builtin__clear(&svm_lock_callback, sizeof(svm_lock_callback));
    __builtin__clear(&svm_unlock_callback, sizeof(svm_unlock_callback));
    __builtin__clear(&data_0207007c, sizeof(data_0207007c));
    __builtin__clear(&data_02070074, sizeof(data_02070074));

    for (s32 i = 0; i < 6; i++) {
        svm_exec_cnt[i] = 0;
    }

    svm_test_and_set_callback = NULL;
}

void SVM_Init(void) {
    if (svm_init_level == 0) {
        svm_reset_variable();
    }

    svm_init_level++;
}

void SVM_Finish() {
    svm_init_level--;

    if (svm_init_level == 0) {
        svm_reset_variable();
        svm_error_callback.func   = NULL;
        svm_error_callback.object = NULL;
    }
}

s32 SVM_TestAndSet(s32* mem) {
    s32 result;

    if (svm_test_and_set_callback != NULL) {
        result = svm_test_and_set_callback(mem);
    } else {
        SVM_Lock();
        result = *mem != 1;
        *mem   = 1;
        SVM_Unlock();
    }

    return result;
}
