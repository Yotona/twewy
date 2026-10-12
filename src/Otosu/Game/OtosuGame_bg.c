#include "Otosu/OtosuGame.h"

void func_0200d898(void* buf, void* src, s32 w, s32 h);
void func_0200d858(void* obj, s32 a, s32 b, s32 c);

typedef struct {
    /* 0x00 */ s32 slot;
    /* 0x04 */ s32 width;
    /* 0x08 */ s32 height;
    /* 0x0C */ s32 fileId;
    /* 0x10 */ s32 group;
    /* 0x14 */ s32 layers;
    /* 0x18 */ s32 priority;
    /* 0x1C */ s32 idx[3]; // per layout variant
    /* 0x28 */ s32 idx1;
    /* 0x2C */ s32 idx2;
    /* 0x30 */ s32 palStart;
    /* 0x34 */ s32 palCount;
} OtuResParams;

static OtuResParams data_ov039_0209a664 = {
    1, 1, 1, 4, 3, 8, 3, {5, 6, 7},
           8, 9, 11, 1
};
static OtuResParams data_ov039_0209a69c = {
    0, 2, 2, 3, 2, -1, 0, {5, 5, 5},
           6, 7, 11, 1
};
static OtuResParams data_ov039_0209a6d4 = {
    1, 2, 2, 3, 3, 8, 1, {8, 8, 8},
           9, 10, 12, 1
};
static OtuResParams data_ov039_0209a62c = {
    0, 4, 4, 4, 2, 4, 3, {10, 11, 12},
           13, 14, 12, 3
};

static OtuResParams* data_ov039_0209a618[2] = {&data_ov039_0209a69c, &data_ov039_0209a6d4};
static OtuResParams* data_ov039_0209a610[2] = {&data_ov039_0209a62c, &data_ov039_0209a664};

static OtuResParams** OtosuGame_bg_LayerParams[3] = {data_ov039_0209a618, data_ov039_0209a618, data_ov039_0209a610};

static s32 OtosuGame_bg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_OtosuGame_bg = {"Tsk_OtosuGame_bg", OtosuGame_bg_RunTask, sizeof(OtosuGame_bg)};

static s32 OtosuGame_bg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

// Nonmatching: register naming and scheduling
static void OtosuGame_bg_LoadLayer(OtosuGame_bg* self, OtuResParams* params) {
    u8* ref0;
    u8* ref1;
    u8* ref2;

    self->data[params->slot] = DatMgr_LoadRawData(self->dataType, NULL, 0, (BinIdentifier*)&OtuFile_Badge[params->fileId]);
    ref0                     = Data_GetPackEntryData(self->data[params->slot], params->idx[self->layout->variant]);
    ref1                     = Data_GetPackEntryData(self->data[params->slot], params->idx1);
    ref2                     = Data_GetPackEntryData(self->data[params->slot], params->idx2);

    self->palettes[params->slot] =
        PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[0], ref0, 0, params->palStart, params->palCount);

    {
        u32 size = (*(u32*)ref1 & ~0xFF) >> 8;

        if ((*(u8*)ref1 & 0xF0) == 0) {
            size -= 4;
        }
        self->chars[params->slot] = BgResMgr_AllocChar32(
            g_BgResourceManagers[0], ref1, g_DisplaySettings.engineState[0].bgSettings[params->group].charBase, 0, size);
    }

    PaletteMgr_Flush(g_PaletteManagers[0], self->palettes[params->slot]);

    self->buffers[params->slot] = Mem_AllocHeapTail(self->heap, params->height * (params->width * 4));

    func_0200d898(self->buffers[params->slot], ref2 + 4, params->width, params->height);

    func_0200d1d8(&self->maps[params->slot], 0, params->group, 0, self->buffers[params->slot], params->width, params->height);

    func_0200d858(&self->maps[params->slot], 1, 1, 0);

    self->widths[params->slot]  = params->width << 20;
    self->heights[params->slot] = params->height << 20;

    if (params->layers >= 0) {
        DisplayControlSettings* ctrl = &g_DisplaySettings.controls[DISPLAY_MAIN];

        if (params->layers >= 0) {
            ctrl->layers |= params->layers;
        } else {
            ctrl->layers = 0x1F;
        }
    }

    g_DisplaySettings.engineState[0].bgSettings[params->group].priority = params->priority;
}

static s32 OtosuGame_bg_Init(TaskPool* pool, Task* task, void* args) {
    OtuBoardArgs* taskArgs = args;
    OtosuGame_bg* self     = task->data;

    self->dataType = taskArgs->dataType;
    self->heap     = taskArgs->heap;
    self->layout   = taskArgs->layout;

    if (self->layout->kind != 0) {
        self->active = 1;
    } else {
        self->active = 0;
        return 1;
    }

    for (s32 i = 0; i < 2; i++) {
        OtosuGame_bg_LoadLayer(self, OtosuGame_bg_LayerParams[self->layout->kind][i]);
        self->scrollX[i] = 0;
        self->scrollY[i] = 0;
    }
    return 1;
}

static s32 OtosuGame_bg_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_bg* self = task->data;

    if (self->active == 0) {
        return 1;
    }

    switch (self->layout->kind) {
        case 1: {
            self->scrollX[1] += 410;
            if (self->scrollX[1] >= self->widths[1]) {
                self->scrollX[1] %= self->widths[1];
                self->maps[1].flags |= 2;
            }

            self->scrollY[1] += 410;
            if (self->scrollY[1] >= self->heights[1]) {
                self->scrollY[1] %= self->heights[1];
                self->maps[1].flags |= 2;
            }
        } break;
        case 2: {
            self->scrollX[0] += 819;
            if (self->scrollX[0] >= self->widths[0]) {
                self->scrollX[0] %= self->widths[0];
                self->maps[0].flags |= 2;
            }

            self->scrollY[0] += 819;
            if (self->scrollY[0] >= self->heights[0]) {
                self->scrollY[0] %= self->heights[0];
                self->maps[0].flags |= 2;
            }

            self->scrollX[1] += 410;
            if (self->scrollX[1] >= self->widths[1]) {
                self->scrollX[1] %= self->widths[1];
                self->maps[1].flags |= 2;
            }

            self->scrollY[1] += 410;
            if (self->scrollY[1] >= self->heights[1]) {
                self->scrollY[1] %= self->heights[1];
                self->maps[1].flags |= 2;
            }
        } break;
    }
    return 1;
}

static s32 OtosuGame_bg_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 OtosuGame_bg_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_bg* self = task->data;

    if (self->active == 0) {
        return 1;
    }

    func_0200d954(0, 2);
    func_0200d954(0, 3);

    for (s32 i = 0; i < 2; i++) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->chars[i]);
        PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->palettes[i]);
        Mem_Free(self->heap, self->buffers[i]);
        DatMgr_ReleaseData(self->data[i]);
    }
    return 1;
}

static s32 OtosuGame_bg_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_bg_Init,
        .update     = OtosuGame_bg_Update,
        .render     = OtosuGame_bg_Render,
        .cleanup    = OtosuGame_bg_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_bg_CreateTask(TaskPool* pool, s32 dataType, Heap* heap, OtuBoardLayout* layout) {
    OtuBoardArgs args;

    args.dataType = dataType;
    args.heap     = heap;
    args.layout   = layout;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_bg, NULL, 0, NULL, &args);
}

void OtosuGame_bg_SetScroll(OtosuGame_bg* self, s32 x, s32 y) {
    if (self->active) {
        s32 px = x + self->scrollX[0];
        s32 py = y + self->scrollY[0];
        Display_SetBGOffset(self->maps[0].engineId, self->maps[0].bgLayer, px, py);

        px = x + self->scrollX[1];
        py = y + self->scrollY[1];
        Display_SetBGOffset(self->maps[1].engineId, self->maps[1].bgLayer, px, py);
    }
}

void OtosuGame_bg_Refresh(OtosuGame_bg* self, s32 event) {
    if (event != 0) {
        s32  i;
        s32* flags = &self->maps[0].flags;
        for (i = 0; i < 2; i++) {
            *flags |= 2;
            flags += sizeof(ScreenMapEntry) / sizeof(s32);
        }
    }
}
