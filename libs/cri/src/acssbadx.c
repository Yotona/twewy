#include <cri/acssbadx.h>

const _acssbadx_vtable ACSSBADX_VTable = {
    ACSSBADX_CreateHndl, ACSSBADX_DestroyAdxt, func_02021004,      ACSSBADX_StartFnameRange, ACSSBADX_StartAfs,
    ACSSBADX_StartMem2,  ACSSBADX_StartMemIdx, ACSSBADX_Stop,      ACSSBADX_Pause,           ACSSBADX_GetStatPause,
    ACSSBADX_GetStat,    ACSSBADX_SetOutVol,   ACSSBADX_GetOutVol, ACSSBADX_SetOutVol2,      ACSSBADX_GetOutVol2,
    ACSSBADX_SetLpFlg,   ACSSBADX_GetAdxt,
};

ACSSBADX* ACSSBADX_Create(ACSSBADX* badx, unsigned short workSize) {
    if (badx == NULL) {
        func_0201f55c("E2003052800 ACSSBADX_Create : Work pointor is NULL.");
        return NULL;
    }
    if (workSize < sizeof(ACSSBADX)) {
        func_0201f55c("E2003091200 ACSSBADX_Create : Work size is too small.");
        return NULL;
    }
    func_02021168(badx);
    return badx;
}

void func_02020f64(ACSSBADX* badx) {
    if (badx) {
        func_02021168(badx);
    }
}

int ACSSBADX_CreateHndl(ACSSBADX* badx, int* param_2, void* work, int workSize) {
    if (param_2[2] != 0) {
        badx->adxt = func_02016c64(work, workSize - sizeof(ACSSBADX));
    } else {
        badx->adxt = ADXT_Create(*param_2, work, workSize - sizeof(ACSSBADX));
    }

    if (badx->adxt == NULL) {
        func_0201f55c("E2003052802 ACSSBADX_CreateHndl : Failed to create handle.");
        return 0;
    }

    badx->unk_8 = *param_2;
    return 1;
}

void ACSSBADX_DestroyAdxt(ACSSBADX* badx) {
    if (badx->adxt != NULL) {
        ADXT_Destroy(badx->adxt);
        badx->adxt = NULL;
    }
}

void func_02021004(ACSSBADX* badx, const char* filename) {
    func_02017d80(badx->adxt, filename);
}

void ACSSBADX_StartFnameRange(ACSSBADX* badx) {
    ADXT_StartFnameRange(badx->adxt);
}

void ACSSBADX_StartAfs(ACSSBADX* badx, int param_1, int param_2) {
    ADXT_StartAfs(badx->adxt, param_1, param_2);
}

void ACSSBADX_StartMem2(ACSSBADX* badx, int param_1, int param_2) {
    ADXT_StartMem2(badx->adxt, param_1, param_2);
}

void ACSSBADX_StartMemIdx(ACSSBADX* badx, int param_1, int param_2) {
    ADXT_StartMemIdx(badx->adxt, param_1, param_2);
}

void ACSSBADX_Stop(ACSSBADX* badx) {
    ADXT_Stop(badx->adxt);
}

void ACSSBADX_Pause(ACSSBADX* badx) {
    ADXT_Pause(badx->adxt);
}

int ACSSBADX_GetStatPause(ACSSBADX* badx) {
    return ADXT_GetStatPause(badx->adxt);
}

int ACSSBADX_GetStat(ACSSBADX* badx) {
    switch (ADXT_GetStat(badx->adxt)) {
        case ADXT_STAT_STOPPED:
            return ACSSBADX_STAT_STOPPED;
        case ADXT_STAT_LOADING:
        case ADXT_STAT_PREPPING:
            return ACSSBADX_STAT_LOADING;
        case ADXT_STAT_PLAYING:
        case ADXT_STAT_DECEND:
            return ACSSBADX_STAT_PLAYING;
        case ADXT_STAT_PLAYEND:
            return ACSSBADX_STAT_ENDING;
        case ADXT_STAT_ERROR:
            return ACSSBADX_STAT_ERROR;
        default:
            return ACSSBADX_STAT_ERROR;
    }
}

void ACSSBADX_SetOutVol(ACSSBADX* badx, short vol) {
    if (vol < -960) {
        vol = -960;
    }
    ADXT_SetOutVol(badx->adxt, vol);
}

int ACSSBADX_GetOutVol(ACSSBADX* badx) {
    return ADXT_GetOutVol(badx->adxt);
}

void ACSSBADX_SetOutVol2(ACSSBADX* badx, int param_1, short vol) {
    if (param_1 == 0) {
        ADXT_SetOutVol(badx->adxt, vol);
    }
}

int ACSSBADX_GetOutVol2(ACSSBADX* badx, int param_1) {
    if (param_1 == 0) {
        return ADXT_GetOutVol(badx->adxt);
    }
    return 0;
}

void ACSSBADX_SetLpFlg(ACSSBADX* badx, int flag) {
    ADXT_SetLpFlg(badx->adxt, flag);
}

ADXT ACSSBADX_GetAdxt(ACSSBADX* badx) {
    return badx->adxt;
}

void func_02021168(ACSSBADX* badx) {
    badx->vtable = &ACSSBADX_VTable;
    badx->adxt   = NULL;
    badx->unk_8  = 0;
}
