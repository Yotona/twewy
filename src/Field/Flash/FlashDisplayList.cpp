#include "Field/FieldRegion.h"

extern "C" {
#include <nitro/mi/cpumem.h>
}
#include "Field/Flash/FlashPlayer.h"

BOOL data_ov030_0210a218[2];

FlashDisplayList::~FlashDisplayList() {
    FlashDisplayNode* node = m_head;
    while (node->next != NULL) {
        node->obj->m_useCount--;
        node = node->next;
    }
}

void FlashDisplayList::Clear() {
    Init();
}

void FlashDisplayList::Init() {
    FlashDisplayNode* node = m_head;
    s32               i;

    if (node != NULL) {
        while (TRUE) {
            if (node->next == NULL) {
                break;
            }
            if (node->obj != NULL) {
                node->obj->m_useCount = 0;
            }
            node = node->next;
        }
    }
    MI_CpuSet(this, 0, sizeof(m_nodes));
    node = m_nodes;
    for (i = 0; i < 256; i++) {
        node->next = node + 1;
        node       = node + 1;
    }
    m_free = m_nodes;
    m_head = &m_nodes[257];
}

FlashDisplayNode* FlashDisplayList::AllocNode() {
    FlashDisplayNode* node = m_free;
    m_free                 = node->next;
    return node;
}

void FlashDisplayList::Insert(FlashObject* obj, u16 depth, const FlashMatrix* mtx) {
    FlashDisplayNode* node = m_head;
    FlashDisplayNode* newNode;

    while (TRUE) {
        if (node->next == NULL || node->depth > depth) {
            break;
        }
        node = node->next;
    }
    newNode     = AllocNode();
    *newNode    = *node;
    node->obj   = obj;
    node->depth = depth;
    MI_CpuCopyU8(mtx, &node->mtx, sizeof(FlashMatrix));
    node->next = newNode;
}

void FlashDisplayList::Modify(u16 depth, u8 alpha, const FlashColorTransform* cxform, const FlashMatrix* mtx) {
    FlashDisplayNode* node;

    for (node = m_head;; node = node->next) {
        if (node->next == NULL) {
            break;
        }
        if (node->depth == depth) {
            if (mtx != NULL) {
                MI_CpuCopyU8(mtx, &node->mtx, sizeof(FlashMatrix));
            }
            node->obj->SetAlpha(alpha);
            node->obj->SetColorTransform(cxform);
        }
    }
}

void FlashDisplayList::Remove(u16 id, u16 depth) {
    FlashDisplayNode* node = m_head;
    FlashDisplayNode* next;

    for (;;) {
        next = node->next;
        if (next == NULL) {
            break;
        }
        if (id == node->obj->m_id && node->depth == depth) {
            node->obj->m_useCount--;
            next       = node->next;
            *node      = *next;
            next->next = m_free;
            m_free     = next;
            return;
        }
        node = next;
    }
}

void FlashDisplayList::RemoveDepth(u16 depth) {
    FlashDisplayNode* node = m_head;
    FlashDisplayNode* next;

    for (;;) {
        next = node->next;
        if (next == NULL) {
            break;
        }
        if (node->depth != depth) {
            node = next;
            continue;
        }
        node->obj->m_useCount--;
        next       = node->next;
        *node      = *next;
        next->next = m_free;
        m_free     = next;
    }
}

FlashMatrix* FlashDisplayList::FindMatrix(u16 depth) {
    FlashDisplayNode* node;

    for (node = m_head;; node = node->next) {
        if (node->next == NULL) {
            break;
        }
        if (node->depth == depth) {
            return &node->mtx;
        }
    }
}

FlashObject::FlashObject() {
    m_palette = NULL;
    Reset();
}

FlashObject::~FlashObject() {
    if (m_palette != NULL) {
        FlashFreeArray(m_palette);
        m_palette = NULL;
    }
}

void FlashObject::SetBounds(s16 x, s16 y, s16 w, s16 h) {
    m_x = x << 12;
    m_y = y << 12;
    m_w = w << 12;
    m_h = h << 12;
}

void FlashObject::Reset() {
    m_useCount = 0;
    m_alpha    = 31;
}

FlashFileStream::FlashFileStream() {
    data_ov030_0210a218[0] = TRUE;
    FS_FileInit(&m_file);
}

BOOL FlashFileStream::Open(const char* path) {
    m_isOpen = FALSE;
    if (path == NULL) {
        return FALSE;
    }
    if (!FS_FileOpen(&m_file, path)) {
        return FALSE;
    }
    m_isOpen = TRUE;
    return TRUE;
}

BOOL FlashFileStream::Read(void* buf, u32 size) {
    if (m_isOpen == FALSE) {
        return FALSE;
    }
    return FS_FileRead(&m_file, buf, size) == size;
}

BOOL FlashFileStream::Seek(s32 pos) {
    if (!m_isOpen) {
        return FALSE;
    }
    return FS_FileSeek(&m_file, pos, 0);
}

u32 FlashFileStream::GetLength() {
    if (!m_isOpen) {
        return 0;
    }
    return m_file.endPosition - m_file.startPosition;
}

void FlashFileStream::Close() {
    if (!m_isOpen) {
        return;
    }
    FS_FileClose(&m_file);
    m_isOpen = FALSE;
}

u32 FlashFileStream::Tell() {
    if (!m_isOpen) {
        return 0;
    }
    return m_file.currentPosition - m_file.startPosition;
}
