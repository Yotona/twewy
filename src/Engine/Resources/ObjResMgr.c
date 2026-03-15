#include "Engine/Resources/ObjResMgr.h"

static void ObjResMgr_FreeBitmapByOffset(ObjResBitmap* bitmap, s32 offset);
static s32  func_0200c818(ObjResBitmap* bitmap, s32 value, s32 shift);
static void ObjResMgr_LoadToVram(ObjResMgr* mgr, ObjResource* entry, void* data, s32 mode);

extern void func_0200ed38(void);
extern void func_0200ed4c(void);
extern void func_0200ed74(void);
extern void func_0200ed90(void);

extern u8  data_020676ec;
extern u8  data_020676f4;
extern u8  data_02067700;
extern s32 data_0206a9bc;

typedef struct {
    u16 sizes[4];
    void (*funcs[4])();
} SprPalSizeTable;

static const SprPalSizeTable data_0205ad9c = {
    {                   0x0020,                    0x0040,                    0x0060,                    0x0060},
    {(void (*)())func_0200ed38, (void (*)())func_0200ed4c, (void (*)())func_0200ed74, (void (*)())func_0200ed90}
};

ObjResMgr* g_ObjResourceManagers[3];

static void ObjResMgr_InitBitmap(ObjResBitmap* bitmap, u32 sizeMode) {
    s16* slots;

    MI_CpuFillU16(0, bitmap, sizeof(*bitmap));
    bitmap->mode = sizeMode;
    slots        = bitmap->slots;

    s16 size;
    switch (bitmap->mode) {
        case 1:
            size = 0x80;
            break;
        case 2:
            size = 0x100;
            break;
        case 3:
            size = 0x200;
            break;
        case 4:
            size = 0x300;
            break;
        case 5:
            size = 0x400;
            break;
        case 6:
            size = 0x800;
            break;
        default:
        case 0:
            size = 0x40;
            break;
    }

    s16 remainder = (s16)(0 - (s16)(0x800 - size));
    slots[0]      = size;
    s16* end      = slots + size * 2;
    end[0]        = remainder;
    end[1]        = size;
    if (size != 0x800) {
        slots[0x1001] = remainder;
    }
}

static s16 ObjResMgr_AllocBitmapFromStart(s16* bitmap, s32 count, s32 alignment) {
    s32 temp;

    s16 curSize  = bitmap[0];
    s16 negCount = (s16)(0 - count);
    s16 result   = -1;
    s16 curIdx   = 0;
    if (curSize != 0) {
        do {
            s32 alignOff = curIdx & alignment;
            if (curSize >= count + alignOff) {
                if (alignOff != 0) {
                    s16 splitSize                      = (s16)((alignment + 1) - alignOff);
                    s16 newIdx                         = curIdx + splitSize;
                    s16 remain                         = curSize - splitSize;
                    bitmap[curIdx * 2]                 = splitSize;
                    bitmap[newIdx * 2 + 1]             = splitSize;
                    bitmap[newIdx * 2]                 = remain;
                    bitmap[(curIdx + curSize) * 2 + 1] = remain;
                    curIdx                             = newIdx;
                }
                temp = bitmap[curIdx * 2];
                if (temp == count) {
                    bitmap[curIdx * 2]               = negCount;
                    result                           = curIdx;
                    bitmap[(curIdx + count) * 2 + 1] = negCount;
                } else if (temp > count) {
                    s32 next                          = curIdx + count;
                    s16 leftover                      = temp + negCount;
                    bitmap[curIdx * 2]                = negCount;
                    bitmap[next * 2 + 1]              = negCount;
                    bitmap[next * 2]                  = leftover;
                    result                            = curIdx;
                    bitmap[(next + leftover) * 2 + 1] = leftover;
                } else {
                    goto advance;
                }
            } else {
            advance:
                temp = bitmap[curIdx * 2];
                if (temp < 0) {
                    temp = -temp;
                }
                curIdx += temp;
                curSize = bitmap[curIdx * 2];
                if (curSize != 0)
                    continue;
                break;
            }
            break;
        } while (1);
    }
    return result;
}

static s32 ObjResMgr_AllocBitmapFromEnd(s16* bitmap, s32 count, s32 alignment) {
    s16 curSize;
    s16 negCount;
    s32 result;
    s16 curIdx;
    s16 temp;
    s32 alignOff;

    curSize  = bitmap[0x800 * 2 + 1];
    negCount = (s16)(0 - count);
    result   = -1;
    curIdx   = 0x800;
    if (curSize != 0) {
        do {
            alignOff = curIdx & alignment;
            if (curSize >= count + alignOff) {
                if (alignOff != 0) {
                    s16* curSlot             = &bitmap[curIdx * 2];
                    s16  prevSize            = curSlot[1];
                    s16  splitOff            = (s16)(curIdx - alignOff);
                    s16  newRemain           = (s16)((prevSize - alignOff) & 0xFFFF);
                    s32  prevIdx             = curIdx - prevSize;
                    bitmap[splitOff * 2]     = (s16)alignOff;
                    curSlot[1]               = (s16)alignOff;
                    bitmap[prevIdx * 2]      = newRemain;
                    bitmap[splitOff * 2 + 1] = newRemain;
                    curIdx                   = splitOff;
                }
                {
                    s16* curSlot  = &bitmap[curIdx * 2];
                    s16  slotSize = curSlot[1];
                    if (slotSize == count) {
                        result             = curIdx + negCount;
                        bitmap[result * 2] = negCount;
                        curSlot[1]         = negCount;
                    } else if (slotSize > count) {
                        s16 leftover                    = slotSize + negCount;
                        result                          = curIdx + negCount;
                        bitmap[(result - leftover) * 2] = leftover;
                        bitmap[result * 2 + 1]          = leftover;
                        bitmap[result * 2]              = negCount;
                        curSlot[1]                      = negCount;
                    } else {
                        goto advance;
                    }
                }
            } else {
            advance:
                temp = bitmap[curIdx * 2 + 1];
                if (temp < 0) {
                    temp = (s16)(0 - temp);
                }
                curIdx -= temp;
                curSize = bitmap[curIdx * 2 + 1];
                if (curSize != 0)
                    continue;
                break;
            }
            break;
        } while (1);
    }
    return result;
}

static s32 ObjResMgr_AllocBitmapFromStartValidated(ObjResBitmap* bitmap, u32 size, s32 shift) {
    s16 result;
    s32 allocSize;
    s32 mask;

    allocSize = 1 << shift;
    if (allocSize < 4) {
        allocSize = 4;
    }
    mask   = allocSize - 1;
    result = ObjResMgr_AllocBitmapFromStart(bitmap->slots, (s32)((s16)((~mask & (mask + (size >> 5))) << 0xE) >> 0x10),
                                            (s32)((s16)(mask + ((u32)(mask >> 1) >> 0x1E)) >> 2));
    if (result >= 0) {
        result <<= 7;
        if (func_0200c818(bitmap, size + result, shift) >= 0x400) {
            ObjResMgr_FreeBitmapByOffset(bitmap, result);
            result = -1;
        }
    }
    return result;
}

static s32 ObjResMgr_AllocBitmapFromEndValidated(ObjResBitmap* bitmap, u32 size, s32 shift) {
    s32 result;
    s32 allocSize;
    s32 mask;

    allocSize = 1 << shift;
    if (allocSize < 4) {
        allocSize = 4;
    }
    mask   = allocSize - 1;
    result = ObjResMgr_AllocBitmapFromEnd(bitmap->slots, (s32)((s16)((~mask & (mask + (size >> 5))) << 0xE) >> 0x10),
                                          (s32)((s16)(mask + ((u32)(mask >> 1) >> 0x1E)) >> 2));
    if (result >= 0) {
        result <<= 7;
        if (func_0200c818(bitmap, size + result, shift) >= 0x400) {
            ObjResMgr_FreeBitmapByOffset(bitmap, result);
            result = -1;
        }
    }
    return result;
}

static void ObjResMgr_FreeAndCoalesceBitmap(s16* base, s32 idx) {
    s16 val;
    s32 freeSize;
    s32 curVal;
    s32 nextIdx;
    s32 prevFree;
    s32 curFree;
    s16 prevBack;

    if (idx < 0)
        return;
    if (idx >= 0x800)
        return;

    val = base[idx * 2];
    if (val >= 0)
        return;

    freeSize      = (0 - val);
    base[idx * 2] = freeSize;
    nextIdx       = (idx + freeSize) * 2;
    if (base[nextIdx] > 0) {
        base[idx * 2] = (s16)(base[idx * 2] + base[nextIdx]);
        base[nextIdx] = 0;
    }

    // Merge forward
    prevFree = 0;
    curFree  = 0;
    if (idx >= 0) {
        do {
            if (curFree == idx && prevFree != idx) {
                s16 prevVal = base[prevFree * 2];
                if (prevVal > 0) {
                    base[prevFree * 2] = (s16)(prevVal + base[idx * 2]);
                    base[idx * 2]      = 0;
                }
                break;
            }
            curVal   = base[curFree * 2];
            prevFree = curFree;
            if (curVal < 0) {
                curVal = (0 - curVal);
            }
            curFree += curVal;
        } while (curFree <= idx);
    }

    // Backward merge
    nextIdx               = idx + freeSize;
    base[nextIdx * 2 + 1] = freeSize;

    prevBack = base[(nextIdx - freeSize) * 2 + 1];
    if (prevBack > 0) {
        base[nextIdx * 2 + 1]              = (s16)(base[nextIdx * 2 + 1] + prevBack);
        base[(nextIdx - freeSize) * 2 + 1] = 0;
    }

    // Merge backward through chain
    prevFree = 0x800;
    curFree  = 0x800;
    if (nextIdx <= 0x800) {
        do {
            if (curFree == nextIdx && prevFree != nextIdx) {
                if (base[prevFree * 2 + 1] > 0) {
                    base[prevFree * 2 + 1] = (s16)(base[prevFree * 2 + 1] + base[nextIdx * 2 + 1]);
                    base[nextIdx * 2 + 1]  = 0;
                }
                return;
            }
            curVal   = base[curFree * 2 + 1];
            prevFree = curFree;
            if (curVal < 0) {
                curVal = -curVal;
            }
            curFree -= curVal;
        } while (curFree >= nextIdx);
    }
}

static void ObjResMgr_FreeBitmapByOffset(ObjResBitmap* bitmap, s32 offset) {
    ObjResMgr_FreeAndCoalesceBitmap(bitmap->slots, (s32)(offset + ((u32)(offset >> 6) >> 0x19)) >> 7);
}

static s32 func_0200c7e8(ObjResBitmap* bitmap, s32 offset) {
    s32 result = 0;
    if (offset >= 0) {
        result = (0 - bitmap->slots[((s32)(offset + ((u32)(offset >> 6) >> 0x19)) >> 7) * 2]) << 7;
    }
    return result;
}

static s32 func_0200c818(ObjResBitmap* bitmap, s32 value, s32 shift) {
    s32 result = -1;
    if (value >= 0) {
        result = value >> (shift + 5);
    }
    return result;
}

static ObjResource* ObjResMgr_PopFreeEntry(ObjResMgr* mgr) {
    ObjResource* entry = mgr->freeList;
    if (entry != NULL) {
        mgr->freeList = entry->next;
    }
    return entry;
}

static void ObjResMgr_PushActiveEntry(ObjResMgr* mgr, ObjResource* entry) {
    if (entry != NULL) {
        entry->next     = mgr->activeList;
        mgr->activeList = entry;
    }
}

static void ObjResMgr_Reset(ObjResMgr* mgr, ObjResource* entry) {
    entry->flags         = (u16)(data_0205ad9c.sizes[mgr->engine] | 6);
    entry->refCount      = 0;
    entry->mode          = 0;
    entry->skipTransfer  = 0;
    entry->unk_02        = 0;
    entry->hasSourceData = 0;
    entry->unk_0C        = 0;
    entry->unk_0A        = 0;
    entry->bitmapIndex   = -1;
    entry->unk_12        = -1;
    entry->vramOffset    = -1;
    entry->unk_18        = NULL;
    entry->unk_1C        = 0;
}

static ObjResource* ObjResMgr_RemoveActiveEntry(ObjResMgr* mgr, ObjResource* target) {
    ObjResource* prev;
    ObjResource* cur = NULL;
    if (target != NULL) {
        if ((target->flags & 0x10) && (cur = mgr->activeList, prev = (ObjResource*)&mgr->activeList, cur != NULL)) {
            do {
                if (cur == target) {
                    *(ObjResource**)prev = cur->next;
                    break;
                }
                prev = cur;
                cur  = cur->next;
            } while (cur != NULL);
        }
    }
    return cur;
}

static void ObjResMgr_PushFreeEntry(ObjResMgr* mgr, ObjResource* entry) {
    if (entry == NULL)
        return;
    if (entry->flags & 0x10) {
        entry->next   = mgr->freeList;
        mgr->freeList = entry;
    }
}

void ObjResMgr_Init(ObjResMgr* mgr, DisplayEngine engine) {
    g_ObjResourceManagers[engine] = mgr;

    if (mgr == NULL) {
        return;
    }

    mgr->engine     = engine;
    mgr->activeList = NULL;
    mgr->freeList   = NULL;

    for (u32 i = 0; i < 0x80; i++) {
        ObjResource* cur = &mgr->pool[i];
        cur->next        = mgr->freeList;
        mgr->freeList    = cur;
    }

    s32 sizeMode;
    switch (engine) {
        case 0:
            switch (GX_GetBankForObj()) {
                case GX_VRAM_F:
                    sizeMode = 1;
                    break;
                case GX_VRAM_G:
                    sizeMode = 1;
                    break;
                case GX_VRAM_FG:
                    sizeMode = 2;
                    break;
                case GX_VRAM_E:
                case GX_VRAM_EF:
                case GX_VRAM_EG:
                    sizeMode = 3;
                    break;
                case GX_VRAM_EFG:
                    sizeMode = 4;
                    break;
                case GX_VRAM_A:
                    sizeMode = 5;
                    break;
                case GX_VRAM_B:
                    sizeMode = 5;
                    break;
                case GX_VRAM_AB:
                    sizeMode = 6;
                    break;
                default:
                    sizeMode = 0;
                    break;
            }
            mgr->unk_04   = 7;
            mgr->vramBase = 0x06400000;
            break;
        case 1:
            switch (GX_GetBankForSubObj()) {
                case GX_VRAM_D:
                    sizeMode = 5;
                    break;
                case GX_VRAM_I:
                    sizeMode = 1;
                    break;
                default:
                    sizeMode = 0;
                    break;
            }
            mgr->unk_04   = 7;
            mgr->vramBase = 0x06600000;
            break;
        case 2: {
            switch (GX_GetBankForTex()) {
                case 0:
                    sizeMode = 0;
                    break;
                case 1:
                case 2:
                case 4:
                case 8:
                    sizeMode = 5;
                    break;
                case 3:
                case 6:
                case 7:
                case 12:
                case 14:
                case 15:
                    sizeMode = 6;
                    break;
                case 5:
                case 9:
                case 10:
                case 11:
                case 13:
                    sizeMode = 5;
                    break;
                default:
                    break;
            }
            mgr->unk_04   = 1;
            mgr->vramBase = 0;
            break;
        }
    }

    ObjResMgr_InitBitmap(&mgr->bitmap, sizeMode);
}

ObjResource* ObjResMgr_AllocResource(ObjResMgr* mgr, void* data, u32 size, u32 mode) {
    ObjResource* entry;

    if (mgr == NULL) {
        return NULL;
    }

    if (data == NULL && size == 0) {
        return NULL;
    }

    if (size == 0) {
        size = (u32)(*(s32*)data & ~0xFF) >> 8;
        if (!((*(u8*)data) & 0xF0)) {
            size -= 4;
        }
    }

    if (data != NULL) {
        ObjResource* found = mgr->activeList;
        if (found != NULL) {
            do {
                if (found->hasSourceData == 1 && mode == found->mode && found->unk_18 == data) {
                    if (found->vramOffset >= size) {
                        if (found->refCount != 0xFFFF) {
                            found->refCount++;
                        }
                        return found;
                    }
                }
                found = found->next;
            } while (found != NULL);
        }
    }

    entry = ObjResMgr_PopFreeEntry(mgr);
    if (entry == NULL)
        return NULL;

    ObjResMgr_Reset(mgr, entry);
    entry->mode = mode;

    switch (mode) {
        case 0:
            entry->bitmapIndex = ObjResMgr_AllocBitmapFromStartValidated(
                                     &mgr->bitmap, size, *(s32*)((u8*)&data_020676f4 + mgr->engine * 0x108C + 8)) >>
                                 5;
            break;
        case 1:
            entry->bitmapIndex = ObjResMgr_AllocBitmapFromEndValidated(
                                     &mgr->bitmap, size, *(s32*)((u8*)&data_02067700 + mgr->engine * 0x108C + 8)) >>
                                 5;
            break;
    }

    if (entry->bitmapIndex < 0) {
        ObjResMgr_PushFreeEntry(mgr, entry);
        return NULL;
    }
    if (data != NULL) {
        entry->hasSourceData = 1;
    }
    entry->refCount = 1;
    entry->flags |= 0x10;
    entry->vramOffset = func_0200c7e8(&mgr->bitmap, entry->bitmapIndex << 5);
    ObjResMgr_PushActiveEntry(mgr, entry);
    ObjResMgr_LoadToVram(mgr, entry, data, 0);
    return entry;
}

BOOL ObjResMgr_ReleaseResource(ObjResMgr* mgr, ObjResource* resource) {
    BOOL released = FALSE;
    if (mgr == NULL || resource == NULL) {
        return released;
    }

    if ((resource->flags & 0x10) == 0x10) {
        resource->refCount--;
        if (resource->refCount == 0) {
            ObjResMgr_FreeBitmapByOffset(&mgr->bitmap, resource->bitmapIndex << 5);
            ObjResMgr_RemoveActiveEntry(mgr, resource);
            ObjResMgr_PushFreeEntry(mgr, resource);
            released = TRUE;
        }
    }
    return released;
}

void ObjResMgr_LoadToVram(ObjResMgr* mgr, ObjResource* resource, void* data, s32 mode) {
    u32 dataSize;
    s32 header;

    if (mgr == NULL || data == NULL)
        return;
    if (resource->skipTransfer == 1)
        return;

    resource->unk_12 = resource->bitmapIndex;
    if (mode != 0) {
        if (!(*(u8*)data & 0xF0)) {
            OamMgr_QueueCellCharTransfers((s32)((u8*)&data_020676ec + mgr->engine * 0x108C), (s32)mgr->vramBase,
                                          resource->bitmapIndex << 5, (void*)((u8*)data + 4), mode);
        }
    } else {
        header   = *(s32*)data;
        dataSize = (u32)(header & ~0xFF) >> 8;
        if ((header & 0xF0) == 0) {
            dataSize -= 4;
        }
        if (resource->vramOffset < dataSize) {
            dataSize = resource->vramOffset;
        }
        if ((header & 0xF0) == 0) {
            func_02001b44((void*)mgr->unk_04, (void*)(mgr->vramBase + (resource->bitmapIndex << 5)), (void*)((u8*)data + 4),
                          dataSize);
        } else if (dataSize <= resource->vramOffset) {
            if (mgr->engine == 2) {
                void* buf = func_02004ce8(&data_0206a9bc, dataSize);
                func_02004d60(buf, data);
                func_02001b44((void*)mgr->unk_04, (void*)(mgr->vramBase + (resource->bitmapIndex << 5)), buf, dataSize);
            } else {
                func_02004e0c((s32)(mgr->vramBase + (resource->bitmapIndex << 5)), data);
            }
        }
    }
    resource->unk_18 = data;
    resource->unk_1C = mode;
}
