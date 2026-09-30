#include <cri/private/adx_rna.h>

struct {
    void* arg;
    void (*fn)(void* arg, void* obj);
} data_020713b4 = {0, 0};

void func_0201d558(ADXRNA rna) {
    if (data_020713b4.fn != 0) {
        data_020713b4.fn(data_020713b4.arg, rna);
    }
}
