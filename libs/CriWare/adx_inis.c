#include <CriWare/lsc.h>
#include <CriWare/private/adx_tlk.h>

extern s32 dtx_send_sw;

const char data_0205bf94[] = "\nADXT/NITRO Ver.10.36 Build:Sep 28 2007 13:14:01\n\0Append: MW4020\n";

// data
char* volatile adxt_build       = "";
s32 adxt_init_cnt               = 0;
s32 adxt_svr_main_id            = 0;
s32 adxt_svr_fs_id              = 0;
s32 volatile adxt_vsync_cnt     = 0;
ADX_TALK adxt_obj[ADXT_MAX_OBJ] = {0};

s32 data_0206bd7c = 0;

void (*data_0206c0a4)(s32) = NULL;
s32 data_0206c0a8          = 0;
void (*data_0206c0ac)(s32) = NULL;
s32 data_0206c0b0          = 0;

// forward decls
s32  adxt_exec_tsvr(void* object);
s32  adxt_exec_fssvr(void* object);
void func_0201489c(void);
void func_02014888(void);

const char* ADXT_GetVersionInfo() {
    return data_0205bf94;
}

void adxini_rnaerr_cbfn(void* object, char* msg) {
    ADXERR_CallErrFunc1(msg);
}

void adxini_lscerr_cbfn(void* object, char* msg) {
    ADXERR_CallErrFunc1(msg);
}

s32 adxt_exec_main_thrd(void* object) {
    func_02014888();
    return 0;
}

s32 adxt_exec_tsvr(void* object) {
    ADXT_ExecServer();
    return 0;
}

s32 adxt_exec_fssvr(void* object) {
    func_02014528();
    return 0;
}

void ADXT_Init() {
    adxt_build = data_0205bf94;

    func_020215fc();

    if (adxt_init_cnt == 0) {
        ADXCRS_Init();
        ADXCRS_Lock();
        SJUNI_Init();
        SJRBF_Init();
        SJMEM_Init();
        ADXERR_Init();
        ADXSTM_Init();
        ADXSJD_Init();
        ADXF_Init();
        ADXRNA_Init();
        LSC_Init();
        SVM_Init();
        ADXRNA_EntryErrFunc(adxini_rnaerr_cbfn, NULL);
        LSC_EntryErrFunc(adxini_lscerr_cbfn, NULL);

        __builtin__clear(adxt_obj, sizeof(adxt_obj));

        func_0201a9fc(2, 1, adxt_exec_tsvr, 0, "adxt_exec_tsvr");

        adxt_svr_fs_id   = SVM_SetCbSvrId(4, adxt_exec_fssvr, NULL, "adxt_exec_fssvr");
        adxt_svr_main_id = SVM_SetCbSvrId(5, adxt_exec_main_thrd, NULL, "adxt_exec_main_thrd");

        adxt_vsync_cnt = 0;
        data_0206bd7c  = 0;
        ADXT_SetDefSvrFreq(60);
        ADXCRS_Unlock();
    }

    adxt_init_cnt++;
}

void ADXT_Finish() {

    if (adxt_init_cnt <= 0) {
        SVM_CallErr1("E2006100201: ADXT_Finish function has been executed before initialization.");
        return;
    }

    if (--adxt_init_cnt == 0) {
        ADXT_DestroyAll();
        ADXRNA_Finish();
        ADXF_Finish();
        ADXSTM_Finish();
        LSC_Finish();
        ADXCRS_Lock();
        SVM_DelCbSvr(2, 1);
        SVM_DelCbSvr(4, adxt_svr_fs_id);
        SVM_DelCbSvr(5, adxt_svr_main_id);
        SVM_Finish();
        ADXSJD_Finish();
        ADXERR_Finish();
        SJMEM_Finish();
        SJRBF_Finish();
        SJUNI_Finish();
        ADXCRS_Unlock();
        func_02012f48();

        for (s32 i = 0; i < ADXT_MAX_OBJ; i++) {
            if (adxt_obj[i].used != '\0') {
                SVM_CallErr1("E2005041230: ADXT handle is still active.");
                ADXT_Destroy(&adxt_obj[i]);
            }
        }
    }
}

void func_02014888(void) {
    func_02012f88();
    func_0201489c();
    func_02012f8c();
}

void func_0201489c(void) {
    if (data_0206c0ac != NULL) {
        data_0206c0ac(data_0206c0a8);
    }
    LSC_ExecServer();
    if (data_0206c0a4 != NULL) {
        data_0206c0a4(data_0206c0b0);
    }
}