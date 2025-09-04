#include <CriWare/adxsjd.h>

BOOL    data_0206c0c0;
ADXSJD* data_0206c0c8;

void func_0201575c(ADXSJD* sjd);
void func_0201562c(ADXSJD* sjd);

s32 func_02015aa4(ADXSJD* sjd);

void ADXSJD_Init(void) {}

void ADXSJD_Finish(void) {}

void ADXSJD_Clear(ADXSJD* sjd) {
    sjd->unk_A0 = 0;
    sjd->unk_2C = 0;
    sjd->unk_30 = 0;
    sjd->unk_34 = 0;
    sjd->unk_38 = ~0x80000000;
    sjd->unk_3C = -1;
    sjd->unk_40 = 0;
    sjd->unk_44 = 0;
    sjd->unk_03 = 0;
    sjd->unk_A8 = 0;
    sjd->unk_AC = 0;
}

ADXSJD* ADXSJD_Create() {}

void ADXSJD_Destroy(ADXSJD* sjd) {
    void* puVar1;

    if (sjd == NULL) {
        return;
    }
    puVar1 = sjd->unk_04;
    if (puVar1 != NULL) {
        sjd->unk_04 = NULL;
        func_02012408(puVar1);
    }
    ADXCRS_Lock();
    func_02049118(sjd, 0, sizeof(ADXSJD));
    ADXCRS_Unlock();
}

s8 func_02014b20(ADXSJD* sjd) {
    return sjd->unk_01;
}

void func_02014b28(ADXSJD* sjd, SJ* sj) {}

void func_02014b3c() {}

void func_02014b50() {}

void func_02014b60(ADXSJD* sjd) {
    ADXSJD_Clear(sjd);
    sjd->unk_01 = 1;
}

void func_02014b78(ADXSJD* sjd) {
    func_02012ac8(sjd->unk_04);
    sjd->unk_01 = 0;
}

void func_02014b94(ADXSJD* sjd) {}

void func_02014e74() {}

void func_02014f44() {}

void func_02015344() {}

void func_020154f0(ADXSJD* sjd) {
    void* pvVar4 = sjd->unk_04;
    s32   iVar1  = func_020128a8(pvVar4);
    s32   iVar2  = func_02012b10(pvVar4);
    s32   iVar3  = func_02012b18(pvVar4);

    iVar1 -= sjd->unk_34;

    if (iVar3 >= iVar1) {
        iVar3 = iVar1;
    }
    sjd->unk_2C += iVar3;
    sjd->unk_30 += iVar2;
    sjd->unk_34 += iVar3;
}

void func_02015554(ADXSJD* sjd) {}

void func_020155c0(ADXSJD* sjd) {
    if (0 < sjd->unk_A8) {
        ADXCRS_Lock();
        func_0201562c(sjd);
        ADXCRS_Unlock();
    }
    if (sjd->unk_01 == 2) {
        func_02015554(sjd);
    } else if (sjd->unk_01 == 1) {
        func_02014b94(sjd);
    }
    if (sjd->unk_AC > 0) {
        ADXCRS_Lock();
        func_0201575c(sjd);
        ADXCRS_Unlock();
    }
}

void func_0201562c(ADXSJD* sjd) {}

void func_0201575c(ADXSJD* sjd) {}

void func_02015878() {}

u32 func_020158e4(ADXSJD* sjd) {
    return sjd->unk_30;
}

s32 func_020158ec(ADXSJD* sjd) {
    return sjd->unk_2C;
}

void func_020158f4(ADXSJD* sjd, s32 param_2) {
    sjd->unk_34 = param_2;
}

void func_020158fc(ADXSJD* sjd, s32 param_2) {
    sjd->unk_A4 = param_2;
}

void func_02015904(ADXSJD* sjd, s32 param_2, s32 param_3) {
    sjd->unk_48 = param_2;
    sjd->unk_4C = param_3;
}

void func_02015910(ADXSJD* sjd, s32 param_2) {
    sjd->unk_3C = param_2;
}

void func_02015918(ADXSJD* sjd, s32 param_2) {
    sjd->unk_40 = param_2;
}

void func_02015920(ADXSJD* sjd, s32 param_2) {
    sjd->unk_44 = param_2;
}

void func_02015928() {}

void func_02015938() {}

void func_02015948() {}

s32 func_02015958(ADXSJD* sjd) {}

void func_02015968() {}

void func_02015978() {}

void func_02015988() {}

void func_02015998() {}

s32 func_020159a8(ADXSJD* sjd) {
    if (sjd != NULL) {
        return func_020128c0(sjd->unk_04);
    }
    return 0;
}

void func_020159c4() {}

void func_020159d4() {}

void func_020159e4() {}

void func_02015a2c() {}

s32* func_02015a7c(ADXSJD* sjd) {
    return &sjd->unk_60;
}

void func_02015a84() {}

void func_02015a94() {}

s32 func_02015aa4(ADXSJD* sjd) {
    return sjd->unk_B0;
}
