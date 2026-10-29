#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatCutin.h"
#include "EasyFade.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

// Nonmatching: all functions and data exports match individually; data ordering
// still differs, so this TU continues linking its retail object.

typedef struct CutinTable3 {
    s32 values[3];
} CutinTable3;
typedef struct CutinResources {
    u16 pack, palette, characters, maps[2];
} CutinResources;
typedef struct CutinMap {
    u32 header;
    u16 tiles[];
} CutinMap;

extern char                 data_ov003_020e5184[28];
extern char                 data_ov003_020e51a0[28];
extern char                 data_ov003_020e51bc[32];
extern char                 data_ov003_020e51dc[32];
extern char                 data_ov003_020e51fc[32];
extern char                 data_ov003_020e521c[36];
extern char                 data_ov003_020e5240[36];
extern char                 data_ov003_020e5264[36];
extern char                 data_ov003_020e5288[36];
extern char                 data_ov003_020e52ac[36];
extern char                 data_ov003_020e52d0[36];
extern char                 data_ov003_020e52f4[36];
extern char                 data_ov003_020e5318[36];
extern const BinIdentifier  data_ov003_020dd77c;
extern const BinIdentifier  data_ov003_020dd784;
extern const BinIdentifier  data_ov003_020dd78c;
extern const BinIdentifier  data_ov003_020dd794;
extern const BinIdentifier  data_ov003_020dd79c;
extern const BinIdentifier  data_ov003_020dd7a4;
extern const BinIdentifier  data_ov003_020dd7ac;
extern const BinIdentifier  data_ov003_020dd7b4;
extern const BinIdentifier  data_ov003_020dd7bc;
extern const BinIdentifier  data_ov003_020dd7c4;
extern const BinIdentifier  data_ov003_020dd7cc;
extern const BinIdentifier  data_ov003_020dd7d4;
extern const BinIdentifier  data_ov003_020dd7dc;
extern const CutinTable3    data_ov003_020dd7e4;
extern const CutinTable3    data_ov003_020dd7f0;
extern const CutinTable3    data_ov003_020dd7fc;
extern const CutinTable3    data_ov003_020dd808;
extern const CutinTable3    data_ov003_020dd814;
extern const CutinTable3    data_ov003_020dd820;
extern const CutinTable3    data_ov003_020dd82c;
extern const TaskHandle     data_ov003_020dd838;
extern const TaskStages     data_ov003_020dd844;
extern const CutinResources data_ov003_020dd854[6];
extern Ov003CutinTask*      data_ov003_020e71d4;
extern char                 data_ov003_020e533c[16];

extern s32         func_ov003_020843b0(s32, s32);
extern s32         func_ov003_020843ec(s32, s32, s32);
extern s32         func_ov003_0208442c(s32, s32);
extern s32         func_ov003_02084460(s32, s32, s32);
extern void        func_0200d858(ScreenMapEntry*, s32, s32, s32);
extern void        func_0200d954(s32, s32);
extern void        func_02026590(s32*, s32, u16);
extern BgResource* BgResMgr_AllocChar32(BgResMgr*, void*, u32, u32, u32);
extern u8          data_ov003_020e71d9;

s32 func_ov003_020bd1bc(s32 mode, Ov003ActorSnapshot* entry) {
    if (mode == 1)
        return func_ov003_020843b0(1, entry->unit->actor.position.x);
    return func_ov003_020843b0(0, entry->unit->actor.position.x);
}
s32 func_ov003_020bd1ec(s32 mode, Ov003ActorSnapshot* entry) {
    if (mode == 1)
        return func_ov003_020843ec(1, entry->unit->actor.position.y, entry->unit->actor.position.z);
    return func_ov003_020843ec(0, entry->unit->actor.position.y, entry->unit->actor.position.z);
}
void func_ov003_020bd224(s32 mode, Ov003ActorSnapshot* entry, s32 x) {
    if (mode == 1)
        entry->unit->actor.position.x = func_ov003_0208442c(1, x);
    else
        entry->unit->actor.position.x = func_ov003_0208442c(0, x);
}
void func_ov003_020bd260(s32 mode, Ov003ActorSnapshot* entry, s32 y) {
    if (mode == 1)
        entry->unit->actor.position.y = func_ov003_02084460(1, y, entry->unit->actor.position.z);
    else
        entry->unit->actor.position.y = func_ov003_02084460(0, y, entry->unit->actor.position.z);
}
void func_ov003_020bd2a8(Ov003CutinTask* self) {
    data_ov003_020e71d4 = self;
}
Ov003CutinTask* func_ov003_020bd2b8(void) {
    return data_ov003_020e71d4;
}

void func_ov003_020bd2c8(s32 kind, s32 index) {
    switch (kind) {
        case 0: {
            CutinTable3 voices = data_ov003_020dd7fc;
            SndMgr_StartPlayingSE(voices.values[index]);
            break;
        }
        case 1: {
            CutinTable3 voices = data_ov003_020dd808;
            SndMgr_StartPlayingSE(voices.values[index]);
            break;
        }
        case 2: {
            CutinTable3 voices = data_ov003_020dd820;
            SndMgr_StartPlayingSE(voices.values[index]);
            break;
        }
        case 3:
            SndMgr_StartPlayingSE(0x544);
            break;
    }
}
void func_ov003_020bd370(s32 kind, s32 index) {
    switch (kind) {
        case 0: {
            CutinTable3 voices = data_ov003_020dd7f0;
            SndMgr_StartPlayingSE(voices.values[index]);
            break;
        }
        case 1: {
            CutinTable3 voices = data_ov003_020dd814;
            SndMgr_StartPlayingSE(voices.values[index]);
            break;
        }
        case 2: {
            CutinTable3 voices = data_ov003_020dd7e4;
            SndMgr_StartPlayingSE(voices.values[index]);
            break;
        }
        case 3:
            SndMgr_StartPlayingSE(0x52B);
            break;
    }
}
void func_ov003_020bd418(Ov003CutinTask* self, void (*update)(Ov003CutinTask*)) {
    self->update = update;
    self->timer  = 0;
    self->phase  = 0;
}

void func_ov003_020bd430(Ov003CutinTask* self) {
    Display_SetBGOffset(self->overlayMaps[0].engineId, self->overlayMaps[0].bgLayer, self->overlayOffsets[0], 0);
    Display_SetBGOffset(self->overlayMaps[1].engineId, self->overlayMaps[1].bgLayer, self->overlayOffsets[1], 0);
    Display_SetBGOffset(self->backgrounds[0].maps[0].engineId, self->backgrounds[0].maps[0].bgLayer,
                        self->backgrounds[0].offsets[0] - 0x80000, 0);
    Display_SetBGOffset(self->backgrounds[0].maps[1].engineId, self->backgrounds[0].maps[1].bgLayer,
                        self->backgrounds[0].offsets[1] - 0x80000, 0);
    Display_SetBGOffset(self->backgrounds[1].maps[0].engineId, self->backgrounds[1].maps[0].bgLayer,
                        self->backgrounds[1].offsets[0] - 0x80000, 0);
    Display_SetBGOffset(self->backgrounds[1].maps[1].engineId, self->backgrounds[1].maps[1].bgLayer,
                        self->backgrounds[1].offsets[1] - 0x80000, 0);
}

static inline void* CutinMapTiles(Data* pack, s32 index) {
    CutinMap* map = Data_GetPackEntryData(pack, index);
    return map->tiles;
}

void func_ov003_020bd6bc(Ov003CutinBackground* self, s32 index) {
    const CutinResources* resources = &data_ov003_020dd854[index];
    Display_InitSubBG1(0, 1, 0, 0, 1, 0, 0x4004);
    Display_InitSubBG2(0, 1, 0, 2, 1, 1, 0x4204);
    g_DisplaySettings.engineState[1].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[1].bgSettings[2].priority = 0;
    Display_SetBGOffset(1, 1, 0, 0);
    Display_SetBGOffset(1, 2, 0, 0);
    self->offsets[0] = -0x80000;
    self->offsets[1] = -0x80000;
    self->pack       = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, resources->pack, 0);
    void* palette    = Data_GetPackEntryData(self->pack, resources->palette);
    void* characters = Data_GetPackEntryData(self->pack, resources->characters);
    self->palette    = PaletteMgr_AllocPalette(g_PaletteManagers[1], palette, 0, 0, 15);
    self->characters = BgResMgr_AllocChar32(g_BgResourceManagers[1], characters,
                                            g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, 0x8000);
    PaletteMgr_Flush(g_PaletteManagers[1], self->palette);
    self->tileData[0] = CutinMapTiles(self->pack, resources->maps[0]);
    self->tileData[1] = CutinMapTiles(self->pack, resources->maps[1]);
    func_0200d1d8(&self->maps[0], 1, 1, NULL, &self->tileData[0], 1, 1);
    func_0200d1d8(&self->maps[1], 1, 2, NULL, &self->tileData[1], 1, 1);
    func_0200d858(&self->maps[0], 0, 0, 0);
    func_0200d858(&self->maps[1], 0, 0, 0);
}
void func_ov003_020bd9b4(Ov003CutinBackground* self, s32 index) {
    const CutinResources* resources = &data_ov003_020dd854[index];
    Display_InitMainBG1(0, 1, 0, 0, 3, 0, 0x400C);
    Display_InitMainBG2(0, 1, 0, 2, 3, 1, 0x420C);
    g_DisplaySettings.engineState[0].bgSettings[1].priority = 0;
    g_DisplaySettings.engineState[0].bgSettings[2].priority = 0;
    Display_SetBGOffset(0, 1, 0, 0);
    Display_SetBGOffset(0, 2, 0, 0);
    self->offsets[0] = 0x180000;
    self->offsets[1] = 0x180000;
    self->pack       = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, resources->pack, 0);
    void* palette    = Data_GetPackEntryData(self->pack, resources->palette);
    void* characters = Data_GetPackEntryData(self->pack, resources->characters);
    self->palette    = PaletteMgr_AllocPalette(g_PaletteManagers[0], palette, 0, 0, 15);
    self->characters = BgResMgr_AllocChar32(g_BgResourceManagers[0], characters,
                                            g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, 0x8000);
    PaletteMgr_Flush(g_PaletteManagers[0], self->palette);
    self->tileData[0] = CutinMapTiles(self->pack, resources->maps[0]);
    self->tileData[1] = CutinMapTiles(self->pack, resources->maps[1]);
    func_0200d1d8(&self->maps[0], 0, 1, NULL, &self->tileData[0], 1, 1);
    func_0200d1d8(&self->maps[1], 0, 2, NULL, &self->tileData[1], 1, 1);
    func_0200d858(&self->maps[0], 0, 0, 0);
    func_0200d858(&self->maps[1], 0, 0, 0);
}

void func_ov003_020bdcb8(Ov003CutinTask* self, s32 index) {
    Display_InitMainBG3(0, 1, 0, 4, 1, 1, 0x4404);
    g_DisplaySettings.controls[0].windows &= ~4;
    g_DisplaySettings.engineState[0].bgSettings[3].priority = 1;
    Display_SetBGOffset(0, 3, 0, 0);
    Display_InitSubBG3(0, 1, 0, 4, 5, 1, 0x4414);
    g_DisplaySettings.controls[1].windows &= ~4;
    g_DisplaySettings.engineState[1].bgSettings[3].priority = 0;
    Display_SetBGOffset(1, 3, 0, 0);
    self->overlayPack          = DatMgr_LoadPackEntry(1, NULL, 0, &data_ov003_020dd7ac, 1, 0);
    self->overlayCharacters[0] = BgResMgr_AllocChar32(g_BgResourceManagers[0], Data_GetPackEntryData(self->overlayPack, 1),
                                                      g_DisplaySettings.engineState[0].bgSettings[3].charBase, 0, 0x3AA0);
    self->overlayPalettes[0] =
        PaletteMgr_AllocPalette(g_PaletteManagers[0], Data_GetPackEntryData(self->overlayPack, 5), 0, 15, 1);
    PaletteMgr_Flush(g_PaletteManagers[0], self->overlayPalettes[0]);
    self->overlayTileData[0][0] = CutinMapTiles(self->overlayPack, 2);
    self->overlayTileData[0][1] = CutinMapTiles(self->overlayPack, 3);
    self->overlayTileData[0][2] = CutinMapTiles(self->overlayPack, 4);
    func_0200d1d8(&self->overlayMaps[0], 0, 3, NULL, self->overlayTileData[0], 3, 1);
    func_0200d858(&self->overlayMaps[0], 0, 0, 0);
    self->overlayCharacters[1] = BgResMgr_AllocChar32(g_BgResourceManagers[1], Data_GetPackEntryData(self->overlayPack, 6),
                                                      g_DisplaySettings.engineState[1].bgSettings[3].charBase, 0, 0x3AA0);
    CutinTable3 palettes       = data_ov003_020dd82c;
    self->overlayPalettes[1]   = PaletteMgr_AllocPalette(
        g_PaletteManagers[1], Data_GetPackEntryData(self->overlayPack, palettes.values[index - 1]), 0, 15, 1);
    PaletteMgr_Flush(g_PaletteManagers[1], self->overlayPalettes[1]);
    self->overlayTileData[1][0] = CutinMapTiles(self->overlayPack, 7);
    self->overlayTileData[1][1] = CutinMapTiles(self->overlayPack, 8);
    self->overlayTileData[1][2] = CutinMapTiles(self->overlayPack, 9);
    func_0200d1d8(&self->overlayMaps[1], 1, 3, NULL, self->overlayTileData[1], 3, 1);
    func_0200d858(&self->overlayMaps[1], 0, 0, 0);
    self->overlayTimer      = 90;
    self->overlayOffsets[0] = 0x200000;
    self->overlayOffsets[1] = 0;
}

void func_ov003_020be100(Ov003CutinTask* self) {
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], self->backgrounds[0].palette);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], self->backgrounds[0].characters);
    func_0200d954(1, 1);
    func_0200d954(1, 2);
    DatMgr_ReleaseData(self->backgrounds[0].pack);
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->backgrounds[1].palette);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->backgrounds[1].characters);
    func_0200d954(0, 1);
    func_0200d954(0, 2);
    DatMgr_ReleaseData(self->backgrounds[1].pack);
}
void func_ov003_020be194(Ov003CutinTask* self) {
    PaletteMgr_ReleaseResource(g_PaletteManagers[0], self->overlayPalettes[0]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[0], self->overlayCharacters[0]);
    PaletteMgr_ReleaseResource(g_PaletteManagers[1], self->overlayPalettes[1]);
    BgResMgr_ReleaseChar(g_BgResourceManagers[1], self->overlayCharacters[1]);
    func_0200d954(0, 3);
    func_0200d954(1, 3);
    DatMgr_ReleaseData(self->overlayPack);
}
void func_ov003_020be208(Ov003CutinTask* self) {}
void func_ov003_020be20c(Ov003CutinTask* self) {}

void func_ov003_020be210(Ov003CutinTask* self) {
    if (self->timer == 0)
        self->timer = 15;
    if (self->timer > 0) {
        func_02026590(&self->backgrounds[0].offsets[0], 0x55000, self->timer);
        func_02026590(&self->backgrounds[1].offsets[0], 0xAA000, self->timer);
        self->backgrounds[0].offsets[1] = self->backgrounds[0].offsets[0];
        self->backgrounds[1].offsets[1] = self->backgrounds[1].offsets[0];
        self->timer--;
    }
    if (self->timer <= 0)
        func_ov003_020bd418(self, func_ov003_020be2a8);
}
void func_ov003_020be2a8(Ov003CutinTask* self) {
    if (self->timer == 0)
        self->timer = 80;
    if (self->timer == 20)
        EasyFade_FadeBothDisplays(0, 16, 0x1000);
    if (self->timer > 50) {
        func_02026590(&self->backgrounds[0].offsets[0], 0x66000, self->timer);
        func_02026590(&self->backgrounds[0].offsets[1], 0xA6000, self->timer);
        func_02026590(&self->backgrounds[1].offsets[0], 0x99000, self->timer);
        func_02026590(&self->backgrounds[1].offsets[1], 0x59000, self->timer);
    } else if (self->timer > 0) {
        func_02026590(&self->backgrounds[0].offsets[0], 0x80000, self->timer);
        func_02026590(&self->backgrounds[0].offsets[1], 0xC0000, self->timer);
        func_02026590(&self->backgrounds[1].offsets[0], 0x80000, self->timer);
        func_02026590(&self->backgrounds[1].offsets[1], 0x40000, self->timer);
    }
    if (--self->timer <= 0)
        func_ov003_020bd418(self, func_ov003_020be3d0);
}
void func_ov003_020be3d0(Ov003CutinTask* self) {
    switch (self->phase) {
        case 0:
            self->timer = 15;
            self->phase++;
            break;
        case 1:
            if (!EasyFade_IsFading())
                self->active = 0;
            break;
    }
}
s32 func_ov003_020be420(TaskPool* pool, Task* task, void* args) {
    Ov003CutinTask* self = task->data;
    Ov003CutinArgs* arg  = args;
    MI_CpuSet(self, 0, sizeof(*self));
    self->kind    = arg != NULL ? arg->kind : 0;
    self->unk_E4  = 0;
    self->unk_DC  = 0x180000;
    self->unk_E0  = -0x80000;
    self->elapsed = 0;
    self->active  = 1;
    if (self->kind == 2) {
        func_ov003_020bd6bc(&self->backgrounds[0], 4);
        func_ov003_020bd9b4(&self->backgrounds[1], 5);
        func_ov003_020bdcb8(self, 2);
        self->voiceKind = 3;
    } else {
        switch (data_ov003_020e71d9) {
            case 1:
                func_ov003_020bd6bc(&self->backgrounds[0], 2);
                func_ov003_020bd9b4(&self->backgrounds[1], 0);
                func_ov003_020bdcb8(self, 2);
                self->voiceKind = 1;
                break;
            case 2:
                func_ov003_020bd6bc(&self->backgrounds[0], 3);
                func_ov003_020bd9b4(&self->backgrounds[1], 0);
                func_ov003_020bdcb8(self, 3);
                self->voiceKind = 2;
                break;
            case 0:
            default:
                func_ov003_020bd6bc(&self->backgrounds[0], 1);
                func_ov003_020bd9b4(&self->backgrounds[1], 0);
                func_ov003_020bdcb8(self, 1);
                self->voiceKind = 0;
                break;
        }
    }
    func_ov003_020be208(self);
    func_ov003_020bd430(self);
    func_ov003_020bd418(self, func_ov003_020be210);
    EasyFade_FadeBothDisplays(0, 0, 0x800);
    return 1;
}
s32 func_ov003_020be59c(TaskPool* pool, Task* task, void* args) {
    Ov003CutinTask* self = task->data;
    if (self->update != NULL)
        self->update(self);
    if (self->overlayTimer > 0) {
        func_02026590(&self->overlayOffsets[0], 0, self->overlayTimer);
        func_02026590(&self->overlayOffsets[1], 0x200000, self->overlayTimer);
        self->overlayTimer--;
    }
    func_ov003_020bd430(self);
    if (self->elapsed == 30)
        func_ov003_020bd2c8(self->voiceKind, 0);
    if (self->elapsed == 72)
        func_ov003_020bd370(self->voiceKind, 0);
    self->elapsed++;
    return self->active;
}

char data_ov003_020e5184[28] = "Apl_Fuk/Grp_BtlSpecial3.bin";

char data_ov003_020e51a0[28] = "Apl_Fuk/Grp_BtlSpecial2.bin";

char data_ov003_020e51bc[32] = "Apl_Fuk/Grp_BtlBgEff22_SCR.bin";

char data_ov003_020e51dc[32] = "Apl_Fuk/Grp_BtlSpecialBito.bin";

char data_ov003_020e51fc[32] = "Apl_Fuk/Grp_BtlSpecialSiki.bin";

char data_ov003_020e521c[36] = "Apl_Fuk/Grp_BtlSpecial2_Bito.bin";

char data_ov003_020e5240[36] = "Apl_Fuk/Grp_BtlSpecialYoshua.bin";

char data_ov003_020e5264[36] = "Apl_Fuk/Grp_BtlSpecial2_Sini.bin";

char data_ov003_020e5288[36] = "Apl_Fuk/Grp_BtlSpecial2_Shiki.bin";

char data_ov003_020e52ac[36] = "Apl_Fuk/Grp_BtlSpecialYoshua1.bin";

char data_ov003_020e52d0[36] = "Apl_Fuk/Grp_BtlSpecialYoshua2a.bin";

char data_ov003_020e52f4[36] = "Apl_Fuk/Grp_BtlSpecial2_Yoshua.bin";

char data_ov003_020e5318[36] = "Apl_Fuk/Grp_BtlSpecialYoshua2b.bin";

const BinIdentifier data_ov003_020dd77c = {5, data_ov003_020e52d0};

const BinIdentifier data_ov003_020dd784 = {5, data_ov003_020e5318};

const BinIdentifier data_ov003_020dd78c = {4, data_ov003_020e51fc};

const BinIdentifier data_ov003_020dd794 = {5, data_ov003_020e5240};

const BinIdentifier data_ov003_020dd79c = {3, data_ov003_020e5264};

const BinIdentifier data_ov003_020dd7a4 = {6, data_ov003_020e51dc};

const BinIdentifier data_ov003_020dd7ac = {3, data_ov003_020e5184};

const BinIdentifier data_ov003_020dd7b4 = {5, data_ov003_020e52ac};

const BinIdentifier data_ov003_020dd7bc = {6, data_ov003_020e521c};

const BinIdentifier data_ov003_020dd7c4 = {4, data_ov003_020e5288};

const BinIdentifier data_ov003_020dd7cc = {6, data_ov003_020e51bc};

const BinIdentifier data_ov003_020dd7d4 = {5, data_ov003_020e52f4};

const BinIdentifier data_ov003_020dd7dc = {3, data_ov003_020e51a0};

const CutinTable3 data_ov003_020dd7e4 = {
    {1021, 1023, 1025}
};

const CutinTable3 data_ov003_020dd7f0 = {
    {1009, 1011, 1013}
};

const CutinTable3 data_ov003_020dd7fc = {
    {1008, 1010, 1012}
};

const CutinTable3 data_ov003_020dd808 = {
    {1014, 1016, 1018}
};

const CutinTable3 data_ov003_020dd814 = {
    {1015, 1017, 1019}
};

const CutinTable3 data_ov003_020dd820 = {
    {1020, 1022, 1024}
};

const CutinTable3 data_ov003_020dd82c = {
    {10, 11, 12}
};

const TaskHandle data_ov003_020dd838 = {data_ov003_020e533c, func_ov003_020be688, sizeof(Ov003CutinTask)};

const TaskStages data_ov003_020dd844 = {
    {func_ov003_020be420, func_ov003_020be59c, func_ov003_020be658, func_ov003_020be660}
};

const CutinResources data_ov003_020dd854[6] = {
    {4, 4, 1, {2, 3}},
    {5, 4, 1, {2, 3}},
    {6, 4, 1, {2, 3}},
    {7, 4, 1, {2, 3}},
    {8, 4, 1, {2, 3}},
    {9, 4, 1, {2, 3}},
};

Ov003CutinTask* data_ov003_020e71d4;
