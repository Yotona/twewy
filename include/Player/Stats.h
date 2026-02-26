#ifndef PLAYER_STATS_H
#define PLAYER_STATS_H

#include <types.h>

typedef enum {
    STAT_ATTACK  = 0,
    STAT_DEFENSE = 1,
    STAT_HEALTH  = 2,
} StatType;

typedef enum {
    FRIEND_SHIKI  = 0,
    FRIEND_JOSHUA = 1,
    FRIEND_BEAT   = 2,
    FRIEND_NONE   = 0xFF,
} ActiveFriend;

typedef struct {
    /* 0x00 */ u16 unk_00;
    /* 0x02 */ u16 baseHealth;
    /* 0x04 */ u16 unk_04;
    /* 0x06 */ u16 level;
    /* 0x08 */ u32 expToNextLevel;
    /* 0x0C */ u32 money;
    /* 0x10 */ u16 attack;
    /* 0x12 */ u16 defense;
    /* 0x14 */ u16 dropRate;
    /* 0x16 */ u16 bravery;
    /* 0x18 */ u8  activeFriend; // Shiki, Joshua, Beat, or None
    /* 0x19 */ struct {
        u8 unk_19_0 : 2;
        u8 unk_19_2 : 2;
        u8 unk_19_4 : 2;
        u8 unk_19_6 : 1;
        u8 unk_19_7 : 1;
    };
    /* 0x1A */ struct {
        u8 unk_1A_0 : 1;
        u8 unk_1A_1 : 2;
        u8          : 5;
    };
    /* 0x1B */ u8  _pad_1B;
    /* 0x1C */ u16 equippedThreads[4]; // Item IDs of equipped threads, or 0xFFFF if empty
    /* 0x24 */ u8  unk_24[4];
    /* 0x28 */ u16 unk_28;
    /* 0x2A */ u8  unk_2A;
    /* 0x2B */ u8  unk_2B;
} PlayerStats; // Size: 0x2C

typedef struct {
    /* 0x00 */ u16 attack;
    /* 0x02 */ u16 defense;
    /* 0x04 */ u16 sync;               // Represented as tenths of a percent, (500 = 50.0% sync rate)
    /* 0x06 */ u16 bravery;
    /* 0x08 */ u16 equippedThreads[4]; // Item IDs of equipped threads, or 0xFFFF if empty
    /* 0x10 */ u8  unk_10[4];
    /* 0x14 */ u16 unk_14;
    /* 0x16 */ u8  unk_16;
    /* 0x17 */ u8  unk_17;
} FriendStats; // Size: 0x18

typedef struct {
    /* 0x0 */ u16 unk_0;
    /* 0x2 */ u16 unk_2;
    /* 0x4 */ u32 current;
    /* 0x8 */ u32 unk_8;
} Experience;

/**
 * @brief Gets the effective value of a player stat, including bonuses from equipped threads.
 *
 * @param activeFriend The currently active friend, if any.
 * @param statType The type of stat to calculate.
 * @return The effective value of the specified stat for the player.
 */
s16 Stats_GetEffectiveValue(ActiveFriend activeFriend, StatType statType);

/**
 * @brief Gets the player's effective maximum health.
 *
 * @param activeFriend The currently active friend, if any.
 * @param playerLevel The player's current set level.
 * @return The effective maximum health of the player.
 */
u32 Stats_GetMaxHealth(ActiveFriend activeFriend, u16 playerLevel);

/**
 * @brief Gets the effective value of a friend stat, including bonuses from equipped threads.
 *
 * @param activeFriend The currently active friend, if any.
 * @param statType The type of stat to calculate.
 * @return The effective value of the specified stat for the specified friend.
 */
s16 Stats_GetEffectiveFriendValue(ActiveFriend activeFriend, StatType statType);

#endif // PLAYER_STATS_H