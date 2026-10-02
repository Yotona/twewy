#include <mem.h>

// Only the RIFF scanner of the WAVE decoder survives on NITRO; it is the WAVE
// counterpart of ADX_ScanInfoCode and sits last in the Wii adx_bwav.c.

int ADX_ScanInfoCodeWav(char* ibuf, int ibuflen, short* dlen) {
    int ptr;
    int minptr = 0x7FFFFFFF;

    for (ptr = 0; ptr < ibuflen - 3; ptr++) {
        if (memcmp(&ibuf[ptr], "RIFF", 4) == 0) {
            minptr = (ptr < minptr) ? ptr : minptr;
            break;
        }
    }

    if (minptr != 0x7FFFFFFF) {
        *dlen = minptr;
        return 0;
    } else {
        *dlen = 0;
        return -1;
    }
}
