#include "Engine/Core/Memory.h"
#include "Field/FO3D.h"

typedef struct {
    void* (*allocate)(Heap*, s32);
    void (*free)(Heap*, void*);
    u32 parameters[9];
} FieldAllocatorCallbacks;

FieldAllocatorState gFOAllocator;

void* FOAllocator_Alloc(Heap* heap, s32 size);
void  FOAllocator_Free(Heap* heap, void* data);

const FieldAllocatorCallbacks FOAllocator_Callbacks = {
    FOAllocator_Alloc, FOAllocator_Free, {0x7C0003E0, 31, 16, 16, 32767, 0, 10, 0x7FFF0000, 5}
};

void* FOAllocator_Alloc(Heap* heap, s32 size) {
    void* data = Mem_AllocHeapTail(&gDebugHeap, size);
    Mem_SetSequence(&gDebugHeap, data, "FOAllocatorAlloc");
    return data;
}

void FOAllocator_Free(Heap* heap, void* data) {
    Mem_Free(&gDebugHeap, data);
}

void FOAllocator_Init(void) {
    gFOAllocator.callbacks = &FOAllocator_Callbacks;
    gFOAllocator.state0    = 0;
    gFOAllocator.state1    = 0;
    gFOAllocator.state2    = 0;
}

FieldAllocatorState* FOAllocator_Get(void) {
    return &gFOAllocator;
}

#define G3D_SIGNATURE_BMD0 0x30444D42
#define G3D_SIGNATURE_BTX0 0x30585442
#define G3D_SIGNATURE_BCA0 0x30414342
#define G3D_SIGNATURE_BVA0 0x30415642
#define G3D_SIGNATURE_BMA0 0x30414D42
#define G3D_SIGNATURE_BTP0 0x30505442
#define G3D_SIGNATURE_BTA0 0x30415442

extern NNSG3dResTex* func_0202bd5c(void* resource);
extern void*         func_0202bd4c(void* resource);
extern u32           func_02028458(NNSG3dResTex* tex);
extern u32           func_02028464(NNSG3dResTex* tex);
extern u32           func_0202853c(NNSG3dResTex* tex);
extern void          func_02028470(NNSG3dResTex* tex, u32 texKey, u32 tex4x4Key);
extern void          func_02028548(NNSG3dResTex* tex, u32 plttKey);
extern void          func_02028484(NNSG3dResTex* tex, BOOL exec);
extern void          func_02028550(NNSG3dResTex* tex, BOOL exec);
extern s32           func_02028880(void* mdlSet, NNSG3dResTex* tex);

extern u32 (*data_02065fcc)(u32 size, BOOL is4x4comp, u32 opt);
extern s32 (*data_02065fd0)(u32 key);
extern u32 (*data_02065fd4)(u32 size, BOOL is4pltt, u32 opt);
extern s32 (*data_02065fd8)(u32 key);

BOOL FO3D_LoadToVram(void* resource) {
    switch (*(u32*)resource) {
        case G3D_SIGNATURE_BMD0:
        case G3D_SIGNATURE_BTX0: {
            NNSG3dResTex* tex;
            u32           szTex;
            u32           szTex4x4;
            u32           szPltt;
            BOOL          sucTex    = TRUE;
            BOOL          sucTex4x4 = TRUE;
            BOOL          sucPltt   = TRUE;
            u32           keyTex;
            u32           keyTex4x4;
            u32           keyPltt;

            tex = func_0202bd5c(resource);
            if (tex != NULL) {
                szTex    = func_02028458(tex);
                szTex4x4 = func_02028464(tex);
                szPltt   = func_0202853c(tex);

                if (szTex > 0) {
                    keyTex = data_02065fcc(szTex, FALSE, 0);
                    if (keyTex == 0) {
                        sucTex = FALSE;
                    }
                } else {
                    keyTex = 0;
                }

                if (szTex4x4 > 0) {
                    keyTex4x4 = data_02065fcc(szTex4x4, TRUE, 0);
                    if (keyTex4x4 == 0) {
                        sucTex4x4 = FALSE;
                    }
                } else {
                    keyTex4x4 = 0;
                }

                if (szPltt > 0) {
                    keyPltt = data_02065fd4(szPltt, tex->tex4x4Flag & 0x8000, 0);
                    if (keyPltt == 0) {
                        sucPltt = FALSE;
                    }
                } else {
                    keyPltt = 0;
                }

                if (!sucTex || !sucTex4x4 || !sucPltt) {
                    if (szPltt > 0) {
                        data_02065fd8(keyPltt);
                    }
                    if (szTex4x4 > 0) {
                        data_02065fd0(keyTex4x4);
                    }
                    if (szTex > 0) {
                        data_02065fd0(keyTex);
                    }
                    return FALSE;
                }

                func_02028470(tex, keyTex, keyTex4x4);
                func_02028548(tex, keyPltt);
                func_02028484(tex, TRUE);
                func_02028550(tex, TRUE);
            }

            if (*(u32*)resource == G3D_SIGNATURE_BMD0) {
                void* mdlSet = func_0202bd4c(resource);
                if (tex != NULL) {
                    func_02028880(mdlSet, tex);
                }
            }
        }
            return TRUE;
        case G3D_SIGNATURE_BCA0:
        case G3D_SIGNATURE_BVA0:
        case G3D_SIGNATURE_BMA0:
        case G3D_SIGNATURE_BTP0:
        case G3D_SIGNATURE_BTA0:
            return TRUE;
        default:
            return FALSE;
    }
}
