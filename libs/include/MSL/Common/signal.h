#ifndef MSL_COMMON_SIGNAL_H
#define MSL_COMMON_SIGNAL_H

#define SIGABRT  1
#define SIGFPE   2
#define SIGILL   3
#define SIGINT   4
#define SIGSEGV  5
#define SIGTERM  6
#define SIGBREAK 7

#define __msl_signal_max 7

typedef void (*__signal_func_ptr)(int);
#define SIG_DFL ((__signal_func_ptr)0)
#define SIG_IGN ((__signal_func_ptr)1)
#define SIG_ERR ((__signal_func_ptr) - 1)

#endif // MSL_COMMON_SIGNAL_H