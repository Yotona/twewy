#include <MSL/ARM/critical_regions.h>
#include <MSL/Common/errno.h>
#include <MSL/Common/signal.h>

__signal_func_ptr signal_funcs[__msl_signal_max];

int raise(int signal) {
    __signal_func_ptr signal_func;

    if (signal < 1 || signal > __msl_signal_max)
        return (-1);

    __begin_critical_region(signal_funcs_access);

    signal_func = signal_funcs[signal - 1];

    if (signal_func != SIG_IGN)
        signal_funcs[signal - 1] = SIG_DFL;

    __end_critical_region(signal_funcs_access);

    if (signal_func == SIG_IGN || (signal_func == SIG_DFL && signal == SIGABRT))
        return 0;

    if (signal_func == SIG_DFL)
        exit(0);

    signal_func(signal);

    return 0;
}
