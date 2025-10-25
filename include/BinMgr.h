#ifndef BIN_MGR_H
#define BIN_MGR_H

#include <NitroSDK/fs/file.h>
#include <types.h>

typedef struct BinIdentifier {
    /* 0x00 */ u32   id;
    /* 0x04 */ char* path;
} BinIdentifier;

typedef struct Bin {
    /* 0x00000 */ struct Bin* next;          // Next node in linked list
    /* 0x00004 */ u16         unk_04;        // Unknown field
    /* 0x00006 */ u16         unk_06;        // Unknown field
    /* 0x00008 */ u16         flags;         // Status flags: bit 0=allocated, bit 1=secondary data, bit 2=compressed
    /* 0x0000A */ u16         refCount;      // Reference counter for memory management
    /* 0x0000C */ u32         id;            // Unique resource identifier
    /* 0x00010 */ u32         size;          // Size of resource data in bytes
    /* 0x00014 */ void*       data;          // Pointer to primary resource data
    /* 0x00018 */ void*       secondaryData; // Pointer to secondary/auxiliary data
    /* 0x0001C */ u32         unk_1C;        // Unknown field
} Bin;

typedef struct BinMgr {
    /* 0x00000 */ Bin   rootBin;      // Root bin node (head of allocation list)
    /* 0x00020 */ Bin*  freeList;     // Head of free/available bins list
    /* 0x00024 */ char  unk_24[0x1C]; // Unknown data
    /* 0x00040 */ Bin*  listTail;     // Tail pointer for efficient list appending
    /* 0x00044 */ s32   nodeCount;    // Total number of nodes in the pool
    /* 0x00048 */ void* nodePool;     // Pool of pre-allocated memory for nodes
} BinMgr;

/**
 * Allocates a node from the free list
 * @param freeListHead Pointer to the head of the free list
 * @return Pointer to the allocated node, or NULL if no nodes are available
 */
Bin* BinMgr_AllocNode(Bin* freeListHead);

/**
 * Appends a node to the end of the BinMgr's active list
 * @param binMgr Pointer to the BinMgr structure
 * @param node Pointer to the node to append to the tail
 */
void BinMgr_AppendNode(BinMgr* binMgr, Bin* node);

/**
 * Adds a node to the head of the BinMgr's free list
 * @param binMgr Pointer to the BinMgr structure
 * @param bin Pointer to the node to add to the free list
 */
void BinMgr_AddToFreeList(BinMgr* binMgr, Bin* bin);

/**
 * Removes a specific node from the BinMgr's free list
 * @param binMgr Pointer to the BinMgr structure
 * @param bin Pointer to the node to remove from the free list
 */
Bin* BinMgr_RemoveFromFreeList(BinMgr* binMgr, Bin* bin);

/**
 * Initializes the Binary Manager system with a node pool
 * @param binMgr Pointer to the BinMgr structure to initialize
 * @param nodeCount Number of nodes to pre-allocate in the pool
 * @return Pointer to the previous BinMgr instance
 */
BinMgr* BinMgr_Init(BinMgr* binMgr, u32 nodeCount);

/**
 * Loads raw/uncompressed data from a resource without creating a bin
 * @param targetBuffer Target buffer (can be NULL for auto-allocation)
 * @param resIden Resource file identifier (can be NULL for auto-lookup)
 * @param binIden Resource details
 * @param offset Byte offset to start reading from within the resource
 * @param outSize Pointer to size variable (input: max size, output: actual size read)
 * @return Pointer to loaded raw data
 */
void* BinMgr_LoadRawData(Bin* targetBuffer, FS_FileIdentifier* resIden, BinIdentifier* binIden, int offset, u32* outSize);

/**
 * Loads and decompresses data from a resource without creating a bin
 * @param targetBuffer Target buffer (can be NULL for auto-allocation)
 * @param resIden Resource file identifier (can be NULL for auto-lookup)
 * @param resourceId Resource details
 * @param offset Byte offset to start reading from within the resource
 * @param outSize Pointer to size variable (input: max size, output: decompressed size)
 * @return Pointer to decompressed data
 */
Bin* BinMgr_LoadCompressed(Bin* targetBuffer, FS_FileIdentifier* resIden, int resourceId, int offset, u32* outSize);

/**
 * Loads uncompressed/raw data into a new bin without decompression
 * @param resIden Resource file identifier (can be NULL for auto-lookup)
 * @param resourceId Resource details
 * @return Pointer to the loaded bin containing raw data
 */
Bin* BinMgr_LoadUncompressed(FS_FileIdentifier* resIden, u32 resourceId);

/**
 * Loads a compressed resource into a new bin with automatic decompression
 * @param resourceDesc Pointer to the resource descriptor (can be NULL for auto-lookup)
 * @param resourceId Identifier of the resource to load
 * @return Pointer to the loaded and decompressed resource bin
 */
Bin* BinMgr_LoadResource(FS_FileIdentifier* resIden, u32 resourceId);

/**
 * Creates a bin wrapper around existing external data (does not copy data)
 * @param id Unique identifier for the bin
 * @param data Pointer to the existing data (not owned by BinMgr)
 * @param size Size of the data in bytes
 * @return Pointer to the created bin wrapper
 */
Bin* BinMgr_CreateFromData(u32 id, void* data, u32 size);

/**
 * Releases a bin by decrementing its reference count and freeing if unused
 * @param bin Pointer to the bin to release
 * @return TRUE if bin was freed and moved to available list, FALSE if still referenced
 */
BOOL BinMgr_ReleaseBin(Bin* bin);

/**
 * Searches for a bin by its unique ID in the free/active bins list
 * @param id The resource ID to search for
 * @return Pointer to the bin if found, NULL otherwise
 */
Bin* BinMgr_FindById(s32 id);

#endif // BIN_MGR_H