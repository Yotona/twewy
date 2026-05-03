#ifndef PLAYER_INVENTORY_H
#define PLAYER_INVENTORY_H

#include "Player/Inventory/Items.h"
#include "Player/Inventory/Pins.h"

typedef enum {
    ITEM_CATEGORY_PIN     = 0,
    ITEM_CATEGORY_THREAD  = 1,
    ITEM_CATEGORY_FOOD    = 2,
    ITEM_CATEGORY_SWAG    = 3,
    ITEM_CATEGORY_INVALID = 0xFFFF,
} ItemCategory;

/**
 * @brief Get the index of an item within its category (pin, thread, food, swag) based on its global item ID.
 *
 * @param itemID The global item ID of the item to look up.
 * @return The index of the item within its category, or 0xFFFF if the item ID is invalid.
 */
u16 Inventory_GetCategorizedIndex(u16 itemID);

/**
 * @brief Get the category of an item based on its global item ID.
 *
 * @param itemID The global item ID of the item to look up.
 * @return The category of the item or ITEM_CATEGORY_INVALID if the item ID is invalid.
 */
ItemCategory Inventory_GetCategory(u16 itemID);

/**
 * @brief Get the remaining capacity of the pin stockpile, accounting for currently equipped and stockpiled pins.
 *
 * @return The number of additional pins that can be added to the stockpile, or 0 if the stockpile is full.
 */
u16 Inventory_GetOpenPinStockpileCapacity(void);

/**
 * @brief Check if a pin can be added to the inventory.
 *
 * @param itemID The global item ID of the pin to check.
 * @param arg1 Additional argument for the check (purpose still unknown).
 * @return TRUE if the pin can be added, FALSE otherwise.
 */
BOOL Inventory_CanAddPin(u16 itemID, s32 arg1);

/**
 * @brief Add an item to the inventory if space is available.
 *
 * @param itemID The global item ID of the item to add.
 * @param arg1 Additional argument for the addition (purpose still unknown).
 * @return TRUE if the item was successfully added, FALSE otherwise.
 */
BOOL Inventory_AddItem(u16 itemID, s32 arg1);

/**
 * @brief Check if the inventory contains the required quantity of an item.
 *
 * @param itemID The global item ID of the item to check.
 * @param reqQuantity The required quantity of the item.
 * @param minPinLevel The minimum pin level required to count towards the quantity (only applicable for pins).
 * @return The quantity of the item that the player has, or 0 if they do not have enough.
 */
s32 Inventory_HasRequiredQuantity(u16 itemID, u32 reqQuantity, s32 minPinLevel);

// TODO: Below functions need named and organized into correct headers

u32 func_02023010(u16 arg0);

s32 func_02023480(u32 arg0, s32 arg1);

s32 func_0202353c(u16 arg0);

u16 func_02023588(void);

void func_02023598(s16 arg0);

void func_020235a8(void);

void func_020235cc(u32 arg0, s32 arg1);

void func_020235fc(u32 arg0, s32 arg1);

void func_02023630(s32 arg0);

s32 func_0202366c(u32 arg0, s32 arg1);

s32 func_020236a0(void);

#endif // PLAYER_INVENTORY_H