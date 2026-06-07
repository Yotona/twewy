#ifndef _C_CONSOLE_IO_H
#define _C_CONSOLE_IO_H

#include <file_struc.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int __read_console(__file_handle handle, unsigned char* buffer, int* count, void* ref);
extern int __write_console(__file_handle handle, unsigned char* buffer, int* count, void* ref);
extern int __close_console();

#ifdef __cplusplus
}
#endif

#endif