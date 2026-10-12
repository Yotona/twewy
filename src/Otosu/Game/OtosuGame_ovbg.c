#include "Otosu/OtosuGame.h"

static s32 OtosuGame_ovbg_RunTask(TaskPool* pool, Task* self, void* arg, s32 stage);

static const TaskHandle Tsk_OtosuGame_ovbg = {"Tsk_OtosuGame_ovbg", OtosuGame_ovbg_RunTask, sizeof(OtosuGame_ovbg)};

static const u16 OtosuGame_ovbg_PaletteAnim[36] = {
    0x0,  0x32, 0x10, 0x2, 0x20, 0x4, 0x30, 0x4, 0x40, 0x6, 0x50, 0x6, 0x60, 0x8, 0x70, 0x8, 0x80, 0x8,
    0x90, 0x6,  0x80, 0x8, 0x70, 0x6, 0x60, 0x4, 0x50, 0x4, 0x40, 0x3, 0x30, 0x2, 0x20, 0x2, 0x10, 0x1,
};

static const u16 OtosuGame_ovbg_AlarmPaletteAnim[36] = {
    0x0,  0xA,  0x10, 0x2, 0x20, 0x2, 0x30, 0x2, 0x40, 0x3, 0x50, 0x3, 0x60, 0x3, 0x70, 0x4, 0x80, 0x5,
    0x90, 0x28, 0x80, 0x4, 0x70, 0x3, 0x60, 0x2, 0x50, 0x2, 0x40, 0x2, 0x30, 0x2, 0x20, 0x2, 0x10, 0x1,
};

// Nonmatching: register allocation
static void OtosuGame_ovbg_BuildScreen(OtosuGame_ovbg* self) {
    OtuBoardLayout* layout = self->layout;

    s32 w     = layout->width;
    s32 h     = layout->height;
    s32 dw    = w - 10;
    s32 dh    = h - 10;
    s32 xBase = (0x28 - dw) / 2 + 10;
    s32 yBase = (0x28 - dh) / 2 + 2;
    s32 rows  = (h - 9) / 2;
    s32 cols  = (w - 9) / 2;

    for (s32 j = 0; j < rows; j++) {
        for (s32 i = 0; i < cols; i++) {
            s32 y;
            s32 x;
            u16 mask = 0x23F0;
            u16 bit  = 1;

            for (y = 5 + j * 2; y < 7 + j * 2; y++) {
                for (x = 5 + i * 2; x < 7 + i * 2; x++) {
                    if (layout->cells[y * layout->width + x][0] != 0) {
                        mask |= bit;
                    }
                    bit <<= 1;
                }
            }

            self->screen[0][xBase + (yBase + j) * 32 + i] = mask;
        }
    }
}

static s32 OtosuGame_ovbg_Init(TaskPool* pool, Task* task, void* args) {
    OtuBoardArgs*   taskArgs = args;
    OtosuGame_ovbg* self     = task->data;
    Data*           data;
    u8*             pal;
    u8*             chr;
    u8*             scr;

    self->heap   = taskArgs->heap;
    self->layout = taskArgs->layout;

    g_DisplaySettings.controls[1].layers |= 0xF;
    g_DisplaySettings.engineState[1].blendMode   = 1;
    g_DisplaySettings.engineState[1].blendLayer0 = 3;
    g_DisplaySettings.engineState[1].blendLayer1 = 0x3E;
    g_DisplaySettings.engineState[1].blendCoeff0 = 0xA;
    g_DisplaySettings.engineState[1].blendCoeff1 = 6;

    self->fileData = DatMgr_LoadRawData(taskArgs->dataType, NULL, 0, &OtuFile_OverBg);

    pal = Data_GetPackEntryData(self->fileData, 1);
    chr = Data_GetPackEntryData(self->fileData, 2);
    scr = Data_GetPackEntryData(self->fileData, 3);

    self->palettes[0] = PaletteMgr_AllocPalette(g_PaletteManagers[1], pal, 0, 0, 1);
    {
        u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

        if ((*(u8*)chr & 0xF0) == 0) {
            size -= 4;
        }
        self->chars[0] = BgResMgr_AllocChar32(g_BgResourceManagers[1], chr,
                                              g_DisplaySettings.engineState[1].bgSettings[0].charBase, 0, size);
    }
    self->screens[0] =
        BgResMgr_AllocScreen(g_BgResourceManagers[1], scr, g_DisplaySettings.engineState[1].bgSettings[0].screenBase,
                             (u32)g_DisplaySettings.engineState[1].bgSettings[0].screenSizeText);
    PaletteMgr_Flush(g_PaletteManagers[1], self->palettes[0]);

    pal = Data_GetPackEntryData(self->fileData, 4);
    chr = Data_GetPackEntryData(self->fileData, 6);
    scr = Data_GetPackEntryData(self->fileData, 7);

    self->palettes[1] = PaletteMgr_AllocPalette(
        g_PaletteManagers[1],
        OtuFrameCursor_Init(&self->paletteAnim, (s32)pal, (OtuFrameSlot*)OtosuGame_ovbg_PaletteAnim, 0x12), 0, 1, 1);
    {
        u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

        if ((*(u8*)chr & 0xF0) == 0) {
            size -= 4;
        }
        self->chars[1] = BgResMgr_AllocChar32(g_BgResourceManagers[1], chr,
                                              g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, size);
    }
    self->screens[1] =
        BgResMgr_AllocScreen(g_BgResourceManagers[1], scr, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                             (u32)g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);
    PaletteMgr_Flush(g_PaletteManagers[1], self->palettes[1]);

    pal = Data_GetPackEntryData(self->fileData, 8);
    chr = Data_GetPackEntryData(self->fileData, 9);

    MI_CpuFillU16(0x3F0, &self->screenHeader, 0x804);
    OtosuGame_ovbg_BuildScreen(self);
    self->screenHeader = 0x80400;

    self->palettes[2] = PaletteMgr_AllocPalette(g_PaletteManagers[1], pal, 0, 2, 1);
    {
        u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

        if ((*(u8*)chr & 0xF0) == 0) {
            size -= 4;
        }
        self->chars[2] = BgResMgr_AllocChar32(g_BgResourceManagers[1], chr,
                                              g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0x3F0, size);
    }
    self->screens[2] = BgResMgr_AllocScreen(g_BgResourceManagers[1], &self->screenHeader,
                                            g_DisplaySettings.engineState[1].bgSettings[2].screenBase,
                                            (u32)g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText);
    PaletteMgr_Flush(g_PaletteManagers[1], self->palettes[2]);

    pal = Data_GetPackEntryData(self->fileData, 10);
    chr = Data_GetPackEntryData(self->fileData, 14);
    scr = Data_GetPackEntryData(self->fileData, 15);

    self->palettes[3] = PaletteMgr_AllocPalette(g_PaletteManagers[1], pal, 0, 3, 1);
    {
        u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

        if ((*(u8*)chr & 0xF0) == 0) {
            size -= 4;
        }
        self->chars[3] = BgResMgr_AllocChar32(g_BgResourceManagers[1], chr,
                                              g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, size);
    }
    self->screens[3] =
        BgResMgr_AllocScreen(g_BgResourceManagers[1], scr, g_DisplaySettings.engineState[1].bgSettings[3].screenBase,
                             (u32)g_DisplaySettings.engineState[1].bgSettings[3].screenSizeText);
    PaletteMgr_Flush(g_PaletteManagers[1], self->palettes[3]);

    return 1;
}

static s32 OtosuGame_ovbg_Update(TaskPool* pool, Task* self, void* args) {
    OtosuGame_ovbg* data = (OtosuGame_ovbg*)self->data;

    PaletteMgr_SetSource(g_PaletteManagers[1], data->palettes[1], OtuFrameCursor_Step(&data->paletteAnim));
    return 1;
}

static s32 OtosuGame_ovbg_Render(TaskPool* pool, Task* self, void* args) {
    return 1;
}

static s32 OtosuGame_ovbg_Destroy(TaskPool* pool, Task* self, void* args) {
    OtosuGame_ovbg* data = self->data;

    for (s32 i = 0; i < 4; i++) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[1], data->screens[i]);
        BgResMgr_ReleaseChar(g_BgResourceManagers[1], data->chars[i]);
        PaletteMgr_ReleaseResource(g_PaletteManagers[1], data->palettes[i]);
    }

    DatMgr_ReleaseData(data->fileData);
    return 1;
}

static s32 OtosuGame_ovbg_RunTask(TaskPool* pool, Task* self, void* arg, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_ovbg_Init,
        .update     = OtosuGame_ovbg_Update,
        .render     = OtosuGame_ovbg_Render,
        .cleanup    = OtosuGame_ovbg_Destroy,
    };

    return stages.iter[stage](pool, self, arg);
}

s32 OtosuGame_ovbg_CreateTask(TaskPool* pool, s32 dataType, Heap* heap, OtuBoardLayout* layout) {
    OtuBoardArgs args;

    args.dataType = dataType;
    args.heap     = heap;
    args.layout   = layout;

    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_ovbg, NULL, 0, NULL, &args);
}

void OtosuGame_ovbg_StartPaletteAnim(OtosuGame_ovbg* data) {
    if (data->paletteAnim.slots != (OtuFrameSlot*)OtosuGame_ovbg_AlarmPaletteAnim) {
        OtuFrameCursor_Init(&data->paletteAnim, (s32)Data_GetPackEntryData(data->fileData, 5),
                            (OtuFrameSlot*)OtosuGame_ovbg_AlarmPaletteAnim, 0x12);
    }
}
