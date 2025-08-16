#include <acssvr.h>

BOOL ACSSVR_Init(ACSSVR* svr) {
    if (func_0201f520(svr) == 0 && svr->unk_78 == 0) {
        func_0201f4e8(svr);
        svr->unk_78 = 1;
        func_02014664();
        func_0201a9fc(2, 4, func_0201f534, svr, "adxcs_exec_svr");
        svr->vhl = ACSVHL_Create(&svr->unk_14, 0x60);
        if (ACSVHL_Init(svr->vhl) == FALSE) {
            func_0201f55c("E2003053001 ACSSVR_Init : Failed to initialize VHL.");
            ACSSVR_Finish(svr);
            return FALSE;
        }
        func_020212b0(svr->vhl, svr);
        svr->unk_78 = 0;
    }
    svr->unk_74 += 1;
    return TRUE;
}

void ACSSVR_Finish(ACSSVR* svr) {
    s32 iVar1;

    if (svr->unk_78 == 0) {
        svr->unk_74--;
    }

    if (svr->unk_74 < 0) {
        func_0201f55c("E2003090800 ACSSVR_Finish : Too many Finish was called.");
        return;
    }

    if (svr->unk_74 == 0 || svr->unk_78 == 1) {
        for (iVar1 = 0; iVar1 < 4; iVar1++) {
            func_0201f18c(svr, (ACSSND*)(&svr->snd)[iVar1]);
        }

        if (svr->vhl != NULL) {
            func_02021260(svr->vhl);
            func_020211cc(svr->vhl);
            svr->vhl = NULL;
        }
        func_0201a94c(2, 4);
        func_0201a94c(5, svr->unk_7C);
        func_020147ac();
        svr->unk_78 = 0;
    }
}

// Nonmatching
ACSSND* ACSSVR_CreatSnd(ACSSVR* svr, s32* param_2, ACSSND* work, u32 workSize) {
    s32     sndCount;
    ACSSND* snd = NULL;

    if (func_0201f520(svr) == FALSE) {
        func_0201f55c("E2003053002 ACSSVR_CreatSnd : Not initialized.");
        return NULL;
    }
    for (sndCount = 0; sndCount < 4; sndCount++) {
        if (svr->snd[sndCount] == NULL) {
            svr->snd[sndCount] = snd = ACSSND_Create(work, workSize);
            work++;
            workSize -= sizeof(ACSSND);
            break;
        }
    }
    if (sndCount == 4) {
        func_0201f55c("E2003053003 ACSSVR_CreatSnd : Can\'t create sound any more.");
        func_0201f18c(svr, snd);
        return NULL;
    }
    if (ACSSND_Init(snd, param_2, work, workSize) != 0) {
        return snd;
    }
    func_0201f55c("E2003053004 ACSSVR_CreatSnd : Failed to create sound.");
    func_0201f18c(svr, snd);
    return NULL;
}

void func_0201f18c(ACSSVR* svr, ACSSND* snd) {
    s32 iVar1;

    if (snd == NULL) {
        return;
    }

    func_0201f930(snd);

    for (iVar1 = 0; iVar1 < 4; iVar1++) {
        if ((ACSSND*)(&svr->snd)[iVar1] == snd) {
            func_0201f5b0((ACSSND*)(&svr->snd)[iVar1]);
            (&svr->snd)[iVar1] = NULL;
            return;
        }
    }
}

void func_0201f1d4() {
    /* Not yet implemented */
}

void func_0201f3e4(ACSSVR* svr) {
    s32 idx;

    if (func_0201f520(svr) == FALSE) {
        return;
    }

    if (svr->unk_80 != NULL) {
        svr->unk_80(svr->unk_84);
    }

    if (func_02021290(svr->vhl) != 0) {
        func_020212b8(svr->vhl);
    }

    for (idx = 0; idx < 4; idx++) {
        if ((ACSSND*)(&svr->snd)[idx] != NULL) {
            func_0201f460(svr, (ACSSND*)(&svr->snd)[idx]);
        }
    }

    if (svr->unk_88 != NULL) {
        svr->unk_88(svr->unk_8C);
    }
}

void func_0201f460(ACSSVR* svr, ACSSND* snd) {
    if (func_0201f520(svr) == FALSE) {
        return;
    }

    if (func_0201f9b4(snd) == 0) {
        return;
    }

    func_0201fd5c(snd);

    if (func_02021290(svr->vhl) != 0 && func_02021298(svr->vhl) != 0 && func_0201fb58(snd) == 1) {
        func_0201fd44(snd, func_020212a0(svr->vhl));
    }
    ACSSND_ApplyAllVol(snd);
}

ACSVHL* func_0201f4e0(ACSSVR* svr) {
    return svr->vhl;
}

void func_0201f4e8(ACSSVR* svr) {
    s32 idx;

    for (idx = 0; idx < 4; idx++) {
        (&svr->snd)[idx] = NULL;
    }

    svr->unk_74 = 0;
    svr->unk_78 = 0;
    svr->unk_7C = 0;
    svr->unk_80 = 0;
    svr->unk_84 = 0;
    svr->unk_88 = 0;
    svr->unk_8C = 0;
}

BOOL func_0201f520(ACSSVR* svr) {
    return 0 < svr->unk_74;
}

s32 func_0201f534(ACSSVR* svr) {
    if (svr != NULL) {
        func_0201ef40();
        func_0201f3e4(svr);
        func_0201ef44();
        return 0;
    }
    return 0;
}

void func_0201f55c(const char* error) {
    func_0201433c(error);
}
