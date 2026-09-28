#include "Display.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Interface/Menu/Top.h"
#include "Player/Inventory.h"
#include "Save.h"
#include "SpriteMgr.h"

// The BG loaders call PaletteMgr_AllocPalette without its prototype in scope, so palStart is passed
// through as a plain int instead of being narrowed to the s16 parameter.
#define PaletteMgr_AllocPaletteNoProto ((PaletteResource * (*)()) PaletteMgr_AllocPalette)

s32 MenuTop_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height) {
    if ((x >= left) && (x <= (left + width)) && (y >= top) && (y <= (top + height))) {
        return 1;
    }
    return 0;
}

void MenuTop_SetSpriteFrame(Sprite* sprite, s16 frame) {
    void* anim   = Data_GetPackEntryData(sprite->resourceData, 3);
    void* frames = Data_GetPackEntryData(sprite->resourceData, 2);

    Sprite_ChangeAnimation(sprite, anim, frame, frames);
}

s32 MenuTop_ClampMoney(MenuTopObject* topMenu, u8 capLevel) {
    if (capLevel == 3) {
        if (topMenu->money >= 9999999) {
            topMenu->money = 9999999;
            return 1;
        }
    } else if (capLevel == 2) {
        if (topMenu->money >= 999999) {
            topMenu->money = 999999;
            return 1;
        }
    } else if (capLevel == 1) {
        if (topMenu->money >= 99999) {
            topMenu->money = 99999;
            return 1;
        }
    } else {
        if (topMenu->money >= 99999) {
            topMenu->money = 99999;
            return 1;
        }
    }
    return 0;
}

s32 MenuTop_GetMoneyCapLevel(void) {
    if (func_02023010(0x284) != 0) {
        return 3;
    }
    if (func_02023010(0x283) != 0) {
        return 2;
    }
    if (func_02023010(0x282) != 0) {
        return 1;
    }
    return 0;
}

void MenuTop_LoadFromSave(MenuTopObject* topMenu) {
    u16 i;

    topMenu->currentLevel = gSaveData.playerStats.currentLevel;
    topMenu->maxLevel     = gSaveData.playerStats.maxLevel;
    if (topMenu->maxLevel == 100) {
        topMenu->expToNextLevel = 0;
    } else {
        topMenu->expToNextLevel = gSaveData.playerStats.expToNextLevel;
    }
    topMenu->health      = gSaveData.playerStats.baseHealth + (s16)(((s16)topMenu->currentLevel - 1) * 50 + 200);
    topMenu->difficulty  = gSaveData.playerStats.difficulty;
    topMenu->partnerAI   = gSaveData.playerStats.partnerAI;
    topMenu->unk_16      = 0;
    topMenu->money       = gSaveData.playerStats.money;
    topMenu->dropRate    = gSaveData.playerStats.dropRate;
    topMenu->currentArea = gSaveData.currentArea;
    topMenu->unk_50      = topMenu->currentArea;
    if (topMenu->currentArea < 21) {
        for (i = 0; i < 13; i++) {
            topMenu->areaBrandRanking[i] = gSaveData.brandTrends[topMenu->currentArea].ranking[i];
        }
    } else {
        for (i = 0; i < 13; i++) {
            topMenu->areaBrandRanking[i] = 0;
        }
    }
    for (i = 0; i < 4; i++) {
        topMenu->unk_2E[i] = 0;
    }
    for (i = 0; i < 21; i++) {
        topMenu->areaTopBrand[i] = gSaveData.brandTrends[i].ranking[0];
    }
    if (func_02023010(0x2CC) != 0) {
        topMenu->unlockedDifficulty = 3;
    } else if (func_02023010(0x2CA) != 0) {
        topMenu->unlockedDifficulty = 2;
    } else if (func_02023010(0x2CD) != 0) {
        topMenu->unlockedDifficulty = 1;
    } else {
        topMenu->unlockedDifficulty = 0;
    }
    topMenu->moneyCapLevel = MenuTop_GetMoneyCapLevel();
    if (topMenu->currentArea >= 41) {
        OS_WaitForever();
    }
}

void MenuTop_WriteBackToSave(MenuTopObject* topMenu) {
    gSaveData.playerStats.currentLevel = (u8)topMenu->currentLevel;
    gSaveData.playerStats.difficulty   = topMenu->difficulty;
    gSaveData.playerStats.partnerAI    = topMenu->partnerAI;
    gSaveData.playerStats.money        = topMenu->money;
    gSaveData.playerStats.dropRate     = topMenu->dropRate;
    if (gSaveData.playerStats.dropRate > 999) {
        gSaveData.playerStats.dropRate = 999;
    }
}

s32 MenuTop_IsInRestrictedArea(void) {
    if (func_02023010(0x2AE) == 0 && gSaveData.currentArea >= 37 && gSaveData.currentArea <= 40) {
        return 1;
    }
    return 0;
}

s32 MenuTop_IsEntryAvailable(s16 entry) {
    if (entry == 0) {
        if (func_02023010(0x2D0) == 0) {
            return 0;
        }
    } else if (entry == 1) {
        if (func_02023010(0x2CE) == 0) {
            return 0;
        }
    } else if (entry == 4) {
        if (func_02023010(0x2AE) == 0) {
            return 0;
        }
    } else if (entry == 5) {
        if (func_02023010(0x2CF) == 0) {
            return 0;
        }
    } else if (entry == 6) {
        if (func_02023010(0x2D0) == 0) {
            return 0;
        }
        if (MenuTop_IsInRestrictedArea() == 1) {
            return 0;
        }
    } else if (entry == 7) {
        if (MenuTop_IsInRestrictedArea() == 1) {
            return 0;
        }
    }
    return 1;
}

s16 MenuTop_GetIconButtonAtPoint(s16 x, s16 y) {
    s16 pos[2][2] = {
        {0xCD, 2},
        {0xE6, 2},
    };
    s16 size[2] = {0x17, 0x13};
    s16 i;

    for (i = 0; i < 2; i++) {
        if (MenuTop_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s16 MenuTop_GetEntryAtPoint(s16 x, s16 y) {
    s16 pos[8][2] = {
        {0x22, 0x31},
        {0x61, 0x31},
        {0xA0, 0x31},
        {0xDF, 0x31},
        {0x22, 0x67},
        {0x61, 0x67},
        {0xA0, 0x67},
        {0xDF, 0x67},
    };
    s16 size[2]   = {62, 53};
    s16 offset[2] = {-31, -26};
    s16 i;

    for (i = 0; i < 8; i++) {
        if (MenuTop_IsPointInRect(x, y, (s16)(offset[0] + pos[i][0]), (s16)(offset[1] + pos[i][1]), size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuTop_IsPointOnDifficulty(s16 x, s16 y) {
    s16 pos[2]  = {0x46, 0x9F};
    s16 size[2] = {0x3B, 0x0B};

    if (MenuTop_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

u16 MenuTop_GetDifficultyForRow(u16 row, u16 unlockedDifficulty) {
    u16 table[4][4] = {
        {1, 0xFF, 0xFF, 0xFF},
        {0,    1, 0xFF, 0xFF},
        {0,    1,    2, 0xFF},
        {0,    1,    2,    3},
    };

    return table[unlockedDifficulty][row];
}

s16 MenuTop_GetDifficultyRowAtPoint(s16 x, s16 y, u16 unlockedDifficulty) {
    s16 pos[4][2] = {
        {0x45, 0x7E},
        {0x45, 0x5E},
        {0x45, 0x3E},
        {0x45, 0x1E},
    };
    s16 size[2] = {0x3B, 0x20};
    s16 i;

    for (i = 0; i < 4; i++) {
        if (MenuTop_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1 &&
            MenuTop_GetDifficultyForRow(i, unlockedDifficulty) != 0xFF)
        {
            return i;
        }
    }
    return -1;
}

s32 MenuTop_IsPointOnPartnerAI(s16 x, s16 y) {
    s16 pos[2]  = {0xBC, 0x9F};
    s16 size[2] = {0x3B, 0x0B};

    if (MenuTop_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s16 MenuTop_GetPartnerAIRowAtPoint(s16 x, s16 y) {
    s16 pos[4][2] = {
        {0xBD, 0x1E},
        {0xBD, 0x3E},
        {0xBD, 0x5E},
        {0xBD, 0x7E},
    };
    s16 size[2] = {0x3B, 0x20};
    s16 i;

    for (i = 0; i < 4; i++) {
        if (MenuTop_IsPointInRect(x, y, pos[i][0], pos[i][1], size[0], size[1]) == 1) {
            return i;
        }
    }
    return -1;
}

s32 MenuTop_IsPointOnLevelGauge(s16 x, s16 y) {
    s16 pos[2]  = {0x24, 0x91};
    s16 size[2] = {0xCC, 0x0E};

    if (MenuTop_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuTop_IsPointOnLevelDown(s16 x, s16 y) {
    s16 pos[2]  = {0x15, 0x94};
    s16 size[2] = {0x10, 0x08};

    if (MenuTop_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s32 MenuTop_IsPointOnLevelUp(s16 x, s16 y) {
    s16 pos[2]  = {0xEF, 0x94};
    s16 size[2] = {0x10, 0x08};

    if (MenuTop_IsPointInRect(x, y, pos[0], pos[1], size[0], size[1]) == 1) {
        return 1;
    }
    return 0;
}

s16 MenuTop_GetHelpButtonAtPoint(s16 x, s16 y) {
    // Never read. Their initializer templates still land in .rodata, between the templates of the
    // other hit tests, which is what pins them to this function.
    s16 unusedPos[2][2] = {
        {0x7B, 5},
        {0x96, 5},
    };
    s16 unusedSize[2] = {0x1A, 0x0D};
    s16 pos[3][2]     = {
        {0x20, 0x38},
        {0xC0, 0x38},
        {0x50, 0x73},
    };
    s16 size[3][2] = {
        {0x20, 0x40},
        {0x20, 0x40},
        {0x60, 0x14},
    };
    s16 i;

    for (i = 0; i < 3; i++) {
        if (MenuTop_IsPointInRect(x, y, pos[i][0], pos[i][1], size[i][0], size[i][1]) == 1) {
            return i;
        }
    }
    return -1;
}

void MenuTop_LoadBgResource(MenuTopResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &MenuTop_BinIdentifiers[binIndex]);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, 2);
    res->paletteData = Data_GetPackEntryData(res->data, 3);
    if (engine == DISPLAY_MAIN) {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void MenuTop_LoadBgResourceIndexed(MenuTopResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                   s32 screenIndex, s32 palIndex) {
    res->data        = DatMgr_LoadRawData(1, NULL, 0, &MenuTop_BinIdentifiers[binIndex]);
    res->charData    = Data_GetPackEntryData(res->data, 1);
    res->screenMap   = Data_GetPackEntryData(res->data, screenIndex + 2);
    res->paletteData = Data_GetPackEntryData(res->data, palIndex + 3);
    if (engine == DISPLAY_MAIN) {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_MAIN], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_MAIN], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        u32 charSize = (*(u32*)res->charData & ~0xFF) >> 8;
        if ((*(u8*)res->charData & 0xF0) == 0) {
            charSize -= 4;
        }
        res->charResource =
            BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], res->charData,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].charBase, 0, charSize);
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
        res->paletteResource =
            PaletteMgr_AllocPaletteNoProto(g_PaletteManagers[DISPLAY_SUB], res->paletteData, 0, palStart, palCount);
        PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
}

void MenuTop_LoadBgScreen(MenuTopResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex) {
    res->screenMap = Data_GetPackEntryData(data, screenIndex);
    if (engine == DISPLAY_MAIN) {
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_MAIN].bgSettings[layer].screenSizeText);
    } else {
        res->screenResource =
            BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenMap,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenBase,
                                 g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[layer].screenSizeText);
    }
}

void MenuTop_ReleaseBgResource(MenuTopResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], res->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], res->paletteResource);
    } else {
        BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], res->charResource);
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
        PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], res->paletteResource);
    }
    DatMgr_ReleaseData(res->data);
}

void MenuTop_ReleaseBgScreen(MenuTopResource* res, s32 engine) {
    if (engine == DISPLAY_MAIN) {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], res->screenResource);
    } else {
        BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], res->screenResource);
    }
}

void MenuTop_ReloadBgResource(MenuTopResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount) {
    MenuTop_ReleaseBgResource(res, engine);
    MenuTop_LoadBgResource(res, engine, layer, binIndex, palStart, palCount);
}

void MenuTop_ClearBgResource(MenuTopResource* res) {
    res->data            = NULL;
    res->screenResource  = NULL;
    res->charResource    = NULL;
    res->paletteResource = NULL;
    res->screenMap       = NULL;
    res->charData        = NULL;
    res->paletteData     = NULL;
}

void MenuTop_LoadBackgrounds(MenuTopObject* topMenu) {
    s32              i;
    MenuTopResource* mainRes = &topMenu->resources[0];
    MenuTopResource* subRes  = &topMenu->resources[4];

    for (i = 0; i < 4; i++) {
        MenuTop_ClearBgResource(mainRes);
        MenuTop_ClearBgResource(subRes);
        mainRes += 1;
        subRes += 1;
    }

    MenuTop_LoadBgResource(&topMenu->resources[5], 1, 1, 10, 15, 1);
    MenuTop_LoadBgResource(&topMenu->resources[6], 1, 2, 0, 1, 2);
    MenuTop_LoadBgResource(&topMenu->resources[7], 1, 3, 1, 0, 1);
    MenuTop_LoadBgScreen(&topMenu->resources[4], topMenu->resources[6].data, 1, 0, 4);
    MenuTop_LoadBgResource(&topMenu->resources[0], 0, 0, 9, 15, 1);
    if (topMenu->currentArea <= 20) {
        MenuTop_LoadBgResource(&topMenu->resources[3], 0, 3, 5, 0, 5);
    } else {
        MenuTop_LoadBgResourceIndexed(&topMenu->resources[3], 0, 3, 5, 0, 5, 4, 0);
    }
    MenuTop_LoadBgScreen(&topMenu->resources[1], topMenu->resources[3].data, 0, 1, 4);
    MenuTop_LoadBgScreen(&topMenu->resources[2], topMenu->resources[3].data, 0, 2, 5);
}

void MenuTop_UpdateBackgrounds(MenuTopObject* topMenu) {
    return;
}

void MenuTop_ReleaseBackgrounds(MenuTopObject* topMenu) {
    MenuTop_ReleaseBgScreen(&topMenu->resources[4], 1);
    MenuTop_ReleaseBgResource(&topMenu->resources[5], 1);
    MenuTop_ReleaseBgResource(&topMenu->resources[6], 1);
    MenuTop_ReleaseBgResource(&topMenu->resources[7], 1);
    MenuTop_ReleaseBgResource(&topMenu->resources[0], 0);
    MenuTop_ReleaseBgScreen(&topMenu->resources[1], 0);
    MenuTop_ReleaseBgScreen(&topMenu->resources[2], 0);
    MenuTop_ReleaseBgResource(&topMenu->resources[3], 0);
}
