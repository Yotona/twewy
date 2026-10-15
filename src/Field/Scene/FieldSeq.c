#include "Engine/Overlay/OverlayDispatcher.h"
#include "Player/Inventory.h"
#include "Save.h"
#include <nitro/types.h>

extern void func_ov030_020ae848(void*);
extern void (*data_ov030_020d95dc[])(void*);
extern s32  func_ov030_020af320(s32 area);
extern void func_ov030_020b7c58(s32, s32*, s32*, s32*);
extern s32  func_ov030_020b7b80(void);
extern s32  func_ov030_020b7ba8(void);

void func_ov030_020ae92c(void* arg) {
    OverlayProcessStage stage = MainOvlDisp_GetProcessStage();

    if (stage == PROCESS_STAGE_EXIT) {
        func_ov030_020ae848(arg);
    } else {
        data_ov030_020d95dc[stage](arg);
    }
}

void func_ov030_020ae96c(s32 area) {
    gSaveData.unk_2458 = area;
    func_ov030_020b7c58(1, &gSaveData.unk_248C, &gSaveData.unk_2490, &gSaveData.unk_2494);

    gSaveData.fieldPos.x = func_ov030_020b7b80() << 12;
    gSaveData.fieldPos.y = func_ov030_020b7ba8() << 12;

    if (gSaveData.unk_2458 == 9) {
        gSaveData.fieldPos.x = 0x104000;
        gSaveData.fieldPos.y = 0x13A000;
    } else if (gSaveData.unk_2458 == 7) {
        gSaveData.fieldPos.x = 0x16C000;
        gSaveData.fieldPos.y = 0xF4000;
    } else if (gSaveData.unk_2458 == 0) {
        gSaveData.fieldPos.x = 0x207000;
        gSaveData.fieldPos.y = 0xF3000;
    } else if (gSaveData.unk_2458 == 13) {
        gSaveData.fieldPos.x = 0x154000;
        gSaveData.fieldPos.y = 0x103000;
    } else if (gSaveData.unk_2458 == 41) {
        gSaveData.fieldPos.x = 0x9F000;
        gSaveData.fieldPos.y = 0xB7000;
    } else if (gSaveData.unk_2458 == 42) {
        gSaveData.fieldPos.x = 0x1E000;
        gSaveData.fieldPos.y = 0xF6000;
    } else if (gSaveData.unk_2458 == 19) {
        gSaveData.fieldPos.y = 0x11B000;
    }

    gSaveData.unk_2498 = 0;
    gSaveData.unk_24B8 = 0;
    gSaveData.unk_24B4 = 0;
}

void func_ov030_020aeaa0(s32 area, s32 flags) {
    func_020235cc(func_ov030_020af320(area), flags);
}

void func_ov030_020aeab8(s32 area, s32 flags) {
    func_020235fc(func_ov030_020af320(area), flags);
}

void func_ov030_020aead0(s32 flags) {
    func_02023630(flags);
}

s32 func_ov030_020aeadc(s32 area, s32 flags) {
    return func_0202366c(func_ov030_020af320(area), flags);
}

s32 func_ov030_020aeaf4(void) {
    s16 area = 0x24;

    do {
        if (func_ov030_020aeadc(area, 1) != 0) {
            return area;
        }
        area--;
    } while (area != 0x19);
    return area;
}
