#ifndef COMMON_CONSOLE_IO_H
#define COMMON_CONSOLE_IO_H

#include <types.h>

extern int __read_console(u32 handle, u8* buffer, s32* count, void* ref);
extern int __write_console(u32 handle, u8* buffer, s32* count, void* ref);
extern int __close_console();

#endif // COMMON_CONSOLE_IO_H