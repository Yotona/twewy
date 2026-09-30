#include <CriWare/private/adx_rna.h>

// adx_rna2.c -- second ADXRNA callback hook, same shape as adx_rna.c but with
// the argument word ahead of the function pointer.
//
// delinks.txt gives this TU .text 0x0201d558-0x0201d580 and .bss
// 0x020713b4-0x020713bc: one function, func_0201d558 (0x28 bytes), holding an
// 8-byte object at 0x020713b4. Same corroboration as adx_rna.c -- symbols.txt
// puts func_0201d580 at exactly 0x0201d558+0x28, and the reloc at 0x0201d57c
// (inside func_0201d558) loads 0x020713b4.
//
// Like adx_rna.c this had no source file; the range was gap-filled and
// unmeasurable. Function names stay address-based -- see adx_rna.c.

struct {
    void* arg;
    void (*fn)(void* arg, void* obj);
} data_020713b4 = {0, 0};

void func_0201d558(ADXRNA rna) {
    if (data_020713b4.fn != 0) {
        data_020713b4.fn(data_020713b4.arg, rna);
    }
}
