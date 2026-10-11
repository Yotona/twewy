#include "Engine/Core/HBlank.h"
#include "Engine/EasyTask.h"
#include <nitro/reg.h>

// USA 0x020c0780-0x020c08ec: the HBlank raster wobble task. Each frame the
// update fills a 0xC0-scanline buffer from the fixed-point wave table at
// data_0205e4e0 (stride 4 bytes: s16 value, u16 scale), which HBlank
// DMA writes to the display control register one scanline at a time.
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

extern const Fld_RasterWave data_0205e4e0[];
extern const char           data_ov030_021074ac[];

s32 func_ov030_020c0780(TaskPool* pool, Task* task, void* args);
s32 func_ov030_020c07e8(TaskPool* pool, Task* task, void* args);
s32 func_ov030_020c0880(TaskPool* pool, Task* task, void* args);
s32 func_ov030_020c0888(TaskPool* pool, Task* task, void* args);
s32 func_ov030_020c08a4(TaskPool* pool, Task* task, void* args, s32 stage);

const TaskHandle Tsk_Fld_Raster = {data_ov030_021074ac, func_ov030_020c08a4, 0x188};

const TaskStages data_ov030_020e6f88 = {
    {func_ov030_020c0780, func_ov030_020c07e8, func_ov030_020c0880, func_ov030_020c0888}
};

s32 func_ov030_020c0780(TaskPool* pool, Task* task, void* args) {
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

s32 func_ov030_020c07e8(TaskPool* pool, Task* task, void* args) {
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
        raster->scanline[i] = (s16)((s32)(data_0205e4e0[(s32)v >> 4].wave * 4) >> 0xC);
        i += 1;
        phase = v + 0x800;
    } while (i < 0xC0);
    return 1;
}

s32 func_ov030_020c0880(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov030_020c0888(TaskPool* pool, Task* task, void* args) {
    Fld_Raster* raster = task->data;

    HBlank_ResetEntry(raster->hblankEntry);
    HBlank_RestoreCallbacks();
    return 1;
}

s32 func_ov030_020c08a4(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = data_ov030_020e6f88;

    return stages.iter[stage](pool, task, args);
}
