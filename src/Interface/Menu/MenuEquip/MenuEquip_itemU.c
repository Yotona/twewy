#include "Engine/File/BinMgr.h"
#include "Engine/File/DatMgr.h"
#include "Interface/Menu/MenuEquip.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ s32              dataType;
    /* 0x04 */ Sprite           sprite;
    /* 0x44 */ BOOL             visible;
    /* 0x48 */ MenuEquipObject* owner;
} MenuEquip_itemU; // Size: 0x4C

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
    /* 0x8 */ u16              itemId;
    /* 0xA */ u16              graphicIndex;
    /* 0xC */ u8               category;
    /* 0xD */ u8               bufferIndex;
} MenuEquip_itemU_Args;

static SpriteFrameInfo* MenuEquip_itemU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_itemU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_itemU = {"Tsk_MenuEquip_itemU", MenuEquip_itemU_RunTask, sizeof(MenuEquip_itemU)};

static const SpriteAnimation MenuEquip_itemU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x800,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_itemU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[7],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

// Nonmatching: copy loop register allocation
static void MenuEquip_itemU_LoadPackedData(MenuEquip_itemU* itemU, Sprite* sprite, MenuEquip_itemU_Args* args, u16 packIndex) {
    s32   dataType = args->dataType;
    Data* data;
    u8*   charData;
    u8*   palette;
    u8*   dst;

    if (BinMgr_FindById((s32)&data_ov043_020c9758[6]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &data_ov043_020c9758[6], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &data_ov043_020c9758[6], packIndex, 0);
    }

    charData = (u8*)Data_GetPackEntryData(data, 1) + 4;
    palette  = Data_GetPackEntryData(data, 4);

    dst = (u8*)sprite->unk34 + 4;
    MI_CpuCopyU8(palette, sprite->unk3C, 0x20);

    for (s32 blockY = 0; blockY < 4; blockY++) {
        for (s32 blockX = 0; blockX < 4; blockX++) {
            for (s32 row = 0; row < 8; row++) {
                u8* src = charData + blockY * 0x80 + blockX * 4 + row * 0x10;

                for (s32 col = 0; col < 4; col++) {
                    *dst++ = *src++;
                }
            }
        }
    }

    DatMgr_ReleaseData(data);
}

static SpriteFrameInfo* MenuEquip_itemU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void MenuEquip_itemU_Load(MenuEquip_itemU* itemU, Sprite* sprite, MenuEquip_itemU_Args* args) {
    SpriteAnimation anim = MenuEquip_itemU_Anim;

    anim.dataType  = args->dataType;
    anim.unk_04    = 0xEC;
    anim.unk_06    = 0x1C;
    anim.packIndex = args->bufferIndex + 1;
    _Sprite_Load(sprite, &anim);

    if (args->itemId == 0xFFFF) {
        itemU->visible = FALSE;
        return;
    }

    MenuEquip_itemU_LoadPackedData(itemU, sprite, args, args->graphicIndex + 1);
    itemU->visible = TRUE;
}

static s32 MenuEquip_itemU_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemU*      itemU     = task->data;
    MenuEquip_itemU_Args* itemUArgs = args;

    itemU->dataType = itemUArgs->dataType;
    itemU->owner    = itemUArgs->owner;
    MenuEquip_itemU_Load(itemU, &itemU->sprite, itemUArgs);
    return 1;
}

static s32 MenuEquip_itemU_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemU* itemU = task->data;

    Sprite_Update(&itemU->sprite);
    return 1;
}

static s32 MenuEquip_itemU_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemU* itemU = task->data;

    if (itemU->visible != 0) {
        Sprite_RenderFrame(&itemU->sprite);
    }
    return 1;
}

static s32 MenuEquip_itemU_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_itemU* itemU = task->data;

    Sprite_Release(&itemU->sprite);
    return 1;
}

static s32 MenuEquip_itemU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_itemU_Init,
        .update     = MenuEquip_itemU_Update,
        .render     = MenuEquip_itemU_Render,
        .cleanup    = MenuEquip_itemU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_itemU_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_itemU_Args args;

    args.dataType      = dataType;
    args.owner         = owner;
    args.itemId        = owner->cursorItem.itemId;
    args.graphicIndex  = owner->cursorItem.graphicIndex;
    args.category      = owner->cursorItem.category;
    args.bufferIndex   = owner->itemUBuffer;
    owner->itemUBuffer = 1 - owner->itemUBuffer;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_itemU, NULL, 0, NULL, &args);
}
