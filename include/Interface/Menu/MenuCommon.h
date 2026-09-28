#ifndef INTERFACE_MENU_MENUCOMMON_H
#define INTERFACE_MENU_MENUCOMMON_H

#include "Display.h"
#include "Engine/Core/Memory.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ResourceMgr.h"

BgResource* BgResMgr_AllocChar32(BgResMgr* mgr, void* charData, u32 charBase, u32 offset, u32 size);
BgResource* BgResMgr_AllocScreen(BgResMgr* mgr, void* screenData, u32 screenBase, u32 screenSize);

typedef struct Point {
    /* 0x0 */ s16 x;
    /* 0x2 */ s16 y;
} Point; // Size: 0x4

typedef struct {
    /* 0x00 */ Data*            data;
    /* 0x04 */ BgResource*      screenResource;
    /* 0x08 */ BgResource*      charResource;
    /* 0x0C */ PaletteResource* paletteResource;
    /* 0x10 */ u16*             charData;
    /* 0x14 */ u16*             screenMap;
    /* 0x18 */ u8*              paletteData;
} MenuBgResource; // Size: 0x1C

typedef struct {
    /* 0x00000 */ ResourceManager  resMgr;
    /* 0x11580 */ ResourceManager* prevResMgr;
    /* 0x11584 */ s32              spareDataType;
    /* 0x11588 */ s32              dataType;
    /* 0x1158C */ Heap             heap;
    /* 0x11598 */ u8               heapBuffer[0x10000];
    /* 0x21598 */ TaskPool         taskPool;
} MenuStateBase; // Size: 0x21618

#endif           // INTERFACE_MENU_MENUCOMMON_H
