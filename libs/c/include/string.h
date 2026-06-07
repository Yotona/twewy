#ifndef _C_STRING_H
#define _C_STRING_H

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

size_t strlen(const char* str);
char*  strcpy(char* dest, const char* src);
char*  strncpy(char* dest, const char* src, size_t num);
int    strcmp(char* str1, char* str2);
int    strncmp(char* str1, char* str2, size_t num);
char*  strcat(char* dest, const char* src);
char*  strncat(char* dest, const char* src, int num);
char*  strchr(const char* str, int chr);

#ifdef __cplusplus
}
#endif

#endif