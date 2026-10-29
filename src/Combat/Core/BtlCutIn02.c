#include "Combat/Core/CombatApi.h"
#include "Display.h"
#include "EasyFade.h"
#include <nitro/mi/cpumem.h>
#include <nitro/reg.h>
#include <stddef.h>

typedef struct BtlCutIn02 BtlCutIn02;
typedef s32 (*Cutin02Step)(BtlCutIn02*);
typedef struct Cutin02Point {
    u16 x, y;
} Cutin02Point;
typedef struct Cutin02Path {
    u32          size;
    Cutin02Point points[];
} Cutin02Path;
typedef struct Cutin02Screen {
    ScreenMapEntry map;
    void*          tiles[6];
} Cutin02Screen;
typedef struct Cutin02Scene {
    /* 0x00 */ Data*              pack;
    /* 0x04 */ const Cutin02Path* path;
    /* 0x08 */ PaletteResource*   palettes[2]; // sub, main
    /* 0x10 */ BgResource*        characters[2];
    /* 0x18 */ Cutin02Screen      screens[2];  // sub, main
    /* 0x98 */ u32                reserved98[27];
} Cutin02Scene;
struct BtlCutIn02 {
    /* 0x000 */ const Cutin02Step* program;
    /* 0x004 */ u32                reserved004;
    /* 0x008 */ s32                active;
    /* 0x00C */ Cutin02Scene       scenes[2];
    /* 0x214 */ u16                kind;
    /* 0x216 */ u16                elapsed;
    /* 0x218 */ Data*              overlayPack;
    /* 0x21C */ u32                reserved21C[2];
    /* 0x224 */ PaletteResource*   overlayPalettes[2]; // main, sub
    /* 0x22C */ void*              overlayMainTiles[4];
    /* 0x23C */ void*              overlaySubTiles[4];
    /* 0x24C */ ScreenMapEntry     overlaySubMap;
    /* 0x274 */ ScreenMapEntry     overlayMainMap;
    /* 0x29C */ s32                overlaySubOffset;
    /* 0x2A0 */ s32                overlayMainOffset;
    /* 0x2A4 */ s32                firstX;
    /* 0x2A8 */ s32                firstY;
    /* 0x2AC */ s32                secondX;
    /* 0x2B0 */ s32                secondY;
    /* 0x2B4 */ u32                reserved2B4;
    /* 0x2B8 */ s16                timer;
    /* 0x2BA */ s16                programIndex;
    /* 0x2BC */ u16                pathIndex;
    /* 0x2BE */ u16                phase;
    /* 0x2C0 */ u16                reserved2C0;
    /* 0x2C2 */ u16                subPhase;
    /* 0x2C4 */ u16                subDelay;
    /* 0x2C6 */ u8                 reserved2C6[14];
    /* 0x2D4 */ s16                subTimer;
    /* 0x2D6 */ s16                subState;
    /* 0x2D8 */ s16                subActive;
    /* 0x2DA */ u16                reserved2DA;
};
typedef struct Cutin02Resources {
    u16 pack, palette, characters, tileEntries[5];
    u16 reserved[24];
} Cutin02Resources;
typedef char check_Cutin02Scene[(sizeof(Cutin02Scene) == 0x104) ? 1 : -1];
typedef char check_Cutin02[(sizeof(BtlCutIn02) == 0x2DC) ? 1 : -1];
typedef char check_Cutin02Resources[(sizeof(Cutin02Resources) == 0x40) ? 1 : -1];
typedef char check_Cutin02_kind[(offsetof(BtlCutIn02, kind) == 0x214) ? 1 : -1];
typedef char check_Cutin02_index[(offsetof(BtlCutIn02, programIndex) == 0x2BA) ? 1 : -1];

extern const BinIdentifier            data_ov003_020dd7ac;
extern const Ov003CutinPaletteEntries data_ov003_020dd990;
extern const TaskStages               data_ov003_020dd99c;
extern const Cutin02Resources         data_ov003_020dd9ac[4];
extern Cutin02Step                    data_ov003_020e53a8[15];
extern char                           data_ov003_020e534c[16];
extern s32                            func_ov003_020bfb4c(TaskPool*, Task*, void*, s32);
extern void                           func_0200d858(ScreenMapEntry*, s32, s32, s32);
extern void                           func_0200d954(s32, s32);
extern void*                          G2S_GetBG1ScrPtr(void);
extern void                           func_0203aafc(s32, void*, u32, u32);
extern void                           func_ov003_020bd2c8(u16, s32);
extern void                           func_ov003_020bd370(u16, s32);

void func_ov003_020bfb94(BtlCutIn02* self) {
    self->programIndex++;
    self->subTimer  = 0;
    self->subState  = 0;
    self->subActive = 0;
}
void func_ov003_020bfbb8(BtlCutIn02* self, const Cutin02Step* program) {
    self->program = program;
}
// Nonmatching: display offset register allocation.
void func_ov003_020bfbc0(BtlCutIn02* self) {
    ScreenMapEntry* map = &self->scenes[0].screens[0].map;
    Display_SetBGOffset(map->engineId, map->bgLayer, self->firstX, self->firstY);
    map = &self->scenes[0].screens[1].map;
    Display_SetBGOffset(map->engineId, map->bgLayer, self->firstX, self->firstY + 0x120000);
    map = &self->scenes[1].screens[0].map;
    Display_SetBGOffset(map->engineId, map->bgLayer, self->secondX, self->secondY);
    map = &self->scenes[1].screens[1].map;
    Display_SetBGOffset(map->engineId, map->bgLayer, self->secondX, self->secondY + 0x120000);
    map = &self->overlaySubMap;
    Display_SetBGOffset(map->engineId, map->bgLayer, self->overlaySubOffset, self->overlaySubOffset);
    map = &self->overlayMainMap;
    Display_SetBGOffset(map->engineId, map->bgLayer, self->overlayMainOffset, self->overlayMainOffset);
}
s32 func_ov003_020bfe2c(BtlCutIn02* self) {
    EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
    return 1;
}
// Nonmatching: display-state store scheduling.
s32 func_ov003_020bfe48(BtlCutIn02* self) {
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
    func_ov003_020bfb94(self);
    return 0;
}

s32 func_ov003_020c0058(BtlCutIn02* self) {
    func_ov003_020bfb94(self);
    return 2;
}
s32 func_ov003_020c0068(BtlCutIn02* self) {
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
    func_0200d954(0, 1);
    func_0200d954(0, 2);
    func_0200d954(0, 3);
    func_0200d954(1, 1);
    func_0200d954(1, 2);
    func_0200d954(1, 3);
    DatMgr_ReleaseData(self->overlayPack);
    DatMgr_ReleaseData(self->scenes[1].pack);
    DatMgr_ReleaseData(self->scenes[0].pack);
    func_ov003_020bfb94(self);
    return 0;
}
s32 func_ov003_020c0188(BtlCutIn02* self) {
    EasyFade_FadeBothDisplays(0, 0, 0x800);
    func_ov003_020bfb94(self);
    return 0;
}
s32 func_ov003_020c01b0(BtlCutIn02* self) {
    EasyFade_FadeBothDisplays(0, 0x10, 0x1000);
    func_ov003_020bfb94(self);
    return 0;
}
void func_ov003_020c01d8(BtlCutIn02* self, u16 kind) {
    self->overlayPack                = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, 3, 0);
    Ov003CutinPaletteEntries entries = data_ov003_020dd990;
    void*                    palette = Data_GetPackEntryData(self->overlayPack, entries.entries[kind]);
    self->overlaySubTiles[0]         = Ov003Cutin_Tiles(self->overlayPack, 6);
    self->overlaySubTiles[1]         = Ov003Cutin_Tiles(self->overlayPack, 7);
    self->overlaySubTiles[2]         = Ov003Cutin_Tiles(self->overlayPack, 8);
    self->overlaySubTiles[3]         = Ov003Cutin_Tiles(self->overlayPack, 9);
    self->overlayPalettes[1]         = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0xF, 1);
    func_0200d1d8(&self->overlaySubMap, 1, 3, NULL, self->overlaySubTiles, 2, 2);
    PaletteMgr_Flush(g_PaletteManagers[1], self->overlayPalettes[1]);
    func_0200d858(&self->overlaySubMap, 1, 1, 0);
    palette                   = Data_GetPackEntryData(self->overlayPack, 5);
    self->overlayMainTiles[0] = Ov003Cutin_Tiles(self->overlayPack, 1);
    self->overlayMainTiles[1] = Ov003Cutin_Tiles(self->overlayPack, 2);
    self->overlayMainTiles[2] = Ov003Cutin_Tiles(self->overlayPack, 3);
    self->overlayMainTiles[3] = Ov003Cutin_Tiles(self->overlayPack, 4);
    self->overlayPalettes[0]  = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 0xF, 1);
    func_0200d1d8(&self->overlayMainMap, 0, 3, NULL, self->overlayMainTiles, 2, 2);
    PaletteMgr_Flush(g_PaletteManagers[0], self->overlayPalettes[0]);
    func_0200d858(&self->overlayMainMap, 1, 1, 0);
    self->overlaySubOffset  = 0;
    self->overlayMainOffset = 0x100000;
}
void func_ov003_020c04a0(BtlCutIn02* self, u16 kind) {
    const Cutin02Resources* base    = &data_ov003_020dd9ac[0];
    const Cutin02Resources* variant = &data_ov003_020dd9ac[kind + 1];
    Cutin02Scene*           first   = &self->scenes[0];
    Cutin02Scene*           second  = &self->scenes[1];
    first->pack                     = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, 0xE, 0);
    void* palette                   = Data_GetPackEntryData(first->pack, 7);
    void* characters                = Data_GetPackEntryData(first->pack, base->characters);
    first->screens[0].tiles[0]      = Ov003Cutin_Tiles(first->pack, base->tileEntries[0]);
    first->screens[0].tiles[1]      = Ov003Cutin_Tiles(first->pack, base->tileEntries[0]);
    first->screens[0].tiles[2]      = Ov003Cutin_Tiles(first->pack, base->tileEntries[1]);
    first->screens[0].tiles[3]      = Ov003Cutin_Tiles(first->pack, base->tileEntries[2]);
    first->screens[0].tiles[4]      = Ov003Cutin_Tiles(first->pack, base->tileEntries[3]);
    first->screens[0].tiles[5]      = Ov003Cutin_Tiles(first->pack, base->tileEntries[4]);
    first->palettes[0]              = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0, 7);
    first->characters[0]            = BgResMgr_AllocChar32(g_BgResourceManagers[1], characters,
                                                           g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x8000);
    PaletteMgr_Flush(g_PaletteManagers[1], first->palettes[0]);
    func_0200d1d8(&first->screens[0].map, 1, 1, NULL, first->screens[0].tiles, 2, 3);
    func_0200d858(&first->screens[0].map, 0, 0, 0);
    first->screens[1].tiles[0] = Ov003Cutin_Tiles(first->pack, base->tileEntries[0]);
    first->screens[1].tiles[1] = Ov003Cutin_Tiles(first->pack, base->tileEntries[0]);
    first->screens[1].tiles[2] = Ov003Cutin_Tiles(first->pack, base->tileEntries[1]);
    first->screens[1].tiles[3] = Ov003Cutin_Tiles(first->pack, base->tileEntries[2]);
    first->screens[1].tiles[4] = Ov003Cutin_Tiles(first->pack, base->tileEntries[3]);
    first->screens[1].tiles[5] = Ov003Cutin_Tiles(first->pack, base->tileEntries[4]);
    first->palettes[1]         = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 0, 7);
    first->characters[1]       = BgResMgr_AllocChar32(g_BgResourceManagers[0], characters,
                                                      g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x8000);
    func_0200d1d8(&first->screens[1].map, 0, 1, NULL, first->screens[1].tiles, 2, 3);
    PaletteMgr_Flush(g_PaletteManagers[0], first->palettes[1]);
    func_0200d858(&first->screens[1].map, 0, 0, 0);
    second->pack                = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, variant->pack, 0);
    palette                     = Data_GetPackEntryData(second->pack, variant->palette);
    characters                  = Data_GetPackEntryData(second->pack, variant->characters);
    second->screens[0].tiles[0] = Ov003Cutin_Tiles(second->pack, base->tileEntries[0]);
    second->screens[0].tiles[1] = Ov003Cutin_Tiles(second->pack, base->tileEntries[0]);
    second->screens[0].tiles[2] = Ov003Cutin_Tiles(second->pack, base->tileEntries[1]);
    second->screens[0].tiles[3] = Ov003Cutin_Tiles(second->pack, base->tileEntries[2]);
    second->screens[0].tiles[4] = Ov003Cutin_Tiles(second->pack, base->tileEntries[3]);
    second->screens[0].tiles[5] = Ov003Cutin_Tiles(second->pack, base->tileEntries[4]);
    second->palettes[0]         = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 7, 7);
    second->characters[0]       = BgResMgr_AllocChar32(g_BgResourceManagers[1], characters,
                                                       g_DisplaySettings.engineState[1].bgSettings[2].charBase, 0, 0x8000);
    func_0200d1d8(&second->screens[0].map, 1, 2, NULL, second->screens[0].tiles, 2, 3);
    PaletteMgr_Flush(g_PaletteManagers[1], second->palettes[0]);
    func_0200d858(&second->screens[0].map, 0, 0, 0);
    second->screens[1].tiles[0] = Ov003Cutin_Tiles(second->pack, base->tileEntries[0]);
    second->screens[1].tiles[1] = Ov003Cutin_Tiles(second->pack, base->tileEntries[0]);
    second->screens[1].tiles[2] = Ov003Cutin_Tiles(second->pack, base->tileEntries[1]);
    second->screens[1].tiles[3] = Ov003Cutin_Tiles(second->pack, base->tileEntries[2]);
    second->screens[1].tiles[4] = Ov003Cutin_Tiles(second->pack, base->tileEntries[3]);
    second->screens[1].tiles[5] = Ov003Cutin_Tiles(second->pack, base->tileEntries[4]);
    second->palettes[1]         = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 7, 7);
    second->characters[1]       = BgResMgr_AllocChar32(g_BgResourceManagers[0], characters,
                                                       g_DisplaySettings.engineState[0].bgSettings[2].charBase, 0, 0x8000);
    func_0200d1d8(&second->screens[1].map, 0, 2, NULL, second->screens[1].tiles, 2, 3);
    PaletteMgr_Flush(g_PaletteManagers[0], second->palettes[1]);
    func_0200d858(&second->screens[1].map, 0, 0, 0);
    func_ov003_020c01d8(self, kind);
    self->firstX  = -0x80000;
    self->firstY  = 0x120000;
    self->secondX = 0x100000;
    self->secondY = 0;
    func_ov003_020bfbc0(self);
}
s32 func_ov003_020c0d90(BtlCutIn02* self) {
    func_ov003_020c04a0(self, self->kind);
    func_ov003_020bfb94(self);
    return 2;
}
s32 func_ov003_020c0db4(BtlCutIn02* self) {
    self->active            = 1;
    self->pathIndex         = 0;
    self->phase             = 0;
    self->subPhase          = 0;
    self->subDelay          = 0;
    self->overlaySubOffset  = 0;
    self->overlayMainOffset = 0x100000;
    self->firstX            = -0x80000;
    self->firstY            = 0x120000;
    self->secondX           = 0x100000;
    self->secondY           = 0;
    func_0200d1d8(&self->scenes[1].screens[0].map, 1, 2, NULL, self->scenes[1].screens[0].tiles, 2, 3);
    func_ov003_020bfbc0(self);
    self->scenes[0].path = Data_GetPackEntryData(self->overlayPack, 13);
    self->scenes[1].path = Data_GetPackEntryData(self->overlayPack, 14);
    func_ov003_020bfb94(self);
    return 2;
}
// Coordinates use a sign bit and a 15-bit magnitude, including signed zero.
static inline s32 Cutin02_Coordinate(u16 value) {
    s32 sign = (value & 0x8000) ? -1 : 1;
    return (value & 0x7FFF) * sign;
}
// Nonmatching: path coordinate decoding register allocation.
s32 func_ov003_020c0e88(BtlCutIn02* self) {
    const Cutin02Point* first  = &self->scenes[0].path->points[self->pathIndex];
    const Cutin02Point* second = &self->scenes[1].path->points[self->pathIndex];
    s16                 x1     = Cutin02_Coordinate(first->x);
    s16                 y1     = Cutin02_Coordinate(first->y);
    s32                 x2     = Cutin02_Coordinate(second->x);
    s32                 y2     = Cutin02_Coordinate(second->y);
    if (x1 != 0 || y1 != 0) {
        self->firstX = -x1 * 0x1000;
        self->firstY = -y1 * 0x1000;
        if (self->firstY < 0) {
            func_0200d954(1, 1);
            func_0203aafc(3, G2S_GetBG1ScrPtr(), 0, 0x1000);
        }
    }
    if (x2 != 0 || y2 != 0) {
        self->secondX = -x2 * 0x1000;
        self->secondY = -y2 * 0x1000;
    }
    self->pathIndex++;
    if (x2 == 0 && y2 == 0 && x1 == 0 && y1 == 0) {
        func_ov003_020bfb94(self);
        return 0;
    }
    return 0;
}
s32 func_ov003_020c0fe4(BtlCutIn02* self) {
    self->pathIndex = 10;
    func_ov003_020bfb94(self);
    return 2;
}
s32 func_ov003_020c1000(BtlCutIn02* self) {
    self->pathIndex = 30;
    func_ov003_020bfb94(self);
    return 2;
}
s32 func_ov003_020c101c(BtlCutIn02* self) {
    if (self->pathIndex != 0)
        self->pathIndex--;
    else
        func_ov003_020bfb94(self);
    return 0;
}
void func_ov003_020c1044(BtlCutIn02* self) {
    MI_CpuSet(self, 0, sizeof(*self));
    func_ov003_020bfbb8(self, data_ov003_020e53a8);
    EasyFade_FadeBothDisplays(0, 0, 0x800);
    switch (data_ov003_020e71b8->unk3D88D) {
        case 2:
            self->kind = 2;
            return;
        case 1:
            self->kind = 1;
            return;
        default:
        case 0:
            self->kind = 0;
            return;
    }
}
s32 func_ov003_020c10d4(TaskPool* pool, Task* task, void* args) {
    func_ov003_020c1044(task->data);
    return 1;
}
// Nonmatching: update dispatch and scrolling instruction scheduling.
s32 func_ov003_020c10e8(TaskPool* pool, Task* task, void* args) {
    BtlCutIn02* self = task->data;
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
    if (self->elapsed == 30)
        func_ov003_020bd2c8(self->kind, 2);
    if (self->elapsed == 72)
        func_ov003_020bd370(self->kind, 2);
    self->elapsed++;
    if ((self->overlaySubOffset >> 12) >= 0x100) {
        self->overlaySubOffset  = 0x100000;
        self->overlayMainOffset = 0;
    } else {
        self->overlaySubOffset += 0x4000;
        self->overlayMainOffset -= 0x4000;
    }
    func_ov003_020bfbc0(self);
    return 1;
}
s32 func_ov003_020c11cc(TaskPool* pool, Task* task, void* args) {
    return 1;
}
s32 func_ov003_020c11d4(TaskPool* pool, Task* task, void* args) {
    return 0;
}
s32 func_ov003_020c11dc(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020dd99c;
    return stages.iter[stage](pool, task, args);
}

const TaskHandle               data_ov003_020dd978 = {data_ov003_020e534c, func_ov003_020bfb4c, 0x270};
extern char                    data_ov003_020e5398[16];
const TaskHandle               data_ov003_020dd984 = {data_ov003_020e5398, func_ov003_020c11dc, sizeof(BtlCutIn02)};
const Ov003CutinPaletteEntries data_ov003_020dd990 = {
    {10, 11, 12}
};
const TaskStages data_ov003_020dd99c = {func_ov003_020c10d4, func_ov003_020c10e8, func_ov003_020c11cc, func_ov003_020c11d4};
const Cutin02Resources data_ov003_020dd9ac[4] = {
    {14, 7, 1, {2, 3, 4, 5, 6}, {0}},
    {15, 7, 1, {2, 3, 4, 5, 6}, {0}},
    {16, 7, 1, {2, 3, 4, 5, 6}, {0}},
    {17, 7, 1, {2, 3, 4, 5, 6}, {0}}
};
char        data_ov003_020e5398[16] = "Tsk_BtlCutIn02";
Cutin02Step data_ov003_020e53a8[15] = {func_ov003_020bfe48, func_ov003_020c0058, func_ov003_020c0d90, func_ov003_020c0188,
                                       func_ov003_020c0db4, func_ov003_020c0e88, func_ov003_020c0fe4, func_ov003_020c101c,
                                       func_ov003_020c0fe4, func_ov003_020c101c, func_ov003_020c01b0, func_ov003_020c1000,
                                       func_ov003_020c101c, func_ov003_020c0068, func_ov003_020bfe2c};
