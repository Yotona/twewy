#ifndef CRIWARE_SVM_H
#define CRIWARE_SVM_H

#include <nitro/types.h>

// NOTE: this header used to be a verbatim copy of the PS2 CriWare SDK's
// svm.h. That version declares SVM_LockVar/UnlockVar/LockRsc/UnlockRsc,
// SVM_SetCbBdr and a 4-argument SVM_SetCbSvrId, none of which exist in this
// NITRO build of the library (see symbols.txt 0x0201a6b4-0x0201ad70). The
// declarations below match the DS binary instead.

void SVM_Init();
void SVM_Lock();
void SVM_Unlock();
void SVM_CallErr(const char* format, ...);
void SVM_SetCbErr(void (*callback)(void*, char*), void* object);
// Registers a server callback in the first free slot of `svtype` and returns
// that slot index (or -1). `tag` is stored in SVMSVRCallback::tag.
s32 SVM_SetCbSvrId(s32 svtype, s32 (*func)(void*), void* object, char* tag);
// Registers a server callback at the explicit slot `id`. Declared/defined as
// func_0201a9fc/func_0201aa38 in svm.c; its in-binary error strings
// ("1071201:SVM_SetCbSvrId:illegal id", "2100801:SVM_SetCbSvrId:over write
// callback function.") identify it as SVM_SetCbSvrId, but it is still unnamed
// because it has no callers outside adx_inis.c.
void func_0201a9fc(s32 svtype, s32 id, s32 (*func)(void*), void* object, char* tag);
void SVM_DelCbSvr(s32 svtype, s32 id);
void SVM_SetCbLock(void (*func)(void*), void* object);
void SVM_SetCbUnlock(void (*func)(void*), void* object);
void SVM_Finish();
s32  SVM_ExecSvrVint();
s32  SVM_ExecSvrVsync();
s32  SVM_ExecSvrFs();
s32  SVM_ExecSvrMain();
s32  SVM_TestAndSet(s32* mem);
void SVM_CallErr1(const char* msg);

// The DS build has exactly one extra lock/unlock pair beyond SVM_Lock/SVM_Unlock
// (0x0201a798 / 0x0201a7a8), both using lock type 3. The PS2 SDK numbers its
// types 1=base 2=Var 3=Sync 4=Rsc 5=Thrd 6=Etc, so type 3 there is Sync. This
// build has no threading, so the pair is most likely SVM_LockRsc/SVM_UnlockRsc
// with the enum renumbered (1=base 2=Var 3=Rsc), but that is unproven.
void func_0201a798();
void func_0201a7a8();

#endif // CRIWARE_SVM_H