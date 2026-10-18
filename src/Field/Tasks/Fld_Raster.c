#include "Field/Tasks/Fld_Raster.h"
#include "Engine/Core/HBlank.h"
#include "Engine/EasyTask.h"
#include <nitro/reg.h>

typedef struct {
    /* 0x000 */ u16 scanline[0xC0];
    /* 0x180 */ u32 hblankEntry;
    /* 0x184 */ s16 phase;
    /* 0x186 */ s16 timer;
} Fld_Raster; // Size: 0x188

typedef struct {
    /* 0x0 */ s16 wave;
    /* 0x2 */ u16 scale;
} Fld_RasterWave;

extern const Fld_RasterWave FX_SinCosTable_[];
static s32                  Fld_Raster_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_Fld_Raster = {"Tsk_Fld_Raster", Fld_Raster_RunTask, sizeof(Fld_Raster)};

static s32 Fld_Raster_Init(TaskPool* pool, Task* task, void* args) {
    Fld_Raster* raster = task->data;
    s32         i;

    for (i = 0; i < 0xC0; i++) {
        raster->scanline[i] = 0;
    }
    raster->phase = 0;
    HBlank_InstallCallbacks(HBlank_RunScanlineWrites);
    raster->hblankEntry = HBlank_AllocEntry((void*)&REG_BG3OFS_SUB, raster, 0);
    HBlank_EnableInterrupt();
    raster->timer = 0;
    return 1;
}

static s32 Fld_Raster_Update(TaskPool* pool, Task* task, void* args) {
    Fld_Raster* raster = task->data;
    s32         phase;
    u16         v;
    s32         i;

    if (raster->timer != 0) {
        raster->timer--;
        if (raster->timer != 0) {
            return 1;
        }
    }
    raster->timer = 2;
    phase         = raster->phase;
    i             = 0;
    raster->phase = (s16)(phase + 0x800);
    raster->phase = (s16)raster->phase;
    do {
        v                   = (u16)phase;
        raster->scanline[i] = (s16)((s32)(FX_SinCosTable_[(s32)v >> 4].wave * 4) >> 0xC);
        i += 1;
        phase = v + 0x800;
    } while (i < 0xC0);
    return 1;
}

static s32 Fld_Raster_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 Fld_Raster_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_Raster* raster = task->data;

    HBlank_ResetEntry(raster->hblankEntry);
    HBlank_RestoreCallbacks();
    return 1;
}

static s32 Fld_Raster_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_Raster_Init,
        .update     = Fld_Raster_Update,
        .render     = Fld_Raster_Render,
        .cleanup    = Fld_Raster_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}
