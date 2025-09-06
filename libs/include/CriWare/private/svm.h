#ifndef CRIWARE_SVM_H
#define CRIWARE_SVM_H

#include <types.h>

void SVM_Init();
void SVM_Lock();
void SVM_Unlock();
void SVM_LockRsc();
void SVM_UnlockRsc();
void SVM_LockVar();
void SVM_UnlockVar();
void SVM_CallErr(const char* format, ...);
void SVM_SetCbBdr(s32 arg0, void (*arg1)(), s32 arg2);
void SVM_SetCbErr(void (*callback)(void*, char*), void* object);
s32  SVM_SetCbSvr(s32, s32 (*)(void*), void*);
void SVM_SetCbSvrId(s32, s32, s32 (*)(void*), void*);
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

#endif // CRIWARE_SVM_H