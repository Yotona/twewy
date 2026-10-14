#include "Field/FlashRegion.h"

#include "Field/FlashPlayer.h"

typedef struct {
    /* 0x00 */ FlashEventCallback* eventListener;
    /* 0x04 */ BOOL                initialized;
    /* 0x08 */ FlashManager*       manager;
    /* 0x0C */ FlashFrameListener* frameListener;
#ifdef REGION_USA
    /* 0x10 */ s32 unk_10;
#endif
} FlashState; // Size: 0x14

FlashState data_ov030_0210a1f8;

extern "C" void func_ov030_020d63c8(void) {
    data_ov030_0210a1f8.initialized = FALSE;
    data_ov030_0210a1f8.manager     = new FlashManager();
    FlashManager::InitGraphics();
    data_ov030_0210a1f8.eventListener = new FlashEventCallback();
    data_ov030_0210a1f8.frameListener = new FlashFrameListener();
    data_ov030_0210a1f8.initialized   = TRUE;
}

extern "C" void func_ov030_020d6470(void) {
    data_ov030_0210a1f8.initialized = FALSE;
    data_ov030_0210a1f8.manager->RemoveListener(data_ov030_0210a1f8.eventListener);
    delete data_ov030_0210a1f8.eventListener;
    data_ov030_0210a1f8.eventListener = NULL;
    data_ov030_0210a1f8.manager->RemoveFrameListener(data_ov030_0210a1f8.frameListener);
    delete data_ov030_0210a1f8.frameListener;
    data_ov030_0210a1f8.frameListener = NULL;
    FlashTexture::FreeHeaps();
    delete data_ov030_0210a1f8.manager;
    data_ov030_0210a1f8.manager = NULL;
}

extern "C" void func_ov030_020d6548(const char* path) {
    data_ov030_0210a1f8.manager->Load(path);
    data_ov030_0210a1f8.manager->AddListener(data_ov030_0210a1f8.eventListener);
    data_ov030_0210a1f8.manager->AddFrameListener(data_ov030_0210a1f8.frameListener);
}

extern "C" void func_ov030_020d6584(void) {
    data_ov030_0210a1f8.manager->Update();
}

extern "C" void func_ov030_020d659c(void) {
    data_ov030_0210a1f8.manager->Draw();
}

extern "C" void func_ov030_020d65b4(void) {
    if (data_ov030_0210a1f8.manager != NULL && data_ov030_0210a1f8.initialized == TRUE) {
        data_ov030_0210a1f8.manager->VBlank();
    }
}

extern "C" BOOL func_ov030_020d65e0(void) {
    return data_ov030_0210a1f8.manager->Start();
}

extern "C" BOOL func_ov030_020d65f8(void) {
    return data_ov030_0210a1f8.manager->IsPlaying();
}

extern "C" void func_ov030_020d6610(void (*callback)(void)) {
    data_ov030_0210a1f8.manager->SetDrawCallback(callback);
}

extern "C" void func_ov030_020d662c(void) {
    data_ov030_0210a1f8.manager->ClearDrawCallback();
}
