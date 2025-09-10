#include <MSL/Runtime/semihost.h>

void _ExitProcess(void) {
    sys_Exit();
}
