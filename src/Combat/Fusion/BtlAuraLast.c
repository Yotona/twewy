#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatSprite.h"
#include "Combat/Fusion/BtlFusion.h"
#include "Engine/EasyTask.h"
#include "Engine/Math/Random.h"
#include "SpriteMgr.h"

/// Projection/screen context owned by overlay 3.
typedef struct AuraTarget {
    /* 0x00 */ u8  unk_00[0x28];
    /* 0x28 */ s32 unk_28;
    /* 0x2C */ s32 unk_2C;
    /* 0x30 */ s32 unk_30;
    /* 0x34 */ s16 unk_34;
    /* 0x36 */ s16 unk_36;
} AuraTarget;

extern BinIdentifier data_ov003_020d7800;
extern void*         func_ov003_0208495c(void* list);
extern void*         func_ov003_02084984(void* node);
extern void          func_ov003_020849ac(void* list);
extern void          func_ov003_020849bc(void* list, void* node, void* value);
extern void          func_ov003_020849dc(void* list, void* node);
extern void          func_ov003_02087f28(s32, s32);
extern void          func_ov003_02087f64(s32, s32);
extern void          func_ov003_020974f8(s32, s32);
extern void          func_ov003_0208cbc8(s16*);
extern void          func_020265d4(void*, s32, u16);
extern s32           func_ov003_0208b690(u16);

s32  func_ov007_020e7c98(TaskPool*, Task*, void*);
s32  func_ov007_020e7d48(TaskPool*, Task*, void*);
s32  func_ov007_020e7da4(TaskPool*, Task*, void*);
s32  func_ov007_020e7eb0(TaskPool*, Task*, void*);
s32  func_ov007_020e7ee0(TaskPool*, Task*, void*, s32);
void func_ov007_020e75e4(BtlAuraLast*, void (*)(BtlAuraLast*));
void func_ov007_020e75fc(s32, CombatSprite*, u16, u16);
void func_ov007_020e76dc(BtlAuraLast*, s32);
void func_ov007_020e769c(s16*);
void func_ov007_020e78e4(BtlAuraLast*, BtlAuraSprite*);
void func_ov007_020e791c(BtlAuraLast*);
void func_ov007_020e7970(BtlAuraLast*);
void func_ov007_020e79d8(BtlAuraLast*);
void func_ov007_020e7a54(BtlAuraLast*);
void func_ov007_020e7a8c(BtlAuraLast*);
void func_ov007_020e7b54(BtlAuraLast*);
void func_ov007_020e764c(void);

const TaskHandle Tsk_BtlAuraLast = {"Tsk_BtlAuraLast", func_ov007_020e7ee0, 0x810};

static const TaskStages data_ov007_020e7f70 = {
    .initialize = func_ov007_020e7c98,
    .update     = func_ov007_020e7d48,
    .render     = func_ov007_020e7da4,
    .cleanup    = func_ov007_020e7eb0,
};

static const SpriteAnimEntry data_ov007_020e7f80[2] = {
    {0x10, 0x12, 0x11, 0},
    {0x10, 0x12, 0x11, 2},
};

void func_ov007_020e75e4(BtlAuraLast* data, void (*callback)(BtlAuraLast*)) {
    data->unk_7F4 = callback;
    data->unk_7FA = 0;
    data->unk_7F8 = 0;
}

void func_ov007_020e75fc(s32 arg0, CombatSprite* cSprite, u16 arg2, u16 arg3) {
    CombatSprite_LoadFromTable(arg0, cSprite, &data_ov003_020d7800, data_ov007_020e7f80, 0, arg3, 0x40);
    CombatSprite_SetAnimFromTable(cSprite, arg2, 0);
}

void func_ov007_020e764c(void) {
    if (data_ov003_020e71b8->unk3D8EE < 0x1F4) {
        func_ov003_02087f28(0x365, ((AuraTarget*)data_ov003_020e71b8->unk3D898)->unk_28);
    } else {
        func_ov003_02087f28(0x366, ((AuraTarget*)data_ov003_020e71b8->unk3D898)->unk_28);
    }
}

void func_ov007_020e769c(s16* out) {
    *out = (s8)data_ov003_020e71b8->unk3D8ED * 0x32 + 0xC8;
    if (*out > 0x1F4) {
        *out = 0x1F4;
    }
    func_ov003_0208cbc8(out);
}

void func_ov007_020e76dc(BtlAuraLast* data, s32 arg) {
    s32 mode = arg;

    if (data->unk_7F0 > 0) {
        data->unk_7F0--;
        return;
    }

    s32            flag;
    BtlAuraSprite* sprite;
    s16            posX;
    s16            posY;
    s16            animIndex;
    u16            size;
    s32            posXBase;

    sprite = func_ov003_0208495c(&data->unk_120);
    if (sprite == NULL) {
        return;
    }

    if (mode != 0 && mode == 1) {
        animIndex = (s16)RNG_Next(5);
        if (data->unk_808 < 0xC0000) {
            posXBase = data->unk_804;
            flag     = 1;
            size     = 0x40;
            posX     = (s16)((RNG_Next(0x11) - 8) + (posXBase << 12));
            posY     = (s16)(data->unk_808 << 12);
        } else {
            posXBase = data->unk_804;
            flag     = 0;
            size     = 0x3E;
            posX     = (s16)((RNG_Next(0x11) - 8) + (posXBase << 12));
            posY     = (s16)((data->unk_808 << 12) - 0xC0);
        }
        data->unk_7F0 = 2;
    } else {
        animIndex     = (s16)(RNG_Next(2) + 5);
        flag          = 0;
        size          = 0x3E;
        posX          = (s16)(((AuraTarget*)data_ov003_020e71b8->unk3D898)->unk_34 + (RNG_Next(0x19) - 0xC));
        posY          = (s16)(((AuraTarget*)data_ov003_020e71b8->unk3D898)->unk_36 + (RNG_Next(0x31) - 0x30));
        data->unk_7F0 = 4;
    }

    s32 flip;
    if (flag == 0) {
        flip = 0x80000000 - ((AuraTarget*)data_ov003_020e71b8->unk3D898)->unk_2C;
    } else {
        flip = 0;
    }

    CombatSprite_LoadDirect(flag, &sprite->sprite, &data_ov003_020d7800, 0x13, 0x15, 0x14, size, (u16)animIndex);
    CombatSprite_SetAnim(&sprite->sprite, (u16)animIndex, 1);
    CombatSprite_SetPosition(&sprite->sprite, posX, posY);
    func_ov003_02082730(&sprite->sprite, flip);
    func_ov003_020849dc(&data->unk_120, sprite->node);
    func_ov003_020849bc(&data->unk_128, sprite->node, sprite);
}

void func_ov007_020e78e4(BtlAuraLast* data, BtlAuraSprite* sprite) {
    if (sprite == NULL) {
        return;
    }
    CombatSprite_Release(&sprite->sprite);
    func_ov003_020849dc(&data->unk_128, sprite->node);
    func_ov003_020849bc(&data->unk_120, sprite->node, sprite);
}

void func_ov007_020e791c(BtlAuraLast* data) {
    func_ov003_020849ac(&data->unk_120);
    func_ov003_020849ac(&data->unk_128);
    data->unk_7F0 = 0;

    s32            i;
    BtlAuraSprite* s = data->sprites;
    for (i = 0; i < 16; i++, s++) {
        CombatSprite_Init(&s->sprite);
        func_ov003_020849bc(&data->unk_120, s->node, s);
    }
}

void func_ov007_020e7970(BtlAuraLast* data) {
    BtlAuraSprite* sprite = func_ov003_0208495c(&data->unk_128);
    if (sprite == NULL) {
        return;
    }

    do {
        if (SpriteMgr_IsAnimationFinished(&sprite->sprite.sprite)) {
            BtlAuraSprite* next = func_ov003_02084984(sprite->node);
            func_ov007_020e78e4(data, sprite);
            sprite = next;
        } else {
            CombatSprite_Update(&sprite->sprite);
            sprite = func_ov003_02084984(sprite->node);
        }
    } while (sprite != NULL);
}

void func_ov007_020e79d8(BtlAuraLast* data) {
    BtlAuraSprite* sprite = func_ov003_0208495c(&data->unk_128);
    if (sprite == NULL) {
        return;
    }

    do {
        if ((sprite->sprite.sprite.bits_0_1) == 1) {
            func_ov003_02082730(&sprite->sprite, 0);
        } else {
            AuraTarget* target = (AuraTarget*)data_ov003_020e71b8->unk3D898;
            func_ov003_02082730(&sprite->sprite, 0x80000000 - target->unk_2C);
        }
        CombatSprite_Render(&sprite->sprite);
        sprite = func_ov003_02084984(sprite->node);
    } while (sprite != NULL);
}

void func_ov007_020e7a54(BtlAuraLast* data) {
    for (s32 i = 0; i < 16; i++) {
        if (Sprite_HasAnimation(&data->sprites[i].sprite.sprite)) {
            CombatSprite_Release(&data->sprites[i].sprite);
        }
    }
}

void func_ov007_020e7a8c(BtlAuraLast* data) {
    if (data_ov003_020e71b8->unk3D8EC == 1) {
        return;
    }
    if (data_ov003_020e71b8->unk3D8EC == 2) {
        data_ov003_020e71b8->unk3D8EC = 0;
        data->unk_804                 = ((CombatActor*)data_ov003_020e71b8->unk3D89C)->screenX << 12;
        data->unk_808                 = (((CombatActor*)data_ov003_020e71b8->unk3D89C)->screenY - 0x20) << 12;
        func_ov003_02087f64(0x21, data->unk_804);
        data_ov003_020e71b8->unk3D8F0 = 0;
        func_ov007_020e769c(&data->unk_80C);
        data->unk_7FC = 1;
        func_ov007_020e75e4(data, func_ov007_020e7b54);
        return;
    }
    if (data->unk_800 != 0) {
        func_ov007_020e76dc(data, 0);
    }
}

void func_ov007_020e7b54(BtlAuraLast* data) {
    AuraTarget* target = (AuraTarget*)data_ov003_020e71b8->unk3D898;
    s32         posX   = target->unk_34 << 12;
    s32         posY   = (target->unk_36 + 0xA0) << 12;

    switch (data->unk_7FA) {
        case 0:
            if (data->unk_7F8 == 0) {
                data->unk_7F8 = 0x5A;
                data->unk_7F0 = 2;
            }
            if (data->unk_7F8 > 0) {
                func_020265d4(&data->unk_804, posX, (u16)data->unk_7F8);
                func_020265d4(&data->unk_808, posY, (u16)data->unk_7F8);
                data->unk_7F8--;
            }
            func_ov007_020e76dc(data, 1);
            if (data->unk_7F8 <= 0) {
                data->unk_7FA++;
                data->unk_7F8 = 0;
            }
            break;
        case 1:
            data->unk_7FC = 0;
            data->unk_800 = 1;
            func_ov003_020974f8(posX, posY);
            data_ov003_020e71b8->unk3D8EE = data->unk_80C;
            data_ov003_020e71b8->unk3D8F4 = 1;
            func_ov007_020e764c();
            func_ov007_020e75e4(data, func_ov007_020e7a8c);
            break;
    }
}

s32 func_ov007_020e7c98(TaskPool* pool, Task* task, void* args) {
    BtlAuraLast* data = task->data;

    data->unk_804                 = 0;
    data->unk_808                 = 0;
    data->unk_7FC                 = 0;
    data->unk_800                 = 0;
    data_ov003_020e71b8->unk3D8EE = 0x64;
    data_ov003_020e71b8->unk3D8ED = 0;
    data_ov003_020e71b8->unk3D8F0 = 0;
    data_ov003_020e71b8->unk3D8F4 = 0;

    func_ov007_020e75fc(1, &data->unk_000, 0, 0x40);
    func_ov007_020e75fc(0, &data->unk_060, 0, 0x3E);
    func_ov007_020e75fc(0, &data->unk_0C0, 1, 0x3E);
    func_ov007_020e791c(data);
    func_ov007_020e75e4(data, func_ov007_020e7a8c);
    return 1;
}

s32 func_ov007_020e7d48(TaskPool* pool, Task* task, void* args) {
    BtlAuraLast* data = task->data;

    if (data->unk_7F4 != NULL) {
        data->unk_7F4(data);
    }
    if (data->unk_7FC != 0) {
        CombatSprite_Update(&data->unk_000);
        CombatSprite_Update(&data->unk_060);
    }
    if (data->unk_800 != 0) {
        CombatSprite_Update(&data->unk_0C0);
    }
    func_ov007_020e7970(data);
    return 1;
}

s32 func_ov007_020e7da4(TaskPool* pool, Task* task, void* args) {
    BtlAuraLast* data = task->data;

    if (data->unk_7FC != 0) {
        if (data->unk_808 < 0xD8000) {
            CombatSprite_SetPosition(&data->unk_000, data->unk_804 << 4 >> 16, data->unk_808 << 4 >> 16);
            func_ov003_02082730(&data->unk_000, 0);
            CombatSprite_Render(&data->unk_000);
        }
        if (data->unk_808 > 0xA8000) {
            AuraTarget* target = (AuraTarget*)data_ov003_020e71b8->unk3D898;
            CombatSprite_SetPosition(&data->unk_060, (s16)(data->unk_804 >> 12), (s16)((data->unk_808 >> 12) - 0xC0));
            func_ov003_02082730(&data->unk_060, 0x80000001 - target->unk_2C);
            CombatSprite_Render(&data->unk_060);
        }
    }
    if (data->unk_800 != 0) {
        AuraTarget* target = (AuraTarget*)data_ov003_020e71b8->unk3D898;
        CombatSprite_SetPosition(&data->unk_0C0, target->unk_34, (s16)(target->unk_36 - 0x20));
        func_ov003_02082730(&data->unk_0C0, 0x80000001 - target->unk_2C);
        CombatSprite_Render(&data->unk_0C0);
    }
    func_ov007_020e79d8(data);
    return 1;
}

s32 func_ov007_020e7eb0(TaskPool* pool, Task* task, void* args) {
    BtlAuraLast* data = task->data;

    CombatSprite_Release(&data->unk_000);
    CombatSprite_Release(&data->unk_060);
    CombatSprite_Release(&data->unk_0C0);
    func_ov007_020e7a54(data);
    return 1;
}

s32 func_ov007_020e7ee0(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov007_020e7f70;

    if (func_ov003_0208b690(stage) != 0) {
        return 1;
    }
    return stages.iter[stage](pool, task, args);
}
