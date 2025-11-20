// #include "common.h"
// #include <cri/private/libadxe/structs.h> // This import shouldn't be necessary, but right now it is

// #include <cri/private/libadxe/adx_crs.h>
// #include <cri/private/libadxe/adx_errs.h>
// #include <cri/private/libadxe/adx_rnap.h>
// #include <cri/private/libadxe/adx_sjd.h>
// #include <cri/private/libadxe/adx_stmc.h>
// #include <cri/private/libadxe/adx_tlk.h>
// #include <cri/private/libadxe/dtx.h>
// #include <cri/private/libadxe/lsc.h>
// #include <cri/private/libadxe/lsc_err.h>
// #include <cri/private/libadxe/lsc_ini.h>
// #include <cri/private/libadxe/svm.h>

// #include <cri/cri_adxt.h>
// #include <cri/ee/cri_mw.h>

// #include <string.h>

#include <CriWare/private/adx_tlk.h>

#include <types.h>

extern s32 dtx_send_sw;

extern void func_02014644();

char* data_020638fc;

// data
char* volatile adxt_build       = "\nADXT/PS2EE Ver.9.00 Build:Sep 18 2003 10:00:00\n";
char adxt_obj_mark[16]          = {'M', 'A', 'R', 'K', ':', 'a', 'd', 'x', 't', '_', 'o', 'b', 'j'};
s32  adxt_init_cnt              = 0;
s32  adxt_svr_id                = 0;
s32  adxt_svr_main_id           = 0;
s32  adxt_vsync_svr_flag        = 1;
s32  adxt_svr_fs_id             = 0;
s32 volatile adxt_vsync_cnt     = 0;
ADX_TALK adxt_obj[ADXT_MAX_OBJ] = {0};

// forward decls
s32 adxt_exec_tsvr(void* object);
s32 adxt_exec_fssvr(void* object);

void ADXT_ConfigVsyncSvr(s32 flag) {
    adxt_vsync_svr_flag = flag;
}

void adxini_rnaerr_cbfn(void* object, char* msg) {
    ADXERR_CallErrFunc1(msg);
}

void adxini_lscerr_cbfn(void* object, char* msg) {
    ADXERR_CallErrFunc1(msg);
}

void ADXT_VsyncProc() {
    adxt_vsync_cnt += 1;
    ADXT_ExecServer();
}

s32 adxt_exec_main_thrd(void* object) {
    LSC_ExecServer();
    return 0;
}

s32 adxt_exec_main_nothrd(void* object) {
    adxt_exec_tsvr(NULL);
    adxt_exec_fssvr(NULL);
    adxt_exec_main_thrd(NULL);
    return 0;
}

s32 adxt_exec_tsvr(void* object) {
    ADXT_ExecServer();
    return 0;
}

s32 adxt_exec_fssvr(void* object) {
    ADXT_ExecFsSvr();
    return 0;
}

void ADXT_Init() {
    BOOL is_thread_setup;
    // adxt_build;

    data_020638fc = "\nADXT/NITRO Ver.10.36 Build:Sep 28 2007 13:14:01\n";

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

#if defined(TARGET_PS2)
        ADXRNA_Init();
#endif

        LSC_Init();
        SVM_Init();
        ADXRNA_EntryErrFunc(adxini_rnaerr_cbfn, NULL);
        LSC_EntryErrFunc(adxini_lscerr_cbfn, NULL);
        memset(adxt_obj, 0, sizeof(adxt_obj));

        func_0201a9fc(2, 1, func_02014644, 0, "adxt_exec_tsvr");

        is_thread_setup = ADXM_IsSetupThrd();

        if ((is_thread_setup == 1) && (adxt_vsync_svr_flag == 1)) {
            SVM_SetCbSvrId(2, 1, adxt_exec_tsvr, NULL);
            adxt_svr_fs_id   = SVM_SetCbSvr(4, adxt_exec_fssvr, NULL);
            adxt_svr_main_id = SVM_SetCbSvr(5, adxt_exec_main_thrd, NULL);
        } else {
            adxt_svr_main_id = SVM_SetCbSvr(5, adxt_exec_main_nothrd, NULL);
        }

        adxt_vsync_cnt = 0;
        ADXT_SetDefSvrFreq(60);
        ADXCRS_Unlock();
    }

    adxt_init_cnt += 1;
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