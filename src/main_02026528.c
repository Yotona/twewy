#include "Engine/Core/Memory.h"
#include "Engine/Resources/ObjResMgr.h"

typedef struct {
    /* 0x00 */ s32           count;
    /* 0x04 */ s32           unk_04;
    /* 0x08 */ s32           totalSize;
    /* 0x0C */ ObjResource** resources;
} NNSTexVramPool; // Size: 0x10

extern u32 (*data_02065fcc)(u32 size, BOOL is4x4comp, u32 opt);
extern s32 (*data_02065fd0)(u32 key);

s32  func_02026528(s32 angle, s32 target);
void func_02026564(u16* angle, u16 target, s32 shift);
void func_02026590(s32* value, s32 target, u16 frames);
u16  func_020265c0(s32 scale);
void func_020265d4(s32* value, s32 target, u16 scale);
void func_02026608(s32* value, s32 target, u16 duration, s32 minStep);
s32  NNSTexVram_FindFreeSlot(void);
u32  NNSTexVram_Alloc(u32 size, BOOL is4x4comp, u32 opt);
s32  NNSTexVram_Free(u32 key);
void NNSTexVram_Init(s32 count);
void NNSTexVram_Shutdown(void);

#define NNS_TEXKEY(addr, size, is4x4) ((u16)((u32)(addr) >> 3) | (((u32)(size) >> 4) << 16) | ((is4x4) << 31))

static NNSTexVramPool sTexVramPool;

s32 func_02026528(s32 angle, s32 target) {
    u16 a    = angle;
    u16 b    = target;
    s32 diff = a - b;

    if (diff <= -0x8000) {
        return a + 0x10000 - b;
    }
    if (diff >= 0x8000) {
        return a - (b + 0x10000);
    }
    return diff;
}

void func_02026564(u16* angle, u16 target, s32 shift) {
    s32 diff = func_02026528(target, *angle);

    if (diff != 0) {
        *angle += diff >> shift;
    }
}

void func_02026590(s32* value, s32 target, u16 frames) {
    if (frames < 1) {
        frames = 1;
    }
    *value += (target - *value) / frames;
}

u16 func_020265c0(s32 scale) {
    u16 half = scale >> 1;

    if (half < 1) {
        half = 1;
    }
    return half;
}

void func_020265d4(s32* value, s32 target, u16 scale) {
    s32 diff = target - *value;

    *value += diff / func_020265c0(scale);
}

void func_02026608(s32* value, s32 target, u16 duration, s32 minStep) {
    s32 diff = target - *value;
    s32 step = diff / func_020265c0(duration);
    s32 next = (*value + step) / (duration - 1);

    if (next < 0) {
        next = -next;
    }
    if (next < minStep) {
        step = diff / duration;
    }
    *value += step;
}

s32 NNSTexVram_FindFreeSlot(void) {
    s32 i;

    for (i = 0; i < sTexVramPool.count; i++) {
        if (sTexVramPool.resources[i] == NULL) {
            return i;
        }
    }
    return i;
}

// Nonmatching: r2/r3 swapped in the size update
u32 NNSTexVram_Alloc(u32 size, BOOL is4x4comp, u32 opt) {
    ObjResource* res;
    s32          slot = NNSTexVram_FindFreeSlot();

    if (is4x4comp) {
        return 0;
    }
    if (slot == sTexVramPool.count) {
        return 0;
    }

    sTexVramPool.resources[slot] = ObjResMgr_AllocResource(g_ObjResourceManagers[2], NULL, (size + 0x1F) & ~0x1F, 0);
    sTexVramPool.totalSize       = sTexVramPool.totalSize + sTexVramPool.resources[slot]->vramOffset;
    res                          = sTexVramPool.resources[slot];
    if (res == NULL) {
        return 0;
    }
    return (((u32)res->vramOffset >> 4) << 16) | (u16)(res->bitmapIndex << 2) | (is4x4comp << 31);
}

s32 NNSTexVram_Free(u32 key) {
    s32 index = (key & 0xFFFF) >> 2;
    s32 i;

    if (key == 0) {
        return 0;
    }
    for (i = 0; i < sTexVramPool.count; i++) {
        if (sTexVramPool.resources[i] != NULL && index == sTexVramPool.resources[i]->bitmapIndex) {
            ObjResMgr_ReleaseResource(g_ObjResourceManagers[2], sTexVramPool.resources[i]);
            sTexVramPool.resources[i] = NULL;
            return 0;
        }
    }
    return 1;
}

void NNSTexVram_Init(s32 count) {
    ObjResource** resources;
    s32           i;

    sTexVramPool.count = count;
    resources          = Mem_AllocHeapTail(&gDebugHeap, count * 32);
    Mem_SetSequence(&gDebugHeap, resources, "SprRscTexForNNS");
    sTexVramPool.resources = resources;
    for (i = 0; i < sTexVramPool.count; i++) {
        sTexVramPool.resources[i] = NULL;
    }
    sTexVramPool.unk_04    = 0;
    data_02065fcc          = NNSTexVram_Alloc;
    data_02065fd0          = NNSTexVram_Free;
    sTexVramPool.totalSize = 0;
}

void NNSTexVram_Shutdown(void) {
    s32 i;

    for (i = 0; i < sTexVramPool.count; i++) {
        if (sTexVramPool.resources[i] != NULL) {
            ObjResMgr_ReleaseResource(g_ObjResourceManagers[2], sTexVramPool.resources[i]);
        }
    }
    Mem_Free(&gDebugHeap, sTexVramPool.resources);
}
