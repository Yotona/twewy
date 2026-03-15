#include "Engine/Core/HBlank.h"
#include "Engine/Core/System.h"
#include "common_data.h"

#include <NitroSDK/os.h>
#include <registers.h>

typedef struct {
    /* 0x00 */ s32   mode;        // 0 = u16, 1 = s32
    /* 0x04 */ s32   unk_04;
    /* 0x08 */ void* activeSrc;   // front-buffer source scanline table
    /* 0x0C */ void* pendingSrc;  // back-buffer source scanline table
    /* 0x10 */ void* activeDest;  // front-buffer destination register
    /* 0x14 */ void* pendingDest; // back-buffer destination register
} HBlankEntry;                    // Size: 0x18

typedef struct {
    /* 0x000 */ HBlankEntry entries[10];
    /* 0x0F0 */ s32         unk_F0;
    /* 0x0F4 */ s32         nullSink;      // sentinel; inactive entry dests point here
    /* 0x0F8 */ IRQCallback hblankFunc;    // active HBlank callback
    /* 0x0FC */ IRQCallback savedVBlankCb; // previously saved VBlank callback
    /* 0x100 */ IRQCallback savedHBlankCb; // previously saved HBlank callback
} HBlankManager;                           // Size: 0x104

static HBlankManager g_HBlankManager = {0};

static void HBlank_DisableInterrupt(void);

void HBlank_RunScanlineWrites(void) {
    u32 scanline = *(vu8*)0x04000006;
    scanline += 1;
    if (scanline >= 192) {
        scanline = 0;
    }

    HBlankEntry* entryPtr = &g_HBlankManager.entries[0];
    u32          i        = 10;
    while (i > 0) {
        s32   mode;
        void* src;
        void* dest;

        mode = entryPtr->mode;
        src  = entryPtr->activeSrc;
        dest = entryPtr->activeDest;
        entryPtr++;

        if (mode == 0) {
            *(u16*)dest = *(u16*)((u8*)src + scanline * 2);
        } else {
            *(u32*)dest = *(u32*)((u8*)src + scanline * 4);
        }
        i--;
    }
}

static void HBlank_VBlankDispatch(void) {
    OS_DisableInterrupts(IRQ_HBLANK);
    if (SystemStatusFlags.vblank) {
        Display_Commit();
    }
    for (s32 i = 0; i < 10; i++) {
        g_HBlankManager.entries[i].activeDest = g_HBlankManager.entries[i].pendingDest;
        g_HBlankManager.entries[i].activeSrc  = g_HBlankManager.entries[i].pendingSrc;
    }

    g_HBlankManager.hblankFunc();

    if (SystemStatusFlags.vblank) {
        DMA_Flush();
        DC_PurgeRange(&data_0206770c, 0x400);
        GX_LoadOam(&data_0206770c, 0, 0x400);
        DC_PurgeRange(&data_02068798, 0x400);
        GXs_LoadOam(&data_02068798, 0, 0x400);
        DC_PurgeRange(&data_02066aec, 0x400);
        GX_LoadBgPltt(&data_02066aec, 0, 0x200);
        GX_LoadObjPltt(&data_02066cec, 0, 0x200);
        DC_PurgeRange(&data_02066eec, 0x400);
        GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
        GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
    }

    OS_EnableInterrupts(IRQ_HBLANK);
}

void HBlank_Init(void) {
    g_HBlankManager.nullSink   = 0;
    g_HBlankManager.hblankFunc = NULL;
    for (u32 i = 0; i < 10; i++) {
        HBlank_ResetEntry(i);
    }
}

void HBlank_InstallCallbacks(IRQCallback cb) {
    g_HBlankManager.hblankFunc    = cb;
    g_HBlankManager.savedVBlankCb = Interrupts_SaveVBlankCallback(HBlank_VBlankDispatch);
    g_HBlankManager.savedHBlankCb = Interrupts_SaveHBlankCallback(g_HBlankManager.hblankFunc);
}

void HBlank_RestoreCallbacks(void) {
    HBlank_DisableInterrupt();
    Interrupts_SaveVBlankCallback(g_HBlankManager.savedVBlankCb);
    Interrupts_SaveHBlankCallback(g_HBlankManager.savedHBlankCb);
}

void HBlank_EnableInterrupt(void) {
    OS_EnableInterrupts(IRQ_HBLANK);
    GX_HBlankIntr(1);
}

static void HBlank_DisableInterrupt(void) {
    OS_DisableInterrupts(IRQ_HBLANK);
    GX_HBlankIntr(0);
}

static void HBlank_SetPendingEntry(u32 idx, void* dest, void* src, s32 mode) {
    if (idx >= 10) {
        return;
    }
    HBlankEntry* entry = &g_HBlankManager.entries[idx];
    entry->pendingDest = dest;
    entry->pendingSrc  = src;
    entry->mode        = mode;
    if ((Interrupts_IsVBlankInterrupted() == FALSE) || (Interrupts_IsHBlankInterrupted() == FALSE)) {
        entry->activeDest = entry->pendingDest;
        entry->activeSrc  = entry->pendingSrc;
    }
}

u32 HBlank_AllocEntry(void* dest, void* src, s32 mode) {
    u32 idx = -1;

    HBlankManager* manager    = &g_HBlankManager;
    void*          activeDest = manager->entries[0].activeDest;

    for (u32 i = 0; i < 10; i++) {
        if ((activeDest == &g_HBlankManager.nullSink) && (manager->entries[0].pendingDest == &g_HBlankManager.nullSink)) {
            idx = i;
            break;
        }
    }

    HBlank_SetPendingEntry(idx, dest, src, mode);
    return idx;
}

void HBlank_ResetEntry(u32 idx) {
    if (idx >= 10) {
        return;
    }

    HBlankEntry* entry;
    entry              = &g_HBlankManager.entries[idx];
    entry->pendingDest = &g_HBlankManager.nullSink;
    entry->pendingSrc  = &data_0205a128;
    entry->mode        = 1;
    if ((Interrupts_IsVBlankInterrupted() == FALSE) || (Interrupts_IsHBlankInterrupted() == FALSE)) {
        entry->activeDest = entry->pendingDest;
        entry->activeSrc  = entry->pendingSrc;
    }
}
