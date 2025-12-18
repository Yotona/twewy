#include "Interrupts.h"
#include "System.h"
#include "common_data.h"
#include <NitroSDK/gx/gx.h>
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
    ENTER_CRITICAL_SECTION();

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

    LEAVE_CRITICAL_SECTION();
}

void Interrupts_FlushCache(void) {
    if (SystemStatusFlags.vblank != FALSE) {
        Display_Commit();
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
    }
}

void Interrupts_TriggerHBlank(void) {
    BIOS_INTERRUPT_IRQCHECK |= 2;
}

void Interrupts_ForceVBlank(void) {
    SystemStatusFlags.vblank = TRUE;
    if (Interrupts_IsVBlankInterrupted() == TRUE) {
        func_02021938();
        return;
    }
    SysControl.vBlankCallback();
    SystemStatusFlags.vblank = FALSE;
}

static void HandleVBlank(void) {
    func_0202190c();
    if (SystemStatusFlags.vblank != FALSE) {
        OS_DisableInterrupts(IRQ_HBLANK);
        GX_HBlankIntr(FALSE);
        OS_SetIRQCallback(IRQ_HBLANK, SysControl.hBlankCallback);
        if (SysControl.hBlankCallback != Interrupts_TriggerHBlank) {
            OS_EnableInterrupts(IRQ_HBLANK);
            GX_HBlankIntr(TRUE);
        }
    } else {
        u32 val                  = 1 + SystemStatusFlags.unk_03;
        SystemStatusFlags.unk_03 = val;
    }

    SysControl.vBlankCallback();
    SysControl.frameCount++;
    SystemStatusFlags.vblank = FALSE;
    BIOS_INTERRUPT_IRQCHECK |= 1;
}

IRQCallback Interrupts_RegisterVBlankCallback(IRQCallback callback, BOOL enable) {
    if (callback == NULL) {
        callback = Interrupts_FlushCache;
    }

    IRQCallback prevCallback = Interrupts_SaveVBlankCallback(callback);
    if (enable == TRUE) {
        OS_EnableInterrupts(IRQ_VBLANK);
        GX_VBlankIntr(TRUE);
    } else {
        OS_DisableInterrupts(IRQ_VBLANK);
        GX_VBlankIntr(FALSE);
    }
    return prevCallback;
}

IRQCallback Interrupts_SaveVBlankCallback(IRQCallback callback) {
    IRQCallback prevCallback = SysControl.vBlankCallback;
    if (callback == NULL) {
        callback = Interrupts_FlushCache;
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
        GX_HBlankIntr(TRUE);
    } else {
        OS_DisableInterrupts(IRQ_HBLANK);
        GX_HBlankIntr(FALSE);
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
