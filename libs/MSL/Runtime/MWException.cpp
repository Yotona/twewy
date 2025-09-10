#include <MSL/ARM/abort_exit.h>
#include <MSL/C++/exception.hpp>
#include <MSL/Runtime/MWException.h>

namespace std {

    static void dthandler() {
        abort();
    }

    static terminate_handler thandler = dthandler;

    static void duhandler() {
        terminate();
    }

    static unexpected_handler uhandler = duhandler;

    extern void terminate() {
        thandler();
    }

    extern void unexpected() {
        uhandler();
    }

} // namespace std

#define CONST_CHAR 'K'

extern char __throw_catch_compare(const u8* throwtype, const u8* catchtype, s32* offset_result) {
    const u8 *cptr1, *cptr2;

    *offset_result = 0;
    if ((cptr2 = catchtype) == 0) {
        return true;
    }
    cptr1 = throwtype;

    if (*cptr2 == 'P') {
        cptr2++;

        if (*cptr2 == 'V')
            cptr2++;
        if (*cptr2 == CONST_CHAR)
            cptr2++;

        if (*cptr2 == 'v') {
            if (*cptr1 == 'P' || *cptr1 == '*') {
                return true;
            }
        }
        cptr2 = catchtype;
    }

    switch (*cptr1) {
        case '*':
        case '!':
            if (*cptr1++ != *cptr2++)
                return false;
            for (;;) {
                if (*cptr1 == *cptr2++) {
                    if (*cptr1++ == '!') {
                        long offset;

                        for (offset = 0; *cptr1 != '!';)
                            offset = offset * 10 + *cptr1++ - '0';
                        *offset_result = offset;
                        return true;
                    }
                } else {
                    while (*cptr1++ != '!')
                        ;
                    while (*cptr1++ != '!')
                        ;
                    if (*cptr1 == 0)
                        return false;
                    cptr2 = catchtype + 1;
                }
            }
            return false;
    }

    while ((*cptr1 == 'P' || *cptr1 == 'R') && *cptr1 == *cptr2) {
        cptr1++;
        cptr2++;
        if (*cptr2 == CONST_CHAR) {
            if (*cptr1 == CONST_CHAR)
                cptr1++;
            cptr2++;
        }
        if (*cptr1 == CONST_CHAR)
            return false;

        if (*cptr2 == 'V') {
            if (*cptr1 == 'V')
                cptr1++;
            cptr2++;
        }
        if (*cptr1 == 'V')
            return false;
    }

    for (; *cptr1 == *cptr2; cptr1++, cptr2++)
        if (*cptr1 == 0)
            return true;
    return false;
}