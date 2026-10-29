#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatSprite.h"
#include "EasyFade.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include <nitro/mi/cpumem.h>

// The victory screen (`Tsk_BtlWin`): two banner sprites slide in from the sides of the screen, the partner (or the
// boss, when it has one) calls out a victory line, and after a short wait the banners slide out again, the screen
// fades and the battle ends.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

struct BtlWin;

/// One phase of the screen; it swaps itself out through `func_ov003_020a0f08`.
typedef void (*BtlWinState)(struct BtlWin*);

typedef struct BtlWin {
    /* 0x00 */ CombatSprite sprites[2]; // [0] main screen, [1] sub screen
    /* 0xC0 */ s32          x;          // the main screen banner
    /* 0xC4 */ s32          subX;       // the sub screen banner
    /* 0xC8 */ BtlWinState  state;
    /* 0xCC */ s16          timer;
    /* 0xCE */ s16          phase;
    /* 0xD0 */ s32          running;
} BtlWin; // Size: 0xD4

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern const BinIdentifier data_ov003_020d7818;

extern void func_020265d4(void* dst, s32 step, u16 count);
extern void func_ov003_02082730(CombatSprite* sprite, s32 sortKey);
extern void func_ov003_02082738(CombatSprite* sprite, s32 arg1);
extern void func_ov003_0208b278(s32 arg0);

void func_ov003_020a13f8(BtlWin* self);
void func_ov003_020a147c(BtlWin* self);
void func_ov003_020a14c8(BtlWin* self);
s32  func_ov003_020a15a4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a1608(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a163c(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a16ac(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020a16cc(TaskPool* pool, Task* task, void* args, s32 stage);

char             data_ov003_020e3e18[12] = "Tsk_BtlWin";
const TaskHandle data_ov003_020d8810     = {data_ov003_020e3e18, func_ov003_020a16cc, sizeof(BtlWin)};

/// The stage table of the screen: build, update, draw, release.
const TaskStages data_ov003_020d881c = {
    func_ov003_020a15a4,
    func_ov003_020a1608,
    func_ov003_020a163c,
    func_ov003_020a16ac,
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

void func_ov003_020a0f08(BtlWin* self, BtlWinState state) {
    self->state = state;
    self->phase = 0;
    self->timer = 0;
}

/// Loads a banner sprite for `engine`.
void func_ov003_020a0f1c(s32 engine, CombatSprite* sprite) {
    SpriteAnimationEx anim;

    CombatSprite_InitAnim(&anim.anim, engine, &data_ov003_020d7818);
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_18     = 2;
    anim.anim.packIndex  = 2;
    anim.anim.unk_1C     = 1;
    anim.anim.unk_26     = 2;
    anim.anim.unk_28     = 3;
    anim.anim.unk_20     = 4;
    anim.anim.animIndex  = 1;
    anim.animTable       = NULL;
    CombatSprite_Load(sprite, &anim);
    func_ov003_02082730(sprite, 0);
    func_ov003_02082738(sprite, 0);
}

/// The generic victory line; `weak` picks the line for a hard fought win.
void func_ov003_020a0fac(s32 weak) {
    u16 line;

    if (weak) {
        func_ov003_02087ed8(0x375);
        return;
    }
    line = RNG_Next(0xFFFF) % 3;
    switch (line) {
        case 0:
            func_ov003_02087ed8(0x372);
            break;
        case 1:
            func_ov003_02087ed8(0x373);
            break;
        case 2:
            func_ov003_02087ed8(0x374);
            break;
    }
}

/// The partner's own victory line.
void func_ov003_020a1038(s32 weak) {
    s32 line;

    if (weak) {
        switch (data_ov003_020e71b8->unk3D88D) {
            case 0:
                func_ov003_02087ed8(0x39D);
                break;
            case 1:
                func_ov003_02087ed8(0x3C5);
                break;
            case 2:
                func_ov003_02087ed8(0x3ED);
                break;
        }
        return;
    }
    line = RNG_Next(3);
    if (data_ov003_020e71b8->unk3D88D == 0) {
        switch (line) {
            case 0:
                func_ov003_02087ed8(0x39A);
                break;
            case 1:
                func_ov003_02087ed8(0x39B);
                break;
            case 2:
                func_ov003_02087ed8(0x39C);
                break;
        }
    }
    if (data_ov003_020e71b8->unk3D88D == 1) {
        switch (line) {
            case 0:
                func_ov003_02087ed8(0x3C2);
                break;
            case 1:
                func_ov003_02087ed8(0x3C3);
                break;
            case 2:
                func_ov003_02087ed8(0x3C4);
                break;
        }
    }
    if (data_ov003_020e71b8->unk3D88D == 2) {
        switch (line) {
            case 0:
                func_ov003_02087ed8(0x3EA);
                break;
            case 1:
                func_ov003_02087ed8(0x3EB);
                break;
            case 2:
                func_ov003_02087ed8(0x3EC);
                break;
        }
    }
}

/// Plays the victory line of the boss of this battle. Returns 0 when the battle has no boss line.
s32 func_ov003_020a11c8(void) {
    s32 result = 1;

    switch (data_ov003_020e71b8->unk3D88E) {
        case 13:
            func_ov003_02087ed8(0x4E5);
            break;
        case 16:
            func_ov003_02087ed8(result + 0x500);
            break;
        case 33:
            func_ov003_02087ed8(result + 0x500);
            break;
        case 18:
            func_ov003_02087ed8(0x50A);
            break;
        case 19:
            func_ov003_02087ed8(0x515);
            break;
        case 20:
            func_ov003_02087ed8(0x4DA);
            break;
        case 21:
            func_ov003_02087ed8(0x4DA);
            break;
        case 23:
            func_ov003_02087ed8(0x3E8);
            break;
        case 24:
            func_ov003_02087ed8(0x3E8);
            break;
        case 25:
            func_ov003_02087ed8(0x3E8);
            break;
        case 26:
            func_ov003_02087ed8(0x3E8);
            break;
        case 27:
            func_ov003_02087ed8(0x4D4);
            break;
        case 28:
            func_ov003_02087ed8(0x524);
            break;
        case 29:
            func_ov003_02087ed8(0x53C);
            break;
        case 30:
            if (RNG_Next(2) != 0) {
                func_ov003_02087ed8(0x524);
            } else {
                func_ov003_02087ed8(0x53C);
            }
            break;
        case 31:
            if (RNG_Next(2) != 0) {
                func_ov003_02087ed8(0x530);
            } else {
                func_ov003_02087ed8(0x548);
            }
            break;
        case 32:
            func_ov003_02087ed8(0x4B0);
        default:
            result = 0;
            break;
    }
    return result;
}

/// Picks and plays the victory line.
void func_ov003_020a1374(void) {
    s32 weak = 0;
    s32 other;

    if (func_ov003_020a11c8() != 0) {
        return;
    }
    if (data_ov003_020e71b8->unk3D8D0 < data_ov003_020e71b8->unk3D8D2 / 2) {
        weak = 1;
    }
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
        func_ov003_020a0fac(weak);
    } else {
        func_ov003_020a1038(weak);
    }
}

/// The banners slide in.
void func_ov003_020a13f8(BtlWin* self) {
    if (self->timer == 0) {
        self->timer = 30;
        func_ov003_02087f00(0x16, 0x80);
    }
    if (self->timer > 0) {
        func_020265d4(&self->x, 0x80000, (u16)self->timer);
        func_020265d4(&self->subX, 0x80000, self->timer);
        self->timer--;
    }
    if (self->timer > 0) {
        return;
    }
    func_ov003_020a0f08(self, func_ov003_020a147c);
}

/// The victory line plays while the banners wait.
void func_ov003_020a147c(BtlWin* self) {
    if (self->timer == 0) {
        func_ov003_020a1374();
        self->timer = 100;
    }
    self->timer--;
    if (self->timer > 0) {
        return;
    }
    func_ov003_020a0f08(self, func_ov003_020a14c8);
}

/// The banners slide out and the screen fades.
void func_ov003_020a14c8(BtlWin* self) {
    switch (self->phase) {
        case 0:
            if (self->timer == 0) {
                self->timer = 30;
                func_ov003_02087f00(0x17, 0x80);
            }
            if (self->timer > 0) {
                func_020265d4(&self->x, 0x180000, (u16)self->timer);
                func_020265d4(&self->subX, -0x180000, self->timer);
                self->timer--;
            }
            if (self->timer > 0) {
                return;
            }
            EasyFade_FadeBothDisplays(0, 0x10, 0x800);
            func_ov003_02087ed8(1);
            self->phase++;
            self->timer = 0;
            break;
        case 1:
            if (EasyFade_IsFading()) {
                return;
            }
            func_ov003_0208b278(7);
            self->running = 0;
            break;
    }
}

s32 func_ov003_020a15a4(TaskPool* pool, Task* task, void* args) {
    BtlWin* self = task->data;

    MI_CpuSet(self, 0, sizeof(*self));
    func_ov003_020a0f1c(1, &self->sprites[0]);
    func_ov003_020a0f1c(0, &self->sprites[1]);
    self->x       = -0x180000;
    self->subX    = 0x180000;
    self->running = 1;
    func_ov003_020a0f08(self, func_ov003_020a13f8);
    return 1;
}

s32 func_ov003_020a1608(TaskPool* pool, Task* task, void* args) {
    BtlWin* self = task->data;

    if (self->state != NULL) {
        self->state(self);
    }
    CombatSprite_Update(&self->sprites[0]);
    CombatSprite_Update(&self->sprites[1]);
    return self->running;
}

s32 func_ov003_020a163c(TaskPool* pool, Task* task, void* args) {
    BtlWin* self = task->data;

    CombatSprite_SetPosition(&self->sprites[1], (self->subX * 0x10) >> 16, 0x60);
    CombatSprite_Render(&self->sprites[1]);
    if (data_ov003_020e71b8->unk3D88D == 3) {
        return 1;
    }
    CombatSprite_SetPosition(&self->sprites[0], (self->x * 0x10) >> 16, 0x60);
    CombatSprite_Render(&self->sprites[0]);
    return 1;
}

s32 func_ov003_020a16ac(TaskPool* pool, Task* task, void* args) {
    BtlWin* self = task->data;

    CombatSprite_Release(&self->sprites[0]);
    CombatSprite_Release(&self->sprites[1]);
    return 1;
}

s32 func_ov003_020a16cc(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d881c;

    return stages.iter[stage](pool, task, args);
}
