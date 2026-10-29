#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Save.h"

typedef struct BtlInfExp {
    /* 0x000 */ CombatSprite label;
    /* 0x060 */ CombatSprite digits[5];
    /* 0x240 */ s32          value;
    /* 0x244 */ s16          digitCount;
    /* 0x246 */ s16          timer;
} BtlInfExp; // Size: 0x248

extern const BinIdentifier data_ov003_020d7800;

// Extract decimal digits least-significant first, saturating to the five-digit HUD.
s32 func_ov003_0209d8a0(s32* digits, s32 value) {
    s32 remainder;
    if (value < 0)
        value = 0;
    if (value > 99999)
        value = 99999;
    remainder = value % 10000;
    digits[4] = value / 10000;
    digits[3] = remainder / 1000;
    remainder %= 1000;
    digits[2] = remainder / 100;
    remainder %= 100;
    digits[1] = remainder / 10;
    digits[0] = remainder % 10;
    if (value >= 10000)
        return 5;
    if (value >= 1000)
        return 4;
    if (value >= 100)
        return 3;
    return value >= 10 ? 2 : 1;
}

void func_ov003_0209d9a8(BtlInfExp* panel) {
    s32 digits[5];
    s32 i;
    panel->value      = gSaveData.battleExpToNextLevel;
    panel->digitCount = func_ov003_0209d8a0(digits, panel->value);
    for (i = 0; i < panel->digitCount; i++) {
        CombatSprite_SetAnim(&panel->digits[i], (u16)(s16)digits[i], 0);
    }
}

s32 func_ov003_0209da30(TaskPool* pool, Task* task, void* args) {
    BtlInfExp* panel = task->data;
    s32        i;
    CombatSprite_LoadDirect(1, &panel->label, &data_ov003_020d7800, 7, 9, 8, 0x3F, 0);
    CombatSprite_SetPosition(&panel->label, 6, 15);
    func_ov003_02082738(&panel->label, 0);
    for (i = 0; i < 5; i++) {
        CombatSprite_LoadDirect(1, &panel->digits[i], &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3F, 0);
        func_ov003_02082738(&panel->digits[i], 0);
    }
    func_ov003_0209d9a8(panel);
    panel->timer = 0;
    return 1;
}

s32 func_ov003_0209db08(TaskPool* pool, Task* task, void* args) {
    BtlInfExp* panel = task->data;
    s32        i;
    if (gSaveData.battleMaxLevel < 100 && panel->value != gSaveData.battleExpToNextLevel) {
        panel->timer = 180;
        func_ov003_0209d9a8(panel);
    }
    if (panel->timer > 0)
        panel->timer--;
    CombatSprite_Update(&panel->label);
    for (i = 0; i < panel->digitCount; i++)
        CombatSprite_Update(&panel->digits[i]);
    return 1;
}

s32 func_ov003_0209dba8(TaskPool* pool, Task* task, void* args) {
    BtlInfExp* panel = task->data;
    s32        i;
    s16        x;
    if (panel->timer <= 0)
        return 1;
    CombatSprite_Render(&panel->label);
    x = 6 + (panel->digitCount - 1) * 7;
    for (i = 0; i < panel->digitCount; i++) {
        CombatSprite_SetPosition(&panel->digits[i], (s16)(x - i * 7), 15);
        CombatSprite_Render(&panel->digits[i]);
    }
    return 1;
}

s32 func_ov003_0209dc3c(TaskPool* pool, Task* task, void* args) {
    BtlInfExp* panel = task->data;
    s32        i;
    CombatSprite_Release(&panel->label);
    for (i = 0; i < 5; i++)
        CombatSprite_Release(&panel->digits[i]);
    return 1;
}

extern const TaskStages data_ov003_020d8180;
s32                     func_ov003_0209dc74(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8180;
    if (func_ov003_0208b690((u16)stage))
        return 1;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e3d1c[16] = "Tsk_BtlInf_Exp";
const TaskHandle data_ov003_020d8174     = {data_ov003_020e3d1c, func_ov003_0209dc74, sizeof(BtlInfExp)};
const TaskStages data_ov003_020d8180     = {
    func_ov003_0209da30,
    func_ov003_0209db08,
    func_ov003_0209dba8,
    func_ov003_0209dc3c,
};
