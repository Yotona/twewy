#include "Field/FieldRegion.h"

#include "Engine/EasyTask.h"
#include "Field/Event/EventTables.h"
#include "Field/Tasks/Fld_GetMess.h"
#include "Field/Tasks/Fld_MapJumpName.h"
#include "SpriteMgr.h"
#include "Util/SysFont.h"
#include <nitro/types.h>

extern void func_020265d4(s32* value, s32 target, u16 scale);

typedef struct {
    /* 0x00 */ Sprite  sprite;
    /* 0x40 */ s32     unk_40;
    /* 0x44 */ s32     unk_44;
    /* 0x48 */ u16     unk_48;
    /* 0x4A */ u8      unk_4A;
    /* 0x4C */ SysFont font;
    /* 0xC8 */ void*   unk_C8;
} Fld_GetMess; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32 unk_0;
    /* 0x4 */ u16 unk_4;
    /* 0x6 */ u16 unk_6;
} Fld_GetMess_Args;

static SpriteFrameInfo* Fld_GetMess_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Fld_GetMess_RunTask(TaskPool*, Task*, void*, s32);

static const TaskHandle Tsk_Fld_GetMess = {"Tsk_Fld_GetMess", Fld_GetMess_RunTask, sizeof(Fld_GetMess)};

static const SpriteAnimation Fld_GetMess_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .posX              = 0xFF80,
    .posY              = 0xA0,
    .frameInfoCallback = Fld_GetMess_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Iden_FldDownScreen,
    .unk_18            = 2,
    .packIndex         = 11,
    .unk_1C            = 2,
    .unk_1E            = 0,
    .unk_20            = 1,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 3,
    .unk_28            = 4,
    .animIndex         = 3,
};

static SpriteFrameInfo* Fld_GetMess_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallback(sprite, mode);
}

static void Fld_GetMess_Load(Sprite* sprite, Fld_GetMess_Args* args) {
    SpriteAnimation anim = Fld_GetMess_Anim;

    anim.dataType = args->unk_0;
    _Sprite_Load(sprite, &anim);
}

static s32 Fld_GetMess_Init(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess*      mess     = task->data;
    Fld_GetMess_Args* messArgs = args;

    SysFont_InitWithFont(&mess->font, 1, TRUE);
    SysFont_SetSpacing(&mess->font, TRUE, 0);
    Fld_GetMess_Load(&mess->sprite, messArgs);
    Sprite_Update(&mess->sprite);

    mess->unk_4A = 2;
    mess->unk_40 = 0x180000;
    mess->unk_44 = 0x80000;
    mess->unk_48 = 0x14;

    if (messArgs->unk_6 == 0xFFFF) {
        mess->unk_C8 = SysFont_GetMsgBuf(&mess->font, messArgs->unk_4);
        SysFont_SetHAlign(&mess->font, 1, SysFont_MeasureWidth(&mess->font, mess->unk_C8));
        SysFont_SetPos(&mess->font, 13, 9);
    } else if (messArgs->unk_6 == 0xFFFE) {
        mess->unk_C8 = SysFont_GetMsgBuf(&mess->font, messArgs->unk_4);
        Fld_Mess_CenterText(&mess->font, mess->unk_C8, 0, &mess->sprite);
    } else {
        mess->unk_C8 = Fld_Mess_GetStrCat(&mess->font, messArgs->unk_4, messArgs->unk_6);
        Fld_Mess_CenterText(&mess->font, mess->unk_C8, 0, &mess->sprite);
    }
    return 1;
}

static s32 Fld_GetMess_Update(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess* mess = task->data;

    if (mess->unk_48 != 0) {
        func_020265d4(&mess->unk_40, mess->unk_44, mess->unk_48);
        mess->unk_48--;
        mess->sprite.posX = F2I(mess->unk_40);
    }
    return 1;
}

static s32 Fld_GetMess_Render(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess* mess = task->data;

    Sprite_RenderFrame(&mess->sprite);
    if (mess->unk_4A != 0) {
        mess->unk_4A--;
        if ((mess->unk_4A & 0xFF) == 0) {
            SysFont_DrawToSprite(&mess->font, mess->unk_C8, &mess->sprite, TRUE);
            Mem_Free(&gDebugHeap, mess->unk_C8);
            SysFont_Destroy(&mess->font);
            mess->unk_C8 = NULL;
        }
    }
    return 1;
}

static s32 Fld_GetMess_Destroy(TaskPool* pool, Task* task, void* args) {
    Fld_GetMess* mess = task->data;

    Sprite_Destroy(&mess->sprite);
    if (mess->unk_C8 != NULL) {
        Mem_Free(&gDebugHeap, mess->unk_C8);
        SysFont_Destroy(&mess->font);
    }
    return 1;
}

static s32 Fld_GetMess_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    const TaskStages stages = {
        .initialize = Fld_GetMess_Init,
        .update     = Fld_GetMess_Update,
        .render     = Fld_GetMess_Render,
        .cleanup    = Fld_GetMess_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Fld_GetMess_CreateTask(TaskPool* arg0, s32 arg1, s16 arg2, s16 arg3) {
    Fld_GetMess_Args args;
    args.unk_0 = arg1;
    args.unk_4 = arg2;
    args.unk_6 = arg3;
    return EasyTask_CreateTask(arg0, &Tsk_Fld_GetMess, NULL, 0, NULL, &args);
}
