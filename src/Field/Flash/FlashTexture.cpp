#include "Field/FlashRegion.h"

extern "C" {
#include <nitro/g3.h>
}
#include "Field/FlashPlayer.h"

typedef struct {
    /* 0x0 */ FlashHeap* texHeap;
    /* 0x4 */ FlashHeap* plttHeap;
#ifdef REGION_USA
    /* 0x8 */ s32 unk_08;
#endif
} FlashTextureHeaps; // Size: 0xC

FlashTextureHeaps data_ov030_0210a20c;

FlashTexture::FlashTexture() {
    m_def = NULL;
}

FlashTexture::~FlashTexture() {
    m_def->m_texRefs--;
    if (m_def->m_texRefs == 0) {
        data_ov030_0210a20c.texHeap->Free(m_def->m_texNode);
        data_ov030_0210a20c.plttHeap->Free(m_def->m_plttNode);
        m_def->m_state = 0;
        FlashFreeArray(m_def->m_palette);
        m_def->m_palette = NULL;
    }
}

void FlashTexture::InitHeaps() {
    if (data_ov030_0210a20c.texHeap == NULL) {
        data_ov030_0210a20c.texHeap = new FlashHeap();
    }
    data_ov030_0210a20c.texHeap->Init(0x40000);
    if (data_ov030_0210a20c.plttHeap == NULL) {
        data_ov030_0210a20c.plttHeap = new FlashHeap();
    }
    data_ov030_0210a20c.plttHeap->Init(0x10000);
}

void FlashTexture::FreeHeaps() {
    if (data_ov030_0210a20c.texHeap != NULL) {
        delete data_ov030_0210a20c.texHeap;
        data_ov030_0210a20c.texHeap = NULL;
    }
    if (data_ov030_0210a20c.plttHeap != NULL) {
        delete data_ov030_0210a20c.plttHeap;
        data_ov030_0210a20c.plttHeap = NULL;
    }
}

void FlashTexture::Init(FlashPlayer* player, FlashCharDef* def) {
    u32 size;

    m_def       = def;
    m_id        = def->m_id;
    m_widthLog  = def->m_widthLog;
    m_heightLog = def->m_heightLog;
    m_format    = def->m_format;
    m_unk_12    = def->m_unk_16;
    m_unk_14    = def->m_unk_18;
    def->m_texRefs++;
    if (def->m_state != 0) {
        return;
    }
    size = (8 << m_widthLog) * (8 << m_heightLog);
    switch (m_format) {
        case 2:
            size >>= 2;
            break;
        case 3:
            size >>= 1;
            break;
        case 4:
            break;
    }
    def->m_texNode  = data_ov030_0210a20c.texHeap->Alloc(size);
    def->m_plttNode = data_ov030_0210a20c.plttHeap->Alloc(0x200);
    player->m_manager->m_loader->Load(player->m_stream, def, def->m_texNode->addr, def->m_plttNode->addr);
}

static inline u32 FlashTexCoord(fx32 s, fx32 t) {
    return (u16)(fx16)(s >> 8) | ((u16)(fx16)(t >> 8) << 16);
}

#define FLASH_VERTEX(x, y)                                                                     \
    {                                                                                          \
        fx32 vx = FX_Mul(x, mtx->m[0][0]) + FX_Mul(y, mtx->m[0][1]) + mtx->m[0][2];            \
        fx32 vy = -(FX_Mul(x, mtx->m[1][0]) + FX_Mul(y, mtx->m[1][1]) + mtx->m[1][2]);         \
        s32  px = FX_Whole(FX_Mul(vx, nodeMtx->m[0][0]) + FX_Mul(vy, -nodeMtx->m[0][1])) / 20; \
        s32  py = FX_Whole(FX_Mul(vx, -nodeMtx->m[1][0]) + FX_Mul(vy, nodeMtx->m[1][1])) / 20; \
        G3_Vtx((s16)(px << 4), (s16)(py << 4), 0);                                             \
    }

// Nonmatching: scheduling and 64-bit multiply CSE in the vertex transforms
void FlashTexture::Draw(const FlashMatrix* nodeMtx, const FlashMatrix* mtx, fx32 z, u8 alpha, u16 yOffset, const fx32* bounds,
                        u16 polygonId) {
    s32  x;
    s32  y;
    u32  plttAddr;
    fx32 s;
    fx32 t;

    if (m_def->m_state != 2) {
        return;
    }
    x                             = FX_Whole(nodeMtx->m[0][2]) / 20 - 128;
    y                             = 96 - (FX_Whole(nodeMtx->m[1][2]) / 20 - yOffset);
    REG_GFX_FIFO_MATRIX_PUSH      = 0;
    REG_GFX_FIFO_MATRIX_TRANSLATE = x << 4;
    REG_GFX_FIFO_MATRIX_TRANSLATE = y << 4;
    REG_GFX_FIFO_MATRIX_TRANSLATE = z;
    REG_GFX_FIFO_TEXTURE_PARAM    = (m_def->m_texNode->addr >> 3) | (m_format << 26) | 0x40000000 | (m_widthLog << 20) |
                                 (m_heightLog << 23) | 0x30000 | 0x20000000;
    plttAddr                                = (m_format != 2) ? m_def->m_plttNode->addr : m_def->m_plttNode->addr << 1;
    REG_GFX_FIFO_TEXTURE_PALETTE            = plttAddr >> 4;
    REG_GFX_FIFO_MATERIAL_SPECULAR_EMISSION = 0x7FFF0000;
    REG_GFX_FIFO_POLYGON_ATTR               = (polygonId << 24) | 0x8C0 | (alpha << 16);
    t                                       = m_unk_14 << 12;
    s                                       = m_unk_12 << 12;
    REG_GFX_FIFO_POLYGONS_BEGIN             = 1;
    REG_GFX_FIFO_VERTEX_TEXCOORD            = FlashTexCoord(0, t);
    REG_GFX_FIFO_NORMAL                     = 0x1FF00000;
    FLASH_VERTEX(bounds[0], bounds[3])
    REG_GFX_FIFO_VERTEX_TEXCOORD = FlashTexCoord(0, 0);
    REG_GFX_FIFO_NORMAL          = 0x1FF00000;
    FLASH_VERTEX(bounds[0], bounds[1])
    REG_GFX_FIFO_VERTEX_TEXCOORD = FlashTexCoord(s, 0);
    REG_GFX_FIFO_NORMAL          = 0x1FF00000;
    FLASH_VERTEX(bounds[2], bounds[1])
    REG_GFX_FIFO_VERTEX_TEXCOORD = FlashTexCoord(s, t);
    REG_GFX_FIFO_NORMAL          = 0x1FF00000;
    FLASH_VERTEX(bounds[2], bounds[3])
    REG_GFX_FIFO_POLYGONS_END = 0;
    REG_GFX_FIFO_MATRIX_POP   = 1;
}
