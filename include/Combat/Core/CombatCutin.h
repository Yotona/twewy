#ifndef COMBAT_COMBATCUTIN_H
#define COMBAT_COMBATCUTIN_H

#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatActor.h"
#include "Combat/Core/CombatSprite.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "Engine/Resources/ScreenMapper.h"

// Cut-in tile entries store a size word before their tile payload.
typedef struct Ov003CutinTileBlock {
    u32 size;
    u8  tiles[];
} Ov003CutinTileBlock;

typedef struct Ov003CutinPaletteEntries {
    s32 entries[3];
} Ov003CutinPaletteEntries;

static inline void* Ov003Cutin_Tiles(Data* pack, s32 index) {
    return ((Ov003CutinTileBlock*)Data_GetPackEntryData(pack, index))->tiles;
}

typedef union Ov003CutinFlags {
    /* 0x0 */ u16 raw;
    struct {
        /* bit 0 */ u16       b0   : 1;
        /* bit 1 */ u16       b1   : 1;
        /* bit 2 */ u16       b2   : 1;
        /* bit 3 */ u16       b3   : 1;
        /* bit 4 */ u16       b4   : 1;
        /* bit 5 */ u16       b5   : 1;
        /* bit 6 */ u16       b6   : 1;
        /* bit 7 */ u16       b7   : 1;
        /* bit 8 */ u16       b8   : 1;
        /* bit 9 */ u16       b9   : 1;
        /* bit 10 */ u16      b10  : 1;
        /* bit 11 */ u16      b11  : 1;
        /* bits 12..15 */ u16 rest : 4;
    } bits;
} Ov003CutinFlags; // Size: 0x2

typedef struct Ov003ActorParams {
    /* 0x00 */ u8              unk_00;
    /* 0x01 */ u8              unk_01;
    /* 0x02 */ u8              unk_02;
    /* 0x03 */ u8              unk_03;
    /* 0x04 */ u8              unk_04;
    /* 0x05 */ s8              unk_05;
    /* 0x06 */ s8              unk_06;
    /* 0x07 */ u8              unk_07;
    /* 0x08 */ u16             unk_08;
    /* 0x0A */ s16             unk_0A;
    /* 0x0C */ s16             unk_0C;
    /* 0x0E */ s16             unk_0E;
    /* 0x10 */ s16             unk_10;
    /* 0x12 */ s16             unk_12;
    /* 0x14 */ u16             unk_14;
    /* 0x16 */ Ov003CutinFlags flags16;
    /* 0x18 */ s8              unk_18;
    /* 0x19 */ s8              unk_19;
    /* 0x1A */ u8              unk_1A[0x1C - 0x1A];
    /* 0x1C */ s16             unk_1C;
    /* 0x1E */ s16             unk_1E;
    /* 0x20 */ s16             unk_20;
    /* 0x22 */ s16             unk_22;
    /* 0x24 */ s16             unk_24;
    /* 0x26 */ s16             unk_26;
    /* 0x28 */ u16             unk_28;
    /* 0x2A */ u16             unk_2A;
} Ov003ActorParams; // Size: 0x2C

typedef struct Ov003CutinMotion {
    /* 0x00 */ s32            engine;
    /* 0x04 */ s32            x;
    /* 0x08 */ s32            y; // stored doubled
    /* 0x0C */ s32            z;
    /* 0x10 */ s32            scaleX;
    /* 0x14 */ s32            scaleY;
    /* 0x18 */ u16            flags;
    /* 0x1A */ u16            unk_1A;
    /* 0x1C */ s32            unk_1C;
    /* 0x20 */ s32            offsetX;
    /* 0x24 */ s32            offsetY;
    /* 0x28 */ CombatListNode node;
} Ov003CutinMotion; // Size: 0x34

/// The first 0x1C0 bytes of a cut-in actor: what `func_ov003_020c41e8` resets. Enemy shot tasks embed only this part
/// and place their own state behind it.
typedef struct Ov003EnemyActor {
    /* 0x000 */ CombatActor             actor;
    /* 0x07C */ s32                     group; // index into data_ov003_020e70ac
    /* 0x080 */ u16                     spriteIndex;
    /* 0x082 */ u16                     id;    // running cut-in instance id
    /* 0x084 */ CombatSprite            sprite;
    /* 0x0E4 */ CombatSprite            subSprite;
    /* 0x144 */ Ov003CutinMotion        motion;
    /* 0x178 */ CombatListNode          node;
    /* 0x184 */ struct Ov003CutinActor* partner;
    /* 0x188 */ u8                      unk_188[0x18C - 0x188];
    /* 0x18C */ u16                     flags18C;
    /* 0x18E */ s16                     unk_18E;
    /* 0x190 */ s16                     unk_190;
    /* 0x192 */ s16                     unk_192;
    /* 0x194 */ s16                     unk_194;
    /* 0x196 */ Ov003CutinFlags         flags196; // { ...; hasPalette : 1 @0x100; ... }
    /* 0x198 */ s16                     unk_198;
    /* 0x19A */ s16                     unk_19A;
    /* 0x19C */ s16                     unk_19C;
    /* 0x19E */ s16                     unk_19E;
    /* 0x1A0 */ s16                     unk_1A0;
    /* 0x1A2 */ s16                     unk_1A2;
    /* 0x1A4 */ s16                     unk_1A4;
    /* 0x1A6 */ s16                     unk_1A6;
    /* 0x1A8 */ s16                     unk_1A8;
    /* 0x1AA */ s16                     unk_1AA;
    /* 0x1AC */ s32                     unk_1AC;
    /* 0x1B0 */ s32                     unk_1B0;
    /* 0x1B4 */ s32                     unk_1B4;
    /* 0x1B8 */ s32                     unk_1B8;
    /* 0x1BC */ s32                     unk_1BC;
} Ov003EnemyActor; // Size: 0x1C0

typedef struct Ov003CutinActor {
    Ov003EnemyActor;
    /* 0x1C0 */ u8  unk_1C0[0x1D0 - 0x1C0];
    /* 0x1D0 */ s32 velocityX;
    /* 0x1D4 */ s32 velocityY;
    /* 0x1D8 */ s32 velocityZ;
    /* 0x1DC */ s32 targetX;
    /* 0x1E0 */ s32 targetY;
    /* 0x1E4 */ s32 targetZ;
    /* 0x1E8 */ s32 unk_1E8;
    /* 0x1EC */ s32 timer;
    /* 0x1F0 */ s32 timerLimit;
    /* 0x1F4 */ s32 state;
} Ov003CutinActor; // Size: 0x1F8

typedef union Ov003CutinFlags8 {
    /* 0x0 */ u8 raw;
    struct {
        /* bit 0 */ u8     b0   : 1;
        /* bit 1 */ u8     b1   : 1;
        /* bit 2 */ u8     b2   : 1;
        /* bit 3 */ u8     b3   : 1;
        /* bits 4..7 */ u8 rest : 4;
    } bits;
} Ov003CutinFlags8; // Size: 0x1

typedef struct Ov003CutinGlobal {
    /* 0x00 */ u16              unk_00;
    /* 0x02 */ u8               unk_02;
    /* 0x03 */ u8               unk_03;
    /* 0x04 */ Ov003CutinFlags8 flags;
    /* 0x05 */ u8               unk_05[0x8 - 0x5];
    /* 0x08 */ void*            taskMain; // main-engine cut-in task data
    /* 0x0C */ void*            taskSub;
    /* 0x10 */ void*            unk_10;
    /* 0x14 */ void*            cameraMain;
    /* 0x18 */ void*            cameraSub;
    /* 0x1C */ void*            unk_1C;
} Ov003CutinGlobal; // Size: 0x20

typedef struct Ov003CutinBackground {
    /* 0x00 */ Data*            pack;
    /* 0x04 */ BgResource*      characters;
    /* 0x08 */ PaletteResource* palette;
    /* 0x0C */ ScreenMapEntry   maps[2];
    /* 0x5C */ void*            tileData[2];
    /* 0x64 */ s32              offsets[2];
} Ov003CutinBackground; // Size: 0x6C

typedef struct Ov003CutinTask {
    /* 0x000 */ Ov003CutinBackground backgrounds[2];
    /* 0x0D8 */ u16                  kind;
    /* 0x0DA */ u16                  voiceKind;
    /* 0x0DC */ s32                  unk_DC;
    /* 0x0E0 */ s32                  unk_E0;
    /* 0x0E4 */ u16                  unk_E4;
    /* 0x0E6 */ u16                  reserved_E6;
    /* 0x0E8 */ Data*                overlayPack;
    /* 0x0EC */ u32                  reserved_EC;
    /* 0x0F0 */ BgResource*          overlayCharacters[2];
    /* 0x0F8 */ PaletteResource*     overlayPalettes[2];
    /* 0x100 */ s16                  overlayTimer;
    /* 0x102 */ u16                  reserved_102;
    /* 0x104 */ s32                  overlayOffsets[2];
    /* 0x10C */ ScreenMapEntry       overlayMaps[2];
    /* 0x15C */ void*                overlayTileData[2][3];
    /* 0x174 */ s16                  elapsed;
    /* 0x176 */ u16                  reserved_176;
    /* 0x178 */ void (*update)(struct Ov003CutinTask*);
    /* 0x17C */ s16 timer;
    /* 0x17E */ s16 phase;
    /* 0x180 */ s32 active;
    /* 0x184 */ u32 reserved_184;
} Ov003CutinTask; // Size: 0x188

typedef struct Ov003CutinArgs {
    u16 kind;
    u16 partner;
} Ov003CutinArgs; // Size: 0x4

typedef struct Ov003ActorSnapshot {
    /* 0x00 */ Ov003Unit*  unit;
    /* 0x04 */ CombatActor actor;
    /* 0x80 */ u8          reserved[0x1C];
} Ov003ActorSnapshot; // Size: 0x9C

typedef struct Ov003ActorSnapshots {
    /* 0x00 */ u16                 mainCount;
    /* 0x02 */ u16                 subCount;
    /* 0x04 */ Ov003ActorSnapshot* sub;
    /* 0x08 */ Ov003ActorSnapshot* main;
} Ov003ActorSnapshots; // Size: 0xC

typedef void (*Ov003SnapshotCallback)(s32, Ov003ActorSnapshot*, void*);

u16  func_ov003_020bce6c(CombatList* list);
u16  func_ov003_020bceac(CombatList* list, Ov003ActorSnapshot** snapshots);
void func_ov003_020bcf64(CombatList* list, s32 command);
void func_ov003_020bcf98(CombatList* list, Ov003ActorSnapshot* snapshots, u32 count);
void func_ov003_020bd020(void);
void func_ov003_020bd078(void);
void func_ov003_020bd114(void);
void func_ov003_020bd150(s32 mode, Ov003SnapshotCallback callback, void* args);

void func_ov003_020be194(Ov003CutinTask* self);
void func_ov003_020be100(Ov003CutinTask* self);
void func_ov003_020be20c(Ov003CutinTask* self);

#endif
