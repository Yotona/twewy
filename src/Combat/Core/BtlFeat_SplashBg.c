#include "Combat/Core/Combat.h"
#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Math/Random.h"
#include "Engine/Resources/BgResMgr.h"
#include <nitro/fx.h>
#include <nitro/mi/cpumem.h>
#include <nitro/os/cache.h>

// The splash background (`Tsk_BtlFeat_SplashBg`): a splash picture on BG layer 1 of one screen that is rebuilt at
// random flips as it scrolls by. Two 0x684 byte screen buffers are filled from a packed tile map and handed to the
// background resource manager one after the other.

// ---------------------------------------------------------------------------------------------
// Types
// ---------------------------------------------------------------------------------------------

struct BtlFeatSplashBg;

/// One step of a script: returns 2 to run the next step at once, 1 when the task is done and 0 to wait a frame.
typedef s32 (*BtlFeatSplashBgStep)(struct BtlFeatSplashBg*);

/// The task parameter.
typedef struct BtlFeatSplashBgArgs {
    /* 0x0 */ s32 screen; // 1 for the sub screen
    /* 0x4 */ u16 group;  // picks the pack and the row of the tables
    /* 0x6 */ u16 variant;
    /* 0x8 */ u16 duration;
    /* 0xA */ u16 reserved0A;
} BtlFeatSplashBgArgs; // Size: 0xC

typedef struct BtlFeatSplashBg {
    /* 0x00 */ BtlFeatSplashBgStep* steps;
    /* 0x04 */ BtlFeatSplashBgArgs  args;
    /* 0x10 */ Data*                pack;
    /* 0x14 */ BgResource*          characters;
    /* 0x18 */ BgResource*          screen;
    /* 0x1C */ u8                   flips;  // bit 1 mirrors horizontally, bit 0 vertically
    /* 0x1D */ u8                   reserved1D;
    /* 0x1E */ u16                  buffer; // the screen buffer that is rebuilt next
    /* 0x20 */ void*                buffers[2];
    /* 0x28 */ s32                  scroll;
    /* 0x2C */ u16                  tile;
    /* 0x2E */ u16                  reserved2E;
    /* 0x30 */ s32                  speed;
    /* 0x34 */ s32                  reserved34;
    /* 0x38 */ const char*          name;
    /* 0x3C */ s16                  step;
    /* 0x3E */ u16                  timer;
    /* 0x40 */ u8                   reserved40[0x5C - 0x40];
} BtlFeatSplashBg; // Size: 0x5C

// ---------------------------------------------------------------------------------------------
// Data
// ---------------------------------------------------------------------------------------------

extern s32 data_ov003_020e71e0;

extern BgResource* BgResMgr_AllocChar32(BgResMgr* mgr, void* charData, u32 charBase, u32 offset, u32 size);
extern BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* screenData, u32 screenBase, u32 screenSize);

extern BinIdentifier data_ov003_020dd7bc;
extern BinIdentifier data_ov003_020dd7c4;
extern BinIdentifier data_ov003_020dd7d4;

s32 func_ov003_020bca14(BtlFeatSplashBg* self);
s32 func_ov003_020bca5c(BtlFeatSplashBg* self);
s32 func_ov003_020bca0c(BtlFeatSplashBg* self);
s32 func_ov003_020bcd00(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020bcd50(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020bcdb4(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020bcdbc(TaskPool* pool, Task* task, void* args);
s32 func_ov003_020bcdd0(TaskPool* pool, Task* task, void* args, s32 stage);

/// The script of the task: set up and scroll.
BtlFeatSplashBgStep data_ov003_020e5104[3] = {func_ov003_020bca14, func_ov003_020bca5c, func_ov003_020bca0c};

/// The characters to load per group row; 0xFFFF for none.
u16 data_ov003_020e5122[9] = {0xFFFF, 1, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF, 0xFFFF};
u16 data_ov003_020e5110[9] = {0xFFFF, 2, 0xFFFF, 0xFFFF, 0xFFFF, 1, 0xFFFF, 0xFFFF, 0xFFFF};

char             data_ov003_020e5134[0x18] = "Tsk_BtlFeat_SplashBg";
const TaskHandle data_ov003_020dd428       = {data_ov003_020e5134, func_ov003_020bcdd0, sizeof(BtlFeatSplashBg)};

/// The stage table of the task: build, update, draw, release.
const TaskStages data_ov003_020dd440 = {
    func_ov003_020bcd00,
    func_ov003_020bcd50,
    func_ov003_020bcdb4,
    func_ov003_020bcdbc,
};

// ---------------------------------------------------------------------------------------------
// Functions
// ---------------------------------------------------------------------------------------------

void func_ov003_020bc6e8(BtlFeatSplashBg* self) {
    self->step++;
}

void func_ov003_020bc6f8(BtlFeatSplashBg* self, BtlFeatSplashBgStep* steps, const char* name) {
    self->steps = steps;
    self->name  = name;
}

/// Copies the 0x20 by 0x1A tile map `src` to `dst`, optionally mirrored.
void func_ov003_020bc704(u16* src, u16* dst, s32 flipX, s32 flipY) {
    u16 y;
    u16 x;

    for (y = 0; y < 0x1A; y++) {
        for (x = 0; x < 0x20; x++) {
            u16 tile = src[y * 0x20 + x];
            s32 row  = flipY ? 0x19 - y : y;
            s32 col  = flipX ? 0x1F - x : x;
            u16 dstIndex;

            if (flipX) {
                tile ^= 0x400;
            }
            dstIndex = col + row * 0x20;
            if (flipY) {
                tile ^= 0x800;
            }
            dst[dstIndex] = tile;
        }
    }
}

/// Rebuilds screen buffer `buffer` from the tile map `index` of the pack and shows it.
void func_ov003_020bc7a0(BtlFeatSplashBg* self, s32 index, s32 flipX, s32 flipY, u16 buffer) {
    s32 screen          = self->args.screen;
    u16 half            = screen != 1;
    u16 row             = self->args.group * 3 + self->args.variant;
    s32 table[2][9][11] = {
        {
         {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
         {3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13},
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
         {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12},
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
         {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11},
         },
        {
         {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
         {14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24},
         {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
         {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
         {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
         {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23},
         {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
         {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
         {12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22},
         },
    };
    u16* map;
    u16* dst;

    if (self->screen != NULL) {
        if (screen == 1) {
            BgResMgr_ReleaseScreen(g_BgResourceManagers[1], self->screen);
        } else {
            BgResMgr_ReleaseScreen(g_BgResourceManagers[0], self->screen);
        }
    }
    map    = Data_GetPackEntryData(self->pack, table[half][row][index]);
    dst    = self->buffers[buffer];
    dst[0] = map[0];
    dst[1] = map[1];
    func_ov003_020bc704(map + 2, dst + 2, flipX, flipY);
    DC_PurgeAll();
    if (self->args.screen == 1) {
        self->screen = BgResMgr_AllocScreen(g_BgResourceManagers[1], self->buffers[buffer],
                                            g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                                            g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);
    } else {
        self->screen = BgResMgr_AllocScreen(g_BgResourceManagers[0], self->buffers[buffer],
                                            g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                            g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    }
}

/// Loads the characters of the group, when it has any.
void func_ov003_020bc928(BtlFeatSplashBg* self) {
    s32   screen = self->args.screen;
    u16   row    = self->args.group * 3 + self->args.variant;
    u16   entry  = (screen == 1) ? data_ov003_020e5122[row] : data_ov003_020e5110[row];
    void* data;

    if (entry == 0xFFFF) {
        return;
    }
    data = Data_GetPackEntryData(self->pack, entry);
    if (screen == 1) {
        self->characters = BgResMgr_AllocChar32(g_BgResourceManagers[1], data,
                                                g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x4000);
    } else {
        self->characters = BgResMgr_AllocChar32(g_BgResourceManagers[0], data,
                                                g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x4000);
    }
}

s32 func_ov003_020bca0c(BtlFeatSplashBg* self) {
    return 1;
}

s32 func_ov003_020bca14(BtlFeatSplashBg* self) {
    func_ov003_020bc7a0(self, 0, 0, 0, self->buffer);
    self->scroll = 0;
    self->tile   = 0;
    self->timer  = 0;
    func_ov003_020bc6e8(self);
    return 0;
}

s32 func_ov003_020bca5c(BtlFeatSplashBg* self) {
    u32 tile;

    self->scroll += self->speed;
    if (self->scroll >= 0xB000) {
        self->scroll -= 0xB000;
        self->flips = RNG_Next(100) & 3;
    }
    tile = (u32)(self->scroll << 4) >> 16;
    if (self->tile != tile) {
        self->tile = tile;
        func_ov003_020bc7a0(self, tile, self->flips & 2, self->flips & 1, self->buffer);
        self->buffer++;
        self->buffer &= 1;
        self->speed = FX_Divide((RNG_Next(3) + 2) << 12, 0xA000);
    }
    if (self->timer != self->args.duration) {
        self->timer++;
    }
    return 0;
}

void func_ov003_020bcb2c(BtlFeatSplashBg* self) {
    u16 packs[9] = {0xFFFF, 1, 2, 0xFFFF, 1, 2, 0xFFFF, 15, 16};
    u16 i;

    for (i = 0; i < 2; i++) {
        void* buffer = Mem_AllocHeapTail(&gMainHeap, 0x684);

        Mem_SetSequence(&gMainHeap, buffer, "SplashBg");
        self->buffers[i] = buffer;
    }
    self->speed = 0x4CD;
    {
        BinIdentifier* ids[3] = {&data_ov003_020dd7c4, &data_ov003_020dd7d4, &data_ov003_020dd7bc};
        u16            row    = self->args.group * 3 + self->args.variant;

        self->pack = DatMgr_LoadPackEntry(1, NULL, 0, ids[self->args.group], packs[row], 0);
    }
    func_ov003_020bc928(self);
    func_ov003_020bc7a0(self, 0, 0, 0, self->buffer);
}

void func_ov003_020bcc50(BtlFeatSplashBg* self) {
    u16 i;

    if (self->screen != NULL) {
        if (self->args.screen == 1) {
            BgResMgr_ReleaseScreen(g_BgResourceManagers[1], self->screen);
        } else {
            BgResMgr_ReleaseScreen(g_BgResourceManagers[0], self->screen);
        }
    }
    if (self->characters != NULL) {
        if (self->args.screen == 1) {
            BgResMgr_ReleaseChar(g_BgResourceManagers[1], self->characters);
        } else {
            BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->characters);
        }
    }
    DatMgr_ReleaseData(self->pack);
    for (i = 0; i < 2; i++) {
        Mem_Free(&gMainHeap, self->buffers[i]);
    }
}

s32 func_ov003_020bcd00(TaskPool* pool, Task* task, void* args) {
    BtlFeatSplashBg*     self  = task->data;
    BtlFeatSplashBgArgs* param = args;

    MI_CpuSet(self, 0, sizeof(*self));
    self->args = *param;
    func_ov003_020bcb2c(self);
    func_ov003_020bc6f8(self, data_ov003_020e5104, "prcset_BtlFeat_SplashBg");
    return 1;
}

s32 func_ov003_020bcd50(TaskPool* pool, Task* task, void* args) {
    BtlFeatSplashBg* self = task->data;
    s32              result;

    if (self->steps[self->step] == NULL) {
        return 0;
    }
    do {
        result = self->steps[self->step](self);
    } while (result == 2);
    if (result == 1) {
        return 0;
    }
    return data_ov003_020e71e0 != 1;
}

s32 func_ov003_020bcdb4(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 func_ov003_020bcdbc(TaskPool* pool, Task* task, void* args) {
    func_ov003_020bcc50(task->data);
    return 0;
}

s32 func_ov003_020bcdd0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020dd440;

    return stages.iter[stage](pool, task, args);
}

/// Starts the splash background on `screen`.
void func_ov003_020bce18(s32 screen, u16 group, u16 variant, u16 duration) {
    BtlFeatSplashBgArgs args;

    args.screen   = screen;
    args.group    = group;
    args.variant  = variant;
    args.duration = duration;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_34230, &data_ov003_020dd428, NULL, 0, NULL, &args);
}
