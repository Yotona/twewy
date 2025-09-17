#include "Input.h"
#include "System.h"
#include "common_data.h"
#include "game.h"

extern vu32 data_02066a58;

extern s32 data_020636b8;
extern s32 data_0206a9bc;
extern s32 data_020636a0;

extern InputState data_0206a9e0;

extern void func_02001254();

// Nonmatching: goto loops are bork
void main(void) {
    s32 sp18;
    u32 sp14;
    s32 sp10;
    u32 spC;
    s32 sp8;
    u32 sp4;
    s32 sp0;
    s32 temp_r0;
    s32 temp_r4;

    func_020398d0();
    func_0203a77c();
    func_020383bc();
    func_020385a4(0x40000);

    REG_IME = 1;

    temp_r4 = func_02039a88(0);
    func_02039c50(0, func_02039f5c(0, func_02039a9c(0), temp_r4, 1));

    temp_r0 = func_02039c64(0, 0x288000, 0x20);
    func_02039f2c(0, func_0203a004(0, temp_r0, temp_r0 + 0x288000));

    sp0 = func_02039dbc(0, -1, 0x280000);
    func_0203e59c(2);

    s32 temp_r0_2 = func_0203ed04(0, 0);
    func_0203ed04(func_02039dbc(0, -1, temp_r0_2), temp_r0_2);

    func_020065e0();

    System_ClearFlag(SYSFLAG_UNKNOWN_2);
    System_SetFlag(SYSFLAG_UNKNOWN_4);

    while (TRUE) {
        func_0203a09c();
        func_0203a0ac();
        IC_Enable();
        DC_Enable();
        func_02035240();
        func_02041438(1);

        if (func_0203a3cc() == 0) {
            func_0203a354();
        }

        if (func_0203a5d4() == 1) {
            func_0203a594();
        } else {
            func_0203a554();
        }

        func_0203523c();
        REG_EXMEM_CNT |= 0x8000;

        System_ClearFlag(SYSFLAG_UNKNOWN_0);
        System_ClearFlag(SYSFLAG_RESET);
        System_SetFlag(SYSFLAG_UNKNOWN_7);
        System_ClearFlag(SYSFLAG_UNKNOWN_3);
        System_SetFlag(SYSFLAG_UNKNOWN_4);
        System_ClearFlag(SYSFLAG_UNKNOWN_10);
        System_SetFlag(SYSFLAG_UNKNOWN_5);
        System_SetFlag(SYSFLAG_UNKNOWN_6);

        func_020410ac(&sp18, &sp14);
        if (sp18 == 1) {
            System_SetFlag(SYSFLAG_UNKNOWN_8);
        } else {
            System_ClearFlag(SYSFLAG_UNKNOWN_8);
        }
        if (sp14 == 1) {
            System_SetFlag(SYSFLAG_UNKNOWN_9);
        } else {
            System_ClearFlag(SYSFLAG_UNKNOWN_9);
        }

        System_SetFlag(SYSFLAG_UNKNOWN_7);

        data_02066a58 &= ~1;
        data_02066a58 &= ~2;
        data_02066a58 &= ~4;
        data_02066a58 &= ~8;
        data_02066a58 &= ~0x10;
        data_02066a58 &= ~0x20;
        data_02066a58 &= ~0x40;
        data_02066a58 &= ~0x80;
        data_02066a58 &= ~0x100;

        if ((func_02039380() & 0xF0000000) == 0) {
            data_02066a58 |= 0x20;
        } else {
            data_02066a58 |= 0x10;
        }

        func_02004590(&data_0206a9a4, sp0, 0x280000);
        func_02004bbc(&data_0206a9bc, 0x20000, &data_020636a0);

        s32 temp_r0_3 = func_02004618(&data_0206a9a4, 0x80000);
        func_020049a8(&data_0206a9a4, temp_r0_3, &data_020636b8);
        func_0203b2d0(0, temp_r0_3, func_0200498c(&data_0206a9a4, temp_r0_3));
        func_02004590(&data_0206a9b0, temp_r0_3, 0x80000);

        SndMgr_Init();
        func_02006618(-1);
        Interrupts_Init();
        func_02005250();
        Interrupts_ForceVBlank();
        func_02001760(0x100);
        Input_Init(&data_0206a9e0, 8, 1, 2);
        func_02007128();
        func_0200713c(0x80000001, &func_02001254, 0, 0);

        System_SetFlag(SYSFLAG_UNKNOWN_2);

        func_020072ec();
        func_020073a4();
        func_020073b8();
        func_02006798(0);
        func_02008158(0, 0);
        func_0200cef0(0);
        SysControl.unk_20 = 0;
        func_02006ad8();
        func_020415a4();
        func_02025b1c();

    loop_44:
        if (System_CheckFlag(SYSFLAG_UNKNOWN_5) == 0) {
        block_16:
            func_02004c44(&data_0206a9bc);

            Input_PollState(&data_0206a9e0);
            Input_UpdateRepeat(&data_0206a9e0, INPUT_MASK_ALLBTNS);
            SysControl.currButtons    = data_0206a9e0.currButtons;
            SysControl.pressedButtons = data_0206a9e0.pressedButtons;
            SysControl.holdButtons    = data_0206a9e0.holdButtons;
            SysControl.prevButtons    = data_0206a9e0.prevButtons;

            func_02007240();
            func_020072b8();
            func_02026a94();

            if (System_CheckFlag(SYSFLAG_UNKNOWN_3)) {
                func_020218ec();
            }

            // Trigger a soft reset when Start + Select + L + R are pressed
            if ((data_0206a9e0.currButtons & INPUT_SOFT_RESET) == INPUT_SOFT_RESET) {
                System_SetFlag(SYSFLAG_RESET);
            }

            System_ClearFlag(SYSFLAG_UNKNOWN_10);

            u32 var_r0 = System_CheckFlag(SYSFLAG_UNKNOWN_6);
            if (var_r0) {
                if (var_r0 == 1) {
                    if (Input_IsSystemHingeClosed()) {
                        if (System_CheckFlag(SYSFLAG_UNKNOWN_5)) {
                            func_02041134(0xC, 0, 0);
                            System_SetFlag(SYSFLAG_UNKNOWN_10);
                        }
                    } else {
                        func_020410ac(&sp10, &spC);
                        if (System_CheckFlag(SYSFLAG_UNKNOWN_8) && sp10 == 0) {
                            func_02040f34(0, 1);
                        }
                        if (System_CheckFlag(SYSFLAG_UNKNOWN_9) && spC == 0) {
                            func_02040f34(1, 1);
                        }
                    }
                }
            } else {
                func_020410ac(&sp8, &sp4);
                if (System_CheckFlag(SYSFLAG_UNKNOWN_8)) {
                    if (Input_IsSystemHingeClosed()) {
                        if (sp8 == 1) {
                            func_02040f34(0, 0);
                        }
                    } else if (sp8 == 0) {
                        func_02040f34(0, 1);
                    }
                }
                if (System_CheckFlag(SYSFLAG_UNKNOWN_9)) {
                    if (Input_IsSystemHingeClosed()) {
                        if (sp4 == 1) {
                            func_02040f34(1, 0);
                        }
                    } else if (sp4 == 0) {
                        func_02040f34(1, 1);
                    }
                }
            }

            func_020235a8();
            Interrupts_ForceVBlank();
            goto loop_44;
        }
        if (System_CheckFlag(SYSFLAG_UNKNOWN_7) == 0) {
            goto block_16;
        }
        func_02006608();
        func_0203a8b0(BIOS_RESET);
    }
}

extern u32 OVERLAY_37_ID;
extern u32 OVERLAY_46_ID;

extern void func_ov037_0208370c(GameState*);
extern void func_ov046_02083630(GameState*);

const char* data_020636cc = "Seq_Boot(void *)";

// Nonmatching: Differences in overlay handling
void func_02001254(void) {
    if (func_02007278() == 0) {
        const char* temp_r5 = data_020636cc;
        s32         temp_r0 = func_02004618(&data_0206a9b0, 0x304);

        func_020049a8(&data_0206a9b0, temp_r0, temp_r5);
        func_0203b2d0(0, temp_r0, func_0200498c(&data_0206a9b0, temp_r0));
        func_02007260(temp_r0);
        BinMgr_Init(temp_r0, 8);
        func_02007b40(temp_r0 + 0x60, 0x20);
        func_02008158(temp_r0 + 0xC0, 0x100);
        func_020072a4();
        func_0200713c(OVERLAY_37_ID, &func_ov037_0208370c, NULL, 0);
        return;
    }

    func_0200713c(OVERLAY_46_ID, &func_ov046_02083630, NULL, 0);
}
