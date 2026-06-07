#include <locale.h>
#include <stddef.h>

char*  strncpy(char* dest, const char* src, size_t n);
size_t strlen(const char* str);

int mbtowc(wchar_t* pwc, const char* s, int n) {
    return _current_locale.ctype_cmpt_ptr->decode_mb(pwc, s, n);
}

int __mbtowc_noconv(wchar_t* pwc, const char* str, int n) {
    if (str == NULL) {
        return 0;
    }

    if (n == 0) {
        return -1;
    }

    if (pwc != NULL) {
        *pwc = (unsigned char)*str;
    }

    if (*str == 0) {
        return 0;
    }

    return 1;
}

int __wctomb_noconv(char* s, wchar_t wchar) {
    if (s == NULL) {
        return 0;
    }
    *s = wchar;
    return 1;
}

int wctomb(char* s, wchar_t wchar) {
    return _current_locale.ctype_cmpt_ptr->encode_wc(s, wchar);
}

int wcstombs(char* s, const wchar_t* pwcs, unsigned int n) {
    int                   chars_written = 0;
    int                   result;
    char                  temp[3];
    const unsigned short* source;

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
