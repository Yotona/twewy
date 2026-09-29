#include "Engine/Core/Memory.h"
#include "Interface/Menu/Tusin.h"
#include "Player/Inventory.h"
#include "Util/SysFont.h"
#include <nitro/mi/cpumem.h>

// USA shows the gift item icon left of the belt; JP creates the two sprites the other way round.
#ifdef REGION_USA
    #define BELT_SPRITE 1
    #define ICON_SPRITE 0
#else
    #define BELT_SPRITE 0
    #define ICON_SPRITE 1
#endif

typedef struct {
    /* 0x000 */ s32          unk_00;
    /* 0x004 */ Sprite       sprites[2]; // [BELT_SPRITE], [ICON_SPRITE]
    /* 0x084 */ BOOL         visible[2];
    /* 0x08C */ TusinObject* tusin;
    /* 0x090 */ s32          posX;
    /* 0x094 */ s32          posY;
    /* 0x098 */ s32          speedX;
    /* 0x09C */ s32          speedY;
    /* 0x0A0 */ s32          decelX;
    /* 0x0A4 */ s32          unk_A4;
    /* 0x0A8 */ u16          state;
    /* 0x0AA */ s16          timer;
    /* 0x0AC */ s16          row;
    /* 0x0AE */ u16          mode;
    /* 0x0B0 */ SysFont      fonts[2];
    /* 0x1A8 */ SysCode      texts[2][50];
    /* 0x270 */ s32          unk_270;
    /* 0x274 */ u16          drawDelay;
    /* 0x276 */ char         unk_276[0x27C - 0x276];
} Tusin_beltU; // Size: 0x27C

typedef struct {
    /* 0x0 */ s32          dataType;
    /* 0x4 */ TusinObject* tusin;
    /* 0x8 */ u16          mode;
#ifdef REGION_USA
    /* 0xA */ u16 index;
#endif
} Tusin_beltU_Args;

static SpriteFrameInfo* Tusin_beltU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Tusin_beltU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Tusin_beltU = {"Tsk_Tusin_beltU", Tusin_beltU_RunTask, sizeof(Tusin_beltU)};

static const SpriteAnimation Tusin_beltU_Anim = {
    .bits_0_1          = 0,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .posX              = 0x50,
    .posY              = 0x50,
    .frameInfoCallback = Tusin_beltU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Tusin_BinIdentifiers[5],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 1,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .animIndex         = 1,
};

#ifdef REGION_USA
static void Tusin_beltU_LoadPackedData(Tusin_beltU* beltU, Sprite* sprite, Tusin_beltU_Args* args, u16 packIndex) {
    s32 dataType = args->dataType;

    Data* data;
    if (BinMgr_FindById((s32)&Tusin_BinIdentifiers[8]) == NULL) {
        data = DatMgr_LoadPackEntry(dataType, NULL, 0, &Tusin_BinIdentifiers[8], packIndex, FALSE);
    } else {
        data = DatMgr_LoadPackEntryDirect(dataType, &Tusin_BinIdentifiers[8], packIndex, 0);
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
#endif

static SpriteFrameInfo* Tusin_beltU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Tusin_beltU_Load(Tusin_beltU* beltU, Sprite* sprites, Tusin_beltU_Args* args) {
    TusinObject*    tusin = beltU->tusin;
    SpriteAnimation anim  = Tusin_beltU_Anim;
    RawItemData     itemData;
    RawFoodData     foodData;
    RawTreasureData treasureData;
    u16             index    = Inventory_GetCategorizedIndex(tusin->giftItemId);
    s32             category = Inventory_GetCategory(tusin->giftItemId);
    u16             graphicIndex;

#ifdef REGION_USA
    if (index == 0xFFFF) {
        graphicIndex      = 0;
        beltU->visible[0] = FALSE;
        beltU->visible[1] = TRUE;
    } else {
        switch (category) {
            case ITEM_CATEGORY_THREAD:
                Tusin_LoadItemData(&itemData, index);
                graphicIndex = itemData.unk_00;
                break;
            case ITEM_CATEGORY_FOOD:
                Tusin_LoadFoodData(&foodData, index);
                graphicIndex = foodData.unk_00;
                break;
            case ITEM_CATEGORY_SWAG:
                Tusin_LoadTreasureData(&treasureData, index);
                graphicIndex = treasureData.unk_00;
                break;
        }
        beltU->visible[0] = TRUE;
        beltU->visible[1] = TRUE;
    }
#else
    // The icon is meant to be hidden when there is no gift, but the unconditional stores below undo it.
    if (index == 0xFFFF) {
        graphicIndex      = 1;
        beltU->visible[0] = TRUE;
        beltU->visible[1] = FALSE;
    } else {
        switch (category) {
            case ITEM_CATEGORY_THREAD:
                Tusin_LoadItemData(&itemData, index);
                graphicIndex = itemData.unk_00;
                break;
            case ITEM_CATEGORY_FOOD:
                Tusin_LoadFoodData(&foodData, index);
                graphicIndex = foodData.unk_00;
                break;
            case ITEM_CATEGORY_SWAG:
                Tusin_LoadTreasureData(&treasureData, index);
                graphicIndex = treasureData.unk_00;
                break;
        }
    }
    beltU->visible[0] = TRUE;
    beltU->visible[1] = TRUE;
#endif

    beltU->state     = 0;
    beltU->timer     = 2;
    beltU->row       = 0;
    beltU->posX      = 0x180000;
    beltU->speedX    = 0x19000;
    beltU->decelX    = -0x1000;
    beltU->posY      = 0xB8000;
    beltU->speedY    = 0x4000;
    beltU->unk_A4    = 0;
    beltU->unk_270   = 0;
    beltU->drawDelay = 2;

#ifdef REGION_USA
    anim.dataType   = args->dataType;
    anim.bits_0_1   = 0;
    anim.bits_10_11 = 0;
    anim.binIden    = &Tusin_BinIdentifiers[10];
    anim.unk_18     = 2;
    anim.packIndex  = args->index + 1;
    anim.animIndex  = 1;
    anim.unk_22     = 1;
    anim.posX -= 108;
    anim.posY -= 10;
    _Sprite_Load(&sprites[0], &anim);

    if (index != 0xFFFF) {
        Tusin_beltU_LoadPackedData(beltU, sprites, args, graphicIndex + 1);
    }

    anim.bits_0_1   = 0;
    anim.bits_10_11 = 1;
    anim.binIden    = &Tusin_BinIdentifiers[5];
    anim.unk_18     = 0;
    anim.packIndex  = 0;
    anim.animIndex  = 1;
    anim.unk_22     = 1;
    anim.posX       = beltU->posX >> 12;
    anim.posY       = beltU->posY >> 12;
    _Sprite_Load(&sprites[1], &anim);
#else
    anim.dataType   = args->dataType;
    anim.bits_0_1   = 0;
    anim.bits_10_11 = 1;
    anim.binIden    = &Tusin_BinIdentifiers[5];
    anim.unk_18     = 0;
    anim.packIndex  = 0;
    anim.animIndex  = 1;
    anim.unk_22     = 1;
    anim.posX       = beltU->posX >> 12;
    anim.posY       = beltU->posY >> 12;
    _Sprite_Load(&sprites[0], &anim);

    anim.bits_0_1   = 2;
    anim.bits_10_11 = 0;
    anim.binIden    = &Tusin_BinIdentifiers[8];
    anim.unk_18     = 2;
    anim.packIndex  = graphicIndex + 1;
    anim.animIndex  = 1;
    anim.unk_22     = 1;
    anim.posX -= 108;
    anim.posY -= 10;
    _Sprite_Load(&sprites[1], &anim);
#endif
}

static void Tusin_beltU_InitFonts(Tusin_beltU* beltU) {
    SysFont_Init(&beltU->fonts[0]);
    SysFont_Init(&beltU->fonts[1]);
#ifdef REGION_USA
    SysFont_SetSpacing(&beltU->fonts[0], TRUE, 0);
    SysFont_SetSpacing(&beltU->fonts[1], TRUE, 0);
#endif
    SysFont_SetPos(&beltU->fonts[0], 40, 5);
    SysFont_SetPos(&beltU->fonts[1], 40, 23);
}

static void Tusin_beltU_SetText(Tusin_beltU* beltU, u16 mode) {
    TusinObject* tusin = beltU->tusin;
    SysCode*     fmt;
    SysCode*     name;

    if (mode == 0) {
        if (tusin->lastRakedMoney != 0) {
            fmt  = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_MINGLE_RAN_INTO_FMT);
            name = SysFont_GetSysCodeBuf_from_DsCode(tusin->friendName, 10);
            SysFont_Format(beltU->texts[0], fmt, name);
            Mem_Free(&gDebugHeap, fmt);
            Mem_Free(&gDebugHeap, name);

            fmt = SysFont_GetMsgBuf(&beltU->fonts[1], SYSMSG_MINGLE_RAKED_IN_FMT);
            SysFont_Format(beltU->texts[1], fmt, tusin->lastRakedMoney);
            Mem_Free(&gDebugHeap, fmt);
        } else {
            fmt  = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_MINGLE_RAN_INTO_FMT);
            name = SysFont_GetSysCodeBuf_from_DsCode(tusin->friendName, 10);
            SysFont_Format(beltU->texts[0], fmt, name);
            Mem_Free(&gDebugHeap, fmt);
            Mem_Free(&gDebugHeap, name);
        }
    } else if (mode == 1) {
        fmt  = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_MINGLE_RAN_INTO_OTHER_FMT);
        name = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_MINGLE_CIVVIES);
        SysFont_Format(beltU->texts[0], fmt, name);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, name);
    } else {
        u16 kind = tusin->alienKind;

        fmt  = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_MINGLE_RAN_INTO_OTHER_FMT);
        name = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_MINGLE_ALIEN_NAMES_START + kind);
        SysFont_Format(beltU->texts[0], fmt, name);
        Mem_Free(&gDebugHeap, fmt);
        Mem_Free(&gDebugHeap, name);
    }
}

static void Tusin_beltU_DrawText(Tusin_beltU* beltU) {
    TusinObject* tusin = beltU->tusin;

    if (beltU->drawDelay == 0) {
        return;
    }

    beltU->drawDelay--;
    if (beltU->drawDelay != 0) {
        return;
    }

    SysFont_DrawToSprite(&beltU->fonts[0], beltU->texts[0], &beltU->sprites[BELT_SPRITE], TRUE);
    if (beltU->mode != 0) {
        return;
    }
    if (tusin->lastRakedMoney == 0) {
        return;
    }
    SysFont_DrawToSprite(&beltU->fonts[1], beltU->texts[1], &beltU->sprites[BELT_SPRITE], TRUE);
}

static s32 Tusin_beltU_Init(TaskPool* pool, Task* task, void* args) {
    Tusin_beltU*      beltU    = task->data;
    Tusin_beltU_Args* initArgs = args;

    beltU->tusin = initArgs->tusin;
    beltU->mode  = initArgs->mode;
    Tusin_beltU_Load(beltU, beltU->sprites, initArgs);
    Tusin_beltU_InitFonts(beltU);
    Tusin_beltU_SetText(beltU, initArgs->mode);
    return 1;
}

static s32 Tusin_beltU_Update(TaskPool* pool, Task* task, void* args) {
    Tusin_beltU* beltU = task->data;
    TusinObject* tusin = beltU->tusin;
    s32          targetY;
    u16          i;

    switch (beltU->state) {
        case 0: {
            if (beltU->posX < 0x80000) {
                beltU->posX += beltU->speedX;
            } else if (beltU->posX > 0x80000) {
                beltU->posX -= beltU->speedX;
            } else {
                beltU->posX = 0x80000;
            }

            if (beltU->speedX > 0) {
                beltU->speedX += beltU->decelX;
                if (beltU->speedX <= 0) {
                    beltU->speedX = 0;
                    beltU->posX   = 0x80000;
                    beltU->state  = 1;
                }
            }
        } break;

        case 1: {
            if (beltU->timer > 0) {
                beltU->timer--;
            } else {
                tusin->flags |= 4;
                beltU->state = 2;
            }
        } break;

        case 2: {
            if (tusin->flags & 2) {
                if (beltU->row >= 6) {
                    return 0;
                }
                beltU->row++;
                beltU->state = 3;
            }
        } break;

        case 3: {
            beltU->posY -= beltU->speedY;
            targetY = ((beltU->row * -36) << 12) + 0xB8000;
            if (beltU->posY <= targetY) {
                beltU->posY  = targetY;
                beltU->state = 2;
            }
        } break;
    }

    beltU->sprites[BELT_SPRITE].posX = beltU->posX >> 12;
    beltU->sprites[BELT_SPRITE].posY = beltU->posY >> 12;
    beltU->sprites[ICON_SPRITE].posX = beltU->sprites[BELT_SPRITE].posX - 108;
    beltU->sprites[ICON_SPRITE].posY = beltU->sprites[BELT_SPRITE].posY - 10;
    for (i = 0; i < 2; i++) {
        Sprite_Update(&beltU->sprites[i]);
    }
    return 1;
}

static s32 Tusin_beltU_Render(TaskPool* pool, Task* task, void* args) {
    Tusin_beltU* beltU = task->data;
    u16          i;

    for (i = 0; i < 2; i++) {
        if (beltU->visible[i]) {
            Sprite_RenderFrame(&beltU->sprites[i]);
        }
    }
    Tusin_beltU_DrawText(beltU);
    return 1;
}

static s32 Tusin_beltU_Destroy(TaskPool* pool, Task* task, void* args) {
    Tusin_beltU* beltU = task->data;
    u16          i;

    for (i = 0; i < 2; i++) {
        Sprite_Release(&beltU->sprites[i]);
    }
    SysFont_Destroy(&beltU->fonts[0]);
    SysFont_Destroy(&beltU->fonts[1]);
    return 1;
}

static s32 Tusin_beltU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Tusin_beltU_Init,
        .update     = Tusin_beltU_Update,
        .render     = Tusin_beltU_Render,
        .cleanup    = Tusin_beltU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

#ifdef REGION_USA
s32 Tusin_beltU_CreateTask(TaskPool* pool, s32 dataType, s32 kind, TusinObject* tusin, u16 index) {
#else
s32 Tusin_beltU_CreateTask(TaskPool* pool, s32 dataType, s32 kind, TusinObject* tusin) {
#endif
    Tusin_beltU_Args args;

    args.dataType = dataType;
    args.tusin    = tusin;
#ifdef REGION_USA
    args.index = index;
#endif
    if (kind == 0) {
        args.mode = 0;
    } else if (kind == 1) {
        args.mode = 1;
    } else {
        args.mode = 2;
    }

    return EasyTask_CreateTask(pool, &Tsk_Tusin_beltU, NULL, 0, NULL, &args);
}
