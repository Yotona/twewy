#ifndef STRING_H
#define STRING_H

#include <types.h>

void* memcpy(void* dst, const void* src, s32 n);
void* memset(void* dest, s32 val, s32 n);
void* memchr(const void* src, s32 val, s32 n);
s32   memcmp(const void* src1, const void* src2, s32 n);

s32   strlen(const char* str);
char* strcpy(char* dest, const char* src);
char* strncpy(char* dest, const char* src, s32 n);
char* strcat(char* dest, const char* src);
char* strncat(char* dest, const char* src, s32 n);
s32   strcmp(const char* str1, const char* str2);
s32   strncmp(const char* str1, const char* str2, s32 n);
char* strchr(const char* str, s32 chr);

#endif /* STRING_H */