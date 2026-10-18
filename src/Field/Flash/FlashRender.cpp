#include "Field/FlashRegion.h"

extern "C" {
#include "Engine/Core/System.h"
#include "FixedPoint.h"
#include <nitro/g3.h>
#include <nitro/mi/cpumem.h>
#include <nitro/os/cache.h>
#include <nitro/os/cpustat.h>
}
#include "Field/FlashPlayer.h"

extern "C" {
extern const VecFx32 FlashCameraPos = {0, 0, 0x100000};
extern const VecFx32 FlashCameraUp  = {0, 0x1000, 0};

extern BOOL gFlashDrawBottomScreen;
extern BOOL gFlashSwapPending;

void FlashGfx_ShowMainOnTop(void);
void FlashGfx_ShowMainOnBottom(void);
void FlashGfx_SwapBuffers(void);
void func_02028108(s32 type, u32 dst, const void* src, u32 size);
void G3X_SetClearColor(u32 rgb, u32 alpha, u32 depth, u32 polygonId, BOOL fog);
void G3i_LookAt(const VecFx32* camPos, const VecFx32* camUp, const VecFx32* target, BOOL draw, void* mtx);
}

void FlashPlayer::DrawDisplayList(u16 yOffset) {
    FlashDisplayNode* node      = m_displayList.m_head;
    u16               polygonId = 0;

    while (node->next != NULL) {
        node->obj->Draw(&node->mtx, I2F(node->depth), yOffset, polygonId);
        node      = node->next;
        polygonId = (polygonId + 1) & 0x3F;
    }
}

void FlashPlayer::Render() {
    G3X_Reset();
    G3X_SetClearColor(m_bgColor, 1, 0x7FFF, 63, FALSE);

    VecFx32 camPos = FlashCameraPos;
    VecFx32 target = {0, 0, 0};
    VecFx32 camUp  = FlashCameraUp;

    G3i_LookAt(&camPos, &camUp, &target, TRUE, NULL);
    REG_GFX_FIFO_MATRIX_PUSH     = 0;
    REG_GFX_FIFO_MATRIX_MODE     = 3;
    REG_GFX_FIFO_MATRIX_IDENTITY = 0;
    REG_GFX_FIFO_MATRIX_MODE     = 2;
    if (gFlashDrawBottomScreen) {
        DrawDisplayList(m_height - 192);
    } else {
        DrawDisplayList(0);
    }
    REG_GFX_FIFO_MATRIX_POP = 1;
    FlashGfx_SwapBuffers();
}

extern "C" void FlashGfx_SwapScreens(void) {
    if (gFlashSwapPending && REG_VCOUNT <= (s16)193) {
        OS_Delay(784);
    }
    if ((REG_GFX_STATUS & 0x8000000) || (gFlashSwapPending == 0)) {
        return;
    }
    if (gFlashDrawBottomScreen) {
        FlashGfx_ShowMainOnBottom();
    } else {
        FlashGfx_ShowMainOnTop();
    }
    gFlashSwapPending      = FALSE;
    gFlashDrawBottomScreen = !gFlashDrawBottomScreen;
}

void FlashRect::Init(u16 id, const void* verts, u16 color) {
    m_id    = id;
    m_color = color;
    MI_CpuCopyU8(verts, m_verts, sizeof(m_verts));
}

void FlashRect::Draw(const FlashMatrix* mtx, fx32 z, u16 yOffset, u16 polygonId) {
    s32 x = FX_Whole(mtx->m[0][2]) / 20 - 128;
    s32 y = 96 - (FX_Whole(mtx->m[1][2]) / 20 - yOffset);
    s32 i;

    REG_GFX_FIFO_MATRIX_PUSH      = 0;
    REG_GFX_FIFO_MATRIX_TRANSLATE = x << 4;
    REG_GFX_FIFO_MATRIX_TRANSLATE = y << 4;
    REG_GFX_FIFO_MATRIX_TRANSLATE = z;
    REG_GFX_FIFO_TEXTURE_PARAM    = 0;
    REG_GFX_FIFO_POLYGON_ATTR     = 0x1F00C0 | (polygonId << 24);
    REG_GFX_FIFO_POLYGONS_BEGIN   = 1;
    REG_GFX_FIFO_VERTEX_COLOR     = m_color;
    for (i = 0; i < 4; i++) {
        G3_Vtx((s16)((m_verts[i].x / 20) << 4), (s16)((-m_verts[i].y / 20) << 4), 0);
    }
    REG_GFX_FIFO_POLYGONS_END = 0;
    REG_GFX_FIFO_MATRIX_POP   = 1;
}

void FlashImage::Init(u16 id, u16 charId, const FlashMatrix* mtx) {
    s32 i;
    s32 j;

    m_id     = id;
    m_charId = charId;
    m_mtx    = *mtx;
    for (i = 0; i < 2; i++) {
        for (j = 0; j < 2; j++) {
            m_mtx.m[i][j] /= 20;
        }
    }
}

FlashImage::~FlashImage() {
    Release();
}

void FlashImage::Draw(const FlashMatrix* mtx, fx32 z, u16 yOffset, u16 polygonId) {
    if (m_texture == NULL || m_alpha == 0) {
        return;
    }
    m_texture->Draw(mtx, &m_mtx, z, m_alpha, yOffset, &m_x, polygonId);
}

void FlashImage::OnPlace(FlashPlayer* player) {}

void FlashImage::Load(FlashPlayer* player) {
    if (m_texture != NULL) {
        return;
    }
    m_texture = new FlashTexture();
    m_texture->Init(player, player->FindCharDef(m_charId));
}

void FlashImage::Release() {
    if (m_texture != NULL) {
        delete m_texture;
        m_texture = NULL;
    }
    if (m_palette != NULL) {
        FlashFreeArray(m_palette);
        m_palette = NULL;
    }
}

void FlashImage::SetColorTransform(const FlashColorTransform* cxform) {
    u16*          dst;
    u16*          src;
    FlashCharDef* def;
    s32           i;

    if (cxform == NULL) {
        return;
    }
    MI_CpuCopyU8(cxform, &m_cxform, sizeof(m_cxform));
    if (m_palette == NULL) {
        m_palette = (u16*)FlashAllocArray(0x200);
    }
    dst = m_palette;
    def = m_texture->m_def;
    src = def->m_palette;
    if (cxform->flags != 0) {
        for (i = 0; i < m_texture->m_def->m_plttCount; i++) {
            u16 color = *src;
            s16 r     = ((color & 0x1F) * cxform->mul[0]) / 31 + cxform->add[0];
            s16 g;
            s16 b;

            if (r < 0) {
                r = 0;
            }
            if (r > 31) {
                r = 31;
            }
            g = (((color >> 5) & 0x1F) * cxform->mul[1]) / 31 + cxform->add[1];
            if (g < 0) {
                g = 0;
            }
            if (g > 31) {
                g = 31;
            }
            b = (((color >> 10) & 0x1F) * cxform->mul[2]) / 31 + cxform->add[2];
            if (b < 0) {
                b = 0;
            }
            if (b > 31) {
                b = 31;
            }
            *dst = (b << 10) | (g << 5) | r;
            dst++;
            src++;
        }
#ifdef REGION_USA
        DC_PurgeRange(m_palette, 0x200);
#endif
        func_02028108(1, m_texture->m_def->m_plttNode->addr, m_palette, 0x200);
    } else {
        func_02028108(1, def->m_plttNode->addr, src, 0x200);
        FlashFreeArray(m_palette);
        m_palette = NULL;
    }
}

void FlashImage::Reset() {
    FlashObject::Reset();
    Release();
}
