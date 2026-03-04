#include "Engine/Resources/ResourceMgr.h"
#include "Display.h"

static ResourceManager* ActiveResourceManager;

ResourceManager* ResourceMgr_ReinitManagers(ResourceManager* mgr) {
    ResourceManager* prev = ActiveResourceManager;
    ActiveResourceManager = mgr;

    if (mgr == NULL) {
        BgResMgr_Init(NULL, DISPLAY_MAIN);
        BgResMgr_Init(NULL, DISPLAY_SUB);
        ObjResMgr_Init(NULL, DISPLAY_MAIN);
        ObjResMgr_Init(NULL, DISPLAY_SUB);
        ObjResMgr_Init(NULL, DISPLAY_EXTENDED);
        PaletteMgr_Init(NULL, DISPLAY_MAIN);
        PaletteMgr_Init(NULL, DISPLAY_SUB);
        PaletteMgr_Init(NULL, DISPLAY_EXTENDED);
    } else {
        BgResMgr_Init(&mgr->mainBgMgr, DISPLAY_MAIN);
        BgResMgr_Init(&mgr->subBgMgr, DISPLAY_SUB);
        ObjResMgr_Init(&mgr->objResMgr0, DISPLAY_MAIN);
        ObjResMgr_Init(&mgr->objResMgr1, DISPLAY_SUB);
        ObjResMgr_Init(&mgr->objResMgr2, DISPLAY_EXTENDED);
        PaletteMgr_Init(&mgr->palMgr0, DISPLAY_MAIN);
        PaletteMgr_Init(&mgr->palMgr1, DISPLAY_SUB);
        PaletteMgr_Init(&mgr->palMgr2, DISPLAY_EXTENDED);
    }
    return prev;
}

ResourceManager* ResourceMgr_SetActiveManagers(ResourceManager* mgr) {
    ResourceManager* prev = ActiveResourceManager;
    if (mgr == prev) {
        return prev;
    }
    ActiveResourceManager = mgr;

    if (mgr == NULL) {
        BgResMgr_Init(NULL, DISPLAY_MAIN);
        BgResMgr_Init(NULL, DISPLAY_SUB);
        ObjResMgr_Init(NULL, DISPLAY_MAIN);
        ObjResMgr_Init(NULL, DISPLAY_SUB);
        ObjResMgr_Init(NULL, DISPLAY_EXTENDED);
        PaletteMgr_Init(NULL, DISPLAY_MAIN);
        PaletteMgr_Init(NULL, DISPLAY_SUB);
        PaletteMgr_Init(NULL, DISPLAY_EXTENDED);
    } else {
        g_BgResourceManagers[mgr->mainBgMgr.engine]   = &mgr->mainBgMgr;
        g_BgResourceManagers[mgr->subBgMgr.engine]    = &mgr->subBgMgr;
        g_ObjResourceManagers[mgr->objResMgr0.engine] = &mgr->objResMgr0;
        g_ObjResourceManagers[mgr->objResMgr1.engine] = &mgr->objResMgr1;
        g_ObjResourceManagers[mgr->objResMgr2.engine] = &mgr->objResMgr2;
        g_PaletteManagers[mgr->palMgr0.engine]        = &mgr->palMgr0;
        g_PaletteManagers[mgr->palMgr1.engine]        = &mgr->palMgr1;
        g_PaletteManagers[mgr->palMgr2.engine]        = &mgr->palMgr2;
    }
    return prev;
}

void ResourceMgr_ReleaseResource(Resource* resource) {
    DisplayEngine engine;

    switch (resource->flags & 0x60) {
        case 0x20:
            engine = DISPLAY_MAIN;
            break;
        case 0x40:
            engine = DISPLAY_SUB;
            break;
        case 0x60:
        default:
            engine = DISPLAY_EXTENDED;
            break;
    }

    switch (resource->flags & 7) {
        case 4:
            BgResMgr_ReleaseChar(g_BgResourceManagers[engine], (BgResource*)resource);
            break;
        case 5:
            BgResMgr_ReleaseScreen(g_BgResourceManagers[engine], (BgResource*)resource);
            break;
        case 6:
            ObjResMgr_ReleaseResource(g_ObjResourceManagers[engine], (ObjResource*)resource);
            break;
        case 7:
            PaletteMgr_ReleaseResource(g_PaletteManagers[engine], (PaletteResource*)resource);
            break;
    }
}
