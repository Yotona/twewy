#include "Engine/IO/TouchInput.h"
#include "Interface/Menu/MenuEquip.h"
#include "SndMgr.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[5];
    /* 0x140 */ BOOL             shouldRender[5];
    /* 0x154 */ MenuEquipObject* owner;
} MenuEquip_partner; // Size: 0x158

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_partner_Args;

static SpriteFrameInfo* MenuEquip_partner_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_partner = {"Tsk_MenuEquip_partner", MenuEquip_partner_RunTask,
                                                 sizeof(MenuEquip_partner)};

static const SpriteAnimation MenuEquip_partner_Anim = {
    .bits_0_1   = 0,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0xC00,
    .unk_04     = -0xD,
    .unk_06     = 0xC,
    .unk_08     = MenuEquip_partner_GetFrameInfo,
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

static SpriteFrameInfo* MenuEquip_partner_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
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

static void MenuEquip_partner_Load(MenuEquip_partner* partner, Sprite* sprites, MenuEquip_partner_Args* args) {
    MenuEquipObject* owner = partner->owner;
    SpriteAnimation  anim  = MenuEquip_partner_Anim;

    anim.dataType = args->dataType;

    anim.unk_2A = 0x18;
    anim.unk_04 = 0x51;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[0], &anim);

    for (s16 i = 0; i < 3; i++) {
        anim.unk_2A = i + 0x15;
        anim.unk_04 = i * 0x12 + 0x85;
        anim.unk_06 = 0xC;
        _Sprite_Load(&sprites[i + 1], &anim);
    }

    anim.unk_2A = 0x14;
    anim.unk_04 = owner->activePartner * 0x12 + 0x85;
    anim.unk_06 = 0xC;
    _Sprite_Load(&sprites[4], &anim);
}

static s32 MenuEquip_partner_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partner*      partner     = task->data;
    MenuEquip_partner_Args* partnerArgs = args;

    partner->owner = partnerArgs->owner;
    for (u16 i = 0; i < 5; i++) {
        partner->shouldRender[i] = TRUE;
    }
    MenuEquip_partner_Load(partner, partner->sprites, partnerArgs);
    return 1;
}

static s32 MenuEquip_partner_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partner* partner = task->data;
    MenuEquipObject*   owner   = partner->owner;
    TouchCoord         coord;

    if (owner->helpOpen != 0) {
        partner->shouldRender[4] = FALSE;
        return 1;
    }

    partner->shouldRender[4] = TRUE;

    if (owner->dirtyFlags & 0x10) {
        owner->dirtyFlags &= ~0x10;
    }

    if (TouchInput_WasTouchPressed()) {
        TouchInput_GetCoord(&coord);

        s16 partnerIndex = MenuEquip_GetPartnerTabAtPoint(coord.x, coord.y);
        if (partnerIndex != -1 && owner->activePartner != partnerIndex) {
            SndMgr_StartPlayingSE(SEIDX_MENU_MSYSTEM_CURSOR);
            partner->sprites[4].posX = partnerIndex * 0x12 + 0x85;
            owner->activePartner     = partnerIndex;
            owner->dirtyFlags |= 0x10;
            MenuEquip_UpdateStatBonuses(owner);
            MenuEquip_SwitchPartnerEquipment(owner, partnerIndex);
            owner->dirtyFlags |= 0x180;
#ifdef REGION_USA
            MenuEquip_RefreshCursorInfo();
#endif
            MenuEquip_ReleaseBgResource(&owner->resources[7], 0);
            MenuEquip_LoadBgResourceIndexed(&owner->resources[7], 0, 3, 0, 0, 0xE,
                                            owner->activePartner + owner->partnerArtSet * 3, owner->activePartner);
        }
    }

    for (s32 i = 0; i < 5; i++) {
        Sprite_Update(&partner->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_partner_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        if (partner->shouldRender[i] != 0) {
            Sprite_RenderFrame(&partner->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_partner_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_partner* partner = task->data;

    for (s32 i = 0; i < 5; i++) {
        Sprite_Release(&partner->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_partner_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_partner_Init,
        .update     = MenuEquip_partner_Update,
        .render     = MenuEquip_partner_Render,
        .cleanup    = MenuEquip_partner_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_partner_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_partner_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_partner, NULL, 0, NULL, &args);
}
