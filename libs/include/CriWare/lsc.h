#ifndef CRIWARE_LSC_H
#define CRIWARE_LSC_H

#include "types.h"
#include <CriWare/sj.h>

typedef struct LSC_OBJ LSC_OBJ;
typedef LSC_OBJ*       LSC;

void LSC_Init(void);
void LSC_Finish(void);
LSC  LSC_Create(SJ sj);
void LSC_Destroy(LSC);
void LSC_EntryErrFunc(void (*err_func)(void* obj, char* msg), void* obj);
void func_020215ec(void);
void LSC_SetStmHndl(void* lsc, void* stm);
void LSC_Stop(LSC lsc);
void LSC_ExecServer(void);

#endif // CRIWARE_LSC_H