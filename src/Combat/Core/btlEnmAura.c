#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatAura.h"
#include "Engine/Math/Random.h"
#include <stddef.h>

typedef struct BtlEnmAura BtlEnmAura;
typedef void (*BtlEnmAuraUpdate)(BtlEnmAura*);
struct BtlEnmAura {
    /* 0x000 */ CombatSprite       sprites[2];
    /* 0x0C0 */ CombatList         free;
    /* 0x0C8 */ CombatList         active;
    /* 0x0D0 */ BtlAuraFlameSprite flames[16];
    /* 0x790 */ s16                cooldown;
    /* 0x792 */ u16                reserved792;
    /* 0x794 */ BtlEnmAuraUpdate   update;
    /* 0x798 */ s16                timer;
    /* 0x79A */ s16                phase;
    /* 0x79C */ s32                side;
    /* 0x7A0 */ s32                x;
    /* 0x7A4 */ s32                y;
    /* 0x7A8 */ Vec                flamePosition;
    /* 0x7B4 */ s32                intensity;
    /* 0x7B8 */ s16                remaining;
    /* 0x7BA */ u16                reserved7BA;
};
typedef char check_BtlEnmAura[(sizeof(BtlEnmAura) == 0x7BC) ? 1 : -1];
typedef char check_BtlEnmAura_flames[(offsetof(BtlEnmAura, flames) == 0xD0) ? 1 : -1];
typedef char check_BtlEnmAura_update[(offsetof(BtlEnmAura, update) == 0x794) ? 1 : -1];
typedef char check_BtlEnmAura_intensity[(offsetof(BtlEnmAura, intensity) == 0x7B4) ? 1 : -1];
typedef char check_BtlEnmAuraState[(sizeof(BtlEnmAuraState) == 0x1C) ? 1 : -1];

extern const BinIdentifier   data_ov003_020e0b90;
extern const TaskHandle      data_ov003_020e0b98;
extern const SpriteAnimEntry data_ov003_020e0ba4[2];
extern const TaskStages      data_ov003_020e0bb4;
extern void                  func_020265d4(s32*, s32, u16);
extern s32                   func_ov003_020c2008(TaskPool* pool);
extern void                  func_ov003_020cac38(s32, s32);
extern s32                   func_ov024_020f10d8(Ov003CutinActor*);
void                         func_ov003_020c9de8(BtlEnmAura*);
void                         func_ov003_020ca188(BtlEnmAura*);

void func_ov003_020c96e4(void) {
    Ov003ActorParams* sub  = func_ov003_020c3c88(&data_ov003_020e7324.ownerSub->base);
    Ov003ActorParams* main = func_ov003_020c3c88(&data_ov003_020e7324.ownerMain->base);
    if (data_ov003_020e7324.activeMain) {
        data_ov003_020e7324.ownerSub->base.actor.unk_08  = sub->unk_0C * 3 / 2;
        data_ov003_020e7324.ownerMain->base.actor.unk_08 = main->unk_0C;
        return;
    }
    if (data_ov003_020e7324.activeSub) {
        data_ov003_020e7324.ownerSub->base.actor.unk_08  = sub->unk_0C;
        data_ov003_020e7324.ownerMain->base.actor.unk_08 = main->unk_0C * 3 / 2;
        return;
    }
    data_ov003_020e7324.ownerSub->base.actor.unk_08  = sub->unk_0C;
    data_ov003_020e7324.ownerMain->base.actor.unk_08 = main->unk_0C;
}
void func_ov003_020c9788(BtlEnmAura* self, BtlEnmAuraUpdate update) {
    self->update = update;
    self->phase  = 0;
    self->timer  = 0;
}
void func_ov003_020c97a0(s32* value, s32 target, u16 frames) {
    frames *= 3;
    s32 delta = target - *value;
    if (frames < 1)
        frames = 1;
    *value += delta / frames;
}
void func_ov003_020c97dc(Ov003CutinActor* owner) {
    switch ((u32)(owner->base.group - 0x37)) {
        case 0:
        case 1:
            if (owner->base.spriteIndex >= 2)
                return;
            if (RNG_Next(2)) {
                func_ov003_02087f28(0x525, data_ov003_020e7324.ownerMain->base.actor.position.x);
                return;
            }
            func_ov003_02087f28(0x526, data_ov003_020e7324.ownerMain->base.actor.position.x);
            return;
        case 2:
        case 3:
            if (owner->base.spriteIndex >= 2)
                return;
            if (RNG_Next(2)) {
                func_ov003_02087f64(0x53D, data_ov003_020e7324.ownerSub->base.actor.position.x);
                return;
            }
            func_ov003_02087f64(0x53E, data_ov003_020e7324.ownerSub->base.actor.position.x);
            return;
        default:
            return;
    }
}
// Nonmatching: flame-spawn position and resource register allocation.
void func_ov003_020c98b0(BtlEnmAura* self, s32 mode) {
    BtlAuraFlameSprite* flame;
    s32                 engine, palette, x, y, sortKey;
    s16                 anim;
    if (self->cooldown > 0) {
        self->cooldown--;
        return;
    }
    flame = func_ov003_0208495c(&self->free);
    if (flame == NULL)
        return;
    if (self->y >= 0xC0000 || data_ov003_020e7324.activeSub) {
        if (self->y <= 0xC0000)
            return;
        if (data_ov003_020e7324.activeMain)
            return;
    }
    if (mode != 0 && mode == 1) {
        anim = RNG_Next(5);
        if (self->y < 0xC0000) {
            engine  = 1;
            palette = 8;
            x       = self->x + ((RNG_Next(0x11) - 8) << 12);
            y       = self->y;
        } else {
            engine  = 0;
            palette = 7;
            x       = self->x + ((RNG_Next(0x11) - 8) << 12);
            y       = self->y - 0xC0000;
        }
        self->cooldown = 2;
    } else {
        anim = RNG_Next(2) + 5;
        if (self->y < 0xC0000) {
            s32 dx                = RNG_Next(0x19) - 12;
            s32 dy                = RNG_Next(0x31) - 16;
            engine                = 1;
            palette               = 8;
            self->flamePosition.x = data_ov003_020e7324.ownerSub->base.actor.position.x + (dx << 12);
            x                     = self->x + (dx << 12);
            y                     = self->y + (dy << 12);
            self->flamePosition.y = data_ov003_020e7324.ownerSub->base.actor.position.y;
            self->flamePosition.z = data_ov003_020e7324.ownerSub->base.actor.position.z + (dy << 12) - 0x20000;
        } else {
            s32 dx                = RNG_Next(0x19) - 12;
            s32 dy                = RNG_Next(0x31) - 16;
            engine                = 0;
            palette               = 7;
            self->flamePosition.x = data_ov003_020e7324.ownerMain->base.actor.position.x + (dx << 12);
            x                     = self->x + (dx << 12);
            self->flamePosition.y = data_ov003_020e7324.ownerMain->base.actor.position.y;
            y                     = self->y + (dy << 12) - 0xC0000;
            self->flamePosition.z = data_ov003_020e7324.ownerMain->base.actor.position.z + (dy << 12) - 0x20000;
        }
        self->cooldown = 4;
    }
    Ov003CutinActor* owner = engine ? data_ov003_020e7324.ownerSub : data_ov003_020e7324.ownerMain;
    sortKey                = 0x80000000 - owner->base.actor.position.y;
    CombatSprite_LoadDirect(engine, &flame->sprite, &data_ov003_020e0b90, 4, 6, 5, palette, (u16)anim);
    CombatSprite_SetAnim(&flame->sprite, (u16)anim, 1);
    CombatSprite_SetPosition(&flame->sprite, (x << 4) >> 16, (y << 4) >> 16);
    func_ov003_02082730(&flame->sprite, sortKey);
    func_ov003_020849dc(&self->free, &flame->node);
    func_ov003_020849bc(&self->active, &flame->node, flame);
}
void func_ov003_020c9b4c(BtlEnmAura* self, BtlAuraFlameSprite* flame) {
    if (flame == NULL)
        return;
    CombatSprite_Release(&flame->sprite);
    func_ov003_020849dc(&self->active, &flame->node);
    func_ov003_020849bc(&self->free, &flame->node, flame);
}
void func_ov003_020c9b84(BtlEnmAura* self) {
    func_ov003_020849ac(&self->free);
    func_ov003_020849ac(&self->active);
    self->cooldown = 0;
    for (s32 i = 0; i < 16; i++) {
        BtlAuraFlameSprite* flame = &self->flames[i];
        CombatSprite_Init(&flame->sprite);
        func_ov003_020849bc(&self->free, &flame->node, flame);
    }
}
void func_ov003_020c9bd8(BtlEnmAura* self) {
    BtlAuraFlameSprite* flame = func_ov003_0208495c(&self->active);
    if (flame == NULL)
        return;
    do {
        if (SpriteMgr_IsAnimationFinished(&flame->sprite.sprite)) {
            BtlAuraFlameSprite* next = func_ov003_02084984(&flame->node);
            func_ov003_020c9b4c(self, flame);
            flame = next;
        } else {
            CombatSprite_Update(&flame->sprite);
            flame = func_ov003_02084984(&flame->node);
        }
    } while (flame != NULL);
}
void func_ov003_020c9c40(BtlEnmAura* self) {
    BtlAuraFlameSprite* flame = func_ov003_0208495c(&self->active);
    if (flame == NULL)
        return;
    do {
        if (flame->sprite.sprite.bits_0_1 == 1)
            func_ov003_02082730(&flame->sprite, 0x80000000 - data_ov003_020e7324.ownerSub->base.actor.position.y);
        else
            func_ov003_02082730(&flame->sprite, 0x80000000 - data_ov003_020e7324.ownerMain->base.actor.position.y);
        CombatSprite_Render(&flame->sprite);
        flame = func_ov003_02084984(&flame->node);
    } while (flame != NULL);
}
void func_ov003_020c9cb8(BtlEnmAura* self) {
    for (s32 i = 0; i < 16; i++) {
        if (Sprite_HasAnimation(&self->flames[i].sprite.sprite))
            CombatSprite_Release(&self->flames[i].sprite);
    }
}
void func_ov003_020c9cf0(BtlEnmAura* self, s32 side) {
    if (side == 1) {
        self->x = data_ov003_020e7324.ownerSub->base.actor.screenX << 12;
        self->y = (data_ov003_020e7324.ownerSub->base.actor.screenY - 0x20) << 12;
        return;
    }
    self->x = data_ov003_020e7324.ownerMain->base.actor.screenX << 12;
    self->y = (data_ov003_020e7324.ownerMain->base.actor.screenY + 0xA0) << 12;
}
void func_ov003_020c9d54(BtlEnmAura* self) {
    if (self->intensity >= 0x1000)
        return;
    self->intensity = 0x1000;
    self->remaining = 0x4B0;
}
void func_ov003_020c9d78(BtlEnmAura* self) {
    if (self->remaining > 0) {
        func_ov003_020c97a0(&self->intensity, 0, (u16)self->remaining);
        self->remaining--;
        return;
    }
    self->intensity                = 0;
    self->remaining                = 0;
    data_ov003_020e7324.ready      = 0;
    data_ov003_020e7324.scale      = 100;
    data_ov003_020e7324.activeSub  = 0;
    data_ov003_020e7324.activeMain = 0;
}
void func_ov003_020c9de8(BtlEnmAura* self) {
    switch (data_ov003_020e7324.mode) {
        case 1:
            data_ov003_020e7324.mode = 0;
            func_ov003_020c9cf0(self, 0);
            if (self->intensity <= 0)
                func_ov003_02087f28(0x21, self->x);
            if (self->intensity > 0 && self->side != 0)
                return;
            self->side                    = 1;
            data_ov003_020e7324.activeSub = 0;
            if (data_ov003_020e7324.ready) {
                data_ov003_020e7324.scale -= 5;
                if (data_ov003_020e7324.scale > 10)
                    data_ov003_020e7324.scale = 10;
            } else
                data_ov003_020e7324.scale = 50;
            data_ov003_020e7324.unk_16 = (data_ov003_020e7324.ready + 7) * 60;
            if (data_ov003_020e7324.unk_16 > 900)
                data_ov003_020e7324.unk_16 = 900;
            data_ov003_020e7324.ready++;
            if (data_ov003_020e7324.ready % 5 == 0 && func_ov024_020f10d8(data_ov003_020e7324.ownerMain)) {
                if (!(data_ov003_020e71b8->unk3D878 & 0x20000)) {
                    data_ov003_020e71b8->unk3D878 |= 0x20000;
                    data_ov003_020e71b8->unk3D878 |= 0x10000;
                    CombatActor* owner = data_ov003_020e71b8->unk3D89C;
                    owner->flags &= ~8;
                    func_ov003_020c2008(&data_ov003_020e71b8->unk_34230);
                }
            }
            func_ov003_020c9d54(self);
            func_ov003_020c9788(self, func_ov003_020ca188);
            return;
        case 2:
            data_ov003_020e7324.mode = 0;
            func_ov003_020c9cf0(self, 1);
            if (self->intensity <= 0)
                func_ov003_02087f64(0x21, self->x);
            if (self->intensity > 0 && self->side != 1)
                return;
            self->side                     = 0;
            data_ov003_020e7324.activeMain = 0;
            if (data_ov003_020e7324.ready) {
                data_ov003_020e7324.scale -= 5;
                if (data_ov003_020e7324.scale > 10)
                    data_ov003_020e7324.scale = 10;
            } else
                data_ov003_020e7324.scale = 50;
            data_ov003_020e7324.unk_16 = (data_ov003_020e7324.ready + 7) * 60;
            if (data_ov003_020e7324.unk_16 > 900)
                data_ov003_020e7324.unk_16 = 900;
            data_ov003_020e7324.ready++;
            if (data_ov003_020e7324.ready % 5 == 0 && func_ov024_020f10d8(data_ov003_020e7324.ownerMain)) {
                if (!(data_ov003_020e71b8->unk3D878 & 0x20000)) {
                    data_ov003_020e71b8->unk3D878 |= 0x20000;
                    data_ov003_020e71b8->unk3D878 |= 0x10000;
                    CombatActor* owner = data_ov003_020e71b8->unk3D89C;
                    owner->flags &= ~8;
                    func_ov003_020c2008(&data_ov003_020e71b8->unk_34230);
                }
            }
            func_ov003_020c9d54(self);
            func_ov003_020c9788(self, func_ov003_020ca188);
            return;
        case 3:
            data_ov003_020e7324.mode = 0;
            if (self->intensity > 0 && self->side == 0)
                func_ov003_020c9d54(self);
            func_ov003_020c9cf0(self, self->side);
            return;
        case 4:
            data_ov003_020e7324.mode = 0;
            if (self->intensity > 0 && self->side == 1)
                func_ov003_020c9d54(self);
            func_ov003_020c9cf0(self, self->side);
            return;
        default:
        case 0:
            func_ov003_020c9d78(self);
            func_ov003_020c9cf0(self, self->side);
            if (self->intensity <= 20)
                return;
            func_ov003_020c98b0(self, 0);
            return;
    }
}
// Nonmatching: burst state and position register allocation.
void func_ov003_020ca188(BtlEnmAura* self) {
    s16 screenX;
    s32 screenY;
    if (self->side == 1) {
        screenX = data_ov003_020e7324.ownerSub->base.actor.screenX;
        screenY = data_ov003_020e7324.ownerSub->base.actor.screenY - 0x20;
    } else {
        screenX = data_ov003_020e7324.ownerMain->base.actor.screenX;
        screenY = data_ov003_020e7324.ownerMain->base.actor.screenY + 0xA0;
    }
    s32 x = screenX << 12;
    s32 y = screenY << 12;
    switch (self->phase) {
        case 0:
            if (self->timer == 0) {
                self->timer = 90;
                CombatSprite_SetAnimFromTable(&self->sprites[0], 0, 0);
                CombatSprite_SetAnimFromTable(&self->sprites[1], 0, 0);
                self->cooldown = 2;
            }
            if (self->timer > 0) {
                func_020265d4(&self->x, x, (u16)self->timer);
                func_020265d4(&self->y, y, (u16)self->timer);
                self->timer--;
            }
            func_ov003_020c98b0(self, 1);
            if (self->timer > 0)
                return;
            self->phase++;
            self->timer = 0;
            return;
        case 1:
            if (self->timer == 0) {
                self->timer++;
                func_ov003_020cac38(x, y);
            }
            if (self->side == 1) {
                data_ov003_020e7324.activeMain = 1;
                func_ov003_020c97dc(data_ov003_020e7324.ownerSub);
            } else {
                data_ov003_020e7324.activeSub = 1;
                func_ov003_020c97dc(data_ov003_020e7324.ownerMain);
            }
            func_ov003_020c96e4();
            self->x = x;
            self->y = y;
            CombatSprite_SetAnimFromTable(&self->sprites[0], 1, 0);
            CombatSprite_SetAnimFromTable(&self->sprites[1], 1, 0);
            func_ov003_020c9788(self, func_ov003_020c9de8);
            return;
    }
}
s32 func_ov003_020ca344(TaskPool* pool, Task* task, void* args) {
    BtlEnmAura* self               = task->data;
    self->x                        = 0;
    self->y                        = 0;
    self->intensity                = 0;
    self->remaining                = 0;
    self->side                     = 1;
    data_ov003_020e7324.scale      = 100;
    data_ov003_020e7324.ready      = 0;
    data_ov003_020e7324.activeMain = 0;
    data_ov003_020e7324.activeSub  = 0;
    CombatSprite_LoadFromTable(1, &self->sprites[0], &data_ov003_020e0b90, data_ov003_020e0ba4, 0, 8, 0x40);
    CombatSprite_LoadFromTable(0, &self->sprites[1], &data_ov003_020e0b90, data_ov003_020e0ba4, 0, 7, 0x40);
    CombatSprite_SetAnimFromTable(&self->sprites[0], 1, 0);
    CombatSprite_SetAnimFromTable(&self->sprites[1], 1, 0);
    func_ov003_020c9b84(self);
    func_ov003_020c9788(self, func_ov003_020c9de8);
    return 1;
}
s32 func_ov003_020ca424(TaskPool* pool, Task* task, void* args) {
    BtlEnmAura* self = task->data;
    if (func_ov003_020c9008())
        return 0;
    if (func_ov003_020c3c28())
        return 1;
    if (self->update)
        self->update(self);
    if (data_ov003_020e7324.unk_16 > 0)
        data_ov003_020e7324.unk_16--;
    CombatSprite_Update(&self->sprites[0]);
    CombatSprite_Update(&self->sprites[1]);
    func_ov003_020c9bd8(self);
    return 1;
}
s32 func_ov003_020ca498(TaskPool* pool, Task* task, void* args) {
    BtlEnmAura* self = task->data;
    if (func_ov003_020c3c28())
        return 1;
    s32 scale = self->intensity;
    if (scale > 20) {
        if (self->y < 0xD8000 && !data_ov003_020e7324.activeSub) {
            Ov003CutinActor* owner = data_ov003_020e7324.ownerSub;
            CombatSprite_SetAffineTransform(&self->sprites[0], 0, scale, scale, 1);
            CombatSprite_SetPosition(&self->sprites[0], (self->x << 4) >> 16, (self->y << 4) >> 16);
            func_ov003_02082730(&self->sprites[0], 0x80000001 - owner->base.actor.position.y);
            CombatSprite_Render(&self->sprites[0]);
        }
        if (self->y > 0xA8000 && !data_ov003_020e7324.activeMain) {
            s32              scale = self->intensity;
            Ov003CutinActor* owner = data_ov003_020e7324.ownerMain;
            CombatSprite_SetAffineTransform(&self->sprites[1], 0, scale, scale, 1);
            CombatSprite_SetPosition(&self->sprites[1], (self->x << 4) >> 16, (s16)((self->y >> 12) - 0xC0));
            func_ov003_02082730(&self->sprites[1], 0x80000001 - owner->base.actor.position.y);
            CombatSprite_Render(&self->sprites[1]);
        }
    }
    func_ov003_020c9c40(self);
    return 1;
}
s32 func_ov003_020ca5bc(TaskPool* pool, Task* task, void* args) {
    BtlEnmAura* self = task->data;
    CombatSprite_Release(&self->sprites[0]);
    CombatSprite_Release(&self->sprites[1]);
    func_ov003_020c9cb8(self);
    return 1;
}
s32 func_ov003_020ca5e4(TaskPool* pool, Task* task, void* args, s32 stage) {
    TaskStages stages = data_ov003_020e0bb4;
    return stages.iter[stage](pool, task, args);
}
void func_ov003_020ca62c(Ov003CutinActor* owner) {
    if (func_ov003_020c37f8(&owner->base.sprite)) {
        data_ov003_020e7324.ownerMain = (Ov003CutinActor*)owner->base.partner;
        data_ov003_020e7324.ownerSub  = owner;
        return;
    }
    data_ov003_020e7324.ownerMain = owner;
    data_ov003_020e7324.ownerSub  = (Ov003CutinActor*)owner->base.partner;
}
void func_ov003_020ca66c(Ov003CutinActor* owner) {
    func_ov003_020ca62c(owner);
    EasyTask_CreateTask(func_ov003_020c3ca4(owner), &data_ov003_020e0b98, NULL, 0, NULL, NULL);
}

extern char           data_ov003_020e6b74[16], data_ov003_020e6b84[28];
const BinIdentifier   data_ov003_020e0b90    = {3, data_ov003_020e6b84};
const TaskHandle      data_ov003_020e0b98    = {data_ov003_020e6b74, func_ov003_020ca5e4, sizeof(BtlEnmAura)};
const SpriteAnimEntry data_ov003_020e0ba4[2] = {
    {1, 3, 2, 0},
    {1, 3, 2, 2}
};
const TaskStages data_ov003_020e0bb4 = {func_ov003_020ca344, func_ov003_020ca424, func_ov003_020ca498, func_ov003_020ca5bc};
char             data_ov003_020e6b64[16] = "BtlEff_SideMove";
char             data_ov003_020e6b74[16] = "Tsk_btlEnmAura";
char             data_ov003_020e6b84[28] = "Apl_Mor/Grp_BtlEnmAura.bin";
BtlEnmAuraState  data_ov003_020e7324;
