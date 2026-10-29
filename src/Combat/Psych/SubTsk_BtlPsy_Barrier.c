#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include "SndMgr.h"
#include <nitro/mi/cpumem.h>

extern s32       func_ov003_020a51c0(void);
extern s32       func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);
extern void      func_ov003_02087f28(SndMgrSeIdx, s32);
extern s32       func_ov003_020843b0(s32, s32);
extern void      func_ov003_0209b564(s32, s32, CombatActor*, s32);
extern void      func_ov003_0208b3cc(s32, s16);
extern const s32 data_ov003_020dc6b4[4];

extern const TaskHandle data_ov003_020dc6a8;

void func_ov003_020aa32c(Ov003EffectState* effect, s32 mode, s32 kind) {
    Ov003BarrierSpawn args;
    args.effect = effect;
    args.mode   = mode;
    args.kind   = kind;
    func_ov003_020a6000(&data_ov003_020dc6a8, &args);
}

// Nonmatching: register allocation and affine-render loop scheduling differ.
s32 func_ov003_020aa358(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003BarrierSpawn* arg = args;
    Ov003Barrier*      barrier;
    switch (stage) {
        default:
        case 0: {
            CombatActor* player;
            barrier = task->data;
            player  = func_ov003_020a6438();
            MI_CpuSet(barrier, 0, sizeof(*barrier));
            barrier->spawn.effect  = arg->effect;
            barrier->spawn.mode    = arg->mode;
            barrier->spawn.kind    = arg->kind;
            barrier->animationKind = arg->kind;
            barrier->phase         = 0;
            if (barrier->animationKind == 2 && barrier->spawn.effect->index == 1)
                barrier->animationKind = 3;
            barrier->spawn.effect->unk_0A++;
            func_ov003_020a3498(&barrier->sprites[2], 2, 1, data_ov003_020dc6b4[barrier->animationKind], 0);
            func_ov003_020a3498(&barrier->sprites[0], 2, 1, data_ov003_020dc6b4[barrier->animationKind], 0);
            func_ov003_020a3498(&barrier->sprites[3], 2, 1, data_ov003_020dc6b4[barrier->animationKind], 0);
            func_ov003_020a3498(&barrier->sprites[1], 2, 1, data_ov003_020dc6b4[barrier->animationKind] + 1, 0);
            CombatSprite_SetAnim(&barrier->sprites[1], 1, 1);
            switch (barrier->spawn.mode) {
                case 0:
                    func_ov003_020a6464(1);
                    break;
                case 1:
                    func_ov003_020a6464(0);
                    break;
            }
            func_ov003_020a64ac();
            switch (barrier->spawn.kind) {
                case 0:
                    func_ov003_02087f28(0xB2, player->position.x);
                    break;
                case 2:
                    func_ov003_02087f28(0x94, player->position.x);
                    break;
                case 1:
                    func_ov003_02087f28(0xB0, player->position.x);
                    barrier->cooldown = 0;
                    break;
            }
            return 1;
        }
        case 1: {
            Ov003Barrier* barrier = task->data;
            CombatActor*  player  = func_ov003_020a6438();
            BtlTouchWork* touch   = func_ov003_020a62a0();
            s32           active  = 1;
            func_ov003_020a5a94(barrier->spawn.effect);
            func_ov003_020a6450();
            func_ov003_020aa860(barrier);
            switch (barrier->spawn.kind) {
                case 0:
                    if (func_ov003_020a6628(&barrier->spawn.effect->streak, 0x162, player->position.x, player->position.y,
                                            player->position.z, 0, 0) == 1)
                        func_ov003_020aa990(barrier);
                    break;
                case 1:
                    if (barrier->cooldown > 0)
                        barrier->cooldown--;
                    else {
                        barrier->cooldown = barrier->spawn.effect->params[1];
                        func_ov003_020aa9f4(barrier->spawn.effect, barrier->spawn.effect->params[0]);
                    }
                    break;
                case 2: {
                    s32 attackId = barrier->animationKind == 2 ? 0x163 : 0x164;
                    if (attackId == 0x164 && func_ov003_020a51c0())
                        attackId = 0x165;
                    if (func_ov003_020a6628(&barrier->spawn.effect->streak, (u16)attackId, player->position.x,
                                            player->position.y, player->position.z, 0, 0) == 1)
                    {
                        func_ov003_020aa990(barrier);
                        func_ov003_020aa9f4(barrier->spawn.effect, barrier->spawn.effect->params[0]);
                    }
                    break;
                }
            }
            switch (barrier->spawn.mode) {
                case 0:
                    if (!func_ov003_020a5aa4(barrier->spawn.effect) || func_ov003_020a63a8(barrier->spawn.effect) ||
                        !func_ov003_02085cfc(6) || touch->heldActor != func_ov003_020a6438())
                        active = 0;
                    break;
                case 1:
                    if (!func_ov003_020a5aa4(barrier->spawn.effect) || func_ov003_020a63a8(barrier->spawn.effect) ||
                        !func_ov003_02085cfc(6) || touch->heldActor != func_ov003_020a6438())
                        active = 0;
                    break;
            }
            for (s32 i = 0; i < 4; i++)
                CombatSprite_Update(&barrier->sprites[i]);
            return active;
        }
        case 2: {
            Ov003Barrier* barrier    = task->data;
            CombatActor*  player     = func_ov003_020a6438();
            s32           flip       = player->isFlipped == 0;
            barrier->affine.rotation = 0;
            barrier->affine.scaleX   = flip ? -0x1000 : 0x1000;
            barrier->affine.scaleY   = 0x1000;
            barrier->affine.unk_0C   = 0;
            barrier->affine.unk_0E   = 0;
            for (s32 i = 0; i < 4; i++) {
                CombatSprite_SetPosition(&barrier->sprites[i], player->screenX, player->screenY);
                func_ov003_02082730(&barrier->sprites[i], 0x384000 + (i << 12));
                CombatSprite_SetFlip(&barrier->sprites[i], flip);
                barrier->sprites[i].sprite.unk_0A.unk_00 = 1;
                barrier->sprites[i].affine               = &barrier->affine;
                if (i == 0 || i == 2)
                    CombatSprite_RenderWithPalette(&barrier->sprites[i], 0x10);
                else
                    CombatSprite_Render(&barrier->sprites[i]);
            }
            return 1;
        }
        case 3:
            barrier = task->data;
            for (s32 i = 0; i < 4; i++)
                CombatSprite_Release(&barrier->sprites[i]);
            switch (barrier->spawn.mode) {
                case 0:
                    func_ov003_020a6464(0);
                    break;
                case 1:
                    break;
            }
            barrier->spawn.effect->unk_0A--;
            return 1;
    }
}

void func_ov003_020aa860(Ov003Barrier* barrier) {
    CombatActor* player = func_ov003_020a6438();
    if (barrier->phase == 0) {
        if (!SpriteMgr_IsAnimationFinished(&barrier->sprites[1].sprite))
            return;
        barrier->phase = 1;
        CombatSprite_SetAnim(&barrier->sprites[1], 2, 0);
        CombatSprite_SetAnim(&barrier->sprites[2], 1, 0);
        CombatSprite_SetAnim(&barrier->sprites[0], 3, 0);
        CombatSprite_SetAnim(&barrier->sprites[3], 5, 1);
        return;
    }
    if (SpriteMgr_IsAnimationFinished(&barrier->sprites[2].sprite))
        CombatSprite_SetAnim(&barrier->sprites[2], 1, 0);
    if (SpriteMgr_IsAnimationFinished(&barrier->sprites[0].sprite))
        CombatSprite_SetAnim(&barrier->sprites[0], 3, 0);
    if (!SpriteMgr_IsAnimationFinished(&barrier->sprites[3].sprite))
        return;
    CombatSprite_SetAnim(&barrier->sprites[3], 5, 1);
    CombatSprite_Restart(&barrier->sprites[3]);
    switch (barrier->spawn.kind) {
        case 0:
            func_ov003_02087f28(0xB3, player->position.x);
            break;
        case 1:
            func_ov003_02087f28(0xB1, player->position.x);
            break;
        case 2:
            func_ov003_02087f28(0x95, player->position.x);
            break;
    }
}

void func_ov003_020aa990(Ov003Barrier* barrier) {
    func_ov003_020a6438();
    if (barrier->phase == 0)
        return;
    CombatSprite_SetAnim(&barrier->sprites[2], 2, 1);
    CombatSprite_Restart(&barrier->sprites[2]);
    CombatSprite_SetAnim(&barrier->sprites[0], 4, 1);
    CombatSprite_Restart(&barrier->sprites[0]);
    CombatSprite_SetAnim(&barrier->sprites[3], 6, 1);
    CombatSprite_Restart(&barrier->sprites[3]);
}

void func_ov003_020aa9f4(Ov003EffectState* effect, s32 percent) {
    CombatActor* player = func_ov003_020a6438();
    s32          amount = data_ov003_020e71b8->unk3D8D2 * percent / 100;
    SndMgr_PlaySEWithPan(0x14E, func_ov003_020843b0(0, player->position.x));
    func_ov003_020b67dc(effect, player);
    func_ov003_0209b564(2, amount, player, 1);
    func_ov003_0208b3cc(0, (s16)amount);
}

char             data_ov003_020e4a58[24] = "SubTsk_BtlPsy_Barrier";
const TaskHandle data_ov003_020dc6a8     = {data_ov003_020e4a58, func_ov003_020aa358, sizeof(Ov003Barrier)};
const s32        data_ov003_020dc6b4[4]  = {0x83, 0x85, 0x87, 0x89};
