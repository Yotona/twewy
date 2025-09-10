#include <MSL/Common/locale_api.h>

s32 mbtowc(s16* pwc, const char* s, s32 n) {
    return _current_locale.ctype_cmpt_ptr->decode_mb(pwc, s, n);
}

s32 __mbtowc_noconv(u16* pwc, const char* s, s32 n) {
    if (s == NULL) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    if (pwc != NULL) {
        *pwc = (u8)*s;
    }

    if (*s == 0) {
        return 0;
    }

    return 1;
}

s32 __wctomb_noconv(char* s, u16 wchar) {
    if (s == NULL) {
        return 0;
    }
    *s = wchar;
    return 1;
}

s32 wctomb(char* s, u16 wchar) {
    return _current_locale.ctype_cmpt_ptr->encode_wc(s, wchar);
}

s32 wcstombs(char* s, const u16* pwcs, u32 n) {
    int  chars_written = 0;
    int  result;
    char temp[3];
    u16* source;

    if (s == NULL || pwcs == NULL) {
        return 0;
    }

    source = pwcs;
    while (chars_written <= n) {
        if (!*source) {
            *(s + chars_written) = '\0';
            break;
        } else {
            result = wctomb(temp, *source++);
            if ((chars_written + result) <= n) {
                strncpy(s + chars_written, temp, result);
                chars_written += result;
            } else {
                break;
            }
        }
    }

    return chars_written;
}
