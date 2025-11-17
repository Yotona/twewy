#include "DatMgr.h"

DatMgr* data_0206b3c0;

Data* func_02008054(DatMgr* mgr) {
    Data* temp_r2 = mgr->unk_208;

    if (temp_r2 != NULL) {
        mgr->unk_208 = (Data*)temp_r2->unk_00;
    }
    return temp_r2;
}

void func_0200806c(DatMgr* mgr, s32 idx, Data* data) {
    if (data != NULL) {
        data->unk_00             = mgr->unk_028[idx].unk_00;
        mgr->unk_028[idx].unk_00 = data;
    }
}

void func_02008090(DatMgr* datMgr, Data* data) {
    if (data == NULL) {
        return;
    }

    data->unk_04 &= ~0xF;
    data->unk_04 &= ~0xF0;
    data->unk_04 &= ~0x100;
    data->unk_04 &= ~0x200;
    data->unk_04 &= ~0x400;
    data->unk_04  = (u16)data->unk_04;
    data->unk_08  = 0;
    data->unk_0C  = 0;
    data->unk_10  = 7;
    data->unk_14  = 0;
    data->unk_18  = 0;
    data->bin     = 0;
    data->pac     = 0;
    data->unk_024 = 0;
}

// Nonmatching
void func_020080ec(DatMgr* mgr, Data* data) {
    u32    index = data->unk_04 & 0xf;
    Data** prev  = &mgr->unk_028[index].unk_00;
    while (*prev != NULL) {
        if (*prev == data) {
            *prev = data->unk_00;
            return;
        }
        prev = &(*prev)->unk_00;
    }
}

void func_02008138(DatMgr* mgr, Data* data) {
    if (data != NULL) {
        mgr->unk_238->unk_00 = data;
        mgr->unk_238         = data;
        data->unk_00         = NULL;
    }
}

s32 DatMgr_Init(s32 arg0, s32 arg1) {}

void* func_0200823c(s32 arg0, u32 arg1, u32 arg2, BinIdentifier* arg3) {}

void* func_0200838c(s32 arg0, u32 arg1, u32 arg2, u32 arg3, u32 arg4) {}

void* func_020084f8(s32 arg0, Bin* arg1, s32 arg2, s32 arg3) {}

void* func_02008650(s32 arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4, s32 arg5) {}

Data* func_02008814(u32 idx, u32 resourceId) {
    Data* pDVar1;
    Data* pGVar1;
    Bin*  pBVar2;
    u32   uVar3;

    DatMgr* datMgr = data_0206b3c0;
    if (datMgr == NULL) {
        return NULL;
    }

    for (pDVar1 = datMgr->unk_028[idx].unk_00; pDVar1 != NULL; pDVar1 = pDVar1->unk_00) {
        uVar3 = (pDVar1->unk_04 << 0x18) >> 0x1c;
        if (uVar3 == 3) {
            uVar3 = pDVar1->unk_14;
            if (uVar3 == resourceId) {
                break;
            }
        }
    }

    if (pDVar1 != NULL) {
        uVar3 = pDVar1->unk_04 >> 0x10;
        if (uVar3 == 0xffff) {
            return pDVar1;
        }
        pDVar1->unk_04 = (uVar3 + 1) * 0x10000 | pDVar1->unk_04 & 0xffff;
        return pDVar1;
    }

    pGVar1 = func_02008054(datMgr);
    func_02008090(datMgr, pGVar1);

    pGVar1->unk_04 &= ~0xF;
    pGVar1->unk_04 |= idx & 0xF;
    pGVar1->unk_04 &= ~0xF0;
    pGVar1->unk_04 |= 0x30;
    pGVar1->unk_04 |= 0x400;
    pGVar1->unk_04 = (u16)pGVar1->unk_04;
    pGVar1->unk_04 |= 0x10000;

    pGVar1->unk_14 = resourceId;
    pBVar2         = BinMgr_LoadUncompressed(NULL, resourceId);
    pGVar1->bin    = pBVar2;
    pGVar1->unk_08 = pBVar2->data;
    pGVar1->unk_0C = pGVar1->bin->size;
    func_0200806c(datMgr, idx, pGVar1);
    return pGVar1;
}

void* func_0200892c(s32 arg0, u32 arg1) {}

void* func_02008a44(s32 arg0, u32 arg1, s32 arg2, s32 arg3) {}

s32 func_02008c10(s32* arg1) {}

void* func_02008c50(s32 arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4) {}

BOOL func_02008dbc(Data* data) {
    DatMgr* local_datMgr = data_0206b3c0;
    BOOL    ret          = FALSE;

    if (local_datMgr == NULL) {
        return ret;
    }

    if (data == NULL) {
        return ret;
    }

    u32 uVar1    = (data->unk_04 >> 0x10) - 1;
    data->unk_04 = uVar1 * 0x10000 | data->unk_04 & 0xffff;

    if ((uVar1 & 0xffff) == 0) {
        if (data->pac != NULL) {
            PacMgr_ReleasePack(data->pac);
        }

        if (data->bin != NULL) {
            BinMgr_ReleaseBin(data->bin);
        }

        if (((u32)(data->unk_04 << 0x17) >> 0x1F) == 1 && data->unk_08 != NULL) {
            Mem_Free(&gMainHeap, data->unk_08);
        }

        func_020080ec(local_datMgr, data);
        func_02008138(local_datMgr, data);
        func_02008090(local_datMgr, data);
        ret = TRUE;
    }

    return ret;
}

void func_02008e80(void) {}

void func_02008ebc(s32 arg0) {}
