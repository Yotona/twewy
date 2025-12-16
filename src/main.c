#include "Input.h"
#include "Memory.h"
#include "NitroSDK/fs.h"
#include "OverlayDispatcher.h"
#include "OverlayManager.h"
#include "Random.h"
#include "System.h"
#include "common_data.h"
#include <NitroSDK/os/interrupt.h>
#include <NitroSDK/os/region.h>

extern vu32 data_02066a58;

extern s32 data_020636b8;
extern s32 data_0206a9bc;
extern s32 data_020636a0;

extern void func_02001254();

void main(void) {
    OS_InitAllSystems();
    OS_EnableFIQ();
    OS_InitIRQQueue();
    OS_EnableInterrupts(IRQ_IPC_RECEIVE);
    OS_RestoreIME(TRUE);

    void* ramTop    = OS_GetRegionTop(REGION_MAIN_ARM9);
    void* ramBottom = OS_GetRegionBottom(REGION_MAIN_ARM9);
    OS_SetRegionBottom(REGION_MAIN_ARM9, func_02039f5c(0, ramBottom, ramTop, 1));

    void* memRegion = OS_AllocRegionMemory(REGION_MAIN_ARM9, 0x288000, 32);
    func_02039f2c(0, func_0203a004(0, memRegion, memRegion + 0x288000));

    void* sp0 = func_02039dbc(0, -1, 0x280000);
    FS_InitFileSystem(2);

    u32 tableSize = FS_RomLoadDefaultTables(NULL, 0);
    FS_RomLoadDefaultTables(func_02039dbc(0, -1, tableSize), tableSize);

    OvlMgr_Init();

    System_ClearFlag(SYSFLAG_UNKNOWN_2);
    System_SetFlag(SYSFLAG_UNKNOWN_4);

    while (TRUE) {
        func_0203a09c();
        func_0203a0ac();
        IC_Enable();
        DC_Enable();
        GX_Init();
        func_02041438(1);

        if (func_0203a3cc() == 0) {
            func_0203a354();
        }

        if (func_0203a5d4() == 1) {
            func_0203a594();
        } else {
            func_0203a554();
        }

        FX_Init();
        REG_EXMEM_CNT |= 0x8000;

        System_ClearFlag(SYSFLAG_UNKNOWN_0);
        System_ClearFlag(SYSFLAG_RESET);
        SystemStatusFlags;

        System_SetFlag(SYSFLAG_UNKNOWN_7);
        System_ClearFlag(SYSFLAG_UNKNOWN_3);
        System_SetFlag(SYSFLAG_UNKNOWN_4);
        System_ClearFlag(SYSFLAG_UNKNOWN_10);
        SystemStatusFlags;
        System_SetFlag(SYSFLAG_UNKNOWN_5);
        SystemStatusFlags;
        System_SetFlag(SYSFLAG_UNKNOWN_6);

        u32 sp18;
        u32 sp14;
        func_020410ac(&sp18, &sp14);
        if (sp18 == 1) {
            SystemStatusFlags;
            System_SetFlag(SYSFLAG_UNKNOWN_8);
        } else {
            SystemStatusFlags;
            System_ClearFlag(SYSFLAG_UNKNOWN_8);
        }
        if (sp14 == 1) {
            SystemStatusFlags;
            System_SetFlag(SYSFLAG_UNKNOWN_9);
        } else {
            SystemStatusFlags;
            System_ClearFlag(SYSFLAG_UNKNOWN_9);
        }
        SystemStatusFlags;
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

        Mem_InitializeHeap(&gMainHeap, sp0, 0x280000);
        func_02004bbc(&data_0206a9bc, 0x20000, "TmpBuf extends FlushBuf");

        void* stdHeap = Mem_AllocHeapTail(&gMainHeap, 0x80000);
        Mem_SetSequence(&gMainHeap, stdHeap, "SeqHeap(StdHeap)");
        func_0203b2d0(0, stdHeap, Mem_GetBlockSize(&gMainHeap, stdHeap));

        Mem_InitializeHeap(&gDebugHeap, stdHeap, 0x80000);

        SndMgr_Init();
        OvlMgr_UnloadOverlay(-1);
        Interrupts_Init();
        Display_Init();
        Interrupts_ForceVBlank();
        DMA_Init(0x100);
        Input_Init(&InputStatus, 8, 1, 2);
        MainOvlDisp_Init();
        MainOvlDisp_Push(OVERLAY_ID_NONE, &func_02001254, NULL, 0);

        System_SetFlag(SYSFLAG_UNKNOWN_2);

        DebugOvlDisp_Init();
        OvlDisp_InitUnused2();
        OvlDisp_InitUnused1();
        RNG_SetSeed(0);
        DatMgr_Init(0, 0);
        func_0200cef0(0);
        SysControl.frameCount = 0;
        func_02006ad8();
        func_020415a4();
        func_02025b1c();

        while (System_CheckFlag(SYSFLAG_RESET) == 0 || System_CheckFlag(SYSFLAG_UNKNOWN_7) == 0) {
            func_02004c44(&data_0206a9bc);

            Input_PollState(&InputStatus);
            Input_UpdateRepeat(&InputStatus, INPUT_MASK_ALLBTNS);
            SysControl.currButtons    = InputStatus.currButtons;
            SysControl.pressedButtons = InputStatus.pressedButtons;
            SysControl.holdButtons    = InputStatus.holdButtons;
            SysControl.prevButtons    = InputStatus.prevButtons;

            func_02007240();
            MainOvlDisp_Run();
            func_02026a94();

            if (System_CheckFlag(SYSFLAG_UNKNOWN_3)) {
                func_020218ec();
            }

            // Trigger a soft reset when Start + Select + L + R are pressed
            if ((InputStatus.currButtons & INPUT_SOFT_RESET) == INPUT_SOFT_RESET) {
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
                        u32 sp10;
                        u32 spC;
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
                u32 sp8;
                u32 sp4;
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
        }

        OvlMgr_UnloadAllOverlays();
        OS_SystemReset(BIOS_RESET);
    }
}

extern void func_ov037_0208370c(void* state);
extern void func_ov046_02083630(void* state);

static const char* data_020636cc = "Seq_Boot(void *)";

// Nonmatching: Differences in overlay handling
void func_02001254(void) {
    if (MainOvlDisp_GetRepeatCount() == 0) {
        const char* seq = data_020636cc;

        void* binData = Mem_AllocHeapTail(&gDebugHeap, 0x304);

        Mem_SetSequence(&gDebugHeap, binData, seq);
        func_0203b2d0(0, binData, Mem_GetBlockSize(&gDebugHeap, binData));
        MainOvlDisp_SetCbArg(binData);
        BinMgr_Init(binData, 8);
        PacMgr_Init(binData + 0x60, 0x20);
        DatMgr_Init(binData + 0xC0, 0x100);
        MainOvlDisp_IncrementRepeatCount();
        MainOvlDisp_Push(OVERLAY_37_ID, func_ov037_0208370c, NULL, 0);
    } else {
        MainOvlDisp_Push(OVERLAY_46_ID, func_ov046_02083630, NULL, 0);
    }
}
