#include "Engine/File/BinMgr.h"
#include "Interface/Menu/TusinSet.h"
#include <nitro/mi/cpumem.h>

typedef struct {
    /* 0x00 */ s32             dataType;
    /* 0x04 */ Sprite          sprites[2];
    /* 0x84 */ BOOL            visible[2];
    /* 0x8C */ TusinSetObject* tusinSet;
} TusinSet_itemU; // Size: 0x90

typedef struct {
    /* 0x0 */ s32             dataType;
    /* 0x4 */ TusinSetObject* tusinSet;
    /* 0x8 */ u16             itemId;
    /* 0xA */ u16             graphicIndex;
    /* 0xC */ u16             slot;
    /* 0xE */ u8              bufferIndex;
    /* 0xF */ u8              unk_F;
} TusinSet_itemU_Args;

static SpriteFrameInfo* TusinSet_itemU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              TusinSet_itemU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_TusinSet_itemU = {"Tsk_TusinSet_itemU", TusinSet_itemU_RunTask, sizeof(TusinSet_itemU)};

static const SpriteAnimation TusinSet_itemU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0xc00,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = TusinSet_itemU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &TusinSet_BinIdentifiers[12],
    .unk_18            = 2,
    .packIndex         = 1,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

static void TusinSet_itemU_LoadPackedData(TusinSet_itemU* itemU, Sprite* sprite, TusinSet_itemU_Args* args, u16 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&TusinSet_BinIdentifiers[10]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &TusinSet_BinIdentifiers[10], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &TusinSet_BinIdentifiers[10], packIndex, 0);
    }

    u8*   src        = (u8*)Data_GetPackEntryData(data, 1) + 4;
    void* paletteSrc = Data_GetPackEntryData(data, 4);
    u8*   dest       = (u8*)sprite->unk34 + 4;
    MI_CpuCopyU8(paletteSrc, sprite->unk3C, 0x20);

    s32 outer = 0;
    while (outer < 4) {
        s32 copyCount;
        s32 x;
        s32 y = 0;
        while (y < 4) {
            x = 0;
            while (x < 8) {
                copyCount = 0;
                while (copyCount < 4) {
                    *dest = *(src + outer * 0x80 + y * 4 + x * 0x10 + copyCount);
                    dest++;
                    copyCount++;
                }
                x++;
            }
            y++;
        }
        outer++;
    }

    DatMgr_ReleaseData(data);
}

static SpriteFrameInfo* TusinSet_itemU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void TusinSet_itemU_Load(TusinSet_itemU* itemU, Sprite* sprites, TusinSet_itemU_Args* args) {
    SpriteAnimation anim         = TusinSet_itemU_Anim;
    Point           positions[9] = {
        { 81, 105},
        {110, 105},
        { 81, 134},
        {110, 134},
        {209, 105},
        {238, 105},
        {209, 134},
        {238, 134},
        { 20, 172},
    };

    anim.dataType = args->dataType;

    itemU->visible[0] = args->unk_F == 1;

    anim.binIden   = &TusinSet_BinIdentifiers[7];
    anim.unk_18    = 0;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x - 11;
    anim.posY      = positions[args->slot].y + 11;
    anim.packIndex = 0;
    anim.animIndex = 58;
    _Sprite_Load(&sprites[0], &anim);

    anim.binIden   = &TusinSet_BinIdentifiers[12];
    anim.unk_18    = 2;
    anim.unk_22    = 1;
    anim.posX      = positions[args->slot].x;
    anim.posY      = positions[args->slot].y;
    anim.packIndex = args->slot + args->bufferIndex * 9 + 1;
    anim.animIndex = 1;
    _Sprite_Load(&sprites[1], &anim);

    if (args->itemId == 0xFFFF) {
        itemU->visible[1] = FALSE;
        return;
    }

    TusinSet_itemU_LoadPackedData(itemU, &sprites[1], args, args->graphicIndex + 1);
    itemU->visible[1] = TRUE;
}

static s32 TusinSet_itemU_Init(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemU*      itemU     = task->data;
    TusinSet_itemU_Args* itemUArgs = args;

    itemU->dataType = itemUArgs->dataType;
    itemU->tusinSet = itemUArgs->tusinSet;
    TusinSet_itemU_Load(itemU, itemU->sprites, itemUArgs);
    return 1;
}

static s32 TusinSet_itemU_Update(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemU* itemU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Update(&itemU->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_itemU_Render(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemU* itemU = task->data;

    for (u16 i = 0; i < 2; i++) {
        if (itemU->visible[i]) {
            Sprite_RenderFrame(&itemU->sprites[i]);
        }
    }
    return 1;
}

static s32 TusinSet_itemU_Destroy(TaskPool* pool, Task* task, void* args) {
    TusinSet_itemU* itemU = task->data;

    for (u16 i = 0; i < 2; i++) {
        Sprite_Release(&itemU->sprites[i]);
    }
    return 1;
}

static s32 TusinSet_itemU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = TusinSet_itemU_Init,
        .update     = TusinSet_itemU_Update,
        .render     = TusinSet_itemU_Render,
        .cleanup    = TusinSet_itemU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 TusinSet_itemU_CreateTask(TaskPool* pool, s32 dataType, u16 slot, TusinSetObject* tusinSet) {
    TusinSet_itemU_Args args;

    args.dataType      = dataType;
    args.tusinSet      = tusinSet;
    args.itemId        = tusinSet->slots[slot]->itemId;
    args.graphicIndex  = tusinSet->slots[slot]->graphicIndex;
    args.slot          = slot;
    args.bufferIndex   = tusinSet->unk_61EC;
    args.unk_F         = tusinSet->slots[slot]->abilityUnlocked;
    tusinSet->unk_61EC = 1 - tusinSet->unk_61EC;

    return EasyTask_CreateTask(pool, &Tsk_TusinSet_itemU, NULL, 0, NULL, &args);
}

void TusinSet_itemU_ReleaseSprite(TaskPool* pool, s32 taskId) {
    Sprite_Release(&((TusinSet_itemU*)EasyTask_GetTaskData(pool, taskId))->sprites[1]);
}
