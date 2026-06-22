#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Top.h"
#include <nitro/fx.h>

typedef struct {
    /* 0x000 */ Sprite             sprites[5];
    /* 0x140 */ s32                unk_140[5];
    /* 0x154 */ UnkStruct_TopMenu* unk_154;
    /* 0x158 */ u16                unk_158;
    /* 0x15A */ u16                _pad_15A;
    /* 0x15C */ s32                unk_15C;
    /* 0x160 */ s32                unk_160;
} MenuTop_lvGauge; // Size: 0x164

typedef struct {
    /* 0x0 */ s32   dataType;
    /* 0x4 */ void* unk_4;
} MenuTop_lvGauge_Args;

extern s32 func_ov043_02084ed8(s16 x, s16 y);
extern s32 func_ov043_02084f38(s16 x, s16 y);
extern s32 func_ov043_02084f98(s16 x, s16 y);

static SpriteFrameInfo* MenuTop_lvGauge_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2);
static s32              MenuTop_lvGauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuTop_lvGauge = {"Tsk_MenuTop_lvGauge", MenuTop_lvGauge_RunTask, sizeof(MenuTop_lvGauge)};

static const SpriteAnimation data_ov043_020c7b88 = {
    .bits_0_1   = 1,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuTop_lvGauge_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c7988,
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 4,
    .unk_22     = 4,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

static void func_ov043_02085e94(UnkStruct_TopMenu* arg0) {
    s16 level = arg0->unk_06;
    s16 max   = arg0->unk_08;
    s16 x;

    if (level == max) {
        x = 238;
    } else {
        x = ((s32)((level - 1) * FX_Divide(0xC8000, (max - 1) << 12)) >> 12) + 0x26;
    }

    arg0->unk_52 = x;
    arg0->unk_54 = 0x98;
}

// Nonmatching
static void func_ov043_02085ee8(UnkStruct_TopMenu* owner) {
    s16 max = owner->unk_08;
    s16 x   = owner->unk_52;

    if (max == 1) {
        owner->unk_06 = 1;
        owner->unk_52 = 238;

        if (TouchInput_WasTouchReleased() == 0) {
            return;
        }

        owner->unk_00 &= ~1;
        owner->unk_00 &= ~4;
        return;
    }

    u32 step  = FX_Divide(0xC8000, (max - 1) << 12);
    u32 base  = x - 0x26;
    u32 index = FX_Divide(base << 12, step) >> 12;

    if (((base << 12) - (step * index)) > ((s32)(step + (step >> 31)) >> 1)) {
        owner->unk_06 = index + 2;
    } else {
        owner->unk_06 = index + 1;
    }

    if (owner->unk_06 > owner->unk_08) {
        owner->unk_06 = owner->unk_08;
    } else if (owner->unk_06 < 1) {
        owner->unk_06 = 1;
    }

    if (TouchInput_WasTouchReleased()) {
        func_ov043_02085e94(owner);
        owner->unk_00 &= ~1;
        owner->unk_00 &= ~4;
    }
}

// Nonmatching
static s32 func_ov043_02085fcc(MenuTop_lvGauge* gauge, s32 arg1) {
    u32 x = I2F(arg1);

    if (x <= 0x26000) {
        gauge->unk_140[1] = 0;
        return 0;
    } else {
        gauge->unk_140[1] = 1;
        return FX_Divide(x - 0x26000, 0x64000);
    }
}

static SpriteFrameInfo* MenuTop_lvGauge_GetFrameInfo(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite*          sprite = arg0;
    SpriteFrameInfo* info   = NULL;

    switch (arg2) {
        case 1:
            data_0206b408.unk_00 = 1;
            return &data_0206b408;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;
    }

    return info;
}

static void MenuTop_lvGauge_Load(MenuTop_lvGauge* gauge, Sprite* sprites, void* arg2) {
    MenuTop_lvGauge_Args* args  = arg2;
    UnkStruct_TopMenu*    owner = gauge->unk_154;
    SpriteAnimation       anim  = data_ov043_020c7b88;

    anim.dataType = args->dataType;

    func_ov043_02085e94(owner);

    anim.unk_02.raw &= ~2;
    anim.unk_2A = 0x23;
    anim.unk_04 = owner->unk_52;
    anim.unk_06 = owner->unk_54;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_02.raw |= 2;
    anim.unk_2A = 0x24;
    anim.unk_04 = 0x26;
    anim.unk_06 = 0x98;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_02.raw &= ~2;
    anim.unk_2A = 0x25;
    anim.unk_04 = 0x26;
    anim.unk_06 = 0x98;
    _Sprite_Load(&sprites[2], &anim);

    anim.unk_02.raw &= ~2;
    anim.unk_2A = 0x21;
    anim.unk_04 = 0x21;
    anim.unk_06 = 0x98;
    _Sprite_Load(&sprites[3], &anim);

    anim.unk_02.raw &= ~2;
    anim.unk_2A = 0x22;
    anim.unk_04 = 0xF3;
    anim.unk_06 = 0x98;
    _Sprite_Load(&sprites[4], &anim);

    for (s32 i = 0; i < 5; i++) {
        gauge->unk_140[i] = 1;
    }

    gauge->unk_158 = 0;
    gauge->unk_15C = 0x1000;
    gauge->unk_160 = 0x1000;
}

static s32 MenuTop_lvGauge_Init(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge*      gauge    = task->data;
    MenuTop_lvGauge_Args* initArgs = args;

    gauge->unk_154 = initArgs->unk_4;
    MenuTop_lvGauge_Load(gauge, gauge->sprites, initArgs);
    return 1;
}

static s32 MenuTop_lvGauge_Update(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge*   gauge = task->data;
    UnkStruct_TopMenu* owner = gauge->unk_154;
    TouchCoord         coord;

    if (owner->unk_56 != 0) {
        return 1;
    }

    if ((owner->unk_00 & 8) != 0) {
        return 1;
    }

    if ((owner->unk_00 & 1) != 0 && (owner->unk_00 & 4) != 0) {
        TouchInput_GetCoord(&coord);
        owner->unk_52 = (s16)coord.x;

        if (owner->unk_52 > 0xEE) {
            owner->unk_52 = 0xEE;
        } else if (owner->unk_52 < 0x26) {
            owner->unk_52 = 0x26;
        }

        func_ov043_02085ee8(owner);
    } else {
        if (owner->unk_08 <= 1) {
            gauge->unk_140[0] = 0;
        } else {
            gauge->unk_140[0] = 1;
        }

        if (TouchInput_IsTouchActive() != 0) {
            TouchInput_GetCoord(&coord);

            if (func_ov043_02084ed8((s16)coord.x, (s16)coord.y) == 1) {
                owner->unk_00 |= 5;
            } else if (TouchInput_WasTouchPressed() != 0) {
                if (func_ov043_02084f38((s16)coord.x, (s16)coord.y) != 0) {
                    owner->unk_06--;
                    if (owner->unk_06 < 1) {
                        owner->unk_06 = 1;
                    }

                    func_ov043_02085e94(owner);
                } else if (func_ov043_02084f98((s16)coord.x, (s16)coord.y) != 0) {
                    owner->unk_06++;
                    if (owner->unk_06 > owner->unk_08) {
                        owner->unk_06 = owner->unk_08;
                    }

                    func_ov043_02085e94(owner);
                }
            }
        }
    }

    gauge->sprites[0].posX = owner->unk_52;
    gauge->unk_15C         = func_ov043_02085fcc(gauge, gauge->sprites[0].posX);

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&gauge->sprites[i]);
    }

    return 1;
}

// Nonmatching
static s32 MenuTop_lvGauge_Render(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge* gauge = task->data;

    u16 affine = OamMgr_AllocAffineGroup(&g_OamMgr[DISPLAY_SUB], gauge->unk_158, gauge->unk_15C, gauge->unk_160, 0);

    gauge->sprites[0].unk_0A.raw = (gauge->sprites[0].unk_0A.raw & ~1) | 1;
    gauge->sprites[0].unk_0A.raw = (gauge->sprites[0].unk_0A.raw & ~0x3E0) | ((affine & 0x1F) << 5);

    for (s32 i = 0; i < 5; i++) {
        if (gauge->unk_140[i] != 0) {
            Sprite_RenderFrame(&gauge->sprites[i]);
        }
    }

    return 1;
}

static s32 MenuTop_lvGauge_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuTop_lvGauge* gauge = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&gauge->sprites[i]);
    }

    return 1;
}

static s32 MenuTop_lvGauge_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuTop_lvGauge_Init,
        .update     = MenuTop_lvGauge_Update,
        .render     = MenuTop_lvGauge_Render,
        .cleanup    = MenuTop_lvGauge_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 MenuTop_lvGauge_CreateTask(TaskPool* pool, s32 arg1, void* arg2) {
    MenuTop_lvGauge_Args args;

    args.dataType = arg1;
    args.unk_4    = arg2;

    return EasyTask_CreateTask(pool, &Tsk_MenuTop_lvGauge, NULL, 0, NULL, &args);
}