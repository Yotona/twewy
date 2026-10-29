#include "Field/FieldRegion.h"

#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Overlay/OverlayManager.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Field/Flash/FO3DRes.h"
#include "Field/Flash/FOAllocator.h"
#include "Field/Scene/FieldSceneLoad.h"
#include "Field/Scene/FieldSceneRender.h"
#include "Field/Tasks/Fld_Human.h"
#include "Save.h"
#include "common_data.h"

#include <nitro/fx/fx_matrix.h>
#include <nitro/g3.h>

extern void TexVram_Init(s32 arg0);
extern void TexVram_Shutdown(void);
extern void func_02027b0c(s32 arg0, s32 arg1);
extern void func_02028a44(void);
extern void func_02028ad0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void func_02028b0c(s32 arg0, s32 arg1);

extern void                 func_020281c0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
extern void                 func_02028250(void* arg0);
extern void                 func_020283c4(void* arg0, s32 arg1);
extern void                 func_02028a80(s32* arg0);
extern void                 func_02028aa8(void* arg0);
extern void                 func_02029270(void* arg0);
extern void                 func_0202b878(void);
extern void                 func_0202b8a0(s32 arg0);
extern s32                  func_0202bac0(s32 arg0, s32 arg1, s32 arg2);
extern void                 func_0202bae4(s32 arg0, s32 arg1);
extern struct G3dResMdlSet* func_0202bd4c(s32 arg0);
extern s32                  func_0202bd5c(s32 arg0);
extern s32                  func_0202bd9c(s32 arg0, s32 arg1);
extern void                 G3X_ResetMtxStack(void);

// NitroSystem G3D resource dictionary (NNSG3dResDict) heading a model set.
typedef struct {
    /* 0x0 */ u8  revision;
    /* 0x1 */ u8  numEntry;
    /* 0x2 */ u16 sizeDictBlk;
    /* 0x4 */ u16 unk_04;
    /* 0x6 */ u16 ofsEntry;
} G3dResDict;

typedef struct {
    /* 0x0 */ u16 sizeUnit;
    /* 0x2 */ u16 ofsName;
    /* 0x4 */ u8  data[];
} G3dResDictEntryHeader;

typedef struct G3dResMdlSet {
    /* 0x0 */ u8         header[8];
    /* 0x8 */ G3dResDict dict;
} G3dResMdlSet;

// NNS_G3dGetMdlByIdx: entry idx of the dictionary holds the model's offset from the set.
static inline void* G3dResMdlSet_GetModel(G3dResMdlSet* set, s32 idx) {
    G3dResDictEntryHeader* hdr = (G3dResDictEntryHeader*)((u8*)&set->dict + set->dict.ofsEntry);

    return (u8*)set + *(u32*)&hdr->data[hdr->sizeUnit * idx];
}

// Render transform object (angles + fixed-point matrix).
typedef struct {
    /* 0x00 */ s32 unk00;
    /* 0x04 */ s32 unk04;
    /* 0x08 */ s32 unk08;
    /* 0x0C */ s32 unk0C;
    /* 0x10 */ s32 unk10;
    /* 0x14 */ s32 unk14;
    /* 0x18 */ s32 unk18;
    /* 0x1C */ s32 unk1C;
    /* 0x20 */ s32 unk20;
    /* 0x24 */ s32 unk24;
    /* 0x28 */ s32 unk28;
    /* 0x2C */ s32 unk2C;
    /* 0x30 */ s32 unk30;
    /* 0x34 */ s32 unk34;
    /* 0x38 */ u8  pad38[0x68 - 0x38];
    /* 0x68 */ s32 unk68;
    /* 0x6C */ s16 unk6C;
    /* 0x6E */ s16 unk6E;
    /* 0x70 */ s16 unk70;
    /* 0x72 */ u8  pad72[0x78 - 0x72];
} Ov030RenderObj; // Size: 0x78

// Field render sub-object: a 16-bit control word block plus a scale field.
typedef struct {
    /* 0x00 */ u16 unk0;
    /* 0x02 */ u16 unk2;
    /* 0x04 */ u16 unk4;
    /* 0x06 */ u16 unk6;
    /* 0x08 */ s32 unk8;
} Ov030B5Sub; // Size: 0xC

// The frame-count table an Ov030B5Anim attaches to; the animation's 12.4
// fixed-point position wraps at frameCount frames.
typedef struct {
    /* 0x00 */ u8  pad00[4];
    /* 0x04 */ u16 frameCount;
} Ov030B5AnimData;

// One frame of a loaded sprite animation: the current frame offset and the
// animation's data table.
typedef struct Ov030B5Anim {
    /* 0x0 */ s32              unk0;
    /* 0x4 */ u8               pad04[4];
    /* 0x8 */ Ov030B5AnimData* unk8;
} Ov030B5Anim;

// One render layer of the field scene: an engine-side record holding one
// attached sprite animation.
typedef struct {
    /* 0x00 */ u8 pad00[0x54];
} Ov030B5SpriteLayer; // Size: 0x54

// One field-data record: 5 pack identifiers, then two fixed-point camera offsets.
typedef struct {
    /* 0x00 */ BinIdentifier binIdens[5];
    /* 0x28 */ s32           unk28;
    /* 0x2C */ s32           unk2C;
} Ov030FieldRec; // Size: 0x30

// Field task root: resource managers, four 64-entry sprite pointer arrays, the
// five loaded pack identifiers, the render transform and the camera block.
typedef struct Ov030B5C58Root {
    /* 0x00000 */ ResourceManager    resources;
    /* 0x11580 */ ResourceManager*   prevResources;
    /* 0x11584 */ u8                 pad11584[0x11588 - 0x11584];
    /* 0x11588 */ Ov030B5SpriteLayer unk11588[64];
    /* 0x12A88 */ void*              unk12A88[64];
    /* 0x12B88 */ void*              unk12B88[64];
    /* 0x12C88 */ Ov030B5Anim*       unk12C88[64];
    /* 0x12D88 */ void*              unk12D88[64];
    /* 0x12E88 */ Ov030B5Anim*       unk12E88[64];
    /* 0x12F88 */ void*              unk12F88[64];
    /* 0x13088 */ Ov030B5Anim*       unk13088[64];
    /* 0x13188 */ void*              unk13188[64];
    /* 0x13288 */ Ov030B5Anim*       unk13288[64];
    /* 0x13388 */ u8                 pad13388[0x13488 - 0x13388];
    /* 0x13488 */ Data*              unk13488[5];
    /* 0x1349C */ Ov030RenderObj     unk1349C;
    /* 0x13514 */ Ov030B5Sub         unk13514;
    /* 0x13520 */ s32                unk13520;
    /* 0x13524 */ s32                unk13524;
    /* 0x13528 */ s32                unk13528;
    /* 0x1352C */ s32                unk1352C;
} Ov030B5C58Root;

extern Vec data_ov030_020e0940;
extern void (*data_ov030_020e094c[])(void*);
extern char*          data_ov030_02106558;
extern Ov030FieldRec* data_ov030_0210655c[];

extern void func_ov030_020b5dc4(Ov030RenderObj* arg0);
extern void func_ov030_020b6534(Ov030B5Sub* arg0, s32 arg1);

s32 func_ov030_020b5c58(FldHuman* human) {
    TouchCoord coord;
    s32        limitX;
    s32        lowY;
    s32        y;
    s32        x;

    x      = human->screen.x;
    y      = human->screen.y;
    lowY   = (y - ((s32)(((s64)human->margin->y * human->scale + 0x800) >> 0xC) + 0x18000)) >> 0xC;
    limitX = x + 0x10000;
    TouchInput_GetCoord(&coord);
    if (coord.x > (s32)((x - 0x10000) >> 0xC) && coord.x < (s32)(limitX >> 0xC) && lowY < coord.y && coord.y < (s32)(y >> 0xC))
    {
        return 1;
    }
    return 0;
}

s16 func_ov030_020b5cd8(Task* task) {
    return ((FldHuman*)task->data)->init.tpl.unk_0C;
}

s32 func_ov030_020b5ce4(Task* task) {
    FldHuman* human = task->data;

    if (human->unk_88 == -1) {
        return 0;
    }
    return func_ov030_020b5c58(human);
}

void func_ov030_020b5d08(Task* task, s32 arg1) {
    ((FldHuman*)task->data)->unk_84 = arg1;
}

void func_ov030_020b5d14(Task* task, s32 arg1, s16 arg2, s32 arg3) {
    FldHuman* human;

    if (task == NULL) {
        return;
    }
    human                  = task->data;
    human->init.tpl.unk_0C = 0;
    human->init.tpl.unk_14 = 1 << arg1;
    human->init.tpl.unk_10 = (s16)arg3;
    human->init.tpl.unk_0E = arg2;
}

s32 func_ov030_020b5d4c(Task* task) {
    return ((FldHuman*)task->data)->pos.y;
}

s32 func_ov030_020b5d58(Task* task) {
    return ((FldHuman*)task->data)->init.tpl.unk_14;
}

void func_ov030_020b5d64(Ov030RenderObj* arg0, s32 arg1, s32 arg2) {
    arg0->unk00 = 0x800;
    arg0->unk04 = 0xDDB;
    arg0->unk08 = 0x1555;
    arg0->unk0C = 0x1000;
    arg0->unk10 = 0x190000;
    arg0->unk20 = 0;
    arg0->unk24 = arg1;
    arg0->unk28 = 0;
    arg0->unk6C = 0x1400;
    arg0->unk6E = 0;
    arg0->unk70 = 0;
    arg0->unk68 = arg2;
    func_ov030_020b5dc4(arg0);
}

// Nonmatching: pure register allocation / instruction scheduling -- logic and every constant
// match (the (s64)prod>>0xC tail is identical); retail keeps r4-r8 live across the whole body so
// it holds the products in registers, while ours reuses temporaries and reloads. Hoisting the
// s64 products into held locals produced byte-identical output, so the frame-size split is a
// backend heuristic, not a source-shape issue (USA 55.28%; JP unvalidated).
void func_ov030_020b5dc4(Ov030RenderObj* arg0) {
    u16 a = arg0->unk6C;
    u16 b = arg0->unk6E;
    u16 c = arg0->unk70;

    arg0->unk14 = (FX_SinCosTable_[(a >> 4) * 2 + 1] * arg0->unk68) >> 0xC;
    arg0->unk18 = (FX_SinCosTable_[(a >> 4) * 2] * arg0->unk68) >> 0xC;
    arg0->unk1C = (FX_SinCosTable_[(a >> 4) * 2 + 1] * arg0->unk68) >> 0xC;
    arg0->unk14 = (arg0->unk14 * FX_SinCosTable_[(b >> 4) * 2] + arg0->unk20) >> 0xC;
    arg0->unk18 = arg0->unk18 + arg0->unk24;
    arg0->unk1C = (arg0->unk1C * FX_SinCosTable_[(b >> 4) * 2 + 1] + arg0->unk28) >> 0xC;
    arg0->unk2C = (s32)(((s64)FX_SinCosTable_[(c >> 4) * 2] * FX_SinCosTable_[(b >> 4) * 2 + 1] + 0x800) >> 0xC);
    arg0->unk30 = FX_SinCosTable_[(c >> 4) * 2 + 1];
    arg0->unk34 = -(s32)(((s64)FX_SinCosTable_[(c >> 4) * 2] * FX_SinCosTable_[(b >> 4) * 2] + 0x800) >> 0xC);
}

void func_ov030_020b5ed4(Ov030RenderObj* arg0) {
    if (SysControl.buttonState.holdButtons & 0x40) {
        arg0->unk6C = arg0->unk6C + 0x100;
        if ((s32)arg0->unk6C > 0x3F00) {
            arg0->unk6C = 0x3F00;
        }
    }
    if (SysControl.buttonState.holdButtons & 0x80) {
        arg0->unk6C = arg0->unk6C - 0x100;
        if ((s32)arg0->unk6C < -0x3F00) {
            arg0->unk6C = -0x3F00;
        }
    }
    if (SysControl.buttonState.holdButtons & 0x20) {
        arg0->unk6E = arg0->unk6E - 0x100;
    }
    if (SysControl.buttonState.holdButtons & 0x10) {
        arg0->unk6E = arg0->unk6E + 0x100;
    }
    if (SysControl.buttonState.holdButtons & 0x200) {
        arg0->unk68 = arg0->unk68 + 0x100;
        if (arg0->unk68 > 0x96000) {
            arg0->unk68 = 0x96000;
        }
    }
    if (SysControl.buttonState.holdButtons & 0x100) {
        arg0->unk68 = arg0->unk68 - 0x100;
        if (arg0->unk68 < 0x4000) {
            arg0->unk68 = 0x4000;
        }
    }
    if (SysControl.buttonState.holdButtons & 0x800) {
        arg0->unk24 = arg0->unk24 - 0x100;
    }
    if (SysControl.buttonState.holdButtons & 0x400) {
        arg0->unk24 = arg0->unk24 + 0x100;
    }
}

const char* func_ov030_020b5fe8(void) {
    // The model file name without its 8-character directory prefix.
    return &data_ov030_0210655c[gSaveData.unk_26E0]->binIdens[0].path[8];
}

s32 func_ov030_020b600c(void) {
    return 0x14;
}

// Nonmatching: DISPCNT bit-op setup now matches via ~0x3000 and the unk11588 fix, but the
// Data-load constant setup (r4=0,r5=0,r7=1) and the predicated addeq/streq then-branch still
// differ by register allocation (USA 97.51%; JP unvalidated).
void func_ov030_020b6014(Ov030B5C58Root* arg0) {
    Ov030FieldRec*      rec;
    s32                 i;
    s32                 j;
    Ov030B5SpriteLayer* elem;
    u16                 stride;

    func_0202b878();
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3002;
    REG_DISP3DCNT = REG_DISP3DCNT & (u16)~0x3004;
    REG_DISP3DCNT = REG_DISP3DCNT & ~0x3000 | 0x08;
    REG_DISP3DCNT = REG_DISP3DCNT & (u16)~0x3010;
    REG_DISP3DCNT = REG_DISP3DCNT & (u16)~0x3020;
    G3X_SetClearColor(0, 0, 0x7FFF, 0, 0);
    REG_GFX_FIFO_VIEWPORT = 0xBFFF0000;
    func_02028ad0(0, 0, 0, 0xFFFFF001);
    func_02028b0c(0, 0x7FFF);
    Mtx_OrthoW(0, -0xC0000, 0, 0x100000, -0x800000, 0x800000, 0x800000, &data_02076a4c);
    data_02076a44.unk_FC = data_02076a44.unk_FC & ~0x50;
    TexVram_Init(0x80);
    func_02027b0c(0x4000, 1);
    if ((u32)gSaveData.unk_26E0 >= 0x14) {
        gSaveData.unk_26E0 = 0;
    }
    rec = data_ov030_0210655c[gSaveData.unk_26E0];
    FOAllocator_Init();
    i = 0;
    do {
        if (rec->binIdens[i].path == NULL) {
            arg0->unk13488[i] = NULL;
        } else {
            arg0->unk13488[i] = DatMgr_LoadRawData(1, NULL, 0, &rec->binIdens[i]);
        }
        i++;
    } while (i < 5);
    arg0->unk1352C = FO3DRes_GetModelCount((s32)arg0->unk13488[0]->buffer);
    stride         = func_0202bd5c((s32)arg0->unk13488[0]->buffer);
    func_0202b8a0((s32)arg0->unk13488[0]->buffer);
    j = 0;
    if (arg0->unk1352C > 0) {
        elem = arg0->unk11588;
        do {
            arg0->unk12A88[j] = G3dResMdlSet_GetModel(func_0202bd4c((s32)arg0->unk13488[0]->buffer), j);
            func_02028250(elem);
            arg0->unk12B88[j] = func_0202bd9c((s32)arg0->unk13488[1]->buffer, j);
            arg0->unk12C88[j] = (Ov030B5Anim*)func_0202bac0(FOAllocator_Get(), arg0->unk12B88[j], (s32)arg0->unk12A88[j]);
            func_020281c0((s32)arg0->unk12C88[j], arg0->unk12B88[j], (s32)arg0->unk12A88[j], 0);
            func_020283c4(elem, (s32)arg0->unk12C88[j]);
            if (arg0->unk13488[2] != NULL) {
                arg0->unk12D88[j] = func_0202bd9c((s32)arg0->unk13488[2]->buffer, j);
                arg0->unk12E88[j] = (Ov030B5Anim*)func_0202bac0(FOAllocator_Get(), arg0->unk12D88[j], (s32)arg0->unk12A88[j]);
                func_020281c0((s32)arg0->unk12E88[j], arg0->unk12D88[j], (s32)arg0->unk12A88[j], 0);
                func_020283c4(elem, (s32)arg0->unk12E88[j]);
            }
            if (arg0->unk13488[3] != NULL) {
                arg0->unk12F88[j] = func_0202bd9c((s32)arg0->unk13488[3]->buffer, j);
                arg0->unk13088[j] = (Ov030B5Anim*)func_0202bac0(FOAllocator_Get(), arg0->unk12F88[j], (s32)arg0->unk12A88[j]);
                func_020281c0((s32)arg0->unk13088[j], arg0->unk12F88[j], (s32)arg0->unk12A88[j], 0);
                func_020283c4(elem, (s32)arg0->unk13088[j]);
            }
            if (arg0->unk13488[4] != NULL) {
                arg0->unk13188[j] = func_0202bd9c((s32)arg0->unk13488[4]->buffer, j);
                arg0->unk13288[j] = (Ov030B5Anim*)func_0202bac0(FOAllocator_Get(), arg0->unk13188[j], (s32)arg0->unk12A88[j]);
                func_020281c0((s32)arg0->unk13288[j], arg0->unk13188[j], (s32)arg0->unk12A88[j], stride);
                func_020283c4(elem, (s32)arg0->unk13288[j]);
            }
            j++;
            arg0->unk13520 = rec->unk28 << 0xC;
            elem           = elem + 1;
            arg0->unk13524 = rec->unk2C << 0xC;
        } while (j < arg0->unk1352C);
    }
    func_ov030_020b5d64(&arg0->unk1349C, 0xA000, 0x10000);
    func_ov030_020b6534(&arg0->unk13514, 0x1800);
}

void func_ov030_020b6464(Ov030B5C58Root* arg0) {
    s32   i;
    Data* data;

    i = 0;
    if (arg0->unk1352C > 0) {
        do {
            func_0202bae4(FOAllocator_Get(), (s32)arg0->unk12C88[i]);
            if (arg0->unk13488[2] != NULL) {
                func_0202bae4(FOAllocator_Get(), (s32)arg0->unk12E88[i]);
            }
            if (arg0->unk13488[3] != NULL) {
                func_0202bae4(FOAllocator_Get(), (s32)arg0->unk13088[i]);
            }
            if (arg0->unk13488[4] != NULL) {
                func_0202bae4(FOAllocator_Get(), (s32)arg0->unk13288[i]);
            }
            i++;
        } while (i < arg0->unk1352C);
    }
    TexVram_Shutdown();
    for (i = 0; i < 5; i++) {
        data = arg0->unk13488[i];
        if (data != NULL) {
            DatMgr_ReleaseData(data);
        }
    }
}

void func_ov030_020b6534(Ov030B5Sub* arg0, s32 arg1) {
    arg0->unk0 = 1;
    arg0->unk2 = 0x7FFF;
    arg0->unk4 = 0x7D40;
    arg0->unk6 = 0x1F;
    arg0->unk8 = arg1;
}

void func_ov030_020b6560(Ov030B5C58Root* arg0) {
    OverlayTag tag;

    if (SysControl.buttonState.pressedButtons & 4) {
        MainOvlDisp_ReplaceTop(&tag, (s32)&OVERLAY_30_ID, (void*)&func_ov030_020b6dec, NULL, 0);
    }
    func_ov030_020b5ed4(&arg0->unk1349C);
    func_ov030_020b5dc4(&arg0->unk1349C);
}

// Nonmatching: display-settings store scheduling / register allocation (retail materializes
// 0x7FFF into r12 early and keeps a distinct load/store order); statement order tuned to the best
// observed 89.70% but the two s16 stores + pool ordering still diverge (USA 89.95%; JP unvalidated).
void func_ov030_020b65c0(void) {
    Ov030B5C58Root* root;
    char*           sequence;

    sequence = data_ov030_02106558;
    root     = Mem_AllocHeapTail(&gDebugHeap, sizeof(Ov030B5C58Root));
    Mem_SetSequence(&gDebugHeap, root, sequence);
    MainOvlDisp_SetCbArg(root);
    func_ov030_020b7400();
    root->prevResources                      = ResourceMgr_ReinitManagers(&root->resources);
    g_PaletteBuffers[DISPLAY_SUB].bg[0]      = 0;
    g_PaletteBuffers[DISPLAY_MAIN].bg[0]     = 0x7FFF;
    g_DisplaySettings.controls[0].brightness = 0;
    g_DisplaySettings.controls[1].brightness = 0;

    root->unk13520 = 0;
    root->unk13524 = 0;
    root->unk13528 = 0x1000;
    func_ov030_020b6014(root);
    MainOvlDisp_NextProcessStage();
}

void func_ov030_020b666c(Ov030B5C58Root* arg0) {
    if (SysControl.buttonState.holdButtons & 0x40) {
        arg0->unk13524 = arg0->unk13524 - 0x4000;
    } else if (SysControl.buttonState.holdButtons & 0x80) {
        arg0->unk13524 = arg0->unk13524 + 0x4000;
    }
    if (SysControl.buttonState.holdButtons & 0x20) {
        arg0->unk13520 = arg0->unk13520 - 0x4000;
    } else if (SysControl.buttonState.holdButtons & 0x10) {
        arg0->unk13520 = arg0->unk13520 + 0x4000;
    }
    if (SysControl.buttonState.holdButtons & 0x200) {
        arg0->unk13528 = arg0->unk13528 - 0x4000;
        return;
    }
    if (!(SysControl.buttonState.holdButtons & 0x100)) {
        return;
    }
    arg0->unk13528 = arg0->unk13528 + 0x4000;
}

void func_ov030_020b6728(Ov030B5C58Root* arg0) {
    Vec                 mtx;
    s32                 trans[3];
    s32                 i;
    s32                 delta;
    Ov030B5SpriteLayer* elem;
    Ov030B5Anim*        anim;
    Ov030B5Anim*        anim2;
    Ov030B5Anim*        anim3;
    Ov030B5Anim*        anim4;

    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    OamMgr_Reset(&g_OamMgr[1], 0, 0);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    OamMgr_ResetCommandQueues(&g_OamMgr[1]);
    func_ov030_020b6560(arg0);
    G3X_Reset();
    G3X_ResetMtxStack();
    func_ov030_020b666c(arg0);
    func_02028a44();
    mtx = data_ov030_020e0940;
    func_02028aa8(&mtx);
    trans[0] = -arg0->unk13520;
    trans[1] = arg0->unk13524;
    trans[2] = 0;
    func_02028a80(trans);
    func_02028a44();
    i = 0;
    if (arg0->unk1352C > 0) {
        elem = arg0->unk11588;
        do {
            delta = 0x800;
            if (SysControl.buttonState.currButtons & 2) {
                delta = -delta;
            } else if (SysControl.buttonState.currButtons & 0x400) {
                delta = delta * 10;
            }
            anim       = arg0->unk12C88[i];
            anim->unk0 = anim->unk0 + delta;
            anim       = arg0->unk12C88[i];
            if (anim->unk0 >= (s32)(anim->unk8->frameCount << 0xC)) {
                anim->unk0 = 0;
            }
            if (arg0->unk13488[2] != NULL) {
                anim2       = arg0->unk12E88[i];
                anim2->unk0 = anim2->unk0 + delta;
                anim2       = arg0->unk12E88[i];
                if (anim2->unk0 >= (s32)(anim2->unk8->frameCount << 0xC)) {
                    anim2->unk0 = 0;
                }
            }
            if (arg0->unk13488[3] != NULL) {
                anim3       = arg0->unk13088[i];
                anim3->unk0 = anim3->unk0 + delta;
                anim3       = arg0->unk13088[i];
                if (anim3->unk0 >= (s32)(anim3->unk8->frameCount << 0xC)) {
                    anim3->unk0 = 0;
                }
            }
            if (arg0->unk13488[4] != NULL) {
                anim4       = arg0->unk13288[i];
                anim4->unk0 = anim4->unk0 + delta;
                anim4       = arg0->unk13288[i];
                if (anim4->unk0 >= (s32)(anim4->unk8->frameCount << 0xC)) {
                    anim4->unk0 = 0;
                }
            }
            func_02029270(elem);
            func_02028a44();
            i++;
            elem = elem + 1;
        } while (i < arg0->unk1352C);
    }
    REG_GFX_FIFO_SWAP_BUFFERS = 1;
    OamMgr_FlushCommands(&g_OamMgr[0]);
    OamMgr_FlushCommands(&g_OamMgr[1]);
    PaletteMgr_Flush(g_PaletteManagers[0], NULL);
    PaletteMgr_Flush(g_PaletteManagers[1], NULL);
}

void func_ov030_020b6984(Ov030B5C58Root* arg0) {
    func_ov030_020b6464(arg0);
    ResourceMgr_ReinitManagers(NULL);
    DatMgr_ClearSlot(1);
    Mem_Free(&gDebugHeap, arg0);
}

void func_ov030_020b69b4(Ov030B5C58Root* arg0) {
    s32 stage;

    stage = MainOvlDisp_GetProcessStage();
    if (stage == 0x7FFFFFFF) {
        func_ov030_020b6984(arg0);
        return;
    }
    data_ov030_020e094c[stage](arg0);
}
