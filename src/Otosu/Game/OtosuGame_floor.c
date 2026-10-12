#include "Otosu/OtosuGame.h"

extern const BinIdentifier OtuFile_CommonBg[4];

static s32 OtosuGame_floor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_OtosuGame_floor = {"Tsk_OtosuGame_floor", OtosuGame_floor_RunTask, sizeof(OtosuGame_floor)};

static const s32 OtosuGame_floor_PaletteAnim7[4] = {
    0x30000,
    0x40010,
    0x60020,
    0x60030,
};

static const s32 OtosuGame_floor_PaletteAnim10[7] = {
    0x280050, 0x40030, 0x120000, 0xE0010, 0xA0020, 0x80030, 0x80040,
};

static const s32 OtosuGame_floor_PaletteAnim6[8] = {
    0x70000, 0x70010, 0x70020, 0x70030, 0x70040, 0x70050, 0x70060, 0x70070,
};

static const s32 OtosuGame_floor_PaletteAnim9[25] = {
    0x320000, 0x10010, 0x20020, 0x20030, 0x30040, 0x30050, 0x40060, 0x40070, 0x30080, 0x30090, 0x200A0, 0x200B0, 0x100C0,
    0x10010,  0x20020, 0x20030, 0x30040, 0x30050, 0x40060, 0x40070, 0x30080, 0x30090, 0x200A0, 0x200B0, 0x100C0,
};

static const s32 OtosuGame_floor_PaletteAnim8[8] = {
    0x60000, 0x60010, 0x60020, 0x60030, 0x60040, 0x60050, 0x60060, 0x60070,
};

static const u8 OtosuGame_floor_TileMap2[172] = {
    0x0,  0x63, 0x4,  0x73, 0x8,  0x83, 0xC,  0x93, 0x14, 0x93, 0x18, 0x93, 0x10, 0x93, 0x1C, 0xA3, 0x84, 0xA3, 0x90, 0xA3,
    0x98, 0xA3, 0x80, 0xA3, 0x8C, 0xA3, 0x94, 0xA3, 0x88, 0xA3, 0x9C, 0xA3, 0x0,  0x0,  0x4,  0x0,  0x8,  0x0,  0xC,  0x0,
    0x10, 0x0,  0x14, 0x0,  0x18, 0x0,  0x1C, 0x0,  0x80, 0x0,  0x84, 0x0,  0x88, 0x0,  0x8C, 0x0,  0x90, 0x0,  0x94, 0x0,
    0x98, 0x0,  0x9C, 0x0,  0x0,  0x1,  0x4,  0x1,  0x8,  0x1,  0xC,  0x1,  0x10, 0x1,  0x14, 0x1,  0x18, 0x1,  0x1C, 0x1,
    0x80, 0x1,  0x84, 0x1,  0x88, 0x1,  0x8C, 0x1,  0x90, 0x1,  0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21,
    0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21,
    0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21,
    0x94, 0x21, 0x98, 0x31, 0x9C, 0x31, 0x0,  0x32, 0x4,  0x32, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x94, 0x21,
    0x94, 0x21, 0x94, 0x21, 0x94, 0x21, 0x8,  0x42, 0xC,  0x42, 0x10, 0x42,
};

static const u8 OtosuGame_floor_TileMap0[172] = {
    0x0,  0x63, 0x4,  0x73, 0x8,  0x83, 0xC,  0x93, 0x14, 0x93, 0x18, 0x93, 0x10, 0x93, 0x1C, 0xA3, 0x84, 0xA3, 0x90, 0xA3,
    0x98, 0xA3, 0x80, 0xA3, 0x8C, 0xA3, 0x94, 0xA3, 0x88, 0xA3, 0x9C, 0xA3, 0x0,  0x0,  0x4,  0x0,  0x8,  0x0,  0xC,  0x0,
    0x10, 0x0,  0x14, 0x0,  0x18, 0x0,  0x1C, 0x0,  0x80, 0x0,  0x84, 0x0,  0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21,
    0x88, 0x0,  0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x8C, 0x0,  0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21,
    0x90, 0x0,  0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x84, 0x11, 0x88, 0x21, 0x88, 0x21, 0x94, 0x10, 0x98, 0x10,
    0x9C, 0x10, 0x0,  0x11, 0x4,  0x11, 0x8,  0x11, 0xC,  0x11, 0x10, 0x11, 0x14, 0x11, 0x18, 0x11, 0x1C, 0x11, 0x80, 0x11,
    0x88, 0x21, 0x8C, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21,
    0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21,
    0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21, 0x88, 0x21,
};

static const u8 OtosuGame_floor_TileMap1[172] = {
    0x0,  0x63, 0x4,  0x73, 0x8,  0x83, 0xC,  0x93, 0x14, 0x93, 0x18, 0x93, 0x10, 0x93, 0x1C, 0xA3, 0x84, 0xA3, 0x90, 0xA3,
    0x98, 0xA3, 0x80, 0xA3, 0x8C, 0xA3, 0x94, 0xA3, 0x88, 0xA3, 0x9C, 0xA3, 0x0,  0x0,  0x4,  0x0,  0x8,  0x0,  0xC,  0x0,
    0x10, 0x0,  0x14, 0x0,  0x18, 0x0,  0x1C, 0x0,  0x80, 0x0,  0x84, 0x0,  0x88, 0x0,  0x8C, 0x0,  0x80, 0x21, 0x80, 0x21,
    0x90, 0x0,  0x94, 0x0,  0x98, 0x0,  0x80, 0x21, 0x80, 0x21, 0x9C, 0x0,  0x0,  0x1,  0x4,  0x1,  0x80, 0x21, 0x80, 0x21,
    0x8,  0x1,  0xC,  0x1,  0x10, 0x1,  0x80, 0x21, 0x80, 0x21, 0x14, 0x11, 0x18, 0x11, 0x1C, 0x11, 0x14, 0x11, 0x14, 0x11,
    0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11, 0x14, 0x11,
    0x80, 0x21, 0x84, 0x21, 0x88, 0x21, 0x8C, 0x31, 0x90, 0x31, 0x94, 0x31, 0x98, 0x31, 0x9C, 0x31, 0x0,  0x32, 0x4,  0x32,
    0x8,  0x32, 0xC,  0x32, 0x10, 0x32, 0xC,  0x32, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21,
    0x80, 0x21, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21, 0x80, 0x21,
};

static u8* OtosuGame_floor_TileMaps[3] = {(u8*)&OtosuGame_floor_TileMap0, (u8*)&OtosuGame_floor_TileMap1,
                                          (u8*)&OtosuGame_floor_TileMap2};

static void OtosuGame_floor_FillTileBlock(u16 start, u16* map) {
    u16 value = start;
    s32 col;
    s32 row;

    for (row = 0; row < 4; row++) {
        for (col = 0; col < 4; col++) {
            map[row * 0x20 + col] = value;
            value++;
        }
        value += 0x1C;
    }
}

// Nonmatching: register naming and two extra stack words
static s32 OtosuGame_floor_Init(TaskPool* pool, Task* task, void* args) {
    OtuBoardArgs*    taskArgs = args;
    OtosuGame_floor* self     = task->data;
    Data*            data;
    Data*            data2;
    u8*              pal;
    u8*              chr;
    u8*              scr;
    Heap*            heap;
    s32              row;
    s32              row2;
    s32              cellsWide;
    s32              cellsHigh;
    s32              i;
    s32              j;
    s32              run;
    u8               wide;
    s32              col;

    self->loaded   = 1;
    self->dataType = taskArgs->dataType;
    self->heap     = taskArgs->heap;
    self->layout   = taskArgs->layout;

    s32 sheetTable[3]    = {2, 3, 4};
    s32 styleTable[3][3] = {
        {1, 2, 3},
        {1, 2, 3},
        {1, 2, 3}
    };
    s32 miscTable[3] = {4, 4, 4};

    heap = self->heap;

    data       = DatMgr_LoadRawData(taskArgs->dataType, NULL, 0, &OtuFile_Badge[sheetTable[self->layout->kind]]);
    self->data = data;

    row2 = miscTable[self->layout->kind];

    row  = styleTable[self->layout->kind][self->layout->variant];
    wide = self->layout->width;

    {
        chr = Data_GetPackEntryData(data, row);
        pal = Data_GetPackEntryData(data, row2);

        self->palette = PaletteMgr_AllocPalette(g_PaletteManagers[0], pal, 0, 0, 5);
        {
            u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

            if ((*(u8*)chr & 0xF0) == 0) {
                size -= 4;
            }
            self->chars = BgResMgr_AllocChar32(g_BgResourceManagers[0], chr,
                                               g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, size);
        }
        PaletteMgr_Flush(g_PaletteManagers[0], self->palette);
    }

    data2          = DatMgr_LoadRawData(taskArgs->dataType, NULL, 0, OtuFile_CommonBg);
    self->animData = data2;

    {
        pal                           = Data_GetPackEntryData(data2, 2);
        self->animPalettes[0].palette = PaletteMgr_AllocPalette(
            g_PaletteManagers[0],
            OtuFrameCursor_Init(&self->animPalettes[0].anim, (s32)pal, (OtuFrameSlot*)OtosuGame_floor_PaletteAnim6, 8), 0, 6,
            1);
        PaletteMgr_Flush(g_PaletteManagers[0], self->animPalettes[0].palette);
    }

    {
        pal                           = Data_GetPackEntryData(data2, 3);
        self->animPalettes[1].palette = PaletteMgr_AllocPalette(
            g_PaletteManagers[0],
            OtuFrameCursor_Init(&self->animPalettes[1].anim, (s32)pal, (OtuFrameSlot*)OtosuGame_floor_PaletteAnim7, 4), 0, 7,
            1);
        PaletteMgr_Flush(g_PaletteManagers[0], self->animPalettes[1].palette);
    }

    {
        pal                           = Data_GetPackEntryData(data2, 4);
        self->animPalettes[2].palette = PaletteMgr_AllocPalette(
            g_PaletteManagers[0],
            OtuFrameCursor_Init(&self->animPalettes[2].anim, (s32)pal, (OtuFrameSlot*)OtosuGame_floor_PaletteAnim8, 8), 0, 8,
            1);
        PaletteMgr_Flush(g_PaletteManagers[0], self->animPalettes[2].palette);
    }

    {
        pal                           = Data_GetPackEntryData(data2, 5);
        self->animPalettes[3].palette = PaletteMgr_AllocPalette(
            g_PaletteManagers[0],
            OtuFrameCursor_Init(&self->animPalettes[3].anim, (s32)pal, (OtuFrameSlot*)OtosuGame_floor_PaletteAnim9, 0x19), 0,
            9, 1);
        PaletteMgr_Flush(g_PaletteManagers[0], self->animPalettes[3].palette);
    }

    {
        pal                           = Data_GetPackEntryData(data2, 6);
        self->animPalettes[4].palette = PaletteMgr_AllocPalette(
            g_PaletteManagers[0],
            OtuFrameCursor_Init(&self->animPalettes[4].anim, (s32)pal, (OtuFrameSlot*)OtosuGame_floor_PaletteAnim10, 7), 0,
            0xA, 1);
        PaletteMgr_Flush(g_PaletteManagers[0], self->animPalettes[4].palette);
    }

    {
        chr = Data_GetPackEntryData(data2, 7);
        {
            u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

            if ((*(u8*)chr & 0xF0) == 0) {
                size -= 4;
            }
            self->animChars = BgResMgr_AllocChar32(g_BgResourceManagers[0], chr,
                                                   g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0x300, size);
        }
    }

    cellsWide = (wide + 7 + ((u32)((wide + 7) >> 2) >> 0x1D)) >> 3;
    cellsHigh = (self->layout->height + 7 + ((u32)((self->layout->height + 7) >> 2) >> 0x1D)) >> 3;

    self->tilePool = Mem_AllocHeapTail(heap, cellsHigh * (cellsWide << 0xB));
    self->tiles    = Mem_AllocHeapTail(heap, cellsHigh * (cellsWide * 4));
    MI_CpuSet(self->tilePool, 0, Mem_GetBlockSize(heap, self->tilePool));

    if (cellsHigh > 0) {
        i   = 0;
        run = 0;
        do {
            j = 0;
            if (cellsWide > 0) {
                do {
                    self->tiles[run + j] = self->tilePool + (j + run) * 0x800;
                    j                    = j + 1;
                } while (j < cellsWide);
            }
            i   = i + 1;
            run = run + cellsWide;
        } while (i < cellsHigh);
    }

    if (self->layout->height > 0) {
        col = 0;
        do {
            if (self->layout->width > 0) {
                j = 0;
                do {
                    u8  cellByte = self->layout->cells[col * self->layout->width + j][1];
                    u16 tileVal  = *(u16*)(OtosuGame_floor_TileMaps[self->layout->kind] + cellByte * 2);

                    OtosuGame_floor_FillTileBlock(
                        tileVal, (u16*)(self->tiles[(j / 8) * cellsWide + (col / 8)] + ((j % 8) * 4 + (col % 8) * 128) * 2));
                    j = j + 1;
                } while (j < (s32)self->layout->width);
            }
            col = col + 1;
        } while (col < (s32)self->layout->height);
    }

    func_0200d1d8(&self->bg, 0, 1, 0, self->tiles, cellsWide, cellsHigh);
    return 1;
}

static s32 OtosuGame_floor_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_floor* self = task->data;

    PaletteMgr_SetSource(g_PaletteManagers[0], self->animPalettes[0].palette,
                         (void*)OtuFrameCursor_Step(&self->animPalettes[0].anim));
    PaletteMgr_SetSource(g_PaletteManagers[0], self->animPalettes[1].palette,
                         (void*)OtuFrameCursor_Step(&self->animPalettes[1].anim));
    PaletteMgr_SetSource(g_PaletteManagers[0], self->animPalettes[2].palette,
                         (void*)OtuFrameCursor_Step(&self->animPalettes[2].anim));
    PaletteMgr_SetSource(g_PaletteManagers[0], self->animPalettes[3].palette,
                         (void*)OtuFrameCursor_Step(&self->animPalettes[3].anim));
    PaletteMgr_SetSource(g_PaletteManagers[0], self->animPalettes[4].palette,
                         (void*)OtuFrameCursor_Step(&self->animPalettes[4].anim));
    return 1;
}

static s32 OtosuGame_floor_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

static s32 OtosuGame_floor_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_floor* self = task->data;

    func_0200d954(0, 1);

    Mem_Free(self->heap, self->tiles);
    Mem_Free(self->heap, self->tilePool);

    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->chars);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->animChars);

    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->palette);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->animPalettes[0].palette);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->animPalettes[1].palette);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->animPalettes[2].palette);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->animPalettes[3].palette);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->animPalettes[4].palette);

    DatMgr_ReleaseData(self->data);
    DatMgr_ReleaseData(self->animData);
    return 1;
}

static s32 OtosuGame_floor_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_floor_Init,
        .update     = OtosuGame_floor_Update,
        .render     = OtosuGame_floor_Render,
        .cleanup    = OtosuGame_floor_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_floor_CreateTask(TaskPool* pool, s32 dataType, Heap* heap, OtuBoardLayout* layout) {
    OtuBoardArgs args;

    args.dataType = dataType;
    args.heap     = heap;
    args.layout   = layout;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_floor, NULL, 0, NULL, &args);
}

void OtosuGame_floor_SetScroll(OtosuGame_floor* self, s32 x, s32 y) {
    Display_SetBGOffset(self->bg.engineId, self->bg.bgLayer, x, y);
}

void OtosuGame_floor_Refresh(OtosuGame_floor* self, s32 event) {
    if (event != 0) {
        self->bg.flags |= 2;
    }
}
