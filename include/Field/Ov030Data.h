#ifndef FIELD_OV030DATA_H
#define FIELD_OV030DATA_H

// ov030 data objects that several Field translation units reference. Keeping the
// layouts and extern declarations in one place avoids each TU hand-rolling its
// own view (which previously produced four near-duplicate typedefs).

#include "Engine/File/DatMgr.h" // BinIdentifier
#include "Field/FieldTypes.h"
#include <nitro/fx.h>           // Mtx44

// Engine-side render state shared by FieldSceneRender / FieldSceneLoad /
// FieldAreaParameters: only the +0xFC control word is touched (bit 4 is cleared
// before each ortho reload), and +0x08 has its own delinked symbol.
typedef struct {
    /* 0x00 */ char unk_00[0xFC];
    /* 0xFC */ s32  unk_FC;
} Data02076A44; // Size: 0x100
extern Data02076A44 data_02076a44;
extern Mtx44        data_02076a4c;

// Message-window task parameters, shared by Fld_Badge and Fld_ScanFukidasi.
typedef struct {
    /* 0x00 */ s32          datSlot; // backing task id
    /* 0x04 */ FldScreenPos unk_04;
    /* 0x08 */ FldScreenPos unk_08;
    /* 0x0C */ s32          unk_0C;
    /* 0x10 */ s32          mesNo;
    /* 0x14 */ s16          unk_14;
    /* 0x16 */ s16          unk_16;
    /* 0x18 */ u8           winKind;
    /* 0x19 */ u8           unk_19;
    /* 0x1A */ s8           unk_1A;
    /* 0x1B */ s8           unk_1B;
    /* 0x1C */ s32          unk_1C;
    /* 0x20 */ s32          unk_20;
    /* 0x24 */ s16          unk_24;
} FldMesTaskParam; // Size: 0x28

// Piecewise-linear lookup row: the first row whose limit exceeds the input
// gives base + input * slope (fx32). FieldFloorHeight and FieldAreaZeroFloor.
typedef struct {
    /* 0x0 */ s32 limit;
    /* 0x4 */ s32 slope;
    /* 0x8 */ s32 base;
} FieldSlope; // Size: 0xC

// Per-area map parameters, indexed by MainData::unk_2458.
typedef struct {
    /* 0x00 */ s16 unk_00;
    /* 0x02 */ s16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ s32 unk_08;
    /* 0x0C */ s32 unk_0C;
    /* 0x10 */ s16 unk_10;
    /* 0x12 */ u16 unk_12;
    /* 0x14 */ s32 unk_14;
    /* 0x18 */ s16 unk_18;
    /* 0x1A */ s16 unk_1A;
    /* 0x1C */ s16 unk_1C;
    /* 0x1E */ u16 unk_1E;
} FieldAreaParameters; // Size: 0x20

extern const FieldAreaParameters data_ov030_020e1d64[];

// Pack identifier used by several Fld_* SpriteAnimation templates.
extern const BinIdentifier data_ov030_020d9944;

#endif // FIELD_OV030DATA_H
