#include "Debug/OpenEnd.h"

extern void func_ov002_02086a4c(GameState* r0);
extern void func_ov030_020b0fe8(GameState* r0);
void        func_ov037_02083188(GameState* r0) {
    OverlayTag tag, tag2;
    if (r0->unk_11A44 != 0) {
        func_020071f4(&tag, &OVERLAY_2_ID, func_ov002_02086a4c, 0, 0);
    } else {
        func_020071f4(&tag2, &OVERLAY_30_ID, func_ov030_020b0fe8, 0, 0);
    }
}

extern void func_ov002_02086acc(GameState* r0);
void        func_ov037_02082f04(GameState* r0) {
    OverlayTag tag;
    *(s32*)(&data_02074d10 + 0x104) = 1;
    if (r0->unk_11A40 != 0) {
        func_020071f4(&tag, &OVERLAY_2_ID, func_ov002_02086acc, 0, 0);
    } else {
        func_ov037_020833a8(r0);
    }
}

void func_ov037_02082cd4(GameState* r0) {
    func_ov037_02082c58(r0);
    func_ov037_02082b30((s32*)r0, 1, 0, 0);
    func_ov037_02082b30((s32*)r0, 0, 0, 1);
    func_ov037_02082b30((s32*)r0, 1, 1, 2);
    func_ov037_02082b30((s32*)r0, 0, 1, 3);
    r0->unk_11A1C = 0;
    r0->unk_11A30 = *(s32*)(&data_ov037_02083a7c + *(s32*)(&data_02074d10 + 0x104));
    r0->unk_11A34 = *(s32*)(&data_ov037_02083a74 + *(s32*)(&data_02074d10 + 0x104));
    func_02007328();
}

void func_ov037_02082c58(GameState* r0) {
    if (r0->unk_11A3C != 0)
        return;
    s32 temp      = func_02024aa4();
    r0->unk_11A38 = temp;
    r0->unk_11A3C = 1;
    r0->unk_11A40 = 0;
    if (temp == 0) {
        if ((*(u16*)(&data_02073710 + 0x5B) & 1) != 0) {
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

void func_ov037_02082c2c(GameState* r0) {
    func_02026180(0, r0->unk_11A30, r0->unk_11A34);
    if (func_0202623c() == 0) {
        func_02007328();
    }
}

void func_ov037_02082c00(GameState* r0) {
    func_02026180(0, NULL, r0->unk_11A34);
    if (func_0202623c() == 0) {
        func_02007328();
    }
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
        func_020379c0();
        func_02037a60(r1, r2, r3);
        func_02037ad0();
    }
    if (r0 != 1) {
        return;
    }
    func_02037c10();
    func_02037c28(r1, r2, r3);
    func_02037c8c();
    return;
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

void func_ov037_020828f8(void) {
    Interrupts_RegisterVBlankCallback(NULL, TRUE);
}

void func_ov037_020828dc(void) {
    func_ov037_020824a0();
    Interrupts_RegisterVBlankCallback(func_ov037_0208280c, TRUE);
}

void func_ov037_0208280c(void)

{
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0) == 0) {
        return;
    }
    func_02006380();
    DMA_Flush();
    DC_PurgeRange(&data_0206770c, 0x400);
    func_02037264(&data_0206770c, 0, 0x400);
    DC_PurgeRange(&data_02068798, 0x400);
    func_020372b8(&data_02068798, 0, 0x400);
    DC_PurgeRange(&data_02066aec, 0x400);
    func_02037108(&data_02066aec, 0, 0x200);
    func_020371b4(&data_02066cec, 0, 0x200);
    DC_PurgeRange(&data_02066eec, 0x400);
    func_0203715c(&data_02066eec, 0, 0x200);
    func_0203720c(&data_020670ec, 0, 0x200);
    return;
}