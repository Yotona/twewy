#include "DatMgr.h"
#include "Input.h"
#include "Memory.h"
#include "OverlayDispatcher.h"
#include "OverlayManager.h"
#include "Random.h"
#include "System.h"
#include "common_data.h"
#include <NitroSDK/fs.h>
#include <NitroSDK/os.h>

extern vu32 data_02066a58;

extern s32 data_020636b8;
extern s32 data_0206a9bc;
extern s32 data_020636a0;

extern void Boot(void*);

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

    SystemStatusFlags.unk_02 = FALSE;
    SystemStatusFlags.unk_04 = TRUE;

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

        SystemStatusFlags.vblank = FALSE;
        SystemStatusFlags.reset  = FALSE;
        SystemStatusFlags;

        SystemStatusFlags.unk_07 = TRUE;
        SystemStatusFlags.unk_03 = FALSE;
        SystemStatusFlags.unk_04 = TRUE;
        SystemStatusFlags.unk_10 = FALSE;
        SystemStatusFlags;

        SystemStatusFlags.unk_05 = TRUE;
        SystemStatusFlags;

        SystemStatusFlags.unk_06 = TRUE;

        u32 sp18;
        u32 sp14;
        func_020410ac(&sp18, &sp14);

        if (sp18 == 1) {
            SystemStatusFlags;
            SystemStatusFlags.unk_08 = TRUE;
        } else {
            SystemStatusFlags;
            SystemStatusFlags.unk_08 = FALSE;
        }

        if (sp14 == 1) {
            SystemStatusFlags;
            SystemStatusFlags.unk_09 = TRUE;
        } else {
            SystemStatusFlags;
            SystemStatusFlags.unk_09 = FALSE;
        }

        SystemStatusFlags;
        SystemStatusFlags.unk_07 = TRUE;

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
        MI_CpuFill(0, stdHeap, Mem_GetBlockSize(&gMainHeap, stdHeap));

        Mem_InitializeHeap(&gDebugHeap, stdHeap, 0x80000);

        SndMgr_Init();
        OvlMgr_UnloadOverlay(-1);
        Interrupts_Init();
        Display_Init();
        Interrupts_ForceVBlank();
        DMA_Init(0x100);
        Input_Init(&InputStatus, 8, 1, 2);
        MainOvlDisp_Init();
        MainOvlDisp_Push(OVERLAY_ID_NONE, &Boot, NULL, 0);

        SystemStatusFlags.unk_02 = TRUE;

        DebugOvlDisp_Init();
        OvlDisp_InitUnused2();
        OvlDisp_InitUnused1();
        RNG_SetSeed(0);
        DatMgr_Init(NULL, 0);
        func_0200cef0(0);
        SysControl.frameCount = 0;
        TouchInput_Init();
        func_020415a4();
        func_02025b1c();

        while (SystemStatusFlags.reset == FALSE || SystemStatusFlags.unk_07 == FALSE) {
            func_02004c44(&data_0206a9bc);

            Input_PollState(&InputStatus);
            Input_UpdateRepeat(&InputStatus, INPUT_MASK_ALLBTNS);
            SysControl.buttonState = InputStatus.buttonState;

            func_02007240();
            MainOvlDisp_Run();
            func_02026a94();

            if (SystemStatusFlags.unk_03 != FALSE) {
                func_020218ec();
            }

            // Trigger a soft reset when Start + Select + L + R are pressed
            if ((InputStatus.buttonState.currButtons & INPUT_SOFT_RESET) == INPUT_SOFT_RESET) {
                SystemStatusFlags.reset = TRUE;
            }

            SystemStatusFlags.unk_10 = FALSE;

            u32 var_r0 = SystemStatusFlags.unk_06;
            if (var_r0 != FALSE) {
                if (var_r0 == 1) {
                    if (Input_IsSystemHingeClosed()) {
                        if (SystemStatusFlags.unk_05 != FALSE) {
                            func_02041134(0xC, 0, 0);
                            SystemStatusFlags.unk_10 = TRUE;
                        }
                    } else {
                        u32 sp10;
                        u32 spC;
                        func_020410ac(&sp10, &spC);
                        if (SystemStatusFlags.unk_08 != FALSE && sp10 == 0) {
                            func_02040f34(0, 1);
                        }
                        if (SystemStatusFlags.unk_09 != FALSE && spC == 0) {
                            func_02040f34(1, 1);
                        }
                    }
                }
            } else {
                u32 sp8;
                u32 sp4;
                func_020410ac(&sp8, &sp4);
                if (SystemStatusFlags.unk_08 != FALSE) {
                    if (Input_IsSystemHingeClosed()) {
                        if (sp8 == 1) {
                            func_02040f34(0, 0);
                        }
                    } else if (sp8 == 0) {
                        func_02040f34(0, 1);
                    }
                }
                if (SystemStatusFlags.unk_09 != FALSE) {
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
