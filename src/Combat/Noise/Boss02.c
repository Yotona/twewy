#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Noise/Boss03.h"
#include "SpriteMgr.h"

extern void func_0200d858(void*, s32, s32, void*);
extern void func_0200d954(s32, s32);
extern void func_02026590(void*, s32, u16);
extern s32  func_02036974();

void func_ov017_02125c6c(void* arg0);
void func_ov017_02125e5c(void* arg0);
void func_ov017_02125fb4(void* arg0);
void func_ov017_02128568(void* arg0);
void func_ov017_02128664(void* arg0);
void func_ov017_02128d2c(void* arg0);
void func_ov017_02128f98(void* arg0);
void func_ov017_02128ebc(void* arg0);
void func_ov017_02128de0(void* arg0);
void func_ov017_0212928c(void* arg0);
void func_ov017_021298e0(void* arg0);
void func_ov017_0212a270(void* arg0);
void func_ov017_0212a37c(void* arg0);
void func_ov017_0212ac58(void* arg0);
void BtlBoss02_Badge_CreateTask(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5);
void func_ov017_0212aea4(void* arg0);
void func_ov017_0212b660(void* arg0);
void func_ov017_0212bf3c(void* arg0);
void func_ov017_0212bee0(void* arg0);
void func_ov017_0212c698(void* arg0);
void func_ov017_0212c508(void* arg0);
void func_ov017_0212c5b8(void* arg0);
void func_ov017_0212c628(void* arg0);
void func_ov017_0212da28();
void func_ov017_0212dbd8(void* arg0);
void func_ov017_0212df88(void* arg0);
void func_ov017_0212e6b8(void* arg0);
void func_ov017_0212e740(void* arg0);
void func_ov017_0212e97c(void* arg0);
void func_ov017_0212ebc0(void* arg0);
void func_ov017_0212eaec(void* arg0);
void func_ov017_0212ea24(void* arg0);
void func_ov017_0212f280(void* arg0);
void func_ov017_0212f18c(void* arg0);
void func_ov017_0212fbc4();
void func_ov017_0212fc70();
void func_ov017_0212fd18(void* arg0);
void func_ov017_0212fe1c(void* arg0);
void func_ov017_0212ff54(CombatSprite* arg0, CombatSprite* arg1);
void func_ov017_0212ffcc(s32, s32);
void func_ov017_0212ffe4(s32, s32, s32);
void func_ov017_02130130(void*, void*);
void func_ov017_02130148();
void func_ov017_0213015c();
void func_ov017_02130178(s32, s32, s32, s32);
void func_ov017_021301e0();
void func_ov017_0213021c(void*);
void func_ov017_02130280(void*);

extern s32 data_0205e4e0;
extern s32 data_02071cf0;

extern s32 data_ov003_020e68d0;

const BinIdentifier     data_ov017_02141520 = {3, "Apl_Mot/GRP_BTLBOSS02_EFF.bin"};
extern SubroutineArgs   data_ov017_02141538;
extern s32              data_ov017_0214153c;
extern s32              data_ov017_02141540;
extern SpriteAnimEntry  data_ov017_021415a8;
extern s16              data_ov017_0214398c;
extern s32              data_ov017_0214399c;
extern TaskHandle       data_ov017_02141f30;
extern u16              data_ov017_02143980;
extern SubroutineArgs   data_ov017_0214165c;
extern s32              data_ov017_0214166c;
extern Boss03DataStruct data_ov017_02143a18;
extern BinIdentifier    data_ov017_02141510;
extern BinIdentifier    data_ov017_02141528;
extern s32              data_ov017_02141684;
extern s32              data_ov017_02141686;
extern s32              data_ov017_021416a4;
extern s32              data_ov017_021416cc;
extern SubroutineArgs   data_ov017_02141674;
extern Boss03DataStruct data_ov017_02143988;
extern s32              data_ov017_021439b0;
extern s32              data_ov017_021439a4;
extern s32              data_ov017_02142df4;
extern s32              data_ov017_02142e0c;
extern s32              data_ov017_02141a44;
extern s32              data_ov017_0214186c;
extern s32              data_ov017_0214179c;
extern s32              data_ov017_02141804;
extern s32              data_ov017_0214172c;
extern s32              data_ov017_02141764;
extern s32              data_ov017_0214171c;
extern BinIdentifier    data_ov017_02141724;
extern SubroutineArgs   data_ov017_0214170c;
extern u16              data_ov017_021439d6;
typedef struct {
    u16 unk0;
    u16 unk2;
} Boss03_SmallPair;
extern Boss03_SmallPair data_ov017_021439d4;
extern SubroutineArgs   data_ov017_02141c38;
extern TaskHandle       data_ov017_02141f4c;
extern BinIdentifier    data_ov017_02141518;
extern SpriteAnimEntry  data_ov017_02141568;
extern SubroutineArgs   data_ov017_02141c54;
extern Boss03DataStruct data_ov017_02141c64;
extern s32              data_ov017_02141c88;
extern s32              data_ov017_02141cb0;
extern s32              data_ov017_02141cd8;
extern s32              data_ov017_02141d40;
extern s32              data_ov017_02141c68;
extern SubroutineArgs   data_ov017_02141c78;
extern s32              data_ov017_02143a04;
extern TaskHandle       data_ov017_02141f94;
extern u16              data_ov017_02143a00;
typedef struct {
    u16    unk0;
    u16    unk2;
    void** unk4;
    void** unk8;
    s32    unkC;
} Boss03_AnimState;
extern Boss03_AnimState data_ov017_021439dc;
extern SubroutineArgs   data_ov017_02141dd0;
extern void*            data_ov017_021439e0;
extern s16              data_ov017_021439de;
extern s32              data_ov017_02142f84;
extern s32              data_ov017_021439d8;
extern TaskHandle       data_ov017_02141dfc;
extern void*            data_ov017_02143984;
extern SubroutineArgs   data_ov017_02141dec;
extern Boss03DataStruct data_ov017_02141e18;
extern SubroutineArgs   data_ov017_02141e08;
extern u16              data_ov017_021439ec;
extern TaskHandle       data_ov017_02141e84;
extern void**           data_ov017_021439f0;
extern SubroutineArgs   data_ov017_02141e58;
extern TaskHandle       data_ov017_02141ea0;
extern SubroutineArgs   data_ov017_02141e74;
extern SubroutineArgs   data_ov017_02141e90;
extern SubroutineArgs   data_ov017_02141eac;
extern s32              data_ov017_02141ebc;
extern SubroutineArgs   data_ov017_02141ec4;
extern s32              data_ov017_02141ee0;
extern SubroutineArgs   data_ov017_02141ee8;
extern SubroutineArgs   data_ov017_021439f4;
extern SubroutineArgs   data_ov017_02141f20;
extern SubroutineArgs   data_ov017_02141f58;
extern SubroutineArgs   data_ov017_02141f3c;
extern SubroutineArgs   data_ov017_02141fa0;
extern SubroutineArgs   data_ov017_02141fbc;
extern SubroutineArgs   data_ov017_02141fd8;
extern s32              data_ov017_02141f68;
extern SubroutineArgs   data_ov017_02142004;
extern BinIdentifier    data_ov017_02141530;
extern SubroutineArgs   data_ov017_02142020;
extern s32              data_ov017_02142030;

void func_ov017_021256c0(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

s16 func_ov017_021256e4(void* arg0) {
    do {
    } while (((Boss03*)arg0)->unk260 == RNG_Next(4));
}

void func_ov017_02125708(void* arg0) {
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;

    temp_r1 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk260 * 0xC));
    ((Boss03*)arg0)->unk28  = temp_r1;
    ((Boss03*)arg0)->unk1AC = temp_r1;
    temp_r1_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk260 * 0xC));
    ((Boss03*)arg0)->unk2C  = temp_r1_2;
    ((Boss03*)arg0)->unk1B0 = temp_r1_2;
    temp_r1_3               = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk260 * 0xC));
    ((Boss03*)arg0)->unk30  = temp_r1_3;
    ((Boss03*)arg0)->unk1B4 = temp_r1_3;
}

void func_ov017_02125794(void* arg0) {
    u32 temp_r0;
    u32 temp_r2;

    CombatSprite_LoadFromTable(1, (CombatSprite*)arg0 + 0x1D8, &data_ov017_02141520, &data_ov017_021415a8, 0x10, 7, 0x80);
    ((Boss03*)arg0)->unk238 = (s32)(arg0 + 0x1D8);
    temp_r0                 = data_ov003_020e71b8->unk3D824;
    ((Boss03*)arg0)->unk23C = (s32)((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1);
    temp_r2                 = data_ov003_020e71b8->unk3D828;
    ((Boss03*)arg0)->unk240 = (s32)((s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1);
    ((Boss03*)arg0)->unk244 = 0x40000;
    func_ov003_020c3acc(1, 0x2FA, ((Boss03*)arg0)->unk23C);
    ((Boss03*)arg0)->unk248 = 1;
}

void func_ov017_02125840(void* arg0) {
    s32 var_r1;

    if (((Boss03*)arg0)->unk238 == 0) {
        return;
    }
    ((Boss03*)arg0)->unk244 = (s32)(((Boss03*)arg0)->unk244 - 0x8000);
    CombatSprite_Update(arg0 + 0x1D8);
    var_r1 = ((Boss03*)data_ov003_020e71b8->unk3D89C)->unk30 - ((Boss03*)arg0)->unk244;
    if (var_r1 < 0) {
        var_r1 = 0 - var_r1;
    }
    if ((((Boss03*)arg0)->unk248 == 1) && ((s32)(var_r1 >> 0xC) <= 0x10) &&
        (func_ov003_0208a1e8(func_ov003_0208a114(0xF0), arg0 + 0x1D0, ((Boss03*)arg0)->unk23C) != 0))
    {
        func_ov017_02130178(1, 0x4E8, 0x56C, ((Boss03*)arg0)->unk28);
        ((Boss03*)arg0)->unk248 = 0;
    }
    if ((s32)((Boss03*)arg0)->unk244 > -0x100000) {
        return;
    }
    CombatSprite_Release(arg0 + 0x1D8);
    ((Boss03*)arg0)->unk238 = 0;
}

void func_ov017_02125908(void* arg0) {
    s32 temp_r4;

    if (((Boss03*)arg0)->unk238 == 0) {
        return;
    }
    temp_r4 = func_ov003_020843b0(1, ((Boss03*)arg0)->unk23C);
    CombatSprite_SetPosition(arg0 + 0x1D8, temp_r4, func_ov003_020843ec(1, ((Boss03*)arg0)->unk240, ((Boss03*)arg0)->unk244));
    func_ov003_02082730(arg0 + 0x1D8, 0x7FFFFFFE - ((Boss03*)arg0)->unk240);
    CombatSprite_Render(arg0 + 0x1D8);
}

void func_ov017_0212596c(void* arg0) {
    if (((Boss03*)arg0)->unk238 == 0) {
        return;
    }
    CombatSprite_Release(((Boss03*)arg0)->unk238);
}

void func_ov017_02125984(void* arg0) {
    s16 temp_r0;
    s16 temp_r5;

    if (((Boss03*)arg0)->unk25C == 0) {
        return;
    }
    temp_r0 = ((Boss03*)arg0)->unk6E;
    temp_r5 = ((u32)(temp_r0 * 3) >> 0x1F) + ((temp_r0 * 3) / 100);
    func_ov017_02130178(1, 0x4E6, 0x56C, ((Boss03*)arg0)->unk28);
    func_ov003_0208b518(1, arg0, temp_r5);
    func_ov003_0209b564(1, temp_r5, arg0, 1);
}

void func_ov017_021259fc(void* arg0) {
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40);
    func_ov017_02130178(1, 0x4E7, 0x56C, ((Boss03*)arg0)->unk28);
    func_ov003_0208a128(func_ov003_0208a114(0xF1), arg0 + 0x1D0, arg0, ((Boss03*)arg0)->unk28);
    ((Boss03*)arg0)->unk262 = 0x78;
    ((Boss03*)arg0)->unk24C = 1;
}

void func_ov017_02125a58(void* arg0) {
    s32 var_r5;
    s32 var_r7;
    s32 var_r7_2;

    if (((Boss03*)arg0)->unk238 == 0) {
        var_r5 = 0;
    loop_2:
        if ((&data_ov017_0214398c)[var_r5] == 0) {
            func_ov017_02125794(arg0);
            (&data_ov017_0214398c)[var_r5] = -1;
        } else {
            var_r5 += 1;
            if (var_r5 >= 1) {

            } else {
                goto loop_2;
            }
        }
    } else {
        func_ov017_02125840(arg0);
    }
    var_r7 = 0;
    do {
        if (*(s16*)((char*)&data_ov017_0214399c + (var_r7 * 2)) == 1) {
            func_ov017_02125984(arg0);
            *(s16*)((char*)&data_ov017_0214399c + (var_r7 * 2)) = -1;
        }
        var_r7 += 1;
    } while (var_r7 < 3);
    var_r7_2 = 0;
    do {
        if (*(s16*)((char*)&data_ov017_0214399c + (var_r7_2 * 2)) == 2) {
            func_ov017_021259fc(arg0);
            *(s16*)((char*)&data_ov017_0214399c + (var_r7_2 * 2)) = -1;
        }
        var_r7_2 += 1;
    } while (var_r7_2 < 3);
}

void func_ov017_02125b30(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_021256c0(arg0, func_ov017_02125c6c);
}

void func_ov017_02125b58(void* arg0) {
    s16 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    s16 var_r1;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 6, 1);
    }
    if (((Boss03*)arg0)->unk1C0 == 0x30) {
        sp8  = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk260 * 0xC));
        spC  = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk260 * 0xC));
        sp10 = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk260 * 0xC));
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        sp14 = var_r1;
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f30, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    func_ov017_021256c0(arg0, func_ov017_02125c6c);
}

void func_ov017_02125c6c(void* arg0) {
    s16 temp_r1_2;
    s32 temp_r1;

    func_ov003_020c4b1c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 >= (s32)((Boss03*)arg0)->unk1C4) {
        if (func_ov003_020c4348(arg0) != 0) {
            func_ov017_021256c0(arg0, func_ov017_02125b58);
        } else {
            ((Boss03*)arg0)->unk260 = (s16)func_ov017_021256e4(arg0);
            func_ov017_02125708(arg0);
            ((Boss03*)arg0)->unk262 = 0x24;
            func_ov017_021256c0(arg0, func_ov017_02125c6c);
        }
    }
    temp_r1_2 = ((Boss03*)arg0)->unk262;
    if ((s32)temp_r1_2 > 0) {
        ((Boss03*)arg0)->unk262 = (s16)(temp_r1_2 - 1);
    }
    if (((Boss03*)arg0)->unk262 != 1) {
        return;
    }
    if (RNG_Next(0x64) < 0x32U) {
        func_ov017_021256c0(arg0, func_ov017_02125b58);
        ((Boss03*)arg0)->unk262 = 0;
    }
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x40);
}

void func_ov017_02125d6c(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk84 = (s32)((((Boss03*)arg0)->unk84 & ~0x60) | 0x40);
        if (((Boss03*)arg0)->unk250 == 1) {
            func_ov017_02130178(1, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(1, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (((Boss03*)arg0)->unk24C == 0) {
        if (func_ov003_020c62c4(arg0, 3) != 0) {
            return;
        }
        func_ov017_021256c0(arg0, func_ov017_02125c6c);
        ((Boss03*)arg0)->unk262 = 0;
        ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x40);
        return;
    }
    func_ov017_021256c0(arg0, func_ov017_02125e5c);
}

void func_ov017_02125e38(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 != 0) {
        return;
    }
    ((Boss03*)arg0)->unk25C = 0;
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_02125e5c(void* arg0) {
    u32 temp_r0;

    temp_r0 = ((Boss03*)arg0)->unk1C8;
    switch (temp_r0) {
        default:
            return;
        case 0:
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 3, 1);
            func_ov003_020c4b1c(arg0);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 4, 0);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 2:
            ((Boss03*)arg0)->unk262 = (s16)(((Boss03*)arg0)->unk262 - 1);
            if (((Boss03*)arg0)->unk262 != 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 5, 1);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_021256c0(arg0, func_ov017_02125c6c);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
    }
}

void func_ov017_02125f48(void* arg0) {
    ((Boss03*)arg0)->unk260 = (s16)func_ov017_021256e4(arg0);
    func_ov017_02125708(arg0);
    ((Boss03*)arg0)->unk262 = 0x24;
    ((Boss03*)arg0)->unk54  = (s32)((((Boss03*)arg0)->unk54 & ~0x40) | 0x10);
    ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
    func_ov017_021256c0(arg0, func_ov017_02125fb4);
    func_ov003_020c3acc(1, 0x2FF, ((Boss03*)arg0)->unk28);
}

void func_ov017_02125fb4(void* arg0) {
    s32 temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
    }
    temp_r0                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r0;
    if (temp_r0 <= 0x1E) {
        return;
    }
    func_ov003_020c3acc(1, 0x300, ((Boss03*)arg0)->unk28);
    ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x10);
    ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C & ~1);
    func_ov017_021256c0(arg0, func_ov017_02125c6c);
}

void func_ov017_0212602c(void* arg0) {
    s32   temp_r0;
    void* temp_r0_2;

    temp_r0 = ((Boss03*)arg0)->unk1C0;
    if (temp_r0 == 0) {
        ((Boss03*)arg0)->unk1C0 = (s32)(temp_r0 + 1);
    }
    temp_r0_2              = func_ov003_020c72b4(arg0, 0, 3);
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 & ~0x40);
    ((Boss03*)arg0)->unk38 = 0;
    ((Boss03*)arg0)->unk3C = 0;
    if (temp_r0_2 != NULL) {
        return;
    }
    ((Boss03*)arg0)->unk262 = 0xA;
    func_ov017_021256c0(arg0, func_ov017_02125c6c);
}

s32 func_ov017_02126094(void* arg1, s32 arg2) {
    s32   temp_r0;
    s32   temp_r1;
    s32   temp_r2;
    s32   var_r3;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x264);
    func_ov003_020c3efc(temp_r4, arg2);
    data_ov017_02143980        = (s16)((Boss03*)temp_r4)->unk7C;
    ((Boss03*)temp_r4)->unk254 = 1;
    ((Boss03*)temp_r4)->unk25C = 1;
    ((Boss03*)temp_r4)->unk262 = 0;
    ((Boss03*)temp_r4)->unk260 = 0;
    ((Boss03*)temp_r4)->unk238 = 0;
    ((Boss03*)temp_r4)->unk24C = 0;
    ((Boss03*)temp_r4)->unk1D0 = 0x64;
    ((Boss03*)temp_r4)->unk1D4 = 0x64;
    ((Boss03*)temp_r4)->unk1D6 = 0x64;
    temp_r0                    = data_ov003_020e71b8->unk3D838 + data_ov017_02141538.unk0;
    ((Boss03*)temp_r4)->unk28  = temp_r0;
    ((Boss03*)temp_r4)->unk1AC = temp_r0;
    temp_r2                    = data_ov003_020e71b8->unk3D83C + data_ov017_02141538.unk4;
    ((Boss03*)temp_r4)->unk2C  = temp_r2;
    ((Boss03*)temp_r4)->unk1B0 = temp_r2;
    ((Boss03*)temp_r4)->unk30  = (s32)data_ov017_02141538.unk8;
    ((Boss03*)temp_r4)->unk1B4 = (s32)data_ov017_02141538.unk8;
    ((Boss03*)temp_r4)->unk38  = 0;
    ((Boss03*)temp_r4)->unk3C  = 0;
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov017_021256c0(temp_r4, func_ov017_02125b30);
    data_ov017_0214398c = -1;
    var_r3              = 0;
    do {
        temp_r1 = var_r3 * 2;
        var_r3 += 1;
        *(&data_ov017_0214399c + temp_r1) = -1;
    } while (var_r3 < 3);
    ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000040);
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x200000);
    func_ov017_02130148();
    return 1;
}

s32 func_ov017_021261f4(void* arg1) {
    void (*temp_r1_2)(void*);
    s16   temp_lr;
    u16   temp_r1;
    u32   temp_r0;
    void* temp_r4;

    temp_lr = data_ov003_020e71b8->unk3D8DA;
    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((s32)data_ov003_020e71b8->unk3D8D8 < (s32)(((u32)(temp_lr * 0x4B) >> 0x1F) + ((temp_lr * 0x4B) / 100))) &&
        (((Boss03*)temp_r4)->unk1CC != (void (*)(void*))func_ov017_02125e38))
    {
        ((Boss03*)temp_r4)->unk20     = 3;
        data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
        func_ov017_021301e0();
    }
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk250 = temp_r0;
    switch (temp_r0) {
        default:
            break;
        case 1:
        case 2:
            func_ov017_021256c0(temp_r4, func_ov017_02125d6c);
            break;
        case 7:
            func_ov017_021256c0(temp_r4, func_ov017_02125f48);
            break;
        case 3:
            temp_r1 = ((Boss03*)temp_r4)->unk18C;
            if (temp_r1 & 0x10) {
                ((Boss03*)temp_r4)->unk18C = (u16)(temp_r1 | 0x20);
            } else {
                func_ov017_021256c0(temp_r4, func_ov017_02125e38);
            }
            break;
        case 6:
            func_ov017_021256c0(temp_r4, func_ov017_0212602c);
            break;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    ((Boss03*)temp_r4)->unk24C = 0;
    func_ov017_02125a58(temp_r4);
    func_ov003_020c4628(temp_r4);
    func_ov017_0213015c();
    return ((Boss03*)temp_r4)->unk254;
}

s32 func_ov017_02126354(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov017_02125908(temp_r4);
    func_ov003_020c4878(temp_r4);
    return 1;
}

s32 func_ov017_02126374(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov017_0212596c(temp_r4);
    func_ov003_020c48fc(temp_r4);
    return 1;
}

void func_ov017_02126394(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_0214165c.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_0214165c.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_0214165c.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_0214165c.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_021263dc(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unkDC = (s32)arg1;
    ((Boss03*)arg0)->unkDA = 0;
    ((Boss03*)arg0)->unkD8 = 0;
}

void func_ov017_021263f0(void* arg0) {
    s32   temp_r1;
    s32   temp_r1_3;
    s32   temp_r2;
    s32   temp_r2_2;
    u32   temp_r0;
    u32   temp_r3;
    void* temp_ip;
    void* temp_lr;
    void* temp_r0_2;
    void* temp_r1_2;

    temp_r2 = ((Boss03*)arg0)->unk2C;
    temp_r1 = (((Boss03*)arg0)->unk28 * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_ip = ((Boss03*)arg0)->unk0;
    temp_r3 = *(u32*)(temp_r1 + (temp_r2 * 0x3C));
    switch (temp_r3) { /* switch 1 */
        default:       /* switch 1 */
            break;
        case 1:        /* switch 1 */
        case 2:        /* switch 1 */
        case 3:        /* switch 1 */
        case 4:        /* switch 1 */
        case 5:        /* switch 1 */
            ((Boss03*)((temp_r2 * 0x2C) + temp_r1))->unk124 = 1;
            break;
    }
    temp_r1_2                   = temp_r1 + (temp_r2 * 8);
    ((Boss03*)temp_r1_2)->unkF0 = (s32)(((Boss03*)temp_ip)->unk28 - 0x80000);
    ((Boss03*)temp_r1_2)->unkF4 = (s32)(((Boss03*)temp_ip)->unk40 - 0x60000);
    temp_r2_2                   = ((Boss03*)arg0)->unk54;
    temp_r1_3                   = (((Boss03*)arg0)->unk50 * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_lr                     = ((Boss03*)arg0)->unk0;
    temp_r0                     = *(u32*)(temp_r1_3 + (temp_r2_2 * 0x3C));
    switch (temp_r0) { /* switch 2 */
        default:       /* switch 2 */
            break;
        case 1:        /* switch 2 */
        case 2:        /* switch 2 */
        case 3:        /* switch 2 */
        case 4:        /* switch 2 */
        case 5:        /* switch 2 */
            ((Boss03*)((temp_r2_2 * 0x2C) + temp_r1_3))->unk124 = 1;
            break;
    }
    temp_r0_2                   = temp_r1_3 + (temp_r2_2 * 8);
    ((Boss03*)temp_r0_2)->unkF0 = (s32)(((Boss03*)temp_lr)->unk28 - 0x80000);
    ((Boss03*)temp_r0_2)->unkF4 = (s32)(((Boss03*)temp_lr)->unk40 - 0x60000);
}

void func_ov017_021264e4(void* arg0) {
    s32   temp_r1;
    s32*  var_r4;
    u16   temp_r2;
    u32   var_r5;
    void* temp_r0;

    ((Boss03*)arg0)->unkD2 = (u16)(((Boss03*)arg0)->unkD2 + 1);
    if ((u32)((Boss03*)arg0)->unkD2 < 4U) {
        return;
    }
    ((Boss03*)arg0)->unkD2 = 0U;
    ((Boss03*)arg0)->unkD0 = (u16)(((Boss03*)arg0)->unkD0 + 1);
    if ((u32)((Boss03*)arg0)->unkD0 >= 8U) {
        ((Boss03*)arg0)->unkD0 = 0U;
    }
    if (((Boss03*)arg0)->unk18 != NULL) {
        DatMgr_ReleaseData(((Boss03*)arg0)->unk18);
    }
    temp_r0                = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c,
                                                  (s32) * (&data_ov017_02141684 + (((Boss03*)arg0)->unkD0 * 4)), 0);
    ((Boss03*)arg0)->unk18 = temp_r0;
    temp_r2                = *(&data_ov017_02141686 + (((Boss03*)arg0)->unkD0 * 4));
    if ((temp_r0 == NULL) || ((s32)temp_r2 <= 0)) {
        var_r4 = NULL;
    } else {
        temp_r1 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r4  = (s32*)(temp_r1 + *(s32*)(temp_r1 + (temp_r2 * 8)));
    }
    var_r5 = (u32)(*var_r4 & ~0xFF) >> 8;
    if (!((u8)*var_r4 & 0xF0)) {
        var_r5 -= 4;
    }
    func_02001b44(7, func_02036974() + 0x7800, var_r4 + 4, var_r5);
}

void func_ov017_021265e4(void* arg0) {
    s16 temp_r1;

    if (((Boss03*)arg0)->unkD8 == 0) {
        (*(Boss03**)arg0)->unk54 = -0x48000;
        ((Boss03*)arg0)->unkD8   = 0x64;
    }
    temp_r1 = ((Boss03*)arg0)->unkD8;
    if ((s32)temp_r1 > 0) {
        func_02026590((u8*)(*(void**)arg0) + 0x54, 0, (u16)temp_r1);
        ((Boss03*)arg0)->unkD8 = (s16)(((Boss03*)arg0)->unkD8 - 1);
    }
    if ((s32)((Boss03*)arg0)->unkD8 > 0) {
        return;
    }
    func_ov017_021263dc(arg0, NULL);
}

s32 func_ov017_0212665c(void* arg1, u16* arg2) {
    void* temp_ip;
    void* temp_r0;
    void* temp_r0_2;
    void* temp_r0_3;
    void* temp_r0_4;
    void* temp_r0_5;
    void* temp_r0_6;
    void* temp_r1;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_4;
    void* temp_r1_5;
    void* temp_r2;
    void* temp_r2_2;
    void* temp_r2_3;
    void* temp_r3;
    void* temp_r3_2;
    void* temp_r3_3;
    void* temp_r4;
    void* var_r0;
    void* var_r0_2;
    void* var_r0_3;
    void* var_r0_4;
    void* var_r1;
    void* var_r1_2;
    void* var_r5;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0xE0);
    func_ov017_0212fbc4();
    temp_ip                   = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D818));
    ((Boss03*)temp_r4)->unk0  = temp_ip;
    ((Boss03*)temp_ip)->unk2  = (u16)(((Boss03*)temp_ip)->unk2 | 1);
    ((Boss03*)temp_r4)->unkD4 = 0;
    ((Boss03*)temp_r4)->unkD0 = 0;
    ((Boss03*)temp_r4)->unkD2 = 0;
    temp_r0                   = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c, 1, 0);
    ((Boss03*)temp_r4)->unk8  = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r2 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r1  = temp_r2 + ((Boss03*)temp_r2)->unk10;
    }
    if (temp_r0 == NULL) {
        var_r5 = NULL;
    } else {
        temp_r2_2 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r5    = temp_r2_2 + ((Boss03*)temp_r2_2)->unk8;
    }
    ((Boss03*)temp_r4)->unk20 = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1,
                                                     g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x1CC0);
    ((Boss03*)temp_r4)->unk24 = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r5,
                                                     g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x7CC0);
    DMA_Flush();
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk8);
    temp_r0_2                = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c, 2, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0_2;
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1  = ((Boss03*)temp_r0_2)->unk8 + 0x20;
        var_r1_2 = temp_r1 + ((Boss03*)temp_r1)->unk8;
    }
    ((Boss03*)temp_r4)->unk1C = PaletteMgr_AllocPalette(g_PaletteManagers[1], var_r1_2, 0, 0, 0xF);
    ((Boss03*)temp_r4)->unk8  = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c, 0xD, 0);
    ((Boss03*)temp_r4)->unkC  = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c, 0x11, 0);
    temp_r0_3                 = ((Boss03*)temp_r4)->unk8;
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_2 = ((Boss03*)temp_r0_3)->unk8 + 0x20;
        var_r0    = temp_r1_2 + ((Boss03*)temp_r1_2)->unk8;
    }
    ((Boss03*)temp_r4)->unk78 = (void*)(var_r0 + 4);
    temp_r0_4                 = ((Boss03*)temp_r4)->unkC;
    if (temp_r0_4 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_3 = ((Boss03*)temp_r0_4)->unk8 + 0x20;
        var_r0_2  = temp_r1_3 + ((Boss03*)temp_r1_3)->unk8;
    }
    ((Boss03*)temp_r4)->unk7C = (void*)(var_r0_2 + 4);
    func_0200d1d8(temp_r4 + 0x28, 1, 1, 0, temp_r4 + 0x78, 2, 1);
    func_0200d858(temp_r4 + 0x28, 0, 1, 0);
    func_ov003_020d3b6c(temp_r4 + 0x88, ((Boss03*)temp_r4)->unk8, 0xD, 1, 0, 0);
    func_ov003_020d3b6c(temp_r4 + 0xAC, ((Boss03*)temp_r4)->unkC, 0x11, 1, 0, 0);
    func_ov003_020d3bec(temp_r4 + 0x88, &data_ov017_0214166c, temp_r4 + 0x28, temp_r4 + 0x78, &data_ov017_021416a4);
    func_ov003_020d3bec(temp_r4 + 0xAC, &data_ov017_0214166c, temp_r4 + 0x28, temp_r4 + 0x7C, &data_ov017_021416cc);
    func_ov003_020d3c98(temp_r4 + 0x88, temp_r4 + 0xAC);
    ((Boss03*)temp_r4)->unk10 = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c, 0xB, 0);
    ((Boss03*)temp_r4)->unk14 = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214166c, 0xC, 0);
    temp_r0_5                 = ((Boss03*)temp_r4)->unk10;
    if (temp_r0_5 == NULL) {
        var_r0_3 = NULL;
    } else {
        temp_r1_4 = ((Boss03*)temp_r0_5)->unk8 + 0x20;
        var_r0_3  = temp_r1_4 + ((Boss03*)temp_r1_4)->unk8;
    }
    ((Boss03*)temp_r4)->unk80 = (void*)(var_r0_3 + 4);
    temp_r0_6                 = ((Boss03*)temp_r4)->unk14;
    if (temp_r0_6 == NULL) {
        var_r0_4 = NULL;
    } else {
        temp_r1_5 = ((Boss03*)temp_r0_6)->unk8 + 0x20;
        var_r0_4  = temp_r1_5 + ((Boss03*)temp_r1_5)->unk8;
    }
    ((Boss03*)temp_r4)->unk84 = (void*)(var_r0_4 + 4);
    func_0200d1d8(temp_r4 + 0x50, 1, 2, 0, temp_r4 + 0x80, 2, 1);
    func_0200d858(temp_r4 + 0x50, 0, 1, 0);
    if (*arg2 == 0) {
        temp_r2_3 = data_ov003_020e71b8->unk3D89C;
        func_ov003_02083ab0(1, ((Boss03*)temp_r2_3)->unk28, ((Boss03*)temp_r2_3)->unk2C, 0);
        temp_r3                     = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3)->unk28   = (s32)((Boss03*)temp_r3)->unk34;
        temp_r3_2                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_2)->unk2C = (s32)((Boss03*)temp_r3_2)->unk38;
        temp_r3_3                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_3)->unk30 = (s32)((Boss03*)temp_r3_3)->unk3C;
        func_ov017_021263dc(temp_r4, func_ov017_021265e4);
    } else {
        func_ov017_021263dc(temp_r4, NULL);
    }
    func_ov017_021263f0(temp_r4);
    return 1;
}

s32 func_ov017_02126a80(void* arg1) {
    void (*temp_r1)(void*);
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unkDC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov017_021264e4(temp_r4);
    func_ov003_020d3cac(temp_r4 + 0x88);
    func_ov017_0212fd18(((Boss03*)temp_r4)->unk0);
    func_ov017_021263f0(temp_r4);
    return 1;
}

s32 func_ov017_02126ae4(void) {
    return 1;
}

s32 func_ov017_02126aec(void* arg1) {
    void* temp_r2;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], ((Boss03*)temp_r4)->unk20);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], ((Boss03*)temp_r4)->unk24);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], ((Boss03*)temp_r4)->unk1C);
    func_0200d954(1, 1);
    func_0200d954(1, 2);
    func_ov003_020d3cf8(temp_r4 + 0xAC);
    func_ov003_020d3cf8(temp_r4 + 0x88);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk18);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk8);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unkC);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk10);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk14);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r2                  = ((Boss03*)temp_r4)->unk0;
    ((Boss03*)temp_r2)->unk2 = (u16)(((Boss03*)temp_r2)->unk2 & ~1);
    return 1;
}

void func_ov017_02126b9c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141674.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141674.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141674.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141674.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_02126be4(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk1D8 = arg1;
    ((Boss03*)arg0)->unk1D4 = 0;
    ((Boss03*)arg0)->unk1D2 = 0;
}

void func_ov017_02126bfc(void* arg0) {
    s32   temp_r1;
    s32   temp_r1_3;
    s32   temp_r2;
    s32   temp_r2_2;
    u32   temp_r0;
    u32   temp_r3;
    void* temp_ip;
    void* temp_lr;
    void* temp_r0_2;
    void* temp_r1_2;

    temp_r2 = ((Boss03*)arg0)->unk20;
    temp_r1 = (((Boss03*)arg0)->unk1C * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_ip = ((Boss03*)arg0)->unk0;
    temp_r3 = *(u32*)(temp_r1 + (temp_r2 * 0x3C));
    switch (temp_r3) { /* switch 1 */
        default:       /* switch 1 */
            break;
        case 1:        /* switch 1 */
        case 2:        /* switch 1 */
        case 3:        /* switch 1 */
        case 4:        /* switch 1 */
        case 5:        /* switch 1 */
            ((Boss03*)((temp_r2 * 0x2C) + temp_r1))->unk124 = 1;
            break;
    }
    temp_r1_2                   = temp_r1 + (temp_r2 * 8);
    ((Boss03*)temp_r1_2)->unkF0 = (s32)(((Boss03*)temp_ip)->unk28 - 0x80000);
    ((Boss03*)temp_r1_2)->unkF4 = (s32)(((Boss03*)temp_ip)->unk40 - 0x60000);
    temp_r2_2                   = ((Boss03*)arg0)->unk48;
    temp_r1_3                   = (((Boss03*)arg0)->unk44 * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_lr                     = ((Boss03*)arg0)->unk0;
    temp_r0                     = *(u32*)(temp_r1_3 + (temp_r2_2 * 0x3C));
    switch (temp_r0) { /* switch 2 */
        default:       /* switch 2 */
            break;
        case 1:        /* switch 2 */
        case 2:        /* switch 2 */
        case 3:        /* switch 2 */
        case 4:        /* switch 2 */
        case 5:        /* switch 2 */
            ((Boss03*)((temp_r2_2 * 0x2C) + temp_r1_3))->unk124 = 1;
            break;
    }
    temp_r0_2                   = temp_r1_3 + (temp_r2_2 * 8);
    ((Boss03*)temp_r0_2)->unkF0 = (s32)(((Boss03*)temp_lr)->unk28 - 0x180000);
    ((Boss03*)temp_r0_2)->unkF4 = (s32)(((Boss03*)temp_lr)->unk40 - 0x60000);
}

void func_ov017_02126cf0(void* arg0) {
    func_ov003_020d3cac(arg0 + 0x7C);
}

s32 func_ov017_02126d00(void* arg1, void* arg2, void* arg3, s32 arg4, s32 arg5) {
    s16   temp_r0;
    s32   var_r4;
    void* var_r3;

    var_r3  = arg3;
    temp_r0 = ((Boss03*)arg2)->unk0;
    if (temp_r0 == 0xD) {
        var_r3 += 0x40000;
    }
    var_r4                = 1;
    ((Boss03*)arg1)->unk4 = func_ov003_020865a0((u16)temp_r0, var_r3, arg4, arg1);
    if (((Boss03*)arg1)->unk0 == -1) {
        var_r4                = 0;
        ((Boss03*)arg1)->unk4 = 0;
        ((Boss03*)arg1)->unk8 = NULL;
    } else {
        ((Boss03*)arg1)->unk8    = arg2;
        data_ov017_02143988.unkA = (s16)(data_ov017_02143988.unkA + ((Boss03*)arg2)->unk4);
        data_ov017_02143988.unk6 = (s16)(data_ov017_02143988.unk6 + 1);
        data_ov017_02143988.unk2 = (s16)(data_ov017_02143988.unk2 + 1);
    }
    return var_r4;
}

void func_ov017_02126d90(void* arg1, void* arg2) {
    data_ov017_02143988.unkA = (s16)(data_ov017_02143988.unkA - (*(Boss03**)((u8*)arg1 + 8))->unk4);
    data_ov017_02143988.unk6 = (s16)(data_ov017_02143988.unk6 - 1);
    data_ov017_02143988.unk2 = (s16)(data_ov017_02143988.unk2 - 1);
    ((Boss03*)arg1)->unk4    = 0;
    ((Boss03*)arg1)->unk8    = NULL;
}

s32 func_ov017_02126dd4(void* arg1, void* arg2, void* arg3, s32 arg4, s32 arg5) {
    s32   sp20;
    s32   sp1C;
    void* sp18;
    u16   sp16;
    s16   sp14;
    s32   sp10;
    s32   spC;
    s32   sp8;
    s16   temp_lr;
    s32   var_r4;

    temp_lr = ((Boss03*)arg2)->unk0;
    sp10    = (s32)temp_lr;
    sp18    = arg3;
    sp1C    = arg4;
    sp16 &= ~1;
    sp14   = ((Boss03*)arg2)->unk2;
    sp20   = 0;
    var_r4 = 1;
    if (func_ov003_020c3d28(0, *(&data_ov003_020e68d0 + (temp_lr * 4)), 0, &sp10, &spC, &sp8) == 0) {
        var_r4                = 0;
        ((Boss03*)arg1)->unk4 = 0;
        ((Boss03*)arg1)->unk8 = NULL;
    } else {
        ((Boss03*)arg1)->unk4    = EasyTask_GetTaskData((TaskPool*)data_ov003_020e71b8, sp8);
        ((Boss03*)arg1)->unk0    = sp8;
        ((Boss03*)arg1)->unk8    = arg2;
        data_ov017_02143988.unkA = (s16)(data_ov017_02143988.unkA + ((Boss03*)arg2)->unk4);
        data_ov017_02143988.unk6 = (s16)(data_ov017_02143988.unk6 + 1);
        data_ov017_02143988.unk8 = (s16)(data_ov017_02143988.unk8 + 1);
    }
    return var_r4;
}

void func_ov017_02126eb4(void* arg1, void* arg2) {
    data_ov017_02143988.unkA = (s16)(data_ov017_02143988.unkA - (*(Boss03**)((u8*)arg1 + 8))->unk4);
    data_ov017_02143988.unk6 = (s16)(data_ov017_02143988.unk6 - 1);
    data_ov017_02143988.unk8 = (s16)(data_ov017_02143988.unk8 - 1);
    ((Boss03*)arg1)->unk4    = 0;
    ((Boss03*)arg1)->unk8    = NULL;
}

void* func_ov017_02126ef8(void* arg0) {
    void* var_r5;
    s32   temp_c;
    u16   temp_r1;
    u16   var_r4;
    u32   temp_r0;

    var_r5  = arg0;
    var_r4  = 0;
    temp_r0 = RNG_Next(0x64);
loop_1:
    temp_r1 = var_r4 + ((Boss03*)var_r5)->unk6;
    temp_c  = temp_r0 >= (u32)temp_r1;
    var_r4  = temp_r1;
    if (temp_c) {
        var_r5 += 8;
        goto loop_1;
    }
    return var_r5;
}

s32 func_ov017_02126f30(s16 arg0, void* arg1) {
    s32 var_r2;

    var_r2 = 0;
    if ((s32)(s16)(arg0 + ((Boss03*)arg1)->unk4) <= 0x1F4) {
        var_r2 = 1;
    }
    return var_r2;
}

void func_ov017_02126f54(void* arg0) {
    void* var_lr;
    s32   temp_r1;
    s32   var_ip;

    var_ip                    = 0;
    data_ov017_02143988.unkA  = 0;
    data_ov017_02143988.unk6  = 0;
    data_ov017_02143988.unk8  = 0;
    data_ov017_02143988.unk2  = 0;
    data_ov017_02143988.unk10 = 0;
    data_ov017_02143988.unkC  = 0;
    var_lr                    = &data_ov017_021439b0;
    do {
        ((Boss03*)var_lr)->unk0 = -1;
        temp_r1                 = var_ip * 2;
        ((Boss03*)var_lr)->unk4 = 0;
        var_ip += 1;
        ((Boss03*)var_lr)->unk8           = 0;
        *(&data_ov017_0214399c + temp_r1) = -1;
        var_lr += 0xC;
    } while (var_ip < 3);
    data_ov017_02143988.unk1C = -1;
    data_ov017_02143988.unk20 = 0;
    data_ov017_02143988.unk24 = 0;
    data_ov017_02143988.unk4  = -1;
}

void func_ov017_02126fd0(void* arg0) {
    if (data_ov017_02143988.unk24 == 0) {
        return;
    }
    if (EasyTask_ValidateTaskId((TaskPool*)data_ov003_020e71b8, &data_ov017_021439a4) != 0) {
        return;
    }
    func_ov017_02126d90(arg0, &data_ov017_021439a4);
}

void func_ov017_02127020(void* arg0) {
    void* var_r6;
    s32   var_r5;

    var_r6 = &data_ov017_021439b0;
    var_r5 = 0;
    do {
        if ((EasyTask_ValidateTaskId((TaskPool*)data_ov003_020e71b8, var_r6) == 0) && (((Boss03*)var_r6)->unk8 != 0)) {
            func_ov017_02126eb4(arg0, var_r6);
        }
        var_r5 += 1;
        var_r6 += 0xC;
    } while (var_r5 < 3);
}

void func_ov017_0212707c(s32* arg0, s32* arg1) {
    u32   temp_r0;
    void* temp_r2;

    temp_r2 = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    *arg0   = ((Boss03*)temp_r2)->unkC;
    temp_r0 = ((Boss03*)temp_r2)->unk10;
    *arg1   = RNG_Next((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 0xD) << 0xC;
}

s32 func_ov017_021270bc(void) {
    void* var_r3;
    s32   var_r2;

    var_r3 = &data_ov017_021439a4;
    var_r2 = 0;
loop_1:
    if (((Boss03*)var_r3)->unk8 == 0) {
        return (var_r2 * 0xC) + (s32)&data_ov017_021439a4;
    }
    var_r2 += 1;
    var_r3 += 0xC;
    if (var_r2 >= 1) {
        return 0;
    }
    goto loop_1;
}

s32 func_ov017_021270fc(void) {
    void* var_r3;
    s32   var_r2;

    var_r3 = &data_ov017_021439b0;
    var_r2 = 0;
loop_1:
    if (((Boss03*)var_r3)->unk8 == 0) {
        return (var_r2 * 0xC) + (s32)&data_ov017_021439b0;
    }
    var_r2 += 1;
    var_r3 += 0xC;
    if (var_r2 >= 3) {
        return 0;
    }
    goto loop_1;
}

void func_ov017_0212713c(void) {
    if (RNG_Next(0x64) < 0x14U) {
        data_ov017_02143988.unkC = func_ov017_02126ef8(&data_ov017_02142df4);
        data_ov017_02143988.unk0 = 0x14;
        return;
    }
    data_ov017_02143988.unk10 = func_ov017_02126ef8(&data_ov017_02142e0c);
    data_ov017_02143988.unk0  = 0x14;
}

void func_ov017_02127194(void* arg0) {
    s32 sp8;
    s32 sp4;
    s32 temp_r0;
    s32 temp_r0_2;

    if (data_ov017_02143988.unkC != 0) {
        temp_r0 = func_ov017_021270bc();
        if ((temp_r0 != 0) && (func_ov017_02126f30(data_ov017_02143988.unkA, data_ov017_02143988.unkC) != 0)) {
            if (data_ov017_02143988.unk0 == 0) {
                func_ov017_0212707c(&sp8, &sp4);
                func_ov017_02126d00(arg0, (void*)temp_r0, data_ov017_02143988.unkC, sp8, sp4);
                data_ov017_02143988.unkC = NULL;
                return;
            }
            data_ov017_02143988.unk0 = (s16)(data_ov017_02143988.unk0 - 1);
            goto block_6;
        }
    }
block_6:
    if (data_ov017_02143988.unk10 != NULL) {
        temp_r0_2 = func_ov017_021270fc();
        if (temp_r0_2 == 0) {
            return;
        }
        if (func_ov017_02126f30(data_ov017_02143988.unkA, data_ov017_02143988.unk10) == 0) {
            return;
        }
        if (data_ov017_02143988.unk0 == 0) {
            func_ov017_0212707c(&sp8, &sp4);
            if (func_ov017_02126dd4(arg0, (void*)temp_r0_2, data_ov017_02143988.unk10, sp8, sp4) != 0) {
                (*(Boss03**)((u8*)temp_r0_2 + 4))->unk1A8 = 3;
            }
            data_ov017_02143988.unk10 = NULL;
            return;
        }
        data_ov017_02143988.unk0 = (s16)(data_ov017_02143988.unk0 - 1);
        return;
    }
    func_ov017_0212713c();
}

void func_ov017_021272f4(void* arg0) {
    func_ov017_02126fd0(arg0);
    func_ov017_02127020(arg0);
    func_ov017_02127194(arg0);
}

void func_ov017_02127314(void* arg0) {
    void* temp_r4;

    temp_r4 = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    func_ov003_02084634(arg0, 4, 0xC0, 0x40);
    func_ov003_020846ac(arg0, ((Boss03*)temp_r4)->unk4, ((Boss03*)temp_r4)->unk10, 0);
}

void func_ov017_02127358(void* arg0) {
    func_ov003_020846fc(((Boss03*)arg0)->unk1CC, ((Boss03*)arg0)->unk1C0);
    func_ov003_020846fc(((Boss03*)arg0)->unk1CC, ((Boss03*)arg0)->unk1C4);
    func_ov003_020846fc(((Boss03*)arg0)->unk1CC, ((Boss03*)arg0)->unk1C8);
}

s32 func_ov017_02127388(void* arg1, void* arg2) {
    void* var_ip;
    s32   var_r3;

    var_ip = &data_ov017_021439a4;
    var_r3 = 0;
loop_1:
    if ((void*)(((Boss03*)var_ip)->unk4 + 0x80) == arg1) {
        return (var_r3 * 0xC) + (s32)&data_ov017_021439a4;
    }
    var_r3 += 1;
    var_ip += 0xC;
    if (var_r3 >= 1) {
        return 0;
    }
    goto loop_1;
}

s32 func_ov017_021273cc(void* arg1, void* arg2) {
    void* var_ip;
    s32   var_r3;

    var_ip = &data_ov017_021439b0;
    var_r3 = 0;
loop_1:
    if ((void*)(((Boss03*)var_ip)->unk4 + 0x144) == arg1) {
        return (var_r3 * 0xC) + (s32)&data_ov017_021439b0;
    }
    var_r3 += 1;
    var_ip += 0xC;
    if (var_r3 >= 3) {
        return 0;
    }
    goto loop_1;
}

void func_ov017_02127410(s16 arg0) {
    s32 var_lr;

    var_lr = 0;
loop_1:
    if ((&data_ov017_0214398c)[var_lr] == -1) {
        (&data_ov017_0214398c)[var_lr] = arg0;
        return;
    }
    var_lr += 1;
    if (var_lr >= 1) {
        return;
    }
    goto loop_1;
}

void func_ov017_02127448(s16 arg0) {
    s32 temp_ip;
    s32 var_lr;

    var_lr = 0;
loop_1:
    temp_ip = var_lr * 2;
    if (*(&data_ov017_0214399c + temp_ip) == -1) {
        *(&data_ov017_0214399c + temp_ip) = arg0;
        return;
    }
    var_lr += 1;
    if (var_lr >= 3) {
        return;
    }
    goto loop_1;
}

void func_ov017_02127480(void* arg0, void* arg1) {
    if ((*(Boss03**)((u8*)arg1 + 4))->unk58 & 0x200) {
        return;
    }
    func_ov017_02127410(0);
    EasyTask_DeleteTask((TaskPool*)data_ov003_020e71b8, ((Boss03*)arg1)->unk0);
    func_ov017_02126d90(arg0, arg1);
}

void func_ov017_021274c8(void* arg1) {
    void* temp_r1;

    temp_r1 = ((Boss03*)arg1)->unk4;
    if (((Boss03*)temp_r1)->unk54 & 0x204) {
        return;
    }
    if (((u32)(((Boss03*)temp_r1)->unk196 << 0x16) >> 0x1F) == 0) {
        func_ov017_02127448(1);
    } else {
        func_ov017_02127448(2);
    }
    func_ov003_020c4fc8(((Boss03*)arg1)->unk4);
}

void func_ov017_02127514(void* arg0) {
    void* temp_r3;
    void* temp_r3_2;
    void* var_r0;
    void* var_r5;
    void* var_r5_2;

    var_r0 = func_ov003_0208495c(((Boss03*)arg0)->unk1C0);
    if (var_r0 != NULL) {
        do {
            if (((Boss03*)var_r0)->unk1C & 0x10) {
                temp_r3 = data_ov003_020e71b8->unk3D898;
                if (temp_r3 != NULL) {
                    ((Boss03*)temp_r3)->unk28   = (s32)(((Boss03*)temp_r3)->unk28 + ((Boss03*)var_r0)->unk20);
                    temp_r3_2                   = data_ov003_020e71b8->unk3D898;
                    ((Boss03*)temp_r3_2)->unk2C = (s32)(((Boss03*)temp_r3_2)->unk2C + ((Boss03*)var_r0)->unk24);
                }
            }
            var_r0 = func_ov003_02084984(var_r0 + 0x28);
        } while (var_r0 != NULL);
    }
    var_r5 = func_ov003_0208495c(((Boss03*)arg0)->unk1C8);
    if (var_r5 != NULL) {
        do {
            if ((((Boss03*)var_r5)->unk1C & 0x10) && (func_ov017_02127388(arg0, var_r5) != 0)) {
                func_ov017_02127480(arg0, var_r5);
            }
            var_r5 = func_ov003_02084984(var_r5 + 0x28);
        } while (var_r5 != NULL);
    }
    var_r5_2 = func_ov003_0208495c(((Boss03*)arg0)->unk1C4);
    if (var_r5_2 == NULL) {
        return;
    }
    do {
        if ((((Boss03*)var_r5_2)->unk1C & 0x10) && (func_ov017_021273cc(arg0, var_r5_2) != 0)) {
            func_ov017_021274c8(arg0);
        }
        var_r5_2 = func_ov003_02084984(var_r5_2 + 0x28);
    } while (var_r5_2 != NULL);
}

void func_ov017_02127624(void* arg0) {
    s32   temp_r0;
    s32   temp_r0_2;
    void* temp_r2;
    void* temp_r2_2;
    void* temp_r3;
    void* temp_r3_2;
    void* temp_r3_3;
    void* temp_r3_4;
    void* var_r0;
    void* var_r4;
    void* var_r4_2;

    var_r0 = func_ov003_0208495c(((Boss03*)arg0)->unk1C0);
    if (var_r0 != NULL) {
        do {
            if (((Boss03*)var_r0)->unk1C & 0x10) {
                temp_r3 = data_ov003_020e71b8->unk3D898;
                if (temp_r3 != NULL) {
                    ((Boss03*)temp_r3)->unk28   = (s32)(((Boss03*)temp_r3)->unk28 + ((Boss03*)var_r0)->unk20);
                    temp_r3_2                   = data_ov003_020e71b8->unk3D898;
                    ((Boss03*)temp_r3_2)->unk2C = (s32)(((Boss03*)temp_r3_2)->unk2C + ((Boss03*)var_r0)->unk24);
                }
            }
            var_r0 = func_ov003_02084984(var_r0 + 0x28);
        } while (var_r0 != NULL);
    }
    var_r4 = func_ov003_0208495c(((Boss03*)arg0)->unk1C8);
    if (var_r4 != NULL) {
        do {
            if (((Boss03*)var_r4)->unk1C & 0x10) {
                temp_r0 = func_ov017_02127388(arg0, var_r4);
                if (temp_r0 != 0) {
                    temp_r3_3                   = ((Boss03*)temp_r0)->unk4;
                    ((Boss03*)temp_r3_3)->unk2C = (s32)(((Boss03*)temp_r3_3)->unk2C + ((Boss03*)var_r4)->unk20);
                    temp_r2                     = ((Boss03*)temp_r0)->unk4;
                    ((Boss03*)temp_r2)->unk30   = (s32)(((Boss03*)temp_r2)->unk30 + ((Boss03*)var_r4)->unk24);
                }
            }
            var_r4 = func_ov003_02084984(var_r4 + 0x28);
        } while (var_r4 != NULL);
    }
    var_r4_2 = func_ov003_0208495c(((Boss03*)arg0)->unk1C4);
    if (var_r4_2 == NULL) {
        return;
    }
    do {
        if (((Boss03*)var_r4_2)->unk1C & 0x10) {
            temp_r0_2 = func_ov017_021273cc(arg0, var_r4_2);
            if (temp_r0_2 != 0) {
                temp_r3_4                   = ((Boss03*)temp_r0_2)->unk4;
                ((Boss03*)temp_r3_4)->unk28 = (s32)(((Boss03*)temp_r3_4)->unk28 + ((Boss03*)var_r4_2)->unk20);
                temp_r2_2                   = ((Boss03*)temp_r0_2)->unk4;
                ((Boss03*)temp_r2_2)->unk2C = (s32)(((Boss03*)temp_r2_2)->unk2C + ((Boss03*)var_r4_2)->unk24);
            }
        }
        var_r4_2 = func_ov003_02084984(var_r4_2 + 0x28);
    } while (var_r4_2 != NULL);
}

void func_ov017_02127774(void* arg0) {
    ((Boss03*)arg0)->unkF8  = 0;
    ((Boss03*)arg0)->unk1D0 = 0x48;
    func_ov017_02127314(arg0 + 0xC4);
    ((Boss03*)arg0)->unk1C0 = func_ov003_020846f0(0);
    ((Boss03*)arg0)->unk1C4 = func_ov003_020846f0(1);
    ((Boss03*)arg0)->unk1C8 = func_ov003_020846f0(3);
    ((Boss03*)arg0)->unk1CC = func_ov003_020846f0(4);
}

void func_ov017_021277cc(void* arg0) {
    s16   temp_r0_2;
    s16   temp_r1_2;
    s32   temp_r2;
    s32   temp_r4;
    s32   temp_r5;
    s32   temp_r5_2;
    s32   temp_r5_3;
    s32   temp_r6;
    u32   temp_r1;
    u32   temp_r1_3;
    u32   temp_r2_2;
    u32   temp_r3;
    u32   temp_r4_2;
    void* temp_r0;

    temp_r0 = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    if (((Boss03*)arg0)->unk54 & 0x4200) {
        return;
    }
    temp_r5   = ((Boss03*)arg0)->unk28 - ((Boss03*)temp_r0)->unk4;
    temp_r6   = ((Boss03*)arg0)->unk2C - ((Boss03*)temp_r0)->unk10;
    temp_r3   = (temp_r5 * temp_r5) + 0x800;
    temp_r1   = (temp_r6 * temp_r6) + 0x800;
    temp_r4   = FX_Sqrt(((temp_r3 >> 0xC) | ((MULT_HI(temp_r5, temp_r5) + M2C_CARRY(temp_r3)) << 0x14)) +
                        ((temp_r1 >> 0xC) | ((MULT_HI(temp_r6, temp_r6) + M2C_CARRY(temp_r1)) << 0x14)));
    temp_r5_2 = ((s32)(u16)(FX_Atan2Idx(temp_r6, temp_r5) + 0x8000) >> 4) * 2;
    temp_r2   = 0x400000 - temp_r4;
    temp_r4_2 = (temp_r2 * 2) + 0x800;
    temp_r1_2 = *(&data_0205e4e0 + ((temp_r5_2 + 1) * 2));
    temp_r5_3 = (temp_r4_2 >> 0xC) | (((((temp_r2 >> 0x1F) * 2) | ((u32)temp_r2 >> 0x1F)) + M2C_CARRY(temp_r4_2)) << 0x14);
    temp_r1_3 = (temp_r1_2 * temp_r5_3) + 0x800;
    temp_r0_2 = *(s16*)((u8*)&data_0205e4e0 + (temp_r5_2 * 2));
    temp_r2_2 = (temp_r0_2 * temp_r5_3) + 0x800;
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 +
                                   ((temp_r1_3 >> 0xC) | ((MULT_HI(temp_r1_2, temp_r5_3) + M2C_CARRY(temp_r1_3)) << 0x14)));
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C +
                                   ((temp_r2_2 >> 0xC) | ((MULT_HI(temp_r0_2, temp_r5_3) + M2C_CARRY(temp_r2_2)) << 0x14)));
}

void func_ov017_021278e4(void* arg0) {
    void* var_r5;
    s32   var_r4;

    func_ov017_021277cc(data_ov003_020e71b8->unk3D898);
    if (data_ov017_02143988.unk20 != 0) {
        func_ov017_021277cc(data_ov017_02143988.unk20 + 4);
    }
    var_r5 = &data_ov017_021439b0;
    var_r4 = 0;
    do {
        if (((Boss03*)var_r5)->unk4 != 0) {
            func_ov017_021277cc(arg0);
        }
        var_r4 += 1;
        var_r5 += 0xC;
    } while (var_r4 < 3);
}

void func_ov017_0212794c(void* arg0) {
    u16 temp_r0;
    u16 temp_r0_2;
    u32 temp_r2;
    u32 temp_r2_2;

    temp_r0 = ((Boss03*)arg0)->unkF8;
    if (((Boss03*)arg0)->unkFA == 0) {
        if (temp_r0 == 0) {
            ((Boss03*)arg0)->unkF8 = 3U;
            ((Boss03*)arg0)->unkFA = (u16)(RNG_Next(0x12C) + 0x5DC);
            ((Boss03*)arg0)->unkFC = 0x1C8U;
            func_ov003_020d3c2c(arg0 + 0x7C, &data_ov017_02141a44);
            func_ov003_020d3c2c(arg0 + 0xA0, &data_ov017_0214186c);
            func_ov003_020d3c98(arg0 + 0x7C, arg0 + 0xA0);
            ((Boss03*)arg0)->unk1D0 = 0x48;
        } else {
            func_ov003_020d3c2c(arg0 + 0x7C, &data_ov017_0214179c);
            func_ov003_020d3c2c(arg0 + 0xA0, &data_ov017_02141804);
            func_ov003_020d3c98(arg0 + 0x7C, arg0 + 0xA0);
            ((Boss03*)arg0)->unkF8 = 0U;
            ((Boss03*)arg0)->unkFA = (u16)(RNG_Next(0x12C) + 0x12C);
            ((Boss03*)arg0)->unkFC = 0U;
            temp_r2                = data_ov003_020e71b8->unk3D7CC;
            func_ov003_020c3acc(0, 0x2FB, (s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1);
        }
    } else if (temp_r0 == 0) {
        ((Boss03*)arg0)->unk1D0 = (s16)(((Boss03*)arg0)->unk1D0 - 1);
        if (((Boss03*)arg0)->unk1D0 == 0) {
            func_ov003_020c3acc(0, 0x301, data_ov003_020e71b8->unk3D7C4);
            ((Boss03*)arg0)->unk1D0 = 0x48;
        }
    } else if (temp_r0 == 1) {
        ((Boss03*)arg0)->unk1D0 = (s16)(((Boss03*)arg0)->unk1D0 - 1);
        if (((Boss03*)arg0)->unk1D0 == 0) {
            func_ov003_020c3acc(0, 0x302, data_ov003_020e71b8->unk3D7C4);
            ((Boss03*)arg0)->unk1D0 = 0x24;
        }
    }
    temp_r0_2 = ((Boss03*)arg0)->unkFC;
    if (temp_r0_2 != 0) {
        ((Boss03*)arg0)->unkFC = (u16)(temp_r0_2 - 1);
        if ((((Boss03*)arg0)->unkFC == 0) && (((Boss03*)arg0)->unkF8 == 3)) {
            func_ov003_020d3c2c(arg0 + 0x7C, &data_ov017_0214172c);
            func_ov003_020d3c2c(arg0 + 0xA0, &data_ov017_02141764);
            func_ov003_020d3c98(arg0 + 0x7C, arg0 + 0xA0);
            ((Boss03*)arg0)->unkF8 = 1U;
            temp_r2_2              = data_ov003_020e71b8->unk3D7CC;
            func_ov003_020c3acc(0, 0x2FC, (s32)(temp_r2_2 + (temp_r2_2 >> 0x1F)) >> 1);
            ((Boss03*)arg0)->unk1D0 = 0x24;
            return;
        }
        return;
    }
    ((Boss03*)arg0)->unkFA = (u16)(((Boss03*)arg0)->unkFA - 1);
}

void func_ov017_02127b88(void* arg0) {
    if (((Boss03*)arg0)->unkF8 == 1) {
        func_ov017_021278e4(arg0);
        func_ov017_02127358(arg0);
        func_ov017_02127514(arg0);
    } else {
        func_ov017_02127358(arg0);
        func_ov017_02127624(arg0);
    }
    func_ov017_0212794c(arg0);
}

void func_ov017_02127bcc(void* arg0) {
    s16 temp_r1;

    if (((Boss03*)arg0)->unk1D2 == 0) {
        ((Boss03*)arg0)->unk1D2 = 0x64;
    }
    temp_r1 = ((Boss03*)arg0)->unk1D2;
    if ((s32)temp_r1 > 0) {
        func_02026590((u8*)(*(void**)arg0) + 0x54, 0, (u16)temp_r1);
        ((Boss03*)arg0)->unk1D2 = (s16)(((Boss03*)arg0)->unk1D2 - 1);
    }
    if ((s32)((Boss03*)arg0)->unk1D2 > 0) {
        return;
    }
    func_ov017_02126be4(arg0, NULL);
}

s32 func_ov017_02127c40(void* arg1, u16* arg2) {
    s32   temp_r2_5;
    u32   temp_r2_3;
    u32   temp_r2_4;
    void* temp_ip;
    void* temp_r0;
    void* temp_r0_2;
    void* temp_r0_3;
    void* temp_r0_4;
    void* temp_r0_5;
    void* temp_r0_6;
    void* temp_r1;
    void* temp_r1_2;
    void* temp_r1_3;
    void* temp_r1_4;
    void* temp_r1_5;
    void* temp_r1_6;
    void* temp_r2;
    void* temp_r2_2;
    void* temp_r3;
    void* temp_r4;
    void* temp_r5;
    void* temp_r6;
    void* temp_r6_2;
    void* var_r0;
    void* var_r0_2;
    void* var_r0_3;
    void* var_r0_4;
    void* var_r1;
    void* var_r1_2;
    void* var_r6;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r5 = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    MI_CpuSet(temp_r4, 0, 0x1DC);
    func_ov017_0212fc70();
    temp_ip                  = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    ((Boss03*)temp_r4)->unk0 = temp_ip;
    ((Boss03*)temp_ip)->unk2 = (u16)(((Boss03*)temp_ip)->unk2 | 1);
    temp_r0                  = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214171c, 1, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r2 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r1  = temp_r2 + ((Boss03*)temp_r2)->unk8;
    }
    if (temp_r0 == NULL) {
        var_r6 = NULL;
    } else {
        temp_r2_2 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r6    = temp_r2_2 + ((Boss03*)temp_r2_2)->unk10;
    }
    ((Boss03*)temp_r4)->unk14 = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r1,
                                                     g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x76E0);
    ((Boss03*)temp_r4)->unk18 = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r6,
                                                     g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, 0x8000);
    DMA_Flush();
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r0_2                = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214171c, 2, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0_2;
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1  = ((Boss03*)temp_r0_2)->unk8 + 0x20;
        var_r1_2 = (void*)((u8*)temp_r1 + (s32)((Boss03*)temp_r1)->unk18);
    }
    ((Boss03*)temp_r4)->unk10 = PaletteMgr_AllocPalette(g_PaletteManagers[0], var_r1_2, 0, 0, 0xF);
    temp_r0_3                 = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_2 = ((Boss03*)temp_r0_3)->unk8 + 0x20;
        var_r0    = temp_r1_2 + ((Boss03*)temp_r1_2)->unk8;
    }
    ((Boss03*)temp_r4)->unk6C = (void*)(var_r0 + 4);
    temp_r0_4                 = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_4 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_3 = ((Boss03*)temp_r0_4)->unk8 + 0x20;
        var_r0_2  = temp_r1_3 + ((Boss03*)temp_r1_3)->unk10;
    }
    ((Boss03*)temp_r4)->unk74 = (void*)(var_r0_2 + 4);
    temp_r0_5                 = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214171c, 3, 0);
    ((Boss03*)temp_r4)->unk8  = temp_r0_5;
    if (temp_r0_5 == NULL) {
        var_r0_3 = NULL;
    } else {
        temp_r1_4 = ((Boss03*)temp_r0_5)->unk8 + 0x20;
        var_r0_3  = temp_r1_4 + ((Boss03*)temp_r1_4)->unk8;
    }
    ((Boss03*)temp_r4)->unk70 = (void*)(var_r0_3 + 4);
    temp_r0_6                 = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_0214171c, 0x1B, 0);
    ((Boss03*)temp_r4)->unkC  = temp_r0_6;
    if (temp_r0_6 == NULL) {
        var_r0_4 = NULL;
    } else {
        temp_r1_5 = ((Boss03*)temp_r0_6)->unk8 + 0x20;
        var_r0_4  = temp_r1_5 + ((Boss03*)temp_r1_5)->unk8;
    }
    ((Boss03*)temp_r4)->unk78 = (void*)(var_r0_4 + 4);
    func_0200d1d8(temp_r4 + 0x1C, 0, 1, 0, temp_r4 + 0x6C, 1, 2);
    func_0200d1d8(temp_r4 + 0x44, 0, 2, 0, temp_r4 + 0x74, 1, 2);
    func_ov003_020d3b6c(temp_r4 + 0x7C, ((Boss03*)temp_r4)->unk8, 1, 1, 0, 0);
    func_ov003_020d3bec(temp_r4 + 0x7C, &data_ov017_0214171c, temp_r4 + 0x1C, temp_r4 + 0x70, &data_ov017_0214179c);
    func_ov003_020d3b6c(temp_r4 + 0xA0, ((Boss03*)temp_r4)->unkC, 1, 1, 0, 0);
    func_ov003_020d3bec(temp_r4 + 0xA0, &data_ov017_0214171c, temp_r4 + 0x44, temp_r4 + 0x78, &data_ov017_02141804);
    func_ov003_020d3c98(temp_r4 + 0x7C, temp_r4 + 0xA0);
    func_0200d858(temp_r4 + 0x1C, 0, 0, 0);
    func_0200d858(temp_r4 + 0x44, 0, 0, 0);
    CombatSprite_LoadDirect(2, (CombatSprite*)(temp_r4 + 0x100), &data_ov017_02141724, 1, 3, 2, 4, 0);
    func_ov003_02082730(temp_r4 + 0x100, -0x1000);
    CombatSprite_LoadDirect(2, (CombatSprite*)(temp_r4 + 0x160), &data_ov017_02141724, 3, 2, 4, 1, 0);
    func_ov003_02082730(temp_r4 + 0x160, -0x1000);
    data_ov003_020e71b8->unk3D7C2 = 0;
    data_ov003_020e71b8->unk3D81A = 0;
    ((Boss03*)temp_r4)->unkFA     = (s16)(RNG_Next(0x12C) + 0x12C);
    func_ov003_02083a2c(0, 0, 0x140, 0x200, 0x1F0, 0);
    func_ov003_02083a2c(1, 0x70, 0x1C8, 0x190, 0x200, 0);
    ((Boss03*)temp_r5)->unk18 = (s32)(0x130000 - ((Boss03*)temp_r5)->unk8);
    ((Boss03*)temp_r5)->unk1C = FX_Atan2Idx(0, 0x140);
    if ((arg2 != NULL) && (*arg2 != 0)) {
        func_ov017_02126be4(temp_r4, NULL);
    } else {
        temp_r2_3                   = data_ov003_020e71b8->unk3D7CC;
        temp_r1_6                   = data_ov003_020e71b8->unk3D898;
        ((Boss03*)temp_r1_6)->unk28 = (s32)((s32)(temp_r2_3 + (temp_r2_3 >> 0x1F)) >> 1);
        temp_r2_4                   = data_ov003_020e71b8->unk3D7D0;
        temp_r2_5                   = (s32)(temp_r2_4 + (temp_r2_4 >> 0x1F)) >> 1;
        ((Boss03*)temp_r1_6)->unk2C = temp_r2_5;
        func_ov003_02083ab0(0, ((Boss03*)temp_r1_6)->unk28, temp_r2_5, 0);
        temp_r3                     = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3)->unk28   = (s32)((Boss03*)temp_r3)->unk34;
        temp_r6                     = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r6)->unk2C   = (s32)((Boss03*)temp_r6)->unk38;
        temp_r6_2                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r6_2)->unk30 = (s32)((Boss03*)temp_r6_2)->unk3C;
        ((Boss03*)temp_r5)->unk54   = -0x30000;
        func_ov017_02126be4(temp_r4, func_ov017_02127bcc);
        func_ov017_02126f54(temp_r4);
    }
    func_ov017_02126bfc(temp_r4);
    func_ov017_02127774(temp_r4);
    return 1;
}

s32 func_ov017_0212816c(void* arg1) {
    void (*temp_r1)(void*);
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1D8;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov017_021272f4(temp_r4);
    func_ov017_02127b88(temp_r4);
    func_ov017_02126cf0(temp_r4);
    func_ov017_0212fe1c(((Boss03*)temp_r4)->unk0);
    func_ov017_02126bfc(temp_r4);
    CombatSprite_Update(temp_r4 + 0x100);
    CombatSprite_Update(temp_r4 + 0x160);
    return 1;
}

s32 func_ov017_021281e8(void* arg1) {
    s16 temp_r0;
    s32 temp_r5;
    s32 temp_r6;

    temp_r6 = ((Boss03*)arg1)->unk18;
    temp_r5 = func_ov003_020843b0(0, 0x1EA000);
    temp_r0 = func_ov003_020843ec(0, 0xC0000, 0);
    CombatSprite_SetPosition(temp_r6 + 0x100, temp_r5, temp_r0);
    CombatSprite_Render(temp_r6 + 0x100);
    CombatSprite_SetPosition(temp_r6 + 0x160, temp_r5, (s16)(temp_r0 + 0x80));
    CombatSprite_Render(temp_r6 + 0x160);
    return 1;
}

s32 func_ov017_02128258(void* arg1) {
    void* temp_r2;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    CombatSprite_Release(temp_r4 + 0x100);
    CombatSprite_Release(temp_r4 + 0x160);
    func_ov003_02084678(temp_r4 + 0xC4);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], ((Boss03*)temp_r4)->unk14);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], ((Boss03*)temp_r4)->unk18);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], ((Boss03*)temp_r4)->unk10);
    func_0200d954(0, 1);
    func_0200d954(0, 2);
    func_ov003_020d3cf8(temp_r4 + 0x7C);
    func_ov003_020d3cf8(temp_r4 + 0xA0);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r2                  = ((Boss03*)temp_r4)->unk0;
    ((Boss03*)temp_r2)->unk2 = (u16)(((Boss03*)temp_r2)->unk2 & ~1);
    return 1;
}

void func_ov017_021282f8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_0214170c.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_0214170c.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_0214170c.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_0214170c.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_02128340(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk28C = arg1;
    ((Boss03*)arg0)->unk288 = 0;
    ((Boss03*)arg0)->unk280 = 0;
}

s32 func_ov017_02128364(void* arg0) {
    do {
    } while (((Boss03*)arg0)->unk290 == RNG_Next(4));
}

void func_ov017_02128388(void* arg0) {
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;

    temp_r1 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk290 * 0xC));
    ((Boss03*)arg0)->unk28  = temp_r1;
    ((Boss03*)arg0)->unk1AC = temp_r1;
    temp_r1_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk290 * 0xC));
    ((Boss03*)arg0)->unk2C  = temp_r1_2;
    ((Boss03*)arg0)->unk1B0 = temp_r1_2;
    temp_r1_3               = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk290 * 0xC));
    ((Boss03*)arg0)->unk30  = temp_r1_3;
    ((Boss03*)arg0)->unk1B4 = temp_r1_3;
}

void func_ov017_02128414(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_02128340(arg0, func_ov017_02128568);
}

void func_ov017_0212843c(void* arg0) {
    s16 sp14;
    s32 sp10;
    s32 spC;
    s32 sp8;
    s16 var_r1;

    if (((Boss03*)arg0)->unk280 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 6, 1);
    }
    if (((Boss03*)arg0)->unk280 == 0x32) {
        sp8  = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk290 * 0xC));
        spC  = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk290 * 0xC));
        sp10 = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk290 * 0xC));
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        sp14 = var_r1;
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f30, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk280 = (s32)(((Boss03*)arg0)->unk280 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    ((Boss03*)arg0)->unk290 = (u16)func_ov017_02128364(arg0);
    func_ov017_02128388(arg0);
    func_ov017_02128340(arg0, func_ov017_02128568);
}

void func_ov017_02128568(void* arg0) {
    s32 temp_r1;

    if (((Boss03*)arg0)->unk280 == 0) {
        ((Boss03*)arg0)->unk298 = 0;
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_020c4b1c(arg0);
        ((Boss03*)arg0)->unk284 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk280 + 1;
    ((Boss03*)arg0)->unk280 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk284) {
        return;
    }
    if (func_ov003_020c4348(arg0) != 0) {
        func_ov017_02128340(arg0, func_ov017_0212843c);
        ((Boss03*)arg0)->unk298 = 1;
        return;
    }
    ((Boss03*)arg0)->unk290 = (s16)func_ov017_02128364(arg0);
    func_ov017_02128388(arg0);
    func_ov017_02128340(arg0, func_ov017_02128568);
}

void func_ov017_02128614(void* arg0) {
    ((Boss03*)arg0)->unk298 = 0;
    CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
    func_ov003_020c4b1c(arg0);
    data_ov017_021439d6 = 1;
    func_ov017_02128364(arg0);
    func_ov017_02128340(arg0, func_ov017_02128664);
}

void func_ov017_02128664(void* arg0) {
    if (data_ov017_021439d6 != 2) {
        return;
    }
    ((Boss03*)arg0)->unk290 = (s16)func_ov017_02128364(arg0);
    func_ov017_02128388(arg0);
    data_ov017_021439d6 = 0;
    func_ov017_02128340(arg0, func_ov017_02128568);
}

void func_ov017_021286b4(void* arg0) {
    if (((Boss03*)arg0)->unk280 == 0) {
        ((Boss03*)arg0)->unk84 = (s32)((((Boss03*)arg0)->unk84 & ~0x60) | 0x40);
        if (((Boss03*)arg0)->unk294 == 1) {
            func_ov017_02130178(1, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(1, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk280 = (s32)(((Boss03*)arg0)->unk280 + 1);
    ((Boss03*)arg0)->unk298 = 1;
    if (func_ov003_020c62c4(arg0, 3) != 0) {
        return;
    }
    func_ov017_02128340(arg0, func_ov017_02128614);
    ((Boss03*)arg0)->unk298 = 0;
}

void func_ov017_02128754(void* arg0) {
    ((Boss03*)arg0)->unk298 = 1;
}

void func_ov017_02128760(void* arg0) {
    void* temp_r4;
    void* var_r0;

    var_r0  = arg0;
    temp_r4 = var_r0;
    if (data_ov017_021439d4.unk0 != 0) {
        ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x10);
    } else {
        ((Boss03*)temp_r4)->unk298 = 1;
        var_r0                     = func_ov003_020c72b4(var_r0, 0, 3);
    }
    if (var_r0 != NULL) {
        return;
    }
    func_ov017_02128340(temp_r4, func_ov017_02128568);
}

s32 func_ov017_021287b8(void* arg1, s32 arg2) {
    s32   temp_r0;
    s32   temp_r0_2;
    s32   temp_r1;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x2A0);
    func_ov003_020c3efc(temp_r4, arg2);
    data_ov017_02143980        = (s16)((Boss03*)temp_r4)->unk7C;
    ((Boss03*)temp_r4)->unk29C = 1;
    ((Boss03*)temp_r4)->unk298 = 0;
    ((Boss03*)temp_r4)->unk290 = (u16)RNG_Next(4);
    temp_r0 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)temp_r4)->unk290 * 0xC));
    ((Boss03*)temp_r4)->unk28  = temp_r0;
    ((Boss03*)temp_r4)->unk1AC = temp_r0;
    temp_r0_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)temp_r4)->unk290 * 0xC));
    ((Boss03*)temp_r4)->unk2C  = temp_r0_2;
    ((Boss03*)temp_r4)->unk1B0 = temp_r0_2;
    temp_r1                    = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)temp_r4)->unk290 * 0xC));
    ((Boss03*)temp_r4)->unk30  = temp_r1;
    ((Boss03*)temp_r4)->unk1B4 = temp_r1;
    ((Boss03*)temp_r4)->unk38  = 0;
    ((Boss03*)temp_r4)->unk3C  = 0;
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov017_0212ff54(temp_r4 + 0x220, temp_r4 + 0x1C0);
    func_ov017_02128340(temp_r4, func_ov017_02128414);
    func_ov017_0213021c(temp_r4);
    ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000000);
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x100000);
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x200000);
    func_ov017_02130148();
    return 1;
}

s32 func_ov017_02128924(void* arg1) {
    void (*temp_r1_2)(void*);
    s16   temp_lr;
    s32   temp_r0_2;
    s32   var_r0;
    u16   temp_r1;
    u32   temp_r0;
    void* temp_r4;

    temp_lr = data_ov003_020e71b8->unk3D8DA;
    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((s32)data_ov003_020e71b8->unk3D8D8 < (s32)(((u32)(temp_lr * 0x19) >> 0x1F) + ((temp_lr * 0x19) / 100))) &&
        (((Boss03*)temp_r4)->unk28C != (void (*)(void*))func_ov017_02128754))
    {
        ((Boss03*)temp_r4)->unk20     = 3;
        data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
        func_ov017_021301e0();
    }
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk294 = temp_r0;
    switch (temp_r0) {
        default:
            break;
        case 1:
        case 2:
            func_ov017_02128340(temp_r4, func_ov017_021286b4);
            break;
        case 3:
            temp_r1 = ((Boss03*)temp_r4)->unk18C;
            if (temp_r1 & 0x10) {
                ((Boss03*)temp_r4)->unk18C = (u16)(temp_r1 | 0x20);
            } else {
                func_ov017_02128340(temp_r4, func_ov017_02128754);
            }
            break;
        case 6:
            func_ov017_02128340(temp_r4, func_ov017_02128760);
            break;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk28C;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    if ((((Boss03*)temp_r4)->unk298 == 0) && (data_ov003_020e71b8->unk3D8A0 == temp_r4)) {
        data_ov003_020e71b8->unk3D8A0 = 0;
    }
    temp_r0_2 = ((Boss03*)temp_r4)->unk54;
    if (data_ov017_021439d4.unk0 == 0) {
        var_r0 = temp_r0_2 & ~0x10;
    } else {
        var_r0 = temp_r0_2 | 0x10;
    }
    ((Boss03*)temp_r4)->unk54 = var_r0;
    func_ov003_020c4628(temp_r4);
    func_ov017_0212ffcc(temp_r4 + 0x220, temp_r4 + 0x1C0);
    func_ov017_0213015c();
    return ((Boss03*)temp_r4)->unk29C;
}

s32 func_ov017_02128ab0(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((Boss03*)temp_r4)->unk298 != 0) {
        func_ov003_020c4878(temp_r4);
    }
    if (data_ov017_021439d4.unk0 == 0) {
        func_ov017_0212ffe4(temp_r4 + 0x220, temp_r4 + 0x1C0, ((Boss03*)temp_r4)->unk290);
    } else {
        func_ov017_0212ffe4(temp_r4 + 0x220, temp_r4 + 0x1C0, -1);
    }
    return 1;
}

s32 func_ov017_02128b10(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov017_02130130(temp_r4 + 0x220, temp_r4 + 0x1C0);
    func_ov003_020c48fc(temp_r4);
    return 1;
}

void func_ov017_02128b34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141c38.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141c38.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141c38.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141c38.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_02128b7c(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_02128ba0(void* arg0) {
    s16 temp_r2_2;
    s16 temp_r3;
    s32 temp_r1;
    s32 temp_r2;
    u32 temp_r2_3;
    u32 temp_r3_2;

    temp_r2   = ((Boss03*)arg0)->unk1D4;
    temp_r3   = *(&data_0205e4e0 + (((((s32)((Boss03*)arg0)->unk1D8 >> 4) * 2) + 1) * 2));
    temp_r3_2 = (temp_r3 * temp_r2) + 0x800;
    ((Boss03*)arg0)->unk28 =
        (s32)(((Boss03*)arg0)->unk28 + ((temp_r3_2 >> 0xC) | ((MULT_HI(temp_r3, temp_r2) + M2C_CARRY(temp_r3_2)) << 0x14)));
    temp_r1   = ((Boss03*)arg0)->unk1D4;
    temp_r2_2 = *(&data_0205e4e0 + (((s32)((Boss03*)arg0)->unk1D8 >> 4) * 4));
    temp_r2_3 = (temp_r2_2 * temp_r1) + 0x800;
    ((Boss03*)arg0)->unk2C =
        (s32)(((Boss03*)arg0)->unk2C + ((temp_r2_3 >> 0xC) | ((MULT_HI(temp_r2_2, temp_r1) + M2C_CARRY(temp_r2_3)) << 0x14)));
}

void func_ov017_02128c24(void* arg0) {
    s32 temp_z;

    temp_z                 = func_ov003_020c5bfc(arg0) == 0;
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x80000000);
    if (!temp_z) {
        return;
    }
    func_ov017_02128b7c(arg0, func_ov017_02128d2c);
}

void func_ov017_02128c58(void* arg0) {
    s32 temp_r1;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_02084694(arg0 + 0x144, 0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 >= (s32)((Boss03*)arg0)->unk1C4) {
        if (func_ov003_020c4348(arg0) != 0) {
            func_ov017_02128b7c(arg0, func_ov017_02128f98);
        } else if (RNG_Next(0x64) < 0x32U) {
            func_ov017_02128b7c(arg0, func_ov017_02128ebc);
        } else {
            func_ov017_02128b7c(arg0, func_ov017_02128de0);
        }
    }
    if (((Boss03*)arg0)->unk1D0 != 0) {
        return;
    }
    func_ov017_02128b7c(arg0, func_ov017_0212928c);
}

void func_ov017_02128d2c(void* arg0) {
    s32 temp_r1;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_02084694(arg0 + 0x144, 0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    if (func_ov003_020c4348(arg0) != 0) {
        func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128f98);
        return;
    }
    if (RNG_Next(0x64) < 0x32U) {
        func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128ebc);
        return;
    }
    func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128de0);
}

void func_ov017_02128de0(void* arg0) {
    s32 temp_r1;
    s32 var_r0;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1D4 = 0x2000;
        ((Boss03*)arg0)->unk1D8 = (s16)RNG_Next(0x10000);
    }
    func_ov017_02128ba0(arg0);
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 >= (s32)((Boss03*)arg0)->unk1C4) {
        if (data_ov017_021439d4.unk0 == 1) {
            func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128c58);
            return;
        }
        func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128d2c);
        return;
    }
    var_r0 = ((s32)((Boss03*)arg0)->unk2C >> 0xC) - ((s32)((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C >> 0xC);
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    if (var_r0 >= 0x10) {
        return;
    }
    func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128f98);
}

void func_ov017_02128ebc(void* arg0) {
    s32 temp_r1;
    s32 var_r0;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1D4 = 0x8000;
        ((Boss03*)arg0)->unk1D8 = (s16)RNG_Next(0x10000);
    }
    func_ov017_02128ba0(arg0);
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 >= (s32)((Boss03*)arg0)->unk1C4) {
        if (data_ov017_021439d4.unk0 == 1) {
            func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128c58);
            return;
        }
        func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128d2c);
        return;
    }
    var_r0 = ((s32)((Boss03*)arg0)->unk2C >> 0xC) - ((s32)((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C >> 0xC);
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    if (var_r0 >= 0x10) {
        return;
    }
    func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128f98);
}

void func_ov017_02128f98(void* arg0) {
    s16   sp14;
    s32   sp10;
    s32   spC;
    s32   sp8;
    s16   var_r0;
    void* temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        temp_r0 = arg0 + 0x84;
        if (data_ov017_021439d4.unk0 == 1) {
            CombatSprite_SetAnimFromTable((CombatSprite*)temp_r0, 6, 1);
        } else {
            CombatSprite_SetAnimFromTable((CombatSprite*)temp_r0, 3, 1);
        }
    }
    if (((Boss03*)arg0)->unk1C0 == 0x30) {
        sp8  = ((Boss03*)arg0)->unk28;
        spC  = ((Boss03*)arg0)->unk2C;
        sp10 = ((Boss03*)arg0)->unk30;
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        sp14 = var_r0;
        EasyTask_CreateTask((TaskPool*)data_ov003_020e71b8, &data_ov017_02141f4c, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    if (data_ov017_021439d4.unk0 == 1) {
        func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128c58);
        return;
    }
    func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128d2c);
}

void func_ov017_021290a4(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk84 = (s32)((((Boss03*)arg0)->unk84 & ~0x60) | 0x40);
        if (((Boss03*)arg0)->unk1DC == 1) {
            func_ov017_02130178(0, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(0, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (func_ov003_020c65cc(arg0, 3) != 0) {
        return;
    }
    func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128c58);
}

void func_ov017_02129134(void) {}

void func_ov017_02129138(void* arg0) {
    s32 temp_r0;

    temp_r0 = ((Boss03*)arg0)->unk1C8;
    switch (temp_r0) { /* irregular */
        case 0:
            func_ov017_02130178(0, 0x4EA, 0x56C, ((Boss03*)arg0)->unk28);
            func_ov003_020c3acc(0, 0x2F6, ((Boss03*)arg0)->unk28);
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 6, 1);
            ((Boss03*)arg0)->unk1C8 = (s32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            CombatSprite_Release(arg0 + 0x84);
            CombatSprite_LoadFromTable(0, (CombatSprite*)arg0 + 0x84, &data_ov017_02141510, &data_ov017_021415a8, 0xC, 0x16,
                                       0x120);
            ((Boss03*)arg0)->unk84  = (s32)((((Boss03*)arg0)->unk84 & ~0x60) | 0x40);
            ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 & ~0x80000000);
            ((Boss03*)arg0)->unk1C8 = (s32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128c58);
            data_ov017_021439d4.unk0 = 1;
            ((Boss03*)arg0)->unk1D0  = 0x3C;
            ((Boss03*)arg0)->unk54   = (s32)(((Boss03*)arg0)->unk54 & ~0x40);
            return;
    }
}

void func_ov017_0212928c(void* arg0) {
    s32 temp_r0;

    temp_r0 = ((Boss03*)arg0)->unk1C8;
    switch (temp_r0) { /* irregular */
        case 0:
            ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x40);
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 0xB, 1);
            func_ov003_020c3acc(0, 0x2F7, ((Boss03*)arg0)->unk28);
            ((Boss03*)arg0)->unk1C8 = (s32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            CombatSprite_Release(arg0 + 0x84);
            CombatSprite_LoadFromTable(0, (CombatSprite*)arg0 + 0x84, &data_ov017_02141518, &data_ov017_02141568, 7, 0xD,
                                       0x120);
            ((Boss03*)arg0)->unk84  = (s32)((((Boss03*)arg0)->unk84 & ~0x60) | 0x40);
            ((Boss03*)arg0)->unk54  = (s32)(((Boss03*)arg0)->unk54 | 0x80000000);
            ((Boss03*)arg0)->unk1C8 = (s32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 2:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128d2c);
            data_ov017_021439d4.unk0 = 0;
            data_ov017_021439d4.unk2 = 2;
            return;
    }
}

void func_ov017_021293c4(void* arg0) {
    if (data_ov017_021439d4.unk0 == 1) {
        if (func_ov003_020c72b4(arg0, 0, 3) != 0) {
            return;
        }
        func_ov017_02128b7c(arg0, (void (*)(void*))func_ov017_02128c58);
        return;
    }
    if (((Boss03*)arg0)->unk1C0 != 0) {
        return;
    }
    ((Boss03*)arg0)->unk1EC = 0;
    CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
}

s32 func_ov017_02129438(void* arg1, s32 arg2) {
    void*  temp_r5;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r5 = func_ov003_020c3c88(temp_r4);
    MI_CpuSet(temp_r4, 0, 0x1F0);
    func_ov003_020c3efc(temp_r4, arg2);
    ((Boss03*)temp_r4)->unk1E4 = 1;
    ((Boss03*)temp_r4)->unk1E8 = 1;
    ((Boss03*)temp_r4)->unk1EC = 1;
    ((Boss03*)temp_r4)->unk1E0 = 0;
    data_ov017_021439d4.unk0   = 0;
    data_ov017_021439d4.unk2   = 0;
    CombatSprite_LoadFromTable(0, (CombatSprite*)temp_r4 + 0x84, &data_ov017_02141518, &data_ov017_02141568, 0, 0xD, 0x120);
    func_ov003_020880e4(0, temp_r4 + 0xE4, temp_r4);
    func_ov003_020c49c8(temp_r4);
    func_ov003_02084634(temp_r4 + 0x144, 1, ((Boss03*)temp_r5)->unk3, ((Boss03*)temp_r5)->unk4);
    func_ov003_020c4b5c(temp_r4);
    func_ov017_02128b7c(temp_r4, (void (*)(void*))func_ov017_02128c24);
    func_ov017_02130280(temp_r4);
    ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000040);
    return 1;
}

s32 func_ov017_0212952c(void* arg1) {
    void (*temp_r1_3)(void*);
    s16    temp_ip;
    s16    temp_r1;
    s16    temp_r1_2;
    u32    temp_r0;
    void** temp_r4;

    temp_ip = data_ov003_020e71b8->unk3D8DA;
    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((s32)data_ov003_020e71b8->unk3D8D8 < (s32)(((u32)(temp_ip * 0x19) >> 0x1F) + ((temp_ip * 0x19) / 100))) &&
        (((Boss03*)temp_r4)->unk1CC != (void (*)(void*))func_ov017_02129134))
    {
        ((Boss03*)temp_r4)->unk20 = 3;
    }
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk1DC = temp_r0;
    switch (temp_r0) {
        default:
            break;
        case 1:
        case 2:
            func_ov003_02084694(temp_r4 + 0x144, 0);
            func_ov017_02128b7c(temp_r4, (void (*)(void*))func_ov017_021290a4);
            break;
        case 7:
            if ((((Boss03*)temp_r4)->unk1EC != 0) && (((Boss03*)temp_r4)->unk1E0 == 0)) {
                func_ov017_02130178(0, 0x4E8, 0x4E9, ((Boss03*)temp_r4)->unk28);
                ((Boss03*)temp_r4)->unk1E0 = 0x12C;
            } else {
                ((Boss03*)temp_r4)->unk1EC = 1;
            }
            break;
        case 3:
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_02128b7c(temp_r4, (void (*)(void*))func_ov017_02129134);
            break;
        case 6:
            func_ov017_02128b7c(temp_r4, (void (*)(void*))func_ov017_021293c4);
            break;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1E0;
    if ((s32)temp_r1 > 0) {
        ((Boss03*)temp_r4)->unk1E0 = (s16)(temp_r1 - 1);
    }
    if ((data_ov017_021439d4.unk2 == 1) && (data_ov017_021439d4.unk0 == 0)) {
        func_ov017_02128b7c(temp_r4, (void (*)(void*))func_ov017_02129138);
        data_ov017_021439d4.unk2 = 0U;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1D0;
    if ((s32)temp_r1_2 > 0) {
        ((Boss03*)temp_r4)->unk1D0 = (s16)(temp_r1_2 - 1);
    }
    temp_r1_3 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1_3 != NULL) {
        temp_r1_3(temp_r4);
    }
    func_ov003_020c4668(temp_r4);
    return ((Boss03*)temp_r4)->unk1E8;
}

s32 func_ov017_021296f4(void* arg1) {
    func_ov003_020c48b0(((Boss03*)arg1)->unk18);
    return 1;
}

s32 func_ov017_02129708(void* arg1) {
    func_ov003_020c492c(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212971c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141c54.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141c54.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141c54.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141c54.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_02129764(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk98 = arg1;
    ((Boss03*)arg0)->unk94 = 0;
    ((Boss03*)arg0)->unk90 = 0;
}

void func_ov017_02129778(void* arg0) {
    s32   temp_r1;
    s32   temp_r2;
    u32   temp_r0;
    void* temp_lr;
    void* temp_r0_2;

    temp_r2 = ((Boss03*)arg0)->unk14;
    temp_r1 = (((Boss03*)arg0)->unk10 * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_lr = ((Boss03*)arg0)->unk0;
    temp_r0 = *(u32*)(temp_r1 + (temp_r2 * 0x3C));
    switch (temp_r0) {
        default:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            ((Boss03*)((temp_r2 * 0x2C) + temp_r1))->unk124 = 1;
            break;
    }
    temp_r0_2                   = temp_r1 + (temp_r2 * 8);
    ((Boss03*)temp_r0_2)->unkF0 = (s32)(((Boss03*)temp_lr)->unk28 - 0x80000);
    ((Boss03*)temp_r0_2)->unkF4 = (s32)(((Boss03*)temp_lr)->unk40 - 0x100000);
}

void func_ov017_021297f8(void* arg0) {
    SubroutineArgs subroutine_arg0;
    u16            spA;
    u16            sp8;
    u16            var_r3;
    u32            temp_r0;

    sp8     = data_ov017_02141c64.unk0;
    spA     = data_ov017_02141c64.unk2;
    temp_r0 = RNG_Next(0x64);
    var_r3  = 0;
loop_1:
    if ((u32)(&sp8)[var_r3] <= temp_r0) {
        var_r3 += 1;
        if ((u32)var_r3 < 3U) {
            goto loop_1;
        }
    }
    func_ov017_02129764(arg0, ((s32*)&subroutine_arg0)[var_r3]);
}

void func_ov017_02129878(void* arg0) {
    s16 temp_r1;

    if (((Boss03*)arg0)->unk90 == 0) {
        ((Boss03*)arg0)->unk90 = 0x64;
    }
    temp_r1 = ((Boss03*)arg0)->unk90;
    if ((s32)temp_r1 > 0) {
        func_02026590((u8*)(*(void**)arg0) + 0x54, 0, (u16)temp_r1);
        ((Boss03*)arg0)->unk90 = (s16)(((Boss03*)arg0)->unk90 - 1);
    }
    if ((s32)((Boss03*)arg0)->unk90 > 0) {
        return;
    }
    func_ov017_02129764(arg0, func_ov017_021298e0);
}

void func_ov017_021298e0(void* arg0) {
    if (((Boss03*)arg0)->unk90 == 0) {
        func_ov003_020d3c2c(arg0 + 0x48, &data_ov017_02141c88);
        ((Boss03*)arg0)->unk64 = (u16)(((Boss03*)arg0)->unk64 & ~1);
        func_ov003_020d3c2c(arg0 + 0x6C, &data_ov017_02141cb0);
        ((Boss03*)arg0)->unk88 = (u16)(((Boss03*)arg0)->unk88 & ~1);
    }
    ((Boss03*)arg0)->unk90 = (s16)(((Boss03*)arg0)->unk90 + 1);
    if (((u32)(((Boss03*)arg0)->unk64 << 0x1B) >> 0x1F) != 1) {
        return;
    }
    func_ov017_021297f8(arg0);
}

void func_ov017_02129958(void* arg0) {
    if (((Boss03*)arg0)->unk90 == 0) {
        func_ov003_020d3c2c(arg0 + 0x48, &data_ov017_02141cd8);
        ((Boss03*)arg0)->unk64 = (u16)(((Boss03*)arg0)->unk64 & ~1);
        func_ov003_020d3c2c(arg0 + 0x6C, &data_ov017_02141d40);
        ((Boss03*)arg0)->unk88 = (u16)(((Boss03*)arg0)->unk88 & ~1);
    }
    ((Boss03*)arg0)->unk90 = (s16)(((Boss03*)arg0)->unk90 + 1);
    if (((u32)(((Boss03*)arg0)->unk64 << 0x1B) >> 0x1F) != 1) {
        return;
    }
    func_ov017_021297f8(arg0);
}

s32 func_ov017_021299d0(void* arg1, u16* arg2) {
    u32    temp_ip;
    u32    temp_r3;
    void*  temp_lr;
    void*  temp_r0;
    void*  temp_r0_2;
    void*  temp_r0_3;
    void*  temp_r0_4;
    void*  temp_r0_5;
    void*  temp_r0_6;
    void*  temp_r1;
    void*  temp_r1_2;
    void*  temp_r1_3;
    void*  temp_r1_4;
    void*  temp_r1_5;
    void*  temp_r1_6;
    void*  temp_r2;
    void*  temp_r3_2;
    void*  temp_r3_3;
    void*  temp_r3_4;
    void*  temp_r5;
    void*  var_r0;
    void*  var_r0_2;
    void*  var_r0_3;
    void*  var_r0_4;
    void*  var_r1;
    void*  var_r1_2;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r5 = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    MI_CpuSet(temp_r4, 0, 0x9C);
    func_ov017_0212fc70();
    temp_lr                  = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    ((Boss03*)temp_r4)->unk0 = temp_lr;
    ((Boss03*)temp_lr)->unk2 = (u16)(((Boss03*)temp_lr)->unk2 | 1);
    temp_r0                  = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_02141c68, 1, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r1 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r1  = temp_r1 + ((Boss03*)temp_r1)->unk8;
    }
    ((Boss03*)temp_r4)->unkC = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r1,
                                                    g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x22C0);
    DMA_Flush();
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r0_2                = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_02141c68, 2, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0_2;
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_2 = ((Boss03*)temp_r0_2)->unk8 + 0x20;
        var_r1_2  = temp_r1_2 + ((Boss03*)temp_r1_2)->unk138;
    }
    ((Boss03*)temp_r4)->unk8 = PaletteMgr_AllocPalette(g_PaletteManagers[0], var_r1_2, 0, 0, 0xF);
    temp_r0_3                = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_3 = ((Boss03*)temp_r0_3)->unk8 + 0x20;
        var_r0    = temp_r1_3 + ((Boss03*)temp_r1_3)->unk8;
    }
    ((Boss03*)temp_r4)->unk38 = (void*)(var_r0 + 4);
    temp_r0_4                 = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_4 == NULL) {
        var_r0_2 = NULL;
    } else {
        temp_r1_4 = ((Boss03*)temp_r0_4)->unk8 + 0x20;
        var_r0_2  = temp_r1_4 + ((Boss03*)temp_r1_4)->unk98;
    }
    ((Boss03*)temp_r4)->unk3C = (void*)(var_r0_2 + 4);
    temp_r0_5                 = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_5 == NULL) {
        var_r0_3 = NULL;
    } else {
        temp_r1_5 = ((Boss03*)temp_r0_5)->unk8 + 0x20;
        var_r0_3  = temp_r1_5 + ((Boss03*)temp_r1_5)->unk128;
    }
    ((Boss03*)temp_r4)->unk40 = (void*)(var_r0_3 + 4);
    temp_r0_6                 = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_6 == NULL) {
        var_r0_4 = NULL;
    } else {
        temp_r1_6 = ((Boss03*)temp_r0_6)->unk8 + 0x20;
        var_r0_4  = temp_r1_6 + ((Boss03*)temp_r1_6)->unk130;
    }
    ((Boss03*)temp_r4)->unk44 = (void*)(var_r0_4 + 4);
    func_0200d1d8(temp_r4 + 0x10, 0, 1, 0, temp_r4 + 0x38, 2, 2);
    func_0200d858(temp_r4 + 0x10, 0, 0, 0);
    func_ov003_020d3b6c(temp_r4 + 0x48, 0, 0, 1, 0, 0);
    func_ov003_020d3bec(temp_r4 + 0x48, &data_ov017_02141c68, temp_r4 + 0x10, temp_r4 + 0x38, &data_ov017_02141c88);
    func_ov003_020d3b6c(temp_r4 + 0x6C, 0, 0, 1, 0, 0);
    func_ov003_020d3bec(temp_r4 + 0x6C, &data_ov017_02141c68, temp_r4 + 0x10, temp_r4 + 0x3C, &data_ov017_02141cb0);
    data_ov003_020e71b8->unk3D7C2 = 0;
    data_ov003_020e71b8->unk3D81A = 0;
    func_ov003_02083a2c(0, 0x40, 0x140, 0x1C0, 0x1E0, 0);
    func_ov003_02083a2c(1, 0x70, 0x1C8, 0x190, 0x200, 0);
    ((Boss03*)temp_r5)->unk18 = (s32)(0x160000 - ((Boss03*)temp_r5)->unk8);
    ((Boss03*)temp_r5)->unk1C = FX_Atan2Idx(0, 0x140);
    if ((arg2 != NULL) && (*arg2 != 0)) {
        func_ov017_02129764(temp_r4, func_ov017_021298e0);
    } else {
        temp_ip                     = data_ov003_020e71b8->unk3D7CC;
        temp_r2                     = data_ov003_020e71b8->unk3D898;
        ((Boss03*)temp_r2)->unk28   = (s32)((s32)(temp_ip + (temp_ip >> 0x1F)) >> 1);
        temp_r3                     = data_ov003_020e71b8->unk3D7D0;
        ((Boss03*)temp_r2)->unk2C   = (s32)((s32)(temp_r3 + (temp_r3 >> 0x1F)) >> 1);
        temp_r3_2                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_2)->unk28 = (s32)((Boss03*)temp_r3_2)->unk34;
        temp_r3_3                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_3)->unk2C = (s32)((Boss03*)temp_r3_3)->unk38;
        temp_r3_4                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_4)->unk30 = (s32)((Boss03*)temp_r3_4)->unk3C;
        ((Boss03*)temp_r5)->unk54   = -0x30000;
        func_ov017_02129764(temp_r4, func_ov017_02129878);
    }
    func_ov017_02129778(temp_r4);
    return 1;
}

s32 func_ov017_02129d78(void* arg1) {
    void (*temp_r1)(void*);
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk98;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_020d3cac(temp_r4 + 0x48);
    func_ov003_020d3cac(temp_r4 + 0x6C);
    func_ov017_0212fe1c(((Boss03*)temp_r4)->unk0);
    func_ov017_02129778(temp_r4);
    return 1;
}

s32 func_ov017_02129ddc(void) {
    return 1;
}

s32 func_ov017_02129de4(void* arg1) {
    void* temp_r2;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], ((Boss03*)temp_r4)->unkC);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], ((Boss03*)temp_r4)->unk8);
    func_0200d954(0, 1);
    func_0200d954(0, 2);
    func_ov003_020d3cf8(temp_r4 + 0x48);
    func_ov003_020d3cf8(temp_r4 + 0x6C);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r2                  = ((Boss03*)temp_r4)->unk0;
    ((Boss03*)temp_r2)->unk2 = (u16)(((Boss03*)temp_r2)->unk2 & ~1);
    return 1;
}

void func_ov017_02129e5c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141c78.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141c78.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141c78.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141c78.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_02129ea4(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk28C = arg1;
    ((Boss03*)arg0)->unk288 = 0;
    ((Boss03*)arg0)->unk280 = 0;
}

s16 func_ov017_02129ec8(void* arg0) {
    do {

    } while (((Boss03*)arg0)->unk290 == RNG_Next(4));
    ((Boss03*)arg0)->unk298 = 1;
}

void func_ov017_02129ef8(void* arg0) {
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;

    temp_r1 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk290 * 0xC));
    ((Boss03*)arg0)->unk28  = temp_r1;
    ((Boss03*)arg0)->unk1AC = temp_r1;
    temp_r1_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk290 * 0xC));
    ((Boss03*)arg0)->unk2C  = temp_r1_2;
    ((Boss03*)arg0)->unk1B0 = temp_r1_2;
    temp_r1_3               = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk290 * 0xC));
    ((Boss03*)arg0)->unk30  = temp_r1_3;
    ((Boss03*)arg0)->unk1B4 = temp_r1_3;
}

void func_ov017_02129f84(void* arg0) {
    s32   sp18;
    s32   sp14;
    s32   sp10;
    s16   spC;
    void* sp8;
    s32   temp_r0;
    s32   var_r8;
    s32   var_r9;

    var_r8 = 0;
    var_r9 = 0;
    sp8    = (void*)0x29;
    spC    = 0;
loop_1:
    if (*(s32*)((char*)&data_ov017_02143a04 + (var_r9 * 4)) == -1) {
        temp_r0 = ((Boss03*)arg0)->unk28;
        if (((Boss03*)arg0)->unkCA & 1) {
            sp10 = temp_r0 - 0x8000;
        } else {
            sp10 = temp_r0 + 0x8000;
        }
        sp14 = ((Boss03*)arg0)->unk2C;
        sp18 = ((Boss03*)arg0)->unk30;
        *(s32*)((char*)&data_ov017_02143a04 + (var_r9 * 4)) =
            EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f94, 0, 0, 0, &sp8);
        ((Boss03*)arg0)->unk292 = (u16)(((Boss03*)arg0)->unk292 + 1);
        var_r8 += 1;
        EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, *(s32*)((char*)&data_ov017_02143a04 + (var_r9 * 4)));
        if (var_r8 < 2) {
            goto block_6;
        }
    } else {
    block_6:
        var_r9 += 1;
        if (var_r9 < 5) {
            goto loop_1;
        }
    }
    ((Boss03*)arg0)->unk298 = 0;
}

void func_ov017_0212a08c(void* arg0) {
    void* var_r7;
    s32   var_r6;

    var_r7 = &data_ov017_02143a04;
    var_r6 = 0;
    do {
        if ((*(s32*)((char*)&data_ov017_02143a04 + (var_r6 * 4)) != -1) &&
            (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, var_r7) == 0))
        {
            ((Boss03*)arg0)->unk292 = (u16)(((Boss03*)arg0)->unk292 - 1);
        }
        var_r6 += 1;
        var_r7 += 4;
    } while (var_r6 < 5);
}

void func_ov017_0212a0f8(s32) {
    s32 var_r3;

    var_r3 = 0;
loop_1:
    s32 taskid = *(s32*)((char*)&data_ov017_02143a04 + (var_r3 * 4));
    if (taskid != -1) {
        EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, taskid);
        func_ov003_020c4ff4();
        return;
    }
    var_r3 += 1;
    if (var_r3 >= 5) {
        return;
    }
    goto loop_1;
}

void func_ov017_0212a148(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_02129ea4(arg0, func_ov017_0212a270);
}

void func_ov017_0212a170(void* arg0) {
    s32 temp_r0_2;
    u32 temp_r0;

    temp_r0 = ((Boss03*)arg0)->unk288;
    switch (temp_r0) {
        default:
            return;
        case 0:
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 8, 1);
            ((Boss03*)arg0)->unk288 = (u32)(((Boss03*)arg0)->unk288 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 9, 0);
            func_ov017_02130178(1, 0x4DF, 0x56C, ((Boss03*)arg0)->unk28);
            ((Boss03*)arg0)->unk288 = (u32)(((Boss03*)arg0)->unk288 + 1);
            return;
        case 2:
            temp_r0_2               = ((Boss03*)arg0)->unk280 + 1;
            ((Boss03*)arg0)->unk280 = temp_r0_2;
            if (temp_r0_2 < 0x3C) {
                return;
            }
            func_ov003_020c3acc(1, 0x2F5, ((Boss03*)arg0)->unk28);
            func_ov017_02129f84(arg0);
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 0xA, 1);
            ((Boss03*)arg0)->unk288 = (u32)(((Boss03*)arg0)->unk288 + 1);
            return;
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_02129ea4(arg0, func_ov017_0212a270);
            return;
    }
}

void func_ov017_0212a270(void* arg0) {
    s32 temp_r1;

    if (((Boss03*)arg0)->unk280 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_020c4b1c(arg0);
        ((Boss03*)arg0)->unk284 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk280 + 1;
    ((Boss03*)arg0)->unk280 = temp_r1;
    if (temp_r1 >= (s32)((Boss03*)arg0)->unk284) {
        if (func_ov003_020c4348(arg0) != 0) {
            func_ov017_02129ea4(arg0, func_ov017_0212a37c);
        } else {
            func_ov017_02129ea4(arg0, func_ov017_0212a270);
        }
    }
    if (data_ov017_02143a00 != 0) {
        data_ov017_02143a00 -= 1;
    }
    if (data_ov017_02143a00 == 0) {
        if ((data_ov017_021439dc.unk0 != 1) && (((Boss03*)arg0)->unk298 != 0)) {
            if ((u32)((Boss03*)arg0)->unk292 < 5U) {
                func_ov017_02129ea4(arg0, func_ov017_0212a170);
            }
        }
    }
    if (data_ov003_020e71b8->unk3D8A0 == arg0) {
        data_ov003_020e71b8->unk3D8A0 = 0;
    }
}

void func_ov017_0212a37c(void* arg0) {
    s16   sp14;
    s32   sp10;
    s32   spC;
    void* sp8;
    s16   var_r1;

    if (((Boss03*)arg0)->unk280 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 6, 1);
    }
    if (((Boss03*)arg0)->unk280 == 0x32) {
        sp8  = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk290 * 0xC));
        spC  = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk290 * 0xC));
        sp10 = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk290 * 0xC));
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        sp14 = var_r1;
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f30, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk280 = (s32)(((Boss03*)arg0)->unk280 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    ((Boss03*)arg0)->unk290 = (u16)func_ov017_02129ec8(arg0);
    func_ov017_02129ef8(arg0);
    func_ov017_02129ea4(arg0, func_ov017_0212a270);
}

void func_ov017_0212a4a8(void* arg0) {
    if (((Boss03*)arg0)->unk280 == 0) {
        if (((Boss03*)arg0)->unk294 == 1) {
            func_ov017_02130178(1, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(1, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk280 = (s32)(((Boss03*)arg0)->unk280 + 1);
    if (func_ov003_020c62c4(arg0, 3) != 0) {
        return;
    }
    ((Boss03*)arg0)->unk290 = (s16)func_ov017_02129ec8(arg0);
    func_ov017_02129ef8(arg0);
    func_ov017_02129ea4(arg0, func_ov017_0212a270);
    func_ov003_020c4b1c(arg0);
}

void func_ov017_0212a548(void* arg0) {
    if (((Boss03*)arg0)->unk280 != 0) {
        return;
    }
    ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
}

s32 func_ov017_0212a568(void* arg1, s32 arg2) {
    s32    var_r2;
    void*  temp_r2;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x2A0);
    func_ov003_020c3efc(temp_r4, arg2);
    data_ov017_02143980        = (s16)((Boss03*)temp_r4)->unk7C;
    ((Boss03*)temp_r4)->unk29C = 1;
    ((Boss03*)temp_r4)->unk298 = 1;
    ((Boss03*)temp_r4)->unk292 = 0;
    data_ov017_02143a00        = 0;
    ((Boss03*)temp_r4)->unk290 = 0;
    func_ov017_02129ef8(temp_r4);
    var_r2                    = 0;
    ((Boss03*)temp_r4)->unk38 = 0;
    ((Boss03*)temp_r4)->unk3C = 0;
    do {
        *(s32*)((char*)&data_ov017_02143a04 + (var_r2 * 4)) = -1;
        var_r2 += 1;
    } while (var_r2 < 5);
    temp_r2                   = data_ov003_020e71b8->unk3D898;
    ((Boss03*)temp_r2)->unk54 = (s32)(((Boss03*)temp_r2)->unk54 | 0x40000000);
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov017_0212ff54((CombatSprite*)(temp_r4 + 0x220), (CombatSprite*)(temp_r4 + 0x1C0));
    func_ov017_02129ea4(temp_r4, func_ov017_0212a148);
    func_ov017_0213021c(temp_r4);
    ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000000);
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x100000);
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x200000);
    func_ov003_0208722c();
    func_ov017_02130148();
    return 1;
}

s32 func_ov017_0212a6a4(void* arg1) {
    void (*temp_r1_2)(void*);
    s16    temp_r3;
    u16    temp_r1;
    u32    temp_r0;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov017_0212a08c(temp_r4);
    if (data_ov017_021439dc.unk0 == 1) {
        func_ov017_0212a0f8(temp_r4);
    }
    temp_r3 = data_ov003_020e71b8->unk3D8DA;
    if (((s32)data_ov003_020e71b8->unk3D8D8 < (s32)(((u32)(temp_r3 * 0xA) >> 0x1F) + ((temp_r3 * 0xA) / 100))) &&
        (((Boss03*)temp_r4)->unk28C != (void (*)(void*))func_ov017_0212a548))
    {
        ((Boss03*)temp_r4)->unk20 = 3;
        ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x10);
        func_ov017_021301e0();
    }
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk294 = temp_r0;
    switch (temp_r0) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
        default:
            break;
        case 1:
        case 2:
            func_ov017_02129ea4(temp_r4, func_ov017_0212a4a8);
            break;
        case 3:
            temp_r1 = ((Boss03*)temp_r4)->unk18C;
            if (temp_r1 & 0x10) {
                ((Boss03*)temp_r4)->unk18C = (u16)(temp_r1 | 0x20);
            } else {
                func_ov017_02129ea4(temp_r4, func_ov017_0212a548);
            }
            break;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk28C;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4628(temp_r4);
    func_ov017_0212ffcc(temp_r4 + 0x220, temp_r4 + 0x1C0);
    func_ov017_0213015c();
    return ((Boss03*)temp_r4)->unk29C;
}

s32 func_ov017_0212a7f0(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c4878(temp_r4);
    func_ov017_0212ffe4(temp_r4 + 0x220, temp_r4 + 0x1C0, ((Boss03*)temp_r4)->unk290);
    return 1;
}

s32 func_ov017_0212a81c(void* arg1) {
    s32 temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c48fc(temp_r4);
    func_ov017_02130130(temp_r4 + 0x220, temp_r4 + 0x1C0);
    return 1;
}

void func_ov017_0212a840(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141dd0.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141dd0.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141dd0.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141dd0.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212a888(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

s16 func_ov017_0212a8ac(void* arg0) {
    void* temp_r2;

    temp_r2 = data_ov003_020e71b8->unk3D898;
    FX_Atan2Idx(((Boss03*)temp_r2)->unk2C - ((Boss03*)arg0)->unk2C, ((Boss03*)temp_r2)->unk28 - ((Boss03*)arg0)->unk28);
}

s16 func_ov017_0212a8e4(void* arg0) {
    FX_Atan2Idx(((Boss03*)data_ov017_021439e0)->unk4 - ((Boss03*)arg0)->unk2C,
                ((Boss03*)data_ov017_021439e0)->unk0 - ((Boss03*)arg0)->unk28);
}

s32 func_ov017_0212a910(void* arg0) {
    s32   temp_ip;
    s32   temp_r1;
    u32   temp_r1_2;
    u32   temp_r3;
    void* temp_r0;

    temp_r0   = data_ov003_020e71b8->unk3D898;
    temp_ip   = ((Boss03*)temp_r0)->unk2C - ((Boss03*)arg0)->unk2C;
    temp_r1   = ((Boss03*)temp_r0)->unk28 - ((Boss03*)arg0)->unk28;
    temp_r3   = (temp_r1 * temp_r1) + 0x800;
    temp_r1_2 = (temp_ip * temp_ip) + 0x800;
    FX_Sqrt(((temp_r3 >> 0xC) | ((MULT_HI(temp_r1, temp_r1) + M2C_CARRY(temp_r3)) << 0x14)) +
            ((temp_r1_2 >> 0xC) | ((MULT_HI(temp_ip, temp_ip) + M2C_CARRY(temp_r1_2)) << 0x14)));
}

s32 func_ov017_0212a974(void* arg0) {
    s16 temp_r6;
    s32 temp_r0;
    s32 var_r1;
    s32 var_r4;
    u16 var_r2;

    var_r4  = 0;
    temp_r6 = (s16)func_ov017_0212a8ac(arg0);
    temp_r0 = (s32)func_ov017_0212a910(arg0);
    var_r1  = temp_r6 - data_ov017_021439de;
    if (var_r1 < 0) {
        var_r1 = 0 - var_r1;
    }
    if (var_r1 > 0x4000) {
        if (temp_r0 >= 0x80000) {
            var_r2 = 3;
        } else {
            var_r2 = 2;
        }
    } else {
        var_r2 = 1;
    }
    if (data_ov017_021439dc.unk0 == 1) {
        var_r2                 = 4;
        ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x1000);
    }
    if (((Boss03*)arg0)->unk1D4 != var_r2) {
        ((Boss03*)arg0)->unk1D4 = var_r2;
        func_ov017_0212a888(arg0, *(s32*)((char*)&data_ov017_02142f84 + (var_r2 * 4)));
        var_r4 = 1;
    }
    return var_r4;
}

void func_ov017_0212aa1c(void* arg0) {
    s16 temp_r2_2;
    s16 temp_r3;
    s32 temp_r1;
    s32 temp_r2;
    u32 temp_r2_3;
    u32 temp_r3_2;

    temp_r2   = ((Boss03*)arg0)->unk1DC;
    temp_r3   = *(&data_0205e4e0 + (((((s32)((Boss03*)arg0)->unk1D8 >> 4) * 2) + 1) * 2));
    temp_r3_2 = (temp_r3 * temp_r2) + 0x800;
    ((Boss03*)arg0)->unk28 =
        (s32)(((Boss03*)arg0)->unk28 + ((temp_r3_2 >> 0xC) | ((MULT_HI(temp_r3, temp_r2) + M2C_CARRY(temp_r3_2)) << 0x14)));
    temp_r1   = ((Boss03*)arg0)->unk1DC;
    temp_r2_2 = *(&data_0205e4e0 + (((s32)((Boss03*)arg0)->unk1D8 >> 4) * 4));
    temp_r2_3 = (temp_r2_2 * temp_r1) + 0x800;
    ((Boss03*)arg0)->unk2C =
        (s32)(((Boss03*)arg0)->unk2C + ((temp_r2_3 >> 0xC) | ((MULT_HI(temp_r2_2, temp_r1) + M2C_CARRY(temp_r2_3)) << 0x14)));
}

void func_ov017_0212aaa0(void* arg0) {
    s32   var_r5;
    s32   var_r7;
    u32   temp_r2;
    void* temp_r9;
    void* var_r8;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        data_ov017_021439d8 = 1;
    }
    if ((data_ov017_021439d8 != 0) && (((Boss03*)arg0)->unk1A8 == 3)) {
        func_ov017_02130178(0, 0x4EB, 0x56C, ((Boss03*)arg0)->unk28);
        temp_r2 = data_ov003_020e71b8->unk3D7CC;
        func_ov003_020c3acc(0, 0x303, (s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1);
        var_r8 = &data_02071cf0 + 0x20;
        var_r7 = 0;
        do {
            if (((Boss03*)var_r8)->unk74 != 0xFFFF) {
                temp_r9 = data_ov003_020e71b8->unk3D898;
                BtlBoss02_Badge_CreateTask(0, ((Boss03*)temp_r9)->unk28, ((Boss03*)temp_r9)->unk2C,
                                           ((Boss03*)temp_r9)->unk30 - 0x28000, 0x29, (s32)(u16)(RNG_Next(0x8000) + 0x2000));
            }
            var_r7 += 1;
            var_r8 += 0xA;
        } while (var_r7 < 6);
        var_r5 = func_ov003_0208495c((void*)((u8*)data_ov003_020e71b8 + 0x3D8BC));
        if (var_r5 != 0) {
            do {
                func_ov003_020a4390(var_r5, 4);
                var_r5 = func_ov003_02084984(var_r5 + 0x108);
            } while (var_r5 != 0);
        }
        data_ov017_021439d8 = 0;
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0212a888(arg0, func_ov017_0212ac58);
    ((Boss03*)arg0)->unk1D4 = 0;
    ((Boss03*)arg0)->unk1D0 = EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141dfc, 0, 0, 0, arg0);
}

void func_ov017_0212ac58(void* arg0) {
    s32 temp_ret;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_02084694(arg0 + 0x144, 0);
        temp_ret                = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1C4 = (s32)(((u32)temp_ret >> 0x1F) + (temp_ret / 10));
    }
    func_ov003_020c4b5c(arg0);
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    func_ov017_0212a974(arg0);
}

void func_ov017_0212acc8(void* arg0) {
    u32 temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 1, 0);
        temp_r0                 = (u32)func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1C4 = (s32)((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1);
        ((Boss03*)arg0)->unk1DC = 0x2000;
    }
    ((Boss03*)arg0)->unk1D8 = (s16)((s32)func_ov017_0212a8e4(arg0) + 0x8000);
    func_ov017_0212aa1c(arg0);
    func_ov003_020c4b5c(arg0);
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    func_ov017_0212a974(arg0);
}

void func_ov017_0212ad44(void* arg0) {
    u32 temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 1, 0);
        temp_r0                 = (u32)func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1C4 = (s32)((s32)(temp_r0 + (temp_r0 >> 0x1F)) >> 1);
        ((Boss03*)arg0)->unk1DC = 0x2000;
    }
    ((Boss03*)arg0)->unk1D8 = (s16)func_ov017_0212a8e4(arg0);
    func_ov017_0212aa1c(arg0);
    func_ov003_020c4b5c(arg0);
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    func_ov017_0212a974(arg0);
}

void func_ov017_0212adbc(void* arg0) {
    s32 temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 2, 0);
        temp_r0                 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1C4 = (s32)((s32)(temp_r0 + ((u32)(temp_r0 >> 1) >> 0x1E)) >> 2);
        ((Boss03*)arg0)->unk1DC = 0x8000;
    }
    ((Boss03*)arg0)->unk1D8 = (s16)func_ov017_0212a8e4(arg0);
    func_ov017_0212aa1c(arg0);
    func_ov003_020c4b5c(arg0);
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (data_ov017_021439dc.unk0 != 1) {
        if (data_ov017_021439dc.unk0 == 2) {
            func_ov017_0212a888(arg0, func_ov017_0212ac58);
            ((Boss03*)arg0)->unk1D4 = 0;
            return;
        }
        if ((s32)((Boss03*)arg0)->unk1C0 < (s32)((Boss03*)arg0)->unk1C4) {
            return;
        }
        func_ov017_0212a974(arg0);
        return;
    }
    func_ov017_0212a888(arg0, func_ov017_0212aea4);
    ((Boss03*)arg0)->unk1D4 = 4;
}

void func_ov017_0212aea4(void* arg0) {
    s32 temp_r1;
    s32 temp_r5;
    s32 temp_ret;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 2, 0);
        temp_ret                = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1C4 = (s32)(((u32)temp_ret >> 0x1F) + (temp_ret / 10));
        ((Boss03*)arg0)->unk1DC = 0x8000;
        temp_r5                 = (s32)func_ov017_0212a8ac(arg0);
        ((Boss03*)arg0)->unk1D8 = (s16)(temp_r5 + 0x4000 + RNG_Next(0x8000));
    }
    func_ov017_0212aa1c(arg0);
    func_ov003_020c4b5c(arg0);
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    func_ov017_0212a888(arg0, func_ov017_0212ac58);
    ((Boss03*)arg0)->unk1D4 = 0;
}

void func_ov017_0212af5c(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        if (((Boss03*)arg0)->unk1E0 == 1) {
            func_ov017_02130178(0, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(0, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (func_ov003_020c65cc(arg0, 3) != 0) {
        return;
    }
    func_ov017_0212a888(arg0, func_ov017_0212ac58);
    ((Boss03*)arg0)->unk1D4 = 0;
}

void func_ov017_0212afe8(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk18C = (u16)(((Boss03*)arg0)->unk18C | 1);
        func_ov003_020bb48c(0, *(s32*)((u8*)&data_02071cf0 + 0x3020 + 0x141), 0x105, ((Boss03*)arg0)->unk28,
                            ((Boss03*)arg0)->unk2C, ((Boss03*)arg0)->unk30, 0);
        if (((Boss03*)arg0)->unk1D0 != -1) {
            EasyTask_DeleteTask(&data_ov003_020e71b8->unk_10118, ((Boss03*)arg0)->unk1D0);
            ((Boss03*)arg0)->unk1D0 = -1;
        }
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
}

s32 func_ov017_0212b088(void* arg1, s32 arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1EC);
    func_ov003_020c3efc(temp_r4, arg2);
    ((Boss03*)temp_r4)->unk1E4 = 1;
    ((Boss03*)temp_r4)->unk1E8 = 1;
    ((Boss03*)temp_r4)->unk1D4 = 0;
    ((Boss03*)temp_r4)->unk1D6 = 0;
    data_ov017_02143984        = temp_r4;
    func_ov003_020c4520(temp_r4);
    func_ov003_020c4b5c(temp_r4);
    func_ov017_0212a888(temp_r4, func_ov017_0212aaa0);
    func_ov017_02130280(temp_r4);
    ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000000);
    return 1;
}

s32 func_ov017_0212b114(void* arg1) {
    void (*temp_r1)(void*);
    s16    temp_lr;
    u32    temp_r0;
    void** temp_r4;

    temp_lr = data_ov003_020e71b8->unk3D8DA;
    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((s32)data_ov003_020e71b8->unk3D8D8 < (s32)(((u32)(temp_lr * 0xA) >> 0x1F) + ((temp_lr * 0xA) / 100))) &&
        (((Boss03*)temp_r4)->unk1CC != (void (*)(void*))func_ov017_0212afe8))
    {
        ((Boss03*)temp_r4)->unk20     = 3;
        ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x10);
        data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
    }
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk1E0 = temp_r0;
    switch ((s32)temp_r0) { /* irregular */
        case 1:
        case 2:
            func_ov003_02084694(temp_r4 + 0x144, 0);
            func_ov017_0212a888(temp_r4, func_ov017_0212af5c);
            break;
        case 3:
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_0212a888(temp_r4, func_ov017_0212afe8);
            break;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    ((Boss03*)temp_r4)->unk1D6 = (u16)data_ov017_021439dc.unk0;
    func_ov003_020c4668(temp_r4);
    return ((Boss03*)temp_r4)->unk1E8;
}

s32 func_ov017_0212b248(void* arg1) {
    func_ov003_020c48b0(((Boss03*)arg1)->unk18);
    return 1;
}

s32 func_ov017_0212b25c(void* arg1) {
    func_ov003_020c492c(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212b270(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141dec.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141dec.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141dec.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141dec.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

s32 func_ov017_0212b2b8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    s32 temp_ip;
    s32 temp_r3;
    s32 temp_r5;
    s32 var_r0;
    s32 var_r1;
    s32 var_r4;
    u32 temp_r1;
    u32 temp_r3_2;

    temp_r3 = arg0 - arg3;
    temp_ip = arg1 - arg4;
    temp_r5 = arg2 + arg5;
    if (temp_r3 < 0) {
        var_r0 = 0 - temp_r3;
    } else {
        var_r0 = temp_r3;
    }
    var_r4 = 0;
    if (var_r0 < temp_r5) {
        if (temp_ip < 0) {
            var_r1 = 0 - temp_ip;
        } else {
            var_r1 = temp_ip;
        }
        if (var_r1 < (s32)((s32)(temp_r5 + ((u32)temp_r5 >> 0x1F)) >> 1)) {
            temp_r3_2 = (temp_r3 * temp_ip) + 0x800;
            temp_r1   = (temp_ip * temp_ip) + 0x800;
            if ((s32)(temp_r5 - FX_Sqrt(((temp_r3_2 >> 0xC) | ((MULT_HI(temp_r3, temp_ip) + M2C_CARRY(temp_r3_2)) << 0x14)) +
                                        ((temp_r1 >> 0xC) | ((MULT_HI(temp_ip, temp_ip) + M2C_CARRY(temp_r1)) << 0x14)))) > 0)
            {
                var_r4 = 1;
            }
        }
    }
    return var_r4;
}

u16 func_ov017_0212b340(void* arg0) {
    s32    temp_r4;
    s32    temp_r6;
    s32    temp_r6_2;
    s32    temp_r7;
    s32    var_r7;
    s32    var_r8;
    u32    var_r5;
    void*  temp_r0;
    void*  temp_r0_2;
    void** var_r10;
    void** var_r9;

    var_r10 = arg0;
    temp_r0 = *var_r10;
    var_r5  = 0;
    temp_r6 = ((Boss03*)temp_r0)->unk28;
    temp_r7 = ((Boss03*)temp_r0)->unk2C;
    var_r8  = 0;
    var_r9  = var_r10;
loop_1:
    if (func_ov017_0212b2b8(temp_r6, temp_r7, 0xC000, ((Boss03*)var_r9)->unk514, ((Boss03*)var_r9)->unk518, 0x8000) != 0) {
        var_r5 = 0x20000U >> 0x10;
    } else {
        var_r8 += 1;
        var_r9 += 0xC;
        if (var_r8 < 4) {
            goto loop_1;
        }
    }
    var_r7    = 0;
    temp_r0_2 = data_ov003_020e71b8->unk3D898;
    temp_r4   = ((Boss03*)temp_r0_2)->unk28;
    temp_r6_2 = ((Boss03*)temp_r0_2)->unk2C;
loop_5:
    if (func_ov017_0212b2b8(temp_r4, temp_r6_2, 0x8000, ((Boss03*)var_r10)->unk4E4, ((Boss03*)var_r10)->unk4E8, 0xC000) != 0) {
        var_r5 = (u32)(u16)(var_r5 + 1);
    } else {
        var_r7 += 1;
        var_r10 += 0xC;
        if (var_r7 < 4) {
            goto loop_5;
        }
    }
    return (u16)var_r5;
}

void func_ov017_0212b428(void* arg0) {
    func_ov017_0212b660(arg0);
    if (((Boss03*)arg0)->unk586 != 1) {
        func_ov017_02130178(0, 0x4E0, 0x56C, (*(Boss03**)arg0)->unk28);
        CombatSprite_LoadFromTable(0, arg0 + 0x424, &data_ov017_02141520, &data_ov017_021415a8, 0xE, 7, 0x30);
        func_ov003_02082730(arg0 + 0x424, 0x7FFFFFFE);
        CombatSprite_LoadFromTable(0, arg0 + 0x484, &data_ov017_02141520, &data_ov017_021415a8, 0xF, 7, 0x20);
        func_ov003_02082730(arg0 + 0x484, 0x7FFFFFFE);
    }
    CombatSprite_Update(arg0 + 0x424);
    CombatSprite_Update(arg0 + 0x484);
}

void func_ov017_0212b508(void* arg0) {
    if (((Boss03*)arg0)->unk586 != 1) {
        return;
    }
    CombatSprite_Release(arg0 + 0x424);
    CombatSprite_Release(arg0 + 0x484);
}

void func_ov017_0212b53c(void* arg0) {
    s16    temp_r1_2;
    s32    var_r5;
    s32    var_r9;
    void*  temp_r0;
    void*  temp_r0_2;
    void*  temp_r1;
    void** var_r8;

    func_ov017_0212b508(arg0);
    if (((Boss03*)arg0)->unk586 != 2) {
        func_ov017_02130178(0, 0x4B9, 0x56C, (*(Boss03**)arg0)->unk28);
        var_r5 = 0;
        do {
            temp_r1 = arg0 + (var_r5 * 2) + 0x500;
            var_r5 += 1;
            ((Boss03*)temp_r1)->unk80 = (s16)RNG_Next(0x1E);
        } while (var_r5 < 3);
    }
    var_r8 = arg0 + 0x304;
    var_r9 = 0;
    do {
        temp_r0   = arg0 + (var_r9 * 2);
        temp_r1_2 = ((Boss03*)temp_r0)->unk580;
        if ((s32)temp_r1_2 > 0) {
            ((Boss03*)temp_r0)->unk580 = (s16)(temp_r1_2 - 1);
        }
        if (((Boss03*)(arg0 + (var_r9 * 2)))->unk580 == 0) {
            CombatSprite_LoadFromTable(0, (CombatSprite*)var_r8, &data_ov017_02141520, &data_ov017_021415a8, 0xD, 7, 0x18);
            temp_r0_2                    = arg0 + (var_r9 * 2);
            ((Boss03*)temp_r0_2)->unk580 = (s16)(((Boss03*)temp_r0_2)->unk580 - 1);
            func_ov003_02082730((CombatSprite*)var_r8, 0x7FFFFFFE);
        }
        if (((Boss03*)(arg0 + (var_r9 * 2)))->unk580 == 0xFFFFFFFF) {
            CombatSprite_Update((CombatSprite*)var_r8);
        }
        var_r9 += 1;
        var_r8 += 0x60;
    } while (var_r9 < 3);
}

void func_ov017_0212b660(void* arg0) {
    s32   var_r5;
    void* var_r6;

    var_r6 = arg0 + 0x304;
    var_r5 = 0;
    do {
        if (((Boss03*)(arg0 + (var_r5 * 2)))->unk580 == -1) {
            CombatSprite_Release(var_r6);
        }
        var_r5 += 1;
        var_r6 += 0x60;
    } while (var_r5 < 3);
}

s32 func_ov017_0212b6a4(void* arg1, void* arg2) {
    s32    var_r8;
    void** temp_r7;
    void** var_r10;
    void** var_r9;

    temp_r7 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r7, 0, 0x58C);
    var_r8                     = 0;
    ((Boss03*)temp_r7)->unk0   = arg2;
    ((Boss03*)temp_r7)->unk588 = 0;
    data_ov017_021439dc.unk0   = 0;
    var_r9                     = temp_r7 + 4;
    var_r10                    = temp_r7 + 0x184;
    do {
        func_ov003_02087fe0(0, var_r9, 4);
        func_ov003_02087fe0(0, var_r10, 2);
        var_r8 += 1;
        var_r9 += 0x60;
        var_r10 += 0x60;
    } while (var_r8 < 4);
    data_ov017_021439dc.unk4   = (void**)(temp_r7 + 0x538);
    data_ov017_021439dc.unk8   = (void**)(temp_r7 + 0x508);
    (*(Boss03**)temp_r7)->unk4 = 0;
    (*(Boss03**)temp_r7)->unk8 = 0x64;
    (*(Boss03**)temp_r7)->unkA = 0x64;
    return 1;
}

s32 func_ov017_0212b770(void* arg1) {
    s32    sp10;
    s32    spC;
    void*  sp8;
    s32    sp4;
    s32    sp0;
    s16    temp_r3;
    s16    temp_r4;
    s32    temp_r2;
    s32    var_r11;
    s32    var_r4;
    s32    var_r5;
    s32    var_r6;
    s32    var_r7;
    u16    temp_r0_3;
    u32    temp_r3_2;
    u32    temp_r9;
    void*  temp_r0;
    void*  temp_r0_2;
    void*  temp_r7;
    void*  var_r8;
    void** var_r10;
    void** var_r9;

    temp_r0   = ((Boss03*)arg1)->unk18;
    sp8       = temp_r0;
    temp_r2   = ((s32)((Boss03*)temp_r0)->unk588 >> 4) * 2;
    temp_r7   = data_ov003_020e71b8->unk3D898;
    temp_r3   = *(s16*)((u8*)&data_0205e4e0 + (temp_r2 * 2));
    var_r6    = ((Boss03*)temp_r7)->unk28;
    temp_r4   = *(&data_0205e4e0 + ((temp_r2 + 1) * 2));
    temp_r0_2 = ((Boss03*)temp_r0)->unk0;
    temp_r9   = temp_r3 * 0x18;
    sp4       = temp_r4 * 0x18;
    temp_r3_2 = temp_r3 * 0xC;
    var_r4    = ((Boss03*)temp_r0_2)->unk28;
    var_r5    = ((Boss03*)temp_r0_2)->unk2C;
    sp10      = temp_r9 + (temp_r9 >> 0x1F);
    var_r8    = sp8;
    spC       = temp_r3_2 + (temp_r3_2 >> 0x1F);
    var_r7    = ((Boss03*)temp_r7)->unk2C;
    sp0       = temp_r4 * 0xC;
    var_r9    = var_r8 + 4;
    var_r10   = var_r8 + 0x184;
    var_r11   = 0;
    do {
        var_r4 += sp4;
        ((Boss03*)var_r8)->unk4E4 = var_r4;
        var_r5 += sp10 >> 1;
        ((Boss03*)var_r8)->unk4E8 = var_r5;
        CombatSprite_Update((CombatSprite*)var_r9);
        var_r6 += sp0;
        ((Boss03*)var_r8)->unk514 = var_r6;
        var_r7 += spC >> 1;
        ((Boss03*)var_r8)->unk518 = var_r7;
        CombatSprite_Update((CombatSprite*)var_r10);
        var_r11 += 1;
        var_r8 += 0xC;
        var_r9 += 0x60;
        var_r10 += 0x60;
    } while (var_r11 < 4);
    ((Boss03*)sp8)->unk586   = (u16)data_ov017_021439dc.unk0;
    temp_r0_3                = func_ov017_0212b340(arg1);
    data_ov017_021439dc.unk0 = temp_r0_3;
    switch (temp_r0_3) { /* irregular */
        case 1:
            func_ov017_0212b428(sp8);
            if ((s32)data_ov017_021439dc.unkC <= 0) {
                func_ov003_020c3acc(0, 0x2F8, (*(Boss03**)sp8)->unk28);
                data_ov017_021439dc.unkC = 0x1B;
            }
            data_ov017_021439dc.unkC = (s32)(data_ov017_021439dc.unkC - 1);
            break;
        case 2:
            func_ov017_0212b53c(sp8);
            func_ov003_0208a1e8(func_ov003_0208a114(0xEA), ((Boss03*)sp8)->unk0 + 4,
                                ((Boss03*)data_ov003_020e71b8->unk3D898)->unk28);
            if ((s32)data_ov017_021439dc.unkC <= 0) {
                func_ov003_020c3acc(0, 0x2F9, ((Boss03*)data_ov003_020e71b8->unk3D898)->unk28);
                data_ov017_021439dc.unkC = 0x1B;
            }
            data_ov017_021439dc.unkC = (s32)(data_ov017_021439dc.unkC - 1);
            break;
        default:
            func_ov017_0212b508(sp8);
            func_ov017_0212b660(sp8);
            data_ov017_021439dc.unkC = 0;
            break;
    }
    ((Boss03*)sp8)->unk588   = (u16)(((Boss03*)sp8)->unk588 + 0x48);
    data_ov017_021439dc.unk2 = (u16)((Boss03*)sp8)->unk588;
    return 1;
}

s32 func_ov017_0212b9d0(void* arg1) {
    s32    sp4;
    s32    sp0;
    void*  var_r7_2;
    s16    temp_r4_2;
    s16    temp_r4_3;
    s16    temp_r5_2;
    s32    var_r10;
    s32    var_r4;
    s32    var_r5;
    s32    var_r8;
    void*  temp_r1;
    void*  temp_r4;
    void*  temp_r5;
    void*  var_r9;
    void** temp_r6;
    void** var_r7;
    void** var_r8_2;

    temp_r6                          = ((Boss03*)arg1)->unk18;
    temp_r5                          = &data_ov017_02141e18 + 0x10;
    ((Boss03DataStruct*)&sp4)->unk0  = (s32)data_ov017_02141e18.unk0;
    ((Boss03DataStruct*)&sp4)->unk4  = (s32)data_ov017_02141e18.unk4;
    ((Boss03DataStruct*)&sp4)->unk8  = (s32)data_ov017_02141e18.unk8;
    ((Boss03DataStruct*)&sp4)->unkC  = (s32)data_ov017_02141e18.unkC;
    temp_r4                          = &sp4 + 0x10;
    ((Boss03DataStruct*)&sp4)->unk10 = (s32)data_ov017_02141e18.unk10;
    ((Boss03*)temp_r4)->unk4         = (s32)((Boss03*)temp_r5)->unk4;
    ((Boss03*)temp_r4)->unk8         = (s32)((Boss03*)temp_r5)->unk8;
    ((Boss03*)temp_r4)->unkC         = (s32)((Boss03*)temp_r5)->unkC;
    var_r7                           = temp_r6;
    ((Boss03*)temp_r4)->unk10        = (s32)((Boss03*)temp_r5)->unk10;
    var_r5                           = temp_r6 + 4;
    var_r8                           = temp_r6 + 0x184;
    var_r4                           = 0;
    do {
        func_ov003_02088064(var_r5, ((Boss03*)var_r7)->unk4E4, ((Boss03*)var_r7)->unk4E8);
        func_ov003_02088064(var_r8, ((Boss03*)var_r7)->unk514, ((Boss03*)var_r7)->unk518);
        var_r4 += 1;
        var_r7 += 0xC;
        var_r5 += 0x60;
        var_r8 += 0x60;
    } while (var_r4 < 4);
    switch (data_ov017_021439dc.unk0) { /* irregular */
        case 1:
            temp_r1   = *temp_r6;
            temp_r4_2 = func_ov003_020843b0(0, ((Boss03*)*temp_r6)->unk28);
            CombatSprite_SetPosition((CombatSprite*)temp_r6 + 0x424, temp_r4_2,
                                     func_ov003_020843ec(0, ((Boss03*)temp_r1)->unk2C - 0x18000, ((Boss03*)temp_r1)->unk30));
            func_ov003_02082730((CombatSprite*)temp_r6 + 0x424, 0x7FFF7FFF - ((Boss03*)*temp_r6)->unk2C);
            CombatSprite_Render((CombatSprite*)temp_r6 + 0x424);
            temp_r4_3 = func_ov003_020843b0(0, ((Boss03*)data_ov003_020e71b8->unk3D898)->unk28);
            CombatSprite_SetPosition((CombatSprite*)temp_r6 + 0x484, temp_r4_3,
                                     func_ov003_020843ec(0, ((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C, 0));
            func_ov003_02082730((CombatSprite*)temp_r6 + 0x484, 0x7FFF7FFF);
            CombatSprite_Render((CombatSprite*)temp_r6 + 0x484);
            break;
        case 2:
            var_r7_2 = &sp4;
            var_r8_2 = temp_r6;
            var_r9   = temp_r6 + 0x304;
            var_r10  = 0;
            sp0      = -1;
            do {
                if (((Boss03*)(temp_r6 + (var_r10 * 2)))->unk580 == sp0) {
                    ((Boss03*)var_r8_2)->unk544 =
                        (s32)(((Boss03*)data_ov003_020e71b8->unk3D898)->unk28 + ((Boss03*)var_r7_2)->unk0);
                    ((Boss03*)var_r8_2)->unk548 =
                        (s32)(((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C + ((Boss03*)var_r7_2)->unk4);
                    ((Boss03*)var_r8_2)->unk54C =
                        (s32)(((Boss03*)data_ov003_020e71b8->unk3D898)->unk30 + ((Boss03*)var_r7_2)->unk8);
                    temp_r5_2 = func_ov003_020843b0(0, ((Boss03*)var_r8_2)->unk544);
                    CombatSprite_SetPosition(var_r9, temp_r5_2,
                                             func_ov003_020843ec(0, ((Boss03*)var_r8_2)->unk548, ((Boss03*)var_r8_2)->unk54C));
                    func_ov003_02082730((CombatSprite*)temp_r6 + 0x424,
                                        0x7FFF7FFF - ((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C);
                    CombatSprite_Render((CombatSprite*)var_r9);
                }
                var_r10 += 1;
                var_r7_2 += 0xC;
                var_r8_2 += 0xC;
                var_r9 += 0x60;
            } while (var_r10 < 3);
            break;
    }
    return 1;
}

s32 func_ov017_0212bc54(void* arg1) {
    s32   var_r5;
    void* temp_r4;
    void* var_r6;
    void* var_r7;

    temp_r4 = ((Boss03*)arg1)->unk18;
    var_r5  = 0;
    var_r6  = temp_r4 + 4;
    var_r7  = temp_r4 + 0x184;
    do {
        CombatSprite_Release(var_r6);
        CombatSprite_Release(var_r7);
        var_r5 += 1;
        var_r6 += 0x60;
        var_r7 += 0x60;
    } while (var_r5 < 4);
    func_ov017_0212b508(temp_r4);
    func_ov017_0212b660(temp_r4);
    return 1;
}

void func_ov017_0212bca4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141e08.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141e08.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141e08.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141e08.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212bcec(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0212bd10(void* arg0) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r2;

    data_ov017_021439ec     = 0;
    ((Boss03*)arg0)->unk1D4 = (u16)RNG_Next(4);
    temp_r0 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk1D4 * 0xC));
    ((Boss03*)arg0)->unk28  = temp_r0;
    ((Boss03*)arg0)->unk1AC = temp_r0;
    temp_r0_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk1D4 * 0xC));
    ((Boss03*)arg0)->unk2C  = temp_r0_2;
    ((Boss03*)arg0)->unk1B0 = temp_r0_2;
    temp_r2                 = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk1D4 * 0xC));
    ((Boss03*)arg0)->unk30  = temp_r2;
    ((Boss03*)arg0)->unk1B4 = temp_r2;
    func_ov017_0212bcec(arg0, func_ov017_0212bf3c);
}

void func_ov017_0212bdcc(void* arg0) {
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;

    if (RNG_Next(0x64) < 0x50U) {
        func_ov017_0212bd10(arg0);
        return;
    }
    ((Boss03*)arg0)->unk1D4 = (u16)RNG_Next(4);
    temp_r0 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk1D4 * 0xC));
    ((Boss03*)arg0)->unk28  = temp_r0;
    ((Boss03*)arg0)->unk1AC = temp_r0;
    temp_r0_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk1D4 * 0xC));
    ((Boss03*)arg0)->unk2C  = temp_r0_2;
    ((Boss03*)arg0)->unk1B0 = temp_r0_2;
    temp_r0_3               = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk1D4 * 0xC));
    ((Boss03*)arg0)->unk30  = temp_r0_3;
    ((Boss03*)arg0)->unk1B4 = temp_r0_3;
}

void func_ov017_0212be84(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov017_02130178(1, 0x4E3, 0x56C, ((Boss03*)arg0)->unk28);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0212bcec(arg0, func_ov017_0212bee0);
}

void func_ov017_0212bee0(void* arg0) {
    s32 temp_r1;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_020c4b1c(arg0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    func_ov017_0212bdcc(arg0);
}

void func_ov017_0212bf3c(void* arg0) {
    if (data_ov017_021439ec != 1) {
        return;
    }
    func_ov017_0212bcec(arg0, func_ov017_0212bee0);
}

void func_ov017_0212bf64(void* arg0) {
    s32 temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        if (((Boss03*)arg0)->unk1D0 == 1) {
            func_ov017_02130178(1, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(1, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    temp_r0                 = func_ov003_020c62c4(arg0, 3);
    ((Boss03*)arg0)->unk1DC = 1;
    if (temp_r0 != 0) {
        return;
    }
    func_ov017_0212bdcc(arg0);
    func_ov017_0212bcec(arg0, func_ov017_0212bee0);
    ((Boss03*)arg0)->unk1DC = 0;
    if (data_ov003_020e71b8->unk3D8A0 == arg0) {
        data_ov003_020e71b8->unk3D8A0 = 0;
    }
}

void func_ov017_0212c018(void* arg0) {
    s32 var_r0;

    if (data_ov017_021439ec == 1) {
        var_r0 = func_ov003_020c6f90();
    } else {
        var_r0 = func_ov003_020c6f40();
    }
    ((Boss03*)arg0)->unk1DC = 1;
    if (var_r0 != 0) {
        return;
    }
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
    ((Boss03*)arg0)->unk1D8       = 0;
}

s32 func_ov017_0212c078(void* arg1, s32 arg2) {
    s32    temp_r0;
    s32    temp_r1;
    s32    temp_r3;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1E0);
    func_ov003_020c3efc(temp_r4, arg2);
    data_ov017_02143980        = (s16)((Boss03*)temp_r4)->unk7C;
    ((Boss03*)temp_r4)->unk1D8 = 1;
    ((Boss03*)temp_r4)->unk1DC = 0;
    ((Boss03*)temp_r4)->unk1D4 = (u16)RNG_Next(4);
    ((Boss03*)temp_r4)->unk38  = 0;
    ((Boss03*)temp_r4)->unk3C  = 0;
    temp_r0 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)temp_r4)->unk1D4 * 0xC));
    ((Boss03*)temp_r4)->unk28  = temp_r0;
    ((Boss03*)temp_r4)->unk1AC = temp_r0;
    temp_r3 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)temp_r4)->unk1D4 * 0xC));
    ((Boss03*)temp_r4)->unk2C  = temp_r3;
    ((Boss03*)temp_r4)->unk1B0 = temp_r3;
    temp_r1                    = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)temp_r4)->unk1D4 * 0xC));
    ((Boss03*)temp_r4)->unk30  = temp_r1;
    ((Boss03*)temp_r4)->unk1B4 = temp_r1;
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov017_0212bcec(temp_r4, func_ov017_0212be84);
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141e84, 0, 0, 0, temp_r4);
    func_ov017_0213021c(temp_r4);
    ((Boss03*)temp_r4)->unk54     = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000000);
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x100000);
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x200000);
    func_ov003_0208722c();
    func_ov017_02130148();
    return 1;
}

s32 func_ov017_0212c20c(void* arg1) {
    void (*temp_r1_2)(void*);
    u16    temp_r1;
    u32    temp_r0;
    void** temp_r4;

    temp_r4                    = ((Boss03*)arg1)->unk18;
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk1D0 = temp_r0;
    switch (temp_r0) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
        default:
            break;
        case 1:
        case 2:
            func_ov017_0212bcec(temp_r4, func_ov017_0212bf64);
            break;
        case 3:
            temp_r1 = ((Boss03*)temp_r4)->unk18C;
            if (temp_r1 & 0x10) {
                ((Boss03*)temp_r4)->unk18C = (u16)(temp_r1 | 0x20);
            } else {
                func_ov017_0212bcec(temp_r4, func_ov017_0212c018);
            }
            break;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    if (data_ov017_021439ec != 1) {
        ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x30);
    } else {
        ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 & ~0x10 & ~0x20);
        data_ov017_021439f0       = temp_r4;
    }
    func_ov003_020c4628(temp_r4);
    func_ov017_0213015c();
    return ((Boss03*)temp_r4)->unk1D8;
}

s32 func_ov017_0212c300(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((Boss03*)temp_r4)->unk1DC != 0) {
        func_ov003_020c4878(temp_r4);
    } else {
        ((Boss03*)temp_r4)->unk34 = func_ov003_020843b0(1, ((Boss03*)temp_r4)->unk28);
        ((Boss03*)temp_r4)->unk36 = func_ov003_020843ec(1, ((Boss03*)temp_r4)->unk2C, ((Boss03*)temp_r4)->unk30);
    }
    return 1;
}

s32 func_ov017_0212c34c(void* arg1) {
    func_ov003_020c48fc(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212c360(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141e58.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141e58.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141e58.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141e58.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212c3a8(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0212c3cc(void* arg0) {
    s32   temp_r6;
    void* temp_r4;

    temp_r4 = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    temp_r6 = (s32)((Boss03*)temp_r4)->unk10 >> 0xC;
    ((Boss03*)arg0)->unk28 =
        (s32)(((Boss03*)temp_r4)->unk4 + (RNG_Next(((s32)((Boss03*)temp_r4)->unkC >> 0xC) - 0x40) << 0xC) + 0x20000);
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)temp_r4)->unk8 + (RNG_Next(temp_r6 - 0x20) << 0xC) + 0x10000);
}

void func_ov017_0212c42c(void* arg0) {
    s16 temp_r2_2;
    s16 temp_r3;
    s32 temp_r1;
    s32 temp_r2;
    u32 temp_r2_3;
    u32 temp_r3_2;

    temp_r2   = ((Boss03*)arg0)->unk1D8;
    temp_r3   = *(&data_0205e4e0 + (((((s32)((Boss03*)arg0)->unk1D4 >> 4) * 2) + 1) * 2));
    temp_r3_2 = (temp_r3 * temp_r2) + 0x800;
    ((Boss03*)arg0)->unk28 =
        (s32)(((Boss03*)arg0)->unk28 + ((temp_r3_2 >> 0xC) | ((MULT_HI(temp_r3, temp_r2) + M2C_CARRY(temp_r3_2)) << 0x14)));
    temp_r1   = ((Boss03*)arg0)->unk1D8;
    temp_r2_2 = *(&data_0205e4e0 + (((s32)((Boss03*)arg0)->unk1D4 >> 4) * 4));
    temp_r2_3 = (temp_r2_2 * temp_r1) + 0x800;
    ((Boss03*)arg0)->unk2C =
        (s32)(((Boss03*)arg0)->unk2C + ((temp_r2_3 >> 0xC) | ((MULT_HI(temp_r2_2, temp_r1) + M2C_CARRY(temp_r2_3)) << 0x14)));
}

void func_ov017_0212c4b0(void* arg0) {
    data_ov017_021439ec = 1;
    func_ov017_0212c3cc(arg0);
    func_ov017_0212c3a8(arg0, func_ov017_0212c698);
}

void func_ov017_0212c4e0(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0212c3a8(arg0, func_ov017_0212c508);
}

void func_ov017_0212c508(void* arg0) {
    s32 temp_r1;
    u32 temp_r0;
    u32 temp_r1_2;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_02084694(arg0 + 0x144, 0);
        func_ov003_020c4b5c(arg0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    temp_r0 = RNG_Next(0x64);
    if (temp_r0 < 5U) {
        func_ov017_0212c4b0(arg0);
        return;
    }
    temp_r1_2 = temp_r0 >> 0x1F;
    if ((temp_r1_2 + ROTATE_RIGHT((temp_r0 << 0x1F) - temp_r1_2, 0x1F)) != 0) {
        func_ov017_0212c3a8(arg0, func_ov017_0212c5b8);
        return;
    }
    func_ov017_0212c3a8(arg0, func_ov017_0212c628);
}

void func_ov017_0212c5b8(void* arg0) {
    s32 temp_r1;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1D8 = 0x2000;
        ((Boss03*)arg0)->unk1D4 = (s16)RNG_Next(0x10000);
    }
    func_ov017_0212c42c(arg0);
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    func_ov017_0212c3a8(arg0, func_ov017_0212c508);
}

void func_ov017_0212c628(void* arg0) {
    s32 temp_r1;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1D8 = 0x6000;
        ((Boss03*)arg0)->unk1D4 = (s16)RNG_Next(0x10000);
    }
    func_ov017_0212c42c(arg0);
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    func_ov017_0212c3a8(arg0, func_ov017_0212c508);
}

void func_ov017_0212c698(void* arg0) {
    if (data_ov017_021439ec != 0) {
        return;
    }
    func_ov017_0212c3a8(arg0, func_ov017_0212c508);
}

void func_ov017_0212c6c0(void* arg0) {
    s32 temp_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        if (((Boss03*)arg0)->unk1D0 == 1) {
            func_ov017_02130178(0, 0x4E0, 0x4E1, ((Boss03*)arg0)->unk28);
        } else {
            func_ov017_02130178(0, 0x4E2, 0x56C, ((Boss03*)arg0)->unk28);
        }
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    temp_r0                 = func_ov003_020c65cc(arg0, 3);
    ((Boss03*)arg0)->unk1DC = 1;
    if (temp_r0 != 0) {
        return;
    }
    func_ov017_0212c4b0(arg0);
    func_ov017_0212c3a8(arg0, func_ov017_0212c508);
    ((Boss03*)arg0)->unk1DC = 0;
}

void func_ov017_0212c758(void* arg0) {
    s32 var_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c3acc(0, 0x4E5, ((Boss03*)arg0)->unk28);
    }
    if (data_ov017_021439ec == 0) {
        var_r0 = func_ov003_020c6f90();
    } else {
        var_r0 = func_ov003_020c6f68();
    }
    ((Boss03*)arg0)->unk1DC = 1;
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (var_r0 == 0) {
        ((Boss03*)arg0)->unk1E0 = 0;
    }
}

s32 func_ov017_0212c7c8(void* arg1, s32 arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1E4);
    func_ov003_020c3efc(temp_r4, arg2);
    ((Boss03*)temp_r4)->unk1E0 = 1;
    ((Boss03*)temp_r4)->unk1DC = 0;
    data_ov017_021439ec        = 0;
    data_ov017_021439f0        = temp_r4;
    func_ov003_020c4520(temp_r4);
    func_ov003_020c4b5c(temp_r4);
    func_ov017_0212c3a8(temp_r4, func_ov017_0212c4e0);
    func_ov017_02130280(temp_r4);
    ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000000);
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141ea0, 0, 0, 0, temp_r4);
    BtlBoss02_Badge_CreateTask(1, 0, 0, 0, 0, 0U);
    return 1;
}

s32 func_ov017_0212c8a4(void* arg1) {
    void (*temp_r1)(void*);
    s32    temp_r0;
    void** temp_r4;

    temp_r4                    = ((Boss03*)arg1)->unk18;
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk1D0 = temp_r0;
    switch (temp_r0) { /* irregular */
        case 1:
        case 2:
            func_ov003_02084694(temp_r4 + 0x144, 0);
            func_ov017_0212c3a8(temp_r4, func_ov017_0212c6c0);
            break;
        case 3:
            func_ov003_02084694(temp_r4 + 0x144, 1);
            func_ov017_0212c3a8(temp_r4, func_ov017_0212c758);
            break;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    if (data_ov017_021439ec != 0) {
        ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x30);
    } else {
        ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 & ~0x10 & ~0x20);
        data_ov017_021439f0       = temp_r4;
    }
    func_ov003_020c4668(temp_r4);
    return ((Boss03*)temp_r4)->unk1E0;
}

s32 func_ov017_0212c98c(void* arg1) {
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (((Boss03*)temp_r4)->unk1DC != 0) {
        func_ov003_020c48b0(temp_r4);
    } else {
        ((Boss03*)temp_r4)->unk34 = func_ov003_020843b0(0, ((Boss03*)temp_r4)->unk28);
        ((Boss03*)temp_r4)->unk36 = func_ov003_020843ec(0, ((Boss03*)temp_r4)->unk2C, ((Boss03*)temp_r4)->unk30);
    }
    return 1;
}

s32 func_ov017_0212c9d8(void* arg1) {
    func_ov003_020c492c(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212c9ec(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141e74.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141e74.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141e74.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141e74.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

u16 func_ov017_0212ca34(void*) {
    s16    temp_r0;
    s16    temp_r4;
    s16    var_r2;
    void*  temp_r5;
    void** temp_r6;

    temp_r5 = data_ov003_020e71b8->unk3D89C;
    temp_r6 = data_ov017_021439f0;
    temp_r4 = func_ov003_020843b0(1, ((Boss03*)temp_r5)->unk28);
    temp_r0 = func_ov003_020843ec(1, ((Boss03*)temp_r5)->unk2C, 0);
    var_r2  = ((Boss03*)temp_r6)->unk36;
    if (data_ov017_021439ec != 1) {
        var_r2 += 0xC0;
    }
    return (u16)(FX_Atan2Idx(var_r2 - temp_r0, ((Boss03*)temp_r6)->unk34 - temp_r4) + 0x8000);
}

s32 func_ov017_0212cab0(void* arg1, void* arg2) {
    s32    var_r6;
    void** temp_r4;
    void** var_r7;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x188);
    ((Boss03*)temp_r4)->unk0   = arg2;
    var_r6                     = 0;
    var_r7                     = temp_r4 + 4;
    ((Boss03*)temp_r4)->unk184 = 0;
    do {
        func_ov003_02087fe0(1, var_r7, 2);
        var_r6 += 1;
        var_r7 += 0x60;
    } while (var_r6 < 4);
    return 1;
}

s32 func_ov017_0212cb10(void* arg1) {
    s32   var_r5;
    void* temp_r4;
    void* var_r6;

    temp_r4 = ((Boss03*)arg1)->unk18;
    var_r5  = 0;
    var_r6  = temp_r4 + 4;
    do {
        CombatSprite_Update(var_r6);
        var_r5 += 1;
        var_r6 += 0x60;
    } while (var_r5 < 4);
    ((Boss03*)temp_r4)->unk184 = func_ov017_0212ca34(temp_r4);
    return 1;
}

s32 func_ov017_0212cb50(void* arg1) {
    s32   temp_r2;
    s32   temp_r4;
    s32   temp_r7;
    s32   var_r5;
    s32   var_r6;
    s32   var_r8;
    u32   temp_r2_2;
    void* temp_ip;
    void* temp_r0;
    void* var_r9;

    temp_r0   = ((Boss03*)arg1)->unk18;
    temp_r2   = ((s32)((Boss03*)temp_r0)->unk184 >> 4) * 2;
    temp_ip   = data_ov003_020e71b8->unk3D89C;
    temp_r2_2 = *(s16*)((u8*)&data_0205e4e0 + (temp_r2 * 2)) * 0xC;
    temp_r7   = *(&data_0205e4e0 + ((temp_r2 + 1) * 2)) * 0xC;
    var_r5    = ((Boss03*)temp_ip)->unk28;
    var_r6    = ((Boss03*)temp_ip)->unk2C;
    temp_r4   = temp_r2_2 + (temp_r2_2 >> 0x1F);
    var_r9    = temp_r0 + 4;
    var_r8    = 0;
    do {
        var_r5 += temp_r7;
        var_r6 += temp_r4 >> 1;
        func_ov003_02088064(var_r9, var_r5, var_r6);
        var_r8 += 1;
        var_r9 += 0x60;
    } while (var_r8 < 4);
    return 1;
}

s32 func_ov017_0212cbe8(void* arg1) {
    s32    var_r4;
    void** var_r5;

    var_r4 = 0;
    var_r5 = ((Boss03*)arg1)->unk18 + 4;
    do {
        CombatSprite_Release((CombatSprite*)var_r5);
        var_r4 += 1;
        var_r5 += 0x60;
    } while (var_r4 < 4);
    return 1;
}

void func_ov017_0212cc18(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141e90.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141e90.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141e90.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141e90.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

u16 func_ov017_0212cc60(void*) {
    s16   var_r0;
    void* temp_ip;

    temp_ip = data_ov003_020e71b8->unk3D898;
    if (data_ov017_021439ec == 0) {
        var_r0 = ((Boss03*)temp_ip)->unk36;
    } else {
        var_r0 = ((Boss03*)temp_ip)->unk36 + 0xC0;
    }
    return (u16)(FX_Atan2Idx(((Boss03*)data_ov017_021439f0)->unk36 - var_r0,
                             ((Boss03*)data_ov017_021439f0)->unk34 - ((Boss03*)temp_ip)->unk34) +
                 0x8000);
}

s32 func_ov017_0212cccc(void* arg1, void* arg2) {
    s32    var_r6;
    void** temp_r4;
    void** var_r7;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x188);
    ((Boss03*)temp_r4)->unk0   = arg2;
    var_r6                     = 0;
    var_r7                     = temp_r4 + 4;
    ((Boss03*)temp_r4)->unk184 = 0;
    do {
        func_ov003_02087fe0(0, var_r7, 2);
        var_r6 += 1;
        var_r7 += 0x60;
    } while (var_r6 < 4);
    return 1;
}

s32 func_ov017_0212cd2c(void* arg1) {
    s32   var_r5;
    void* temp_r4;
    void* var_r6;

    temp_r4 = ((Boss03*)arg1)->unk18;
    var_r5  = 0;
    var_r6  = temp_r4 + 4;
    do {
        CombatSprite_Update(var_r6);
        var_r5 += 1;
        var_r6 += 0x60;
    } while (var_r5 < 4);
    ((Boss03*)temp_r4)->unk184 = func_ov017_0212cc60(temp_r4);
    return 1;
}

s32 func_ov017_0212cd6c(void* arg1) {
    s32   temp_r2;
    s32   temp_r4;
    s32   temp_r7;
    s32   var_r5;
    s32   var_r6;
    s32   var_r8;
    u32   temp_r2_2;
    void* temp_ip;
    void* temp_r0;
    void* var_r9;

    temp_r0   = ((Boss03*)arg1)->unk18;
    temp_r2   = ((s32)((Boss03*)temp_r0)->unk184 >> 4) * 2;
    temp_ip   = data_ov003_020e71b8->unk3D898;
    temp_r2_2 = *(s16*)((u8*)&data_0205e4e0 + (temp_r2 * 2)) * 0xC;
    temp_r7   = *(&data_0205e4e0 + ((temp_r2 + 1) * 2)) * 0xC;
    var_r5    = ((Boss03*)temp_ip)->unk28;
    var_r6    = ((Boss03*)temp_ip)->unk2C;
    temp_r4   = temp_r2_2 + (temp_r2_2 >> 0x1F);
    var_r9    = temp_r0 + 4;
    var_r8    = 0;
    do {
        var_r5 += temp_r7;
        var_r6 += temp_r4 >> 1;
        func_ov003_02088064(var_r9, var_r5, var_r6);
        var_r8 += 1;
        var_r9 += 0x60;
    } while (var_r8 < 4);
    return 1;
}

s32 func_ov017_0212ce04(void* arg1) {
    s32    var_r4;
    void** var_r5;

    var_r4 = 0;
    var_r5 = ((Boss03*)arg1)->unk18 + 4;
    do {
        CombatSprite_Release((CombatSprite*)var_r5);
        var_r4 += 1;
        var_r5 += 0x60;
    } while (var_r4 < 4);
    return 1;
}

void func_ov017_0212ce34(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141eac.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141eac.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141eac.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141eac.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212ce7c(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk44 = arg1;
    ((Boss03*)arg0)->unk42 = 0;
    ((Boss03*)arg0)->unk40 = 0;
}

void func_ov017_0212ce90(void* arg0) {
    s16 temp_r1;

    if (((Boss03*)arg0)->unk40 == 0) {
        (*(Boss03**)arg0)->unk54 = -0x48000;
        ((Boss03*)arg0)->unk40   = 0x64;
    }
    temp_r1 = ((Boss03*)arg0)->unk40;
    if ((s32)temp_r1 > 0) {
        func_02026590((u8*)(*(void**)arg0) + 0x54, 0, (u16)temp_r1);
        ((Boss03*)arg0)->unk40 = (s16)(((Boss03*)arg0)->unk40 - 1);
    }
    if ((s32)((Boss03*)arg0)->unk40 > 0) {
        return;
    }
    func_ov017_0212ce7c(arg0, NULL);
}

void func_ov017_0212cf08(void* arg0) {
    s32   temp_r1;
    s32   temp_r2;
    u32   temp_r0;
    void* temp_lr;
    void* temp_r0_2;

    temp_r2 = ((Boss03*)arg0)->unk14;
    temp_r1 = (((Boss03*)arg0)->unk10 * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_lr = ((Boss03*)arg0)->unk0;
    temp_r0 = *(u32*)(temp_r1 + (temp_r2 * 0x3C));
    switch (temp_r0) {
        default:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            ((Boss03*)((temp_r2 * 0x2C) + temp_r1))->unk124 = 1;
            break;
    }
    temp_r0_2                   = temp_r1 + (temp_r2 * 8);
    ((Boss03*)temp_r0_2)->unkF0 = (s32)(((Boss03*)temp_lr)->unk28 - 0x80000);
    ((Boss03*)temp_r0_2)->unkF4 = (s32)(((Boss03*)temp_lr)->unk40 - 0x60000);
}

s32 func_ov017_0212cf88(void* arg1, u16* arg2) {
    void*  temp_ip;
    void*  temp_r0;
    void*  temp_r0_2;
    void*  temp_r0_3;
    void*  temp_r1;
    void*  temp_r1_2;
    void*  temp_r1_3;
    void*  temp_r3;
    void*  temp_r3_2;
    void*  temp_r3_3;
    void*  var_r0;
    void*  var_r1;
    void*  var_r1_2;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x48);
    func_ov017_0212fbc4();
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x4C)) = (s32)(*(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x4C)) & ~4);
    temp_ip                                               = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D818));
    ((Boss03*)temp_r4)->unk0                              = temp_ip;
    ((Boss03*)temp_ip)->unk2                              = (u16)(((Boss03*)temp_ip)->unk2 | 1);
    ((Boss03*)temp_r4)->unk3C                             = 0;
    temp_r0                  = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_02141ebc, 1, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r1 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r1  = temp_r1 + ((Boss03*)temp_r1)->unk8;
    }
    ((Boss03*)temp_r4)->unkC = BgResMgr_AllocChar32(g_BgResourceManagers[1], var_r1,
                                                    g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x20);
    DMA_Flush();
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r0_2                = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_02141ebc, 2, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0_2;
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_2 = ((Boss03*)temp_r0_2)->unk8 + 0x20;
        var_r1_2  = temp_r1_2 + ((Boss03*)temp_r1_2)->unk10;
    }
    ((Boss03*)temp_r4)->unk8 = PaletteMgr_AllocPalette(g_PaletteManagers[1], var_r1_2, 0, 0, 1);
    temp_r0_3                = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_3 = ((Boss03*)temp_r0_3)->unk8 + 0x20;
        var_r0    = temp_r1_3 + ((Boss03*)temp_r1_3)->unk8;
    }
    ((Boss03*)temp_r4)->unk38 = (void*)(var_r0 + 4);
    func_0200d1d8(temp_r4 + 0x10, 1, 1, 0, temp_r4 + 0x38, 1, 1);
    func_0200d858(temp_r4 + 0x10, 1, 1, 0);
    if (*arg2 == 0) {
        temp_r3                     = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3)->unk28   = (s32)((Boss03*)temp_r3)->unk34;
        temp_r3_2                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_2)->unk2C = (s32)((Boss03*)temp_r3_2)->unk38;
        temp_r3_3                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_3)->unk30 = (s32)((Boss03*)temp_r3_3)->unk3C;
        func_ov017_0212ce7c(temp_r4, func_ov017_0212ce90);
    } else {
        func_ov017_0212ce7c(temp_r4, NULL);
    }
    func_ov017_0212cf08(temp_r4);
    return 1;
}

s32 func_ov017_0212d17c(void* arg1) {
    void (*temp_r1)(void*);
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk44;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov017_0212fd18(((Boss03*)temp_r4)->unk0);
    func_ov017_0212cf08(temp_r4);
    return 1;
}

s32 func_ov017_0212d1d0(void) {
    return 1;
}

s32 func_ov017_0212d1d8(void* arg1) {
    void* temp_r2;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], ((Boss03*)temp_r4)->unkC);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], ((Boss03*)temp_r4)->unk8);
    func_0200d954(1, 1);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r2                  = ((Boss03*)temp_r4)->unk0;
    ((Boss03*)temp_r2)->unk2 = (u16)(((Boss03*)temp_r2)->unk2 & ~1);
    return 1;
}

void func_ov017_0212d234(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141ec4.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141ec4.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141ec4.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141ec4.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212d27c(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk40 = arg1;
    ((Boss03*)arg0)->unk3E = 0;
    ((Boss03*)arg0)->unk3C = 0;
}

void func_ov017_0212d290(void* arg0) {
    s32   temp_r1;
    s32   temp_r2;
    u32   temp_r0;
    void* temp_lr;
    void* temp_r0_2;

    temp_r2 = ((Boss03*)arg0)->unk14;
    temp_r1 = (((Boss03*)arg0)->unk10 * 0x220) + (s32)((u8*)(void*)((u8*)&g_DisplaySettings + 0x64));
    temp_lr = ((Boss03*)arg0)->unk0;
    temp_r0 = *(u32*)(temp_r1 + (temp_r2 * 0x3C));
    switch (temp_r0) {
        default:
            break;
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            ((Boss03*)((temp_r2 * 0x2C) + temp_r1))->unk124 = 1;
            break;
    }
    temp_r0_2                   = temp_r1 + (temp_r2 * 8);
    ((Boss03*)temp_r0_2)->unkF0 = (s32)(((Boss03*)temp_lr)->unk28 - 0x80000);
    ((Boss03*)temp_r0_2)->unkF4 = (s32)(((Boss03*)temp_lr)->unk40 - 0x60000);
}

void func_ov017_0212d310(void* arg0) {
    s16 temp_r1;

    if (((Boss03*)arg0)->unk3C == 0) {
        ((Boss03*)arg0)->unk3C = 0x64;
    }
    temp_r1 = ((Boss03*)arg0)->unk3C;
    if ((s32)temp_r1 > 0) {
        func_02026590((u8*)(*(void**)arg0) + 0x54, 0, (u16)temp_r1);
        ((Boss03*)arg0)->unk3C = (s16)(((Boss03*)arg0)->unk3C - 1);
    }
    if ((s32)((Boss03*)arg0)->unk3C > 0) {
        return;
    }
    func_ov017_0212d27c(arg0, NULL);
}

s32 func_ov017_0212d374(void* arg1, u16* arg2) {
    u32    temp_r2;
    u32    temp_r5;
    void*  temp_ip;
    void*  temp_ip_2;
    void*  temp_r0;
    void*  temp_r0_2;
    void*  temp_r0_3;
    void*  temp_r1;
    void*  temp_r1_2;
    void*  temp_r1_3;
    void*  temp_r3;
    void*  temp_r3_2;
    void*  temp_r3_3;
    void*  temp_r6;
    void*  var_r0;
    void*  var_r1;
    void*  var_r1_2;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x44);
    func_ov017_0212fc70();
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x1C)) = (s32)(*(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x1C)) & ~4);
    temp_ip                                               = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    ((Boss03*)temp_r4)->unk0                              = temp_ip;
    ((Boss03*)temp_ip)->unk2                              = (u16)(((Boss03*)temp_ip)->unk2 | 1);
    temp_r0                  = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_02141ee0, 1, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0;
    if (temp_r0 == NULL) {
        var_r1 = NULL;
    } else {
        temp_r1 = ((Boss03*)temp_r0)->unk8 + 0x20;
        var_r1  = temp_r1 + ((Boss03*)temp_r1)->unk8;
    }
    ((Boss03*)temp_r4)->unkC = BgResMgr_AllocChar32(g_BgResourceManagers[0], var_r1,
                                                    g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x20);
    DMA_Flush();
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r0_2                = DatMgr_LoadPackEntry(1, 0, 0, (BinIdentifier*)&data_ov017_02141ee0, 2, 0);
    ((Boss03*)temp_r4)->unk4 = temp_r0_2;
    if (temp_r0_2 == NULL) {
        var_r1_2 = NULL;
    } else {
        temp_r1_2 = ((Boss03*)temp_r0_2)->unk8 + 0x20;
        var_r1_2  = temp_r1_2 + ((Boss03*)temp_r1_2)->unk10;
    }
    ((Boss03*)temp_r4)->unk8 = PaletteMgr_AllocPalette(g_PaletteManagers[0], var_r1_2, 0, 0, 1);
    temp_r0_3                = ((Boss03*)temp_r4)->unk4;
    if (temp_r0_3 == NULL) {
        var_r0 = NULL;
    } else {
        temp_r1_3 = ((Boss03*)temp_r0_3)->unk8 + 0x20;
        var_r0    = temp_r1_3 + ((Boss03*)temp_r1_3)->unk8;
    }
    ((Boss03*)temp_r4)->unk38 = (void*)(var_r0 + 4);
    func_0200d1d8(temp_r4 + 0x10, 0, 1, 0, temp_r4 + 0x38, 1, 1);
    func_0200d858(temp_r4 + 0x10, 1, 1, 0);
    data_ov003_020e71b8->unk3D7C2 = 0;
    data_ov003_020e71b8->unk3D81A = 0;
    func_ov003_02083a2c(0, 0x20, 0x140, 0x1E0, 0, 0);
    func_ov003_02083a2c(1, 0x70, 0x1C8, 0x190, 0x200, 0);
    temp_r6                   = (void*)((u8*)(void*)((u8*)data_ov003_020e71b8 + 0x3D7C0));
    ((Boss03*)temp_r6)->unk18 = (s32)(0x100000 - ((Boss03*)temp_r6)->unk8);
    ((Boss03*)temp_r6)->unk1C = FX_Atan2Idx(0, 0x140);
    ((Boss03*)temp_r6)->unk54 = -0x30000;
    if ((arg2 != NULL) && (*arg2 != 0)) {
        func_ov017_0212d27c(temp_r4, NULL);
    } else {
        temp_r5                     = data_ov003_020e71b8->unk3D7CC;
        temp_ip_2                   = data_ov003_020e71b8->unk3D898;
        ((Boss03*)temp_ip_2)->unk28 = (s32)((s32)(temp_r5 + (temp_r5 >> 0x1F)) >> 1);
        temp_r2                     = data_ov003_020e71b8->unk3D7D0;
        ((Boss03*)temp_ip_2)->unk2C = (s32)((s32)(temp_r2 + (temp_r2 >> 0x1F)) >> 1);
        temp_r3                     = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3)->unk28   = (s32)((Boss03*)temp_r3)->unk34;
        temp_r3_2                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_2)->unk2C = (s32)((Boss03*)temp_r3_2)->unk38;
        temp_r3_3                   = ((Boss03*)temp_r4)->unk0;
        ((Boss03*)temp_r3_3)->unk30 = (s32)((Boss03*)temp_r3_3)->unk3C;
        func_ov017_0212d27c(temp_r4, func_ov017_0212d310);
    }
    func_ov017_0212d290(temp_r4);
    return 1;
}

s32 func_ov017_0212d630(void* arg1) {
    void (*temp_r1)(void*);
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (data_ov003_020e71b8->unk3D878 & 0x1000) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk40;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov017_0212fe1c(((Boss03*)temp_r4)->unk0);
    func_ov017_0212d290(temp_r4);
    return 1;
}

s32 func_ov017_0212d684(void) {
    return 1;
}

s32 func_ov017_0212d68c(void* arg1) {
    void* temp_r2;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], ((Boss03*)temp_r4)->unkC);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], ((Boss03*)temp_r4)->unk8);
    func_0200d954(0, 1);
    DatMgr_ReleaseData(((Boss03*)temp_r4)->unk4);
    temp_r2                  = ((Boss03*)temp_r4)->unk0;
    ((Boss03*)temp_r2)->unk2 = (u16)(((Boss03*)temp_r2)->unk2 & ~1);
    return 1;
}

void func_ov017_0212d6e8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141ee8.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141ee8.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141ee8.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141ee8.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212d730(void) {
    s16   temp_r1;
    s32   temp_r1_3;
    s32   var_r1;
    u16   temp_r3;
    u32   temp_r1_2;
    void* temp_r2;

    temp_r2 = (void*)((u8*)&g_DisplaySettings + 0x284);
    temp_r3 = *(u16*)0x04000006;
    if ((s32)temp_r3 < 0xC0) {
        temp_r1   = *(&data_0205e4e0 + (((s32)(u16)((data_ov017_021439f4.unk0 << 0xA) + (temp_r3 << 9)) >> 4) * 4));
        temp_r1_2 = (temp_r1 * data_ov017_021439f4.unk8) + 0x800;
        var_r1    = (temp_r1_2 >> 0xC) | ((MULT_HI(temp_r1, data_ov017_021439f4.unk8) + M2C_CARRY(temp_r1_2)) << 0x14);
        if (data_ov017_021439f4.unk0 & 1) {
            var_r1 *= -1;
        }
        *(vu32*)0x04001010 = (s32)((0x1FF & ((s32)(((Boss03*)temp_r2)->unkF0 + var_r1) >> 0xC)) |
                                   (0x01FF0000 & (((s32)((Boss03*)temp_r2)->unkF4 >> 0xC) << 0x10)));
        *(vu32*)0x04001014 = (s32)((0x1FF & ((s32)(((Boss03*)temp_r2)->unkF8 + var_r1) >> 0xC)) |
                                   (0x01FF0000 & (((s32)((Boss03*)temp_r2)->unkFC >> 0xC) << 0x10)));
        *(vu32*)0x04001018 = (s32)((0x1FF & ((s32)(((Boss03*)temp_r2)->unk100 + var_r1) >> 0xC)) |
                                   (0x01FF0000 & (((s32)((Boss03*)temp_r2)->unk104 >> 0xC) << 0x10)));
        *(vu32*)0x04000014 = (s32)((0x1FF & ((s32)(((Boss03*)temp_r2)->unkF8 + var_r1) >> 0xC)) |
                                   (0x01FF0000 & (((s32)((Boss03*)temp_r2)->unkFC >> 0xC) << 0x10)));
        *(vu32*)0x04000018 = (s32)((0x1FF & ((s32)(((Boss03*)temp_r2)->unk100 + var_r1) >> 0xC)) |
                                   (0x01FF0000 & (((s32)((Boss03*)temp_r2)->unk104 >> 0xC) << 0x10)));
    }
    if (temp_r3 != 0xBF) {
        return;
    }
    data_ov017_021439f4.unk0 = (u16)(data_ov017_021439f4.unk0 + 1);
    temp_r1_3                = data_ov017_021439f4.unk4 + 0x20;
    data_ov017_021439f4.unk4 = temp_r1_3;
    if (temp_r1_3 >= 0x1000) {
        data_ov017_021439f4.unk4 = 0x1000;
    }
    data_ov017_021439f4.unk8 = (s32)(data_ov017_021439f4.unk8 - data_ov017_021439f4.unk4);
}

void func_ov017_0212d8bc(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0212d8e0(void* arg0) {
    s32   temp_r7;
    s32   var_r8;
    void* var_r9;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c3acc(0, 0x2FE, ((Boss03*)data_ov003_020e71b8->unk3D898)->unk28);
        ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    }
    if (((Boss03*)arg0)->unk1C0 == 1) {
        var_r8 = 0;
        var_r9 = &data_02071cf0 + 0x20;
        do {
            if (((Boss03*)var_r9)->unk74 != 0xFFFF) {
                temp_r7 = (((Boss03*)data_ov003_020e71b8->unk3D898)->unk28 + (RNG_Next(0x80) << 0xC)) - 0x40000;
                BtlBoss02_Badge_CreateTask(
                    2, temp_r7, (((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C + (RNG_Next(0x60) << 0xC)) - 0x30000, 0, 0,
                    0U);
            }
            var_r8 += 1;
            var_r9 += 0xA;
        } while (var_r8 < 6);
        ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    }
    if ((s32)data_ov017_021439f4.unk8 >= 0) {
        return;
    }
    Interrupts_ResetHBlankCallback();
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x20000000);
    func_ov017_0212d8bc(arg0, func_ov017_0212da28);
}

void func_ov017_0212da28(void) {}

s32 func_ov017_0212da2c(void* arg1) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1D4);
    ((Boss03*)temp_r4)->unk1D0 = 1;
    func_ov017_0212d8bc(temp_r4, func_ov017_0212d8e0);
    data_ov003_020e71b8->unk3D875 = 2;
    data_ov003_020e71b8->unk3D878 = (s32)(data_ov003_020e71b8->unk3D878 | 0x100000);
    data_ov017_021439f4.unk8      = 0x80000;
    data_ov017_021439f4.unk4      = 0;
    data_ov017_021439f4.unk0      = 0;
    Interrupts_RegisterHBlankCallback(func_ov017_0212d730, 1);
    return 1;
}

s32 func_ov017_0212dabc(void* arg1) {
    void (*temp_r1)(void*);
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r1 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    return ((Boss03*)temp_r4)->unk1D0;
}

s32 func_ov017_0212dae0(void) {
    return 1;
}

s32 func_ov017_0212dae8(void) {
    return 1;
}

void func_ov017_0212daf0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141f20.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141f20.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141f20.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141f20.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212db38(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk78 = arg1;
    ((Boss03*)arg0)->unk7E = 0;
}

void func_ov017_0212db48(void* arg0) {
    if (((Boss03*)arg0)->unk7E == 0) {
        func_ov003_020c3acc(1, 0x2F3, ((Boss03*)arg0)->unk68);
    }
    ((Boss03*)arg0)->unk7E = (u16)(((Boss03*)arg0)->unk7E + 1);
    if ((u32)((Boss03*)arg0)->unk7E < 0x10U) {
        return;
    }
    func_ov017_0212db38(arg0, func_ov017_0212dbd8);
    if (data_ov017_02143980 != 0x28) {
        func_ov017_02130178(1, 0x4DD, 0x4DE, ((Boss03*)arg0)->unk68);
        return;
    }
    func_ov017_02130178(1, 0x550, 0x56C, ((Boss03*)arg0)->unk68);
}

void func_ov017_0212dbd8(void* arg0) {
    s32 temp_r0;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r5;
    s32 temp_r6;
    s32 var_r0;
    s32 var_r4;

    if (((Boss03*)arg0)->unk7E == 0) {
        func_ov003_020c3acc(0, 0x2F4, ((Boss03*)arg0)->unk68);
    }
    ((Boss03*)arg0)->unk7E = (u16)(((Boss03*)arg0)->unk7E + 1);
    temp_r0                = ((Boss03*)arg0)->unk74;
    if (((Boss03*)arg0)->unk7C == 1) {
        ((Boss03*)arg0)->unk74 = (s32)(temp_r0 + 0x400);
        var_r4                 = ((Boss03*)arg0)->unk68 + 0x40000;
    } else {
        ((Boss03*)arg0)->unk74 = (s32)(temp_r0 - 0x400);
        var_r4                 = ((Boss03*)arg0)->unk68 - 0x40000;
    }
    ((Boss03*)arg0)->unk68 = (s32)(((Boss03*)arg0)->unk68 + ((Boss03*)arg0)->unk74);
    temp_r1                = ((Boss03*)arg0)->unk70;
    temp_r6                = temp_r1 - 0x20000;
    temp_r5                = ((Boss03*)data_ov003_020e71b8->unk3D89C)->unk2C;
    if (((Boss03*)arg0)->unk80 != 0) {
        if (data_ov017_02143980 != 0x28) {
            if (temp_r1 < -0x40000) {
                var_r0 = func_ov003_0208a164(func_ov003_0208a114(0xEC), arg0 + 0x60, var_r4, temp_r5, temp_r6);
            } else {
                var_r0 = func_ov003_0208a164(func_ov003_0208a114(0xEB), arg0 + 0x60, var_r4, temp_r5, temp_r6);
            }
        } else if (temp_r1 < -0x40000) {
            var_r0 = func_ov003_0208a164(func_ov003_0208a114(0xEE), arg0 + 0x60, var_r4, temp_r5, temp_r6);
        } else {
            var_r0 = func_ov003_0208a164(func_ov003_0208a114(0xED), arg0 + 0x60, var_r4, temp_r5, temp_r6);
        }
        if (var_r0 != 0) {
            ((Boss03*)arg0)->unk80 = 0;
        }
    }
    temp_r1_2 = ((Boss03*)arg0)->unk68;
    if ((temp_r1_2 >= -0x40000) && (temp_r1_2 <= 0x140000)) {
        return;
    }
    ((Boss03*)arg0)->unk84 = 0;
}

s32 func_ov017_0212dd54(void* arg1, void* arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x88);
    ((Boss03*)temp_r4)->unk84 = 1;
    ((Boss03*)temp_r4)->unk80 = 1;
    ((Boss03*)temp_r4)->unk68 = (s32)((Boss03*)arg2)->unk0;
    ((Boss03*)temp_r4)->unk6C = (s32)((Boss03*)arg2)->unk4;
    ((Boss03*)temp_r4)->unk70 = (s32)((Boss03*)arg2)->unk8;
    ((Boss03*)temp_r4)->unk7C = (u16)((Boss03*)arg2)->unkC;
    ((Boss03*)temp_r4)->unk60 = 0;
    ((Boss03*)temp_r4)->unk64 = 0x64;
    ((Boss03*)temp_r4)->unk66 = 0x64;
    CombatSprite_LoadFromTable(1, (CombatSprite*)temp_r4, &data_ov017_02141510, &data_ov017_021415a8, 7, 0x17, 0xA0);
    func_ov017_0212db38(temp_r4, func_ov017_0212db48);
    if (((Boss03*)temp_r4)->unk7C == 1) {
        CombatSprite_SetFlip((CombatSprite*)temp_r4, 1);
    }
    return ((Boss03*)temp_r4)->unk84;
}

s32 func_ov017_0212de18(void* arg1) {
    void (*temp_r1)(void*);
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk78;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    CombatSprite_Update(temp_r4);
    return ((Boss03*)temp_r4)->unk84;
}

s32 func_ov017_0212de54(void* arg1) {
    s16   temp_r4;
    void* temp_r5;

    temp_r5 = ((Boss03*)arg1)->unk18;
    temp_r4 = func_ov003_020843b0(1, ((Boss03*)temp_r5)->unk68);
    CombatSprite_SetPosition(temp_r5, temp_r4, func_ov003_020843ec(1, ((Boss03*)temp_r5)->unk6C, ((Boss03*)temp_r5)->unk70));
    func_ov003_02082730(temp_r5, 0x7FFFFFFF - ((Boss03*)temp_r5)->unk6C);
    CombatSprite_Render(temp_r5);
    return 1;
}

s32 func_ov017_0212deb0(void* arg1) {
    CombatSprite_Release(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212dec4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141f3c.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141f3c.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141f3c.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141f3c.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212df0c(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk78 = arg1;
    ((Boss03*)arg0)->unk7E = 0;
}

void func_ov017_0212df1c(void* arg0) {
    if (((Boss03*)arg0)->unk7E == 0) {
        func_ov003_020c3acc(0, 0x2F3, ((Boss03*)arg0)->unk68);
    }
    ((Boss03*)arg0)->unk7E = (u16)(((Boss03*)arg0)->unk7E + 1);
    if ((u32)((Boss03*)arg0)->unk7E < 0x10U) {
        return;
    }
    func_ov017_0212df0c(arg0, func_ov017_0212df88);
    func_ov017_02130178(0, 0x4DD, 0x4DE, ((Boss03*)arg0)->unk68);
}

void func_ov017_0212df88(void* arg0) {
    s32 temp_r0;
    s32 temp_r1;
    s32 temp_r6;
    s32 temp_r7;
    s32 var_r5;

    if (((Boss03*)arg0)->unk7E == 0) {
        func_ov003_020c3acc(0, 0x2F4, ((Boss03*)arg0)->unk68);
    }
    ((Boss03*)arg0)->unk7E = (u16)(((Boss03*)arg0)->unk7E + 1);
    temp_r0                = ((Boss03*)arg0)->unk74;
    if (((Boss03*)arg0)->unk7C == 1) {
        ((Boss03*)arg0)->unk74 = (s32)(temp_r0 + 0x400);
        var_r5                 = ((Boss03*)arg0)->unk68 + 0x40000;
    } else {
        ((Boss03*)arg0)->unk74 = (s32)(temp_r0 - 0x400);
        var_r5                 = ((Boss03*)arg0)->unk68 - 0x40000;
    }
    ((Boss03*)arg0)->unk68 = (s32)(((Boss03*)arg0)->unk68 + ((Boss03*)arg0)->unk74);
    temp_r7                = ((Boss03*)arg0)->unk70 - 0x20000;
    temp_r6                = ((Boss03*)arg0)->unk6C;
    if ((((Boss03*)arg0)->unk84 != 0) &&
        (func_ov003_0208a164(func_ov003_0208a114(0xEF), arg0 + 0x60, var_r5, temp_r6, temp_r7) != 0))
    {
        ((Boss03*)arg0)->unk84 = 0;
    }
    temp_r1 = ((Boss03*)arg0)->unk68;
    if ((temp_r1 >= -0x40000) && (temp_r1 <= 0x240000)) {
        return;
    }
    ((Boss03*)arg0)->unk88 = 0;
}

s32 func_ov017_0212e064(void* arg1, void* arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x8C);
    ((Boss03*)temp_r4)->unk88 = 1;
    ((Boss03*)temp_r4)->unk84 = 1;
    ((Boss03*)temp_r4)->unk68 = (s32)((Boss03*)arg2)->unk0;
    ((Boss03*)temp_r4)->unk6C = (s32)((Boss03*)arg2)->unk4;
    ((Boss03*)temp_r4)->unk70 = (s32)((Boss03*)arg2)->unk8;
    ((Boss03*)temp_r4)->unk7C = (u16)((Boss03*)arg2)->unkC;
    ((Boss03*)temp_r4)->unk60 = 0;
    ((Boss03*)temp_r4)->unk64 = 0x64;
    ((Boss03*)temp_r4)->unk66 = 0x64;
    CombatSprite_LoadFromTable(0, (CombatSprite*)temp_r4, &data_ov017_02141510, &data_ov017_021415a8, 7, 0x17, 0xA0);
    func_ov017_0212df0c(temp_r4, func_ov017_0212df1c);
    if (((Boss03*)temp_r4)->unk7C == 1) {
        CombatSprite_SetFlip((CombatSprite*)temp_r4, 1);
    }
    return ((Boss03*)temp_r4)->unk88;
}

s32 func_ov017_0212e128(void* arg1) {
    void (*temp_r1)(void*);
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    temp_r1 = ((Boss03*)temp_r4)->unk78;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    CombatSprite_Update(temp_r4);
    return ((Boss03*)temp_r4)->unk88;
}

s32 func_ov017_0212e164(void* arg1) {
    s16   temp_r4;
    void* temp_r5;

    temp_r5 = ((Boss03*)arg1)->unk18;
    temp_r4 = func_ov003_020843b0(0, ((Boss03*)temp_r5)->unk68);
    CombatSprite_SetPosition(temp_r5, temp_r4, func_ov003_020843ec(0, ((Boss03*)temp_r5)->unk6C, ((Boss03*)temp_r5)->unk70));
    func_ov003_02082730(temp_r5, 0x7FFFFFFF - ((Boss03*)temp_r5)->unk6C);
    CombatSprite_Render(temp_r5);
    return 1;
}

s32 func_ov017_0212e1c0(void* arg1) {
    CombatSprite_Release(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212e1d4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141f58.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141f58.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141f58.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141f58.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212e21c(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0212e240(void* arg0) {
    s16 temp_r1_2;
    s16 temp_r1_4;
    s16 temp_r1_6;
    s32 temp_r0;
    s32 temp_r0_2;
    s32 temp_r0_3;
    s32 var_r0;
    u16 temp_r1;
    u16 temp_r2;
    u32 temp_r1_3;
    u32 temp_r1_5;
    u32 temp_r1_7;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_020c4b1c(arg0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1 = ((Boss03*)arg0)->unk1D6;
    if (temp_r1 != 0) {
        ((Boss03*)arg0)->unk1D6 = (u16)(temp_r1 - 1);
        temp_r2                 = ((Boss03*)arg0)->unk1D4;
        if (((Boss03*)arg0)->unkCA & 1) {
            temp_r1_2 = *(&data_0205e4e0 + (((s32)temp_r2 >> 4) * 4));
            temp_r0   = ((Boss03*)arg0)->unk1D0;
            temp_r1_3 = (temp_r1_2 * temp_r0) + 0x800;
            var_r0 =
                ((Boss03*)arg0)->unk28 + ((temp_r1_3 >> 0xC) | ((MULT_HI(temp_r1_2, temp_r0) + M2C_CARRY(temp_r1_3)) << 0x14));
        } else {
            temp_r1_4 = *(&data_0205e4e0 + (((s32)temp_r2 >> 4) * 4));
            temp_r0_2 = ((Boss03*)arg0)->unk1D0;
            temp_r1_5 = (temp_r1_4 * temp_r0_2) + 0x800;
            var_r0    = ((Boss03*)arg0)->unk28 -
                     ((temp_r1_5 >> 0xC) | ((MULT_HI(temp_r1_4, temp_r0_2) + M2C_CARRY(temp_r1_5)) << 0x14));
        }
        ((Boss03*)arg0)->unk28 = var_r0;
        temp_r0_3              = ((Boss03*)arg0)->unk1D0;
        temp_r1_6              = *(&data_0205e4e0 + (((((s32)((Boss03*)arg0)->unk1D4 >> 4) * 2) + 1) * 2));
        temp_r1_7              = (temp_r1_6 * temp_r0_3) + 0x800;
        ((Boss03*)arg0)->unk2C =
            (s32)(((Boss03*)arg0)->unk2C +
                  ((temp_r1_7 >> 0xC) | ((MULT_HI(temp_r1_6, temp_r0_3) + M2C_CARRY(temp_r1_7)) << 0x14)));
        ((Boss03*)arg0)->unk1D0 = (s32)(((Boss03*)arg0)->unk1D0 + 0x800);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
}

void func_ov017_0212e36c(void* arg0) {
    func_ov017_0212e21c(arg0, func_ov017_0212e240);
}

void func_ov017_0212e380(void* arg0) {
    if (((Boss03*)arg0)->unk1C0 == 0) {
        func_ov003_020c3acc(1, 0x2FD, ((Boss03*)arg0)->unk28);
        ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    }
    data_ov017_02143a00 = 0x20;
    if (func_ov003_020c6f40(arg0) == 0) {
        ((Boss03*)arg0)->unk1D8 = 0;
    }
}

s32 func_ov017_0212e3d8(void* arg1, s32 arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1DC);
    func_ov003_020c3efc(temp_r4, arg2);
    ((Boss03*)temp_r4)->unk1D8 = 1;
    ((Boss03*)temp_r4)->unk1D0 = 0x1000;
    ((Boss03*)temp_r4)->unk1D6 = 0xA;
    ((Boss03*)temp_r4)->unk38  = 0;
    ((Boss03*)temp_r4)->unk3C  = 0;
    func_ov003_020c4988(temp_r4);
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov017_0212e21c(temp_r4, func_ov017_0212e240);
    return 1;
}

s32 func_ov017_0212e458(void* arg1) {
    void (*temp_r1_2)(void*);
    u16    temp_r1;
    u32    temp_r0;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    temp_r0 = (u32)func_ov003_02082f2c(temp_r4);
    switch (temp_r0) {
        case 0:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
        case 11:
        case 12:
            break;
        default:
            break;
        case 1:
        case 2:
            func_ov017_0212e21c(temp_r4, func_ov017_0212e36c);
            break;
        case 3:
            temp_r1 = ((Boss03*)temp_r4)->unk18C;
            if (temp_r1 & 0x10) {
                ((Boss03*)temp_r4)->unk18C = (u16)(temp_r1 | 0x20);
            } else {
                func_ov017_0212e21c(temp_r4, func_ov017_0212e380);
            }
            break;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4628(temp_r4);
    return ((Boss03*)temp_r4)->unk1D8;
}

s32 func_ov017_0212e518(void* arg1) {
    void* temp_r0;

    temp_r0                   = ((Boss03*)arg1)->unk18;
    ((Boss03*)temp_r0)->unk54 = (s32)(((Boss03*)temp_r0)->unk54 | 0x80000000);
    func_ov003_020c4878(NULL);
    return 1;
}

s32 func_ov017_0212e538(void* arg1) {
    s32 temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c48fc(temp_r4);
    func_ov003_020c4a34(temp_r4);
    return 1;
}

void func_ov017_0212e558(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141fa0.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141fa0.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141fa0.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141fa0.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212e5a0(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0212e5c4(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0212e5a0(arg0, func_ov017_0212e6b8);
}

void func_ov017_0212e5ec(void* arg0) {
    s16   sp14;
    s32   sp10;
    s32   spC;
    void* sp8;
    s16   var_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 3, 1);
    }
    if (((Boss03*)arg0)->unk1C0 == 0x30) {
        sp8  = ((Boss03*)arg0)->unk28;
        spC  = ((Boss03*)arg0)->unk2C;
        sp10 = ((Boss03*)arg0)->unk30;
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        sp14 = var_r0;
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f30, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    func_ov017_0212e5a0(arg0, func_ov017_0212e6b8);
}

void func_ov017_0212e6b8(void* arg0) {
    s32 temp_r1;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_020c4b1c(arg0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    if (func_ov003_020c4348(arg0) != 0) {
        func_ov017_0212e5a0(arg0, func_ov017_0212e5ec);
        return;
    }
    func_ov017_0212e5a0(arg0, func_ov017_0212e740);
}

void func_ov017_0212e740(void* arg0) {
    if (func_ov003_020c5e98() != 0) {
        return;
    }
    func_ov017_0212e5a0(arg0, func_ov017_0212e6b8);
    ((Boss03*)arg0)->unk54 = (s32)(((Boss03*)arg0)->unk54 | 0x30);
}

s32 func_ov017_0212e774(void* arg1, s32 arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1DC);
    func_ov003_020c3efc(temp_r4, arg2);
    ((Boss03*)temp_r4)->unk1D8 = 1;
    ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x30);
    func_ov003_020c4988(temp_r4);
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov017_0212e5a0(temp_r4, func_ov017_0212e5c4);
    return 1;
}

s32 func_ov017_0212e7e0(void* arg1) {
    void (*temp_r1)(void*);
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    func_ov003_02082f2c(temp_r4);
    temp_r1 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_020c4628(temp_r4);
    return ((Boss03*)temp_r4)->unk1D8;
}

s32 func_ov017_0212e824(void* arg1) {
    void* temp_r0;

    temp_r0                   = ((Boss03*)arg1)->unk18;
    ((Boss03*)temp_r0)->unk54 = (s32)(((Boss03*)temp_r0)->unk54 | 0x80000000);
    func_ov003_020c4878(NULL);
    return 1;
}

s32 func_ov017_0212e844(void* arg1) {
    s32 temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c48fc(temp_r4);
    func_ov003_020c4a34(temp_r4);
    return 1;
}

void func_ov017_0212e864(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141fbc.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141fbc.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141fbc.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141fbc.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212e8ac(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

void func_ov017_0212e8d0(void* arg0) {
    s16 temp_r2_2;
    s16 temp_r3;
    s32 temp_r1;
    s32 temp_r2;
    u32 temp_r2_3;
    u32 temp_r3_2;

    temp_r2   = ((Boss03*)arg0)->unk1D0;
    temp_r3   = *(&data_0205e4e0 + (((((s32)((Boss03*)arg0)->unk1D4 >> 4) * 2) + 1) * 2));
    temp_r3_2 = (temp_r3 * temp_r2) + 0x800;
    ((Boss03*)arg0)->unk28 =
        (s32)(((Boss03*)arg0)->unk28 + ((temp_r3_2 >> 0xC) | ((MULT_HI(temp_r3, temp_r2) + M2C_CARRY(temp_r3_2)) << 0x14)));
    temp_r1   = ((Boss03*)arg0)->unk1D0;
    temp_r2_2 = *(&data_0205e4e0 + (((s32)((Boss03*)arg0)->unk1D4 >> 4) * 4));
    temp_r2_3 = (temp_r2_2 * temp_r1) + 0x800;
    ((Boss03*)arg0)->unk2C =
        (s32)(((Boss03*)arg0)->unk2C + ((temp_r2_3 >> 0xC) | ((MULT_HI(temp_r2_2, temp_r1) + M2C_CARRY(temp_r2_3)) << 0x14)));
}

void func_ov017_0212e954(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0212e8ac(arg0, func_ov017_0212e97c);
}

void func_ov017_0212e97c(void* arg0) {
    s32 temp_r1;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 < (s32)((Boss03*)arg0)->unk1C4) {
        return;
    }
    if (func_ov003_020c4348(arg0) != 0) {
        func_ov017_0212e8ac(arg0, func_ov017_0212ebc0);
        return;
    }
    if (RNG_Next(0x64) < 0x1EU) {
        func_ov017_0212e8ac(arg0, func_ov017_0212eaec);
        return;
    }
    func_ov017_0212e8ac(arg0, func_ov017_0212ea24);
}

void func_ov017_0212ea24(void* arg0) {
    s32 var_r0;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 1, 0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1D0 = 0x2000;
        ((Boss03*)arg0)->unk1D4 = (s16)RNG_Next(0x10000);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    func_ov017_0212e8d0(arg0);
    if ((s32)((Boss03*)arg0)->unk1C0 >= (s32)((Boss03*)arg0)->unk1C4) {
        func_ov017_0212e8ac(arg0, func_ov017_0212e97c);
        return;
    }
    var_r0 = ((s32)((Boss03*)arg0)->unk2C >> 0xC) - ((s32)((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C >> 0xC);
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    if (var_r0 >= 0x10) {
        return;
    }
    func_ov017_0212e8ac(arg0, func_ov017_0212ebc0);
}

void func_ov017_0212eaec(void* arg0) {
    s32 temp_r0;
    s32 var_r0;

    func_ov003_020c4b5c(arg0);
    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 1, 0);
        temp_r0                 = func_ov003_020c42ec(arg0);
        ((Boss03*)arg0)->unk1C4 = (s32)((s32)(temp_r0 + ((u32)(temp_r0 >> 1) >> 0x1E)) >> 2);
        ((Boss03*)arg0)->unk1D0 = 0x8000;
        ((Boss03*)arg0)->unk1D4 = (s16)RNG_Next(0x10000);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    func_ov017_0212e8d0(arg0);
    if ((s32)((Boss03*)arg0)->unk1C0 >= (s32)((Boss03*)arg0)->unk1C4) {
        func_ov017_0212e8ac(arg0, func_ov017_0212e97c);
        return;
    }
    var_r0 = ((s32)((Boss03*)arg0)->unk2C >> 0xC) - ((s32)((Boss03*)data_ov003_020e71b8->unk3D898)->unk2C >> 0xC);
    if (var_r0 < 0) {
        var_r0 = 0 - var_r0;
    }
    if (var_r0 >= 0x10) {
        return;
    }
    func_ov017_0212e8ac(arg0, func_ov017_0212ebc0);
}

void func_ov017_0212ebc0(void* arg0) {
    s16   sp14;
    s32   sp10;
    s32   spC;
    void* sp8;
    s16   var_r0;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 3, 1);
    }
    if (((Boss03*)arg0)->unk1C0 == 0x30) {
        sp8  = ((Boss03*)arg0)->unk28;
        spC  = ((Boss03*)arg0)->unk2C;
        sp10 = ((Boss03*)arg0)->unk30;
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r0 = 1;
        } else {
            var_r0 = 0;
        }
        sp14 = var_r0;
        EasyTask_CreateTask((TaskPool*)data_ov003_020e71b8, &data_ov017_02141f4c, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    func_ov017_0212e8ac(arg0, func_ov017_0212e97c);
}

void func_ov017_0212ec84(void* arg0) {
    if (func_ov003_020c65cc(arg0, 3) != 0) {
        return;
    }
    func_ov017_0212e8ac(arg0, func_ov017_0212e97c);
}

s32 func_ov017_0212ecb0(void* arg1, s32 arg2) {
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1E0);
    func_ov003_020c3efc(temp_r4, arg2);
    ((Boss03*)temp_r4)->unk1D8 = 1;
    ((Boss03*)temp_r4)->unk1DC = 1;
    ((Boss03*)temp_r4)->unk54  = (s32)(((Boss03*)temp_r4)->unk54 | 0x30);
    func_ov003_020c4520(temp_r4);
    func_ov003_020c4b5c(temp_r4);
    func_ov017_0212e8ac(temp_r4, func_ov017_0212e954);
    return 1;
}

s32 func_ov017_0212ed18(void* arg1) {
    void (*temp_r1)(void*);
    s32    temp_r0;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if (func_ov003_020c3c28() != 0) {
        return 0;
    }
    temp_r0 = func_ov003_02082f2c(temp_r4);
    if (temp_r0 <= 0xB) {
        if ((temp_r0 < 0xB) && (temp_r0 <= 3) && (temp_r0 >= 1)) {
            if ((temp_r0 != 1) && (temp_r0 != 2)) {

            } else {
                func_ov003_02084694(temp_r4 + 0x144, 0);
                func_ov017_0212e8ac(temp_r4, func_ov017_0212ec84);
            }
        }
    }
    temp_r1 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    func_ov003_020c4668(temp_r4);
    return ((Boss03*)temp_r4)->unk1DC;
}

s32 func_ov017_0212edac(void* arg1) {
    void* temp_r0;

    temp_r0                   = ((Boss03*)arg1)->unk18;
    ((Boss03*)temp_r0)->unk54 = (s32)(((Boss03*)temp_r0)->unk54 | 0x80000000);
    func_ov003_020c48b0();
    return 1;
}

s32 func_ov017_0212edcc(void* arg1) {
    func_ov003_020c492c(((Boss03*)arg1)->unk18);
    return 1;
}

void func_ov017_0212ede0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02141fd8.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02141fd8.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02141fd8.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02141fd8.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212ee28(void* arg0, void (*arg1)(void*)) {
    func_ov003_020c427c();
    ((Boss03*)arg0)->unk1CC = arg1;
    ((Boss03*)arg0)->unk1C8 = 0;
    ((Boss03*)arg0)->unk1C0 = 0;
}

u16 func_ov017_0212ee4c(void* arg0) {
    do {
    } while (((Boss03*)arg0)->unk1D0 == RNG_Next(4));
}

void func_ov017_0212ee70(void* arg0) {
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r1_3;

    temp_r1 = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk1D0 * 0xC));
    ((Boss03*)arg0)->unk28  = temp_r1;
    ((Boss03*)arg0)->unk1AC = temp_r1;
    temp_r1_2 = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk1D0 * 0xC));
    ((Boss03*)arg0)->unk2C  = temp_r1_2;
    ((Boss03*)arg0)->unk1B0 = temp_r1_2;
    temp_r1_3               = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk1D0 * 0xC));
    ((Boss03*)arg0)->unk30  = temp_r1_3;
    ((Boss03*)arg0)->unk1B4 = temp_r1_3;
}

void func_ov017_0212eefc(void* arg0) {
    s32   sp18;
    s32   sp14;
    s32   sp10;
    s16   spC;
    void* sp8;
    s32   temp_r0;
    s32   var_r4;

    sp8    = (void*)0x29;
    spC    = 1;
    var_r4 = 0;
loop_1:
    if (*(s32*)((char*)&data_ov017_02143a04 + (var_r4 * 4)) == -1) {
        temp_r0 = ((Boss03*)arg0)->unk28;
        if (((Boss03*)arg0)->unkCA & 1) {
            sp10 = temp_r0 - 0x8000;
        } else {
            sp10 = temp_r0 + 0x8000;
        }
        sp14 = ((Boss03*)arg0)->unk2C;
        sp18 = ((Boss03*)arg0)->unk30;
        *(s32*)((char*)&data_ov017_02143a04 + (var_r4 * 4)) =
            EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f94, 0, 0, 0, &sp8);
        ((Boss03*)arg0)->unk1D2 = (u16)(((Boss03*)arg0)->unk1D2 + 1);

        u8* temp = EasyTask_GetTaskData(&data_ov003_020e71b8->taskPool, *(s32*)((char*)&data_ov017_02143a04 + (var_r4 * 4)));
        *(u16*)(temp + 0x1D4) = (u16) * (&data_ov017_02141f68 + (var_r4 * 2));
        return;
    }
    var_r4 += 1;
    if (var_r4 >= 2) {
        return;
    }
    goto loop_1;
}

void func_ov017_0212eff8(void* arg0) {
    void* var_r7;
    s32   var_r6;

    var_r7 = &data_ov017_02143a04;
    var_r6 = 0;
    do {
        if ((*(s32*)((char*)&data_ov017_02143a04 + (var_r6 * 4)) != -1) &&
            (EasyTask_ValidateTaskId(&data_ov003_020e71b8->taskPool, var_r7) == 0))
        {
            ((Boss03*)arg0)->unk1D2 = (u16)(((Boss03*)arg0)->unk1D2 - 1);
        }
        var_r6 += 1;
        var_r7 += 4;
    } while (var_r6 < 2);
}

void func_ov017_0212f064(void* arg0) {
    if (func_ov003_020c5bfc(arg0) != 0) {
        return;
    }
    func_ov017_0212ee28(arg0, func_ov017_0212f18c);
}

void func_ov017_0212f08c(void* arg0) {
    s32 temp_r0_2;
    u32 temp_r0;

    temp_r0 = ((Boss03*)arg0)->unk1C8;
    switch (temp_r0) {
        default:
            return;
        case 0:
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 8, 1);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 1:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 9, 0);
            func_ov017_02130178(1, 0x551, 0x56C, ((Boss03*)arg0)->unk28);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 2:
            temp_r0_2               = ((Boss03*)arg0)->unk1C0 + 1;
            ((Boss03*)arg0)->unk1C0 = temp_r0_2;
            if (temp_r0_2 < 0x1E) {
                return;
            }
            func_ov003_020c3acc(1, 0x2F5, ((Boss03*)arg0)->unk28);
            func_ov017_0212eefc(arg0);
            CombatSprite_SetAnimFromTable(arg0 + 0x84, 0xA, 1);
            ((Boss03*)arg0)->unk1C8 = (u32)(((Boss03*)arg0)->unk1C8 + 1);
            return;
        case 3:
            if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
                return;
            }
            func_ov017_0212ee28(arg0, func_ov017_0212f18c);
            return;
    }
}

void func_ov017_0212f18c(void* arg0) {
    s32 temp_r1;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 0, 0);
        func_ov003_020c4b1c(arg0);
        ((Boss03*)arg0)->unk1C4 = func_ov003_020c42ec(arg0);
    }
    temp_r1                 = ((Boss03*)arg0)->unk1C0 + 1;
    ((Boss03*)arg0)->unk1C0 = temp_r1;
    if (temp_r1 >= (s32)((Boss03*)arg0)->unk1C4) {
        if (func_ov003_020c4348(arg0) != 0) {
            func_ov017_0212ee28(arg0, func_ov017_0212f280);
        } else if (data_ov017_02143a00 == 0) {
            if ((u32)((Boss03*)arg0)->unk1D2 < 2U) {
                func_ov017_0212ee28(arg0, func_ov017_0212f08c);
            }
        } else {
            func_ov017_0212ee28(arg0, func_ov017_0212f18c);
        }
    }
    if (data_ov017_02143a00 != 0) {
        data_ov017_02143a00 -= 1;
    }
    if (data_ov003_020e71b8->unk3D8A0 == arg0) {
        data_ov003_020e71b8->unk3D8A0 = 0;
    }
}

void func_ov017_0212f280(void* arg0) {
    s16   sp14;
    s32   sp10;
    s32   spC;
    void* sp8;
    s16   var_r1;

    if (((Boss03*)arg0)->unk1C0 == 0) {
        CombatSprite_SetAnimFromTable(arg0 + 0x84, 6, 1);
    }
    if (((Boss03*)arg0)->unk1C0 == 0x32) {
        sp8  = data_ov003_020e71b8->unk3D838 + *(s32*)((u8*)&data_ov017_02141538 + (((Boss03*)arg0)->unk1D0 * 0xC));
        spC  = data_ov003_020e71b8->unk3D83C + *(s32*)((u8*)&data_ov017_0214153c + (((Boss03*)arg0)->unk1D0 * 0xC));
        sp10 = *(s32*)((u8*)&data_ov017_02141540 + (((Boss03*)arg0)->unk1D0 * 0xC));
        if (((Boss03*)arg0)->unkCA & 1) {
            var_r1 = 1;
        } else {
            var_r1 = 0;
        }
        sp14 = var_r1;
        EasyTask_CreateTask(&data_ov003_020e71b8->taskPool, &data_ov017_02141f30, 0, 0, 0, &sp8);
    }
    ((Boss03*)arg0)->unk1C0 = (s32)(((Boss03*)arg0)->unk1C0 + 1);
    if (SpriteMgr_IsAnimationFinished((Sprite*)((u8*)arg0 + 0x84)) == 0) {
        return;
    }
    ((Boss03*)arg0)->unk1D0 = (u16)func_ov017_0212ee4c(arg0);
    func_ov017_0212ee70(arg0);
    func_ov017_0212ee28(arg0, func_ov017_0212f18c);
}

void func_ov017_0212f3ac(void* arg0) {
    if (func_ov003_020c703c() == 0) {
        ((Boss03*)arg0)->unk1D8 = 0;
    }
}

void func_ov017_0212f3c8(void* arg0) {
    if (func_ov003_020c72b4(arg0, 0, 3) != 0) {
        return;
    }
    func_ov017_0212ee28(arg0, func_ov017_0212f18c);
}

s32 func_ov017_0212f3f8(void* arg1, s32 arg2) {
    s32    var_r2;
    void*  temp_r2;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x1DC);
    func_ov003_020c3efc(temp_r4, arg2);
    data_ov017_02143980        = (u16)((Boss03*)temp_r4)->unk7C;
    ((Boss03*)temp_r4)->unk1D8 = 1;
    ((Boss03*)temp_r4)->unk1D2 = 0;
    data_ov017_02143a00        = 0;
    ((Boss03*)temp_r4)->unk1D0 = 0;
    func_ov017_0212ee70(temp_r4);
    var_r2                    = 0;
    ((Boss03*)temp_r4)->unk38 = 0;
    ((Boss03*)temp_r4)->unk3C = 0;
    do {
        *(s32*)((char*)&data_ov017_02143a04 + (var_r2 * 4)) = -1;
        var_r2 += 1;
    } while (var_r2 < 2);
    temp_r2                   = data_ov003_020e71b8->unk3D898;
    ((Boss03*)temp_r2)->unk54 = (s32)(((Boss03*)temp_r2)->unk54 | 0x40000000);
    func_ov003_020c4988(temp_r4);
    func_ov003_020c44ac(temp_r4);
    func_ov003_020c4b1c(temp_r4);
    func_ov003_020c3e88(temp_r4, 0x2F);
    func_ov017_0212ee28(temp_r4, func_ov017_0212f064);
    ((Boss03*)temp_r4)->unk54 = (s32)(((Boss03*)temp_r4)->unk54 | 0x08000000);
    func_ov017_02130148();
    return 1;
}

s32 func_ov017_0212f4f0(void* arg1) {
    void (*temp_r1_2)(void*);
    s32    temp_r0;
    u16    temp_r1;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov017_0212eff8(temp_r4);
    temp_r0                    = func_ov003_02082f2c(temp_r4);
    ((Boss03*)temp_r4)->unk1D4 = temp_r0;
    switch (temp_r0) {
        default:
            break;
        case 1:
            func_ov017_02130178(1, 0x552, 0x56C, ((Boss03*)temp_r4)->unk28);
            break;
        case 2:
            func_ov017_02130178(1, 0x553, 0x56C, ((Boss03*)temp_r4)->unk28);
            break;
        case 3:
            temp_r1 = ((Boss03*)temp_r4)->unk18C;
            if (temp_r1 & 0x10) {
                ((Boss03*)temp_r4)->unk18C = (u16)(temp_r1 | 0x20);
            } else {
                func_ov017_0212ee28(temp_r4, func_ov017_0212f3ac);
            }
            break;
        case 6:
            func_ov017_0212ee28(temp_r4, func_ov017_0212f3c8);
            break;
    }
    temp_r1_2 = ((Boss03*)temp_r4)->unk1CC;
    if (temp_r1_2 != NULL) {
        temp_r1_2(temp_r4);
    }
    func_ov003_020c4628(temp_r4);
    func_ov017_0213015c();
    return ((Boss03*)temp_r4)->unk1D8;
}

s32 func_ov017_0212f5e8(void* arg1) {
    func_ov003_020c4878(((Boss03*)arg1)->unk18);
    return 1;
}

s32 func_ov017_0212f5fc(void* arg1) {
    s32 temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    func_ov003_020c48fc(temp_r4);
    func_ov003_020c4a34(temp_r4);
    return 1;
}

void func_ov017_0212f61c(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02142004.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02142004.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02142004.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02142004.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[arg3])(arg0, arg1, arg2);
}

void func_ov017_0212f664(void* arg0, void (*arg1)(void*)) {
    ((Boss03*)arg0)->unk84 = arg1;
    ((Boss03*)arg0)->unk88 = 0;
}

void func_ov017_0212f674(void* arg0) {
    s16 temp_r0;
    s16 temp_r1;
    s32 temp_ip;
    s32 temp_r0_2;
    s32 temp_r1_2;
    s32 temp_r3;
    s32 var_r4;
    s32 var_r5;
    u32 temp_r1_3;
    u32 temp_r3_2;

    var_r4    = ((Boss03*)data_ov017_02143984)->unk28 - ((Boss03*)arg0)->unk6C;
    var_r5    = ((Boss03*)data_ov017_02143984)->unk2C - ((Boss03*)arg0)->unk70;
    temp_ip   = (FX_Atan2Idx(var_r5, var_r4) >> 4) * 2;
    temp_r1   = *(&data_0205e4e0 + ((temp_ip + 1) * 2));
    temp_r3   = ((Boss03*)arg0)->unk80;
    temp_r0   = *(s16*)((u8*)&data_0205e4e0 + (temp_ip * 2));
    temp_r3_2 = (temp_r3 * temp_r1) + 0x800;
    ((Boss03*)arg0)->unk78 =
        (s32)(((Boss03*)arg0)->unk78 + ((temp_r3_2 >> 0xC) | ((MULT_HI(temp_r3, temp_r1) + M2C_CARRY(temp_r3_2)) << 0x14)));
    temp_r1_2 = ((Boss03*)arg0)->unk80;
    temp_r1_3 = (temp_r1_2 * temp_r0) + 0x800;
    ((Boss03*)arg0)->unk7C =
        (s32)(((Boss03*)arg0)->unk7C + ((temp_r1_3 >> 0xC) | ((MULT_HI(temp_r1_2, temp_r0) + M2C_CARRY(temp_r1_3)) << 0x14)));
    ((Boss03*)arg0)->unk6C = (s32)(((Boss03*)arg0)->unk6C + ((Boss03*)arg0)->unk78);
    ((Boss03*)arg0)->unk70 = (s32)(((Boss03*)arg0)->unk70 + ((Boss03*)arg0)->unk7C);
    temp_r0_2              = ((Boss03*)arg0)->unk80 + 8;
    ((Boss03*)arg0)->unk80 = temp_r0_2;
    if (temp_r0_2 > 0x1000) {
        ((Boss03*)arg0)->unk80 = 0x1000;
    }
    if (var_r4 < 0) {
        var_r4 = 0 - var_r4;
    }
    ((Boss03*)arg0)->unk88 = (u16)(((Boss03*)arg0)->unk88 + 1);
    if (var_r4 >= 0xC000) {
        return;
    }
    if (var_r5 < 0) {
        var_r5 = 0 - var_r5;
    }
    if (var_r5 < 0xC000) {
        ((Boss03*)arg0)->unk90 = 0;
    }
}

void func_ov017_0212f784(void* arg0) {
    if (((Boss03*)arg0)->unk88 == 0) {
        ((Boss03*)arg0)->unk8A = (u16)(RNG_Next(0x3C) + 0x78);
    }
    if (((Boss03*)arg0)->unk8C != 0) {
        if (data_ov017_021439ec != ((Boss03*)arg0)->unk66) {
            CombatSprite_Release(arg0);
            CombatSprite_LoadDirect(data_ov017_021439ec, (CombatSprite*)arg0, &data_ov017_02141530, 5, 7, 6, 8, 0);
            ((Boss03*)arg0)->unk66 = (u16)data_ov017_021439ec;
        }
        ((Boss03*)arg0)->unk6C = (s32)((Boss03*)data_ov017_021439f0)->unk28;
        ((Boss03*)arg0)->unk70 = (s32)((Boss03*)data_ov017_021439f0)->unk2C;
        ((Boss03*)arg0)->unk74 = -0x28000;
    }
    ((Boss03*)arg0)->unk88 = (u16)(((Boss03*)arg0)->unk88 + 1);
    if ((u32)((Boss03*)arg0)->unk88 <= (u32)((Boss03*)arg0)->unk8A) {
        return;
    }
    ((Boss03*)arg0)->unk88 = 0U;
    if (((Boss03*)arg0)->unk8C != 0) {
        ((Boss03*)arg0)->unk8C = 0;
        return;
    }
    ((Boss03*)arg0)->unk8C = 1;
}

s32 func_ov017_0212f890(void* arg1, void* arg2) {
    s16    temp_r1;
    s16    temp_r1_2;
    s32    temp_r2;
    s32    temp_r2_3;
    u16    temp_r0;
    u32    temp_r2_2;
    u32    temp_r2_4;
    void** temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    MI_CpuSet(temp_r4, 0, 0x94);
    ((Boss03*)temp_r4)->unk90 = 1;
    ((Boss03*)temp_r4)->unk64 = (u16)((Boss03*)arg2)->unk0;
    ((Boss03*)temp_r4)->unk6C = (s32)((Boss03*)arg2)->unk4;
    ((Boss03*)temp_r4)->unk70 = (s32)((Boss03*)arg2)->unk8;
    ((Boss03*)temp_r4)->unk74 = (s32)((Boss03*)arg2)->unkC;
    temp_r2                   = ((Boss03*)arg2)->unk10;
    temp_r1                   = *(&data_0205e4e0 + (((((s32)((Boss03*)arg2)->unk2 >> 4) * 2) + 1) * 2));
    temp_r2_2                 = (temp_r2 * temp_r1) + 0x800;
    ((Boss03*)temp_r4)->unk78 = (s32)((temp_r2_2 >> 0xC) | ((MULT_HI(temp_r2, temp_r1) + M2C_CARRY(temp_r2_2)) << 0x14));
    temp_r2_3                 = ((Boss03*)arg2)->unk10;
    temp_r1_2                 = *(&data_0205e4e0 + (((s32)((Boss03*)arg2)->unk2 >> 4) * 4));
    temp_r2_4                 = (temp_r2_3 * temp_r1_2) + 0x800;
    ((Boss03*)temp_r4)->unk7C = (s32)((temp_r2_4 >> 0xC) | ((MULT_HI(temp_r2_3, temp_r1_2) + M2C_CARRY(temp_r2_4)) << 0x14));
    ((Boss03*)temp_r4)->unk80 = 0;
    if (((Boss03*)temp_r4)->unk64 != 1) {
        CombatSprite_LoadDirect(0, (CombatSprite*)temp_r4, &data_ov017_02141530, 3, 2, 4, 0, 0);
    } else {
        CombatSprite_LoadDirect(0, (CombatSprite*)temp_r4, &data_ov017_02141530, 5, 7, 6, 8, 0);
    }
    temp_r0 = ((Boss03*)temp_r4)->unk64;
    switch (temp_r0) { /* irregular */
        case 0:
            ((Boss03*)temp_r4)->unk8C = 1;
            func_ov017_0212f664(temp_r4, func_ov017_0212f674);
            break;
        case 1:
            ((Boss03*)temp_r4)->unk8C = 0;
            ((Boss03*)temp_r4)->unk66 = 0;
            func_ov017_0212f664(temp_r4, func_ov017_0212f784);
            break;
        case 2:
            ((Boss03*)temp_r4)->unk8C = 1;
            func_ov017_0212f664(temp_r4, NULL);
            break;
    }
    return ((Boss03*)temp_r4)->unk90;
}

s32 func_ov017_0212fa2c(void* arg1) {
    void (*temp_r1)(void*);
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    temp_r1 = ((Boss03*)temp_r4)->unk84;
    if (temp_r1 != NULL) {
        temp_r1(temp_r4);
    }
    CombatSprite_Update(temp_r4);
    return ((Boss03*)temp_r4)->unk90;
}

s32 func_ov017_0212fa58(void* arg1) {
    s16   var_r0;
    s16   var_r5;
    void* temp_r4;

    temp_r4 = ((Boss03*)arg1)->unk18;
    if ((s32)data_ov003_020e71b8->unk3D8D8 <= 0) {
        return 1;
    }
    if (((Boss03*)temp_r4)->unk8C != 0) {
        if (((Boss03*)temp_r4)->unk66 == 0) {
            var_r5 = func_ov003_020843b0(0, ((Boss03*)temp_r4)->unk70);
            var_r0 = func_ov003_020843ec(0, ((Boss03*)temp_r4)->unk70, ((Boss03*)temp_r4)->unk74);
        } else {
            var_r5 = func_ov003_020843b0(1, ((Boss03*)temp_r4)->unk70);
            var_r0 = func_ov003_020843ec(1, ((Boss03*)temp_r4)->unk70, ((Boss03*)temp_r4)->unk74);
        }
        CombatSprite_SetPosition(temp_r4, var_r5, var_r0);
        func_ov003_02082730(temp_r4, 0x7FFFFFFF - ((Boss03*)temp_r4)->unk70);
        CombatSprite_Render(temp_r4);
    }
    return 1;
}

s32 func_ov017_0212fb0c(void* arg1) {
    CombatSprite_Release(((Boss03*)arg1)->unk18);
    return 1;
}

s32 BtlBoss02_Badge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    SubroutineArgs subroutine_arg0;
    subroutine_arg0.unk0 = data_ov017_02142020.unk0;
    subroutine_arg0.unk4 = (s32)data_ov017_02142020.unk4;
    subroutine_arg0.unk8 = (s32)data_ov017_02142020.unk8;
    subroutine_arg0.unkC = (s32)data_ov017_02142020.unkC;
    ((void (*)(s32, s32, s32))((s32*)&subroutine_arg0)[stage])(pool, task, args);
}

const TaskHandle Tsk_BtlBoss02_Badge = {"Tsk_BtlBoss02_Badge", BtlBoss02_Badge_RunTask, 0x94};

void BtlBoss02_Badge_CreateTask(s16 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, u16 arg5) {
    s32 sp18;
    s32 sp14;
    s32 sp10;
    s32 spC;
    u16 spA;
    s16 sp8;

    spC  = arg1;
    sp10 = arg2;
    sp8  = arg0;
    sp14 = arg3;
    sp18 = arg4;
    spA  = arg5;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_00000, &Tsk_BtlBoss02_Badge, 0, 0, 0, &sp8);
}

void func_ov017_0212fbc4(void) {
    void* temp_r0;
    void* temp_r2;

    temp_r0                                                = (void*)((u8*)&g_DisplaySettings + 0x2C0);
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x2C0)) = 0;
    ((Boss03*)temp_r0)->unk4                               = 1;
    ((Boss03*)temp_r0)->unk1C                              = 0;
    ((Boss03*)temp_r0)->unk20                              = 0;
    ((Boss03*)temp_r0)->unk28                              = 1;
    ((Boss03*)temp_r0)->unk2C                              = 0;
    temp_r2                                                = (void*)((u8*)&g_DisplaySettings + 0x2FC);
    *(vu16*)0x0400100A                                     = (u16)((*(vu16*)0x0400100A & 0x43) | 0x4004);
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x2FC)) = 0;
    ((Boss03*)temp_r2)->unk4                               = 1;
    ((Boss03*)temp_r2)->unk28                              = 3;
    ((Boss03*)temp_r2)->unk1C                              = 0;
    ((Boss03*)temp_r2)->unk20                              = 2;
    ((Boss03*)temp_r2)->unk2C                              = 1;
    *(vu16*)0x0400100C                                     = (u16)((*(vu16*)0x0400100C & 0x43) | 0x420C);
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x2F4)) = 2;
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x330)) = 2;
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x4C)) =
        (s32)(((*(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x4C)) & ~1) | 6) & ~8);
}

void func_ov017_0212fc70(void) {
    void* temp_r0;
    void* temp_r2;

    temp_r0                                                = (void*)((u8*)&g_DisplaySettings + 0xA0);
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0xA0))  = 0;
    ((Boss03*)temp_r0)->unk4                               = 1;
    ((Boss03*)temp_r0)->unk1C                              = 0;
    ((Boss03*)temp_r0)->unk20                              = 0;
    ((Boss03*)temp_r0)->unk28                              = 1;
    ((Boss03*)temp_r0)->unk2C                              = 0;
    temp_r2                                                = (void*)((u8*)&g_DisplaySettings + 0xDC);
    *(vu16*)0x0400000A                                     = (u16)((*(vu16*)0x0400000A & 0x43) | 0x4004);
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0xDC))  = 0;
    ((Boss03*)temp_r2)->unk4                               = 1;
    ((Boss03*)temp_r2)->unk28                              = 3;
    ((Boss03*)temp_r2)->unk1C                              = 0;
    ((Boss03*)temp_r2)->unk20                              = 2;
    ((Boss03*)temp_r2)->unk2C                              = 1;
    *(vu16*)0x0400000C                                     = (u16)((*(vu16*)0x0400000C & 0x43) | 0x420C);
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0xD4))  = 2;
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x110)) = 2;
    *(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x1C)) =
        (s32)((*(s32*)((u8*)(void*)((u8*)&g_DisplaySettings + 0x1C)) | 6) & ~8);
}

void func_ov017_0212fd18(void* arg0) {
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 temp_r2_2;
    s32 temp_r3;
    s32 temp_r3_2;
    s32 var_ip;
    s32 var_r2;
    s32 var_r3;

    var_ip = (s32)(((Boss03*)arg0)->unk34 - ((Boss03*)arg0)->unk28) >> 2;
    var_r2 = (s32)(((Boss03*)arg0)->unk38 - ((Boss03*)arg0)->unk2C) >> 2;
    var_r3 = (s32)(((Boss03*)arg0)->unk3C - ((Boss03*)arg0)->unk30) >> 2;
    if (var_ip > 0x2000) {
        var_ip = 0x2000;
    } else if (var_ip < -0x2000) {
        var_ip = -0x2000;
    }
    if (var_r2 > 0x2000) {
        var_r2 = 0x2000;
    } else if (var_r2 < -0x2000) {
        var_r2 = -0x2000;
    }
    if (var_r3 > 0x2000) {
        var_r3 = 0x2000;
    } else if (var_r3 < -0x2000) {
        var_r3 = -0x2000;
    }
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + var_ip);
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + var_r2);
    temp_r3                = ((Boss03*)arg0)->unk30 + var_r3;
    ((Boss03*)arg0)->unk30 = temp_r3;
    ((Boss03*)arg0)->unk40 = (s32)(temp_r3 + (((Boss03*)arg0)->unk54 + ((Boss03*)arg0)->unk2C));
    temp_r2                = ((Boss03*)arg0)->unk4;
    temp_r1                = ((Boss03*)arg0)->unk28;
    temp_r3_2              = temp_r2 + 0x80000;
    temp_r2_2              = 0x180000 - temp_r2;
    if (temp_r1 < temp_r3_2) {
        ((Boss03*)arg0)->unk28 = temp_r3_2;
    } else if (temp_r1 > temp_r2_2) {
        ((Boss03*)arg0)->unk28 = temp_r2_2;
    }
    temp_r1_2 = ((Boss03*)arg0)->unk40;
    if (temp_r1_2 < 0x160000) {
        ((Boss03*)arg0)->unk40 = 0x160000;
        return;
    }
    if (temp_r1_2 > 0x1A0000) {
        ((Boss03*)arg0)->unk40 = 0x1A0000;
    }
}

void func_ov017_0212fe1c(void* arg0) {
    s32 temp_ip;
    s32 temp_r1;
    s32 temp_r1_2;
    s32 temp_r2;
    s32 temp_r3;
    s32 temp_r4;
    s32 temp_r4_2;
    s32 var_ip;
    s32 var_ip_2;
    s32 var_ip_3;
    s32 var_r2;
    s32 var_r3;
    s32 var_r4;

    var_r4 = (s32)(((Boss03*)arg0)->unk34 - ((Boss03*)arg0)->unk28) >> 5;
    if (var_r4 < 0) {
        var_ip = 0 - var_r4;
    } else {
        var_ip = var_r4;
    }
    var_r2 = (s32)(((Boss03*)arg0)->unk38 - ((Boss03*)arg0)->unk2C) >> 5;
    var_r3 = (s32)(((Boss03*)arg0)->unk3C - ((Boss03*)arg0)->unk30) >> 2;
    if (var_ip < 0x666) {
        var_r4 = 0;
    }
    if (var_r2 < 0) {
        var_ip_2 = 0 - var_r2;
    } else {
        var_ip_2 = var_r2;
    }
    if (var_ip_2 < 0x666) {
        var_r2 = 0;
    }
    if (var_r3 < 0) {
        var_ip_3 = 0 - var_r3;
    } else {
        var_ip_3 = var_r3;
    }
    if (var_ip_3 < 0x666) {
        var_r3 = 0;
    }
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + var_r4);
    ((Boss03*)arg0)->unk2C = (s32)(((Boss03*)arg0)->unk2C + var_r2);
    temp_r3                = ((Boss03*)arg0)->unk30 + var_r3;
    ((Boss03*)arg0)->unk30 = temp_r3;
    ((Boss03*)arg0)->unk40 = (s32)(temp_r3 + (((Boss03*)arg0)->unk54 + ((Boss03*)arg0)->unk2C));
    temp_r4                = ((Boss03*)arg0)->unk4;
    temp_r1                = ((Boss03*)arg0)->unk28;
    temp_r4_2              = temp_r4 + 0x80000;
    temp_ip                = (temp_r4 - 0x80000) + ((Boss03*)arg0)->unkC;
    temp_r2                = (((Boss03*)arg0)->unk8 - 0x60000) + ((Boss03*)arg0)->unk10;
    if (temp_r1 < temp_r4_2) {
        ((Boss03*)arg0)->unk28 = temp_r4_2;
    } else if (temp_r1 > temp_ip) {
        ((Boss03*)arg0)->unk28 = temp_ip;
    }
    temp_r1_2 = ((Boss03*)arg0)->unk40;
    if (temp_r1_2 < 0x60000) {
        ((Boss03*)arg0)->unk40 = 0x60000;
    } else if (temp_r1_2 > temp_r2) {
        ((Boss03*)arg0)->unk40 = temp_r2;
    }
    ((Boss03*)arg0)->unk28 = (s32)(((Boss03*)arg0)->unk28 + ((Boss03*)arg0)->unk44);
    ((Boss03*)arg0)->unk40 = (s32)(((Boss03*)arg0)->unk40 + (((Boss03*)arg0)->unk48 + ((Boss03*)arg0)->unk4C));
}

void func_ov017_0212ff54(CombatSprite* arg0, CombatSprite* arg1) {
    CombatSprite_LoadFromTable(0, arg0, &data_ov017_02141528, &data_ov017_021415a8, 0x12, 4, 0x10);
    CombatSprite_LoadFromTable(0, arg1, &data_ov017_02141510, &data_ov017_021415a8, 0x11, 0x16, 0x20);
}

void func_ov017_0212ffcc(s32 arg0, s32 arg1) {
    CombatSprite_Update(arg0);
    CombatSprite_Update(arg1);
}

void func_ov017_0212ffe4(s32 arg0, s32 arg1, s32 arg2) {
    SubroutineArgs subroutine_arg0;
    s16            sp2[4];
    void*          var_r3;
    void*          var_r4;
    s16            temp_r0_2;
    s16            temp_r4;
    s32            var_r2;
    u16            temp_r0;
    u16            temp_r1;
    u32            temp_r1_2;
    u32            temp_r1_3;

    var_r4 = &data_ov017_02142030;
    var_r3 = &subroutine_arg0;
    var_r2 = 4;
    do {
        temp_r1 = ((Boss03*)var_r4)->unk0;
        temp_r0 = ((Boss03*)var_r4)->unk2;
        var_r4 += 4;
        ((Boss03*)var_r3)->unk0 = temp_r1;
        ((Boss03*)var_r3)->unk2 = temp_r0;
        var_r3 += 4;
        var_r2 -= 1;
    } while (var_r2 != 0);
    if (data_ov003_020e71b8->unk3D878 & 2) {
        return;
    }
    temp_r1_2 = data_ov003_020e71b8->unk3D7CC;
    temp_r4   = func_ov003_020843b0(0, ((s32)(temp_r1_2 + (temp_r1_2 >> 0x1F)) >> 1) + 0x8000);
    temp_r1_3 = data_ov003_020e71b8->unk3D7D0;
    temp_r0_2 = func_ov003_020843ec(0, ((s32)(temp_r1_3 + (temp_r1_3 >> 0x1F)) >> 1) - 0x6C000, 0);
    CombatSprite_SetPosition(arg0, temp_r4, temp_r0_2);
    func_ov003_02082730(arg0, 0x7FFFFFFF);
    CombatSprite_Render(arg0);
    if (arg2 < 0) {
        return;
    }
    if (arg2 & 1) {
        CombatSprite_SetFlip(arg1, 0);
    } else {
        CombatSprite_SetFlip(arg1, 1);
    }
    CombatSprite_SetPosition(arg1, (s16)(temp_r4 + ((s32*)&subroutine_arg0)[arg2]), (s16)(temp_r0_2 + (&sp2[0])[arg2]));
    func_ov003_02082730(arg1, 0x7FFFFFFF);
    CombatSprite_Render(arg1);
}

void func_ov017_02130130(void* arg0, void* arg1) {
    CombatSprite_Release(arg0);
    CombatSprite_Release(arg1);
}

void func_ov017_02130148(void) {
    data_ov017_02143a18.unk8 = 0;
}

void func_ov017_0213015c(void) {
    if (data_ov017_02143a18.unk8 != 0) {
        data_ov017_02143a18.unk8 = (s32)(data_ov017_02143a18.unk8 - 1);
    }
}

void func_ov017_02130178(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 var_r6;

    var_r6 = arg1;
    if (data_ov017_02143a18.unk8 != 0) {
        return;
    }
    if ((arg2 != 0x56C) && (RNG_Next(2) != 0)) {
        var_r6 = arg2;
    }
    func_ov003_020c3acc(arg0, var_r6, arg3);
    data_ov017_02143a18.unk8 = 0x1E;
}

void func_ov017_021301e0(void) {
    data_ov017_02143a18.unk0 = (s16)data_ov003_020e71b8->unk3D8D8;
    data_ov017_02143a18.unk2 = (s16)data_ov003_020e71b8->unk3D8DA;
    data_ov017_02143a18.unk4 = (s16)data_ov003_020e71b8->unk3D8DC;
    data_ov017_02143a18.unk6 = (s16)data_ov003_020e71b8->unk3D8DE;
}

void func_ov017_0213021c(void* arg0) {
    data_ov003_020e71b8->unk3D8D8 = (s16)data_ov017_02143a18.unk0;
    data_ov003_020e71b8->unk3D8DA = (s16)data_ov017_02143a18.unk2;
    data_ov003_020e71b8->unk3D8DC = (s16)data_ov017_02143a18.unk4;
    data_ov003_020e71b8->unk3D8DE = (s16)data_ov017_02143a18.unk6;
    ((Boss03*)arg0)->unk6C        = (s16)data_ov017_02143a18.unk0;
    ((Boss03*)arg0)->unk6E        = (s16)data_ov017_02143a18.unk2;
}

void func_ov017_02130280(void* arg0) {
    ((Boss03*)arg0)->unk6C = (s16)data_ov017_02143a18.unk0;
    ((Boss03*)arg0)->unk6E = (s16)data_ov017_02143a18.unk2;
}