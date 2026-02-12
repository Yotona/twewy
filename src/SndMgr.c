#include "SndMgr.h"
#include "CriSndMgr.h"
#include "Memory.h"
#include "common_data.h"

SndMgr sndMgr = {};

typedef struct UnkStruct_usedby_020310e8 {
    /* 0x00 */ s32 unk_00;
    /* 0x04 */ u8  unk_04;
} UnkStruct_usedby_020310e8;
extern UnkStruct_usedby_020310e8* func_020310e8(s16);

typedef struct {
    /* 0x0 */ s16 unk_00;
    /* 0x2 */ u8  unk_02;
} UnkStruct_usedby_02030f78;
extern UnkStruct_usedby_02030f78* func_02030f78(u32 param_1, u32 param_2);

static void func_020268f0(void) {
    sndMgr.unk_16DA = 0;
    for (u32 i = 0; i < 3; i++) {
        sndMgr.unk_16D4[i] = -1;
    }
}

static BOOL func_02026928(s16 param_1) {
    if ((param_1 < 7) || (param_1 > 37)) {
        return TRUE;
    }

    for (u32 i = 0; i < 3; i++) {
        if (param_1 == sndMgr.unk_16D4[i]) {
            return TRUE;
        }
    }
    return FALSE;
}

void SndMgr_Init(void) {
    func_02027220(0);
    void* heap = Mem_AllocHeapTail(&gMainHeap, 0x40000);
    Mem_SetSequence(&gMainHeap, heap, "SndMgr");
    MI_CpuFill(0, heap, Mem_GetBlockSize(&gMainHeap, heap));
    sndMgr.data = heap;
    func_020268f0();
    func_0202f8a0();
    sndMgr.unk_0004 = func_02031364(sndMgr.data, 0x40000);
    func_02030cec(&sndMgr.unk_1640, "/Sound/sound_data.sdat", sndMgr.unk_0004, 0);
    func_02031f34(sndMgr.unk_0004);
    func_020316d0(0, sndMgr.unk_0004);
    func_020316fc(0, sndMgr.unk_0004);

    for (s32 i = 0; i < 5; i++) {
        sndMgr.unk_1618[i].unk_00 = -1;
        func_0202fc84(&sndMgr.unk_1618[i].unk_04);
    }

    func_02026dd8();
    func_02031510(sndMgr.unk_0004);
    func_02026c04();
    CriSndMgr_Init();
}

void func_02026a94(void) {
    func_02027330();
    func_0202f928();
}

s32 func_02026aa4(s32 idx) {
    s32 sVar1 = sndMgr.unk_1618[idx].unk_00;
    func_0202fb80(&sndMgr.unk_1618[idx].unk_04, 1);
    sndMgr.unk_1618[idx].unk_00 = -1;
    return sVar1;
}

void func_02026ae0(s32 arg0, s32 seqArc, s32 se) {
    func_02027040(data_0205cb3c[se].seqArc);
    func_02031fd0(&sndMgr.unk_1618[arg0].unk_04, seqArc, se);
}

void func_02026b20(s32 seIdx) {
    s32 seqArc = data_0205cb3c[seIdx].seqArc;
    if (seqArc < 0) {
        func_02026e00(seIdx);
        return;
    }

    func_02027040(seqArc);
    s32* psVar2 = &sndMgr.unk_1618[data_0205cb3c[seIdx].unk_01].unk_04;
    func_02031fd0(psVar2, seqArc, data_0205cb3c[seIdx].se);
    func_0202fd24(psVar2, sndMgr.seIdxVolume[seIdx]);
}

void func_02026b9c(s32 seIdx) {
    if (data_0205cb3c[seIdx].seqArc < 0) {
        func_02026e44(0);
    } else {
        func_0202fb90(data_0205cb3c[seIdx].seqArc, data_0205cb3c[seIdx].se, 0);
    }
}

static s32 func_02026bdc(s16 arg0) {
    return func_020310e8(arg0)->unk_04;
}

static u8 func_02026bec(u32 param_1, u32 param_2) {
    UnkStruct_usedby_02030f78* iVar2 = func_02030f78(param_1, param_2);
    if (iVar2 == NULL) {
        return 0;
    }
    return iVar2->unk_02;
}

void func_02026c04(void) {
    u32 uVar2 = -1;
    s8  seqArc;

    for (s32 seIdx = 0; seIdx < 0x56C; seIdx++) {
        seqArc = data_0205cb3c[seIdx].seqArc;
        if (seqArc < 0) {
            sndMgr.seIdxVolume[seIdx] = func_02026bdc(data_0205cb3c[seIdx].se);
        } else {
            if (uVar2 != seqArc) {
                uVar2 = seqArc;
                func_0203155c(sndMgr.unk_0004, 1);
                func_02027040(seqArc);
            }
            sndMgr.seIdxVolume[seIdx] = func_02026bec(seqArc, data_0205cb3c[seIdx].se);
        }
    }
    func_0203155c(sndMgr.unk_0004, 1);
}

s32 SndMgr_GetSeIdxVolume(s32 seIdx) {
    return sndMgr.seIdxVolume[seIdx];
}

void SndMgr_SetSeIdxVolume(s32 seIdx, s32 seIdxVolume) {
    sndMgr.seIdxVolume[seIdx] = seIdxVolume;
    if (data_0205cb3c[seIdx].seqArc >= 0) {
        func_0202fd24(&sndMgr.unk_1618[data_0205cb3c[seIdx].unk_01].unk_04, seIdxVolume);
    }
}

void func_02026d0c(s32 seIdx, s32 sePan) {
    u32 uVar2 = data_0205cb3c[seIdx].unk_01;

    if ((uVar2 + 0xfe & 0xff) <= 1) {
        func_0202fd78(&sndMgr.unk_1618[uVar2].unk_04, 0xffff, SndMgr_ConvertPanToSigned(sePan));
    } else if (uVar2 == 6) {
        func_020323fc(&sndMgr.unk_0064, 0, SndMgr_ConvertPanToVolume(sePan));
    }
}

void func_02026d7c(s32 seIdx, s32 sePan) {
    func_02026b20(seIdx);
    func_02026d0c(seIdx, sePan);
}

s32 SndMgr_ConvertPanToSigned(s32 sePan) {
    if (sePan < 0) {
        return -0x80;
    }
    if (sePan >= 0x100) {
        return 0x7f;
    }
    return sePan - 0x80;
}

s32 SndMgr_ConvertPanToVolume(s32 sePan) {
    if (sePan < 0) {
        return 0;
    }
    if (sePan >= 0x100) {
        return 0x7f;
    }
    return sePan / 2;
}

void func_02026dd8(void) {
    func_02032114(10, sndMgr.unk_0004);
    func_02032418(&sndMgr.unk_0064);
}

BOOL func_02026e00(s32 param_1) {
    return func_0203235c(&sndMgr.unk_0064, data_0205cb3c[param_1].se, 0);
}

BOOL func_02026e28(s32 se) {
    return func_0203235c(&sndMgr.unk_0064, se, 0);
}

void func_02026e44(s32 param_1) {
    func_02032384(param_1);
}

void func_02026e50(s32 param_1) {
    func_020323c8(&sndMgr.unk_0064, param_1, 0);
    func_020323c8(&sndMgr.unk_0064, param_1, 0);
}

s32 func_02026e80(s32 param_1) {
    switch (param_1) {
        case 0:
            return 0;
        case 2:
            return 2;
        case 1:
        case 3:
        case 4:
        case 5:
        case 6:
            return 1;
        case 7:
            return 3;
        case 8:
            return 4;
        case 9:
            return 5;
        case 10:
            return 6;
        case 11:
            return 7;
        case 12:
            return 8;
        case 13:
            return 9;
        case 14:
            return 10;
        case 15:
            return 11;
        case 16:
            return 12;
        case 17:
            return 13;
        case 18:
            return 14;
        case 19:
            return 15;
        case 20:
            return 16;
        case 21:
            return 17;
        case 22:
            return 18;
        case 23:
            return 19;
        case 24:
            return 20;
        case 25:
            return 21;
        case 26:
            return 22;
        case 27:
            return 23;
        case 28:
            return 24;
        case 29:
            return 25;
        case 30:
            return 26;
        case 31:
            return 27;
        case 32:
            return 28;
        case 33:
            return 29;
        case 34:
            return 30;
        case 35:
            return 31;
        case 36:
            return 32;
        case 38:
            return 34;
        case 37:
        default:
            return -1;
    }
}

u32 func_02027040(s32 seqArc) {
    func_02026928(seqArc);

    s32* data  = func_02030fbc(seqArc);
    s32  uVar3 = func_020312fc(*data);

    if (uVar3 != 0) {
        return uVar3;
    }
    func_020316d0(seqArc, sndMgr.unk_0004);

    uVar3 = func_02026e80(seqArc);
    if (uVar3 >= 0) {
        if ((uVar3 >= 3) && (sndMgr.unk_16D0 != 0)) {
            uVar3 = func_02031728(uVar3, 2, sndMgr.unk_0004);
        } else {
            uVar3 = func_020316fc(uVar3, sndMgr.unk_0004);
        }
    }
    if (seqArc != 1) {
        return uVar3;
    }

    return func_02031510(sndMgr.unk_0004);
}

void func_020270e4(void) {
    func_0203155c(sndMgr.unk_0004, 1);
    func_020268f0();
}

void func_02027104(void) {
    func_0203155c(sndMgr.unk_0004, 2);
    func_020268f0();
}

BOOL func_02027124(s32 param_1) {
    BOOL result;

    if (data_0205cb3c[param_1].seqArc < 0) {
        result = func_0203243c(&sndMgr.unk_0064);
    } else {
        result = func_0202fcb0(data_0205cb3c[param_1].seqArc, data_0205cb3c[param_1].se);
    }

    if (result == 0) {
        return FALSE;
    }
    return TRUE;
}

void func_02027170(s32 param_1, s32 param_2) {
    if (data_0205cb3c[param_1].seqArc < 0) {
        func_02026e50(param_2);
    } else {
        func_0202fd10(&sndMgr.unk_1618[data_0205cb3c[param_1].unk_01].unk_04, param_2);
    }
}

void func_020271b8(s32 seIdx, s32 sePitch) {
    if (data_0205cb3c[seIdx].seqArc >= 0) {
        func_0202fd58(&sndMgr.unk_1618[data_0205cb3c[seIdx].unk_01].unk_04, 0xFFFF, sePitch);
    }
}

void func_02027200(u32 param_1) {
    func_0202fc34(param_1);
    if (param_1 != 0) {
        func_02026e44(0);
    }
}

void func_02027220(s32 param_1) {
    sndMgr.unk_16D0 = param_1;
}

void func_02027230(s32 param_1) {
    for (u32 i = 0; i < 3; i++) {
        if (sndMgr.unk_16D4[i] == param_1) {
            return;
        }
        if (sndMgr.unk_16D4[i] < 0) {
            sndMgr.unk_16D4[i] = param_1;
            sndMgr.unk_16DA++;
            return;
        }
    }
}
