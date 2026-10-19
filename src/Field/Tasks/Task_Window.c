#include "Field/Tasks/Task_Window.h"
#include "Engine/EasyTask.h"
#include "Engine/File/DatMgr.h"
#include "Engine/Resources/PaletteMgr.h"
#include "SpriteMgr.h"
#include <nitro/types.h>

typedef struct {
    Sprite            sprite;
    s32               childTaskId;
    s16               unk_44;
    s16               unk_46;
    s32               paletteParam;
    s32               unk_4C;
    s32               unk_50;
    s32               unk_54;
    s32               unk_58;
    s16               unk_5C;
    s16               unk_5E;
    u8                unk_60;
    u8                unk_61[7];
    u8                unk_68;
    u8                unk_69;
    u8                unk_6A;
    u8                unk_6B[5];
    Data*             resourceData;
    UnkSmallInternal* paletteData;
    PaletteResource*  paletteResource;
    u16               paletteSlot;
    u16               unk_7E;
    s32               keepResources;
} Task_Window; // Size: 0x84

extern s32 func_ov030_020cab64(TaskPool* pool, Task* task, void* args);
extern s32 func_ov030_020cae10(TaskPool* pool, Task* task, void* args);

s32 Task_Window_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

char data_ov030_02108904[28] = "Apl_Fuk/Grp_FldMessWin.bin";

const u8 data_ov030_020ecdfc[40] = {
    1,  1,  2,  2,  3,  3,  4, 4, 4, 5, 5, 6, 6, 7, 7, 8, 8, 9, 9, 10,
    10, 10, 11, 11, 12, 12, 1, 1, 2, 2, 3, 3, 4, 4, 4, 5, 5, 6, 6, 0,
};

u8 func_ov030_020ca4bc(u8* toggle, s32 category, s32 value) {
    if (category < 2) {
        if (*toggle == 1) {
            *toggle = 0;
        }
    } else if (*toggle == 0) {
        *toggle = 1;
    }
    value -= 4;
    if (value < 0) {
        value = 0;
    } else if (value > 12) {
        value = 12;
    }
    if (category > 2) {
        category = 2;
    }
    return (&data_ov030_020ecdfc[category * 13])[value];
}

u8 func_ov030_020ca514(s32 alternate, s32 value) {
    if (alternate == 0) {
        value -= 3;
        if (value <= 1) {
            return 1;
        }
        if (value > 13) {
            return 13;
        }
        return (u8)value;
    }
    if (value <= 4) {
        return 0xE;
    }
    if (value <= 5) {
        return 0xF;
    }
    if (value <= 6) {
        return 0x10;
    }
    if (value <= 7) {
        return 0x11;
    }
    if (value <= 8) {
        return 0x12;
    }
    if (value <= 9) {
        return 0x13;
    }
    return 0x14;
}

u32 func_ov030_020cab3c(u32 value) {
    if (value == 0 || value == 1 || value == 6 || value == 4) {
        return 0;
    }
    return value < 0x15;
}

void func_ov030_020cb09c(Task_Window* state) {
    u32 palette = state->sprite.unk_0A.unk_10;
    s32 originalParam;

    state->sprite.unk_0A.unk_10 = 2;
    originalParam               = state->paletteParam;
    state->paletteParam         = originalParam + 10;
    state->sprite.posX += 3;
    state->sprite.posY += 3;
    Sprite_RenderAltPalette(&state->sprite, state->paletteResource, state->paletteData, 0);
    state->sprite.unk_0A.unk_10 = palette;
    state->paletteParam         = originalParam;
    state->sprite.posX -= 3;
    state->sprite.posY -= 3;
}

const TaskHandle Tsk_Task_Window = {"Task_Window", Task_Window_RunTask, sizeof(Task_Window)};

s32 Task_Window_Render(TaskPool* pool, Task* task, void* args) {
    Task_Window* data = task->data;

    if (data->unk_69 >= 7) {
        func_ov030_020cb09c(data);
    }
    Sprite_RenderAltPalette(&data->sprite, NULL, NULL, 0);
    data->unk_6A = 1;
    return 1;
}

s32 Task_Window_Destroy(TaskPool* pool, Task* task, void* args) {
    Task_Window* data = task->data;

    if (data->keepResources != 0) {
        return 1;
    }
    Sprite_Destroy(&data->sprite);
    if (data->childTaskId != -1) {
        EasyTask_DeleteTask(pool, data->childTaskId);
    }
    if (data->paletteResource != NULL) {
        PaletteMgr_ReleaseResource(g_PaletteManagers[data->paletteSlot], data->paletteResource);
    }
    DatMgr_ReleaseData(data->resourceData);
    return 1;
}

s32 Task_Window_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = func_ov030_020cab64,
        .update     = func_ov030_020cae10,
        .render     = Task_Window_Render,
        .cleanup    = Task_Window_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}
