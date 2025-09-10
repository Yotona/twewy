#include <MSL/Common/ansi_files.h>
#include <MSL/Common/console_io.h>

FILE __files[3] = {
    {0,
     {__must_exist, __write, 1, __console_file, __unoriented, 0},
     {__neutral, 0, 0, 0},
     0, 0,
     {0, 0},
     {0, 0},
     0,  stdin_buff,
     sizeof(stdin_buff),
     stdin_buff, 0,
     0, 0,
     0, NULL,
     __read_console, __write_console,
     __close_console, NULL},
    {1,
     {__must_exist, __read_write, 1, __console_file, __unoriented, 0},
     {__neutral, 0, 0, 0},
     0, 0,
     {0, 0},
     {0, 0},
     0, stdout_buff,
     sizeof(stdout_buff),
     stdout_buff, 0,
     0, 0,
     0, NULL,
     __read_console, __write_console,
     __close_console, NULL},
    {2,
     {__must_exist, __read_write, 0, __console_file, __unoriented, 0},
     {__neutral, 0, 0, 0},
     0, 0,
     {0, 0},
     {0, 0},
     0, stderr_buff,
     sizeof(stderr_buff),
     stderr_buff, 0,
     0, 0,
     0, NULL,
     __read_console, __write_console,
     __close_console, NULL},
};

s32 __flush_all(void) {
    s32   result     = 0;
    FILE* file       = &__files[0];
    s32   file_index = 1;

    do {
        if (file->mode.file_kind != __closed_file) {
            if (fflush(file) != 0) {
                result = -1;
            }
        }
        if (file_index < 3) {
            file = &__files[file_index++];
        } else {
            file = NULL;
        }
    } while (file != NULL);

    return result;
}