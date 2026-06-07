#include <abort_exit_arm_eabi.h>
#include <critical_regions.h>

typedef void (*sig_func)(int sig);
sig_func signal_funcs[7];

int raise(int sig) {
    sig_func signal_func;

    if (sig < 1 || sig > 7) {
        return -1;
    }

    __begin_critical_region(signal_funcs_access);

    signal_func = signal_funcs[sig - 1];

    if ((unsigned int)signal_func != 1) {
        signal_funcs[sig - 1] = 0;
    }

    __end_critical_region(signal_funcs_access);

    if ((unsigned int)signal_func == 1 || ((int)signal_func == 0 && sig == 1)) {
        return 0;
    }

    if ((unsigned int)signal_func == 0) {
        exit(0);
    }

    signal_func(sig);
    return 0;
}