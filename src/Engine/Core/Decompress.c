#include "Engine/Core/Decompress.h"

u32 func_02004d60(void* dest, const void* src) {
    u32 size  = (*(u32*)src & ~0xFF) >> 8;
    u8* bytes = (u8*)src;

    if ((*(u8*)src & 0xF0) == 0) {
        size -= 4;
    }

    if ((u32)dest >= 0x05000000 && (u32)dest + size <= 0x08000000) {
        return func_02004e0c(dest, src);
    }

    switch (*bytes & 0xF0) {
        case 0x10:
            func_0203b59c(src, dest);
            break;
        case 0x20:
            func_0203b708(src, dest);
            break;
        case 0x30:
            func_0203b7e0(src, dest);
            break;
        default:
            MI_CpuCopyU8(((const u8*)src) + 4, dest, size);
            break;
    }

    return size;
}

u32 func_02004e0c(void* dest, const u32* src) {
    u32 size  = (*src & ~0xFF) >> 8;
    u8  head  = *(u8*)src;
    u8* bytes = (u8*)src;

    if ((head & 0xF0) == 0) {
        size -= 4;
    }
    switch (*bytes & 0xF0) {
        case 0x10:
            func_0203b630(src, (s16*)dest);
            break;

        case 0x20:
            func_0203b708(src, dest);
            break;

        case 0x30:
            func_0203b84c(src, (s16*)dest);
            break;

        default:
            MI_CpuCopyU8(src + 1, dest, size);
            break;
    }

    return size;
}
