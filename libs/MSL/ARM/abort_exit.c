#include <MSL/ARM/abort_exit.h>
#include <MSL/ARM/critical_regions.h>
#include <MSL/Common/signal.h>
#include <MSL/Runtime/MWException.h>

static void (*__atexit_funcs[64])(void);
static int __atexit_curr_func = 0;

void (*__stdio_exit)(void)   = NULL;
void (*__console_exit)(void) = NULL;

BOOL __aborting = FALSE;

void abort(void) {
    raise(SIGABRT);
    __aborting = TRUE;
    exit(EXIT_FAILURE);
}

void exit(int status) {
    if (__aborting == FALSE) {
        __destroy_global_chain();

        if (__stdio_exit != NULL) {
            __stdio_exit();
            __stdio_exit = NULL;
        }
    }
    __exit(status);
}

void __exit(int status) {
    __begin_critical_region(atexit_funcs_access);

    while (__atexit_curr_func > 0) {
        __atexit_funcs[--__atexit_curr_func]();
    }

    __end_critical_region(atexit_funcs_access);

    if (__console_exit != NULL) {
        __console_exit();
        __console_exit = NULL;
    }

    fflush(NULL);

    _ExitProcess();
}
