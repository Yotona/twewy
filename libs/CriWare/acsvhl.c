#include <CriWare/acssnd.h>
#include <CriWare/acssvr.h>
#include <CriWare/acsvhl.h>

ACSVHL* ACSVHL_Create(ACSVHL* vhl, u16 workSize) {
    if (vhl == NULL) {
        func_0201f55c("E2003091209 ACSVHL_Create : Work pointor is NULL.");
        return NULL;
    }
    if (workSize < sizeof(ACSVHL)) {
        func_0201f55c("E2003091210 ACSVHL_Create : Work size is too small.");
        return NULL;
    }
    func_02021488(vhl);
    return vhl;
}

void func_020211cc(ACSVHL* vhl) {
    if (vhl != NULL) {
        func_02021488(vhl);
    }
}

BOOL ACSVHL_Init(ACSVHL* vhl) {
    if (vhl->unk_04 != NULL) {
        func_0201f55c("E2003091600 ACSVHL_Init : Already initialized.");
        return FALSE;
    }
    vhl->unk_04 = ACSFDR_Create(&vhl->fdr, sizeof(ACSFDR));
    if (vhl->unk_04 == NULL) {
        func_0201f55c("E2003091601 ACSVHL_Init : Failed to initialize ACSFDR.");
        return FALSE;
    }
    func_020207d8(vhl->unk_04, 0, 0, 0, 0);
    return TRUE;
}

void func_02021260(ACSVHL* vhl) {
    if (vhl->unk_04 != NULL) {
        func_0202079c(vhl->unk_04);
        vhl->unk_04 = NULL;
    }
}

void func_02021284(ACSVHL* vhl) {
    vhl->unk_4C = 0;
}

s32 func_02021290(ACSVHL* vhl) {
    return vhl->unk_4C;
}

s8 func_02021298(ACSVHL* vhl) {
    return vhl->unk_50;
}

s32 func_020212a0(ACSVHL* vhl) {
    return func_020207b8(vhl->unk_04);
}

void func_020212b0(ACSVHL* vhl, s32 param_2) {
    vhl->snd = param_2;
}

void func_020212b8(ACSVHL* vhl, s32 param_2) {
    s32  idx;
    BOOL bVar1 = FALSE;

    for (idx = 0; idx < 4; idx++) {
        ACSSND* snd = vhl->snd[idx];
        if (snd != NULL && func_0201f9b4(snd) != 0 && func_0201fb58(snd) == 2) {
            switch (ACSSND_GetStat(snd)) {
                case 1:
                case 2:
                    if (ACSSND_GetStatPause(snd) == 0) {
                        bVar1 = TRUE;
                    }
                    break;
                case 3:
                case 4:
                    break;
            }
        }
    }

    switch (vhl->unk_50) {
        case 0:
            if (bVar1 == TRUE) {
                func_020207d8(vhl->unk_04, -vhl->unk_5C, vhl->unk_54, 0, 0);
                func_020214d0(vhl, 1);
            }
            break;
        case 1:
            if (bVar1 == FALSE) {
                func_020207d8(vhl->unk_04, 0, func_020214d8(vhl), 0, 0);
                func_020214d0(vhl, 3);
            } else {
                if (func_020207b0(vhl->unk_04) == 0) {
                    func_020214d0(vhl, 2);
                }
            }
            break;
        case 2:
            if (bVar1 == FALSE) {
                func_020207d8(vhl->unk_04, 0, vhl->unk_58, 0, 0);
                func_020214d0(vhl, 3);
            }
            break;
        case 3:
            if (bVar1 == TRUE) {
                func_020207d8(vhl->unk_04, -vhl->unk_5C, func_02021504(vhl), 0, 0);
                func_020214d0(vhl, 1);
            } else {
                if (func_020207b0(vhl->unk_04) == 0) {
                    func_020214d0(vhl, 0);
                }
            }
            break;
    }
    func_02020874(vhl->unk_04);
}

void func_02021488(ACSVHL* vhl) {
    vhl->snd    = NULL;
    vhl->unk_04 = NULL;
    memset(&vhl->fdr, 0, sizeof(ACSFDR));
    vhl->unk_4C = 0;
    vhl->unk_50 = 0;
    vhl->unk_54 = 30;
    vhl->unk_58 = 60;
    vhl->unk_5C = 120;
}

void func_020214d0(ACSVHL* vhl, s8 param_2) {
    vhl->unk_50 = param_2;
}

s32 func_020214d8(ACSVHL* vhl) {
    s32 iVar2 = vhl->unk_54;
    s32 uVar3 = _s32_div_f(iVar2 * func_020207b8(vhl->unk_04), -vhl->unk_5C);
    return iVar2 - uVar3;
}

s32 func_02021504(ACSVHL* vhl) {
    s32 iVar1 = func_020207b8(vhl->unk_04);
    s32 uVar2 = _s32_div_f(vhl->unk_58 * iVar1, -vhl->unk_5C);
    return vhl->unk_54 - uVar2;
}
