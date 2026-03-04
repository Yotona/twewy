#ifndef MEMORY_H
#define MEMORY_H

#include <types.h>

/**
 * @brief Header for a block of memory in the heap allocator.
 *
 * Contains metadata for allocation tracking and doubly-linked lists
 * for free block management and physical memory ordering.
 */
typedef struct MemBlock {
    /* 0x00 */ u32              magic;        /**< Magic number for validation (MEM_MAGIC) */
    /* 0x04 */ char*            sequence;     /**< Debug sequence string identifying the allocation */
    /* 0x08 */ u32              size;         /**< Size of the block in bytes */
    /* 0x0C */ s32              state;        /**< Allocation state: -1 = free, 0 = allocated */
    /* 0x10 */ struct MemBlock* freeListPrev; /**< Previous block in free list */
    /* 0x14 */ struct MemBlock* freeListNext; /**< Next block in free list */
    /* 0x18 */ struct MemBlock* physMemPrev;  /**< Previous block in physical memory order */
    /* 0x1C */ struct MemBlock* physMemNext;  /**< Next block in physical memory order */
} MemBlock;                                   // Size: 0x20

/**
 * @brief Represents a memory pool for heap allocation.
 *
 * Contains the base address, total size, and head sentinel block for managing allocations.
 */
typedef struct MemPool {
    /* 0x00 */ MemBlock* base; /**< Base address of the memory pool */
    /* 0x04 */ u32       size; /**< Total size of the memory pool in bytes */
    /* 0x08 */ MemBlock* head; /**< Head sentinel block for list management */
} MemPool;                     // Size: 0x0C

/** @brief Main heap pool used for general memory allocations */
extern MemPool gMainHeap;

/** @brief Debug heap pool used for debug-related memory allocations */
extern MemPool gDebugHeap;

/**
 * @brief Initializes a memory pool with the given data buffer.
 *
 * Sets up the pool's internal structures including sentinel blocks and an initial free block.
 * The size is aligned to 32-byte boundaries and must be at least 0x60 bytes.
 *
 * @param pool Pointer to the MemPool to initialize
 * @param data Pointer to the memory buffer to use for the pool
 * @param size Size of the memory buffer in bytes
 * @return TRUE if initialization succeeded, FALSE if size is too small
 */
BOOL Mem_InitializeHeap(MemPool* pool, void* data, u32 size);

/**
 * @brief Allocates memory from the tail (end) of the heap.
 *
 * Searches for a free block starting from the physical end of memory,
 * preferring allocations at the higher addresses.
 *
 * @param pool Pointer to the MemPool to allocate from
 * @param size Size of memory to allocate in bytes (will be aligned to 64 bytes)
 * @return Pointer to allocated memory, or NULL if allocation failed
 */
void* Mem_AllocHeapTail(MemPool* pool, s32 size);

/**
 * @brief Allocates memory from the head (beginning) of the heap.
 *
 * Searches for a free block starting from the physical beginning of memory,
 * preferring allocations at the lower addresses.
 *
 * @param pool Pointer to the MemPool to allocate from
 * @param size Size of memory to allocate in bytes (will be aligned to 64 bytes)
 * @return Pointer to allocated memory, or NULL if allocation failed
 */
void* Mem_AllocHeapHead(MemPool* pool, s32 size);

/**
 * @brief Allocates memory using the best-fit algorithm.
 *
 * Finds the smallest free block that can accommodate the requested size,
 * minimizing wasted space.
 *
 * @param pool Pointer to the MemPool to allocate from
 * @param size Size of memory to allocate in bytes (will be aligned to 64 bytes)
 * @return Pointer to allocated memory, or NULL if allocation failed
 */
void* Mem_AllocBestFit(MemPool* pool, s32 size);

/**
 * @brief Frees previously allocated memory.
 *
 * Marks the block as free and coalesces adjacent free blocks to reduce fragmentation.
 *
 * @param pool Pointer to the MemPool containing the allocation
 * @param data Pointer to the memory to free
 */
void Mem_Free(MemPool* pool, void* data);

/**
 * @brief Gets the size of an allocated memory block.
 *
 * @param pool Pointer to the MemPool containing the allocation
 * @param data Pointer to the allocated memory
 * @return Size of the block in bytes (excluding header), or 0 if invalid
 */
s32 Mem_GetBlockSize(MemPool* pool, void* data);

/**
 * @brief Sets a debug sequence string for an allocated memory block.
 *
 * Used for tracking allocations by associating a descriptive name.
 *
 * @param pool Pointer to the MemPool containing the allocation
 * @param data Pointer to the allocated memory
 * @param sequence Null-terminated string to set as the sequence
 */
void Mem_SetSequence(MemPool* pool, void* data, const char* sequence);

/**
 * @brief Gets the debug sequence string for an allocated memory block.
 *
 * @param pool Pointer to the MemPool containing the allocation
 * @param data Pointer to the allocated memory
 * @return Pointer to the sequence string, or NULL if invalid
 */
const char* Mem_GetSequence(MemPool* pool, void* data);

#endif // MEMORY_H