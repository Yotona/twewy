#ifndef FIELD_FLDBADGE_H
#define FIELD_FLDBADGE_H

#include "Engine/EasyTask.h"

s32 func_ov030_020b3d88(TaskPool* pool, s32 datSlot, s32 anchorX, s16 anchorY, s32 windowX, s16 windowY, s32 mesNo,
                        u32 winKind, s16 arg8, u8 arg9, u16 arg10, u16 arg11);

// Message-window constructor. Both y positions are supplied even when equal.
s32 func_ov030_020b3fbc(TaskPool* pool, s32 datSlot, s32 anchorX, s16 anchorY, s16 windowX, s16 windowY, s32 mesNo, u8 winKind,
                        s16 arg8, u8 arg9, u16 arg10);

#endif
