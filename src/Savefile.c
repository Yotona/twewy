#include "DatMgr.h"
#include "Save.h"
#include "System.h"

extern void func_020389c0(u16);
extern s64  func_0203a444(void);
extern s32  func_02042330(void);
extern void func_0204237c(u16);
extern void func_0204238c(u16);
extern void func_0204285c(u32, u16, u16, s32, s32, s32, s32, s32, s32);
extern u8   func_02042944(void);
extern void func_0204296c(s32);
extern void func_02042aa4(void);
extern s32  func_02042ab0(void);

void func_02025b1c(void);
s32  func_020258bc(u8*, const u8*, s32, s32);
u16  func_0202593c(u16*, s16);

extern s32 data_0205c23c;
extern u16 data_0205c5be[];

const char data_0205c96c[27] = "SubarAsikiKonosEkai070613b";

u16* func_020246d4(s32 arg0) {
    u16* backupData = Mem_AllocHeapTail(&gMainHeap, (arg0 + 7) & ~7);
    Mem_SetSequence(&gMainHeap, backupData, "BackUpData_Tmp");
    return backupData;
}

void func_0202470c(u16* arg0) {
    Mem_Free(&gMainHeap, arg0);
}

s32 func_02024724(u16 arg0) {
    s32 var_r5;

    u16* temp_r0 = func_020246d4(0x3444);

    func_0204285c(arg0, temp_r0, 0x3444, 0, 0, 1, 6, 1, 0);
    func_02042aa4();
    func_02042330();
    if (func_020258bc(temp_r0 + 0x3420 / 2, data_0205c96c, 0x1B, 0x3420) != 0) {
        if (func_0202593c(temp_r0, 0x3444) != 0) {
            var_r5 = 8;
        } else {
            var_r5                        = 0;
            data_02071cf0.unk_20.unk_1AB6 = temp_r0[0x1AB6 / 2];
        }
    } else {
        var_r5 = 1;
    }
    Mem_Free(&gMainHeap, temp_r0);
    return var_r5;
}

s32 func_020247f4(void) {
    s32 var_r4;

    data_02071cf0.lockID = OS_GetLockID();
    if (data_02071cf0.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(data_02071cf0.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        s32 temp_r0_2 = func_02024724(0);
        if (temp_r0_2 != 0) {

            s32 temp_r0_3 = func_02024724(0x5470);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 == 1) && (temp_r0_3 == 1)) {
                    var_r4 = 1;
                } else if ((temp_r0_2 == 2) || (temp_r0_3 == 2)) {
                    var_r4 = 2;
                } else {
                    var_r4 = 8;
                }
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }

    func_0204238c(data_02071cf0.lockID);
    func_020389c0(data_02071cf0.lockID);
    return var_r4;
}

s32 func_020248e4(s32 arg0) {
    s32  var_r5;
    u16* temp_r0;

    temp_r0 = func_020246d4(0x202C);
    func_0204285c(arg0, temp_r0, 0x202C, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    if (func_020258bc(temp_r0 + 0x2008 / 2, data_0205c96c, 0x1B, 0x2008) != 0) {
        if (func_0202593c(temp_r0, 0x202C) == 0) {
            var_r5 = 0;
        } else {
            var_r5 = 8;
        }
    } else {
        var_r5 = 1;
    }
    Mem_Free(&gMainHeap, temp_r0);
    return var_r5;
}

s32 func_0202499c(void) {
    s32 var_r4;

    data_02071cf0.lockID = OS_GetLockID();
    if (data_02071cf0.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(data_02071cf0.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        s32 temp_r0_2 = func_020248e4(0x3444);
        if (temp_r0_2 != 0) {
            s32 temp_r0_3 = func_020248e4(0x88B4);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 == 1) && (temp_r0_3 == 1)) {
                    var_r4 = 1;
                } else if ((temp_r0_2 == 2) || (temp_r0_3 == 2)) {
                    var_r4 = 2;
                } else {
                    var_r4 = 8;
                }
            } else if (!(data_02071cf0.unk_20.unk_1AB6 & 1)) {
                var_r4 = 8;
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c(data_02071cf0.lockID);
    func_020389c0(data_02071cf0.lockID);
    return var_r4;
}

s32 func_02024aa4(void) {
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 0;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 0;

    s32 temp_r0 = func_020247f4() | func_0202499c();

    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;

    return temp_r0;
}

s32 func_02024b08(s32 arg0) {
    u16 var_r4 = 0;
    do {
        s32 temp_r5 = (var_r4 * 0x98) + arg0;

        u16 var_r7 = 0;
        do {
            *(u8*)(temp_r5 + var_r7) = 0xFF;
            var_r7 += 1;
        } while (var_r7 < 6);

        s32 temp_r8             = (var_r4 * 0x98) + arg0;
        *(u16*)(temp_r8 + 0x6A) = 0xFFFF;

        u16 var_r9 = 0;
        do {
            *(u16*)(temp_r8 + (var_r9 * 2) + 0x6C) = 0xFFFF;
            var_r9 += 1;
        } while (var_r9 < 6);

        u16 var_r8 = 0;
        do {
            *(u16*)(temp_r5 + (var_r8 * 2) + 0x78) = 0xFFFF;
            var_r8 += 1;
        } while (var_r8 < 0x10);

        var_r4 += 1;
    } while (var_r4 < 0x32);

    u16 var_r8_2 = 0;
    do {
        u16 var_r2 = 0;
        do {
            *(u8*)((var_r8_2 * 0xC) + arg0 + var_r2 + 0x1DB0) = 0xFF;
            var_r2 += 1;
        } while (var_r2 < 6);
        var_r8_2 += 1;
    } while (var_r8_2 < 0x32);
    return 0;
}

void func_02024c00(void) {
    func_02025b1c();
    data_02071cf0.unk_20.unk_1AB6 &= ~1;
}

void func_02024c20(void) {
    void* globalFriendData = Mem_AllocHeapTail(&gMainHeap, 0x2008);
    Mem_SetSequence(&gMainHeap, globalFriendData, "GlobalFriendData");
    MI_CpuFill(0, globalFriendData, Mem_GetBlockSize(&gMainHeap, globalFriendData));
    data_02071cf0.globalFriendData = globalFriendData;
    func_02024b08(globalFriendData);
}

void func_02024c84(void) {
    Mem_Free(&gMainHeap, data_02071cf0.globalFriendData);
}

s32 func_02024ca4(void) {
    func_02024c00();
    func_02024c20();
    data_02071cf0.unk_00++;
    return 0;
}

BOOL func_02024ccc(void) {
    func_02024c84();
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = TRUE;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = TRUE;
    return TRUE;
}

void func_02024d04(void) {
    data_02071cf0.unk_00 = 0;
    data_02071cf0.unk_02 = 0;
    data_02071cf0.unk_08 = 0;
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 0;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 0;
}

s32 func_02024d48(void) {
    s32*  var_r4;
    s32   temp_lr;
    s32   temp_r0_2;
    s32   temp_r1;
    s32   temp_r2_2;
    s32   temp_r3_2;
    s32   var_lr;
    u16   var_r1;
    u16   var_r5;
    u32   temp_r4;
    u8    temp_r3;
    void* temp_r0;
    void* temp_r2;
    void* var_ip;

    MI_CpuSet((void*)data_02071cf0.unk_10, 0, 0x3444);
    var_ip = (void*)data_02071cf0.unk_10;
    var_r1 = 0;
    do {
        temp_r3 = data_0205c96c[var_r1];
        temp_r0 = var_ip + var_r1 + 0x3420;
        var_r1 += 1;
        *(u8*)temp_r0 = temp_r3;
    } while (var_r1 < 0x1B);
    var_r5 = 0;
    do {
        temp_lr = var_r5 * 4;
        temp_r4 = 0x3420 & (0xF << temp_lr);
        temp_r2 = var_ip + var_r5 + 0x343B;
        var_r5 += 1;
        *(u8*)temp_r2 = (s8)(temp_r4 >> temp_lr);
    } while (var_r5 < 4);
    var_r4 = (s32*)&data_02071cf0.unk_20;
    var_lr = 0x342;
    do {
        temp_r0_2 = var_r4[0];
        temp_r1   = var_r4[1];
        temp_r2_2 = var_r4[2];
        temp_r3_2 = var_r4[3];
        var_r4 += 4;
        *(s32*)((s32)var_ip + 0)  = temp_r0_2;
        *(s32*)((s32)var_ip + 4)  = temp_r1;
        *(s32*)((s32)var_ip + 8)  = temp_r2_2;
        *(s32*)((s32)var_ip + 12) = temp_r3_2;
        var_ip                    = (void*)((s32)var_ip + 0x10);
        var_lr -= 1;
    } while (var_lr != 0);
    return 0;
}

s32 func_02024e04(void* arg0) {
    s32   temp_r0_2;
    s32   temp_r1;
    s32   temp_r2_2;
    s32   temp_r3_2;
    s32   temp_r5;
    s32   var_lr;
    u16   var_r1;
    u16   var_r6;
    u32   temp_lr;
    u8    temp_r3;
    void* temp_r0;
    void* temp_r2;
    void* var_ip;
    void* var_r4;

    var_r4 = arg0;
    MI_CpuSet((void*)data_02071cf0.unk_14, 0, 0x202C);
    var_ip = (void*)data_02071cf0.unk_14;
    var_r1 = 0;
    do {
        temp_r3 = data_0205c96c[var_r1];
        temp_r0 = var_ip + var_r1 + 0x2008;
        var_r1 += 1;
        *(u8*)temp_r0 = temp_r3;
    } while (var_r1 < 0x1B);
    var_r6 = 0;
    do {
        temp_r5 = var_r6 * 4;
        temp_lr = 0x2008 & (0xF << temp_r5);
        temp_r2 = var_ip + var_r6 + 0x2023;
        var_r6 += 1;
        *(u8*)temp_r2 = (s8)(temp_lr >> temp_r5);
    } while (var_r6 < 4);
    var_lr = 0x200;
    do {
        temp_r0_2                 = *(s32*)((s32)var_r4 + 0);
        temp_r1                   = *(s32*)((s32)var_r4 + 4);
        temp_r2_2                 = *(s32*)((s32)var_r4 + 8);
        temp_r3_2                 = *(s32*)((s32)var_r4 + 12);
        var_r4                    = (void*)((s32)var_r4 + 0x10);
        *(s32*)((s32)var_ip + 0)  = temp_r0_2;
        *(s32*)((s32)var_ip + 4)  = temp_r1;
        *(s32*)((s32)var_ip + 8)  = temp_r2_2;
        *(s32*)((s32)var_ip + 12) = temp_r3_2;
        var_ip                    = (void*)((s32)var_ip + 0x10);
        var_lr -= 1;
    } while (var_lr != 0);
    *(s32*)((s32)var_ip + 0) = *(s32*)((s32)var_r4 + 0);
    *(s32*)((s32)var_ip + 4) = *(s32*)((s32)var_r4 + 4);
    return 0;
}

void func_02024ec0(u16* arg0) {
    func_0204238c(data_02071cf0.lockID);
    func_020389c0(data_02071cf0.lockID);
    func_0202470c(arg0);
}

s32 func_02024f00(void) {
    u16* temp_r0 = func_020246d4(0x3444);

    data_02071cf0.unk_14 = (s32)temp_r0;
    func_02024d48();
    *(u16*)((s32)temp_r0 + 0x3440) = 0;
    *(u16*)((s32)temp_r0 + 0x3440) = func_0202593c(temp_r0, 0x3444);

    data_02071cf0.lockID = OS_GetLockID();
    if (data_02071cf0.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(data_02071cf0.lockID);

    func_0204296c(0x1001);

    if (func_02042330() != 0) {
        data_02071cf0.unk_02 = 4;
        func_02024ec0(data_02071cf0.unk_14);
        return 1;
    }

    data_02071cf0.unk_00++;
    return 0;
}

s32 func_02024fc0(void) {
    s32 temp_r1;
    s32 temp_r4;
    s64 temp_ret;

    temp_r4              = data_02071cf0.unk_10;
    data_02071cf0.unk_08 = 0;
    if (func_02042944() == 1) {
        temp_ret             = func_0203a444();
        temp_r1              = (s32)(temp_ret >> 32);
        data_02071cf0.unk_18 = (s32)temp_ret;
        data_02071cf0.unk_1C = temp_r1;
        func_0204285c(temp_r4, NULL, 0x3444, 0, 0, 1, 8, 0xA, 2);
    } else {
        OS_WaitForever();
    }
    data_02071cf0.unk_00 += 1;
    return 0;
}

s32 func_02025058(void) {
    s32 temp_r4;

    temp_r4              = data_02071cf0.unk_10;
    data_02071cf0.unk_08 = 0;
    if (func_02042944() == 1) {
        func_0204285c(temp_r4, 0x5470, 0x3444, 0, 1, 8, 0xA, 2, 0);
    } else {
        OS_WaitForever();
    }
    data_02071cf0.unk_00 += 1;
    return 0;
}

s32 func_020250e8(void) {
    u16 var_r1;

    if (func_02042ab0() != 0) {
        if (func_02042330() == 0) {
            var_r1 = 0;
        } else {
            var_r1 = data_02071cf0.unk_02 | 4;
        }
        data_02071cf0.unk_02 = var_r1;
        data_02071cf0.unk_00 += 1;
    }
    return 0;
}

s32 func_02025138(void) {
    s32 temp_r1;
    s64 temp_ret;
    u32 temp_r0;

    func_02024ec0((u16*)data_02071cf0.unk_10);
    temp_ret             = func_0203a444();
    temp_r1              = (s32)(temp_ret >> 32);
    temp_r0              = (s32)temp_ret - data_02071cf0.unk_18;
    data_02071cf0.unk_18 = temp_r0;
    data_02071cf0.unk_1C = (s32)((temp_r1 - data_02071cf0.unk_1C) + ((s64)temp_r0 < 0));
    data_02071cf0.unk_00 += 1;
    return 0;
}

s32 func_02025180(void) {
    func_02024ec0(data_02071cf0.unk_10);

    s64 temp_ret = func_0203a444();

    s32 temp_r1 = temp_ret < data_02071cf0.unk_18;

    data_02071cf0.unk_18 = temp_ret - data_02071cf0.unk_18;
    data_02071cf0.unk_1C = ((temp_ret >> 32) - data_02071cf0.unk_1C);
    data_02071cf0.unk_00++;
    return 0;
}

void func_020251c8(void* globalFriendData) {
    data_02071cf0.globalFriendData = globalFriendData;
}

s32 func_020251d8(void) {
    s32  temp_r0_2;
    u16* temp_r0;

    temp_r0              = func_020246d4(0x202C);
    data_02071cf0.unk_10 = temp_r0;

    func_02024e04(data_02071cf0.globalFriendData);

    *(u16*)((s32)temp_r0 + 0x2028) = 0;
    *(u16*)((s32)temp_r0 + 0x2028) = func_0202593c(temp_r0, 0x202C);

    data_02071cf0.lockID = OS_GetLockID();
    if (data_02071cf0.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(data_02071cf0.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        data_02071cf0.unk_02 = 4;
        func_02024ec0(data_02071cf0.unk_10);
        return 1;
    }
    data_02071cf0.unk_00++;
    return 0;
}

s32 func_0202529c(void) {
    s32 temp_r4 = data_02071cf0.unk_10;

    data_02071cf0.unk_08 = 0;

    if ((func_02042944() & 0xFF) == 1) {
        s64 temp = func_0203a444();

        data_02071cf0.unk_18 = temp;
        data_02071cf0.unk_1C = (temp >> 32);
        func_0204285c(temp_r4, 0x3444, 0x202C, 0, 0, 1, 8, 0xA, 2);
    } else {
        OS_WaitForever();
    }

    data_02071cf0.unk_00++;
    return 0;
}

s32 func_0202533c(void) {
    s32 temp_r4 = data_02071cf0.unk_10;

    data_02071cf0.unk_08 = 0;

    if ((func_02042944() & 0xFF) == 1) {
        func_0204285c(temp_r4, 0x88B4, 0x202C, 0, 0, 1, 8, 0xA, 2);
    } else {
        OS_WaitForever();
    }

    data_02071cf0.unk_00++;
    return 0;
}

s32 func_020253cc(void) {
    if (func_02042ab0() != 0) {
        if (func_02042330() != 0) {
            data_02071cf0.unk_02 = 4;
        } else {
            data_02071cf0.unk_02 = 0;
        }
        data_02071cf0.unk_00++;
    }
    return 0;
}

s32 func_02025414(void) {
    SystemStatusFlags;
    SystemStatusFlags.unk_05 = 1;
    SystemStatusFlags;
    SystemStatusFlags.unk_07 = 1;
    return 1;
}

static s32 (*data_0205c2a4[13])() = {
    func_02024ca4, func_02024f00, func_02024fc0, func_020250e8, func_02025058, func_020250e8, func_02025138,
    func_020251d8, func_0202529c, func_0202533c, func_020253cc, func_02025180, func_02024ccc,
};
void func_02025444(void) {
    s32 (**funcs)() = data_0205c2a4;
    funcs[data_02071cf0.unk_00]();
}

void func_02025498(void) {
    s32 sp0[13];
    s32 var_r3;

    for (var_r3 = 0; var_r3 < 13; var_r3++) {
        sp0[var_r3] = *(s32*)((s32)&data_0205c23c + var_r3 * 4);
    }
    ((void (*)(void))sp0[data_02071cf0.unk_00])();
}

s32 func_020254ec(s32 arg0) {
    s32* var_lr;
    s32  temp_r0;
    s32  temp_r1;
    s32  temp_r2;
    s32  temp_r3;
    s32  var_ip;
    u16* var_r4;

    func_0204285c(arg0, (u16*)data_02071cf0.unk_10, 0x3444, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    var_r4 = (u16*)data_02071cf0.unk_10;
    if (func_020258bc(var_r4 + 0x3420 / 2, data_0205c96c, 0x1B, 0x3420) != 0) {
        if (func_0202593c(var_r4, 0x3444) == 0) {
            var_lr = (s32*)&data_02071cf0.unk_20;
            var_ip = 0x342;
            do {
                temp_r0 = *(s32*)((s32)var_r4 + 0);
                temp_r1 = *(s32*)((s32)var_r4 + 4);
                temp_r2 = *(s32*)((s32)var_r4 + 8);
                temp_r3 = *(s32*)((s32)var_r4 + 12);
                var_r4 += 8;
                var_lr[0] = temp_r0;
                var_lr[1] = temp_r1;
                var_lr[2] = temp_r2;
                var_lr[3] = temp_r3;
                var_lr += 4;
                var_ip -= 1;
            } while (var_ip != 0);
            return 0;
        }
        return 8;
    }
    return 1;
}

// ============ func_020255bc ============
// Loads primary save data (checks both slots 0 and 0x5470)
// Returns: 0=success, 2=error, 8=data corrupt
s32 func_020255bc(void) {
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r4;

    data_02071cf0.unk_14 = func_020246d4(0x3444);

    data_02071cf0.lockID = OS_GetLockID();
    if (data_02071cf0.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(data_02071cf0.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        temp_r0_2 = func_020254ec(0);
        if (temp_r0_2 != 0) {
            temp_r0_3 = func_020254ec(0x5470);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 == 2) || (temp_r0_3 == 2)) {
                    var_r4 = 2;
                } else {
                    var_r4 = 8;
                }
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c(data_02071cf0.lockID);
    func_020389c0(data_02071cf0.lockID);
    func_0202470c(data_02071cf0.unk_14);
    return var_r4;
}

// ============ func_020256bc ============
// Wrapper function to load save data
void func_020256bc(void) {
    func_020255bc();
}

// ============ func_020256c8 ============
// Loads backup save data from a specific slot into buffer
// arg0: destination buffer, arg1: slot offset (0x3444 or 0x88B4)
// Returns: 0=success, 1=checksum fail, 8=data corrupt
s32 func_020256c8(void* arg0, s32 arg1) {
    s32   temp_r0;
    s32   temp_r1;
    s32   temp_r2;
    s32   temp_r3;
    s32   var_ip;
    u16*  var_r4;
    void* var_r5;

    var_r5 = arg0;
    func_0204285c(arg1, data_02071cf0.unk_14, 0x202C, 0, 1, 6, 1, 0, 0);
    func_02042aa4();
    func_02042330();
    var_r4 = (u16*)data_02071cf0.unk_14;
    if (func_020258bc(var_r4 + 0x2008 / 2, data_0205c96c, 0x1B, 0x2008) != 0) {
        if (func_0202593c(var_r4, 0x202C) == 0) {
            var_ip = 0x200;
            do {
                temp_r0 = *(s32*)((s32)var_r4 + 0);
                temp_r1 = *(s32*)((s32)var_r4 + 4);
                temp_r2 = *(s32*)((s32)var_r4 + 8);
                temp_r3 = *(s32*)((s32)var_r4 + 12);
                var_r4 += 8;
                *(s32*)((s32)var_r5 + 0)  = temp_r0;
                *(s32*)((s32)var_r5 + 4)  = temp_r1;
                *(s32*)((s32)var_r5 + 8)  = temp_r2;
                *(s32*)((s32)var_r5 + 12) = temp_r3;
                var_r5                    = (void*)((s32)var_r5 + 0x10);
                var_ip -= 1;
            } while (var_ip != 0);
            *(s32*)((s32)var_r5 + 0) = *(s32*)((s32)var_r4 + 0);
            *(s32*)((s32)var_r5 + 4) = *(s32*)((s32)var_r4 + 4);
            return 0;
        }
        return 8;
    }
    return 1;
}

// ============ func_0202579c ============
// Loads backup save data (checks both slots 0x3444 and 0x88B4)
// arg0: destination buffer for friend data
// Returns: 0=success, 2=error, 8=data corrupt
s32 func_0202579c(void* arg0) {
    s32 var_r4;

    data_02071cf0.unk_10 = func_020246d4(0x202C);

    data_02071cf0.lockID = OS_GetLockID();
    if (data_02071cf0.lockID == -3) {
        OS_WaitForever();
    }
    func_0204237c(data_02071cf0.lockID);

    func_0204296c(0x1001);
    if (func_02042330() != 0) {
        var_r4 = 2;
    } else if ((func_02042944() & 0xFF) == 1) {
        s32 temp_r0_2 = func_020256c8(arg0, 0x3444);
        if (temp_r0_2 != 0) {
            s32 temp_r0_3 = func_020256c8(arg0, 0x88B4);
            if (temp_r0_3 != 0) {
                if ((temp_r0_2 == 2) || (temp_r0_3 == 2)) {
                    var_r4 = 2;
                } else {
                    var_r4 = 8;
                }
            } else {
                var_r4 = 0;
            }
        } else {
            var_r4 = 0;
        }
    } else {
        var_r4 = 2;
    }
    func_0204238c(data_02071cf0.lockID);
    func_020389c0(data_02071cf0.lockID);
    func_0202470c(data_02071cf0.unk_10);
    return var_r4;
}

u16 func_020258ac(void) {
    return data_02071cf0.unk_02;
}

s32 func_020258bc(u8* arg0, const u8* arg1, s32 arg2, s32 arg3) {
    // Verify checksums match
    for (u16 i = 0; i < arg2; i++) {
        if (arg0[i] != arg1[i]) {
            return 0;
        }
    }

    // Verify nibble flags match
    for (u16 i = 0; i < 4; i++) {
        u8 actual   = *(u8*)(arg0[i] + 27);
        u8 expected = (u8)((arg3 >> (i * 4)) & 0xFF);
        if (expected != actual) {
            return 0;
        }
    }

    return 1;
}

u16 func_0202593c(u16* arg0, s16 arg1) {
    u32 checksum = 0;

    while (arg1 > 1) {
        checksum += *arg0;
        arg0++;
        arg1 -= 2;
    }

    if (arg1 > 0) {
        checksum += (u8)*arg0;
    }

    checksum = (checksum & 0xFFFF) + (checksum >> 16);
    checksum = (checksum & 0xFFFF) + (checksum >> 16);

    return ~checksum;
}

s32 func_0202599c(s32 arg0) {
    return (data_02071cf0.unk_20.unk_246C & (1LL << arg0)) != 0;
}

// Nonmatching
// Scratch: vuc2a
s32 func_020259e4(s32 arg0) {
    s32  var_r3 = (arg0 + ((u32)(arg0 >> 4) >> 0x1B)) >> 5;
    u32* var_r1 = &data_02071cf0.unk_20.unk_2474;

    if (var_r3 > 1) {
        var_r1 = &data_02071cf0.unk_20.unk_247C;
        var_r3 -= 2;
    }

    if (var_r3 != 0) {
        var_r1 += 1;
    }

    return *var_r1 & (1 << ((arg0 >> 0x1F) + (((arg0 << 0x1B) - (arg0 >> 0x1F)) >> 0x1B)));
}

s32 func_02025a2c(s32 arg0) {
    s16 retval = 0;

    for (u32 i = 0; i < 67; i++) {
        u16 flags = data_0205c5be[i * 7 + 6];

        if ((flags & 2) || ((flags & 1) && func_02023208(686, 1, 0) == 0)) {
            continue;
        }

        if (*(s16*)(data_0205c5be + i * 7 + 0) <= arg0) {
            if (arg0 <= *(s16*)(data_0205c5be + i * 7 + 1)) {
                if (func_020259e4(i) == 0) {
                    retval += 1;
                }
            }
        }
    }

    return retval;
}

void func_02025ac4(MainData* arg0) {
    arg0->unk_246C = 0;
    arg0->unk_2474 = 0;
    arg0->unk_2478 = 0;
    arg0->unk_247C = 0;
    arg0->unk_2480 = 0;
    arg0->unk_3106 = 0;
    arg0->unk_3108 = -1;
    arg0->unk_3112 = 0;
    arg0->unk_3114 = 0;
    arg0->unk_3116 = 0;
    arg0->unk_3118 = 0;
    arg0->unk_311A = 0;
    arg0->unk_311C = 0;
    arg0->unk_3124 = -1;
    arg0->unk_312C = 1;
}

void func_02025b1c(void) {
    data_02071cf0.unk_20.unk_2484 = 1;
    data_02071cf0.unk_20.unk_3130 = 0;
    data_02071cf0.unk_20.unk_3238 = 127;
    data_02071cf0.unk_20.unk_3234 = 0;
    data_02071cf0.unk_20.unk_313C = 0;
    Savefile_ResetAllGameplay(&data_02071cf0.unk_20);
    func_02025ac4(&data_02071cf0.unk_20);
}