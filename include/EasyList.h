#ifndef EASYLIST_H
#define EASYLIST_H

#include <types.h>

typedef struct EasyListNode {
    /* 0x00 */ struct EasyListNode* next;
    /* 0x04 */ struct EasyListNode* prev;
    /* 0x08 */ s32                  priority;
    /* 0x0C */ u32                  data;
} EasyListNode; // size: 0x10

typedef void (*EasyListCallback)(EasyListNode*, s32);

typedef struct {
    /* 0x00 */ u32              flags;
    /* 0x04 */ u32              nodeCount;
    /* 0x08 */ EasyListNode*    nodePool;
    /* 0x0C */ EasyListCallback callback;
    /* 0x10 */ EasyListNode*    freeListHead;
    /* 0x14 */ u32              unk_14;
    /* 0x18 */ u32              unk_18;
    /* 0x1C */ u32              unk_1C;
    /* 0x20 */ EasyListNode     headSentinel;
    /* 0x30 */ EasyListNode     tailSentinel;
} EasyList; // size: 0x40

/**
 * @brief Pop a node from the free list
 * @param list List to allocate from
 *
 * @return Pointer to allocated node, or NULL if none are available
 */
EasyListNode* EasyList_AllocateNode(EasyList* list);

/**
 * @brief Return a node to the free list
 * @param list List to free to
 * @param node Node to free
 */
void EasyList_FreeNode(EasyList* list, EasyListNode* node);

/**
 * @brief Reset the list to an empty state, rebuilding the free list
 * @param list List to reset
 */
void EasyList_Reset(EasyList* list);

/**
 * @brief Initialize an EasyList
 * @param list List to initialize
 * @param pool Optional preallocated pool of nodes; if NULL, a pool will be allocated
 * @param nodeCount Number of nodes in the pool
 * @param callback Optional callback to invoke on node insertions
 */
void EasyList_Init(EasyList* list, void* pool, u32 nodeCount, EasyListCallback callback);

/**
 * @brief Insert a new node at the front of the list
 * @param list List to insert into
 * @param data Data to store in the new node
 * @return Pointer to the new node, or NULL if allocation failed
 */
EasyListNode* EasyList_InsertFront(EasyList* list, s32 data);

/**
 * @brief Insert a new node at the back of the list
 * @param list List to insert into
 * @param data Data to store in the new node
 * @return Pointer to the new node, or NULL if allocation failed
 */
EasyListNode* EasyList_InsertBack(EasyList* list, s32 data);

/**
 * @brief Insert a new node before the specified node
 * @param list List to insert into
 * @param insertBefore Node to insert before; if NULL, inserts at front
 * @param data Data to store in the new node
 * @return Pointer to the new node, or NULL if allocation failed
 */
EasyListNode* EasyList_InsertBefore(EasyList* list, EasyListNode* insertBefore, u32 data);

/**
 * @brief Insert a new node in sorted order based on priority
 * @param list List to insert into
 * @param priority Priority value for sorting
 * @param data Data to store in the new node
 * @return Pointer to the new node, or NULL if allocation failed
 */
EasyListNode* EasyList_InsertSorted(EasyList* list, s32 priority, s32 data);

/**
 * @brief Remove a node from the list
 * @param list List to remove from
 * @param node Node to remove
 * @return The data value of the removed node
 */
s32 EasyList_RemoveNode(EasyList* list, EasyListNode* node);

#endif // EASYLIST_H