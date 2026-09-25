#include "Interface/Menu/MenuEquip.h"

typedef struct {
    /* 0x000 */ Sprite           sprites[30];
    /* 0x780 */ BOOL             shouldRender[30];
    /* 0x7F8 */ MenuEquipObject* owner;
} MenuEquip_numNek; // Size: 0x7FC

typedef struct {
    /* 0x0 */ s32              dataType;
    /* 0x4 */ MenuEquipObject* owner;
} MenuEquip_numNek_Args;

static SpriteFrameInfo* MenuEquip_numNek_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode);
static s32              MenuEquip_numNek_RunTask(TaskPool* pool, Task* task, void* args, s32 stage);

static const TaskHandle Tsk_MenuEquip_numNek = {"Tsk_MenuEquip_numNek", MenuEquip_numNek_RunTask, sizeof(MenuEquip_numNek)};

static const SpriteAnimation MenuEquip_numNek_Anim = {
    .bits_0_1          = 2,
    .dataType          = 0,
    .bit_6             = 0,
    .bits_7_9          = 5,
    .bits_10_11        = 0,
    .bits_12_13        = 1,
    .bits_14_15        = 0,
    .unk_02.raw        = 0x400,
    .unk_04            = 0x50,
    .unk_06            = 0x50,
    .frameInfoCallback = MenuEquip_numNek_GetFrameInfo,
    .callbackArg       = 0,
    .owner             = NULL,
    .binIden           = &data_ov043_020c9758[2],
    .unk_18            = 0,
    .packIndex         = 0,
    .unk_1C            = 4,
    .unk_1E            = 0,
    .unk_20            = 0xA,
    .unk_22            = 6,
    .unk_24            = 0,
    .unk_26            = 5,
    .unk_28            = 6,
    .unk_2A            = 1,
};

static void MenuEquip_numNek_CalcDigits3(s16 value, s16* digits, BOOL* visible) {
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

static void MenuEquip_numNek_CalcDigits4(s16 value, s16* digits, BOOL* visible) {
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
            if (digits[2] == 0) {
                visible[2] = FALSE;
            }
        }
    }
}

static SpriteFrameInfo* MenuEquip_numNek_GetFrameInfo(Sprite* sprite, s32 arg, s32 mode) {
    Sprite_FrameInfoCallbackSorted(sprite, mode, 0x3A2000);
}

static void MenuEquip_numNek_UpdateDigits(MenuEquip_numNek* numNek) {
    MenuEquipObject* owner = numNek->owner;
    s16              digits[4];
    s16              signFrame;
    s32              frameBase;
    s16              i;

    MenuEquip_numNek_CalcDigits4(owner->stats[0].health, digits, &numNek->shouldRender[0]);
    for (i = 0; i < 4; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i], digits[i] + 1, 6, 5);
    }

    if (owner->stats[0].healthBonus >= 0) {
        signFrame = 0x17;
        frameBase = 0xD;
    } else {
        signFrame = 0x22;
        frameBase = 0x18;
    }
    numNek->shouldRender[4] = TRUE;
    MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[4], signFrame, 6, 5);
    MenuEquip_numNek_CalcDigits4(owner->stats[0].healthBonus, digits, &numNek->shouldRender[5]);
    for (i = 0; i < 4; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 5], frameBase + digits[i], 6, 5);
    }

    MenuEquip_numNek_CalcDigits3(0, digits, &numNek->shouldRender[9]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 9], digits[i] + 1, 6, 5);
        numNek->shouldRender[i + 9] = FALSE;
    }

    MenuEquip_numNek_CalcDigits3(owner->stats[0].bravery, digits, &numNek->shouldRender[13]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 13], digits[i] + 0x23, 6, 5);
    }

    MenuEquip_numNek_CalcDigits3(owner->stats[0].attack, digits, &numNek->shouldRender[16]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 16], digits[i] + 1, 6, 5);
    }

    if (owner->stats[0].attackBonus >= 0) {
        signFrame = 0x17;
        frameBase = 0xD;
    } else {
        signFrame = 0x22;
        frameBase = 0x18;
    }
    numNek->shouldRender[19] = TRUE;
    MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[19], signFrame, 6, 5);
    MenuEquip_numNek_CalcDigits3(owner->stats[0].attackBonus, digits, &numNek->shouldRender[20]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 20], frameBase + digits[i], 6, 5);
    }

    MenuEquip_numNek_CalcDigits3(owner->stats[0].defense, digits, &numNek->shouldRender[23]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 23], digits[i] + 1, 6, 5);
    }

    if (owner->stats[0].defenseBonus >= 0) {
        signFrame = 0x17;
        frameBase = 0xD;
    } else {
        signFrame = 0x22;
        frameBase = 0x18;
    }
    numNek->shouldRender[26] = TRUE;
    MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[26], signFrame, 6, 5);
    MenuEquip_numNek_CalcDigits3(owner->stats[0].defenseBonus, digits, &numNek->shouldRender[27]);
    for (i = 0; i < 3; i++) {
        MenuEquip_SetSpriteFrameFromPack(&numNek->sprites[i + 27], frameBase + digits[i], 6, 5);
    }
}

// Nonmatching: regswap between frameBase and the x counter
static void MenuEquip_numNek_Load(MenuEquip_numNek* numNek, Sprite* sprites, MenuEquip_numNek_Args* args) {
    MenuEquipObject* owner = numNek->owner;
    SpriteAnimation  anim  = MenuEquip_numNek_Anim;
    s16              digits[4];
    s32              frameBase;
    s16              i;

    anim.dataType = args->dataType;

    MenuEquip_numNek_CalcDigits4(owner->stats[0].health, digits, &numNek->shouldRender[0]);
    for (i = 0; i < 4; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0x16;
        anim.unk_06 = 0x4C;
        _Sprite_Load(&sprites[i], &anim);
    }

    if (owner->stats[0].healthBonus >= 0) {
        anim.unk_2A = 0x17;
        frameBase   = 0xD;
    } else {
        anim.unk_2A = 0x22;
        frameBase   = 0x18;
    }
    anim.unk_04             = 0x2E;
    anim.unk_06             = 0x4C;
    numNek->shouldRender[4] = TRUE;
    _Sprite_Load(&sprites[4], &anim);
    MenuEquip_numNek_CalcDigits4(owner->stats[0].healthBonus, digits, &numNek->shouldRender[5]);
    for (i = 0; i < 4; i++) {
        anim.unk_2A = frameBase + digits[i];
        anim.unk_04 = i * 5 + 0x2E;
        anim.unk_06 = 0x4C;
        _Sprite_Load(&sprites[i + 5], &anim);
    }

    MenuEquip_numNek_CalcDigits3(owner->stats[0].braveryBonus, digits, &numNek->shouldRender[9]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0x5D;
        anim.unk_06 = 0x5A;
        _Sprite_Load(&sprites[i + 9], &anim);
        numNek->shouldRender[i + 9] = FALSE;
    }

    anim.unk_2A              = 0x2D;
    anim.unk_04              = 0x70;
    anim.unk_06              = 0x5A;
    numNek->shouldRender[12] = FALSE;
    _Sprite_Load(&sprites[12], &anim);

    MenuEquip_numNek_CalcDigits3(owner->stats[0].bravery, digits, &numNek->shouldRender[13]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 0x23;
        anim.unk_04 = i * 5 + 0x70;
        anim.unk_06 = 0x5A;
        _Sprite_Load(&sprites[i + 13], &anim);
    }

    MenuEquip_numNek_CalcDigits3(owner->stats[0].attack, digits, &numNek->shouldRender[16]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0x5D;
        anim.unk_06 = 0x62;
        _Sprite_Load(&sprites[i + 16], &anim);
    }

    if (owner->stats[0].attackBonus >= 0) {
        anim.unk_2A = 0x17;
        frameBase   = 0xD;
    } else {
        anim.unk_2A = 0x22;
        frameBase   = 0x18;
    }
    anim.unk_04              = 0x70;
    anim.unk_06              = 0x62;
    numNek->shouldRender[19] = TRUE;
    _Sprite_Load(&sprites[19], &anim);
    MenuEquip_numNek_CalcDigits3(owner->stats[0].attackBonus, digits, &numNek->shouldRender[20]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = frameBase + digits[i];
        anim.unk_04 = i * 5 + 0x70;
        anim.unk_06 = 0x62;
        _Sprite_Load(&sprites[i + 20], &anim);
    }

    MenuEquip_numNek_CalcDigits3(owner->stats[0].defense, digits, &numNek->shouldRender[23]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = digits[i] + 1;
        anim.unk_04 = i * 5 + 0x5D;
        anim.unk_06 = 0x6A;
        _Sprite_Load(&sprites[i + 23], &anim);
    }

    if (owner->stats[0].defenseBonus >= 0) {
        anim.unk_2A = 0x17;
        frameBase   = 0xD;
    } else {
        anim.unk_2A = 0x22;
        frameBase   = 0x18;
    }
    anim.unk_04              = 0x70;
    anim.unk_06              = 0x6A;
    numNek->shouldRender[26] = TRUE;
    _Sprite_Load(&sprites[26], &anim);
    MenuEquip_numNek_CalcDigits3(owner->stats[0].defenseBonus, digits, &numNek->shouldRender[27]);
    for (i = 0; i < 3; i++) {
        anim.unk_2A = frameBase + digits[i];
        anim.unk_04 = i * 5 + 0x70;
        anim.unk_06 = 0x6A;
        _Sprite_Load(&sprites[i + 27], &anim);
    }
}

static s32 MenuEquip_numNek_Init(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numNek*      numNek     = task->data;
    MenuEquip_numNek_Args* numNekArgs = args;

    numNek->owner = numNekArgs->owner;
    MenuEquip_numNek_Load(numNek, numNek->sprites, numNekArgs);
    return 1;
}

static s32 MenuEquip_numNek_Update(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numNek* numNek = task->data;
    MenuEquipObject*  owner  = numNek->owner;

    if (owner->dirtyFlags & 0x80) {
        MenuEquip_numNek_UpdateDigits(numNek);
        owner->dirtyFlags &= ~0x80;
    }

    for (s32 i = 0; i < 30; i++) {
        Sprite_Update(&numNek->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_numNek_Render(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numNek* numNek = task->data;

    for (s32 i = 0; i < 30; i++) {
        if (numNek->shouldRender[i] != 0) {
            Sprite_RenderFrame(&numNek->sprites[i]);
        }
    }
    return 1;
}

static s32 MenuEquip_numNek_Destroy(TaskPool* pool, Task* task, void* args) {
    MenuEquip_numNek* numNek = task->data;

    for (s32 i = 0; i < 30; i++) {
        Sprite_Release(&numNek->sprites[i]);
    }
    return 1;
}

static s32 MenuEquip_numNek_RunTask(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = {
        .initialize = MenuEquip_numNek_Init,
        .update     = MenuEquip_numNek_Update,
        .render     = MenuEquip_numNek_Render,
        .cleanup    = MenuEquip_numNek_Destroy,
    };
    return stages.iter[stage](pool, task, args);
}

s32 MenuEquip_numNek_CreateTask(TaskPool* pool, s32 dataType, MenuEquipObject* owner) {
    MenuEquip_numNek_Args args;

    args.dataType = dataType;
    args.owner    = owner;

    return EasyTask_CreateTask(pool, &Tsk_MenuEquip_numNek, NULL, 0, NULL, &args);
}
