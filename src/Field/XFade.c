#include "Display.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/EasyTask.h"
#include "Interface/Menu/MenuCommon.h"
#include "SpriteMgr.h"

typedef struct {
    s32 left;
    s32 top;
    s32 right;
    s32 bottom;
} FieldRect;

typedef struct {
    u32 engine : 2;
    u32 unk_02 : 30;
} XFadeSelector;

typedef struct {
    XFadeSelector* selector;
} XFadeSelectorArg;

extern s32 func_ov030_020d3eb0(TaskPool* pool, Task* task, void* args);
extern s32 func_ov030_020d4078(TaskPool* pool, Task* task, void* args);
extern s32 func_ov030_020d4728(TaskPool* pool, Task* task, void* args);
extern s32 func_ov030_020d4760(TaskPool* pool, Task* task, void* args);
s32        XFade_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_XFade              = {"Tsk_XFade", XFade_RunTask, 0x20C8};
const s32        data_ov030_020ee9b4[4] = {1, 2, 4, 8};
const s32        data_ov030_020ee9c4[4] = {0x200, 0x200, -0x200, -0x200};

void func_ov030_020d317c(s32 engine, s32 layer) {
    s32                     mask    = data_ov030_020ee9b4[layer];
    DisplayControlSettings* control = &g_DisplaySettings.controls[engine];

    if (mask >= 0) {
        control->layers |= mask;
    } else {
        control->layers = 0x1F;
    }
}

void func_ov030_020d31b0(s32 engine, s32 layer) {
    s32                     mask    = data_ov030_020ee9b4[layer];
    DisplayControlSettings* control = &g_DisplaySettings.controls[engine];

    if (mask >= 0) {
        control->layers &= ~mask;
    } else {
        control->layers = 0;
    }
}

void func_ov030_020d31f0(s32 engine, s32 layer) {
    g_DisplaySettings.engineState[engine].blendLayer0 = data_ov030_020ee9b4[layer];
    g_DisplaySettings.engineState[engine].blendLayer1 = 0x3F;
}

Point func_ov030_020d3224(OamCellPiece* piece) {
    Point point;

    point.x = piece->attr1 & 0x1FF;
    point.y = piece->attr0 & 0xFF;
    if (point.x >= 0x100) {
        point.x -= 0x200;
    }
    if (point.y >= 0x80) {
        point.y -= 0x100;
    }
    return point;
}

void func_ov030_020d3284(FieldRect* out, OamCellPiece* piece) {
    Point         position = func_ov030_020d3224(piece);
    OamCellShape* shape    = OamMgr_GetCellShape(piece);
    FieldRect     rect;

    rect.left   = position.x;
    rect.top    = position.y;
    rect.right  = position.x + shape->width;
    rect.bottom = position.y + shape->height;
    *out        = rect;
}

s32 func_ov030_020d35cc(Unk_Bitfield attrs) {
    return attrs.unk_05 & 0x10;
}

s32 func_ov030_020d35e8(Unk_Bitfield attrs) {
    return attrs.unk_05 & 0x8;
}

u16 func_ov030_020d3604(Sprite* sprite) {
    return *((u16*)sprite->cellTable + (sprite->cellIndex * 4 + 1));
}

void func_ov030_020d3e38(XFadeSelectorArg* arg) {
    s32 engine = arg->selector->engine;

    func_ov030_020d317c(engine, 0);
    func_ov030_020d31b0(engine, 1);
    func_ov030_020d31f0(engine, 0);
}

void func_ov030_020d3e74(XFadeSelectorArg* arg) {
    s32 engine = arg->selector->engine;

    func_ov030_020d31b0(engine, 0);
    func_ov030_020d317c(engine, 1);
    func_ov030_020d31f0(engine, 1);
}

s32 XFade_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = func_ov030_020d3eb0,
        .update     = func_ov030_020d4078,
        .render     = func_ov030_020d4728,
        .cleanup    = func_ov030_020d4760,
    };
    return stages.iter[stage](pool, task, args);
}
