#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Save.h"

#include <nitro/mi/cpumem.h>

// The combo-count popup: a task that shows the running hit combo next to the main actor, then
// re-shows it with a flash every time the combo grows. The combo itself lives in
// `data_ov003_020e71b8->unk3D8F8`; hitting the actor's hit flags feed it through the shared
// `data_ov003_020e71bc` command mailbox.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

struct BtlPop_Combo;

/// Task data of the combo popup (`Tsk_BtlPop_Combo`, 0x9C bytes).
typedef struct BtlPop_Combo {
    /* 0x00 */ CombatSprite sprite;
    /* 0x60 */ u16          digits[4];     // current combo digits, least significant first
    /* 0x68 */ u16          prevDigits[4]; // digits of the combo shown when the popup was last
                                           // refreshed; they slide in as the "gain" digits
    /* 0x70 */ u16 shownCombo;             // combo the popup was last refreshed with
    /* 0x72 */ u16 combo;                  // current combo count
    /* 0x74 */ s16 digitCount;
    /* 0x76 */ s16 prevDigitCount;
    /* 0x78 */ s32 x;                                     // slide offset of the digits
    /* 0x7C */ s32 y;                                     // unused outside the initialiser
    /* 0x80 */ s32 z;                                     // slide offset of the second digit row
    /* 0x84 */ s32 unk84;
    /* 0x88 */ void (*update)(struct BtlPop_Combo* data); // per-state-frame callback
    /* 0x8C */ s16 state;
    /* 0x8E */ s16 timer;
    /* 0x90 */ s32 visible;
    /* 0x94 */ u16 flashTimer;
    /* 0x96 */ s16 holdFrames; // combo + the per-psych bonus, in frames
    /* 0x98 */ s32 active;
} BtlPop_Combo;                // Size: 0x9C

/// The combo command mailbox at `data_ov003_020e71bc`: `func_ov003_0209c150` posts a combo
/// increase into it and the popup task consumes it the next frame.
typedef struct BtlPop_ComboCmd {
    /* 0x0 */ s16 type;  // 1 = add combo, 2 = hide the popup
    /* 0x2 */ s16 value; // combo increase, or the combo the popup should show
} BtlPop_ComboCmd;       // Size: 0x4

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern void func_020265d4(s32* dst, s32 target, u16 weight);

extern const BinIdentifier data_ov003_020d7800;
extern const TaskHandle    data_ov003_020d8018;
extern const TaskStages    data_ov003_020d8024;
extern BtlPop_ComboCmd     data_ov003_020e71bc;

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

s32  func_ov003_0209bdf0(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209bea0(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209bed4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209c0d4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_0209c0e8(TaskPool* pool, Task* task, void* args, s32 stage);
void func_ov003_0209ba40(BtlPop_Combo* data);
void func_ov003_0209ba94(BtlPop_Combo* data);
void func_ov003_0209b8f4(BtlPop_Combo* data);
void func_ov003_0209b88c(BtlPop_Combo* data, s32 combo);

/// Installs the per-state-frame callback and restarts the state machine.
void func_ov003_0209b760(BtlPop_Combo* data, void (*update)(BtlPop_Combo* data)) {
    data->update = update;
    data->state  = 0;
    data->timer  = 0;
}

/// Splits `value` (clamped to 0..9999) into decimal digits, least significant first, and
/// returns how many digits are needed to show it.
s16 func_ov003_0209b774(s32* digits, s32 value) {
    s32 v = value;
    s32 rest;

    if (v > 9999) {
        v = 9999;
    }

    digits[3] = v / 1000;
    rest      = v % 1000;
    digits[2] = rest / 100;
    rest      = rest % 100;
    digits[1] = rest / 10;
    digits[0] = rest % 10;

    if (v >= 1000) {
        return 4;
    }
    if (v >= 100) {
        return 3;
    }
    if (v >= 10) {
        return 2;
    }
    return 1;
}

/// Re-splits `combo` into the digit row the popup draws.
void func_ov003_0209b838(BtlPop_Combo* data, s32 combo) {
    s32 digits[4];
    s32 i;

    data->digitCount = func_ov003_0209b774(digits, combo);
    for (i = 0; i < data->digitCount; i++) {
        data->digits[i] = (u16)digits[i];
    }
}

/// Re-splits the previous combo into the row that slides in behind the current one. A combo of
/// zero clears the row instead.
void func_ov003_0209b88c(BtlPop_Combo* data, s32 combo) {
    s32 digits[4];
    s32 i;

    if (combo == 0) {
        data->prevDigitCount = 0;
        return;
    }
    data->prevDigitCount = func_ov003_0209b774(digits, combo);
    for (i = 0; i < data->prevDigitCount; i++) {
        data->prevDigits[i] = (u16)digits[i];
    }
}

/// Restarts the popup on the current combo.
void func_ov003_0209b8f4(BtlPop_Combo* data) {
    data->x          = 0;
    data->z          = -0x16000;
    data->active     = 1;
    data->shownCombo = 0;
    data->combo      = data_ov003_020e71b8->unk3D8F8;
    func_ov003_0209b760(data, func_ov003_0209ba94);
}

/// Consumes a combo command from the shared mailbox: a type-1 command banks the combo increase
/// and (re)starts the popup, a type-2 command hides it.
// Nonmatching: the `data->combo` re-read after `data->timer = 0` lands in r1 instead of r0;
// every other instruction is identical.
void func_ov003_0209b93c(BtlPop_Combo* data, BtlPop_ComboCmd* cmd) {
    switch (cmd->type) {
        case 2:
            cmd->type    = 0;
            data->active = 0;
            return;

        case 1:
            cmd->type        = 0;
            data->holdFrames = cmd->value;
            if (func_ov003_020a3330(0x22) != 0) {
                data->holdFrames = (s16)(data->holdFrames + (s16)func_ov003_020a3468(0x22));
            }
            if (data->active == 0) {
                if (data_ov003_020e71b8->unk3D8F8 > 1) {
                    func_ov003_0209b8f4(data);
                    return;
                }
                data->timer = 0;
                func_ov003_0209b760(data, func_ov003_0209ba40);
                return;
            }
            if (data->state != 4) {
                return;
            }
            data->combo = data_ov003_020e71b8->unk3D8F8;
            data->timer = 0;
            if (data->combo != 1) {
                data->state = 2;
                return;
            }
            func_ov003_0209b88c(data, data->combo);
            data->state = 0;
            return;
    }
}

/// Idle state: waits out the combo hold time, then resets the shared combo counter and hides
/// the popup.
void func_ov003_0209ba40(BtlPop_Combo* data) {
    if (data->timer == 0) {
        data->timer = data->holdFrames + 0x14;
    }
    data->timer = data->timer - 1;
    if (data->timer > 0) {
        return;
    }
    data_ov003_020e71b8->unk3D8F8 = 0;
    data->timer                   = 0;
    func_ov003_0209b760(data, NULL);
}

/// The popup's state machine: slide the digits in, flash on a combo increase, hold, then fade.
void func_ov003_0209ba94(BtlPop_Combo* data) {
    switch (data->state) {
        case 0:
            if (data->timer == 0) {
                data->visible    = 1;
                data->flashTimer = 0;
                func_ov003_0209b838(data, data->combo);
                data->timer = 8;
            }
            if (data->timer > 0) {
                data->timer--;
                func_020265d4(&data->x, 0x25000, data->timer);
            }
            if (data->timer > 0) {
                return;
            }
            data->x     = 0x25000;
            data->state = 1;
            return;

        case 1:
            if (data->timer == 0) {
                data->timer = 2;
            }
            if (data->timer > 0) {
                data->timer--;
                func_020265d4(&data->x, 0x20000, data->timer);
            }
            if (data->timer > 0) {
                return;
            }
            data->x     = 0x20000;
            data->state = 4;
            return;

        case 2:
            if (data->timer == 0) {
                data->visible = 1;
                func_ov003_0209b838(data, data->combo);
                data->timer = 10;
                data->x     = 0x1E000;
                data->z     = 0;
            }
            if (data->timer > 0) {
                func_020265d4(&data->z, -0x18000, (u16)data->timer);
            }
            data->timer--;
            if (data->timer > 0) {
                return;
            }
            data->state = 3;
            data->timer = 0;
            return;

        case 3:
            if (data->timer == 0) {
                func_ov003_0209b838(data, data->combo);
                data->timer = 2;
                data->x     = 0x1E000;
            }
            if (data->timer > 0) {
                func_020265d4(&data->z, -0x16000, (u16)data->timer);
            }
            data->timer--;
            if (data->timer > 0) {
                return;
            }
            data->state = 4;
            data->timer = 0;
            return;

        case 4:
            if (data->timer == 0) {
                data->x          = 0x20000;
                data->shownCombo = data->combo;
                func_ov003_0209b88c(data, data->shownCombo);
                data->timer = data->holdFrames + 0x14;
            }
            data->visible = (data->timer > data->holdFrames + 5);
            if (data->visible == 0) {
                data->flashTimer = 0;
            }
            if (!(data_ov003_020e71b8->unk3D878 & 8)) {
                data->timer--;
                if (data->timer <= 0) {
                    data->state = 5;
                }
            }
            if (data_ov003_020e71b8->unk3D8F8 <= data->combo) {
                return;
            }
            data->state = 2;
            data->timer = 0;
            data->combo = data_ov003_020e71b8->unk3D8F8;
            return;

        case 5:
            if (data->timer == 0) {
                func_ov003_0209d4b0();
                data_ov003_020e71b8->unk3D8F8 = 0;
                data->timer                   = 5;
                func_ov003_0209b88c(data, 0);
            }
            if (data->timer > 0) {
                func_020265d4(&data->x, 0, (u16)data->timer);
            }
            data->timer--;
            if (data->timer > 0) {
                return;
            }
            if (data_ov003_020e71b8->unk3D8F8 > 1) {
                func_ov003_0209b8f4(data);
                return;
            }
            data->active = 0;
            func_ov003_0209b760(data, NULL);
            return;
    }
}

// Nonmatching: the `CombatSprite_LoadDirect` argument setup is scheduled four instructions
// apart (the address/sprite-argument loads are hoisted above the 7th stack argument).
s32 func_ov003_0209bdf0(TaskPool* pool, Task* task, void* args) {
    BtlPop_Combo* data = task->data;

    MI_CpuSet(data, 0, sizeof(BtlPop_Combo));
    data_ov003_020e71b8->unk3D8F8 = 0;
    data->x                       = 0;
    data->y                       = 0xB4000;
    data->z                       = 0;
    data->unk84                   = 0;
    data->timer                   = 0;
    data->state                   = 0;
    data->digitCount              = 0;
    data->active                  = 0;

    CombatSprite_LoadDirect(0, &data->sprite, &data_ov003_020d7800, 0x25, 0x27, 0x26, 0x3D, 0x36);
    func_ov003_02082738(&data->sprite, 0);
    func_ov003_02082730(&data->sprite, 0);
    return 1;
}

s32 func_ov003_0209bea0(TaskPool* pool, Task* task, void* args) {
    BtlPop_Combo* data = task->data;

    func_ov003_0209b93c(data, &data_ov003_020e71bc);
    if (data->update != NULL) {
        data->update(data);
    }
    return 1;
}

/// Draws the old digit row at the fixed "gain" position and the current row at the popup's
/// screen position, plus the plate sprite behind them.
// Nonmatching: `posX`/`posY`/`animOffset`/`i`/`offset` occupy a rotated register set
// (target r9/r5/r8/r10/r7); the instruction sequence itself is identical.
s32 func_ov003_0209bed4(TaskPool* pool, Task* task, void* args) {
    BtlPop_Combo* data = task->data;
    s32           posX;
    s16           posY;
    s32           animOffset;
    s32           i;
    s32           offset;

    if (data->active == 0) {
        return 1;
    }

    if (data->prevDigitCount > 0) {
        CombatSprite_SetAnim(&data->sprite, 0x36, 0);
        CombatSprite_SetPosition(&data->sprite, 0x20, 0xAA);
        CombatSprite_Update(&data->sprite);
        CombatSprite_Render(&data->sprite);
    }
    for (i = 0, offset = 0; i < data->prevDigitCount; i++, offset += 7) {
        CombatSprite_SetAnim(&data->sprite, (u16)(s16)(data->prevDigits[i] + 0x14), 0);
        CombatSprite_SetPosition(&data->sprite, (s16)(0x20 - (offset + 7)), 0xAA);
        CombatSprite_Update(&data->sprite);
        CombatSprite_Render(&data->sprite);
    }

    posX = (data->x * 0x10) >> 0x10;
    posY = (s16)((data->z >> 0xC) + 0xC0);
    if (data->visible != 0) {
        animOffset = (data->flashTimer & 0x11) ? 0 : 0x14;
        data->flashTimer++;
    } else {
        animOffset = 0x14;
    }

    if (data->state != 2 && data->state != 3 && (data->state != 4 || data->timer != 0)) {
        CombatSprite_SetAnim(&data->sprite, 0x36, 0);
        CombatSprite_SetPosition(&data->sprite, posX, posY);
        CombatSprite_Update(&data->sprite);
        CombatSprite_Render(&data->sprite);
    }
    for (i = 0, offset = 0; i < data->digitCount; i++, offset += 7) {
        CombatSprite_SetAnim(&data->sprite, (u16)(s16)(animOffset + data->digits[i]), 0);
        CombatSprite_SetPosition(&data->sprite, (s16)(posX - (offset + 7)), posY);
        CombatSprite_Update(&data->sprite);
        CombatSprite_Render(&data->sprite);
    }
    return 1;
}

s32 func_ov003_0209c0d4(TaskPool* pool, Task* task, void* args) {
    CombatSprite_Release(task->data);
    return 1;
}

/// Task dispatcher: calls the stage function for `stage`, which is a no-op for a popup that
/// finished this frame.
s32 func_ov003_0209c0e8(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8024;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Posts a combo increase through the mailbox and records the longest combo of the battle.
void func_ov003_0209c150(s16 combo) {
    data_ov003_020e71bc.type  = 1;
    data_ov003_020e71bc.value = combo;
    data_ov003_020e71b8->unk3D8F8++;
    if (gSaveData.battleMaxHits < data_ov003_020e71b8->unk3D8F8) {
        gSaveData.battleMaxHits = data_ov003_020e71b8->unk3D8F8;
    }
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3ca0[0x14] = "Tsk_BtlPop_Combo";

const TaskHandle data_ov003_020d8018 = {data_ov003_020e3ca0, func_ov003_0209c0e8, sizeof(BtlPop_Combo)};

const TaskStages data_ov003_020d8024 = {
    func_ov003_0209bdf0,
    func_ov003_0209bea0,
    func_ov003_0209bed4,
    func_ov003_0209c0d4,
};

BtlPop_ComboCmd data_ov003_020e71bc;
