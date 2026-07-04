#include "Interface/Menu/Shop.h"

#include "Engine/IO/TouchInput.h"
#include "SndMgr.h"

typedef struct {
    /* 0x00 */ Sprite      sprites[3];
    /* 0xC0 */ char        unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ ShopObject* shop;
    /* 0xC8 */ u16         prevValue;
    /* 0xCA */ u16         unk_C6;
    /* 0xCC */ u32         unk_C8;
} Shop_sbar; // Size: 0xCC

typedef struct {
    /* 0x00 */ s32         dataType;
    /* 0x04 */ ShopObject* shop;
} Shop_sbar_Args;

s32 FX_Divide(s32, s32);
s32 func_ov043_020b1b14(s16, s16);
s32 func_ov043_020b1b74(s16, s16, s16, s16);
s32 func_ov043_020b1bd4(s16, s16);

static SpriteFrameInfo* Shop_sbar_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode);
static s32              Shop_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Shop_sbar = {"Tsk_Shop_sbar", Shop_sbar_RunTask, 0xCC};

static const SpriteAnimation data_ov043_020ca4e4 = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0,
    .unk_04     = 0xD1,
    .unk_06     = 0x38,
    .unk_08     = Shop_sbar_GetFrameInfo,
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

// Nonmatching: regswap
u16 func_ov043_020b3f68(ShopObject* shop, u16 arg1) {
    u32 y = arg1;
    if (y < 56) {
        y = 56;
    }

    u32 tmp    = FX_Divide((y - 0x38) << 0xC, 0x27000 / (shop->unk_870 + 1)) * 0x10;
    u32 scroll = tmp >> 0x10;

    if (shop->unk_870 < scroll) {
        scroll = shop->unk_870;
    }
    return scroll * 6;
}

// Nonmatching: casting difference
u16 func_ov043_020b3fcc(ShopObject* shop, u16 arg1) {
    u16 y = ((((arg1 / 6)) * 39) / shop->unk_872 + 56);
    if (y > 95) {
        y = 95;
    }
    return y;
}

static SpriteFrameInfo* Shop_sbar_GetFrameInfo(Sprite* sprite, s32 arg1, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;
            temp->unk_04          = 0;
            temp->unk_08          = 0;
            temp->unk_0C          = 0;
            temp->unk_10          = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;

        default:
            break;
    }

    return info;
}

static void Shop_sbar_Load(Sprite* sprite, Shop_sbar_Args* args) {
    SpriteAnimation anim = data_ov043_020ca4e4;

    anim.dataType = args->dataType;

    anim.unk_2A = 45;
    anim.unk_04 = 209;
    anim.unk_06 = 56;
    _Sprite_Load(&sprite[0], &anim);

    anim.unk_2A = 43;
    anim.unk_04 = 209;
    anim.unk_06 = 29;
    _Sprite_Load(&sprite[1], &anim);

    anim.unk_2A = 44;
    anim.unk_04 = 209;
    anim.unk_06 = 102;
    _Sprite_Load(&sprite[2], &anim);
}

static s32 Shop_sbar_Init(TaskPool* pool, Task* task, void* args) {
    Shop_sbar_Args* taskArgs = args;
    ShopObject*     shop     = taskArgs->shop;
    Shop_sbar*      sbar     = task->data;

    sbar->shop      = shop;
    sbar->prevValue = shop->unk_850;
    Shop_sbar_Load(sbar->sprites, taskArgs);
    return 1;
}

static s32 Shop_sbar_Update(TaskPool* pool, Task* task, void* args) {
    Shop_sbar*  sbar = task->data;
    ShopObject* shop = sbar->shop;

    TouchCoord coord;

    if (shop->unk_836 != 0) {
        for (s32 i = 0; i < 3; i++) {
            Sprite_Update(&sbar->sprites[i]);
        }
        return 1;
    }

    if (shop->unk_834 & 0x10) {
        TouchInput_GetCoord(&coord);
        shop->unk_850 = func_ov043_020b3f68(shop, coord.y + 10);
        shop->unk_834 &= ~0x10;
        shop->unk_834 |= 0x20;
        sbar->sprites[0].posY = func_ov043_020b3fcc(shop, shop->unk_850);
    }

    if (shop->unk_834 & 0x20) {

        TouchInput_GetCoord(&coord);

        s16 currentY = sbar->sprites[0].posY;
        s16 targetY  = coord.y + 10;

        s16 diff = (s16)(((s32)(targetY - currentY)) >> 2);

        if (diff != 0) {
            sbar->sprites[0].posY = currentY + diff;
        } else {
            sbar->sprites[0].posY = targetY;
        }

        if (sbar->sprites[0].posY < 56) {
            sbar->sprites[0].posY = 56;
        } else if (sbar->sprites[0].posY > 95) {
            sbar->sprites[0].posY = 95;
        }

        shop->unk_850 = func_ov043_020b3f68(shop, sbar->sprites[0].posY);

        if (TouchInput_WasTouchReleased()) {
            shop->unk_834 &= ~0x20;
        }

        s32 delta = shop->unk_848 - shop->unk_850;
        if (delta < 0) {
            shop->unk_844 = shop->unk_848 % 6;
        } else if (delta < 12) {
            shop->unk_844 = delta;
        } else {
            shop->unk_844 = (delta % 6) + 6;
        }
    } else {
        if (TouchInput_WasTouchPressed() != 0) {
            TouchInput_GetCoord(&coord);
            if (func_ov043_020b1b14(coord.x, coord.y) == 1) {
                if (func_ov043_020b1b74(coord.x, coord.y, sbar->sprites[0].posX, sbar->sprites[0].posY) == 1) {
                    shop->unk_834 |= 0x20;
                } else {
                    shop->unk_834 |= 0x10;
                }
            } else {
                s32 touchResult = func_ov043_020b1bd4(coord.x, coord.y);

                if (touchResult == 0) {
                    if (shop->unk_850 >= 6) {
                        shop->unk_850 -= 6;
                        if (shop->unk_844 < 6) {
                            shop->unk_844 += 6;
                        }
                    }
                } else if (touchResult == 1) {
                    if (shop->unk_850 <= ((1 - shop->unk_870) * -6)) {
                        shop->unk_850 += 6;
                        if (shop->unk_844 >= 6 && shop->unk_844 < 12) {
                            shop->unk_844 -= 6;
                        }
                    }
                }
            }
        }
        sbar->sprites[0].posY = func_ov043_020b3fcc(shop, shop->unk_850);
    }

    if (sbar->prevValue != shop->unk_850) {
        SndMgr_StartPlayingSE(0x119);
    }

    sbar->prevValue = shop->unk_850;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->sprites[i]);
    }

    return 1;
}

static s32 Shop_sbar_Render(TaskPool* pool, Task* task, void* args) {
    Shop_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_RenderFrame(&sbar->sprites[i]);
    }
    return 1;
}

static s32 Shop_sbar_Destroy(TaskPool* pool, Task* task, void* args) {
    Shop_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->sprites[i]);
    }
    return 1;
}

static s32 Shop_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Shop_sbar_Init,
        .update     = Shop_sbar_Update,
        .render     = Shop_sbar_Render,
        .cleanup    = Shop_sbar_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Shop_sbar_CreateTask(TaskPool* arg0, s32 dataType, ShopObject* shop) {
    Shop_sbar_Args args;
    args.dataType = dataType;
    args.shop     = shop;
    return EasyTask_CreateTask(arg0, &Tsk_Shop_sbar, NULL, 0, NULL, &args);
}
