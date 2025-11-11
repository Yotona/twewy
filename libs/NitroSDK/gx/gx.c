#include <NitroSDK/gx/gx.h>
#include <NitroSDK/gx/gx_state.h>
#include <registers.h>

static u16 IsDisplayOn   = TRUE;
u32        data_0206625c = 3;

static u16 DisplayMode = 0;
u16        GX_LockID   = 0;

void GX_Init(void) {
    REG_POWER_CNT |= (1 << 15);
    REG_POWER_CNT = (REG_POWER_CNT & ~526) | 526;
    REG_POWER_CNT |= 1;
    GX_InitGxState();

    s32 lockId;
    while (GX_LockID == 0) {
        lockId = OS_GetLockID();
        if (lockId == -3) {
            OS_WaitForever();
        }
        GX_LockID = lockId;
    }

    REG_DISPSTAT = 0;
    REG_DISPCNT  = 0;

    if (data_0206625c != -1) {
        func_0203aafc(data_0206625c, &REG_BG0CNT, 0, 0x60);
        REG_MASTER_BRIGHT = 0;
        func_0203aafc(data_0206625c, &REG_DISPCNT_SUB, 0, 0x70);
    } else {
        func_0203b28c(0, &REG_BG0CNT, 0x60);
        REG_MASTER_BRIGHT = 0;
        func_0203b28c(0, &REG_DISPCNT_SUB, 0x70);
    }

    REG_BG2PA     = 0x100;
    REG_BG2PD     = 0x100;
    REG_BG3PA     = 0x100;
    REG_BG3PD     = 0x100;
    REG_BG2PA_SUB = 0x100;
    REG_BG2PD_SUB = 0x100;
    REG_BG3PA_SUB = 0x100;
    REG_BG3PD_SUB = 0x100;
}

s32 GX_HBlankIntr(BOOL enabled) {
    s32 prev = REG_DISPSTAT & 0x10;
    if (enabled) {
        REG_DISPSTAT |= 0x10;
    } else {
        REG_DISPSTAT &= ~0x10;
    }
    return prev;
}

s32 GX_VBlankIntr(BOOL enabled) {
    s32 prev = REG_DISPSTAT & 0x8;
    if (enabled) {
        REG_DISPSTAT |= 0x8;
    } else {
        REG_DISPSTAT &= ~0x8;
    }
    return prev;
}

void GX_DispOff(void) {
    u32 prev    = REG_DISPCNT;
    IsDisplayOn = FALSE;
    DisplayMode = (prev & 0x30000) >> 16;
    REG_DISPCNT = prev & ~0x30000;
}

void GX_DispOn(void) {
    IsDisplayOn = TRUE;
    if (DisplayMode != 0) {
        REG_DISPCNT = (REG_DISPCNT & ~0x30000) | (DisplayMode << 16);
    } else {
        REG_DISPCNT |= 0x10000;
    }
}

void GX_SetGraphicsMode(GXDisplayMode displayMode, GXBGMode bgMode, GX2D3D dimension) {
    u32 cnt = REG_DISPCNT;

    DisplayMode = displayMode;
    if (IsDisplayOn == 0) {
        displayMode = GX_DISPMODE_OFF;
    }

    cnt &= ~(0x7 | 0x8 | 0x30000 | 0xC0000);

    REG_DISPCNT = (cnt | (displayMode << 16) | (bgMode) | (dimension << 3));
    if (DisplayMode == 0) {
        IsDisplayOn = FALSE;
    }
}

void GXs_SetGraphicsMode(GXBGMode bgMode) {
    REG_DISPCNT_SUB = (REG_DISPCNT_SUB & ~0x7) | bgMode;
}

void GXx_SetMasterBrightness(u16* arg0, s32 arg1) {
    if (arg1 == 0) {
        *arg0 = 0;
    } else if (arg1 > 0) {
        *arg0 = arg1 | 0x4000;
    } else {
        *arg0 = -arg1 | 0x8000;
    }
}

s32 GXx_GetMasterBrightness(vu16* arg0) {
    u16 mode = *arg0 & 0xC000;

    if (mode == 0) {
        return 0;
    } else if (mode == 0x4000) {
        return *arg0 & 0x1F;
    } else if (mode == 0x8000) {
        return -(*arg0 & 0x1F);
    } else {
        return 0;
    }
}