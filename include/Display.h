#ifndef DISPLAY_H
#define DISPLAY_H

#include <NitroSDK/gx.h>

/// MARK: Defines

enum {
    LAYER_NONE = 0,
    LAYER_BG0  = (1 << 0),
    LAYER_BG1  = (1 << 1),
    LAYER_BG2  = (1 << 2),
    LAYER_BG3  = (1 << 3),
    LAYER_OBJ  = (1 << 4),
};

typedef enum {
    DISPLAY_ENGINE_MAIN = 0,
    DISPLAY_ENGINE_SUB  = 1,
} DisplayEngine;

typedef enum {
    DISPLAY_BG0 = 0,
    DISPLAY_BG1 = 1,
    DISPLAY_BG2 = 2,
    DISPLAY_BG3 = 3,
} DisplayBGLayer;

typedef struct {
    /* 0x00 */ GXDisplayMode dispMode;
    /* 0x04 */ GXBGMode      bgMode;
    /* 0x08 */ GX2D3D        dimension;
    /* 0x0C */ GXOBJTileMode objTileMode; // Size/mapping mode for OBJ tiles
    /* 0x10 */ GXOBJBmpMode  objBmpMode;  // Size/mapping mode for OBJ bitmaps
    /* 0x14 */ BOOL          displaying;  // Whether display is enabled
    /* 0x18 */ u32           layers;      // BG/OBJ layer visibility
    /* 0x1C */ u32           windows;
    /* 0x20 */ BOOL          hBlank;
    /* 0x24 */ u32           screenBase;
    /* 0x28 */ u32           charBase;
    /* 0x2C */ s32           brightness;
} DisplayControlSettings; // Size: 0x30

typedef struct {
    /* 0x00 */ u32 unk_00;
    /* 0x04 */ u32 screenSize;
    /* 0x08 */ u32 unk_08;
    /* 0x0C */ u32 unk_0C;
    /* 0x10 */ u32 unk_10;
    /* 0x14 */ u32 unk_14;
    /* 0x18 */ u32 unk_18;
    /* 0x1C */ u32 colorMode;
    /* 0x20 */ u32 screenBase;
    /* 0x24 */ u32 unk_24;
    /* 0x28 */ u32 charBase;
    /* 0x2C */ u32 extPlttSlot;
    /* 0x30 */ u32 unk_30;
    /* 0x34 */ u32 unk_34;
    /* 0x38 */ u32 unk_38;
} DisplayBGSettings; // Size: 0x3C

typedef struct {
    /* 0x000 */ DisplayBGSettings bgSettings[4];
    /* 0x0F0 */ u32               unk_0F0;
    /* 0x0F4 */ u32               unk_0F4;
    /* 0x0F8 */ u32               unk_0F8;
    /* 0x0FC */ u32               unk_0FC;
    /* 0x100 */ u32               unk_100;
    /* 0x104 */ u32               unk_104;
    /* 0x108 */ u32               unk_108;
    /* 0x10C */ u32               unk_10C;
    /* 0x110 */ char              unk_110[0x58];
    /* 0x168 */ u16               unk_168;
    /* 0x16A */ u16               unk_16A;
    /* 0x16C */ u16               unk_16C;
    /* 0x16E */ u16               unk_16E;
    /* 0x170 */ u32               unk_170;
    /* 0x174 */ u32               unk_174;
    /* 0x178 */ char              unk_178[0x4];
    /* 0x17C */ u32               unk_17C;
    /* 0x180 */ u32               unk_180;
    /* 0x184 */ u32               unk_184;
    /* 0x188 */ u32               unk_188;
    /* 0x18C */ u32               unk_18C;
    /* 0x190 */ u32               unk_190;
    /* 0x194 */ u16               unk_194;
    /* 0x196 */ u16               unk_196;
    /* 0x198 */ u16               unk_198;
    /* 0x19A */ u16               unk_19A;
    /* 0x19C */ u32               unk_19C;
    /* 0x1A0 */ u32               unk_1A0;
    /* 0x1A4 */ char              unk_1A4[0x4];
    /* 0x1A8 */ u32               unk_1A8;
    /* 0x1AC */ u32               unk_1AC;
    /* 0x1B0 */ u32               unk_1B0;
    /* 0x1B4 */ u32               unk_1B4;
    /* 0x1B8 */ u32               unk_1B8;
    /* 0x1BC */ u32               unk_1BC;
    /* 0x1C0 */ u32               window0;              // Layers contained in window 0
    /* 0x1C4 */ BOOL              window0Effects;       // Brightness/blending effects for window 0
    /* 0x1C8 */ u32               windowOutside;        // Layers contained outside windows
    /* 0x1CC */ BOOL              windowOutsideEffects; // Brightness/blending effects outside windows
    /* 0x1D0 */ s32               unk_1D0;
    /* 0x1D4 */ s32               unk_1D4;
    /* 0x1D8 */ s32               unk_1D8;
    /* 0x1DC */ s32               unk_1DC;
    /* 0x1E0 */ u32               window1;          // Layers contained in window 1
    /* 0x1E4 */ BOOL              window1Effects;   // Brightness/blending effects for window 1
    /* 0x1E8 */ u32               windowObj;        // Layers contained in OBJ window
    /* 0x1EC */ u32               windowObjEffects; // Brightness/blending effects for OBJ window
    /* 0x1F0 */ s32               unk_1F0;
    /* 0x1F4 */ s32               unk_1F4;
    /* 0x1F8 */ s32               unk_1F8;
    /* 0x1FC */ s32               unk_1FC;
    /* 0x200 */ u16               unk_200;
    /* 0x202 */ u16               unk_202;
    /* 0x204 */ u16               unk_204;
    /* 0x206 */ u16               unk_206;
    /* 0x208 */ u32               unk_208;
    /* 0x20C */ u32               unk_20C;
    /* 0x210 */ u32               unk_210;
    /* 0x214 */ u32               unk_214;
    /* 0x218 */ u32               unk_218;
    /* 0x21C */ u32               unk_21C;
} UnkStruct_usedby_20050B4; // Size: 0x220

typedef struct {
    /* 0x000 */ s32                      unk_000;
    /* 0x004 */ DisplayControlSettings   mainControl;
    /* 0x034 */ DisplayControlSettings   subControl;
    /* 0x064 */ UnkStruct_usedby_20050B4 unk_064[2];
} DisplaySettings;

extern s32             data_0206aa78;
extern s32             data_0206aa7c;
extern DisplaySettings data_0206aa80;

/// MARK: Functions

void func_02005250(void);

/// MARK: Inlines

static inline DisplayControlSettings* Display_GetMainControl(void) {
    return &data_0206aa80.mainControl;
}

static inline DisplayControlSettings* Display_GetSubControl(void) {
    return &data_0206aa80.subControl;
}

static inline DisplayBGSettings* Display_GetBGSettings(DisplayEngine engine, DisplayBGLayer layer) {
    return &data_0206aa80.unk_064[engine].bgSettings[layer];
}

static inline DisplayBGSettings* Display_GetBG0Settings(DisplayEngine engine) {
    return &data_0206aa80.unk_064[engine].bgSettings[DISPLAY_BG0];
}

static inline DisplayBGSettings* Display_GetBG1Settings(DisplayEngine engine) {
    return &data_0206aa80.unk_064[engine].bgSettings[DISPLAY_BG1];
}

static inline DisplayBGSettings* Display_GetBG2Settings(DisplayEngine engine) {
    return &data_0206aa80.unk_064[engine].bgSettings[DISPLAY_BG2];
}

static inline DisplayBGSettings* Display_GetBG3Settings(DisplayEngine engine) {
    return &data_0206aa80.unk_064[engine].bgSettings[DISPLAY_BG3];
}

static inline void Display_SetMainBrightness(u32 brightness) {
    data_0206aa80.mainControl.brightness = brightness;
}

static inline void Display_SetSubBrightness(u32 brightness) {
    data_0206aa80.subControl.brightness = brightness;
}

#endif // DISPLAY_H