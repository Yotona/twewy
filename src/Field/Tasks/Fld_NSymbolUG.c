#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Field/NSymbol.h"
#include "Field/TaskHelpers.h"
#include "Save.h"
#include "SndMgr.h"
#include "SpriteMgr.h"
#include <nitro/fx/fx_vector.h>

// USA 0x020bad80-0x020bbe08: task rendering and drag lifecycle for the
// scan-symbol ("Noise symbol") cluster in the underground. Each symbol is an
// FldNSymbolUG task object (sprite + fixed-point position + spawn record) whose
// update function switches between drag-follow, wobble, anchored-movement and
// scripted modes. The task's saved record lives in the scan-symbol table at
// MainData::scanSymbols: the object's recIdx selects the record and
// func_ov030_020bbdc4 commits record.state = 2 when the symbol despawns.

// Per-category sprite setup; indexed by the normalized category. The first
// four fields feed SpriteAnimation's resource indices.
typedef struct {
    /* 0x0 */ s16 charDataIndex;
    /* 0x2 */ s16 paletteIndex;
    /* 0x4 */ s16 cellTableIndex;
    /* 0x6 */ s16 animDataIndex;
    /* 0x8 */ s16 halfW;
    /* 0xA */ s16 halfH;
    /* 0xC */ s32 arg;
} NsymCatEntry; // Size: 0x10

typedef struct {
    /* 0x00 */ s32       spawnType; // sprite dataType and effect request id
    /* 0x04 */ u16       x;
    /* 0x06 */ u16       y;
    /* 0x08 */ s32       category;
    /* 0x0C */ s16       mode;
    /* 0x10 */ s32       progressFlagMask;
    /* 0x14 */ FieldPos* anchor;
    /* 0x18 */ s16       recIdx;
} NsymSpawnArgs; // Size: 0x1C

typedef struct FldNSymbolUG {
    /* 0x00 */ Sprite       sprite;
    /* 0x40 */ FldScreenPos home;
    /* 0x44 */ FieldPos*    anchor;
    /* 0x48 */ s16          recIdx;
    /* 0x4A */ s16          pad_4A;
    /* 0x4C */ s32          xWobbleFlag;
    /* 0x50 */ s32          yWobbleFlag;
    /* 0x54 */ s16          mode;
    /* 0x56 */ s16          pad_56;
    /* 0x58 */ s32          progressFlagMask;
    /* 0x5C */ s16          dragDX;
    /* 0x5E */ s16          dragDY;
    /* 0x60 */ s32          x12;  // 12.20 position
    /* 0x64 */ s32          y12;
    /* 0x68 */ s32          x12b; // 12.20 target
    /* 0x6C */ s32          y12b;
    /* 0x70 */ s32          speed;
    /* 0x74 */ u16          timer;
    /* 0x76 */ s16          halfW;
    /* 0x78 */ s16          halfH;
    /* 0x7A */ s16          pad_7A;
    /* 0x7C */ void (*update)(TaskPool*, struct FldNSymbolUG*);
    /* 0x80 */ s32 catIdx;
    /* 0x84 */ s32 category;
    /* 0x88 */ s32 spawnType;
    /* 0x8C */ u32 taskId;
    /* 0x90 */ s32 state;
    /* 0x94 */ u32 taskId2;
    /* 0x98 */ s32 dist;
    /* 0x9C */ u16 angle;
    /* 0x9E */ u16 pad_9E;
    /* 0xA0 */ s32 angleStep;
    /* 0xA4 */ s32 turnFlag;
} FldNSymbolUG; // Size: 0xA8

u16  FX_Atan2Idx(s32 y, s32 x);
s32  FX_Sqrt(s32 value);
void func_02026564(u16* angle, u16 target, s32 mode);
void func_ov030_020aebf0(s32 flagMask);
s32  func_ov030_020aeca4(s32 y);
s32  FieldScan_IsEnteringBattle(void);
void Field_GetPlayerScreenPos(s16* x, s16* y);
void FieldScan_RaiseBgLevel(s32 category);
s32  FieldScan_CanChainEncounter(void);
void FieldScan_AddChainedEncounter(void);
void func_ov030_020be430(s32* x12, s32* y12, s32 angle, s32 step);
s32  func_ov030_020c5704(TaskPool* pool, s32 spawnType, s32 x, s32 y, s32 category);
s32  FO3DViewer_GetNodePos(void* name, Vec* pos);

extern const char  data_ov030_021072b4[];
extern void* const data_ov030_021072c8[];

s32              func_ov030_020bad80(FldNSymbolUG* sym);
void             func_ov030_020bae10(FldNSymbolUG* sym, void (*update)(TaskPool*, FldNSymbolUG*));
void             func_ov030_020bae18(TaskPool* pool, FldNSymbolUG* sym);
void             func_ov030_020bae90(TaskPool* pool, FldNSymbolUG* sym);
void             func_ov030_020baf48(FldNSymbolUG* sym);
s32              func_ov030_020bb004(s32 category);
void             func_ov030_020bb020(FldNSymbolUG* sym);
void             func_ov030_020bb14c(FldNSymbolUG* sym);
void             func_ov030_020bb1e4(TaskPool* pool, FldNSymbolUG* sym);
void             func_ov030_020bb35c(TaskPool* pool, FldNSymbolUG* sym);
void             func_ov030_020bb378(TaskPool* pool, FldNSymbolUG* sym);
void             func_ov030_020bb43c(TaskPool* pool, FldNSymbolUG* sym);
void             func_ov030_020bb4ec(TaskPool* pool, FldNSymbolUG* sym);
SpriteFrameInfo* func_ov030_020bb544(Sprite* sprite, s32 arg, s32 mode);
void             func_ov030_020bb614(Sprite* sprite, s32 dataType, FldScreenPos pos, s32 category);
s32              func_ov030_020bb760(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bba90(FldNSymbolUG* sym);
s32              func_ov030_020bbac0(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bbafc(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bbb24(TaskPool* pool, Task* task, void* args);
s32              func_ov030_020bbb6c(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle data_ov030_020e5ee0 = {data_ov030_021072b4, func_ov030_020bbb6c, sizeof(FldNSymbolUG)};

const TaskStages data_ov030_020e5eec = {
    {func_ov030_020bb760, NULL, NULL, NULL}
};

const SpriteAnimation data_ov030_020e5efc = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02            = 0,
    .posX              = 0,
    .posY              = 0,
    .frameInfoCallback = NULL,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = NULL,
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 0,
    .unk_1E            = 0,
    .unk_20            = 0,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 0,
    .unk_28            = 0,
    .animIndex         = 1,
};

const NsymCatEntry data_ov030_020e5f28[15] = {
    {0x01, 0x2E, 0x02, 0x03, 0x10, 0x10, 0},
    {0x04, 0x2E, 0x05, 0x06, 0x10, 0x18, 1},
    {0x07, 0x2E, 0x08, 0x09, 0x10, 0x18, 1},
    {0x0A, 0x2E, 0x0B, 0x0C, 0x20, 0x18, 2},
    {0x0D, 0x2E, 0x0E, 0x0F, 0x08, 0x08, 0},
    {0x10, 0x2E, 0x11, 0x12, 0x10, 0x18, 1},
    {0x13, 0x2E, 0x14, 0x15, 0x08, 0x08, 0},
    {0x19, 0x2E, 0x1A, 0x1B, 0x10, 0x10, 1},
    {0x16, 0x30, 0x17, 0x18, 0x08, 0x08, 0},
    {0x1C, 0x31, 0x1D, 0x1E, 0x38, 0x28, 2},
    {0x1F, 0x31, 0x20, 0x21, 0x10, 0x20, 2},
    {0x22, 0x31, 0x23, 0x24, 0x10, 0x20, 2},
    {0x25, 0x31, 0x26, 0x27, 0x18, 0x18, 2},
    {0x28, 0x31, 0x29, 0x2A, 0x10, 0x18, 2},
    {0x36, 0x39, 0x37, 0x38, 0x20, 0x30, 2},
};

// Touch hit test against the symbol's sprite bounds. The position halfword is
// read through the first bound (mwcc then hoists that load ahead of the halfW
// load and CSEs it with the `x` local), which is what retail does.
s32 func_ov030_020bad80(FldNSymbolUG* sym) {
    TouchCoord coord;
    s16        x;
    s16        y;

    if (sym->state == 1) {
        return 0;
    }
    if (TouchInput_IsTouchActive() == 0) {
        return 0;
    }
    TouchInput_GetCoord(&coord);
    x = sym->sprite.posX;
    y = sym->sprite.posY;
    if (sym->sprite.posX - sym->halfW < coord.x && coord.x < x + sym->halfW && y - sym->halfH < coord.y &&
        coord.y < y + sym->halfH)
    {
        return 1;
    }
    return 0;
}

void func_ov030_020bae10(FldNSymbolUG* sym, void (*update)(TaskPool*, FldNSymbolUG*)) {
    sym->update = update;
}

void func_ov030_020bae18(TaskPool* pool, FldNSymbolUG* sym) {
    TouchCoord coord;

    if (TouchInput_IsTouchActive() == 0) {
        sym->x12 = sym->sprite.posX << 0xC;
        sym->y12 = sym->sprite.posY << 0xC;
        func_ov030_020bae10(sym, func_ov030_020bae90);
        return;
    }
    TouchInput_GetCoord(&coord);
    sym->sprite.posX = sym->dragDX + coord.x;
    sym->sprite.posY = sym->dragDY + coord.y;
}

void func_ov030_020bae90(TaskPool* pool, FldNSymbolUG* sym) {
    TouchCoord coord;
    s16        x;
    s16        y;

    if (func_ov030_020bad80(sym) != 0) {
        TouchInput_GetCoord(&coord);
        sym->dragDX = sym->sprite.posX - coord.x;
        sym->dragDY = sym->sprite.posY - coord.y;
        func_ov030_020bae10(sym, func_ov030_020bae18);
        return;
    }
    Field_GetPlayerScreenPos(&x, &y);
    func_ov030_020be430(&sym->x12, &sym->y12, FX_Atan2Idx((y << 0xC) - sym->y12, (x << 0xC) - sym->x12), sym->speed);
    sym->sprite.posX = sym->x12 >> 0xC;
    sym->sprite.posY = sym->y12 >> 0xC;
}

void func_ov030_020baf48(FldNSymbolUG* sym) {
    if (sym->xWobbleFlag != 0) {
        sym->sprite.posX = sym->sprite.posX - 3;
        if (sym->sprite.posX <= (s16)(sym->home.sx - 0x20)) {
            sym->xWobbleFlag = 0;
        }
    } else {
        sym->sprite.posX = sym->sprite.posX + 3;
        if (sym->sprite.posX >= (s16)(sym->home.sx + 0x20)) {
            sym->xWobbleFlag = 1;
        }
    }
    if (sym->yWobbleFlag != 0) {
        sym->sprite.posY = sym->sprite.posY - 1;
        if (sym->sprite.posY <= (s16)(sym->home.sy - 0xC)) {
            sym->yWobbleFlag = 0;
        }
    } else {
        sym->sprite.posY = sym->sprite.posY + 1;
        if (sym->sprite.posY >= (s16)(sym->home.sy + 0xC)) {
            sym->yWobbleFlag = 1;
        }
    }
}

s32 func_ov030_020bb004(s32 category) {
    if (category >= 0x17 && category < 0x1F) {
        return 1;
    }
    return 0;
}

void func_ov030_020bb020(FldNSymbolUG* sym) {
    s16 halfW;
    s16 halfH;
    s16 x;
    s16 y;
    s16 angle;

    if (sym->timer != 0) {
        sym->timer = sym->timer - 1;
    } else {
        angle = (s16)FX_Atan2Idx(sym->y12b - sym->y12, sym->x12b - sym->x12);
        func_02026564(&sym->angle, (u16)angle, 2);
        sym->timer = RNG_Next(0x3C);
    }
    func_ov030_020be430(&sym->x12, &sym->y12, sym->angle, sym->angleStep);
    if (FieldScan_IsEnteringBattle() != 0) {
        halfW = sym->halfW;
        x     = sym->sprite.posX;
        if (x <= -halfW) {
            return;
        }
        if (x >= halfW + 0x100) {
            return;
        }
        halfH = sym->halfH;
        y     = sym->sprite.posY;
        if (y <= -halfH) {
            return;
        }
        if (y >= halfH + 0xC0) {
            return;
        }
        if (x <= halfW || x >= 0x100 - halfW || y <= halfH || y >= 0xC0 - halfH) {
            if (sym->turnFlag == 0) {
                sym->angle    = sym->angle + 0x8000;
                sym->turnFlag = 1;
            }
        } else {
            sym->turnFlag = 0;
        }
    }
    sym->sprite.posX = sym->x12 >> 0xC;
    sym->sprite.posY = sym->y12 >> 0xC;
}

void func_ov030_020bb14c(FldNSymbolUG* sym) {
    u16 t;

    sym->timer = sym->timer + 1;
    t          = sym->timer;
    if (t <= 0x14) {
        sym->y12 -= 0x33;
        sym->y12 -= 0x300;
    } else if (t <= 0x3C) {
        sym->y12 = sym->y12 + 0x333;
    } else if (t < 0x50) {
        sym->y12 -= 0x33;
        sym->y12 -= 0x300;
    } else {
        sym->timer = 0;
        sym->y12 -= 0x33;
        sym->y12 -= 0x300;
    }
    sym->sprite.posX = sym->x12 >> 0xC;
    sym->sprite.posY = sym->y12 >> 0xC;
}

void func_ov030_020bb1e4(TaskPool* pool, FldNSymbolUG* sym) {
    Vec        pos;
    TouchCoord coord;
    s16        mode;

    if (FieldScan_IsEnteringBattle() != 0 && func_ov030_020bad80(sym) != 0 && FieldScan_CanChainEncounter() != 0) {
        TouchInput_GetCoord(&coord);
        sym->dragDX = sym->sprite.posX - coord.x;
        sym->dragDY = sym->sprite.posY - coord.y;
        func_ov030_020bae10(sym, func_ov030_020bae18);
        FieldScan_AddChainedEncounter();
        sym->state = 2;
        if (sym->taskId != (u32)-1) {
            return;
        }
        sym->taskId =
            func_ov030_020bc028(pool, sym->spawnType, (u16*)&sym->sprite.posX, 1, data_ov030_020e5f28[sym->catIdx].arg);
        FieldScan_RaiseBgLevel(sym->category);
        SndMgr_PlaySEWithPan(0x113, sym->sprite.posX);
        return;
    }
    mode = sym->mode;
    if (mode == 1) {
        func_ov030_020baf48(sym);
    } else if (mode >= 3) {
        FO3DViewer_GetNodePos(data_ov030_021072c8[mode - 3], &pos);
        sym->sprite.posX = (pos.x - sym->anchor->x) >> 0xC;
        sym->sprite.posY = (0 - (pos.y + sym->anchor->y)) >> 0xC;
    } else if (mode == 2) {
        func_ov030_020bb020(sym);
    } else {
        func_ov030_020bb14c(sym);
    }
}

void func_ov030_020bb35c(TaskPool* pool, FldNSymbolUG* sym) {
    if (func_ov030_020bad80(sym) == 0) {
        return;
    }
    FieldScan_CanChainEncounter();
}

void func_ov030_020bb378(TaskPool* pool, FldNSymbolUG* sym) {
    if (EasyTask_ValidateTaskId(pool, &sym->taskId2) != 0) {
        return;
    }
    func_ov030_020bae10(sym, func_ov030_020bb1e4);
    if (sym->category == 0x16) {
        func_ov030_020bae10(sym, func_ov030_020bb35c);
        SndMgr_PlaySEWithPan(0x130, sym->sprite.posX);
        return;
    }
    if (func_ov030_020bb004(sym->category) == 0) {
        return;
    }
    sym->speed = 0x1000;
    func_ov030_020bae10(sym, func_ov030_020bae90);
    sym->state = 2;
    if (sym->taskId != (u32)-1) {
        return;
    }
    sym->taskId = func_ov030_020bc028(pool, sym->spawnType, (u16*)&sym->sprite.posX, 1, data_ov030_020e5f28[sym->catIdx].arg);
}

void func_ov030_020bb43c(TaskPool* pool, FldNSymbolUG* sym) {
    if ((sym->dist < 0 || sym->dist > FieldScan_GetRadius()) && func_ov030_020aef2c() != 0) {
        return;
    }
    func_ov030_020bae10(sym, func_ov030_020bb378);
    sym->taskId2 =
        func_ov030_020c5704(pool, sym->spawnType, (sym->x12 * 0x10) >> 0x10, (sym->y12 * 0x10) >> 0x10, sym->category);
    if (sym->category == 8) {
        SndMgr_PlaySEWithPan(0x144, (sym->x12 * 0x10) >> 0x10);
    } else {
        SndMgr_PlaySEWithPan(0x111, (sym->x12 * 0x10) >> 0x10);
    }
}

void func_ov030_020bb4ec(TaskPool* pool, FldNSymbolUG* sym) {
    if (sym->timer == 0) {
        return;
    }
    func_020265d4(&sym->x12, sym->x12b, sym->timer);
    func_020265d4(&sym->y12, sym->y12b, sym->timer);
    sym->timer       = sym->timer - 1;
    sym->sprite.posX = sym->x12 >> 0xC;
    sym->sprite.posY = sym->y12 >> 0xC;
}

SpriteFrameInfo* func_ov030_020bb544(Sprite* sprite, s32 arg, s32 mode) {
    SpriteFrameInfo* info;

    info = NULL;
    switch (mode) {
        case SPRITE_FRAME_UPDATE:
            g_SpriteFrameInfo.updateSteps = 1;
            return &g_SpriteFrameInfo;

        case SPRITE_FRAME_RENDER: {
            // The second copy of the base pointer matters for codegen: with only
            // `info = &g_SpriteFrameInfo;` mwcc reuses the base register for the
            // sprite->bits_0_1 load and has to reload the base from the pool for the
            // sortKey store. Copying it through a local keeps the base in a register
            // across the load, which is what the target does.
            SpriteFrameInfo* temp = &g_SpriteFrameInfo;
            Sprite_FillFrameInfo(temp, sprite);
            info = temp;
            if (sprite->bits_0_1 == 0) {
                info->sortKey = 0x180 - sprite->posY * 2;
            } else {
                info->sortKey = (sprite->posY << 0xC) + 0x1C3000;
            }
        }
        /* fallthrough */
        default:
            return info;
    }
}

// Nonmatching: the category == 0x16 remap: retail emits a plain `mov r12, #0xe`
// before a runtime palette load; the guard form here gives `moveq` (USA 97.59%; JP unvalidated).
void func_ov030_020bb614(Sprite* sprite, s32 dataType, FldScreenPos pos, s32 category) {
    SpriteAnimation anim;
    u16             type;

    anim          = data_ov030_020e5efc;
    type          = (u16)dataType;
    anim.dataType = type;
    anim.posX     = pos.x;
    anim.posY     = pos.y;
    // Category normalization shared with func_ov030_020bb760: the upper bands
    // map onto table rows via their band base.
    if (category >= 0x1F) {
        anim.unk_20 = 0x32;
        category -= 0x1F;
    } else if (category >= 0x17) {
        anim.unk_20 = 0x2F;
        category -= 0x17;
    } else if (category >= 0xE && category < 0x16) {
        anim.unk_20 = 0x31;
        category -= 0xE;
    } else if (category == 0x16) {
        // A plain `category = 0xE;` lets mwcc fold the palette read below into
        // an immediate address; the target keeps it as a runtime table load.
        category      = category == 0x16 ? 0xE : category;
        anim.bits_0_1 = 2;
        anim.unk_20   = data_ov030_020e5f28[category].paletteIndex;
    } else {
        anim.unk_20 = data_ov030_020e5f28[category].paletteIndex;
    }
    anim.unk_1C = data_ov030_020e5f28[category].charDataIndex;
    anim.unk_26 = data_ov030_020e5f28[category].cellTableIndex;
    anim.unk_28 = data_ov030_020e5f28[category].animDataIndex;
    _Sprite_Load(sprite, &anim);
}

s32 func_ov030_020bb760(TaskPool* pool, Task* task, void* args) {
    NsymSpawnArgs* params;
    FldNSymbolUG*  sym;
    Vec            world;
    FldScreenPos   pos;
    u16            py;
    s32            dx;
    s32            dy;

    params                = args;
    sym                   = task->data;
    sym->mode             = params->mode;
    sym->progressFlagMask = params->progressFlagMask;
    sym->anchor           = params->anchor;
    sym->recIdx           = params->recIdx;
    sym->catIdx           = params->category;
    sym->category         = params->category;
    sym->turnFlag         = 0;
    sym->speed            = 0x3CCD;
    // Same category normalization as func_ov030_020bb614.
    if (sym->catIdx >= 0x1F) {
        sym->catIdx -= 0x1F;
    } else if (sym->catIdx >= 0x17) {
        sym->catIdx -= 0x17;
    } else if (sym->catIdx >= 0xE && sym->catIdx < 0x16) {
        sym->catIdx -= 0xE;
    } else if (sym->catIdx == 0x16) {
        sym->catIdx = 0xE;
    }
    if ((u32)sym->catIdx >= 0xF) {
        sym->catIdx = 0;
    }
    sym->halfW = data_ov030_020e5f28[sym->catIdx].halfW;
    sym->halfH = data_ov030_020e5f28[sym->catIdx].halfH;
    if (sym->mode == 1) {
        py          = params->y;
        pos.y       = py;
        sym->home.x = params->x;
        sym->home.y = py;
        pos.x       = sym->home.x;
    } else if (sym->mode >= 3) {
        FO3DViewer_GetNodePos(data_ov030_021072c8[sym->mode - 3], &world);
        pos.x = (world.x - sym->anchor->x) >> 0xC;
        pos.y = (0 - (world.y + sym->anchor->y)) >> 0xC;
    } else {
        pos.x = (s16)params->x - (sym->anchor->x >> 0xC);
        pos.y = (s16)params->y - (sym->anchor->y >> 0xC);
    }
    sym->x12  = pos.sx << 0xC;
    sym->x12b = sym->x12;
    sym->y12  = pos.sy << 0xC;
    sym->y12b = sym->y12;
    func_ov030_020bb614(&sym->sprite, params->spawnType, pos, params->category);
    sym->xWobbleFlag = (u32)RNG_Next(0x100) >> 7;
    sym->yWobbleFlag = (u32)RNG_Next(0x100) >> 7;
    sym->timer       = 0;
    if (sym->mode == 2) {
        sym->timer     = 4;
        sym->angleStep = 0x666;
        sym->angle     = RNG_Next(0x10000);
    }
    sym->spawnType = params->spawnType;
    sym->taskId    = (u32)-1;
    sym->state     = 0;
    sym->taskId2   = (u32)-1;
    if (sym->anchor == NULL) {
        sym->dist = 1;
        func_ov030_020bae10(sym, func_ov030_020bb1e4);
    } else {
        dx = gSaveData.fieldPos.x - (sym->anchor->x + sym->x12);
        dy = (gSaveData.fieldPos.y - func_ov030_020aeca4(gSaveData.fieldPos.y) * 0x2C) - (sym->anchor->y + sym->y12);
        if ((dx < 0 ? -dx : dx) >= 0x100000 || (dy < 0 ? -dy : dy) >= 0xC0000) {
            sym->dist = -1;
        } else {
            sym->dist = FX_Sqrt((s32)(((s64)dx * dx + 0x800) >> 12) + (s32)(((s64)dy * dy + 0x800) >> 12));
        }
        func_ov030_020bae10(sym, func_ov030_020bb43c);
    }
    return 1;
}

s32 func_ov030_020bba90(FldNSymbolUG* sym) {
    if (sym->update == func_ov030_020bb378) {
        return 0;
    }
    return sym->update != func_ov030_020bb43c;
}

s32 func_ov030_020bbac0(TaskPool* pool, Task* task, void* args) {
    FldNSymbolUG* sym;

    sym = task->data;
    if (sym->update != NULL) {
        sym->update(pool, sym);
    }
    if (func_ov030_020bba90(sym) != 0) {
        Sprite_UpdateAndCheck(&sym->sprite);
    }
    return 1;
}

s32 func_ov030_020bbafc(TaskPool* pool, Task* task, void* args) {
    FldNSymbolUG* sym;

    sym = task->data;
    if (func_ov030_020bba90(sym) != 0) {
        Sprite_Render(&sym->sprite);
    }
    return 1;
}

s32 func_ov030_020bbb24(TaskPool* pool, Task* task, void* args) {
    FldNSymbolUG* sym;

    sym = task->data;
    if (sym->taskId != (u32)-1) {
        EasyTask_DeleteTask(pool, sym->taskId);
    }
    if (sym->taskId2 != (u32)-1) {
        EasyTask_DeleteTask(pool, sym->taskId2);
    }
    Sprite_Destroy(&sym->sprite);
    return 1;
}

s32 func_ov030_020bbb6c(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = data_ov030_020e5eec;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020bbbb4(TaskPool* pool, s32 spawnType, FldScreenPos pos, s32 category, s16 mode, s32 progressFlagMask,
                        FieldPos* anchor, s16 recIdx) {
    NsymSpawnArgs params;

    params.spawnType        = spawnType;
    params.x                = pos.x;
    params.y                = pos.y;
    params.category         = category;
    params.mode             = mode;
    params.progressFlagMask = progressFlagMask;
    params.anchor           = anchor;
    params.recIdx           = recIdx;
    return EasyTask_CreateTask(pool, &data_ov030_020e5ee0, NULL, 0, NULL, &params);
}

s32 func_ov030_020bbc24(TaskPool* pool, u32 id) {
    return func_ov030_020bad80(EasyTask_GetTaskById(pool, id)->data);
}

s32 func_ov030_020bbc38(TaskPool* pool, u32 id) {
    FldNSymbolUG* sym;
    s16           x;
    s16           y;

    sym = EasyTask_GetTaskById(pool, id)->data;
    if (sym->state != 2) {
        return 0;
    }
    Field_GetPlayerScreenPos(&x, &y);
    if (x - 0x10 < sym->sprite.posX && sym->sprite.posX < x + 0x10 && y - 0x10 < sym->sprite.posY &&
        sym->sprite.posY < y + 0x20)
    {
        return 1;
    }
    return 0;
}

void func_ov030_020bbcb4(TaskPool* pool, u32 id) {
    FldNSymbolUG* sym;
    s16           x;
    s16           y;

    sym = EasyTask_GetTaskById(pool, id)->data;
    if (sym->category == 0x16) {
        return;
    }
    FieldScan_AddChainedEncounter();
    sym->timer = 0x5A;
    sym->state = 1;
    Field_GetPlayerScreenPos(&x, &y);
    sym->x12b = x << 0xC;
    sym->y12b = y << 0xC;
    sym->x12  = sym->sprite.posX << 0xC;
    sym->y12  = sym->sprite.posY << 0xC;
    func_ov030_020bae10(sym, func_ov030_020bb4ec);
    func_ov030_020bbdc4(pool, id);
    if (sym->taskId != (u32)-1) {
        EasyTask_DeleteTask(pool, sym->taskId);
    }
    sym->taskId = func_ov030_020bc028(pool, sym->spawnType, (u16*)&sym->sprite.posX, 0, data_ov030_020e5f28[sym->catIdx].arg);
    if (sym->category == 8) {
        SndMgr_PlaySEWithPan(0x143, sym->sprite.posX);
    } else {
        SndMgr_PlaySEWithPan(0x112, sym->sprite.posX);
    }
}

void func_ov030_020bbdc4(TaskPool* pool, u32 id) {
    FldNSymbolUG* sym;

    sym = EasyTask_GetTaskById(pool, id)->data;
    if (sym->progressFlagMask != 0) {
        func_ov030_020aebf0(sym->progressFlagMask);
    }
    if (sym->recIdx >= 0) {
        gSaveData.scanSymbols[sym->recIdx].state = 2;
    }
}
