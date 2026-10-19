#ifndef FIELD_BATTLE_FIELDNOISEQUOTA_H
#define FIELD_BATTLE_FIELDNOISEQUOTA_H

#include <nitro/types.h>

s32  FieldNoise_GetQuota(s32 kind);
s16  FieldNoise_GetTotalQuota(void);
void FieldNoise_AdvanceQuotaStep(void);

#endif // FIELD_BATTLE_FIELDNOISEQUOTA_H
