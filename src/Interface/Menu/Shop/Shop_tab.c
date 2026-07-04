#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Shop.h"
#include "SndMgr.h"

typedef struct {
    /* 0x000 */ Sprite      unk_000[5];
    /* 0x140 */ s32         unk_140[5];
    /* 0x154 */ ShopObject* shop;
    /* 0x158 */ u16         unk_158;
    /* 0x15A */ u16         unk_15A;
} Shop_tab; // Size: 0x15C

typedef struct {
    /* 0x00 */ s32         unk_00;
    /* 0x04 */ ShopObject* unk_04;
} Shop_tab_Args;

extern void func_ov043_020af42c(Sprite* sprite, s16 frame);
extern u32  func_ov043_020b1830(s16 arg0, s16 arg1);
extern void func_ov043_020b28d4(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

SpriteFrameInfo* func_ov043_020b4bc8(Sprite* arg0, s32 arg1, s32 arg2);
s32              func_ov043_020b4e50(TaskPool* arg0, Task* arg1, void* arg2);
s32              func_ov043_020b4e70(TaskPool* arg0, Task* arg1, void* arg2);
s32              func_ov043_020b50b0(TaskPool* arg0, Task* arg1, void* arg2);
s32              func_ov043_020b50f0(TaskPool* arg0, Task* arg1, void* arg2);
s32              func_ov043_020b511c(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3);

static s16 data_ov043_020cc920[4][5] = {
    {0x30, 0x34, 0x36, 0x3C, 0x3E},
    {0x30, 0x32, 0x3A, 0x00, 0x3E},
    {0x32, 0x38, 0x00, 0x00, 0x3E},
    {0x32, 0x00, 0x00, 0x00, 0x3E},
};

static const s16 data_ov043_020ca5a0[6] = {
    0x1F, 0x50, 0x81, 0xB2, 0xE3, 0x00,
};

static const TaskHandle data_ov043_020ca5ac = {"Tsk_Shop_tab", func_ov043_020b511c, sizeof(Shop_tab)};

static const SpriteAnimation data_ov043_020ca5c8 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x0C00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = func_ov043_020b4bc8,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020ca1b0,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 3,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

void func_ov043_020b4af8(ShopObject* arg0, u32 arg1, u16 arg2) {
    switch (arg1) {
        case 0:
            if (arg2 == 0) {
                arg1 = 8;
            } else if (arg2 == 1) {
                arg1 = 0;
            } else {
                arg1 = 2;
            }
            break;

        case 1:
            arg1 = 3;
            break;

        case 2:
            if (arg2 == 0) {
                arg1 = 9;
            } else {
                arg1 = 4;
            }
            break;

        case 3:
            arg1 = 5;
            break;

        case 4:
            arg1 = 6;
            break;
    }

    func_ov043_020b28d4(&arg0->unk_938, arg0->unk_938, 0, 3, arg1 + 2);
}

// Nonmatching
s32 func_ov043_020b4b80(u32 arg0, u16 arg1, s32 arg2) {
    if (data_ov043_020cc920[arg1][arg0] != 0) {
        if (arg0 != 4) {
            return 1;
        }
        if (arg2 == 1) {
            return 1;
        }
    }

    return 0;
}

SpriteFrameInfo* func_ov043_020b4bc8(Sprite* arg0, s32 arg1, s32 arg2) {
    SpriteFrameInfo* info = NULL;

    switch (arg2) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

            if ((arg0->animData != NULL) && (arg0->frameDataTable != NULL) && (arg0->unk16 >= 0)) {
                temp->unk_04 = *((u16*)arg0->frameDataTable + (arg0->unk16 * 4 + 1));
                temp->unk_08 = (s32)((u16*)arg0->frameDataTable + *((u16*)((u8*)arg0->frameDataTable + (arg0->unk16 * 8))));
            }
            info = temp;
        } break;

        default:
            break;
    }

    return info;
}

// Nonmatching
void func_ov043_020b4c64(Shop_tab* arg0, Sprite* arg1, Shop_tab_Args* arg2) {
    ShopObject* sp0;
    s16         xPos[5];
    const u8*   tabEntry;
    const s16*  frameData;
    s32         canUseFifth;
    u16         currentTab;

    sp0 = arg0->shop;

    SpriteAnimation anim = data_ov043_020ca5c8;

    xPos[0] = data_ov043_020ca5a0[0];
    xPos[1] = data_ov043_020ca5a0[1];
    xPos[2] = data_ov043_020ca5a0[2];
    xPos[3] = data_ov043_020ca5a0[3];
    xPos[4] = data_ov043_020ca5a0[4];

    anim.dataType = arg2->unk_00;

    currentTab  = sp0->unk_848;
    tabEntry    = (const u8*)sp0 + (currentTab * 0x2C);
    canUseFifth = *(s32*)(tabEntry + 0x10);

    if (*(u16*)(tabEntry + 0x00) == 0xFFFF) {
        for (u16 i = 0; i < 5; i++) {
            anim.unk_2A      = 0x30;
            anim.unk_04      = xPos[i];
            anim.unk_06      = 0xBB;
            arg0->unk_140[i] = FALSE;
            _Sprite_Load(&arg1[i], &anim);
        }
    } else {
        frameData = data_ov043_020cc920[*(u16*)(tabEntry + 0x04)];

        for (u16 i = 0; i < 5; i++) {
            if (frameData[i] == 0) {
                anim.unk_2A      = 0x30;
                arg0->unk_140[i] = FALSE;
            } else {
                anim.unk_2A      = frameData[i] + (i != 0);
                arg0->unk_140[i] = TRUE;
                if ((i == 4) && (canUseFifth == 0)) {
                    arg0->unk_140[4] = FALSE;
                }
            }

            anim.unk_04 = xPos[i];
            anim.unk_06 = 0xBB;
            _Sprite_Load(&arg1[i], &anim);
        }
    }

    arg0->unk_158 = sp0->unk_848;
}

s32 func_ov043_020b4e50(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_tab*      temp_r0 = arg1->data;
    Shop_tab_Args* args    = arg2;

    temp_r0->shop = args->unk_04;
    func_ov043_020b4c64(temp_r0, temp_r0->unk_000, args);
    return 1;
}

// Nonmatching
s32 func_ov043_020b4e70(TaskPool* pool, Task* task, void* args) {
    Shop_tab*   tab     = task->data;
    ShopObject* temp_r5 = tab->shop;

    if (temp_r5->unk_836 != 0) {
        for (s32 var_r4 = 0; var_r4 < 5; var_r4++) {
            Sprite_Update(&tab->unk_000[var_r4]);
        }
        return 1;
    }

    const u8* temp_r0 = (const u8*)temp_r5 + (temp_r5->unk_848 * 0x2C);
    u16       temp_r7 = *(u16*)(temp_r0 + 0x04);
    s32       temp_r6 = *(s32*)(temp_r0 + 0x10);

    if (TouchInput_WasTouchPressed()) {
        TouchCoord coord;

        TouchInput_GetCoord(&coord);
        u32 temp_r0_2 = func_ov043_020b1830(coord.x, coord.y);

        if ((temp_r0_2 != -1) && (func_ov043_020b4b80(temp_r0_2, temp_r7, temp_r6) == 1) && (temp_r0_2 != temp_r5->unk_854)) {
            SndMgr_StartPlayingSE(0x11A);
            temp_r5->unk_85E = 0;
            temp_r5->unk_834 |= 0x40;
            func_ov043_020b4af8(temp_r5, temp_r0_2, temp_r7);
            temp_r5->unk_854 = temp_r0_2;
        }
    }

    if (temp_r5->unk_848 != tab->unk_158) {
        if (data_ov043_020cc920[temp_r7][temp_r5->unk_854] == 0) {
            temp_r5->unk_854 = 0;
        }

        if ((temp_r5->unk_854 == 4) && (temp_r6 == 0)) {
            temp_r5->unk_854 = 0;
        }

        temp_r5->unk_85E = 0;
        temp_r5->unk_834 |= 0x40;
        func_ov043_020b4af8(temp_r5, temp_r5->unk_854, temp_r7);
    }

    const s16* temp_r8 = data_ov043_020cc920[temp_r7];
    s32        var_r10 = 0;
    Sprite*    var_r7  = &tab->unk_000[0];
    tab->unk_158       = temp_r5->unk_848;

    do {
        if (var_r10 == 4) {
            if (temp_r6 == 0) {
                tab->unk_140[4] = 0;
            } else {
                tab->unk_140[4] = 1;
            }
        } else if (temp_r8[var_r10] == 0) {
            tab->unk_140[var_r10] = 0;
        } else {
            tab->unk_140[var_r10] = 1;
        }

        if (var_r10 == temp_r5->unk_854) {
            func_ov043_020af42c(var_r7, temp_r8[var_r10]);
        } else {
            func_ov043_020af42c(var_r7, (s16)(temp_r8[var_r10] + 1));
        }

        var_r10 += 1;
        var_r7 += 1;
    } while (var_r10 < 5);

    s32 var_r4_2 = 0;
    do {
        Sprite_Update(&tab->unk_000[var_r4_2]);
        var_r4_2 += 1;
    } while (var_r4_2 < 5);

    return 1;
}

s32 func_ov043_020b50b0(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_tab* temp_r4 = arg1->data;
    s32       var_r5;

    for (var_r5 = 0; var_r5 < 5; var_r5++) {
        if (temp_r4->unk_140[var_r5] != 0) {
            Sprite_RenderFrame(&temp_r4->unk_000[var_r5]);
        }
    }

    return 1;
}

s32 func_ov043_020b50f0(TaskPool* arg0, Task* arg1, void* arg2) {
    Shop_tab* var_r5 = arg1->data;
    s32       var_r4;

    for (var_r4 = 0; var_r4 < 5; var_r4++) {
        Sprite_Release(&var_r5->unk_000[var_r4]);
    }

    return 1;
}

s32 func_ov043_020b511c(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    TaskStages stages = {
        .initialize = func_ov043_020b4e50,
        .update     = func_ov043_020b4e70,
        .render     = func_ov043_020b50b0,
        .cleanup    = func_ov043_020b50f0,
    };
    return stages.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov043_020b5164(TaskPool* arg0, s32 arg1, s32 arg2) {
    Shop_tab_Args args;

    args.unk_00 = arg1;
    args.unk_04 = (ShopObject*)arg2;
    return EasyTask_CreateTask(arg0, &data_ov043_020ca5ac, NULL, 0, NULL, &args);
}
