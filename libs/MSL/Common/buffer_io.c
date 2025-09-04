#include <MSL/Common/file_struct.h>

void __convert_from_newlines(u8* p, s32* n) {
    return;
}

void __prep_buffer(FILE* file) {
    file->buffer_ptr = file->buffer;
    file->buffer_len = file->buffer_size;
    file->buffer_len -= file->position & file->buffer_alignment;
    file->buffer_pos = file->position;
}

s32 __flush_buffer(FILE* file, s32* bytes_flushed) {
    s32 buffer_len;
    s32 io_result;

    buffer_len = file->buffer_ptr - file->buffer;
    if (buffer_len != 0) {
        file->buffer_len = buffer_len;

        if (file->mode.binary_io == 0) {
            __convert_from_newlines(file->buffer, &file->buffer_len);
        }
        io_result = file->write_proc(file->handle, file->buffer, &file->buffer_len, file->ref);

        if (bytes_flushed != NULL) {
            *bytes_flushed = file->buffer_len;
        }
        if (io_result != 0) {
            return io_result;
        }
        file->position += file->buffer_len;
    }

    __prep_buffer(file);
    return 0;
}