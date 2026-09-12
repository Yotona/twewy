#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[27];
    /* 0x6C0 */ BOOL             shouldRender[27];
    /* 0x72C */ MenuEquipObject* owner;
} MenuEquip_numPtr; // Size: 0x730

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_numPtr_Args;

static SpriteFrameInfo* MenuEquip_numPtr_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode);
static s32              MenuEquip_numPtr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_numPtr = {"Tsk_MenuEquip_numPtr", MenuEquip_numPtr_RunTask, sizeof(MenuEquip_numPtr)};

static const SpriteAnimation MenuEquip_numPtr_Anim = {
    .bits_0_1   = 2,
    .dataType   = 0,
    .bit_6      = 0,
    .bits_7_9   = 5,
    .bits_10_11 = 0,
    .bits_12_13 = 1,
    .bits_14_15 = 0,
    .unk_02.raw = 0x400,
    .unk_04     = 0x50,
    .unk_06     = 0x50,
    .unk_08     = MenuEquip_numPtr_GetFrameInfo,
    .unk_0C     = 0,
    .unk_10     = 0,
    .binIden    = &data_ov043_020c9758[2],
    .unk_18     = 0,
    .packIndex  = 0,
    .unk_1C     = 4,
    .unk_1E     = 0,
    .unk_20     = 0xA,
    .unk_22     = 6,
    .unk_24     = 0,
    .unk_26     = 5,
    .unk_28     = 6,
    .unk_2A     = 1,
};

static void MenuEquip_numPtr_CalcDigits3(s16 value, s16* digits, BOOL* visible) {
    s16 rem;

    if (value >= 999) {
        value = 999;
    }
    if (value <= 0) {
        value *= -1;
    }

    digits[0] = value / 100;
    rem       = value % 100;
    digits[1] = rem / 10;
    digits[2] = rem % 10;

    visible[0] = TRUE;
    visible[1] = TRUE;
    visible[2] = TRUE;

    if (digits[0] == 0) {
        visible[0] = FALSE;
        if (digits[1] == 0) {
            visible[1] = FALSE;
        }
    }
}

static void MenuEquip_numPtr_CalcDigits4(s16 value, s16* digits, BOOL* visible) {
    s16 rem1000;
    s16 rem100;

    if (value >= 9999) {
        value = 9999;
    }
    if (value <= 0) {
        value *= -1;
    }

    digits[0] = value / 1000;
    rem1000   = value % 1000;
    digits[1] = rem1000 / 100;
    rem100    = rem1000 % 100;
    digits[2] = rem100 / 10;
    digits[3] = rem100 % 10;

    visible[0] = TRUE;
    visible[1] = TRUE;
    visible[2] = TRUE;
    visible[3] = TRUE;

    if (digits[0] == 0) {
        visible[0] = FALSE;
        if (digits[1] == 0) {
            visible[1] = FALSE;
        }
    }
}

static SpriteFrameInfo* MenuEquip_numPtr_GetFrameInfo(Sprite* sprite, s32 frameIndex, s32 mode) {
    SpriteFrameInfo* info = &data_0206b408;

    switch (mode) {
        case 1:
            info->unk_00 = 1;
            return info;

        case 2:
            info         = &data_0206b408;
            info->unk_04 = 0;
            info->unk_08 = 0;
            info->unk_0C = 0;
            info->unk_10 = -1;
            if (sprite->animData != NULL && sprite->frameDataTable != NULL && sprite->unk16 >= 0) {
                info->unk_04 = *((u16*)sprite->frameDataTable + (sprite->unk16 * 4 + 1));
                info->unk_08 =
                    (s32)((u16*)sprite->frameDataTable + *((u16*)((u8*)sprite->frameDataTable + (sprite->unk16 * 8))));
            }
            info->unk_10 = 0x398000;
            return info;

        default:
            return NULL;
    }
}

static void MenuEquip_numPtr_UpdateDigits(MenuEquip_numPtr* numPtr) {
    MenuEquipObject* owner = numPtr->owner;
    s16              digits[4];
    s16              signFrame;
    s32              frameBase;
    s16              i;

    MenuEquip_numPtr_CalcDigits4(owner->stats[owner->activePartner + 1].sync, digits, &numPtr->shouldRender[0]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i], digits[i] + 1, 6, 5);
    }
    numPtr->shouldRender[4] = TRUE;
    MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[4], digits[i] + 1, 6, 5);

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].braveryBonus, digits, &numPtr->shouldRender[6]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i + 6], digits[i] + 1, 6, 5);
        numPtr->shouldRender[i + 6] = FALSE;
    }

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].bravery, digits, &numPtr->shouldRender[10]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i + 10], digits[i] + 0x23, 6, 5);
    }

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].attack, digits, &numPtr->shouldRender[13]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i + 13], digits[i] + 1, 6, 5);
    }

    if (owner->stats[owner->activePartner + 1].attackBonus >= 0) {
        signFrame = 0x17;
        frameBase = 0xD;
    } else {
        signFrame = 0x22;
        frameBase = 0x18;
    }
    numPtr->shouldRender[16] = TRUE;
    MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[16], signFrame, 6, 5);
    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].attackBonus, digits, &numPtr->shouldRender[17]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i + 17], frameBase + digits[i], 6, 5);
    }

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].defense, digits, &numPtr->shouldRender[20]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i + 20], digits[i] + 1, 6, 5);
    }

    if (owner->stats[owner->activePartner + 1].defenseBonus >= 0) {
        signFrame = 0x17;
        frameBase = 0xD;
    } else {
        signFrame = 0x22;
        frameBase = 0x18;
    }
    numPtr->shouldRender[23] = TRUE;
    MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[23], signFrame, 6, 5);
    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].defenseBonus, digits, &numPtr->shouldRender[24]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numPtr->sprites[i + 24], frameBase + digits[i], 6, 5);
    }
}

// Nonmatching: regswap between frameBase and the x counter
static void MenuEquip_numPtr_Load(MenuEquip_numPtr* numPtr, Sprite* sprites, MenuEquip_numPtr_Args* args) {
    MenuEquipObject* owner = numPtr->owner;
    SpriteAnimation  anim  = MenuEquip_numPtr_Anim;
    s16              digits[4];
    s32              frameBase;
    s16              i;

    anim.dataType = args->dataType;

    MenuEquip_numPtr_CalcDigits4(owner->stats[owner->activePartner + 1].sync, digits, &numPtr->shouldRender[0]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0xA6;
        anim.unk_06 = 0x4C;
        _Sprite_Load(&sprites[i], &anim);
    }

    anim.unk_2A             = 0xB;
    anim.unk_04             = 0xB7;
    anim.unk_06             = 0x4C;
    numPtr->shouldRender[3] = TRUE;
    _Sprite_Load(&sprites[3], &anim);

    anim.unk_2A             = digits[3] + 1;
    anim.unk_04             = 0xB7;
    anim.unk_06             = 0x4C;
    numPtr->shouldRender[4] = TRUE;
    _Sprite_Load(&sprites[4], &anim);

    anim.unk_2A             = 0xC;
    anim.unk_04             = 0xB7;
    anim.unk_06             = 0x4C;
    numPtr->shouldRender[5] = TRUE;
    _Sprite_Load(&sprites[5], &anim);

    MenuEquip_numPtr_CalcDigits3(0, digits, &numPtr->shouldRender[6]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0xDC;
        anim.unk_06 = 0x5A;
        _Sprite_Load(&sprites[i + 6], &anim);
        numPtr->shouldRender[i + 6] = FALSE;
    }

    anim.unk_2A             = 0x2D;
    anim.unk_04             = 0xEF;
    anim.unk_06             = 0x5A;
    numPtr->shouldRender[9] = FALSE;
    _Sprite_Load(&sprites[9], &anim);

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].bravery, digits, &numPtr->shouldRender[10]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 0x23;
        anim.unk_04 = i * 5 + 0xEF;
        anim.unk_06 = 0x5A;
        _Sprite_Load(&sprites[i + 10], &anim);
    }

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].attack, digits, &numPtr->shouldRender[13]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0xDC;
        anim.unk_06 = 0x62;
        _Sprite_Load(&sprites[i + 13], &anim);
    }

    if (owner->stats[owner->activePartner + 1].attackBonus >= 0) {
        anim.unk_2A = 0x17;
        frameBase   = 0xD;
    } else {
        anim.unk_2A = 0x22;
        frameBase   = 0x18;
    }
    anim.unk_04              = 0xEF;
    anim.unk_06              = 0x62;
    numPtr->shouldRender[16] = TRUE;
    _Sprite_Load(&sprites[16], &anim);
    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].attackBonus, digits, &numPtr->shouldRender[17]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = frameBase + digits[i];
        anim.unk_04 = i * 5 + 0xEF;
        anim.unk_06 = 0x62;
        _Sprite_Load(&sprites[i + 17], &anim);
    }

    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].defense, digits, &numPtr->shouldRender[20]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0xDC;
        anim.unk_06 = 0x6A;
        _Sprite_Load(&sprites[i + 20], &anim);
    }

    if (owner->stats[owner->activePartner + 1].defenseBonus >= 0) {
        anim.unk_2A = 0x17;
        frameBase   = 0xD;
    } else {
        anim.unk_2A = 0x22;
        frameBase   = 0x18;
    }
    anim.unk_04              = 0xEF;
    anim.unk_06              = 0x6A;
    numPtr->shouldRender[23] = TRUE;
    _Sprite_Load(&sprites[23], &anim);
    MenuEquip_numPtr_CalcDigits3(owner->stats[owner->activePartner + 1].defenseBonus, digits, &numPtr->shouldRender[24]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = frameBase + digits[i];
        anim.unk_04 = i * 5 + 0xEF;
        anim.unk_06 = 0x6A;
        _Sprite_Load(&sprites[i + 24], &anim);
    }
}

static s32 MenuEquip_numPtr_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numPtr*      numPtr     = task->data;
    MenuEquip_numPtr_Args* numPtrArgs = args;

    numPtr->owner = numPtrArgs->owner;
    MenuEquip_numPtr_Load(numPtr, numPtr->sprites, numPtrArgs);
    return 1;
}

static s32 MenuEquip_numPtr_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numPtr* numPtr = task->data;
    MenuEquipObject*  owner  = numPtr->owner;

    if (owner->dirtyFlags & 0x100) {
        MenuEquip_numPtr_UpdateDigits(numPtr);
        owner->dirtyFlags &= ~0x100;
    }

    for (s32 i = 0; i < 27; i++) {
        Sprite_Update(&numPtr->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_numPtr_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numPtr* numPtr = task->data;

    for (s32 i = 0; i < 27; i++) {
        if (numPtr->shouldRender[i] != 0) {
            Sprite_RenderFrame(&numPtr->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_numPtr_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numPtr* numPtr = task->data;

    for (s32 i = 0; i < 27; i++) {
        Sprite_Release(&numPtr->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_numPtr_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_numPtr_Init,
        .update     = MenuEquip_numPtr_Update,
        .render     = MenuEquip_numPtr_Render,
        .cleanup    = MenuEquip_numPtr_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_numPtr_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_numPtr_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_numPtr, NULL, 0, NULL, &args);
}
