#include <types.h>

s32 adxt_fssvr_enter_cnt = 0;

void (*data_0206bd64)(s32) = NULL;
s32 data_0206bd68          = 0;
void (*data_0206bd6c)(s32) = NULL;
s32 data_0206bd78          = 0;

static void adxt_ExecFsSvr(void);

void ADXT_ExecFsSvr(void) {
    func_02012f88();
    adxt_ExecFsSvr();
    func_02012f8c();
}

static void adxt_ExecFsSvr(void) {
    ADXCRS_Lock();

    if (adxt_fssvr_enter_cnt != 0) {
        ADXCRS_Unlock();
        return;
    }

    adxt_fssvr_enter_cnt = 1;
    ADXCRS_Unlock();

    if (data_0206bd6c != NULL) {
        data_0206bd6c(data_0206bd68);
    }
    adxt_fssvr_enter_cnt = 3;
    func_02016868();
    adxt_fssvr_enter_cnt = 4;
    ADXSTM_ExecServer();
    adxt_fssvr_enter_cnt = 5;
    func_020215cc();
    adxt_fssvr_enter_cnt = 6;
    ADXSTM_ExecServer();
    adxt_fssvr_enter_cnt = 7;
    func_02016868();
    adxt_fssvr_enter_cnt = 9;
    func_02016888();
    adxt_fssvr_enter_cnt = 0;
    if (data_0206bd64 != NULL) {
        data_0206bd64(data_0206bd78);
    }
}