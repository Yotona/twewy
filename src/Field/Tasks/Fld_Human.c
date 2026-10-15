#include "Field/TaskHelpers.h"
// USA 0x020b4920-0x020b5c58: the human/NPC field wander tasks. Positions are
// fixed-point (19.12); object field widths come from the ARM loads.
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "Save.h"
#include "SpriteMgr.h"

#include "Field/FldHuman.h"
#include "common_data.h"

// ---- externs without headers -------------------------------------------------
extern s32 FX_Sqrt(s32 x);
// Retail re-truncates the result at the call site, so this TU did not see the
// SDK's u16 return type.
extern s32 FX_Atan2Idx(s32 y, s32 x);

extern s32   func_ov030_020aeca4(s32 arg0);
extern s32   func_ov030_020af01c(void);
extern s32   func_ov030_020af2e4(void);
extern void  func_ov030_020af4d8(s32 arg0, s32 arg1);
extern void  func_ov030_020af4fc(void);
extern void  func_ov030_020af51c(void);
extern void  func_ov030_020af564(FieldPos* pos);
extern void* func_ov030_020af5d4(void);
extern s16   func_ov030_020af944(s32 arg0);
extern s16   func_ov030_020abe64(u32 arg0);
extern u16   func_ov030_020ac144(s32 arg0, s32 arg1);
extern s32   func_ov030_020be7fc(TaskPool* pool, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7,
                                 Task* human);
extern void  func_ov030_020c6498(Sprite** sprites, s32 arg1, s32 arg2);
extern void  func_ov030_020c6aa4(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, OamAffineParam* affine);
extern s32   func_ov030_020cb72c(FieldPos* pos);

// ---- shared data -------------------------------------------------------------
extern s16 data_0205e4e0[];

// ---- owned rodata ------------------------------------------------------------
const u16         data_ov030_020e0918[1] = {0};
const FieldPos    data_ov030_020e091c    = {0x8000, 0x3C000};
extern const char data_ov030_02106548[];

s32 func_ov030_020b49ac(TaskPool*, Task*, void*);
s32 func_ov030_020b5684(TaskPool*, Task*, void*);
s32 func_ov030_020b5a34(TaskPool*, Task*, void*);
s32 func_ov030_020b5b30(TaskPool*, Task*, void*);
s32 func_ov030_020b5b54(TaskPool*, Task*, void*, s32);

const TaskHandle data_ov030_020e0924 = {data_ov030_02106548, func_ov030_020b5b54, 0xB0};

const TaskStages data_ov030_020e0930 = {
    {func_ov030_020b49ac, func_ov030_020b5684, func_ov030_020b5a34, func_ov030_020b5b30}
};

// ---- functions ---------------------------------------------------------------
s32  func_ov030_020b4920(s32 arg0);
void func_ov030_020b4980(FldHuman* arg0);
s32  func_ov030_020b49ac(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b4bf8(FieldPos pos);
s32  func_ov030_020b4ce8(void* arg0);
void func_ov030_020b4d5c(s32* arg0, s32* arg1, u32 arg2, s32 arg3);
void func_ov030_020b4dd0(FldHuman* arg0);
void func_ov030_020b4dfc(FldHuman* arg0);
s32  func_ov030_020b50ac(FldHuman* arg0);
void func_ov030_020b5228(FldHuman* arg0);
void func_ov030_020b5478(FldHuman* arg0);
void func_ov030_020b54b4(TaskPool* pool, FldHuman* arg1);
void func_ov030_020b55f8(TaskPool* pool, FldHuman* arg1, Task* arg2);
s32  func_ov030_020b5684(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b580c(FldHuman* arg0);
void func_ov030_020b5824(FldHuman* arg0, PaletteResource* arg1);
void func_ov030_020b5990(FldHuman* arg0);
s32  func_ov030_020b5a34(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b5b30(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b5b54(TaskPool* pool, Task* task, void* args, s32 stage);
s32  func_ov030_020b5b9c(TaskPool* pool, FieldPos* origin, FieldPos start, s32 arg4, s32 dataType, FldSpawnTemplate tpl,
                         s32 arg7, s32 speed, s32 spawns, s32 angle, s16 arg11, s16 route, FieldPos target);

s32 func_ov030_020b4920(s32 arg0) {
    u16 v = (u16)arg0;

    if ((s32)v < 0x3000) {
        return 1;
    }
    if ((s32)v < 0x5000) {
        return 3;
    }
    if ((s32)v < 0x838E) {
        return 1;
    }
    if ((s32)v < 0xB000) {
        return 2;
    }
    if ((s32)v < 0xD000) {
        return 4;
    }
    if ((s32)v < 0xFC72) {
        return 2;
    }
    return 1;
}

void func_ov030_020b4980(FldHuman* arg0) {
    if (arg0->init.angle < 0x4000 || arg0->init.angle > 0xC000) {
        arg0->flip = 1;
        return;
    }
    arg0->flip = 0;
}

s32 func_ov030_020b49ac(TaskPool* pool, Task* task, void* args) {
    FldHuman* obj = task->data;
    s32       r2;

    obj->init   = *(FldHumanInit*)args;
    obj->unk_84 = 0;
    obj->pos.x  = obj->init.start.x;
    obj->pos.y  = obj->init.start.y;
    obj->scale  = func_ov030_020aeca4(obj->pos.y);
    obj->unk_88 = -1;
    obj->margin = &data_ov030_020e091c;
    obj->unk_68 = 0;
    obj->flip   = 0;
    if (obj->init.tpl.unk_0A == 0 || obj->init.tpl.unk_0A == 8 || obj->init.tpl.unk_0A == 3) {
        obj->flip = 0;
        if (RNG_Next(0x100) < 0x80U) {
            obj->flip = 1;
        }
    } else if (obj->init.tpl.unk_0A == 2 || obj->init.tpl.unk_0A == 5) {
        obj->flip = 1;
    } else if (obj->init.tpl.unk_0A == 1 || obj->init.tpl.unk_0A == 4) {
        obj->flip = 0;
    } else if (obj->init.tpl.unk_0A == 9) {
        obj->unk_AC = func_ov030_020b5228;
        func_ov030_020b4980(obj);
    } else if (obj->init.tpl.unk_0A == 10) {
        obj->unk_AC = func_ov030_020b4dfc;
        func_ov030_020b4980(obj);
    }
    if (obj->init.unk_30 == -1) {
        obj->unk_70 = 0;
    } else {
        obj->unk_70 = 1;
        if (obj->init.tpl.unk_0A < 9) {
            r2 = 0;
            if ((u32)(u16)(s16)(obj->init.tpl.unk_0A - 4) <= 1U) {
                r2 = 5;
            }
        } else {
            if (gSaveData.unk_2488 == 0) {
                if (SysControl.buttonState.currButtons & 0x800) {
                    obj->init.angle = 0xC000;
                } else if (SysControl.buttonState.currButtons & 2) {
                    obj->init.angle = 0x4000;
                } else if (SysControl.buttonState.currButtons & 0x100) {
                    obj->init.angle = 0;
                    obj->flip       = 1;
                } else if (SysControl.buttonState.currButtons & 0x200) {
                    obj->init.angle = 0x8000;
                    obj->flip       = 0;
                }
            }
            r2 = func_ov030_020b4920(obj->init.angle);
        }
        if (gSaveData.unk_2488 == 0 && (SysControl.buttonState.currButtons & 0x300) == 0x300) {
            obj->init.tpl.unk_0A = 0;
            r2                   = 5;
        }
        func_ov030_020c6498(&obj->unk_98, obj->init.unk_30, r2);
        obj->oamAttr2        = data_ov030_020e0918[0];
        obj->affine.rotation = 0;
        obj->affine.scaleX   = 0x1000;
        obj->affine.scaleY   = 0x1000;
        obj->affine.unk_0C   = 0;
        obj->affine.unk_0E   = 0;
        obj->unk_94          = (s16)(RNG_Next(0x258) + 0x258);
    }
    return 1;
}

// The two distance guards are a single `||`, which makes the clamp assignment
// the fall-through.
s32 func_ov030_020b4bf8(FieldPos pos) {
    s32 r4 = 0;
    s32 r5;
    s32 r6;
    s32 r5b;
    s32 r7;

    if (func_ov030_020aefbc() != 0) {
        r5  = (pos.y - (func_ov030_020aeca4(pos.y) * 0x2C)) - 0x8000;
        r6  = gSaveData.fieldPos.x - pos.x;
        r5b = (gSaveData.fieldPos.y - (func_ov030_020aeca4(gSaveData.fieldPos.y) * 0x2C)) - r5;
        r7  = func_ov030_020af2b8();
        if ((r6 < 0 ? -r6 : r6) >= 0x100000 || (r5b < 0 ? -r5b : r5b) >= 0xC0000) {
            return 0;
        }
        if (FX_Sqrt((s32)(((s64)r6 * r6 + 0x800) >> 0xC) + (s32)(((s64)r5b * r5b + 0x800) >> 0xC)) <= r7) {
            r4 = 0x1000;
        }
    }
    return r4;
}

s32 func_ov030_020b4ce8(void* arg0) {
    FldHuman* obj = arg0;
    s32       dx;
    s32       dy;
    s32       ax;
    s32       ay;

    dx = (obj->pos.x >> 0xC) - (gSaveData.fieldPos.x >> 0xC);
    dy = (obj->pos.y >> 0xC) - (gSaveData.fieldPos.y >> 0xC);
    ax = dx < 0 ? -dx : dx;
    if (ax > 0x20) {
        return 0;
    }
    ay = dy < 0 ? -dy : dy;
    if (ay > 0x20) {
        return 0;
    }
    if ((s32)((dx * dx + dy * dy) << 0xC) < (s32)(obj->scale << 0xA)) {
        return 1;
    }
    return 0;
}
// Nonmatching: register allocation / instruction scheduling of the two smull chains (USA 63.28%; JP unvalidated).
void func_ov030_020b4d5c(s32* arg0, s32* arg1, u32 arg2, s32 arg3) {
    s32 idx = ((s32)(u16)arg2 >> 4) * 2;
    s32 dx  = (s32)(((s64)data_0205e4e0[idx + 1] * arg3 + 0x800) >> 0xC);
    s32 dy  = (s32)(((s64)data_0205e4e0[idx] * arg3 + 0x800) >> 0xC);

    *arg0 += dx;
    *arg1 += dy;
}

void func_ov030_020b4dd0(FldHuman* arg0) {
    arg0->screen.x = arg0->pos.x - arg0->init.origin->x;
    arg0->screen.y = arg0->pos.y - arg0->init.origin->y;
}

void func_ov030_020b4dfc(FldHuman* arg0) {
    s32 r4 = 0;
    s32 v;
    s32 floor;

    func_ov030_020b4d5c(&arg0->pos.x, &arg0->pos.y, (u32)arg0->init.angle,
                        (s32)(((s64)arg0->init.speed * arg0->scale + 0x800) >> 0xC));
    if (arg0->screen.x < -(arg0->margin->x)) {
        arg0->pos.x += arg0->margin->x + 0x100000;
        r4 = 1;
    } else if (arg0->screen.x - arg0->margin->x >= 0x100000) {
        arg0->pos.x -= arg0->margin->x + 0x100000;
        r4 = 1;
    }
    v     = func_ov030_020b7c9c(arg0->pos.x, arg0->pos.y);
    floor = arg0->init.origin->y;
    if (floor > v) {
        v = floor;
    }
    if (arg0->init.angle >= 0 && arg0->init.angle <= 0x8000) {
        if (arg0->screen.y >= arg0->margin->y + 0xC0000) {
            arg0->pos.y = v;
            r4          = 1;
        } else if (arg0->pos.y < v) {
            arg0->pos.y = arg0->init.origin->y + 0x90000 + arg0->margin->y;
            r4          = 1;
            if (arg0->init.angle < 0x4000) {
                arg0->pos.x = arg0->init.origin->x - arg0->margin->x;
            } else {
                arg0->pos.x = arg0->init.origin->x + 0x80000 + arg0->margin->x;
            }
        }
    }
    if ((r4 != 0 || arg0->init.angle < 0 || arg0->init.angle > 0x8000) && arg0->pos.y < v) {
        arg0->pos.y = arg0->init.origin->y + 0xC0000 + arg0->margin->y;
        r4          = 1;
    }
    func_ov030_020b4dd0(arg0);
    if (r4 == 0) {
        return;
    }
    if (func_0202599c(9) == 0 && func_ov030_020af944(0) < 6 && RNG_Next(0x100) < 0x40U) {
        if (func_ov030_020abe64((u16)arg0->init.tpl.unk_10) != 0) {
            gSaveData.unk_311E -= 1;
        }
        {
            s32 t                 = func_ov030_020ac144(arg0->init.unk_30, 4);
            arg0->init.tpl.unk_10 = t;
            arg0->init.tpl.unk_0E = (s16)(t + 1);
        }
        return;
    }
    if (arg0->init.tpl.unk_10 != -2) {
        func_ov030_020af944(-1);
        if (func_ov030_020abe64((u16)arg0->init.tpl.unk_10) != 0) {
            gSaveData.unk_311E -= 1;
        }
        arg0->init.tpl.unk_10 = -2;
        arg0->init.tpl.unk_0E = -2;
    }
    if (func_0202599c(9) != 0) {
        return;
    }
    if (func_ov030_020af944(0) >= 4) {
        return;
    }
    if (func_ov030_020abe64((u16)arg0->init.tpl.unk_10) != 0) {
        gSaveData.unk_311E -= 1;
    }
    {
        s32 t                 = func_ov030_020ac144(arg0->init.unk_30, 4);
        arg0->init.tpl.unk_10 = t;
        arg0->init.tpl.unk_0E = (s16)(t + 1);
    }
}

s32 func_ov030_020b50ac(FldHuman* arg0) {
    u16 a = (u16)arg0->init.angle;

    arg0->init.angle = (s32)a;
    if ((s32)a < 0x1000) {
        if (arg0->init.target.x > arg0->pos.x)
            goto zero;
        return 1;
    }
    if ((s32)a < 0x3000) {
        if (arg0->init.target.x <= arg0->pos.x)
            return 1;
        if (arg0->init.target.y > arg0->pos.y)
            goto zero;
        return 1;
    }
    if ((s32)a < 0x5000) {
        if (arg0->init.target.y > arg0->pos.y)
            goto zero;
        return 1;
    }
    if ((s32)a < 0x7000) {
        if (arg0->init.target.x >= arg0->pos.x)
            return 1;
        if (arg0->init.target.y > arg0->pos.y)
            goto zero;
        return 1;
    }
    if ((s32)a < 0x9000) {
        if (arg0->init.target.x < arg0->pos.x)
            goto zero;
        return 1;
    }
    if ((s32)a < 0xB000) {
        if (arg0->init.target.x >= arg0->pos.x)
            return 1;
        if (arg0->init.target.y < arg0->pos.y)
            goto zero;
        return 1;
    }
    if ((s32)a < 0xD000) {
        if (arg0->init.target.y < arg0->pos.y)
            goto zero;
        return 1;
    }
    if ((s32)a < 0xF000) {
        if (arg0->init.target.x <= arg0->pos.x)
            return 1;
        if (arg0->init.target.y < arg0->pos.y)
            goto zero;
        return 1;
    }
    if (arg0->init.target.x > arg0->pos.x)
        goto zero;
    return 1;
zero:
    return 0;
}

// Nonmatching: store of unk_40 plus the trailing (u16) shift go through r1 in
// the target, ours self-shifts r0 (USA 99.93%; JP unvalidated).
void func_ov030_020b5228(FldHuman* arg0) {
    s16 state;

    func_ov030_020b4d5c(&arg0->pos.x, &arg0->pos.y, (u32)arg0->init.angle,
                        (s32)(((s64)arg0->init.speed * arg0->scale + 0x800) >> 0xC));
    if (func_ov030_020b50ac(arg0) == 0) {
        func_ov030_020b4dd0(arg0);
        return;
    }
    state = arg0->init.route;
    if (state >= 0) {
        FldHumanSpawnEntry* e = &arg0->init.spawns[state];
        s32                 v;

        arg0->init.target.x = (e->x1 + RNG_Next(e->w1)) << 0xC;
        arg0->init.target.y = (e->y1 + RNG_Next(e->h1)) << 0xC;
        arg0->pos.x         = (e->x0 + RNG_Next(e->w0)) << 0xC;
        v                   = RNG_Next(e->h0);
        arg0->pos.y         = (e->y0 + v) << 0xC;
    } else {
        FldHumanSpawnEntry* e = &arg0->init.spawns[-1 - state];
        s32                 v;

        arg0->init.target.x = (e->x0 + RNG_Next(e->w0)) << 0xC;
        arg0->init.target.y = (e->y0 + RNG_Next(e->h0)) << 0xC;
        arg0->pos.x         = (e->x1 + RNG_Next(e->w1)) << 0xC;
        v                   = RNG_Next(e->h1);
        arg0->pos.y         = (e->y1 + v) << 0xC;
    }
    arg0->init.angle = (u16)FX_Atan2Idx(arg0->init.target.y - arg0->pos.y, arg0->init.target.x - arg0->pos.x);
    func_ov030_020c6498(&arg0->unk_98, arg0->init.unk_30, func_ov030_020b4920(arg0->init.angle));
    func_ov030_020b4980(arg0);
    func_ov030_020b4dd0(arg0);
    if (func_0202599c(9) == 0 && func_ov030_020af944(0) < 0xF && RNG_Next(0x100) < 0x40U) {
        if (func_ov030_020abe64((u16)arg0->init.tpl.unk_10) != 0) {
            gSaveData.unk_311E -= 1;
        }
        {
            s32 t                 = func_ov030_020ac144(arg0->init.unk_30, 0xF);
            arg0->init.tpl.unk_10 = t;
            arg0->init.tpl.unk_0E = (s16)(t + 1);
        }
        return;
    }
    if (arg0->init.tpl.unk_10 != -2) {
        func_ov030_020af944(-1);
        if (func_ov030_020abe64((u16)arg0->init.tpl.unk_10) != 0) {
            gSaveData.unk_311E -= 1;
        }
    }
    arg0->init.tpl.unk_10 = -2;
    arg0->init.tpl.unk_0E = -2;
}

void func_ov030_020b5478(FldHuman* arg0) {
    if (arg0->init.tpl.unk_0A < 9 || func_ov030_020af01c() == 0) {
        func_ov030_020b4dd0(arg0);
        return;
    }
    arg0->unk_AC(arg0);
}

// The distance guard is written negated (`>= || >=`), so the target's `bge`/`blt`
// pair falls through into the clamp assignment and branches to the smull path.
void func_ov030_020b54b4(TaskPool* pool, FldHuman* arg1) {
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s32 dist;
    s32 scale;
    s32 mode;

    if (arg1->init.tpl.unk_0C == 0) {
        if ((u32)arg1->unk_88 == -1U) {
            return;
        }
        EasyTask_DeleteTask(pool, (u32)arg1->unk_88);
        arg1->unk_88 = -1;
        return;
    }
    dx  = arg1->pos.x - gSaveData.fieldPos.x;
    dy  = arg1->pos.y - gSaveData.fieldPos.y;
    adx = dx < 0 ? -dx : dx;
    if (adx >= 0x100000 || (dy < 0 ? -dy : dy) >= 0xC0000) {
        dist = 0x10000000;
    } else {
        dist = (s32)(((s64)dx * dx + 0x800) >> 0xC) + (s32)(((s64)dy * dy + 0x800) >> 0xC);
    }

    if (dist > (arg1->scale << 0xC)) {
        if ((u32)arg1->unk_88 == -1U) {
            return;
        }
        EasyTask_DeleteTask(pool, (u32)arg1->unk_88);
        arg1->unk_88 = -1;
        return;
    }
    if ((u32)arg1->unk_88 != -1U) {
        return;
    }
    scale = arg1->margin->y - 0x4000;
    mode  = 1;
    if (arg1->init.unk_30 == -1 || arg1->init.unk_30 == 0xCD) {
        scale = 0;
        mode  = 6;
    }
    arg1->unk_88 = func_ov030_020bacb0(pool, arg1->init.dataType, &arg1->screen, &arg1->scale, scale, mode);
}

void func_ov030_020b55f8(TaskPool* pool, FldHuman* arg1, Task* arg2) {
    s32 v = 1;
    s32 dy;

    if (arg1->init.tpl.unk_0A == 3) {
        v = 5;
    }
    dy           = arg1->screen.y - (func_ov030_020aeca4(arg1->pos.y) << 5);
    arg1->unk_64 = func_ov030_020be7fc(pool, arg1->init.dataType, (arg1->screen.x * 0x10) >> 0x10, (dy * 0x10) >> 0x10, v,
                                       arg1->init.tpl.unk_10, (s32)arg1->init.tpl.unk_0E, arg1->init.unk_30, arg2);
    func_ov030_020af51c();
    arg1->unk_68 = 1;
}

s32 func_ov030_020b5684(TaskPool* pool, Task* task, void* args) {
    FldHuman* obj = task->data;
    s32       r0;

    func_ov030_020b5478(obj);
    obj->scale = func_ov030_020aeca4(obj->pos.y);
    func_ov030_020b54b4(pool, obj);
    r0          = func_ov030_020b4bf8(obj->pos);
    obj->unk_8C = r0;
    if (r0 > 0x29) {
        if (obj->unk_88 != -1) {
            func_ov030_020bacf8(EasyTask_GetTaskById(pool, (u32)obj->unk_88));
        }
        if (obj->unk_68 == 0 && func_ov030_020af2e4() == 1) {
            if (obj->init.unk_30 != -1 && obj->init.unk_30 != 0xCD) {
                func_ov030_020b55f8(pool, obj, task);
            }
        }
    } else {
        if (obj->unk_88 != -1) {
            func_ov030_020bad3c(EasyTask_GetTaskById(pool, (u32)obj->unk_88));
        }
        obj->unk_68 = 0;
        if (obj->init.tpl.unk_0A == 8 && func_ov030_020b4ce8(obj) != 0) {
            obj->init.tpl.unk_0A = 0;
            func_ov030_020af4d8(obj->init.tpl.unk_0C, obj->init.unk_0C);
        }
    }
    if (obj->unk_70 != 0) {
        s32 v;
        obj->affine.scaleX = obj->scale;
        obj->affine.scaleY = obj->scale;
        if (obj->flip != 0) {
            obj->affine.scaleX = obj->affine.scaleX * 0xFFFFFFFFu;
        }
        if (gSaveData.unk_2498 == 0 && obj->init.tpl.unk_0A == 0) {
            obj->unk_94 = (s16)(obj->unk_94 - 1);
            if (obj->unk_94 == 0) {
                obj->unk_94 = (s16)(RNG_Next(0x258) + 0x258);
                if (obj->flip == 0) {
                    v = 1;
                } else {
                    v = 0;
                }
                obj->flip = v;
            }
        }
    }
    return 1;
}

s32 func_ov030_020b580c(FldHuman* arg0) {
    if (arg0->init.unk_30 != -1) {
        return 1;
    }
    return 0;
}

// Nonmatching: retail reads the stack attr2 copy twice for the palette update
// and reloads unk_9C after it (the copy looks address-taken); ours reads it
// once and keeps unk_9C live (USA 85.65%; JP unvalidated).
void func_ov030_020b5824(FldHuman* arg0, PaletteResource* arg1) {
    union {
        u16 attr2;
        struct {
            u16 charName : 10;
            u16 priority : 2;
            u16 palette  : 4;
        };
    } attr;
    s32     y;
    s32     x;
    Sprite* ref;
    s32     extra;
    s32     cellArg;

    if (arg0->unk_9C == NULL && arg0->unk_98 == NULL) {
        return;
    }
    if (arg1 == NULL) {
        return;
    }
    x          = arg0->screen.x;
    y          = arg0->screen.y;
    attr.attr2 = arg0->oamAttr2;
    attr.palette += arg1->palIndex;
    extra = 0;
    if (arg1->slotType == 6) {
        extra = 0x200;
    }
    ref = arg0->unk_9C;
    if (ref != NULL) {
        if (ref->bits_3_4 != 0 && ref->bits_3_4 != 2) {
            cellArg = (s32)OamMgr_CloneCellPieces(NULL, ref->lastCellPieces);
        } else {
            cellArg = (s32)ref->lastCellPieces;
        }
        func_ov030_020c6aa4(arg0->unk_90, ref->scaleX + (x >> 0xC), ref->scaleY + (y >> 0xC), cellArg, attr.attr2,
                            ref->charData->bitmapIndex << 5, extra, &arg0->affine);
    }
    ref = arg0->unk_98;
    if (ref == NULL) {
        return;
    }
    if (ref->bits_3_4 != 0 && ref->bits_3_4 != 2) {
        cellArg = (s32)OamMgr_CloneCellPieces(NULL, ref->lastCellPieces);
    } else {
        cellArg = (s32)ref->lastCellPieces;
    }
    func_ov030_020c6aa4(arg0->unk_90, ref->scaleX + (x >> 0xC), ref->scaleY + (y >> 0xC), cellArg, attr.attr2,
                        ref->charData->bitmapIndex << 5, extra, &arg0->affine);
}

void func_ov030_020b5990(FldHuman* arg0) {
    s32 v = SysControl.frameCount & 0x3F;
    s32 old;

    if (v > 0x1F) {
        v = 0x1F - (v - 0x20);
    }
    if (v == 0) {
        v = 1;
    }
    arg0->unk_90 = arg0->unk_90 + 0xFB000;
    func_ov030_020b5824(arg0, arg0->palette);
    arg0->unk_90         = arg0->unk_90 + 0x101000;
    old                  = data_0206a890.unk_0C;
    data_0206a890.unk_0C = v & 0x1F;
    arg0->oamAttr2       = (u16)((arg0->oamAttr2 & ~0xC) | 4);
    func_ov030_020b5824(arg0, func_ov030_020af5d4());
    data_0206a890.unk_0C = old & 0x1F;
    arg0->oamAttr2       = (u16)(arg0->oamAttr2 & ~0xC);
}

s32 func_ov030_020b5a34(TaskPool* pool, Task* task, void* args) {
    FldHuman* obj = task->data;

    if (func_ov030_020b580c(obj) != 0) {
        if (gSaveData.unk_2458 != 1 && func_ov030_020cb72c(&obj->pos) == 1) {
            obj->unk_90 = obj->screen.y + 0x1C3000;
        } else {
            obj->unk_90 = obj->screen.y + 0x1000;
        }
        func_ov030_020af4fc();
        if (func_ov030_020aefbc() == 0) {
            if (gSaveData.unk_2458 != 1) {
                func_ov030_020af564(&obj->pos);
                func_ov030_020b5824(obj, obj->palette);
                REG_GFX_FIFO_VERTEX_COLOR = func_ov030_020af53c();
            } else {
                func_ov030_020b5824(obj, obj->palette);
            }
        } else if (obj->unk_84 != 0) {
            func_ov030_020b5990(obj);
        } else {
            REG_GFX_FIFO_VERTEX_COLOR = 0x4108;
            func_ov030_020b5824(obj, obj->palette);
            REG_GFX_FIFO_VERTEX_COLOR = func_ov030_020af53c();
        }
    }
    return 1;
}

s32 func_ov030_020b5b30(TaskPool* pool, Task* task, void* args) {
    FldHuman* obj = task->data;

    if ((u32)obj->unk_88 != -1U) {
        EasyTask_DeleteTask(pool, (u32)obj->unk_88);
    }
    return 1;
}

s32 func_ov030_020b5b54(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = data_ov030_020e0930;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020b5b9c(TaskPool* pool, FieldPos* origin, FieldPos start, s32 arg4, s32 dataType, FldSpawnTemplate tpl,
                        s32 arg7, s32 speed, s32 spawns, s32 angle, s16 arg11, s16 route, FieldPos target) {
    FldHumanInit init;

    init.origin   = origin;
    init.start    = start;
    init.unk_0C   = arg4;
    init.dataType = dataType;
    init.tpl      = tpl;
    init.unk_30   = arg7;
    init.speed    = speed;
    init.spawns   = (FldHumanSpawnEntry*)spawns;
    init.angle    = angle;
    init.unk_3C   = arg11;
    init.route    = route;
    init.target   = target;
    return EasyTask_CreateTask(pool, &data_ov030_020e0924, NULL, 0, NULL, &init);
}
