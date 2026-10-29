#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Save.h"
#include <nitro/mi/cpumem.h>

typedef struct BtlInfTimer {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ u16          minutes[2];
    /* 0x64 */ u16          seconds[2];
    /* 0x68 */ u16          hundredths[2];
    /* 0x6C */ u16          unk_6C;
    /* 0x6E */ u16          pad_6E;
    /* 0x70 */ void (*update)(struct BtlInfTimer* timer);
    /* 0x74 */ u16 state;
    /* 0x76 */ u16 count;
    /* 0x78 */ s32 visible;
} BtlInfTimer; // Size: 0x7C

extern const BinIdentifier data_ov003_020d7800;

void func_ov003_0209de24(BtlInfTimer* timer, void (*update)(BtlInfTimer*)) {
    timer->update = update;
    timer->state  = 0;
    timer->count  = 0;
}

// The intermediate seconds are deliberately narrowed to 16 bits before division.
// The display saturates at 99:59.99 once the frame count reaches 100 minutes.
// Nonmatching: division scheduling and register allocation in the digit conversion.
void func_ov003_0209de38(BtlInfTimer* timer) {
    u32 frames     = gSaveData.battleClearTime;
    u16 hundredths = (frames * 100 / 60) % 100;
    u16 elapsed    = frames / 60;
    u16 minutes    = (elapsed / 60) % 100;
    u16 seconds    = elapsed % 60;

    if (frames >= 0x57E40) {
        timer->hundredths[0] = 9;
        timer->hundredths[1] = 9;
        timer->seconds[0]    = 9;
        timer->seconds[1]    = 5;
        timer->minutes[0]    = 9;
        timer->minutes[1]    = 9;
        return;
    }
    timer->hundredths[0] = hundredths % 10;
    timer->hundredths[1] = hundredths / 10;
    timer->seconds[0]    = seconds % 10;
    timer->seconds[1]    = seconds / 10;
    timer->minutes[0]    = minutes % 10;
    timer->minutes[1]    = minutes / 10;
}

s32 func_ov003_0209dfac(TaskPool* pool, Task* task, void* args) {
    BtlInfTimer* timer = task->data;

    MI_CpuSet(timer, 0, sizeof(*timer));
    data_ov003_020e71b8->unk3D8F8 = 0;
    timer->visible                = 1;
    timer->count                  = 0;
    timer->state                  = 0;
    timer->unk_6C                 = 0;
    func_ov003_0209de24(timer, func_ov003_0209de38);
    CombatSprite_LoadDirect(0, &timer->sprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3D, 0x37);
    func_ov003_02082738(&timer->sprite, 0);
    func_ov003_02082730(&timer->sprite, 0);
    return 1;
}

s32 func_ov003_0209e060(TaskPool* pool, Task* task, void* args) {
    BtlInfTimer* timer = task->data;
    if (timer->update != NULL) {
        timer->update(timer);
    }
    return 1;
}

// Keep the individual sprite updates: they are observable through frame callbacks.
#define DRAW_TIMER_DIGIT(digit, x)                                       \
    CombatSprite_SetAnim(&timer->sprite, (u16)(s16)((digit) + 0x14), 0); \
    CombatSprite_SetPosition(&timer->sprite, (x), 0xAA);                 \
    CombatSprite_Update(&timer->sprite);                                 \
    CombatSprite_Render(&timer->sprite)

s32 func_ov003_0209e080(TaskPool* pool, Task* task, void* args) {
    BtlInfTimer* timer = task->data;
    if (timer->visible == 0) {
        return 1;
    }
    DRAW_TIMER_DIGIT(timer->hundredths[0], 0xF0);
    DRAW_TIMER_DIGIT(timer->hundredths[1], 0xE8);
    DRAW_TIMER_DIGIT(timer->seconds[0], 0xDD);
    DRAW_TIMER_DIGIT(timer->seconds[1], 0xD5);
    DRAW_TIMER_DIGIT(timer->minutes[0], 0xCC);
    if (timer->minutes[1] != 0) {
        DRAW_TIMER_DIGIT(timer->minutes[1], 0xC4);
    }
    CombatSprite_SetAnim(&timer->sprite, 0x37, 0);
    CombatSprite_SetPosition(&timer->sprite, 0xC4, 0xAA);
    CombatSprite_Update(&timer->sprite);
    CombatSprite_Render(&timer->sprite);
    return 1;
}
#undef DRAW_TIMER_DIGIT

s32 func_ov003_0209e270(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

extern const TaskStages data_ov003_020d81b8;
s32                     func_ov003_0209e284(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d81b8;
    return stages.iter[stage](pool, task, args);
}

char             data_ov003_020e3d3c[20] = "Tsk_BtlInf_Timer";
const TaskHandle data_ov003_020d81ac     = {data_ov003_020e3d3c, func_ov003_0209e284, sizeof(BtlInfTimer)};
const TaskStages data_ov003_020d81b8     = {
    func_ov003_0209dfac,
    func_ov003_0209e060,
    func_ov003_0209e080,
    func_ov003_0209e270,
};
