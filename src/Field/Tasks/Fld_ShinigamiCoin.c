// Preliminary USA TU; original assembly remains linked (incomplete).
// Range: 0x020ccb78-0x020cd578; 22 functions.
// Research draft: docs/decomp/ov030/Fld_ShinigamiCoin.c
// Boundary evidence: docs/tu-boundaries/ov030.md.
// Unassigned data stays in generated gaps; do not mark complete yet.
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "SndMgr.h"
#include "SpriteMgr.h"

// A drop region of the coin minigame (data_ov030_0210896c, 3 entries). Holding
// the coin inside it long enough reports one of its three results.
typedef struct {
    /* 0x00 */ s16 unk0; // result when entered from state 6/3
    /* 0x02 */ s16 unk2; // result when entered from state 4
    /* 0x04 */ s16 unk4; // result when left in state 4
    /* 0x06 */ s16 xMin;
    /* 0x08 */ s16 xMax;
    /* 0x0A */ s16 yMin;
    /* 0x0C */ s16 yMax;
    /* 0x0E */ s16 unkE;
    /* 0x10 */ s32 holdTime;
    /* 0x14 */ s8  state;
    /* 0x15 */ u8  pad15[3];
} Ov030CoinRegion; // Size: 0x18

typedef struct {
    /* 0x00 */ Sprite sprite; // 0x40
    /* 0x40 */ s16    unk40;  // current animation index
    /* 0x42 */ s16    pad42;
    /* 0x44 */ s32    unk44;  // active flag
    /* 0x48 */ s32    unk48;  // dragging flag
    /* 0x4C */ s32    unk4C;  // drag offset X
    /* 0x50 */ s32    unk50;  // drag offset Y
    /* 0x54 */ s32    unk54;  // last X
    /* 0x58 */ s32    unk58;  // last Y
    /* 0x5C */ s32    unk5C;  // hold flag
    /* 0x60 */ s32    unk60;  // hold counter
    /* 0x64 */ s32    unk64;  // tap state
    /* 0x68 */ s32    unk68;  // stationary counter
    /* 0x6C */ s16    unk6C;  // tap countdown
    /* 0x6E */ s16    unk6E;  // suppressed flag
} Ov030CoinSprite;            // 0x70

typedef TaskStages Ov030CoinFns;

extern Ov030CoinFns    data_ov030_020ed114;
extern TaskHandle      data_ov030_020ed108;
extern SpriteAnimation data_ov030_020ed124;
extern Ov030CoinRegion data_ov030_0210896c[3];
extern void            func_ov030_020af6f0(s32);
extern void            func_ov030_020af708(s16);

s32 func_ov030_020ccda8(Sprite*, TouchCoord);
u32 func_ov030_020cd204(Ov030CoinSprite*, Ov030CoinSprite*);

SpriteFrameInfo* func_ov030_020ccb78(Sprite* arg0, s32 arg1, s32 arg2) {
    Sprite_FrameInfoCallback(arg0, arg2);
}

// Nonmatching: SpriteAnimation build; `dataType` is now written last so its store
// follows the posY/posX stores, but retail loads `*arg1`/computes the field before
// them and stores posY before posX; ours defers the load and stores posX first
// (USA 71.97%; JP unvalidated).
void func_ov030_020ccc14(Sprite* arg0, s32* arg1) {
    SpriteAnimation anim = data_ov030_020ed124;

    anim.posY     = 0x64;
    anim.posX     = 0x78;
    anim.dataType = (u16)*arg1;
    SndMgr_PlaySEWithPan(0xC7, 0x78);
    _Sprite_Load(arg0, &anim);
}

s32 func_ov030_020ccc94(Sprite* arg0, Ov030CoinSprite* arg1) {
    TouchCoord coord;

    if (TouchInput_IsTouchActive() != 0) {
        TouchInput_GetCoord(&coord);
        if (arg1->unk48 == 0) {
            if (func_ov030_020ccda8(arg0, coord) != 0) {
                arg1->unk4C = arg0->posX - coord.x;
                arg1->unk50 = arg0->posY - coord.y;
                arg1->unk48 = 1;
                SndMgr_PlaySEWithPan(0x131, coord.x);
            }
        } else {
            arg0->posX = coord.x + arg1->unk4C;
            arg0->posY = coord.y + arg1->unk50;
            if (arg0->posX < 0x10) {
                arg0->posX = 0x10;
            }
            if (arg0->posX > 0xF0) {
                arg0->posX = 0xF0;
            }
            if (arg0->posY < 0x10) {
                arg0->posY = 0x10;
            }
            if (arg0->posY > 0xB0) {
                arg0->posY = 0xB0;
            }
        }
    } else {
        if (arg1->unk48 != 0) {
            SndMgr_PlaySEWithPan(0x132, arg0->posX);
        }
        arg1->unk48 = 0;
    }
    return 0;
}

s32 func_ov030_020ccda8(Sprite* arg0, TouchCoord coord) {
    s32 x  = coord.x;
    s32 y  = coord.y;
    s32 px = arg0->posX;
    s32 py = arg0->posY;

    if ((s32)(px - 0xF) < x && x < (s32)(px + 0xF) && (s32)(py - 0xF) < y && y < (s32)(py + 0xF)) {
        return 1;
    }
    return 0;
}

s32 func_ov030_020ccdec(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 dx = arg0->sprite.posX - arg1->unk54;
    s32 dy = arg0->sprite.posY - arg1->unk58;
    s32 r  = 0;

    if (dx == 0 && dy == 0 && arg1->unk5C == 1) {
        arg1->unk60++;
        if (arg1->unk60 > 0x78) {
            arg1->unk5C = 0;
            arg1->unk60 = 0;
        }
    }
    if (dx > 0 || dx < 0 || dy > 0 || dy < 0) {
        if (arg1->unk5C == 0) {
            arg1->unk60 = 0;
            arg1->unk5C = 1;
            r           = 1;
        }
    }
    return r;
}

// Nonmatching: region-walk register allocation now matches (declared `i`, `t`, `r` in
// that order); residual rows are the inner `v == 0x258` block, which mwcc if-converts
// to `streq/moveq` instead of retail's `bne` (USA 92.19%; JP unvalidated).
s32 func_ov030_020cce68(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 i;
    s8  t;

    for (i = 0; i < 3; i++) {
        t = data_ov030_0210896c[i].state;
        if (t != 6) {
            break;
        }
    }
    if (arg1->unk5C == 0 && t == 6) {
        s32 v       = arg1->unk68 + 1;
        arg1->unk68 = v;
        if (v == 0x258) {
            arg1->unk68 = 0;
            return 1;
        }
        return 0;
    }
    arg1->unk68 = 0;
    return 0;
}

s32 func_ov030_020cced4(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 dx = arg0->sprite.posX - arg1->unk54;
    s32 dy = arg0->sprite.posY - arg1->unk58;

    return dx * dx + dy * dy;
}

s32 func_ov030_020ccef8(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s32 r = func_ov030_020cced4(arg0, arg1);
    s32 t = arg1->unk64;

    if (r > 9) {
        if (t == 0) {
            arg1->unk64 = 1;
            arg1->unk6C = 0x46;
            arg1->unk6E = 1;
            return 1;
        }
    } else if (t > 0xF) {
        arg1->unk64 = 0;
        arg1->unk6C = -1;
    } else if (t != 0) {
        arg1->unk64 = t + 1;
    }
    return 0;
}

s32 func_ov030_020ccf60(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    s16 t = arg1->unk6C;

    if (t > 0) {
        arg1->unk6C = t - 1;
    } else if (t == 0) {
        arg1->unk6C = -1;
        return 1;
    }
    return 0;
}

s32 func_ov030_020ccf90(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    if (arg1->unk6E == 0) {
        if (func_ov030_020ccdec(arg0, arg1) != 0) {
            func_ov030_020af6f0(5);
            func_ov030_020af708(0);
            SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
        }
        if (func_ov030_020ccef8(arg0, arg1) != 0) {
            func_ov030_020af6f0(5);
            func_ov030_020af708(1);
            SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
        }
        if (func_ov030_020cce68(arg0, arg1) != 0) {
            func_ov030_020af6f0(5);
            func_ov030_020af708(2);
            SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
        }
    }
    if (func_ov030_020ccf60(arg0, arg1) != 0) {
        func_ov030_020af6f0(5);
        func_ov030_020af708(0xC);
        SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
    }
    arg1->unk54 = arg0->sprite.posX;
    arg1->unk58 = arg0->sprite.posY;
    return 1;
}

s32 func_ov030_020cd078(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    u32 r = func_ov030_020cd204(arg0, arg1);

    if (arg1->unk6E == 0) {
        switch (r) {
            default:
                break;
            case 2:
                func_ov030_020af6f0(5);
                func_ov030_020af708(4);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 1:
                func_ov030_020af6f0(5);
                func_ov030_020af708(3);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 0:
                func_ov030_020af6f0(5);
                func_ov030_020af708(5);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 6:
                func_ov030_020af6f0(5);
                func_ov030_020af708(0xA);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                arg1->unk6E = 1;
                break;
            case 5:
                func_ov030_020af6f0(5);
                func_ov030_020af708(9);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                arg1->unk6E = 1;
                break;
            case 4:
                func_ov030_020af6f0(5);
                func_ov030_020af708(0xB);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                arg1->unk6E = 1;
                break;
            case 9:
                func_ov030_020af6f0(5);
                func_ov030_020af708(7);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 8:
                func_ov030_020af6f0(5);
                func_ov030_020af708(6);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
            case 7:
                func_ov030_020af6f0(5);
                func_ov030_020af708(8);
                SndMgr_PlaySEWithPan(0xC4, arg0->sprite.posX);
                break;
        }
    }
    return 0;
}

u32 func_ov030_020cd204(Ov030CoinSprite* arg0, Ov030CoinSprite* arg1) {
    TouchCoord coord;
    s32        i;

    TouchInput_GetCoord(&coord);
    arg1->unk40 = 3;
    for (i = 0; i < 3; i++) {
        Ov030CoinRegion* r = &data_ov030_0210896c[i];

        if (arg0->sprite.posX > r->xMin && arg0->sprite.posX < r->xMax && arg0->sprite.posY > r->yMin &&
            arg0->sprite.posY < r->yMax)
        {
            if (r->state == 6 || r->state == 3) {
                r->holdTime++;
                arg1->unk40 = i;
                r->state    = 3;
                if (r->holdTime > 0x64) {
                    data_ov030_0210896c[i].holdTime = 0;
                    data_ov030_0210896c[i].state    = 4;
                    return data_ov030_0210896c[i].unk0;
                }
            } else if (r->state == 4) {
                arg1->unk40 = i;
                r->holdTime++;
                if (r->holdTime > 0x64) {
                    data_ov030_0210896c[i].holdTime = 0;
                    data_ov030_0210896c[i].state    = 6;
                    return data_ov030_0210896c[i].unk2;
                }
            }
        } else {
            r->holdTime = 0;
            if (r->state == 4) {
                data_ov030_0210896c[i].state = 6;
                return data_ov030_0210896c[i].unk4;
            }
            r->state = 6;
        }
    }
    return 3;
}

void func_ov030_020cd388(TaskPool* arg0, Task* arg1, s32* arg2) {
    Ov030CoinSprite* sprite = arg1->data;
    s32              i;

    sprite->unk40 = 3;
    sprite->unk48 = 0;
    sprite->unk60 = 0;
    sprite->unk44 = 0;
    sprite->unk5C = 0;
    sprite->unk54 = 0x78;
    sprite->unk58 = 0x64;
    sprite->unk64 = 0;
    sprite->unk68 = 0;
    sprite->unk6C = -1;
    sprite->unk6E = 0;
    for (i = 0; i < 3; i++) {
        data_ov030_0210896c[i].holdTime = 0;
        data_ov030_0210896c[i].state    = 6;
    }
    func_ov030_020ccc14(&sprite->sprite, arg2);
}

s32 func_ov030_020cd404(TaskPool* arg0, Task* arg1, void* arg2) {
    Ov030CoinSprite* sprite = arg1->data;

    if (sprite->unk44 != 0) {
        func_ov030_020ccc94(&sprite->sprite, sprite);
        func_ov030_020ccf90(sprite, sprite);
        func_ov030_020cd078(sprite, sprite);
    }
    Sprite_Update(&sprite->sprite);
    return 1;
}

s32 func_ov030_020cd44c(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_RenderFrame(arg1->data);
    return 1;
}

s32 func_ov030_020cd460(TaskPool* arg0, Task* arg1, void* arg2) {
    Sprite_Release(arg1->data);
    return 1;
}

s32 func_ov030_020cd474(TaskPool* arg0, Task* arg1, void* arg2, s32 arg3) {
    Ov030CoinFns funcs = data_ov030_020ed114;

    return funcs.iter[arg3](arg0, arg1, arg2);
}

s32 func_ov030_020cd4bc(TaskPool* arg0, s32 arg1) {
    s32 param = arg1;

    return EasyTask_CreateTask(arg0, &data_ov030_020ed108, NULL, 0, NULL, &param);
}

void func_ov030_020cd4ec(Task* arg0, s32 arg1) {
    Ov030CoinSprite* sprite = arg0->data;
    s32              i;

    sprite->unk48 = 0;
    sprite->unk5C = 0;
    sprite->unk60 = 0;
    sprite->unk64 = 0;
    sprite->unk68 = 0;
    for (i = 0; i < 3; i++) {
        data_ov030_0210896c[i].holdTime = 0;
    }
    sprite->unk44 = arg1;
}

s16 func_ov030_020cd530(Task* arg0) {
    Ov030CoinSprite* sprite = arg0->data;
    s16              t      = sprite->unk40;
    s16              r      = t + 1;

    if (t == 3) {
        r = 0;
    }
    return r;
}

void func_ov030_020cd550(Task* arg0) {
    ((Ov030CoinSprite*)arg0->data)->unk6E = 1;
}

s32 func_ov030_020cd560(Task* arg0) {
    if (((Ov030CoinSprite*)arg0->data)->unk6E != 0) {
        return 1;
    }
    return 0;
}
