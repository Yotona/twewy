#ifndef INTERFACE_MENU_RESULT_H
#define INTERFACE_MENU_RESULT_H

#include "Engine/EasyTask.h"
#include "Engine/File/BinMgr.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/BgResMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "SpriteMgr.h"
#include "common_data.h"
#include <nitro/rtc.h>

typedef struct {
    /* 0x00 */ u16  pinID;
    /* 0x02 */ u8   brand;
    /* 0x03 */ u8   unk_03;
    /* 0x04 */ u32  pp[3];
    /* 0x10 */ u32  startPP[3];
    /* 0x1C */ u32  levelPP;
    /* 0x20 */ u32  totalPP;
    /* 0x24 */ u32  nextLevelPP;
    /* 0x28 */ u32  ppToNextLevel; // Snapshot taken when the slot is loaded; caps this session's gain
    /* 0x2C */ u8   level;
    /* 0x2D */ u8   maxLevel;
    /* 0x2E */ u8   slot;
    /* 0x2F */ u8   unk_2F;
    /* 0x30 */ u8   unk_30;
    /* 0x31 */ u8   unk_31;
    /* 0x32 */ u8   ppCurve;
    /* 0x33 */ u8   evolveChoice; // Index into evolvePinID, or 2 if the badge does not evolve
    /* 0x34 */ u8   evolveLevel[2];
    /* 0x36 */ u8   evolveCondition[2];
    /* 0x38 */ u16  evolvePinID[2];
    /* 0x3C */ u16  flags; // Bit 0: maxed out and waiting to evolve
    /* 0x3E */ char unk_3E[0x40 - 0x3E];
} ResultBadge;             // Size: 0x40

typedef struct {
    /* 0x0 */ u16 itemId;
    /* 0x2 */ u8  unk_2;
    /* 0x3 */ u8  brand;
} ResultThread; // Size: 0x4

// Same shape as DepartResources (see Depart.h).
typedef struct {
    /* 0x00 */ Data*            data;
    /* 0x04 */ BgResource*      screenResource;
    /* 0x08 */ BgResource*      charResource;
    /* 0x0C */ PaletteResource* paletteResource;
    /* 0x10 */ u16*             charData;
    /* 0x14 */ u16*             screenMap;
    /* 0x18 */ u8*              paletteData;
} ResultBgResource; // Size: 0x1C

typedef struct {
    /* 0x000 */ ResultBadge  badges[6];
    /* 0x180 */ ResultThread threads[2][4];
    /* 0x1A0 */ u16          dropPinIDs[32];
    /* 0x1E0 */ u16          dropIsMastered[32];
    /* 0x220 */ u16          dropQuantities[32];
    /* 0x260 */ u16          specialBonuses[16];
    /* 0x280 */ u16          foodItems[2];
    /* 0x284 */ u16          foodFinished[2];
    /* 0x288 */ u16          foodStat[2];
    /* 0x28C */ u16          foodStatBonus[2];
    /* 0x290 */ char         unk_290[0x294 - 0x290];
    /* 0x294 */ u16          dropCount;
    /* 0x296 */ u16          foodFinishedCount;
    /* 0x298 */ u16          specialBonusCount;
    /* 0x29A */ u8           badgeCount;
    /* 0x29B */ char         unk_29B[0x29C - 0x29B];
    // Handshake between the stages and the belt/badge tasks. 0x01: a belt was just added,
    // 0x02: ready for the next belt, 0x04: shift the dropped-pin icons, 0x08: PP count finished
    // (show the rank), 0x10: fast-forward held, 0x20: food belt (advances two rows).
    /* 0x29C */ u16              stageFlags;
    /* 0x29E */ u8               unk_29E;
    /* 0x29F */ u8               exitTarget; // Where Result_Update goes once the stages finish
    /* 0x2A0 */ u32              basePP;
    /* 0x2A4 */ u16              battleCount;
    /* 0x2A6 */ u16              timeRank;
    /* 0x2A8 */ u16              rateBRank;
    /* 0x2AA */ u16              specialBonusLevel;
    /* 0x2AC */ u16              battleCountMultiplier;
    /* 0x2AE */ u16              timeMultiplier;
    /* 0x2B0 */ u16              rateBMultiplier;
    /* 0x2B2 */ u16              specialBonusMultiplier;
    /* 0x2B4 */ u32              displayedPP;
    /* 0x2B8 */ u32              earnedPP;
    /* 0x2BC */ u16              overallRank;
    /* 0x2BE */ char             unk_2BE[0x2C0 - 0x2BE];
    /* 0x2C0 */ u32              clearTime;
    /* 0x2C4 */ s32              maxHits;
    /* 0x2C8 */ u32              trendScores[22][13];
    /* 0x740 */ s32              trendGains[22][13];
    /* 0xBB8 */ u8               trendRanking[22][13];
    /* 0xCD6 */ char             unk_CD6[0xCD8 - 0xCD6];
    /* 0xCD8 */ RTCDate          date;
    /* 0xCE8 */ RTCTime          time;
    /* 0xCF4 */ u8               trendRotation;
    /* 0xCF5 */ u8               currentArea;
    /* 0xCF6 */ u8               battlePartner;
    /* 0xCF7 */ char             unk_CF7[0xCF8 - 0xCF7];
    /* 0xCF8 */ s16              flashStep;
    /* 0xCFA */ char             unk_CFA[0xCFC - 0xCFA];
    /* 0xCFC */ s16              flashTimer;
    /* 0xCFE */ s16              unk_CFE;
    /* 0xD00 */ s16              flashLevel;
    /* 0xD02 */ u8               espersMet;
    /* 0xD03 */ u8               civviesMet;
    /* 0xD04 */ u8               aliensMet;
    /* 0xD05 */ char             unk_D05[0xD06 - 0xD05];
    /* 0xD06 */ u16              esperPP;
    /* 0xD08 */ u16              civvyPP;
    /* 0xD0A */ u16              alienPP;
    /* 0xD0C */ u16              elapsedDays;
    /* 0xD0E */ u8               elapsedHours;
    /* 0xD0F */ u8               elapsedMinutes;
    /* 0xD10 */ u8               elapsedSeconds;
    /* 0xD11 */ char             unk_D11[0xD12 - 0xD11];
    /* 0xD12 */ u16              dailyPP[7];
    /* 0xD20 */ u16              mabsPinCountRank;
    /* 0xD22 */ u16              mabsRateBRank;
    /* 0xD24 */ u16              mabsPinCountMultiplier;
    /* 0xD26 */ u16              mabsRateBMultiplier;
    /* 0xD28 */ ResultBgResource bgResources[8]; // [5]-[7]: main BG1-BG3, [3]: sub BG3
} ResultObject;                                  // Size: 0xE08

/**
 * @brief Archive entries for every file the result screen loads: the BG/OBJ graphics for each mode,
 *        the badge graphics, and the pin/item/food/treasure data tables.
 */
extern const BinIdentifier Result_BinIdentifiers[35];

/**
 * @brief Template copied into a badge slot that has no pin equipped.
 */
extern ResultBadge Result_EmptyBadge;

// ResultData.c

/**
 * @brief Switch a sprite to another animation frame from its own resource pack.
 *
 * @param sprite Sprite to update.
 * @param frame  Animation frame to show.
 */
void Result_SetSpriteFrame(Sprite* sprite, s16 frame);

/**
 * @brief Get a badge's total PP (battle + mingle + shutdown).
 *
 * @param result Result screen state.
 * @param index  Badge slot (0-5).
 * @return       Sum of the slot's three PP counters.
 */
u32 Result_GetBadgeTotalPP(ResultObject* result, u16 index);

/**
 * @brief Get the total PP at which a badge reached its current level.
 *
 * @param result Result screen state.
 * @param index  Badge slot (0-5).
 * @param arg2   The badge's PP curve id (ResultBadge.ppCurve, taken from the pin data).
 * @return       PP threshold of the current level.
 */
u32 Result_GetBadgeLevelPP(ResultObject* result, u16 index, s32 arg2);

/**
 * @brief Get the total PP a badge needs to reach its next level.
 *
 * @param result Result screen state.
 * @param index  Badge slot (0-5).
 * @param arg2   The badge's PP curve id (ResultBadge.ppCurve, taken from the pin data).
 * @return       PP threshold of the next level, or 0xFFFF if the badge is already at its maximum level.
 */
u32 Result_GetBadgeNextLevelPP(ResultObject* result, u16 index, s32 arg2);

/**
 * @brief Decide whether a badge that just maxed out evolves, and into which of its two evolutions.
 *
 * Projects the PP gained this session onto the counter named by @p kind, works out which PP type
 * dominates, and checks that against the badge's evolution conditions.
 *
 * @param result Result screen state.
 * @param index  Badge slot (0-5).
 * @param kind   PP counter being awarded: 0 battle, 1 mingle, 2 shutdown.
 * @return       Evolution index (0 or 1) to pass through ResultBadge.evolvePinID, or 2 if the badge does not evolve.
 */
u8 Result_CheckBadgeEvolution(ResultObject* result, u16 index, s32 kind);

/**
 * @brief Replace a badge slot with its evolved pin, starting over at level 1 with no PP.
 *
 * @param result Result screen state.
 * @param index  Badge slot (0-5).
 * @param pinID  Pin the badge evolves into.
 */
void Result_EvolveBadge(ResultObject* result, u16 index, u16 pinID);

/**
 * @brief Fill the badge slots and both characters' threads from the save's equipped items, and
 *        work out how many badge slots are unlocked.
 *
 * @param result Result screen state.
 */
void Result_LoadEquipment(ResultObject* result);

/**
 * @brief Set up the regular post-battle result: base PP and its multipliers, the overall rank,
 *        dropped pins, special bonuses and food digestion.
 *
 * @param result Result screen state.
 */
void Result_InitBattleMode(ResultObject* result);

/**
 * @brief Copy the pins used in mingle mode back into the save's equipped pins and free the
 *        buffers mingle mode allocated for them. USA only.
 *
 * @param result Result screen state (unused).
 */
void Result_ImportSurePins(ResultObject* result);

/**
 * @brief Set up the mingle ("sure") result: mingle PP from the espers, civvies and aliens met.
 *
 * @param result Result screen state.
 */
void Result_InitSureMode(ResultObject* result);

/**
 * @brief Set up the shutdown ("sleep") result: shutdown PP from the time elapsed since the game
 *        was last saved. The rate halves each day for the first four days, then stays flat until
 *        the one-week cap.
 *
 * @param result Result screen state.
 */
void Result_InitSleepMode(ResultObject* result);

/**
 * @brief Set up the Tin Pin Slammer ("mabs") result: PP from the stored match value
 *        (gSaveData.mabsBasePP), scaled by a multiplier that grows as fewer pins are equipped and by
 *        a second one rated from gSaveData.unk_1D84.
 *
 * @param result Result screen state.
 */
void Result_InitMabsMode(ResultObject* result);

/**
 * @brief Write the badge slots back to the save's equipped pins. USA also copies them into the
 *        active pin layout.
 *
 * @param result Result screen state.
 */
void Result_SaveEquippedPins(ResultObject* result);

/**
 * @brief Apply a battle result to the save when the screen closes: experience, brand trends,
 *        dropped pins, finished-food stat bonuses and the partner's sync loss.
 *
 * @param result Result screen state.
 */
void Result_CommitBattle(ResultObject* result);

/**
 * @brief Apply a mingle result when the screen closes. USA does nothing; JP awards experience
 *        equal to the number of people met.
 *
 * @param result Result screen state.
 */
void Result_CommitSure(ResultObject* result);

/**
 * @brief Apply a shutdown result when the screen closes. Does nothing.
 *
 * @param result Result screen state.
 */
void Result_CommitSleep(ResultObject* result);

/**
 * @brief Apply a Tin Pin Slammer result when the screen closes: clear the match counters and
 *        award 3 experience.
 *
 * @param result Result screen state.
 */
void Result_CommitMabs(ResultObject* result);

/**
 * @brief Load the background layers for the current mode (main BG1-BG3 and sub BG3).
 *
 * @param result Result screen state.
 */
void Result_LoadBackgrounds(ResultObject* result);

/**
 * @brief Per-frame background update. Does nothing.
 *
 * @param result Result screen state.
 */
void Result_UpdateBackgrounds(ResultObject* result);

/**
 * @brief Release the background layers loaded by Result_LoadBackgrounds().
 *
 * @param result Result screen state.
 */
void Result_ReleaseBackgrounds(ResultObject* result);

// Task creators. Each one spawns its task in @p pool and returns the new task's id; @p dataType
// is the DatMgr slot the task loads its graphics into, and @p owner is the result screen state.

/** @brief Badge icon for badge slot @p index. */
s32 Result_bdg_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief Numbered marker beside badge slot @p index (its frame follows the slot number); hidden while the slot is locked. */
s32 Result_bdgPRI_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief Status marker beside badge slot @p index, picked from the equipped pin's status byte (ResultBadge.unk_2F). */
s32 Result_bdgSC_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief Flash on badge slot @p index while it waits to evolve; drives the screen brightness pulse. */
s32 Result_flash_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief Cover over badge slot @p index when that slot is still locked. */
s32 Result_slotCover_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief Level-up graphic that rises into place over badge slot @p index. */
s32 Result_lvup_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/**
 * @brief One sparkle over badge slot @p index; the stages spawn 15 at a time.
 *
 * @param kind 0 for a level-up, 1 for an evolution.
 */
s32 Result_star_CreateTask(TaskPool* pool, s32 dataType, u16 index, u16 kind, ResultObject* owner);

/** @brief Evolution graphic that rises into place over badge slot @p index. */
s32 Result_evo_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief PP gauge under badge slot @p index, showing progress toward the next level. */
s32 Result_bdgBP_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/** @brief Level number for badge slot @p index; updates as the badge levels up. */
s32 Result_bdgLV_CreateTask(TaskPool* pool, s32 dataType, u16 index, ResultObject* owner);

/**
 * @brief Overall battle rank that slides in once the PP count finishes.
 *
 * @param rank Rank to show, 0 (best) to 5.
 */
s32 Result_rankU_CreateTask(TaskPool* pool, s32 dataType, u16 rank, ResultObject* owner);

/** @brief Shutdown-mode text: the explanation and the time elapsed since the last save. */
s32 Result_textScr_sleep_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief Mingle-mode text: how many people were met in total, and the esper/civvy/alien labels and counts. */
s32 Result_textScr_sure_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief The battle's max hit count. */
s32 Result_numMaxHit_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief The battle's clear time. */
s32 Result_numTime_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief Mingle-mode breakdown (espers, civvies and aliens met, with their PP) and the PP counter. */
s32 Result_num_Param_sure_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief Shutdown-mode breakdown (PP earned per elapsed day) and the PP counter. */
s32 Result_num_Param_sleep_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief Tin Pin Slammer breakdown (base value and its two rated multipliers) and the PP counter. */
s32 Result_num_Param_mabs_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/** @brief Battle breakdown (base PP and each multiplier with its rank) and the PP counter. */
s32 Result_num_Param_CreateTask(TaskPool* pool, s32 dataType, ResultObject* owner);

/**
 * @brief Icon and count for dropped pin @p index, which slides in on the top screen.
 *
 * @param mode Unused; the stages always pass 0.
 */
s32 Result_bdgU_CreateTask(TaskPool* pool, s32 dataType, s32 mode, u16 index, ResultObject* owner);

/**
 * @brief Message belt on the top screen.
 *
 * @param mode  0: pin obtained (@p index is a dropped-pin entry), 1: food finished (@p index is 0 for
 *              the player, 1 for the partner), 2: special bonus (@p index is a bonus entry).
 * @param index Entry to describe; its meaning depends on @p mode.
 */
s32 Result_beltU_CreateTask(TaskPool* pool, s32 dataType, u16 mode, u16 index, ResultObject* owner);

#endif // INTERFACE_MENU_RESULT_H
