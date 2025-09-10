#include <MSL/Runtime/CompressedNumbers.h>

char* __DecodeSignedNumber(char* p, long* pnum) {
    signed long x, y, z, w;

    x = ((signed char*)p)[0];

    if ((x & 0x01) == 0) {
        *pnum = (x >> 1);
        return (p + 1);
    }

    y = ((unsigned char*)p)[1];

    if ((x & 0x02) == 0) {
        *pnum = (((x >> 2) << 8) | y);
        return (p + 2);
    }

    z = ((unsigned char*)p)[2];

    if ((x & 0x04) == 0) {
        *pnum = (((x >> 3) << 16) | (y << 8) | z);
        return (p + 3);
    }

    w = ((unsigned char*)p)[3];

    *pnum = (((x >> 3) << 24) | (y << 16) | (z << 8) | w);
    return (p + 4);
}

char* __DecodeUnsignedNumber(char* p, unsigned long* pnum) {
    unsigned long x, y, z, w;

    x = ((unsigned char*)p)[0];

    if ((x & 0x01) == 0) {
        *pnum = (x >> 1);
        return (p + 1);
    }

    y = ((unsigned char*)p)[1];

    if ((x & 0x02) == 0) {
        *pnum = (((x >> 2) << 8) | y);
        return (p + 2);
    }

    z = ((unsigned char*)p)[2];

    if ((x & 0x04) == 0) {
        *pnum = (((x >> 3) << 16) | (y << 8) | z);
        return (p + 3);
    }

    w = ((unsigned char*)p)[3];

    *pnum = (((x >> 3) << 24) | (y << 16) | (z << 8) | w);
    return (p + 4);
}