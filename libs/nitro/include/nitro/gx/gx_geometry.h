#ifndef GX_GEOMETRY_H
#define GX_GEOMETRY_H

#ifdef __cplusplus
extern "C" {
#endif

#include <nitro/card.h>
#include <nitro/reg.h>

static inline void GX_Vtx16(u16 x, u16 y, u16 z) {
    REG_GFX_FIFO_VERTEX_16 = x | (y << 16);
    REG_GFX_FIFO_VERTEX_16 = z;
}

static inline void GX_TexCoord(u32 s, u32 t) {
    REG_GFX_FIFO_VERTEX_TEXCOORD = ((u16)(s >> 8)) | (((u16)(t >> 8)) << 16);
}

#ifdef __cplusplus
}
#endif

#endif // GX_GEOMETRY_H