#include "Engine/EasyTask.h"
#include "Engine/Math/Sqrt.h"
#include "Field/Events.h"
#include "Field/TaskHelpers.h"
#include <nitro/reg.h>

// USA 0x020b45b4-0x020b4920: the scan-area task. The task data holds two
// position pointers plus a fixed-point distance/scale pair that is eased with
// func_02026590 and pushed to the GX engine as a textured quad when the area
// is on screen (func_ov030_020b4678 emits the FIFO register sequence).

typedef struct {
    /* 0x00 */ FieldPos* origin;
    /* 0x04 */ FieldPos* pos;
    /* 0x08 */ s32       unk_08; // texture VRAM address (>> 3 for the GX param)
    /* 0x0C */ u32       unk_0C; // palette VRAM address (>> 3)
} FldScanAreaArgs;               // Size: 0x10

typedef struct {
    /* 0x00 */ FldScanAreaArgs base;
    /* 0x10 */ s32             unk_10; // eased value
    /* 0x14 */ s32             unk_14; // ease target
    /* 0x18 */ s32             unk_18; // x delta to the target position
    /* 0x1C */ s32             unk_1C; // y delta to the target position
    /* 0x20 */ u16             unk_20; // ease counter
    /* 0x22 */ u16             unk_22;
    /* 0x24 */ s32             unk_24; // scale (fx32)
    /* 0x28 */ s32             unk_28;
    /* 0x2C */ u16             unk_2C; // scale ease counter
    /* 0x2E */ u16             unk_2E;
    /* 0x30 */ s32             unk_30;
    /* 0x34 */ s32             unk_34;
} FldScanArea; // Size: 0x38

extern const char data_ov030_02106534[];

void func_ov030_020b45b4(FldScanArea* scanArea);
s32  func_ov030_020b45e0(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b4600(TaskPool* pool, Task* task, void* args);
void func_ov030_020b4678(FldScanArea* scanArea);
s32  func_ov030_020b4768(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b47a8(TaskPool* pool, Task* task, void* args);
s32  func_ov030_020b47b0(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle data_ov030_020e08c4 = {data_ov030_02106534, func_ov030_020b47b0, 0x38};

const TaskStages data_ov030_020e08d0 = {
    {func_ov030_020b45e0, func_ov030_020b4600, func_ov030_020b4768, func_ov030_020b47a8}
};

void func_ov030_020b45b4(FldScanArea* scanArea) {
    scanArea->unk_10 = 0;
    scanArea->unk_14 = 0;
    scanArea->unk_20 = 0;
    scanArea->unk_30 = 0;
    scanArea->unk_24 = 0x10000;
    scanArea->unk_2C = 0;
    scanArea->unk_34 = 1;
}

s32 func_ov030_020b45e0(TaskPool* pool, Task* task, void* args) {
    FldScanArea* scanArea = task->data;

    scanArea->base = *(FldScanAreaArgs*)args;
    func_ov030_020b45b4(scanArea);
    return 1;
}

s32 func_ov030_020b4600(TaskPool* pool, Task* task, void* args) {
    FldScanArea* scanArea = task->data;

    if (scanArea->unk_20 != 0) {
        func_02026590(&scanArea->unk_10, scanArea->unk_14, scanArea->unk_20);
        scanArea->unk_30 = (scanArea->unk_10 * 0x20) / 100;
        scanArea->unk_20 = scanArea->unk_20 - 1;
    }
    if (scanArea->unk_2C != 0) {
        func_02026590(&scanArea->unk_24, 0, scanArea->unk_2C);
        scanArea->unk_2C = scanArea->unk_2C - 1;
    }
    return 1;
}

void func_ov030_020b4678(FldScanArea* scanArea) {
    s32 c0 = 0xFAE1FAE1;
    s32 c1 = 0x051FFAE1;
    s32 scale;
    s32 tx;
    s32 ty;

    REG_GFX_FIFO_MATRIX_PUSH      = 0;
    tx                            = scanArea->unk_18 + 0x28F;
    ty                            = scanArea->unk_1C + 0x28F;
    REG_GFX_FIFO_MATRIX_TRANSLATE = tx;
    REG_GFX_FIFO_MATRIX_TRANSLATE = ty;
    REG_GFX_FIFO_MATRIX_TRANSLATE = 0x3E8000;
    scale                         = scanArea->unk_10;
    REG_GFX_FIFO_MATRIX_SCALE     = scale;
    REG_GFX_FIFO_MATRIX_SCALE     = scale;
    REG_GFX_FIFO_MATRIX_SCALE     = 0x1000;
    REG_GFX_FIFO_TEXTURE_PARAM    = 0x69B00000 | ((u32)scanArea->base.unk_08 >> 3);
    REG_GFX_FIFO_TEXTURE_PALETTE  = scanArea->base.unk_0C >> 3;
    REG_GFX_FIFO_POLYGON_ATTR     = 0x3D000080 | ((scanArea->unk_24 >> 0xC) << 0x10);
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

s32 func_ov030_020b4768(TaskPool* pool, Task* task, void* args) {
    FldScanArea* scanArea = task->data;

    if (scanArea->unk_34 == 0) {
        return 1;
    }
    if (scanArea->unk_24 < 0x1000) {
        return 1;
    }
    if (scanArea->base.unk_08 >= 0) {
        func_ov030_020b4678(scanArea);
    }
    return 1;
}

s32 func_ov030_020b47a8(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov030_020b47b0(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = data_ov030_020e08d0;

    return stages.iter[stage](pool, task, args);
}

s32 func_ov030_020b47f8(TaskPool* pool, FieldPos* origin, FieldPos* pos, s32 unk_08, u32 unk_0C) {
    FldScanAreaArgs args;

    args.origin = origin;
    args.pos    = pos;
    args.unk_0C = unk_0C;
    args.unk_08 = unk_08;
    return EasyTask_CreateTask(pool, &data_ov030_020e08c4, NULL, 0, NULL, &args);
}

s32 func_ov030_020b4838(Task* task, s32 target) {
    FldScanArea* scanArea = task->data;

    scanArea->unk_14 = target;
    scanArea->unk_20 = 1;
    return 1;
}

s32 func_ov030_020b484c(Task* task) {
    FldScanArea* scanArea = task->data;

    return scanArea->unk_10;
}

s32 func_ov030_020b4858(Task* task) {
    FldScanArea* scanArea = task->data;

    return scanArea->unk_30;
}

void func_ov030_020b4864(Task* task) {
    FldScanArea* scanArea = task->data;

    scanArea->unk_2C = 0x4B;
}

void func_ov030_020b4874(Task* task) {
    FldScanArea* scanArea = task->data;
    s32          x12;
    s32          y12;
    s32          tx;
    s32          ty;
    s32          v;
    s32          h;

    func_ov030_020b45b4(scanArea);
    h                = func_ov030_020aeca4(scanArea->base.pos->y);
    scanArea->unk_18 = scanArea->base.pos->x - scanArea->base.origin->x;
    scanArea->unk_1C = (scanArea->base.pos->y - h * 0x2C) - scanArea->base.origin->y;
    x12              = scanArea->unk_18 >> 0xC;
    y12              = scanArea->unk_1C >> 0xC;
    tx               = x12 < 0x80 ? 0x100 : 0;
    ty               = y12 < 0x60 ? 0xC0 : 0;
    v                = Sqrti((tx - x12) * (tx - x12) + (ty - y12) * (ty - y12)) * 0x64;
    v <<= 0xC;
    scanArea->unk_14 = v / 0x20;
    scanArea->unk_20 = 0x64;
}
