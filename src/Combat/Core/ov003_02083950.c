#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatCutin.h"
#include "Combat/Core/CombatList.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/EasyTask.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Player/Inventory.h"
#include "Save.h"
#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>

// The combat core: battle background selection, the camera and its projection, the collision lists, the touch gesture
// recognizer that turns stylus input into taps, slashes and holds, and the intrusive list primitives every battle
// object list is built from.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// One round collider that pushes its neighbours away (`Ov003Global` keeps three lists of them, by side).
typedef struct Ov003Collider {
    /* 0x00 */ s32            side; // which list the collider is in
    /* 0x04 */ s32            x;
    /* 0x08 */ s32            y;
    /* 0x0C */ s32            z;
    /* 0x10 */ s32            radius;
    /* 0x14 */ s32            height;
    /* 0x18 */ u16            flags;   // bit 0: switched off
    /* 0x1A */ u16            reserved1A;
    /* 0x1C */ u32            hitMask; // one bit per side that touched this collider
    /* 0x20 */ s32            pushX;
    /* 0x24 */ s32            pushY;
    /* 0x28 */ CombatListNode node;
} Ov003Collider; // Size: 0x34

/// The persistent settings array (25 limits and tunables), validated with a magic word.
typedef struct Ov003Settings {
    /* 0x00 */ s32 reserved00;
    /* 0x04 */ s32 values[25];
    /* 0x68 */ s32 unk_68;
    /* 0x6C */ s32 unk_6C;
    /* 0x70 */ s32 magic;
    /* 0x74 */ s8  current;
    /* 0x75 */ u8  reserved75[3];
} Ov003Settings; // Size: 0x78

/// The touch recognizer state at `Ov003Global + 0x3D370`.
typedef struct Ov003TouchState {
    /* 0x00 */ s32          active;
    /* 0x04 */ u16          flags; // which gestures the current mode allows
    /* 0x06 */ u16          gesture;
    /* 0x08 */ s16          startX;
    /* 0x0A */ s16          startY;
    /* 0x0C */ s16          endX;
    /* 0x0E */ s16          endY;
    /* 0x10 */ s32          distance;
    /* 0x14 */ s16          angle;
    /* 0x16 */ s16          tapTimer;
    /* 0x18 */ s16          tapCount;
    /* 0x1A */ u8           reserved1A[2];
    /* 0x1C */ s32          watching;
    /* 0x20 */ s16          watchFrames;
    /* 0x22 */ u8           reserved22[2];
    /* 0x24 */ s32          reserved24;
    /* 0x28 */ s16          sweepCenterX;
    /* 0x2A */ s16          sweepCenterY;
    /* 0x2C */ s16          sweepMinX;
    /* 0x2E */ s16          sweepMaxX;
    /* 0x30 */ s16          sweepMinY;
    /* 0x32 */ s16          sweepMaxY;
    /* 0x34 */ s16          sweepFrames;
    /* 0x36 */ s16          holdFrames;
    /* 0x38 */ s16          releaseFrames;
    /* 0x3A */ s16          x;
    /* 0x3C */ s16          y;
    /* 0x3E */ s16          velocityX;
    /* 0x40 */ s16          velocityY;
    /* 0x42 */ u8           reserved42[2];
    /* 0x44 */ s32          targetKind;
    /* 0x48 */ CombatActor* target;
    /* 0x4C */ CombatActor* held;
    /* 0x50 */ CombatActor* released;
    /* 0x54 */ s16          stillFrames;
    /* 0x56 */ s16          anchorX;
    /* 0x58 */ s16          anchorY;
    /* 0x5A */ u8           reserved5A[2];
} Ov003TouchState; // Size: 0x5C

/// The sprite descriptor template at `data_ov003_020e38f0`.
typedef struct Ov003SpriteTemplate {
    /* 0x00 */ u32   unk_00;
    /* 0x04 */ u32   unk_04;
    /* 0x08 */ void* callback;
    /* 0x0C */ u32   unk_0C;
    /* 0x10 */ u32   unk_10[4];
    /* 0x20 */ u32   unk_20;
    /* 0x24 */ u32   unk_24;
    /* 0x28 */ u32   unk_28;
} Ov003SpriteTemplate; // Size: 0x2C

#define TOUCH_STATE     ((Ov003TouchState*)((u8*)data_ov003_020e71b8 + 0x3D370))
#define SETTINGS        ((Ov003Settings*)((u8*)&gSaveData + 0x3454))
#define SETTINGS_VALUES ((s32*)((u8*)&gSaveData + 0x3458))

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern u8               data_02071cf0[];
extern const TaskHandle data_ov003_020d790c;
extern const TaskHandle data_ov003_020d7928;
extern const u8         data_ov003_020d75fc[];
extern const TaskHandle data_ov003_020d81e0;
extern const TaskHandle data_ov003_020d7b68;
extern const TaskHandle data_ov003_020d7b4c;

// Boss background task handles of other overlays.
extern const TaskHandle data_ov018_020feb70;
extern const TaskHandle data_ov018_020fec54;
extern const TaskHandle data_ov017_021416f4;
extern const TaskHandle data_ov017_02141700;
extern const TaskHandle data_ov017_02141da8;
extern const TaskHandle data_ov017_02141ed4;
extern const TaskHandle data_ov017_02141ef8;
extern const TaskHandle data_ov019_020fa764;
extern const TaskHandle data_ov019_020fa780;
extern const TaskHandle data_ov020_020f6fc0;
extern const TaskHandle data_ov020_020f701c;
extern const TaskHandle data_ov021_020f7824;
extern const TaskHandle data_ov021_020f7880;
extern const TaskHandle data_ov021_020f78b4;
extern const TaskHandle data_ov021_020f798c;
extern const TaskHandle data_ov008_020e7a68;

extern void func_ov003_0208380c(void);
extern void func_ov003_020838d8(void);
extern s32  func_0202599c(void);
extern void func_ov003_0208b2fc(s32* x, s32* y, s32 angle, s32 length);
extern void func_ov003_020a2f7c(TaskPool* pool, ...);
void        func_ov003_02084dc0(void);
void        func_ov003_02084ee4(void);
s32         func_ov003_02084d80(s32 index);
extern s32  func_ov003_020a2a04(u16 kind, u16 life, s32 x, s32 y, s16 width, s16 height);
extern void func_ov003_02086590(s32 kind, s32 x, s32 y);

void  func_ov003_02084880(CombatListNode* node, CombatListNode** head, CombatListNode** tail);
void  func_ov003_020848b4(CombatListNode* node, CombatListNode** head, CombatListNode** tail);
void  func_ov003_02084924(CombatListNode* node, CombatListNode** head, CombatListNode** tail, CombatListNode* before);
void* func_ov003_0208495c(CombatList* list);
void* func_ov003_02084970(CombatList* list);
void* func_ov003_02084984(CombatListNode* node);
void* func_ov003_02084998(CombatListNode* node);
void  func_ov003_020849ac(CombatList* list);
void  func_ov003_020849bc(CombatList* list, CombatListNode* node, void* item);
void* func_ov003_020849dc(CombatList* list, CombatListNode* node);
void  func_ov003_02084514(Ov003Collider* a, Ov003Collider* b);

CombatList data_ov003_020e71a0[3];

const u32 data_ov003_020d4f74[85][3] = {
    {0x08080808, 0x0000408E, 0x0000002D},
    {0x08080808, 0x0081008E, 0x00000028},
    {0x08080803, 0x0000808E, 0x00000000},
    {0x01010108, 0x0000408F, 0x00000096},
    {0x381E383C, 0x0006008F, 0x00000032},
    {0x08080808, 0x00004186, 0x0000002D},
    {0x08080808, 0x00804186, 0x00000028},
    {0x08080803, 0x00018186, 0x00000000},
    {0x01010108, 0x00004187, 0x00000096},
    {0x381E383C, 0x00060187, 0x0000002D},
    {0x08080808, 0x0000C08E, 0x00000023},
    {0x08080803, 0x0000008E, 0x00000046},
    {0x08010108, 0x0029008F, 0x0000FCB2},
    {0x00000000, 0x0000001A, 0x0000002D},
    {0x08080808, 0x0001C186, 0x00000023},
    {0x08080803, 0x00000186, 0x00000046},
    {0x08010108, 0x00298187, 0x0000FCB2},
    {0x00000000, 0x0000011A, 0x0000012C},
    {0x1E1E1E08, 0x0006019A, 0x0000012C},
    {0x1E1E1E08, 0x0006009A, 0x00000023},
    {0x00000003, 0x000001A2, 0x1E060046},
    {0x1810101E, 0x001005A3, 0x00000023},
    {0x00000003, 0x000003A2, 0x1E060046},
    {0x1010101E, 0x001005A5, 0x00000064},
    {0x10101008, 0x00018586, 0x00000046},
    {0x18101008, 0x0008008E, 0x00000046},
    {0x18101008, 0x0028008E, 0x00000046},
    {0x18101008, 0x0048008E, 0x00000096},
    {0x18181808, 0x00018587, 0x00000064},
    {0x18101008, 0x0000808E, 0x00000096},
    {0x18181808, 0x0000808E, 0x0000012C},
    {0x1860601E, 0x0003808F, 0x0000012C},
    {0x1820583C, 0x0001008F, 0x00000064},
    {0x10101008, 0x0000C08E, 0x000000C8},
    {0x1018181E, 0x0000808E, 0x000001F4},
    {0x1860601E, 0x0003808F, 0x00000000},
    {0x1010101E, 0x0000009A, 0x000A0096},
    {0x08080802, 0x0000144A, 0x00000014},
    {0x0404040A, 0x0000044A, 0x0000001E},
    {0x08080810, 0x0000044B, 0x000A0064},
    {0x40181828, 0x00040453, 0x00000064},
    {0x40181819, 0x00060453, 0x000A0064},
    {0x40181830, 0x00040459, 0x00000064},
    {0x40181819, 0x00060459, 0x000A012C},
    {0x40181830, 0x00040459, 0x0000012C},
    {0x40181819, 0x00060459, 0x00040032},
    {0x2018180A, 0x00048453, 0x00000032},
    {0x1818181E, 0x00048453, 0x00050026},
    {0x2020200A, 0x0003024A, 0x0005002D},
    {0x2020200A, 0x0003024A, 0x0000001E},
    {0x1818180A, 0x0000044A, 0x00080050},
    {0x1818180A, 0x0000844A, 0x00000014},
    {0x1818180A, 0x0000044A, 0x1E000078},
    {0x1414141E, 0x0000022A, 0x00000023},
    {0x20181819, 0x00000455, 0x00000023},
    {0x40181814, 0x00000455, 0x00000023},
    {0x40181814, 0x00000456, 0x00000023},
    {0x40181814, 0x00000455, 0x00000023},
    {0x40181814, 0x00E00459, 0x00000069},
    {0x40181814, 0x00E00459, 0x0000004A},
    {0x4018181E, 0x00DA0456, 0x1E030032},
    {0x1818180A, 0x00000458, 0x1E030032},
    {0x1818180A, 0x00000459, 0x1E030096},
    {0x1818180A, 0x00000459, 0x1E050037},
    {0x18181805, 0x00000458, 0x0003001E},
    {0x08080804, 0x00000458, 0x0000002D},
    {0x1414141E, 0x00000455, 0x00000041},
    {0x1414141E, 0x00000454, 0x1E00002D},
    {0x1414140F, 0x00034458, 0x00000028},
    {0x18181808, 0x00000458, 0x00050028},
    {0x08080805, 0x00000458, 0x0000002D},
    {0x4014141E, 0x00008255, 0x00000023},
    {0x4014141E, 0x00000257, 0x00000023},
    {0x4014141E, 0x00000259, 0x00000069},
    {0x4014141E, 0x00C00259, 0x00000023},
    {0x0000001E, 0x00E8040A, 0x00000041},
    {0x0000001E, 0x00F0020A, 0x0000001E},
    {0x00000008, 0x00000415, 0x00000014},
    {0x00000008, 0x00000413, 0x00000014},
    {0x00000008, 0x00000412, 0x0F0A002A},
    {0x1414140A, 0x00000418, 0x1E000014},
    {0x14141408, 0x00C0844A, 0x0F1E0064},
    {0x1010641E, 0x0000045A, 0x0000002D},
    {0x1414141E, 0x00000451, 0x00000041},
    {0x1414141E, 0x00000459, 0x000000C3},
};

Ov003SpriteTemplate data_ov003_020e38f0 = {
    0x08001284, 0, CombatSprite_GetFrameInfo, 0, {0, 0, 0, 0},
        0x00010000, 0, 0x00010000
};

// ---------------------------------------------------------------------------------------------
// Battle background
// ---------------------------------------------------------------------------------------------

/// The brand trend byte of save slot `index`.
u8 func_ov003_02083950(s32 index) {
    BrandTrend* trend = &gSaveData.brandTrends[index];

    return trend->ranking[0];
}

/// Picks the battle index (the chapter's battle table row) from the mode and the save.
void func_ov003_02083968(void) {
    gSaveData.unk_3232 = 0;
    switch (gSaveData.unk_3138) {
        case 0:
        case 1:
            gSaveData.unk_3232 = func_ov003_02083950(0);
            break;
        case 2:
            if (gSaveData.chapter < 3 && Inventory_GetOwnedCount(0x2AE) == 0) {
                gSaveData.unk_3232 = 0xD;
                break;
            }
            gSaveData.unk_3232 = func_ov003_02083950(1);
            break;
        case 3:
            gSaveData.unk_3232 = func_ov003_02083950(0);
            break;
        case 4:
            if (func_0202599c() != 0) {
                gSaveData.unk_3232 = 1;
            }
            break;
    }
}

// ---------------------------------------------------------------------------------------------
// Camera
// ---------------------------------------------------------------------------------------------

/// Places the camera of `index` over the field (`x0`,`y0`)-(`x1`,`y1`).
void func_ov003_02083a2c(s32 index, s32 x0, s32 y0, s32 x1, s32 y1) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];
    s32          width  = x1 - x0;

    camera->shakeX = 0;
    camera->unk_44 = 0;
    camera->unk_48 = 0;
    camera->unk_4C = 0;
    camera->unk_04 = x0 << 12;
    camera->unk_08 = y0 << 12;
    camera->unk_0C = width << 12;
    camera->unk_10 = (y1 - y0) << 12;
    camera->unk_20 = camera->unk_0C / 2;
    camera->unk_24 = camera->unk_10 / 2;
}

/// Sets the point the camera of `index` moves towards.
void func_ov003_02083ab0(s32 index, s32 x, s32 y, s32 z) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    camera->unk_34 = camera->unk_04 + x;
    camera->unk_38 = camera->unk_08 + y;
    camera->unk_3C = camera->shakeY + z;
}

/// Whether a box around (`x`,`y`) is outside the camera's field.
s32 func_ov003_02083af8(s32 x, s32 y, s32 marginX, s32 marginY) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[0];
    s32          limit  = camera->posX + FX_Mul(FX_SinCosTable_[(camera->rotation >> 4) * 2], x);

    if (y - marginY < limit) {
        return 1;
    }
    if (y + marginY > camera->unk_10) {
        return 1;
    }
    if (x - marginX < 0) {
        return 1;
    }
    if (x + marginX > camera->unk_0C) {
        return 1;
    }
    return 0;
}

s32 func_ov003_02083b8c(s32* value, s32 margin, s32 scale) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[0];
    s32          moved  = 0;
    s32          limit  = camera->posX + FX_Mul(FX_SinCosTable_[(camera->rotation >> 4) * 2], scale);

    if (*value - margin < limit) {
        *value = limit + margin;
        moved  = 1;
    }
    return moved;
}

s32 func_ov003_02083bfc(s32* value, s32 margin, s32 scale) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[0];
    s32          moved  = 0;

    if (func_ov003_02083b8c(value, margin, scale) != 0) {
        moved = 1;
    }
    if (*value + margin > camera->unk_10) {
        *value = camera->unk_10 - margin;
        moved  = 1;
    }
    return moved;
}

s32 func_ov003_02083c50(s32* value, s32 margin) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[0];
    s32          moved  = 0;

    if (*value - margin < 0) {
        *value = margin;
        moved  = 1;
    } else if (*value + margin > camera->unk_0C) {
        *value = camera->unk_0C - margin;
        moved  = 1;
    }
    return moved;
}

BOOL func_ov003_02083ca0(s32* x, s32* y, s32 marginX, s32 marginY) {
    BOOL moved = FALSE;

    if (func_ov003_02083c50(x, marginX) != 0) {
        moved = TRUE;
    }
    if (func_ov003_02083bfc(y, marginY, *x) != 0) {
        moved = TRUE;
    }
    return moved;
}

s32 func_ov003_02083ce4(s32* value, s32 margin) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[0];
    s32          moved  = 0;

    if (*value - margin < 0) {
        *value = margin;
        moved  = 1;
    } else if (*value + margin > camera->unk_10) {
        *value = camera->unk_10 - margin;
        moved  = 1;
    }
    return moved;
}

/// Picks the battle background for the encounter and tells the battle manager about it.
void func_ov003_02083d34(void) {
    u16          id        = gSaveData.unk_3138;
    Ov003Global* global    = data_ov003_020e71b8;
    s32          encounter = global->unk3D88E;

    if (encounter <= 0xF) {
        if (encounter < 9) {
        } else if (encounter == 9) {
            id = 0x1A;
        } else if (encounter == 0xE) {
            id = 0x18;
        } else if (encounter == 0xF) {
            id = 0x28;
        }
    } else {
        switch (encounter - 0x14) {
            case 0:
                id = 0x2A;
                break;
            case 1:
                id = 0x2A;
                break;
            case 6:
                id = 0x10;
                break;
            case 8:
                id = 0xE;
                break;
            case 9:
                id = 6;
                break;
            case 11:
                id = 0xA;
                break;
        }
    }
    if (id >= 0x29) {
        id = 0;
    }
    global->unk3D7C0[0].pathId = id;
    global->unk3D7C0[0].unk_02 = 0;
    global->unk3D7C0[1].pathId = id;
    global->unk3D7C0[1].unk_02 = 0;
    func_ov003_0208380c();
    func_ov003_020838d8();
}

/// Starts the two background tasks (main and sub screen) of the current battle.
void func_ov003_02083e50(s32 flag) {
    u16 args;

    if (flag == 0) {
        func_ov003_02083968();
    }
    args = flag;
    switch (data_ov003_020e71b8->unk3D88E) {
        case 7:
        case 8:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov018_020feb70, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov018_020fec54, NULL, 0, NULL, &args);
            return;
        case 10:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_021416f4, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141700, NULL, 0, NULL, &args);
            return;
        case 11:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_021416f4, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141da8, NULL, 0, NULL, &args);
            return;
        case 12:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_021416f4, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141da8, NULL, 0, NULL, &args);
            return;
        case 13:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141ed4, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov017_02141ef8, NULL, 0, NULL, &args);
            return;
        case 19:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov019_020fa764, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov019_020fa780, NULL, 0, NULL, &args);
            return;
        case 20:
        case 21:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov020_020f6fc0, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov020_020f701c, NULL, 0, NULL, &args);
            return;
        case 22:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov021_020f7824, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov021_020f7880, NULL, 0, NULL, &args);
            return;
        case 27:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov021_020f78b4, NULL, 0, NULL, &args);
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov021_020f798c, NULL, 0, NULL, &args);
            return;
        default:
            if (data_ov003_020e71b8->unk3D88D == 3) {
                EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7b68, NULL, 0, NULL, &args);
                EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov008_020e7a68, NULL, 0, NULL, &args);
            } else {
                EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7b68, NULL, 0, NULL, &args);
                EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d7b4c, NULL, 0, NULL, &args);
            }
            return;
    }
}

/// Starts the transition task, except in the last four battles.
void func_ov003_020842e0(void) {
    switch (data_ov003_020e71b8->unk3D88E) {
        case 0x13:
        case 0x14:
        case 0x15:
        case 0x16:
            break;
        default:
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_3C2BC, &data_ov003_020d81e0, NULL, 0, NULL, NULL);
            break;
    }
}

// ---------------------------------------------------------------------------------------------
// Projection
// ---------------------------------------------------------------------------------------------

/// Projects the world point onto the screen of engine `index`.
void func_ov003_02084348(s32 index, s16* screenX, s16* screenY, s32 x, s32 y, s32 z) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    *screenX = (x - camera->unk_28 + 0x80000 + camera->unk_04) >> 12;
    *screenY = (y + z - camera->unk_40 + 0x60000 + camera->unk_08) >> 12;
}

s32 func_ov003_020843b0(s32 index, s32 x) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    return (s16)((x - camera->unk_28 + 0x80000 + camera->unk_04) * 0x10 >> 16);
}

s32 func_ov003_020843ec(s32 index, s32 y, s32 z) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    return (s16)((y + z - camera->unk_40 + 0x60000 + camera->unk_08) * 0x10 >> 16);
}

/// Screen X to world X.
s32 func_ov003_0208442c(s32 index, s32 screenX) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    return camera->unk_28 + (screenX << 12) - 0x80000 - camera->unk_04;
}

/// Screen Y to world Y for height `z`.
s32 func_ov003_02084460(s32 index, s32 screenY, s32 z) {
    Ov003Camera* camera = &data_ov003_020e71b8->unk3D7C0[index];

    return camera->unk_40 + (screenY << 12) - 0x60000 - camera->unk_08 - z;
}

// ---------------------------------------------------------------------------------------------
// Colliders
// ---------------------------------------------------------------------------------------------

/// The collider list of `side`.
CombatList* func_ov003_02084498(s32 side) {
    switch (side) {
        case 0:
            return &data_ov003_020e71a0[0];
        case 1:
        case 2:
        case 3:
            return &data_ov003_020e71a0[1];
        case 4:
        case 5:
            return &data_ov003_020e71a0[2];
        default:
            return &data_ov003_020e71a0[1];
    }
}

/// Clears the hit mask of every collider in `list`.
void func_ov003_020844e8(CombatList* list) {
    Ov003Collider* collider = func_ov003_0208495c(list);

    if (collider == NULL) {
        return;
    }
    do {
        collider->hitMask = 0;
        collider          = func_ov003_02084984(&collider->node);
    } while (collider != NULL);
}

/// Pushes two colliders apart when they overlap.
void func_ov003_02084514(Ov003Collider* a, Ov003Collider* b) {
    s32 radii = a->radius + b->radius;
    s32 dx    = a->x - b->x;
    s32 dy    = a->y - b->y;
    s32 distance;
    s32 overlap;
    s32 dz;
    s32 angle;

    if ((dx < 0 ? -dx : dx) >= radii) {
        return;
    }
    if ((dy < 0 ? -dy : dy) >= radii) {
        return;
    }
    distance = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    overlap  = radii - distance;
    if (overlap <= 0) {
        return;
    }
    dz = a->z - b->z;
    if (!(dz < a->height && -dz < b->height)) {
        return;
    }
    a->hitMask |= 1 << b->side;
    b->hitMask |= 1 << a->side;
    angle    = FX_Atan2Idx(dy, dx);
    a->pushY = 0;
    a->pushX = 0;
    func_ov003_0208b2fc(&a->pushX, &a->pushY, angle, overlap);
    b->pushX = -a->pushX;
    b->pushY = -a->pushY;
}

void func_ov003_02084634(Ov003Collider* collider, s32 side, s32 radius, s32 height) {
    collider->radius  = radius << 12;
    collider->height  = height << 12;
    collider->side    = side;
    collider->hitMask = 0;
    collider->pushX   = 0;
    collider->pushY   = 0;
    func_ov003_020849bc(func_ov003_02084498(collider->side), &collider->node, collider);
}

void func_ov003_02084678(Ov003Collider* collider) {
    func_ov003_020849dc(func_ov003_02084498(collider->side), &collider->node);
}

/// Switches a collider on or off.
void func_ov003_02084694(Ov003Collider* collider, s32 off) {
    if (off != 0) {
        collider->flags |= 1;
    } else {
        collider->flags &= ~1;
    }
}

void func_ov003_020846ac(Ov003CutinMotion* motion, s32 x, s32 y, s32 z) {
    motion->x = x;
    motion->y = y << 1;
    motion->z = z;
}

/// Adds half of the motion's push to a position.
void func_ov003_020846c0(Ov003CutinMotion* motion, s32* x, s32* y) {
    if (motion->flags & 1) {
        return;
    }
    *x += motion->offsetX >> 1;
    *y += motion->offsetY >> 1;
}

CombatList* func_ov003_020846f0(s32 side) {
    return func_ov003_02084498(side);
}

/// Collides every collider of `listA` with every other one of `listB`.
void func_ov003_020846fc(CombatList* listA, CombatList* listB) {
    Ov003Collider* a = func_ov003_0208495c(listA);

    if (a == NULL) {
        return;
    }
    do {
        Ov003Collider* b = func_ov003_02084970(listB);

        if (b != NULL) {
            do {
                if (a == b) {
                    break;
                }
                func_ov003_02084514(a, b);
                b = func_ov003_02084998(&b->node);
            } while (b != NULL);
        }
        a = func_ov003_02084984(&a->node);
    } while (a != NULL);
}

/// The same, skipping colliders that are switched off.
void func_ov003_02084758(CombatList* listA, CombatList* listB) {
    Ov003Collider* a = func_ov003_0208495c(listA);

    if (a == NULL) {
        return;
    }
    do {
        if (a->flags & 1) {
            a = func_ov003_02084984(&a->node);
        } else {
            Ov003Collider* b = func_ov003_02084970(listB);

            if (b != NULL) {
                do {
                    if (a == b) {
                        break;
                    }
                    if (b->flags & 1) {
                        b = func_ov003_02084998(&b->node);
                    } else {
                        func_ov003_02084514(a, b);
                        b = func_ov003_02084998(&b->node);
                    }
                } while (b != NULL);
            }
            a = func_ov003_02084984(&a->node);
        }
    } while (a != NULL);
}

void func_ov003_020847f0(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_ov003_020849ac(&data_ov003_020e71a0[i]);
    }
}

/// Runs a frame of collision.
void func_ov003_0208481c(void) {
    s32 i;

    for (i = 0; i < 3; i++) {
        func_ov003_020844e8(&data_ov003_020e71a0[i]);
    }
    func_ov003_02084758(&data_ov003_020e71a0[0], &data_ov003_020e71a0[0]);
    func_ov003_02084758(&data_ov003_020e71a0[0], &data_ov003_020e71a0[1]);
    func_ov003_020846fc(&data_ov003_020e71a0[0], &data_ov003_020e71a0[2]);
    func_ov003_02084758(&data_ov003_020e71a0[1], &data_ov003_020e71a0[1]);
}

// ---------------------------------------------------------------------------------------------
// Lists
// ---------------------------------------------------------------------------------------------

/// Appends `node` at the tail.
void func_ov003_02084880(CombatListNode* node, CombatListNode** head, CombatListNode** tail) {
    if (*head == NULL) {
        *head = node;
    }
    node->prev = *tail;
    if (*tail != NULL) {
        node->prev->next = node;
    }
    node->next = NULL;
    *tail      = node;
}

/// Unlinks `node`.
void func_ov003_020848b4(CombatListNode* node, CombatListNode** head, CombatListNode** tail) {
    if (node->prev == NULL) {
        if (node->next == NULL) {
            *head = NULL;
            *tail = NULL;
            return;
        }
        node->next->prev = NULL;
        *head            = node->next;
    } else if (node->next == NULL) {
        *tail            = node->prev;
        node->prev->next = NULL;
    } else {
        node->next->prev = node->prev;
        node->prev->next = node->next;
    }
}

/// Links `node` in front of `before` (or at the tail when there is no `before`).
void func_ov003_02084924(CombatListNode* node, CombatListNode** head, CombatListNode** tail, CombatListNode* before) {
    if (before != NULL) {
        node->next   = before;
        node->prev   = before->prev;
        before->prev = node;
        if (node->prev != NULL) {
            node->prev->next = node;
        } else {
            *head = node;
        }
        return;
    }
    func_ov003_02084880(node, head, tail);
}

void* func_ov003_0208495c(CombatList* list) {
    return list->head != NULL ? list->head->item : NULL;
}

void* func_ov003_02084970(CombatList* list) {
    return list->tail != NULL ? list->tail->item : NULL;
}

void* func_ov003_02084984(CombatListNode* node) {
    return node->next != NULL ? node->next->item : NULL;
}

void* func_ov003_02084998(CombatListNode* node) {
    return node->prev != NULL ? node->prev->item : NULL;
}

void func_ov003_020849ac(CombatList* list) {
    list->head = NULL;
    list->tail = NULL;
}

void func_ov003_020849bc(CombatList* list, CombatListNode* node, void* item) {
    node->item = item;
    func_ov003_02084880(node, &list->head, &list->tail);
}

/// Unlinks `node` and returns the item that followed it.
void* func_ov003_020849dc(CombatList* list, CombatListNode* node) {
    CombatListNode* next = node->next;

    func_ov003_020848b4(node, &list->head, &list->tail);
    return next != NULL ? next->item : NULL;
}

void func_ov003_02084a08(CombatList* list, CombatListNode* before, CombatListNode* node, void* item) {
    node->item = item;
    func_ov003_02084924(node, &list->head, &list->tail, before);
}

// ---------------------------------------------------------------------------------------------
// Settings
// ---------------------------------------------------------------------------------------------

/// Clamps setting `index` into [`lo`, `hi`].
void func_ov003_02084a30(s32 index, s32 lo, s32 hi) {
    s32 value = SETTINGS_VALUES[index];

    if (value < lo) {
        SETTINGS_VALUES[index] = lo;
        return;
    }
    if (value > hi) {
        SETTINGS_VALUES[index] = hi;
    }
}

void func_ov003_02084d90(s32 index, s32 value);

/// Loads the default settings.
void func_ov003_02084a54(void) {
    func_ov003_02084d90(0, 0);
    func_ov003_02084d90(1, 0);
    func_ov003_02084d90(2, 0);
    func_ov003_02084d90(3, 0);
    func_ov003_02084d90(4, 0);
    func_ov003_02084d90(5, 0);
    func_ov003_02084d90(6, 0);
    func_ov003_02084d90(7, 0);
    func_ov003_02084d90(8, 0);
    func_ov003_02084d90(9, 0);
    func_ov003_02084d90(10, 1);
    func_ov003_02084d90(11, 1);
    func_ov003_02084d90(12, 0x1F4);
    func_ov003_02084d90(13, 0);
    func_ov003_02084d90(14, 1);
    func_ov003_02084d90(15, 1);
    func_ov003_02084d90(16, 0x200);
    func_ov003_02084d90(17, 0x100);
    func_ov003_02084d90(18, 0x10);
    func_ov003_02084d90(19, 0x80);
    func_ov003_02084d90(20, -0x28);
    func_ov003_02084d90(21, 0);
    func_ov003_02084d90(22, 0x120);
    func_ov003_02084d90(23, -0x28);
    func_ov003_02084d90(24, 0x38);
}

/// Sets the settings up on the first call.
void func_ov003_02084b88(void) {
    if (SETTINGS->magic == 0x6128248) {
        return;
    }
    MI_CpuSet(SETTINGS, 0, 0x78);
    func_ov003_02084a54();
    SETTINGS->unk_68  = 0;
    SETTINGS->unk_6C  = 0;
    SETTINGS->magic   = 0x6128248;
    SETTINGS->current = -1;
}

s32 func_ov003_02084d80(s32 index);

/// Clamps every setting into its legal range.
void func_ov003_02084bdc(void) {
    func_ov003_02084a30(0, 0, 2);
    func_ov003_02084a30(1, 0, 2);
    func_ov003_02084a30(2, 0, 1);
    func_ov003_02084a30(3, 0, 3);
    func_ov003_02084a30(4, 0, 0x41);
    func_ov003_02084a30(5, 0, 0x4FA);
    func_ov003_02084a30(6, 0, 0x28);
    func_ov003_02084a30(7, 0, 1);
    func_ov003_02084a30(8, 0, 1);
    func_ov003_02084a30(9, 0, 1);
    func_ov003_02084a30(10, 1, 0x64);
    func_ov003_02084a30(11, 1, func_ov003_02084d80(10));
    func_ov003_02084a30(12, 0, 0x3E8);
    func_ov003_02084a30(13, 0, 3);
    func_ov003_02084a30(14, 0, 3);
    func_ov003_02084a30(15, 1, 0x10);
    func_ov003_02084a30(16, 0x100, 0x800);
    func_ov003_02084a30(17, 0x10, 0x100);
    func_ov003_02084a30(18, 0, 0x80);
    func_ov003_02084a30(19, 0, 0x140);
    func_ov003_02084a30(20, -0x100, 0x100);
    func_ov003_02084a30(24, 0x10, 0x100);
    func_ov003_02084a30(21, 0, 0x80);
    func_ov003_02084a30(22, 0, 0x140);
    func_ov003_02084a30(23, -0x100, 0x100);
}

s32 func_ov003_02084d80(s32 index) {
    return SETTINGS_VALUES[index];
}

void func_ov003_02084d90(s32 index, s32 value) {
    SETTINGS_VALUES[index] = value;
}

void func_ov003_02084da0(s32 value) {
    SETTINGS->current = value;
}

s32 func_ov003_02084db0(void) {
    return SETTINGS->current;
}

// ---------------------------------------------------------------------------------------------
// Debug display
// ---------------------------------------------------------------------------------------------

/// Draws the hit circles of the player and of every enemy.
void func_ov003_02084dc0(void) {
    CombatActor* player = data_ov003_020e71b8->unk3D898;
    CombatActor* actor;
    u8*          item;
    s32          radius = player->unk_70;

    func_ov003_020a2a04(0, 0, player->position.x - (radius << 12), player->position.y - ((radius / 2) << 12), radius * 2,
                        radius);
    actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    if (actor != NULL) {
        do {
            radius = actor->unk_70;
            func_ov003_020a2a04(0, 0, actor->position.x - (radius << 12), actor->position.y - ((radius / 2) << 12), radius * 2,
                                radius);
            actor = func_ov003_02084984((CombatListNode*)((u8*)actor + 0x178));
        } while (actor != NULL);
    }
    item = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8B4);
    if (item == NULL) {
        return;
    }
    do {
        radius = ((CombatActor*)(item + 4))->unk_70;
        func_ov003_020a2a04(0, 0, ((CombatActor*)(item + 4))->position.x - (radius << 12),
                            ((CombatActor*)(item + 4))->position.y - ((radius / 2) << 12), radius * 2, radius);
        item = func_ov003_02084984((CombatListNode*)(item + 0xB8));
    } while (item != NULL);
}

/// Draws the hit boxes of the player and of every enemy.
void func_ov003_02084ee4(void) {
    CombatActor* player = data_ov003_020e71b8->unk3D898;
    CombatActor* actor;
    u8*          item;
    s32          radius = player->unk_70;
    s32          height = player->unk_72;

    func_ov003_020a2a04(3, 0, player->position.x - ((radius + 0x18) << 12),
                        player->position.z + (player->position.y - ((height + 0x12) << 12)), (radius + 0x18) * 2,
                        height + 0x30);
    actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
    if (actor != NULL) {
        do {
            radius = actor->unk_70;
            height = actor->unk_72;
            func_ov003_020a2a04(3, 0, actor->position.x - (radius << 12),
                                actor->position.z + (actor->position.y - (height << 12)), radius * 2, height + 0xC);
            actor = func_ov003_02084984((CombatListNode*)((u8*)actor + 0x178));
        } while (actor != NULL);
    }
    item = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8B4);
    if (item == NULL) {
        return;
    }
    do {
        CombatActor* sub = (CombatActor*)(item + 4);

        radius = sub->unk_74;
        height = sub->unk_76;
        func_ov003_020a2a04(3, 0, sub->position.x - (radius << 12), sub->position.z + (sub->position.y - (height << 12)),
                            radius * 2, height + 0xC);
        item = func_ov003_02084984((CombatListNode*)(item + 0xB8));
    } while (item != NULL);
}

void func_ov003_02085054(void) {
    switch (func_ov003_02084d80(1)) {
        case 0:
            return;
        case 1:
            func_ov003_02084dc0();
            break;
        case 2:
            func_ov003_02084ee4();
            break;
    }
}

void func_ov003_0208508c(void) {
    Ov003Global* global;
    CombatActor* player;
    CombatActor* partner;

    if (func_ov003_02084d80(2) == 0) {
        return;
    }
    global  = data_ov003_020e71b8;
    player  = global->unk3D898;
    partner = global->unk3D89C;
    func_ov003_020a2f7c(&global->unk_10118, &partner->unk_68, &player->unk_68, &partner->unk_66, &player->unk_66, 0,
                        &global->unk3D8D2, 0, 0);
}

// ---------------------------------------------------------------------------------------------
// Touch gestures
// ---------------------------------------------------------------------------------------------

/// Distance between the middle of an actor's box and a screen point.
s32 func_ov003_02085104(CombatActor* actor, s32 x, s32 y) {
    s32 centerY = actor->screenY + (12 - actor->unk_72) / 2;
    s32 dx      = (x << 12) - (actor->screenX << 12);
    s32 dy      = (y << 12) - (centerY << 12);

    return FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
}

/// Whether segment (x0,y0)-(x1,y1) crosses segment (ax,ay)-(bx,by).
BOOL func_ov003_02085164(s32 x0, s32 y0, s32 x1, s32 y1, s32 ax, s32 ay, s32 bx, s32 by) {
    s32 lo;
    s32 hi;
    s32 denominator;
    s32 s;
    s32 t;

    lo = ax < bx ? ax : bx;
    hi = x0 > x1 ? x0 : x1;
    if (hi < lo) {
        return FALSE;
    }
    lo = ax > bx ? ax : bx;
    hi = x0 < x1 ? x0 : x1;
    if (hi > lo) {
        return FALSE;
    }
    lo = ay < by ? ay : by;
    hi = y0 > y1 ? y0 : y1;
    if (hi < lo) {
        return FALSE;
    }
    lo = ay > by ? ay : by;
    hi = y0 < y1 ? y0 : y1;
    if (hi > lo) {
        return FALSE;
    }
    denominator = (x1 - x0) * (by - ay) - (y1 - y0) * (bx - ax);
    if (denominator == 0) {
        return FALSE;
    }
    s = (y0 - ay) * (bx - ax) - (x0 - ax) * (by - ay);
    t = (y0 - ay) * (x1 - x0) - (x0 - ax) * (y1 - y0);
    if (!((denominator > 0 && s >= 0 && s <= denominator) || (denominator < 0 && denominator <= s && s <= 0))) {
        return FALSE;
    }
    if (!((denominator > 0 && t >= 0 && t <= denominator) || (denominator < 0 && denominator <= t && t <= 0))) {
        return FALSE;
    }
    return TRUE;
}

/// A quick short touch.
BOOL func_ov003_020852b4(Ov003TouchState* touch) {
    return touch->holdFrames < 12 && touch->distance < 0x10000;
}

/// Counts taps; the fifth quick tap starts the tap effect.
BOOL func_ov003_020852d0(Ov003TouchState* touch) {
    if (!(touch->flags & 2)) {
        return FALSE;
    }
    if (touch->tapTimer > 0) {
        touch->tapCount++;
    }
    touch->tapTimer = 0x22;
    if (touch->tapCount < 5) {
        return FALSE;
    }
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_3C2BC, &data_ov003_020d790c, NULL, 0, NULL, NULL);
    return TRUE;
}

/// A short touch that travelled far: a flick.
BOOL func_ov003_02085358(Ov003TouchState* touch) {
    if (!(touch->flags & 4)) {
        return FALSE;
    }
    if (touch->holdFrames < 12 && touch->distance > 0x10000) {
        return TRUE;
    }
    return FALSE;
}

/// A long touch that stays near where it started.
BOOL func_ov003_0208538c(Ov003TouchState* touch) {
    s32 dx;
    s32 dy;

    if (!(touch->flags & 8)) {
        return FALSE;
    }
    if (touch->watching == 0) {
        return FALSE;
    }
    dx = (touch->startX - touch->x) << 12;
    dy = (touch->startY - touch->y) << 12;
    if (dx < 0) {
        dx = -dx;
    }
    if (dx > 0x4000 || (dy < 0 ? -dy : dy) > 0x4000) {
        touch->watching = 0;
        return FALSE;
    }
    touch->watchFrames++;
    return TRUE;
}

/// A fast back and forth scribble.
BOOL func_ov003_0208540c(Ov003TouchState* touch) {
    s32 speed;

    if (!(touch->flags & 0x20)) {
        return FALSE;
    }
    speed = FX_Sqrt(FX_Mul(touch->velocityX << 12, touch->velocityX << 12) +
                    FX_Mul(touch->velocityY << 12, touch->velocityY << 12));
    if (touch->sweepFrames == 0) {
        touch->sweepMaxX = touch->sweepMinX = touch->sweepCenterX = touch->x;
        touch->sweepMaxY = touch->sweepMinY = touch->sweepCenterY = touch->y;
    }
    if (speed >= 0xC000) {
        if (touch->sweepMinX > touch->x) {
            touch->sweepMinX = touch->x;
        } else if (touch->sweepMaxX < touch->x) {
            touch->sweepMaxX = touch->x;
        }
        if (touch->sweepMinY > touch->y) {
            touch->sweepMinY = touch->y;
        } else if (touch->sweepMaxY < touch->y) {
            touch->sweepMaxY = touch->y;
        }
        touch->sweepFrames++;
    } else {
        touch->sweepFrames--;
    }
    if (touch->sweepFrames <= 0) {
        touch->sweepFrames = 0;
    }
    if (touch->sweepFrames <= 8) {
        return FALSE;
    }
    touch->sweepCenterX = (touch->sweepMinX + touch->sweepMaxX) >> 1;
    touch->sweepCenterY = (touch->sweepMinY + touch->sweepMaxY) >> 1;
    touch->sweepMaxX = touch->sweepMinX = touch->x;
    touch->sweepMaxY = touch->sweepMinY = touch->y;
    touch->sweepFrames                  = 9;
    return TRUE;
}

/// Whether the current touch point is on `actor`.
BOOL func_ov003_02085564(CombatActor* actor) {
    s32 x     = data_ov003_020e71b8->unk3D3AA;
    s32 y     = data_ov003_020e71b8->unk3D3AC;
    s32 flags = actor->flags;

    if ((flags & 4) || (flags & 0x10)) {
        return FALSE;
    }
    if (actor->unk_00 == 0) {
        s32 dx = actor->screenX - x;

        if (dx < 0) {
            dx = -dx;
        }
        if (dx < actor->unk_70 + 0x18) {
            if (actor->screenY + 0x1E >= y) {
                if (actor->screenY - actor->unk_72 - 0x12 <= y) {
                    return TRUE;
                }
            }
        }
    } else {
        s32 dx = actor->screenX - x;

        if (dx < 0) {
            dx = -dx;
        }
        if (dx <= actor->unk_70) {
            if (actor->screenY + 0xC >= y) {
                if (actor->screenY - actor->unk_72 <= y) {
                    return TRUE;
                }
            }
        }
    }
    return FALSE;
}

/// Whether segment (x0,y0)-(x1,y1) touches the box of `actor`.
BOOL func_ov003_02085638(CombatActor* actor, s32 x0, s32 y0, s32 x1, s32 y1) {
    s32 left   = actor->screenX - actor->unk_70;
    s32 right  = actor->screenX + actor->unk_70;
    s32 bottom = actor->screenY;
    s32 top    = actor->screenY - actor->unk_72;
    s32 min;
    s32 max;

    if (left > x1 && left > x0) {
        return FALSE;
    }
    if (right < x1 && right < x0) {
        return FALSE;
    }
    if (top > y1 && top > y0) {
        return FALSE;
    }
    if (bottom < y1 && bottom < y0) {
        return FALSE;
    }
    min = x0 < x1 ? x0 : x1;
    if (left < min) {
        max = x0 > x1 ? x0 : x1;
        if (max < right) {
            min = y0 < y1 ? y0 : y1;
            if (top < min) {
                max = y0 > y1 ? y0 : y1;
                if (max < bottom) {
                    return TRUE;
                }
            }
        }
    }
    if (func_ov003_02085164(x0, y0, x1, y1, left, top, right, top)) {
        return TRUE;
    }
    if (func_ov003_02085164(x0, y0, x1, y1, left, bottom, right, bottom)) {
        return TRUE;
    }
    if (func_ov003_02085164(x0, y0, x1, y1, left, top, left, bottom)) {
        return TRUE;
    }
    return func_ov003_02085164(x0, y0, x1, y1, right, top, right, bottom) != 0;
}

/// Keeps `actor` as the best candidate when the stroke crosses it near its middle.
void func_ov003_020857e0(CombatActor** best, s32* bestDistance, CombatActor* actor) {
    Ov003Global* global = data_ov003_020e71b8;
    s16          x1     = global->unk3D37C;
    s16          x0     = global->unk3D378;
    s16          y0     = global->unk3D37A;
    s16          y1     = global->unk3D37E;
    s32          distance;

    if (!func_ov003_02085638(actor, x0, y0, x1, y1)) {
        return;
    }
    distance = func_ov003_02085104(actor, (s16)((x0 + x1) / 2), (s16)((y0 + y1) / 2));
    if (distance < *bestDistance) {
        *bestDistance = distance;
        *best         = actor;
    }
}

/// Keeps `actor` as the best candidate when the touch point is on it and closer than the current best.
void func_ov003_02085888(CombatActor** best, s32* bestDistance, CombatActor* actor) {
    s32 distance;

    if (!func_ov003_02085564(actor)) {
        return;
    }
    distance = func_ov003_02085104(actor, data_ov003_020e71b8->unk3D3AA, data_ov003_020e71b8->unk3D3AC);
    if (distance < *bestDistance) {
        *bestDistance = distance;
        *best         = actor;
    }
}

void func_ov003_020858e4(CombatActor** best, s32* bestDistance) {
    CombatActor* actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);

    if (actor == NULL) {
        return;
    }
    do {
        func_ov003_02085888(best, bestDistance, actor);
        actor = func_ov003_02084984((CombatListNode*)((u8*)actor + 0x178));
    } while (actor != NULL);
}

void func_ov003_02085934(CombatActor** best, s32* bestDistance) {
    u8* item = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8B4);

    if (item == NULL) {
        return;
    }
    do {
        func_ov003_02085888(best, bestDistance, (CombatActor*)(item + 4));
        item = func_ov003_02084984((CombatListNode*)(item + 0xB8));
    } while (item != NULL);
}

/// The actor under the touch point, the player first and then the closest enemy.
CombatActor* func_ov003_02085984(void) {
    CombatActor* best     = NULL;
    s32          distance = 0x7FFFFFFF;

    if (func_ov003_02085564(data_ov003_020e71b8->unk3D898)) {
        return data_ov003_020e71b8->unk3D898;
    }
    func_ov003_020858e4(&best, &distance);
    func_ov003_02085934(&best, &distance);
    return best;
}

/// The stylus came down.
void func_ov003_020859f8(Ov003TouchState* touch) {
    CombatActor* actor = func_ov003_02085984();

    if (actor != NULL) {
        switch (actor->unk_00) {
            case 0:
                touch->targetKind = 1;
                break;
            case 1:
                touch->targetKind = 2;
                break;
            case 2:
                touch->targetKind = 3;
                break;
        }
        actor->flags |= 1;
        touch->target = actor;
    } else {
        touch->targetKind = 0;
        touch->target     = NULL;
    }
    touch->held = touch->target;
}

/// The stylus came up.
void func_ov003_02085a70(Ov003TouchState* touch) {
    s32 dx;
    s32 dy;

    touch->endX     = touch->x;
    touch->endY     = touch->y;
    dx              = (touch->endX - touch->startX) << 12;
    dy              = (touch->endY - touch->startY) << 12;
    touch->distance = FX_Sqrt(FX_Mul(dx, dx) + FX_Mul(dy, dy));
    touch->angle    = FX_Atan2Idx(dy, dx);
    touch->released = touch->held;
    touch->held     = NULL;
}

/// Re-targets the held actor when the stylus slid off it.
void func_ov003_02085afc(Ov003TouchState* touch) {
    if (touch->held != NULL) {
        if (func_ov003_02085564(touch->held)) {
            return;
        }
        touch->held = func_ov003_02085984();
    } else {
        touch->held = func_ov003_02085984();
    }
}

/// Classifies the touch of this frame: 0 none, 1 down, 2/3/4 tap kinds, 5 hold, 6 drag, 7 scribble.
s32 func_ov003_02085b34(Ov003TouchState* touch) {
    if (TouchInput_WasTouchReleased()) {
        if (func_ov003_020852b4(touch)) {
            if (func_ov003_020852d0(touch)) {
                return 3;
            }
            if (touch->flags & 1) {
                EasyTask_CreateTask(&data_ov003_020e71b8->unk_3C2BC, &data_ov003_020d790c, NULL, 0, NULL, NULL);
                return 2;
            }
        }
        if (func_ov003_02085358(touch)) {
            EasyTask_CreateTask(&data_ov003_020e71b8->unk_3C2BC, &data_ov003_020d7928, NULL, 0, NULL, NULL);
            return 4;
        }
    }
    if (TouchInput_IsTouchActive()) {
        if (touch->holdFrames >= 12) {
            if (func_ov003_0208538c(touch)) {
                return 5;
            }
            if (func_ov003_0208540c(touch)) {
                return 7;
            }
            if (touch->flags & 0x10) {
                return 6;
            }
        }
        return 1;
    }
    return 0;
}

void func_ov003_02085c78(void) {
    TOUCH_STATE->gesture       = 0;
    TOUCH_STATE->active        = 0;
    TOUCH_STATE->holdFrames    = 0;
    TOUCH_STATE->releaseFrames = 0;
    TOUCH_STATE->watchFrames   = 0;
    TOUCH_STATE->targetKind    = 0;
    TOUCH_STATE->target        = NULL;
    TOUCH_STATE->held          = NULL;
    TOUCH_STATE->released      = NULL;
}

/// Whether the touch state is in gesture `gesture`.
BOOL func_ov003_02085cfc(u16 gesture) {
    if (data_ov003_020e71b8->touchInput.unk_00 != 0) {
        return data_ov003_020e71b8->unk3D376 == gesture;
    }
    return FALSE;
}

BOOL func_ov003_02085d3c(s32 kind) {
    return TOUCH_STATE->targetKind == kind;
}

/// Whether the current drag crosses `actor`.
BOOL func_ov003_02085d60(CombatActor* actor) {
    Ov003Global* global;

    if (!func_ov003_02085cfc(4)) {
        return FALSE;
    }
    global = data_ov003_020e71b8;
    return func_ov003_02085638(actor, global->unk3D378, global->unk3D37A, global->unk3D37C, global->unk3D37E);
}

/// The actor of `kind` under the stylus on a tap.
CombatActor* func_ov003_02085dc8(s32 kind) {
    CombatActor* best     = NULL;
    s32          distance = 0x7FFFFFFF;

    if (!func_ov003_02085cfc(2)) {
        return NULL;
    }
    switch (kind) {
        case 0:
            if (func_ov003_02085564(data_ov003_020e71b8->unk3D898)) {
                best = data_ov003_020e71b8->unk3D898;
            }
            break;
        case 1:
            func_ov003_020858e4(&best, &distance);
            break;
        case 2:
            func_ov003_02085934(&best, &distance);
            break;
    }
    return best;
}

/// The held actor, when it is of side `side`.
CombatActor* func_ov003_02085e78(s32 side) {
    CombatActor* held = TOUCH_STATE->held;

    if (!func_ov003_02085cfc(6)) {
        return NULL;
    }
    if (held != NULL && held->unk_00 != side) {
        held = NULL;
    }
    return held;
}

/// The actor of `kind` the drag crosses closest to its middle.
CombatActor* func_ov003_02085ec0(s32 kind) {
    s32          distance;
    CombatActor* best = NULL;

    if (!func_ov003_02085cfc(4)) {
        return NULL;
    }
    if (kind == 0) {
        Ov003Global* global = data_ov003_020e71b8;

        if (func_ov003_02085638(global->unk3D898, global->unk3D378, global->unk3D37A, global->unk3D37C, global->unk3D37E)) {
            best = data_ov003_020e71b8->unk3D898;
        }
    }
    if (kind == 1) {
        CombatActor* actor;

        best     = NULL;
        distance = 0x7FFFFFFF;
        actor    = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8A4);
        if (actor != NULL) {
            do {
                func_ov003_020857e0(&best, &distance, actor);
                actor = func_ov003_02084984((CombatListNode*)((u8*)actor + 0x178));
            } while (actor != NULL);
        }
    }
    if (kind == 2) {
        u8* item;

        best     = NULL;
        distance = 0x7FFFFFFF;
        item     = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8B4);
        if (item != NULL) {
            do {
                func_ov003_020857e0(&best, &distance, (CombatActor*)(item + 4));
                item = func_ov003_02084984((CombatListNode*)(item + 0xB8));
            } while (item != NULL);
        }
    }
    return best;
}

/// The actor of `kind` under the stylus on a flick.
CombatActor* func_ov003_02086014(s32 kind) {
    CombatActor* best     = NULL;
    s32          distance = 0x7FFFFFFF;

    if (!func_ov003_02085cfc(3)) {
        return NULL;
    }
    switch (kind) {
        case 0:
            if (func_ov003_02085564(data_ov003_020e71b8->unk3D898)) {
                best = data_ov003_020e71b8->unk3D898;
            }
            break;
        case 1:
            func_ov003_020858e4(&best, &distance);
            break;
        case 2:
            func_ov003_02085934(&best, &distance);
            break;
    }
    return best;
}

void func_ov003_020860c4(void) {
    Ov003TouchState* touch = TOUCH_STATE;

    touch->active      = 0;
    touch->flags       = 0;
    touch->targetKind  = 0;
    touch->held        = NULL;
    touch->target      = NULL;
    touch->released    = NULL;
    touch->tapTimer    = 0;
    touch->tapCount    = 0;
    touch->watching    = 1;
    touch->watchFrames = 0;
    touch->x           = 0x400;
    touch->y           = 0x400;
    touch->velocityX   = 0;
    touch->velocityY   = 0;
    touch->flags |= 0x15;
}

/// Reads the stylus and advances the gesture recognizer by one frame.
void func_ov003_02086130(void) {
    Ov003TouchState* touch = TOUCH_STATE;
    TouchCoord       coord;
    TouchCoord       delta;

    TouchInput_GetCoord(&coord);
    TouchInput_GetDelta(&delta);
    touch->x         = coord.x;
    touch->y         = coord.y;
    touch->velocityX = delta.x;
    touch->velocityY = delta.y;
    touch->tapTimer--;
    if (touch->tapTimer <= 0) {
        touch->tapCount = 0;
        touch->tapTimer = 0;
    }
    touch->active = 1;
    if (TouchInput_WasTouchPressed()) {
        touch->startX      = touch->x;
        touch->startY      = touch->y;
        touch->distance    = 0;
        touch->angle       = 0;
        touch->watching    = 1;
        touch->watchFrames = 0;
        touch->reserved24  = 0;
        touch->sweepFrames = 0;
        touch->stillFrames = 0;
        touch->anchorX     = touch->x;
        touch->anchorY     = touch->y;
        func_ov003_020859f8(touch);
    }
    if (TouchInput_WasTouchReleased()) {
        func_ov003_02085a70(touch);
    }
    if (TouchInput_IsTouchActive()) {
        s32 dx;
        s32 dy;

        touch->holdFrames = TouchInput_GetHoldDuration();
        func_ov003_02085afc(touch);
        dx = touch->anchorX - touch->x;
        if (dx < 0) {
            dx = -dx;
        }
        if (dx < 4) {
            dy = touch->anchorY - touch->y;
            if (dy < 0) {
                dy = -dy;
            }
            if (dy < 4) {
                touch->stillFrames++;
                goto classify;
            }
        }
        touch->stillFrames = 0;
        touch->anchorX     = touch->x;
        touch->anchorY     = touch->y;
    } else {
        touch->releaseFrames = TouchInput_GetHoldDuration();
    }
classify:
    touch->gesture = func_ov003_02085b34(touch);
}

/// Drops a spawn of `kind` at a random place of the field.
void func_ov003_020862a4(s32 kind) {
    Ov003Global* global = data_ov003_020e71b8;
    s32          height = global->unk3D7C0[0].unk_10;
    s32          width  = global->unk3D7C0[0].unk_0C;
    s32          y      = RNG_Next((u32)(height << 4) >> 16);
    s32          x      = RNG_Next((u32)(width << 4) >> 16);

    func_ov003_02086590(kind, x << 12, y << 12);
}

/// Spawns `count` things of the sort picked from `kind`.
void func_ov003_020862f8(s32 kind, s32 count) {
    s32 index = 0x14;
    s32 i;
    u8  id;

    switch (kind) {
        case 0:
            index = RNG_Next(2);
            break;
        case 1:
            index = RNG_Next(2) + 2;
            break;
        case 2:
            index = 4;
            break;
        case 3:
            index = RNG_Next(3) + 5;
            break;
        case 4:
            index = RNG_Next(2) + 8;
            break;
        case 5:
            index = RNG_Next(4) + 10;
            break;
        case 6:
            index = 0xE;
            break;
        case 7:
            index = 0xF;
            break;
        case 8:
            index = 0x10;
            break;
        case 9:
            index = 0x11;
            break;
        case 10:
            index = 0x12;
            break;
        case 11:
            index = 0x13;
            break;
        case 12:
            break;
        case 13:
            index = 0x15;
            break;
    }
    i = 0;
    if (count <= 0) {
        return;
    }
    id = data_ov003_020d75fc[index];
    do {
        func_ov003_020862a4(id);
        i++;
    } while (i < count);
}
