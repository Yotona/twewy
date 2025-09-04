#ifndef RUNTIME_SEMIHOST_H
#define RUNTIME_SEMIHOST_H

#include "types.h"

#define SEMIHOST_INTERRUPT 0x123456

#define SYS_WRITECHAR 0x03
#define SYS_READCHAR  0x07
#define SYS_EXIT      0x18

/**
 * @brief Terminate the application via semihosting interface
 */
extern void sys_Exit(void);

#endif // RUNTIME_SEMIHOST_H