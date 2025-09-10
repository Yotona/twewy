#include <MSL/Common/string.h>

#define K1 0x80808080
#define K2 0xFEFEFEFF

s32 strlen(const char* str) {
    s32 len = -1;
    do {
        len++;
    } while (*str++ != 0);
    return len;
}

char* strcpy(char* dest, const char* src) {
    register u8 *destb, *fromb;
    register u32 w, t, align;
    register u32 k1, k2;

    fromb = (u8*)src;
    destb = (u8*)dest;

    if ((align = ((u32)fromb & 3)) != ((u32)destb & 3)) {
        goto bytecopy;
    }

    if (align != 0) {
        if ((*destb = *fromb) == 0) {
            return dest;
        }

        for (align = 3 - align; align != 0; align--) {
            if ((*++destb = *++fromb) == 0) {
                return dest;
            }
        }
        ++destb;
        ++fromb;
    }

    k1 = K1;
    k2 = K2;

    w = *((s32*)fromb);
    t = w + k2;
    t &= ~w;
    t &= k1;

    if (t != 0) {
        goto bytecopy;
    }

    --((s32*)(destb));

    do {
        *(++((s32*)destb)) = w;
        w                  = *(++((s32*)fromb));
        t                  = w + k2;
        t &= ~w;
        t &= k1;
        if (t != 0) {
            goto adjust;
        }
    } while (TRUE);

adjust:
    ++((s32*)destb);
bytecopy:
    if ((*destb = *fromb) == 0) {
        return dest;
    }

    do {
        if ((*++destb = *++fromb) == 0) {
            return dest;
        }
    } while (TRUE);

    return dest;
}

char* strncpy(char* dest, const char* src, s32 n) {
    const char* p = src;
    char*       q = dest;

    n++;

    while (--n != 0) {
        if ((*q++ = *p++) == 0) {
            while (--n != 0) {
                *q++ = 0;
            }
            break;
        }
    }

    return dest;
}

char* strcat(char* dest, const char* src) {
    const char* p = src;
    char*       q = dest;

    while (*q++ != 0)
        ;
    q--;
    while ((*q++ = *p++) != 0)
        ;
    return dest;
}

char* strncat(char* dest, const char* src, s32 n) {
    const char* p = src;
    char*       q = dest;

    while (*q++ != 0)
        ;
    q--;
    n++;

    while (--n != 0) {
        if ((*q++ = *p++) == 0) {
            q--;
            break;
        }
    }

    *q = 0;

    return dest;
}

s32 strcmp(const char* str1, const char* str2) {
    register u8* left  = (u8*)str1;
    register u8* right = (u8*)str2;
    u32          k1, k2, align, l1, r1, x;

    l1 = *left;
    r1 = *right;
    if (l1 - r1) {
        return (l1 - r1);
    }

    if ((align = ((u32)left & 3)) != ((u32)right & 3)) {
        goto bytecopy;
    }
    if (align) {
        if (l1 == 0) {
            return (0);
        }
        for (align = 3 - align; align; align--) {
            l1 = *(++left);
            r1 = *(++right);
            if (l1 - r1) {
                return (l1 - r1);
            }
            if (l1 == 0) {
                return (0);
            }
        }
        left++;
        right++;
    }

    k1 = K1;
    k2 = K2;

    l1 = *(u32*)left;
    r1 = *(u32*)right;
    x  = l1 + k2;
    x &= ~l1;
    if (x & k1) {
        goto adjust;
    }
    while (l1 == r1) {
        l1 = *(++((u32*)(left)));
        r1 = *(++((u32*)(right)));
        x  = l1 + k2;
        if (x & k1) {
            goto adjust;
        }
    }

    --left;
    --right;
    goto bytecopy;

adjust:
    l1 = *left;
    r1 = *right;
    if (l1 - r1) {
        return (l1 - r1);
    }
bytecopy:
    if (l1 == 0) {
        return 0;
    }

    do {
        l1 = *(++left);
        r1 = *(++right);
        if (l1 - r1) {
            return (l1 - r1);
        }
        if (l1 == 0) {
            return 0;
        }
    } while (TRUE);
}

s32 strncmp(const char* str1, const char* str2, s32 n) {
    const u8* p = (u8*)str1;
    const u8* q = (u8*)str2;
    u8        c1, c2;

    n++;

    while (--n != 0) {
        if ((c1 = *p++) != (c2 = *q++)) {
            return (c1 - c2);
        } else if (c1 == 0) {
            break;
        }
    }

    return 0;
}

char* strchr(const char* str, s32 chr) {
    const char* p = str;
    char        c = chr;
    char        ch;

    while (ch = *p++) {
        if (ch == c) {
            return ((char*)(p - 1));
        }
    }

    return (c ? 0 : (char*)(p - 1));
}
