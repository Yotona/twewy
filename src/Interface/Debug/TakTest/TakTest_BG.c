#include "Display.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/IO/Input.h"
#include "Engine/Overlay/OverlayDispatcher.h"
#include "Engine/Resources/ResourceMgr.h"
#include "Interface/Debug/TakTest.h"
#include "common_data.h"

typedef struct {
    /* 0x00 */ BgResource*      resScreen;
    /* 0x04 */ BgResource*      resChar;
    /* 0x08 */ PaletteResource* resPal;
    /* 0x0C */ s32              unk_0C;
    /* 0x10 */ s32              unk_10;
    /* 0x14 */ UnkOv31Struct    unk_14;
} TakTest_BG; // Size: 0x90

s32 TakTest_BG_RunTask(TaskPool* pool, Task* task, void* arg2, s32 arg3);

static const u16           data_ov043_020c7884[4] = {4, 5, 6, -1};
static const u16           data_ov043_020c788c[4] = {7, 8, 9, -1};
const TaskHandle           Tsk_TakTest_BG         = {"Tsk_TakTest_BG", TakTest_BG_RunTask, 0x90};
static const BinIdentifier data_ov043_020c787c    = {43, "Apl_Tak/Grp_MenuBadge_BGD00.bin"};

s32 TakTest_BG_Init(TaskPool* pool, Task* task, void* args) {
    Data*       pDVar1;
    BgResource* pBVar2;

    TakTest_BG* bg = task->data;

    MI_CpuSet(bg, 0, 0x90);
    FS_LoadOverlay(0, &OVERLAY_31_ID);

    pDVar1 = DatMgr_LoadRawData(1, 0, 0, &data_ov043_020c787c);

    u8*   puVar5 = Data_GetPackEntryData(pDVar1, 1);
    u32*  puVar6 = Data_GetPackEntryData(pDVar1, 2);
    void* iVar7  = Data_GetPackEntryData(pDVar1, 3);

    s32 uVar4 = (*(u32*)puVar5 & ~0xFF) >> 8;
    if ((*puVar5 & 0xF0) == 0) {
        uVar4 = uVar4 - 4;
    }
    bg->resChar = BgResMgr_AllocChar32(g_BgResourceManagers[DISPLAY_SUB], puVar5,
                                       g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[DISPLAY_BG1].charBase, 0, uVar4);

    bg->resScreen = BgResMgr_AllocScreen(g_BgResourceManagers[DISPLAY_SUB], puVar6,
                                         g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[DISPLAY_BG1].screenBase,
                                         g_DisplaySettings.engineState[DISPLAY_SUB].bgSettings[DISPLAY_BG1].screenSizeText);

    bg->resPal = PaletteMgr_AllocPalette(g_PaletteManagers[DISPLAY_SUB], iVar7, 0, 1, 7);

    u16 local_1c[4];
    local_1c   = data_ov043_020c7884;
    bg->unk_10 = puVar5;
    bg->unk_0C = puVar6;

    func_ov031_0210aa94(&bg->unk_14);
    func_ov031_0210b5fc(&bg->unk_14, &local_1c);
    func_ov031_0210ab28(&bg->unk_14, 0x32, 100);
    func_ov031_0210be18(&bg->unk_14, bg->unk_0C + 4, bg->unk_10 + 4, 0);
    func_ov031_0210aabc(&bg->unk_14);
    return 1;
}

s32 TakTest_BG_Update(TaskPool* pool, Task* task, void* args) {
    TakTest_BG* bg;
    u16         local_10[4];

    bg = task->data;
    if ((InputStatus.buttonState.pressedButtons & INPUT_BUTTON_A)) {
        local_10 = data_ov043_020c788c;
        func_ov031_0210aa94(&bg->unk_14);
        func_ov031_0210b5fc(&bg->unk_14, local_10);
        func_ov031_0210ab28(&bg->unk_14, 0x32, 100);
        func_ov031_0210be18(&bg->unk_14, bg->unk_0C + 4, bg->unk_10 + 4, 0);
        func_ov031_0210aabc(&bg->unk_14);
    }
    return 1;
}

s32 TakTest_BG_Render(TaskPool* pool, Task* task, void* args) {
    return 1;
}

s32 TakTest_BG_Destroy(TaskPool* pool, Task* task, void* args) {
    TakTest_BG* bg = task->data;
    BgResMgr_ReleaseChar(g_BgResourceManagers[DISPLAY_SUB], bg->resChar);
    BgResMgr_ReleaseScreen(g_BgResourceManagers[DISPLAY_SUB], bg->resScreen);
    PaletteMgr_ReleaseResource(g_PaletteManagers[DISPLAY_SUB], bg->resPal);
    FS_UnloadOverlay(0, &OVERLAY_31_ID);
    return 1;
}

s32 TakTest_BG_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = TakTest_BG_Init,
        .update     = TakTest_BG_Update,
        .render     = TakTest_BG_Render,
        .cleanup    = TakTest_BG_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
