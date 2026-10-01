#include <cri/cri_xpt.h>

// NITROCI/NITRO: the CVFS device for the NITRO-SDK file system ("NITRO"),
// the NITRO counterpart of gcci.c / dvci.c. Only the error hook is in C so far.

struct {
    int f_0;
    void (*fn)(void* obj, const char* msg, void* arg);
    void* obj;
    int   f_c;
} data_02070a48 = {0, 0, 0, 0};

void nitroci_call_errfn(void* arg, const char* msg) {
    if (data_02070a48.fn != 0) {
        data_02070a48.fn(data_02070a48.obj, msg, arg);
    }
}
