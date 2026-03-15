#include "Engine/Color.h"
#include "common_data.h"
#include <NitroSDK/mi/cpumem.h>

extern u16 data_02059d24;

void Color_CopyRange(u16* dest, u16* src, s32 destOffset, u32 count, BOOL skipFirst) {
    if ((skipFirst == TRUE) && (destOffset == 0)) {
        if (count <= 1) {
            return;
        }
        destOffset++;
        count--;
        src++;
    }
    MI_CpuCopyU16(src, dest + destOffset, count * sizeof(u16));
}

static void func_02001c74(u16* arg0, u16* arg1, u32 arg2, s32 arg3) {
    s32 var_r3 = (arg3 < 0x20) ? (~arg3 & 0x1F) : 0x20;

    while (arg2-- != 0) {
        s32 var_r7 = 0;
        s32 var_r6 = 31;

        do {
            var_r7 |= var_r6 & ((var_r3 * (arg1[arg2] & var_r6)) >> 5);
            var_r6 <<= 5;
        } while ((var_r6 >> 0xF) == 0);

        arg0[arg2] = var_r7;
    }
}

static void func_02001ce0(u16* arg0, u16* arg1, u32 arg2, s32 arg3) {
    if (arg3 < 32) {
        arg3 = ~arg3 & 0x1F;
    } else {
        arg3 = 32;
    }

    while (arg2-- != 0) {
        u32 var_ip = 0;
        s32 var_r3 = 31;

        do {
            s32 temp_r4 = var_r3 & ~((s32)(arg3 * (~arg1[arg2] & var_r3)) >> 5);
            var_r3 <<= 5;
            var_ip |= temp_r4;
        } while ((var_r3 >> 0xF) == 0);

        arg0[arg2] = var_ip;
    }
}

void func_02001d54(u16* arg0, u16* arg1, u16* arg2, u32 arg3, u32 arg4) {
    u32 var_r8;
    s32 var_r7;
    s32 temp_ip;
    s32 temp_r10;

    while (arg3-- != 0) {
        var_r8 = 0;
        var_r7 = 31;

        do {
            temp_ip  = arg2[arg3] & var_r7;
            temp_r10 = var_r7 & (temp_ip + ((s32)(arg4 * ((arg1[arg3] & var_r7) - temp_ip)) >> 5));
            var_r7 <<= 5;
            var_r8 |= temp_r10;
        } while ((var_r7 >> 0xF) == 0);

        arg0[arg3] = var_r8;
    }
}

void func_02001dc4(u16* arg0, u16* arg1, u32 arg2, s32 arg3) {
    while (arg2-- != 0) {
        u16 temp_r4 = arg1[arg2];
        u32 var_r7  = 0;
        s32 var_r6  = 31;

        do {
            s32 temp_r9 = temp_r4 & var_r6;
            s32 temp_r8 = var_r6 & (temp_r9 + ((u32)(arg3 * ((~temp_r4 & var_r6) - temp_r9)) >> 5));
            var_r7 |= temp_r8;
            var_r6 <<= 5;
        } while ((var_r6 >> 0xF) == 0);

        arg0[arg2] = var_r7;
    }
}

void Color_BlendToGreyscale(u16* dest, u16* src, u32 index, u32 blendStrength) {
    if (blendStrength >= 32) {
        blendStrength = 32;
    }

    while (index-- != 0) {
        u32 red   = Color_UnpackRed(src[index]);
        u32 green = Color_UnpackGreen(src[index]);
        u32 blue  = Color_UnpackBlue(src[index]);
        u32 alpha = Color_UnpackAlpha(src[index]);

        s32 gray = (red * 1225) + (green * 2404) + (blue * 467);

        switch (blendStrength) {
            case 0:
                dest[index] = src[index];
                break;
            case 32:
                dest[index] = Color_PackRGB(F2I(gray), F2I(gray), F2I(gray), alpha);
                break;
            default: {
                u32 r = Color_BlendChannel(red, gray, blendStrength) & 0x1F;
                u32 g = Color_BlendChannel(green, gray, blendStrength) & 0x1F;
                u32 b = Color_BlendChannel(blue, gray, blendStrength) & 0x1F;

                dest[index] = Color_PackRGB(r, g, b, alpha);
            }
        }
    }
}

void func_02001f34(u16* arg0, u16* arg1, u32 arg2, u32 arg3) {
    if (arg3 >= 32) {
        arg3 = 32;
    }

    while (arg2-- != 0) {
        u32 red   = Color_UnpackRed(arg1[arg2]);
        u32 green = Color_UnpackGreen(arg1[arg2]);
        u32 blue  = Color_UnpackBlue(arg1[arg2]);
        u32 alpha = Color_UnpackAlpha(arg1[arg2]);

        s32 gray = (red * 1225) + (green * 2404) + (blue * 467);

        switch (arg3) {
            case 0:
                arg0[arg2] = arg1[arg2];
                break;
            case 32: {
                s32 gray_bit = (gray < 0x10000) ? 0 : 0x7FFF;
                arg0[arg2]   = gray_bit | (alpha << 15);
                break;
            }
            default: {
                s32 gray_component = (gray < 0x10000) ? 0 : 0x1F000;
                u32 r              = Color_BlendChannel(red, gray_component, arg3);
                u32 g              = Color_BlendChannel(green, gray_component, arg3);
                u32 b              = Color_BlendChannel(blue, gray_component, arg3);

                arg0[arg2] = Color_PackRGB(r, g, b, alpha);
            }
        }
    }
}

void func_0200204c(u16* arg0, u16* arg1, u32 arg2, u32 blendStrength) {
    if (blendStrength >= 32) {
        blendStrength = 32;
    }

    while (arg2-- != 0) {
        u32 red     = Color_UnpackRed(arg1[arg2]);
        u32 green   = Color_UnpackGreen(arg1[arg2]);
        u32 blue    = Color_UnpackBlue(arg1[arg2]);
        u32 alpha   = Color_UnpackAlpha(arg1[arg2]);
        s32 temp_r9 = (red * 1225) + (green * 2404) + (blue * 467);

        switch (blendStrength) {
            case 0: {
                arg0[arg2] = arg1[arg2];
            } break;

            case 32: {
                arg0[arg2] = Color_PackRGB(F2I(temp_r9), F2I(temp_r9), F2I(temp_r9), alpha);
            } break;

            default: {
                u32 r      = Color_BlendChannel(red, F2I(temp_r9 * 0xF80), blendStrength) & 0x1F;
                u32 g      = (Color_BlendChannel(green, F2I(temp_r9 * 0xD00), blendStrength) & 0x1F);
                u32 b      = (Color_BlendChannel(blue, F2I(temp_r9 * 0xB00), blendStrength) & 0x1F);
                arg0[arg2] = Color_PackRGB(r, g, b, alpha);

            } break;
        }
    }
}

void func_02002180(u16* arg0, u16* arg1, u16* arg2, s32 arg3, u32 arg4, u32 arg5, s32 arg6) {
    if ((arg6 == 1) && (arg3 == 0)) {
        if (arg4 <= 1) {
            return;
        }
        arg3 += 1;
        arg4 -= 1;
        arg1 += 1;
        arg2 += 1;
    }

    u16* temp_r3 = arg0 + arg3;
    if (arg5 == 0) {
        MI_CpuCopyU16(arg2, temp_r3, arg4 * 2);
        return;
    }
    if (arg5 > 31) {
        MI_CpuCopyU16(arg1, temp_r3, arg4 * 2);
        return;
    }
    if (arg1 == (u16*)&data_0205a128) {
        func_02001c74(temp_r3, arg2, arg4, arg5);
        return;
    }
    if (arg1 == &data_02059d24) {
        func_02001ce0(temp_r3, arg2, arg4, arg5);
        return;
    }
    func_02001d54(temp_r3, arg1, arg2, arg4, arg5);
}

void func_02002254(u16* arg0, u16* arg1, u16* arg2, s32 arg3, u32 arg4, u32 arg5, s32 arg6, s32 arg7) {
    if ((arg7 == 1) && (arg3 == 0)) {
        if (arg4 <= 1) {
            return;
        }
        arg3 += 1;
        arg4 -= 1;
        arg1 += 1;
        arg2 += 1;
    }
    u16* temp_r4 = arg0 + arg3;
    if (arg5 == 0) {
        MI_CpuCopyU16(arg2, temp_r4, arg4 * 2);
    } else if (arg5 > 31) {
        MI_CpuCopyU16(arg1, temp_r4, arg4 * 2);
    } else if (arg1 == (u16*)&data_0205a128) {
        func_02001c74(temp_r4, arg2, arg4, arg5);
    } else if (arg1 == &data_02059d24) {
        func_02001ce0(temp_r4, arg2, arg4, arg5);
    } else {
        func_02001d54(temp_r4, arg1, arg2, arg4, arg5);
    }
    if (arg6 > 0) {
        func_02002180(arg0, &data_02059d24, temp_r4, arg3, arg4, arg6, arg7);
        return;
    }
    if (arg6 >= 0) {
        return;
    }
    func_02002180(arg0, (u16*)&data_0205a128, temp_r4, arg3, arg4, -arg6, arg7);
}

void func_02002398(u16* arg0, u16* arg1, s32 arg2, u32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    if ((arg6 == 1) && (arg2 == 0)) {
        if (arg3 <= 1) {
            return;
        }
        arg2 += 1;
        arg3 -= 1;
        arg1 += 1;
    }
    u16* temp_r6 = arg0 + arg2;
    func_02001dc4(temp_r6, arg1, arg3, arg4);
    if (arg5 > 0) {
        func_02002180(arg0, &data_02059d24, temp_r6, arg2, arg3, arg5, arg6);
        return;
    }
    if (arg5 >= 0) {
        return;
    }
    func_02002180(arg0, (u16*)&data_0205a128, temp_r6, arg2, arg3, -arg5, arg6);
}

void func_0200245c(u16* arg0, u16* arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    if ((arg6 == 1) && (arg2 == 0)) {
        if (arg3 <= 1) {
            return;
        }
        arg2 += 1;
        arg3 -= 1;
        arg1 += 1;
    }

    Color_BlendToGreyscale(arg0 + arg2, arg1, arg3, arg4);
    if (arg5 > 0) {
        func_02002180(arg0, &data_02059d24, arg0 + arg2, arg2, arg3, arg5, arg6);
    } else if (arg5 < 0) {
        func_02002180(arg0, (u16*)&data_0205a128, arg0 + arg2, arg2, arg3, -arg5, arg6);
    }
}

void func_02002520(u16* arg0, u16* arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    if ((arg6 == 1) && (arg2 == 0)) {
        if (arg3 <= 1U) {
            return;
        }
        arg2 += 1;
        arg3 -= 1;
        arg1 += 1;
    }

    func_02001f34(arg0 + arg2, arg1, arg3, arg4);
    if (arg5 > 0) {
        func_02002180(arg0, &data_02059d24, arg0 + arg2, arg2, arg3, arg5, arg6);
    } else if (arg5 < 0) {
        func_02002180(arg0, (u16*)&data_0205a128, arg0 + arg2, arg2, arg3, -arg5, arg6);
    }
}

void func_020025e4(u16* arg0, u16* arg1, s32 arg2, u32 arg3, u32 arg4, s32 arg5, s32 arg6) {
    if ((arg6 == 1) && (arg2 == 0)) {
        if (arg3 <= 1) {
            return;
        }
        arg2++;
        arg3--;
        arg1++;
    }

    func_0200204c(arg0 + arg2, arg1, arg3, arg4);
    if (arg5 > 0) {
        func_02002180(arg0, &data_02059d24, arg0 + arg2, arg2, arg3, arg5, arg6);
    } else if (arg5 < 0) {
        func_02002180(arg0, (u16*)&data_0205a128, arg0 + arg2, arg2, arg3, -arg5, arg6);
    }
}
