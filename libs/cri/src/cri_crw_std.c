#include <printf.h>
#include <stdarg.h>
#include <string.h>

#ifdef REGION_JP
const char data_0205c0d8[] = "\nCRI CRW:STD/NITRO Ver.0.81 Build:Jun 22 2007 15:54:54\n\0Append: MW4020\n";
#else
const char data_0205c0d8[] = "\nCRI CRW:STD/NITRO Ver.0.82 Build:Sep 28 2007 13:14:11\n\0Append: MW4020\n";
#endif

const char* data_020659c4 = NULL;

void func_020215fc(void) {
    data_020659c4 = data_0205c0d8;
}

char* CRICRW_Strcpy(char* dst, int size, const char* src) {
    return strcpy(dst, src);
}

char* CRICRW_Strncpy(char* dst, int size, const char* src, int n) {
    return strncpy(dst, src, n);
}

char* CRICRW_Strcat(char* dst, int size, const char* src) {
    return strcat(dst, src);
}

char* CRICRW_Strncat(char* dst, int size, const char* src, int n) {
    return strncat(dst, src, n);
}

int CRICRW_Sprintf(char* dst, int size, const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    return vsprintf(dst, fmt, ap);
}

int CRICRW_Vsprintf(char* dst, int size, const char* fmt, va_list ap) {
    return vsprintf(dst, fmt, ap);
}
