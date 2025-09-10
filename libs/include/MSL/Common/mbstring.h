#ifndef MSL_MBSTRING_H
#define MSL_MBSTRING_H

#include <types.h>

s32 __mbtowc_noconv(u16* pwc, const char* s, s32 n);
s32 __wctomb_noconv(char* s, u16 wchar);

#endif // MSL_MBSTRING_H