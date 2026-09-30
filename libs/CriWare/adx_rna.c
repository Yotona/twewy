#include <CriWare/private/adx_rna.h>
#include <nitro/types.h>

// adx_rna.c -- ADXRNA (Audio Renderer) callback hook.
//
// delinks.txt gives this TU .text 0x0201bf78-0x0201bfa0 and .bss
// 0x02070a48-0x02070a58: one function, func_0201bf78 (0x28 bytes), holding a
// 16-byte object at 0x02070a48. The extents are corroborated three ways --
// symbols.txt has the next function func_0201bfa0 at exactly 0x0201bf78+0x28,
// symbols.txt has data_02070a58 at the end of that bss run, and the reloc at
// 0x0201bf9c (inside func_0201bf78) loads 0x02070a48.
//
// This TU had no source file at all, so nothing that calls into ADXRNA had a
// definition; the range was being filled by a generated _dsd_gap object and was
// invisible to objdiff.
//
// Function names are address-based on purpose: objdiff pairs target and base
// symbols by name, so renaming would drop the function out of the report even
// if the code were byte-identical. The host installs .fn and .arg.

struct {
    s32 f_0;
    void (*fn)(void* arg, void* obj);
    void* arg;
    s32   f_c;
} data_02070a48 = {0, 0, 0, 0};

void func_0201bf78(ADXRNA rna) {
    if (data_02070a48.fn != 0) {
        data_02070a48.fn(data_02070a48.arg, rna);
    }
}
