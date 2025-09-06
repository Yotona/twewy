#include <CriWare/acsfdr.h>

void func_020209a8(ACSFDR* fdr);
s32  func_02020a40();
s32  func_02020cb8();
s32  func_02020a88(ACSFDR* fdr, s32 param_2);
void func_02020ec4();
void func_02020ef0();

ACSFDR* ACSFDR_Create(ACSFDR* fdr, u16 workSize) {
    if (fdr == NULL) {
        func_0201f55c("E2003052803 ACSFDR_Create : Work pointor is NULL.");
        return NULL;
    }
    if (workSize < sizeof(ACSFDR)) {
        func_0201f55c("E2003091202 ACSFDR_Create : Work size is too small.");
        return NULL;
    }
    func_020209a8(fdr);
    return fdr;
}

void func_0202079c(ACSFDR* fdr) {
    if (fdr != NULL) {
        func_020209a8(fdr);
    }
}

s8 func_020207b0(ACSFDR* fdr) {
    return fdr->unk_00;
}

s32 func_020207b8(ACSFDR* fdr) {
    return fdr->unk_1C;
}

void func_020207c0(ACSFDR* fdr, s32 param_2) {
    fdr->unk_1C = param_2;
    fdr->unk_20 = func_02020cb8(fdr);
}

void func_020207d8(ACSFDR* fdr, s32 param_2, s32 param_3, s32 param_4, s32 param_5) {
    fdr->unk_28 = fdr->unk_24;
    fdr->unk_24 = param_2;

    fdr->unk_04 = func_02020a40(fdr, fdr->unk_1C, param_2, param_3);

    fdr->unk_10 = param_4;
    fdr->unk_14 = param_5;
    fdr->unk_08 = 0;
    fdr->unk_0C = 0;

    if ((param_3 <= 0) && (param_4 <= 0)) {
        fdr->unk_00 = 2;
        func_020207c0(fdr, param_2);
    } else {
        fdr->unk_00 = 1;
    }
}

void func_02020850(ACSFDR* fdr) {
    func_020207c0(fdr, fdr->unk_24);
    fdr->unk_00 = 0;
    func_02020ef0(fdr);
}

void func_02020874(ACSFDR* fdr) {
    s8 sVar1 = func_020207b0(fdr);

    if (sVar1 == 0) {
        return;
    }

    fdr->unk_08++;

    if (fdr->unk_08 < fdr->unk_10) {
        return;
    }

    if (sVar1 == 1) {
        sVar1       = 2;
        fdr->unk_00 = 2;
        func_02020ec4(fdr);
    }

    if (sVar1 == 2) {
        fdr->unk_20 += fdr->unk_04;

        fdr->unk_20 = fdr->unk_20 <= 0 ? 0 : fdr->unk_20;
        fdr->unk_20 = fdr->unk_20 >= 3238200 ? 3238200 : fdr->unk_20;

        fdr->unk_1C = func_02020a88(fdr, fdr->unk_20);
        if (((0 < fdr->unk_04 && fdr->unk_1C >= fdr->unk_24) || (fdr->unk_04 < 0 && fdr->unk_1C <= fdr->unk_24)) ||
            (fdr->unk_04 == 0))
        {
            func_020207c0(fdr, fdr->unk_24);
            sVar1       = 3;
            fdr->unk_0C = fdr->unk_08;
            fdr->unk_00 = 3;
        }
    }
    if (sVar1 != 3) {
        return;
    }
    if (fdr->unk_08 - fdr->unk_0C < fdr->unk_14) {
        return;
    }
    fdr->unk_00 = 0;
    func_02020ef0(fdr);
}

void func_02020988(ACSFDR* fdr, s32 param_2, s32 param_3, s32 param_4) {
    *((s32*)fdr->unk_2C) = param_2;
    *((s32*)fdr->unk_30) = param_3;
    *((s32*)fdr->unk_34) = param_4;
}

void func_02020998(ACSFDR* fdr, s32 param_2, s32 param_3, s32 param_4) {
    *((s32*)fdr->unk_38) = param_2;
    *((s32*)fdr->unk_3C) = param_3;
    *((s32*)fdr->unk_40) = param_4;
}

void func_020209a8(ACSFDR* fdr) {
    fdr->unk_00    = 0;
    fdr->unk_04    = 0;
    fdr->unk_08    = 0;
    fdr->unk_0C    = 0;
    fdr->unk_10    = 0;
    fdr->unk_14    = 0;
    fdr->unk_18    = 0;
    fdr->unk_1C    = -960;
    fdr->unk_20    = 0;
    fdr->unk_24    = -960;
    fdr->unk_28    = -960;
    fdr->unk_2C[0] = 0;
    fdr->unk_2C[1] = 0;
    fdr->unk_2C[2] = 0;
    fdr->unk_2C[3] = 0;
    fdr->unk_30[0] = 0;
    fdr->unk_30[1] = 0;
    fdr->unk_30[2] = 0;
    fdr->unk_30[3] = 0;
    fdr->unk_34[0] = 0;
    fdr->unk_34[1] = 0;
    fdr->unk_34[2] = 0;
    fdr->unk_34[3] = 0;
    fdr->unk_38[0] = 0;
    fdr->unk_38[1] = 0;
    fdr->unk_38[2] = 0;
    fdr->unk_38[3] = 0;
    fdr->unk_3C[0] = 0;
    fdr->unk_3C[1] = 0;
    fdr->unk_3C[2] = 0;
    fdr->unk_3C[3] = 0;
    fdr->unk_40[0] = 0;
    fdr->unk_40[1] = 0;
    fdr->unk_40[2] = 0;
    fdr->unk_40[3] = 0;
}

s32 func_02020a40() {}

s32 func_02020a88(ACSFDR* fdr, s32 param_2) {
    f32 uVar1;
    f32 fVar2;
    f32 fVar3;
    s64 lVar6;
    u64 uVar7;
    s64 uVar8;

    if (param_2 == 0) {
        return -960;
    }

    if (param_2 != 3238200) {
        switch (fdr->unk_18) {
            case 0:
            default:
                lVar6 = (f32)param_2 / 3238200;
                uVar7 = func_0205308c(lVar6);
                uVar7 = func_02054bfc(0, 3.640625f, uVar7);
                fVar2 = func_0205494c(uVar7);
                break;
            case 1:
                uVar1 = 960.0f * ((f32)param_2 / 3238200);
                fVar2 = func_020554c8(-960.0f, uVar1);
                break;
            case 2:
                lVar6 = (1.5707964f * param_2) / 3238200;
                uVar1 = func_02052f00(lVar6);
                uVar1 = func_0205494c(uVar1);
                uVar1 = 3238200 * uVar1;
                uVar1 /= 3238200;
                lVar6 = uVar1;
                uVar7 = func_0205308c(lVar6);
                uVar7 = func_02054bfc(0, 3.640625f, uVar7);
                fVar2 = func_0205494c(uVar7);
                break;
            case 3:
                lVar6 = (1.5707964f * param_2) / 3238200;
                uVar1 = func_020528a8(lVar6);
                uVar1 = func_0205494c(uVar1);
                uVar1 = func_02056264(1, uVar1);
                uVar1 = 3238200 * uVar1;
                uVar1 /= 3238200;
                lVar6 = uVar1;
                uVar7 = func_0205308c(lVar6);
                uVar7 = func_02054bfc(0, 3.640625f, uVar7);
                fVar2 = (f32)func_0205494c(uVar7);
                break;
            case 4:
                uVar7 = func_0205308c(0, 4.4375f);
                fVar3 = (f32)func_0205494c(uVar7);
                uVar1 = (f32)(3238200 - param_2);
                uVar1 = fVar3 * uVar1;
                uVar1 /= 3238200;
                lVar6 = uVar1;
                uVar8 = func_02053098(0, 2.5625f, lVar6);
                uVar1 = func_0205494c(uVar8);
                fVar2 = (f32)func_02056264(0, uVar1);
        }
        func_02055a74(fVar2, -960.0f);
        if (!((3 < fdr->unk_18) || (fdr->unk_18 == 4))) {
            fVar2 = -960.f;
        }
        func_02055ad0(fVar2, 0.0f);
        if (3 < fdr->unk_18) {
            fVar2 = 0.0f;
        }
        return fVar2;
    }
    return 0;
}

s32 func_02020cb8() {}

void func_02020ec4() {}

void func_02020ef0() {}
