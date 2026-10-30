#include "Combat/Core/BtlBar.h"
#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"

#include <nitro/mi/cpumem.h>

// The BS (beat/sync) bar HUD: the partner's sync gauge with its two "banked" segments, drawn with
// the same ten-sprite layout as the HP bar (`BtlBarHP`). The gauge itself lives in `Ov003Global`
// (0x3D8D8 value, 0x3D8DA max, 0x3D8DC/0x3D8DE the two banked segments); this unit splits it into
// the bar's pixel widths and drives the fill/drain animation.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// The BS bar's task data (0x400 bytes): the shared bar layout plus a trailing word.
typedef struct BtlBarBS {
    /* 0x000 */ BtlBar bar;
    /* 0x3FC */ s32    unk_3FC;
} BtlBarBS; // Size: 0x400

// ---------------------------------------------------------------------------------------------
// External functions
// ---------------------------------------------------------------------------------------------

extern const BinIdentifier data_ov003_020d7800;

extern const TaskHandle data_ov003_020d7fb4;
extern const TaskStages data_ov003_020d7fc0;

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Selects the animation of the six gauge sprites: the "filling" set while the gauge is below a
/// quarter, the idle set otherwise.
void func_ov003_020999c4(BtlBar* bar) {
    if (bar->unk_3F8 != 0) {
        CombatSprite_SetAnim(&bar->sprites[6], 1, 0);
        CombatSprite_SetAnim(&bar->sprites[5], 6, 0);
        CombatSprite_SetAnim(&bar->sprites[9], 2, 0);
        CombatSprite_SetAnim(&bar->sprites[1], 1, 0);
        CombatSprite_SetAnim(&bar->sprites[0], 6, 0);
        CombatSprite_SetAnim(&bar->sprites[4], 2, 0);
    } else {
        CombatSprite_SetAnim(&bar->sprites[6], 8, 0);
        CombatSprite_SetAnim(&bar->sprites[5], 0xA, 0);
        CombatSprite_SetAnim(&bar->sprites[9], 0xB, 0);
        CombatSprite_SetAnim(&bar->sprites[1], 8, 0);
        CombatSprite_SetAnim(&bar->sprites[0], 0xA, 0);
        CombatSprite_SetAnim(&bar->sprites[4], 0xB, 0);
    }
}

/// Fills the gauge: spreads the second banked segment over the width that is still free and
/// advances the two sampled segment widths.
// Nonmatching: `oldB` and `bankedA` are allocated to lr/r12 the other way round than the retail
// build; everything else is instruction-identical.
void func_ov003_02099a98(BtlBar* bar) {
    s32 oldB    = bar->unk_3DC;
    s32 oldA    = bar->unk_3DA;
    s32 bankedB = data_ov003_020e71b8->unk3D8DE;
    s32 bankedA = data_ov003_020e71b8->unk3D8DC;
    s32 delta   = (bankedB - oldB) + (bankedA - oldA);
    s32 free    = (bankedB + bankedA) - data_ov003_020e71b8->unk3D8DA * 3 / 4;

    // The gauge width is recomputed per store; the retail build has a division by 4 and then a
    // separate division by 2 for each of the three segments.
    bar->unk_3DC                  = data_ov003_020e71b8->unk3D8DA * 3 / 4 / 2;
    bar->unk_3DA                  = data_ov003_020e71b8->unk3D8DA * 3 / 4 / 2;
    data_ov003_020e71b8->unk3D8DE = data_ov003_020e71b8->unk3D8DA * 3 / 4 / 2;

    if (delta > 0) {
        data_ov003_020e71b8->unk3D8DE += (s16)(free * (bankedB - oldB) / delta);
    }
    data_ov003_020e71b8->unk3D8DC =
        data_ov003_020e71b8->unk3D8DA - data_ov003_020e71b8->unk3D8D8 - data_ov003_020e71b8->unk3D8DE;
}

/// Drains the gauge: the mirror image of `func_ov003_02099a98`.
// Nonmatching: same lr/r12 swap as `func_ov003_02099a98`.
void func_ov003_02099b94(BtlBar* bar) {
    s32 oldB    = bar->unk_3DC;
    s32 oldA    = bar->unk_3DA;
    s32 bankedB = data_ov003_020e71b8->unk3D8DE;
    s32 bankedA = data_ov003_020e71b8->unk3D8DC;
    s32 delta   = (oldB - bankedB) + (oldA - bankedA);
    s32 free    = data_ov003_020e71b8->unk3D8DA * 3 / 4 - (bankedB + bankedA);

    bar->unk_3DC                  = data_ov003_020e71b8->unk3D8DA * 3 / 4 / 2;
    bar->unk_3DA                  = data_ov003_020e71b8->unk3D8DA * 3 / 4 / 2;
    data_ov003_020e71b8->unk3D8DE = data_ov003_020e71b8->unk3D8DA * 3 / 4 / 2;

    if (delta > 0) {
        data_ov003_020e71b8->unk3D8DE -= (s16)(free * (oldB - bankedB) / delta);
    }
    data_ov003_020e71b8->unk3D8DC =
        data_ov003_020e71b8->unk3D8DA - data_ov003_020e71b8->unk3D8D8 - data_ov003_020e71b8->unk3D8DE;
}

/// Clamps both banked segments to at least 3/8 of the gauge's maximum.
/// Clamps both banked segments to at least 3/8 of the gauge's maximum.
void func_ov003_02099c90(void) {
    s32 span    = data_ov003_020e71b8->unk3D8DA * 3 / 4;
    s32 width   = span / 2;
    s32 bankedB = data_ov003_020e71b8->unk3D8DE;

    if (bankedB < width) {
        data_ov003_020e71b8->unk3D8DE = width;
        data_ov003_020e71b8->unk3D8DC -= (s16)(width - bankedB);
    }
    {
        s32 bankedA = data_ov003_020e71b8->unk3D8DC;

        if (bankedA < width) {
            data_ov003_020e71b8->unk3D8DC = width;
            data_ov003_020e71b8->unk3D8DE -= (s16)(width - bankedA);
        }
    }
}

/// Converts a banked segment value to a bar width in pixels, for a gauge that is filling.
s32 func_ov003_02099d24(s16 value) {
    s32 span  = data_ov003_020e71b8->unk3D8DA * 3 / 4;
    s16 local = value - (s16)(span / 2);
    s32 unit  = data_ov003_020e71b8->unk3D8DA / 4;

    if (local > unit) {
        local = unit;
    }
    return (((local << 12) / unit) * 0xA0 + 2 << 4) >> 16;
}

/// Converts a banked segment value to a bar width in pixels.
s32 func_ov003_02099d98(s16 value) {
    s32 span  = data_ov003_020e71b8->unk3D8DA * 3 / 4;
    s16 local = value;

    if (local > span) {
        local = span;
    }
    return (((local << 12) / span) * 0xA0 + 2 << 4) >> 16;
}

/// Task init: seeds the sync gauge from the partner actor and snaps the bar to it.
void func_ov003_02099dec(BtlBarBS* bs, CombatActor* actor) {
    BtlBar* bar = &bs->bar;

    data_ov003_020e71b8->unk3D8D8 = actor->currentHp;
    data_ov003_020e71b8->unk3D8DA = actor->maxHp;
    actor->flags |= 0x8000000;
    if (actor->unk_78 != NULL) {
        ((CombatActor*)actor->unk_78)->flags |= 0x8000000;
    }

    bar->flags   = 0;
    bar->unk_3DA = data_ov003_020e71b8->unk3D8DC;
    bar->unk_3DC = data_ov003_020e71b8->unk3D8DE;

    if (data_ov003_020e71b8->unk3D8D8 <= data_ov003_020e71b8->unk3D8DA / 4) {
        func_ov003_02099a98(bar);
        bar->unk_3F8 = 1;
        bar->unk_3EC = func_ov003_02099d24(data_ov003_020e71b8->unk3D8DC);
        bar->unk_3EE = func_ov003_02099d24(data_ov003_020e71b8->unk3D8DE);
    } else {
        bar->unk_3F8 = 0;
        bar->unk_3EC = func_ov003_02099d98(data_ov003_020e71b8->unk3D8DC);
        bar->unk_3EE = func_ov003_02099d98(data_ov003_020e71b8->unk3D8DE);
    }

    func_ov003_020999c4(bar);
    bar->unk_3E4 = bar->unk_3EC << 12;
    bar->unk_3E8 = bar->unk_3EE << 12;
    bar->unk_3DE = 0;
    bar->unk_3E0 = 0;
    bs->unk_3FC  = 0;
}

/// Per-frame bar state machine: runs the fill/clamp/drain helper for the current gauge value and
/// publishes the segment widths and visibility flags the render stage consumes.
// Nonmatching: the two width selects keep `fixed` and the sampled width in the opposite scratch
// registers than the retail build (r0/r1); the logic and instruction count are identical.
void func_ov003_02099f40(BtlBar* bar) {
    s32 width;
    s32 target;
    s32 fixed;
    s32 other;

    bar->flags = 0;
    if (bar->unk_3F8 == 0) {
        if (data_ov003_020e71b8->unk3D8D8 <= data_ov003_020e71b8->unk3D8DA / 4) {
            func_ov003_02099a98(bar);
            bar->unk_3F8 = 1;
            bar->unk_3EC = 0;
            bar->unk_3EE = 0;
            bar->unk_3E4 = 0;
            bar->unk_3E8 = 0;
            bar->unk_3C0 = 0;
            bar->unk_3C2 = 0;
            func_ov003_020999c4(bar);
        }
    } else if (data_ov003_020e71b8->unk3D8D8 > data_ov003_020e71b8->unk3D8DA / 4) {
        func_ov003_02099b94(bar);
        bar->unk_3F8 = 0;
        bar->unk_3EC = func_ov003_02099d98(data_ov003_020e71b8->unk3D8DC);
        bar->unk_3EE = func_ov003_02099d98(data_ov003_020e71b8->unk3D8DE);
        bar->unk_3E4 = bar->unk_3EC << 12;
        bar->unk_3E8 = bar->unk_3EE << 12;
        bar->unk_3C0 = 0;
        bar->unk_3C2 = 0;
        func_ov003_020999c4(bar);
    } else {
        func_ov003_02099c90();
    }

    if (bar->unk_3DA < data_ov003_020e71b8->unk3D8DC) {
        bar->unk_3DE = 0x1E;
        bar->unk_3E4 = bar->unk_3EC << 12;
        bar->unk_3F0 = 1;
    } else if (bar->unk_3DA > data_ov003_020e71b8->unk3D8DC) {
        bar->unk_3DE = 0x1E;
        bar->unk_3E4 = bar->unk_3EC << 12;
        bar->unk_3F0 = 2;
    } else if (bar->unk_3DE <= 0) {
        bar->unk_3F0 = 0;
    }
    if (bar->unk_3DC < data_ov003_020e71b8->unk3D8DE) {
        bar->unk_3E0 = 0x1E;
        bar->unk_3E8 = bar->unk_3EE << 12;
        bar->unk_3F4 = 1;
    } else if (bar->unk_3DC > data_ov003_020e71b8->unk3D8DE) {
        bar->unk_3E0 = 0x1E;
        bar->unk_3E8 = bar->unk_3EE << 12;
        bar->unk_3F4 = 2;
    } else if (bar->unk_3E0 <= 0) {
        bar->unk_3F4 = 0;
    }

    bar->unk_3DA = data_ov003_020e71b8->unk3D8DC;
    bar->unk_3DC = data_ov003_020e71b8->unk3D8DE;

    if (bar->unk_3F8 != 0) {
        bar->unk_3EC = func_ov003_02099d24(data_ov003_020e71b8->unk3D8DC);
    } else {
        bar->unk_3EC = func_ov003_02099d98(data_ov003_020e71b8->unk3D8DC);
    }
    fixed = (bar->unk_3E4 << 4) >> 16;

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

    if (bar->unk_3F8 != 0) {
        bar->unk_3EE = func_ov003_02099d24(data_ov003_020e71b8->unk3D8DE);
    } else {
        bar->unk_3EE = func_ov003_02099d98(data_ov003_020e71b8->unk3D8DE);
    }
    fixed = (bar->unk_3E8 << 4) >> 16;

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

    if (data_ov003_020e71b8->unk3D8D8 > 0) {
        return;
    }
    bar->flags &= ~0x400;
    bar->flags &= ~0x800;
}

/// Task init: loads the ten bar sprites, then seeds the gauge from the partner actor in `args`.
s32 func_ov003_0209a474(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;

    MI_CpuSet(bar, 0, sizeof(BtlBarBS));

    CombatSprite_LoadDirect(1, &bar->sprites[5], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 6);
    CombatSprite_LoadDirect(1, &bar->sprites[6], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 1);
    CombatSprite_LoadDirect(1, &bar->sprites[9], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 2);
    CombatSprite_LoadDirect(1, &bar->sprites[8], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 3);
    CombatSprite_LoadDirect(1, &bar->sprites[7], &data_ov003_020d7800, 0x2E, 0x30, 0x2F, 0x3F, 4);
    CombatSprite_LoadDirect(0, &bar->sprites[0], &data_ov003_020d7800, 0x2B, 0x2D, 0x2C, 0x3D, 6);
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

    func_ov003_02099dec((BtlBarBS*)bar, args);
    func_ov003_020999c4(bar);
    return 1;
}

/// Task update.
s32 func_ov003_0209a77c(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;

    func_ov003_02099f40(bar);
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
s32 func_ov003_0209a7e4(TaskPool* pool, Task* task, void* args) {
    BtlBar* bar = task->data;

    CombatSprite_SetPosition(&bar->sprites[5], 8, 0xC0);
    CombatSprite_Render(&bar->sprites[5]);

    if (bar->flags & 0x400) {
        CombatSprite_SetPosition(&bar->sprites[6], 8, (s16)(bar->unk_3C0 + 0xC0));
        CombatSprite_Render(&bar->sprites[6]);
    }
    if (bar->flags & 1) {
        CombatSprite_SetPosition(&bar->sprites[9], 8, (s16)(bar->unk_3C4 + 0xC0));
        CombatSprite_Render(&bar->sprites[9]);
    }
    if (bar->flags & 4) {
        func_ov003_02082730(&bar->sprites[8], 8);
        CombatSprite_SetPosition(&bar->sprites[8], 8, (s16)(bar->unk_3C8 + 0xC0));
        CombatSprite_Render(&bar->sprites[8]);
    }
    if (bar->flags & 8) {
        func_ov003_02082730(&bar->sprites[8], 6);
        CombatSprite_SetPosition(&bar->sprites[8], 8, (s16)(bar->unk_3D0 + 0x110));
        CombatSprite_Render(&bar->sprites[8]);
    }
    if (bar->flags & 0x40) {
        func_ov003_02082730(&bar->sprites[7], 8);
        CombatSprite_SetPosition(&bar->sprites[7], 8, (s16)(bar->unk_3CC + 0xC0));
        CombatSprite_Render(&bar->sprites[7]);
    }
    if (bar->flags & 0x80) {
        func_ov003_02082730(&bar->sprites[7], 6);
        CombatSprite_SetPosition(&bar->sprites[7], 8, (s16)(bar->unk_3D4 + 0x110));
        CombatSprite_Render(&bar->sprites[7]);
    }

    CombatSprite_SetPosition(&bar->sprites[0], 8, 0);
    CombatSprite_Render(&bar->sprites[0]);

    if (bar->flags & 0x800) {
        CombatSprite_SetPosition(&bar->sprites[1], 8, bar->unk_3C2);
        CombatSprite_Render(&bar->sprites[1]);
    }
    if (bar->flags & 2) {
        CombatSprite_SetPosition(&bar->sprites[4], 8, bar->unk_3C6);
        CombatSprite_Render(&bar->sprites[4]);
    }
    if (bar->flags & 0x10) {
        func_ov003_02082730(&bar->sprites[3], 8);
        CombatSprite_SetPosition(&bar->sprites[3], 8, bar->unk_3CA);
        CombatSprite_Render(&bar->sprites[3]);
    }
    if (bar->flags & 0x20) {
        func_ov003_02082730(&bar->sprites[3], 6);
        CombatSprite_SetPosition(&bar->sprites[3], 8, (s16)(bar->unk_3D2 - 0x50));
        CombatSprite_Render(&bar->sprites[3]);
    }
    if (bar->flags & 0x100) {
        func_ov003_02082730(&bar->sprites[2], 8);
        CombatSprite_SetPosition(&bar->sprites[2], 8, bar->unk_3CE);
        CombatSprite_Render(&bar->sprites[2]);
    }
    if (bar->flags & 0x200) {
        func_ov003_02082730(&bar->sprites[2], 6);
        CombatSprite_SetPosition(&bar->sprites[2], 8, (s16)(bar->unk_3D6 - 0x50));
        CombatSprite_Render(&bar->sprites[2]);
    }
    return 1;
}

/// Task cleanup.
s32 func_ov003_0209aad4(TaskPool* pool, Task* task, void* args) {
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
s32 func_ov003_0209ab34(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7fc0;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}

/// Spawns the BS bar task for the actor that just entered the battle.
void func_ov003_0209ab9c(CombatActor* actor) {
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7fb4, NULL, 0, NULL, actor);
}

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

char data_ov003_020e3c70[0x10] = "Tsk_BtlBarBS";

const TaskHandle data_ov003_020d7fb4 = {data_ov003_020e3c70, func_ov003_0209ab34, sizeof(BtlBarBS)};

const TaskStages data_ov003_020d7fc0 = {
    func_ov003_0209a474,
    func_ov003_0209a77c,
    func_ov003_0209a7e4,
    func_ov003_0209aad4,
};
