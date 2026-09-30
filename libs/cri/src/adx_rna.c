#include <cri/cri_xpt.h>
#include <cri/private/adx_rna.h>

struct {
    int f_0;
    void (*fn)(void* arg, void* obj);
    void* arg;
    int   f_c;
} data_02070a48 = {0, 0, 0, 0};

void func_0201bf78(ADXRNA rna) {
    if (data_02070a48.fn != 0) {
        data_02070a48.fn(data_02070a48.arg, rna);
    }
}
