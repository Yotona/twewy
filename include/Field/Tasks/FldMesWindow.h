#ifndef FIELD_TASKS_FLDMESWINDOW_H
#define FIELD_TASKS_FLDMESWINDOW_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

// Message-window task parameters.
typedef struct {
    /* 0x00 */ s32          datSlot; // backing task id
    /* 0x04 */ FldScreenPos unk_04;
    /* 0x08 */ FldScreenPos unk_08;
    /* 0x0C */ s32          unk_0C;
    /* 0x10 */ s32          mesNo;
    /* 0x14 */ s16          unk_14;
    /* 0x16 */ s16          unk_16;
    /* 0x18 */ u8           winKind;
    /* 0x19 */ u8           unk_19;
    /* 0x1A */ s8           unk_1A;
    /* 0x1B */ s8           unk_1B;
    /* 0x1C */ s32          unk_1C;
    /* 0x20 */ s32          unk_20;
    /* 0x24 */ s16          unk_24;
} FldMesTaskParam; // Size: 0x28

s32  FldMesWindow_Create(TaskPool* pool, s32 datSlot, s32 arg2, s16 arg3, s32 arg4, s16 arg5, s32 mesNo, u32 winKind, s16 arg8,
                         u8 arg9, u16 arg10, u16 arg11);
s32  FldMesWindow_CreateMessage(TaskPool* pool, s32 arg1, s32 arg2, s16 arg3, s16 arg4, s16 arg5, s32 arg6, u8 arg7, s16 arg8,
                                u8 arg9, u16 arg10);
void FldMesWindow_SetPriority(Task* task, s32 mode);
void FldMesWindow_SkipAnim(Task* task);
void FldMesWindow_ScrollUp(Task* task, s16 arg1);
void FldMesWindow_Move(Task* task, s32 arg1, s32 arg2);
BOOL FldMesWindow_IsAnimating(Task* task);
void FldMesWindow_Close(Task* task);
void FldMesWindow_Highlight(Task* task);
void FldMesWindow_Press(Task* task);

#endif // FIELD_TASKS_FLDMESWINDOW_H
