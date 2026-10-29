#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatEnemyShot.h"
#include "Combat/Core/CombatList.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include <nitro/fx.h>
#include <nitro/gx/gx_geometry.h>
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>

// The battle debug overlay (`Tsk_BtlDbg`): coloured rectangles that other code posts for a number of frames to
// visualise hit boxes. The task keeps a pool of 0x40 rectangles on two lists (free and shown) and draws every shown
// rectangle as an untextured quad through the 3D engine.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

/// One rectangle; `kind` picks the colour and, for 2, whether `x`/`y` are screen coordinates.
typedef struct BtlDbgRect {
    /* 0x00 */ s32            x;
    /* 0x04 */ s32            y;
    /* 0x08 */ s16            width;
    /* 0x0A */ s16            height;
    /* 0x0C */ s16            life; // frames left
    /* 0x0E */ u16            kind;
    /* 0x10 */ CombatListNode node;
} BtlDbgRect; // Size: 0x1C

typedef struct BtlDbg {
    /* 0x000 */ BtlDbgRect rects[0x40];
    /* 0x700 */ CombatList free;
    /* 0x708 */ CombatList shown;
} BtlDbg; // Size: 0x710

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern void           func_020370cc(fx16 sin, fx16 cos);
extern OamAffineParam data_02059ad4;
extern VtxXY          data_02059ab0[4];
extern TexCoordQuad   data_02059b14[];

s32 func_ov003_020a2890(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a28ec(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a28f4(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a2968(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020a2970(TaskPool* pool, Task* task, void* args, s32 stage);

extern char data_ov003_020e3e44[]; // `Tsk_BtlDbg`, pooled into the `.data` of BtlPause

const TaskHandle data_ov003_020d8864 = {data_ov003_020e3e44, func_ov003_020a2970, sizeof(BtlDbg)};

/// The stage table of the overlay: build, update, draw, release.
const TaskStages data_ov003_020d8870 = {
    func_ov003_020a2890,
    func_ov003_020a28ec,
    func_ov003_020a28f4,
    func_ov003_020a2968,
};

s32 data_ov003_020e71c0; // the task id

// The pointer tables below belong to the battle info display; they point into its `.rodata`.
extern char data_ov003_020d889c[];
extern char data_ov003_020d88c0[];
extern char data_ov003_020d8998[];
extern char data_ov003_020d8a10[];
extern char data_ov003_020d8a38[];
extern char data_ov003_020d8a60[];
extern char data_ov003_020d8a88[];
extern char data_ov003_020d8ab0[];
extern char data_ov003_020d8ad8[];
extern char data_ov003_020d8b00[];
extern char data_ov003_020d8b28[];
extern char data_ov003_020d8b50[];
extern char data_ov003_020d8b78[];
extern char data_ov003_020d8ba0[];
extern char data_ov003_020d8bc8[];
extern char data_ov003_020d8bf0[];
extern char data_ov003_020d8cc0[];
extern char data_ov003_020d8cec[];
extern char data_ov003_020d8d18[];
extern char data_ov003_020d8d44[];
extern char data_ov003_020d8d70[];
extern char data_ov003_020d8d9c[];
extern char data_ov003_020d8dc8[];
extern char data_ov003_020d8df4[];
extern char data_ov003_020d8e20[];
extern char data_ov003_020d8e4c[];
extern char data_ov003_020d8e78[];
extern char data_ov003_020d8ea4[];
extern char data_ov003_020d8ed0[];
extern char data_ov003_020d8efc[];
extern char data_ov003_020d8f34[];
extern char data_ov003_020d8f6c[];
extern char data_ov003_020d8fa4[];
extern char data_ov003_020d8fdc[];
extern char data_ov003_020d9014[];
extern char data_ov003_020d904c[];
extern char data_ov003_020d9084[];
extern char data_ov003_020d90bc[];
extern char data_ov003_020d90f4[];
extern char data_ov003_020d912c[];
extern char data_ov003_020d9164[];
extern char data_ov003_020d919c[];

char* data_ov003_020e3e64     = data_ov003_020d88c0;
char* data_ov003_020e3e68     = data_ov003_020d8998;
char* data_ov003_020e3e6c[13] = {
    data_ov003_020d8ed0, data_ov003_020d8d18, data_ov003_020d8d44, data_ov003_020d8ea4, data_ov003_020d8d70,
    data_ov003_020d8d9c, data_ov003_020d8df4, data_ov003_020d8dc8, data_ov003_020d8e78, data_ov003_020d8e4c,
    data_ov003_020d8e20, data_ov003_020d8cc0, data_ov003_020d8cec,
};
char* data_ov003_020e3ea0[13] = {
    data_ov003_020d90f4, data_ov003_020d8fa4, data_ov003_020d9164, data_ov003_020d8f34, data_ov003_020d8fdc,
    data_ov003_020d919c, data_ov003_020d9084, data_ov003_020d9014, data_ov003_020d8efc, data_ov003_020d90bc,
    data_ov003_020d904c, data_ov003_020d8f6c, data_ov003_020d912c,
};
char* data_ov003_020e3ed4[14] = {
    data_ov003_020d8a60, data_ov003_020d8a38, data_ov003_020d8ad8, data_ov003_020d8a10, data_ov003_020d8b28,
    data_ov003_020d8b78, data_ov003_020d8ab0, data_ov003_020d8b00, data_ov003_020d8bc8, data_ov003_020d8a88,
    data_ov003_020d8bf0, data_ov003_020d8b50, data_ov003_020d8ba0, data_ov003_020d889c,
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

/// Draws one rectangle: `x`/`y` is its top left corner, the colour comes from `kind`.
void func_ov003_020a2524(s32 kind, s32 x, s32 y, s32 width, s32 height) {
    OamAffineParam* affine   = &data_02059ad4;
    TexCoordQuad*   texCoord = data_02059b14;
    VtxXY*          vertices = data_02059ab0;
    s32             rotation;

    switch (kind) {
        case 1:
            REG_GFX_FIFO_VERTEX_COLOR = 0x1F;
            break;
        case 2:
            REG_GFX_FIFO_VERTEX_COLOR = 0x3DFF;
            break;
        case 3:
            REG_GFX_FIFO_VERTEX_COLOR = 0x7C00;
            break;
        case 0:
        default:
            REG_GFX_FIFO_VERTEX_COLOR = 0x7FFF;
            break;
    }
    REG_GFX_FIFO_MATRIX_PUSH      = 0;
    REG_GFX_FIFO_MATRIX_TRANSLATE = x + ((width << 12) >> 1);
    REG_GFX_FIFO_MATRIX_TRANSLATE = y + ((height << 12) >> 1);
    REG_GFX_FIFO_MATRIX_TRANSLATE = 0x3E8000;
    rotation                      = (affine->rotation >> 4) * 2;
    func_020370cc(FX_SinCosTable_[rotation], FX_SinCosTable_[rotation + 1]);
    REG_GFX_FIFO_MATRIX_SCALE    = width * affine->scaleX;
    REG_GFX_FIFO_MATRIX_SCALE    = height * affine->scaleY;
    REG_GFX_FIFO_MATRIX_SCALE    = 0x1000;
    REG_GFX_FIFO_TEXTURE_PARAM   = 0x20000000;
    REG_GFX_FIFO_TEXTURE_PALETTE = 0;
    REG_GFX_FIFO_POLYGON_ATTR    = 0xC0;
    REG_GFX_FIFO_POLYGONS_BEGIN  = 1;
    GX_TexCoord(texCoord->vtx[0].s * width, texCoord->vtx[0].t * height);
    GX_Vtx16(vertices[0].x, vertices[0].y, 0x8000);
    GX_TexCoord(texCoord->vtx[1].s * width, texCoord->vtx[1].t * height);
    GX_Vtx16(vertices[1].x, vertices[1].y, 0x8000);
    GX_TexCoord(texCoord->vtx[2].s * width, texCoord->vtx[2].t * height);
    GX_Vtx16(vertices[2].x, vertices[2].y, 0x8000);
    GX_TexCoord(texCoord->vtx[3].s * width, texCoord->vtx[3].t * height);
    GX_Vtx16(vertices[3].x, vertices[3].y, 0x8000);
    REG_GFX_FIFO_POLYGONS_END = 0;
    REG_GFX_FIFO_MATRIX_POP   = 1;
    REG_GFX_FIFO_VERTEX_COLOR = 0x7FFF;
}

/// Draws a posted rectangle, converting its world position to the screen unless its kind says it already is one.
void func_ov003_020a2794(BtlDbgRect* rect) {
    s32 x;
    s32 y;

    if (rect->kind == 2) {
        x = rect->x;
        y = rect->y;
    } else {
        x = func_ov003_020843b0(0, rect->x) << 12;
        y = func_ov003_020843ec(0, rect->y, 0) << 12;
    }
    func_ov003_020a2524(rect->kind, x, y, rect->width, rect->height);
}

/// Moves a rectangle from the free list to the shown list.
void func_ov003_020a27f0(BtlDbg* self, u16 kind, u16 life, s32 x, s32 y, s16 width, s16 height) {
    BtlDbgRect* rect = func_ov003_0208495c(&self->free);

    if (rect == NULL) {
        return;
    }
    rect->kind   = kind;
    rect->life   = life;
    rect->x      = x;
    rect->y      = y;
    rect->width  = width;
    rect->height = height;
    func_ov003_020849dc(&self->free, &rect->node);
    func_ov003_020849bc(&self->shown, &rect->node, rect);
}

/// Moves a rectangle back to the free list.
void func_ov003_020a285c(BtlDbg* self, BtlDbgRect* rect) {
    if (rect == NULL) {
        return;
    }
    func_ov003_020849dc(&self->shown, &rect->node);
    func_ov003_020849bc(&self->free, &rect->node, rect);
}

s32 func_ov003_020a2890(TaskPool* pool, Task* task, void* args) {
    BtlDbg* self = task->data;
    s32     i;

    MI_CpuSet(self, 0, sizeof(*self));
    func_ov003_020849ac(&self->free);
    func_ov003_020849ac(&self->shown);
    for (i = 0; i < 0x40; i++) {
        func_ov003_020849bc(&self->free, &self->rects[i].node, &self->rects[i]);
    }
    return 1;
}

s32 func_ov003_020a28ec(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov003_020a28f4(TaskPool* pool, Task* task, void* args) {
    BtlDbg*     self = task->data;
    BtlDbgRect* rect = func_ov003_0208495c(&self->shown);
    BtlDbgRect* next;

    while (rect != NULL) {
        func_ov003_020a2794(rect);
        if (rect->life == 0) {
            next = func_ov003_02084984(&rect->node);
            func_ov003_020a285c(self, rect);
            rect = next;
        } else {
            rect->life--;
            rect = func_ov003_02084984(&rect->node);
        }
    }
    return 1;
}

s32 func_ov003_020a2968(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov003_020a2970(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020d8870;

    return stages.iter[stage](pool, task, args);
}

/// Starts the overlay task.
void func_ov003_020a29b8(void) {
    data_ov003_020e71c0 = EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020d8864, NULL, 0, NULL, NULL);
}

/// Posts a rectangle for `life` frames.
s32 func_ov003_020a2a04(u16 kind, u16 life, s32 x, s32 y, s16 width, s16 height) {
    BtlDbg* self = EasyTask_GetTaskData(&data_ov003_020e71b8->unk_10118, data_ov003_020e71c0);

    if (func_ov003_02084d80(1) == 0) {
        return;
    }
    func_ov003_020a27f0(self, kind, life, x, y, width, height);
}
