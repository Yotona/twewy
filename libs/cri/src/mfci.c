// MFCI/NITRO: the CVFS memory-file device ("MFS"). Only the error hook is in
// C so far.

struct {
    void* obj;
    void (*fn)(void* obj, const char* msg, void* arg);
} data_020713b4 = {0, 0};

void mfci_call_errfn(void* arg, const char* msg) {
    if (data_020713b4.fn != 0) {
        data_020713b4.fn(data_020713b4.obj, msg, arg);
    }
}
