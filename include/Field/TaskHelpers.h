#ifndef FIELD_TASK_HELPERS_H
#define FIELD_TASK_HELPERS_H

#include "Engine/EasyTask.h"
#include "Field/FieldTypes.h"

// Helpers consume the Task returned by EasyTask_GetTaskById.
s32  func_ov030_020b3058(Task* task);
s32  func_ov030_020b484c(Task* task);
s32  func_ov030_020b4858(Task* task);
void func_ov030_020bacf8(Task* task);
void func_ov030_020bad3c(Task* task);
s32  func_ov030_020c01ec(Task* task);

s32               func_ov030_020aef2c(void);
s32               func_ov030_020af2b8(void);
struct FieldExit* func_ov030_020b84a4(s32 area, s32* blocked, s32* direction);
s32               func_ov030_020c04d8(TaskPool* pool, s32 dataType, s16 animation);

// Task constructors return EasyTask_CreateTask's ID; pointer arguments are
// marshalled into 32-bit slots in the copied creation parameters.
s32 func_ov030_020bacb0(TaskPool* pool, s32 dataType, FieldPos* screen, s32* scale, s32 arg4, s32 arg5);
s32 func_ov030_020bc028(TaskPool* pool, s32 dataType, u16* pos, s32 mode, s32 effect);
s32 func_ov030_020c2c94(TaskPool* pool, s32 dataType, FieldPos* origin, s32 dir);

u16 func_ov030_020af53c(void);
s32 func_ov030_020af6a0(s32 value);
s16 func_ov030_020c105c(void);
s16 func_ov030_020b7b00(void);
s16 func_ov030_020b7b40(void);
s16 func_ov030_020b7b60(void);
s32 func_ov030_020b7b80(void);
s16 func_ov030_020b7bf8(void);

typedef struct Ov030Player Ov030Player;
typedef void (*Ov030PlayerCallback)(TaskPool*, Ov030Player*);
void func_ov030_020b21c0(Ov030Player* player, Ov030PlayerCallback callback);
void func_ov030_020b2418(TaskPool* pool, Ov030Player* player);
void func_ov030_020b24b0(TaskPool* pool, Ov030Player* player);

// The caller supplies both coordinates; this helper currently uses only x.
s32 func_ov030_020b7c9c(s32 x, s32 unusedY);

// Shared arm9 (non-overlay) helpers called by several Field TUs.
void func_02026590(s32* value, s32 target, u16 frames);
void func_020265d4(s32* value, s32 target, u16 scale);
void func_020267e0(s32 arg0);
void func_0202688c(void);
void func_02027b0c(s32 arg0, s32 arg1);
void func_02028a44(void);
void func_02028ad0(s32 arg0, s32 arg1, s32 arg2, s32 arg3);
void func_02028b0c(s32 arg0, s32 arg1);
s32  func_0202599c(s32 flagIndex);

// ov030 helpers shared by several Field TUs.
s32  func_ov030_020aefbc(void);
s32  func_ov030_020aefe0(void);
void func_ov030_020af6f0(s32 arg0);
void func_ov030_020af708(s16 arg0);

#endif
