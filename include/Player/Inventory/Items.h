#ifndef INVENTORY_ITEMS_H
#define INVENTORY_ITEMS_H

#include <types.h>

/**
 * @brief Item information as stored from within an external binary data file.
 */
typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ u8   unk_02;
    /* 0x03 */ u8   unk_03;
    /* 0x04 */ u32  unk_04;
    /* 0x06 */ char unk_06[0xA - 0x08];
    /* 0x0A */ s16  defense;
    /* 0x0C */ s16  attack;
    /* 0x0E */ s16  health;
    /* 0x10 */ char unk_10[0x8];
} RawItemData; // Size: 0x18

/**
 * @brief Food information as stored from within an external binary data file.
 */
typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ char unk_02[0x2];
    /* 0x04 */ s32  unk_04;
    /* 0x08 */ char unk_08[0x14 - 0x08];
} RawFoodData; // Size: 0x14

typedef struct {
    /* 0x00 */ u16  unk_00;
    /* 0x02 */ char unk_02[0x2];
    /* 0x04 */ s32  unk_04;
} RawTreasureData; // Size: 0x8

typedef struct {
    /* 0x00 */ char unk_00[0xC];
} RawShopData; // Size: 0xC

typedef struct {
    /* 0x0 */ u16 itemID;
    /* 0x2 */ u8  flags;
    /* 0x3 */ u8  unk_3;
} InventoryItem;

// TODO: Add as enums?
// typedef enum {} ItemID;

#endif // INVENTORY_ITEMS_H