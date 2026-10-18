#include "Field/FieldRegion.h"

#include "Engine/EasyTask.h"
#include "Engine/Math/Sqrt.h"
#include "Field/Event/EventFlags.h"
#include "Field/FieldTypes.h"
#include "Field/Tasks/Fld_ScanArea.h"
#include <nitro/reg.h>

extern void func_02026590(s32* value, s32 target, u16 frames);

typedef struct {
    /* 0x00 */ FieldPos* origin;
    /* 0x04 */ FieldPos* pos;
    /* 0x08 */ s32       texAddr; // texture VRAM address (>> 3 for the GX param)
    /* 0x0C */ u32       palAddr; // palette VRAM address (>> 3)
} FldScanAreaArgs;                // Size: 0x10

typedef struct {
    /* 0x00 */ FldScanAreaArgs base;
    /* 0x10 */ s32             scale;       // eased value
    /* 0x14 */ s32             targetScale; // ease target
    /* 0x18 */ s32             screenX;     // x delta to the target position
    /* 0x1C */ s32             screenY;     // y delta to the target position
    /* 0x20 */ u16             scaleTimer;  // ease counter
    /* 0x22 */ u16             unk_22;
    /* 0x24 */ s32             alpha;       // polygon alpha (fx32)
    /* 0x28 */ s32             unk_28;
    /* 0x2C */ u16             fadeTimer;   // alpha ease counter
    /* 0x2E */ u16             unk_2E;
    /* 0x30 */ s32             radius;
    /* 0x34 */ s32             visible;
} FldScanArea; // Size: 0x38
static s32 Fld_ScanArea_Init(TaskPool* pool, Task* task, void* args);
static s32 Fld_ScanArea_Update(TaskPool* pool, Task* task, void* args);
static s32 Fld_ScanArea_Render(TaskPool* pool, Task* task, void* args);
static s32 Fld_ScanArea_Destroy(TaskPool* pool, Task* task, void* args);
static s32 Fld_ScanArea_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Fld_ScanArea = {"Tsk_Fld_ScanArea", Fld_ScanArea_RunTask, sizeof(FldScanArea)};

static const TaskStages Fld_ScanArea_Stages = {
    {Fld_ScanArea_Init, Fld_ScanArea_Update, Fld_ScanArea_Render, Fld_ScanArea_Destroy}
};

static void Fld_ScanArea_Reset(FldScanArea* scanArea) {
    scanArea->scale       = 0;
    scanArea->targetScale = 0;
    scanArea->scaleTimer  = 0;
    scanArea->radius      = 0;
    scanArea->alpha       = 0x10000;
    scanArea->fadeTimer   = 0;
    scanArea->visible     = 1;
}

static s32 Fld_ScanArea_Init(TaskPool* pool, Task* task, void* args) {
    FldScanArea* scanArea = task->data;

    scanArea->base = *(FldScanAreaArgs*)args;
    Fld_ScanArea_Reset(scanArea);
    return 1;
}

static s32 Fld_ScanArea_Update(TaskPool* pool, Task* task, void* args) {
    FldScanArea* scanArea = task->data;

    if (scanArea->scaleTimer != 0) {
        func_02026590(&scanArea->scale, scanArea->targetScale, scanArea->scaleTimer);
        scanArea->radius     = (scanArea->scale * 0x20) / 100;
        scanArea->scaleTimer = scanArea->scaleTimer - 1;
    }
    if (scanArea->fadeTimer != 0) {
        func_02026590(&scanArea->alpha, 0, scanArea->fadeTimer);
        scanArea->fadeTimer = scanArea->fadeTimer - 1;
    }
    return 1;
}

const u32 Fld_ScanArea_QuadVertices[6] = {0xFAE1FAE1, 0x051FFAE1, 0x051F051F, 0xFAE1051F, 0x051F, 0x051F};

static void Fld_ScanArea_DrawQuad(FldScanArea* scanArea) {
    s32 texCoords[4][2] = {
        {      0,       0},
        {      0, 0x40000},
        {0x40000, 0x40000},
        {0x40000,       0},
    };
    s32 c0 = 0xFAE1FAE1;
    s32 c1 = 0x051FFAE1;
    s32 scale;
    s32 tx;
    s32 ty;

    REG_GFX_FIFO_MATRIX_PUSH      = 0;
    tx                            = scanArea->screenX + 0x28F;
    ty                            = scanArea->screenY + 0x28F;
    REG_GFX_FIFO_MATRIX_TRANSLATE = tx;
    REG_GFX_FIFO_MATRIX_TRANSLATE = ty;
    REG_GFX_FIFO_MATRIX_TRANSLATE = 0x3E8000;
    scale                         = scanArea->scale;
    REG_GFX_FIFO_MATRIX_SCALE     = scale;
    REG_GFX_FIFO_MATRIX_SCALE     = scale;
    REG_GFX_FIFO_MATRIX_SCALE     = 0x1000;
    REG_GFX_FIFO_TEXTURE_PARAM    = 0x69B00000 | ((u32)scanArea->base.texAddr >> 3);
    REG_GFX_FIFO_TEXTURE_PALETTE  = scanArea->base.palAddr >> 3;
    REG_GFX_FIFO_POLYGON_ATTR     = 0x3D000080 | ((scanArea->alpha >> 0xC) << 0x10);
    REG_GFX_FIFO_POLYGONS_BEGIN   = 1;
    REG_GFX_FIFO_VERTEX_TEXCOORD  = 0;
    REG_GFX_FIFO_VERTEX_16        = c0;
    REG_GFX_FIFO_VERTEX_16        = 0;
    REG_GFX_FIFO_VERTEX_TEXCOORD  = 0x04000000;
    REG_GFX_FIFO_VERTEX_16        = c1;
    REG_GFX_FIFO_VERTEX_16        = 0;
    REG_GFX_FIFO_VERTEX_TEXCOORD  = 0x04000400;
    REG_GFX_FIFO_VERTEX_16        = 0x10000 - c0;
    REG_GFX_FIFO_VERTEX_16        = 0;
    REG_GFX_FIFO_VERTEX_TEXCOORD  = 0x400;
    REG_GFX_FIFO_VERTEX_16        = 0x10000 - c1;
    REG_GFX_FIFO_VERTEX_16        = 0;
    REG_GFX_FIFO_POLYGONS_END     = 0;
    REG_GFX_FIFO_MATRIX_POP       = 1;
}

static s32 Fld_ScanArea_Render(TaskPool* pool, Task* task, void* args) {
    FldScanArea* scanArea = task->data;

    if (scanArea->visible == 0) {
        return 1;
    }
    if (scanArea->alpha < 0x1000) {
        return 1;
    }
    if (scanArea->base.texAddr >= 0) {
        Fld_ScanArea_DrawQuad(scanArea);
    }
    return 1;
}

static s32 Fld_ScanArea_Destroy(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_ScanArea_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = Fld_ScanArea_Stages;

    return stages.iter[stage](pool, task, args);
}

s32 Fld_ScanArea_CreateTask(TaskPool* pool, FieldPos* origin, FieldPos* pos, s32 texAddr, u32 palAddr) {
    FldScanAreaArgs args;

    args.origin  = origin;
    args.pos     = pos;
    args.palAddr = palAddr;
    args.texAddr = texAddr;
    return EasyTask_CreateTask(pool, &Tsk_Fld_ScanArea, NULL, 0, NULL, &args);
}

s32 Fld_ScanArea_SetTargetScale(Task* task, s32 target) {
    FldScanArea* scanArea = task->data;

    scanArea->targetScale = target;
    scanArea->scaleTimer  = 1;
    return 1;
}

s32 Fld_ScanArea_GetScale(Task* task) {
    FldScanArea* scanArea = task->data;

    return scanArea->scale;
}

s32 Fld_ScanArea_GetRadius(Task* task) {
    FldScanArea* scanArea = task->data;

    return scanArea->radius;
}

void Fld_ScanArea_FadeOut(Task* task) {
    FldScanArea* scanArea = task->data;

    scanArea->fadeTimer = 0x4B;
}

void Fld_ScanArea_Open(Task* task) {
    FldScanArea* scanArea = task->data;
    s32          x12;
    s32          y12;
    s32          tx;
    s32          ty;
    s32          v;
    s32          h;

    Fld_ScanArea_Reset(scanArea);
    h                 = func_ov030_020aeca4(scanArea->base.pos->y);
    scanArea->screenX = scanArea->base.pos->x - scanArea->base.origin->x;
    scanArea->screenY = (scanArea->base.pos->y - h * 0x2C) - scanArea->base.origin->y;
    x12               = scanArea->screenX >> 0xC;
    y12               = scanArea->screenY >> 0xC;
    tx                = x12 < 0x80 ? 0x100 : 0;
    ty                = y12 < 0x60 ? 0xC0 : 0;
    v                 = Sqrti((tx - x12) * (tx - x12) + (ty - y12) * (ty - y12)) * 0x64;
    v <<= 0xC;
    scanArea->targetScale = v / 0x20;
    scanArea->scaleTimer  = 0x64;
}
