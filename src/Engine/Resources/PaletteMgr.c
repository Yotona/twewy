
#include "Engine/Resources/PaletteMgr.h"
#include "Display.h"
#include <NitroSDK/mi/cpumem.h>

extern u16 data_02059d24;
extern u8  data_02066aec;
extern s32 data_0206a9bc;

static void func_0200b09c(PaletteResource* arg0, void* arg1, s32 arg2, u32 arg3);
static void func_0200b198(PaletteResource* arg0, void* arg1, s32 arg2, u32 arg3);
static void func_0200b29c(PaletteResource* arg0, void* arg1, s32 arg2, u32 arg3);
static void func_0200b390(PaletteResource* arg0, void* arg1, s32 arg2, u32 arg3);
static void func_0200b484(PaletteResource* arg0, void* arg1, s32 arg2, u32 arg3);
static void func_0200b578(PaletteResource* arg0, void* arg1, s32 arg2, u32 arg3);
static void func_0200b66c(PaletteResource* arg0, void* arg1, u32 arg2, s32 arg3, s32 arg4);
static void func_0200b7b8(PaletteResource* arg0, void* arg1, u32 arg2, s32 arg3, s32 arg4);
static void func_0200b91c(PaletteResource* arg0, void* arg1, u32 arg2, s32 arg3, s32 arg4);
static void func_0200ba64(PaletteResource* arg0, void* arg1, u32 arg2, s32 arg3, s32 arg4);
static void func_0200bbac(PaletteResource* arg0, void* arg1, u32 arg2, s32 arg3, s32 arg4);
static void func_0200bcf4(PaletteResource* arg0, void* arg1, u32 arg2, s32 arg3, s32 arg4);
static void PaletteMgr_SetSource(PaletteMgr* mgr, PaletteResource* entry, void* dataPtr);

static const u16 data_0205ad64[4] = {0x20, 0x40, 0x60, 0x60};

static const PalLoadFunc data_0205ad6c[6] = {
    func_0200b09c, func_0200b198, func_0200b29c, func_0200b390, func_0200b484, func_0200b578,
};

static const PalLoadIndirectFunc data_0205ad84[6] = {
    func_0200b66c, func_0200b7b8, func_0200b91c, func_0200ba64, func_0200bbac, func_0200bcf4,
};

PaletteMgr* g_PaletteManagers[3];

static void PalSlot_InitAll(PalSlot slots[7][16]) {
    for (s32 row = 0; row < 7; row++) {
        for (s32 col = 0; col < 16; col++) {
            slots[row][col].index = -1;
            slots[row][col].flags = 0;
        }
    }
}

static s32 PalSlot_AllocContiguous(PalSlot* slots, s32 count) {
    s32 result = -1;

    for (s32 offset = 0; offset + count <= 16;) {
        // Check if all slots starting at offset are available
        s32 i;
        for (i = 0; i < count; i++) {
            if (slots[offset + i].index >= 0) {
                offset += (i + 1);
                break;
            }
        }

        // If all slots were available, allocate them
        if (i == count) {
            for (s32 j = 0; j < count; j++) {
                slots[offset + j].index = offset;
                slots[offset + j].flags = 0xFFFF;
            }
            result = offset;
            break;
        }
    }

    return result;
}

static void PalSlot_AllocContiguousPacked(PalSlot* result, PalSlot slots[7][16], u32 row, s32 count) {
    PalSlot slot;
    slot.index = PalSlot_AllocContiguous(slots[row], count);
    slot.flags = 0xFFFF;
    *result    = slot;
}

static s16 func_0200a25c(PalSlot* slots, u32 mask) {
    PalSlot* cur;
    s16      found;
    s32      i;

    cur   = slots;
    found = -1;
    i     = 0;
    do {
        if (cur->flags != 0 && !(cur->flags & mask)) {
            cur->index = i;
            found      = i;
            cur->flags |= mask;
            goto done;
        }
        i++;
        cur++;
    } while (i < 16);
done:
    if (found < 0) {
        i = 0;
        do {
            if (!(slots->flags & mask)) {
                slots->index = i;
                found        = i;
                slots->flags |= mask;
                goto end;
            }
            i++;
            slots++;
        } while (i < 16);
    }
end:
    return found;
}

static void func_0200a2f0(PalSlot* result, PalSlot slots[7][16], u32 row, s16 mask) {
    PalSlot slot;
    slot.index = func_0200a25c(slots[row], mask);
    slot.flags = mask;
    *result    = slot;
}

static s16 func_0200a32c(PalSlot* slots, s16 start, s32 count) {
    if (start + count > 16) {
        return -1;
    }

    // Check if all slots are available
    for (s32 i = 0; i < count; i++) {
        if (slots[start + i].index >= 0) {
            return -1;
        }
    }

    // Mark all slots as occupied
    for (s32 j = 0; j < count; j++) {
        slots[start + j].index = start;
        slots[start + j].flags = 0xFFFF;
    }

    return start;
}

static void func_0200a3b8(PalSlot* result, PalSlot slots[7][16], u32 row, s16 start, s32 count) {
    PalSlot* slot = result;
    slot->index   = func_0200a32c(slots[row], start, count);
    slot->flags   = 0xFFFF;
    slot->pad     = 0; // Initialize pad to 0
}

static void func_0200a3fc(PalSlot slots[7][16], u32 row, PalSlot* slotRef) {
    if (slotRef->index < 0) {
        return;
    }

    PalSlot* cur = &slots[row][slotRef->index];

    if (slotRef->flags == 0xFFFF) {
        for (s16 idx = slotRef->index; idx < 16; idx++) {
            if (cur->index != slotRef->index) {
                return;
            }
            cur->index = -1;
            cur->flags = 0;
            cur++;
        }
    } else {
        cur->flags &= ~slotRef->flags;
        if (cur->flags == 0)
            cur->index = -1;
    }
}

static void func_0200a484(PalSlot* slots) {
    return;
}

static void func_0200a488(PalSlot slots[7][16]) {
    func_0200a484(slots[0]);
    func_0200a484(slots[5]);
    func_0200a484(slots[6]);
}

static PaletteResource* PalRes_PopFree(PaletteMgr* mgr) {
    PaletteResource* entry = mgr->freeList;
    if (entry != NULL) {
        mgr->freeList = entry->next;
    }
    return entry;
}

static void PalRes_PushActive(PaletteMgr* mgr, PaletteResource* entry) {
    if (entry != NULL) {
        entry->next     = mgr->activeList;
        mgr->activeList = entry;
    }
}

static void PalRes_InitDefaults(PaletteMgr* mgr, PaletteResource* entry) {
    entry->flags = data_0205ad64[mgr->engine];
    entry->flags |= 0xF07;
    entry->refCount      = 0;
    entry->unk_08        = 0x80000000;
    entry->slotType      = 0;
    entry->chunkSize     = 0;
    entry->chunkCount    = 0;
    entry->colorParam    = 0;
    entry->palIndex      = -1;
    entry->unk_12        = 0;
    entry->processMode   = 0;
    entry->colorBias     = 0;
    entry->sourcePalette = NULL;
    entry->blendPalette  = &data_0205a128;
}

static PaletteResource* PalRes_RemoveActive(PaletteMgr* mgr, PaletteResource* target) {
    PaletteResource* prev;
    PaletteResource* cur = NULL;

    if (target != NULL) {
        if ((target->flags & 0x10) && (cur = mgr->activeList, prev = (PaletteResource*)&mgr->activeList, cur != NULL)) {
            do {
                if (cur == target) {
                    *(PaletteResource**)prev = cur->next;
                    break;
                }
                prev = cur;
                cur  = cur->next;
            } while (cur != NULL);
        }
    }
    return cur;
}

static void PalRes_PushFree(PaletteMgr* mgr, PaletteResource* entry) {
    if (entry != NULL && entry->flags & 0x10) {
        entry->next   = mgr->freeList;
        mgr->freeList = entry;
    }
}

void PaletteMgr_Init(PaletteMgr* mgr, DisplayEngine engine) {
    g_PaletteManagers[engine] = mgr;
    if (mgr == NULL)
        return;

    mgr->engine     = engine;
    mgr->activeList = NULL;
    mgr->freeList   = NULL;

    PaletteResource* cur = mgr->pool;
    for (u32 idx = 0; idx < 168; idx++) {
        cur->next     = mgr->freeList;
        mgr->freeList = cur;
        cur           = (PaletteResource*)((u8*)cur + 0x20);
    }

    PalSlot_InitAll(mgr->slots);

    GXVRam vramBank = GX_VRAM_NONE;
    switch (engine) {
        case DISPLAY_MAIN:
            mgr->unk_04 = &data_02066aec + (engine << 0xA);
            vramBank    = GX_GetBankForBgExtPltt();
            break;
        case DISPLAY_SUB:
            mgr->unk_04 = &data_02066aec + (engine << 0xA);
            vramBank    = GX_GetBankForSubBgExtPltt();
            break;
        case DISPLAY_EXTENDED: {
            PalSlot slot;
            mgr->unk_04 = &data_02066aec + (engine << 0xA);
            func_0200a3b8(&slot, mgr->slots, 0, 0, 16);
        } break;
    }

    mgr->unk_08 = 0;
    mgr->unk_0C = 0;
    mgr->unk_10 = 0;
    mgr->unk_14 = 0;

    switch (vramBank) {
        case GX_VRAM_F: {
            mgr->unk_08 = 0x06890000;
            mgr->unk_0C = 0x06890000 + 0x2000;
            PalSlot slot0;
            func_0200a3b8(&slot0, mgr->slots, 3, 0, 0x10);
            PalSlot slot1;
            func_0200a3b8(&slot1, mgr->slots, 4, 0, 0x10);
        } break;
        case GX_VRAM_G: {
            mgr->unk_10 = 0x06894000;
            mgr->unk_14 = 0x06894000 + 0x2000;
            PalSlot slot0;
            func_0200a3b8(&slot0, mgr->slots, 1, 0, 0x10);
            PalSlot slot1;
            func_0200a3b8(&slot1, mgr->slots, 2, 0, 0x10);
        } break;
        case GX_VRAM_E:
            mgr->unk_08 = 0x06880000;
            mgr->unk_0C = 0x06880000 + 0x2000;
            mgr->unk_10 = 0x06880000 + 0x4000;
            mgr->unk_14 = 0x06880000 + 0x6000;
            break;
        case GX_VRAM_FG:
            mgr->unk_08 = 0x06890000;
            mgr->unk_0C = 0x06890000 + 0x2000;
            mgr->unk_10 = 0x06890000 + 0x4000;
            mgr->unk_14 = 0x06890000 + 0x6000;
            break;
        case GX_VRAM_H:
            mgr->unk_08 = 0x06898000;
            mgr->unk_0C = 0x06898000 + 0x2000;
            mgr->unk_10 = 0x06898000 + 0x4000;
            mgr->unk_14 = 0x06898000 + 0x6000;
            break;
        default:
            PalSlot slot0;
            func_0200a3b8(&slot0, mgr->slots, 1, 0, 0x10);
            PalSlot slot1;
            func_0200a3b8(&slot1, mgr->slots, 2, 0, 0x10);
            PalSlot slot2;
            func_0200a3b8(&slot2, mgr->slots, 3, 0, 0x10);
            PalSlot slot3;
            func_0200a3b8(&slot3, mgr->slots, 4, 0, 0x10);
            break;
    }

    vramBank    = GX_VRAM_NONE;
    mgr->unk_1C = NULL;

    if (engine == DISPLAY_EXTENDED) {
        void* temp  = &data_02066aec + (engine << 0xA);
        mgr->unk_18 = temp;
        mgr->unk_1C = (void*)((u8*)temp + 0x200);
        return;
    }

    switch (engine) {
        case DISPLAY_MAIN:
            mgr->unk_18 = &data_02066aec + (engine << 0xA) + 0x200;
            vramBank    = GX_GetBankForObjExtPltt();
            break;
        case DISPLAY_SUB:
            mgr->unk_18 = &data_02066aec + (engine << 0xA) + 0x200;
            vramBank    = GX_GetBankForSubObjExtPltt();
            break;
    }

    switch (vramBank) {
        case GX_VRAM_F:
            mgr->unk_1C = (void*)0x06890000;
            return;
        case GX_VRAM_G:
            mgr->unk_1C = (void*)0x06894000;
            return;
        case GX_VRAM_I:
            mgr->unk_1C = (void*)0x068A0000;
            return;
        default: {
            PalSlot slot;
            func_0200a3b8(&slot, mgr->slots, 6, 0, 0x10);
            return;
        }
    }
}

// Nonmatching: Significant instruction differences in "found" loop and switch table
PaletteResource* PaletteMgr_AcquireContiguous(PaletteMgr* mgr, void* source, u16 slotType, u32 count) {
    PalSlot          slot;
    PaletteResource* found;

    if (mgr == NULL)
        return NULL;

    if (source != NULL) {
        found = mgr->activeList;

        while (found != NULL) {
            if (found->refCount != 0) {
                if (found->sourcePalette == source && found->chunkCount == count && found->unk_12 == 0xFFFF) {
                    break;
                }
            }
            found = found->next;
        }

        if (found != NULL) {
            if (found->refCount != 0xFFFF) {
                found->refCount++;
            }
            return found;
        }
    }

    PaletteResource* resource = PalRes_PopFree(mgr);
    if (resource == NULL) {
        return NULL;
    }

    PalRes_InitDefaults(mgr, resource);
    PalSlot_AllocContiguousPacked(&slot, mgr->slots, slotType, count);
    if (slot.index < 0) {
        PalRes_PushFree(mgr, resource);
        return NULL;
    }

    resource->flags |= 0x10;
    if (source != NULL) {
        resource->refCount = 1;
    }
    resource->slotType  = slotType & 7;
    resource->palIndex  = slot.index;
    resource->unk_12    = 0xFFFF;
    resource->colorBias = 0;

    switch (slotType) {
        default:
            break;
        case 0:
            resource->flags |= 0x2000;
            resource->chunkSize  = 1;
            resource->chunkCount = count;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            resource->flags |= 0x2000;
            resource->chunkSize  = 16;
            resource->chunkCount = count;
            break;
        case 5:
            resource->flags |= 0x1000;
            resource->chunkSize  = 1;
            resource->chunkCount = count;
            break;
        case 6:
            if (mgr->engine != DISPLAY_EXTENDED) {
                resource->flags |= 0x1000;
                resource->chunkSize  = 16;
                resource->chunkCount = count;
            } else {
                resource->flags |= 0x1000;
                resource->chunkSize  = 1;
                resource->chunkCount = count;
            }
            break;
    }

    PalRes_PushActive(mgr, resource);
    PaletteMgr_SetSource(mgr, resource, source);
    return resource;
}

PaletteResource* PaletteMgr_AcquireMasked(PaletteMgr* mgr, void* source, u16 slotType, s16 mask) {
    PalSlot          slot;
    PaletteResource* found;

    if (mgr == NULL)
        return NULL;

    if (source != NULL) {
        found = mgr->activeList;
        if (found != NULL) {
            do {
                if (found->refCount != 0 && found->sourcePalette == source && found->unk_12 == mask) {
                    break;
                }
                found = found->next;
            } while (found != NULL);
        }
        if (found != NULL) {
            if (found->refCount != 0xFFFF) {
                found->refCount++;
            }
            return found;
        }
    }

    PaletteResource* entry = PalRes_PopFree(mgr);
    if (entry == NULL)
        return NULL;

    PalRes_InitDefaults(mgr, entry);
    func_0200a2f0(&slot, mgr->slots, slotType, mask);
    if (slot.index < 0) {
        PalRes_PushFree(mgr, entry);
        return NULL;
    }

    entry->flags |= 0x10;
    if (source != NULL) {
        entry->refCount = 1;
    }
    entry->slotType   = slotType & 7;
    entry->chunkCount = 0;
    entry->palIndex   = slot.index;
    entry->unk_12     = slot.flags;
    entry->colorBias  = 0;

    switch (slotType) {
        default:
            break;
        case 0:
            entry->flags |= 0x2000;
            entry->chunkSize = 1;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            entry->flags |= 0x2000;
            entry->chunkSize = 16;
            break;
        case 5:
            entry->flags |= 0x1000;
            entry->chunkSize = 1;
            break;
        case 6:
            if (mgr->engine != DISPLAY_EXTENDED) {
                entry->flags |= 0x1000;
                entry->chunkSize = 16;
            } else {
                entry->flags |= 0x1000;
                entry->chunkSize = 1;
            }
            break;
    }

    PalRes_PushActive(mgr, entry);
    PaletteMgr_SetSource(mgr, entry, source);
    return entry;
}

PaletteResource* PaletteMgr_AllocPalette(PaletteMgr* mgr, void* sourcePalette, u16 slotType, s16 start, s32 count) {
    PalSlot          slot;
    PaletteResource* resource;

    if (mgr == NULL)
        return NULL;

    resource = PalRes_PopFree(mgr);
    if (resource == NULL)
        return NULL;

    PalRes_InitDefaults(mgr, resource);
    func_0200a3b8(&slot, mgr->slots, slotType, start, count);
    if (slot.index < 0) {
        PalRes_PushFree(mgr, resource);
        return NULL;
    }

    resource->flags |= 0x10;
    resource->slotType  = slotType & 7;
    resource->palIndex  = slot.index;
    resource->unk_12    = 0xFFFF;
    resource->colorBias = 0;

    switch (slotType) {
        default:
            break;
        case 0:
            resource->flags |= 0x2000;
            resource->chunkSize  = 1;
            resource->chunkCount = count;
            break;
        case 1:
        case 2:
        case 3:
        case 4:
            resource->flags |= 0x2000;
            resource->chunkSize  = 16;
            resource->chunkCount = count;
            break;
        case 5:
            resource->flags |= 0x1000;
            resource->chunkSize  = 1;
            resource->chunkCount = count;
            break;
        case 6:
            if (mgr->engine != DISPLAY_EXTENDED) {
                resource->flags |= 0x1000;
                resource->chunkSize  = 16;
                resource->chunkCount = count;
            } else {
                resource->flags |= 0x1000;
                resource->chunkSize  = 1;
                resource->chunkCount = count;
            }
            break;
    }

    PalRes_PushActive(mgr, resource);
    PaletteMgr_SetSource(mgr, resource, sourcePalette);
    return resource;
}

BOOL PaletteMgr_ReleaseResource(PaletteMgr* mgr, PaletteResource* resource) {
    BOOL released = FALSE;

    if (mgr == NULL) {
        return released;
    }

    if (resource == NULL || (resource->flags & 0x10) != 0x10) {
        return released;
    }

    if (resource->refCount != 0) {
        resource->refCount--;
    }
    if (resource->refCount == 0) {
        PalSlot slot;
        slot.index = resource->palIndex;
        slot.flags = resource->unk_12;
        func_0200a3fc(mgr->slots, resource->slotType, &slot);
        PalRes_RemoveActive(mgr, resource);
        PalRes_PushFree(mgr, resource);
        released = TRUE;
    }
    return released;
}

static void func_0200b09c(PaletteResource* entry, void* dest, s32 offset, u32 palOffset) {
    void* src;

    s16 adj = entry->colorBias;
    if (adj >= 0) {
        src = &data_02059d24;
    } else {
        src = &data_0205a128;
        adj = -adj;
    }

    if (entry->unk_12 == 0xFFFF) {
        func_02002180(dest, src, offset, palOffset, entry->chunkSize * 0x10 * entry->chunkCount, (u32)adj, 1);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            func_02002180(dest, src, offset, palOffset, entry->chunkSize, (u32)adj, 1);
            src = (void*)((u8*)src + entry->chunkSize * 2);
            offset += entry->chunkSize * 2;
        }
        palOffset += entry->chunkSize;
    }
}

static void func_0200b198(PaletteResource* entry, void* dest, s32 offset, u32 palOffset) {
    s32 src  = (s32)entry->blendPalette;
    s16 bias = entry->colorBias;

    if (entry->unk_12 == 0xFFFF) {
        func_02002254(dest, (void*)src, offset, palOffset, entry->chunkSize * 0x10 * entry->chunkCount, entry->colorParam,
                      bias, 1);
        return;
    }

    for (s32 i = 0xF; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            func_02002254(dest, (void*)src, offset, palOffset, entry->chunkSize, entry->colorParam, entry->colorBias, 1);
            src += entry->chunkSize * 2;
            offset += entry->chunkSize * 2;
        }
        palOffset += entry->chunkSize;
    }
}

static void func_0200b29c(PaletteResource* entry, void* dest, s32 offset, u32 palOffset) {
    if (entry->unk_12 == 0xFFFF) {
        func_02002398(dest, offset, palOffset, entry->chunkSize * 0x10 * entry->chunkCount, entry->colorParam,
                      entry->colorBias, 1);
        return;
    }

    for (s32 i = 0xF; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            func_02002398(dest, offset, palOffset, entry->chunkSize, entry->colorParam, entry->colorBias, 1);
            offset += entry->chunkSize * 2;
        }
        palOffset += entry->chunkSize;
    }
}

static void func_0200b390(PaletteResource* entry, void* dest, s32 offset, u32 palOffset) {
    if (entry->unk_12 == 0xFFFF) {
        func_0200245c(dest, offset, palOffset, entry->chunkSize * 0x10 * entry->chunkCount, entry->colorParam,
                      entry->colorBias, 1);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            func_0200245c(dest, offset, palOffset, entry->chunkSize, entry->colorParam, entry->colorBias, 1);
            offset += entry->chunkSize * 2;
        }
        palOffset += entry->chunkSize;
    }
}

static void func_0200b484(PaletteResource* entry, void* dest, s32 offset, u32 palOffset) {
    if (entry->unk_12 == 0xFFFF) {
        func_02002520(dest, offset, palOffset, entry->chunkSize * 0x10 * entry->chunkCount, entry->colorParam,
                      entry->colorBias, 1);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            func_02002520(dest, offset, palOffset, entry->chunkSize, entry->colorParam, entry->colorBias, 1);
            offset += entry->chunkSize * 2;
        }
        palOffset += entry->chunkSize;
    }
}

static void func_0200b578(PaletteResource* entry, void* dest, s32 offset, u32 palOffset) {
    if (entry->unk_12 == 0xFFFF) {
        func_020025e4(dest, offset, palOffset, entry->chunkSize * 16 * entry->chunkCount, entry->colorParam, entry->colorBias,
                      1);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            func_020025e4(dest, offset, palOffset, entry->chunkSize, entry->colorParam, entry->colorBias, 1);
            offset += entry->chunkSize * 2;
        }
        palOffset += entry->chunkSize;
    }
}

static void func_0200b66c(PaletteResource* entry, void* channelDest, u32 baseOffset, s32 srcOffset, s32 dstOffset) {
    void* src;
    void* buf;

    u32 totalSize = entry->chunkSize * 16;
    totalSize *= entry->chunkCount;

    s16 adj = entry->colorBias;
    if (adj >= 0) {
        src = &data_02059d24;
    } else {
        src = &data_0205a128;
        adj = -adj;
    }

    if (entry->unk_12 == 0xFFFF) {
        u32 bufSize = totalSize * 2;
        buf         = func_02004ce8(&data_0206a9bc, bufSize);
        func_02002180(buf, src, srcOffset, totalSize, adj, 0, 0);
        func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, bufSize);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            buf = func_02004ce8(&data_0206a9bc, entry->chunkSize * 2);
            func_02002180(buf, src, srcOffset, 0U, entry->chunkSize, adj, 0);
            func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, entry->chunkSize * 2);
            src = (void*)((u8*)src + entry->chunkSize * 2);
            srcOffset += entry->chunkSize * 2;
        }
        dstOffset += entry->chunkSize;
    }
}

static void func_0200b7b8(PaletteResource* entry, void* channelDest, u32 baseOffset, s32 srcOffset, s32 dstOffset) {
    void* buf;

    u32 totalSize = entry->chunkSize * 16;
    totalSize *= entry->chunkCount;

    s32 src = (s32)entry->blendPalette;

    if (entry->unk_12 == 0xFFFF) {
        u32 bufSize = totalSize * 2;
        buf         = func_02004ce8(&data_0206a9bc, bufSize);
        func_02002254(buf, src, srcOffset, totalSize, entry->colorParam, entry->colorBias, 0, 0);
        func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, bufSize);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            buf = func_02004ce8(&data_0206a9bc, entry->chunkSize * 2);
            func_02002254(buf, src, srcOffset, entry->chunkSize, entry->colorParam, entry->colorBias, 0, 0);
            func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, entry->chunkSize * 2);
            src += entry->chunkSize * 2;
            srcOffset += entry->chunkSize * 2;
        }
        dstOffset += entry->chunkSize;
    }
}

static void func_0200b91c(PaletteResource* entry, void* channelDest, u32 baseOffset, s32 srcOffset, s32 dstOffset) {
    void* buf;

    u32 totalSize = entry->chunkSize * 16;
    totalSize *= entry->chunkCount;

    if (entry->unk_12 == 0xFFFF) {
        u32 bufSize = totalSize * 2;
        buf         = func_02004ce8(&data_0206a9bc, bufSize);
        func_02002398(buf, srcOffset, 0, totalSize, entry->colorParam, entry->colorBias, 0);
        func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, bufSize);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            buf = func_02004ce8(&data_0206a9bc, entry->chunkSize * 2);
            func_02002398(buf, srcOffset, 0, entry->chunkSize, entry->colorParam, entry->colorBias, 0);
            func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, entry->chunkSize * 2);
            srcOffset += entry->chunkSize * 2;
        }
        dstOffset += entry->chunkSize;
    }
}

static void func_0200ba64(PaletteResource* entry, void* channelDest, u32 baseOffset, s32 srcOffset, s32 dstOffset) {
    void* buf;

    u32 totalSize = entry->chunkSize * 16;
    totalSize *= entry->chunkCount;

    if (entry->unk_12 == 0xFFFF) {
        u32 bufSize = totalSize * 2;
        buf         = func_02004ce8(&data_0206a9bc, bufSize);
        func_0200245c(buf, srcOffset, 0, totalSize, entry->colorParam, entry->colorBias, 0);
        func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, bufSize);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            buf = func_02004ce8(&data_0206a9bc, entry->chunkSize * 2);
            func_0200245c(buf, srcOffset, 0U, entry->chunkSize, entry->colorParam, entry->colorBias, 0);
            func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, entry->chunkSize * 2);
            srcOffset += entry->chunkSize * 2;
        }
        dstOffset += entry->chunkSize;
    }
}

static void func_0200bbac(PaletteResource* entry, void* channelDest, u32 baseOffset, s32 srcOffset, s32 dstOffset) {
    void* buf;

    u32 totalSize = entry->chunkSize * 16;
    totalSize *= entry->chunkCount;

    if (entry->unk_12 == 0xFFFF) {
        u32 bufSize = totalSize * 2;
        buf         = func_02004ce8(&data_0206a9bc, bufSize);
        func_02002520(buf, srcOffset, 0, totalSize, entry->colorParam, entry->colorBias, 0);
        func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, bufSize);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            buf = func_02004ce8(&data_0206a9bc, entry->chunkSize * 2);
            func_02002520(buf, srcOffset, 0, entry->chunkSize, entry->colorParam, entry->colorBias, 0);
            func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, entry->chunkSize * 2);
            srcOffset += entry->chunkSize * 2;
        }
        dstOffset += entry->chunkSize;
    }
}

static void func_0200bcf4(PaletteResource* entry, void* channelDest, u32 baseOffset, s32 srcOffset, s32 dstOffset) {
    void* buf;

    u32 totalSize = entry->chunkSize * 16;
    totalSize *= entry->chunkCount;

    if (entry->unk_12 == 0xFFFF) {
        u32 bufSize = totalSize * 2;
        buf         = func_02004ce8(&data_0206a9bc, bufSize);
        func_020025e4(buf, srcOffset, 0, totalSize, entry->colorParam, entry->colorBias, 0);
        func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, bufSize);
        return;
    }

    for (s32 i = 15; i >= 0; i--) {
        if (entry->unk_12 & (1 << i)) {
            buf = func_02004ce8(&data_0206a9bc, entry->chunkSize * 2);
            func_020025e4(buf, srcOffset, 0, entry->chunkSize, entry->colorParam, entry->colorBias, 0);
            func_02001b44(channelDest, (void*)(baseOffset + dstOffset * 2), buf, entry->chunkSize * 2);
            srcOffset += entry->chunkSize * 2;
        }
        dstOffset += entry->chunkSize;
    }
}

static void PaletteMgr_CommitResourceIfDirty(PaletteMgr* mgr, PaletteResource* resource) {
    if ((resource->flags & 0x8000) == 0) {
        return;
    }

    if (resource->sourcePalette == NULL) {
        return;
    }

    u32 size = resource->chunkSize * 16;
    size *= resource->palIndex;

    void* vramPtr = ((void**)&mgr->unk_04)[resource->slotType];

    BOOL useIndirect = TRUE;
    s32  dest;

    switch (resource->slotType) {
        case 0:
        case 5: {
            useIndirect = FALSE;
        } break;

        case 6: {
            switch (mgr->engine) {
                case DISPLAY_MAIN:
                    dest = 4;
                    break;
                case DISPLAY_SUB:
                    dest = 6;
                    break;
                case DISPLAY_EXTENDED:
                    useIndirect = FALSE;
                    dest        = 2;
                    break;
            }
        } break;

        default: {
            switch (mgr->engine) {
                case DISPLAY_MAIN:
                    dest = 3;
                    break;
                case DISPLAY_SUB:
                    dest = 5;
                    break;
                case DISPLAY_EXTENDED:
                    useIndirect = FALSE;
                    dest        = 2;
                    break;
            }
        } break;
    }

    if (useIndirect == TRUE) {
        data_0205ad84[resource->processMode](resource, dest, vramPtr, resource->sourcePalette, size);
    } else {
        data_0205ad6c[resource->processMode](resource, vramPtr, resource->sourcePalette, size);
    }
    resource->flags &= ~0x8000;
}

// Nonmatching: Missing instruction, related registers are misallocated
void PaletteMgr_Flush(PaletteMgr* mgr, PaletteResource* resource) {
    if (mgr == NULL) {
        return;
    }

    if (resource != NULL) {
        resource->flags |= 0x8000;
        PaletteMgr_CommitResourceIfDirty(mgr, resource);
        return;
    }

    for (PaletteResource* cur = mgr->activeList; cur != NULL; cur = cur->next) {
        PaletteMgr_CommitResourceIfDirty(mgr, cur);
    }
}

void PaletteMgr_SetSource(PaletteMgr* mgr, PaletteResource* resource, void* sourcePalette) {
    if (mgr != NULL && resource->sourcePalette != sourcePalette) {
        resource->sourcePalette = sourcePalette;
        if (sourcePalette != NULL) {
            resource->flags |= 0x8000;
        }
    }
}

void PaletteMgr_SetBlendProcess(PaletteMgr* mgr, PaletteResource* resource, s32 filter, u32 colorParam, void* blendPalette) {
    if (mgr == NULL) {
        return;
    }
    if (filter == 0) {
        filter = 0x3F00;
    }
    if (blendPalette == NULL) {
        blendPalette = &data_0205a128;
    }

    BOOL allMode = FALSE;
    if (resource == NULL) {
        resource = mgr->activeList;
        allMode  = TRUE;
    }
    if (resource == NULL)
        return;

    do {
        BOOL match = FALSE;
        if ((resource->flags & (filter & 0x3000)) && (resource->flags & (filter & 0xF00))) {
            match = TRUE;
        }
        if (match == TRUE) {
            if (resource->processMode != 1 || colorParam != resource->colorParam || resource->blendPalette != blendPalette) {
                resource->flags |= 0x8000;
                resource->processMode  = 1;
                resource->colorParam   = colorParam;
                resource->blendPalette = blendPalette;
            }
        }
        resource = resource->next;
    } while (allMode != FALSE && resource != NULL);
}

static void PaletteMgr_SetProcessMode(PaletteMgr* mgr, PaletteResource* resource, s32 filter, s32 processMode,
                                      u32 colorParam) {
    if (mgr == NULL) {
        return;
    }

    if (filter == 0) {
        filter = 0x3F00;
    }

    BOOL allMode = FALSE;
    if (resource == NULL) {
        resource = mgr->activeList;
        allMode  = TRUE;
    }

    if (resource != NULL) {
        do {
            BOOL match = FALSE;
            if ((resource->flags & (filter & 0x3000)) && (resource->flags & (filter & 0xF00))) {
                match = TRUE;
            }
            if (match == TRUE && (resource->processMode != processMode || resource->colorParam != colorParam)) {
                resource->flags |= 0x8000;
                resource->processMode = processMode;
                resource->colorParam  = colorParam;
            }
            resource = resource->next;
        } while (allMode != FALSE && resource != NULL);
    }
}

void PaletteMgr_SetProcess(PaletteMgr* mgr, PaletteResource* resource, s32 filter, s32 mode, s32 colorParam, void* blend) {
    if (mode == 1) {
        PaletteMgr_SetBlendProcess(mgr, resource, filter, colorParam, blend);
    } else {
        PaletteMgr_SetProcessMode(mgr, resource, filter, mode, colorParam);
    }
}

void PaletteMgr_SetColorBias(PaletteMgr* mgr, PaletteResource* resource, s32 filter, s16 colorBias) {
    if (mgr == NULL) {
        return;
    }
    if (filter == 0) {
        filter = 0x3F00;
    }

    BOOL allMode = FALSE;
    if (resource == NULL) {
        resource = mgr->activeList;
        allMode  = TRUE;
    }

    if (resource != NULL) {
        do {
            BOOL match = ((resource->flags & (filter & 0x3000)) && (resource->flags & (filter & 0xF00)));
            if (match == TRUE && resource->colorBias != colorBias) {
                resource->flags |= 0x8000;
                resource->colorBias = colorBias;
            }
            resource = resource->next;
        } while (allMode != FALSE && resource != NULL);
    }
}

void PaletteMgr_UnknownLoop(PaletteMgr* mgr) {
    if (mgr != NULL) {
        func_0200a488(mgr->slots);
        for (PaletteResource* cur = mgr->activeList; cur != NULL; cur = cur->next) {
            // Intentionally empty
        }
    }
}
