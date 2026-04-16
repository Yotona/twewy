#ifndef COMBAT_FRIEND_PRIVATE_SHIKI_H
#define COMBAT_FRIEND_PRIVATE_SHIKI_H

#include <types.h>

extern s32 func_ov003_0208b690(u16);

BOOL func_ov004_020e7360(void);

void BtlEff_Doll_CreateTask(u16, void*);
void Feat00_Sword_CreateTask(u32 arg0, u16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5);
void Feat00_EffDoll_CreateTask(s32 arg0, u16 animIndex, s16 posX, s16 posY, s32 arg4, s32 arg5);
void BtlFeat00_Multi_CreateTask(s32 arg0, u16 arg1, s16 arg2, s16 arg3, s32 arg4, s32 arg5);

extern const TaskHandle Tsk_BtlArm_Doll;

#endif // COMBAT_FRIEND_PRIVATE_SHIKI_H