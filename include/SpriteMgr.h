#ifndef UNK_TASK_THING_H
#define UNK_TASK_THING_H

#include "Engine/File/DatMgr.h"
#include "Engine/Resources/ResourceMgr.h"

enum {
    ANIM_MODE_STATIC = 0,
    ANIM_MODE_LOOPED = 1,
    ANIM_MODE_ONCE   = 2,
};

// frameInfoCallback modes, in the order the engine issues them over a sprite's lifetime.
enum {
    SPRITE_FRAME_LOAD    = 0, // end of Sprite_Load; `arg` is SpriteAnimation.callbackArg
    SPRITE_FRAME_UPDATE  = 1, // Sprite_Update
    SPRITE_FRAME_RENDER  = 2, // Sprite_RenderFrame
    SPRITE_FRAME_RELEASE = 3, // Sprite_Release
};

struct OamCellPiece;
struct OamAffineParam;

// Returned by a sprite's frameInfoCallback; callbacks fill and return the shared g_SpriteFrameInfo.
typedef struct {
    /* 0x00 */ s32                    updateSteps; // SPRITE_FRAME_UPDATE: animation command steps to run this update
    /* 0x04 */ s32                    pieceCount;  // SPRITE_FRAME_RENDER: current cell's piece count (not read by the engine)
    /* 0x08 */ struct OamCellPiece*   cellPieces;  // SPRITE_FRAME_RENDER: current cell's OAM piece list, NULL = draw nothing
    /* 0x0C */ struct OamAffineParam* affine;      // SPRITE_FRAME_RENDER: 3D path transform, NULL = derive from OAM attrs
    /* 0x10 */ s32                    sortKey;     // SPRITE_FRAME_RENDER: OamMgr_SubmitCommand key (3D: depth), < 0 = unsorted
} SpriteFrameInfo;
extern SpriteFrameInfo g_SpriteFrameInfo;

struct Sprite;
typedef SpriteFrameInfo* (*SpriteFrameInfoCallback)(struct Sprite* sprite, s32 arg, s32 mode);

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

// One entry of a sprite's cell table (a pack entry). Entry 0's charByteSize doubles as the
// largest cell's size, which Sprite_Load uses to reserve VRAM for per-cell char transfers.
typedef struct {
    /* 0x00 */ u16 pieceOffset;   // offset to this cell's OamCellPiece list, in u16 units from the table start
    /* 0x02 */ u16 pieceCount;    // pieces before the 0xFFFF terminator
    /* 0x04 */ u16 charTileCount; // char data size in 4bpp tiles
    /* 0x06 */ u16 charByteSize;  // charTileCount * 0x20
} SpriteCell;

typedef union {
    struct {
        u16 unk_00 : 1;
        u16 unk_01 : 1;
        u16 unk_02 : 2;
        u16 unk_04 : 1;
        u16 unk_05 : 5; // Flip flags?
        u16 unk_10 : 2;
        u16 unk_12 : 4; // palette Index?
    };
    u16 raw;
} Unk_Bitfield;

typedef struct Sprite {
    /* 0x00 */             // struct {
    u32 bits_0_1      : 2; // DisplayEngine? (Top vs Bottom screen) indexed into g_ObjResourceManagers and g_PaletteManagers
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
    /* 0x04 */ u16                     currentFrame;
    /* 0x06 */ u16                     loopFrame;
    /* 0x08 */ s16                     frameTimer;
    /* 0x0A */ Unk_Bitfield            unk_0A; // OAM Attributes?
    /* 0x0C */ s16                     posX;
    /* 0x0E */ s16                     posY;
    /* 0x10 */ s16                     scaleX;
    /* 0x12 */ s16                     scaleY;
    /* 0x14 */ s16                     animIndex;
    /* 0x16 */ s16                     cellIndex; // cellTable entry being shown, -1 = none
    /* 0x18 */ s16*                    animData;  // List of animation commands?
    /* 0x1C */ SpriteCell*             cellTable;
    /* 0x20 */ SpriteFrameInfoCallback frameInfoCallback;
    /* 0x24 */ void*                   owner; // SpriteAnimation.owner; callbacks read their task/object through it
    /* 0x28 */ Data*                   resourceData;
    /* 0x2C */ ObjResource*            charData;
    /* 0x30 */ PaletteResource*        paletteData;
    /* 0x34 */ void*                   unk34;          // charDataSource? Pack entry data for char info?
    /* 0x38 */ struct OamCellPiece*    lastCellPieces; // pieces whose char data was last sent to VRAM (per-cell mode)
    /* 0x3C */ UnkSmallInternal*       unk3C;          // paletteDataSource? Pack entry data for palette info?
} Sprite;                                              // Size: 0x40

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
    /* 0x02 */ Unk_Bitfield            unk_02;            // OAM Attributes?
    /* 0x04 */ s16                     unk_04;            // posX?
    /* 0x06 */ s16                     unk_06;            // posY?
    /* 0x08 */ SpriteFrameInfoCallback frameInfoCallback; // NULL keeps the engine default
    /* 0x0C */ s32                     callbackArg;       // passed to frameInfoCallback with SPRITE_FRAME_LOAD
    /* 0x10 */ void*                   owner;             // copied to Sprite.owner
    /* 0x14 */ const BinIdentifier*    binIden;
    /* 0x18 */ u16                     unk_18;            // ResourceLoadType?
    /* 0x1A */ s16                     packIndex;
    /* 0x1C */ s16                     unk_1C;            // charDataIndex?
    /* 0x1E */ s16                     unk_1E;            // charDataFormat?
    /* 0x20 */ s16                     unk_20;            // paletteDataIndex?
    /* 0x22 */ s16                     unk_22;            // paletteSlots?
    /* 0x24 */ u16                     unk_24;
    /* 0x26 */ s16                     unk_26;            // cellTableIndex?
    /* 0x28 */ s16                     unk_28;            // animDataIndex?
    /* 0x2A */ s16                     unk_2A;
} SpriteAnimation;

// Fills g_SpriteFrameInfo-style render info from the sprite's current cell
static inline void Sprite_FillFrameInfo(SpriteFrameInfo* info, Sprite* sprite) {
    info->pieceCount = 0;
    info->cellPieces = NULL;
    info->affine     = NULL;
    info->sortKey    = -1;
    if (sprite->animData != NULL && sprite->cellTable != NULL && sprite->cellIndex >= 0) {
        info->pieceCount = *((u16*)sprite->cellTable + (sprite->cellIndex * 4 + 1));
        info->cellPieces =
            (struct OamCellPiece*)((u16*)sprite->cellTable + *((u16*)((u8*)sprite->cellTable + (sprite->cellIndex * 8))));
    }
}

#define Sprite_FrameInfoCallback(sprite, mode)               \
    {                                                        \
        SpriteFrameInfo* info_ = NULL;                       \
        switch (mode) {                                      \
            case SPRITE_FRAME_UPDATE:                        \
                g_SpriteFrameInfo.updateSteps = 1;           \
                return &g_SpriteFrameInfo;                   \
            case SPRITE_FRAME_RENDER: {                      \
                SpriteFrameInfo* temp_ = &g_SpriteFrameInfo; \
                Sprite_FillFrameInfo(temp_, sprite);         \
                info_ = temp_;                               \
            } break;                                         \
        }                                                    \
        return info_;                                        \
    }

#define Sprite_FrameInfoCallbackEarlyImpl(sprite, mode, STMT) \
    {                                                         \
        SpriteFrameInfo* info_ = &g_SpriteFrameInfo;          \
        switch (mode) {                                       \
            case SPRITE_FRAME_UPDATE:                         \
                info_->updateSteps = 1;                       \
                return info_;                                 \
            case SPRITE_FRAME_RENDER:                         \
                info_ = &g_SpriteFrameInfo;                   \
                Sprite_FillFrameInfo(info_, sprite);          \
                STMT;                                         \
                return info_;                                 \
        }                                                     \
        return NULL;                                          \
    }

#define Sprite_FrameInfoCallbackEarly(sprite, mode) Sprite_FrameInfoCallbackEarlyImpl(sprite, mode, (void)0)
#define Sprite_FrameInfoCallbackSorted(sprite, mode, sortKey_)                   \
    Sprite_FrameInfoCallbackEarlyImpl(sprite, mode, info_->sortKey = (sortKey_))

void Sprite_Init(Sprite* sprite);

void Sprite_Update(Sprite* sprite);

void Sprite_RenderFrame(Sprite* sprite);

s32 Sprite_Load(Sprite* sprite, SpriteAnimation* anim);

void Sprite_Release(Sprite* sprite);

BOOL Sprite_HasAnimation(Sprite* sprite);

BOOL SpriteMgr_IsAnimationFinished(Sprite* sprite);

BOOL SpriteMgr_IsFrameFinished(Sprite* sprite);

void Sprite_SetAnimation(Sprite* sprite, s16* animData, s16 animFrame, SpriteCell* cellTable);

s32 Sprite_ChangeAnimation(Sprite* sprite, void*, s16, SpriteCell* cellTable);

s32 Sprite_ChangePalette(Sprite* sprite, s32, void*, u16, u32);

void Sprite_Restart(Sprite* sprite);

void Sprite_RenderAltPalette(Sprite* sprite, PaletteResource*, UnkSmallInternal*, s16);

s32 _Sprite_Load(Sprite* sprite, SpriteAnimation* anim);

BOOL Sprite_UpdateAndCheck(Sprite* sprite);

void Sprite_Render(Sprite* sprite);

void Sprite_Destroy(Sprite* sprite);

#endif // UNK_TASK_THING_H