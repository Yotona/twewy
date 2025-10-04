// #include "OverlayManager.h"
#include "Interrupts.h"
#include "System.h"
#include "common_data.h"
#include "game.h"
#include <NitroSDK/os/cache.h>
#include <NitroSDK/os/interrupt.h>
#include <NitroSDK/os/valarm.h>
#include <NitroSDK/util.h>
#include <bios.h>
#include <registers.h>

IRQCallback Interrupts_RegisterVBlankCallback(IRQCallback callback, s32 param_2);

static void HandleVBlank(void);
static void HandleHBlank(void);

BOOL        Interrupts_IsVBlankInterrupted(void);
IRQCallback Interrupts_SaveVBlankCallback(IRQCallback);
IRQCallback Interrupts_SaveHBlankCallback(IRQCallback callback);

void Interrupts_Init(void) {
    CRITICAL_SECTION_ENTER();

    Interrupts_RegisterVBlankCallback(NULL, TRUE);
    OS_SetIRQCallback(IRQ_VBLANK, HandleVBlank);

    Interrupts_RegisterHBlankCallback(NULL, FALSE);
    OS_SetIRQCallback(IRQ_HBLANK, HandleHBlank);

    if (OS_IsVAlarmSystemActive() == TRUE) {
        OS_StopAllVAlarms();
        OS_VAlarmSystemHalt();
    } else {
        OS_VAlarmSystemInit();
    }

    CRITICAL_SECTION_LEAVE();
}

void Interrupts_HandleVBlank(void) {
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
        func_02006380();
        func_020019ac();
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
    }
}

void Interrupts_TriggerHBlank(void) {
    BIOS_INTERRUPT_IRQCHECK |= 2;
}

void Interrupts_ForceVBlank(void) {
    System_ForceFlag(SYSFLAG_UNKNOWN_0);
    if (Interrupts_IsVBlankInterrupted() == TRUE) {
        func_02021938();
        return;
    }
    SysControl.vBlankCallback();
    System_ClearFlag(SYSFLAG_UNKNOWN_0);
}

static void HandleVBlank(void) {
    func_0202190c();
    if (System_CheckFlag(SYSFLAG_UNKNOWN_0)) {
        OS_DisableInterrupts(IRQ_HBLANK);
        func_0203538c(0);
        OS_SetIRQCallback(IRQ_HBLANK, SysControl.hBlankCallback);
        if (SysControl.hBlankCallback != Interrupts_TriggerHBlank) {
            OS_EnableInterrupts(IRQ_HBLANK);
            func_0203538c(1);
        }
    } else {
        SystemStatusFlags = SystemStatusFlags & ~8 | (u32)((1 - ((SystemStatusFlags << 0x1c) >> 0x1f)) * -0x80000000) >> 0x1c;
    }

    SysControl.vBlankCallback();
    SysControl.unk_20++;
    System_ClearFlag(SYSFLAG_UNKNOWN_0);
    BIOS_INTERRUPT_IRQCHECK |= 1;
}

IRQCallback Interrupts_RegisterVBlankCallback(IRQCallback callback, BOOL enable) {
    if (callback == NULL) {
        callback = Interrupts_HandleVBlank;
    }

    IRQCallback prevCallback = Interrupts_SaveVBlankCallback(callback);
    if (enable == TRUE) {
        OS_EnableInterrupts(IRQ_VBLANK);
        func_020353c0(1);
    } else {
        OS_DisableInterrupts(IRQ_VBLANK);
        func_020353c0(0);
    }
    return prevCallback;
}

IRQCallback Interrupts_SaveVBlankCallback(IRQCallback callback) {
    IRQCallback prevCallback = SysControl.vBlankCallback;
    if (callback == NULL) {
        callback = Interrupts_HandleVBlank;
    }
    SysControl.vBlankCallback = callback;
    return prevCallback;
}

BOOL Interrupts_IsVBlankInterrupted(void) {
    BOOL val = FALSE;
    if ((REG_IME & 1) && (REG_IE & 1) && (REG_DISPSTAT & 8) && func_02041458() == 1) {
        val = TRUE;
    }
    return val;
}

static void HandleHBlank(void) {
    SysControl.hBlankCallback();
    BIOS_INTERRUPT_IRQCHECK |= 2;
}

IRQCallback Interrupts_RegisterHBlankCallback(IRQCallback callback, BOOL enable) {
    IRQCallback prevCallback = Interrupts_SaveHBlankCallback(callback);
    if (enable == TRUE) {
        OS_EnableInterrupts(IRQ_HBLANK);
        func_0203538c(1);
    } else {
        OS_DisableInterrupts(IRQ_HBLANK);
        func_0203538c(0);
    }
    return prevCallback;
}

IRQCallback Interrupts_SaveHBlankCallback(IRQCallback callback) {
    IRQCallback prevCallback = SysControl.hBlankCallback;
    if (callback == NULL) {
        callback = Interrupts_TriggerHBlank;
    }
    SysControl.hBlankCallback = callback;
    return prevCallback;
}

IRQCallback Interrupts_ResetHBlankCallback(void) {
    return Interrupts_SaveHBlankCallback(Interrupts_TriggerHBlank);
}

BOOL Interrupts_IsHBlankInterrupted(void) {
    BOOL val = FALSE;
    if ((REG_IME & 1) && (REG_IE & 2) && (REG_DISPSTAT & 0x10) && func_02041458() == 1) {
        val = TRUE;
    }
    return val;
}
