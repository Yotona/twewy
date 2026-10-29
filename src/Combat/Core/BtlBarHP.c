#include "Combat/Core/BtlBar.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Save.h"

#include <nitro/mi/cpumem.h>

// The HP bar HUD: the player's and the partner's HP bars, each with a lagging "damage" overlay
// and a white after-image segment, plus the two status overlays. The bars are driven from the two
// gauges in `Ov003Global` (0x3D8D0/0x3D8D2 for the player, 0x3D8D4/0x3D8D6 for the partner);
// `func_ov003_02098da0` is the interpolation state machine that turns the gauges into the
// per-sprite pixel widths and flag bits the render stage consumes.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The HP bar's task data (0x3FC bytes). All ten sprites are CombatSprites; the trailing fields
/// hold the widths and interpolation timers shared by the update and render stages.

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern void func_ov003_02087ed8(s32 seIdx);
extern void func_ov003_02087f28(s32 seIdx, s32 pan);
extern void func_ov003_02087f64(s32 seIdx, s32 pan);
extern void func_02026590(s32* value, s32 target, u16 frames);

extern const BinIdentifier data_ov003_020d7800;

extern const TaskHandle data_ov003_020d7f98;
extern const TaskStages data_ov003_020d7fa4;

// mwcc emits each unreferenced .rodata object in its own section; the handle is only used by other
// units, so an (optimised-away) intra-TU reference keeps both objects in one .rodata section like
// the retail build.
static const TaskHandle* const sBtlBarHpHandle = &data_ov003_020d7f98;

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Plays the damage voice sample for the bar that just dropped below half, once the battle is in
/// its "fighting" state (0x2). `arg0` selects the partner's sample table (`unk3D88D`).
void func_ov003_02098c28(s32 arg0, s32 arg1) {
    if (data_ov003_020e71b8->unk3D874 != 2) {
        return;
    }
    if (arg0 == 0) {
        if (arg1 >= -0x50) {
            return;
        }
        func_ov003_02087f28(0x36F, ((CombatActor*)data_ov003_020e71b8->unk3D898)->position.x);
        return;
    }
    if (arg1 <= 0x50) {
        return;
    }
    switch (data_ov003_020e71b8->unk3D88D) {
        case 0:
            func_ov003_02087f64(0x397, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x);
            return;
        case 1:
            func_ov003_02087f64(0x3BF, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x);
            return;
        case 2:
            func_ov003_02087f64(0x3E7, ((CombatActor*)data_ov003_020e71b8->unk3D89C)->position.x);
            return;
    }
}

/// Converts a gauge value to a bar width in pixels (a full bar is 0xA2 pixels wide).
s32 func_ov003_02098ce4(s16 value) {
    s32 width = ((value << 12) / data_ov003_020e71b8->unk3D8D2) * 0xA2;

    return (width << 4) >> 16;
}

/// Resets a freshly created bar to the gauges' current values.
void func_ov003_02098d18(BtlBar* bar) {
    bar->flags   = 0;
    bar->unk_3DA = data_ov003_020e71b8->unk3D8D4;
    bar->unk_3DC = data_ov003_020e71b8->unk3D8D6;
    bar->unk_3EC = func_ov003_02098ce4(bar->unk_3DA);
    bar->unk_3EE = func_ov003_02098ce4(bar->unk_3DC);
    bar->unk_3E4 = bar->unk_3EC << 12;
    bar->unk_3E8 = bar->unk_3EE << 12;
    bar->unk_3DE = 0;
    bar->unk_3E0 = 0;
    bar->unk_3F8 = 0;
}

/// Per-frame bar state machine: samples the two gauges into the banked values, starts draining or
/// filling the interpolated widths, and publishes the widths and visibility flags for the render
/// stage. Also runs the low-HP warning blink on the two bar backgrounds.
// Nonmatching: register allocation/scheduling of the two width selects (the `fixed` value and the
// sampled width land in the opposite scratch registers) - the logic is otherwise identical.
void func_ov003_02098da0(BtlBar* bar) {
    s32 width;
    s32 target;
    s32 fixed;
    s32 other;

    bar->flags = 0;
    if ((data_ov003_020e71b8->unk3D8D0 > 0) && (data_ov003_020e71b8->unk3D8D0 <= (data_ov003_020e71b8->unk3D8D2 >> 2))) {
        if (bar->unk_3F8 == 0) {
            CombatSprite_SetAnim(&bar->sprites[0], 5, 1);
            CombatSprite_SetAnim(&bar->sprites[5], 5, 1);
            func_ov003_02087ed8(0x1D);
            bar->unk_3F8 = 1;
        } else if (SpriteMgr_IsAnimationFinished(&bar->sprites[0].sprite)) {
            CombatSprite_Restart(&bar->sprites[0]);
            CombatSprite_Restart(&bar->sprites[5]);
            func_ov003_02087ed8(0x1D);
        }
    } else {
        CombatSprite_SetAnim(&bar->sprites[0], 0, 0);
        CombatSprite_SetAnim(&bar->sprites[5], 0, 0);
        bar->unk_3F8 = 0;
    }

    if (bar->unk_3DA < data_ov003_020e71b8->unk3D8D4) {
        bar->unk_3DE = 0x1E;
        bar->unk_3E4 = bar->unk_3EC << 12;
        bar->unk_3F0 = 1;
    } else if (bar->unk_3DA > data_ov003_020e71b8->unk3D8D4) {
        bar->unk_3DE = 0x1E;
        bar->unk_3E4 = bar->unk_3EC << 12;
        bar->unk_3F0 = 2;
    } else if (bar->unk_3DE <= 0) {
        bar->unk_3F0 = 0;
    }
    if (bar->unk_3DC < data_ov003_020e71b8->unk3D8D6) {
        bar->unk_3E0 = 0x1E;
        bar->unk_3E8 = bar->unk_3EE << 12;
        bar->unk_3F4 = 1;
    } else if (bar->unk_3DC > data_ov003_020e71b8->unk3D8D6) {
        bar->unk_3E0 = 0x1E;
        bar->unk_3E8 = bar->unk_3EE << 12;
        bar->unk_3F4 = 2;
    } else if (bar->unk_3E0 <= 0) {
        bar->unk_3F4 = 0;
    }

    bar->unk_3DA = data_ov003_020e71b8->unk3D8D4;
    bar->unk_3DC = data_ov003_020e71b8->unk3D8D6;
    bar->unk_3EC = func_ov003_02098ce4(data_ov003_020e71b8->unk3D8D4);
    fixed        = (bar->unk_3E4 << 4) >> 16;

    width        = (bar->unk_3F0 == 2) ? fixed : bar->unk_3EC;
    bar->unk_3C0 = width;
    if (width > 0x50) {
        bar->flags &= ~0x400;
    } else {
        bar->flags |= 0x400;
    }

    other = (bar->unk_3F0 == 2) ? bar->unk_3EC : fixed;
    if (other > 0x50) {
        bar->flags |= 2;
        bar->unk_3C6 = other - 0x50;
    } else {
        bar->flags &= ~2;
    }

    if (bar->unk_3F0 == 1) {
        bar->flags |= 4;
        bar->unk_3C8 = fixed;
        bar->flags |= 0x20;
        bar->unk_3D2 = bar->unk_3EC - 0x50;
    } else if (bar->unk_3F0 == 2) {
        bar->flags |= 0x40;
        bar->unk_3CC = bar->unk_3EC;
        bar->flags |= 0x200;
        bar->unk_3D6 = fixed - 0x50;
    }

    bar->unk_3EE = func_ov003_02098ce4(data_ov003_020e71b8->unk3D8D6);
    fixed        = (bar->unk_3E8 << 4) >> 16;

    target       = (bar->unk_3F4 == 2) ? fixed : bar->unk_3EE;
    bar->unk_3C2 = -target;
    if (target > 0x50) {
        bar->flags &= ~0x800;
    } else {
        bar->flags |= 0x800;
    }

    other = (bar->unk_3F4 == 2) ? bar->unk_3EE : fixed;
    if (other > 0x50) {
        bar->flags |= 1;
        bar->unk_3C4 = -(other - 0x50);
    } else {
        bar->flags &= ~1;
    }

    if (bar->unk_3F4 == 1) {
        bar->flags |= 0x10;
        bar->unk_3CA = -fixed;
        bar->flags |= 8;
        bar->unk_3D0 = -(bar->unk_3EE - 0x50);
    } else if (bar->unk_3F4 == 2) {
        bar->flags |= 0x100;
        bar->unk_3CE = -bar->unk_3EE;
        bar->flags |= 0x80;
        bar->unk_3D4 = -(fixed - 0x50);
    }

    if (bar->unk_3DE > 0) {
        func_02026590(&bar->unk_3E4, bar->unk_3EC << 12, (u16)bar->unk_3DE);
        bar->unk_3DE--;
    }
    if (bar->unk_3E0 > 0) {
        func_02026590(&bar->unk_3E8, bar->unk_3EE << 12, (u16)bar->unk_3E0);
        bar->unk_3E0--;
    }

    if (data_ov003_020e71b8->unk3D8D0 > 0) {
        return;
    }
    bar->flags &= ~0x400;
    bar->flags &= ~0x800;
}

/// Task init: loads the ten bar sprites and snaps them to the current gauges.
s32 func_ov003_0209923c(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;

    MI_CpuSet(bar, 0, sizeof(BtlBar));

    CombatSprite_LoadDirect(1, &bar->sprites[5], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 0);
    CombatSprite_LoadDirect(1, &bar->sprites[6], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 1);
    CombatSprite_LoadDirect(1, &bar->sprites[9], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 2);
    CombatSprite_LoadDirect(1, &bar->sprites[8], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 3);
    CombatSprite_LoadDirect(1, &bar->sprites[7], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 4);
    CombatSprite_LoadDirect(0, &bar->sprites[0], &data_ov003_020d7800, 0x2B, 0x2D, 0x2C, 0x3D, 0);
    CombatSprite_LoadDirect(0, &bar->sprites[1], &data_ov003_020d7800, 0x2B, 0x2D, 0x2C, 0x3D, 1);
    CombatSprite_LoadDirect(0, &bar->sprites[4], &data_ov003_020d7800, 0x2B, 0x2D, 0x2C, 0x3D, 2);
    CombatSprite_LoadDirect(0, &bar->sprites[3], &data_ov003_020d7800, 0x2B, 0x2D, 0x2C, 0x3D, 3);
    CombatSprite_LoadDirect(0, &bar->sprites[2], &data_ov003_020d7800, 0x2B, 0x2D, 0x2C, 0x3D, 4);

    func_ov003_02082730(&bar->sprites[5], 9);
    func_ov003_02082730(&bar->sprites[6], 7);
    func_ov003_02082730(&bar->sprites[9], 5);
    func_ov003_02082730(&bar->sprites[0], 9);
    func_ov003_02082730(&bar->sprites[1], 7);
    func_ov003_02082730(&bar->sprites[4], 5);

    func_ov003_02082738(&bar->sprites[5], 0);
    func_ov003_02082738(&bar->sprites[6], 0);
    func_ov003_02082738(&bar->sprites[7], 0);
    func_ov003_02082738(&bar->sprites[8], 0);
    func_ov003_02082738(&bar->sprites[9], 0);
    func_ov003_02082738(&bar->sprites[0], 0);
    func_ov003_02082738(&bar->sprites[1], 0);
    func_ov003_02082738(&bar->sprites[2], 0);
    func_ov003_02082738(&bar->sprites[3], 0);
    func_ov003_02082738(&bar->sprites[4], 0);

    func_ov003_02098d18(bar);
    return 1;
}

/// Task update: runs the state machine, plays the half-HP voice samples and updates the sprites.
s32 func_ov003_02099530(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;
    s32     playerBelowHalf;
    s32     partnerBelowHalf;

    playerBelowHalf  = bar->unk_3C2 > -0x50;
    partnerBelowHalf = bar->unk_3C0 < 0x50;
    func_ov003_02098da0(bar);
    if (playerBelowHalf != 0) {
        func_ov003_02098c28(0, bar->unk_3C2);
    }
    if (partnerBelowHalf != 0) {
        func_ov003_02098c28(1, bar->unk_3C0);
    }

    CombatSprite_Update(&bar->sprites[5]);
    CombatSprite_Update(&bar->sprites[6]);
    CombatSprite_Update(&bar->sprites[7]);
    CombatSprite_Update(&bar->sprites[8]);
    CombatSprite_Update(&bar->sprites[9]);
    CombatSprite_Update(&bar->sprites[0]);
    CombatSprite_Update(&bar->sprites[1]);
    CombatSprite_Update(&bar->sprites[2]);
    CombatSprite_Update(&bar->sprites[3]);
    CombatSprite_Update(&bar->sprites[4]);
    return 1;
}

/// Task render.
s32 func_ov003_020995f4(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;

    if (gSaveData.unk_3130 == 1) {
        return 1;
    }

    CombatSprite_SetPosition(&bar->sprites[0], 0xF8, 0);
    CombatSprite_Render(&bar->sprites[0]);

    if (bar->flags & 0x800) {
        CombatSprite_SetPosition(&bar->sprites[1], 0xF8, bar->unk_3C2);
        CombatSprite_Render(&bar->sprites[1]);
    }
    if (bar->flags & 2) {
        CombatSprite_SetPosition(&bar->sprites[4], 0xF8, bar->unk_3C6);
        CombatSprite_Render(&bar->sprites[4]);
    }
    if (bar->flags & 0x10) {
        func_ov003_02082730(&bar->sprites[3], 8);
        CombatSprite_SetPosition(&bar->sprites[3], 0xF8, bar->unk_3CA);
        CombatSprite_Render(&bar->sprites[3]);
    }
    if (bar->flags & 0x20) {
        func_ov003_02082730(&bar->sprites[3], 6);
        CombatSprite_SetPosition(&bar->sprites[3], 0xF8, (s16)(bar->unk_3D2 - 0x50));
        CombatSprite_Render(&bar->sprites[3]);
    }
    if (bar->flags & 0x100) {
        func_ov003_02082730(&bar->sprites[2], 8);
        CombatSprite_SetPosition(&bar->sprites[2], 0xF8, bar->unk_3CE);
        CombatSprite_Render(&bar->sprites[2]);
    }
    if (bar->flags & 0x200) {
        func_ov003_02082730(&bar->sprites[2], 6);
        CombatSprite_SetPosition(&bar->sprites[2], 0xF8, (s16)(bar->unk_3D6 - 0x50));
        CombatSprite_Render(&bar->sprites[2]);
    }

    CombatSprite_SetPosition(&bar->sprites[5], 0xF8, 0xC0);
    CombatSprite_Render(&bar->sprites[5]);

    if (bar->flags & 0x400) {
        CombatSprite_SetPosition(&bar->sprites[6], 0xF8, (s16)(bar->unk_3C0 + 0xC0));
        CombatSprite_Render(&bar->sprites[6]);
    }
    if (bar->flags & 1) {
        CombatSprite_SetPosition(&bar->sprites[9], 0xF8, (s16)(bar->unk_3C4 + 0xC0));
        CombatSprite_Render(&bar->sprites[9]);
    }
    if (bar->flags & 4) {
        func_ov003_02082730(&bar->sprites[8], 8);
        CombatSprite_SetPosition(&bar->sprites[8], 0xF8, (s16)(bar->unk_3C8 + 0xC0));
        CombatSprite_Render(&bar->sprites[8]);
    }
    if (bar->flags & 8) {
        func_ov003_02082730(&bar->sprites[8], 6);
        CombatSprite_SetPosition(&bar->sprites[8], 0xF8, (s16)(bar->unk_3D0 + 0x110));
        CombatSprite_Render(&bar->sprites[8]);
    }
    if (bar->flags & 0x40) {
        func_ov003_02082730(&bar->sprites[7], 8);
        CombatSprite_SetPosition(&bar->sprites[7], 0xF8, (s16)(bar->unk_3CC + 0xC0));
        CombatSprite_Render(&bar->sprites[7]);
    }
    if (bar->flags & 0x80) {
        func_ov003_02082730(&bar->sprites[7], 6);
        CombatSprite_SetPosition(&bar->sprites[7], 0xF8, (s16)(bar->unk_3D4 + 0x110));
        CombatSprite_Render(&bar->sprites[7]);
    }
    return 1;
}

/// Task cleanup.
s32 func_ov003_020998fc(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;

    CombatSprite_Release(&bar->sprites[5]);
    CombatSprite_Release(&bar->sprites[6]);
    CombatSprite_Release(&bar->sprites[9]);
    CombatSprite_Release(&bar->sprites[8]);
    CombatSprite_Release(&bar->sprites[7]);
    CombatSprite_Release(&bar->sprites[0]);
    CombatSprite_Release(&bar->sprites[1]);
    CombatSprite_Release(&bar->sprites[2]);
    CombatSprite_Release(&bar->sprites[3]);
    CombatSprite_Release(&bar->sprites[4]);
    return 1;
}

/// Task dispatcher: calls the stage function for `stage`.
s32 func_ov003_0209995c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7fa4;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3c60[0x10] = "Tsk_BtlBarHP";

const TaskHandle data_ov003_020d7f98 = {data_ov003_020e3c60, func_ov003_0209995c, sizeof(BtlBar)};

const TaskStages data_ov003_020d7fa4 = {
    func_ov003_0209923c,
    func_ov003_02099530,
    func_ov003_020995f4,
    func_ov003_020998fc,
};
