#include <MSL/Common/file_struct.h>

int fflush(FILE* file) {
    if (file == NULL) {
        return __flush_all();
    }

    if (file->state.error || file->mode.file_kind == __closed_file) {
        return -1;
    }

    if (file->mode.io_mode == __read) {
        return 0;
    }

    if (file->state.io_state >= __rereading) {
        file->state.io_state = __reading;
    }

    if (file->state.io_state == __reading) {
        file->buffer_len = 0;
    }

    if (file->state.io_state != __writing) {
        file->state.io_state = __neutral;
        return 0;
    }

    if (__flush_buffer(file, NULL)) {
        set_error(file);
        return -1;
    }

    file->state.io_state = __neutral;
    file->position       = 0;
    file->buffer_len     = 0;
    return 0;
}