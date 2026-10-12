#include "Otosu/OtosuGame.h"

typedef struct {
    /* 0x00 */ s32    dataType;
    /* 0x04 */ Sprite sprite;
    /* 0x44 */ BOOL   visible;
    /* 0x48 */ Data*  data;
    /* 0x4C */ struct {
        PaletteResource* palette;
        BgResource*      chars;
        BgResource*      screen;
    } bg[2];        // one per display
} OtosuGame_wrwait; // Size: 0x64

static s32              OtosuGame_wrwait_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);
static SpriteFrameInfo* OtosuGame_wrwait_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);

static const TaskHandle Tsk_OtosuGame_wrwait = {"Tsk_OtosuGame_wrwait", OtosuGame_wrwait_RunTask, sizeof(OtosuGame_wrwait)};

static const SpriteAnimation OtosuGame_wrwait_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0x80,
    .posY              = 0x60,
    .frameInfoCallback = OtosuGame_wrwait_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &OtuFile_UnderObj,
    .unk_18            = 2,
    .packIndex         = 8,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static SpriteFrameInfo* OtosuGame_wrwait_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 3);
}

static void OtosuGame_wrwait_Load(OtosuGame_wrwait* self, Sprite* sprite) {
    SpriteAnimation anim = OtosuGame_wrwait_Anim;

    anim.owner    = self;
    anim.dataType = self->dataType;
    _Sprite_Load(sprite, &anim);
}

static s32 OtosuGame_wrwait_Init(TaskPool* pool, Task* task, void* args) {
    OtuTaskArgs1*     taskArgs = args;
    OtosuGame_wrwait* self     = task->data;

    self->visible  = TRUE;
    self->dataType = taskArgs->dataType;

    g_DisplaySettings.controls[DISPLAY_MAIN].layers = 0x1F;
    g_DisplaySettings.controls[DISPLAY_MAIN].layers |= 0x12;
    Display_SetBGOffset(DISPLAY_MAIN, 1, 0, 0);

    g_DisplaySettings.controls[DISPLAY_SUB].layers = 0x1F;
    g_DisplaySettings.controls[DISPLAY_SUB].layers |= 0x12;
    Display_SetBGOffset(DISPLAY_SUB, 1, 0, 0);

    self->data = DatMgr_LoadRawData(taskArgs->dataType, NULL, 0, &OtuFile_UnderBg);

    void* pal = Data_GetPackEntryData(self->data, 1);
    void* chr = Data_GetPackEntryData(self->data, 2);
    void* scr = Data_GetPackEntryData(self->data, 3);

    self->bg[DISPLAY_MAIN].palette = PaletteMgr_AllocPalette(g_PaletteManagers[0], pal, 0, 0, 1);
    {
        u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

        if ((*(u8*)chr & 0xF0) == 0) {
            size -= 4;
        }
        self->bg[DISPLAY_MAIN].chars = BgResMgr_AllocChar32(g_BgResourceManagers[0], chr,
                                                            g_DisplaySettings.engineState[0].bgSettings[1].charBase, 0, size);
    }
    self->bg[DISPLAY_MAIN].screen =
        BgResMgr_AllocScreen(g_BgResourceManagers[0], scr, g_DisplaySettings.engineState[0].bgSettings[1].screenBase,
                             g_DisplaySettings.engineState[0].bgSettings[1].screenSizeText);
    PaletteMgr_Flush(g_PaletteManagers[0], self->bg[DISPLAY_MAIN].palette);

    self->bg[DISPLAY_SUB].palette = PaletteMgr_AllocPalette(g_PaletteManagers[1], pal, 0, 0, 1);
    {
        u32 size = (u32)((*(u32*)chr & ~0xFF) >> 8);

        if ((*(u8*)chr & 0xF0) == 0) {
            size -= 4;
        }
        self->bg[DISPLAY_SUB].chars = BgResMgr_AllocChar32(g_BgResourceManagers[1], chr,
                                                           g_DisplaySettings.engineState[1].bgSettings[1].charBase, 0, size);
    }
    self->bg[DISPLAY_SUB].screen =
        BgResMgr_AllocScreen(g_BgResourceManagers[1], scr, g_DisplaySettings.engineState[1].bgSettings[1].screenBase,
                             g_DisplaySettings.engineState[1].bgSettings[1].screenSizeText);
    PaletteMgr_Flush(g_PaletteManagers[DISPLAY_SUB], self->bg[DISPLAY_SUB].palette);

    OtosuGame_wrwait_Load(self, &self->sprite);
    EasyFade_FadeMainDisplay(FADER_SMOOTH, 0, 0x1000);
    EasyFade_FadeSubDisplay(FADER_SMOOTH, 0, 0x1000);
    return 1;
}

static s32 OtosuGame_wrwait_Update(TaskPool* pool, Task* task, void* args) {
    OtosuGame_wrwait* self = task->data;

    if (self->visible) {
        Sprite_Update(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_wrwait_Render(TaskPool* pool, Task* task, void* args) {
    OtosuGame_wrwait* self = task->data;

    if (self->visible) {
        Sprite_RenderFrame(&self->sprite);
    }
    return 1;
}

static s32 OtosuGame_wrwait_Destroy(TaskPool* pool, Task* task, void* args) {
    OtosuGame_wrwait* self = task->data;

    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_MAIN], self->bg[DISPLAY_MAIN].screen);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_MAIN], self->bg[DISPLAY_MAIN].chars);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_MAIN], self->bg[DISPLAY_MAIN].palette);

    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], self->bg[DISPLAY_SUB].screen);
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], self->bg[DISPLAY_SUB].chars);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], self->bg[DISPLAY_SUB].palette);

    DatMgr_ReleaseData(self->data);
    Sprite_Release(&self->sprite);
    return 1;
}

static s32 OtosuGame_wrwait_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = OtosuGame_wrwait_Init,
        .update     = OtosuGame_wrwait_Update,
        .render     = OtosuGame_wrwait_Render,
        .cleanup    = OtosuGame_wrwait_Destroy,
    };

    return stages.iter[stage](pool, task, args);
}

s32 OtosuGame_wrwait_CreateTask(TaskPool* pool, s32 dataType) {
    OtuTaskArgs1 args;

    args.dataType = dataType;
    return EasyTask_CreateTask(pool, &Tsk_OtosuGame_wrwait, NULL, 0, NULL, &args);
}
