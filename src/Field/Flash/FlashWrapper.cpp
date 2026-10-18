#include "Field/Flash/FlashWrapper.h"
#include "Field/Flash/FlashPlayer.h"

typedef struct {
    /* 0x00 */ FlashEventCallback* eventListener;
    /* 0x04 */ BOOL                initialized;
    /* 0x08 */ FlashManager*       manager;
    /* 0x0C */ FlashFrameListener* frameListener;
#ifdef REGION_USA
    /* 0x10 */ s32 unk_10;
#endif
} FlashState; // Size: 0x14

FlashState gFlash;

extern "C" void Flash_Init(void) {
    gFlash.initialized = FALSE;
    gFlash.manager     = new FlashManager();
    FlashManager::InitGraphics();
    gFlash.eventListener = new FlashEventCallback();
    gFlash.frameListener = new FlashFrameListener();
    gFlash.initialized   = TRUE;
}

extern "C" void Flash_Shutdown(void) {
    gFlash.initialized = FALSE;
    gFlash.manager->RemoveListener(gFlash.eventListener);
    delete gFlash.eventListener;
    gFlash.eventListener = NULL;
    gFlash.manager->RemoveFrameListener(gFlash.frameListener);
    delete gFlash.frameListener;
    gFlash.frameListener = NULL;
    FlashTexture::FreeHeaps();
    delete gFlash.manager;
    gFlash.manager = NULL;
}

extern "C" void Flash_Load(const char* path) {
    gFlash.manager->Load(path);
    gFlash.manager->AddListener(gFlash.eventListener);
    gFlash.manager->AddFrameListener(gFlash.frameListener);
}

extern "C" void Flash_Update(void) {
    gFlash.manager->Update();
}

extern "C" void Flash_Draw(void) {
    gFlash.manager->Draw();
}

extern "C" void Flash_VBlank(void) {
    if (gFlash.manager != NULL && gFlash.initialized == TRUE) {
        gFlash.manager->VBlank();
    }
}

extern "C" BOOL Flash_Start(void) {
    return gFlash.manager->Start();
}

extern "C" BOOL Flash_IsPlaying(void) {
    return gFlash.manager->IsPlaying();
}

extern "C" void Flash_SetDrawCallback(void (*callback)(void)) {
    gFlash.manager->SetDrawCallback(callback);
}

extern "C" void Flash_ClearDrawCallback(void) {
    gFlash.manager->ClearDrawCallback();
}
