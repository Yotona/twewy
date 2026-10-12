#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/DMA.h"
#include "Engine/Core/Interrupts.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "common_data.h"
#include <nitro/gx.h>

typedef struct {
    void** items;
    s32    capacity;
} FieldPtrList;

extern FieldPtrList* data_ov030_02109de4;
extern void*         func_ov030_020d2488(s32 size);
extern void          func_ov030_020d24bc(void* data);

void* func_ov030_020d24d4(void) {
    data_ov030_02109de4           = func_ov030_020d2488(sizeof(*data_ov030_02109de4));
    data_ov030_02109de4->items    = NULL;
    data_ov030_02109de4->capacity = 0;
    return data_ov030_02109de4;
}

void func_ov030_020d2504(void) {
    func_ov030_020d24bc(data_ov030_02109de4->items);
    func_ov030_020d24bc(data_ov030_02109de4);
    data_ov030_02109de4 = NULL;
}

void func_ov030_020d2538(FieldPtrList* list) {
    data_ov030_02109de4 = list;
}

FieldPtrList* func_ov030_020d2548(void) {
    return data_ov030_02109de4;
}

void func_ov030_020d258c(void);
// Nonmatching: objdiff reports 99.615% only because of the intra-TU forward bl relocation
void func_ov030_020d2558(s32 capacity) {
    func_ov030_020d24d4();
    data_ov030_02109de4->items    = func_ov030_020d2488(capacity * sizeof(*data_ov030_02109de4->items));
    data_ov030_02109de4->capacity = capacity;
    func_ov030_020d258c();
}

void func_ov030_020d258c(void) {
    s32 i;

    if (data_ov030_02109de4 == NULL) {
        return;
    }
    i = data_ov030_02109de4->capacity;
    while (i > 0) {
        data_ov030_02109de4->items[--i] = NULL;
    }
}

s32 func_ov030_020d25cc(void* item) {
    s32           result = -1;
    s32           i      = 0;
    FieldPtrList* list   = data_ov030_02109de4;

    if (list->capacity > 0) {
        do {
            if (list->items[i] == NULL) {
                result         = i;
                list->items[i] = item;
                break;
            }
            i++;
        } while (i < list->capacity);
    }
    return result;
}

extern void func_ov030_020d2620(void);

void func_ov030_020d2930(void) {
    if (SystemStatusFlags.vblank == FALSE) {
        return;
    }
    Display_Commit();
    DMA_Flush();
    DC_PurgeRange(g_OamMgr[DISPLAY_MAIN].oam, 0x400);
    GX_LoadOam(g_OamMgr[DISPLAY_MAIN].oam, 0, 0x400);
    DC_PurgeRange(g_OamMgr[DISPLAY_SUB].oam, 0x400);
    GXs_LoadOam(g_OamMgr[DISPLAY_SUB].oam, 0, 0x400);
    DC_PurgeRange(&data_02066aec, 0x400);
    GX_LoadBgPltt(&data_02066aec, 0, 0x200);
    GX_LoadObjPltt(&data_02066cec, 0, 0x200);
    DC_PurgeRange(&data_02066eec, 0x400);
    GXs_LoadBgPltt(&data_02066eec, 0, 0x200);
    GXs_LoadObjPltt(&data_020670ec, 0, 0x200);
}

void func_ov030_020d2a00(void) {
    func_ov030_020d2620();
    Interrupts_RegisterVBlankCallback(func_ov030_020d2930, 1);
}

void func_ov030_020d2a1c(void) {
    Interrupts_RegisterVBlankCallback(NULL, 1);
}

void func_ov030_020d2a30(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x800);
    if (EasyFade_IsFading()) {
        return;
    }
    DebugOvlDisp_Pop();
}

void func_ov030_020d2a58(void) {
    EasyFade_FadeBothDisplays(FADER_LINEAR, -0x20, 0x800);
    if (EasyFade_IsFading()) {
        return;
    }
    DebugOvlDisp_Pop();
}
