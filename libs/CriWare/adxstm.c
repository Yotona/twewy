#include <CriWare/adxstm.h>

void func_02015ee4(ADXSTM* stm);
void func_02015f30(ADXSTM* stm);
s32  func_02015f80(ADXSTM* stm);
s32  func_02015fb4(ADXSTM* stm, s32 param_2);
s32  func_0201602c(ADXSTM* stm);
void func_02016058(ADXSTM* stm);
void func_02016074(ADXSTM* stm);
void func_020160bc(ADXSTM* stm);
void func_020160d8(ADXSTM* stm);
void func_02016130(ADXSTM* stm, s32 param_2, s32 param_3);
void func_02016160(ADXSTM* stm, int param_2);
void func_0201680c();
void func_020168d0();
void func_020168dc();
s32  func_020168e8(s32*);
s32  func_020168c0(ADXSTM* stm, s32 param_2, s32 param_3);
void func_0201687c(void);

ADXSTMWork data_0206c398;
ADXSTM     data_0206c3b0[10];

void func_02015e10(ADXSTM* stm, s32 param_2, s32 param_3, s32 param_4, s32 param_5, int param_6) {
    func_020168f4();
    func_02015e5c(stm, param_2, param_3, param_4, param_5, param_6);
    func_02016900();
}

void func_02015e5c(ADXSTM* stm, const char* filename, s32 param_3, s32 param_4, s32 param_5, s32 param_6) {
    func_020168d0();
    stm->unk_0C   = param_4;
    stm->unk_10   = param_5;
    stm->unk_14   = param_6;
    stm->unk_18   = func_020564ec(param_5 + 0x7ff, param_6 + (u32)(0xfffff800 < param_5), 0x800, 0);
    stm->filename = filename;
    stm->unk_58   = param_3;
    stm->unk_49   = 1;
    func_020168dc();
}

void func_02015ec8(ADXSTM* stm) {
    func_020168f4();
    func_02015ee4(stm);
    func_02016900();
}

void func_02015ee4(ADXSTM* stm) {
    func_02016058(stm);
    func_020168d0();
    if (stm->unk_4D == 1) {
        stm->unk_4A = 1;
    }
    stm->unk_49 = 0;
    func_020168dc();
}

void func_02015f14(ADXSTM* stm) {
    func_020168f4();
    func_02015f30(stm);
    func_02016900();
}

void func_02015f30(ADXSTM* stm) {
    func_020160bc(stm);
    func_02015ec8(stm);

    while (TRUE) {
        if (stm->unk_4D == 0) {
            break;
        }
        func_02014528(stm->unk_4D);
    }
}

s32 func_02015f5c(ADXSTM* stm) {
    s32 val;

    func_020168f4();
    val = func_02015f80(stm);
    func_02016900();
    return val;
}

s32 func_02015f80(ADXSTM* stm) {
    return stm->unk_01;
}

s32 func_02015f88(ADXSTM* stm, int param_2) {
    s32 val;

    func_020168f4();
    val = func_02015fb4(stm, param_2);
    func_02016900();
    return val;
}

s32 func_02015fb4(ADXSTM* stm, s32 param_2) {
    stm->unk_5C = param_2;
    if (param_2 > stm->unk_18) {
        stm->unk_5C = stm->unk_18;
    }
    return stm->unk_5C;
}

void func_02015fcc(ADXSTM* stm) {
    stm->unk_38 = 0;
    stm->unk_50 = 0;

    stm->unk_01        = stm->unk_18 == 0 ? 3 : 2;
    stm->unk_02        = 0;
    stm->unk_28.data   = NULL;
    stm->unk_28.length = 0;
    stm->unk_4B        = 1;
}

s32 func_02016008(ADXSTM* stm) {
    s32 sVar1;

    func_020168f4();
    sVar1 = func_0201602c(stm);
    func_02016900();
    return sVar1;
}

s32 func_0201602c(ADXSTM* stm) {
    func_020168d0();
    func_02015fcc(stm);
    stm->unk_60 = 0xfffff;
    func_020168dc();
    return 1;
}

void func_02016058(ADXSTM* stm) {
    func_020168f4();
    func_02016074(stm);
    func_02016900();
}

void func_02016074(ADXSTM* stm) {
    func_020168d0();

    if (stm->unk_01 == 2 && stm->unk_02 == 1) {
        stm->unk_4C = 1;
        if (stm->unk_4B == 1) {
            stm->unk_4B = 0;
        }
    } else {
        stm->unk_01 = 1;
    }

    func_020168dc();
}

void func_020160bc(ADXSTM* stm) {
    func_020168f4();
    func_020160d8(stm);
    func_02016900();
}

void func_020160d8(ADXSTM* stm) {
    func_02016058(stm);

    while (TRUE) {
        if (stm->unk_01 == 1) {
            if (stm->unk_28.data == NULL) {
                break;
            }
        }
        func_02014528();
    }
}

void func_02016104(ADXSTM* stm, s32 param_2, s32 param_3) {
    func_020168f4();
    func_02016130(stm, param_2, param_3);
    func_02016900();
}

void func_02016130(ADXSTM* stm, s32 param_2, s32 param_3) {
    stm->unk_3C = param_2;
    stm->unk_40 = param_3;
}

void func_0201613c(ADXSTM* stm, int param_2) {
    func_020168f4();
    func_02016160(stm, param_2);
    func_02016900();
}

void func_02016160(ADXSTM* stm, int param_2) {
    if (param_2 < 0) {
        param_2 = stm->unk_18;
    }
    stm->unk_34 = param_2;
}

void func_02016170(void) {
    data_0206c398.unk_14++;
}

void func_02016188(ADXSTM* stm) {
    int         iVar2;
    _sj_vtable* p_Var3;
    s32         iVar6;
    s32         iVar7;
    SJCK        SStack_18;
    SJCK        SStack_20;
    SJCK        SStack_28;

    SJ* sj   = stm->sj;
    s32 stat = cvFsGetStat(stm->fileHndl);

    func_020168d0();
    if (stm->unk_02 == 1) {
        if (stat == 1) {
            stm->unk_02 = 0;
            func_020168dc();
            iVar2 = stm->unk_24;
            func_0201a670(&stm->unk_28, iVar2 << 0xb, &SStack_18, &SStack_20);
            SJ_PutChunk(sj, 1, &SStack_18);
            SJ_UngetChunk(sj, 0, &SStack_20);
            stm->unk_5C += stm->unk_24;
            stm->unk_38 += iVar2 * 0x800;
            stm->unk_28.data   = NULL;
            stm->unk_28.length = 0;

            iVar2 = stm->unk_18;
            if (stm->unk_5C == stm->unk_34 && stm->unk_3C != NULL) {
                stm->unk_3C(stm->unk_40);
            }
            if (stm->unk_5C >= iVar2) {
                stm->unk_01 = 3;
            } else if (stm->unk_60 <= stm->unk_38 >> 0xb && stm->unk_60 < 0xfffff) {
                stm->unk_01 = 3;
            }
            stm->unk_50 = 0;
        } else if (stat == 3) {
            stm->unk_02 = 0;
            func_020168dc();
            SJ_UngetChunk(sj, 0, &stm->unk_28);
            stm->unk_28.data   = NULL;
            stm->unk_28.length = 0;

            if (0 <= data_0206c398.unk_10 && stm->unk_50 >= data_0206c398.unk_10) {
                stm->unk_01 = 4;
            } else if (stm->unk_50 < 0x7fffffff) {
                stm->unk_50++;
            }

        } else {
            func_020168dc();
        }
    } else {
        func_020168dc();
    }
    if (stm->unk_01 == 4) {
        return;
    }
    func_020168d0();
    if (stm->unk_02 == 0) {

        stm->unk_02          = 1;
        (stm->unk_28).data   = NULL;
        (stm->unk_28).length = 0;
        func_020168dc();
        if (stm->unk_48 == 1 || stm->unk_4C == 1) {
            stm->unk_02 = 0;
            return;
        }
        p_Var3 = stm->unk_18;
        if (p_Var3 == NULL) {
            stm->unk_02 = 0;
            stm->unk_24 = 0;
            stm->unk_01 = 3;
            return;
        }
        if (sj == NULL || (p_Var3 = sj->vtable) == NULL) {
            stm->unk_02 = 0;
            func_02016170();
            return;
        }
        if (stm->unk_44 - (*p_Var3->GetNumData)(sj, 0) >= stm->unk_20) {
            stm->unk_02 = 0;
            return;
        }

        SJ_GetChunk(sj, 0, stm->unk_1C, &SStack_28);

        iVar2 = SStack_28.length / 0x800;
        iVar7 = stm->unk_34 - stm->unk_5C;
        if (iVar2 >= iVar7) {
            iVar2 = iVar7;
        }

        iVar7 = stm->unk_18 - stm->unk_5C;
        if (iVar2 >= iVar7) {
            iVar2 = iVar7;
        }
        if (iVar2 >= stm->unk_30) {
            iVar2 = stm->unk_30;
        }
        cvFsSeek(stm->fileHndl, stm->unk_0C + stm->unk_5C, 0);
        if (stm->unk_60 != 0xfffff) {
            iVar6 = stm->unk_60 - ((s32)stm->unk_38 / 0x800);
            if (iVar2 >= iVar6) {
                iVar2 = iVar6;
            }
        }
        stm->unk_24        = cvFsReqRd(stm->fileHndl, iVar2, SStack_28.data);
        stm->unk_28.data   = SStack_28.data;
        stm->unk_28.length = SStack_28.length;
        if (0 < stm->unk_24) {
            return;
        }
        SJ_UngetChunk(sj, 0, &stm->unk_28);
        stm->unk_28.data   = NULL;
        stm->unk_28.length = 0;
        stm->unk_02        = 0;

        if (cvFsGetStat(stm->fileHndl) != 3) {
            return;
        }

        if (0 <= data_0206c398.unk_10 && stm->unk_50 >= data_0206c398.unk_10) {
            stm->unk_01 = 4;
            return;
        }
        if (stm->unk_50 < 0x7fffffff) {
            stm->unk_50++;
        }
        return;
    }
    func_020168dc();
    return;
}

void func_0201654c(ADXSTM* stm) {
    void* handle;
    s64   size;
    s32   uVar3;
    int   iVar4;
    u32   uVar7;
    BOOL  bVar8;

    if (stm->unk_02 == 0) {
        if ((stm->unk_4C == 1) && (stm->unk_4C = 0, stm->unk_4B == 0)) {
            stm->unk_01 = 1;
        }
        if (stm->unk_4A == 1) {
            handle = stm->fileHndl;
            if (handle != NULL) {
                stm->fileHndl = 0;
                cvFsClose(handle);
            }
            func_020168d0();
            stm->unk_4A = 0;
            stm->unk_4D = 0;
            func_020168dc();
        }
        func_020168d0();
        bVar8 = FALSE;
        if (stm->unk_49 == 1) {
            if (stm->unk_4D == 0) {
                stm->unk_4D = 1;
                func_020168dc();
                bVar8 = TRUE;
                if (stm->fileHndl == NULL) {
                    stm->fileHndl = cvFsOpen(stm->filename, stm->unk_58, 0);
                    if (stm->fileHndl == NULL) {
                        func_02014398("E02110501 adxstmf_stat_exec: can\'t open ", stm->filename);
                        stm->unk_01 = 4;
                        stm->unk_4D = 0;
                        stm->unk_49 = 0;
                        return;
                    }
                }
            }
            if (stm->unk_4D == 1) {
                if (bVar8 == FALSE) {
                    func_020168dc();
                }
                if (stm->unk_49 == 1 && stm->unk_4A == 1) {
                    return;
                }
                if (stm->unk_58 == 0) {
                    size = cvFsGetFileSizeByHndl(stm->fileHndl);
                    if (size < 0) {
                        size = cvFsGetFileSize(stm->filename);
                    }
                    iVar4 = func_020564ec(size + 0x7FF, 0x800, 0);
                } else {
                    cvFsSeek(stm->fileHndl, 0, 2);
                    iVar4 = cvFsTell(stm->fileHndl);
                    uVar3 = (iVar4 * 0x800);
                    size  = uVar3 >> 0x1F;
                    cvFsSeek(stm->fileHndl, 0, 0);
                }
                if (stm->unk_14 == stm->unk_10 || stm->unk_14 == 0x7FFFF800) {
                    stm->unk_10 = uVar3;
                    stm->unk_14 = size;
                    stm->unk_18 = iVar4;
                }
                if (stm->unk_0C > iVar4) {
                    stm->unk_0C = iVar4;
                }
                if (stm->unk_18 + stm->unk_0C > iVar4) {
                    uVar7       = iVar4 - stm->unk_0C;
                    stm->unk_18 = iVar4 - stm->unk_0C;
                    stm->unk_10 = stm->unk_18 * 0x800;
                    stm->unk_14 = uVar7 >> 0x15 | ((int)uVar7 >> 0x1f) << 0xb;
                }
                func_02015f88(stm, 0);
                stm->unk_49 = 0;
                if (cvFsGetStat(stm->fileHndl) == 3) {
                    func_02014398("E05072801 adxstmf_stat_exec: can\'t open ", stm->filename);
                    handle = stm->fileHndl;
                    if (handle != NULL) {
                        stm->fileHndl = NULL;
                        cvFsClose(handle);
                    }
                    stm->unk_01 = 4;
                    stm->unk_4D = 0;
                    stm->unk_49 = 0;
                    return;
                }
            }
        } else {
            func_020168dc();
        }
        if (stm->unk_4B == 1) {
            stm->unk_4B = 0;
        }
    }
    if (stm->unk_01 == 2 && stm->unk_4D == 1 && stm->unk_49 == 0) {
        func_02016188(stm);
    }
}

void func_020167f8() {
    func_020168f4();
    func_0201680c();
    func_02016900();
}

void func_0201680c(void) {
    int idx;

    if (func_020168e8(&data_0206c398.unk_04) == 0) {
        return;
    }

    for (idx = 0; idx < 10; idx++) {
        if (data_0206c3b0[idx].unk_00 == 1) {
            func_0201654c(&data_0206c3b0[idx]);
        }
    }

    data_0206c398.unk_04 = 0;
}

void func_02016868(void) {
    func_020168f4();
    func_0201687c();
    func_02016900();
}

void func_0201687c(void) {
    func_0201b4a0();
}

void func_02016888(void) {
    return;
}

s32 func_0201688c(ADXSTM* stm, s32 param_2, s32 param_3) {
    s32 sVar1;

    func_020168f4();
    sVar1 = func_020168c0(stm, param_2, param_3);
    func_02016900();
    return sVar1;
}

s32 func_020168c0(ADXSTM* stm, s32 param_2, s32 param_3) {
    stm->unk_20 = param_2;
    stm->unk_1C = param_3;
    return 1;
}

void func_020168d0(void) {
    ADXCRS_Lock();
}

void func_020168dc(void) {
    ADXCRS_Unlock();
}

s32 func_020168e8(s32*) {
    return func_0201ad20();
}
