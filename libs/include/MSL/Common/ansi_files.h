#ifndef MSL_COMMON_ANSI_FILES_H
#define MSL_COMMON_ANSI_FILES_H

#include "types.h"
#include <MSL/Common/file_struct.h>

typedef u8          console_buff[256];
static console_buff stdin_buff;
static console_buff stdout_buff;
static console_buff stderr_buff;

#endif // MSL_COMMON_ANSI_FILES_H