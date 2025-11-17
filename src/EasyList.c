#include "EasyList.h"
#include "Memory.h"

EasyListNode* EasyList_AllocateNode(EasyList* list) {
    EasyListNode* node = list->freeListHead;
    if (node != NULL) {
        list->freeListHead = node->next;
    }
    return node;
}

void EasyList_FreeNode(EasyList* list, EasyListNode* node) {
    node->next         = list->freeListHead;
    list->freeListHead = node;
}

void EasyList_Reset(EasyList* list) {
    list->headSentinel.next = NULL;
    list->headSentinel.prev = NULL;
    list->tailSentinel.next = NULL;
    list->tailSentinel.prev = NULL;
    list->freeListHead      = NULL;
    list->unk_14            = 0;

    EasyListNode* currentNode = list->nodePool;
    for (u32 i = 0; i < list->nodeCount; i++) {
        currentNode->next  = list->freeListHead;
        list->freeListHead = currentNode;
        currentNode++;
    }
}

void EasyList_Init(EasyList* list, void* pool, u32 nodeCount, EasyListCallback callback) {
    if (pool == NULL) {
        void* data = Mem_AllocBestFit(&gMainHeap, nodeCount * sizeof(EasyListNode));
        Mem_SetSequence(&gMainHeap, data, "EasyList_Init()");
        list->nodePool = data;
        list->flags &= ~0x1;
        list->flags |= 0x1;
    } else {
        list->nodePool = pool;
        list->flags &= ~0x1;
    }

    list->nodeCount = nodeCount;
    list->callback  = callback;
    EasyList_Reset(list);
}

EasyListNode* EasyList_InsertFront(EasyList* list, s32 data) {
    EasyListNode* newNode = EasyList_AllocateNode(list);
    if (newNode == NULL) {
        return NULL;
    }

    EasyListNode* nextNode  = list->headSentinel.next;
    newNode->next           = nextNode;
    newNode->prev           = NULL;
    list->headSentinel.next = newNode;
    if (nextNode == NULL) {
        nextNode = &list->tailSentinel;
    }
    nextNode->prev    = newNode;
    newNode->priority = 0;
    newNode->data     = data;
    if (list->callback != NULL) {
        list->callback(newNode, 0);
    }
    return newNode;
}

EasyListNode* EasyList_InsertBack(EasyList* list, s32 data) {
    EasyListNode* newNode = EasyList_AllocateNode(list);
    if (newNode == NULL) {
        return NULL;
    }
    EasyListNode* prevNode  = list->tailSentinel.prev;
    newNode->next           = NULL;
    newNode->prev           = prevNode;
    list->tailSentinel.prev = newNode;
    if (prevNode == NULL) {
        prevNode = &list->headSentinel;
    }
    prevNode->next    = newNode;
    newNode->priority = 0;
    newNode->data     = data;
    if (list->callback != NULL) {
        list->callback(newNode, 0);
    }
    return newNode;
}

EasyListNode* EasyList_InsertBefore(EasyList* list, EasyListNode* insertBefore, u32 data) {
    if (insertBefore == NULL) {
        return EasyList_InsertFront(list, data);
    }
    EasyListNode* newNode          = EasyList_AllocateNode(list);
    EasyListNode* insertBeforePrev = NULL;
    if (newNode == NULL) {
        return NULL;
    }
    if (insertBefore != NULL) {
        insertBeforePrev = insertBefore->prev;
    }
    newNode->prev      = insertBeforePrev;
    newNode->next      = insertBefore;
    insertBefore->prev = newNode;

    EasyListNode* prevNode = newNode->prev;
    if (prevNode == NULL) {
        list->headSentinel.next = newNode;
    } else {
        prevNode->next = newNode;
    }
    newNode->priority = 0;
    newNode->data     = data;

    if (list->callback != NULL) {
        list->callback(newNode, 0);
    }
    return newNode;
}

EasyListNode* EasyList_InsertSorted(EasyList* list, s32 priority, s32 data) {
    EasyListNode* currentNode = list->headSentinel.next;
    if (currentNode != NULL) {
    loop_2:
        if (currentNode->priority >= priority) {
            currentNode = EasyList_InsertBefore(list, currentNode, data);
        } else {
            EasyListNode* nextNode = NULL;
            if (currentNode != NULL) {
                nextNode = currentNode->next;
            }
            currentNode = nextNode;
            if (nextNode != NULL) {
                goto loop_2;
            }
        }
    }
    if (currentNode == NULL) {
        currentNode = EasyList_InsertBack(list, data);
    }
    if (currentNode != NULL) {
        currentNode->priority = priority;
    }
    return currentNode;
}

s32 EasyList_RemoveNode(EasyList* list, EasyListNode* node) {
    s32 value = 0;

    if (node != NULL) {
        if (list->callback != NULL) {
            list->callback(node, 1);
        }
        EasyListNode* nextNode = NULL;
        if (node != NULL) {
            nextNode = node->next;
        }
        EasyListNode* prevNode = NULL;
        if (node != NULL) {
            prevNode = node->prev;
        }
        value = node->data;
        if (nextNode != NULL && prevNode != NULL) {
            nextNode->prev = prevNode;
            prevNode->next = nextNode;
        } else if ((nextNode != NULL) && (prevNode == NULL)) {
            nextNode->prev          = NULL;
            list->headSentinel.next = nextNode;
        } else if ((nextNode == NULL) && (prevNode != NULL)) {
            prevNode->next          = NULL;
            list->tailSentinel.prev = prevNode;
        } else {
            list->headSentinel.next = NULL;
            list->tailSentinel.prev = NULL;
        }
        EasyList_FreeNode(list, node);
    }

    return value;
}