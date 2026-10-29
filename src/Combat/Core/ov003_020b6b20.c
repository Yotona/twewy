#include "Combat/Core/CombatPsychEffects.h"

#include "Combat/Core/CombatApi.h"
extern const TaskHandle data_ov003_020dcb04;

s32 func_ov003_020b6b20(Ov003ShockWaveArgs* args) {
    return func_ov003_020a6000(&data_ov003_020dcb04, args);
}

char             data_ov003_020e4f1c[28] = "Tsk_BtlPsy_ShockWave_Body";
const TaskHandle data_ov003_020dcb04     = {data_ov003_020e4f1c, func_ov003_020b6ba4, 0xBC};
