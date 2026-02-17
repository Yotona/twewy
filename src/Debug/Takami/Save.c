#include "Debug/Takami/Save.h"
#include "DatMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "OverlayDispatcher.h"
#include "System.h"
#include "TouchInput.h"
#include "common_data.h"
#include <NitroSDK/fx.h>

s32 func_ov043_020c61b0(TaskPool* pool, s32 arg1, void* arg2);

void func_ov043_020bf250(SaveState* state) {
    // Not yet implemented
}

void func_ov043_020bf2e0(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bf4d4(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bf798(s32 arg0, s32 arg1) {
    // Not yet implemented
}

void func_ov043_020bf874(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bf940(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bfa04(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bfc24(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bfc78(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bfd70(SaveState* state) {
    if (state->unk_21698 > 0) {
        state->unk_21698--;
    } else {
        SndMgr_StartPlayingSE(283);
        SndMgr_StartPlayingSE(279);
        DebugOvlDisp_Pop();
    }
}

void func_ov043_020bfdb0(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bfef4(void* arg0) {
    // Not yet implemented
}

void func_ov043_020bff8c(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c0048(SaveState* state) {
    // Not yet implemented
}

void func_ov043_020c0248(SaveState* state) {
    // Not yet implemented
}

void func_ov043_020c0468(SaveState* state) {
    // Not yet implemented
}

static const OverlayProcess OvlProc_Save = {
    .init = func_ov043_020c0048,
    .main = func_ov043_020c0248,
    .exit = func_ov043_020c0468,
};

void func_ov043_020c04f0(SaveState* state) {
    s32 stage = MainOvlDisp_GetProcessStage();
    if (stage == PROCESS_STAGE_EXIT) {
        func_ov043_020c0468(state);
    } else {
        OvlProc_Save.funcs[stage](state);
    }
}

void func_ov043_020c0530(void) {
    // Not yet implemented
}

void func_ov043_020c09e8(void) {
    if (SystemStatusFlags.vblank) {
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
        func_02001b44(2, 0, &data_020672ec, 0x400);
    }
}

void func_ov043_020c0ad0(void) {
    func_ov043_020c0530();
    Interrupts_RegisterVBlankCallback(func_ov043_020c09e8, 1);
}

void func_ov043_020c0aec(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

s32 func_ov043_020c0b00(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s16 arg4, s16 arg5) {
    if ((arg0 >= arg2) && (arg0 <= (arg2 + arg4)) && (arg1 >= arg3) && (arg1 <= (arg3 + arg5))) {
        return 1;
    }
    return 0;
}

void func_ov043_020c0b3c(void* arg0, s16 arg1) {
    // Not yet implemented
}

void func_ov043_020c0b88(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c0bac(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c0bd0(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c0bf4(void* arg0) {
    // Not yet implemented
}

u8 func_ov043_020c0c18(void) {
    u8 val = func_02023010(680) + 2;
    if (val > 6) {
        val = 6;
    }
    return val;
}

u16 func_ov043_020c0c38(u16 arg0, u16 arg1, u16 arg2) {
    return (u16)(arg2 + (arg0 + arg1));
}

u16 func_ov043_020c0c4c(void) {
    return func_02023480();
}

u16 func_ov043_020c0c60(u32 arg0, u8 arg1, u8 arg2) {
    if (arg0 == arg1) {
        return 0xFFFFU;
    }
    return func_02023480((u8)(arg0 + 1), arg2);
}

void func_ov043_020c0c90(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c1254(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c12bc(s8 arg1) {
    // Not yet implemented
}

s16 func_ov043_020c1530(s32 arg0, s32 arg1) {
    // Not yet implemented
}

s32 func_ov043_020c15d8(s32 arg0, s32 arg1) {
    // Not yet implemented
}

s16 func_ov043_020c1638(s32 arg0, s32 arg1) {
    // Not yet implemented
}

s16 func_ov043_020c16d8(s32 arg0, s32 arg1) {
    // Not yet implemented
}

void func_ov043_020c17bc(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    // Not yet implemented
}

void func_ov043_020c19bc(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7) {
    // Not yet implemented
}

void func_ov043_020c1be4(void* arg0, void* arg1, s32 arg2, s32 arg3, s32 arg4) {
    // Not yet implemented
}

void func_ov043_020c1c90(void* arg0, s32 arg1) {
    // Not yet implemented
}

void func_ov043_020c1d18(void* arg0, s32 arg1) {
    // Not yet implemented
}

void func_ov043_020c1d50(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5) {
    // Not yet implemented
}

void func_ov043_020c1d98(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c1dbc(void* arg0) {
    // Not yet implemented
}

void func_ov043_020c1f98(void) {
    return;
}

void func_ov043_020c1f9c(s32 arg0) {
    func_ov043_020c1d18(arg0 + 0x220, 0);
    func_ov043_020c1c90(arg0 + 0x23C, 0);
    func_ov043_020c1c90(arg0 + 0x258, 0);
    func_ov043_020c1c90(arg0 + 0x274, 0);
    func_ov043_020c1d18(arg0 + 0x1CC, 1);
    func_ov043_020c1d18(arg0 + 0x1E8, 1);
    func_ov043_020c1c90(arg0 + 0x1B0, 1);
    func_ov043_020c1c90(arg0 + 0x204, 1);
}

void* func_ov043_020c2008(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c20a4(void* arg0, s32* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c214c(void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c216c(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2210(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c223c(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c2268(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c22b0(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void* func_ov043_020c22e4(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c2380(void* arg0, s32* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2404(void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c242c(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2440(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2454(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c2468(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c24b0(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void* func_ov043_020c24e4(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c2580(void* arg0, void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c26c0(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c26e0(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2748(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2790(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c27c4(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c280c(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void* func_ov043_020c2840(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c28dc(void* arg0, void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c2aa8(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c2ac8(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2afc(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2b44(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c2b78(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c2bc0(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void* func_ov043_020c2bf4(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c2c90(void* arg0, s32* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2d14(void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c2d44(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2dac(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c2dc0(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c2dd4(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c2e1c(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void* func_ov043_020c2e50(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c2eec(void* arg0, void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c2ff0(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c3034(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c31a4(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c31e4(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c3210(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c3258(TaskPool* arg0, s32 arg1, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c328c(void** arg0) {
    s32 var_r5;
    s32 var_r6;

    var_r6 = arg0 + 1;
    var_r5 = 0;
    do {
        func_ov031_0210aa94(var_r6);
        func_ov031_0210ab34(var_r6, 0xE);
        var_r5 += 1;
        var_r6 += 0x7C;
    } while (var_r5 < 0xA);
}

void func_ov043_020c32c4(void** arg0, s32 arg1, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c35d0(void** arg0, s32 arg1, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c3640(void** arg0) {
    // Not yet implemented
}

s32 func_ov043_020c391c(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c3944(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c39a8(void) {
    return 1;
}

s32 func_ov043_020c39b0(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c39e0(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c3a28(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void* func_ov043_020c3a5c(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c3af8(void* arg0, void* arg1, s32* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c3bf8(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c3c18(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c3d10(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c3d50(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c3d7c(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c3dc4(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c3df8(u32 arg0) {
    if (arg0 == 10000) {
        return 0;
    }
    if (arg0 >= 7000) {
        return 1;
    }
    if (arg0 >= 5000) {
        return 2;
    }
    if (arg0 >= 3000) {
        return 3;
    }
    if (arg0 >= 2000) {
        return 4;
    }
    if (arg0 >= 1000) {
        return 5;
    }
    if (arg0 >= 500) {
        return 6;
    }
    if (arg0 >= 300) {
        return 7;
    }
    if (arg0 >= 200) {
        return 8;
    }
    if (arg0 >= 100) {
        return 9;
    }
    return 10;
}

s32 func_ov043_020c3e8c(u32 arg0) {
    if (arg0 >= 10000) {
        return 0;
    }
    if (arg0 >= 7000) {
        return 1;
    }
    if (arg0 >= 5000) {
        return 1;
    }
    if (arg0 >= 3000) {
        return 1;
    }
    if (arg0 >= 2000) {
        return 2;
    }
    if (arg0 >= 1000) {
        return 2;
    }
    if (arg0 >= 500) {
        return 3;
    }
    if (arg0 >= 300) {
        return 3;
    }
    if (arg0 >= 200) {
        return 4;
    }
    return 5;
}

s32 func_ov043_020c3f14(u32 arg0) {
    if (arg0 >= 10000) {
        return 0;
    }
    if (arg0 >= 5000) {
        return 1;
    }
    if (arg0 >= 3000) {
        return 2;
    }
    if (arg0 >= 2000) {
        return 3;
    }
    if (arg0 >= 1000) {
        return 4;
    }
    return 5;
}

// Nonmatching
void func_ov043_020c3f64(u32 arg0, u32* arg1) {
    arg0 = (arg0 * 1000) / 0x60;

    arg1[0] = arg0 / 10;
    arg1[1] = arg0 % 10;
}

s32 func_ov043_020c3fb0(u32 arg0) {
    if (arg0 >= 1000) {
        return 0;
    }
    if (arg0 >= 800) {
        return 1;
    }
    if (arg0 >= 600) {
        return 2;
    }
    if (arg0 >= 400) {
        return 3;
    }
    if (arg0 >= 200) {
        return 4;
    }
    return 5;
}

void func_ov043_020c3ff0(u16 arg0, void** arg1) {
    // Not yet implemented
}

s32 func_ov043_020c4044(u32 arg0) {
    if (arg0 >= 1000) {
        return 0;
    }
    if (arg0 >= 800) {
        return 1;
    }
    if (arg0 >= 600) {
        return 2;
    }
    if (arg0 >= 400) {
        return 3;
    }
    if (arg0 >= 200) {
        return 4;
    }
    return 5;
}

void func_ov043_020c4084(u16 arg0, void** arg1) {
    // Not yet implemented
}

s32 func_ov043_020c40d0(u32 arg0) {
    if (arg0 >= 1000) {
        return 0;
    }
    if (arg0 >= 800) {
        return 1;
    }
    if (arg0 >= 600) {
        return 2;
    }
    if (arg0 >= 400) {
        return 3;
    }
    if (arg0 >= 200) {
        return 4;
    }
    return 5;
}

s32 func_ov043_020c4110(u32 arg0, void** arg1) {
    // Not yet implemented
}

s32 func_ov043_020c41ec(u32 arg0) {
    if (arg0 == 0) {
        return -1;
    }

    s32 temp_r0 = FX_Divide(arg0 << 0xC, 0x11490000);

    if (temp_r0 < 0x1000) {
        return 0;
    }
    if (temp_r0 < 0x1400) {
        return 1;
    }
    if (temp_r0 < 0x1800) {
        return 2;
    }
    if (temp_r0 < 0x1C00) {
        return 3;
    }
    if (temp_r0 < 0x2000) {
        return 4;
    }
    return 5;
}

void func_ov043_020c424c(void** arg0) {
    s32 var_r5;
    s32 var_r6;

    var_r6 = arg0 + 1;
    var_r5 = 0;
    do {
        func_ov031_0210aa94(var_r6);
        func_ov031_0210ab34(var_r6, 14);
        var_r5 += 1;
        var_r6 += 0x7C;
    } while (var_r5 < 22);
}

void func_ov043_020c4284(void** arg0) {
    // Not yet implemented
}

void func_ov043_020c4b68(void** arg0) {
    // Not yet implemented
}

s32 func_ov043_020c4d08(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c4d30(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c4dac(void) {
    return 1;
}

s32 func_ov043_020c4db4(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c4de4(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c4e2c(TaskPool* arg0, s32 arg1, void* arg2) {
    // Not yet implemented
}

void func_ov043_020c4e60(void* arg1, s32* arg2, s32 arg3) {
    // Not yet implemented
}

void* func_ov043_020c4fac(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c504c(void* arg0, void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c5208(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c5230(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c5264(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c52a8(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c52dc(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c5324(TaskPool* arg0, s32 arg1, u16 arg2, void* arg3) {
    // Not yet implemented
}

void func_ov043_020c538c(TaskPool* arg0, u32 arg1) {
    Sprite* sprite = EasyTask_GetTaskData(arg0, arg1) + 0x44;

    Sprite_Release(sprite);
}

void func_ov043_020c53a0(void* arg1, s32* arg2, s32 arg3) {
    // Not yet implemented
}

void* func_ov043_020c54ec(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c558c(void* arg0, void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c5ae8(void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c5b10(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c5b44(void* arg1) {
    // Not yet implemented
}

s32 func_ov043_020c5b88(void* arg1) {
    // Not yet implemented
}

void func_ov043_020c5bbc(TaskPool* arg0, Task* arg1, s32 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c5c04(TaskPool* arg0, s32 arg1, u16 arg2, s32 arg3) {
    // Not yet implemented
}

void func_ov043_020c5c98(TaskPool* pool, u32 taskId) {
    Sprite* sprite = EasyTask_GetTaskData(pool, taskId) + 0x1C4;

    Sprite_Release(sprite);
}

void* func_ov043_020c5cac(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c5d48(void* arg0, void* arg1, void* arg2) {
    // Not yet implemented
}

s32 func_ov043_020c5e38(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU    = task->data;
    SlotCoverUArgs* slotCoverArgs = args;

    slotCoverU->unk_44 = slotCoverArgs->unk_04;
    func_ov043_020c5d48(slotCoverU, slotCoverU, slotCoverArgs);
    return 1;
}

s32 func_ov043_020c5e58(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU = task->data;

    Sprite_Update(&slotCoverU->sprite);
    return 1;
}

s32 func_ov043_020c5e6c(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU = task->data;

    if (slotCoverU->unk_40 != 0) {
        Sprite_RenderFrame(&slotCoverU->sprite);
    }
    return 1;
}

s32 func_ov043_020c5e8c(TaskPool* pool, Task* task, void* args) {
    SaveSlotCoverU* slotCoverU = task->data;

    Sprite_Release(&slotCoverU->sprite);
    return 1;
}

s32 func_ov043_020c5ea0(TaskPool* pool, Task* task, void* args, s32 method) {
    const struct DispatchFuncs {
        s32 (*funcs[4])(TaskPool* pool, Task* task, void* args);
    } SlotCoverUFuncs = {
        func_ov043_020c5e38,
        func_ov043_020c5e58,
        func_ov043_020c5e6c,
        func_ov043_020c5e8c,
    };

    return SlotCoverUFuncs.funcs[method](pool, task, args);
}

void func_ov043_020c5ee8(TaskPool* pool, s32 arg1, u16 arg2, void* arg3) {
    static const TaskHandle data_ov043_020cb3bc = {"Tsk_Save_slotCoverU", func_ov043_020c5ea0, sizeof(SaveSlotCoverU)};

    SlotCoverUArgs args;
    args.unk_00 = arg1;
    args.unk_08 = arg2;
    args.unk_04 = arg3;
    EasyTask_CreateTask(pool, &data_ov043_020cb3bc, NULL, 0, NULL, &args);
}

void* func_ov043_020c5f20(void* arg0, s32 arg2) {
    // Not yet implemented
}

void func_ov043_020c5fbc(SaveHelpCurU* helpCurU, HelpCurUArgs* args) {
    // Not yet implemented
}

s32 func_ov043_020c6064(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;
    HelpCurUArgs* curUArgs = args;

    helpCurU->unk_88 = curUArgs->unk_04;
    func_ov043_020c5fbc(helpCurU, curUArgs);
    return 1;
}

s32 func_ov043_020c6084(TaskPool* pool, Task* task, void* args) {
    // Not yet implemented
}

s32 func_ov043_020c60f8(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;
    for (u16 i = 0; i < 2; i++) {
        if (helpCurU->unk_80[i] != 0) {
            Sprite_RenderFrame(&helpCurU->sprites[i]);
        }
    }
    return 1;
}

s32 func_ov043_020c6138(TaskPool* pool, Task* task, void* args) {
    SaveHelpCurU* helpCurU = task->data;
    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&helpCurU->sprites[i]);
    }
    return 1;
}

s32 func_ov043_020c6168(TaskPool* pool, Task* task, void* args, s32 method) {
    const struct DispatchFuncs {
        s32 (*funcs[4])(TaskPool* pool, Task* task, void* args);
    } HelpCurUFuncs = {
        func_ov043_020c6064,
        func_ov043_020c6084,
        func_ov043_020c60f8,
        func_ov043_020c6138,
    };

    return HelpCurUFuncs.funcs[method](pool, task, args);
}

s32 func_ov043_020c61b0(TaskPool* pool, s32 arg1, void* arg2) {
    static const TaskHandle data_ov043_020cb41c = {"Tsk_Save_helpCurU", func_ov043_020c6168, sizeof(SaveHelpCurU)};

    HelpCurUArgs args;
    args.unk_00 = arg1;
    args.unk_04 = arg2;
    return EasyTask_CreateTask(pool, &data_ov043_020cb41c, NULL, 0, NULL, &args);
}
