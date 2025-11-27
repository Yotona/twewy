#include "Debug/OpenEnd.h"

void func_ov037_020824a0() {
    /*Not Implemented: function is too huge.*/
}

void func_ov037_0208280c(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0) == 0) {
        return;
    }
    func_02006380();
    DMA_Flush();
    DC_PurgeRange(&data_0206770c, 0x400);
    GX_LoadOam(&data_0206770c, 0, 0x400);
    DC_PurgeRange(&data_02068798, 0x400);
    GXs_LoadOam(&data_02068798, 0, 0x400);
    DC_PurgeRange(&data_02066aec, 0x400);
    GX_LoadBgPltt(&data_02066aec, 0, 0x200);
    GX_LoadObjPltt(&data_02066cec, 0, 0x200);
    DC_PurgeRange(&data_02066eec, 0x400);
    GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
    GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    return;
}

void func_ov037_020828dc(void) {
    func_ov037_020824a0();
    Interrupts_RegisterVBlankCallback(func_ov037_0208280c, TRUE);
}

void func_ov037_020828f8(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov037_0208290c(s32 r0, s32 r1, s32 r2, s32 r3, s32 s1) {
    if (r0 == 0) {
        if (r1 == 0) {
            func_020373c0(r2, r3, s1);
        }
        if (r1 == 1) {
            func_02037480(r2, r3, s1);
        }
        if (r1 == 2) {
            func_02037540(r2, r3, s1);
        }
        if (r1 == 3) {
            func_02037600(r2, r3, s1);
        }
    }
    if (r0 != 1) {
        return;
    }
    if (r1 == 0) {
        func_02037420(r2, r3, s1);
    }
    if (r1 == 1) {
        func_020374e0(r2, r3, s1);
    }
    if (r1 == 2) {
        func_020375a0(r2, r3, s1);
    }
    if (r1 != 3) {
        return;
    }
    func_02037660(r2, r3, s1);
}

void func_ov037_020829f4(s32 r0, s32 r1, s32 r2, s32 r3, s32 s1)

{
    if (r0 == 0) {
        if (r1 == 0) {
            func_020376c0(r2, r3, s1);
        }
        if (r1 == 1) {
            func_02037780(r2, r3, s1);
        }
        if (r1 == 2) {
            func_02037840(r2, r3, s1);
        }
        if (r1 == 3) {
            func_02037900(r2, r3, s1);
        }
    }
    if (r0 != 1) {
        return;
    }
    if (r1 == 0) {
        func_02037720(r2, r3, s1);
    }
    if (r1 == 1) {
        func_020377e0(r2, r3, s1);
    }
    if (r1 == 2) {
        func_020378a0(r2, r3, s1);
    }
    if (r1 != 3) {
        return;
    }
    func_02037960(r2, r3, s1);
}

void func_ov037_02082adc(int r0, int r1, int r2, int r3) {
    if (r0 == 0) {
        GX_BeginLoadBgExtPltt();
        GX_LoadBgExtPltt(r1, r2, r3);
        GX_EndLoadBgExtPltt();
    }
    if (r0 != 1) {
        return;
    }
    func_02037c10();
    func_02037c28(r1, r2, r3);
    func_02037c8c();
    return;
}

/* Nonmatching: if r0 isn't GameState* then what is it? Decompiling this TU won't be easy until we know for sure.*/
void func_ov037_02082b30(s32* r0, s32 r1, s32 r2, s32 r3) {
    s32 temp1 = r3 * 3;
    for (s32 idx = 0; idx < 3; idx++) {
        temp1++;
        *(s32*)(r0 + idx) = func_0200823c(*(r0 + 3), 0, 0, &data_ov037_02083aa8 + (temp1 << 3));
    }
    DC_PurgeAll();
    func_ov037_0208290c(r1, r2, *(s32*)(*(r0 + 2) + 2), 0, *(s32*)(*(r0 + 2) + 3));
    func_ov037_020829f4(r1, r2, *(s32*)(*r0 + 2), 0, *(s32*)(*(r0) + 3));
    func_ov037_02082adc(r1, *(s32*)(*(r0 + 1) + 2), r2 << 0xd, *(s32*)*(r0 + 1) + 3);
    for (s32 idx = 0; idx < 3; idx++) {
        func_02008dbc(*(r0 + idx));
    }
}

void func_ov037_02082c00(GameState* r0) {
    func_02026180(0, NULL, r0->unk_11A34);
    if (func_0202623c() == 0) {
        func_02007328();
    }
}

void func_ov037_02082c2c(GameState* r0) {
    func_02026180(0, r0->unk_11A30, r0->unk_11A34);
    if (func_0202623c() == 0) {
        func_02007328();
    }
}

void func_ov037_02082c58(GameState* r0) {
    if (r0->unk_11A3C != 0)
        return;
    s32 temp      = func_02024aa4();
    r0->unk_11A38 = temp;
    r0->unk_11A3C = 1;
    r0->unk_11A40 = 0;
    if (temp == 0) {
        if (((data_02073710[0x5B]) & 1) != 0) {
            r0->unk_11A44 = 1;
        }
        return;
    } else if (temp != 1) {
        r0->unk_11A48 = 1;
        return;
    }
    func_02024d04();
    r0->unk_11A40 = 1;
}

void func_ov037_02082cd4(GameState* r0) {
    func_ov037_02082c58(r0);
    func_ov037_02082b30((s32*)r0, 1, 0, 0);
    func_ov037_02082b30((s32*)r0, 0, 0, 1);
    func_ov037_02082b30((s32*)r0, 1, 1, 2);
    func_ov037_02082b30((s32*)r0, 0, 1, 3);
    r0->unk_11A1C = 0;
    r0->unk_11A30 = data_ov037_02083a7c[data_02074d10.unk_410];
    r0->unk_11A34 = data_ov037_02083a74[data_02074d10.unk_410];
    func_02007328();
}

void func_ov037_02082d7c(GameState* r0) {
    switch (r0->unk_11A20) {
        case 0:
            if (r0->unk_11A1C > 0x78) {
                r0->unk_11A1C = 0;
                r0->unk_11A20++;
                break;
            }
            r0->unk_11A1C++;
            break;
        case 1:
            func_020261d4(0, -0x10, 0x333);
            func_02026208(0, -0x10, 0x333);
            if (func_0202623c() != 0)
                break;
            r0->unk_11A1C = 0;
            r0->unk_11A20++;
            break;
        case 2:
            if (r0->unk_11A1C == 0) {
                data_0206aa80.unk_1C = data_0206aa80.unk_1C & ~0x1 | 0x2;
                data_0206aa80.unk_4C = data_0206aa80.unk_4C & ~0x1 | 0x2;
            }
            func_020261d4(0, 0, 0x333);
            func_02026208(0, 0, 0x333);
            if (func_0202623c() == 0) {
                r0->unk_11A1C = 0;
                r0->unk_11A20++;
                break;
            }
            r0->unk_11A1C++;
            break;
        case 3:
            if (r0->unk_11A1C > 0x78) {
                func_02007328();
                break;
            }
            r0->unk_11A1C++;
            break;
    }
}

extern void func_ov002_02086acc(GameState* r0);
void        func_ov037_02082f04(GameState* r0) {
    OverlayTag tag;
    data_02074d10.unk_410 = 1;
    if (r0->unk_11A40 != 0) {
        func_020071f4(&tag, &OVERLAY_2_ID, func_ov002_02086acc, NULL, 0);
    } else {
        func_ov037_020833a8(r0);
    }
}

/*Nonmatching: regswap */
extern void func_ov002_02086a8c(GameState* state);
extern void func_ov002_02086b0c(GameState* state);
void        func_ov037_02082f60(GameState* r0) {
    OverlayTag tag, tag2;
    func_ov037_02082c58(r0);
    if (r0->unk_11A48 != 0) {
        if (r0->unk_11A38 == 2) {
            func_020071f4(&tag, &OVERLAY_2_ID, func_ov002_02086b0c, 0, 0);
        } else {
            func_020071f4(&tag2, &OVERLAY_2_ID, func_ov002_02086a8c, 0, 0);
        }
        func_020072ec();
        return;
    }
    data_0206aa80.unk_1C = (data_0206aa80.unk_1C | 0x1) & ~0x2;
    data_0206aa80.unk_4C = (data_0206aa80.unk_4C | 0x1) & ~0x2;
    func_ov037_02082b30((s32*)r0, 0, 0, 5);
    func_ov037_02082b30((s32*)r0, 1, 0, 4);
    func_ov037_02083a34(0);
    if (r0->unk_11A44 != 0) {
        func_ov037_02083a34(1);
    }
    r0->unk_11A20 = 0;
    r0->unk_11A24 = 0;
    r0->unk_11A30 = data_ov037_02083a7c[data_02074d10.unk_410]; // Regswap here
    r0->unk_11A34 = data_ov037_02083a74[data_02074d10.unk_410]; // and here
    r0->unk_11A2C = 0;
    func_0202733c();
    func_02007328();
}

/* Nonmatching: Opcode reordering*/
void func_ov037_020830a8(GameState* r0) {
    typedef struct {
        s32 x;
        s32 y;
    } TouchCoords;
    TouchCoords coords;
    func_02006df0(&coords); // TODO: Confirm that this function is for TouchScreen info
    switch (r0->unk_11A20) {
        case 0:
            if (r0->unk_11A24 <= 0x9e340) {
                r0->unk_11A24 = 0x9e341;
            } else {
                func_02027388(0);
                r0->unk_11A24 = 0;
                r0->unk_11A20 += 1;
            }
            break;
        case 1:
            if (r0->unk_11A24 <= 0xa) {
                r0->unk_11A24++;
            } else {
                func_0202733c(0);
                r0->unk_11A20 = 0;
                r0->unk_11A24 = 0;
            }
            break;
    }
    if (r0->unk_11A2C == 0)
        return;
    r0->unk_11A30 = ~0xF;
    r0->unk_11A34 = 0x800;
    func_02007328();
}

extern void func_ov002_02086a4c(GameState* r0);
extern void func_ov030_020b0fe8(GameState* r0);
void        func_ov037_02083188(GameState* r0) {
    OverlayTag tag, tag2;
    if (r0->unk_11A44 != 0) {
        func_020071f4(&tag, &OVERLAY_2_ID, func_ov002_02086a4c, NULL, 0);
    } else {
        func_020071f4(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, NULL, 0);
    }
}

/*Nonmatching: data_02073710 has weird 0x100 offset, also the array accesses don't have the right index?*/
extern void func_ov044_02084a88();
extern void func_ov030_020ae92c();
void        func_ov037_020831ec(GameState* r0) {
    OverlayTag tag, tag2, tag3, tag4;
    if (func_020256bc() == 0) {
        if (func_02023010(0x2AB) != 0) {
            data_02073710[0xB4] |= 0x10;
            func_020071f4(&tag, &OVERLAY_44_ID, func_ov044_02084a88, 0, 0);
            return;
        }
        if ((data_02073710[0xB4] & 0x2) != 0) {
            data_02073710[0xB4] &= ~0x2;
            func_020071f4(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, 0, 0);
            return;
        }
        func_020071f4(&tag3, &OVERLAY_30_ID, func_ov030_020ae92c, 0, 0);
        return;
    }
    func_020071f4(&tag4, &OVERLAY_2_ID, func_ov002_02086a8c, 0, 0);
}

void func_ov037_020832dc(GameState* r0) {
    OverlayTag tag, tag2;
    r0->unk_11A3C = 0;
    func_ov037_02082c58(r0);
    if (r0->unk_11A48 != 0) {
        if (r0->unk_11A38 == 2) {
            func_020071f4(&tag, &OVERLAY_2_ID, func_ov002_02086b0c, 0, 0);
        } else {
            func_020071f4(&tag2, &OVERLAY_2_ID, func_ov002_02086a8c, 0, 0);
        }
        func_020072ec();
        return;
    }
    func_02027388(0);
    switch (r0->unk_11A28) {
        case 0:
            func_ov037_02083188(r0);
            break;
        case 1:
            func_ov037_020831ec(r0);
            break;
    }
    func_02007328();
}

/*Nonmatching: The data entries referenced here are likely struct arrays (funcpointer tables?) with each struct being 0xC in
 * size.*/
void func_ov037_020833a8(GameState* r0) {
    func_020072ec();
    func_02007300(data_ov037_02083a98[data_02074d10.unk_410], r0, 0);
    func_02007300(func_ov037_02082c2c, r0, 0);
    func_02007300(data_ov037_02083a94[data_02074d10.unk_410], r0, 0);
    func_02007300(func_ov037_02082c00, r0, 0);
    func_02007300(data_ov037_02083a90[data_02074d10.unk_410], r0, 0);
}

void func_ov037_0208345c(GameState* r0) {
    // Not Implemented: figure out .bss entry data_ov037_02083e00 first.
    /*if(r0 == NULL){
        void* unk = Mem_AllocHeapTail(&gDebugHeap, 0x11A4C);
        Mem_SetSequence(0x11A4C, unk, "    ");
        data_ov037_02083e00.unk_00 = unk;
        func_02007260(unk);
    }
    func_0203b2d0(0, r0, Mem_GetBlockSize(&gDebugHeap, r0));
    func_ov037_020828dc();
    u32 temp = data_ov037_02083a7c[data_02074d10.unk_410];
    if(data_ov037_02083a7c[data_02074d10.unk_410] <= 0x10 && data_ov037_02083a7c[data_02074d10.unk_410] <= ~0xf){
        temp = ~0xf;
    }
    data_0206aa80.unk_30 = temp;
    if(data_ov037_02083a7c[data_02074d10.unk_410] <= 0x10 && data_ov037_02083a7c[data_02074d10.unk_410] <= ~0xf){
        temp = ~0xf;
    }
    data_0206aa80.unk_60 = temp;
    func_0200cef0(&(r0->binMgr.rootBin.size));
    Input_Init(&InputStatus, 8, 1, 2);
    func_02006ad8();
    func_02006ba0();
    func_02025b1c();
    data_02066a58.unk_00 &= ~0x8;
    r0->binMgr.rootBin.id = func_02008e80();
    r0->unk_11A38 = 0;
    Mem_InitializeHeap(&r0->unk_11610, &r0->unk_1161C, 0x400);
    func_0200f390(&r0->unk_11590, &r0->unk_11610, 0x10, 0, 0);
    data_ov037_02083e00.unk_08 = &r0->unk_11590;
    func_0200f68c(&r0->unk_11590, &data_0205cb10, 0, 0, 0, 0);*/
    func_ov037_020833a8(r0);
    func_020072a4();
}

void func_ov037_02083604(GameState* r0) {
    func_0200283c(&data_020676ec, 0, 0);
    func_0200283c(&data_02068778, 0, 0);
    func_02003440(&data_020676ec);
    func_02003440(&data_02068778);
    func_02006ba0();
    if (func_02006d4c() != 0) {
        // data_ov037_02083e00.unk_04 = 0x1;
    }
    func_0200f514(&r0->unk_11590);
    func_02007390();
    if (func_0200737c() != 0)
        return;
    func_020034b0(&data_020676ec);
    func_020034b0(&data_02068778);
    func_0200bf60(data_0206b3cc.unk_00, 0);
    func_0200bf60(data_0206b3cc.unk_04, 0);
}

void func_ov037_020836b4(GameState* r0) {
    if (r0 == NULL) {
        return;
    }
    data_02074d10.unk_410 = 1;
    func_ov037_020828f8();
    func_02008ebc(r0->binMgr.rootBin.id);
    func_0200f4f8(&r0->unk_11590);
    Mem_Free(&gDebugHeap, r0);
    func_02007260(0);
    func_02027388(0);
}

void func_ov037_0208370c(GameState* r0) {
    u32 index = func_02007278();
    if (index == ~0x80000000) {
        func_ov037_020836b4(r0);
        return;
    }
    // data_ov037_02083a84(r0)[index];
}

void func_ov037_0208374c(GameState* r0) {
    /*
    if(data_ov037_02083e00.unk_11A2C != 0)
        return;
    data_ov037_02083e00.unk_11A28 = r0;
    data_ov037_02083e00.unk_11A2C = 1;
    */
    func_02026b20(2);
}

void func_ov037_0208378c(u32 r0, u32 r1, u32 r2) {
    /*
    load r3 and r12 from r0 struct;
    return (((r1 - r3) ^ 2) + ((r2 - r12) ^ 2)) > ((r0->unk_08) ^ 2) ? 0 : 1;
    */
}

void func_ov037_020837b8(void* r0, s16 r1, s16 r2, s16 r3, s32 s1) {
    /*
    Store the four members of data_ov037_02083b84 into r0
    then do it again(?)
    then do it again but only with the first three members (?)
    r0->0x14 = data_ov037_02083aa8;
    r0->0x1C = r1;
    r0->0x26 = r2;
    r0->0x28 = r3;
    r0->0x2A = stack[0x20];
    */
}

int func_ov037_02083814(void* unused_r0, void* r1, void* r2) {
    u32* unk = *(u32*)(r1 + (0x18));
    func_0203b3c0(*unk, 0, 0x84);
    u32 unk2            = *(u32*)(r2 + 0x0);
    *(u32*)(unk + 0x80) = unk2;
    // func_ov037_020837b8(&stack[0x4], (s16)(unk2 * 3 + 2) , (s16)((unk2 + 1) * 3) , (s16)(unk2 * 3 + 4) , 1);
    // func_0200e98c(unk->0x40, &stack[0x4]);
    // stack[0x2E] = 2;
    // func_0200e98c(unk, &stack[0x4]);
    return 1;
}

int func_ov037_020838a4(void* unused_r0, void* r1, void* r2) {
    u32* unk = *(u32*)(r1 + (0x18));
    // func_02006df0(&stack[0x0]);
    //&stack[0x8] = &stac[0x0];
    //&stack[0xC] = &stack[0x4];
    /*if(data_ov037_02083e04.unk_00 != 0){
        if(func_02006d30() != 0){
            if(func_ov037_0208378c(data_ov037_02083b5c[unk->0x80 * 0x14] , stack[0x8], stack[0xC]) != 0){
                unk->0xC = 0x82;
                unk->0xE = 0x62;
            }
        }
    }else{
        unk->0xC = 0x82;
        unk->0xE = 0x62;
    }
    if(data_ov037_02083e04.unk_00 != 0 && func_02006d80() != 0){
        if(func_ov037_0208378c(data_ov037_02083b5c[unk->0x80 * 0x14] , stack[0x8], stack[0xC]) != 0){
            func_ov037_0208374c(data_ov037_02083b6c[unk->0x80 * 0x14]);
        }
    }*/
    func_0200dd60(unk);
    // func_0200dd60(&unk->0x40);
    return 1;
}

int func_ov037_020839ac(void* unused_r0, void* r1, void* r2) {
    u32* unk = *(u32*)(r1 + (0x18));
    func_0200e2c4(unk + 0x10);
    func_0200e2c4(unk);
    return 1;
}

int func_ov037_020839cc(void* unused_r0, void* r1, void* r2) {
    u32* unk = *(u32*)(r1 + (0x18));
    func_0200e998(unk);
    func_0200e998(unk + 0x10);
    return 1;
}

void func_ov037_020839ec(u32 r0, u32 r1, u32 r2, u32 index) {
    /*
    copy the four members of data_ov037_02083b4c to &stack[0x0]
    &stack(r0, r1, r2)[index * 4];
    */
}

void func_ov037_02083a34(u32 r0) {
    func_0200f68c(data_ov037_02083e08, &data_ov037_02083b40, 0, 0, 0, &r0, r0);
}