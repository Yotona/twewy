#include "Combat/Core/Combat.h"
#include "Combat/Core/CombatApi.h"
#include "Combat/Core/CombatTouch.h"
#include "Engine/Core/System.h"
#include <nitro/mi/cpumem.h>

extern void             func_02040770(BtlMicSampling* sampling);
extern void*            func_ov003_0208495c(CombatList* list);
extern void*            func_ov003_02084984(CombatListNode* node);
extern const TaskHandle data_ov003_020dc228;

void func_ov003_020a5080(void) {
    BtlMicInput* input = &data_ov003_020e71b8->microphone;
    EasyTask_CreateTask(&data_ov003_020e71b8->unk_10118, &data_ov003_020dc228, NULL, 0, NULL, &input);
}

s32 func_ov003_020a50cc(void) {
    BtlMicInput* input = &data_ov003_020e71b8->microphone;
    return input->cooldown != 0 ? 0 : input->detected;
}

// The retail callback tests positive signed samples only; the result code is unused.
// Nonmatching: mwcc removes the retail loop's initial branch to its condition.
void func_ov003_020a50f4(s32 result, BtlMicInput* input) {
    s32 i;
    input->detected = 0;
    for (i = 0; i < 32; i++) {
        if (input->samples[i] > 100) {
            input->detected = 1;
        }
    }
}

static inline BOOL BtlMic_IsSuppressed(void) {
    if (SystemStatusFlags.unk_10) {
        return TRUE;
    }
    return FALSE;
}

s32 func_ov003_020a5120(TaskPool* pool, Task* task, void* args, s32 stage) {
    switch (stage) {
        default:
        case 0:
            *(BtlMicInput**)task->data = *(BtlMicInput**)args;
            return 1;
        case 1: {
            BtlMicInput** state = task->data;
            BtlMicInput*  input = *state;
            if (input->cooldown != 0) {
                input->cooldown--;
            } else if (BtlMic_IsSuppressed()) {
                input->cooldown = 180;
            }
            func_02040770(&(*state)->sampling);
            return 1;
        }
        case 2:
            func_ov003_020a50cc();
            return 1;
        case 3:
            return 1;
    }
}

// All six microphone-psych actor kinds must be represented, regardless of duplicates.
s32 func_ov003_020a51c0(void) {
    s32              seen[6];
    s32              count = 0;
    Ov003TouchActor* actor;

    MI_CpuSet(seen, 0, sizeof(seen));
    actor = func_ov003_0208495c(&data_ov003_020e71b8->unk3D8BC);
    while (actor != NULL) {
        if (actor->kind >= 0xD4 && actor->kind <= 0xD9) {
            s32 index = actor->kind - 0xD4;
            if (seen[index] == 0) {
                seen[index] = 1;
                count++;
            }
        }
        actor = func_ov003_02084984(&actor->node);
    }
    return count == 6;
}

char             data_ov003_020e48b0[12] = "Tsk_BtlMic";
const TaskHandle data_ov003_020dc228     = {data_ov003_020e48b0, func_ov003_020a5120, sizeof(BtlMicInput*)};
