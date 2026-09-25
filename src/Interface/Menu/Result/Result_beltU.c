#include "Engine/Core/Memory.h"
#include "Engine/IO/Input.h"
#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/Result.h"
#include "Save.h"
#include "SndMgr.h"
#include "SndMgrSeIdx.h"
#include "Util/SysFont.h"

typedef struct {
    /* 0x000 */ s32           unk_00;
    /* 0x004 */ Sprite        sprite;
    /* 0x044 */ BOOL          visible;
    /* 0x048 */ ResultObject* owner;
    /* 0x04C */ s32           posX;
    /* 0x050 */ s32           posY;
    /* 0x054 */ s32           speedX;
    /* 0x058 */ s32           speedY;
    /* 0x05C */ s32           decelX;
    /* 0x060 */ s32           unk_60;
    /* 0x064 */ u16           state;
    /* 0x066 */ s16           timer;
    /* 0x068 */ s16           row;
    /* 0x06A */ u16           index;
    /* 0x06C */ u16           mode;
    /* 0x06E */ char          unk_6E[0x70 - 0x6E];
    /* 0x070 */ SysFont       fonts[2];
#ifdef REGION_USA
    /* 0x168 */ SysCode texts[2][100];
    /* 0x2F8 */ u16     drawDelay;
#else
    /* 0x168 */ SysCode texts[2][30];
    /* 0x1E0 */ u16     drawDelay;
#endif
} Result_beltU; // Size: 0x2FC (JP: 0x1E4)

typedef struct {
    /* 0x0 */ s32           dataType;
    /* 0x4 */ ResultObject* owner;
    /* 0x8 */ u16           mode;
    /* 0xA */ u16           index;
} Result_beltU_Args;

static SpriteFrameInfo* Result_beltU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              Result_beltU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_Result_beltU = {"Tsk_Result_beltU", Result_beltU_RunTask, sizeof(Result_beltU)};

static const SpriteAnimation Result_beltU_Anim = {
    .bits_0_1          = 1,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 1,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = Result_beltU_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &Result_BinIdentifiers[25],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 1,
    .unk_1E            = 0,
    .unk_20            = 4,
    .unk_22            = 4,
    .unk_24            = 0,
    .unk_26            = 2,
    .unk_28            = 3,
    .unk_2A            = 1,
};

static SpriteFrameInfo* Result_beltU_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackEarly(sprite, mode);
}

static void Result_beltU_Load(Result_beltU* beltU, Sprite* sprite, Result_beltU_Args* args) {
    SpriteAnimation anim = Result_beltU_Anim;

    beltU->visible   = TRUE;
    beltU->state     = 0;
    beltU->timer     = 2;
    beltU->row       = 0;
    beltU->mode      = args->mode;
    beltU->posX      = 0x180000;
    beltU->speedX    = 0x19000;
    beltU->decelX    = -0x1000;
    beltU->posY      = 0xB8000;
    beltU->speedY    = 0x2000;
    beltU->unk_60    = 0;
    beltU->index     = args->index;
    beltU->drawDelay = 2;

    anim.dataType = args->dataType;
    anim.unk_2A   = args->mode + 0x12;
    anim.unk_04   = beltU->posX >> 12;
    anim.unk_06   = beltU->posY >> 12;
    _Sprite_Load(sprite, &anim);
}

static void Result_beltU_InitFonts(Result_beltU* beltU) {
    SysFont_Init(&beltU->fonts[0]);
    SysFont_Init(&beltU->fonts[1]);
#ifdef REGION_USA
    SysFont_SetSpacing(&beltU->fonts[0], TRUE, 0);
    SysFont_SetSpacing(&beltU->fonts[1], TRUE, 0);
#endif
}

static void Result_beltU_SetPinText(Result_beltU* beltU) {
    ResultObject* owner = beltU->owner;
    u16           pinId = owner->dropPinIDs[beltU->index];
    u16           count = owner->dropQuantities[beltU->index];
    SysCode*      fmt;
    SysCode*      pinName;

    fmt     = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_RESULT_PIN_OBTAINED_FMT);
    pinName = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_PIN_NAMES_START + pinId);
    SysFont_Format(beltU->texts[0], fmt, pinName, count);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, pinName);

    SysFont_SetPos(&beltU->fonts[0], 29, 4);
    SysFont_SetHAlign(&beltU->fonts[0], 0, 216);
}

static void Result_beltU_SetFoodText(Result_beltU* beltU) {
    ResultObject* owner = beltU->owner;
    u16           partner;
    u16           foodId;
    u16           stat;
    u16           amount;
    SysCode*      fmt;
    SysCode*      partnerName;
    SysCode*      foodName;
    SysCode*      statName;

    if (beltU->index > 1) {
        return;
    }

    if (beltU->index == 0) {
        partner = 0;
    } else {
        partner = gSaveData.playerStats.activePartner + 1;
    }

    foodId = owner->foodItems[beltU->index];
    stat   = owner->foodStat[beltU->index];
    amount = owner->foodStatBonus[beltU->index];

    fmt         = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_RESULT_FOOD_DIGESTED_FMT);
    partnerName = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_PARTNER_NEKU + partner);
    foodName    = SysFont_GetMsgBuf(&beltU->fonts[0], SYSMSG_FOOD_NAMES_START + foodId);
    SysFont_Format(beltU->texts[0], fmt, partnerName, foodName);
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, foodName);

#ifdef REGION_USA
    fmt      = SysFont_GetMsgBuf(&beltU->fonts[1], SYSMSG_RESULT_STAT_UP_FMT);
    statName = SysFont_GetMsgBuf(&beltU->fonts[1], SYSMSG_STAT_ATTACK + stat);
    SysFont_Format(beltU->texts[1], fmt, statName, amount);
#else
    fmt      = SysFont_GetMsgBuf(&beltU->fonts[1], SYSMSG_RESULT_PARTNER_STAT_UP_FMT);
    statName = SysFont_GetMsgBuf(&beltU->fonts[1], SYSMSG_STAT_ATTACK + stat);
    SysFont_Format(beltU->texts[1], fmt, partnerName, statName, amount);
#endif
    Mem_Free(&gDebugHeap, fmt);
    Mem_Free(&gDebugHeap, statName);
    Mem_Free(&gDebugHeap, partnerName);

    SysFont_SetPos(&beltU->fonts[0], 36, 4);
    SysFont_SetPos(&beltU->fonts[1], 5, 22);
    SysFont_SetHAlign(&beltU->fonts[0], 0, 209);
    SysFont_SetHAlign(&beltU->fonts[1], 0, 240);
}

static void Result_beltU_SetBonusText(Result_beltU* beltU) {
    SysFont_SetMsg(&beltU->fonts[0], SYSMSG_RESULT_SPECIAL_BONUS_START + beltU->owner->specialBonuses[beltU->index]);
    SysFont_SetPos(&beltU->fonts[0], 51, 4);
    SysFont_SetHAlign(&beltU->fonts[0], 0, 194);
}

static void Result_beltU_DrawText(Result_beltU* beltU) {
    if (beltU->drawDelay == 0) {
        return;
    }

    beltU->drawDelay--;
    if (beltU->drawDelay != 0) {
        return;
    }

    switch (beltU->mode) {
        case 0: {
            SysFont_DrawToSprite(&beltU->fonts[0], beltU->texts[0], &beltU->sprite, TRUE);
        } break;

        case 1: {
            SysFont_DrawToSprite(&beltU->fonts[0], beltU->texts[0], &beltU->sprite, TRUE);
            SysFont_DrawToSprite(&beltU->fonts[1], beltU->texts[1], &beltU->sprite, TRUE);
        } break;

        case 2: {
            SysFont_DrawCurrentToSprite(&beltU->fonts[0], &beltU->sprite, TRUE);
        } break;
    }
}

static s32 Result_beltU_Init(TaskPool* pool, Task* task, void* args) {
    Result_beltU*      beltU     = task->data;
    Result_beltU_Args* beltUArgs = args;

    beltU->owner = beltUArgs->owner;
    Result_beltU_Load(beltU, &beltU->sprite, beltUArgs);
    Result_beltU_InitFonts(beltU);

    switch (beltU->mode) {
        case 0: {
            SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_BADGEGET);
            Result_beltU_SetPinText(beltU);
        } break;

        case 1: {
            SndMgr_StartPlayingSE(SEIDX_BTL_SYSTEM_FOOD);
            Result_beltU_SetFoodText(beltU);
        } break;

        case 2: {
            SndMgr_StartPlayingSE(SEIDX_BTL_RESULT_BADGEGET);
            Result_beltU_SetBonusText(beltU);
        } break;
    }
    return 1;
}

static s32 Result_beltU_Update(TaskPool* pool, Task* task, void* args) {
    Result_beltU* beltU = task->data;
    ResultObject* owner = beltU->owner;
    s32           targetY;

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

            if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
                beltU->posX  = 0x80000;
                beltU->state = 1;
            }
        } break;

        case 1: {
            if (beltU->timer > 0) {
                beltU->timer--;
            } else {
                owner->stageFlags |= 2;
                beltU->state = 2;
            }
        } break;

        case 2: {
            if (owner->stageFlags & 1) {
                if (owner->stageFlags & 0x20) {
                    beltU->row += 2;
                    beltU->state = 4;
                } else {
                    if (beltU->row >= 3) {
                        return 0;
                    }
                    beltU->row++;
                    beltU->state = 3;
                }
            }

            if (beltU->posY < 0x82000) {
                return 0;
            }
        } break;

        case 3: {
            beltU->posY -= beltU->speedY;
            targetY = ((beltU->row * -18) << 12) + 0xB8000;
            if (beltU->posY <= targetY) {
                beltU->posY  = targetY;
                beltU->state = 2;
            }

            if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
                beltU->posY  = targetY;
                beltU->state = 2;
            }
        } break;

        case 4: {
#ifdef REGION_USA
            beltU->posY -= beltU->speedY * 2;
#else
            beltU->posY -= beltU->speedY;
#endif
            targetY = ((beltU->row * -18) << 12) + 0xB8000;
            if (beltU->posY < 0x82000) {
                return 0;
            }

            if (beltU->posY <= targetY) {
                beltU->posY  = targetY;
                beltU->state = 2;
            }

            if (TouchInput_IsTouchActive() || (InputStatus.buttonState.currButtons & 1)) {
                beltU->posY  = targetY;
                beltU->state = 2;
            }
        } break;
    }

    beltU->sprite.posX = beltU->posX >> 12;
    beltU->sprite.posY = beltU->posY >> 12;
    Sprite_Update(&beltU->sprite);
    return 1;
}

static s32 Result_beltU_Render(TaskPool* pool, Task* task, void* args) {
    Result_beltU* beltU = task->data;

    if (beltU->visible != 0) {
        Sprite_RenderFrame(&beltU->sprite);
    }
    Result_beltU_DrawText(beltU);
    return 1;
}

static s32 Result_beltU_Destroy(TaskPool* pool, Task* task, void* args) {
    Result_beltU* beltU = task->data;

    Sprite_Release(&beltU->sprite);
    SysFont_Destroy(&beltU->fonts[0]);
    SysFont_Destroy(&beltU->fonts[1]);
    return 1;
}

static s32 Result_beltU_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = Result_beltU_Init,
        .update     = Result_beltU_Update,
        .render     = Result_beltU_Render,
        .cleanup    = Result_beltU_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 Result_beltU_CreateTask(TaskPool* pool, s32 dataType, u16 mode, u16 index, ResultObject* owner) {
    Result_beltU_Args args;

    args.dataType = dataType;
    args.mode     = mode;
    args.owner    = owner;
    args.index    = index;

    return EasyTask_CreateTask(pool, &Tsk_Result_beltU, NULL, 0, NULL, &args);
}
