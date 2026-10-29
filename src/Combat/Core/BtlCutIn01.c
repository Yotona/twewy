#include "Combat/Core/CombatApi.h"
#include "Display.h"
#include "EasyFade.h"
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>
#include <stddef.h>

typedef struct BtlCutIn01 BtlCutIn01;
typedef s32 (*Cutin01Step)(BtlCutIn01*);
typedef struct Cutin01Scene {
    /* 0x00 */ Data*            pack;
    /* 0x04 */ PaletteResource* palettes[2]; // sub, main
    /* 0x0C */ BgResource*      characters[2];
    /* 0x14 */ ScreenMapEntry   subMap;
    /* 0x3C */ void*            subTiles;
    /* 0x40 */ u32              reserved40;
    /* 0x44 */ ScreenMapEntry   mainMap;
    /* 0x6C */ void*            mainTiles[2];
    /* 0x74 */ s32              x;
    /* 0x78 */ CombatSprite     sprite;
    /* 0xD8 */ s16              y;
    /* 0xDA */ u16              reservedDA;
} Cutin01Scene;
struct BtlCutIn01 {
    /* 0x000 */ const Cutin01Step* program;
    /* 0x004 */ u32                reserved004;
    /* 0x008 */ s32                active;
    /* 0x00C */ Cutin01Scene       scenes[2];
    /* 0x1C4 */ u16                kind;
    /* 0x1C6 */ u16                reserved1C6;
    /* 0x1C8 */ Data*              overlayPack;
    /* 0x1CC */ u32                reserved1CC[2];
    /* 0x1D4 */ PaletteResource*   overlayPalettes[2]; // main, sub
    /* 0x1DC */ void*              overlayMainTiles[3];
    /* 0x1E8 */ void*              overlaySubTiles[3];
    /* 0x1F4 */ ScreenMapEntry     overlaySubMap;
    /* 0x21C */ ScreenMapEntry     overlayMainMap;
    /* 0x244 */ s32                overlaySubY;
    /* 0x248 */ s32                overlayMainY;
    /* 0x24C */ u32                reserved24C;
    /* 0x250 */ s16                timer;
    /* 0x252 */ s16                programIndex;
    /* 0x254 */ u16                elapsed;
    /* 0x256 */ u16                delay;
    /* 0x258 */ u32                reserved258;
    /* 0x25C */ s32                speed;
    /* 0x260 */ void (*update)(BtlCutIn01*);
    /* 0x264 */ u32 reserved264;
    /* 0x268 */ s16 subTimer;
    /* 0x26A */ s16 subPhase;
    /* 0x26C */ s16 subActive;
    /* 0x26E */ u16 reserved26E;
};
typedef struct Cutin01Resources {
    u16 pack, subPalette, subCharacters, mainCharacters, subMap, mainMap, extraMainMap;
    u16 reserved[9];
} Cutin01Resources;
typedef struct Cutin01SpriteLayout {
    u16 paletteSlots[3], y[3];
} Cutin01SpriteLayout;
typedef char check_Cutin01Scene[(sizeof(Cutin01Scene) == 0xDC) ? 1 : -1];
typedef char check_Cutin01[(sizeof(BtlCutIn01) == 0x270) ? 1 : -1];
typedef char check_Cutin01Resources[(sizeof(Cutin01Resources) == 0x20) ? 1 : -1];
typedef char check_Cutin01_kind[(offsetof(BtlCutIn01, kind) == 0x1C4) ? 1 : -1];
typedef char check_Cutin01_timer[(offsetof(BtlCutIn01, timer) == 0x250) ? 1 : -1];
typedef char check_Cutin01_subActive[(offsetof(BtlCutIn01, subActive) == 0x26C) ? 1 : -1];

extern const BinIdentifier            data_ov003_020d7828, data_ov003_020dd7ac;
extern const Cutin01Resources         data_ov003_020dd8f8[4];
extern const Cutin01SpriteLayout      data_ov003_020dd890;
extern const Ov003CutinPaletteEntries data_ov003_020dd89c;
extern const SpriteAnimEntry          data_ov003_020dd8b8[2], data_ov003_020dd8c8[6];
extern const TaskStages               data_ov003_020dd8a8;
extern Cutin01Step                    data_ov003_020e535c[15];
extern u8                             data_ov003_020e71d9;
extern void                           func_0200d858(ScreenMapEntry*, s32, s32, s32);
extern void                           func_0200d954(s32, s32);
extern void                           func_ov003_020bd2c8(u16, s32);
extern void                           func_ov003_020bd370(u16, s32);

void func_ov003_020be71c(BtlCutIn01* self) {
    self->programIndex++;
    self->subTimer  = 0;
    self->subPhase  = 0;
    self->subActive = 0;
}
void func_ov003_020be740(BtlCutIn01* self, const Cutin01Step* program) {
    self->program = program;
}
s32 func_ov003_020be748(BtlCutIn01* self) {
    EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
    return 1;
}

// Nonmatching: display-state store scheduling.
s32 func_ov003_020be764(BtlCutIn01* self) {
    g_DisplaySettings.controls[0].windows |= 4;
    g_DisplaySettings.engineState[0].windowObj            = 8;
    g_DisplaySettings.engineState[0].windowObjEffects     = 0;
    g_DisplaySettings.engineState[0].windowOutside        = 0x1F;
    g_DisplaySettings.engineState[0].windowOutsideEffects = 0;
    g_DisplaySettings.controls[1].windows |= 4;
    g_DisplaySettings.engineState[1].windowObj                    = 8;
    g_DisplaySettings.engineState[1].windowObjEffects             = 0;
    g_DisplaySettings.engineState[1].windowOutside                = 0x1F;
    g_DisplaySettings.engineState[1].windowOutsideEffects         = 0;
    g_DisplaySettings.engineState[0].blendMode                    = 1;
    g_DisplaySettings.engineState[0].blendLayer0                  = 0;
    g_DisplaySettings.engineState[0].blendLayer1                  = 0x21;
    g_DisplaySettings.engineState[1].blendMode                    = 1;
    g_DisplaySettings.engineState[1].blendLayer0                  = 0;
    g_DisplaySettings.engineState[1].blendLayer1                  = 0x21;
    g_DisplaySettings.engineState[1].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[1].bgSettings[1].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[1].bgSettings[1].screenBase     = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[1].bgSettings[1].extPlttSlot    = 0;
    REG_BG1CNT_SUB                                                = (u16)((REG_BG1CNT_SUB & 0x43) | 0x4004);
    g_DisplaySettings.engineState[1].bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[1].bgSettings[2].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[1].bgSettings[2].screenBase     = 2;
    g_DisplaySettings.engineState[1].bgSettings[2].charBase       = 3;
    g_DisplaySettings.engineState[1].bgSettings[2].extPlttSlot    = 1;
    REG_BG2CNT_SUB                                                = (u16)((REG_BG2CNT_SUB & 0x43) | 0x420C);
    g_DisplaySettings.engineState[1].bgSettings[3].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[1].bgSettings[3].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[1].bgSettings[3].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[1].bgSettings[3].screenBase     = 4;
    g_DisplaySettings.engineState[1].bgSettings[3].charBase       = 1;
    g_DisplaySettings.engineState[1].bgSettings[3].extPlttSlot    = 1;
    REG_BG3CNT_SUB                                                = (u16)((REG_BG3CNT_SUB & 0x43) | 0x4404);
    g_DisplaySettings.engineState[0].bgSettings[1].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[0].bgSettings[1].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[1].screenBase     = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].charBase       = 1;
    g_DisplaySettings.engineState[0].bgSettings[1].extPlttSlot    = 0;
    REG_BG1CNT                                                    = (REG_BG1CNT & 0x43) | 0x4004;
    g_DisplaySettings.engineState[0].bgSettings[2].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[2].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[0].bgSettings[2].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[2].screenBase     = 2;
    g_DisplaySettings.engineState[0].bgSettings[2].charBase       = 3;
    g_DisplaySettings.engineState[0].bgSettings[2].extPlttSlot    = 1;
    REG_BG2CNT                                                    = (u16)((REG_BG2CNT & 0x43) | 0x420C);
    g_DisplaySettings.engineState[0].bgSettings[3].bgMode         = DISPLAY_BGMODE_TEXT;
    g_DisplaySettings.engineState[0].bgSettings[3].screenSizeText = GX_BG_SIZE_TEXT_512x256;
    g_DisplaySettings.engineState[0].bgSettings[3].colorMode      = GX_BG_COLORS_16;
    g_DisplaySettings.engineState[0].bgSettings[3].screenBase     = 4;
    g_DisplaySettings.engineState[0].bgSettings[3].charBase       = 1;
    g_DisplaySettings.engineState[0].bgSettings[3].extPlttSlot    = 1;
    REG_BG3CNT                                                    = (u16)((REG_BG3CNT & 0x43) | 0x4404);
    g_DisplaySettings.engineState[0].bgSettings[0].priority       = 0;
    g_DisplaySettings.engineState[0].bgSettings[1].priority       = 1;
    g_DisplaySettings.engineState[0].bgSettings[2].priority       = 2;
    g_DisplaySettings.engineState[0].bgSettings[3].priority       = 3;
    g_DisplaySettings.engineState[1].bgSettings[0].priority       = 0;
    g_DisplaySettings.engineState[1].bgSettings[1].priority       = 1;
    g_DisplaySettings.engineState[1].bgSettings[2].priority       = 2;
    g_DisplaySettings.engineState[1].bgSettings[3].priority       = 3;
    g_DisplaySettings.controls[0].layers                          = 0x1E;
    g_DisplaySettings.controls[1].layers                          = 0x1E;
    Display_Commit();
    func_ov003_020be71c(self);
    return 0;
}

s32 func_ov003_020be974(BtlCutIn01* self) {
    func_ov003_020be71c(self);
    return 2;
}
s32 func_ov003_020be984(BtlCutIn01* self) {
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->scenes[0].palettes[1]);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], self->scenes[0].palettes[0]);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->scenes[1].palettes[1]);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], self->scenes[1].palettes[0]);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->overlayPalettes[0]);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], self->overlayPalettes[1]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->scenes[0].characters[1]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], self->scenes[0].characters[0]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->scenes[1].characters[1]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], self->scenes[1].characters[0]);
    CombatSprite_Release(&self->scenes[0].sprite);
    CombatSprite_Release(&self->scenes[1].sprite);
    func_0200d954(0, 1);
    func_0200d954(0, 2);
    func_0200d954(0, 3);
    func_0200d954(1, 1);
    func_0200d954(1, 2);
    func_0200d954(1, 3);
    DatMgr_ReleaseData(self->overlayPack);
    DatMgr_ReleaseData(self->scenes[1].pack);
    DatMgr_ReleaseData(self->scenes[0].pack);
    func_ov003_020be71c(self);
    return 0;
}
s32 func_ov003_020beab4(BtlCutIn01* self) {
    EasyFade_FadeBothDisplays(0, 0, 0x800);
    func_ov003_020be71c(self);
    return 0;
}
s32 func_ov003_020beadc(BtlCutIn01* self) {
    EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
    func_ov003_020be71c(self);
    return 0;
}

void func_ov003_020beb04(BtlCutIn01* self, u16 kind) {
    self->overlayPack                = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, 2, 0);
    Ov003CutinPaletteEntries entries = data_ov003_020dd89c;
    void*                    palette = Data_GetPackEntryData(self->overlayPack, entries.entries[kind]);
    self->overlaySubTiles[0]         = Ov003Cutin_Tiles(self->overlayPack, 5);
    self->overlaySubTiles[1]         = Ov003Cutin_Tiles(self->overlayPack, 6);
    self->overlaySubTiles[2]         = Ov003Cutin_Tiles(self->overlayPack, 7);
    self->overlayPalettes[1]         = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0xF, 1);
    func_0200d1d8(&self->overlaySubMap, 1, 3, NULL, self->overlaySubTiles, 1, 3);
    PaletteMgr_Flush(g_PaletteManagers[1], self->overlayPalettes[1]);
    func_0200d858(&self->overlaySubMap, 1, 1, 0);
    self->overlaySubY         = 0;
    palette                   = Data_GetPackEntryData(self->overlayPack, 4);
    self->overlayMainTiles[0] = Ov003Cutin_Tiles(self->overlayPack, 1);
    self->overlayMainTiles[1] = Ov003Cutin_Tiles(self->overlayPack, 2);
    self->overlayMainTiles[2] = Ov003Cutin_Tiles(self->overlayPack, 3);
    self->overlayPalettes[0]  = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 0xF, 1);
    func_0200d1d8(&self->overlayMainMap, 0, 3, NULL, self->overlayMainTiles, 1, 3);
    PaletteMgr_Flush(g_PaletteManagers[0], self->overlayPalettes[0]);
    func_0200d858(&self->overlayMainMap, 1, 1, 0);
    self->overlayMainY = 0x200000;
    Display_SetBGOffset(self->overlaySubMap.engineId, self->overlaySubMap.bgLayer, 0, self->overlaySubY);
    Display_SetBGOffset(self->overlayMainMap.engineId, self->overlayMainMap.bgLayer, 0, self->overlayMainY);
}

// Nonmatching: resource loading and map/sprite register allocation.
void func_ov003_020bee48(BtlCutIn01* self, u16 kind) {
    const Cutin01Resources* variant = &data_ov003_020dd8f8[kind + 1];
    const Cutin01Resources* base    = &data_ov003_020dd8f8[0];
    Cutin01Scene*           first   = &self->scenes[0];
    Cutin01Scene*           second  = &self->scenes[1];
    first->pack                     = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, 0xA, 0);
    void* palette                   = Data_GetPackEntryData(first->pack, 5);
    void* characters                = Data_GetPackEntryData(first->pack, base->subCharacters);
    first->subTiles                 = Ov003Cutin_Tiles(first->pack, base->subMap);
    first->palettes[0]              = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0, 7);
    first->characters[0]            = BgResMgr_AllocChar32(g_BgResourceManagers[1], characters,
                                                           g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x8000);
    func_0200d1d8(&first->subMap, 1, 1, NULL, &first->subTiles, 1, 1);
    PaletteMgr_Flush(g_PaletteManagers[1], first->palettes[0]);
    func_0200d858(&first->subMap, 0, 0, 0);
    palette              = Data_GetPackEntryData(first->pack, base->subPalette);
    characters           = Data_GetPackEntryData(first->pack, base->mainCharacters);
    first->mainTiles[0]  = Ov003Cutin_Tiles(first->pack, base->mainMap);
    first->palettes[1]   = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 0, 7);
    first->characters[1] = BgResMgr_AllocChar32(g_BgResourceManagers[0], characters,
                                                g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x8000);
    func_0200d1d8(&first->mainMap, 0, 1, NULL, first->mainTiles, 1, 1);
    PaletteMgr_Flush(g_PaletteManagers[0], first->palettes[1]);
    func_0200d858(&first->mainMap, 0, 0, 0);
    CombatSprite_LoadFromTable(1, &first->sprite, &data_ov003_020d7828, data_ov003_020dd8b8, 0, 4, 0x50);
    CombatSprite_SetAnimFromTable(&first->sprite, 0, 1);
    CombatSprite_SetFlip(&first->sprite, 0);
    first->x              = -0x100000;
    second->pack          = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, variant->pack, 0);
    palette               = Data_GetPackEntryData(second->pack, variant->subPalette);
    characters            = Data_GetPackEntryData(second->pack, variant->subCharacters);
    second->subTiles      = Ov003Cutin_Tiles(second->pack, variant->subMap);
    second->palettes[0]   = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 7, 7);
    second->characters[0] = BgResMgr_AllocChar32(g_BgResourceManagers[1], characters,
                                                 g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x8000);
    func_0200d1d8(&second->subMap, 1, 2, NULL, &second->subTiles, 1, 1);
    PaletteMgr_Flush(g_PaletteManagers[1], second->palettes[0]);
    func_0200d858(&second->subMap, 0, 0, 0);
    palette               = Data_GetPackEntryData(second->pack, variant->subPalette);
    characters            = Data_GetPackEntryData(second->pack, variant->mainCharacters);
    second->mainTiles[0]  = Ov003Cutin_Tiles(second->pack, variant->mainMap);
    second->palettes[1]   = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 7, 7);
    second->characters[1] = BgResMgr_AllocChar32(g_BgResourceManagers[0], characters,
                                                 g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, 0x8000);
    if (kind == 2) {
        second->mainTiles[1] = Ov003Cutin_Tiles(second->pack, variant->extraMainMap);
        func_0200d1d8(&second->mainMap, 0, 2, NULL, second->mainTiles, 2, 1);
    } else
        func_0200d1d8(&second->mainMap, 0, 2, NULL, second->mainTiles, 1, 1);
    PaletteMgr_Flush(g_PaletteManagers[0], second->palettes[1]);
    func_0200d858(&second->mainMap, 0, 0, 0);
    func_ov003_020beb04(self, kind);
    Cutin01SpriteLayout layout = data_ov003_020dd890;
    second->y                  = layout.y[kind];
    CombatSprite_LoadFromTable(1, &second->sprite, &data_ov003_020d7828, data_ov003_020dd8c8, (u16)(kind * 2),
                               layout.paletteSlots[kind], 0x50);
    CombatSprite_SetAnimFromTable(&second->sprite, self->kind * 2, 1);
    CombatSprite_SetFlip(&second->sprite, 0);
    second->x = 0x100000;
}

s32 func_ov003_020bf4b0(BtlCutIn01* self) {
    func_ov003_020bee48(self, self->kind);
    func_ov003_020be71c(self);
    return 2;
}
s32 func_ov003_020bf4d4(BtlCutIn01* self) {
    self->speed  = 0x6000;
    self->delay  = 0;
    self->active = 1;
    func_ov003_020be71c(self);
    return 2;
}
s32 func_ov003_020bf500(BtlCutIn01* self) {
    self->scenes[0].x += self->speed;
    self->scenes[1].x -= self->speed;
    if (self->scenes[0].x > 0x16000) {
        if (self->delay == 0) {
            self->delay = 1;
            self->speed = 0x2000;
        } else if (self->speed > 0x333)
            self->speed -= 0xCD;
    }
    if (self->scenes[0].x >= 0x40000) {
        self->scenes[0].x = 0x40000;
        self->scenes[1].x = -0x40000;
        func_ov003_020be71c(self);
    }
    return 0;
}
s32 func_ov003_020bf594(BtlCutIn01* self) {
    CombatSprite_SetAnimFromTable(&self->scenes[0].sprite, 1, 1);
    CombatSprite_SetAnimFromTable(&self->scenes[1].sprite, self->kind * 2 + 1, 1);
    self->timer = 0x1E;
    func_ov003_020be71c(self);
    return 2;
}
s32 func_ov003_020bf5ec(BtlCutIn01* self) {
    self->timer--;
    if (self->timer == 0)
        func_ov003_020be71c(self);
    return 0;
}
s32 func_ov003_020bf618(BtlCutIn01* self) {
    self->delay = 0x1E;
    func_ov003_020be71c(self);
    return 2;
}
s32 func_ov003_020bf634(BtlCutIn01* self) {
    self->delay = 5;
    func_ov003_020be71c(self);
    return 2;
}
s32 func_ov003_020bf650(BtlCutIn01* self) {
    if (self->delay != 0)
        self->delay--;
    else
        func_ov003_020be71c(self);
    return 0;
}

s32 func_ov003_020bf678(TaskPool* pool, Task* task, void* args) {
    BtlCutIn01* self = task->data;
    MI_CpuSet(self, 0, sizeof(*self));
    func_ov003_020be740(self, data_ov003_020e535c);
    EasyFade_FadeBothDisplays(0, 0, 0x800);
    switch (data_ov003_020e71d9) {
        case 2:
            self->kind = 2;
            break;
        case 1:
            self->kind = 1;
            break;
        default:
        case 0:
            self->kind = 0;
            break;
    }
    return 1;
}

// Nonmatching: step dispatch and screen-map offset register allocation.
s32 func_ov003_020bf708(TaskPool* pool, Task* task, void* args) {
    BtlCutIn01* self = task->data;
    if (self->program[self->programIndex] == NULL)
        return 0;
    s32 result;
    do {
        result = self->program[self->programIndex](self);
    } while (result == 2);
    if (result == 1)
        return 0;
    if (!self->active)
        return 1;
    if (self->elapsed == 0x1E)
        func_ov003_020bd2c8(self->kind, 1);
    if (self->elapsed == 0x48)
        func_ov003_020bd370(self->kind, 1);
    self->elapsed++;
    CombatSprite_Update(&self->scenes[0].sprite);
    CombatSprite_Update(&self->scenes[1].sprite);
    if ((self->overlaySubY >> 12) >= 0x200) {
        self->overlaySubY  = 0x200000;
        self->overlayMainY = 0;
    } else {
        self->overlaySubY += 0x4000;
        self->overlayMainY -= 0x4000;
    }
    Display_SetBGOffset(self->overlaySubMap.engineId, self->overlaySubMap.bgLayer, 0, self->overlaySubY);
    Display_SetBGOffset(self->overlayMainMap.engineId, self->overlayMainMap.bgLayer, 0, self->overlayMainY);
    s32 offset = -(self->scenes[0].x & ~0xFFF);
    Display_SetBGOffset(self->scenes[0].subMap.engineId, self->scenes[0].subMap.bgLayer, offset, 0);
    Display_SetBGOffset(self->scenes[0].mainMap.engineId, self->scenes[0].mainMap.bgLayer, offset, 0);
    offset = -(self->scenes[1].x & ~0xFFF);
    Display_SetBGOffset(self->scenes[1].subMap.engineId, self->scenes[1].subMap.bgLayer, offset, 0);
    Display_SetBGOffset(self->scenes[1].mainMap.engineId, self->scenes[1].mainMap.bgLayer, offset, 0);
    return 1;
}

s32 func_ov003_020bfa80(TaskPool* pool, Task* task, void* args) {
    BtlCutIn01*   self   = task->data;
    Cutin01Scene* second = &self->scenes[1];
    if (!self->active)
        return 1;
    CombatSprite_SetPosition(&self->scenes[0].sprite, (s16)((self->scenes[0].x >> 12) + 0x80), 0x50);
    CombatSprite_SetAffineTransform(&self->scenes[0].sprite, 0, 0x1000, 0x1000, 1);
    func_ov003_02082730(&self->scenes[0].sprite, 1);
    CombatSprite_Render(&self->scenes[0].sprite);
    CombatSprite_SetPosition(&second->sprite, (s16)((second->x >> 12) + 0x80), second->y);
    CombatSprite_SetAffineTransform(&second->sprite, 0, 0x1000, 0x1000, 1);
    func_ov003_02082730(&second->sprite, 2);
    CombatSprite_Render(&second->sprite);
    return 1;
}
s32 func_ov003_020bfb44(TaskPool* pool, Task* task, void* args) {
    return 0;
}
s32 func_ov003_020bfb4c(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020dd8a8;
    return stages.iter[stage](pool, task, args);
}

const Cutin01SpriteLayout data_ov003_020dd890 = {
    {   8,   12,   16},
    {0x60, 0x50, 0x40}
};
const Ov003CutinPaletteEntries data_ov003_020dd89c = {
    {8, 9, 10}
};
const TaskStages data_ov003_020dd8a8 = {func_ov003_020bf678, func_ov003_020bf708, func_ov003_020bfa80, func_ov003_020bfb44};
const SpriteAnimEntry data_ov003_020dd8b8[2] = {
    {1, 3, 2, 0},
    {1, 3, 2, 1}
};
const SpriteAnimEntry data_ov003_020dd8c8[6] = {
    { 5,  7,  6, 0},
    { 5,  7,  6, 1},
    { 9, 11, 10, 0},
    { 9, 11, 10, 1},
    {13, 15, 14, 0},
    {13, 15, 14, 1}
};
const Cutin01Resources data_ov003_020dd8f8[4] = {
    {0xA, 5, 1, 2, 3, 4, 0xFFFF, {0}},
    {0xB, 5, 1, 2, 3, 4, 0xFFFF, {0}},
    {0xC, 5, 1, 2, 3, 4, 0xFFFF, {0}},
    {0xD, 6, 1, 2, 3, 4,      5, {0}}
};
char        data_ov003_020e534c[16] = "Tsk_BtlCutIn01";
Cutin01Step data_ov003_020e535c[15] = {func_ov003_020be764, func_ov003_020be974, func_ov003_020bf4b0, func_ov003_020beab4,
                                       func_ov003_020bf4d4, func_ov003_020bf500, func_ov003_020bf634, func_ov003_020bf650,
                                       func_ov003_020bf594, func_ov003_020bf5ec, func_ov003_020beadc, func_ov003_020bf618,
                                       func_ov003_020bf650, func_ov003_020be984, func_ov003_020be748};
