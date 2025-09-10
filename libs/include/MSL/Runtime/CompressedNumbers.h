#ifndef MSL_RUNTIME_COMPRESSEDNUMBERS_H
#define MSL_RUNTIME_COMPRESSEDNUMBERS_H

inline char* __DecodeAddress(char* p, void** paddr) {
    unsigned long addr;

    addr = ((unsigned char*)p)[0] | (((unsigned char*)p)[1] << 8) | (((unsigned char*)p)[2] << 16) |
           (((unsigned char*)p)[3] << 24);

    *paddr = (void*)addr;
    return (p + 4);
}

extern char* __DecodeSignedNumber(char* p, long* pnum);
extern char* __DecodeUnsignedNumber(char* p, unsigned long* pnum);

#endif // MSL_RUNTIME_COMPRESSEDNUMBERS_H