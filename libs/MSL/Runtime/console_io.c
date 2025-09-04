#include <MSL/Common/console_io.h>
#include <MSL/Runtime/semihost.h>

// clang-format off
static asm void sys_WriteChar(register void* c) {
    str     lr, [sp, #-4]!
    mov     r1, c
    mov     r0, #SYS_WRITECHAR
    swi     SEMIHOST_INTERRUPT
    ldr     pc, [sp], #4
}

static asm int sys_ReadChar(void) {
    str     lr, [sp, #-4]!
    mov     r1, #0
    mov     r0, #SYS_READCHAR
    swi     SEMIHOST_INTERRUPT
    ldr     pc, [sp], #4
}

asm void sys_Exit(void) {
    mov     r1, #0
    mov     r0, #SYS_EXIT
    swi     SEMIHOST_INTERRUPT
    mov     pc, lr
}
// clang-format on

int __read_console(u32 handle, u8* buffer, s32* count, void* ref) {
    u32 i, n;

    for (i = 0, n = *count; i < n; i++) {
        buffer[i] = sys_ReadChar();
        if (buffer[i] == '\r' || buffer[i] == '\n') {
            *count = i + 1;
            break;
        }
    }

    return 0;
}

int __write_console(u32 handle, u8* buffer, s32* count, void* ref) {
    u32 i, n;

    for (i = 0, n = *count; i < n; i++) {
        sys_WriteChar(&buffer[i]);
    }

    return 0;
}

int __close_console() {
    return 0;
}