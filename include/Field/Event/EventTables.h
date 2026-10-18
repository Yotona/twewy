#ifndef FIELD_EVENT_EVENTTABLES_H
#define FIELD_EVENT_EVENTTABLES_H

#include "Engine/File/BinMgr.h"
#include "Field/Event/EventManager.h"
#include "Field/Tasks/Fld_Human.h"

// Walking routes of an area: NPCs spawn along one of `count` segments, in
// either direction.
typedef struct {
    /* 0x0 */ FldHumanSpawnEntry* routes;
    /* 0x4 */ s16                 count;
} FldAreaRoutes; // Size: 0x8

// Two encounter id ranges; the second is used when FieldEncounter_UsesFirstRange says no.
typedef struct {
    /* 0x0 */ u16 lo1;
    /* 0x2 */ u16 hi1;
    /* 0x4 */ u16 lo2;
    /* 0x6 */ u16 hi2;
} FldEncounterRanges;

typedef s32 (*FieldEventRewardFunc)(FieldEventManager*);

extern s16                 data_ov030_020d9586[];
extern s16                 data_ov030_020d9588[];
extern s16                 data_ov030_020d958a[];
extern const BinIdentifier Iden_FldBG;
extern const BinIdentifier Iden_NpcCeparate;
extern const BinIdentifier Iden_FldScan;
extern const BinIdentifier Iden_FldTouch;
extern const BinIdentifier Iden_ShopData;
extern u16                 data_ov030_020d95c4[][2];
extern void (*data_ov030_020d95dc[])(void*);
extern const s16           FieldPresetCoords[];
extern const BinIdentifier Iden_FldEventPacks[3];
extern s32                 data_ov030_020d98ec[][2];
extern FldEncounterRanges  FieldEncounterRangesByChapterGroup[]; // by chapter group
extern s16                 data_ov030_020d9924[];
extern const BinIdentifier Iden_FldDownScreen;
extern const BinIdentifier Iden_FldUpBg;
extern const BinIdentifier Iden_FldEncount;
extern const u16           data_ov030_020d9964[];
extern s32                 data_ov030_020d9acc;
extern s32                 data_ov030_020d9af4;
extern s32                 data_ov030_020d9b1c;
extern s32                 data_ov030_020d9b44;
extern s32                 data_ov030_020d9b6c;
extern s32                 data_ov030_020d9b94;
extern s32                 data_ov030_020d9bbc;
extern s32                 data_ov030_020d9be4;
extern s32                 data_ov030_020d9c0c;
extern s16                 FieldRankLevelRanges[][2];
extern s32                 data_ov030_020d9d34;
// Per-area human spawn percentages (24 areas): [0] random spawns, [1] route spawns.
extern const s16          FieldHumanSpawnPercents[24][2];
extern FldEncounterRanges FieldEncounterRangesByRank[]; // by brand group
// Fixed NPC placements of area 5: x, y (pixels), unk_08 (-1 = by brand ranking), unk_0A.
extern s16 FieldArea5HumanPlacements[][4];
extern s32 Field_AreaTitleKinds[];
extern s32 (*const FieldChapterEventChecks[])(FieldEventManager*);
extern const FieldEventRewardFunc data_ov030_020da618[44];
extern const FieldEventRewardFunc data_ov030_020da6c8[];
extern const s32                  data_ov030_020da778[44];
extern FldEncounterRanges         FieldEncounterRangesByArea[]; // by area
extern const FldAreaRoutes        FieldAreaRoutes[24];
extern s32                        Field_AreaBgIndices[];
extern s32                        Field_BrandBgIndices[];
extern void*                      EventScripts[2207];

#endif // FIELD_EVENT_EVENTTABLES_H
