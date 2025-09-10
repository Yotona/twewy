#include <MSL/Common/mem_funcs.h>

#define cps ((u8*)src)
#define cpd ((u8*)dst)
#define lps ((u32*)src)
#define lpd ((u32*)dst)

#define deref_auto_inc(p) *(p)++

void __fill_mem(void* dst, s32 val, u32 n) {
    u32 v = (u8)val;
    u32 i;

    if (n >= 32) {
        i = (-(u32)dst) & 3;

        if (i) {
            n -= i;
            do {
                deref_auto_inc(cpd) = v;
            } while (--i);
        }

        if (v) {
            v |= v << 24 | v << 16 | v << 8;
        }

        i = n >> 5;

        if (i) {
            do {
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
                deref_auto_inc(lpd) = v;
            } while (--i);
        }

        i = (n & 31) >> 2;

        if (i) {
            do {
                deref_auto_inc(lpd) = v;
            } while (--i);
        }

        n &= 3;
    }

    if (n) {
        do {
            deref_auto_inc(cpd) = v;
        } while (--n);
    }
}