#ifndef FIELD_FLDPLAYER_H
#define FIELD_FLDPLAYER_H

#include "Field/FieldChara.h"
#include "Field/TaskHelpers.h"

// The field player task object: a Sprite at offset 0 extended with control
// state, plus a second Sprite at 0x5C used for the shadow/aura pass.
struct Ov030Player {
    /* 0x00 */ FldCharaSprite      chara;
    /* 0x50 */ s32                 unk50;
    /* 0x54 */ s32                 unk54;
    /* 0x58 */ u8                  pad58[0x4];
    /* 0x5C */ Sprite              sprite2;
    /* 0x9C */ s16                 unk9C;
    /* 0x9E */ s16                 unk9E;
    /* 0xA0 */ s16                 unkA0;
    /* 0xA2 */ s16                 unkA2;
    /* 0xA4 */ Ov030PlayerCallback unkA4;
    /* 0xA8 */ u16                 unkA8;
    /* 0xAA */ u8                  padAA[0x2];
    /* 0xAC */ s32                 unkAC;
    /* 0xB0 */ s32                 unkB0;
    /* 0xB4 */ u8                  padB4[0x4];
    /* 0xB8 */ FieldPos*           origin; // field camera origin
    /* 0xBC */ FieldPos*           pos;    // map position (MainData::fieldPos)
    /* 0xC0 */ FieldPos            screen; // pos - origin
    /* 0xC8 */ s32                 unkC8;
    /* 0xCC */ u8                  padCC[0x4];
    /* 0xD0 */ s16                 unkD0;
    /* 0xD2 */ u8                  padD2[0x2];
    /* 0xD4 */ s32                 unkD4;
    /* 0xD8 */ s32                 unkD8;
    /* 0xDC */ u8                  padDC[0xC];
    /* 0xE8 */ s32                 unkE8;
    /* 0xEC */ s32                 unkEC;
    /* 0xF0 */ s32                 unkF0;
}; // Size: 0xF4

#endif // FIELD_FLDPLAYER_H
