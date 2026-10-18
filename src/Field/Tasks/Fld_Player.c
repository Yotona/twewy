#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Field/FldPlayer.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x0 */ FieldPos* origin;
    /* 0x4 */ FieldPos* pos;
} Ov030PlayerArgs;

typedef TaskStages Ov030PlayerFns;

s32  FX_Atan2Idx(s32 x, s32 y);
s32  FX_Sqrt(s32 x);
s32  func_ov030_020aad40(void);
s32  func_ov030_020aeca4(s32 y);
s32  Field_IsMoveTouchActive(void);
s32  Field_IsMoveInputActive(void);
s32  Field_IsIdle(void);
void Field_ApplyVertexColorAt(FieldPos* pos);
s32  func_ov030_020af730(void);
s32  func_ov030_020af748(void);
s32  func_ov030_020b7c18(void);
s32  func_ov030_020b7d04(s32 x);
u16  func_ov030_020bc4b8(s32 angle);
u16  func_ov030_020bc4d8(Sprite* sprite);
s32  func_ov030_020bc518(FieldPos* pos, s32 angle, s32 dist);
s32  func_ov030_020c8818(TaskPool* pool, s32 arg1, s32 x, s32 y, s32 arg4, s32 scale, s32 arg6);
s32  func_ov030_020cb72c(FieldPos* pos);
void func_ov030_020b27bc(TaskPool* pool, Ov030Player* obj);

extern SpriteAnimation data_ov030_020df5e8;
extern BinIdentifier   data_ov030_020df5a0;
extern FldCharaCell    data_ov030_020df614[];
extern Ov030PlayerFns  data_ov030_020df5b4;
extern TaskHandle      data_ov030_020df5a8;
extern s32             data_ov030_020df5c4[];

SpriteFrameInfo* func_ov030_020b20dc(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

void func_ov030_020b2178(Sprite* arg0) {
    SpriteAnimation anim = data_ov030_020df5e8;

    _Sprite_Load(arg0, &anim);
}

void func_ov030_020b21c0(Ov030Player* arg0, Ov030PlayerCallback arg1) {
    arg0->unkA4 = arg1;
    arg0->unkA2 = 0;
    arg0->unkA0 = 0;
}

void func_ov030_020b21d4(Ov030Player* arg0) {
    s32 lo;
    s32 hi;
    s32 loSh;
    s32 hiSh;

    lo   = (func_ov030_020b7b80() - func_ov030_020b7b00() / 2) + 1;
    loSh = lo << 0xC;
    hi   = (func_ov030_020b7b80() + func_ov030_020b7b00() / 2) - 0x101;
    hiSh = hi << 0xC;
    if (arg0->origin->x < (lo << 0xC)) {
        arg0->origin->x = loSh;
    } else if (arg0->origin->x > hiSh) {
        arg0->origin->x = hiSh;
    }
    if (gSaveData.fieldArea == 0) {
        hi = func_ov030_020b7d04(arg0->origin->x);
        lo = 0xE000;
    } else {
        hi = (func_ov030_020b7b40() + func_ov030_020b7b60() - 0xC1) << 0xC;
        lo = (func_ov030_020b7bf8() + 1) << 0xC;
    }
    if (arg0->origin->y < lo) {
        arg0->origin->y = lo;
        return;
    }
    if (arg0->origin->y > hi) {
        arg0->origin->y = hi;
    }
}

void func_ov030_020b22a4(Ov030Player* arg0, s32 arg1, s32 arg2) {
    s32       dx;
    FieldPos* p;
    s32       dy;
    s32       halfH;
    s32       t;

    p     = arg0->origin;
    dx    = arg1 - p->x;
    halfH = ((s32)arg0->unk9E / 2) + 0x10;
    dy    = (arg2 - (halfH << 0xC)) - func_ov030_020b7c18() - p->y;
    if (dx < 0) {
        t = -dx;
    } else {
        t = dx;
    }
    if (t < 0x666) {
        dx = 0;
    }
    if (dy < 0) {
        t = -dy;
    } else {
        t = dy;
    }
    if (t < 0x666) {
        dy = 0;
    }
    p->x = p->x + dx;
    p    = arg0->origin;
    p->y = p->y + dy;
    func_ov030_020b21d4(arg0);
}

void func_ov030_020b233c(s32 arg0, Sprite* arg1) {
    s32 result;

    if (arg1->bit_11 != 1) {
        return;
    }
    result = func_ov030_020bc4d8(arg1);
    switch (result) {
        case 1:
            SndMgr_PlaySEWithPan(3, arg0);
            return;
        case 5:
            SndMgr_PlaySEWithPan(4, arg0);
            return;
    }
}

void func_ov030_020b2394(Ov030Player* arg0) {
    func_ov030_020be258(&arg0->chara, (u16)(s16)(arg0->unkD0 + 8), 1);
    if (arg0->unkD4 != 0) {
        arg0->unkD4 = 0;
    }
    func_ov030_020b233c((arg0->pos->x - arg0->origin->x) >> 0xC, &arg0->chara.sprite);
}

void func_ov030_020b23f0(TaskPool* pool, Ov030Player* arg1) {
    func_ov030_020b21c0(arg1, func_ov030_020b24b0);
    arg1->unkA0 = arg1->unkA0 + 1;
}

// Nonmatching: register allocation: scale temp lands in r3 and the <<4 shift temps in r2/r1
// vs retail r2/r1/r3 (USA 99.21%; JP unvalidated).
void func_ov030_020b2418(TaskPool* arg0, Ov030Player* arg1) {
    s32 offsetX;
    s32 offsetY;
    s16 state;

    if (arg1->unkA0 == 0) {
        func_ov030_020be258(&arg1->chara, 4, 1);
    }
    state = arg1->unkA0;
    if ((s32)state < 5) {
        arg1->unkA0 = state + 1;
    }
    if (arg1->unkA0 != 4) {
        return;
    }
    offsetY     = arg1->screen.y;
    offsetX     = arg1->screen.x;
    arg1->unkF0 = func_ov030_020c8818(arg0, 1, (offsetX * 0x10) >> 0x10, (offsetY * 0x10) >> 0x10,
                                      func_ov030_020be3c0(&arg1->chara.sprite), arg1->unkC8, 0);
}

void func_ov030_020b24b0(TaskPool* arg0, Ov030Player* arg1) {
    FieldPos* p;
    s16       state;

    arg1->unkAC = arg1->pos->x;
    arg1->unkB0 = arg1->pos->y;
    p           = arg1->pos;
    func_ov030_020b22a4(arg1, p->x - 0x80000, p->y - 0x60000);
    if (Field_IsMoveInputActive() != 0 && Field_IsIdle() != 0 && func_ov030_020af748() != 0) {
        func_ov030_020b21c0(arg1, func_ov030_020b27bc);
        if (arg1->unkD4 == 0) {
            func_ov030_020b233c((arg1->pos->x - arg1->origin->x) >> 0xC, &arg1->chara.sprite);
        }
        if (arg1->unkE8 == -1) {
            return;
        }
        EasyTask_DeleteTask(arg0, arg1->unkE8);
        arg1->unkE8 = -1;
        return;
    }
    if (gSaveData.unk_2498 != 0) {
        return;
    }
    arg1->unkD4 = 1;
    state       = arg1->unkA0;
    if (state == 0) {
        func_ov030_020be258(&arg1->chara, (u16)(s16)(arg1->unkD0 + 3), 1);
    } else if (state == 0xF0) {
        func_ov030_020be258(&arg1->chara, 2, 1);
    } else if (state == 0xF8) {
        if (arg1->unkE8 == -1) {
            if (func_ov030_020aad40() != 0 && gSaveData.unk_2488 != 0) {
                if (func_0202599c(1) != 0 && gSaveData.chapter != 0x15) {

                } else {
                    arg1->unkE8 = func_ov030_020bacb0(arg0, 1, &arg1->screen, &arg1->unkC8, 0x34000, 1);
                }
            }
        }
        func_ov030_020be258(&arg1->chara, 1, 1);
    }
    if (Field_IsIdle() == 0) {
        arg1->unkA0 = 0;
        if (arg1->unkE8 == -1) {
            return;
        }
        EasyTask_DeleteTask(arg0, arg1->unkE8);
        arg1->unkE8 = -1;
        return;
    }
    arg1->unkA0 = arg1->unkA0 + 1;
}

s32 func_ov030_020b26bc(void) {
    s32 v;

    v = SysControl.buttonState.currButtons & 0xF0;
    if (v == 0) {
        if (SysControl.buttonState.currButtons & 1) {
            v |= 0x10;
        }
        if (SysControl.buttonState.currButtons & 2) {
            v |= 0x80;
        }
        if (SysControl.buttonState.currButtons & 0x400) {
            v |= 0x40;
        }
        if (SysControl.buttonState.currButtons & 0x800) {
            v |= 0x20;
        }
    }
    if (v == 0x60) {
        return 0xA000;
    }
    if (v == 0x50) {
        return 0xE000;
    }
    if (v == 0xA0) {
        return 0x6000;
    }
    if (v == 0x90) {
        return 0x2000;
    }
    if (v & 0x40) {
        return 0xC000;
    }
    if (v & 0x80) {
        return 0x4000;
    }
    if (v & 0x20) {
        return 0x8000;
    }
    return 0;
}

void func_ov030_020b2748(Ov030Player* arg0) {
    gSaveData.unk_245A = arg0->unkD0;
    if (func_ov030_020be3c0(&arg0->chara.sprite) != 0) {
        return;
    }
    gSaveData.unk_245A = 8 - (s16)gSaveData.unk_245A;
    if ((s32)(s16)gSaveData.unk_245A >= 8) {
        gSaveData.unk_245A = 0;
    }
}

s32 func_ov030_020b278c(TaskPool* pool, Ov030Player* obj, s32* out) {
    s32 value = 0;

    if (func_ov030_020b84a4(gSaveData.fieldArea, &value, out) == 0) {
        return value;
    }
    return 0;
}

void func_ov030_020b27bc(TaskPool* arg0, Ov030Player* arg1) {
    FieldPos   saved;
    s32        exitDir;
    TouchCoord pos;
    TouchCoord coord;
    s32        dir;
    s32        dx;
    s32        dy;
    s32        len;
    s32        ret;

    arg1->unkA8 |= 1;
    if (FieldScan_IsOpen() != 0) {
        arg1->unkA8 &= ~1;
        func_ov030_020b21c0(arg1, func_ov030_020b24b0);
        arg1->unkA0 = 0xF8;
        return;
    }
    if (Field_IsIdle() == 0 || func_ov030_020af748() == 0) {
        arg1->unkA8 &= ~1;
        func_ov030_020b22a4(arg1, arg1->pos->x - 0x80000, arg1->pos->y - 0x60000);
        func_ov030_020b233c((arg1->pos->x - arg1->origin->x) >> 0xC, &arg1->chara.sprite);
        return;
    }
    if (SysControl.buttonState.currButtons & 0xCF3) {
        len = 9;
        dir = func_ov030_020b26bc();
        arg1->unkA8 &= ~1;
        arg1->unkAC = arg1->pos->x;
        arg1->unkB0 = arg1->pos->y;
    } else {
        if (Field_IsMoveTouchActive() != 0) {
            TouchInput_GetCoord(&coord);
            pos         = coord;
            arg1->unkAC = arg1->origin->x + (pos.x << 0xC);
            arg1->unkB0 = arg1->origin->y + (pos.y << 0xC);
        }
        dx  = arg1->unkAC - arg1->pos->x;
        dy  = arg1->unkB0 - arg1->pos->y;
        len = FX_Sqrt((s32)(((s64)dx * dx + 0x800) >> 0xC) + (s32)(((s64)dy * dy + 0x800) >> 0xC)) >> 0xC;
        dir = FX_Atan2Idx(dy, dx);
    }
    if (len >= 9) {
        arg1->unkD0 = func_ov030_020bc4b8(dir);
        if ((s32)(u16)(dir - 0x4000) < 0x8000) {
            func_ov030_020be368(&arg1->chara.sprite, 0);
        } else {
            func_ov030_020be368(&arg1->chara.sprite, 1);
        }
        saved = *arg1->pos;
        ret   = func_ov030_020b278c(arg0, arg1, &exitDir);
        if (func_ov030_020bc518(arg1->pos, dir, (s32)(((s64)arg1->unkD8 * arg1->unkC8 + 0x800) >> 0xC)) != 0) {
            arg1->unkA8 &= ~1;
            func_ov030_020b21c0(arg1, func_ov030_020b24b0);
        }
        if (ret == 0 && func_ov030_020b278c(arg0, arg1, &exitDir) != 0) {
            *arg1->pos = saved;
            if (arg1->unkEC == -1) {
                arg1->unkEC = func_ov030_020c2c94(arg0, 1, arg1->origin, exitDir);
            }
            arg1->unkA8 &= ~1;
            func_ov030_020b21c0(arg1, func_ov030_020b24b0);
        }
        func_ov030_020b2394(arg1);
        func_ov030_020b22a4(arg1, arg1->pos->x - 0x80000, arg1->pos->y - 0x60000);
        return;
    }
    if (Field_IsMoveInputActive() != 0) {
        func_ov030_020b2394(arg1);
        return;
    }
    arg1->unkA8 &= ~1;
    func_ov030_020b21c0(arg1, func_ov030_020b24b0);
}

s32 func_ov030_020b2af0(TaskPool* pool, Task* task, Ov030PlayerArgs* args) {
    Ov030Player* obj = task->data;
    s32          y;

    MI_CpuSet(obj, 0, sizeof(Ov030Player));
    obj->unk9C         = 0xC;
    obj->unk9E         = 0x30;
    obj->unkD8         = 0x2CCD;
    obj->origin        = args->origin;
    obj->pos           = args->pos;
    obj->unkC8         = func_ov030_020aeca4(obj->pos->y);
    gSaveData.unk_2454 = obj->unkC8;
    obj->unkAC         = obj->pos->x;
    obj->unkB0         = obj->pos->y;
    obj->origin->x     = obj->pos->x - 0x80000;
    y                  = ((s32)obj->unk9E / 2) + 0x10 + func_ov030_020b7c18();
    y                  = (y + 0x60) << 0xC;
    obj->origin->y     = obj->pos->y - y;
    obj->screen.x      = obj->pos->x - obj->origin->x;
    obj->screen.y      = obj->pos->y - obj->origin->y;
    obj->unkE8         = -1;
    obj->unkEC         = -1;
    obj->unkF0         = -1;
    func_ov030_020b21d4(obj);
    func_ov030_020b21c0(obj, func_ov030_020b23f0);
    func_ov030_020be1d8(&obj->chara, &data_ov030_020df5a0, data_ov030_020df614, 1, 0x22, 0x50);
    // Retail reads the field through an s16 lvalue; the u16 load + (s16) cast lets
    // mwcc narrow the reload to ldrh in the <=4 branch.
    if ((s32)(s16)gSaveData.unk_245A <= 4) {
        func_ov030_020be368(&obj->chara.sprite, 1);
        obj->unkD0 = (s16)gSaveData.unk_245A;
    } else {
        func_ov030_020be368(&obj->chara.sprite, 0);
        obj->unkD0 = (s16)(8 - (s16)gSaveData.unk_245A);
    }
    func_ov030_020b2178(&obj->sprite2);
    Sprite_Update(&obj->chara.sprite);
    return 1;
}

s32 func_ov030_020b2c94(TaskPool* arg0, Task* task) {
    Ov030Player* obj = task->data;

    if (obj->unkEC != -1) {
        EasyTask_ValidateTaskId(arg0, (u32*)&obj->unkEC);
    }
    if (obj->unkA4 != NULL) {
        obj->unkA4(arg0, obj);
    }
    obj->unkC8         = func_ov030_020aeca4(obj->pos->y);
    gSaveData.unk_2454 = obj->unkC8;
    if (gSaveData.unk_2498 == 0) {
        Sprite_Update(&obj->chara.sprite);
    }
    if (obj->unkA8 & 1) {
        Sprite_Update(&obj->sprite2);
    }
    func_ov030_020b2748(obj);
    return 1;
}

// Nonmatching: register allocation and OamMgr argument scheduling vs retail:
// mwcc hoists the obj->unkC8 scale load above the &g_OamMgr[] pointer math
// (USA 85.67%; JP unvalidated).
s32 func_ov030_020b2d24(TaskPool* pool, Task* task) {
    Ov030Player* obj = task->data;
    s32          dy;
    s32          flip;
    s32          scale;

    if (func_ov030_020af730() != 0 && gSaveData.unk_2498 == 0) {
        return 1;
    }
    if (obj->unkF0 != -1 && (s32)obj->unkA0 >= 5) {
        return 1;
    }
    obj->screen.x          = obj->pos->x - obj->origin->x;
    dy                     = obj->pos->y - obj->origin->y;
    obj->screen.y          = dy;
    obj->chara.sprite.posX = (s16)(obj->screen.x >> 0xC);
    obj->chara.sprite.posY = (s16)(obj->screen.y >> 0xC);
    if (func_ov030_020cb72c(obj->pos) == 1) {
        func_ov030_020be3dc(&obj->chara, obj->screen.y + 0x1C3000);
    } else {
        func_ov030_020be3dc(&obj->chara, obj->screen.y + 0x1000);
    }
    flip = 0;
    if (func_ov030_020be3c0(&obj->chara.sprite) != 0) {
        flip = 1;
    }
    scale      = obj->unkC8;
    obj->unk50 = OamMgr_AllocAffineGroup(&g_OamMgr[obj->chara.sprite.bits_0_1], 0, scale, scale, flip);
    Field_ApplyVertexColorAt(obj->pos);
    Sprite_RenderFrame(&obj->chara.sprite);
    flip       = 2;
    flip       = flip | (func_ov030_020be3c0(&obj->chara.sprite) != 0);
    scale      = obj->unkC8;
    obj->unk50 = OamMgr_AllocAffineGroup(&g_OamMgr[obj->chara.sprite.bits_0_1], 0, scale, scale / 6, flip);
    func_ov030_020be3dc(&obj->chara, 0x1000);
    REG_GFX_FIFO_VERTEX_COLOR = 0x421;
    func_ov030_020be3e4(&obj->chara.sprite, 0xC);
    REG_GFX_FIFO_VERTEX_COLOR = Field_GetLightColor();
    if (obj->unkA8 & 1) {
        dy                = obj->unkAC - obj->origin->x;
        flip              = obj->unkB0 - obj->origin->y;
        obj->sprite2.posX = (s16)(dy >> 0xC);
        obj->sprite2.posY = (s16)(flip >> 0xC);
        Sprite_RenderFrame(&obj->sprite2);
    }
    return 1;
}

s32 func_ov030_020b2f0c(TaskPool* pool, Task* task) {
    Ov030Player* obj = task->data;

    Sprite_Release(&obj->chara.sprite);
    Sprite_Release(&obj->sprite2);
    if (obj->unkE8 != -1) {
        EasyTask_DeleteTask(pool, obj->unkE8);
    }
    if (obj->unkEC != -1) {
        EasyTask_DeleteTask(pool, obj->unkEC);
    }
    if (obj->unkF0 != -1) {
        EasyTask_DeleteTask(pool, obj->unkF0);
    }
    return 1;
}

s32 func_ov030_020b2f78(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030PlayerFns fns = data_ov030_020df5b4;

    return fns.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020b2fc0(TaskPool* pool, FieldPos* origin, FieldPos* pos) {
    Ov030PlayerArgs params;

    params.origin = origin;
    params.pos    = pos;
    return EasyTask_CreateTask(pool, &data_ov030_020df5a8, NULL, 0, NULL, &params);
}

s32 func_ov030_020b2ff4(Task* task) {
    Ov030Player* obj    = task->data;
    s32          result = 0;

    if (obj->unkA4 == func_ov030_020b27bc) {
        result = 1;
    }
    return result;
}

void func_ov030_020b3014(Task* task, s32 arg1) {
    ((Ov030Player*)task->data)->unkD8 = arg1;
}

s32 func_ov030_020b3020(Task* arg0) {
    Ov030Player* obj = arg0->data;

    return obj->unk9E * obj->unkC8;
}

void func_ov030_020b3034(Task* arg0) {
    Ov030Player* obj = arg0->data;

    obj->unkA8 &= ~1;
    func_ov030_020b21c0(obj, func_ov030_020b24b0);
}

s32 func_ov030_020b3058(Task* arg0) {
    Ov030Player* obj = arg0->data;
    s32          right;
    s32          half = obj->unk9C;
    s32          left;
    s32          top;
    s32          cx = obj->screen.x;
    s32          cy = obj->screen.y;
    TouchCoord   coord;
    s32          y = func_ov030_020b3020(arg0);

    left  = (cx - (half << 12)) >> 12;
    top   = (cy - (y + 0x18000)) >> 12;
    right = (cx + (half << 12)) >> 12;

    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    TouchInput_GetCoord(&coord);
    if (left < coord.x && coord.x < right && top < coord.y && coord.y < (cy >> 12)) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020b30d0(Task* arg0) {
    if (((Ov030Player*)arg0->data)->unkE8 == -1) {
        return 0;
    }
    return func_ov030_020b3058(arg0);
}

void func_ov030_020b30f4(Task* arg0) {
    func_ov030_020b21c0(arg0->data, func_ov030_020b2418);
}

s32 func_ov030_020b310c(s32 arg0) {
    return data_ov030_020df5c4[((arg0 >> 12) & 1) + (arg0 >> 13)];
}
