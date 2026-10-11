#ifndef FIELD_FLDHUMAN_H
#define FIELD_FLDHUMAN_H

// Field NPCs (Fld_Human.c); FieldSceneRender.c holds their accessors.

#include "Engine/Core/OamMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Field/FieldTypes.h"
#include "SpriteMgr.h"

// Wander area entry; a route (FldHumanInit::route) walks from one rectangle to
// the other, the sign of the route picking the direction.
typedef struct {
    /* 0x00 */ s16 x0;
    /* 0x02 */ s16 y0;
    /* 0x04 */ s16 w0;
    /* 0x06 */ s16 h0;
    /* 0x08 */ s16 x1;
    /* 0x0A */ s16 y1;
    /* 0x0C */ s16 w1;
    /* 0x0E */ s16 h1;
    /* 0x10 */ s16 unk_10;
} FldHumanSpawnEntry; // Size: 0x12

// Creation parameters; the task copies them to the start of its data.
typedef struct {
    /* 0x00 */ FieldPos*           origin;
    /* 0x04 */ FieldPos            start;
    /* 0x0C */ s32                 unk_0C;
    /* 0x10 */ s32                 dataType;
    /* 0x14 */ FldSpawnTemplate    tpl;
    /* 0x30 */ s32                 unk_30;
    /* 0x34 */ s32                 speed;
    /* 0x38 */ FldHumanSpawnEntry* spawns;
    /* 0x3C */ s16                 unk_3C;
    /* 0x3E */ s16                 route;
    /* 0x40 */ s32                 angle;
    /* 0x44 */ FieldPos            target;
} FldHumanInit; // Size: 0x4C

typedef struct FldHuman {
    /* 0x00 */ FldHumanInit     init;
    /* 0x4C */ s32              scale;
    /* 0x50 */ FieldPos         screen; // pos - init.origin
    /* 0x58 */ FieldPos         pos;
    /* 0x60 */ s32              flip;
    /* 0x64 */ s32              unk_64;
    /* 0x68 */ s32              unk_68;
    /* 0x6C */ const FieldPos*  margin;
    /* 0x70 */ s32              unk_70;
    /* 0x74 */ OamAffineParam   affine;
    /* 0x84 */ s32              unk_84;
    /* 0x88 */ s32              unk_88;
    /* 0x8C */ s32              unk_8C;
    /* 0x90 */ s32              unk_90;
    /* 0x94 */ s16              unk_94;
    /* 0x96 */ s16              pad_96;
    /* 0x98 */ Sprite*          unk_98;
    /* 0x9C */ Sprite*          unk_9C;
    /* 0xA0 */ char             unk_A0[4];
    /* 0xA4 */ PaletteResource* palette;
    /* 0xA8 */ u16              oamAttr2;
    /* 0xAA */ u16              pad_AA;
    /* 0xAC */ void (*unk_AC)(struct FldHuman*);
} FldHuman; // Size: 0xB0

#endif      // FIELD_FLDHUMAN_H
