#include "Display.h"
#include "EasyFade.h"
#include "Engine/Color.h"
#include "Engine/Core/Memory.h"
#include "Engine/Core/OamMgr.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/TouchInput.h"
#include "Engine/Math/Random.h"
#include "Engine/Math/Sqrt.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/ObjResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Field/EventFlags.h"
#include "Field/Events.h"
#include "Field/FieldTypes.h"
#include "Field/FldHuman.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"
#include "common_data.h"
#include <nitro/fx/fx_atan.h>
#include <nitro/types.h>

extern s32        func_0202599c(s32 flagIndex);
extern s32        func_ov030_020b30d0(Task* task);
extern s32        func_ov030_020b38ec(Task* task);
extern s32        func_ov030_020b5cd8(Task* task);
extern s32        func_ov030_020b5d4c(Task* task);
extern FieldExit* func_ov030_020b8358(s32 area, FieldPos pos, s32 arg3, s32* arg4);
extern void       func_ov030_020be0f8(Task* task);
extern s32        func_ov030_020bd31c(Task* task);
extern s32        func_ov030_020bd340(Task* task);
extern s32        func_ov030_020c8a0c(TaskPool* pool, s32 arg1);
extern void       func_ov030_020c8a3c(Task* task);
extern void       func_ov030_020d1d08(void* arg0);
extern void       func_ov030_020aec38(s32 arg0);
extern void       func_ov030_020a7b50(FieldEventManager* mgr);
extern s32        func_ov030_020847f4(FieldEventManager* mgr);
extern void       func_ov030_020826a4(FieldEventManager* mgr);
extern void       func_ov030_02082788(FieldEventManager* mgr);
extern void       func_ov030_020827b0(FieldEventManager* mgr);
extern void       func_ov030_02082a50(FieldEventManager* mgr);
extern s32        func_ov030_020ac028(void);
extern void       func_ov030_020acc10(FieldEventManager* mgr);
extern void       func_ov030_020acd38(FieldEventManager* mgr);
extern void       func_02001b44(s32 arg0, s32 arg1, void* arg2, s32 arg3);
extern s32 (*data_02065fd4)(s32 arg0, s32 arg1, s32 arg2);
extern void func_ov030_020b7744(s32* arg0, s32 arg1);
extern s32  func_ov030_020b47f8(void* arg0, void* arg1, void* arg2, s32 arg3, s32 arg4);
extern s32  func_ov030_020c5ed0(void* arg0, s32 arg1, void* arg2, s32 arg3);
extern void func_ov030_020c6150(s32* arg0);
extern void func_ov030_020ac95c(FieldEventManager* mgr);

extern void                    func_ov030_020af980(s16 arg0);
extern s16                     func_ov030_020b7b20(void);
extern s16                     func_ov030_020b7bc4(void);
extern s32                     func_ov030_020aec08(s32 mask);
extern const FldSpawnTemplate* func_ov030_020baa2c(s32* count);
// Per-area pair of spawn-table selectors (24 areas): [0] feeds 020ac730, [1] 020ac438.
extern const s16 data_ov030_020d9eca[24][2];

// Walking routes of an area: NPCs spawn along one of `count` segments, in
// either direction.
typedef struct {
    /* 0x0 */ FldHumanSpawnEntry* routes;
    /* 0x4 */ s16                 count;
} FldAreaRoutes; // Size: 0x8

extern const FldAreaRoutes data_ov030_020da998[24];
extern s16                 data_0205e4e0[];
extern s32                 func_ov030_020c1af8(void);
extern s32                 func_ov030_020bd85c(void* arg0, s32 arg1, s32 arg2);
extern s32                 func_ov030_020c1a9c(void);
extern s32                 func_ov030_020bde98(void* arg0, s32 arg1);
extern s32                 func_ov030_020be0c8(void* arg0, s32 arg1);
extern s32                 func_020236a0(void);
extern s32                 func_ov030_020b2fc0(void* arg0, void* arg1, void* arg2);
extern s32                 func_ov030_020bd264(void* arg0, void* arg1, void* arg2);
extern void                func_ov030_02083024(FieldEventManager* mgr);
extern s32                 func_ov030_020b44b8(void* arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5);
extern s32                 func_ov030_020c4a48(void* arg0, s32 arg1);
extern s32                 func_ov030_020bd598(void* arg0, s32 arg1, s32 arg2);
extern s32                 func_ov030_020ccb48(void* arg0, s32 arg1);
extern s32                 func_ov030_020bff30(void* arg0, s32 arg1, u16 arg2, s32 arg3);
extern s32                 func_ov030_020c8fdc(void* arg0, s32 arg1);
extern s32                 func_ov030_020bfb50(void* arg0, s32 arg1);
extern void                func_ov030_020c0760(s16 arg0);
extern s16                 data_ov030_020d9924[];
extern BinIdentifier       data_ov030_020d95b4;
extern BinIdentifier       data_ov030_020d98bc[];
extern BgResource*         BgResMgr_AllocScreen(BgResMgr* mgr, void* source, s32 screenBase, s32 screenSize);
extern BgResource*         BgResMgr_AllocChar32(BgResMgr* mgr, void* source, u32 charBase, u32 offset, u32 size);
extern BgResource*         BgResMgr_AllocCharExtended(BgResMgr* mgr, void* source, s32 charBase, u32 arg3, u32 size);
extern void                func_ov030_020c2d50(s32 arg0);
extern void                func_ov030_020aeaa0(s32 arg0, s32 arg1);
extern void                func_ov030_020c3360(void);
extern void                func_ov030_020afab8(FieldEventManager* mgr);
extern s32                 func_ov030_020c0bac(TaskPool* pool, s32 arg1);
extern void                func_ov030_020c0f50(TaskPool* pool, s32 arg1, void* arg2);
extern void                func_ov030_020b4874(Task* task);
extern void                func_ov030_020c6300(void);
extern void                func_ov030_020aec1c(s32 arg0);
extern u16                 data_ov030_020d95c4[][2];
extern BinIdentifier       data_ov030_020d9944;
extern BinIdentifier       data_ov030_020d95a4;
extern TaskHandle          data_ov030_020e6360;
extern s16                 data_ov030_020d9c94[][2];

// Two encounter id ranges; the second is used when func_ov030_020abe18 says no.
typedef struct {
    /* 0x0 */ u16 lo1;
    /* 0x2 */ u16 hi1;
    /* 0x4 */ u16 lo2;
    /* 0x6 */ u16 hi2;
} FldEncounterRanges;

extern FldEncounterRanges         data_ov030_020da8de[]; // by area
extern FldEncounterRanges         data_ov030_020d9f2a[]; // by brand group
extern FldEncounterRanges         data_ov030_020d9904[]; // by chapter group
extern const FieldEventRewardFunc data_ov030_020da6c8[];
extern s32 (*const data_ov030_020da568[])(FieldEventManager*);
extern s32 data_ov030_020d98ec[][2];
extern s32 data_ov030_021002c0;
extern s32 data_ov030_021002cc;
extern s32 data_ov030_021002d8;

// Initial template values shared by the spawner setup functions.
extern FldSpawnTemplate data_ov030_020e5e7c;

void func_ov030_020aac84(FieldEventManager* mgr);
s32  func_ov030_020aacb0(void);
s32  func_ov030_020aad28(void);
s32  func_ov030_020aad40(void);
u16  func_ov030_020aad84(void);
s32  func_ov030_020aad94(FieldEventManager* mgr, s32* arg1);
s32  func_ov030_020aaeec(FieldEventManager* mgr);
s32  func_ov030_020aaf88(FieldEventManager* mgr);
s32  func_ov030_020aafc0(FieldEventManager* mgr);
s32  func_ov030_020ab0ec(FieldEventManager* mgr);
void func_ov030_020ab180(void);
void func_ov030_020ab1bc(FieldEventManager* mgr);
void func_ov030_020ab1fc(FieldEventManager* mgr);
s32  func_ov030_020ab2fc(FieldEventManager* mgr);
s32  func_ov030_020ab3b8(s32 arg0);
s32  func_ov030_020ab3ec(s32 arg0);
u16  func_ov030_020ab420(s32 arg0);
u16  func_ov030_020ab454(void);
s32  func_ov030_020ab494(FieldEventManager* mgr);
void func_ov030_020ab554(FieldEventManager* mgr);
void func_ov030_020ab924(FieldEventManager* mgr);
void func_ov030_020ab9cc(FieldEventManager* mgr);
void func_ov030_020aba1c(FieldEventManager* mgr);
void func_ov030_020abba8(FieldEventManager* mgr);
void func_ov030_020abbcc(FieldEventManager* mgr);
void func_ov030_020abc18(FieldEventManager* mgr);
s16  func_ov030_020abccc(s32 arg0);
s16  func_ov030_020abd08(s32 arg0);
s32  func_ov030_020abd54(s32 arg0);
s32  func_ov030_020abe18(s32 arg0);
s32  func_ov030_020abe64(u32 arg0);
u16  func_ov030_020abea0(s32 arg0);
u16  func_ov030_020abf64(s32 arg0);
s32  func_ov030_020ac028(void);
u16  func_ov030_020ac080(s32 arg0);
u16  func_ov030_020ac144(s32 arg0, s32 arg1);
void func_ov030_020ac1e8(s32 arg0, FldSpawnTemplate* arg1, s32 arg2);
s32  func_ov030_020ac254(void);
s32  func_ov030_020ac294(void);
void func_ov030_020ac2c0(FieldEventManager* mgr);
void func_ov030_020acd98(FieldEventManager* mgr, u32* arg1);
void func_ov030_020ace8c(FieldEventManager* mgr);
void func_ov030_020acfc4(FieldEventManager* mgr);
void func_ov030_020ad044(FieldEventManager* mgr);
void func_ov030_020ad1ac(FieldEventManager* mgr);
void func_ov030_020ad278(FieldEventManager* mgr);
void func_ov030_020ad3a8(FieldEventManager* mgr);
void func_ov030_020ad428(FieldEventManager* mgr);
void func_ov030_020ad48c(FieldEventManager* mgr);
void func_ov030_020ad4f4(FieldEventManager* mgr);
void func_ov030_020ad730(FieldEventManager* mgr);
void func_ov030_020ad958(FieldEventManager* mgr, s32 arg1);
void func_ov030_020adaf8(FieldEventManager* mgr);
s16  func_ov030_020adc84(void);
void func_ov030_020aca64(FieldEventManager* mgr, FieldPos pos, s32 arg3, FldSpawnTemplate tmpl, s32 arg5, s16 arg6, s16 arg7,
                         s32 arg8, FieldPos target);
void func_ov030_020acb8c(FieldEventManager* mgr, FieldPos pos, s32 arg3, FldSpawnTemplate tmpl, s32 arg5);
extern s32 func_ov030_020af944(s32 arg0);
extern s32 func_ov030_020aeaf4(void);
extern s32 func_ov030_020b86bc(FieldPos* pos, s16* arg1, s32* arg2, s32 arg3);
extern s32 func_ov030_020bc43c(TaskPool* pool, FieldPos* origin, FieldPos pos, s32 arg3, FldSpawnTemplate arg4, s32 arg5);
extern s32 func_ov030_020b5b9c(TaskPool* pool, FieldPos* origin, FieldPos start, s32 arg4, s32 dataType, FldSpawnTemplate tpl,
                               s32 arg7, s32 speed, s32 spawns, s32 angle, s16 arg11, s16 route, FieldPos target);
// Fixed NPC placements of area 5: x, y (pixels), unk_08 (-1 = by brand ranking), unk_0A.
extern s16 data_ov030_020d9ffe[][4];

void func_ov030_020aac84(FieldEventManager* mgr) {
    if (gSaveData.unk_24A0[gSaveData.unk_249C] != 0x48) {
        return;
    }
    func_ov030_020a7b50(mgr);
}

// Nonmatching: register allocation only (loop index/manager in r10/r7 vs
// target r7/r10, plus mov order of the final pair); all loads, stores and
// branches verified identical.
s32 func_ov030_020aacb0(void) {
    FieldEventManager* mgr      = data_ov030_02109d60;
    s32                found_lo = 0;
    s32                found_hi = 0;
    s32                i;

    for (i = 0; i < 0x23; i++) {
        if (mgr->unk_21BBC[i] != -1) {
            if (func_ov030_020b38ec(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_21BBC[i])) < 0x80) {
                found_lo = 1;
            } else {
                found_hi = 1;
            }
        }
    }
    if (found_lo == 0 || found_hi == 0) {
        return 0;
    }
    return 1;
}

s32 func_ov030_020aad28(void) {
    return data_ov030_02109d60->unk_21CEC;
}

s32 func_ov030_020aad40(void) {
    FieldEventManager* mgr = data_ov030_02109d60;

    if (gSaveData.chapter >= 0x2C) {
        gSaveData.chapter = 0;
    }
    return data_ov030_020da6c8[gSaveData.chapter](mgr);
}

u16 func_ov030_020aad84(void) {
    return gSaveData.unk_24BE;
}

// NOTE: the two mid-function `return 0`s share the single trailing
// epilogue (both assemble as `beq` to it), so they are written as a branch
// to the `ret0` label rather than predicated early returns.
s32 func_ov030_020aad94(FieldEventManager* mgr, s32* arg1) {
    if (func_0202599c(1) != 0 && gSaveData.chapter != 0x15) {
        s32 v = func_ov030_020ac028();

        if (v == 3) {
            return 0;
        }
        if (func_ov030_020bd31c(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D8)) == 0) {
            goto ret0;
        }
        mgr->unk_22E14 = 0x1E;
        if (RNG_Next(0x80) < 0x20U) {
            mgr->unk_22E18 = 0x404;
        } else if (RNG_Next(0x80) < 0x40U) {
            mgr->unk_22E18 = 0x405;
        } else if (RNG_Next(0x80) < 0x60U) {
            mgr->unk_22E18 = data_ov030_020d98ec[v][0];
        } else {
            mgr->unk_22E18 = data_ov030_020d98ec[v][1];
        }
        *arg1 = func_ov030_020bd340(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D8));
        return 1;
    }
    if (func_ov030_020b30d0(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D4)) != 0) {
        *arg1 = gSaveData.fieldPos.y;
        return 1;
    }
ret0:
    return 0;
}

s32 func_ov030_020aaeec(FieldEventManager* mgr) {
    TouchCoord coord;

    if (mgr->unk_216FC == -1) {
        return 0;
    }
    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    TouchInput_GetCoord(&coord);
    if (coord.x >= 6 && coord.x <= 0x16 && coord.y >= 2 && coord.y <= 0x12) {
        func_ov030_020c8a3c(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216FC));
        return 1;
    }
    return 0;
}

s32 func_ov030_020aaf88(FieldEventManager* mgr) {
    if (gSaveData.chapter >= 0x2C) {
        gSaveData.chapter = 0;
    }
    return data_ov030_020da568[gSaveData.chapter](mgr);
}

s32 func_ov030_020aafc0(FieldEventManager* mgr) {
    s32 sp0;
    s32 temp;
    s32 want_hook = 0;

    if (func_ov030_020aad94(mgr, &sp0) != 0) {
        FldEvent_BeginScript(mgr, func_ov030_020aad40(), 0);
        want_hook = 1;
    }
    temp = func_ov030_020847f4(mgr);
    if (temp != -1) {
        if (want_hook != 0) {
            if (func_ov030_020b5d4c(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_21788[temp])) < sp0) {
                return 1;
            }
        }
        mgr->unk_22E14     = 0;
        gSaveData.unk_2498 = 0;
        FldEvent_BeginScript(mgr, func_ov030_020b5cd8(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_21788[temp])), temp);
        return 1;
    }
    if (want_hook != 0) {
        return 1;
    }
    if (func_ov030_020aaeec(mgr) == 0) {
        return 0;
    }
    FldEvent_BeginScript(mgr, func_ov030_020aad84(), 0);
    func_ov030_020aec38(0xA);
    return 1;
}

s32 func_ov030_020ab0ec(FieldEventManager* mgr) {
    s32 sp4;

    mgr->unk_21A58 = func_ov030_020b8358(gSaveData.unk_2458, mgr->origin, mgr->unk_21AC0, &sp4);
    if (mgr->unk_21A58 != NULL) {
        if (func_0202599c(7) != 0) {
            return 1;
        }
        mgr->unk_21AC0 = 0;
        return 1;
    }
    if (sp4 == 0) {
        mgr->unk_21AC0 = 1;
    }
    return 0;
}

void func_ov030_020ab180(void) {
    u16 r = RNG_Next(0x400);

    if (r < 0x80U) {
        SndMgr_StartPlayingSE(0x402);
        return;
    }
    if (r >= 0x100U) {
        return;
    }
    SndMgr_StartPlayingSE(0x403);
}

void func_ov030_020ab1bc(FieldEventManager* mgr) {
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020826a4, mgr, 0);
    mgr->unk_21AAE = 0x1E;
    mgr->unk_21AB0 = 0;
}

void func_ov030_020ab1fc(FieldEventManager* mgr) {
    s16 sel    = gSaveData.unk_2458;
    s32 played = 0;

    if (sel >= 0x18 && sel < 0x25) {
        if (gSaveData.fieldPos.x < 0xF4000) {
            func_ov030_020d1d08(&data_ov030_021002c0);
            played = 1;
        } else if (gSaveData.fieldPos.x < 0x1C7000) {
            func_ov030_020d1d08(&data_ov030_021002cc);
            played = 1;
        }
    }
    sel = gSaveData.unk_2458;
    if (sel >= 0x1A && sel < 0x25 && gSaveData.fieldPos.x >= 0x1C7000) {
        func_ov030_020d1d08(&data_ov030_021002d8);
        played = 1;
    }
    if (played == 0) {
        return;
    }
    SndMgr_PlaySEWithPan(0x126, (gSaveData.fieldPos.x - mgr->origin.x) >> 0xC);
    mgr->unk_21AAC = 0x51;
    DebugOvlDisp_Push((OverlayCB)func_ov030_020827b0, mgr, 0);
}

s32 func_ov030_020ab2fc(FieldEventManager* mgr) {
    TouchCoord coord;

    if (Inventory_HasRequiredQuantity(0x2A7, 1, 0) == 0) {
        return 0;
    }
    if (SysControl.buttonState.pressedButtons & 8) {
        return 1;
    }
    if (TouchInput_WasTouchPressed() == 0) {
        return 0;
    }
    TouchInput_GetCoord(&coord);
    if (coord.x >= 0xEE && coord.y <= 0x1A) {
        func_ov030_020be0f8(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216F4));
        SndMgr_StartPlayingSE(0x116);
        return 1;
    }
    return 0;
}

s32 func_ov030_020ab3b8(s32 arg0) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (arg0 == data_ov030_020d95c4[i][0]) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020ab3ec(s32 arg0) {
    u32 i;

    for (i = 0; i < 3; i++) {
        if (arg0 == data_ov030_020d95c4[i][1]) {
            return 1;
        }
    }
    return 0;
}

u16 func_ov030_020ab420(s32 arg0) {
    s32 i = 0;

    for (;;) {
        if (arg0 == data_ov030_020d95c4[i][0]) {
            return data_ov030_020d95c4[i][1];
        }
        i++;
    }
}

u16 func_ov030_020ab454(void) {
    u8  chapter = gSaveData.chapter;
    s32 index;

    if (chapter == 0xF) {
        index = 0;
    } else if ((u8)(chapter + 0xF0) <= 1U) {
        index = 1;
    } else {
        index = 2;
    }
    return data_ov030_020d95c4[index][0];
}

s32 func_ov030_020ab494(FieldEventManager* mgr) {
    s16 sel;
    s16 r = (s16)RNG_Next(0x64);

    if (func_0202599c(7) != 0) {
        return 0;
    }
    sel = gSaveData.unk_2458;
    if (sel == 0x17) {
        return 0;
    }
    if (sel == 0x29) {
        return 0;
    }
    if (sel == 0x2A) {
        return 0;
    }
    if (sel == 0x2B) {
        return 0;
    }
    if (sel == 0x25) {
        return 0;
    }
    if (r >= gSaveData.unk_3120) {
        return 0;
    }
    func_ov030_020aec1c(7);
    gSaveData.unk_2498 = 0;
    FldEvent_BeginScript(mgr, func_ov030_020ab454(), 0);
    return 1;
}

// Nonmatching: the shared 0x40000 swaps r2/r3, and retail stores the sub BG0
// vOffset after the bgMode switch while ours hoists it with unk_21B38
// (USA 99.03%; JP unvalidated).
void func_ov030_020ab554(FieldEventManager* mgr) {
    func_ov030_020c2d50(gSaveData.unk_2548[gSaveData.unk_2458]);
    if (mgr->unk_216FC == -1) {
        if (func_ov030_020aad84() != 0 && gSaveData.unk_2458 != 0x25) {
            mgr->unk_216FC = func_ov030_020c8a0c(&mgr->taskPool, mgr->unk_21614);
        }
    }
    if (mgr->unk_21AF4 != 0) {
        mgr->unk_21AAC = mgr->unk_21AF4;
        mgr->unk_21AF4 = 0;
        DebugOvlDisp_Push((OverlayCB)func_ov030_02082788, mgr, 0);
        return;
    }
    if (gSaveData.unk_2458 != 0x17 && gSaveData.unk_2458 != 0x25) {
        func_ov030_020aeaa0(gSaveData.unk_2458, 1);
    }
    mgr->unk_21AF0 = 0;
    mgr->unk_21AC8 = 0;
    if (func_ov030_020aaf88(mgr) != 0) {
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)func_ov030_020ab924, mgr, 0);
        return;
    }
    if (mgr->unk_21630 == 4) {
        return;
    }
    if (mgr->unk_21AD0 != 0) {
        mgr->unk_21AD0 = 0;
        mgr->unk_21A58 = &mgr->unk_21AF8;
        func_ov030_020ab1bc(mgr);
        return;
    }
    if (func_ov030_02084788(mgr) != 0) {
        func_ov030_020c3360();
        mgr->unk_21B10 = 1;
        mgr->unk_21B14 = 0;
        mgr->unk_21B18 = 0;
        mgr->unk_21B1C = 0x19;
        mgr->unk_21B20 = -1;
        SndMgr_StartPlayingSE(0xBA);
        mgr->unk_21B24 = EasyTask_CreateTask(&mgr->taskPool, &data_ov030_020e6360, NULL, 0, NULL, NULL);
        mgr->unk_21B30 = func_ov030_020c0bac(&mgr->taskPool, mgr->unk_21614);
        mgr->unk_21B34 = 0;
        func_ov030_020c0f50(&mgr->taskPool, mgr->unk_21B30, &mgr->origin);
        if (mgr->unk_21700 != -1) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21700);
            mgr->unk_21700 = -1;
        }
        func_ov030_020b4874(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B4));
        EasyFade_FadeSubDisplay(FADER_SMOOTH, 0x10, 0);
        g_DisplaySettings.controls[DISPLAY_SUB].layers |= 1;
        {
            DisplayEngineState* sub = &g_DisplaySettings.engineState[DISPLAY_SUB];

            sub->bgOffsets[0].vOffset = mgr->unk_21B38 = 0x40000;

            switch (sub->bgSettings[0].bgMode) {
                default:
                    break;
                case DISPLAY_BGMODE_AFFINE:
                case DISPLAY_BGMODE_PLTT:
                case DISPLAY_BGMODE_BMP256:
                case DISPLAY_BGMODE_BMPDIRECT:
                case DISPLAY_BGMODE_BMPLARGE:
                    sub->bgAffines[0].unk_14 = 1;
                    break;
            }
            sub->bgOffsets[0].hOffset = 0;
        }
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)func_ov030_020ab9cc, mgr, 0);
        return;
    }
    if (func_ov030_020aafc0(mgr) != 0) {
        DebugOvlDisp_Pop();
        DebugOvlDisp_Push((OverlayCB)func_ov030_020ab924, mgr, 0);
        return;
    }
    if (func_ov030_020ab0ec(mgr) != 0) {
        if (mgr->unk_21A58->destArea >= 0x46) {
            DebugOvlDisp_Pop();
            mgr->unk_21630 = 5;
            return;
        }
        if (func_ov030_020ab494(mgr) != 0) {
            DebugOvlDisp_Pop();
            DebugOvlDisp_Push((OverlayCB)func_ov030_020ab924, mgr, 0);
            return;
        }
        func_ov030_020aec38(7);
        func_ov030_020ab1bc(mgr);
        func_ov030_020ab1fc(mgr);
        return;
    }
    if (func_ov030_020ab2fc(mgr) != 0) {
        DebugOvlDisp_Pop();
        mgr->unk_21630 = 1;
        return;
    }
    func_ov030_02083088(mgr);
    mgr->unk_21AD4 = 1;
}

void func_ov030_020ab924(FieldEventManager* mgr) {
    func_ov030_020a7b50(mgr);
    if (mgr->unk_21ABC != 0) {
        mgr->unk_21630 = 2;
        SndMgr_StartPlayingSE(0);
        DebugOvlDisp_Pop();
        return;
    }
    if (gSaveData.unk_2498 != 0) {
        return;
    }
    if (mgr->unk_21AD0 != 0) {
        mgr->unk_21AD0 = 0;
        mgr->unk_21A58 = &mgr->unk_21AF8;
        func_ov030_020ab1bc(mgr);
        return;
    }
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020ab554, mgr, 0);
    func_ov030_020c2de8();
    func_ov030_020acc10(mgr);
    mgr->unk_21AC0 = 0;
}

void func_ov030_020ab9cc(FieldEventManager* mgr) {
    func_ov030_020afab8(mgr);
    if (mgr->unk_21B10 != 0) {
        return;
    }
    DebugOvlDisp_Pop();
    DebugOvlDisp_Push((OverlayCB)func_ov030_020ab554, mgr, 0);
    DebugOvlDisp_Push((OverlayCB)func_ov030_02082a50, mgr, 0);
    func_ov030_020c2de8();
}

void func_ov030_020aba1c(FieldEventManager* mgr) {
    mgr->unk_2162C     = 0;
    mgr->unk_21618     = 0;
    mgr->unk_21AC0     = 0;
    mgr->unk_21AD4     = 0;
    gSaveData.unk_245A = 1;
    mgr->unk_21B14     = 5;
    mgr->unk_21AC8     = 1;
    mgr->unk_21A5C     = 0;
    mgr->unk_21CF4     = 0;
    mgr->unk_21AD0     = 0;
    mgr->unk_21ACC     = 1;
    mgr->unk_216C8     = -1;
    mgr->unk_21A90     = -1;
    mgr->unk_21A94     = -1;
    mgr->unk_21A6C     = 0;
    mgr->unk_21A78     = 0;
    mgr->unk_21A84     = 0;
    mgr->taskId_UGFade = -1;
    mgr->unk_21700     = -1;
    mgr->unk_216E0     = -1;
    mgr->unk_216E8     = -1;
    mgr->unk_21B2C     = -1;
    mgr->unk_216FC     = -1;
    mgr->unk_21A24     = -1;
    mgr->taskId_Choice = -1;
    func_ov030_020c0760(0);
    mgr->unk_21AF0 = 0;
    mgr->unk_21AF4 = 0;
    mgr->unk_21630 = 0;
    mgr->unk_21D22 = 0;
    mgr->unk_21AAE = 0;
    mgr->unk_21AAC = 0;
    mgr->unk_21AB0 = 0;
    mgr->unk_21620 = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d9944, 6, FALSE);
    mgr->unk_22E34 = 0;
    mgr->unk_21B10 = 0;
    mgr->unk_21A54 = 0;
    mgr->unk_21D14 = 0;
    mgr->unk_22E14 = 0;
    mgr->unk_21D18 = 0;
    mgr->unk_21ABC = 0;
    mgr->unk_22E38 = 0;
    mgr->unk_22E2C = 3;
    mgr->unk_22E3C = 0x1F;
    FldEvent_InitState(mgr);
    mgr->unk_21AC4 = 0;
    mgr->unk_21A28 = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95a4, 0x4A, FALSE);
    func_ov030_02083088(mgr);
    func_ov030_020aec1c(3);
    func_ov030_020aec1c(2);
}

void func_ov030_020abba8(FieldEventManager* mgr) {
    DatMgr_ReleaseData(mgr->unk_21A28);
    DatMgr_ReleaseData(mgr->unk_21620);
}

void func_ov030_020abbcc(FieldEventManager* mgr) {
    if (mgr->unk_21AC4 == 0) {
        return;
    }
    func_ov030_020b7aac(&mgr->unk_2191C);
    ObjResMgr_ReleaseResource(g_ObjResourceManagers[2], mgr->unk_21A9C);
    mgr->unk_21AC4 = 0;
}

void func_ov030_020abc18(FieldEventManager* mgr) {
    s32 i;

    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216B4);
    for (i = 0; i < mgr->unk_21918; i++) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21788[i]);
    }
    mgr->unk_21918 = 0;
    if (gSaveData.unk_2458 == 0x29) {
        for (i = 0; i < 0x40; i++) {
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21924[i]);
        }
    }
    func_ov030_020c6300();
    func_ov030_020acd38(mgr);
    func_ov030_020abbcc(mgr);
}

s16 func_ov030_020abccc(s32 arg0) {
    s32 i;

    for (i = 0; i < 0xC; i++) {
        if (arg0 == data_ov030_020d9c94[i][0]) {
            break;
        }
    }
    return data_ov030_020d9c94[i][1];
}

s16 func_ov030_020abd08(s32 arg0) {
    s32 i;

    for (i = 0; i < 0xC; i++) {
        s16 lo = data_ov030_020d9c94[i][1];

        if (lo > arg0) {
            continue;
        }
        if (arg0 < lo + 0xC) {
            return data_ov030_020d9c94[i][0];
        }
    }
    return data_ov030_020d9c94[i][0];
}

s32 func_ov030_020abd54(s32 arg0) {
    s32 place = 0xE;
    s32 temp  = func_ov030_020af350();

    (void)arg0;
    if (temp >= 0) {
        u16 div10 = RNG_Next(0x3E8) / 10;

        if (div10 < 0x46) {
            place = gSaveData.brandTrends[temp].ranking[0];
        } else if (div10 < 0x5A) {
            place = gSaveData.brandTrends[temp].ranking[1];
        }
    }
    if (place < 0xD) {
        return func_ov030_020abccc(place) + RNG_Next(0x2EE0) / 1000;
    }
    temp = RNG_Next(0xA8);
    if (temp >= 0xA2) {
        temp -= 0x96;
    }
    return temp;
}

s32 func_ov030_020abe18(s32 arg0) {
    if (arg0 < 6) {
        return 1;
    }
    if (arg0 < 0x12) {
        return 0;
    }
    if (arg0 < 0xA2) {
        return (((arg0 - 0x12) / 6) & 1) == 0;
    }
    return 1;
}

s32 func_ov030_020abe64(u32 arg0) {
    if (arg0 >= 0x596E && arg0 <= 0x5AA3) {
        return 1;
    }
    if (arg0 >= 0x5BB8) {
        return 1;
    }
    return 0;
}

u16 func_ov030_020abea0(s32 arg0) {
    s16 sel = gSaveData.unk_2458;
    u16 lo1 = data_ov030_020da8de[sel].lo1;
    u16 hi1 = data_ov030_020da8de[sel].hi1;
    u16 lo2 = data_ov030_020da8de[sel].lo2;
    u16 hi2 = data_ov030_020da8de[sel].hi2;
    if (func_ov030_020abe18(arg0) != 0) {
        if ((s32)(lo1 + gSaveData.unk_3112[4]) > (s32)hi1) {
            gSaveData.unk_3112[4] = 0;
        }
        {
            s16 v = gSaveData.unk_3112[4];

            gSaveData.unk_3112[4] = v + 2;
            return lo1 + v;
        }
    }
    if ((s32)(lo2 + gSaveData.unk_3112[5]) > (s32)hi2) {
        gSaveData.unk_3112[5] = 0;
    }
    {
        s16 v = gSaveData.unk_3112[5];

        gSaveData.unk_3112[5] = v + 2;
        return lo2 + v;
    }
}

u16 func_ov030_020abf64(s32 arg0) {
    s16 sel = func_ov030_020abd08(arg0);
    u16 lo1 = data_ov030_020d9f2a[sel].lo1;
    u16 hi1 = data_ov030_020d9f2a[sel].hi1;
    u16 lo2 = data_ov030_020d9f2a[sel].lo2;
    u16 hi2 = data_ov030_020d9f2a[sel].hi2;

    if (func_ov030_020abe18(arg0) != 0) {
        if ((s32)(lo1 + gSaveData.unk_3112[2]) > (s32)hi1) {
            gSaveData.unk_3112[2] = 0;
        }
        {
            s16 v = gSaveData.unk_3112[2];

            gSaveData.unk_3112[2] = v + 2;
            return lo1 + v;
        }
    }
    if ((s32)(lo2 + gSaveData.unk_3112[3]) > (s32)hi2) {
        gSaveData.unk_3112[3] = 0;
    }
    {
        s16 v = gSaveData.unk_3112[3];

        gSaveData.unk_3112[3] = v + 2;
        return lo2 + v;
    }
}

s32 func_ov030_020ac028(void) {
    u8 chapter = gSaveData.chapter;

    if (chapter <= 6U) {
        return 0;
    }
    if (chapter <= 0xDU) {
        return 1;
    }
    if (chapter <= 0x14U) {
        return 2;
    }
    if (chapter == 0x15) {
        return 3;
    }
    if (chapter <= 0x1CU) {
        return 0;
    }
    if (chapter <= 0x23U) {
        return 1;
    }
    return 2;
}

u16 func_ov030_020ac080(s32 arg0) {
    s32 sel = func_ov030_020ac028();
    u16 lo1 = data_ov030_020d9904[sel].lo1;
    u16 hi1 = data_ov030_020d9904[sel].hi1;
    u16 lo2 = data_ov030_020d9904[sel].lo2;
    u16 hi2 = data_ov030_020d9904[sel].hi2;

    if (func_ov030_020abe18(arg0) != 0) {
        if ((s32)(lo1 + gSaveData.unk_3112[0]) > (s32)hi1) {
            gSaveData.unk_3112[0] = 0;
        }
        {
            s16 v = gSaveData.unk_3112[0];

            gSaveData.unk_3112[0] = v + 2;
            return lo1 + v;
        }
    }
    if ((s32)(lo2 + gSaveData.unk_3112[1]) > (s32)hi2) {
        gSaveData.unk_3112[1] = 0;
    }
    {
        s16 v = gSaveData.unk_3112[1];

        gSaveData.unk_3112[1] = v + 2;
        return lo2 + v;
    }
}

u16 func_ov030_020ac144(s32 arg0, s32 arg1) {
    s32 count = (u16)(arg1 / 10);
    u16 v;

    if (count == 0) {
        count = 1;
    }
    if (gSaveData.unk_311E < count && RNG_Next(0x3E8) < 0x64U) {
        gSaveData.unk_311E++;
        if (RNG_Next(0x100) < 0x80U) {
            v = func_ov030_020abf64(arg0);
        } else {
            v = func_ov030_020abea0(arg0);
        }
    } else {
        v = func_ov030_020ac080(arg0);
    }
    func_ov030_020af944(1);
    return v;
}

void func_ov030_020ac1e8(s32 arg0, FldSpawnTemplate* arg1, s32 arg2) {
    if (func_0202599c(9) == 0 && func_ov030_020af944(0) < arg2 && RNG_Next(0x100) < 0x40U) {
        arg1->unk_10 = func_ov030_020ac144(arg0, arg2);
        arg1->unk_0E = arg1->unk_10 + 1;
        return;
    }
    arg1->unk_10 = -2;
    arg1->unk_0E = -2;
}

s32 func_ov030_020ac254(void) {
    u8 chapter = gSaveData.chapter;

    if (chapter == 0x11) {
        return 0x14;
    }
    if (chapter == 0x12) {
        return 0x32;
    }
    if (chapter == 0x13) {
        return 0x50;
    }
    if (chapter == 0x14) {
        return 0x64;
    }
    return 0;
}

s32 func_ov030_020ac294(void) {
    s32 v = func_ov030_020ac254();

    if (v == 0) {
        return 0;
    }
    return RNG_Next(0x64) < v;
}

// Nonmatching: scheduling (separate i*4 induction plus register rotation);
// table values, bounds, RNG calls and call sequence verified identical.
void func_ov030_020ac2c0(FieldEventManager* mgr) {
    FldSpawnTemplate template = data_ov030_020e5e7c;
    s16 sel                   = gSaveData.unk_2458;
    s16 cap;
    s32 want;
    s32 i;

    if (sel != 5) {
        return;
    }
    cap  = gSaveData.unk_2548[sel];
    want = 0xE;
    if (cap <= 0xE) {
        want = cap;
    }
    want -= mgr->unk_21918;
    if (want <= 0) {
        return;
    }
    for (i = 0; i < want; i++) {
        template.pos.x  = data_ov030_020d9ffe[i][0] << 0xC;
        template.pos.y  = data_ov030_020d9ffe[i][1] << 0xC;
        template.unk_08 = data_ov030_020d9ffe[i][2];
        if (template.unk_08 == -1) {
            template.unk_08 = func_ov030_020abccc(gSaveData.brandTrends[1].ranking[0]);
        }
        template.unk_08 += RNG_Next(0x12C) / 100;
        template.unk_0A = data_ov030_020d9ffe[i][3];
        if (func_ov030_020ac294() != 0) {
            template.unk_0A = 3;
            template.unk_10 = -2;
            template.unk_0E = -2;
        } else {
            func_ov030_020ac1e8(template.unk_08, &template, 0xF);
        }
        func_ov030_020acb8c(mgr, template.pos, template.unk_08, template, 0);
    }
}

// Nonmatching: register coloring of the loop locals (retail: i=r10, seg=r6,
// sx=r7, sy=r8) and stack slots of n / count*2 / angle; logic, calls and
// offsets match (USA 96.54%; JP unvalidated).
void func_ov030_020ac438(FieldEventManager* mgr, s32 percent) {
    FldSpawnTemplate    tpl = data_ov030_020e5e7c;
    FldHumanSpawnEntry* routes;
    FldHumanSpawnEntry* e;
    FieldPos            pos;
    FieldPos            target;
    s32                 n;
    s32                 i;
    s16                 count;
    s16                 seg;
    s32                 sx;
    s32                 sy;
    s32                 dx;
    s32                 dy;
    s32                 r;
    s32                 angle;
    s32                 level;
    s32                 cap;
    s32                 spawned;
    s16                 area;

    area    = gSaveData.unk_2458;
    cap     = gSaveData.unk_2548[area];
    spawned = mgr->unk_21918;
    n       = cap * percent / 100 - spawned;
    if (n < 0) {
        return;
    }
    if ((u32)area >= 24) {
        return;
    }
    routes = data_ov030_020da998[area].routes;
    if (routes == NULL) {
        return;
    }
    count = data_ov030_020da998[area].count;
    n -= spawned;
    for (i = 0; i < n; i++) {
        seg = i % (count * 2);
        if (seg >= count) {
            seg = seg - count * 2;
        }
        if (seg < 0) {
            e        = &routes[-1 - seg];
            sx       = e->x1 + RNG_Next(e->w1);
            sy       = e->y1 + RNG_Next(e->h1);
            target.x = e->x0 + RNG_Next(e->w0);
            target.y = e->y0 + RNG_Next(e->h0);
        } else {
            e        = &routes[seg];
            sx       = e->x0 + RNG_Next(e->w0);
            sy       = e->y0 + RNG_Next(e->h0);
            target.x = e->x1 + RNG_Next(e->w1);
            target.y = e->y1 + RNG_Next(e->h1);
        }
        dy = target.y - sy;
        dx = target.x - sx;
        target.y <<= 0xC;
        target.x <<= 0xC;
        r          = RNG_Next(Sqrti(dx * dx + dy * dy));
        angle      = FX_Atan2Idx(dy << 0xC, dx << 0xC);
        pos.x      = r * data_0205e4e0[(angle >> 4) * 2 + 1] + (sx << 0xC);
        pos.y      = r * data_0205e4e0[(angle >> 4) * 2] + (sy << 0xC);
        level      = func_ov030_020abd54(i);
        tpl.unk_0A = 9;
        if (func_ov030_020ac294() != 0) {
            tpl.unk_0A = 3;
            tpl.unk_10 = -2;
            tpl.unk_0E = -2;
        } else {
            func_ov030_020ac1e8(level, &tpl, 15);
        }
        func_ov030_020aca64(mgr, pos, level, tpl, (s32)routes, count, seg, angle, target);
    }
}

// Nonmatching: retail keeps the area cap in r5 and n in r10 (ours r3/r9) and
// swaps the operands of one fieldPos.y add; declaration order has no effect
// (USA 99.42%; JP unvalidated).
void func_ov030_020ac730(FieldEventManager* mgr, s32 percent) {
    FldSpawnTemplate tpl = data_ov030_020e5e7c;
    FieldPos         pos;
    s32              i;
    s32              j;
    s32              lim;
    s32              n;
    s32              k;
    s32              level;

    k = 0;
    func_ov030_020b7b40();
    n = gSaveData.unk_2548[gSaveData.unk_2458] * percent / 100 - mgr->unk_21918;
    if (n < 0) {
        n = k;
    }
    for (i = 0; i < n; i++) {
        if (func_ov030_020ac294() != 0) {
            if (++k >= 3) {
                n++;
                k = 0;
            }
            tpl.unk_0A = 3;
            tpl.unk_10 = -2;
            tpl.unk_0E = -2;
            pos.x      = RNG_Next(func_ov030_020b7bc4()) << 0xC;
            pos.y      = RNG_Next(func_ov030_020b7b20()) << 0xC;
            for (j = 0; j < 99; j++) {
                lim = func_ov030_020b7c9c(pos.x, pos.y);
                if (pos.y >= lim) {
                    goto placed;
                }
                pos.y = gSaveData.fieldPos.y + ((RNG_Next(0xC0) - 0x60) << 0xC);
            }
            pos.y = lim + (RNG_Next(0xC0) << 0xC);
        } else {
            tpl.unk_0A = 10;
            pos.x      = gSaveData.fieldPos.x + ((RNG_Next(0x100) - 0x80) << 0xC);
            pos.y      = gSaveData.fieldPos.y + ((RNG_Next(0xC0) - 0x60) << 0xC);
            for (;;) {
                if (pos.y >= func_ov030_020b7c9c(pos.x, pos.y)) {
                    break;
                }
                pos.y = gSaveData.fieldPos.y + ((RNG_Next(0xC0) - 0x60) << 0xC);
            }
        }
    placed:
        level = func_ov030_020abd54(i);
        if (tpl.unk_0A != 3) {
            func_ov030_020ac1e8(level, &tpl, 6);
        }
        func_ov030_020acb8c(mgr, pos, level, tpl, RNG_Next(0x10000));
    }
}

void func_ov030_020ac95c(FieldEventManager* mgr) {
    s32                     count = 0;
    s32                     i;
    const FldSpawnTemplate* tpl;
    s16                     area;

    mgr->unk_21918 = 0;
    mgr->unk_21920 = 0;
    mgr->unk_21922 = 0;
    func_ov030_020af980(0);
    tpl = func_ov030_020baa2c(&count);
    for (i = 0; i < count; i++) {
        if (tpl[i].unk_18 == func_ov030_020aec08(tpl[i].unk_14)) {
            func_ov030_020acb8c(mgr, tpl[i].pos, tpl[i].unk_08, tpl[i], 0);
        }
    }
    func_ov030_020acc10(mgr);
    area = gSaveData.unk_2458;
    if ((u32)area >= 24) {
        return;
    }
    func_ov030_020ac2c0(mgr);
    func_ov030_020ac730(mgr, data_ov030_020d9eca[area][0]);
    func_ov030_020ac438(mgr, data_ov030_020d9eca[area][1]);
}

void func_ov030_020aca64(FieldEventManager* mgr, FieldPos pos, s32 arg3, FldSpawnTemplate tmpl, s32 arg5, s16 arg6, s16 arg7,
                         s32 arg8, FieldPos target) {
    s32 rng;

    if (mgr->unk_21AB8 == 0) {
        return;
    }
    rng                            = RNG_Next(0x333);
    mgr->unk_21788[mgr->unk_21918] = func_ov030_020b5b9c(&mgr->taskPool, &mgr->origin, pos, mgr->unk_21918, mgr->unk_21614,
                                                         tmpl, arg3, rng + 0x333, arg5, arg8, arg6, arg7, target);
    if (mgr->unk_21788[mgr->unk_21918] == -1) {
        mgr->unk_21AB8 = 0;
    }
    mgr->unk_21918++;
    if (mgr->unk_21918 >= 0x64) {
        mgr->unk_21AB8 = 0;
    }
}

void func_ov030_020acb8c(FieldEventManager* mgr, FieldPos pos, s32 arg3, FldSpawnTemplate tmpl, s32 arg5) {
    FieldPos target = {0, 0};

    func_ov030_020aca64(mgr, pos, arg3, tmpl, 0, 0, 0, arg5, target);
}

void func_ov030_020acc10(FieldEventManager* mgr) {
    FldSpawnTemplate template = data_ov030_020e5e7c;
    FieldPos sp28;
    s32      sp24;
    s32      ret;
    s32      id;

    func_ov030_020acd38(mgr);
    ret = func_ov030_020b86bc(&sp28, &template.unk_0A, &sp24, mgr->unk_21A54);
    if (ret == 0) {
        return;
    }
    do {
        if (ret != 1) {
            if (sp24 == 0x25 && gSaveData.unk_2458 == 0x18) {
                sp24 = func_ov030_020aeaf4();
            }
            id = func_ov030_020bc43c(&mgr->taskPool, &mgr->origin, sp28, mgr->unk_21614, template, sp24);
            mgr->unk_21A34[mgr->unk_21A54] = id;
        } else {
            mgr->unk_21A34[mgr->unk_21A54] = -1;
        }
        mgr->unk_21A54++;
    } while ((ret = func_ov030_020b86bc(&sp28, &template.unk_0A, &sp24, mgr->unk_21A54)) != 0);
}

void func_ov030_020acd38(FieldEventManager* mgr) {
    s32 i = 0;

    if (mgr->unk_21A54 > 0) {
        do {
            s32 id = mgr->unk_21A34[i];

            if (id != -1) {
                EasyTask_DeleteTask(&mgr->taskPool, id);
            }
            i++;
        } while (i < mgr->unk_21A54);
    }
    mgr->unk_21A54 = 0;
}

void func_ov030_020acd98(FieldEventManager* mgr, u32* arg1) {
    ObjResource* res = ObjResMgr_AllocResource(g_ObjResourceManagers[2], NULL, 0x400, 0);

    mgr->unk_21A9C = res;
    mgr->unk_21AA0 = (u32)(res->bitmapIndex << 5);
    func_02001b44(1, mgr->unk_21AA0, Data_GetPackEntryData(mgr->unk_21D2C, 4), 0x400);
    mgr->unk_21AA4 = *arg1;
    func_02001b44(2, *arg1, Data_GetPackEntryData(mgr->unk_21D2C, 3), 0x10);
    *arg1 += 0x10;
    mgr->unk_21AA8 = *arg1;
    func_02001b44(2, *arg1, Data_GetPackEntryData(mgr->unk_21D2C, 5), 0x10);
}

void func_ov030_020ace8c(FieldEventManager* mgr) {
    u32 sp4 = 0x800;
    s32 idx;
    s32 id;

    func_ov030_020b7744(&mgr->unk_2191C, mgr->unk_21610);
    sp4 = (u32)(data_02065fd4(0x800, 0, 1) << 0x10) >> 0xD;
    func_ov030_020acd98(mgr, &sp4);
    mgr->unk_21AB8 = 1;
    mgr->unk_21AC4 = 1;
    mgr->unk_21AE8 = 0;
    mgr->unk_21AE4 = 0;
    mgr->unk_21AEC = 0;
    mgr->unk_216B4 = func_ov030_020b47f8(&mgr->taskPool, &mgr->origin, &gSaveData.fieldPos, mgr->unk_21AA0, mgr->unk_21AA4);
    if (gSaveData.unk_2458 == 0x29) {
        idx = 0;
        do {
            id                  = func_ov030_020c5ed0(&mgr->taskPool, mgr->unk_21610, &mgr->origin, idx);
            mgr->unk_21924[idx] = id;
            idx++;
        } while (idx < 0x40);
    }
    gSaveData.unk_311E = 0;
    func_ov030_020c6150(&mgr->unk_21D30);
    mgr->unk_21918 = 0;
    func_ov030_020ac95c(mgr);
}

void func_ov030_020acfc4(FieldEventManager* mgr) {
    s32 v = func_ov030_020c1af8();
    s32 id;

    if (mgr->unk_216E0 == -1) {
        return;
    }
    if (mgr->unk_216E8 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E8);
    }
    if (v < 0) {
        id = 0xA;
    } else if (v < 0x1E) {
        id = 7;
    } else if (v < 0x3C) {
        id = 8;
    } else {
        id = 9;
    }
    mgr->unk_216E8 = func_ov030_020bd85c(&mgr->taskPool, mgr->unk_21614, id);
}

void func_ov030_020ad044(FieldEventManager* mgr) {
    s32 v1;
    s32 v2;
    s32 v3;

    if (mgr->unk_216E0 != -1) {
        return;
    }
    if (gSaveData.unk_2458 == 0x25) {
        return;
    }
    v1 = 6;
    if (func_ov030_020c1a9c() != 0) {
        v1 = 5;
    }
    mgr->unk_216E0 = func_ov030_020bd85c(&mgr->taskPool, mgr->unk_21614, v1);
    v2             = 4;
    if (func_0202599c(0xA) != 0) {
        v2 = 3;
    }
    mgr->unk_216E4 = func_ov030_020bd85c(&mgr->taskPool, mgr->unk_21614, v2);
    func_ov030_020acfc4(mgr);
    mgr->unk_216EC = func_ov030_020bd85c(&mgr->taskPool, mgr->unk_21614, (s16)(gSaveData.unk_2470 + 11));
    v3             = 2;
    if (func_020236a0() != 0) {
        v3 = 1;
    }
    mgr->unk_216F8 = func_ov030_020bd85c(&mgr->taskPool, mgr->unk_21614, v3);
    mgr->unk_216F0 = func_ov030_020bde98(&mgr->taskPool, mgr->unk_21614);
    mgr->unk_216F4 = func_ov030_020be0c8(&mgr->taskPool, mgr->unk_21614);
    if (mgr->unk_216FC != -1) {
        return;
    }
    if (func_ov030_020aad84() == 0) {
        return;
    }
    mgr->unk_216FC = func_ov030_020c8a0c(&mgr->taskPool, mgr->unk_21614);
}

void func_ov030_020ad1ac(FieldEventManager* mgr) {
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E0);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E4);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216E8);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216EC);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216F8);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216F0);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216F4);
    mgr->unk_216E0 = -1;
    mgr->unk_216E8 = -1;
    if (mgr->unk_216FC == -1) {
        return;
    }
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216FC);
    mgr->unk_216FC = -1;
}

// Nonmatching: pool-load CSE (target loads the gSaveData+0x244C pool word
// separately for each call; ours hoists it into r5, growing the frame);
// all argument values, stores and call sequence verified identical.
void func_ov030_020ad278(FieldEventManager* mgr) {
    void* dst;

    mgr->unk_216D4 = func_ov030_020b2fc0(&mgr->taskPool, &mgr->origin, &gSaveData.fieldPos);
    mgr->unk_216D8 = func_ov030_020bd264(&mgr->taskPool, &mgr->origin, &gSaveData.fieldPos);
    func_ov030_02083024(mgr);
    dst            = Data_GetPackEntryData(mgr->unk_21A28, 1);
    mgr->unk_21A2C = dst;
    mgr->unk_21A30 = PaletteMgr_AcquireContiguous(g_PaletteManagers[2], dst, 5, 1);
    func_ov030_020ad044(mgr);
    mgr->unk_216B0          = func_ov030_020b44b8(&mgr->taskPool, mgr->unk_21614, 0xF0, 0xB0, 0, 0);
    mgr->taskId_KeywordMenu = -1;
    mgr->unk_216BC          = -1;
    mgr->unk_216C0          = -1;
    mgr->unk_216CC          = -1;
    if (mgr->unk_216C8 == -1 && mgr->unk_21CF4 != 0) {
        mgr->unk_216C8 = func_ov030_020c4a48(&mgr->taskPool, mgr->unk_21614);
    }
    func_ov030_020ace8c(mgr);
}

void func_ov030_020ad3a8(FieldEventManager* mgr) {
    func_ov030_020abc18(mgr);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216D4);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216D8);
    PaletteMgr_ReleaseResource(g_PaletteManagers[2], mgr->unk_21A30);
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_216B0);
    mgr->unk_216B0 = -1;
    func_ov030_020ad1ac(mgr);
    func_ov030_020ad428(mgr);
}

void func_ov030_020ad428(FieldEventManager* mgr) {
    if (mgr->unk_21A84 == NULL) {
        return;
    }
    BgResMgr_ReleaseScreen(g_BgResourceManagers[0], mgr->unk_21A84);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], mgr->unk_21A88);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], mgr->unk_21A8C);
    mgr->unk_21A84 = NULL;
}

void func_ov030_020ad48c(FieldEventManager* mgr) {
    func_ov030_020ad428(mgr);
    if (mgr->unk_21A90 != -1) {
        EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21A90);
        mgr->unk_21A90 = -1;
    }
    if (mgr->unk_21A94 == -1) {
        return;
    }
    EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21A94);
    mgr->unk_21A94 = -1;
}

void func_ov030_020ad4f4(FieldEventManager* mgr) {
    s32   v = mgr->unk_21D14;
    s32   v0;
    s32   v5;
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    if (v == 0) {
        SndMgr_StartPlayingSE(0xE8);
    }
    if (v == 3 || v == 8) {
        SndMgr_StartPlayingSE(0xD3);
    }
    if (v == 8) {
        v5 = 0;
        v0 = 6;
    } else if (v < 5) {
        v0 = v + 3;
        v5 = 0;
    } else if (v < 9) {
        v0 = v - 4;
        v5 = 1;
    } else {
        v0 = v - 8;
        v5 = 2;
    }
    if (v == 0x94) {
        pack = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95b4, 9, FALSE);
    } else {
        pack = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d98bc[v5], v0, FALSE);
    }
    charData   = Data_GetPackEntryData(pack, 1);
    screenData = Data_GetPackEntryData(pack, 2);
    palData    = Data_GetPackEntryData(pack, 3);
    func_ov030_020ad428(mgr);
    mgr->unk_21A84 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A88 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, charSize);
    mgr->unk_21A8C = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 0x10);
    if (v == 3) {
        mgr->unk_21A90 = func_ov030_020bd598(&mgr->taskPool, mgr->unk_21614, 0);
    } else if (v != 8) {
        mgr->unk_21A90 = -1;
    } else {
        mgr->unk_21A90 = func_ov030_020bd598(&mgr->taskPool, mgr->unk_21614, 1);
    }
    DatMgr_ReleaseData(pack);
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x3F;
    g_DisplaySettings.engineState[0].blendCoeff0 = 0xA;
    g_DisplaySettings.engineState[0].blendCoeff1 = 6;
}

void func_ov030_020ad730(FieldEventManager* mgr) {
    s16   sel = mgr->unk_21D22;
    s32   v2  = 8;
    Data* pack;
    void* charData;
    void* screenData;
    void* palData;
    u32   charSize;

    if (sel == 3) {
        v2 = 9;
    } else if ((u16)(s16)(sel - 4) <= 1) {
        v2 = 10;
    }
    pack           = DatMgr_LoadPackEntry(mgr->unk_21614, NULL, 0, &data_ov030_020d95b4, v2, FALSE);
    charData       = Data_GetPackEntryData(pack, 1);
    screenData     = Data_GetPackEntryData(pack, 2);
    palData        = Data_GetPackEntryData(pack, 3);
    mgr->unk_21A84 = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], screenData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                                          g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    charSize       = (((UnkSmallStruct_DispBG*)charData)->header.raw & ~0xFF) >> 8;
    if ((((UnkSmallStruct_DispBG*)charData)->header.format & 0xF0) == 0) {
        charSize -= 4;
    }
    mgr->unk_21A88 = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], charData,
                                          g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, charSize);
    mgr->unk_21A8C = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_MAIN], palData, 0, 0, 0x10);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], mgr->unk_21A8C);
    DatMgr_ReleaseData(pack);
    func_ov030_020c0760(0);
    if (mgr->unk_21D22 == 3) {
        mgr->unk_21A90 = func_ov030_020ccb48(&mgr->taskPool, mgr->unk_21614);
        mgr->unk_21A94 = func_ov030_020bff30(&mgr->taskPool, mgr->unk_21614, mgr->unk_21D28, 0);
        return;
    }
    if ((u16)(s16)(mgr->unk_21D22 - 4) <= 1) {
        mgr->unk_21A90 = func_ov030_020c8fdc(&mgr->taskPool, mgr->unk_21614);
        return;
    }
    mgr->unk_21A90 = func_ov030_020bfb50(&mgr->taskPool, mgr->unk_21614);
}

void func_ov030_020ad958(FieldEventManager* mgr, s32 arg1) {
    s32 v5;

    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForBg();
    GX_DisableBankForObj();
    GX_DisableBankForObjExtPltt();
    GX_SetBankForBg(1);
    GX_SetBankForObj(2);
    GX_SetBankForObjExtPltt(0x20);
    g_DisplaySettings.controls[DISPLAY_MAIN].objTileMode = GX_OBJTILEMODE_1D_128K;
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_2D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_2D);
    if (arg1 != 0) {
        v5 = 1;
    } else {
        v5 = 0;
    }
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, v5, 0, 1, 0, (v5 << 7) | 4);
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 1;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x3F;
    OamMgr_InitEngine(0, 0);
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    OamMgr_InitEngine(0, 2);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    Color_CopyRange((u16*)&data_020672ec, &data_0205a128, 0, 0x200, 1);
    OamMgr_ResetCommandQueues(&g_OamMgr[0]);
    BgResMgr_Init(&mgr->resources.mainBgMgr, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr0, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr2, 2);
    PaletteMgr_Init(&mgr->resources.palMgr0, 0);
    PaletteMgr_Init(&mgr->resources.palMgr2, 2);
}

void func_ov030_020adaf8(FieldEventManager* mgr) {
    GX_DisableBankForTex();
    GX_DisableBankForTexPltt();
    GX_DisableBankForBg();
    GX_DisableBankForObj();
    GX_DisableBankForObjExtPltt();
    GX_SetBankForTex(3);
    GX_SetBankForTexPltt(0x40);
    GX_SetBankForBg(0x20);
    GX_SetBankForObj(0x10);
    Display_CommitSynced();
    g_DisplaySettings.controls[DISPLAY_MAIN].dispMode  = GX_DISPMODE_GRAPHICS;
    g_DisplaySettings.controls[DISPLAY_MAIN].bgMode    = GX_BGMODE_0;
    g_DisplaySettings.controls[DISPLAY_MAIN].dimension = GX2D3D_MODE_3D;
    GX_SetGraphicsMode(GX_DISPMODE_GRAPHICS, GX_BGMODE_0, GX2D3D_MODE_3D);
    Display_InitMainBG1(DISPLAY_BGMODE_TEXT, GX_BG_SIZE_TEXT_256x256, GX_BG_COLORS_16, 6, 0, 0, 0x600);
    g_DisplaySettings.engineState[0].blendMode   = 1;
    g_DisplaySettings.engineState[0].blendLayer0 = 2;
    g_DisplaySettings.engineState[0].blendLayer1 = 0x33;
    OamMgr_InitEngine(0, 0);
    OamMgr_Reset(&g_OamMgr[0], 0, 0);
    Color_CopyRange(&data_02066aec, &data_0205a128, 0, 0x200, 1);
    OamMgr_InitEngine(0, 2);
    OamMgr_SetAffineCount(&g_OamMgr[2], 0);
    Color_CopyRange((u16*)&data_020672ec, &data_0205a128, 0, 0x200, 1);
    BgResMgr_Init(&mgr->resources.mainBgMgr, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr0, 0);
    ObjResMgr_Init(&mgr->resources.objResMgr2, 2);
    PaletteMgr_Init(&mgr->resources.palMgr0, 0);
    PaletteMgr_Init(&mgr->resources.palMgr2, 2);
}

s16 func_ov030_020adc84(void) {
    s32 v = func_ov030_020ac028();
    s32 off;

    if (v == 0) {
        off = 0;
        if (gSaveData.chapter == 5) {
            off = 12;
        }
    } else if (v == 1) {
        off = 4;
    } else if (v == 2) {
        off = 8;
    } else {
        return 0;
    }
    return data_ov030_020d9924[off + gSaveData.unk_2470];
}
