#ifndef COMMON_FILE_STRUCT_H
#define COMMON_FILE_STRUCT_H

#include "types.h"

enum __file_kinds {
    __closed_file,
    __disk_file,
    __console_file,
    __unavailable_file
};

enum __open_modes {
    __must_exist,
    __create_if_necessary,
    __create_or_truncate
};

enum __file_orientation {
    __unoriented,
    __char_oriented,
    __wide_oriented
};

enum __io_modes {
    __read,
    __write,
    __read_write,
    __append,
};

enum __io_states {
    __neutral,
    __writing,
    __reading,
    __rereading
};

typedef struct __file_modes {
    u32 open_mode        : 2;
    u32 io_mode          : 3;
    u32 buffer_mode      : 2;
    u32 file_kind        : 3;
    u32 file_orientation : 2;
    u32 binary_io        : 1;
} file_modes;

typedef struct __file_state {
    u32 io_state    : 3;
    u32 free_buffer : 1;
    u8  eof;
    u8  error;
} file_state;

typedef struct _FILE FILE;
extern FILE          __files[];

typedef int (*__pos_proc)(u32 file, u32* position, s32 mode, void* ref);
typedef int (*__io_proc)(u32 file, u8* buffer, s32* count, void* ref);
typedef int (*__close_proc)(u32 file);

struct _FILE {
    u32          handle;
    file_modes   mode;
    file_state   state;
    u8           char_buffer;
    u8           char_buffer_overflow;
    u8           ungetc_buffer[2];
    u16          ungetwc_buffer[2];
    u32          position;
    u8*          buffer;
    u32          buffer_size;
    u8*          buffer_ptr;
    u32          buffer_len;
    u32          buffer_alignment;
    u32          saved_buffer_len;
    u32          buffer_pos;
    __pos_proc   position_proc;
    __io_proc    read_proc;
    __io_proc    write_proc;
    __close_proc close_proc;
    void*        ref;
}; // size: 0x4C

#define set_error(file)          \
    do {                         \
        (file)->state.error = 1; \
        (file)->buffer_len  = 0; \
    } while (FALSE)

#define stdin  (&__files[0])
#define stdout (&__files[1])
#define stderr (&__files[2])

#endif // COMMON_FILE_STRUCT_H