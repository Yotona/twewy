#include "Field/Events.h"
#include "Save.h"

extern void func_ov030_020b7c58(s32, s32*, s32*, s32*);
extern void func_ov030_020b7e94(s32*, s32*, s32*, s32*, s32*, s32, s32*, s32*);
extern s32  func_ov030_020aeaf4(void);

void func_ov030_020b87bc(FieldExit* exit) {
    s32 out0;
    s32 out1;
    s32 out2;
    s32 out3;
    s32 direction;
    s16 entry = exit->destArea;

    if (entry == 0x25 && gSaveData.unk_2458 == 0x18) {
        gSaveData.unk_2458 = func_ov030_020aeaf4();
    } else {
        gSaveData.unk_2458 = entry;
    }
    func_ov030_020b7e94(&out0, &out1, &out2, &out3, &direction, exit->destRegion, &gSaveData.fieldPos.x,
                        &gSaveData.fieldPos.y);
    if (direction == 2) {
        gSaveData.fieldPos.x -= 0x10000;
    } else if (direction == 6) {
        gSaveData.fieldPos.x += 0x10000;
    } else if (direction == 0) {
        gSaveData.fieldPos.y -= 0x10000;
    } else if (direction == 4) {
        gSaveData.fieldPos.y += 0x10000;
    }
    if (gSaveData.unk_2458 != 0) {
        return;
    }
    if (gSaveData.fieldPos.x >= 0x80000) {
        return;
    }
    gSaveData.fieldPos.x = 0x18000;
    gSaveData.fieldPos.y = 0x100000;
}

void func_ov030_020b88e8(FieldExit* exit) {
    func_ov030_020b87bc(exit);
    func_ov030_020b7c58(gSaveData.unk_2458, &gSaveData.unk_248C, &gSaveData.unk_2490, &gSaveData.unk_2494);
}
