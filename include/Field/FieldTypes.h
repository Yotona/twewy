#ifndef FIELD_FIELDTYPES_H
#define FIELD_FIELDTYPES_H

#include <nitro/types.h>

// A map position in fx32.
typedef struct FieldPos {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} FieldPos; // Size: 0x8

// A screen position in pixels, packed in one word. Readers use the unsigned or the signed view.
typedef union FldScreenPos {
    struct {
        /* 0x0 */ u16 x;
        /* 0x2 */ u16 y;
    };
    struct {
        /* 0x0 */ s16 sx;
        /* 0x2 */ s16 sy;
    };
} FldScreenPos; // Size: 0x4

// Spawn template of a field NPC, passed by value along the spawner chain.
typedef struct FldSpawnTemplate {
    /* 0x00 */ FieldPos pos;
    /* 0x08 */ s16      unk_08;
    /* 0x0A */ s16      unk_0A;
    /* 0x0C */ s16      unk_0C;
    /* 0x0E */ s16      unk_0E;
    /* 0x10 */ s32      unk_10;
    /* 0x14 */ s32      unk_14;
    /* 0x18 */ s32      unk_18;
} FldSpawnTemplate; // Size: 0x1C

// A map exit: an entry of an area's exit table.
typedef struct FieldExit {
    /* 0x0 */ s32 region;     // trigger region kind
    /* 0x4 */ s16 destArea;   // area the exit leads to, 0x46 = none
    /* 0x6 */ s16 unk_06;
    /* 0x8 */ s32 destRegion; // region to arrive at; 0x66 = FieldEventManager::unk_21B08/unk_21B0C
    /* 0xC */ s32 exitId;     // MainData::exitStates index
} FieldExit;                  // Size: 0x10

// Creation parameters for a sprite selected only by resource slot and animation.
typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 animIndex;
} FldSpriteAnimationParams;

#endif // FIELD_FIELDTYPES_H
