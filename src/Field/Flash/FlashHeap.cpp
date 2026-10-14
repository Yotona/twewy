#include "Field/FlashRegion.h"

extern "C" {
#include <nitro/mi/cpumem.h>
}
#include "Field/FlashPlayer.h"

void FlashHeap::Init(u32 size) {
    FlashHeapNode* node;
    s32            i;

    size   = (size + 3) & ~3;
    m_size = size;
    MI_CpuSet(m_nodes, 0, sizeof(m_nodes));
    node = m_nodes;
    for (i = 0; i < 64; i++) {
        node->next = node + 1;
        node       = node + 1;
    }
    m_free        = m_nodes;
    m_head        = &m_nodes[65];
    m_head->state = 2;
    node          = AllocNode();
    node->state   = 0;
    node->addr    = 0;
    node->size    = size;
    node->next    = m_head;
    m_head        = node;
}

FlashHeapNode* FlashHeap::AllocNode() {
    FlashHeapNode* node = m_free;
    m_free              = node->next;
    return node;
}

FlashHeapNode* FlashHeap::Alloc(u32 size) {
    FlashHeapNode* node = m_head;
    FlashHeapNode* next;
    u32            nodeSize;

    while (TRUE) {
        if (node->state == 0 && node->next->state == 0) {
            next = node->next;
            node->size += next->size;
            node->next = next->next;
            FreeNode(next);
            continue;
        }
        if (node->state == 0 && (nodeSize = node->size) >= size) {
            break;
        }
        node = node->next;
        if (node->state == 2) {
            return NULL;
        }
    }
    if (nodeSize == size) {
        node->state = 1;
    } else {
        node->size  = size;
        node->state = 1;
        next        = AllocNode();
        next->next  = node->next;
        node->next  = next;
        next->size  = nodeSize - size;
        next->state = 0;
        next->addr  = node->addr + node->size;
    }
    return node;
}

void FlashHeap::Free(FlashHeapNode* node) {
    node->state = 0;
}

void FlashHeap::FreeNode(FlashHeapNode* node) {
    node->next = m_free;
    m_free     = node;
}
