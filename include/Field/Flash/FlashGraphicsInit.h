#ifndef FIELD_FLASH_FLASHGRAPHICSINIT_H
#define FIELD_FLASH_FLASHGRAPHICSINIT_H

#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

extern BOOL gFlashDrawBottomScreen;
extern BOOL gFlashSwapPending;
void        FlashGfx_Init(void);
void        FlashGfx_ShowMainOnTop(void);
void        FlashGfx_ShowMainOnBottom(void);
void        FlashGfx_SwapBuffers(void);
void        FlashGfx_Init3D(void);

#ifdef __cplusplus
}
#endif

#endif // FIELD_FLASH_FLASHGRAPHICSINIT_H
