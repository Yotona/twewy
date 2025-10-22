#include "Memory.h"

MemPool gDebugHeap = {};
MemPool gMainHeap  = {};

#define MEM_MAGIC 0x51342403

static inline MemBlock* Mem_GetBlockWithHeader(void* data) {
    return (MemBlock*)((u8*)data - sizeof(MemBlock));
}

static inline void* Mem_GetBlockWithoutHeader(MemBlock* block) {
    return (void*)(block + 1);
}

static MemBlock* Mem_FindBlockByPointer(MemPool* pool, void* data) {
    MemBlock* sentinelBlock = pool->head;
    MemBlock* current       = sentinelBlock->physMemNext;

    while (current != sentinelBlock) {
        if (current == Mem_GetBlockWithHeader(data)) {
            break;
        }
        current = current->physMemNext;
    }

    if (current != sentinelBlock && current->state >= 0 && current->magic == MEM_MAGIC) {
        return current;
    }
    return NULL;
}

const char* const memLabelHead = "_Head";
const char* const memLabelList = "_List";

BOOL Mem_InitializeHeap(MemPool* pool, void* data, u32 size) {
    if (size < 0x60) {
        return FALSE;
    }
    size = size & ~0x1F;

    MemBlock* sentinelBlock    = data;
    MemBlock* initialFreeBlock = sentinelBlock + 1;

    pool->base = sentinelBlock;
    pool->size = size;
    pool->head = sentinelBlock;

    initialFreeBlock->magic        = MEM_MAGIC;
    initialFreeBlock->sequence     = memLabelHead;
    initialFreeBlock->size         = size - sizeof(MemBlock);
    initialFreeBlock->state        = -1;
    initialFreeBlock->freeListPrev = sentinelBlock;
    initialFreeBlock->freeListNext = sentinelBlock;
    initialFreeBlock->physMemPrev  = sentinelBlock;
    initialFreeBlock->physMemNext  = sentinelBlock;

    sentinelBlock->magic        = MEM_MAGIC;
    sentinelBlock->sequence     = memLabelList;
    sentinelBlock->size         = size;
    sentinelBlock->state        = 0;
    sentinelBlock->freeListPrev = initialFreeBlock;
    sentinelBlock->freeListNext = initialFreeBlock;
    sentinelBlock->physMemPrev  = initialFreeBlock;
    sentinelBlock->physMemNext  = initialFreeBlock;

    return TRUE;
}

void* Mem_AllocHeapTail(MemPool* pool, s32 size) {
    size = (size + 0x3F) & ~0x1F;

    MemBlock* allocBlock;

    MemBlock* freeBlock = pool->head->physMemNext;
    while (freeBlock != pool->head) {
        if (freeBlock->state < 0 && freeBlock->size >= size) {
            break;
        }
        freeBlock = freeBlock->physMemNext;
    }

    if (freeBlock->state >= 0) {
        return NULL;
    }

    if (freeBlock->size >= (size + 0x40)) {
        allocBlock = (MemBlock*)((u8*)freeBlock + (freeBlock->size - size));

        allocBlock->magic       = MEM_MAGIC;
        allocBlock->sequence    = NULL;
        allocBlock->size        = size;
        allocBlock->state       = 0;
        allocBlock->physMemPrev = freeBlock;
        allocBlock->physMemNext = freeBlock->physMemNext;

        allocBlock->physMemNext->physMemPrev = allocBlock;

        freeBlock->size -= size;
        freeBlock->physMemNext = allocBlock;
    } else {
        freeBlock->state                      = 0;
        freeBlock->freeListPrev->freeListNext = freeBlock->freeListNext;
        freeBlock->freeListNext->freeListPrev = freeBlock->freeListPrev;
        allocBlock                            = freeBlock;
    }
    return Mem_GetBlockWithoutHeader(allocBlock);
}

void* Mem_AllocHeapHead(MemPool* pool, s32 size) {
    size = (size + 0x3F) & ~0x1F;

    MemBlock* alloc = NULL;
    MemBlock* head  = pool->head;
    MemBlock* cur;

    for (cur = head->physMemNext; cur != head; cur = cur->physMemNext) {
        if (cur->state < 0 && cur->size >= size) {
            alloc = cur;
            break;
        }
    }

    if (alloc == NULL) {
        return NULL;
    }

    if (alloc->size >= (size + 0x40)) {
        MemBlock* new_MemBlock = (MemBlock*)(((char*)alloc) + size);

        *new_MemBlock = *alloc;

        new_MemBlock->size -= size;
        new_MemBlock->physMemPrev = alloc;

        alloc->sequence = NULL;
        alloc->size     = size;
        alloc->state    = 0;

        alloc->physMemNext->physMemPrev   = new_MemBlock;
        alloc->freeListPrev->freeListNext = new_MemBlock;
        alloc->freeListNext->freeListPrev = new_MemBlock;
        alloc->physMemNext                = new_MemBlock;
    } else {
        alloc->state                      = 0;
        alloc->freeListPrev->freeListNext = cur->freeListNext;
        alloc->freeListNext->freeListPrev = cur->freeListPrev;
    }
    return Mem_GetBlockWithoutHeader(alloc);
}

void* Mem_AllocBestFit(MemPool* pool, s32 size) {
    size = (size + 0x3F) & ~0x1F;

    MemBlock* head    = pool->head;
    MemBlock* memPrev = head->physMemPrev;

    MemBlock* alloc = NULL;

    while (memPrev != head) {
        if (memPrev->state < 0 && memPrev->size >= size) {
            if (alloc == NULL) {
                alloc = memPrev;
            }
            if (alloc->size > memPrev->size) {
                alloc = memPrev;
            }
        }
        memPrev = memPrev->physMemPrev;
    }

    if (alloc == NULL) {
        return NULL;
    }

    MemBlock* orig = alloc;
    if (alloc->size >= (size + 0x40)) {
        alloc           = (MemBlock*)((u8*)alloc + (alloc->size - size));
        alloc->magic    = MEM_MAGIC;
        alloc->sequence = NULL;
        alloc->size     = size;
        alloc->state    = 0;

        alloc->physMemPrev              = orig;
        alloc->physMemNext              = orig->physMemNext;
        alloc->physMemNext->physMemPrev = alloc;

        orig->size -= size;
        orig->physMemNext = alloc;
    } else {
        alloc->state                      = 0;
        alloc->freeListPrev->freeListNext = alloc->freeListNext;
        alloc->freeListNext->freeListPrev = alloc->freeListPrev;
    }
    return Mem_GetBlockWithoutHeader(alloc);
}

const char* memLabelFree = "_free_";

void Mem_Free(MemPool* pool, void* data) {
    MemBlock* temp_r1;
    MemBlock* temp_r1_3;
    u32       temp_r1_2;
    MemPool*  cont;

    MemBlock* var_r0 = Mem_FindBlockByPointer(pool, data);
    if (var_r0 == NULL) {
        return;
    }
    var_r0->sequence = memLabelFree;
    var_r0->state    = -1;

    MemBlock* temp_r3 = var_r0->physMemPrev;
    if (temp_r3->state < 0) {
        temp_r1              = var_r0->physMemNext;
        temp_r3->physMemNext = temp_r1;
        temp_r1->physMemPrev = temp_r3;
        temp_r1_2            = var_r0->size;
        temp_r3->size += temp_r1_2;
        temp_r3->freeListPrev->freeListNext = temp_r3->freeListNext;
        temp_r3->freeListNext->freeListPrev = temp_r3->freeListPrev;
        var_r0                              = temp_r3;
    }

    MemBlock* temp_r3_2 = var_r0->physMemNext;
    if (temp_r3_2->state < 0) {
        temp_r1_3              = temp_r3_2->physMemNext;
        var_r0->physMemNext    = temp_r1_3;
        temp_r1_3->physMemPrev = var_r0;
        var_r0->size += temp_r3_2->size;
        temp_r3_2->freeListPrev->freeListNext = temp_r3_2->freeListNext;
        temp_r3_2->freeListNext->freeListPrev = temp_r3_2->freeListPrev;
    }

    MemBlock* temp_r2                   = pool->head;
    var_r0->freeListPrev                = temp_r2;
    var_r0->freeListNext                = temp_r2->freeListNext;
    temp_r2->freeListNext->freeListPrev = var_r0;
    temp_r2->freeListNext               = var_r0;
}

s32 Mem_GetBlockSize(MemPool* pool, void* data) {
    MemBlock* block = Mem_FindBlockByPointer(pool, data);

    if (block == NULL) {
        return 0;
    }
    return block->size - sizeof(MemBlock);
}

void Mem_SetSequence(MemPool* pool, void* data, const char* sequence) {
    MemBlock* block = Mem_FindBlockByPointer(pool, data);

    if (block != NULL) {
        block->sequence = sequence;
    }
}

const char* Mem_GetSequence(MemPool* pool, void* data) {
    MemBlock* block = Mem_FindBlockByPointer(pool, data);

    char* sequence = NULL;
    if (block != NULL) {
        sequence = block->sequence;
    }
    return sequence;
}
