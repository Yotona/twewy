#include <cri/sj.h>

typedef struct SJCK_CHAIN {
    /* 0x00 */ struct SJCK_CHAIN* next;
    /* 0x04 */ int                rsv;
    /* 0x08 */ SJCK               ck;
} SJCK_CHAIN; // Size: 0x10

typedef struct {
    /* 0x00 */ SJ_OBJ      sj;
    /* 0x04 */ char        used;
    /* 0x05 */ char        mode;
    /* 0x06 */ short       rsv1;
    /* 0x08 */ const UUID* uuid;
    /* 0x0C */ SJCK_CHAIN* ckcnwk;
    /* 0x10 */ int         nckcn;
    /* 0x14 */ SJCK_CHAIN* pool;
    /* 0x18 */ SJCK_CHAIN* lin[4];
    /* 0x28 */ void (*errfunc)(void* obj, int ecode);
    /* 0x2C */ void* errobj;
} SJUNI_OBJ; // Size: 0x30

#define SJUNI_MAX_OBJ 40

static void sjuni_Init(void);
static void sjuni_Finish(void);

int       sjuni_init_cnt           = 0;
SJUNI_OBJ sjuni_obj[SJUNI_MAX_OBJ] = {0};

void SJUNI_Init(void) {
    SJCRS_Init();
    SJCRS_Lock();
    sjuni_Init();
    SJCRS_Unlock();
}

static void sjuni_Init(void) {
    if (sjuni_init_cnt == 0) {
        __builtin__clear(sjuni_obj, sizeof(sjuni_obj));
    }
    sjuni_init_cnt++;
}

void SJUNI_Finish(void) {
    SJCRS_Lock();
    sjuni_Finish();
    SJCRS_Unlock();
    SJCRS_Finish();
}

static void sjuni_Finish(void) {
    if (--sjuni_init_cnt == 0) {
        __builtin__clear(sjuni_obj, sizeof(sjuni_obj));
    }
}
