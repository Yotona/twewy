#ifndef FIELD_FLASH_FLASHWRAPPER_H
#define FIELD_FLASH_FLASHWRAPPER_H

#include <nitro/types.h>

#ifdef __cplusplus
extern "C" {
#endif

void Flash_Init(void);
void Flash_Shutdown(void);
void Flash_Load(const char* path);
void Flash_Update(void);
void Flash_Draw(void);
void Flash_VBlank(void);
BOOL Flash_Start(void);
BOOL Flash_IsPlaying(void);
void Flash_SetDrawCallback(void (*callback)(void));
void Flash_ClearDrawCallback(void);

#ifdef __cplusplus
}
#endif

#endif // FIELD_FLASH_FLASHWRAPPER_H
