#include "Display.h"
#include "EasyFade.h"
#include "Engine/Core/System.h"
#include "Engine/EasyTask.h"
#include "Engine/Resources/BgResMgr.h"
#include "Field/Events.h"
#include "Field/TaskHelpers.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SndMgr.h"

// USA 0x020af320-0x020b04ac: battle request entry points and the battle
// transition state machine. The savedata writes connect selectors, difficulty
// and drop-rate snapshots to combat (see docs/decomp/ov030/README.md).
extern const u8         data_ov030_020dbdac[44];
extern const s16        data_ov030_020d98a6[];
extern const u16        data_ov030_020d9964[];
extern const TaskHandle Tsk_Fld_Raster;
extern const u32        data_0205a124[];

s32  func_ov030_020af2fc(void);
s16  func_ov030_020c5804(s32 kind);
void func_ov030_020ab180(void);
void func_ov030_020ad1ac(FieldEventManager* mgr);
void func_ov030_020ad428(FieldEventManager* mgr);
void func_ov030_020aed54(s32 id);
void func_ov030_020830ac(FieldEventManager* mgr);
void func_ov030_020831f4(FieldEventManager* mgr);
void func_ov030_02083350(FieldEventManager* mgr);
void func_ov030_020833fc(FieldEventManager* mgr);
void func_ov030_020834c4(FieldEventManager* mgr);
void func_ov030_02083528(FieldEventManager* mgr);
void func_ov030_02083664(FieldEventManager* mgr);
void func_ov030_020836c8(FieldEventManager* mgr);
void func_ov030_02083804(FieldEventManager* mgr);
void func_ov030_02083dfc(FieldEventManager* mgr);
void func_ov030_02083f58(FieldEventManager* mgr);
void func_ov030_0208403c(FieldEventManager* mgr);
void func_ov030_020846bc(FieldEventManager* mgr, s32 mode);
s32  func_ov030_02084788(FieldEventManager* mgr);
s32  func_ov030_020b2ff4(Task* task);
s32  func_ov030_020b3020(Task* task);
void func_ov030_020b30f4(Task* task);
void func_ov030_020b458c(Task* task, s32 stage);
void func_ov030_020b4598(Task* task, s32 stage);
s32  func_ov030_020b4838(Task* task, s32 stage);
void func_ov030_020b4864(Task* task);
void func_ov030_020bd34c(Task* task);
s32  func_ov030_020bf330(TaskPool* pool, s32 id);
s32  func_ov030_020c0d28(TaskPool* pool, s32 id, s32 id2);
s32  func_ov030_020c0e34(TaskPool* pool, u32 id);
s32  func_ov030_020c5f08(TaskPool* pool, u32 id, s32 arg);
s32  func_ov030_020cb6cc(FieldPos* pos, u16* out);
void DebugOvlDisp_Pop(void);

u8 func_ov030_020af320(u32 area) {
    if (area >= 44) {
        return 35;
    }
    return data_ov030_020dbdac[area];
}

// Raw m2c declares this tail call void; r0 carries the lookup result.
u8 func_ov030_020af338(void) {
    return func_ov030_020af320(gSaveData.unk_2458);
}

s32 func_ov030_020af350(void) {
    s32 value = func_ov030_020af338();
    if (value >= 21) {
        value = -1;
    }
    return value;
}

void func_ov030_020af364(s32 selector) {
    data_ov030_02109d60->unk_21ABC = 1;
    gSaveData.unk_3130             = 0;
    gSaveData.unk_3134             = selector;
    gSaveData.unk_3142             = 0;
    gSaveData.battleDifficulty     = gSaveData.playerStats.difficulty;
    gSaveData.unk_3138             = func_ov030_020af2fc();
    gSaveData.unk_3140             = 1;
    if (selector == 2) {
        gSaveData.unk_3140 = 0;
        gSaveData.unk_3130 = 1;
    } else if (selector == 3) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 4) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 5) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 6) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 9) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 18) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 33) {
        gSaveData.unk_3130 = 1;
    } else if (selector == 31) {
        gSaveData.unk_3130 = 1;
        gSaveData.unk_3140 = 0;
    } else if ((u32)(selector - 47) <= 2) {
        gSaveData.unk_3130 = 2;
    }
    if (selector == -1) {
        gSaveData.unk_3134    = 1;
        gSaveData.battleCount = 1;
        // MainData +0x3146: first selected encounter in the battle-request block.
        gSaveData.battleEncounters[0] = func_ov030_020c5804(0);
    }
    if (selector == -2) {
        gSaveData.unk_3134            = 1;
        gSaveData.battleCount         = 1;
        gSaveData.battleEncounters[0] = func_ov030_020c5804(2);
    }
    // MainData +0x3166: request-time drop-rate snapshot, 0x20 bytes past the
    // first encounter slot.
    gSaveData.battleDropRate = gSaveData.playerStats.dropRate;
}

void func_ov030_020af4d8(s32 event, s32 arg2) {
    // The delinked object names this tail call FldEvent_BeginScript.
    FldEvent_BeginScript(data_ov030_02109d60, (FieldEvent)event, arg2);
}

void func_ov030_020af4fc(void) {
    data_ov030_02109d60->unk_22E38 += 1;
}

void func_ov030_020af51c(void) {
    data_ov030_02109d60->unk_21B18 += 1;
}

u16 func_ov030_020af53c(void) {
    s32 v = data_ov030_02109d60->unk_22E3C;
    return (u16)(v | (v << 5) | (v << 10));
}

void func_ov030_020af564(FieldPos* pos) {
    u16 color;

    if (gSaveData.unk_2458 == 8) {
        if (func_ov030_020cb6cc(pos, &color) != 0) {
            if (color != 0x4E73) {
                REG_GFX_FIFO_VERTEX_COLOR = color;
            }
        } else {
            REG_GFX_FIFO_VERTEX_COLOR = 0x4E73;
        }
    } else {
        if (func_ov030_020cb6cc(pos, &color) != 0) {
            REG_GFX_FIFO_VERTEX_COLOR = color;
        }
    }
}

struct PaletteResource;
struct PaletteResource* func_ov030_020af5d4(void) {
    return data_ov030_02109d60->unk_21A30;
}

s32 func_ov030_020af5ec(void) {
    return func_ov030_020b3020(EasyTask_GetTaskById(&data_ov030_02109d60->taskPool, data_ov030_02109d60->unk_216D4));
}

s32 func_ov030_020af618(void) {
    return func_ov030_020b2ff4(EasyTask_GetTaskById(&data_ov030_02109d60->taskPool, data_ov030_02109d60->unk_216D4));
}

void func_ov030_020af644(s16* out0, s16* out1) {
    FieldEventManager* mgr = data_ov030_02109d60;

    *out0 = (s16)((gSaveData.fieldPos.x - mgr->origin.x) >> 0xC);
    *out1 = (s16)((gSaveData.fieldPos.y - mgr->origin.y - func_ov030_020af5ec()) >> 0xC);
}

s32 func_ov030_020af6a0(s32 x) {
    if (x < -0x3FD) {
        return x;
    }
    if (x >= -0x3F2) {
        return x;
    }
    return data_ov030_020d98a6[x + 0x3FD];
}

void func_ov030_020af6d4(s32 mode) {
    func_ov030_020846bc(data_ov030_02109d60, mode);
}

void func_ov030_020af6f0(s32 arg0) {
    data_ov030_02109d60->unk_21D18 = arg0;
}

void func_ov030_020af708(s16 arg0) {
    data_ov030_02109d60->unk_21D1C = arg0;
    SndMgr_StartPlayingSE((SndMgrSeIdx)0xC2);
}

s32 func_ov030_020af730(void) {
    return data_ov030_02109d60->unk_21AF0;
}

s32 func_ov030_020af748(void) {
    return data_ov030_02109d60->unk_21AD4;
}

void func_ov030_020af760(s32 arg0) {
    data_ov030_02109d60->unk_21B1C += arg0;
}

void func_ov030_020af780(s32 arg0) {
    FieldEventManager* mgr = data_ov030_02109d60;

    if (arg0 >= 0x17 && mgr->unk_21A98 < 0x17) {
        mgr->unk_21A98 = arg0;
        func_ov030_020833fc(mgr);
        func_ov030_02083350(mgr);
        return;
    }
    if (arg0 < 9) {
        return;
    }
    if (mgr->unk_21A98 >= 9) {
        return;
    }
    mgr->unk_21A98 = arg0;
    func_ov030_020833fc(mgr);
    func_ov030_02083350(mgr);
}

void func_ov030_020af7f0(void) {
    FieldEventManager* mgr = data_ov030_02109d60;

    func_ov030_020831f4(mgr);
    if (mgr->unk_216B0 == -1) {
        return;
    }
    func_ov030_020b4598(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B0), 3);
}

s32 func_ov030_020af834(s32 arg0, s32 arg1) {
    FieldEventManager* mgr = data_ov030_02109d60;
    s32                start;
    s32                end;
    s32                i;

    if (arg0 < 0x20) {
        start = 0;
        end   = 0x20;
    } else {
        start = 0x20;
        end   = 0x40;
    }
    for (i = start; i < end; i++) {
        if (i != arg0 && func_ov030_020c5f08(&mgr->taskPool, mgr->unk_21924[i], arg1) != 0) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020af8ac(void) {
    s32 v = 0;

    if (Inventory_HasRequiredQuantity(0x2C7, 1, 0) != 0) {
        v = 4;
    }
    if (Inventory_HasRequiredQuantity(0x2C8, 1, 0) != 0) {
        v = 0x10;
    }
    return v;
}

s32 func_ov030_020af8f0(void) {
    FieldEventManager* mgr = data_ov030_02109d60;
    s32                limit;

    limit = func_ov030_020af8ac();
    return (s16)mgr->unk_21B34 < limit;
}

void func_ov030_020af920(void) {
    data_ov030_02109d60->unk_21B34 = (s16)data_ov030_02109d60->unk_21B34 + 1;
}

s16 func_ov030_020af944(s32 arg0) {
    FieldEventManager* mgr = data_ov030_02109d60;

    mgr->unk_22E1C += arg0;
    if (mgr->unk_22E1C < 0) {
        mgr->unk_22E1C = 0;
    }
    return mgr->unk_22E1C;
}

void func_ov030_020af980(s16 arg0) {
    data_ov030_02109d60->unk_22E1C = arg0;
}

FieldPos* func_ov030_020af99c(void) {
    return &data_ov030_02109d60->origin;
}

s32 func_ov030_020af9b4(void) {
    u32 i;
    u16 encounter;

    if (gSaveData.battleCount != 1) {
        return 0;
    }
    // The field is a u16 halfword: mwcc emits ldrh for this read. Keeping the
    // local unsigned is load-bearing -- an s16 local collapses the load into
    // the ldrsh that the neighbouring battleCount check already uses (measured
    // 100.00 -> 97.00).
    encounter = gSaveData.battleEncounters[0];
    for (i = 0; i < 0x11; i++) {
        if (encounter == data_ov030_020d9964[i]) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020afa04(void) {
    s32 i;

    for (i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (gSaveData.scanSymbols[i].state == 2 && gSaveData.scanSymbols[i].category == 8) {
            return 1;
        }
    }
    return 0;
}

s32 func_ov030_020afa58(void) {
    s32 i;

    for (i = 0; i < gSaveData.scanSymbolCount; i++) {
        if (gSaveData.scanSymbols[i].state == 2) {
            s16 category = gSaveData.scanSymbols[i].category;
            if (category >= 0x17 && category < 0x1F) {
                return 1;
            }
        }
    }
    return 0;
}

void func_ov030_020afab8(FieldEventManager* mgr) {
    switch (mgr->unk_21B14) {
        case 0:
            if (mgr->unk_21B1C != 0) {
                if (mgr->unk_21B1C == 0x19) {
                    func_ov030_02083528(mgr);
                    func_ov030_020aed54(-2);
                }
                if (mgr->unk_21B1C < 0x17) {
                    EasyFade_FadeSubDisplay(FADER_LINEAR, 0, 0xA3D);
                }
                func_02026590(&mgr->unk_21B38, 0, (u16)mgr->unk_21B1C);
                Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, mgr->unk_21B38);
                mgr->unk_21B1C -= 1;
                return;
            }
            EasyFade_FadeSubDisplay(FADER_SMOOTH, 0x10, 0);
            mgr->unk_21B1C = 1;
            mgr->unk_21B14 += 1;
            return;
        case 1:
            if (mgr->unk_21B1C != 0) {
                mgr->unk_21B1C = mgr->unk_21B1C - 1;
                g_DisplaySettings.controls[DISPLAY_SUB].layers |= 3;
                g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 1;
                g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 1;
                func_ov030_02083804(mgr);
                func_ov030_020836c8(mgr);
                func_ov030_020ab180();
                Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG0, 0, 0x40000);
                Display_SetBGOffset(DISPLAY_SUB, DISPLAY_BG1, 0, 0x40000);
                return;
            }
            mgr->unk_21B28 = func_ov030_020bf330(&mgr->taskPool, mgr->unk_21614);
            func_ov030_020846bc(mgr, 4);
            func_ov030_020830ac(mgr);
            mgr->unk_21B14 += 1;
            mgr->unk_21AB0                                          = 0;
            mgr->unk_21AB4                                          = 0x10000;
            mgr->unk_21B1C                                          = 0x4B;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode   = 1;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 2;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x33;
            func_ov030_020b4864(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B4));
            return;
        case 2:
            EasyFade_FadeSubDisplay(FADER_LINEAR, 0, 0x369);
            g_DisplaySettings.controls[DISPLAY_MAIN].layers |= 2;
            if (mgr->unk_21B1C != 0) {
                func_02026590(&mgr->unk_21AB0, 0x10000, (u16)mgr->unk_21B1C);
                func_02026590(&mgr->unk_21AB4, 0x6000, (u16)mgr->unk_21B1C);
                mgr->unk_21B1C -= 1;
                {
                    s32 v0                                                  = mgr->unk_21AB0;
                    s32 v1                                                  = mgr->unk_21AB4;
                    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = (u32)(v0 >> 0xC);
                    g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = (u32)(v1 >> 0xC);
                }
                if (mgr->unk_21B1C != 0) {
                    return;
                }
                g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~1;
                g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~2;
                EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B28);
                return;
            }
            mgr->unk_21B14 += 1;
            return;
        case 3:
            if (func_ov030_02084788(mgr) != 0) {
                SndMgr_StartPlayingSE((SndMgrSeIdx)0xB9);
                mgr->unk_21B14 += 1;
                EasyFade_FadeBothDisplays(FADER_LINEAR, 0x10, 0x1000);
                return;
            }
            if (func_ov030_020c0d28(&mgr->taskPool, mgr->unk_21B30, mgr->unk_216B0) == 0) {
                return;
            }
            {
                s16 category   = gSaveData.scanSymbols[gSaveData.scanSymbolSelection].category;
                mgr->unk_21A98 = category;
                if (category == 0x16) {
                    mgr->unk_21630 = 7;
                    DebugOvlDisp_Pop();
                    return;
                }
            }
            mgr->unk_21B2C = EasyTask_CreateTask(&mgr->taskPool, &Tsk_Fld_Raster, NULL, 0, NULL, NULL);
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~4;
            g_DisplaySettings.controls[DISPLAY_SUB].layers &= ~8;
            func_ov030_02083dfc(mgr);
            func_ov030_02083f58(mgr);
            func_ov030_0208403c(mgr);
            func_ov030_02083350(mgr);
            g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 2;
            g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[2].priority = 1;
            g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[3].priority = 3;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendMode              = 1;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer0            = 0xC;
            g_DisplaySettings.engineState[DISPLAY_SUB].blendLayer1            = 0x22;
            mgr->unk_21AB0                                                    = 0;
            mgr->unk_21AB4                                                    = 0x10000;
            {
                u32 coeff0                                             = (u32)(mgr->unk_21AB0 >> 0xC);
                u32 coeff1                                             = (u32)(mgr->unk_21AB4 >> 0xC);
                g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = coeff0;
                g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = coeff1;
            }
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode              = 0;
            g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[1].priority = 3;
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B24);
            func_ov030_020ad1ac(mgr);
            func_ov030_020b458c(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B0), 0);
            for (s32 i = 0; i < mgr->unk_21918; i++) {
                EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21788[i]);
            }
            mgr->unk_21918 = 0;
            mgr->unk_21B14 = 6;
            mgr->unk_21B1C = 0x5A;
            return;
        case 4:
            if (EasyFade_IsFading() != 0) {
                return;
            }
            if (mgr->unk_21B2C != -1) {
                EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B2C);
                mgr->unk_21B2C = -1;
            }
            g_DisplaySettings.controls[DISPLAY_MAIN].layers &= ~2;
            func_ov030_020846bc(mgr, 1);
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B24);
            func_ov030_020b4838(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216B4), 0);
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendMode   = 1;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer0 = 1;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendLayer1 = 0x3F;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff0 = 0xA;
            g_DisplaySettings.engineState[DISPLAY_MAIN].blendCoeff1 = 6;
            func_ov030_020834c4(mgr);
            func_ov030_02083664(mgr);
            func_ov030_020ad428(mgr);
            mgr->unk_21A6C = (s32)BgResMgr_AllocScreen(g_BgResourceManagers[1], data_0205a124,
                                                       g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].screenBase, 0);
            mgr->unk_21A78 = (s32)BgResMgr_AllocScreen(g_BgResourceManagers[1], data_0205a124,
                                                       g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].screenBase, 0);
            BgResMgr_ReleaseScreen(g_BgResourceManagers[1], (BgResource*)mgr->unk_21A6C);
            BgResMgr_ReleaseScreen(g_BgResourceManagers[1], (BgResource*)mgr->unk_21A78);
            mgr->unk_21A6C                                                    = 0;
            mgr->unk_21A78                                                    = 0;
            g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[0].priority = 2;
            g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[1].priority = 2;
            mgr->unk_21B14 += 1;
            EasyFade_FadeBothDisplays(FADER_LINEAR, 0, 0x1000);
            EasyTask_DeleteTask(&mgr->taskPool, mgr->unk_21B30);
            return;
        case 5:
            mgr->unk_21B10 = 0;
            return;
        case 6:
            g_DisplaySettings.controls[DISPLAY_SUB].layers |= 0xE;
            if (mgr->unk_21B1C != 0) {
                func_02026590(&mgr->unk_21AB0, 0x10000, (u16)mgr->unk_21B1C);
                func_02026590(&mgr->unk_21AB4, 0, (u16)mgr->unk_21B1C);
                mgr->unk_21B1C -= 1;
                {
                    s32 v0                                                 = mgr->unk_21AB0;
                    s32 v1                                                 = mgr->unk_21AB4;
                    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff0 = (u32)(v0 >> 0xC);
                    g_DisplaySettings.engineState[DISPLAY_SUB].blendCoeff1 = (u32)(v1 >> 0xC);
                }
                return;
            }
            mgr->unk_21B14 = 7;
            mgr->unk_21B1C = 0x1E;
            func_ov030_020b30f4(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D4));
            func_ov030_020bd34c(EasyTask_GetTaskById(&mgr->taskPool, mgr->unk_216D8));
            func_ov030_020c0e34(&mgr->taskPool, mgr->unk_21B30);
            return;
        case 7:
            if (mgr->unk_21B1C != 0) {
                mgr->unk_21B1C -= 1;
                return;
            }
            mgr->unk_21630                = 2;
            gSaveData.unk_3130            = 0;
            gSaveData.unk_3134            = 1;
            gSaveData.unk_3138            = func_ov030_020af2fc();
            gSaveData.unk_3142            = 0;
            gSaveData.battleDifficulty    = gSaveData.playerStats.difficulty;
            gSaveData.battleEncounters[0] = gSaveData.scanSymbols[gSaveData.scanSymbolSelection].encounter;
            gSaveData.unk_3140            = 0;
            if (gSaveData.scanSymbols[gSaveData.scanSymbolSelection].category >= 0x1F) {
                gSaveData.unk_3140 |= 1;
            }
            {
                s16 encounter = gSaveData.scanSymbols[gSaveData.scanSymbolSelection].encounter;
                if (encounter == 0x58) {
                    gSaveData.unk_3130 = 1;
                    gSaveData.unk_3134 = 0x33;
                } else if (encounter == 0x59) {
                    gSaveData.unk_3130 = 1;
                    gSaveData.unk_3134 = 0x34;
                } else if (encounter == 0x5A) {
                    gSaveData.unk_3130 = 1;
                    gSaveData.unk_3134 = 0x32;
                }
            }
            if (func_ov030_020af9b4() != 0) {
                gSaveData.unk_3130 = 2;
            }
            if (func_ov030_020afa04() != 0) {
                gSaveData.battleDifficulty = 1;
            }
            if (func_ov030_020afa58() != 0) {
                gSaveData.unk_3140 |= 1;
            }
            gSaveData.battleDropRate = func_ov030_020c105c();
            SndMgr_StartPlayingSE(SEIDX_SE_BATTLE_IN);
            DebugOvlDisp_Pop();
            return;
    }
}

s16 func_ov030_020b0418(void) {
    s16 i;

    for (i = 0; i < 0x10; i++) {
        if (gSaveData.unk_24C8[i] == -1) {
            return i;
        }
    }
    return i;
}

s32 func_ov030_020b0454(void) {
    if (gSaveData.unk_2498 == 0) {
        return 0;
    }
    if (Inventory_HasRequiredQuantity(0x2AE, 1, 0) == 0) {
        return 0;
    }
    if (SysControl.buttonState.holdButtons & 0x300) {
        return 1;
    }
    return 0;
}
