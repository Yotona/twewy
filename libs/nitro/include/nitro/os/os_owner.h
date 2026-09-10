#ifndef _NITRO_OS_OWNER_H
#define _NITRO_OS_OWNER_H

#ifdef __cplusplus
extern "C" {
#endif

#include "nitro/types.h"

typedef struct OSOwnerInfo {
    /* 0x00 */ u8 language;
    /* 0x01 */ u8 favoriteColor;
    struct {
        /* 02 */ u8 month;
        /* 03 */ u8 day;
    } birthday;
    /* 04 */ u16   nickName[11];
    /* 1a */ u16   nickNameLength;
    /* 0x1C */ u16 message[27];
    /* 0x52 */ u16 messageLength;
    /* 0x54 */
} OSOwnerInfo;

void OS_func_0177(OSOwnerInfo* info);

void OS_GetOwnerInfo(OSOwnerInfo* info);

#ifdef __cplusplus
} // extern "C"
#endif

#endif