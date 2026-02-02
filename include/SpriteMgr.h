#ifndef UNK_TASK_THING_H
#define UNK_TASK_THING_H

#include "DatMgr.h"

enum {
    ANIM_MODE_STATIC = 0,
    ANIM_MODE_LOOPED = 1,
    ANIM_MODE_ONCE   = 2,
};

typedef struct {
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
} SpriteFrameInfo;
extern SpriteFrameInfo data_0206b408;

typedef struct UnkSmallInternal {
    /* 0x00 */ char                     unk_00[0x6];
    /* 0x06 */ u16                      unk_06;
    /* 0x08 */ char                     unk_08[0x4];
    /* 0x0C */ s32                      unk_0C;
    /* 0x10 */ s16                      unk_10;
    /* 0x12 */ char                     unk_12[0x6];
    /* 0x18 */ struct UnkSmallInternal* unk_18;
} UnkSmallInternal;

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
} SpriteFrameData;

typedef struct Sprite {
    /* 0x00 */ struct {
        u32 bits_0_1      : 2;
        u32 bit_2         : 1;
        u32 bits_3_4      : 2;
        u32 animationMode : 2;
        u32 bits_7_9      : 3;
        u32 isPlaying     : 1;
        u32 bit_11        : 1;
        u32 isSingleFrame : 1;
        u32 bit_13        : 1;
        u32 bit_14        : 1;
        u32 bits_15_18    : 4;
        u32 bit_19        : 1;
        u32 bits_20_25    : 6;
        u32 bits_26_29    : 4;
        u32 bits_30_31    : 2;
    };
    /* 0x04 */ u16              currentFrame;
    /* 0x06 */ u16              loopFrame;
    /* 0x08 */ s16              frameTimer;
    /* 0x0A */ u16              unk_0A;
    /* 0x0C */ s16              posX;
    /* 0x0E */ s16              posY;
    /* 0x10 */ u16              scaleX;
    /* 0x12 */ u16              scaleY;
    /* 0x14 */ s16              animIndex;
    /* 0x16 */ s16              unk16;
    /* 0x18 */ s16*             animData;
    /* 0x1C */ SpriteFrameData* unk1C;
    /* 0x20 */ SpriteFrameInfo* (*unk_20)(struct Sprite*, s32, s32);
    /* 0x24 */ s32               unk24;
    /* 0x28 */ Data*             resourceData;
    /* 0x2C */ UnkSmallInternal* charData;
    /* 0x30 */ UnkSmallInternal* paletteData;
    /* 0x34 */ void*             unk34;
    /* 0x38 */ s32               unk38;
    /* 0x3C */ UnkSmallInternal* unk3C;
} Sprite; // Size: 0x40

typedef struct {
    /* 0x00 */ struct {
        u16 bits_0_1   : 2;
        u16 dataType   : 4;
        u16 bit_6      : 1;
        u16 bits_7_9   : 3;
        u16 bits_10_11 : 2;
        u16 bits_12_13 : 2;
        u16 bits_14_15 : 2;
    };
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ s16 unk_04;
    /* 0x06 */ s16 unk_06;
    /* 0x08 */ SpriteFrameInfo* (*unk_08)(struct Sprite*, s32, s32);
    /* 0x0C */ s32            unk_0C;
    /* 0x10 */ s32            unk_10;
    /* 0x14 */ BinIdentifier* binIden;
    /* 0x18 */ u16            unk_18;
    /* 0x1A */ s16            packIndex;
    /* 0x1C */ s16            unk_1C;
    /* 0x1E */ s16            unk_1E;
    /* 0x20 */ s16            unk_20;
    /* 0x22 */ s16            unk_22;
    /* 0x24 */ u16            unk_24;
    /* 0x26 */ s16            unk_26;
    /* 0x28 */ s16            unk_28;
    /* 0x2A */ s16            unk_2A;
} SpriteAnimation;

void Sprite_Init(Sprite* sprite);

void Sprite_Update(Sprite* sprite);

void Sprite_RenderFrame(Sprite* sprite);

void Sprite_Load(Sprite* sprite, SpriteAnimation* anim);

void Sprite_Release(Sprite* sprite);

BOOL Sprite_HasAnimation(Sprite* sprite);

BOOL func_0200ea4c(Sprite* sprite);

BOOL func_0200ea98(Sprite* sprite);

void Sprite_SetAnimation(Sprite* sprite, s16* animData, s16 animFrame, s32);

s32 Sprite_ChangeAnimation(Sprite* sprite, void*, s16, void*);

s32 Sprite_ChangePalette(Sprite* sprite, s32, void*, u32, u32);

void Sprite_Restart(Sprite* sprite);

void Sprite_RenderAltPalette(Sprite* sprite, UnkSmallInternal*, UnkSmallInternal*, s16);

void _Sprite_Load(Sprite* sprite, SpriteAnimation* anim);

BOOL Sprite_UpdateAndCheck(Sprite* sprite);

void Sprite_Render(Sprite* sprite);

void Sprite_Destroy(Sprite* sprite);

#endif // UNK_TASK_THING_H