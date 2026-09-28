#ifndef INTERFACE_MENU_TOP_H
#define INTERFACE_MENU_TOP_H

#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "SpriteMgr.h"

typedef struct Point {
    s16 x;
    s16 y;
} Point;

typedef struct {
    /* 0x00 */ Data*            data;
    /* 0x04 */ BgResource*      screenResource;
    /* 0x08 */ BgResource*      charResource;
    /* 0x0C */ PaletteResource* paletteResource;
    /* 0x10 */ u16*             charData;
    /* 0x14 */ u16*             screenMap;
    /* 0x18 */ u8*              paletteData;
} MenuTopResource; // Size: 0x1C

typedef struct {
    /* 0x000 */ u16             flags;
    /* 0x002 */ u8              iconPressed[2];
    /* 0x004 */ u8              pressTimer;
    /* 0x005 */ u8              selectedEntry;
    /* 0x006 */ u16             currentLevel;
    /* 0x008 */ u16             maxLevel;
    /* 0x00A */ char            unk_0A[0x0C - 0x0A];
    /* 0x00C */ u32             expToNextLevel;
    /* 0x010 */ u16             health;
    /* 0x012 */ u16             difficulty;
    /* 0x014 */ u16             partnerAI;
    /* 0x016 */ u16             unk_16;
    /* 0x018 */ u32             money;
    /* 0x01C */ u32             dropRate;
    /* 0x020 */ u8              areaBrandRanking[13];
    /* 0x02D */ char            unk_2D;
    /* 0x02E */ u16             unk_2E[4];
    /* 0x036 */ u8              areaTopBrand[21];
    /* 0x04B */ char            unk_4B;
    /* 0x04C */ u16             popupOpen;
    /* 0x04E */ u16             currentArea;
    /* 0x050 */ u16             unk_50;
    /* 0x052 */ s16             levelKnobX;
    /* 0x054 */ s16             levelKnobY;
    /* 0x056 */ u16             upperPage;
    /* 0x058 */ u16             unlockedDifficulty;
    /* 0x05A */ u8              unk_5A;
    /* 0x05B */ u8              unk_5B;
    /* 0x05C */ u8              moneyCapLevel;
    /* 0x05D */ u8              helpPressed[3];
    /* 0x060 */ u8              helpPage;
    /* 0x061 */ char            unk_61;
    /* 0x062 */ s16             blinkTimer;
    /* 0x064 */ s16             blinkPhase;
    /* 0x066 */ s16             blinkOn;
    /* 0x068 */ MenuTopResource resources[8];
} MenuTopObject; // Size: 0x148

extern const Point         MenuTop_AreaMapPos[];
extern const BinIdentifier MenuTop_BinIdentifiers[14];

// TopData.c
s32  MenuTop_IsPointInRect(s32 x, s32 y, s32 left, s32 top, s16 width, s16 height);
void MenuTop_SetSpriteFrame(Sprite* sprite, s16 frame);
s32  MenuTop_ClampMoney(MenuTopObject* topMenu, u8 capLevel);
s32  MenuTop_GetMoneyCapLevel(void);
void MenuTop_LoadFromSave(MenuTopObject* topMenu);
void MenuTop_WriteBackToSave(MenuTopObject* topMenu);
s32  MenuTop_IsInRestrictedArea(void);
s32  MenuTop_IsEntryAvailable(s16 entry);
s16  MenuTop_GetIconButtonAtPoint(s16 x, s16 y);
s16  MenuTop_GetEntryAtPoint(s16 x, s16 y);
s32  MenuTop_IsPointOnDifficulty(s16 x, s16 y);
u16  MenuTop_GetDifficultyForRow(u16 row, u16 unlockedDifficulty);
s16  MenuTop_GetDifficultyRowAtPoint(s16 x, s16 y, u16 unlockedDifficulty);
s32  MenuTop_IsPointOnPartnerAI(s16 x, s16 y);
s16  MenuTop_GetPartnerAIRowAtPoint(s16 x, s16 y);
s32  MenuTop_IsPointOnLevelGauge(s16 x, s16 y);
s32  MenuTop_IsPointOnLevelDown(s16 x, s16 y);
s32  MenuTop_IsPointOnLevelUp(s16 x, s16 y);
s16  MenuTop_GetHelpButtonAtPoint(s16 x, s16 y);
void MenuTop_LoadBgResource(MenuTopResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void MenuTop_LoadBgResourceIndexed(MenuTopResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount,
                                   s32 screenIndex, s32 palIndex);
void MenuTop_LoadBgScreen(MenuTopResource* res, Data* data, s32 engine, s32 layer, s32 screenIndex);
void MenuTop_ReleaseBgResource(MenuTopResource* res, s32 engine);
void MenuTop_ReleaseBgScreen(MenuTopResource* res, s32 engine);
void MenuTop_ReloadBgResource(MenuTopResource* res, s32 engine, s32 layer, s32 binIndex, s32 palStart, u32 palCount);
void MenuTop_ClearBgResource(MenuTopResource* res);
void MenuTop_LoadBackgrounds(MenuTopObject* topMenu);
void MenuTop_UpdateBackgrounds(MenuTopObject* topMenu);
void MenuTop_ReleaseBackgrounds(MenuTopObject* topMenu);

s32 MenuTop_pointer_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_luckNum_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_luckStar_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_lvGauge_CreateTask(TaskPool* pool, s32 dataType, void* topMenu);
s32 MenuTop_select_CreateTask(TaskPool* pool, s32 dataType, s32 topMenu, u16 entry);
s32 MenuTop_icon_CreateTask(TaskPool* pool, s32 dataType, void* topMenu);
s32 MenuTop_nameD_CreateTask(TaskPool* pool, s32 dataType, s32 topMenu);
s32 MenuTop_numLV_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_numExpNext_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_numHP_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_numMoney_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_drawDiff_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_drawPtrAI_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_selDiff_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_selPtrAI_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_nameU_CreateTask(TaskPool* pool, s32 dataType, s32 topMenu);
s32 MenuTop_rankNumU_CreateTask(TaskPool* pool, s32 dataType, s16 positionIndex, s16 value, s32 topMenu);
s32 MenuTop_brdLogoU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_textScrU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_nekuU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_iconU_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu, u16 area, u16 iconFrame);
s32 MenuTop_helpCurU_CreateTask(TaskPool* pool, s32 dataType, s32 topMenu);
s32 MenuTop_textScr_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);
s32 MenuTop_mapTipU_CreateTask(TaskPool* pool, s32 dataType, u16 area, MenuTopObject* topMenu);
s32 MenuTop_mapTipBrdU_CreateTask(TaskPool* pool, s32 dataType, u16 area, MenuTopObject* topMenu);
s32 MenuTop_helpCur_CreateTask(TaskPool* pool, s32 dataType, MenuTopObject* topMenu);

#endif
