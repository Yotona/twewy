#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatPsychEffects.h"
#include <nitro/mi/cpumem.h>

extern void  func_ov003_020849ac(CombatList*);
extern void  func_ov003_020849bc(CombatList*, CombatListNode*, void*);
extern void* func_ov003_020849dc(CombatList*, CombatListNode*);
extern s32   func_ov003_020a3498(CombatSprite*, s32, s32, s32, u16);

extern const TaskHandle data_ov003_020dc96c;

s32 func_ov003_020b2e30(CombatList* list, CombatActor* actor) {
    Ov003TargetArgs args;
    args.list  = list;
    args.actor = actor;
    return func_ov003_020a6000(&data_ov003_020dc96c, &args);
}

s32 func_ov003_020b2e58(TaskPool* pool, Task* task, void* args, s32 stage) {
    Ov003TargetArgs* arg = args;
    Ov003Target*     self;
    switch (stage) {
        default:
        case 0:
            self = task->data;
            MI_CpuSet(self, 0, sizeof(*self));
            self->list         = arg->list;
            self->entry.taskId = ((u32)task->generation << 16) | (u32)task->id;
            self->entry.actor  = arg->actor;
            func_ov003_020849bc(self->list, &self->entry.node, &self->entry);
            func_ov003_02087f28(0x14C, self->entry.actor->position.x);
            func_ov003_020a3498(&self->sprite, 2, 1, 0x10, 1);
            return 1;
        case 1:
            self = task->data;
            if (self->entry.actor == NULL) {
                func_ov003_020849dc(self->list, &self->entry.node);
                return 0;
            }
            if ((self->entry.actor->flags & 4) || (self->entry.actor->flags & 0x10)) {
                func_ov003_020849dc(self->list, &self->entry.node);
                return 0;
            }
            if (SpriteMgr_IsAnimationFinished(&self->sprite.sprite))
                func_ov003_020a35b8(&self->sprite, 0x11, 0);
            CombatSprite_Update(&self->sprite);
            return 1;
        case 2:
            self = task->data;
            CombatSprite_SetPosition(&self->sprite, self->entry.actor->screenX,
                                     (s16)(self->entry.actor->screenY - self->entry.actor->unk_76));
            CombatSprite_Render(&self->sprite);
            return 1;
        case 3:
            self = task->data;
            CombatSprite_Release(&self->sprite);
            return 1;
    }
}

void func_ov003_020b2fd8(CombatList* list) {
    func_ov003_020849ac(list);
}

void func_ov003_020b2fe4(CombatList* list) {
    if (func_ov003_020b30d8(list))
        return;
    do {
        func_ov003_020b3078(list);
    } while (!func_ov003_020b30d8(list));
}

void func_ov003_020b3014(CombatList* list, CombatActor* actor) {
    if (actor->flags & 4)
        return;
    if (actor->flags & 0x10)
        return;
    func_ov003_020b2e30(list, actor);
}

void func_ov003_020b3034(CombatList* list, CombatActor* actor) {
    Ov003TargetEntry* entry = func_ov003_020b30a0(list, actor);
    func_ov003_020b3050(list, entry);
}
char             data_ov003_020e4db0[24] = "SubTask_BtlPsy_Target";
const TaskHandle data_ov003_020dc96c     = {data_ov003_020e4db0, func_ov003_020b2e58, sizeof(Ov003Target)};
