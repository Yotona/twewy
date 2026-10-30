#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/Input.h"
#include "Engine/Math/Random.h"
#include "SpriteMgr.h"

#include <nitro/mi/cpumem.h>

// The command display (`Tsk_BtlCmd`): the row of arrow cells that the player's partner shows while the player works
// through a command chain. Three records (the current one and the two it can branch into) each hold sixteen cells and
// two marker sprites; the pad or the stylus moves along the cells, and the display ends the chain with a sound.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

struct BtlCmd;

typedef void (*BtlCmdState)(struct BtlCmd*);

/// One cell of a record: what it shows and the animation it plays.
typedef struct BtlCmdCell {
    /* 0x0 */ u16 state; // 0 unused, 1 more cells follow, 2 last cell
    /* 0x2 */ u16 anim;
} BtlCmdCell;            // Size: 0x4

/// A row of cells with the two markers that branch off it.
typedef struct BtlCmdRecord {
    /* 0x000 */ s16          kind;   // which of the three the record is
    /* 0x002 */ s16          first;  // cell where the first branch marker sits, -1 for none
    /* 0x004 */ s16          second; // ... and the second
    /* 0x006 */ u16          value;
    /* 0x008 */ s32          ready;
    /* 0x00C */ CombatSprite marker;
    /* 0x06C */ CombatSprite cell;
    /* 0x0CC */ BtlCmdCell   cells[16];
} BtlCmdRecord; // Size: 0x10C

/// The length and branch ranges of one chain of cells: a row of seven halfwords.
typedef struct BtlCmdParams {
    /* 0x0 */ s16 length;
    /* 0x2 */ s16 firstMin;
    /* 0x4 */ s16 firstMax;
    /* 0x6 */ s16 branchMin;
    /* 0x8 */ s16 branchMax;
    /* 0xA */ s16 markerMin;
    /* 0xC */ s16 markerMax;
} BtlCmdParams; // Size: 0xE

typedef struct BtlCmd {
    /* 0x000 */ u8            reserved000[6];
    /* 0x006 */ s16           timer; // frames left to answer
    /* 0x008 */ u8            reserved008[0x11C - 0x8];
    /* 0x11C */ CombatSprite  frame;
    /* 0x17C */ CombatSprite  firstMarker;
    /* 0x1DC */ CombatSprite  secondMarker;
    /* 0x23C */ CombatSprite  burst;
    /* 0x29C */ s32           x;
    /* 0x2A0 */ s32           y;
    /* 0x2A4 */ s32           targetX;
    /* 0x2A8 */ s32           targetY;
    /* 0x2AC */ s32           cursorX;
    /* 0x2B0 */ s32           cursorY;
    /* 0x2B4 */ s32           burstY;
    /* 0x2B8 */ s16           cellWidth; // +0x10 when the chain runs to the right, -0x10 to the left
    /* 0x2BA */ u16           reserved2BA;
    /* 0x2BC */ s32           bursting;
    /* 0x2C0 */ s32           visible;
    /* 0x2C4 */ BtlCmdState   state;
    /* 0x2C8 */ s16           unk_2C8;
    /* 0x2CA */ s16           moveTimer;
    /* 0x2CC */ s16           cursor; // index of the cell the cursor is on
    /* 0x2CE */ s16           step;   // how many records have been answered
    /* 0x2D0 */ u16           wanted;
    /* 0x2D2 */ u16           input;
    /* 0x2D4 */ u16           mode;
    /* 0x2D6 */ s16           counter;
    /* 0x2D8 */ u8            chain;
    /* 0x2D9 */ u8            reserved2D9;
    /* 0x2DA */ BtlCmdParams  params[2];
    /* 0x2F6 */ u8            reserved2F6[2];
    /* 0x2F8 */ BtlCmdRecord  records[3];
    /* 0x61C */ BtlCmdRecord* current;
    /* 0x620 */ BtlCmdRecord* otherA;
    /* 0x624 */ BtlCmdRecord* otherB;
} BtlCmd; // Size: 0x628

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern const BinIdentifier data_ov003_020d7808;
extern u8                  data_02071cf0[];

extern void func_ov003_020868c8(u16 value);
extern void func_ov003_0208697c(void);
extern void func_ov003_020869c8(s32 value);
extern s32  func_ov003_02086a64(void);
extern s32  func_ov003_02086a80(void);
extern void func_ov003_02086af0(void);
extern void func_ov003_02086b14(Ov003ChargeState* state);
extern u16  func_ov003_02086d24(void);
extern void func_ov003_02086d8c(CombatSprite* sprite, s32 animId, s32 flipped, s32 x, s16 y, s16 z);
extern void func_ov003_02086e38(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern s32  func_ov003_02086f7c(void);
extern void func_02026590(s32* dst, s32 target, u16 weight);

void func_ov003_0209842c(BtlCmd* self);
void func_ov003_02098458(BtlCmd* self);
void func_ov003_02098484(BtlCmd* self);
void func_ov003_0209862c(BtlCmd* self);
s32  func_ov003_02098718(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020988b4(TaskPool* pool, Task* task, void* args);
s32  func_ov003_020989e8(TaskPool* pool, Task* task, void* args);
s32  func_ov003_02098b60(TaskPool* pool, Task* task, void* args);
s32  func_ov003_02098bc0(TaskPool* pool, Task* task, void* args, s32 stage);

char             data_ov003_020e3c54[0xC] = "Tsk_BtlCmd";
const TaskHandle data_ov003_020d7eec      = {data_ov003_020e3c54, func_ov003_02098bc0, sizeof(BtlCmd)};

/// The stage table of the display: build, update, draw, release.
const TaskStages data_ov003_020d7ef8 = {
    func_ov003_02098718,
    func_ov003_020988b4,
    func_ov003_020989e8,
    func_ov003_02098b60,
};

/// The frames to wait before a held pad is read as pressed again: pairs of (short, long) for each difficulty.
const s16 data_ov003_020d7f08[1] = {0};
const s16 data_ov003_020d7f0a[7] = {0, 0x1E, 0x78, 0xB4, 0x78, 0x168, 0x78};

/// The chain parameters, picked by the badges the player wears.
const BtlCmdParams data_ov003_020d7f18[9] = {
    {0, 7, 8, 1, 1, 0, 0},
    {0, 4, 5, 1, 1, 0, 0},
    {0, 1, 3, 1, 1, 0, 0},
    {1, 7, 8, 1, 2, 7, 7},
    {1, 4, 5, 1, 1, 1, 1},
    {1, 2, 3, 2, 3, 0, 3},
    {1, 2, 5, 2, 5, 0, 5},
    {2, 6, 6, 1, 2, 6, 6},
    {3, 2, 2, 2, 2, 0, 2},
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

void func_ov003_02097544(BtlCmd* self, BtlCmdState state) {
    self->state     = state;
    self->unk_2C8   = 0;
    self->moveTimer = 0;
}

/// Loads a cell sprite.
void func_ov003_0209755c(CombatSprite* sprite, s32 index, s32 mode) {
    SpriteAnimationEx anim;

    CombatSprite_InitAnim(&anim.anim, 1, &data_ov003_020d7808);
    anim.anim.bits_10_11 = 0;
    anim.anim.unk_18     = 2;
    anim.anim.packIndex  = 1;
    anim.anim.unk_1C     = 1;
    anim.anim.unk_26     = 2;
    anim.anim.unk_28     = 3;
    anim.anim.unk_20     = 4;
    anim.anim.animIndex  = index + 1;
    anim.animTable       = NULL;
    CombatSprite_Load(sprite, &anim);
    func_ov003_02082738(sprite, 0);
    CombatSprite_SetAnim(sprite, index, mode);
    func_ov003_02082730(sprite, 3);
}

/// Loads a marker sprite for the kind of effect command the battle is running.
void func_ov003_02097604(CombatSprite* sprite) {
    SpriteAnimationEx anim;
    Ov003EffectCmd*   command = data_ov003_020e71b8->effectCmd;

    CombatSprite_InitAnim(&anim.anim, 1, &data_ov003_020d7808);
    switch (command->mode) {
        case 1:
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_18     = 2;
            anim.anim.packIndex  = 3;
            anim.anim.unk_1C     = 4;
            anim.anim.unk_26     = 5;
            anim.anim.unk_28     = 6;
            anim.anim.unk_20     = 7;
            anim.anim.animIndex  = 1;
            anim.animTable       = NULL;
            break;
        case 2:
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_18     = 2;
            anim.anim.packIndex  = 4;
            anim.anim.unk_1C     = 1;
            anim.anim.unk_26     = 2;
            anim.anim.unk_28     = 3;
            anim.anim.unk_20     = 4;
            anim.anim.animIndex  = 1;
            anim.animTable       = NULL;
            break;
        case 0:
        default:
            anim.anim.bits_10_11 = 0;
            anim.anim.unk_18     = 2;
            anim.anim.packIndex  = 2;
            anim.anim.unk_1C     = 4;
            anim.anim.unk_26     = 5;
            anim.anim.unk_28     = 6;
            anim.anim.unk_20     = 7;
            anim.anim.animIndex  = 1;
            anim.animTable       = NULL;
            break;
    }
    CombatSprite_Load(sprite, &anim);
    func_ov003_02082738(sprite, 0);
    func_ov003_02082730(sprite, 3);
}

/// The chain parameter row that applies when no badge asks for another.
s32 func_ov003_02097760(s32 side) {
    s32 row = 5;

    switch (data_ov003_020e71b8->unk3D88D) {
        case 0:
            if (side != 1) {
                row = 4;
            }
            break;
        case 1:
            break;
        case 2:
            if (side != 1) {
                row = 4;
            }
            break;
        default:
            break;
    }
    return row;
}

/// Adds `amount` to `*value`, keeping it between 1 and 16.
void func_ov003_020977b0(s16* value, s32 amount) {
    *value = *value + amount;
    if (*value > 0x10) {
        *value = 0x10;
    }
    if (*value < 1) {
        *value = 1;
    }
}

/// Picks the two chains' parameters and lets the badges stretch them.
void func_ov003_020977e0(BtlCmd* self) {
    s32 row;
    s16 bonus;

    if (func_ov003_020a3330(0xA) != 0) {
        row = func_ov003_020a3468(0xA);
    } else {
        row = func_ov003_02097760(0);
    }
    self->params[0] = data_ov003_020d7f18[row];
    if (func_ov003_020a3330(0xB) != 0) {
        row = func_ov003_020a3468(0xB);
    } else {
        row = func_ov003_02097760(1);
    }
    self->params[1] = data_ov003_020d7f18[row];
    if (func_ov003_020a3330(8) != 0) {
        bonus = func_ov003_020a3468(8);
        func_ov003_020977b0(&self->params[0].firstMin, bonus);
        func_ov003_020977b0(&self->params[0].firstMax, bonus);
        func_ov003_020977b0(&self->params[0].branchMin, bonus);
        func_ov003_020977b0(&self->params[0].branchMax, bonus);
        func_ov003_020977b0(&self->params[1].firstMin, bonus);
        func_ov003_020977b0(&self->params[1].firstMax, bonus);
        func_ov003_020977b0(&self->params[1].branchMin, bonus);
        func_ov003_020977b0(&self->params[1].branchMax, bonus);
    }
    if (func_ov003_020a3330(0xC) != 0) {
        s16 extra = func_ov003_020a3468(0xC);

        self->params[0].length += extra;
        self->params[1].length += extra;
    }
    self->chain = 0;
}

/// Which way the enemies mostly stand compared with the gauge actor: 0x800 for one side, 1 for the other.
u32 func_ov003_02097984(u32 wanted) {
    s32              ahead  = 0;
    s32              behind = 0;
    u32              result = 0;
    CombatActor*     gauge  = data_ov003_020e71b8->unk3D89C;
    Ov003CutinActor* enemy  = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8AC);

    while (enemy != NULL) {
        if (!(enemy->base.actor.flags & 4) && !(enemy->base.actor.flags & 0x10)) {
            if (gauge->position.x < enemy->base.actor.position.x) {
                ahead++;
            } else {
                behind++;
            }
        }
        enemy = func_ov003_02084984(&enemy->base.node);
    }
    if (wanted == 0x800 && behind > 0) {
        return wanted;
    }
    if (wanted == 1 && ahead > 0) {
        return wanted;
    }
    if (behind + ahead > 0) {
        if (behind > ahead) {
            result = 0x800;
        } else {
            result = 1;
        }
    }
    return result;
}

/// Counts the wait before the pad is read again.
void func_ov003_02097a40(BtlCmd* self) {
    s32 level = (data_02071cf0[0x39] >> 2) & 3;
    s32 wait;

    if (level == 0) {
        return;
    }
    switch (self->mode) {
        case 0:
            wait = data_ov003_020d7f08[level * 2];
            break;
        case 1:
            wait = data_ov003_020d7f08[level * 2];
            break;
        case 2:
            wait = data_ov003_020d7f0a[level * 2];
            break;
        default:
            return;
    }
    self->counter++;
    if (self->counter < wait) {
        return;
    }
    data_ov003_020e71b8->unk3D878 |= 0x10;
    self->mode    = 2;
    self->counter = 0;
    self->wanted  = func_ov003_02097984(self->wanted);
    self->input   = self->wanted;
}

void func_ov003_02097b1c(BtlCmd* self) {
    s32 level = (data_02071cf0[0x39] >> 2) & 3;

    if (level == 0) {
        return;
    }
    if (self->mode != 2) {
        return;
    }
    self->counter++;
    if (self->counter < 0xF) {
        return;
    }
    self->counter = 0;
    self->input   = self->wanted;
}

/// Reads the pad.
void func_ov003_02097b70(BtlCmd* self) {
    s32 level = (data_02071cf0[0x39] >> 2) & 3;

    if (level == 0) {
        self->input = InputStatus.buttonState.holdButtons;
        return;
    }
    if (InputStatus.buttonState.currButtons & (INPUT_BUTTON_A | INPUT_BUTTON_B | INPUT_BUTTON_RIGHT | INPUT_BUTTON_LEFT |
                                               INPUT_BUTTON_UP | INPUT_BUTTON_DOWN | INPUT_BUTTON_X | INPUT_BUTTON_Y))
    {
        data_ov003_020e71b8->unk3D878 &= ~0x10;
        self->mode    = 1;
        self->counter = 0;
    }
    self->input = InputStatus.buttonState.holdButtons;
}

/// Shows a cell: more follow, or it is the last (and `value` says which way the chain runs).
void func_ov003_02097c00(BtlCmdCell* cell, s32 value, s32 last) {
    u16 anim;

    if (value > 0) {
        anim = 6;
    } else {
        anim = 4;
    }
    if (last != 0) {
        anim = anim + 1;
    }
    cell->anim = anim;
}

void func_ov003_02097c24(BtlCmdRecord* record, s32 width) {
    s32 i;

    for (i = 0; i < 16; i++) {
        func_ov003_02097c00(&record->cells[i], width, 1);
    }
    record->ready = 1;
}

/// Sets the frame and the two marker sprites from where the cursor is.
void func_ov003_02097c68(BtlCmd* self) {
    BtlCmdRecord* current = self->current;
    s16           anim    = 0;

    if (self->cursor == current->first) {
        CombatSprite_SetAnim(&self->firstMarker, 9, 0);
        anim = 1;
    } else if (self->cursor > current->first) {
        CombatSprite_SetAnim(&self->firstMarker, 10, 0);
        func_ov003_02097c24(self->otherA, self->cellWidth);
    }
    if (self->cursor == current->second) {
        CombatSprite_SetAnim(&self->secondMarker, 12, 0);
        anim = anim + 2;
    } else if (self->cursor > current->second) {
        CombatSprite_SetAnim(&self->secondMarker, 13, 0);
        func_ov003_02097c24(self->otherB, self->cellWidth);
    }
    CombatSprite_SetAnim(&self->frame, (u16)anim, 0);
}

/// Lays a record out: `count` cells, the branch markers somewhere inside the allowed range.
void func_ov003_02097d40(BtlCmd* self, BtlCmdRecord* record, s32 count) {
    BtlCmdParams* params = &self->params[self->chain];
    s32           last   = count - 1;
    s32           i;
    s32           lim;
    s32           lo;
    s32           span;

    for (i = 0; i < last; i++) {
        record->cells[i].state = 1;
    }
    record->cells[last].state = 2;
    for (i = count; i < 16; i++) {
        record->cells[i].state = 0;
    }
    record->value = func_ov003_02086d24();
    if (self->current != record && self->step >= params->length - 1) {
        record->first  = -1;
        record->second = -1;
    } else if (self->step >= params->length) {
        record->first  = -1;
        record->second = -1;
    } else {
        lim = params->markerMax;
        if (lim > last) {
            lim = last;
        }
        lo = params->markerMin;
        if (lo > lim) {
            lo = lim;
        }
        span           = lim - lo;
        record->first  = lo + RNG_Next(span + 1);
        record->second = lo + RNG_Next(span + 1);
    }
    for (i = 0; i < 16; i++) {
        func_ov003_02097c00(&record->cells[i], self->cellWidth, 0);
    }
    record->ready = 0;
}

void func_ov003_02097e8c(BtlCmd* self, BtlCmdRecord* record) {}

/// Draws the cells of a record from (`x`,`y`).
void func_ov003_02097e90(BtlCmd* self, BtlCmdRecord* record, s16 x, s16 y) {
    s32 i;

    for (i = 0; i < 16; i++) {
        if (record->cells[i].state == 0) {
            break;
        }
        CombatSprite_SetAnim(&record->cell, record->cells[i].anim, 0);
        CombatSprite_SetPosition(&record->cell, x, y);
        CombatSprite_Update(&record->cell);
        CombatSprite_Render(&record->cell);
        x = (s16)(x + self->cellWidth);
    }
    func_ov003_02086d8c(&record->marker, record->value, record->ready, x, y, self->cellWidth);
}

/// Starts the chain `chain`; `direction` 2 runs it to the right.
void func_ov003_02097f3c(BtlCmd* self, s32 chain, s32 direction) {
    BtlCmdParams* params = &self->params[chain];

    if (direction == 2) {
        self->cellWidth = 0x10;
    } else {
        self->cellWidth = -0x10;
    }
    func_ov003_02086af0();
    func_ov003_020869c8(direction);
    if (func_ov003_02086f7c() != 0) {
        func_ov003_020871cc(1);
    }
    self->step    = 0;
    self->chain   = chain;
    self->cursor  = 0;
    self->targetX = self->x;
    self->targetY = self->y;
    self->cursorX = self->x + (self->cellWidth << 12);
    self->cursorY = self->y;
    func_ov003_020868c8(1);
    CombatSprite_SetAnim(&self->frame, 0, 0);
    CombatSprite_SetAnim(&self->firstMarker, 8, 0);
    CombatSprite_SetAnim(&self->secondMarker, 11, 0);
    func_ov003_02097d40(self, self->current, params->firstMin + RNG_Next(params->firstMax - params->firstMin + 1));
    func_ov003_02097d40(self, self->otherA, params->branchMin + RNG_Next(params->branchMax - params->branchMin + 1));
    func_ov003_02097d40(self, self->otherB, params->branchMin + RNG_Next(params->branchMax - params->branchMin + 1));
}

/// Moves on to `record` after a branch was taken.
void func_ov003_02098090(BtlCmd* self, BtlCmdRecord* record) {
    BtlCmdParams* params = &self->params[self->chain];

    self->current = record;
    switch (record->kind) {
        case 0:
            self->otherA = &self->records[1];
            self->otherB = &self->records[2];
            break;
        case 1:
            self->otherA = &self->records[0];
            self->otherB = &self->records[2];
            break;
        case 2:
            self->otherA = &self->records[0];
            self->otherB = &self->records[1];
            break;
    }
    self->step++;
    self->cursor  = 0;
    self->targetX = self->x;
    self->targetY = self->y;
    CombatSprite_SetAnim(&self->frame, 0, 0);
    CombatSprite_SetAnim(&self->firstMarker, 8, 0);
    CombatSprite_SetAnim(&self->secondMarker, 11, 0);
    if (record->first == -1 && record->second == -1) {
        return;
    }
    func_ov003_02097d40(self, self->otherA, params->branchMin + RNG_Next(params->branchMax - params->branchMin + 1));
    func_ov003_02097d40(self, self->otherB, params->branchMin + RNG_Next(params->branchMax - params->branchMin + 1));
}

void func_ov003_020981dc(BtlCmd* self, s32 y) {
    self->burstY   = y;
    self->bursting = 1;
    CombatSprite_Restart(&self->burst);
}

/// Answers the cell under the cursor (or ends the chain when `cancel` is set).
void func_ov003_020981f8(BtlCmd* self, s32 cancel) {
    BtlCmdCell* cells;
    s32         cursor;

    if (cancel != 0) {
        self->visible = 0;
        func_ov003_0208697c();
        func_ov003_020868c8(3);
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    cursor = self->cursor * sizeof(BtlCmdCell);
    cells  = self->current->cells;
    switch (((BtlCmdCell*)((u8*)cells + cursor))->state) {
        case 1:
            func_ov003_02087ed8(SEIDX_SE_GIRL_DART_CURSOR01);
            func_ov003_020868c8(1);
            break;
        case 2:
            self->visible = 0;
            func_ov003_02087ed8(SEIDX_SE_GIRL_DART_CURSOR02);
            func_ov003_020868c8(2);
            func_ov003_02086e38(self->current->value, self->x, self->y, self->cellWidth);
            break;
    }
    func_ov003_02097c00((BtlCmdCell*)((u8*)cells + cursor), self->cellWidth, 1);
    self->cursor++;
    self->targetX = self->cursorX - (self->cellWidth << 12);
    self->targetY = self->cursorY;
    func_ov003_020981dc(self, self->y);
    func_ov003_02097c68(self);
    func_ov003_02097544(self, func_ov003_0209862c);
}

/// Waits for the first answer of the left chain.
void func_ov003_0209830c(BtlCmd* self) {
    u16 input = self->input;

    if (input & (INPUT_BUTTON_B | INPUT_BUTTON_DOWN)) {
        func_ov003_020868c8(6);
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    if (input & (INPUT_BUTTON_X | INPUT_BUTTON_UP)) {
        func_ov003_020868c8(7);
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    if (input & (INPUT_BUTTON_Y | INPUT_BUTTON_LEFT)) {
        func_ov003_02097f3c(self, 0, 1);
        func_ov003_02097544(self, func_ov003_0209862c);
        return;
    }
    if (input & (INPUT_BUTTON_A | INPUT_BUTTON_RIGHT)) {
        func_ov003_02097f3c(self, 0, 2);
        func_ov003_02097544(self, func_ov003_0209862c);
    }
}

/// The same for the right chain.
void func_ov003_020983ac(BtlCmd* self) {
    u16 input = self->input;

    if (input & (INPUT_BUTTON_B | INPUT_BUTTON_DOWN)) {
        func_ov003_020868c8(6);
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    if (input & (INPUT_BUTTON_Y | INPUT_BUTTON_LEFT)) {
        func_ov003_02097f3c(self, 1, 1);
        func_ov003_02097544(self, func_ov003_0209862c);
        return;
    }
    if (input & (INPUT_BUTTON_A | INPUT_BUTTON_RIGHT)) {
        func_ov003_02097f3c(self, 1, 2);
        func_ov003_02097544(self, func_ov003_0209862c);
    }
}

void func_ov003_0209842c(BtlCmd* self) {
    if (func_ov003_02086a64() == 0) {
        return;
    }
    func_ov003_0208697c();
    func_ov003_02097544(self, func_ov003_02098458);
}

void func_ov003_02098458(BtlCmd* self) {
    func_ov003_02097a40(self);
    if (func_ov003_02086a80() != 0) {
        func_ov003_020983ac(self);
    } else {
        func_ov003_0209830c(self);
    }
}

/// Follows the cursor along the current record.
void func_ov003_02098484(BtlCmd* self) {
    func_ov003_02097b1c(self);
    if (self->input & (INPUT_BUTTON_Y | INPUT_BUTTON_LEFT)) {
        func_ov003_020981f8(self, self->cellWidth > 0 ? 1 : 0);
        return;
    }
    if (self->input & (INPUT_BUTTON_A | INPUT_BUTTON_RIGHT)) {
        func_ov003_020981f8(self, self->cellWidth <= 0 ? 1 : 0);
        return;
    }
    if (self->input & (INPUT_BUTTON_X | INPUT_BUTTON_UP)) {
        if (self->cursor == self->current->first) {
            func_ov003_020868c8(4);
            func_ov003_02098090(self, self->otherA);
            self->cursorX = self->x + (self->cellWidth << 12);
            self->cursorY = self->y - 0x12000;
            func_ov003_020981dc(self, self->cursorY);
            func_ov003_02097544(self, func_ov003_0209862c);
            return;
        }
        self->visible = 0;
        func_ov003_020868c8(8);
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    if (self->input & (INPUT_BUTTON_B | INPUT_BUTTON_DOWN)) {
        if (self->cursor == self->current->second) {
            func_ov003_020868c8(5);
            func_ov003_02098090(self, self->otherB);
            self->cursorX = self->x + (self->cellWidth << 12);
            self->cursorY = self->y + 0x12000;
            func_ov003_020981dc(self, self->cursorY);
            func_ov003_02097544(self, func_ov003_0209862c);
            return;
        }
        self->visible = 0;
        func_ov003_020868c8(8);
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    if (self->timer > 0) {
        self->timer--;
        return;
    }
    self->visible = 0;
    func_ov003_020868c8(8);
    func_ov003_02097544(self, func_ov003_0209842c);
}

/// Slides the cursor onto the next cell.
void func_ov003_0209862c(BtlCmd* self) {
    if (self->moveTimer == 0) {
        self->moveTimer = 3;
    }
    if (self->moveTimer > 0) {
        func_02026590(&self->cursorX, self->targetX, (u16)self->moveTimer);
        func_02026590(&self->cursorY, self->targetY, self->moveTimer);
        self->moveTimer--;
    }
    if (self->moveTimer > 0) {
        return;
    }
    if (self->current->cells[self->cursor].state == 0) {
        func_ov003_02097544(self, func_ov003_0209842c);
        return;
    }
    self->visible = 1;
    self->timer   = 0x5A;
    if (func_ov003_020a3330(0xE) != 0) {
        self->timer = self->timer + (s16)func_ov003_020a3468(0xE);
    }
    func_ov003_02097544(self, func_ov003_02098484);
}

s32 func_ov003_02098718(TaskPool* pool, Task* task, void* args) {
    BtlCmd* self = task->data;
    s32     i;

    MI_CpuSet(self, 0, sizeof(*self));
    self->records[0].kind = 0;
    self->records[1].kind = 1;
    self->records[2].kind = 2;
    self->current         = &self->records[0];
    self->otherA          = &self->records[1];
    self->otherB          = &self->records[2];
    self->cursor          = 0;
    self->cellWidth       = -0x10;
    self->x               = 0x80000;
    self->y               = 0xA3000;
    self->targetX         = self->x;
    self->cursorX         = self->x;
    self->targetY         = self->y;
    self->cursorY         = self->y;
    for (i = 0; i < 3; i++) {
        func_ov003_0209755c(&self->records[i].cell, 0, 0);
        func_ov003_02097604(&self->records[i].marker);
    }
    func_ov003_0209755c(&self->frame, 3, 0);
    func_ov003_02082730(&self->frame, 4);
    func_ov003_0209755c(&self->firstMarker, 8, 0);
    func_ov003_02082730(&self->firstMarker, 3);
    func_ov003_0209755c(&self->secondMarker, 11, 0);
    func_ov003_02082730(&self->secondMarker, 3);
    func_ov003_0209755c(&self->burst, 14, 1);
    func_ov003_02082730(&self->burst, 2);
    self->visible  = 0;
    self->bursting = 0;
    self->mode     = 0;
    self->wanted   = 0;
    self->input    = 0;
    self->counter  = 0;
    data_ov003_020e71b8->unk3D878 &= ~0x10;
    func_ov003_02086b14((Ov003ChargeState*)self);
    func_ov003_020977e0(self);
    func_ov003_02097544(self, func_ov003_0209842c);
    return 1;
}

s32 func_ov003_020988b4(TaskPool* pool, Task* task, void* args) {
    BtlCmd* self = task->data;

    if (data_ov003_020e71b8->unk3D878 & 0x40) {
        return 1;
    }
    if (data_ov003_020e71b8->unk3D874 != 2) {
        self->input = 0;
    } else {
        func_ov003_02097b70(self);
    }
    if (data_ov003_020e71b8->unk3D8E0 != 0) {
        data_ov003_020e71b8->unk3D8E0 = 0;
        self->timer                   = 0;
        self->visible                 = 0;
        func_ov003_0208697c();
        func_ov003_020868c8(8);
        func_ov003_02097544(self, func_ov003_0209842c);
    }
    if (self->state != NULL) {
        self->state(self);
    }
    CombatSprite_Update(&self->frame);
    func_ov003_02097e8c(self, self->current);
    if (self->current->first != -1) {
        CombatSprite_Update(&self->firstMarker);
        func_ov003_02097e8c(self, self->otherA);
    }
    if (self->current->second != -1) {
        CombatSprite_Update(&self->secondMarker);
        func_ov003_02097e8c(self, self->otherB);
    }
    if (self->bursting != 0) {
        if (SpriteMgr_IsAnimationFinished(&self->burst.sprite) != 0) {
            self->bursting = 0;
        }
        CombatSprite_Update(&self->burst);
    }
    return 1;
}

s32 func_ov003_020989e8(TaskPool* pool, Task* task, void* args) {
    BtlCmd* self = task->data;
    s32     px;
    s32     py;
    s32     width;
    s16     yy;
    s16     xx;

    if (self->bursting != 0) {
        CombatSprite_SetPosition(&self->burst, (self->x * 0x10) >> 16, (self->burstY * 0x10) >> 16);
        CombatSprite_Render(&self->burst);
    }
    if (self->visible == 0) {
        return 1;
    }
    CombatSprite_SetPosition(&self->frame, (self->x * 0x10) >> 16, (self->y * 0x10) >> 16);
    CombatSprite_Render(&self->frame);
    px = (self->cursorX * 0x10) >> 16;
    py = (self->cursorY * 0x10) >> 16;
    func_ov003_02097e90(self, self->current, px, py);
    if (self->current->first != -1) {
        width = self->cellWidth;
        yy    = py - 0x12;
        xx    = (s16)(self->current->first * width + px);
        func_ov003_02097e90(self, self->otherA, (s16)(width + xx), yy);
        CombatSprite_SetPosition(&self->firstMarker, xx, yy);
        CombatSprite_Render(&self->firstMarker);
    }
    if (self->current->second != -1) {
        width = self->cellWidth;
        yy    = py + 0x12;
        xx    = (s16)(self->current->second * width + px);
        func_ov003_02097e90(self, self->otherB, (s16)(width + xx), yy);
        CombatSprite_SetPosition(&self->secondMarker, xx, yy);
        CombatSprite_Render(&self->secondMarker);
    }
    return 1;
}

s32 func_ov003_02098b60(TaskPool* pool, Task* task, void* args) {
    BtlCmd* self = task->data;
    s32     i;

    for (i = 0; i < 3; i++) {
        CombatSprite_Release(&self->records[i].cell);
        CombatSprite_Release(&self->records[i].marker);
    }
    CombatSprite_Release(&self->frame);
    CombatSprite_Release(&self->firstMarker);
    CombatSprite_Release(&self->secondMarker);
    CombatSprite_Release(&self->burst);
    return 1;
}

s32 func_ov003_02098bc0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d7ef8;

    if (func_ov003_0208b690((u16)stage)) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
