#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "CriSndMgr.h"
#include "Display.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

// The game over screen (`Tsk_BtlOver`): the battle music stops, two banner sprites slide in, the screen is
// darkened by a blend that closes in from both sides, and the partner (or boss) calls out a line. When it has
// finished the battle ends.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

struct BtlOver;

/// One phase of the screen; it swaps itself out through `func_ov003_020a1714`.
typedef void (*BtlOverState)(struct BtlOver*);

typedef struct BtlOver {
    /* 0x00 */ CombatSprite sprites[2]; // [0] main screen, [1] sub screen
    /* 0xC0 */ s32          x;
    /* 0xC4 */ s32          subX;
    /* 0xC8 */ s32          blend; // how far the blend has closed in, 20.12
    /* 0xCC */ s16          blendTimer;
    /* 0xCE */ u16          reservedCE;
    /* 0xD0 */ s32          dim; // whether the screens are darkened by the blend
    /* 0xD4 */ BtlOverState state;
    /* 0xD8 */ s16          timer;
    /* 0xDA */ s16          phase;
} BtlOver; // Size: 0xDC

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern const BinIdentifier data_ov003_020d7818;

extern u16 data_02066aec[];
extern u16 data_02066eec[];

extern void func_0202fbf4(s32 arg0);
extern void func_02026590(void* dst, s32 step, u16 count);
extern void func_ov003_02082738(CombatSprite* sprite, s32 arg1);
extern void func_ov003_02082730(CombatSprite* sprite, s32 sortKey);
extern void func_ov003_0208b278(s32 arg0);

void func_ov003_020a1c80(BtlOver* self);
void func_ov003_020a1d04(BtlOver* self);
void func_ov003_020a1d78(BtlOver* self);
void func_ov003_020a1db4(BtlOver* self);
void func_ov003_020a1eb4(BtlOver* self);
s32  func_ov003_020a1f50(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a1ff4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a203c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a20a4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a20c4(TaskPool* pool, Task* task, void* args, s32 stage);

// `Tsk_BtlPause` is pooled into the `.data` of this unit.
char data_ov003_020e3e24[0x10] = "Task_BtlOver";
char data_ov003_020e3e34[0x10] = "Tsk_BtlPause";

const TaskHandle data_ov003_020d882c = {data_ov003_020e3e24, func_ov003_020a20c4, sizeof(BtlOver)};

/// The stage table of the screen: build, update, draw, release.
const TaskStages data_ov003_020d8838 = {
    func_ov003_020a1f50,
    func_ov003_020a1ff4,
    func_ov003_020a203c,
    func_ov003_020a20a4,
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

void func_ov003_020a1714(BtlOver* self, BtlOverState state) {
    self->state = state;
    self->phase = 0;
    self->timer = 0;
}

/// Loads a banner sprite for `engine`.
void func_ov003_020a1728(s32 engine, CombatSprite* sprite) {
    SpriteAnimationEx anim;

    CombatSprite_InitAnim(&anim.anim, engine, &data_ov003_020d7818);
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_18     = 2;
    anim.anim.packIndex  = 3;
    anim.anim.unk_1C     = 1;
    anim.anim.unk_26     = 2;
    anim.anim.unk_28     = 3;
    anim.anim.unk_20     = 4;
    anim.anim.animIndex  = 1;
    anim.animTable       = NULL;
    CombatSprite_Load(sprite, &anim);
    CombatSprite_SetAnim(sprite, 0, 1);
    func_ov003_02082738(sprite, 0);
}

/// Plays the line of the boss of this battle. Returns 0 when the battle has no boss line.
s32 func_ov003_020a17bc(void) {
    s32 result = 1;

    switch (data_ov003_020e71b8->unk3D88E) {
        case 10:
            SndMgr_StartPlayingSE(0x4E4);
            break;
        case 11:
            SndMgr_StartPlayingSE(0x4E4);
            break;
        case 12:
            SndMgr_StartPlayingSE(0x4E4);
            break;
        case 13:
            SndMgr_StartPlayingSE(0x4E4);
            break;
        case 14:
            SndMgr_StartPlayingSE(0x4E4);
            break;
        case 15:
            SndMgr_StartPlayingSE(0x4F2);
            break;
        case 16:
            SndMgr_StartPlayingSE(0x500);
            break;
        case 17:
            SndMgr_StartPlayingSE(0x4C3);
            break;
        case 18:
            SndMgr_StartPlayingSE(0x4C3);
            break;
        case 19:
            SndMgr_StartPlayingSE(0x514);
            break;
        case 20:
            SndMgr_StartPlayingSE(0x4D9);
            break;
        case 21:
            SndMgr_StartPlayingSE(0x4D9);
            break;
        case 22:
            SndMgr_StartPlayingSE(0x4D9);
            break;
        case 23:
        case 24:
        case 25:
        case 26:
            if (RNG_Next(2) != 0) {
                SndMgr_StartPlayingSE(0x3EA);
            } else {
                SndMgr_StartPlayingSE(0x3EC);
            }
            break;
        case 27:
            SndMgr_StartPlayingSE(0x4D3);
            break;
        case 28:
            SndMgr_StartPlayingSE(0x523);
            break;
        case 29:
            SndMgr_StartPlayingSE(0x53B);
            break;
        case 30:
            if (RNG_Next(2) != 0) {
                SndMgr_StartPlayingSE(0x523);
            } else {
                SndMgr_StartPlayingSE(0x53B);
            }
            break;
        case 32:
            SndMgr_StartPlayingSE(0x558);
            break;
        default:
            result = 0;
            break;
    }
    return result;
}

/// Picks and plays the line of the screen.
void func_ov003_020a1994(void) {
    s32 line;
    s32 other;

    if (func_ov003_020a17bc() != 0) {
        return;
    }
    line = RNG_Next(2);
    switch (data_ov003_020e71b8->unk3D88D) {
        case 0:
        case 1:
        case 2:
            other = 0;
            break;
        default:
            other = 1;
            break;
    }
    if (other || RNG_Next(2) != 0) {
        switch (line) {
            case 0:
                SndMgr_StartPlayingSE(0x370);
                break;
            case 1:
                SndMgr_StartPlayingSE(0x371);
                break;
        }
        return;
    }
    if (data_ov003_020e71b8->unk3D88D == 0) {
        switch (line) {
            case 0:
                SndMgr_StartPlayingSE(0x398);
                break;
            case 1:
                SndMgr_StartPlayingSE(0x399);
                break;
        }
    }
    if (data_ov003_020e71b8->unk3D88D == 1) {
        switch (line) {
            case 0:
                SndMgr_StartPlayingSE(0x3C0);
                break;
            case 1:
                SndMgr_StartPlayingSE(0x3C1);
                break;
        }
    }
    if (data_ov003_020e71b8->unk3D88D == 2) {
        switch (line) {
            case 0:
                SndMgr_StartPlayingSE(0x3E8);
                break;
            case 1:
                SndMgr_StartPlayingSE(0x3E9);
                break;
        }
    }
}

/// Returns 1 when the screens are darkened (every battle except the last four of the story).
s32 func_ov003_020a1af0(void) {
    switch (data_ov003_020e71b8->unk3D88E) {
        case 19:
        case 20:
        case 21:
        case 22:
            return 0;
    }
    return 1;
}

/// Sets up the blend that darkens both screens.
void func_ov003_020a1b34(BtlOver* self) {
    if (self->dim == 0) {
        self->blendTimer = 0;
        return;
    }
    g_DisplaySettings.controls[0].windows &= ~4;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 0;
    g_DisplaySettings.engineState[0].blendMode              = 1;
    g_DisplaySettings.engineState[0].blendLayer0            = 8;
    g_DisplaySettings.engineState[0].blendLayer1            = 0x37;
    g_DisplaySettings.engineState[0].blendCoeff0            = 8;
    g_DisplaySettings.engineState[0].blendCoeff1            = 8;
    g_DisplaySettings.controls[1].windows &= ~4;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 0;
    g_DisplaySettings.engineState[1].blendMode              = 1;
    g_DisplaySettings.engineState[1].blendLayer0            = 8;
    g_DisplaySettings.engineState[1].blendLayer1            = 0x37;
    g_DisplaySettings.engineState[1].blendCoeff0            = 8;
    g_DisplaySettings.engineState[1].blendCoeff1            = 8;
    self->blend                                             = 0;
    self->blendTimer                                        = 0x50;
}

/// Closes the blend in.
void func_ov003_020a1bbc(BtlOver* self) {
    s32 amount;

    if (self->blendTimer > 0) {
        func_02026590(&self->blend, 0x8000, (u16)self->blendTimer);
        self->blendTimer--;
    }
    amount                                       = self->blend >> 12;
    g_DisplaySettings.engineState[0].blendCoeff0 = 8 - amount;
    g_DisplaySettings.engineState[0].blendCoeff1 = amount + 8;
    g_DisplaySettings.engineState[1].blendCoeff0 = 8 - amount;
    g_DisplaySettings.engineState[1].blendCoeff1 = amount + 8;
    if (self->blendTimer > 0) {
        return;
    }
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[0].blendMode              = 1;
    g_DisplaySettings.engineState[0].blendLayer0            = 1;
    g_DisplaySettings.engineState[0].blendLayer1            = 0x3E;
    g_DisplaySettings.engineState[0].blendCoeff0            = 8;
    g_DisplaySettings.engineState[0].blendCoeff1            = 8;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 3;
    g_DisplaySettings.engineState[1].blendMode              = 1;
    g_DisplaySettings.engineState[1].blendLayer0            = 1;
    g_DisplaySettings.engineState[1].blendLayer1            = 0x3E;
    g_DisplaySettings.engineState[1].blendCoeff0            = 8;
    g_DisplaySettings.engineState[1].blendCoeff1            = 8;
}

/// The music stops and the screen fades.
void func_ov003_020a1c80(BtlOver* self) {
    if (self->timer == 0) {
        self->timer = 30;
        func_0202fbf4(0);
        CriSndMgr_Stop(data_ov003_020e71b8->unk3D870);
        SndMgr_PlaySEWithPan(0x18, 0x80);
        EasyFade_FadeBothDisplays(0, 0, 0x1000);
    }
    self->timer--;
    if (self->timer > 0) {
        return;
    }
    func_ov003_020a1714(self, func_ov003_020a1d04);
}

/// The banners slide in.
void func_ov003_020a1d04(BtlOver* self) {
    if (self->timer == 0) {
        self->timer = 40;
    }
    if (self->timer > 0) {
        func_02026590(&self->x, 0x80000, (u16)self->timer);
        func_02026590(&self->subX, 0x80000, self->timer);
        self->timer--;
    }
    if (self->timer > 0) {
        return;
    }
    func_ov003_020a1714(self, func_ov003_020a1d78);
}

/// The banners wait.
void func_ov003_020a1d78(BtlOver* self) {
    if (self->timer == 0) {
        self->timer = 60;
    }
    self->timer--;
    if (self->timer > 0) {
        return;
    }
    func_ov003_020a1714(self, func_ov003_020a1db4);
}

/// The background layers are switched off.
void func_ov003_020a1db4(BtlOver* self) {
    if (self->timer == 0) {
        self->timer = 120;
        if (self->dim != 0) {
            g_DisplaySettings.controls[0].windows &= ~4;
            g_DisplaySettings.engineState[0].bgSettings[3].priority = 0;
            data_02066aec[0]                                        = 0;
            g_DisplaySettings.controls[0].layers &= ~1;
            g_DisplaySettings.controls[0].layers &= ~2;
            g_DisplaySettings.controls[0].layers &= ~4;
            g_DisplaySettings.controls[1].windows &= ~4;
            g_DisplaySettings.engineState[1].bgSettings[3].priority = 0;
            data_02066eec[0]                                        = 0;
            g_DisplaySettings.controls[1].layers &= ~1;
            g_DisplaySettings.controls[1].layers &= ~2;
            g_DisplaySettings.controls[1].layers &= ~4;
        } else {
            g_DisplaySettings.controls[0].layers = 0x10;
            g_DisplaySettings.controls[1].layers = 0x10;
        }
        data_ov003_020e71b8->unk3D884 = 1;
    }
    self->timer--;
    if (self->timer > 0) {
        return;
    }
    EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
    func_ov003_020a1714(self, func_ov003_020a1eb4);
}

/// The banners switch to their last animation and the battle ends.
void func_ov003_020a1eb4(BtlOver* self) {
    if (self->timer == 0) {
        self->timer = 30;
        EasyFade_FadeBothDisplays(0, 0, 0x1000);
        CombatSprite_SetAnim(&self->sprites[0], 1, 1);
        CombatSprite_SetAnim(&self->sprites[1], 1, 1);
        g_DisplaySettings.controls[0].layers &= ~8;
        g_DisplaySettings.controls[1].layers &= ~8;
    }
    self->timer--;
    if (self->timer > 0) {
        return;
    }
    func_ov003_020a1714(self, NULL);
    func_ov003_0208b278(7);
}

s32 func_ov003_020a1f50(TaskPool* pool, Task* task, void* args) {
    BtlOver* self = task->data;

    MI_CpuSet(self, 0, sizeof(*self));
    data_ov003_020e71b8->unk3D878 |= 0x20;
    func_ov003_020a1728(1, &self->sprites[0]);
    func_ov003_020a1728(0, &self->sprites[1]);
    self->phase = 0;
    self->timer = 0;
    self->x     = -0x180000;
    self->subX  = 0x180000;
    self->dim   = func_ov003_020a1af0();
    EasyFade_FadeBothDisplays(2, 0x10, 0x1000);
    func_ov003_020a1b34(self);
    func_ov003_020a1714(self, func_ov003_020a1c80);
    func_ov003_020a1994();
    return 1;
}

s32 func_ov003_020a1ff4(TaskPool* pool, Task* task, void* args) {
    BtlOver* self = task->data;

    if (self->state != NULL) {
        self->state(self);
    }
    if (self->blendTimer > 0) {
        func_ov003_020a1bbc(self);
    }
    CombatSprite_Update(&self->sprites[0]);
    CombatSprite_Update(&self->sprites[1]);
    return 1;
}

s32 func_ov003_020a203c(TaskPool* pool, Task* task, void* args) {
    BtlOver* self = task->data;

    CombatSprite_SetPosition(&self->sprites[0], (self->x * 0x10) >> 16, 0x60);
    func_ov003_02082730(&self->sprites[0], 0);
    CombatSprite_Render(&self->sprites[0]);
    CombatSprite_SetPosition(&self->sprites[1], (self->subX * 0x10) >> 16, 0x60);
    func_ov003_02082730(&self->sprites[1], 0);
    CombatSprite_Render(&self->sprites[1]);
    return 1;
}

s32 func_ov003_020a20a4(TaskPool* pool, Task* task, void* args) {
    BtlOver* self = task->data;

    CombatSprite_Release(&self->sprites[0]);
    CombatSprite_Release(&self->sprites[1]);
    return 1;
}

s32 func_ov003_020a20c4(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8838;

    return stages.iter[stage](pool, task, args);
}
