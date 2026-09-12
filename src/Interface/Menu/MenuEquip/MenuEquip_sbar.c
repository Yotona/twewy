#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuEquip.h"
#include "SndMgr.h"
#include <nitro/fx/fx_division.h>

typedef struct {
    /* 0x00 */ Sprite           sprites[3];
    /* 0xC0 */ char             unk_C0[0xC4 - 0xC0];
    /* 0xC4 */ MenuEquipObject* owner;
    /* 0xC8 */ u16              lastScroll;
} MenuEquip_sbar; // Size: 0xCC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_sbar_Args;

static SpriteFrameInfo* MenuEquip_sbar_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_sbar = {"Tsk_MenuEquip_sbar", MenuEquip_sbar_RunTask, sizeof(MenuEquip_sbar)};

static const SpriteAnimation MenuEquip_sbar_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuEquip_sbar_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[2],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 1,
    .unk_1E     = 0,
    .unk_20     = 0xA,
    .unk_22     = 6,
    .unk_24     = 0,
    .unk_26     = 2,
    .unk_28     = 3,
    .unk_2A     = 1,
};

// Nonmatching: min() select direction and register allocation
static u16 MenuEquip_sbar_PosYToScroll(MenuEquipObject* owner, u16 posY) {
    u32 row;
    s32 scaledRow;

    if (posY < 0x8B) {
        posY = 0x8B;
    }

    scaledRow = FX_Divide((posY - 0x8B) << 12, 0x25000 / (owner->scrollBarRange[owner->currentTab] + 1)) * 16;
    row       = (owner->maxScrollRow[owner->currentTab] < (u32)scaledRow >> 16) ? owner->maxScrollRow[owner->currentTab]
                                                                                : (u32)scaledRow >> 16;
    return row * 8;
}

static u16 MenuEquip_sbar_ScrollToPosY(MenuEquipObject* owner, u16 scroll) {
    u16 posY = (u16)(scroll / 8) * 0x25 / owner->scrollBarRange[owner->currentTab] + 0x8B;

    if (posY > 0xB0) {
        posY = 0xB0;
    }
    return posY;
}

static SpriteFrameInfo* MenuEquip_sbar_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = NULL;

    switch (mode) {
        case 1: {
            data_0206b408.unk_00 = 1;
            return &data_0206b408;
        } break;

        case 2: {
            SpriteFrameInfo* temp = &data_0206b408;

            temp->unk_04 = 0;
            temp->unk_08 = 0;
            temp->unk_0C = 0;
            temp->unk_10 = -1;

            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                temp->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                temp->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }

            info = temp;
        } break;
    }

    return info;
}

static void MenuEquip_sbar_Load(Sprite* sprites, MenuEquip_sbar_Args* args) {
    SpriteAnimation anim = MenuEquip_sbar_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x13;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0x8B;
    _Sprite_Load(&sprites[0], &anim);

    anim.unk_2A = 0x11;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0x78;
    _Sprite_Load(&sprites[1], &anim);

    anim.unk_2A = 0x12;
    anim.unk_04 = 0xF8;
    anim.unk_06 = 0xB7;
    _Sprite_Load(&sprites[2], &anim);
}

static s32 MenuEquip_sbar_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar_Args* sbarArgs = args;
    MenuEquipObject*     owner    = sbarArgs->owner;

    MenuEquip_sbar* sbar = task->data;

    sbar->owner      = owner;
    sbar->lastScroll = owner->listScroll;
    MenuEquip_sbar_Load(sbar->sprites, sbarArgs);
    return 1;
}

static s32 MenuEquip_sbar_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar*  sbar  = task->data;
    MenuEquipObject* owner = sbar->owner;
    TouchCoord       coord;

    if (owner->helpOpen != 0) {
        return 1;
    }
    if (owner->dirtyFlags & 1) {
        return 1;
    }
    if (owner->fitsOnePage[owner->currentTab] == 1) {
        return 1;
    }

    if (owner->dirtyFlags & 4) {
        TouchInput_GetCoord(&coord);
        owner->listScroll = MenuEquip_sbar_PosYToScroll(owner, coord.y + 7);
        owner->dirtyFlags &= ~4;
        owner->dirtyFlags |= 8;
        sbar->sprites[0].posY = MenuEquip_sbar_ScrollToPosY(owner, owner->listScroll);
    }

    if (owner->dirtyFlags & 8) {
        s16 targetY;
        s16 step;

        TouchInput_GetCoord(&coord);
        targetY = coord.y + 7;
        step    = (targetY - sbar->sprites[0].posY) >> 2;
        if (step != 0) {
            sbar->sprites[0].posY += step;
        } else {
            sbar->sprites[0].posY = targetY;
        }

        if (sbar->sprites[0].posY < 0x8B) {
            sbar->sprites[0].posY = 0x8B;
        } else if (sbar->sprites[0].posY > 0xB0) {
            sbar->sprites[0].posY = 0xB0;
        }

        owner->listScroll = MenuEquip_sbar_PosYToScroll(owner, sbar->sprites[0].posY);
        if (TouchInput_WasTouchReleased()) {
            owner->dirtyFlags &= ~8;
        }

        if (owner->cursorSlot >= 10) {
            s32 cursorOffset = owner->cursorListIndex - owner->listScroll;

            if (cursorOffset < 0) {
                owner->cursorSlot = owner->cursorListIndex % 8 + 10;
            } else if (cursorOffset < 16) {
                owner->cursorSlot = cursorOffset + 10;
            } else {
                owner->cursorSlot = cursorOffset % 8 + 18;
            }
        }
    } else {
        if (TouchInput_WasTouchPressed()) {
            TouchInput_GetCoord(&coord);
            if (MenuEquip_IsPointOnScrollBar(coord.x, coord.y) == 1) {
                if (MenuEquip_IsPointOnScrollKnob(coord.x, coord.y, sbar->sprites[0].posX, sbar->sprites[0].posY) == 1) {
                    owner->dirtyFlags |= 8;
                } else {
                    owner->dirtyFlags |= 4;
                }
            } else {
                s32 arrow = MenuEquip_GetScrollArrowAtPoint(coord.x, coord.y);

                if (arrow == 0) {
                    if (owner->listScroll >= 8) {
                        owner->listScroll -= 8;
                        if (owner->cursorSlot >= 10 && owner->cursorSlot < 18) {
                            owner->cursorSlot += 8;
                        }
                    }
                } else if (arrow == 1) {
                    if (owner->listScroll <= owner->maxScrollRow[owner->currentTab] * 8 - 8) {
                        owner->listScroll += 8;
                        if (owner->cursorSlot >= 18 && owner->cursorSlot < 26) {
                            owner->cursorSlot -= 8;
                        }
                    }
                }
            }
        }
        sbar->sprites[0].posY = MenuEquip_sbar_ScrollToPosY(owner, owner->listScroll);
    }

    if (sbar->lastScroll != owner->listScroll) {
        SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_SCROLL);
    }
    sbar->lastScroll = owner->listScroll;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Update(&sbar->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_sbar_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar*  sbar  = task->data;
    MenuEquipObject* owner = sbar->owner;

    if (owner->fitsOnePage[owner->currentTab] == 0) {
        for (s32 i = 0; i < 3; i++) {
            Sprite_RenderFrame(&sbar->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_sbar_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_sbar* sbar = task->data;

    for (s32 i = 0; i < 3; i++) {
        Sprite_Release(&sbar->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_sbar_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_sbar_Init,
        .update     = MenuEquip_sbar_Update,
        .render     = MenuEquip_sbar_Render,
        .cleanup    = MenuEquip_sbar_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_sbar_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_sbar_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_sbar, NULL, 0, NULL, &args);
}
