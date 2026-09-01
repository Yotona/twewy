#ifndef UTIL_MSGXLS_H
#define UTIL_MSGXLS_H

#include <nitro/types.h>

s32         MsgXls_GetMesNum(s32 index);
s32         MsgXls_GetXlsFileCount(void);
s32         MsgXls_GetMesNumEnd(s32 index);
const char* MsgXls_GetXlsName(s32 index);

#endif
