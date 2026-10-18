#ifndef FIELD_FIELDTYPES_H
#define FIELD_FIELDTYPES_H

#include <nitro/types.h>

// A map position in fx32. The field camera origin (FieldEventManager::origin,
// returned by Field_GetCameraOrigin) has this shape, and the field tasks keep a
// pointer to it to turn their map positions into screen positions.
typedef struct FieldPos {
    /* 0x0 */ s32 x;
    /* 0x4 */ s32 y;
} FieldPos; // Size: 0x8

// A screen position in pixels, packed in one word so it can be passed by value
// in a register. Readers use the unsigned or the signed view.
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

// Spawn template of a field NPC (0x1C bytes), passed by value along the spawner
// chain (FieldEncounter -> Fld_Human, Fld_MapJumpArrow).
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

// Creation parameters for a sprite selected only by resource slot and animation.
typedef struct {
    /* 0x0 */ s32 dataType;
    /* 0x4 */ s16 animIndex;
} FldSpriteAnimationParams;

#endif // FIELD_FIELDTYPES_H
