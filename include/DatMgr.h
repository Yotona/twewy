#ifndef DATMGR_H
#define DATMGR_H

#include "BinMgr.h"
#include "Memory.h"
#include "PacMgr.h"
#include <types.h>

enum {
    DAT_TYPE_RAW               = 0,
    DAT_TYPE_COMPRESSED        = 1,
    DAT_TYPE_PACK_ENTRY        = 2,
    DAT_TYPE_UNCOMPRESSED_BIN  = 3,
    DAT_TYPE_RESOURCE          = 4,
    DAT_TYPE_PACK_ENTRY_DIRECT = 5,
    DAT_TYPE_GENERATED         = 12
};

typedef struct Data {
    /* 0x00 */ struct Data* next;
    /* 0x04 */ struct {
        u32 slotIndex    : 4;
        u32 dataType     : 4;
        u32 ownsData     : 1;
        u32 isCompressed : 1;
        u32 isLoaded     : 1;
        u32 _pad         : 5;
        u32 refCount     : 16;
    };
    /* 0x08 */ void*          buffer; // Pointer to loaded data
    /* 0x0C */ s32            dataSize;
    /* 0x10 */ s32            unk_10;
    /* 0x14 */ BinIdentifier* binIden;
    /* 0x18 */ s32            offset;
    /* 0x1C */ Bin*           bin;
    /* 0x20 */ Pack*          pack;
    /* 0x24 */ s32            packIndex; // Index within a pack
    /* 0x28 */ s32            unk_28;
    /* 0x2C */ s32            unk_2C;
} Data;

typedef struct DatMgr {
    /* 0x000 */ BOOL  slotActive[10]; // Indicates if a dataLists slot is in use
    /* 0x028 */ Data  dataLists[10];  // Heads of linked lists bucketed by type (slotIndex)
    /* 0x208 */ Data  freeList;       // Head of free Data entry list
    /* 0x238 */ Data* freeListTail;   // Tail pointer for free Data entries
    /* 0x23C */ s32   dataCount;      // Total number of Data entries in the pool
    /* 0x240 */ Data* dataPool;       // Pointer to the pool of Data entries
} DatMgr;

/**
 * @brief Initializes the Data Manager.
 *
 * Sets up the data pool and free list. Slots 0 and 1 are activated by default.
 *
 * @param mgr Pointer to the DatMgr structure to initialize.
 * @param count Number of Data entries to allocate for the pool.
 * @return The previously active DatMgr pointer.
 */
DatMgr* DatMgr_Init(DatMgr* mgr, s32 count);

/**
 * @brief Loads raw data from a binary file.
 *
 * Checks if the data is already loaded in the specified slot. If not, loads it using BinMgr.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param buffer Optional buffer to load data into. If NULL, memory is allocated.
 * @param dataSize Size of the data to load.
 * @param iden Identifier for the binary file.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadRawData(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden);

/**
 * @brief Loads raw data from a binary file with an offset.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param buffer Optional buffer to load data into. If NULL, memory is allocated.
 * @param dataSize Size of the data to load.
 * @param iden Identifier for the binary file.
 * @param offset Offset into the file to start loading from.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadRawDataWithOffset(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden, s32 offset);

/**
 * @brief Loads a compressed binary file.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param buffer Optional buffer to load data into. If NULL, memory is allocated.
 * @param dataSize Size of the data to load.
 * @param iden Identifier for the binary file.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadCompressedBin(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden);

/**
 * @brief Loads an entry from a pack file.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param buffer Optional buffer to load data into. If NULL, memory is allocated.
 * @param dataSize Size of the data to load.
 * @param iden Identifier for the pack file.
 * @param packIndex Index of the entry within the pack.
 * @param isCompressed Whether the entry is compressed.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadPackEntry(s32 dataType, void* buffer, s32 dataSize, BinIdentifier* iden, s32 packIndex, BOOL isCompressed);

/**
 * @brief Loads an uncompressed binary file.
 *
 * Uses BinMgr_LoadUncompressed which returns a Bin structure.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param iden Identifier for the binary file.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadUncompressedBin(s32 dataType, BinIdentifier* iden);

/**
 * @brief Loads a resource file.
 *
 * Uses BinMgr_LoadResource.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param iden Identifier for the resource file.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadResource(s32 dataType, BinIdentifier* iden);

/**
 * @brief Loads a pack entry directly, potentially pointing to mapped data.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param iden Identifier for the pack file.
 * @param packIndex Index of the entry within the pack.
 * @param arg3 Mode: 0 for direct pointer, 1 for processing/copying.
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_LoadPackEntryDirect(s32 dataType, BinIdentifier* iden, s32 packIndex, s32 arg3);

/**
 * @brief Generates packed data.
 *
 * @param dataType The slot index (bucket) to store/search for the data.
 * @param buffer Optional buffer.
 * @param arg2 Argument passed to PacMgr_GenPack.
 * @param iden Identifier for the pack file.
 * @param arg4 Argument passed to PacMgr_GenPack (cast to s32*).
 * @return Pointer to the loaded Data entry.
 */
Data* DatMgr_GeneratePackedData(s32 dataType, void* buffer, void* arg2, BinIdentifier* iden, s32 arg4);

/**
 * @brief Releases a Data entry.
 *
 * Decrements the reference count. If it reaches 0, frees associated resources (buffer, Bin, Pack) and returns the entry to the
 * free list.
 *
 * @param data Pointer to the Data entry to release.
 * @return TRUE if the data was fully released (refCount reached 0), FALSE otherwise.
 */
BOOL DatMgr_ReleaseData(Data* data);

/**
 * @brief Activates a new slot in the Data Manager.
 *
 * Searches for the first inactive slot starting from index 2 and marks it as active.
 */
void DatMgr_AllocateSlot(void);

/**
 * @brief Clears all data in a specific slot and deactivates it.
 *
 * Releases all Data entries associated with the given slot index.
 *
 * @param dataType The slot index to clear.
 */
void DatMgr_ClearSlot(s32 dataType);

#endif // DATMGR_H