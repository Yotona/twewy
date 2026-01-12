#include "BgResMgr.h"

void BgResMgr_LoadCharToVram(BgResMgr* arg0, BgResource* arg1, UnkSmallStruct_DispBG* arg2);
void BgResMgr_LoadScreenToVram(BgResMgr* arg0, BgResource* arg1, UnkSmallStruct_DispBG* arg2);

BgResMgr*        g_BgResourceManagers[2] = {NULL, NULL};
static const u16 DefaultFlags[4]         = {0x20, 0x40, 0x60, 0x60};

static void func_0200927c(s32*, s32) {
    // Not yet implemented
}

// Nonmatching: Several register differences
// Scratch: iINBi
static s32 BgResMgr_AllocateVram(s16* param_1, u32 param_2, u32 param_3) {
    s32 offset      = param_2 >> 8;
    s32 alloc_size  = param_3 >> 8;
    s16 current_val = param_1[offset];

    if (current_val != 0) {
        if (alloc_size == current_val) {
            param_1[offset] = -alloc_size;
        } else if (current_val > alloc_size) {
            param_1[offset + alloc_size] = current_val - alloc_size;
            param_1[offset]              = -alloc_size;
        } else {
            if (current_val > 0) {
                offset = -(offset + current_val);
            } else {
                offset = -(offset - current_val);
            }
        }
    } else {
        s32 prev_idx = 0;
        s32 curr_idx = 0;

        while (curr_idx < offset) {
            prev_idx    = curr_idx;
            current_val = param_1[curr_idx];
            curr_idx += (current_val < 0) ? -current_val : current_val;
        }

        current_val = param_1[prev_idx];

        if (current_val > 0 && (curr_idx - offset) >= alloc_size) {
            param_1[offset]   = -alloc_size;
            param_1[prev_idx] = offset - prev_idx;
            if (curr_idx > alloc_size) {
                param_1[offset + alloc_size] = curr_idx - (offset + alloc_size);
            }
        } else {
            offset = (current_val < 0) ? -(offset - current_val) : -prev_idx;
        }
    }

    return offset;
}

static u32 BgResMgr_AllocateVramAligned(s16* arg0, u32 arg1, s32 arg2, u32 arg3, u32 arg4) {
    u32 result = -1;

    while ((u32)(arg1 + arg2) <= arg4) {
        s32 alloc_result = BgResMgr_AllocateVram(arg0, arg1, arg2);

        if (alloc_result >= 0) {
            result = arg1 >> 8;
            break;
        }

        s32 offset = alloc_result << 8;
        if (offset < 0) {
            offset = -offset;
        }

        arg1 = arg3 * ((u32)((arg3 - 1) + offset) / arg3);
    }

    return result;
}

static void BgResMgr_FreeVram(s16* arg0, s32 bitmapIndex) {
    s32 var_r5 = 0;
    s32 var_r3 = 0;

    while (var_r3 < bitmapIndex) {
        var_r5     = var_r3;
        s32 var_r2 = arg0[var_r3];
        var_r3 += (var_r2 < 0) ? -var_r2 : var_r2;
    }

    if (var_r3 != bitmapIndex) {
        return;
    }

    s32 var_r2_2 = arg0[bitmapIndex];
    if (var_r2_2 > 0) {
        return;
    }

    if (var_r2_2 < 0) {
        var_r2_2 = -var_r2_2;
    }

    s32 temp_r4   = bitmapIndex;
    arg0[temp_r4] = var_r2_2;
    if (var_r5 != bitmapIndex) {
        s32 temp_lr = var_r5;
        s16 temp_ip = *(arg0 + temp_lr);
        if (temp_ip > 0) {
            bitmapIndex   = var_r5;
            arg0[temp_lr] = temp_ip + arg0[temp_r4];
            arg0[temp_r4] = 0;
        }
    }
    s32 temp_r2 = bitmapIndex + arg0[bitmapIndex];
    s16 temp_r1 = arg0[temp_r2];
    if (temp_r1 <= 0) {
        return;
    }
    arg0[bitmapIndex] += temp_r1;
    arg0[temp_r2] = 0;
}

static BgResource* BgResMgr_PopFree(BgResMgr* mgr) {
    BgResource* resource = mgr->freeList.next;

    if (resource != NULL) {
        mgr->freeList.next = resource->next;
    }
    return resource;
}

static void BgResMgr_PushActive(BgResMgr* mgr, BgResource* resource) {
    if (resource != NULL) {
        resource->next       = mgr->activeList.next;
        mgr->activeList.next = resource;
    }
}

static void BgResMgr_ResetResource(BgResMgr* mgr, BgResource* resource) {
    resource->flags        = DefaultFlags[mgr->engine];
    resource->refCount     = 0;
    resource->resourceType = RESTYPE_CHAR_SM;
    resource->unk_0A       = 0;
    resource->unk_0C       = 0;
    resource->bitmapIndex  = -1;
    resource->vramOffset   = 0;
    resource->size         = 0;
    resource->unk_1C       = NULL;
}

static BgResource* BgResMgr_RemoveActive(BgResMgr* mgr, BgResource* resource) {
    BgResource* current = NULL;

    if (resource != NULL && (resource->flags & 0x10) != 0) {
        current           = mgr->activeList.next;
        BgResource** prev = &mgr->activeList.next;

        while (current != NULL) {
            if (current == resource) {
                *prev = current->next;
                break;
            }
            prev    = &current->next;
            current = current->next;
        }
    }

    return current;
}

static void BgResMgr_RecycleResource(BgResMgr* mgr, BgResource* resource) {
    if (resource != NULL) {
        if ((resource->flags & 0x10) != 0) {
            resource->next     = mgr->freeList.next;
            mgr->freeList.next = resource;
        }
    }
}

void BgResMgr_Init(BgResMgr* mgr, DisplayEngine engine) {
    s32 var_r4;

    g_BgResourceManagers[engine] = mgr;
    if (mgr == NULL) {
        return;
    }
    mgr->engine          = engine;
    mgr->activeList.next = NULL;
    mgr->freeList.next   = NULL;

    for (u32 idx = 0; idx < ARRAY_COUNT(mgr->resourcePool); idx++) {
        BgResource* resource = &mgr->resourcePool[idx];

        resource->next     = mgr->freeList.next;
        mgr->freeList.next = resource;
    }

    if (engine == DISPLAY_MAIN) {
        s32 bank = GX_GetBankForBg();
        switch (bank) {
            case GX_VRAM_NONE:
                var_r4 = 0;
                break;
            case GX_VRAM_G:
            case GX_VRAM_F:
                var_r4 = 1;
                break;
            case GX_VRAM_FG:
                var_r4 = 2;
                break;
            case GX_VRAM_E:
                var_r4 = 4;
                break;
            case GX_VRAM_EG:
            case GX_VRAM_EF:
                var_r4 = 5;
                break;
            case GX_VRAM_EFG:
                var_r4 = 6;
                break;
            case GX_VRAM_A:
            case GX_VRAM_B:
            case GX_VRAM_C:
            case GX_VRAM_D:
                var_r4 = 7;
                break;
            case GX_VRAM_AB:
            case GX_VRAM_AC:
            case GX_VRAM_BC:
            case GX_VRAM_AD:
            case GX_VRAM_BD:
            case GX_VRAM_CD:
                var_r4 = 8;
                break;
            case GX_VRAM_ABC:
            case GX_VRAM_ABD:
            case GX_VRAM_ACD:
            case GX_VRAM_BCD:
                var_r4 = 9;
                break;
            case GX_VRAM_ABCD:
                var_r4 = 10;
                break;
            default:
                switch (bank & 0x70) {
                    case 0x10:
                        var_r4 = 0x13;
                        break;
                    case 0x70:
                        var_r4 = 0x1B;
                        break;
                    case 0x20:
                    case 0x40:
                        var_r4 = 0xB;
                        break;
                    case 0x60:
                        var_r4 = 0xF;
                        break;
                    case 0x50:
                    case 0x30:
                        var_r4 = 0x17;
                        break;
                }
                switch (bank & 0xF) {
                    default:
                        break;
                    case 3:
                    case 5:
                    case 6:
                    case 9:
                    case 10:
                    case 12:
                        var_r4 += 1;
                        break;
                    case 7:
                    case 11:
                    case 13:
                    case 14:
                        var_r4 += 2;
                        break;
                    case 15:
                        var_r4 += 3;
                        break;
                }
                break;
        }
        mgr->vramBase = 0x06000000;
        mgr->vramSize = GX_GetSizeOfBg();
    } else {
        s32 bank = GX_GetBankForSubBg();
        switch (bank) {
            case GX_VRAM_C:
                var_r4 = 7;
                break;
            case GX_VRAM_H:
                var_r4 = 2;
                break;
            case GX_VRAM_HI:
                var_r4 = 3;
                break;
            default:
                var_r4 = 0;
                break;
        }
        mgr->vramBase = 0x06200000;
        mgr->vramSize = GX_GetSizeOfSubBg();
    }
    func_0200927c(&mgr->unk_860, var_r4);
}

// Nonmatching: Register differences
// Scratch: ZhVLR
void* BgResMgr_AllocChar32(BgResMgr* arg0, void* arg1, s32 arg2, u32 arg3, u32 arg4) {
    if (arg0 == NULL) {
        return NULL;
    }
    arg4 = (arg4 + 0xFF) & ~0xFF;

    BgResource* temp_r0 = BgResMgr_PopFree(arg0);
    if (temp_r0 == NULL) {
        return NULL;
    }
    BgResMgr_ResetResource(arg0, temp_r0);

    s32 var_r5 = 0;
    if (arg0->engine == DISPLAY_MAIN) {
        var_r5 = g_DisplaySettings.controls[DISPLAY_MAIN].charBase << 0x10;
    }

    s32 temp_r5          = var_r5 + ((arg2 << 0xE) + (arg3 << 5));
    temp_r0->bitmapIndex = BgResMgr_AllocateVram(&arg0->unk_860, temp_r5, arg4);

    if (temp_r0->bitmapIndex < 0) {
        BgResMgr_RecycleResource(arg0, temp_r0);
        return NULL;
    }

    temp_r0->flags |= 0x14;
    temp_r0->refCount     = 0;
    temp_r0->resourceType = RESTYPE_CHAR_SM;
    temp_r0->vramOffset   = temp_r5;
    temp_r0->size         = arg4;
    BgResMgr_PushActive(arg0, temp_r0);
    BgResMgr_LoadCharToVram(arg0, temp_r0, arg1);
    return temp_r0;
}

// Nonmatching: Register differences
// Scratch: K9oVA
void* BgResMgr_AllocChar64(BgResMgr* arg0, void* arg1, s32 arg2, u32 arg3, u32 arg4) {
    if (arg0 == NULL) {
        return NULL;
    }
    arg4 = (arg4 + 0xFF) & ~0xFF;

    if (0x4000 < arg3 + arg4) {
        return NULL;
    }

    BgResource* temp_r0 = BgResMgr_PopFree(arg0);
    if (temp_r0 == NULL) {
        return NULL;
    }
    BgResMgr_ResetResource(arg0, temp_r0);

    s32 var_r5 = 0;
    if (arg0->engine == DISPLAY_MAIN) {
        var_r5 = g_DisplaySettings.controls[DISPLAY_MAIN].charBase << 0x10;
    }

    s32 temp_r5          = var_r5 + ((arg2 << 0xE) + (arg3 << 6));
    temp_r0->bitmapIndex = BgResMgr_AllocateVram(&arg0->unk_860, temp_r5, arg4);

    if (temp_r0->bitmapIndex < 0) {
        BgResMgr_RecycleResource(arg0, temp_r0);
        return NULL;
    }

    temp_r0->flags |= 0x14;
    temp_r0->refCount     = 0;
    temp_r0->resourceType = RESTYPE_CHAR_MD;
    temp_r0->vramOffset   = temp_r5;
    temp_r0->size         = arg4;
    BgResMgr_PushActive(arg0, temp_r0);
    BgResMgr_LoadCharToVram(arg0, temp_r0, arg1);
    return temp_r0;
}

// Nonmatching: Register differences
// Scratch: t3XHS
void* BgResMgr_AllocCharExtended(BgResMgr* arg0, void* arg1, s32 arg2, u32 arg3, u32 arg4) {
    if (arg0 == NULL) {
        return NULL;
    }
    arg4 = (arg4 + 0xFF) & ~0xFF;

    if (0x10000 < arg3 + arg4) {
        return NULL;
    }

    BgResource* temp_r0 = BgResMgr_PopFree(arg0);
    if (temp_r0 == NULL) {
        return NULL;
    }
    BgResMgr_ResetResource(arg0, temp_r0);

    s32 var_r5 = 0;
    if (arg0->engine == DISPLAY_MAIN) {
        var_r5 = g_DisplaySettings.controls[DISPLAY_MAIN].charBase << 0x10;
    }

    s32 temp_r5          = var_r5 + ((arg2 << 0xE) + (arg3 << 6));
    temp_r0->bitmapIndex = BgResMgr_AllocateVram(&arg0->unk_860, temp_r5, arg4);

    if (temp_r0->bitmapIndex < 0) {
        BgResMgr_RecycleResource(arg0, temp_r0);
        return NULL;
    }

    temp_r0->flags |= 0x14;
    temp_r0->refCount     = 0;
    temp_r0->resourceType = RESTYPE_CHAR_LG;
    temp_r0->vramOffset   = temp_r5;
    temp_r0->size         = arg4;
    BgResMgr_PushActive(arg0, temp_r0);
    BgResMgr_LoadCharToVram(arg0, temp_r0, arg1);
    return temp_r0;
}

// Nonmatching: Unwanted stack allocation, register differences
// Scratch: Vf480
BgResource* BgResMgr_AllocCharAligned(BgResMgr* mgr, UnkSmallStruct_DispBG* arg1, s32 arg2, u32 arg3, u32 arg4) {
    if (mgr == NULL) {
        return NULL;
    }
    arg4 = (arg4 + 0xFF) & ~0xFF;

    BgResource* resource = BgResMgr_PopFree(mgr);
    if (resource == NULL) {
        return NULL;
    }

    BgResMgr_ResetResource(mgr, resource);

    s32 var_r5 = 0;
    if (mgr->engine == DISPLAY_MAIN) {
        var_r5 = g_DisplaySettings.controls[DISPLAY_MAIN].charBase << 0x10;
    }

    s32 offset = var_r5 + (arg2 << 0xE);

    resource->bitmapIndex = BgResMgr_AllocateVramAligned(&mgr->unk_860, offset, arg4, 0x100, offset + 0x8000);

    if (resource->bitmapIndex < 0) {
        BgResMgr_RecycleResource(mgr, resource);
        return NULL;
    }

    resource->flags |= 0x14;
    resource->refCount     = 0;
    resource->resourceType = RESTYPE_CHAR_LG;
    resource->vramOffset   = offset;
    resource->size         = arg4;
    BgResMgr_PushActive(mgr, resource);
    BgResMgr_LoadCharToVram(mgr, resource, arg1);
    return resource;
}

BOOL BgResMgr_ReleaseChar(BgResMgr* mgr, BgResource* resource) {
    BOOL released = FALSE;

    if (mgr == NULL || resource == NULL) {
        return released;
    }

    if ((resource->flags & 0x10) == 0x10) {
        if (resource->refCount != 0) {
            resource->refCount--;
        }
        if (resource->refCount == 0) {
            BgResMgr_FreeVram(&mgr->unk_860, resource->bitmapIndex);
            BgResMgr_RemoveActive(mgr, resource);
            BgResMgr_RecycleResource(mgr, resource);
            released = TRUE;
        }
    }

    return released;
}

// Nonmatching: Missing instruction, minor register differences
// Scratch: 6yKTi
static void BgResMgr_LoadCharToVram(BgResMgr* mgr, BgResource* resource, UnkSmallStruct_DispBG* arg2) {
    s32 temp_r1;
    u32 temp_r1_2;

    if (mgr == NULL) {
        return;
    }
    if (arg2 != NULL) {
        u8  val    = arg2->unk_00;
        u32 var_r3 = (u32)(val & ~0xFF) >> 8;
        temp_r1    = (u8)val & 0xF0;
        if (temp_r1 == 0) {
            var_r3 -= 4;
        }
        temp_r1_2 = resource->size;
        if (temp_r1 == 0) {
            if (var_r3 > temp_r1_2) {
                var_r3 = temp_r1_2;
            }
            func_02001b44(7, mgr->vramBase + (resource->bitmapIndex << 8), &arg2->request, var_r3);
        } else if (var_r3 <= temp_r1_2) {
            func_02004d60(mgr->vramBase + (resource->bitmapIndex << 8), arg2);
        }
    }
    resource->unk_1C = arg2;
}

// Nonmatching: Register differences
// Scratch: MF4jF
BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, UnkSmallStruct_DispBG* arg1, s32 screenBase, GXBGScreenSizeText screenSize) {
    if (mgr == NULL) {
        return NULL;
    }

    s32 vramSize;
    switch (screenSize) {
        case GX_BG_SIZE_TEXT_256x256:
            vramSize = 0x800;
            break;
        case GX_BG_SIZE_TEXT_512x256:
        case GX_BG_SIZE_TEXT_256x512:
            vramSize = 0x1000;
            break;
        case GX_BG_SIZE_TEXT_512x512:
            vramSize = 0x2000;
            break;
    }

    if ((u32)(screenBase + ((u32)(vramSize + 0x7FF) >> 0xB)) > 0x20U) {
        return NULL;
    }

    BgResource* resource = BgResMgr_PopFree(mgr);
    if (resource == NULL) {
        return NULL;
    }
    BgResMgr_ResetResource(mgr, resource);

    s32 var_r6 = 0;
    if (mgr->engine == DISPLAY_MAIN) {
        var_r6 = g_DisplaySettings.controls[DISPLAY_MAIN].screenBase << 0x10;
    }
    s32 offset = var_r6 + (screenBase << 0xB);

    resource->bitmapIndex = BgResMgr_AllocateVram(&mgr->unk_860, offset, vramSize);
    if (resource->bitmapIndex < 0) {
        BgResMgr_RecycleResource(mgr, resource);
        return NULL;
    }
    resource->flags |= 0x15;
    resource->refCount     = 0;
    resource->resourceType = RESTYPE_CHAR_SM;
    resource->vramOffset   = offset;
    resource->size         = vramSize;
    BgResMgr_PushActive(mgr, resource);
    BgResMgr_LoadScreenToVram(mgr, resource, arg1);
    return resource;
}

BOOL BgResMgr_ReleaseScreen(BgResMgr* mgr, BgResource* resource) {
    BOOL released = FALSE;

    if (mgr == NULL || resource == NULL) {
        return released;
    }

    if ((resource->flags & 0x10) == 0x10) {
        if (resource->refCount != 0) {
            resource->refCount--;
        }
        if (resource->refCount == 0) {
            BgResMgr_FreeVram(&mgr->unk_860, resource->bitmapIndex);
            BgResMgr_RemoveActive(mgr, resource);
            BgResMgr_RecycleResource(mgr, resource);
            released = TRUE;
        }
    }

    return released;
}

// Nonmatching: Missing instruction, minor register differences
// Scratch: Y9tAB
static void BgResMgr_LoadScreenToVram(BgResMgr* mgr, BgResource* resource, UnkSmallStruct_DispBG* arg2) {
    if (mgr == NULL) {
        return;
    }

    if (arg2 != NULL) {
        u32 var_r3  = (u32)(arg2->unk_00 & ~0xFF) >> 8;
        u32 temp_r1 = (u8)arg2->unk_00 & 0xF0;
        if (temp_r1 == 0) {
            var_r3 -= 4;
        }

        if (temp_r1 == 0) {
            if (var_r3 > resource->size) {
                var_r3 = resource->size;
            }
            func_02001b44(7, mgr->vramBase + resource->vramOffset, &arg2->request, var_r3);
        } else if (var_r3 <= resource->size) {
            func_02004d60(mgr->vramBase + resource->vramOffset, arg2);
        }
    }
    resource->unk_1C = arg2;
}
