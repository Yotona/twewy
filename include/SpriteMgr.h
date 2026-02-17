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
    s32 unk_08; // VRAM Offset? OAM entry pointer?
    s32 unk_0C;
    s32 unk_10; // Render flag?
} SpriteFrameInfo;
extern SpriteFrameInfo data_0206b408;

typedef struct UnkSmallInternal {
    /* 0x00 */ char                     unk_00[0x6];
    /* 0x06 */ u16                      unk_06;
    /* 0x08 */ char                     unk_08[0x4];
    u32                                 unk_0C_bits_0_2  : 3; // current paletteSlot?
    u32                                 unk_0C_bits_3_31 : 29;
    /* 0x10 */ s16                      unk_10;               // index to something? current frame?
    s16                                 unk_12;               // max frame?
    char                                unk_14[0x4];
    /* 0x18 */ struct UnkSmallInternal* unk_18;               // vramData?
} UnkSmallInternal;

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 unk_02;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 unk_06;
} SpriteFrameData;

typedef union {
    struct {
        u16 unk_00 : 1;
        u16 unk_01 : 4;
        u16 unk_05 : 5; // Flip flags?
        u16 unk_10 : 2;
        u16 unk_12 : 4; // palette Index?
    };
    u16 raw;
} Unk_Bitfield;

typedef struct Sprite {
    /* 0x00 */             // struct {
    u32 bits_0_1      : 2; // DisplayEngine? (Top vs Bottom screen) indexed into data_0206b3d8 and data_0206b3cc
    u32 bit_2         : 1;
    u32 bits_3_4      : 2; // Rendering mode?
    u32 animationMode : 2;
    u32 bits_7        : 1; // visible/enabled?
    u32 bits_8        : 1;
    u32 bits_9        : 1;
    u32 isPlaying     : 1;
    u32 bit_11        : 1; // Frame-change flag?
    u32 isSingleFrame : 1;
    u32 bit_13        : 1; // owns Char Data?
    u32 bit_14        : 1; // owns Palette Data?
    u32 bits_15_18    : 4;
    u32 bit_19        : 1;
    u32 bits_20_25    : 6;
    u32 bits_26_29    : 4;
    u32 bits_30_31    : 2;
    //};
    /* 0x04 */ u16              currentFrame;
    /* 0x06 */ u16              loopFrame;
    /* 0x08 */ s16              frameTimer;
    /* 0x0A */ Unk_Bitfield     unk_0A; // OAM Attributes?
    /* 0x0C */ s16              posX;
    /* 0x0E */ s16              posY;
    /* 0x10 */ s16              scaleX;
    /* 0x12 */ s16              scaleY;
    /* 0x14 */ s16              animIndex;
    /* 0x16 */ s16              unk16;          // frameDataTable index?
    /* 0x18 */ s16*             animData;       // List of animation commands?
    /* 0x1C */ SpriteFrameData* frameDataTable; // what data does this store?
    /* 0x20 */ SpriteFrameInfo* (*frameInfoCallback)(struct Sprite*, s32, s32);
    /* 0x24 */ s32               unk24;
    /* 0x28 */ Data*             resourceData;
    /* 0x2C */ UnkSmallInternal* charData;
    /* 0x30 */ UnkSmallInternal* paletteData;
    /* 0x34 */ void*             unk34; // charDataSource? Pack entry data for char info?
    /* 0x38 */ s32               unk38; // VRAM Offset for rendering?
    /* 0x3C */ UnkSmallInternal* unk3C; // paletteDataSource? Pack entry data for palette info?
} Sprite;                               // Size: 0x40

typedef struct {
    /* 0x00 */ struct {
        u16 bits_0_1   : 2;
        u16 dataType   : 4;
        u16 bit_6      : 1;
        u16 bits_7_9   : 3; // Current palette slot?
        u16 bits_10_11 : 2; // Rendering Mode?
        u16 bits_12_13 : 2;
        u16 bits_14_15 : 2;
    };
    /* 0x02 */ Unk_Bitfield unk_02; // OAM Attributes?
    /* 0x04 */ s16          unk_04; // posX?
    /* 0x06 */ s16          unk_06; // posY?
    /* 0x08 */ SpriteFrameInfo* (*unk_08)(struct Sprite*, s32, s32);
    /* 0x0C */ s32            unk_0C;
    /* 0x10 */ s32            unk_10;
    /* 0x14 */ BinIdentifier* binIden;
    /* 0x18 */ u16            unk_18; // ResourceLoadType?
    /* 0x1A */ s16            packIndex;
    /* 0x1C */ s16            unk_1C; // charDataIndex?
    /* 0x1E */ s16            unk_1E; // charDataFormat?
    /* 0x20 */ s16            unk_20; // paletteDataIndex?
    /* 0x22 */ s16            unk_22; // paletteSlots?
    /* 0x24 */ u16            unk_24;
    /* 0x26 */ s16            unk_26; // frameDataIndex?
    /* 0x28 */ s16            unk_28; // animDataIndex?
    /* 0x2A */ s16            unk_2A;
} SpriteAnimation;

void Sprite_Init(Sprite* sprite);

void Sprite_Update(Sprite* sprite);

void Sprite_RenderFrame(Sprite* sprite);

s32 Sprite_Load(Sprite* sprite, SpriteAnimation* anim);

void Sprite_Release(Sprite* sprite);

BOOL Sprite_HasAnimation(Sprite* sprite);

BOOL SpriteMgr_IsAnimationFinished(Sprite* sprite);

BOOL SpriteMgr_IsFrameFinished(Sprite* sprite);

void Sprite_SetAnimation(Sprite* sprite, s16* animData, s16 animFrame, SpriteFrameData* frameDataTable);

s32 Sprite_ChangeAnimation(Sprite* sprite, void*, s16, SpriteFrameData* frameDataTable);

s32 Sprite_ChangePalette(Sprite* sprite, s32, void*, u32, u32);

void Sprite_Restart(Sprite* sprite);

void Sprite_RenderAltPalette(Sprite* sprite, UnkSmallInternal*, UnkSmallInternal*, s16);

s32 _Sprite_Load(Sprite* sprite, SpriteAnimation* anim);

BOOL Sprite_UpdateAndCheck(Sprite* sprite);

void Sprite_Render(Sprite* sprite);

void Sprite_Destroy(Sprite* sprite);

#endif // UNK_TASK_THING_H